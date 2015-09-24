
#include "at_ble_api.h"

#include "platform.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "dbg_task.h"
#include "dbg_logs.h"
#include "device.h"

at_ble_status_t at_ble_authenticate(at_ble_handle_t conn_handle,
                                    at_ble_pair_features_t *features,
                                    at_ble_LTK_t *ltk,
                                    at_ble_CSRK_t *csrk)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint8_t conenctedDevRole = AT_BLE_ROLE_ALL;
    uint8_t devIndex = AT_BLE_MAX_CONCURRENT_CONNS;
    uint8_t auth_req;
    uint8_t bParingResp_accept = 1;
    FN_IN();
    do
    {
        devIndex = check_ConnData_idx_role(conn_handle, &conenctedDevRole);
        if (AT_BLE_MAX_CONCURRENT_CONNS <= devIndex)
        {
            PRINT_ERR("INVALID CONN. HANDLE\n");
            status = AT_BLE_ATT_INVALID_HANDLE;
            break;
        }
        PRINT_DBG(">>CONN. DEV. Role %s\n", (gstrConnData[devIndex].role == ROLE_MASTER ? "MASTER" : "SLAVE"));
        /*Check Features pointer*/
        if (NULL == features)
        {
            if (gstrConnData[devIndex].role == ROLE_SLAVE)
            {
                PRINT_DBG("Pairing refused\n");
                gapc_bond_cfm_handler_pair_resp(0, 0, 0, 0, 0, 0, 0, 0, conn_handle);
            }
            else // if (gstrConnData[devIndex].role == ROLE_MASTER)
            {
                PRINT_ERR("INVALID FEATURES\n");
                status = AT_BLE_INVALID_PARAM;
            }
            break;
        }
        /*Check Key Size*/
        if ((features->min_key_size < 7) || (features->max_key_size > 16) ||
                (features->min_key_size > features->max_key_size))
        {
            PRINT_ERR("INVALID KEY SIZE\r\n");
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        /*Check desired authen*/
        if (features->desired_auth > AT_BLE_MODE2_L2_AUTH_DATA_SGN)
        {
            PRINT_ERR("UNKNOWN AUTHEN. TYPE\r\n");
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        /*Check iKey and rKey Dist.*/
        if ((features->initiator_keys > AT_BLE_KEY_DIS_ALL) ||
                (features->responder_keys > AT_BLE_KEY_DIS_ALL))
        {
            PRINT_ERR("UNKNOWN KEY DIST.\r\n");
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        if (features->bond)
        {
            if (ltk == NULL)
            {
                PRINT_ERR("LTK is not provided\r\n");
                status = AT_BLE_INVALID_PARAM;
                break;
            }
            else
            {
                memcpy(&device.ltk, ltk , sizeof(at_ble_LTK_t));
            }
        }
        if (csrk != NULL)
        {
            memcpy(&device.csrk , csrk , sizeof(at_ble_CSRK_t));
        }
        features->mitm_protection = (features->mitm_protection ? 1 : 0);
        features->bond = (features->bond ? 1 : 0);
        auth_req = (features->mitm_protection << 2) | (features->bond & 0x01);
        PRINT_DBG("AUTH. REQ 0x%0X\r\n", auth_req);
        if (gstrConnData[devIndex].role == ROLE_MASTER)
        {
            /*Check Keys*/
            if (((features->initiator_keys & AT_BLE_KEY_DIST_SIGN) && (csrk == NULL)) ||
                    ((features->initiator_keys & AT_BLE_KEY_DIST_ENC) && (ltk == NULL)))
            {
                PRINT_ERR("INVALID KEY ENTRY\r\n");
                status = AT_BLE_INVALID_PARAM;
                break;
            }
            gapc_bond_cmd_handler(conn_handle, features->io_cababilities, features->oob_avaiable,
                                  auth_req, features->max_key_size, features->initiator_keys, features->responder_keys,
                                  features->desired_auth);
        }
        else if (gstrConnData[devIndex].role == ROLE_SLAVE)
        {
            /*Check Keys*/
            if (((features->responder_keys & AT_BLE_KEY_DIST_SIGN) && (csrk == NULL)) ||
                    ((features->responder_keys & AT_BLE_KEY_DIST_ENC) && (ltk == NULL)))
            {
                PRINT_ERR("INVALID KEY ENTRY\r\n");
                status = AT_BLE_INVALID_PARAM;
                break;
            }
            gapc_bond_cfm_handler_pair_resp(bParingResp_accept, features->io_cababilities,
                                            features->oob_avaiable, auth_req, features->max_key_size,
                                            features->initiator_keys, features->responder_keys,
                                            features->desired_auth, conn_handle);
        }
        else
        {
            PRINT_ERR("INVALID DEV MODE\r\n");
            status = AT_BLE_INVALID_PARAM;
        }
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_pair_key_reply(at_ble_handle_t conn_handle, at_ble_pair_key_type_t type, uint8_t *key)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint8_t tk[KEY_LEN] = {0};
    int8_t i;
    uint32_t pin_code = 0;
    uint8_t bParingAccept_KeyReply = 1;
    FN_IN();
    do
    {
        if (NULL == key)
        {
            PRINT_INFO("INVALID KEY ENTRY\r\n");
            bParingAccept_KeyReply = 0;
            key = &(tk[0]);
        }
        // To do add out of band pairing
        if (type == AT_BLE_PAIR_PASSKEY)
        {
            uint32_t power_base = 1;
            for (i = AT_BLE_PASSKEY_LEN - 1; i >= 0; i--)
            {
                // issue - if key passed as NULL key will point to tk, initialized to zeros
                // issue - if size of key buffer != AT_BLE_PASSKEY_LEN the pin_code will be invalid value
                pin_code += (key[i] - '0') * power_base;
                power_base *= 10;
            }
            tk[0] = (uint8_t)((pin_code & 0x000000FF) >>  0);
            tk[1] = (uint8_t)((pin_code & 0x0000FF00) >>  8);
            tk[2] = (uint8_t)((pin_code & 0x00FF0000) >> 16);
            tk[3] = (uint8_t)((pin_code & 0xFF000000) >> 24);
        }
        else if (type == AT_BLE_PAIR_OOB)
        {
            for (i = 0; i < KEY_LEN; i++)
            {
                // issue - if size of key buffer < KEY_LEN
                tk[i] = key[i];
            }
        }
        else
        {
            PRINT_ERR("INVALID TYPE\r\n");
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        // in case of AT_BLE_PAIR_PASSKEY tk length is 4 bytes, while in case of AT_BLE_PAIR_OOB tk length is 16 bytes
        gapc_bond_cfm_handler_key_exch(GAPC_TK_EXCH, bParingAccept_KeyReply, tk, conn_handle);
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_send_slave_sec_request(at_ble_handle_t conn_handle, bool mitm_protection, bool bond)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint8_t devIndex = AT_BLE_MAX_CONCURRENT_CONNS;
    uint8_t auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
    FN_IN();
    do
    {
        devIndex = check_ConnData_idx_role(conn_handle, NULL);
        if (AT_BLE_MAX_CONCURRENT_CONNS <= devIndex)
        {
            PRINT_ERR("Unknow Handle\r\n");
            status = AT_BLE_ATT_INVALID_HANDLE;
            break;
        }
        if (gstrConnData[devIndex].role != ROLE_SLAVE)
        {
            PRINT_ERR("Invalid Role\r\n");
            status = AT_BLE_INVALID_STATE;
            break;
        }
        PRINT_DBG("AUTH. REQ 0x%0X\r\n", auth);
        mitm_protection = (mitm_protection ? 1 : 0);
        bond = (bond ? 1 : 0);
        auth = (mitm_protection << 2) | (bond & 0x01);
        gapc_security_cmd_handler(conn_handle, auth);
    }
    while (0);
    FN_OUT(status);
    return status;
}

static at_ble_status_t check_enc_process(at_ble_handle_t conn_handle, at_ble_LTK_t *key, at_ble_auth_t auth, uint8_t *index)
{
    at_ble_status_t status;
    do
    {
        // caller functions checked key pointer against NULL
        switch (auth)
        {
        case AT_BLE_AUTH_NO_MITM_NO_BOND:
        case AT_BLE_AUTH_NO_MITM_BOND:
        case AT_BLE_AUTH_MITM_NO_BOND:
        case AT_BLE_AUTH_MITM_BOND:
            status = AT_BLE_SUCCESS;
            break;
        default:
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        if (AT_BLE_SUCCESS != status)
        {
            PRINT_ERR("Invalid Auth Type %d\r\n", auth);
            break;
        }
        if ((key->key_size > 16) || (key->key_size < 7))
        {
            PRINT_ERR("Invalid Key size %d\r\n", key->key_size);
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        if (NULL != index)
        {
            *index = check_ConnData_idx_role(conn_handle, NULL);
            if (AT_BLE_MAX_CONCURRENT_CONNS <= (*index))
            {
                PRINT_ERR("Unknow handle %x", conn_handle);
                status = AT_BLE_ATT_INVALID_HANDLE;
                break;
            }
        }
    }
    while (0);
    return status;
}

at_ble_status_t at_ble_encryption_start(at_ble_handle_t conn_handle,
                                        at_ble_LTK_t *key,
                                        at_ble_auth_t auth)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    uint8_t index = AT_BLE_MAX_CONCURRENT_CONNS;
    FN_IN();
    do
    {
        if (NULL == key)
        {
            break;
        }
        if (AT_BLE_SUCCESS == (status = check_enc_process(conn_handle, key, auth, &index)))
        {
            if (ROLE_MASTER != gstrConnData[index].role)
            {
                PRINT_ERR("Invalid Role %x", gstrConnData[index].role);
                status = AT_BLE_INVALID_STATE;
                break;
            }
            memcpy((uint8_t *)&device.ltk, key , sizeof(at_ble_LTK_t));
            gapc_encrypt_cmd_handler(conn_handle, key->key, key->ediv, key->nb, key->key_size, auth);
        }
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_encryption_request_reply(at_ble_handle_t conn_handle,
        at_ble_auth_t auth,
        bool key_found,
        at_ble_LTK_t *key)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        if (NULL == key)
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        if (false == key_found || AT_BLE_SUCCESS == (status = check_enc_process(conn_handle, key, auth, NULL)))
        {
            gapc_encrypt_cfm_handler(conn_handle, auth, key_found, key->key, key->key_size);
        }
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
