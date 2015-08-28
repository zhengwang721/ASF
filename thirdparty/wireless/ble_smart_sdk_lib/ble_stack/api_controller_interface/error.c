
#include "at_ble_api.h"
#include "gattc_task.h"
#include "gapm_task.h"
#include "error.h"

at_ble_status_t at_ble_att_error(uint8_t att_error_code)
{
	at_ble_status_t status;
	switch(att_error_code)
	{
	case  ATT_ERR_NO_ERROR:
		status = AT_BLE_SUCCESS;
		break;
	case ATT_ERR_INVALID_HANDLE:
		status = AT_BLE_INVALID_HANDLE;
		break;
	case ATT_ERR_INSUFF_RESOURCE:
		status = AT_BLE_INSUFF_RESOURCE;
		break;
	case ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN:
		status = AT_BLE_INVALID_ATTRIBUTE_VAL_LEN;
		break;
	default:
		status = AT_BLE_FAILURE;
		break;
	}
	return status;
}


at_ble_status_t at_ble_gap_error(uint8_t gap_error_code)
{
	at_ble_status_t status;
	switch(gap_error_code)
	{
	case   GAP_ERR_INVALID_PARAM:
		status = AT_BLE_INVALID_PARAM;
		break;
	case  GAP_ERR_COMMAND_DISALLOWED:
		status = AT_BLE_COMMAND_DISALLOWED;
		break;
	case GAP_ERR_NO_ERROR:
		status = AT_BLE_SUCCESS;
		break;
	default:
		status = AT_BLE_FAILURE;
		break;
	}
	return status;
}
