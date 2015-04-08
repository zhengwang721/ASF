
#ifndef WIDBG_MGR_CLIENT_H
#define WIDBG_MGR_CLIENT_H

#include "widbg.h"
#include "widbg_mgr.h"

extern node_address_t common_server_address;
extern node_address_t upgrade_server_address;

void configure_common_server_details(addr_mode_t addr_mode, uint8_t *addr);
void get_common_server_details(addr_mode_t addr_mode, uint8_t *addr);
void configure_upgrade_server_details(addr_mode_t addr_mode, uint8_t *addr);
void get_upgrade_server_details(addr_mode_t addr_mode, uint8_t *addr);
void configure_debug_server_details(addr_mode_t addr_mode, uint8_t *addr);
void get_debug_server_details(addr_mode_t addr_mode, uint8_t *addr);
uint8_t *get_parent_native_addr(void);
#endif /* WIDBG_MGR_CLIENT_H */