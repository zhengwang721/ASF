#include <stdint.h>
#include "sercom.h"

/**
 * \brief Calculate synchronous baudrate value (SPI/UART)
 */
enum status_code sercom_get_sync_baud_val(uint32_t baudrate,
		uint32_t external_clock, uint16_t *baudval)
{
	uint16_t ret = 0;

	if (baudrate > (external_clock / 2)) {
		// assert error, outside valid range
	}

	//ret = (external_clock / (2 * baudrate)) - 1;

	if (ret > 0xFF) {
		// assert error; max BAUD value for sync is 255 (8-bit)
	} else {
		return ret;
	}
}

/**
 * \brief Calculate asynchronous baudrate value (UART)
*/
enum status_code sercom_get_async_baud_val(uint32_t baudrate,
		uint32_t peripheral_clock, uint16_t *baudval)
{

	if ((baudrate * 16) >= peripheral_clock) {
		// Unattainable baud rate
		return 4;
	}

	uint64_t ratio = 0;
	uint64_t scale = 0;
	uint64_t retval = 0;

	ratio = ((16 * (uint64_t)baudrate) << SHIFT) / peripheral_clock;
	scale = (1ull << SHIFT) - ratio;
	retval = (65536 * scale) >> SHIFT;

	*baudval = retval;

	return STATUS_OK;
}

