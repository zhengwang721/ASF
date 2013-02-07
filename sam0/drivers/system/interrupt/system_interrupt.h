/**
 * \file
 *
 * \brief SAMD20 System Interrupt Driver
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef SYSTEM_INTERRUPT_H_INCLUDED
#define SYSTEM_INTERRUPT_H_INCLUDED

#include <compiler.h>
#include <core_cm0plus.h>

#if !defined(__DOXYGEN__)
#  define _MODULE_IRQn(n, module) \
		SYSTEM_INTERRUPT_MODULE_##module##n = module##n##_IRQn,

#  define _SYSTEM_INTERRUPT_MODULES(name) \
		MREPEAT(name##_INST_NUM, _MODULE_IRQn, name)
#endif

enum system_interrupt_vector {
	SYSTEM_INTERRUPT_NON_MASKABLE      = NonMaskableInt_IRQn,
	SYSTEM_INTERRUPT_HARD_FAULT        = HardFault_IRQn,
	SYSTEM_INTERRUPT_SV_CALL           = SVCall_IRQn,
	SYSTEM_INTERRUPT_PENDING_SV        = PendSV_IRQn,
	SYSTEM_INTERRUPT_SYSTICK           = SysTick_IRQn,

	SYSTEM_INTERRUPT_MODULE_PM         = PM_IRQn,
	SYSTEM_INTERRUPT_MODULE_SYSCTRL    = SYSCTRL_IRQn,
	SYSTEM_INTERRUPT_MODULE_WDT        = WDT_IRQn,
	SYSTEM_INTERRUPT_MODULE_RTC        = RTC_IRQn,
	SYSTEM_INTERRUPT_MODULE_EIC        = EIC_IRQn,
	SYSTEM_INTERRUPT_MODULE_NVMCTRL    = NVMCTRL_IRQn,
	SYSTEM_INTERRUPT_MODULE_EVSYS      = EVSYS_IRQn,
#if defined(__DOXYGEN__)
	SYSTEM_INTERRUPT_MODULE_SERCOMn    = SERCOMn_IRQn
	SYSTEM_INTERRUPT_MODULE_TCn        = TCn_IRQn
#else
	_SYSTEM_INTERRUPT_MODULES(SERCOM)
	_SYSTEM_INTERRUPT_MODULES(TC)
#endif
	SYSTEM_INTERRUPT_MODULE_AC         = AC_IRQn,
	SYSTEM_INTERRUPT_MODULE_DAC        = DAC_IRQn,

	SYSTEM_INTERRUPT_MODULE_NONE,
};

enum system_interrupt_priority_level {
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_0,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_1,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_2,
	SYSTEM_INTERRUPT_PRIORITY_LEVEL_3,
};

#define _SYSTEM_INTERRUPT_IPSR_MASK              0x0000003f
#define _SYSTEM_INTERRUPT_PRIORITY_MASK          0x00000007

#define _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START  0

#define _SYSTEM_INTERRUPT_SYSTICK_PRI_POS        29

/**
 * \brief Enter critical section
 *
 * This function will disable global interrupts. To support nested critical sections it will also
 * keep a counter for how many times it is called.
 *
 */
static inline void system_interrupt_enter_critical_section(void)
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
static inline void system_interrupt_leave_critical_section(void)
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
static inline bool system_interrupt_is_global_enabled(void)
{
	return cpu_irq_is_enabled();
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
static inline bool system_interrupt_is_enabled(
		enum system_interrupt_vector vector)
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
static inline void system_interrupt_enable(
		enum system_interrupt_vector vector)
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
static inline void system_interrupt_disable(
		enum system_interrupt_vector vector)
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
bool system_interrupt_is_pending(
		enum system_interrupt_vector vector);

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
enum status_code system_interrupt_set_pending(
		enum system_interrupt_vector vector);

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
enum status_code system_interrupt_clear_pending(
		enum system_interrupt_vector vector);

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
enum status_code system_interrupt_set_priority(
		enum system_interrupt_vector vector,
		enum system_interrupt_priority_level priority_level);

/**
 * \brief Get interrupt vector priority level
 *
 * Get the priority level of the requested external interrupt or exception
 */
enum system_interrupt_priority_level system_interrupt_get_priority(
		enum system_interrupt_vector vector);

#endif
