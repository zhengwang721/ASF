
#include "at_ble_api.h"

#include "platform.h"
#include "gattc_task.h"
#include "device.h"
#include "dbg_logs.h"

#define AUTO_EXCUTE_ENABLE  (1)
#define AUTO_EXCUTE_DISABLE (0)
#define EXCUTE_ALL          (1)
#define CANCEL_ALL          (0)

static uint16_t u16GattcSeqNo = 0;

at_ble_status_t at_ble_primary_service_discover_all(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* check the handles */
        if ((start_handle < 1) || (end_handle < start_handle))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* call discover command pack and send handler*/
        gattc_disc_cmd_handler(GATTC_DISC_ALL_SVC, 0, start_handle, end_handle, NULL, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("primary_service_discover_all sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_primary_service_discover_by_uuid(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t *uuid)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    uint8_t uuid_len = 0;
    uint8_t i;
    FN_IN();
    do
    {
        /* Null Pointer checks*/
        if (!uuid)
        {
            PRINT_ERR("NULL pointer error");
            break;
        }
        /* check the handles */
        if ((start_handle < 1) || (end_handle < start_handle))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* check UUID Length */
        uuid_len = at_ble_uuid_type2len(uuid->type);
        if (AT_BLE_INVALID_PARAM == uuid_len)
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* check if UUID value is zero */
        for (i = 0; i < uuid_len; i++)
        {
            if (0 != uuid->uuid[i])
            {
                PRINT_ERR("Invalid Parameters error");
                break;
            }
        }
        if ((i == uuid_len))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* call discover command pack and send handler*/
        gattc_disc_cmd_handler((uint8_t)GATTC_DISC_BY_UUID_SVC, uuid_len, start_handle, end_handle, uuid->uuid, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("primary_service_discover_by_uuid sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_included_service_discover_all(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* check the handles */
        if ((start_handle < 1) || (end_handle < start_handle))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /*TODO Add check for connection handle*/
        /*  Pack and send the message */
        gattc_disc_cmd_handler(GATTC_DISC_INCLUDED_SVC, 0, start_handle, end_handle, NULL, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("included_service_discover_all sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_discover_all(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* check the handles */
        if ((start_handle < 1) || (end_handle < start_handle))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /*  Pack and send the message */
        gattc_disc_cmd_handler(GATTC_DISC_ALL_CHAR, 0, start_handle, end_handle, NULL, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("characteristic_discover_all sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_discover_by_uuid(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t *uuid)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    uint8_t uuid_len = 0;
    uint8_t i;
    FN_IN();
    do
    {
        /* Null Pointer checks*/
        if (!uuid)
        {
            PRINT_ERR("NULL pointer error");
            break;
        }
        /* check the handles */
        if ((start_handle < 1) || (end_handle < start_handle))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* check UUID Length */
        uuid_len = at_ble_uuid_type2len(uuid->type);
        if (AT_BLE_INVALID_PARAM == uuid_len)
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* check if UUID value is zero */
        for (i = 0; i < uuid_len; i++)
        {
            if (0 != uuid->uuid[i])
            {
                PRINT_INFO("UUID is valid");
                break;
            }
        }
        if ((i == uuid_len))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        /* call discover command pack and send handler*/
        gattc_disc_cmd_handler((uint8_t)GATTC_DISC_BY_UUID_CHAR, uuid_len, start_handle, end_handle, uuid->uuid, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("characteristic_discover_by_uuid sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_descriptor_discover_all(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* check the handles */
        if ((start_handle < 1) || (end_handle < start_handle))
        {
            PRINT_ERR("Invalid Parameters");
            break;
        }
        /*  Pack and send the message */
        gattc_disc_cmd_handler(GATTC_DISC_DESC_CHAR, 0, start_handle, end_handle, NULL, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("descriptor_discover_all sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_write(at_ble_handle_t conn_handle, at_ble_handle_t char_handle,
        uint16_t offset, uint16_t length, uint8_t *data,
        bool signed_write, bool with_response)
{
    uint8_t req = GATTC_WRITE_NO_RESPONSE;
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* Null Pointer checks*/
        if (!data)
        {
            PRINT_ERR("NULL pointer\r\n");
            break;
        }
        /* Param checking */
        if (0 == length)
        {
            PRINT_ERR("Invalid Parameters\r\n");
            break;
        }
        if (with_response && signed_write)
        {
            PRINT_ERR("Signed can't be with response\r\n");
            break;
        }
        else if (signed_write)
        {
            req = GATTC_WRITE_SIGNED;
        }
        else if (with_response)
        {
            req = GATTC_WRITE;
        }
        gattc_write_cmd_handler(req, AUTO_EXCUTE_ENABLE, char_handle, offset, length, data, conn_handle);
        status = AT_BLE_SUCCESS;
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_reliable_write_prepare(at_ble_handle_t conn_handle,
        at_ble_handle_t char_handle,
        uint16_t offset, uint16_t length, uint8_t *data)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* Null Pointer checks*/
        if (!data)
        {
            PRINT_ERR("NULL pointer error");
            break;
        }
        gattc_write_cmd_handler(GATTC_WRITE, AUTO_EXCUTE_DISABLE, char_handle, offset, length, data, conn_handle);
        status = AT_BLE_SUCCESS;
        PRINT_INFO("characteristic_write sent");
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_reliable_write_execute(at_ble_handle_t conn_handle)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    gattc_write_execute_cmd_handler(EXCUTE_ALL, conn_handle);
    PRINT_INFO("characteristic_reliable_write_execute sent");
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_reliable_write_cancel(at_ble_handle_t conn_handle)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    gattc_write_execute_cmd_handler(CANCEL_ALL, conn_handle);
    PRINT_INFO("characteristic_reliable_write_cancel sent");
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_exchange_mtu(at_ble_handle_t conn_handle)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    gattc_mtu_cmd_handler(conn_handle);
    PRINT_INFO("exchange_mtu command sent");
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_read_by_uuid(at_ble_handle_t conn_handle,
        at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t *uuid)
{
    at_ble_status_t  status = AT_BLE_SUCCESS;
    FN_IN();
    if (end_handle < start_handle)
    {
        PRINT_ERR("Invaild end or start handles \n");
        status = AT_BLE_INVALID_PARAM;
        goto EXIT;
    }
    if (!uuid)
    {
        PRINT_ERR("Null UUID ptr \n");
        status = AT_BLE_INVALID_PARAM;
        goto EXIT;
    }
    gattc_read_cmd_handler(GATTC_READ_BY_UUID, u16GattcSeqNo, start_handle, end_handle,
                           at_ble_uuid_type2len(uuid->type), uuid->uuid, conn_handle);
    u16GattcSeqNo++;
EXIT:
    FN_OUT(status);
    return status;
}



at_ble_status_t at_ble_characteristic_read(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, uint16_t offset, uint16_t length)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    void *uuid = 0; //dummy parameter
    FN_IN();
    gattc_read_cmd_handler(GATTC_READ, u16GattcSeqNo, char_handle, offset, length, uuid, conn_handle);
    u16GattcSeqNo++;
    FN_OUT(status);
    return status;
}


at_ble_status_t at_ble_characteristic_read_long(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, uint16_t offset, uint16_t length)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    void *uuid = 0; //dummy parameter
    FN_IN();
    gattc_read_cmd_handler(GATTC_READ_LONG, u16GattcSeqNo, char_handle, offset, length, uuid, conn_handle);
    u16GattcSeqNo++;
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_characteristic_read_multiple(at_ble_handle_t conn_handle,  at_ble_char_list *char_handle_list, uint8_t char_handle_count)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    if (char_handle_count < 1)
    {
        status = AT_BLE_INVALID_PARAM;
        PRINT_ERR("Invaild char_handle_count \n");
        goto EXIT;
    }
    if (!char_handle_list)
    {
        status = AT_BLE_INVALID_PARAM;
        PRINT_ERR("Null char_handle_list ptr \n");
        goto EXIT;
    }
    gattc_read_multiple_handler(char_handle_list, char_handle_count, u16GattcSeqNo, conn_handle);
    u16GattcSeqNo++;
EXIT:
    FN_OUT(status);
    return status;
}
