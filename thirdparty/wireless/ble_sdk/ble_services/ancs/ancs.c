

#include "at_ble_api.h"

#include "platform.h"
#include "anp_client.h"
#include "ancs.h"




void ancs_init(ancs_prf_t *ancs_data)
{
	memset(ancs_data, 0, sizeof(ancs_prf_t));
}


void ancs_enable(ancs_prf_t *anp_data, at_ble_handle_t handle)
{
	anp_data->ancs_serv.conn_handle = handle;
	anp_data->ancs_serv.service_uuid.type = AT_BLE_UUID_128;
	memcpy(&anp_data->ancs_serv.service_uuid.uuid[0], ANCS_SERV_UUID, UUID_128BIT_LEN);
}





