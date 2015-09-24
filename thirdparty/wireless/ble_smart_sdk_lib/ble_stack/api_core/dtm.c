/*!
*  @file        dtm.c
*  @brief       this file contains APIs for Direct test mode
*  @details     Detailed file description
*  @todo        Tasks to be done
*  @sa          (See Also) file's refernces, related functions... etc
*  @author      Ahmed Fathy
*  @date        22 June 2015
*  @version     1.0
*/
#include "at_ble_api.h"
#include "interface.h"

#define DTM_RESET_CMD_LENGTH 4
#define DTM_RX_TEST_CMD_LENGTH 5
#define DTM_TX_TEST_CMD_LENGTH 7
#define DTM_STOP_TEST_CMD_LENGTH 4
#define USER_DEFINED 0x00

at_ble_status_t at_ble_dtm_reset(void)
{
    uint8_t status = AT_BLE_SUCCESS;
    /*Preparing the Reset command*/
    interface_send_msg[DTM_RESET_CMD_LENGTH - 4] = 0x01;
    interface_send_msg[DTM_RESET_CMD_LENGTH - 3] = 0x03;
    interface_send_msg[DTM_RESET_CMD_LENGTH - 2] = 0x0C;
    interface_send_msg[DTM_RESET_CMD_LENGTH - 1] = 0x00;
    /*Sending the command to DUT*/
    status = interface_send(interface_send_msg, (uint16_t)DTM_RESET_CMD_LENGTH);
    return (at_ble_status_t)status;
}

at_ble_status_t at_ble_dtm_rx_test_start(uint8_t frequency_index)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    /* Checking parameters*/
    if (frequency_index > 39)
    {
        status = AT_BLE_INVALID_PARAM;
    }
    if (AT_BLE_SUCCESS == status)
    {
        /*Preparing the rx test command*/
        interface_send_msg[DTM_RX_TEST_CMD_LENGTH - 5] = 0x01;
        interface_send_msg[DTM_RX_TEST_CMD_LENGTH - 4] = 0x1D;
        interface_send_msg[DTM_RX_TEST_CMD_LENGTH - 3] = 0x20;
        interface_send_msg[DTM_RX_TEST_CMD_LENGTH - 2] = 0x01;
        interface_send_msg[DTM_RX_TEST_CMD_LENGTH - 1] = frequency_index;
        /*Sending the command to DUT*/
        interface_send(interface_send_msg, (uint16_t)DTM_RX_TEST_CMD_LENGTH);
    }
    return status;
}

at_ble_status_t at_ble_dtm_tx_test_start(uint8_t frequency_index, uint8_t data_length, at_ble_dtm_payload_type_t payload)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    /* Checking parameters*/
    if ((frequency_index > 39) || (data_length > 36) || (payload > 7))
    {
        status = AT_BLE_INVALID_PARAM;
    }
    if (AT_BLE_SUCCESS == status)
    {
        /*Preparing the tx test command*/
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 7] = 0x01;
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 6] = 0x1E;
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 5] = 0x20;
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 4] = 0x03;
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 3] = frequency_index;
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 2] = data_length;
        interface_send_msg[DTM_TX_TEST_CMD_LENGTH - 1] = payload;
        /*Sending the command to DUT*/
        interface_send(interface_send_msg, (uint16_t)DTM_TX_TEST_CMD_LENGTH);
    }
    return status;
}

at_ble_status_t at_ble_dtm_stop_test(void)
{
    uint8_t status = AT_BLE_SUCCESS;
    /*Preparing the stop test command*/
    interface_send_msg[DTM_STOP_TEST_CMD_LENGTH - 4] = 0x01;
    interface_send_msg[DTM_STOP_TEST_CMD_LENGTH - 3] = 0x1F;
    interface_send_msg[DTM_STOP_TEST_CMD_LENGTH - 2] = 0x20;
    interface_send_msg[DTM_STOP_TEST_CMD_LENGTH - 1] = 0x00;
    /*Sending the command to DUT*/
    status = interface_send(interface_send_msg, (uint16_t)DTM_STOP_TEST_CMD_LENGTH);
    return (at_ble_status_t)status;
}







