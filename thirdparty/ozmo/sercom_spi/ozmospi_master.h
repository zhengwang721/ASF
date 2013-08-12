#ifndef OZMOSPI_MASTER_H
#define OZMOSPI_MASTER_H

#include <conf_ozmospi_master.h>
#include <port.h>
#include <status_codes.h>

/**
 * \defgroup ozmo_sercom_spi_master_group SERCOM SPI master driver for OZMO stack
 *
 * This driver is a single-instance, compile-time configured SPI master driver.
 * It supports both simplex and duplex transfers with scatter/gather.
 *
 * Scatter/gather is implemented by the use of buffer descriptor arrays, which
 * must be passed to the driver to start a transfer.
 * See \ref ozmospi_transceive_buffers_wait() for more information.
 *
 * @{
 */

/**
 * \name Configuration macros
 * @{
 */
/**
 * \def CONF_OZMOSPI_SERCOM
 * \brief SERCOM module to use for this SPI driver.
 */
/**
 * \def CONF_OZMOSPI_BAUDRATE
 * \brief Baudrate to configure the SERCOM SPI module for.
 */
/**
 * \def CONF_OZMOSPI_SS_PIN
 * \brief Number of IO pin to use as Slave Select line.
 */
/**
 * \def CONF_OZMOSPI_GCLK_SOURCE
 * \brief Generic clock generator to use for the SERCOM module.
 */
/**
 * \def CONF_OZMOSPI_SIGNAL_MUX
 * \brief Signal multiplexing setting, i.e., pad multiplexing.
 */
/**
 * \def CONF_OZMOSPI_PINMUX_PAD0
 * \brief First IO pin function multiplexing setting.
 */
/**
 * \def CONF_OZMOSPI_PINMUX_PAD1
 * \brief Second IO pin function multiplexing setting.
 */
/**
 * \def CONF_OZMOSPI_PINMUX_PAD2
 * \brief Third IO pin function multiplexing setting.
 */
/**
 * \def CONF_OZMOSPI_PINMUX_PAD3
 * \brief Fourth IO pin function multiplexing setting.
 */
/** @} */

/** Type to contain length of described buffers */
typedef uint16_t ozmospi_buflen_t;

/** Struct to describe a buffer for writing or reading */
struct ozmospi_bufdesc {
	uint8_t *data;
	ozmospi_buflen_t length;
};

/**
 * \brief Select or deselect slave
 *
 * Drive the configured Slave Select line low or high, depending on whether the
 * slave is being selected or deselected.
 *
 * \param[in] select
 * \arg \c true to select the slave.
 * \arg \c false to deselect the slave.
 */
static inline void ozmospi_select_slave(bool select)
{
	if (select) {
		port_pin_set_output_level(CONF_OZMOSPI_SS_PIN, false);
		} else {
		port_pin_set_output_level(CONF_OZMOSPI_SS_PIN, true);
	}
}

enum status_code ozmospi_init(void);
void ozmospi_enable(void);
void ozmospi_disable(void);
enum status_code ozmospi_transceive_buffers_wait(
		struct ozmospi_bufdesc tx_bufdescs[],
		struct ozmospi_bufdesc rx_bufdescs[]);

/**
 * @}
 */

#endif /* OZMOSPI_MASTER_H */