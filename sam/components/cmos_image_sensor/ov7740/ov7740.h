/**
 * \file
 *
 * \brief API driver for OV7740 CMOS image sensor.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef OV7740_H_INCLUDED
#define OV7740_H_INCLUDED

#include "compiler.h"
#include "board.h"
#include "twi.h"
#include "delay.h"
#include "pio.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/** Slave address of OMNIVISION chip. */
#define OV_CAPTOR_ADDRESS   (0x42 >> 1)   /* OV7740 -> 0x42 */

/** Terminating list entry for register in configuration file. */
#define OV_REG_TERM 0xff

/** Terminating list entry for value in configuration file. */
#define OV_VAL_TERM 0xff

/** Register definitions. */
#define OV7740_GAIN                    0x00
#define OV7740_BLUE_GAIN               0x01
#define OV7740_RED_GAIN                0x02
#define OV7740_GREEN_GAIN              0x03
#define OV7740_REG04                   0x04
#define OV7740_BLUE_AVG                0x05
#define OV7740_GREEN_AVG               0x06
#define OV7740_RED_AVG                 0x07
#define OV7740_PIDH                    0x0a
#define OV7740_PIDL                    0x0b
#define OV7740_REG0C                   0x0c
#define OV7740_REG0C_FLIP              0x80
#define OV7740_REG0C_MIRROR            0x40
#define OV7740_REG0C_YUV_SWAP          0x10
#define OV7740_REG0C_ML_SWAP           0x08

#define OV7740_REG0D                   0x0d
#define OV7740_REG0E                   0x0e /* default:
	                                     * OV7740_REG0E_BLC_BOTH|OV7740_REG0E_BLC_OPTICAL */
#define OV7740_REG0E_OUTPUT_1x         0x00
#define OV7740_REG0E_OUTPUT_2x         0x01
#define OV7740_REG0E_OUTPUT_3x         0x02
#define OV7740_REG0E_OUTPUT_4x         0x03
#define OV7740_REG0E_SLEEP             0x08
#define OV7740_REG0E_BLC_RED           0x20
#define OV7740_REG0E_BLC_BLUE          0x40
#define OV7740_REG0E_BLC_BOTH          0x60
#define OV7740_REG0E_BLC_ELECTRICAL    0x00
#define OV7740_REG0E_BLC_OPTICAL       0x80

#define OV7740_HAEC                    0x0f
#define OV7740_AEC                     0x10
#define OV7740_CLK                     0x11
#define OV7740_CLK_DIV_MASK            0x3f
#define OV7740_CLK_PLL_MASK            0xc0

#define OV7740_REG12                   0x12
#define OV7740_REG12_RAW_RGB           0x01
#define OV7740_REG12_RAW               0x10
#define OV7740_REG12_CC656             0x20
#define OV7740_REG12_VSKIP             0x40
#define OV7740_REG12_RESET             0x80

#define OV7740_REG13                   0x13
#define OV7740_REG13_EXP_AUTO          0x01
#define OV7740_REG13_WBAL_AUTO         0x02
#define OV7740_REG13_AGC_AUTO          0x04
#define OV7740_REG13_LAEC_ENABLE       0x08
#define OV7740_REG13_BANDING_OPT       0x10
#define OV7740_REG13_BANDING_ENABLE    0x20
#define OV7740_REG13_FRAME_DROP        0x40
#define OV7740_REG13_AEC_FASTER        0x80

#define OV7740_REG14                   0x14
#define OV7740_REG15                   0x15
#define OV7740_REG15_GAIN_LSB_MASK     0x03
#define OV7740_REG15_NIGHT_2X_GAIN     0x00
#define OV7740_REG15_NIGHT_4X_GAIN     0x04
#define OV7740_REG15_NIGHT_8X_GAIN     0x08
#define OV7740_REG15_NIGHT_16X_GAIN    0x0c
#define OV7740_REG15_CEIL_0            0x00
#define OV7740_REG15_CEIL_1            0x10
#define OV7740_REG15_CEIL_2            0x20
#define OV7740_REG15_CEIL_3            0x30
#define OV7740_REG15_CEIL_7            0x70
#define OV7740_REG15_ENABLE_NIGHT      0x80

#define OV7740_REG16                   0x16
#define OV7740_AHSTART                 0x17
#define OV7740_AHSIZE                  0x18
#define OV7740_AVSTART                 0x19
#define OV7740_AVSIZE                  0x1a
#define OV7740_PSHFT                   0x1b
#define OV7740_MIDH                    0x1c
#define OV7740_MIDL                    0x1d
#define OV7740_REG1E                   0x1e
#define OV7740_REG1F                   0x1f
#define OV7740_REG1E                   0x1e
#define OV7740_REG20                   0x20
#define OV7740_REG21                   0x21
#define OV7740_WPT                     0x24
#define OV7740_BPT                     0x25
#define OV7740_VPT                     0x26
#define OV7740_REG27                   0x27
#define OV7740_REG27_BLACKSUN          0x80

#define OV7740_REG28                   0x28
#define OV7740_REG28_VSYNC_NEG         0x02
#define OV7740_REG28_NO_VSYNC_DROP     0x08
#define OV7740_REG28_HREF_NEG          0x10
#define OV7740_REG28_HSYNC_NEG         0x20
#define OV7740_REG28_HSYNC             0x40
#define OV7740_REG28_OUTPUT_REVERSE    0x80

#define OV7740_YUV422CTRL              0xd9
#define OV7740_YUV422CTRL_AVERAGE      0x00
#define OV7740_YUV422CTRL_DROP         0x01
#define OV7740_YUV422CTRL_YUYV         0x00
#define OV7740_YUV422CTRL_YVYU         0x02

#define OV7740_REG065                  0x65
#define OV7740_REG065_BIT_SWAP         0x08

/**
 * OV7740 register struture.
 */
typedef struct _ov_reg {
	uint8_t reg; /*!< Register to be written */
	uint8_t val; /*!< Value to be written in the register */
} ov_reg;

/**
 * Enumeration of different format which could be used by the 0V7740 sensor.
 * These enumeration allow to configure the OV7740 registers using
 * the ov7740_table_registre.c Array.
 */
typedef enum _EOV7740_Format {
	QVGA_YUV422_10FPS, /*!< Register to be written  */
	QVGA_YUV422_15FPS, /*!< Register to be written */
	QVGA_YUV422_20FPS, /*!< Register to be written */
	QVGA_YUV422_30FPS, /*!< Register to be written */
	QVGA_RGB888, /*!< Register to be written */
	QQVGA_YUV422, /*!< Register to be written */
	QQVGA_RGB888, /*!< Register to be written */
	TEST_PATTERN, /*!< Register to be written */
	VGA_YUV422_20FPS /*!< Register to be written */
} EOV7740_Format;

extern const ov_reg OV7740_QVGA_YUV422_10FPS[];
extern const ov_reg OV7740_QVGA_YUV422_15FPS[];
extern const ov_reg OV7740_QVGA_YUV422_20FPS[];
extern const ov_reg OV7740_QVGA_YUV422_30FPS[];
extern const ov_reg OV7740_QVGA_RGB888[];
extern const ov_reg OV7740_QQVGA_YUV422[];
extern const ov_reg OV7740_QQVGA_RGB888[];
extern const ov_reg OV7740_TEST_PATTERN[];
extern const ov_reg OV7740_VGA_YUV422_20FPS[];

void ov_power(uint32_t on_off, Pio *p_pio, const uint32_t ul_mask);
void ov_reset(Pio *p_pio, uint32_t ul_mask);
uint32_t ov_read_reg(Twi *p_twi, twi_packet_t *p_packet);
uint32_t ov_write_reg(Twi *p_twi, twi_packet_t *p_packet);
uint32_t ov_write_regs( Twi *p_twi, const ov_reg *p_reg_list );
void ov_dump_registers( Twi *p_twi, ov_reg *p_regs );
uint32_t ov_init( Twi *p_twi );
uint32_t ov_configure( Twi *p_twi, const EOV7740_Format format );
uint32_t ov_configure_finish( Twi *p_twi );
uint32_t ov_configure_manual( Twi *p_twi );
uint32_t ov_store_manual(Twi *p_twi, volatile uint32_t *p_backup_addr,
		uint32_t ul_size);
uint32_t ov_restore_manual(volatile uint32_t *p_backup_addr, uint32_t ul_size);

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \page sam_component_ov7740_quick_start Quick Start Guide for the OV7740
 * component.
 *
 * This is the quick start guide for the \ref ov7740_sensor_group, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case. The code examples can be copied into e.g the main
 * application loop or any other function that will need to control the
 * OV7740 CMOS image sensor.
 *
 * \section ov7740_qs_use_cases Use cases
 * - \ref ov7740_basic
 *
 * \section ov7740_basic ov7740 basic usage
 *
 * This use case will demonstrate how to initialize the OV7740 sensor
 * module.
 *
 * \section ov7740_basic_setup Setup steps
 *
 * \subsection ov7740_basic_prereq Prerequisites
 *
 * This module requires the following driver
 * - \ref group_sam_pio_capture
 *
 * \subsection c42364a_basic_usage_code
 *
 * First set the PLLB to the system core clock and PLLA to clock PCK0
 * (used to get data from image sensor).
 * The next step is to configure the LCD controller and display information
 * to the user.
 * Then external SRAM, which is used to store data after acquisition, is
 * configured.
 * Finally this exemple configures the OV740 CMOS image sensor and the PIO
 * capture mode.
 *
 * \endcode
 */

#endif /* OV7740_H_INCLUDED */
