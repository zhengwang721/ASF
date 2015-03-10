
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "at_ble_api.h"

/**
@defgroup platform Platform API

@{
*/

 /** @brief implemets platform-specefic initialization
  *
  * @param[in] platform_params platform specefic params, this pointer is passed from the 
  * at_ble_init function and interpreted by the platform 
  *
  * @return AT_BLE_SUCCESS operation completed successfully
  * @return AT_BLE_FAILURE Generic error.
  */
at_ble_status_t platform_init(void* platform_params);

 /** @brief sends a message over the platform-specific bus
  *
  * Sends a message over the platform-specific bus that might be UART, SPI or other
  * if the BTLC1000 external wakeup signal is used, it is up to this function implementation
  * to assert/deassert it appropiately
  *
  * @param[in] data data to send over the interface
  * @param[in] len length of data
  *
  */
void platform_interface_send(uint8_t* data, uint32_t len);

 /** @brief the callback to upper layers to process recieved packets
  *
  * This function is implemented by the upper layers (the event loop) and it is up
  * to the platofrm implementaion to call it whenever data are recieved from the interface
  *
  * @param[in] data data received from the interface
  * @param[in] len length of data
  *
  */
void platform_interface_callback(uint8_t* data, uint32_t len);

 /** @brief fires the comand-complete signal
  *  @note more details at the platform porting guide
  *
  */
void platform_cmd_cmpl_signal(void);

 /** @brief blocks until the command-complete signal is fired
  *  @note more details at the platform porting guide
  *
  */
void platform_cmd_cmpl_wait(void);

 /** @brief fires the event signal
  *  @note more details at the platform porting guide
  *
  */
void platform_event_signal(void);

 /** @brief blocks until the event signal is fired
  *  @note more details at the platform porting guide
  *
  */
void platform_event_wait(void);

 /** @}*/

#endif // __PLATFORM_H__
