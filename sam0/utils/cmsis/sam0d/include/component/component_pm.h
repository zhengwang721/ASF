#ifndef PM_HEADER_H_INCLUDED
#define PM_HEADER_H_INCLUDED
/* SYSCTRL Register Map */
typedef struct {
	__REG8(CTRL, __IO);
	__REG8(SLEEP, __IO);
	__REG8(RESERVED_0x2, __I);
	__REG8(RESERVED_0x3, __I);
	__REG8(RESERVED_0x4, __I);
	__REG8(RESERVED_0x5, __I);
	__REG8(RESERVED_0x6, __I);
	__REG8(RESERVED_0x7, __I);
	__REG8(CPUSEL, __IO);
	__REG8(APBASEL, __IO);
	__REG8(APBBSEL, __IO);
	__REG8(APBCSEL, __IO); // reserved ?
	__REG8(APBDSEL, __IO); // reserved ?
	__REG8(RESERVED_0xD, __I);
	__REG8(RESERVED_0xE, __I);
	__REG8(RESERVED_0xF, __I);
	__REG32(CPUMASK, __IO);
	__REG32(AHBMASK, __IO);
	__REG32(APBAMASK, __IO);
	__REG32(APBBMASK, __IO);
	__REG32(APBCMASK, __IO); //reserved ?
	__REG32(APBDMASK, __IO);
	__REG8(RESERVED_0x28, __I);
	__REG8(RESERVED_0x29, __I);
	__REG8(RESERVED_0x2A, __I);
	__REG8(RESERVED_0x2B, __I);
	__REG8(RESERVED_0x2C, __I);
	__REG32(RESERVED_0x30, __I); // Reserved ? crossed out in datasheet (SLEEPWALKCTRL)
	__REG8(INTENCRL, __O);
	__REG8(INTENSET, __O);
	__REG8(INTFLAG, __I);
	__REG8(RESERVED_0X37, __I);
	__REG8(RCAUSE, __I);
} PM_t;

#define PM_CFDEN_bm 0
#define PM_MCSEL_gm 0
#define PM_MCSEL_gp 0

#endif /* PM_HEADER_H_INCLUDED */
