#ifndef SYSTEM_INTERRUPT_H_INCLUDED
#define SYSTEM_INTERRUPT_H_INCLUDED

#include <compiler.h>

enum system_interrupt_vector {
	SYSTEM_INTERRUPT_NON_MASKABLE      = NonMaskableInt_IRQn,
	SYSTEM_INTERRUPT_HARD_FAULT        = HardFault_IRQn,
	SYSTEM_INTERRUPT_MEMORY_MANAGEMENT = MemoryManagement_IRQn,
	SYSTEM_INTERRUPT_BUS_FAULT         = BusFault_IRQn,
	SYSTEM_INTERRUPT_USER_FAULT        = UsageFault_IRQn,
	SYSTEM_INTERRUPT_SV_CALL           = SVCall_IRQn,
	SYSTEM_INTERRUPT_DEBUG_MONITOR     = DebugMonitor_IRQn,
	SYSTEM_INTERRUPT_PENDING_SV        = PendSV_IRQn,
	SYSTEM_INTERRUPT_SYSTICK           = SysTick_IRQn,

	SYSTEM_INTERRUPT_MODULE_PM         = PM_CFD_IRQn,
	SYSTEM_INTERRUPT_MODULE_SYSCTRL    = SYSCTRL_BOD12DET_IRQn,
	SYSTEM_INTERRUPT_MODULE_WDT        = WDT_EW_IRQn,
	SYSTEM_INTERRUPT_MODULE_RTC        = RTC_CMP_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_EIC        = EIC_EXTINT_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_NVMCTRL    = NVMCTRL_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_EVSYS      = EVSYS_EVD_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM0    = SERCOM0_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM1    = SERCOM1_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM2    = SERCOM2_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM3    = SERCOM3_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM4    = SERCOM4_0_IRQn,
	SYSTEN_INTERRUPT_MODULE_SERCOM5    = SERCOM5_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC0        = TC0_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC1        = TC1_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC2        = TC2_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC3        = TC3_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC4        = TC4_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC5        = TC5_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC6        = TC6_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC7        = TC7_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_ADC        = ADC_OVERRUN_IRQn,
	SYSTEM_INTERRUPT_MODULE_AC         = AC_COMP_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_DAC        = DAC_EMPTY_IRQn,
	SYSTEM_INTERRUPT_MODULE_PTC        = PTC_EOC_IRQn,

	SYSTEM_INTERRUPT_MODULE_NONE,
}

enum system_interrupt_priority {
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_0,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_1,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_2,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_3,
}

#define _SYSTEM_INTERRUPT_IPSR_MASK              0x0000003f
#define _SYSTEM_INTERRUPT_PRIORITY_MASK          0x00000007

#define _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START  0

#define _SYSTEM_INTERRUPT_SYSTICK_PRI_POS        29

/**
 * \brief Enter critical section
 *
 * This function will disable global interrupts. To support nested critical sections it will also
 * keep a counter for how meny times it is called.
 *
 */
void system_interrupt_enter_critical_section(void)
{
	cpu_irq_enter_critical();
}

/**
 * \brief Leave critical section
 *
 * This function will enable global interrupts. To support nested critical sections it will only
 * enable interrupts when the counter reaches 0.
 *
 */
void system_interrupt_leave_critical_section(void)
{
	cpu_irq_leave_critical();
}

/**
 * \brief Check if global interrupts are enabled
 *
 * This function will check if global interrupts are enabled.
 *
 * \returns A boolean that identifies if the global interrupts are enabled or not.
 *
 * \retval true Global interrupts are enabled
 * \retval false Global interrupts are disabled
 *
 */
bool system_interrupt_is_global_enabled(void)
{
	return cpu_irq_is_enabled_flags();
}

/**
 * \brief Check if interrupt vector is enabled or not
 *
 * This function will check if an interrupt is enabled or not and return the state in bool form.
 *
 * \param[in] vector Interrupt vector number
 *
 * \returns A variable identifying if the requested interrupt vector is enabled
 *
 * \retval true Interrupt enabled
 * \retval false Interrupt disabled
 *
 */
static inline bool system_interrupt_is_enabled(enum system_interrupt_vector vector)
{
	return (bool)((NVIC->ISER[0] >> vector) & 0x00000001);
}

/**
 * \brief Enable interrupt vector
 *
 * This function will enable execution of the software handler for the requested interrupt vector.
 *
 * \param[in] vector Interrupt vector number which is beeing enabled
 */
static inline void system_interrupt_enable(enum system_interrupt_vector vector)
{
	NVIC->ISER[0] = (uint32_t)(1 << (vector & 0x0000001f));
}

/**
 * \brief Disable interrupt vector
 *
 * This function will disable execution of the software handler for the requested interrupt vector.
 *
 * \param[in] vector Interrupt vector number which is beeing disabled
 */
static inline void system_interrupt_disable(enum system_interrupt_vector vector)
{
	NVIC->ICER[0] = (uint32_t)(1 << (vector & 0x0000001f));
}

/**
 * \brief Get active interrupt if any
 *
 * This function will return the vector number for the current executing software handler if any.
 *
 */
static inline enum system_interrupt_vector system_interrupt_get_active(void)
{
	uint32_t IPSR = __get_IPSR();

	return (enum system_interrupt_vector)(IPSR & _SYSTEM_INTERRUPT_IPSR_MASK);
}

/**
 * \brief Check if a interrupt line is pending
 *
 * This function will check if the requested interrupt vector is pending, the state will be returned in bool form.
 *
 * \param[in] vector Interrupt vector number which is beening probed
 *
 * \returns A boolean identifying if the requested interrupt vector is pending or not.
 *
 * \retval true Vector is pending
 * \retval false Vector is not pending
 *
 */
static inline bool system_interrupt_is_pending(enum system_interrupt_vector vector)
{
	bool result;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		result = (bool)((NVIC->ISPR[0] >> vector) & 0x00000001);

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {

		result =  (bool)((SCB->ICSR[0] >> SCB_ICSR_PENDSTSET_Pos) & 0x00000001);

	} else {

		Assert(false);
		result = false;
	}

	return result:
}

/**
 * \brief Set a interrupt vector as pending
 *
 * This function will set the requested interrupt vector as pending, this software handler will be handled (if enabled)
 * in priorized order based on vector number and priority settings.
 *
 * \param[in] vector Interrupt vector number which is set as pending
 *
 * \returns A status code identifying if the function was sucessfully executed or not
 *
 * \retval STATUS_OK If no error where detected
 * \retval STATUS_INVALID_ARG If and unsupported interrupt vector number is used
 */
static inline enum status_code system_interrupt_set_pending(enum system_interrupt_vector vector)
{
	enum status_code status = STATUS_OK;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		NVIC->ISPR[0] = 1 << vector;

	} else if((vector == SYSTEM_INTERRUPT_SYSTICK) || (vector == SYSTEM_INTERRUPT_NON_MASKABLE)) {
		/* Because NMI has highest priority it will be executed immediately after it has been set pending */

		uint8_t vector_to_pos_translater[] = {1 << SCB_ICSR_NMIPENDSET_Pos, 1 << SCB_ICSR_PENDSTSET_Pos};

		SCB->ICSR[0] = vector_to_pos_translater[vector == SYSTEM_INTERRUPT_SYSTICK];

	} else {

		/* The user want to set something unsopported as pending */
		Assert(false);
		status = STATUS_INVALID_ARG;

	}

	return status;
}

/**
 * \brief Clear pending interrupt vector
 *
 * Clear a pending interrupt vector so the software handler is not executed.
 *
 * \param[in] vector Interrupt vector number which is cleared 
 *
 * \returns A status code identifying if the function was sucessfully executed or not
 *
 * \retval STATUS_OK If no error where detected
 * \retval STATUS_INVALID_ARG If and unsupported interrupt vector number is used
 */
static inline enum status_code system_interrupt_clear_pending(enum system_interrupt_vector vector)
{

	enum status_code status = STATUS_OK;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		NVIC->ICPR[0] = 1 << vector;

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {
		/* Clearing of NMI pending interrupts does not make sense and is not supported by the
		   device because it has the highest priority and will executed at the moment it is set */

		SCB->ICSR[0] = (uint32_t)(1 << SCB_ICSR_PENDSTCLR_Pos);

	} else {

		Assert(false);
		status = STATUS_INVALID_ARG;
	}

	return status;
}

/**
 * \brief Set interrupt vector priority level
 *
 * Set the priority level of an external interrupt or exception.
 *
 * \param[in] vector Interrupt vector which the priority level is assigned to
 *
 * \returns A status code identifying if the function was sucessfully executed or not
 *
 * \retval STATUS_OK If no error where detected
 * \retval STATUS_INVALID_ARG If and unsupported interrupt vector number is used
 */
enum status_code system_interrupt_set_priority(enum system_interrupt_vector vector,
		enum system_interrupt_priority_level priority_level);
{
	enum status_code status = STATUS_OK;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		uint8_t register_num = vector / 4;
		uint8_t priority_pos = ((vector % 4) * 8) + 5;

		NVIC->IP[register_num] = priority_level << priority_pos;

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {

		SCB->SHP[1] = priority_level << SYSTEM_INTERRUPT_SYSTICK_PRI_POS;

	} else {

		Assert(false);
		status = STATUS_INVALID_ARG;

	}

	return status;
}

/**
 * \brief Get interrupt vector priority level
 *
 * Get the priority level of the requested external interrupt or exception
 */
enum system_interrupt_priority system_interrupt_get_priority(enum system_interrupt_vector vector)
{
	uint8_t register_num = vector / 4;
	uint8_t priority_pos = ((vector % 4) * 8) + 5;
	enum system_interrupt_priority priority;

	if (vector >= 0) {

		priority = (NVIC->IP[register_num] >> priority_pos) & _SYSTEM_INTERRUPT_PRIORITY_MASK;

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {

		priority = (SCB->SHP[1] >> SYSTEM_INTERRUPT_SYSTICK_PRI_POS) & _SYSTEM_INTERRUPT_PRIORITY_MASK;

	}

	return priority;
}

#endif
