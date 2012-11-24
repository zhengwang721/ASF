#ifndef _SYSTEM_CPU_H_
#define _SYSTEM_CPU_H_

enum system_cpu_status_flag {
	SYSTEM_CPU_STATUS_PENDING_NMI,
	SYSTEM_CPU_STATUS_PENDING_SV,
	SYSTEM_CPU_STATUS_PENDING_ST,
	SYSTEM_CPU_STATUS_PENDING_ISR,
};

enum system_cpu_interrupt_flag {
	SYSTEM_CPU_INTERRUPT_SYSTICK,
};




uint8_t system_interrupt_get_active_vector(void);
uint8_t system_interrupt_get_pending_vector(void);

#endif
