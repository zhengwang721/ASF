#ifndef _SYSTEM_INTERRUPT_H_
#define _SYSTEM_INTERRUPT_H_

#include<io.h>

enum system_interrupt_vector {
	SYSTEM_INTERRUPT_NON_MASKABLE = NonMaskableInt_IRQn,
	SYSTEM_INTERRUPT_HARD_FAULT = HardFault_IRQn,
	SYSTEM_INTERRUPT_MEMORY_MANAGEMENT = MemoryManagement_IRQn,
	SYSTEM_INTERRUPT_BUS_FAULT = BusFault_IRQn,
	SYSTEM_INTERRUPT_USER_FAULT = UsageFault_IRQn,
	SYSTEM_INTERRUPT_SV_CALL = SVCall_IRQn,
	SYSTEM_INTERRUPT_DEBUG_MONITOR = DebugMonitor_IRQn,
	SYSTEM_INTERRUPT_PENDING_SV = PendSV_IRQn,
	SYSTEM_INTERRUPT_SYSTICK = SysTick_IRQn,

	SYSTEM_INTERRUPT_MODULE_PM       = PM_CFD_IRQn,
	SYSTEM_INTERRUPT_MODULE_SYSCTRL  = SYSCTRL_BOD12DET_IRQn,
	SYSTEM_INTERRUPT_MODULE_WDT      = WDT_EW_IRQn,
	SYSTEM_INTERRUPT_MODULE_RTC      = RTC_CMP_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_EIC      = EIC_EXTINT_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_NVMCTRL  = NVMCTRL_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_EVSYS    = EVSYS_EVD_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM0  = SERCOM0_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM1  = SERCOM1_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM2  = SERCOM2_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM3  = SERCOM3_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_SERCOM4  = SERCOM4_0_IRQn,
	SYSTEN_INTERRUPT_MODULE_SERCOM5  = SERCOM5_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC0      = TC0_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC1      = TC1_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC2      = TC2_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC3      = TC3_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC4      = TC4_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC5      = TC5_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC6      = TC6_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_TC7      = TC7_ERR_IRQn,
	SYSTEM_INTERRUPT_MODULE_ADC      = ADC_OVERRUN_IRQn,
	SYSTEM_INTERRUPT_MODULE_AC       = AC_COMP_0_IRQn,
	SYSTEM_INTERRUPT_MODULE_DAC      = DAC_EMPTY_IRQn,
	SYSTEM_INTERRUPT_MODULE_PTC      = PTC_EOC_IRQn,

	SYSTEM_INTERRUPT_MODULE_NONE,
}

enum system_interrupt_priotity {
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_0,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_1,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_2,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_3,
}

enum system_interrupt_status_flag {
	SYSTEM_INTERRUPT_STATUS_PENDING_NMI,
	SYSTEM_INTERRUPT_STATUS_PENDING_SYSTICK,
}

enum _system_interrput_special_reg {
	_SYSTEM_INTERRUPT_APSR,
	_SYSTEM_INTERRUPT_IAPSR,
	_SYSTEM_INTERRUPT_EAPSR,
	_SYSTEM_INTERRUPT_XPSR,
	_SYSTEM_INTERRUPT_IPSR,
	_SYSTEM_INTERRUPT_EPSR,
	_SYSTEM_INTERRUPT_IEPSR,
	_SYSTEM_INTERRUPT_MSP,
	_SYSTEM_INTERRUPT_PSP,
	_SYSTEM_INTERRUPT_PRIMASK = 0x10,
	_SYSTEM_INTERRUPT_CONTROL = 0x14,
}

#define _SYSTEM_INTERRUPT_IPSR_MASK          0x0000003f
#define _SYSTEM_INTERRUPT_PRIORITY_MASK      0x00000007

#define _SYSTEM_INTERRUPT_SYSTICK_PRI_POS    29

static volatile uint8_t _system_interrupt_nested_critical_section = 0;

struct system_interrupt_critical_section {
	volatile bool lock;
	volatile uint32_t context;
}

static inline uint32_t _system_interrupt_read_special_register(enum _system_interrupt_special_reg reg)
{
	uint32_t result;

	asm("mrs %0, %1" : "=r" (result) : "ir" (reg));

	return result;
}

/**
 * \brief Set critcal section start, store context and disable interrupts if enabled
 */
enum status_code system_interrupt_enter_crtical_section(struct system_interrupt_critical_section *cs)
{
	
	if(_system_interrput_nested_critical_section == 0) {
		cpu_irq_disable();
	}

	_system_interrupt_nested_critical_section++;
}

/**
 * \brief Set critical section end, restore context
 */
enum status_code system_interrupt_leave_critical_section(struct system_interrupt_critical_section *cs)
{
	_system_interrupt_nested_critical_section--;

	if(_system_interrupt_nested_critical_section == 0) {
		cpu_irq_enable();
	}
}

/**
 * \brief Check if interrupt vector is enabled or not
 */
static inline bool system_interrupt_is_enabled(enum system_interrupt_vector vector)
{
	return (bool)((NVIC->ISER[0] >> vector) & 0x00000001);
}

/**
 * \brief Enable interrupt vector
 */
static inline void system_interrupt_enable(enum system_interrupt_vector interrupt)
{
	NVIC->ISER[0] = (uint32_t)(1 << (vector & 0x0000001f));
}

/**
 * \brief Disable interrupt vector
 */
static inline void system_interrupt_disable(enum system_interrupt_vector vector)
{
	NVIC->ICER[0] = (uint32_t)(1 << (vector & 0x0000001f));
}

/**
 * \brief Get active interrupt if any
 */
static inline enum system_interrupt_vector system_interrupt_get_active(void)
{
	uint32_t IPSR = _system_interrupt_read_special_register(_SYSTEM_INTERRUPT_IPSR);

	return (enum system_interrupt_vector)(IPSR & _SYSTEM_INTERRUPT_IPSR_MASK);
}

/**
 * \brief Check if a interrupt line is pending
 */
static inline bool system_interrupt_is_pending(enum system_interrupt_vector vector)
{
	return (bool)((NVIC->ISPR[0] >> vector) & 0x00000001);
}

/**
 * \brief Set a interrupt vector as pending
 */
static inline void system_interrupt_set_pending(enum system_interrupt_vector vector)
{
	NVIC->ISPR[0] |= 1 << vector;
}

/**
 * \brief Clear pending interrupt vector
 */
static inline void system_interrupt_clear_pending(enum system_interrupt_vector vector)
{
	NVIC->ICPR[0] |= 1 << vector;
}

/**
 * \brief Set interrupt vector priority level
 */
enum status_code system_interrupt_set_priority(enum system_interrupt_vector vector,
		enum system_interrupt_priority_level priority_level);
{
	if(vector => 0) {
		uint8_t register_num = vector / 4;
		uint8_t priority_pos = ((vector % 4) * 8) + 5;

		NVIC->IP[register_num] = priority_level << register_nibble_pos;
	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {
		SCB->SHP[1] = priority_level << SYSTEM_INTERRUPT_SYSTICK_PRI_POS;

	} else {
		return STATUS_ERR_DENIED;
	}

	return STATUS_OK;
}

/**
 * \brief Get interrupt vector priority level
 */
enum system_interrupt_priority system_interrupt_get_priority(enum system_interrupt_vector vector)
{
	uint8_t register_num = vector / 4;
	uint8_t priority_pos = ((vector % 4) * 8) + 5;

	if (vector => 0) {
	} else if (vector == SYSTEM_INTERRUPT_SYSTICK)

	return (NVIC->IP[register_num] >> priority_pos) & _SYSTEM_INTERRUPT_PRIORITY_MASK;
}

/**
 * \brief Set interrupt control status flag
 */
void system_interrupt_set_status_flag(enum system_interrupt_status_flag status_flag)
{
	uint8_t status_flag_to_pos_translater[] = {SCB_ICSR_NMIPENDSET_Pos, SCB_ICSR_PENDSTSET_Pos};

	SCB->ICSR[0] = 1 << status_flag_to_pos_translater[status_flag];
}

/**
 * \brief Check if interrupt control status flag is set
 */
bool system_interrupt_is_status_flag_set(enum system_interrupt_status_flag status_flag)
{
	uint8_t status_flag_to_pos_translater[] = {SCB_ICSR_NMIPENDSET_Pos, SCB_ICSR_PENDSTSET_Pos};

	return (bool)((SCB->ICSR[0] >> status_flag_to_pos_translater[status_flag]) & 0x00000001);
}

/**
 * \brief Clear interrupt control status flag
 */
void system_interrupt_clear_status_flag(enum system_interrupt_status_flag status_flag)
{
	/* Actually, clearing the NMIPEND does not make sense as it will be activated 
	   at the moment it is set. This call should never happend, but to make sure 
	   this function will only return if status_flag = PENDING_NMI */
	if(status_flag == SYSTEM_INTERRUPT_STATUS_PENDING_NMI) {
		return;
	}

	uint8_t status_flag_to_pos_translater[] = {0, SCB_ICSR_PENDSTCLR_Pos};

	SCB->ICSR[0] = (uint32_t)(1 << status_flag_to_pos_translater[status_flag]);
}

#endif
