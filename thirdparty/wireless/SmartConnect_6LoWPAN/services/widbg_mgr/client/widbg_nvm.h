



#ifndef WIDBG_NVM_H
#define WIDBG_NVM_H

#include "widbg.h"

void widbg_nvm_init(void);
void widbg_nvm_write(address_type_t addr_type, uint32_t addr, uint16_t size, uint8_t *content);
void widbg_nvm_read(address_type_t addr_type, uint32_t addr, uint16_t size, uint8_t *content);
void widbg_nvm_swap_image(address_type_t addr_type, uint32_t start_addr, uint32_t size);

#endif /* WIDBG_NVM_H */