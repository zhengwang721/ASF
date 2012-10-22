#ifndef SYSCTRL_HEADER_H_INCLUDED
#define SYSCTRL_HEADER_H_INCLUDED
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
	REG32(INTENCLR, __O);
	REG32(INTENSET, __O);
	REG32(INTFLAG, __I);
	REG32(PCLKSR, __IO);
	REG16(XOSC, __IO);
	REG8(RESERVED_0X12, __I);
	REG8(RESERVED_0X13, __I);
	REG16(XOSC32K, __IO);
	REG8(XOSC32KTEST, __IO); // Not external ?
	REG8(RESERVED_0X17, __I);
	REG16(DFLLCTRL, __IO);
	REG8(RESERVED_0X1A, __I);
	REG8(RESERVED_0X1B, __I);
	REG16(DFLLVAL, __IO);
	REG16(DFLLDIFF, __IO);
	REG16(DFLLMUL, __IO);
	REG16(DFLLSTEP, __IO);
	REG8(DFLLSYNC, __I);
	REG8(RESERVED_0X25, __I);
	REG8(RESERVED_0X26, __I);
	REG8(RESERVED_0X27, __I);
	REG16(BOD33CTRL, __IO);
	REG16(BOD33LEVEL, __IO);
	REG16(BOD12CTRL, __IO);
	REG16(BOD12LEVEL, __IO);
	REG32(OSC32K, __IO);
	REG32(OSCULP32K, __IO);
	REG8(OSCULP32KTEST, __IO); // Probably internal
	REG32(OSC8M, __IO);
	REG16(VREGCTRL, __IO);
	REG8(RESERVED_0X3E, __I);
	REG8(RESERVED_0X3F, __I);
	REG32(VREGTEST, __IO); // Internal
	REG16(VREFCTRLA, __IO);
	REG16(VREFCTRLB, __IO);
	REG32(VREFTESTA, __IO); // INTERNAL
	REG16(VREFTESTB, __IO); // INTERNAL
	REG8(RESERVED_0X4E, __I);
	REG8(RESERVED_0X4F, __I);
	REG32(TESTMODE, __IO);
} SYSCTRL_t;


#define SYSCTRL (*( SYSCTRL_t * ) 0x42000000)

/** \endcond */

#endif /* SYSCTRL_HEADER_H_INCLUDED */
