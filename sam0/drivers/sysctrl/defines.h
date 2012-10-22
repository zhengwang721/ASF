#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include "sysctrl_header.h"
#include "pm_header.h"
typedef uint8_t bool;

#include <stdint.h>
#define true 1
#define false 0



/*typedef struct EVSYS_struct{
	__REG16_ARRAY(CHSTATUS_UINT16_T, 16, __IO);
	__REG8(CHCTRL, __IO);
	__REG16(CLKCTRL, __IO);
	__REG32(GENCTRL, __IO);
	__REG32(GENDIV, __IO);
	} EVSYS_t;
#define EVSYS (*(EVSYS_t*) 0x4000)
*/

#define PORTA (*(PORT_t*) 0x4000)
#define EICA (*(EIC_t*) 0x4000)

#define EVSYS_CHSTATUS_CHBUSY0_bp 0
#define EVSYS_CHSTATUS_USRREADY0_bp 0
#define EVSYS_CHCTRL_CHSEL_gp 0
#define EVSYS_CHCTRL_STROBE_bm 0

#define SYSCTRL_MODE_bm 0
#define SYSCTRL_LLAW_bm 0
#define SYSCTRL_FINE_bm 0
#define SYSCTRL_CCDIS_bm 0
#define SYSCTRL_PRESC_bm 0
#define SYSCTRL_DFLLRDY_bm 0
#define SYSCTRL_SYNC_bm 0
#define SYSCTRL_OSC32KRDY_bm 0
#define SYSCTRL_CALIB_gp 0
#define SYSCTRL_FRANGE_gp 0
#define SYSCTRL_PRESC_gp 0
#define SYSCTRL_XTALEN_bm 0
#define SYSCTRL_AMPGC_bm 0
#define SYSCTRL_AAMPEN_bm 0
#define SYSCTRL_EN1K_bm 0
#define SYSCTRL_EN32K_bm 0
#define SYSCTRL_XOSCCRDY_bm 0
#define SYSCTRL_DFLL_ENABLE_bm 0
#define SYSCTRL_XOSC32KRDY_bm 0
#define SYSCTRL_XOSC32K_ENABLE_bm 0
#define SYSCTRL_XOSC_ENABLE_bm 0
#define SYSCTRL_OSC32K_ENABLE_bm 0
#define SYSCTRL_XOSC32K_ENABLE_bm 0
#define SYSCTRL_FIN_gp 0
#define SYSCTRL_DFLL_CLOSED_LOOP 0
#define SYSCTRL_FSTEP_gp 0
#define SYSCTRL_RC8MHZ_ENABLE 0
#define SYSCTRL_RC8MHZ_ENABLE_bm 0
#define SYSCTRL_XOSCRDY_bm 0
#define SYSCTRL_BOD33CTRL_ACTION_gp 0
#define SYSCTRL_BOD33CTRL_MODE_bp 0
#define SYSCTRL_BOD33CTRL_CEN_bm 0
#define SYSCTRL_BOD33CTRL_HYST_bm 0
#define SYSCTRL_BOD33CTRL_ENABLE_bm 0
#define SYSCTRL_ACTION_RESET_gc 0
#define SYSCTRL_ACTION_INTERRUPT_gc 0
#define SCR_SLEEPDEEP_bm 0

#define CONF_CLOCK_TIMEOUT 0
#define PM_CFDEN_bm 0
#define PM_MCSEL_gm 0
#define PM_MCSEL_gp 0

#define SCR_SLEEPDEPP_bm 0

#define CLKSOURCE_RC32KHZ 0




#endif
