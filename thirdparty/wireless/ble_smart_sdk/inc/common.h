#ifndef __COMMON_H
#define __COMMON_H
#include <stdint.h>

#define REG_WR(addr, value)      (*(volatile unsigned int *)(addr)) = (value)
#define REG_RD(addr)             (*(volatile unsigned int *)(addr))

enum INTC_STATUS
{
    /// INTC status OK
    INTC_OK = 0x00,
		///	INTC Invalid IRQ number
		INTC_INVALID_IRQ,
		/// INTC Invalid IRQ handler
		INTC_INVALID_ISR,
    /// INTC Error Register ISR
    INTC_ISR_ALREADY_REGISTERED,
};

#ifndef CHIPVERSION_B0
#define TASK_INTERNAL_APP	0x3E
#define APP_FIRST_MSG(event_index)	((event_index << 10))
#else
#define TASK_INTERNAL_APP	0x05
#define APP_FIRST_MSG(event_index)	((event_index << 8))
#endif

#define BUILD_INTR_SRCID(callback_index,intr_index)	((callback_index << 8)|(intr_index))

enum internal_app_msg_id
{
	PERIPHERAL_INTERRUPT_EVENT = APP_FIRST_MSG(TASK_INTERNAL_APP),
};

#define BUILD_INTR_EVENT_MSG(msg,src_id,param,param_len)	\
			msg->id = PERIPHERAL_INTERRUPT_EVENT;								\
			msg->dest_id = TASK_INTERNAL_APP;										\
			msg->src_id = src_id;																\
			msg->param_len = param_len;													\
			if(param_len > 0)																		\
				memcpy(msg->param,param,param_len);
/**
 * \brief Registers an ISR for an IRQ.
 *
 * Registers an ISR into the vector table for the given IRQ index that are usable by
 * SAMB11 applications.
 *
 * \param[in]	isr_index  index into the vector table
 * \param[in]	fp  			 Pointer to the ISR routine.
 *
 */
extern uint8_t (*platform_register_isr)(uint8_t isr_index,void *fp);

/**
 * \brief Unregisters an ISR for an IRQ.
 *
 * Unregisters an ISR from the vector table for the given IRQ index that are usable by
 * SAMB11 applications.
 *
 * \param[in]	isr_index  index into the vector table
 *
 */
extern uint8_t (*platform_unregister_isr)(uint8_t isr_index);

//#ifdef CHIPVERSION_B0
extern void (*handle_ext_wakeup_isr)(void);
void PORT1_COMB_Handler(void);
//#endif	//CHIPVERSION_B0

void gpio1_combined_isr_handler(void);
void gpio0_combined_isr_handler(void);

#endif	//__COMMON_H
