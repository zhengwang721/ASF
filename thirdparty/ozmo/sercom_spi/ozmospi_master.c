#include "ozmospi_master.h"
#include <sercom_interrupt.h>


#define OZMOSPI_SERCOM_SPI     (SercomSpi *)(CONF_OZMOSPI_SERCOM)
#define OZMOSPI_TRANSFER_MODE  (SERCOM_SPI_CTRLA_CPOL | SERCOM_SPI_CTRLA_CPHA)


enum _ozmospi_direction {
	OZMOSPI_DIRECTION_READ,
	OZMOSPI_DIRECTION_WRITE,
	OZMOSPI_DIRECTION_BOTH,
	OZMOSPI_DIRECTION_IDLE,
};

struct ozmospi_module {
	enum _ozmospi_direction direction;
	enum status_code status;
	ozmospi_buflen_t rx_head;
	ozmospi_buflen_t rx_length;
	ozmospi_buflen_t tx_head;
	ozmospi_buflen_t tx_length;
	struct ozmospi_bufdesc *rx_bufdesc_ptr;
	struct ozmospi_bufdesc *tx_bufdesc_ptr;
};


volatile struct ozmospi_module _ozmospi_module;


static void _ozmospi_int_handler(uint8_t not_used);


enum status_code ozmospi_init(void)
{
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;
	struct port_config portpin_conf;
	struct system_gclk_chan_config gclk_chan_conf;
	struct system_pinmux_config pin_conf;
	uint32_t sercom_index = _sercom_get_sercom_inst_index((Sercom *)spi_hw);
	uint32_t pm_index = sercom_index + PM_APBCMASK_SERCOM0_Pos;
	uint32_t gclk_index = sercom_index + SERCOM0_GCLK_ID_CORE;

	/* Enable clock for the module interface */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);

	/* Set up the GCLK for the module */
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = CONF_OZMOSPI_GCLK_SOURCE;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	sercom_set_gclk_generator(CONF_OZMOSPI_GCLK_SOURCE, false);

	/* Set up the SERCOM SPI module as master */
	spi_hw->CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER;
	spi_hw->CTRLA.reg |= OZMOSPI_TRANSFER_MODE | CONF_OZMOSPI_SIGNAL_MUX;

	/* Register our interrupt handler */
	_sercom_set_handler(sercom_index, _ozmospi_int_handler);

	/* Configure the slave select pin */
	port_get_config_defaults(&portpin_conf);
	portpin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(CONF_OZMOSPI_SS_PIN, &portpin_conf);
	port_pin_set_output_level(CONF_OZMOSPI_SS_PIN, true);

	/* Configure the pin MUXes */
	system_pinmux_get_config_defaults(&pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD0 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD0 >> 16, &pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD1 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD1 >> 16, &pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD2 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD2 >> 16, &pin_conf);

	pin_conf.mux_position = CONF_OZMOSPI_PINMUX_PAD3 & 0xFFFF;
	system_pinmux_pin_set_config(CONF_OZMOSPI_PINMUX_PAD3 >> 16, &pin_conf);

	/* Initialize our struct */
	_ozmospi_module.status = STATUS_OK;

	/* Wait for the SERCOM to finish synchronizing */
	return STATUS_OK;
}

void ozmospi_enable(void)
{
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;

	spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE | SERCOM_SPI_INTFLAG_RXC
			| SERCOM_SPI_INTFLAG_TXC;

	while (spi_hw->STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY) {
		/* Intentionally left empty */
	}

	spi_hw->CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;

	system_interrupt_enable(_sercom_get_interrupt_vector((Sercom *)OZMOSPI_SERCOM_SPI));
}

void ozmospi_disable(void)
{
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;

	system_interrupt_disable(_sercom_get_interrupt_vector((Sercom *)OZMOSPI_SERCOM_SPI));

	while (spi_hw->STATUS.reg & SERCOM_SPI_STATUS_SYNCBUSY) {
		/* Intentionally left empty */
	}

	spi_hw->CTRLB.reg = 0;
	spi_hw->CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
}

enum status_code ozmospi_transceive_buffers_wait(
	struct ozmospi_bufdesc tx_bufdescs[],
	struct ozmospi_bufdesc rx_bufdescs[])
{
	enum status_code status;
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;
	uint8_t tmp_intenset;

	Assert(tx_bufdescs || rx_bufdescs);

	if (_ozmospi_module.status == STATUS_BUSY) {
		return STATUS_BUSY;
	} else {
		_ozmospi_module.status = STATUS_BUSY;
	}

	_ozmospi_module.tx_bufdesc_ptr = tx_bufdescs;
	_ozmospi_module.rx_bufdesc_ptr = rx_bufdescs;
	_ozmospi_module.tx_head = 0;
	_ozmospi_module.rx_head = 0;

	if (tx_bufdescs && rx_bufdescs) {
		_ozmospi_module.direction = OZMOSPI_DIRECTION_BOTH;
		_ozmospi_module.tx_length = tx_bufdescs[0].length;
		_ozmospi_module.rx_length = rx_bufdescs[0].length;
		spi_hw->CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;

		tmp_intenset = SERCOM_SPI_INTFLAG_DRE | SERCOM_SPI_INTFLAG_RXC;
	} else {
		if (tx_bufdescs) {
			_ozmospi_module.direction = OZMOSPI_DIRECTION_WRITE;
			_ozmospi_module.tx_length = tx_bufdescs[0].length;

			tmp_intenset = SERCOM_SPI_INTFLAG_DRE;
		} else {
			_ozmospi_module.direction = OZMOSPI_DIRECTION_READ;
			_ozmospi_module.rx_length = rx_bufdescs[0].length;
			spi_hw->CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;

			tmp_intenset = SERCOM_SPI_INTFLAG_DRE | SERCOM_SPI_INTFLAG_RXC;
		}
	}

	spi_hw->INTENSET.reg = tmp_intenset;

	do {
		status = _ozmospi_module.status;
	} while (status == STATUS_BUSY);

	return status;
}

static void _ozmospi_int_handler(uint8_t not_used)
{
	enum _ozmospi_direction dir = _ozmospi_module.direction;
	SercomSpi *const spi_hw = OZMOSPI_SERCOM_SPI;
	uint8_t int_status;

	uint8_t *tx_data;
	uint8_t *rx_data;
	ozmospi_buflen_t tx_head;
	ozmospi_buflen_t tx_length;
	ozmospi_buflen_t rx_head;
	ozmospi_buflen_t rx_length;

	int_status = spi_hw->INTFLAG.reg & spi_hw->INTENSET.reg;

	if (int_status & SERCOM_SPI_INTFLAG_DRE) {
		if (dir == OZMOSPI_DIRECTION_READ) {
			spi_hw->DATA.reg = 0;

check_for_read_end:
			tx_length = _ozmospi_module.rx_length - 1;

			if (!tx_length) {
				tx_length = (_ozmospi_module.rx_bufdesc_ptr + 1)->length;

				if (!tx_length) {
					// disable DRE -- end of read transaction
					spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
				}
			}

			// OZMOSPI_DIRECTION_WRITE || OZMOSPI_DIRECTION_BOTH
		} else {
			tx_data = _ozmospi_module.tx_bufdesc_ptr->data;
			tx_head = _ozmospi_module.tx_head;
			spi_hw->DATA.reg = tx_data[tx_head];

			tx_length = _ozmospi_module.tx_length - 1;

			if (tx_length) {
				_ozmospi_module.tx_head = tx_head + 1;
				_ozmospi_module.tx_length = tx_length;
			} else {
				tx_length = (++_ozmospi_module.tx_bufdesc_ptr)->length;

				if (tx_length) {
					_ozmospi_module.tx_head = 0;
					_ozmospi_module.tx_length = tx_length;
				} else {
					if (dir == OZMOSPI_DIRECTION_WRITE) {
						// disable DRE and enable TXC -- end of write transaction
						spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
						spi_hw->INTENSET.reg = SERCOM_SPI_INTFLAG_TXC;
					} else {
						// bidirectional transaction has become a read
						dir = OZMOSPI_DIRECTION_READ;
						_ozmospi_module.direction = dir;
						goto check_for_read_end;
					}
				}
			}
		}
	}

	if (int_status & SERCOM_SPI_INTFLAG_RXC) {
		rx_data = _ozmospi_module.rx_bufdesc_ptr->data;
		rx_head = _ozmospi_module.rx_head;
		rx_data[rx_head] = spi_hw->DATA.reg;

		rx_length = _ozmospi_module.rx_length - 1;

		if (rx_length) {
			_ozmospi_module.rx_head = rx_head + 1;
			_ozmospi_module.rx_length = rx_length;
		} else {
			rx_length = (++_ozmospi_module.rx_bufdesc_ptr)->length;

			if (rx_length) {
				_ozmospi_module.rx_head = 0;
				_ozmospi_module.rx_length = rx_length;
			} else {
				spi_hw->CTRLB.reg = 0;

				if (dir == OZMOSPI_DIRECTION_READ) {
					// disable RXC, update direction and status
					spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_RXC;

					dir = OZMOSPI_DIRECTION_IDLE;
					_ozmospi_module.direction = dir;
					_ozmospi_module.status = STATUS_OK;
				} else {
					dir = OZMOSPI_DIRECTION_WRITE;
					_ozmospi_module.direction = dir;
				}
			}
		}
	}

	if (int_status & SERCOM_SPI_INTFLAG_TXC) {
		// disable TXC, update direction and status
		spi_hw->INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;

		dir = OZMOSPI_DIRECTION_IDLE;
		_ozmospi_module.direction = dir;
		_ozmospi_module.status = STATUS_OK;
	}
}