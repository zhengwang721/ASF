/**
 * \file
 *
 * \brief BPM example.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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

 /**
 * \mainpage BPM example
 * \section intro Introduction
 * This is the documentation for the data structures, functions, variables,
 * defines, enums, and typedefs for the Backup Power Manager (BPM) driver.
 * It also comes bundled with an application-example of usage.
 *
 * This example demonstrates how to use the BPM driver.
 * <b>Operating mode: </b>It uses the USART driver connected to
 * board monitor to display power mode switched.
 * The power mode switch is triggered by the interrupt controller module.
 *
 * \section files Main Files
 * - bpm.c: BPM driver;
 * - bpm.h: BPM driver header file;
 * - bpm_example.c: BPM example application.
 *
 * \section compilinfo Compilation Information
 * This software is written for GNU GCC and IAR Embedded Workbench
 * for Atmel. Other compilers may or may not work.
 *
 * \section deviceinfo Device Information
 * All SAM devices with an BPM and a USART module can be used.
 *
 * \section configinfo Configuration Information
 * This example has been tested with the following configuration:
 * - SAM4L_EK evaluation kit;
 * - CPU clock: 12 MHz;
 * - USART2 (on SAM4L_EK) abstracted with a USB CDC connection to a PC;
 * - USART0 (on SAM4L_EK) which connected to board monitor;
 * - PC terminal settings:
 *   - 115200 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 * Support and FAQ: http://support.atmel.no/
 */

#include <asf.h>
#include "conf_board.h"
#include "bpm_example.h"
#include "ui.h"

/* Configuration list for example modes. */
const mode_config_t mode_configures[] = {
	/* Prompt,  Scaling, Sleep,          Fast wakeup */
	{""       , PS_18V,  BPM_SM_ACTIVE,  0},
	{""       , PS_18V,  BPM_SM_WAIT   , PM_FASTSLEEP_FASTRCOSC(2)},
	{""       , PS_12V,  BPM_SM_ACTIVE,  0},
	{""       , PS_12V,  BPM_SM_WAIT   , PM_FASTSLEEP_FASTRCOSC(2)},
	{""       , PS_12V,  BPM_SM_RET    , PM_FASTSLEEP_FASTRCOSC(2)},
	{""       , PS_12V,  BPM_SM_BACKUP , PM_FASTSLEEP_FASTRCOSC(2)}
};

/* Mode index */
static uint8_t mode_index = 0;

/* Wakeup event bits */
static volatile uint8_t wakeup_events = 0;

/* EIC driver */

/*! \name Mode Trigger Options
 */
//! @{
#define EIC_MODE_EDGE_TRIGGERED   0 //!<
#define EIC_MODE_LEVEL_TRIGGERED  1 //!<
//! @}

/*! \name Edge level Options
 */
//! @{
#define EIC_EDGE_FALLING_EDGE     0 //!<
#define EIC_EDGE_RISING_EDGE      1 //!<
//! @}

/*! \name Level Options
 */
//! @{
#define EIC_LEVEL_LOW_LEVEL       0 //!<
#define EIC_LEVEL_HIGH_LEVEL      1 //!<
//! @}

/*! \name Filter Options
 */
//! @{
#define EIC_FILTER_ENABLED        1 //!<
#define EIC_FILTER_DISABLED       0 //!<
//! @}

/*! \name Synch Mode Options
 */
//! @{
#define EIC_SYNCH_MODE            0 //!<
#define EIC_ASYNCH_MODE           1 //!<
//! @}

/* Configuration parameters of the EIC module. */
typedef struct {
	/* Line */
	uint8_t eic_line;
	/* Mode : EDGE_LEVEL or TRIGGER_LEVEL */
	uint8_t eic_mode;
	/* Edge : FALLING_EDGE or RISING_EDGE */
	uint8_t eic_edge;
	/* Level :  LOW_LEVEL or HIGH_LEVEL */
	uint8_t eic_level;
	/* Filter:  NOT_FILTERED or FILTERED */
	uint8_t eic_filter;
	/* Async:  SYNC mode or ASYNC */
	uint8_t eic_async;
} eic_options_t;

/**
 * \brief Init the EIC driver with specified configuration value.
 *
 * \param eic Base address of the EIC module
 * \param opt Configuration parameters of the EIC module
 *            (see \ref eic_options_t)
 * \param nb_lines Number of lines to consider, equal to size of opt buffer
 */
static void eic_init(
	volatile Eic *eic, const eic_options_t *opt, uint32_t nb_lines)
{
	uint32_t i;
	sysclk_enable_peripheral_clock(EIC);
	for (i = 0; i < nb_lines; i++) {
		/* Set up mode level */
		eic->EIC_MODE = (opt[i].eic_mode == EIC_MODE_LEVEL_TRIGGERED)
			? (eic->EIC_MODE | (1 << opt[i].eic_line))
			: (eic->EIC_MODE & ~(1 << opt[i].eic_line));
		/* Set up edge type */
		eic->EIC_EDGE = (opt[i].eic_edge == EIC_EDGE_RISING_EDGE)
			? (eic->EIC_EDGE | (1 << opt[i].eic_line))
			: (eic->EIC_EDGE & ~(1 << opt[i].eic_line));
		/* Set up level */
		eic->EIC_LEVEL = (opt[i].eic_level == EIC_LEVEL_HIGH_LEVEL)
			? (eic->EIC_LEVEL | (1 << opt[i].eic_line))
			: (eic->EIC_LEVEL & ~(1 << opt[i].eic_line));
		/* Set up if filter is used */
		eic->EIC_FILTER = (opt[i].eic_filter == EIC_FILTER_ENABLED)
			? (eic->EIC_FILTER | (1 << opt[i].eic_line))
			: (eic->EIC_FILTER & ~(1 << opt[i].eic_line));
		/* Set up which mode is used : asynchronous/synchronous */
		eic->EIC_ASYNC = (opt[i].eic_async == EIC_ASYNCH_MODE)
			? (eic->EIC_ASYNC | (1 << opt[i].eic_line))
			: (eic->EIC_ASYNC & ~(1 << opt[i].eic_line));
	}
}

/**
 * \brief Enable the external interrupt on some mask pins.
 *
 * \param eic Base address of the EIC module
 * \param mask_lines Mask for current selected lines
 */
static inline void eic_enable_mask_lines(volatile Eic *eic, uint32_t mask_lines)
{
	eic->EIC_EN = mask_lines;
}

/**
 * \brief Enables the external interrupt from some mask pins to propagate from
 * EIC to interrupt controller.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param mask_lines Mask for current selected lines
 */
static inline void eic_enable_interrupt_mask_lines(
	volatile Eic *eic, uint32_t mask_lines)
{
	eic->EIC_IER = mask_lines;
}

/**
 * \brief Clear the interrupt flag of some mask pins.
 *          Call this function once you've handled the interrupt.
 *
 * \param eic Base address of the EIC (i.e. EIC).
 * \param mask_lines Mask for current selected lines
 */
static inline void eic_clear_interrupt_mask_lines(
	volatile Eic *eic, uint32_t mask_lines)
{
	eic->EIC_ICR = mask_lines;
	eic->EIC_ISR;
}

/**
 * \brief Tells whether an EIC interrupt line is pending.
 *
 * \param eic Base address of the EIC module
 * \param line_number Line number to test
 *
 * \return Whether an EIC interrupt line is pending.
 */
static inline bool eic_is_interrupt_line_pending(
	volatile Eic *eic, uint32_t line_number)
{
	return (eic->EIC_ISR & (1 << line_number)) != 0;
}

/* AST driver */

/*! Timeout to prevent code hang in clock initialization */
#define AST_POLL_TIMEOUT 10000

/*! 1KHz clock from 32KHz oscillator (CLK_1K) */
#define AST_OSC_1KHZ     4

/*
 * The PSEL value to set the AST source clock (after the prescaler) to 1 Hz,
 * when using an external 32-kHz crystal.
 */
#define AST_PSEL_32KHZ_1HZ    14

/**
 * \brief Check the busy status of AST.
 *
 * \param ast Base address of the AST.
 *
 * \return 1 If AST is busy, else it will return 0.
 */
static uint32_t ast_is_busy(volatile Ast *ast)
{
	return (ast->AST_SR & AST_SR_BUSY) != 0;
}

/**
 * \brief Check the busy status of AST clock
 *
 * \param ast Base address of the AST.
 *
 * \return 1 If AST clock is busy, else it will return 0.
 */
static uint32_t ast_is_clkbusy(volatile Ast *ast)
{
	return (ast->AST_SR & AST_SR_CLKBUSY) != 0;
}

/**
 * \brief Enable the AST.
 *
 * \param ast Base address of the AST.
 */
static void ast_enable(volatile Ast *ast)
{
	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}
	/* Enable the AST */
	ast->AST_CR |= AST_CR_EN;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function sets the AST current counter value.
 *
 * \param ast         Base address of the AST.
 * \param ast_counter Startup counter value
 */
static void ast_set_counter_value(volatile Ast *ast,
		uint32_t ast_counter)
{
	/* Wait until we can write into the VAL register */
	while (ast_is_busy(ast)) {
	}
	/* Set the new val value */
	ast->AST_CV = ast_counter;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function will initialize the AST module in counter Mode.
 *
 * \note  If you use the 32 KHz oscillator, it will enable this module.
 *
 * \param ast Base address of the AST.
 * \param osc_type The oscillator you want to use. If you need a better
 *        accuracy, use the 32 KHz oscillator (i.e. AST_OSC_32KHZ).
 * \param psel The preselector value for the corresponding oscillator (4-bits).
 *        To obtain this value, you can use this formula:
 *        psel = log(Fosc/Fast)/log(2)-1, where Fosc is the frequency of the
 *        oscillator you are using (32 KHz or 115 KHz) and Fast the frequency
 *        desired.
 * \param ast_counter Startup counter value
 *
 * \return 1 if the initialization succeeds otherwise it will return 0.
 */
static uint32_t ast_init_counter(volatile Ast *ast, uint8_t osc_type,
		uint8_t psel, uint32_t ast_counter)
{
	uint32_t time_out = AST_POLL_TIMEOUT;
	while (ast_is_clkbusy(ast)) {
		if (--time_out == 0) {
			return 0;
		}
	}
	ast->AST_CLOCK = osc_type << AST_CLOCK_CSSEL_Pos;
	time_out = AST_POLL_TIMEOUT;
	while (ast_is_clkbusy(ast)) {
		if (--time_out == 0) {
			return 0;
		}
	}
	ast->AST_CLOCK |= AST_CLOCK_CEN;
	time_out = AST_POLL_TIMEOUT;
	while (ast_is_clkbusy(ast)) {
		if (--time_out == 0) {
			return 0;
		}
	}

	/* Set the new AST configuration */
	ast->AST_CR = psel << AST_CR_PSEL_Pos;

	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}

	/* Set the calendar */
	ast_set_counter_value(ast, ast_counter);

	return 1;
}

/**
 * \brief This function set the AST periodic0 value.
 *
 * \param ast Base address of the AST.
 * \param pir AST periodic0.
 */
static void ast_set_periodic0_value(volatile Ast *ast, uint32_t pir)
{
	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}
	/* Set the periodic prescaler value */
	ast->AST_PIR0 = pir;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function enables the AST periodic0 event.
 *
 * \param ast Base address of the AST.
 */
static void ast_enable_periodic0(volatile Ast *ast)
{
	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}
	/* Enable the AST periodic0 peripheral event */
	ast->AST_EVE |= AST_EVE_PER0;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function clears the AST status flags.
 *
 * \param ast          Base address of the AST.
 * \param status_mask  AST status flag to be cleared
 */
static void ast_clear_status_flag(volatile Ast *ast,
		uint32_t status_mask)
{
	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}
	/* Clear the AST status flags */
	ast->AST_SCR = status_mask;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function clears the AST Periodic event status flags.
 *
 * \param ast            Base address of the AST.
 * \param periodic_channel  AST wake-up flag to be cleared.
 */
static void ast_clear_periodic_status_flag(volatile Ast *ast,
		uint32_t periodic_channel)
{
	if (periodic_channel) {
		/* Clear the AST periodic event status flag */
		ast_clear_status_flag(ast, AST_SCR_PER1);
	} else {
		/* Clear the AST periodic event status flag */
		ast_clear_status_flag(ast, AST_SCR_PER0);
	}
}

/**
 * \brief This function enables the AST interrupts
 *
 * \param ast             Base address of the AST.
 * \param interrupt_mask  AST Interrupts to be enabled
 */
static void ast_enable_interrupt(volatile Ast *ast,
		uint32_t interrupt_mask)
{
	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}
	/* Enable the AST interrupt */
	ast->AST_IER |= interrupt_mask;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function enables the AST Periodic interrupt.
 *
 * \param ast              Base address of the AST.
 * \param periodic_channel AST Periodic Channel
 */
static void ast_enable_periodic_interrupt(volatile Ast *ast,
		uint8_t periodic_channel)
{
	/* Enable the AST Periodic Asynchronous Wake-up */
	if (periodic_channel) {
		ast_enable_interrupt(ast, AST_IER_PER1);
	} else {
		ast_enable_interrupt(ast, AST_IER_PER0);
	}
}

/**
 * \brief This function enables the AST Asynchronous wake-up.
 *
 * \param ast          Base address of the AST.
 * \param wakeup_mask  AST wake-up flag to be enabled.
 */
static void ast_enable_async_wakeup(volatile Ast *ast,
		uint32_t wakeup_mask)
{
	/* Wait until the ast CTRL register is up-to-date */
	while (ast_is_busy(ast)) {
	}
	/* Enable the AST Asynchronous Wake-up */
	ast->AST_WER |= wakeup_mask;
	/* Wait until write is done */
	while (ast_is_busy(ast)) {
	}
}

/**
 * \brief This function enables the AST Periodic Asynchronous wake-up.
 *
 * \param ast              Base address of the AST.
 * \param periodic_channel AST Periodic Channel
 */
static void ast_enable_periodic_async_wakeup(volatile Ast *ast,
		uint8_t periodic_channel)
{
	/* Enable the AST Periodic Asynchronous Wake-up */
	if (periodic_channel) {
		ast_enable_async_wakeup(ast, AST_WER_PER1);
	} else {
		ast_enable_async_wakeup(ast, AST_WER_PER0);
	}
}

/**
 * EIC interrupt handler for push button interrupt
 */
void EIC_5_Handler(void)
{
	sysclk_enable_peripheral_clock(EIC);
	if(eic_is_interrupt_line_pending(EIC,GPIO_PUSH_BUTTON_EIC_LINE)) {
		wakeup_events |= WAKEUP_EIC;
		ast_set_counter_value(AST,0); /* Reset AST counter */
		eic_clear_interrupt_mask_lines(EIC,
				1 << GPIO_PUSH_BUTTON_EIC_LINE);
	}
	sysclk_disable_peripheral_clock(EIC);
}

/**
 * AST interrupt handler
 */
void AST_PER_Handler(void)
{
	wakeup_events |= WAKEUP_AST;
	ast_clear_periodic_status_flag(AST,0);
}

/**
 * Initialize AST to generate 1Hz counter
 */
static void config_ast(void)
{
	osc_priv_enable_osc32();
	sysclk_enable_peripheral_clock(AST);
	ast_init_counter(AST, AST_OSC_1KHZ, AST_PSEL_32KHZ_1HZ - 6, 0);
	ast_set_periodic0_value(AST,AST_PSEL_32KHZ_1HZ - 3);
	ast_enable_periodic_interrupt(AST,0);
	ast_enable_periodic_async_wakeup(AST,0);
	ast_enable_periodic0(AST);
	ast_clear_periodic_status_flag(AST,0);

	/* Enable AST interrupt */
	NVIC_ClearPendingIRQ(AST_PER_IRQn);
	NVIC_EnableIRQ(AST_PER_IRQn);

	/* Enable the AST */
	ast_enable(AST);
}

/*! Configure to wakeup device by button. */
static void config_buttons(void)
{
	/* Initialize EIC for button wakeup */
	sysclk_enable_peripheral_clock(EIC);
	eic_options_t eic_opt={
		GPIO_PUSH_BUTTON_EIC_LINE,
		EIC_MODE_EDGE_TRIGGERED,
		EIC_EDGE_FALLING_EDGE,
		EIC_LEVEL_LOW_LEVEL,
		EIC_FILTER_DISABLED,
		EIC_ASYNCH_MODE
	};
	eic_init(EIC, &eic_opt, 1);
	eic_enable_mask_lines(EIC, 1 << GPIO_PUSH_BUTTON_EIC_LINE);
	eic_enable_interrupt_mask_lines(EIC, 1 << GPIO_PUSH_BUTTON_EIC_LINE);
	eic_clear_interrupt_mask_lines(EIC, 1 << GPIO_PUSH_BUTTON_EIC_LINE);

	/* Enable EIC interrupt */
	NVIC_ClearPendingIRQ(EIC_5_IRQn);
	NVIC_EnableIRQ(EIC_5_IRQn);
}

/* Misc configurations for wakeup */
static void config_sleep(void)
{
	/* EIC and AST can wakeup the device */
	bpm_enable_wakeup_source(BPM,
			(1 << BPM_BKUPWEN_EIC) | (1 << BPM_BKUPWEN_AST));
	/* EIC can wake the device from backup mode */
	bpm_enable_backup_pin(BPM, 1 << GPIO_PUSH_BUTTON_EIC_LINE);
	/* Retain I/O lines after wakeup from backup */
	bpm_disable_io_retention(BPM);
	bpm_enable_io_retention(BPM);

	bpm_enable_fast_wakeup(BPM);
}

/* Run selected sleep mode */
static void execute_mode(void)
{
	ui_show_mode_info(mode_index, &mode_configures[mode_index]);
	sysclk_disable_hsb_module(SYSCLK_PBA_BRIDGE);
	/* Enter sleep mode */
	wakeup_events = 0;
	/* Power scaling setup */
	bpm_configure_power_scaling(BPM,
			BPM_PMCON_PS(mode_configures[mode_index].ps_value),
			true);
	while((bpm_get_status(BPM) & BPM_SR_PSOK) == 0);
	/* Fast wakeup setup */
	if (mode_configures[mode_index].fastsleep) {
		bpm_enable_fast_wakeup(BPM);
	} else {
		bpm_disable_fast_wakeup(BPM);
	}
	/* Active/Sleep mode */
	if (mode_configures[mode_index].sleep_mode == BPM_SM_ACTIVE) {
		/* Do something while waiting event */
		uint8_t tmp_buf[32], i;
		for (;0 == wakeup_events;i ++) {
			if (tmp_buf[i]) {
				i *= i;
			}
		}
	} else {
		cpu_irq_disable();
		bpm_sleep(BPM, mode_configures[mode_index].sleep_mode);
		/* Wait event */
		while(0 == wakeup_events);
	}
	sysclk_enable_hsb_module(SYSCLK_PBA_BRIDGE);
	ui_show_wakeup_info(mode_index, wakeup_events);
}

/* Switch to next sleep mode */
static void switch_mode(void)
{
	mode_index = (mode_index + 1) %
			(sizeof(mode_configures)/sizeof(mode_config_t));
}

/* Initialize IOs (input & pull-up) */
static void io_init(void)
{
	ioport_init();
#ifdef PIN_PA00
	ioport_set_port_mode(IOPORT_GPIOA, 0xFFFFFFFF, IOPORT_MODE_PULLUP);
	ioport_set_port_dir(IOPORT_GPIOA, 0xFFFFFFFF, IOPORT_DIR_INPUT);
#endif
#ifdef PIN_PB00
	ioport_set_port_mode(IOPORT_GPIOB, 0xFFFFFFFF, IOPORT_MODE_PULLUP);
	ioport_set_port_dir(IOPORT_GPIOB, 0xFFFFFFFF, IOPORT_DIR_INPUT);
#endif
#ifdef PIN_PC00
	ioport_set_port_mode(IOPORT_GPIOC, 0xFFFFFFFF, IOPORT_MODE_PULLUP);
	ioport_set_port_dir(IOPORT_GPIOC, 0xFFFFFFFF, IOPORT_DIR_INPUT);
#endif
#ifdef LED0_GPIO
	ioport_set_pin_dir(LED0_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED0_GPIO, IOPORT_PIN_LEVEL_LOW);
#endif
#ifdef LCD_BL_GPIO
	ioport_set_pin_dir(LCD_BL_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LCD_BL_GPIO, IOPORT_PIN_LEVEL_LOW);
#endif
}

/**
 * \brief Application entry point for BPM example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t wakeup_cause = 0;
	wakeup_cause = bpm_get_backup_wakeup_cause(BPM);
	if (!wakeup_cause) {
		io_init();
	}

	sysclk_init();
	board_init();

	ui_init();

	config_ast();
	config_buttons();
	config_sleep();

	ui_show_backup_wakeup_cause(wakeup_cause);
	while(1) {
		execute_mode();
		switch_mode();
	}
}
