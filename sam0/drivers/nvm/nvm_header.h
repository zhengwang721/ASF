#ifndef NVM_HEADER_H_INCLUDED
#define NVM_HEADER_H_INCLUDED
#include <stdint.h>

#define __I  volatile const
#define __O  volatile
#define __IO volatile

#define Assert(check)

/* Default register macro for sub access of 32-bit registers */
#define REG32(name, type) union { \
	type volatile uint32_t name; \
	type volatile uint16_t name ## _UINT16_T[2]; \
	type volatile uint8_t name ## _UINT8_T[4]; \
}

/* Default register macro for sub access of 16-bit registers */
#define REG16(name, type) union { \
	type volatile uint16_t name; \
	type volatile uint8_t name ## _UINT8_T[2]; \
}

/* Default register definition for 8-bit registers */
#define REG8(name, type) type volatile uint8_t name


#define NVM_CMDEX_EXECUTION_KEY 0xA5

/*
enum NVM_COMMAND_enum {
	NVM_CMD_ER_gc  = (0x02 << 0x00);
	NVM_CMD_WP_gc  = (0x04 << 0x00);
	NVM_CMD_EAR_gc = (0x05 << 0x00);
	NVM_CMD_WAP_gc = (0x06 << 0x00);
	NVM_CMD_WL_gc = (0x0f << 0x00);
	NVM_CMD_LR_gc = (0x40 << 0x00);
	NVM_CMD_UR_gc = (0x41 << 0x00);
	NVM_CMD_SPRM_gc = (0x42 << 0x00);
	NVM_CMD_CPRM_gc = (0x43 << 0x00);
	NVM_CMD_PBC_gc = (0x44 << 0x00);
	NVM_CMD_SSB_gc = (0x45 << 0x00);
} NVM_COMMAND_t;
*/

#define NVM_CMD_ER_gc     (0x02 << 0x00)
#define NVM_CMD_WP_gc     (0x04 << 0x00)
#define NVM_CMD_EAR_gc    (0x05 << 0x00)
#define NVM_CMD_WAP_gc    (0x06 << 0x00)
#define NVM_CMD_WL_gc     (0x0f << 0x00)
#define NVM_CMD_LR_gc     (0x40 << 0x00)
#define NVM_CMD_UR_gc     (0x41 << 0x00)
#define NVM_CMD_SPRM_gc   (0x42 << 0x00)
#define NVM_CMD_CPRM_gc   (0x43 << 0x00)
#define NVM_CMD_PBC_gc    (0x44 << 0x00)
#define NVM_CMD_SSB_gc    (0x45 << 0x00)

#define NVM_WAKEUP_gp         8
#define NVM_WAKEONACCESS_gc  (0x00 << NVM_WAKEUP_gp)
#define NVM_WAKEUPINSTANT_gc (0x01 << NVM_WAKEUP_gp)
#define NVM_DISABLED_gc      (0x11 << NVM_WAKEUP_gp)

#define NVM_AUTO_WAITSTATES_DISABLED_bm 0x00


#define NVM_SLEEP_ENABLE_gp     8
#define NVM_MAN_PAGE_WRITE_bp   7
#define NVM_MAN_PAGE_WRITE_bm   0x00000008
#define NVM_AUTO_WAIT_STATE_bp  1
#define NVM_WAIT_STATES_gp      0
#define NVM_SECURITY_BIT_bm     0x0100
#define NVM_AUX_ADDRESS         0x00200000
#define NVM_AUX_BOOTPROT_gp     1
#define NVM_AUX_EEPROM_gp       4


#define NVM_COMMAND_gp          0
#define NVM_CMDEX_gp            8

#define NVM_PSZ_gm              0x00070000
#define NVM_NVMP_gm             0x00001111
#define NVM_PSZ_gp              16
#define NVM_NVMP_gp             0
#define NVM_INTFLAG_READY_bm    0x0001

#define NVM_COMMAND_KEY 0xA5
#define NVM_ERRORS_gp   2



typedef struct {
		REG16(CTRLA, __IO);
		__I uint16_t reserved_0x02;
		REG16(CTRLB, __IO);
		__I uint16_t reserved_0x06;
		REG32(PARAM, __IO);
		REG8(INTENCLR, __IO);
		REG8(INTENSET, __IO);
		REG8(INTFLAG, __IO);
		__I uint8_t reserved_0x0f;
		REG16(STATUS, __IO);
		__I uint16_t reserved_0x12;
		REG32(ADDR, __IO);
		REG16(LOCK, __IO);
		__I uint16_t reserved_0x1a;
} NVM_t;

#define NVM (*( NVM_t * ) 0x20000000)

#endif // NVM_HEADER_H_INCLUDED
