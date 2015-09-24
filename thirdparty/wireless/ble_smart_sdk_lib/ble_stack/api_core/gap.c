
#include "at_ble_api.h"
#include "dbg_logs.h"
#include "platform.h"
#include "interface.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "dbg_task.h"

#include "device.h"
#include "event.h"
#ifndef SAMB11 
#include "at_ble_patch.h"
#endif


// Slave preferred Connection interval Min
#define APP_CON_INTV_MIN    8
// Slave preferred Connection interval Max
#define APP_CON_INTV_MAX    10
// Slave preferred Connection latency
#define APP_CON_LATENCY     0
// Slave preferred Link supervision timeout
#define APP_SUPERV_TO       200 // 2s (500*10ms)

#define APP_MAX_MTU         L2C_MIN_LE_MTUSIG
#define APP_FLAGS           0

#define LOCAL_ADDR_TYPE GAPM_PUBLIC_ADDR
#define GAP_DEVICE_NAME "ATMEL_BLE"

#define PWR_REG_ADDR        0x4002084C

struct device_info device;
tstrConnData gstrConnData[AT_BLE_MAX_CONCURRENT_CONNS];
extern volatile int init_done;


at_ble_status_t at_ble_init(at_ble_init_config_t *args)
{

#ifdef SAMB11
	/* TODO map Driver errors to at_ble_status_t */
	plf_drv_status platform_status = STATUS_SUCCESS;
#endif
	at_ble_status_t  status = AT_BLE_FAILURE;
//#ifndef SAMB11
#ifndef PATCH_GUI
#ifndef USE_RW_FPGA
	at_ble_events_t event;
	uint8_t params[8];
#endif
#endif
//#endif

	FN_IN();

#ifdef SAMB11
	do
	{
		/* Initialise database (GATT Server)*/
		if(args && args->memPool.memStartAdd)
		{
			if(args->memPool.memSize <= 0)
			{
				PRINT_ERR("Invalid Parameters, DB memory size provided is incorrect \r\n");
				break;
			}

			db_init(args->memPool.memStartAdd, args->memPool.memSize);
			PRINT_INFO("Database Init Success \r\n");
		}
		else
		{
			PRINT_DBG("No Database Info provided\r\n");
		}

		/* Platform Initialisation */
		platform_status = platform_driver_init();
		if (platform_status != STATUS_SUCCESS)
		{
			PRINT_ERR("Unable to init platform <%02X>\r\n", platform_status);
			break;
		}

		/* Register Event Handler Callback */
		platform_status = platform_register_ble_msg_handler((platform_interface_callback) rx_callback);
		if (platform_status != STATUS_SUCCESS)
		{
			PRINT_ERR("Unable Register Event Handler Callback <%02X>\r\n", platform_status);
			break;
		}

		/* initialize interface layer */
		interface_init();
		/* initialise event handler */
		event_init();

		while (at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
		{
			if (event == AT_BLE_DEVICE_READY)
			{
				break;
			}
		}
    //init_done = 1;

		/* Send Gap Reset to FW task */
		status = (at_ble_status_t)gapm_reset_req_handler();
		if (status != AT_BLE_SUCCESS)
		{
			PRINT_ERR("Unable to reset <%02X>", status);
			break;
		}
			PRINT_INFO("GAPM Reset Success \r\n");


		/* Initialise device info */
		{
			uint8_t loopCntr;
			uint8_t *ptr = NULL;
			for (loopCntr = 0; loopCntr < AT_BLE_MAX_CONCURRENT_CONNS; loopCntr++)
			{
				gstrConnData[loopCntr].conHandle = 0xFFFF;
			}
			device.config.role = AT_BLE_ROLE_ALL;
			device.config.renew_dur = AT_RENEW_DUR_VAL_MIN;
			device.config.address.type = AT_BLE_ADDRESS_PUBLIC;
			ptr = (uint8_t *) & (device.config.irk[0]);
			for (loopCntr = 0; loopCntr < (AT_BLE_MAX_KEY_LEN * 2); loopCntr += 2)
			{
				*(ptr++) = loopCntr | ((loopCntr + 1) << 4);
			}
			//0x00 (Default 0x20)
			device.config.att_cfg.b2NamePerm = AT_BLE_WRITE_DISABLE;
			device.config.att_cfg.b2AppearancePerm = AT_BLE_WRITE_DISABLE;
			device.config.att_cfg.b1EnableSpcs = 0;
			device.config.att_cfg.b1EnableServiceChanged = 0;//(Default 1)
			device.config.att_cfg.b2Rfu = AT_BLE_WRITE_DISABLE;
			device.config.gap_start_hdl = AT_BLE_AUTO_ALLOC_HANDLE;
			device.config.gatt_start_hdl = AT_BLE_AUTO_ALLOC_HANDLE;
			device.config.max_mtu = AT_MTU_VAL_RECOMMENDED;
			/*LocaLS*/
			device.srLen = 0;
			device.advLen = 0;
			device.privacy_flags = 0;
			device.appearance = 0;
			device.spcp_param.con_intv_max = AT_BLE_SLV_PREF_CON_INTV_MAX;
			device.spcp_param.con_intv_min = AT_BLE_SLV_PREF_CON_INTV_MIN;
			device.spcp_param.con_latency = AT_BLE_SLV_PREF_CON_LATENCY;
			device.spcp_param.superv_to = AT_BLE_SLV_PREF_SUPV_TO;
			device.dev_name_write_perm = GAPM_WRITE_DISABLE;
			status = at_ble_set_dev_config((at_ble_dev_config_t *)&device.config);
			//Advertising channel
			device.u8AdvChnlMap = AT_BLE_ADV_ALL_CHNLS_EN;
		}
		PRINT_INFO("Device Info Initialisation success \r\n");

		/* Set Device Default Name */
		{
			uint8_t dev_name[] = {'A', 't', 'm', 'e', 'l', ' ', 'C', 'o', 'r', 'p', '.'};
			status = at_ble_device_name_set(&dev_name[0], sizeof(dev_name));
			if (AT_BLE_SUCCESS != status)
			{
				PRINT_ERR("Unable to set default device name <%02X>\r\n", status);
				break;
			}
		}
		PRINT_INFO("Device Name set Success \r\n");

		/* If We made till here then success*/
		status = AT_BLE_SUCCESS;
	}while(0);
#else
	do
	{
		/* Platform Initialization */
		if (args && args->plf_config)
		{
			status = platform_init((args->plf_config));
			if (status != AT_BLE_SUCCESS)
			{
				PRINT_ERR("Unable to init platform <%02X>\r\n", status);
				break;
			}
		}
		else
		{
            platform_config tempConfig = {AT_BLE_UART, 0};
			status = platform_init(&tempConfig);
			if (status != AT_BLE_SUCCESS)
			{
				PRINT_ERR("Unable to init platform <%02X>\r\n", status);
				break;
			}
		}
		/* initialize event handler before patching, to be able to receive ready message! */
		event_init();
#ifndef PATCH_GUI
#ifndef USE_RW_FPGA
		status = patch_init();
		if (status != AT_BLE_SUCCESS)
		{
			PRINT_ERR("Unable to init patch <%02X>\r\n", status);
			break;
		}
		while (at_ble_event_get(&event, params, -1) == AT_BLE_SUCCESS)
		{
			if (event == AT_BLE_DEVICE_READY)
			{
				break;
			}
		}
        init_done = 1;
#endif
#endif
		status = gapm_reset_req_handler();
		if (status != AT_BLE_SUCCESS)
		{
			PRINT_ERR("Unable to reset <%02X>", status);
			break;
		}
		// init device info
		if (status == AT_BLE_SUCCESS)
		{
			uint8_t loopCntr;
			uint8_t *ptr = NULL;
			for (loopCntr = 0; loopCntr < AT_BLE_MAX_CONCURRENT_CONNS; loopCntr++)
			{
				gstrConnData[loopCntr].conHandle = 0xFFFF;
			}
			device.config.role = AT_BLE_ROLE_ALL;
			device.config.renew_dur = AT_RENEW_DUR_VAL_MIN;
			device.config.address.type = AT_BLE_ADDRESS_PUBLIC;
			ptr = (uint8_t *) & (device.config.irk[0]);
			for (loopCntr = 0; loopCntr < (AT_BLE_MAX_KEY_LEN * 2); loopCntr += 2)
			{
				*(ptr++) = loopCntr | ((loopCntr + 1) << 4);
			}
			//0x00 (Default 0x20)
			device.config.att_cfg.b2NamePerm = AT_BLE_WRITE_DISABLE;
			device.config.att_cfg.b2AppearancePerm = AT_BLE_WRITE_DISABLE;
			device.config.att_cfg.b1EnableSpcs = 0;
			device.config.att_cfg.b1EnableServiceChanged = 0;//(Default 1)
			device.config.att_cfg.b2Rfu = AT_BLE_WRITE_DISABLE;
			device.config.gap_start_hdl = AT_BLE_AUTO_ALLOC_HANDLE;
			device.config.gatt_start_hdl = AT_BLE_AUTO_ALLOC_HANDLE;
			device.config.max_mtu = AT_MTU_VAL_RECOMMENDED;
			/*LocaLS*/
			device.srLen = 0;
			device.advLen = 0;
			device.privacy_flags = 0;
			device.appearance = 0;
			device.spcp_param.con_intv_max = AT_BLE_SLV_PREF_CON_INTV_MAX;
			device.spcp_param.con_intv_min = AT_BLE_SLV_PREF_CON_INTV_MIN;
			device.spcp_param.con_latency = AT_BLE_SLV_PREF_CON_LATENCY;
			device.spcp_param.superv_to = AT_BLE_SLV_PREF_SUPV_TO;
			device.dev_name_write_perm = GAPM_WRITE_DISABLE;
			status = at_ble_set_dev_config((at_ble_dev_config_t *)&device.config);
			//Advertising channel
			device.u8AdvChnlMap = AT_BLE_ADV_ALL_CHNLS_EN;



			/* Set Device Default Name */
			status = at_ble_device_name_set(&dev_name[0], sizeof(dev_name));
			if (AT_BLE_SUCCESS != status)
			{
				PRINT_ERR("Unable to set default device name <%02X>\r\n", status);
				break;
			}
		}
	}
	while (0);
#endif

	FN_OUT(status);
	return status;
}

at_ble_status_t at_ble_set_privacy_key(at_ble_gap_irk_t *irk , uint16_t interval)
{
    at_ble_status_t  status = AT_BLE_INVALID_PARAM;
    do
    {
        if (interval <= 0)
        {
            PRINT_ERR("Interval can't be 0 or less\r\n");
            break;
        }
        if (NULL == irk)
        {
            PRINT_ERR("NULL pointer\r\n");
            break;
        }
        device.config.renew_dur = interval;
        memcpy(&(device.config.irk[0]), &(irk->irk[0]) , AT_BLE_MAX_KEY_LEN);
        status = at_ble_set_dev_config((at_ble_dev_config_t *)&device.config);
    }
    while (0);
    return status;
}

at_ble_status_t at_ble_set_gap_deviceinfo(at_ble_gap_deviceinfo_t  *gap_deviceinfo)
{
    uint8_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        /* Null Pointer checks*/
        if (!gap_deviceinfo)
        {
            PRINT_ERR("NULL pointer error");
            break;
        }
        /* Param check */
        if (gap_deviceinfo->dev_name_perm  > AT_BLE_WRITE_AUTH)
        {
            PRINT_ERR("Invalid device name write permission");
            break;
        }
        /* save to Internal DB*/
        if (AT_BLE_SUCCESS != at_ble_device_name_set(&device.dev_name.value[0], (uint8_t)device.dev_name.length))
        {
            PRINT_ERR("Invalid Parameters error");
            break;
        }
        device.appearance = (uint8_t)gap_deviceinfo->appearance;
        device.dev_name_write_perm = gap_deviceinfo->dev_name_perm;
        device.spcp_param.con_intv_max = gap_deviceinfo->slv_params.con_intv_max;
        device.spcp_param.con_intv_min = gap_deviceinfo->slv_params.con_intv_min;
        device.spcp_param.con_latency = gap_deviceinfo->slv_params.con_latency;
        device.spcp_param.superv_to = gap_deviceinfo->slv_params.superv_to;
        /*  prepare and Send the permissions to BLE stack */
        device.config.att_cfg.b2AppearancePerm = (uint8_t)device.appearance;
        device.config.att_cfg.b2NamePerm = device.dev_name_write_perm;
        status = at_ble_set_dev_config(&device.config);
        if (AT_BLE_SUCCESS != status)
        {
            PRINT_ERR("set device config failed, status: <%d>", status);
            break;
        }
        /* If we made it tell here then success */
        PRINT_INFO("Device info set success\n");
        status = AT_BLE_SUCCESS;
    }
    while (0);
    FN_OUT(status);
    return (at_ble_status_t)status;
}

at_ble_status_t at_ble_gap_get_peer_deviceinfo(uint16_t conn_handle, at_ble_gapc_get_info_cmd_t *info)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        if (NULL == info)
        {
            break;
        }
        switch (info->operation)
        {
    case AT_BLE_GET_PEER_NAME:
            info->operation =  GAPC_GET_PEER_NAME;
        break;
    case AT_BLE_GET_PEER_APPEARANCE:
            info->operation = GAPC_GET_PEER_APPEARANCE ;
        break;
    case AT_BLE_GET_PEER_SLV_PREF_PARAMS:
            info->operation = GAPC_GET_PEER_SLV_PREF_PARAMS;
        break;
    case AT_BLE_GET_CON_CHANNEL_MAP:
            info->operation = GAPC_GET_CON_CHANNEL_MAP;
        break;
    default:
        status = AT_BLE_INVALID_PARAM;
        PRINT_ERR("Invalid Parameters error");
        break;
    }
    if (AT_BLE_SUCCESS == status)
    {
            gapc_get_info_cmd_handler(conn_handle, info->operation, NULL);
            PRINT_INFO("gap_get_deviceinfo command sent, Operation: <%d>, Connection Handle: <%X> ", info->operation, conn_handle);
    }
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_set_att_config(at_ble_att_cfg_t  *dev_att_cfg)
{
    at_ble_status_t u8Status = AT_BLE_INVALID_PARAM;
    if (NULL != dev_att_cfg)
    {
        device.config.att_cfg.b2Rfu = dev_att_cfg->b2Rfu;
        device.config.att_cfg.b1EnableServiceChanged = dev_att_cfg->b1EnableServiceChanged;
        device.config.att_cfg.b1EnableSpcs = dev_att_cfg->b1EnableSpcs;
        device.config.att_cfg.b2AppearancePerm = dev_att_cfg->b2AppearancePerm;
        device.config.att_cfg.b2NamePerm = dev_att_cfg->b2NamePerm;
        u8Status = at_ble_set_dev_config(&device.config);
    }
    return u8Status;
}

at_ble_status_t at_ble_device_name_set(uint8_t *dev_name, uint8_t len)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    uint8_t i = 0;
    do
    {
        if ((NULL == dev_name) || (0 == len) || (len > AT_BLE_MAX_NAME_LEN))
        {
            PRINT_ERR("Invalid Parameters");
            break;
        }
        /* save the Device name to local database */
        device.dev_name.length = len;
        for (i = 0; i < len; i++)
        {
            device.dev_name.value[i] = dev_name[i];
        }
        status = AT_BLE_SUCCESS;
    }
    while (0);
    return status;
}


at_ble_status_t at_ble_addr_set(at_ble_addr_t *address)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        if ((address->addr == NULL) ||
                (address->type > AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        if ((AT_BLE_ADDRESS_RANDOM_STATIC == address->type) && !(address->addr[0] & address->addr[0] & address->addr[0]))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        memcpy(&(device.config.address.addr[0]), &(address->addr[0]) , AT_BLE_ADDR_LEN);
        device.config.address.type = address->type;
        status = at_ble_set_dev_config((at_ble_dev_config_t *)&device.config);
#ifndef USE_RW_FPGA
        if (AT_BLE_ADDRESS_PUBLIC == address->type)
        {
            status = dbg_le_set_bd_addr_req_handler(&(address->addr[0]));
            if (AT_BLE_SUCCESS != status)
            {
                break;
            }
        }
#else
        device.config.address.type = AT_BLE_ADDRESS_PUBLIC;
#endif
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_addr_get(at_ble_addr_t *address)
{
    at_ble_status_t u8Status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        if (address == NULL)
        {
            break;
        }
        u8Status = (at_ble_status_t)gapm_get_dev_config_cmd_handler(AT_BLE_DEVICE_ADDRESS, (void *)address);
        address->type = device.config.address.type;
    }
    while (0);
    FN_OUT(u8Status);
    return u8Status;
}

at_ble_status_t at_ble_set_dev_config(at_ble_dev_config_t *config)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    at_ble_addr_type_t u8AddrType = config->address.type;
    FN_IN();
    do
    {
        uint8_t att;
        if (NULL == config)
        {
            PRINT_ERR("NULL Pointer\r\n");
            break;
        }
        if (config->address.type > AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE)
        {
            PRINT_ERR("Unknown address type 0x<%2>\r\n", config->address.type);
            break;
        }
        if (config->role >= AT_BLE_ROLE_END)
        {
            PRINT_ERR("Unknown device role 0x<%2>\r\n", config->role);
            status = AT_BLE_INVALID_STATE;
            break;
        }
        att = (config->att_cfg.b2Rfu                 << 6) |
              (config->att_cfg.b1EnableServiceChanged << 5) |
              (config->att_cfg.b1EnableSpcs           << 4) |
              (config->att_cfg.b2AppearancePerm       << 2) |
              (config->att_cfg.b2NamePerm             << 0);
        device.config.address.type = config->address.type;
        switch (config->address.type)
        {
        case AT_BLE_ADDRESS_PUBLIC:
            config->address.type = (at_ble_addr_type_t)GAPM_CFG_ADDR_PUBLIC;
            break;
        case AT_BLE_ADDRESS_RANDOM_STATIC:
            config->address.type = (at_ble_addr_type_t)GAPM_CFG_ADDR_PRIVATE;
            break;
        case AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE:
        case AT_BLE_ADDRESS_RANDOM_PRIVATE_NON_RESOLVABLE:
            config->address.type = (at_ble_addr_type_t)GAPM_CFG_ADDR_PRIVACY;
            break;
        }
        PRINT_INFO("\r\nDevice Configuration (0x):\r\n"
                   "\tRole             :   %02X\r\n"
                   "\tRenew Dur.       : %04X\r\n"
                   "\tAddress Type     :   %02X\r\n"
                   "\tAttributes Value :   %02X\r\n"
                   "\tGAP Start handle : %04X\r\n"
                   "\tGATT Start Handle: %04X\r\n"
                   "\tMTU              : %04X\r\n", config->role, config->renew_dur, config->address.type,
                   att, config->gap_start_hdl, config->gatt_start_hdl, config->max_mtu);
        status = (at_ble_status_t)gapm_set_dev_config_cmd_handler(config->role, config->renew_dur, &(config->address.addr[0]),
                 &config->irk[0], config->address.type, att, config->gap_start_hdl,
                 config->gatt_start_hdl, config->max_mtu);
        config->address.type = u8AddrType;
        device.config = *config;
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_set_channel_map(at_ble_channel_map_t *map)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        if (NULL == map)
        {
            break;
        }
        status = (at_ble_status_t)gapm_set_channel_map_cmd_handler(&(map->map[0]));
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_adv_data_set(uint8_t const *const adv_data,
                                    uint8_t adv_data_len, uint8_t const *const scan_resp_data,
                                    uint8_t scan_response_data_len)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        if ((adv_data == NULL) && (scan_resp_data == NULL))
        {
            status = AT_BLE_FAILURE;
            break;
        }
        if ((adv_data_len > AT_BLE_ADV_MAX_SIZE) ||
                (scan_response_data_len > AT_BLE_ADV_MAX_SIZE))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        if ((adv_data != NULL) && (adv_data_len == 0))
        {
            // Clear advertising data
            memset(device.ADVData, 0, AT_BLE_ADV_MAX_SIZE);
            device.advLen =  0;
        }
        else if ((adv_data != NULL) && (adv_data_len > 0))
        {
            // To do add advertising data check
            memcpy(device.ADVData, adv_data, adv_data_len);
            device.advLen =  adv_data_len;
        }
        if (scan_resp_data != NULL && scan_response_data_len == 0)
        {
            // Clear scan response data
            memset(device.SrData, 0, AT_BLE_ADV_MAX_SIZE);
            device.srLen = 0;
        }
        else if ((scan_resp_data != NULL) && (scan_response_data_len > 0))
        {
            // To do add scan response data check
            memcpy(device.SrData, scan_resp_data, scan_response_data_len);
            device.srLen = scan_response_data_len;
        }
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_set_adv_channel_Map(at_ble_adv_channel_map_t ch)
{
    at_ble_status_t u8Status = AT_BLE_SUCCESS;
    FN_IN();
    switch (ch)
    {
    case AT_BLE_ADV_CHNL_37_EN:
    case AT_BLE_ADV_CHNL_38_EN:
    case AT_BLE_ADV_CHNL_39_EN:
    case AT_BLE_ADV_ALL_CHNLS_EN:
        device.u8AdvChnlMap = ch;
        break;
    default:
        u8Status = AT_BLE_INVALID_PARAM;
        device.u8AdvChnlMap = AT_BLE_ADV_ALL_CHNLS_EN;
        break;
    }
    FN_OUT(u8Status);
    return u8Status;
}

at_ble_status_t at_ble_adv_start(at_ble_adv_type_t type, at_ble_adv_mode_t mode,
                                 at_ble_addr_t *peer_addr, at_ble_filter_type_t filtered,
                                 uint16_t interval, uint16_t timeout, bool disable_randomness)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint8_t adv_type, gap_addr_type;
    uint8_t peer_addr_type;
    uint8_t scan_rsp_len = device.srLen;
    uint16_t ke_timeout = 100 * timeout;
    FN_IN();
    do
    {
        //If the advertising event type is either a discoverable undirected
        //event type or a non-connectable undirected event type, the advInterval shall
        //not be less than 100 ms. If the advertising event type is a connectable undirected
        //event type, the advInterval can be 20 ms or greater.
        if (((type == AT_BLE_ADV_TYPE_DIRECTED) && (timeout != 0))
                || (type > AT_BLE_ADV_TYPE_DIRECTED_LDC)
                || (interval > AT_BLE_ADV_INTERVAL_MAX)
                || ((type == AT_BLE_ADV_TYPE_UNDIRECTED) && (interval < AT_BLE_ADV_INTERVAL_MIN))
                || ((interval < AT_BLE_ADV_NONCON_INTERVAL_MIN) &&
                    ((type >= AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED)))
                || ((type == AT_BLE_ADV_TYPE_DIRECTED) && (peer_addr->addr == NULL)))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        };
        if ((AT_BLE_ADV_FP_ANY != filtered)
                && ((AT_BLE_ADV_GEN_DISCOVERABLE == mode) || (AT_BLE_ADV_LIM_DISCOVERABLE == mode))
           )
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        gap_addr_type = get_gap_local_addr_type();
        if ((peer_addr != NULL) &&
                (peer_addr->type != AT_BLE_ADDRESS_PUBLIC))
        {
            peer_addr_type = 1;
        }
        else
        {
            peer_addr_type = 0;
        }
        switch (type)
        {
        default:
        case  AT_BLE_ADV_TYPE_UNDIRECTED :
            adv_type = GAPM_ADV_UNDIRECT;
            break;
        case  AT_BLE_ADV_TYPE_DIRECTED:
            adv_type = GAPM_ADV_DIRECT;
            break;
        case AT_BLE_ADV_TYPE_DIRECTED_LDC:
            adv_type =  GAPM_ADV_DIRECT_LDC;
            break;
        /* differentiate between them by using scan response length*/
        case AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED:
            scan_rsp_len = 0;
        case AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED:
            adv_type = GAPM_ADV_NON_CONN;
            break;
        }
        status = (at_ble_status_t)gapm_start_adv_cmd_handler(adv_type, gap_addr_type,
                                            (uint16_t)AT_RENEW_DUR_VAL_MIN, peer_addr_type, (uint8_t *)(peer_addr == NULL ? NULL : & (peer_addr->addr)),
                                            interval, interval, ((device.u8AdvChnlMap == 0) ? ((uint8_t)AT_BLE_ADV_ALL_CHNLS_EN) : ((uint8_t)device.u8AdvChnlMap)), mode, filtered,
                                            device.advLen, device.ADVData, scan_rsp_len, device.SrData, ke_timeout, disable_randomness);
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_adv_stop(void)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = gapm_cancel_cmd_handler();
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_scan_start(uint16_t interval, uint16_t window, uint16_t timeout,
                                  at_ble_scan_type_t type , at_ble_scan_mode_t mode, bool filter_whitelist,
                                  bool filter_dublicates)
{
    uint8_t scan_type, gap_addr_type;
    at_ble_addr_t local_addr;
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        // if the scanning window is smaller or equal to the scanning interval
        if ((mode > AT_BLE_SCAN_OBSERVER_MODE)
                || (window > interval)
                || (type > AT_BLE_SCAN_ACTIVE)
                || (window > AT_BLE_SCAN_WINDOW_MAX)
                || (window < AT_BLE_SCAN_WINDOW_MIN)
                || (interval > AT_BLE_SCAN_INTERVAL_MAX)
                || (interval < AT_BLE_SCAN_INTERVAL_MIN))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        };
        if ((filter_whitelist)
                && ((AT_BLE_SCAN_GEN_DISCOVERY == mode) || (AT_BLE_SCAN_LIM_DISCOVERY == mode))
           )
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        gap_addr_type = get_gap_local_addr_type();
        if (type == AT_BLE_SCAN_PASSIVE)
        {
            scan_type = GAPM_SCAN_PASSIVE;
        }
        else
        {
            scan_type = GAPM_SCAN_ACTIVE;
        }
        gapm_start_scan_cmd_handler(scan_type, gap_addr_type,
                                    device.config.renew_dur,
                                    local_addr.addr, interval, window, mode,
                                    filter_whitelist, filter_dublicates, (100 * timeout));
    }
    while (0);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_scan_stop(void)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = gapm_cancel_cmd_handler();
    if (AT_BLE_SUCCESS == status)
    {
        at_ble_event_flush(GAPM_ADV_REPORT_IND);
    }
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_set_conn_mode(at_ble_conn_mode_t mode)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    if ((mode < AT_BLE_CONNECTION_DIRECT) || (mode > AT_BLE_CONNECTION_AUTO))
    {
        PRINT_ERR("Invalid Con. Mode <%d>\r\n", mode);
        status = AT_BLE_INVALID_PARAM;
    }
    else
    {
        device.conMode = mode;
    }
    FN_OUT(status);
    return status;
}

/*
 * TODO:
 *  1. Check for return valaue if operation mode is not Centeral or All, also with scaning and advertising
 *  2. Make an array of device's connections
 *
 * Read about:
 *  1. connection handle
 *  2. connections data; scan interval, scan window, con. latency, ....
 *
 */
at_ble_status_t at_ble_connect(at_ble_addr_t peers[], uint8_t peer_count,
                               uint16_t scan_interval, uint16_t scan_window, at_ble_connection_params_t *connection_params)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    at_ble_addr_t local_addr;
    uint8_t gatt_dev_addr;
    FN_IN();
    do
    {
        if (AT_BLE_MAX_CONCURRENT_CONNS <= check_ConnData_emptyEntry())
        {
            PRINT_ERR("No Empty Entry for connection\r\n");
            status = AT_BLE_GAP_INSUFF_RESOURCES;
            break;
        }
        /* check for the range validity of the values */
        if ((connection_params->con_intv_max < AT_CNX_INTERVAL_MIN ||
                connection_params->con_intv_max > AT_CNX_INTERVAL_MAX) ||
                (connection_params->con_intv_min < AT_CNX_INTERVAL_MIN ||
                 connection_params->con_intv_min > AT_CNX_INTERVAL_MAX) ||
                (connection_params->superv_to  < AT_CNX_SUP_TO_MIN   ||
                 connection_params->superv_to  > AT_CNX_SUP_TO_MAX)   ||
                (connection_params->con_latency  > AT_CNX_LATENCY_MAX))
        {
            PRINT_ERR("Invalid interval\r\n");
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        status = at_ble_addr_get(&local_addr);
        if (status != AT_BLE_SUCCESS)
        {
            PRINT_ERR("Unable to get address\r\n");
            status = AT_BLE_FAILURE;
            break;
        }
        /*Own BD address source of the device */
        gatt_dev_addr = get_gap_local_addr_type();
        switch (device.conMode)
        {
        case AT_BLE_CONNECTION_DIRECT:
        case AT_BLE_CONNECTION_AUTO:
            break;
        default:
            PRINT_INFO("Set default mode\r\n");
            device.conMode = AT_BLE_CONNECTION_AUTO;
            break;
        }
        gapm_start_connection_cmd_handler((((uint8_t)(device.conMode) - 1) + GAPM_CONNECTION_DIRECT), gatt_dev_addr,
                                          device.config.renew_dur, local_addr.addr, scan_interval, scan_window,
                                          connection_params->con_intv_min, connection_params->con_intv_max,
                                          connection_params->con_latency,
                                          connection_params->superv_to,
                                          connection_params->ce_len_min,
                                          connection_params->ce_len_max,
                                          peer_count, peers);
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_disconnect(at_ble_handle_t handle, at_ble_disconnect_reason_t reason)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = (at_ble_status_t)gapc_disconnect_cmd_handler(reason, handle);
    at_ble_conn_flush(handle);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_whitelist_add(at_ble_addr_t *address)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        if (NULL == address ||
                (address->type != AT_BLE_ADDRESS_PUBLIC &&
                 address->type != AT_BLE_ADDRESS_RANDOM_STATIC))
        {
            status = AT_BLE_INVALID_PARAM;
            PRINT_ERR("0x%02X\n", status);
            break;
        }
        status = (at_ble_status_t)gapm_white_list_mgm_cmd(GAPM_ADD_DEV_IN_WLIST, address->type, address->addr);
    }
    while (0);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_whitelist_remove(at_ble_addr_t *address)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        if (NULL == address ||
                (address->type != AT_BLE_ADDRESS_PUBLIC &&
                 address->type != AT_BLE_ADDRESS_RANDOM_STATIC))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        status = (at_ble_status_t)gapm_white_list_mgm_cmd(GAPM_RMV_DEV_FRM_WLIST, address->type, address->addr);
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_whitelist_clear(void)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = (at_ble_status_t)gapm_white_list_mgm_cmd(GAPM_CLEAR_WLIST, 0, NULL);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_get_whitelist_size(uint8_t *size)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        if (NULL == size)
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        status = (at_ble_status_t)gapm_white_list_mgm_cmd(GAPM_GET_WLIST_SIZE, 0, size);
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_connection_param_update(at_ble_handle_t handle,
        at_ble_connection_params_t *connection_params)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        /* check for connection_params validity */
        if (NULL == connection_params ||
                connection_params->con_intv_max < AT_CNX_INTERVAL_MIN ||
                connection_params->con_intv_max > AT_CNX_INTERVAL_MAX ||
                connection_params->con_intv_min < AT_CNX_INTERVAL_MIN ||
                connection_params->con_intv_min > AT_CNX_INTERVAL_MAX ||
                connection_params->con_latency > AT_CNX_LATENCY_MAX ||
                connection_params->superv_to < AT_CNX_SUP_TO_MIN ||
                connection_params->superv_to > AT_CNX_SUP_TO_MAX)
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        status = gapc_param_update_cmd_handler(handle,
                                               connection_params->con_intv_min,
                                               connection_params->con_intv_max,
                                               connection_params->con_latency,
                                               connection_params->superv_to,
                                               connection_params->ce_len_min,
                                               connection_params->ce_len_max);
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_conn_update_reply(at_ble_handle_t handle,
        bool accept,
        uint16_t ce_len_min,
        uint16_t ce_len_max)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    status = gapc_param_update_cfm_handler(handle,
                                           accept,
                                           ce_len_min,
                                           ce_len_max);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_random_address_resolve(uint8_t u8NoOfKeys, at_ble_addr_t *paddrPeerAddress, uint8_t *pu8IrKey)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    do
    {
        if ((0 == u8NoOfKeys) || (NULL == paddrPeerAddress) || (NULL == pu8IrKey) ||
                (paddrPeerAddress->type != AT_BLE_ADDRESS_RANDOM_PRIVATE_RESOLVABLE))
        {
            status = AT_BLE_INVALID_PARAM;
            break;
        }
        gapm_resolv_addr_cmd_handler(u8NoOfKeys, paddrPeerAddress->addr, pu8IrKey);
    }
    while (0);
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_send_sec_config(at_ble_signature_info_t *signature_info,
                                       at_ble_auth_t authen,
                                       bool service_changed)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        if (NULL == signature_info)
        {
            break;
        }
        switch (authen)
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
            PRINT_ERR("Invalid Auth Type %d\r\n", authen);
            break;
        }
        status = (at_ble_status_t)gapc_connection_cfm_handler((uint8_t *) & (signature_info->stLocalCsrk.key[0]),
                                             signature_info->stSignCounter.u32LocalCounter,
                                             (uint8_t *) & (signature_info->stPeerCsrk.key[0]),
                                             signature_info->stSignCounter.u32PeerCounter,
                                             authen, service_changed, signature_info->stSignCounter.stConnHandle);
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_connect_cancel(void)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    FN_IN();
    if (gapm_cancel_cmd_handler())
    {
        status = AT_BLE_FAILURE;
    }
    FN_OUT(status);
    return status;
}

at_ble_status_t at_ble_rx_power_get(at_ble_handle_t conn_handle, int8_t *rx_power)
{
    at_ble_status_t status = AT_BLE_INVALID_PARAM;
    FN_IN();
    do
    {
        if (NULL == rx_power)
        {
            break;
        }
        status = (at_ble_status_t)gapc_get_info_cmd_handler(conn_handle, GAPC_GET_CON_RSSI, (uint8_t *)rx_power);
        if (*((int8_t *)rx_power) > 127)
        {
            *((int8_t *)rx_power) = (*((int8_t *)rx_power) + 1) & 0xFF;
    }
    }
    while (0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
at_ble_status_t at_ble_tx_power_set(at_ble_tx_power_level_t power)
    {
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint32_t level = (uint32_t) power  & 0x1F;
    FN_IN();
    status = (at_ble_status_t)dbg_wr_mem_req_handler(PWR_REG_ADDR, (uint8_t *)&level, 4, 32, 0);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
    }
at_ble_status_t at_ble_tx_power_get(at_ble_tx_power_level_t *power)
{
    at_ble_status_t status = AT_BLE_SUCCESS;
    uint32_t level = 0;
    FN_IN();
    status = (at_ble_status_t)dbg_rd_mem_req_handler(PWR_REG_ADDR, (uint8_t *)&level, 1, 32);
    *power = (at_ble_tx_power_level_t) (level & 0x1F);
    ASSERT_PRINT_ERR(AT_BLE_SUCCESS != status, "Status : 0x%02X\n", status);
    FN_OUT(status);
    return status;
}
/* utility functions, might be removed later*/
uint8_t at_ble_uuid_type2len(at_ble_uuid_type_t type)
{
    switch (type)
    {
    case AT_BLE_UUID_16 :
        return AT_BLE_UUID_16_LEN;
    case AT_BLE_UUID_32 :
        return AT_BLE_UUID_32_LEN;
    case AT_BLE_UUID_128 :
        return AT_BLE_UUID_128_LEN;
    default:
        return AT_BLE_INVALID_PARAM;
    }
}

at_ble_uuid_type_t at_ble_uuid_len2type(uint8_t len)
{
    switch (len)
    {
    case AT_BLE_UUID_16_LEN:
        return AT_BLE_UUID_16;
    case AT_BLE_UUID_32_LEN :
        return AT_BLE_UUID_32;
    case AT_BLE_UUID_128_LEN :
        return AT_BLE_UUID_128;
    default:
        return AT_BLE_UUID_INVALID;
    }
}
