/**
 * \file
 *
 * \brief HID Service
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel micro controller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 *Support</a>
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the HID Service
 */
/*- Includes ---------------------------------------------------------------*/
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "at_ble_api.h"
#include "ble_manager.h"
#include "hid.h"

generic_Att_Desc report_desc[HID_NUM_OF_REPORT];
gatt_service_handler_t hid_inst[HID_MAX_SERV_INST];
hid_serv_t hid_serv_inst[HID_MAX_SERV_INST];   /**< HID service instance. */
uint8_t ctrl_point[0];

/**
* \HID service definition initialization function
*/
void hid_serv_def_init(uint8_t servnum)
{
	uint8_t id = 0;
	
	hid_serv_inst[servnum].hid_dev_serv_handle = NULL;
	hid_serv_inst[servnum].hid_dev_serv_uuid = NULL;
	hid_serv_inst[servnum].hid_dev_proto_mode_char = NULL;
	hid_serv_inst[servnum].hid_dev_report_map_char = NULL;
	for(; id<HID_NUM_OF_REPORT; id++)
	{
		hid_serv_inst[servnum].hid_dev_report_val_char[id] = NULL;
	}
	
#ifdef HID_KEYBOARD_DEVICE	
	hid_serv_inst[servnum].hid_dev_boot_keyboard_in_report = NULL;
	hid_serv_inst[servnum].hid_dev_boot_keyboard_out_report= NULL;
#endif

#ifdef HID_MOUSE_DEVICE
    hid_serv_inst[servnum].hid_dev_boot_mouse_in_report= NULL;
#endif
    
    hid_serv_inst[servnum].hid_dev_info= NULL;
    hid_serv_inst[servnum].hid_control_point= NULL;
}

/**
* \HID service Configuration function
*/
void hid_serv_init(uint8_t servinst, uint8_t device, uint8_t *mode, uint8_t report_num, uint8_t *report_type, uint8_t **report_val, uint8_t *report_len, hid_info_t *info)
{
	uint8_t id = 0;
	
	/* Configure HID Service */
	hid_inst[servinst].serv_uuid.type = AT_BLE_UUID_16;
	hid_inst[servinst].serv_uuid.uuid[0] = (uint8_t) HID_SERV_UUID;
	hid_inst[servinst].serv_uuid.uuid[1] = (uint8_t) (HID_SERV_UUID>>8);	
	
	/*Map the service UUID instance*/
	hid_serv_inst[servinst].hid_dev_serv_uuid = &hid_inst[servinst].serv_uuid;
	
	hid_inst[servinst].serv_handle = 0;
	
	/*Map the service UUID instance*/
	hid_serv_inst[servinst].hid_dev_serv_handle = &hid_inst[servinst].serv_handle;
	
	/*Configure HID Protocol Mode Characteristic*/
    hid_inst[servinst].serv_chars[0].uuid.type = AT_BLE_UUID_16;
	hid_inst[servinst].serv_chars[0].uuid.uuid[0] =(uint8_t) HID_UUID_CHAR_PROTOCOL_MODE;
	hid_inst[servinst].serv_chars[0].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_PROTOCOL_MODE >> 8);
	hid_inst[servinst].serv_chars[0].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE);
	hid_inst[servinst].serv_chars[0].init_value = mode;
	hid_inst[servinst].serv_chars[0].value_init_len = sizeof(uint8_t);
	hid_inst[servinst].serv_chars[0].value_max_len = sizeof(uint8_t);
	hid_inst[servinst].serv_chars[0].value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);
	hid_inst[servinst].serv_chars[0].DescList = NULL;
	hid_inst[servinst].serv_chars[0].DescCount = 0;
	
    /*Map the device protocol mode chatacteristic*/
	hid_serv_inst[servinst].hid_dev_proto_mode_char = &hid_inst[servinst].serv_chars[0];

	/*Configure HID Report Map Characteristic*/
	hid_inst[servinst].serv_chars[1].uuid.type = AT_BLE_UUID_16;
	hid_inst[servinst].serv_chars[1].uuid.uuid[0] =(uint8_t) HID_UUID_CHAR_REPORT_MAP;
	hid_inst[servinst].serv_chars[1].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_REPORT_MAP >> 8);
	hid_inst[servinst].serv_chars[1].properties = AT_BLE_CHAR_READ;
	hid_inst[servinst].serv_chars[1].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	hid_inst[servinst].serv_chars[1].DescList = NULL;
	hid_inst[servinst].serv_chars[1].DescCount = 0;
	
	/*Map the device protocol mode chatacteristic*/
	hid_serv_inst[servinst].hid_dev_report_map_char = &hid_inst[servinst].serv_chars[1];
	
	/*Configure number of HID report*/
	for(id = 1; id <= report_num; ++id)
	{
		/*Configure HID Report Characteristic*/
		hid_inst[servinst].serv_chars[id + 1].uuid.type = AT_BLE_UUID_16;
		hid_inst[servinst].serv_chars[id + 1].uuid.uuid[0] = (uint8_t)HID_UUID_CHAR_REPORT;
		hid_inst[servinst].serv_chars[id + 1].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_REPORT >> 8);
		hid_inst[servinst].serv_chars[id + 1].init_value = (uint8_t *)((report_val[id-1]));
		hid_inst[servinst].serv_chars[id + 1].value_init_len = report_len[id-1];
		hid_inst[servinst].serv_chars[id + 1].value_max_len =  report_len[id-1];
		
		if(report_type[id-1] == INPUT_REPORT)
		{
			hid_inst[servinst].serv_chars[id + 1].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_NOTIFY);
			hid_inst[servinst].serv_chars[id + 1].value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR);
		}
		else if(report_type[id-1] == OUTPUT_REPORT)
		{
			hid_inst[servinst].serv_chars[id + 1].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE|AT_BLE_CHAR_WRITE);
			hid_inst[servinst].serv_chars[id + 1].value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);
		}
		else if(report_type[id-1] == FEATURE_REPORT)
		{
			hid_inst[servinst].serv_chars[id + 1].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_WRITE);
			hid_inst[servinst].serv_chars[id + 1].value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);
  	    }
		
		hid_inst[servinst].serv_desc[id - 1].handle   =  0;
		hid_inst[servinst].serv_desc[id - 1].desc_val_length = (sizeof(uint8_t)*2);
		hid_inst[servinst].serv_desc[id - 1].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		hid_inst[servinst].serv_desc[id - 1].uuid.type = AT_BLE_UUID_16;
		hid_inst[servinst].serv_desc[id - 1].uuid.uuid[0] =(uint8_t) HID_REPORT_REF_DESC;
		hid_inst[servinst].serv_desc[id - 1].uuid.uuid[1] = (uint8_t)(HID_REPORT_REF_DESC >> 8);
		
		hid_inst[servinst].serv_chars[id + 1].DescList = &hid_inst[servinst].serv_desc[id - 1];
		hid_inst[servinst].serv_chars[id + 1].DescCount = 1;
	
		/*Map the report characteristic*/
		hid_serv_inst[servinst].hid_dev_report_val_char[id-1] = &hid_inst[servinst].serv_chars[id + 1];
		
	}//End of for loop
	
	id++;
	
	if(device == HID_MOUSE_MODE)
	{
#ifdef HID_MOUSE_DEVICE

         /*Configure HID Boot Mouse Input Report Characteristic*/
        hid_inst[servinst].serv_chars[id].uuid.type = AT_BLE_UUID_16;
		hid_inst[servinst].serv_chars[id].uuid.uuid[0] =(uint8_t) HID_UUID_CHAR_BOOT_MOUSE_INPUT_REPORT;
		hid_inst[servinst].serv_chars[id].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_BOOT_MOUSE_INPUT_REPORT>>8);
        hid_inst[servinst].serv_chars[id].init_value = NULL;
        hid_inst[servinst].serv_chars[id].value_init_len = 0;
        hid_inst[servinst].serv_chars[id].value_max_len = 0;
        hid_inst[servinst].serv_chars[id].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_NOTIFY);
        hid_inst[servinst].serv_chars[id].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		hid_inst[servinst].serv_chars[id].DescList = NULL;
		hid_inst[servinst].serv_chars[id].DescCount = 0;
		
		/*Map the device protocol mode chatacteristic*/
		hid_serv_inst[servinst].hid_dev_boot_mouse_in_report = &hid_inst[servinst].serv_chars[id];
		id++;
#endif
	}
	else if(device == HID_KEYBOARD_MODE)
	{
#ifdef HID_KEYBOARD_DEVICE

         /*Configure HID Boot Mouse Input Report Characteristic*/
		 hid_inst[servinst].serv_chars[id].uuid.type = AT_BLE_UUID_16;
		 hid_inst[servinst].serv_chars[id].uuid.uuid[0] = (uint8_t)HID_UUID_CHAR_BOOT_KEY_OUTPUT_REPORT;
		 hid_inst[servinst].serv_chars[id].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_BOOT_KEY_OUTPUT_REPORT>>8);
		 hid_inst[servinst].serv_chars[id].init_value = NULL;
		 hid_inst[servinst].serv_chars[id].value_init_len = 0;
		 hid_inst[servinst].serv_chars[id].value_max_len = 0;
		 hid_inst[servinst].serv_chars[id].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE|AT_BLE_CHAR_WRITE);
		 hid_inst[servinst].serv_chars[id].value_permissions = (AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR | AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR);
		 hid_inst[servinst].serv_chars[id].DescList = NULL;
		 hid_inst[servinst].serv_chars[id].DescCount = 0;
		 
		 /*Map the device protocol mode characteristic*/
		 hid_serv_inst[servinst].hid_dev_boot_keyboard_out_report = &hid_inst[servinst].serv_chars[id];
		 
		 id++;
		 
		/*Configure HID Boot Keyboard Output Report Characteristic*/
		 hid_inst[servinst].serv_chars[id].uuid.type = AT_BLE_UUID_16;
		 hid_inst[servinst].serv_chars[id].uuid.uuid[0] = (uint8_t)HID_UUID_CHAR_BOOT_KEY_INPUT_REPORT;
		 hid_inst[servinst].serv_chars[id].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_BOOT_KEY_INPUT_REPORT>>8);
		 hid_inst[servinst].serv_chars[id].init_value = NULL;
		 hid_inst[servinst].serv_chars[id].value_init_len = 0;
		 hid_inst[servinst].serv_chars[id].value_max_len = 0;
		 hid_inst[servinst].serv_chars[id].properties = (AT_BLE_CHAR_READ|AT_BLE_CHAR_NOTIFY);
		 hid_inst[servinst].serv_chars[id].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		 hid_inst[servinst].serv_chars[id].DescList = NULL;
		 hid_inst[servinst].serv_chars[id].DescCount = 0;
		 
		 /*Map the device protocol mode characteristic*/
		 hid_serv_inst[servinst].hid_dev_boot_keyboard_in_report = &hid_inst[servinst].serv_chars[id];
		 
		 id++;
#endif
	}
 
    /*Configure HID Information Characteristic*/
    hid_inst[servinst].serv_chars[id].uuid.type = AT_BLE_UUID_16;
	hid_inst[servinst].serv_chars[id].uuid.uuid[0] = (uint8_t)HID_UUID_CHAR_HID_INFORMATION;
	hid_inst[servinst].serv_chars[id].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_HID_INFORMATION>>8);
    hid_inst[servinst].serv_chars[id].init_value = (uint8_t*)info;
    hid_inst[servinst].serv_chars[id].value_init_len = sizeof(hid_info_t);
    hid_inst[servinst].serv_chars[id].value_max_len = sizeof(hid_info_t);
    hid_inst[servinst].serv_chars[id].properties = AT_BLE_CHAR_READ;
    hid_inst[servinst].serv_chars[id].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	hid_inst[servinst].serv_chars[id].DescList = NULL;
	hid_inst[servinst].serv_chars[id].DescCount = 0;
	
	/*Map the HID Information characteristic*/
	hid_serv_inst[servinst].hid_dev_info = &hid_inst[servinst].serv_chars[id];
	
	id++;
	
	/*Configure HID Control Point Characteristic*/
	hid_inst[servinst].serv_chars[id].uuid.type = AT_BLE_UUID_16;
	hid_inst[servinst].serv_chars[id].uuid.uuid[0] = (uint8_t)HID_UUID_CHAR_HID_CONTROL_POINT;
	hid_inst[servinst].serv_chars[id].uuid.uuid[1] = (uint8_t)(HID_UUID_CHAR_HID_CONTROL_POINT>>8);
	hid_inst[servinst].serv_chars[id].init_value = (uint8_t*)&ctrl_point;
	hid_inst[servinst].serv_chars[id].value_init_len = sizeof(uint8_t);
	hid_inst[servinst].serv_chars[id].value_max_len = sizeof(uint8_t);
	hid_inst[servinst].serv_chars[id].properties = AT_BLE_CHAR_WRITE_WITHOUT_RESPONSE;
	hid_inst[servinst].serv_chars[id].value_permissions = AT_BLE_ATTR_WRITABLE_NO_AUTHN_NO_AUTHR;
	hid_inst[servinst].serv_chars[id].DescList = NULL;
	hid_inst[servinst].serv_chars[id].DescCount = 0;
	
	/*Map the HID Information characteristic*/
	hid_serv_inst[servinst].hid_control_point = &hid_inst[servinst].serv_chars[id];
}

/**
* \Function update the report descriptor for HID device
*/
void hid_serv_report_map(uint8_t servinst, uint8_t *report_info, uint16_t len)
{
	hid_inst[servinst].serv_chars[1].init_value = report_info;
	hid_serv_inst[servinst].hid_dev_report_map_char->value_init_len = len;
	hid_serv_inst[servinst].hid_dev_report_map_char->value_max_len = len;
}

/**
* \Function register the HID service.
*/
uint16_t hid_service_dbreg(uint8_t inst, uint8_t *report_type, uint8_t *report_id, uint8_t report_num)
{
	
	uint8_t id = 0;
	uint8_t descval[2] = {0, 0};
	
	DBG_LOG_DEV("Number of characteristic %d", HID_CHARACTERISTIC_NUM);
	
	if( at_ble_service_define(&hid_inst[inst].serv_uuid, &hid_inst[inst].serv_handle, NULL, 0, hid_inst[inst].serv_chars, HID_CHARACTERISTIC_NUM, PRIMAR_SVC) == AT_BLE_SUCCESS)
	{
		
		DBG_LOG_DEV("Define service handle %d", hid_inst[inst].serv_handle);
		DBG_LOG_DEV("Report Reference descriptor handle %d", hid_serv_inst[inst].hid_dev_report_val_char[0]->DescList->handle);

		for(; id<report_num; id++)
		{
			DBG_LOG_DEV("Report Reference descriptor ID = %d :: Type = %d", report_id[id], report_type[id]);
			descval[0] = report_id[id];
			descval[1] = report_type[id];
			if(at_ble_descriptor_value_set(hid_serv_inst[inst].hid_dev_report_val_char[id]->DescList->handle, descval, 2) == AT_BLE_SUCCESS)
			{
				DBG_LOG_DEV("Descriptor Value set successfully");
			}
			id++;	
		}
		
		return hid_inst[inst].serv_handle;
	}
	else
	{
		return HID_SERV_FAILED;
	}
}

/**
* \Function to get service instance.
*/
uint8_t hid_serv_get_instance(uint16_t handle)
{
	
	uint8_t id = 0;
	
	DBG_LOG_DEV("hid_serv_get_instance : Search Handle %d", handle);
	
	for(; id<HID_MAX_SERV_INST; id++)
	{
		DBG_LOG_DEV("hid_serv_get_instance : Service Handle %d  Characteristic Handle %d", *(hid_serv_inst[id].hid_dev_serv_handle), hid_serv_inst[id].hid_control_point->char_val_handle);
		if((handle > *(hid_serv_inst[id].hid_dev_serv_handle)) && (handle <= hid_serv_inst[id].hid_control_point->char_val_handle))
		{
			DBG_LOG_DEV("hid_serv_get_instance : Service Handle %d", id);
			return id;
		}
	}
	return -1;
}

/**
* \Function to get the notification instance.
*/
uint8_t hid_ntf_instance(uint8_t serv_num, uint16_t char_handle)
{
	uint8_t id = 0;
	
	DBG_LOG_DEV("hid_ntf_instance : Search Handle %d", char_handle);
	DBG_LOG_DEV("hid_ntf_instance : Protocol Mode Handle %d", hid_serv_inst[serv_num].hid_dev_proto_mode_char->char_val_handle);

	if(hid_serv_inst[serv_num].hid_dev_proto_mode_char->char_val_handle == char_handle)
	{
		return PROTOCOL_MODE;
	}
		
	for(; id<HID_NUM_OF_REPORT; id++)
	{
		DBG_LOG_DEV("hid_ntf_instance : Report Handle %d", hid_serv_inst[serv_num].hid_dev_report_val_char[id]->char_val_handle);
		if(char_handle == hid_serv_inst[serv_num].hid_dev_report_val_char[id]->char_val_handle)
		{
			return CHAR_REPORT;
		}
	
		DBG_LOG_DEV("hid_ntf_instance : Report CCD Handle %d",hid_serv_inst[serv_num].hid_dev_report_val_char[id]->client_config_handle);

		if(char_handle == hid_serv_inst[serv_num].hid_dev_report_val_char[id]->client_config_handle)
		{
		   return CHAR_REPORT_CCD;
		}
			
	}
		
#ifdef HID_KEYBOARD_DEVICE

	DBG_LOG_DEV("hid_ntf_instance : Boot Keyboard Handle %d", hid_serv_inst[serv_num].hid_dev_boot_keyboard_in_report->char_val_handle);

	if(hid_serv_inst[serv_num].hid_dev_boot_keyboard_in_report->char_val_handle == char_handle)
	{
		return BOOT_KEY_INPUT_REPORT;
	}
#endif

#ifdef HID_MOUSE_DEVICE
	DBG_LOG_DEV("hid_ntf_instance : Boot Mouse Handle %d", hid_serv_inst[serv_num].hid_dev_boot_mouse_in_report->char_val_handle);
	if(hid_serv_inst[serv_num].hid_dev_boot_mouse_in_report->char_val_handle == char_handle)
	{
		return BOOT_MOUSE_INPUT_REPORT;
	}
#endif

	DBG_LOG_DEV("hid_ntf_instance : Control Point Handle %d", hid_serv_inst[serv_num].hid_control_point->char_val_handle);
	if(hid_serv_inst[serv_num].hid_control_point->char_val_handle == char_handle)
	{
		return CONTROL_POINT;
	}
	return 0;
}

/**
* \Function to get the report ID.
*/
uint8_t hid_get_reportid(uint8_t serv, uint16_t handle, uint8_t reportnum)
{
	uint8_t status;
	uint8_t id = 0;
	uint8_t descval[2] = {0, 0};
	uint16_t len;	

	DBG_LOG_DEV("Inside hid_get_reportid : Report Number %d", reportnum);
	
	for(id = 0; id <= reportnum; id++)
	{
		DBG_LOG_DEV("id %d Search Handle %d, CCD Handle %d", id, handle, hid_serv_inst[serv].hid_dev_report_val_char[id]->client_config_handle);
		if(handle == hid_serv_inst[serv].hid_dev_report_val_char[id]->client_config_handle)
		{
			DBG_LOG_DEV("Inside hid_get_reportid : Report ID Descriptor Handle %d :: id %d :: serv %d", hid_serv_inst[serv].hid_dev_report_val_char[id]->DescList->handle, id, serv);
			status = at_ble_descriptor_value_get(hid_serv_inst[serv].hid_dev_report_val_char[id]->DescList->handle, descval, &len);
			if (status != AT_BLE_SUCCESS)
			{
				DBG_LOG_DEV("decriptor value get failed");
			}
			DBG_LOG_DEV("hid_get_reportid : Report ID %d", descval[0]);
			return descval[0];
		}
	}
	return HID_INVALID_INST;
}

/**
* \Function to get the report characteristic id.
*/
uint8_t hid_get_reportchar(uint16_t handle, uint8_t serv, uint8_t reportid)
{
	uint8_t id = 0;
	uint16_t len = 0;
	uint8_t descval[2] = {0, 0};
	at_ble_status_t status ;

	DBG_LOG_DEV("Inside hid_get_reportchar : Handle %d Service Instance %d Report ID %d", handle, serv, reportid);	

	for(id = 0; id <= HID_NUM_OF_REPORT; id++)
	{
		DBG_LOG_DEV("Inside hid_get_reportchar : Report ID Descriptor Handle %d :: id %d :: serv %d", hid_serv_inst[serv].hid_dev_report_val_char[id]->DescList->handle, id, serv);
		status = at_ble_descriptor_value_get(hid_serv_inst[serv].hid_dev_report_val_char[id]->DescList->handle, descval, &len);
		if (status != AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("decriptor value get failed");
		}
		DBG_LOG_DEV("Inside hid_get_reportchar : Report Value ID %d", descval[0]);
		if(descval[0] == reportid)
		{
			DBG_LOG_DEV("hid_get_reportchar : Report Characteristic ID %d", id);
			return id;
		}
	}
    return HID_INVALID_INST;
}

/**
* \Function to update the report.
*/
void hid_serv_report_update(uint16_t conn_handle, uint8_t serv_inst, uint8_t reportid, uint8_t *report, uint16_t len)
{
	uint8_t value = 0;
	uint8_t id;
	uint16_t status = 0;
	
	id = hid_get_reportchar(conn_handle, serv_inst, reportid);

	DBG_LOG_DEV("hid_serv_report_update : Report Characteristic ID %d", id);
	status = at_ble_characteristic_value_get(hid_serv_inst[serv_inst].hid_dev_report_val_char[id]->client_config_handle, &value, 0, sizeof(uint16_t), sizeof(uint16_t));
	if (status != AT_BLE_SUCCESS)
	{
		DBG_LOG_DEV("decriptor value get failed");
	}
	DBG_LOG_DEV("hid_serv_report_update : Value %d", value);

	//If Notification Enabled
	if(value == 1)
	{
		if((status = at_ble_characteristic_value_set(hid_serv_inst[serv_inst].hid_dev_report_val_char[id]->char_val_handle, report, 0, len))==AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("hid_serv_report_update : Notify Value : conn_handle %d", conn_handle);
			//Need to check for connection handle
			status = at_ble_notification_send(conn_handle, hid_serv_inst[serv_inst].hid_dev_report_val_char[0]->char_val_handle);	
			if (status != AT_BLE_SUCCESS)
			{
				DBG_LOG_DEV("decriptor value get failed");
			}
		}
		else
		{
			DBG_LOG_DEV("Fail Reason %d", status);

		}
	}
}

/**
* \Function to update the boot mouse report.
*/
void hid_boot_mousereport_update(at_ble_handle_t conn_handle, uint8_t serv_inst, uint8_t *bootreport, uint16_t len)
{
	
#ifdef HID_MOUSE_DEVICE	
	uint8_t value = 0;
	uint8_t status;
	status = at_ble_characteristic_value_get(hid_serv_inst[serv_inst].hid_dev_boot_mouse_in_report->client_config_handle, &value, 0, sizeof(uint16_t), sizeof(uint16_t));
	if (status != AT_BLE_SUCCESS)
	{
		DBG_LOG_DEV("charactertistic  value get failed");
	}
	//If Notification Enabled
	if(value == 1)
	{
		status = at_ble_characteristic_value_set(hid_serv_inst[serv_inst].hid_dev_boot_mouse_in_report->char_val_handle, bootreport, 0, len);
		if (status != AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("decriptor value get failed");
		}
		//Need to check for connection handle
		status = at_ble_notification_send(conn_handle, hid_serv_inst[serv_inst].hid_dev_boot_mouse_in_report->char_val_handle);
		if (status != AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("decriptor value get failed");
		}
	}
#endif	
}

/**
* \Function to update the boot keyboard report.
*/
void hid_boot_keyboardreport_update(at_ble_handle_t conn_handle, uint8_t serv_inst, uint8_t *bootreport, uint16_t len)
{
#ifdef HID_KEYBOARD_DEVICE	
	at_ble_status_t status;
	uint8_t value = 0;
	status = at_ble_characteristic_value_get(hid_serv_inst[serv_inst].hid_dev_boot_keyboard_in_report->client_config_handle, &value, 0, sizeof(uint16_t), sizeof(uint16_t));
	if (status != AT_BLE_SUCCESS)
	{
		DBG_LOG_DEV_DEV("at_ble_characteristic_value_get value get failed");
	}
	//If Notification Enabled
	if(value == 1)
	{
		status = at_ble_characteristic_value_set(hid_serv_inst[serv_inst].hid_dev_boot_keyboard_in_report->char_val_handle, bootreport, 0, len);
		if (status != AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("at_ble_characteristic_value_set value get failed");
		}
		//Need to check for connection handle
		status = at_ble_notification_send(conn_handle, hid_serv_inst[serv_inst].hid_dev_boot_keyboard_in_report->char_val_handle);
		if (status != AT_BLE_SUCCESS)
		{
			DBG_LOG_DEV("at_ble_characteristic_value_set value get failed");
		}
	}
#endif	
}



