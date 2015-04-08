
#include "compiler.h"
#include "string.h"

#include "widbg_mgr_client.h"
#include "uip.h"
#include "uip-ds6-route.h"

node_address_t common_server_address = {
	.extended_addr = 0x1122334455667788,
	.native_addr = 0x0000
};

node_address_t upgrade_server_address = {
	.extended_addr = 0x1122334455667788,
	.native_addr = 0x0000
};

node_address_t debug_server_address = {
	.extended_addr = 0x1122334455667788,
	.native_addr = 0x0000
};

//uint16_t parent_native_addr;

void configure_common_server_details(addr_mode_t addr_mode, uint8_t *addr)
{
	if (NATIVE_ADDR_MODE == addr_mode)
	{
		memcpy(&common_server_address.native_addr, addr, NATIVE_ADDR_SIZE);
	}
	else if (EXTENDED_ADDR_MODE == addr_mode)
	{
		memcpy(&common_server_address.extended_addr, addr, EXTENDED_ADDR_SIZE);
	}
}


void configure_upgrade_server_details(addr_mode_t addr_mode, uint8_t *addr)
{
	if (NATIVE_ADDR_MODE == addr_mode)
	{
		memcpy(&upgrade_server_address.native_addr, addr, NATIVE_ADDR_SIZE);
	}
	else if (EXTENDED_ADDR_MODE == addr_mode)
	{
		memcpy(&upgrade_server_address.extended_addr, addr, EXTENDED_ADDR_SIZE);
	}
}

void configure_debug_server_details(addr_mode_t addr_mode, uint8_t *addr)
{
	if (NATIVE_ADDR_MODE == addr_mode)
	{
		memcpy(&debug_server_address.native_addr, addr, NATIVE_ADDR_SIZE);
	}
	else if (EXTENDED_ADDR_MODE == addr_mode) 
	{
		memcpy(&debug_server_address.extended_addr, addr, EXTENDED_ADDR_SIZE);
	}
}

void get_common_server_details(addr_mode_t addr_mode, uint8_t *addr)
{
	if (NATIVE_ADDR_MODE == addr_mode)
	{
		addr = (uint8_t *)&common_server_address.native_addr;
		//memcpy(addr, &common_server_address.native_addr, NATIVE_ADDR_SIZE);
	}
	else if (EXTENDED_ADDR_MODE == addr_mode)
	{
		addr = (uint8_t *)&common_server_address.extended_addr;
		//memcpy(addr, &common_server_address.extended_addr, EXTENDED_ADDR_SIZE);
	}
}

void get_upgrade_server_details(addr_mode_t addr_mode, uint8_t *addr)
{
	if (NATIVE_ADDR_MODE == addr_mode)
	{
		addr = (uint8_t *)&upgrade_server_address.native_addr;
		//memcpy(addr, &upgrade_server_address.native_addr, NATIVE_ADDR_SIZE);
	}
	else if (EXTENDED_ADDR_MODE == addr_mode)
	{
		addr = (uint8_t *)&upgrade_server_address.extended_addr;
		//memcpy(addr, &upgrade_server_address.extended_addr, EXTENDED_ADDR_SIZE);
	}
}

void get_debug_server_details(addr_mode_t addr_mode, uint8_t *addr)
{
	if (NATIVE_ADDR_MODE == addr_mode)
	{
		addr = (uint8_t *)&debug_server_address.native_addr;
		//memcpy(addr, &upgrade_server_address.native_addr, NATIVE_ADDR_SIZE);
	}
	else if (EXTENDED_ADDR_MODE == addr_mode)
	{
		addr = (uint8_t *)&debug_server_address.extended_addr;
		//memcpy(addr, &upgrade_server_address.extended_addr, EXTENDED_ADDR_SIZE);
	}
}

uint8_t *get_parent_native_addr(void)
{
	uip_ipaddr_t *ipaddr;
	ipaddr = uip_ds6_defrt_choose();
	return &ipaddr->u16[4];
}
