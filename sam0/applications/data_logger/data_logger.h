/**
 * \file
 *
 * \brief SAM D21 Data Logger Application
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#ifndef DLOG_H_INCLUDED
#define DLOG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \mainpage SAM D21 Data Logger Application
 * See \ref appdoc_main "here" for project documentation.
 * \copydetails appdoc_preface
 *
 *
 * \page appdoc_preface Overview
 * This application demonstrates a data logger. The environment temperature is
 * read using 103 AP-2 thermistor from Semitec.
 * The readings are then stored in the serial flash AT25FD081A on the
 * SAM D21 Xplained PRO. The user can access the logs via serial port.
 */

/**
 * \page appdoc_main SAM D21 Data Logger Application
 *
 * Overview:
 * - \ref appdoc_sam0_datalogger_intro
 * - \ref appdoc_sam0_datalogger_setup
 * - \ref appdoc_sam0_datalogger_usage
 * - \ref appdoc_sam0_datalogger_compinfo
 * - \ref appdoc_sam0_datalogger_contactinfo
 * - \ref asfdoc_sam0_datalogger_api
 *
 * \section appdoc_sam0_datalogger_intro Introduction
 * Data logger, as its name indicates measures the given input periodically
 * and stores them in a non-volatile media. In this application, the RTC inside
 * SAM D21 is configured to trigger ADC conversions at regular intervals.
 *
 *
 * \section appdoc_sam0_datalogger_setup Hardware Setup
 * Connect the thermistor as shown in
 * \ref appdoc_sam0_datalogger_thermistor_conn "the diagram below"
 *
 * \anchor appdoc_sam0_datalogger_thermistor_conn
 *
 * \image html therm_conn.svg "Thermistor connection"
 *
 * \section appdoc_sam0_datalogger_usage Usage
 * The CPU is kept in Idle sleep and the RTC timeout event triggers ADC
 * conversion. The DMAC module is used for data transfer from the ADC result
 * register into an array in SRAM. Once a predefined number of conversions
 * are done, the data is transferred into the Serial Flash for non-volatile
 * storage.
 *
 * SERCOM3 is configured as USART, through which the user can read the logs.
 * This is the same port used as virtual COM port, by the Embedded debugger
 * on the SAM D21 Xplained Pro.
 *
 * \section appdoc_sam0_datalogger_compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for ARM.
 * Other compilers may or may not work.
 *
 * \section appdoc_sam0_datalogger_contactinfo Contact Information
 * For further information, visit
 * <a href="http://www.atmel.com">http://www.atmel.com</a>.
 */

#include <asf.h>
#include <conf_example.h>
#include <conf_at25dfx.h>
#include "math.h"

/**
 * \section asfdoc_sam0_datalogger_api API Overview
 * @{
 */

/** We expect a single byte command through USART */
#define CMD_RX_BUFFER_LENGTH                 1
/** Beat count for the DMA resource which transfers data fro ADC to SRAM */
#define NUM_OF_BEATS_IN_ADC_SRAM_TRANSFER    10

/**
 * Define the number of bytes transferred into the serial flash at a time.
 * Keep this number low, since different buffers are allocated depending
 * on the transfer size
 */
#define SF_TRANSFER_SIZE                     10

/**
 * The number of locations written, before we start again from address 0
 * in a 4K block of the serial flash
 */
#define SF_ADDRESS_LIMIT                     600
/** Size of the buffer used for storing different messages */
#define INFO_BUFFER_SIZE                     50
/** Macros used when accessing the Serial Flash */
#define SF_4K_BLOCK0_START                   0x0000
#define SF_4K_BLOCK1_START                   0x1000
#define SF_4K_BLOCK2_START                   0x2000
/** Two bytes are used for representing a temperature reading */
#define BYTES_USED_FOR_A_READING             2

/**
 * Prescaler for the RTC. When the prescaler is 1024, the RTC
 * overflows every 1 sec
 */
#define TIMER_PRESCALER                      RTC_COUNT_PRESCALER_DIV_128

/**
 * Parameters for the ADC channel to which the thermistor is connected
 * ADC reference will be selected as ADC_REFERENCE_INTVCC0 -> VCC/1.48
 */
#define THERM_ADC_RESOLUTION                 16
#define THERM_ADC_REF_VOLT                   (float)(3.3 / 1.48)
#define THERM_ADC_VOLTS_PER_BIT              ((float)THERM_ADC_REF_VOLT / \
												(pow(2, THERM_ADC_RESOLUTION)))
/** Series resistor from VCC to thermistor */
#define THERM_SERIES_RESISTOR                10000
/** VCC is used for exciting the thermistor */
#define THERM_EXCITATION_VOLT                (float)3.3

/**
 * Define the Steinhart-Hart coefficients
 * The equation is
 * 1/T = A + B ln(R) + C(ln(R))^3
 * Reference points taken are 0, 25 and 50 Deg C
 */
#define SHH_COEFF_A                          0.0008913055475
#define SHH_COEFF_B                          0.0002507779917
#define SHH_COEFF_C                          0.0000001957724056

void configure_dma_resource_adc(struct dma_resource *);
void setup_transfer_descriptor_adc(DmacDescriptor *);
void configure_adc(void);
void init_rtc(void);
void rtc_overflow_callback(void);
void configure_rtc_callbacks(void);
void configure_transfer_event(void);
void enable_transfer_trigger(void);
void usart_read_callback(const struct usart_module *);
void configure_usart_callbacks(void);
void transfer_done_usart_tx_dma( const struct dma_resource *);
void sf_store_logs(float *);
void sf_get_recent_logs(char *);
void sf_access_get_status(void);
void sf_access_set_status(void);
void sf_read_all_logs(void);
void sf_sendout_all_logs(void);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* DLOG_H_INCLUDED */
