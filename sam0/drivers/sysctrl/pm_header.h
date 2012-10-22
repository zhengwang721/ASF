#ifndef PM_HEADER_H_INCLUDED
#define PM_HEADER_H_INCLUDED
/** \cond HEADERFILES */
#include <stdint.h>
#define __I  volatile const
#define __O  volatile
#define __IO volatile

/* Default register macro for sub access of 32-bit registers */
#define REG32(name, type) union { \
	type uint32_t name; \
	type uint16_t name ## _UINT16_T[2]; \
	type uint8_t name ## _UINT8_T[4]; \
}

/* Default register macro for sub access of 16-bit registers */
#define REG16(name, type) union { \
	type uint16_t name; \
	type uint8_t name ## _UINT8_T[2]; \
}

/* Default register definition for 8-bit registers */
#define REG8(name, type) type uint8_t name


/* SYSCTRL Register Map */
typedef struct {
	REG8(CTRL, __IO);
	REG8(SLEEP, __IO);
	REG8(RESERVED_0x2, __I);
	REG8(RESERVED_0x3, __I);
	REG8(RESERVED_0x4, __I);
	REG8(RESERVED_0x5, __I);
	REG8(RESERVED_0x6, __I);
	REG8(RESERVED_0x7, __I);
	REG8(CPUSEL, __IO);
	REG8(APBASEL, __IO);
	REG8(APBBSEL, __IO);
	REG8(APBCSEL, __IO); // reserved ?
	REG8(APBDSEL, __IO); // reserved ?
	REG8(RESERVED_0xD, __I);
	REG8(RESERVED_0xE, __I);
	REG8(RESERVED_0xF, __I);
	REG32(CPUMASK, __IO);
	REG32(AHBMASK, __IO);
	REG32(APBAMASK, __IO);
	REG32(APBBMASK, __IO);
	REG32(APBCMASK, __IO); //reserved ?
	REG32(APBDMASK, __IO);
	REG8(RESERVED_0x28, __I);
	REG8(RESERVED_0x29, __I);
	REG8(RESERVED_0x2A, __I);
	REG8(RESERVED_0x2B, __I);
	REG8(RESERVED_0x2C, __I);
	REG32(RESERVED_0x30, __I); // Reserved ? crossed out in datasheet (SLEEPWALKCTRL)
	REG8(INTENCRL, __O);
	REG8(INTENSET, __O);
	REG8(INTFLAG, __I);
	REG8(RESERVED_0X37, __I);
	REG8(RCAUSE, __I);
} PM_t;


#define PM (*( PM_t * ) 0x43000000)
/** \endcond */

#endif /* PM_HEADER_H_INCLUDED */
