#ifndef __DEVICE_H__
#define __DEVICE_H__

/// Constant defining the role
typedef enum
{
    ///Master role
    ROLE_MASTER,
    ///Slave role
    ROLE_SLAVE,
    ///Enumeration end value for role value check
    ROLE_END
}at_ble_dev_role;

struct device_info
{
	// local device adv and scan data
    uint8_t ADVData[AT_BLE_ADV_MAX_SIZE];
    uint8_t SrData[AT_BLE_ADV_MAX_SIZE];
    uint8_t advLen;
    uint8_t srLen;

	// handle when the device is in a slave connection
	at_ble_handle_t conn_handle;

	// local device bt address
	bool at_addr_set;
	at_ble_addr_t at_dev_addr;

	// peer device address
	at_ble_addr_t peer_addr;

	// the role in which the device is running in now
	at_ble_dev_role role;

	// device local keys
	at_ble_LTK_t ltk; 
	at_ble_CSRK_t csrk; 
	at_ble_IRK_t irk;

};

extern struct device_info device;

#endif
