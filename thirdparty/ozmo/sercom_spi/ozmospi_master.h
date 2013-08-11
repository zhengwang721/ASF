#ifndef OZMOSPI_MASTER_H
#define OZMOSPI_MASTER_H

#include <conf_ozmospi_master.h>
#include <port.h>
#include <status_codes.h>


typedef uint16_t ozmospi_buflen_t;

struct ozmospi_bufdesc {
	uint8_t *data;
	ozmospi_buflen_t length;
};


enum status_code ozmospi_init(void);
void ozmospi_enable(void);
void ozmospi_disable(void);

static inline void ozmospi_select_slave(bool select)
{
	if (select) {
		port_pin_set_output_level(CONF_OZMOSPI_SS_PIN, false);
		} else {
		port_pin_set_output_level(CONF_OZMOSPI_SS_PIN, true);
	}
}

enum status_code ozmospi_transceive_buffers_wait(
		struct ozmospi_bufdesc tx_bufdescs[],
		struct ozmospi_bufdesc rx_bufdescs[]);

#endif /* OZMOSPI_MASTER_H */