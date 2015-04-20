
#include "at_ble_api.h"

#include "platform.h"
#include "gattc_task.h"
#include "device.h"


static at_ble_status_t gap_check_device_role(void)
{
#if 0
	if(device.role == ROLE_MASTER)
	{
		return AT_BLE_SUCCESS;
	}
	else
	{
		return AT_BLE_INVALID_STATE;
	}
#endif
	return device.role;
}

at_ble_status_t at_ble_primary_service_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	// check if end handle is not smaller than start handle
	if((end_handle>= start_handle) && start_handle > 0)
	{
		gattc_disc_cmd_handler(GATTC_DISC_ALL_SVC, 
			0,
			start_handle, end_handle, NULL, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}
	
	return status;
}

at_ble_status_t at_ble_primary_service_discover_by_uuid(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t* uuid)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	uint32_t uuid_len = 0, valid_uuid=0;

	// check UUID is non zero
	uuid_len = at_ble_uuid_type2len(uuid->type);

	for(;uuid_len>0;uuid_len--)
	{
		valid_uuid += uuid->uuid[uuid_len - 1];
	}


	// check if end handle is not smaller than start handle
	if((end_handle>= start_handle) && (start_handle > 0) && (valid_uuid))
	{
		gattc_disc_cmd_handler(GATTC_DISC_BY_UUID_SVC, 
			at_ble_uuid_type2len(uuid->type),
			start_handle, end_handle, uuid->uuid, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}
	
	return status;

}

at_ble_status_t at_ble_included_service_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	// check if end handle is not smaller than start handle
	if(end_handle>= start_handle)
	{
		gattc_disc_cmd_handler(GATTC_DISC_INCLUDED_SVC, 
			0,
			start_handle, end_handle, NULL, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}
	
	return status;

}


at_ble_status_t at_ble_characteristic_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
	at_ble_status_t status = AT_BLE_SUCCESS;


	// check if end handle is not smaller than start handle
	if((end_handle >= start_handle) && (start_handle > 0))
	{
		gattc_disc_cmd_handler(GATTC_DISC_ALL_CHAR, 0,
			start_handle, end_handle, NULL, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}

	return status;
}


at_ble_status_t at_ble_characteristic_discover_by_uuid(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t* uuid)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	uint32_t uuid_len = 0, valid_uuid = 0;

	// check UUID is non zero
	uuid_len = at_ble_uuid_type2len(uuid->type);

	for(;uuid_len>0;uuid_len--)
	{
		valid_uuid += uuid->uuid[uuid_len - 1];
	}

	// check if end handle is not smaller than start handle
	if((end_handle >= start_handle) && (start_handle > 0) && (valid_uuid))
	{
		gattc_disc_cmd_handler(GATTC_DISC_BY_UUID_CHAR, 
			at_ble_uuid_type2len(uuid->type),
			start_handle, end_handle, uuid->uuid, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}
	
	return status;
}

at_ble_status_t at_ble_descriptor_discover_all(at_ble_handle_t conn_handle,
	at_ble_handle_t start_handle, at_ble_handle_t end_handle)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	// check if end handle is not smaller than start handle
	if((end_handle >= start_handle) && (start_handle > 0))
	{
		gattc_disc_cmd_handler(GATTC_DISC_DESC_CHAR, 
			0,
			start_handle, end_handle, NULL, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}
	
	return status;	
}



at_ble_status_t at_ble_characteristic_read_by_uuid(at_ble_handle_t conn_handle, 
    at_ble_handle_t start_handle, at_ble_handle_t end_handle, at_ble_uuid_t* uuid)
{
	at_ble_status_t  status = AT_BLE_SUCCESS; //, u8PktId, u8RequestType;
	//uint16_t u16MsgId, u16DestId, u16SrcId;

	// check if end handle is not smaller than start handle
	if(end_handle>= start_handle)
	{
		gattc_read_cmd_handler_by_uuid(start_handle, end_handle,
			at_ble_uuid_type2len(uuid->type), uuid->uuid, conn_handle);
	}
	else
	{
		status = AT_BLE_INVALID_PARAM;
	}
	
	return status;	
}



at_ble_status_t at_ble_characteristic_read(at_ble_handle_t conn_handle, at_ble_handle_t char_handle, uint16_t offset, uint16_t length)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	gattc_read_cmd_handler_simple_read(char_handle, offset, length, conn_handle);
	return status;	
}


at_ble_status_t at_ble_characteristic_read_multible(at_ble_handle_t conn_handle,  at_ble_handle_t* char_handle_list, 
	uint16_t* char_val_lenght, uint8_t char_handle_count)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	
	gattc_read_cmd_handler_multible(char_handle_list, char_handle_count, char_val_lenght, conn_handle);
	return status;
}



at_ble_status_t at_ble_characteristic_write(at_ble_handle_t conn_handle, at_ble_handle_t char_handle,
			uint16_t offset, uint16_t length, uint8_t* data,
			bool signed_write, bool with_response )
{
	uint8_t req;

	at_ble_status_t status = AT_BLE_SUCCESS;

	if(signed_write)
	{
		req = GATTC_WRITE_SIGNED;
	}
	else if(with_response)
	{
		req = GATTC_WRITE;
	}
	else
	{
		req = GATTC_WRITE_NO_RESPONSE;
	}

	gattc_write_cmd_handler(req, 1, char_handle, offset, length, data, conn_handle);
	
	return status;
}


at_ble_status_t at_ble_characteristic_reliable_write_prepare(at_ble_handle_t conn_handle,
        at_ble_handle_t char_handle, 
		uint16_t offset, uint16_t length, uint8_t* data)
{
	at_ble_status_t status = AT_BLE_SUCCESS;

	gattc_write_cmd_handler(GATTC_WRITE, 0, char_handle, offset, length, data, conn_handle);
	return status;
}

at_ble_status_t at_ble_characteristic_reliable_write_execute(at_ble_handle_t conn_handle)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	gattc_execute_write_cmd_handler(1, conn_handle);
	return status;
}

at_ble_status_t at_ble_characteristic_reliable_write_cancel(at_ble_handle_t conn_handle)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	gattc_execute_write_cmd_handler(0, conn_handle);
	return status;
}


