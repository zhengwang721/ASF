/**
 * \file
 *
 * \brief Component description for AON_GP_REGS
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef _SAMB11_AON_GP_REGS_COMPONENT_
#define _SAMB11_AON_GP_REGS_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR AON_GP_REGS */
/* ========================================================================== */
/** \addtogroup SAMB11_AON_GP_REGS Always On General Purpose Registers */
/*@{*/

#define AON_GP_REGS_AGR1234
#define REV_AON_GP_REGS                  0x100

/* -------- AON_GP_REGS_AON_PINMUX_SEL : (AON_GP_REGS Offset: 0x00) (R/W  16) Controls the pinmux selection for the AO GPIOs -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint16_t AO_GPIO_0_SEL:2;           /*!< bit:   0..1  Pinmux select for AO_GPIO_0              */
    uint16_t :2;                        /*!< bit:   2..3  Reserved                                 */
    uint16_t AO_GPIO_1_SEL:2;           /*!< bit:   4..5  Pinmux select for AO_GPIO_1              */
    uint16_t :2;                        /*!< bit:   6..7  Reserved                                 */
    uint16_t AO_GPIO_2_SEL:2;           /*!< bit:   8..9  Pinmux select for AO_GPIO_2              */
    uint16_t :6;                        /*!< bit: 10..15  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint16_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_PINMUX_SEL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_PINMUX_SEL_OFFSET     0x00           /**<  \brief (AON_GP_REGS_AON_PINMUX_SEL offset) Controls the pinmux selection for the AO GPIOs */
#define AON_GP_REGS_AON_PINMUX_SEL_RESETVALUE 0x01ul         /**<  \brief (AON_GP_REGS_AON_PINMUX_SEL reset_value) Controls the pinmux selection for the AO GPIOs */

#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_Pos 0  /**< \brief (AON_GP_REGS_AON_PINMUX_SEL) Pinmux select for AO_GPIO_0     */
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_Msk (0x3ul << AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_Pos)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL(value) (AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_Msk & ((value) << AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_Pos))  
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_Pos 4  /**< \brief (AON_GP_REGS_AON_PINMUX_SEL) Pinmux select for AO_GPIO_1     */
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_Msk (0x3ul << AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_Pos)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL(value) (AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_Msk & ((value) << AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_Pos))  
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_Pos 8  /**< \brief (AON_GP_REGS_AON_PINMUX_SEL) Pinmux select for AO_GPIO_2     */
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_Msk (0x3ul << AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_Pos)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL(value) (AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_Msk & ((value) << AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_Pos))  
#define AON_GP_REGS_AON_PINMUX_SEL_MASK       0x333ul    /**< \brief (AON_GP_REGS_AON_PINMUX_SEL) Register MASK */

/* -------- AON_GP_REGS_AON_PMU_CTRL : (AON_GP_REGS Offset: 0x04) (R/W  32) Always On Misc Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint32_t PMU_REGS_4TO1_SEL:1;       /*!< bit:      0  If 1 then uses the frequency hopping table otherwise registers in PD0 */
    uint32_t PMU_RTC_CLK_EN:1;          /*!< bit:      1  Enables the PMU RTC clock                */
    uint32_t :2;                        /*!< bit:   2..3  Reserved                                 */
    uint32_t PMU_RETN_VAL_SEL:2;        /*!< bit:   4..5  Retention Voltage Selection              */
    uint32_t :1;                        /*!< bit:      6  Reserved                                 */
    uint32_t EFUSE_LDO_EN:1;            /*!< bit:      7  Enables the EFUSE LDO                    */
    uint32_t PMU_MUX_EN:1;              /*!< bit:      8  PMU MUX EN                               */
    uint32_t PMU_MUX_A:3;               /*!< bit:  9..11  PMU MUX A                                */
    uint32_t PMU_MUX_SEL:4;             /*!< bit: 12..15  PMU MUX SEL                              */
    uint32_t PMU_SENS_ADC_EN:1;         /*!< bit:     16  PMU Sensor ADC Enable                    */
    uint32_t PMU_SENS_ADC_RST:1;        /*!< bit:     17  PMU Sensor ADC Reset                     */
    uint32_t PMU_BGR_EN:1;              /*!< bit:     18  PMU BGR EN                               */
    uint32_t PMU_2MHZ_CLK_EN:1;         /*!< bit:     19  Enables the 2MHz OSC clock coming to the digital from the PMU */
    uint32_t PMU_26MHZ_CLK_FORCE_OFF:1;  /*!< bit:     20  If set, this will force off the OSC 26 MHz clock, otherwise this will follow the power for PD1 */
    uint32_t :11;                       /*!< bit: 21..31  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint32_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_PMU_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_PMU_CTRL_OFFSET       0x04           /**<  \brief (AON_GP_REGS_AON_PMU_CTRL offset) Always On Misc Control */
#define AON_GP_REGS_AON_PMU_CTRL_RESETVALUE   0xA0022ul      /**<  \brief (AON_GP_REGS_AON_PMU_CTRL reset_value) Always On Misc Control */

#define AON_GP_REGS_AON_PMU_CTRL_PMU_REGS_4TO1_SEL_Pos 0  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) If 1 then uses the frequency hopping table otherwise registers in PD0 */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_REGS_4TO1_SEL (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_REGS_4TO1_SEL_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RTC_CLK_EN_Pos 1  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) Enables the PMU RTC clock         */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RTC_CLK_EN (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_RTC_CLK_EN_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_Pos 4  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) Retention Voltage Selection       */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_Msk (0x3ul << AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_Pos)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL(value) (AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_Msk & ((value) << AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_Pos))  
#define AON_GP_REGS_AON_PMU_CTRL_EFUSE_LDO_EN_Pos 7  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) Enables the EFUSE LDO             */
#define AON_GP_REGS_AON_PMU_CTRL_EFUSE_LDO_EN (0x1ul << AON_GP_REGS_AON_PMU_CTRL_EFUSE_LDO_EN_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_EN_Pos 8  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) PMU MUX EN                        */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_EN   (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_EN_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_Pos 9  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) PMU MUX A                         */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_Msk (0x7ul << AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_Pos)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A(value) (AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_Msk & ((value) << AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_Pos))  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_Pos 12  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) PMU MUX SEL                       */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_Msk (0xFul << AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_Pos)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL(value) (AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_Msk & ((value) << AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_Pos))  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_EN_Pos 16  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) PMU Sensor ADC Enable             */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_EN (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_EN_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_RST_Pos 17  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) PMU Sensor ADC Reset              */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_RST (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_RST_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_BGR_EN_Pos 18  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) PMU BGR EN                        */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_BGR_EN   (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_BGR_EN_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_2MHZ_CLK_EN_Pos 19  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) Enables the 2MHz OSC clock coming to the digital from the PMU */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_2MHZ_CLK_EN (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_2MHZ_CLK_EN_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_PMU_26MHZ_CLK_FORCE_OFF_Pos 20  /**< \brief (AON_GP_REGS_AON_PMU_CTRL) If set, this will force off the OSC 26 MHz clock, otherwise this will follow the power for PD1 */
#define AON_GP_REGS_AON_PMU_CTRL_PMU_26MHZ_CLK_FORCE_OFF (0x1ul << AON_GP_REGS_AON_PMU_CTRL_PMU_26MHZ_CLK_FORCE_OFF_Pos)  
#define AON_GP_REGS_AON_PMU_CTRL_MASK         0x1FFFB3ul    /**< \brief (AON_GP_REGS_AON_PMU_CTRL) Register MASK */

/* -------- AON_GP_REGS_AON_BLE_LP_CTRL : (AON_GP_REGS Offset: 0x08) (R/W  8) Always On BLE LP Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  WAKEUP_LP_LATCH:1;         /*!< bit:      0  If set, this will latch the wakeup signal in the BLE LP block */
    uint8_t  DEEP_SLEEP_STAT_SYNC_EN:1;  /*!< bit:      1  If set, this will use the sync flops for deep sleep stat */
    uint8_t  :6;                        /*!< bit:   2..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_BLE_LP_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_BLE_LP_CTRL_OFFSET    0x08           /**<  \brief (AON_GP_REGS_AON_BLE_LP_CTRL offset) Always On BLE LP Control */
#define AON_GP_REGS_AON_BLE_LP_CTRL_RESETVALUE 0x00ul         /**<  \brief (AON_GP_REGS_AON_BLE_LP_CTRL reset_value) Always On BLE LP Control */

#define AON_GP_REGS_AON_BLE_LP_CTRL_WAKEUP_LP_LATCH_Pos 0  /**< \brief (AON_GP_REGS_AON_BLE_LP_CTRL) If set, this will latch the wakeup signal in the BLE LP block */
#define AON_GP_REGS_AON_BLE_LP_CTRL_WAKEUP_LP_LATCH (0x1ul << AON_GP_REGS_AON_BLE_LP_CTRL_WAKEUP_LP_LATCH_Pos)  
#define AON_GP_REGS_AON_BLE_LP_CTRL_DEEP_SLEEP_STAT_SYNC_EN_Pos 1  /**< \brief (AON_GP_REGS_AON_BLE_LP_CTRL) If set, this will use the sync flops for deep sleep stat */
#define AON_GP_REGS_AON_BLE_LP_CTRL_DEEP_SLEEP_STAT_SYNC_EN (0x1ul << AON_GP_REGS_AON_BLE_LP_CTRL_DEEP_SLEEP_STAT_SYNC_EN_Pos)  
#define AON_GP_REGS_AON_BLE_LP_CTRL_MASK      0x03ul    /**< \brief (AON_GP_REGS_AON_BLE_LP_CTRL) Register MASK */

/* -------- AON_GP_REGS_AON_MISC_CTRL : (AON_GP_REGS Offset: 0x0c) (R/W  32) Always On Misc Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint32_t :1;                        /*!< bit:      0  Reserved                                 */
    uint32_t USE_RTC_32KHZ_CLK_SLEEP_TIMER:1;  /*!< bit:      1  If set, this will use the RTC version of the 32khz clock for the sleep timer */
    uint32_t USE_EXT_32KHZ_CLK_SLEEP_TIMER:1;  /*!< bit:      2  If set, this will use an external RTC for the 32khz clock for the sleep timer */
    uint32_t LPMCU_BOOT_RESET_MUXSEL:1;  /*!< bit:      3  If 1, then it uses the CPU register bit, otherwise LP_BOOT_PIN is connected */
    uint32_t LPMCU_USE_BOOT_REGS:1;     /*!< bit:      4  If 1, then the ARM will use the boot regs instead of the boot rom for the first 4 boot addresses (0x0 - 0xF) */
    uint32_t LPMCU_CPU_RESET_OVERRIDE_EN:1;  /*!< bit:      5  If 1, then it will use the PD0 register value for LPMCU CPU RESET (higher priority than lpmcu_boot_reset_muxsel) */
    uint32_t LPMCU_CPU_RESET_OVERRIDE_VAL:1;  /*!< bit:      6  If lpmcu_cpu_reset_override_en is set then this value will be used for the LPMCU CPU RESET */
    uint32_t :7;                        /*!< bit:  7..13  Reserved                                 */
    uint32_t USE_2M_AON_PWR_SEQ_CLK:1;  /*!< bit:     14  If set, this will use either the osc 2M or rtc 32kHz clock for the AON power sequencer.  The field use_rtc_aon_pwr_seq_clk determines which. */
    uint32_t USE_OSC2M_AS_TB_CLK:1;     /*!< bit:     15  If set, this will use the osc 2M clock as the PD0 testbus clock */
    uint32_t AON_SLEEP_TIMER_CLK_EN:1;  /*!< bit:     16  If set, this will enable the 32khz clock to the AON Sleep Timer */
    uint32_t AON_EXT_32KHZ_OUT_EN:1;    /*!< bit:     17  If set, this will enable the 32khz clock to the AON pads as an output of the chip */
    uint32_t USE_RTC_AON_PWR_SEQ_CLK:1;  /*!< bit:     18  If set, this will use the RTC clock as the AON power sequencer clock once use_2m_aon_pwr_seq_clk is also set.  This must be set first. */
    uint32_t INVERT_WAKEUP_GPIO_0:1;    /*!< bit:     19  If set, this will invert the wakeup gpio_0 */
    uint32_t FORCE_OFF_XO:1;            /*!< bit:     20  If set, this will force off the XO, otherwise XO is always on if BLE core is powered on. */
    uint32_t FORCE_XO_TO_BYPASS_MODE:1;  /*!< bit:     21  If set, this will force the XO into bypass mode. */
    uint32_t :10;                       /*!< bit: 22..31  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint32_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_MISC_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_MISC_CTRL_OFFSET      0x0C           /**<  \brief (AON_GP_REGS_AON_MISC_CTRL offset) Always On Misc Control */
#define AON_GP_REGS_AON_MISC_CTRL_RESETVALUE  0x10000ul      /**<  \brief (AON_GP_REGS_AON_MISC_CTRL reset_value) Always On Misc Control */

#define AON_GP_REGS_AON_MISC_CTRL_USE_RTC_32KHZ_CLK_SLEEP_TIMER_Pos 1  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will use the RTC version of the 32khz clock for the sleep timer */
#define AON_GP_REGS_AON_MISC_CTRL_USE_RTC_32KHZ_CLK_SLEEP_TIMER (0x1ul << AON_GP_REGS_AON_MISC_CTRL_USE_RTC_32KHZ_CLK_SLEEP_TIMER_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_USE_EXT_32KHZ_CLK_SLEEP_TIMER_Pos 2  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will use an external RTC for the 32khz clock for the sleep timer */
#define AON_GP_REGS_AON_MISC_CTRL_USE_EXT_32KHZ_CLK_SLEEP_TIMER (0x1ul << AON_GP_REGS_AON_MISC_CTRL_USE_EXT_32KHZ_CLK_SLEEP_TIMER_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_BOOT_RESET_MUXSEL_Pos 3  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If 1, then it uses the CPU register bit, otherwise LP_BOOT_PIN is connected */
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_BOOT_RESET_MUXSEL (0x1ul << AON_GP_REGS_AON_MISC_CTRL_LPMCU_BOOT_RESET_MUXSEL_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_USE_BOOT_REGS_Pos 4  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If 1, then the ARM will use the boot regs instead of the boot rom for the first 4 boot addresses (0x0 - 0xF) */
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_USE_BOOT_REGS (0x1ul << AON_GP_REGS_AON_MISC_CTRL_LPMCU_USE_BOOT_REGS_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_EN_Pos 5  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If 1, then it will use the PD0 register value for LPMCU CPU RESET (higher priority than lpmcu_boot_reset_muxsel) */
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_EN (0x1ul << AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_EN_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_VAL_Pos 6  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If lpmcu_cpu_reset_override_en is set then this value will be used for the LPMCU CPU RESET */
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_VAL (0x1ul << AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_VAL_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_USE_2M_AON_PWR_SEQ_CLK_Pos 14  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will use either the osc 2M or rtc 32kHz clock for the AON power sequencer.  The field use_rtc_aon_pwr_seq_clk determines which. */
#define AON_GP_REGS_AON_MISC_CTRL_USE_2M_AON_PWR_SEQ_CLK (0x1ul << AON_GP_REGS_AON_MISC_CTRL_USE_2M_AON_PWR_SEQ_CLK_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_USE_OSC2M_AS_TB_CLK_Pos 15  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will use the osc 2M clock as the PD0 testbus clock */
#define AON_GP_REGS_AON_MISC_CTRL_USE_OSC2M_AS_TB_CLK (0x1ul << AON_GP_REGS_AON_MISC_CTRL_USE_OSC2M_AS_TB_CLK_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_AON_SLEEP_TIMER_CLK_EN_Pos 16  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will enable the 32khz clock to the AON Sleep Timer */
#define AON_GP_REGS_AON_MISC_CTRL_AON_SLEEP_TIMER_CLK_EN (0x1ul << AON_GP_REGS_AON_MISC_CTRL_AON_SLEEP_TIMER_CLK_EN_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_AON_EXT_32KHZ_OUT_EN_Pos 17  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will enable the 32khz clock to the AON pads as an output of the chip */
#define AON_GP_REGS_AON_MISC_CTRL_AON_EXT_32KHZ_OUT_EN (0x1ul << AON_GP_REGS_AON_MISC_CTRL_AON_EXT_32KHZ_OUT_EN_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_USE_RTC_AON_PWR_SEQ_CLK_Pos 18  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will use the RTC clock as the AON power sequencer clock once use_2m_aon_pwr_seq_clk is also set.  This must be set first. */
#define AON_GP_REGS_AON_MISC_CTRL_USE_RTC_AON_PWR_SEQ_CLK (0x1ul << AON_GP_REGS_AON_MISC_CTRL_USE_RTC_AON_PWR_SEQ_CLK_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_INVERT_WAKEUP_GPIO_0_Pos 19  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will invert the wakeup gpio_0 */
#define AON_GP_REGS_AON_MISC_CTRL_INVERT_WAKEUP_GPIO_0 (0x1ul << AON_GP_REGS_AON_MISC_CTRL_INVERT_WAKEUP_GPIO_0_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_FORCE_OFF_XO_Pos 20  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will force off the XO, otherwise XO is always on if BLE core is powered on. */
#define AON_GP_REGS_AON_MISC_CTRL_FORCE_OFF_XO (0x1ul << AON_GP_REGS_AON_MISC_CTRL_FORCE_OFF_XO_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_FORCE_XO_TO_BYPASS_MODE_Pos 21  /**< \brief (AON_GP_REGS_AON_MISC_CTRL) If set, this will force the XO into bypass mode. */
#define AON_GP_REGS_AON_MISC_CTRL_FORCE_XO_TO_BYPASS_MODE (0x1ul << AON_GP_REGS_AON_MISC_CTRL_FORCE_XO_TO_BYPASS_MODE_Pos)  
#define AON_GP_REGS_AON_MISC_CTRL_MASK        0x3FC07Eul    /**< \brief (AON_GP_REGS_AON_MISC_CTRL) Register MASK */

/* -------- AON_GP_REGS_AON_GLOBAL_RESET : (AON_GP_REGS Offset: 0x10) (R/W  8) Active Low Always On Reset Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  GLOBAL_RSTN:1;             /*!< bit:      0  Global Reset                             */
    uint8_t  SLEEP_TIMER_RSTN:1;        /*!< bit:      1  Sleep Timer Reset                        */
    uint8_t  :1;                        /*!< bit:      2  Reserved                                 */
    uint8_t  BLE_LP_RSTN:1;             /*!< bit:      3  BLE Low Power Reset                      */
    uint8_t  PD4_RSTN:1;                /*!< bit:      4  Allow the SW to force PD4 into reset     */
    uint8_t  :3;                        /*!< bit:   5..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_GLOBAL_RESET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_GLOBAL_RESET_OFFSET   0x10           /**<  \brief (AON_GP_REGS_AON_GLOBAL_RESET offset) Active Low Always On Reset Control */
#define AON_GP_REGS_AON_GLOBAL_RESET_RESETVALUE 0x1Bul         /**<  \brief (AON_GP_REGS_AON_GLOBAL_RESET reset_value) Active Low Always On Reset Control */

#define AON_GP_REGS_AON_GLOBAL_RESET_GLOBAL_RSTN_Pos 0  /**< \brief (AON_GP_REGS_AON_GLOBAL_RESET) Global Reset                  */
#define AON_GP_REGS_AON_GLOBAL_RESET_GLOBAL_RSTN (0x1ul << AON_GP_REGS_AON_GLOBAL_RESET_GLOBAL_RSTN_Pos)  
#define AON_GP_REGS_AON_GLOBAL_RESET_SLEEP_TIMER_RSTN_Pos 1  /**< \brief (AON_GP_REGS_AON_GLOBAL_RESET) Sleep Timer Reset             */
#define AON_GP_REGS_AON_GLOBAL_RESET_SLEEP_TIMER_RSTN (0x1ul << AON_GP_REGS_AON_GLOBAL_RESET_SLEEP_TIMER_RSTN_Pos)  
#define AON_GP_REGS_AON_GLOBAL_RESET_BLE_LP_RSTN_Pos 3  /**< \brief (AON_GP_REGS_AON_GLOBAL_RESET) BLE Low Power Reset           */
#define AON_GP_REGS_AON_GLOBAL_RESET_BLE_LP_RSTN (0x1ul << AON_GP_REGS_AON_GLOBAL_RESET_BLE_LP_RSTN_Pos)  
#define AON_GP_REGS_AON_GLOBAL_RESET_PD4_RSTN_Pos 4  /**< \brief (AON_GP_REGS_AON_GLOBAL_RESET) Allow the SW to force PD4 into reset */
#define AON_GP_REGS_AON_GLOBAL_RESET_PD4_RSTN (0x1ul << AON_GP_REGS_AON_GLOBAL_RESET_PD4_RSTN_Pos)  
#define AON_GP_REGS_AON_GLOBAL_RESET_MASK     0x1Bul    /**< \brief (AON_GP_REGS_AON_GLOBAL_RESET) Register MASK */

/* -------- AON_GP_REGS_AON_PULL_ENABLE : (AON_GP_REGS Offset: 0x14) (R/W  8) Active Low Always On Pull Enable Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  AO_GPIO_0:1;               /*!< bit:      0  Pull Enable for AO_GPIO_0                */
    uint8_t  AO_GPIO_1:1;               /*!< bit:      1  Pull Enable for AO_GPIO_1                */
    uint8_t  AO_GPIO_2:1;               /*!< bit:      2  Pull Enable for AO_GPIO_2                */
    uint8_t  :5;                        /*!< bit:   3..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  struct {
    uint8_t  AO_GPIO_:3;                /*!< bit:   0..2  Pull Enable for AO_GPIO_2                */
    uint8_t  :5;                        /*!< bit:   3..7 Reserved                                  */
  } vec;                                /*!< Structure used for vec  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_PULL_ENABLE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_PULL_ENABLE_OFFSET    0x14           /**<  \brief (AON_GP_REGS_AON_PULL_ENABLE offset) Active Low Always On Pull Enable Control */
#define AON_GP_REGS_AON_PULL_ENABLE_RESETVALUE 0x00ul         /**<  \brief (AON_GP_REGS_AON_PULL_ENABLE reset_value) Active Low Always On Pull Enable Control */

#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_0_Pos 0  /**< \brief (AON_GP_REGS_AON_PULL_ENABLE) Pull Enable for AO_GPIO_0      */
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_0 (0x1ul << AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_0_Pos)  
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_1_Pos 1  /**< \brief (AON_GP_REGS_AON_PULL_ENABLE) Pull Enable for AO_GPIO_1      */
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_1 (0x1ul << AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_1_Pos)  
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_2_Pos 2  /**< \brief (AON_GP_REGS_AON_PULL_ENABLE) Pull Enable for AO_GPIO_2      */
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_2 (0x1ul << AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_2_Pos)  
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO__Pos 0  /**< \brief (AON_GP_REGS_AON_PULL_ENABLE) Pull Enable for AO_GPIO_2 */
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO__Msk (0x7ul << AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO__Pos)  
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_(value) (AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO__Msk & ((value) << AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO__Pos))  
#define AON_GP_REGS_AON_PULL_ENABLE_MASK      0x07ul    /**< \brief (AON_GP_REGS_AON_PULL_ENABLE) Register MASK */

/* -------- AON_GP_REGS_AON_RESET_CTRL : (AON_GP_REGS Offset: 0x1c) (R/W  32) Reset Count Control for PD1, PD4, PD6 and PD7 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint32_t :4;                        /*!< bit:   0..3  Reserved                                 */
    uint32_t PD1_COUNT:4;               /*!< bit:   4..7  Number of clock cycles to delay the PD1 reset. (also for PD7) Upper 4 MSBs only (bits 3:0 will be forced to 0) */
    uint32_t :4;                        /*!< bit:  8..11  Reserved                                 */
    uint32_t PD4_COUNT:4;               /*!< bit: 12..15  Number of clock cycles to delay the PD4 reset. Upper 4 MSBs only (bits 3:0 will be forced to 0) */
    uint32_t PD6_COUNT:4;               /*!< bit: 16..19  Number of clock cycles to delay the PD6 reset */
    uint32_t :12;                       /*!< bit: 20..31  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint32_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_RESET_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_RESET_CTRL_OFFSET     0x1C           /**<  \brief (AON_GP_REGS_AON_RESET_CTRL offset) Reset Count Control for PD1, PD4, PD6 and PD7 */
#define AON_GP_REGS_AON_RESET_CTRL_RESETVALUE 0x3A0A0ul      /**<  \brief (AON_GP_REGS_AON_RESET_CTRL reset_value) Reset Count Control for PD1, PD4, PD6 and PD7 */

#define AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_Pos 4  /**< \brief (AON_GP_REGS_AON_RESET_CTRL) Number of clock cycles to delay the PD1 reset. (also for PD7) Upper 4 MSBs only (bits 3:0 will be forced to 0) */
#define AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_Msk (0xFul << AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_Pos)
#define AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT(value) (AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_Msk & ((value) << AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_Pos))  
#define AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_Pos 12  /**< \brief (AON_GP_REGS_AON_RESET_CTRL) Number of clock cycles to delay the PD4 reset. Upper 4 MSBs only (bits 3:0 will be forced to 0) */
#define AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_Msk (0xFul << AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_Pos)
#define AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT(value) (AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_Msk & ((value) << AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_Pos))  
#define AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_Pos 16  /**< \brief (AON_GP_REGS_AON_RESET_CTRL) Number of clock cycles to delay the PD6 reset */
#define AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_Msk (0xFul << AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_Pos)
#define AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT(value) (AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_Msk & ((value) << AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_Pos))  
#define AON_GP_REGS_AON_RESET_CTRL_MASK       0xFF0F0ul    /**< \brief (AON_GP_REGS_AON_RESET_CTRL) Register MASK */

/* -------- AON_GP_REGS_AON_BTRIM_ACTIVE : (AON_GP_REGS Offset: 0x20) (R/W  8) BTRIM settings for active mode (i.e. not in retention) -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  :8;                        /*!< bit:   0..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_BTRIM_ACTIVE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_BTRIM_ACTIVE_OFFSET   0x20           /**<  \brief (AON_GP_REGS_AON_BTRIM_ACTIVE offset) BTRIM settings for active mode (i.e. not in retention) */
#define AON_GP_REGS_AON_BTRIM_ACTIVE_RESETVALUE 0x08ul         /**<  \brief (AON_GP_REGS_AON_BTRIM_ACTIVE reset_value) BTRIM settings for active mode (i.e. not in retention) */

#define AON_GP_REGS_AON_BTRIM_ACTIVE_MASK     0x00ul    /**< \brief (AON_GP_REGS_AON_BTRIM_ACTIVE) Register MASK */

/* -------- AON_GP_REGS_AON_BTRIM_RETENTION : (AON_GP_REGS Offset: 0x24) (R/W  8) BTRIM settings for retention mode -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  :8;                        /*!< bit:   0..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_BTRIM_RETENTION_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_BTRIM_RETENTION_OFFSET 0x24           /**<  \brief (AON_GP_REGS_AON_BTRIM_RETENTION offset) BTRIM settings for retention mode */
#define AON_GP_REGS_AON_BTRIM_RETENTION_RESETVALUE 0x08ul         /**<  \brief (AON_GP_REGS_AON_BTRIM_RETENTION reset_value) BTRIM settings for retention mode */

#define AON_GP_REGS_AON_BTRIM_RETENTION_MASK  0x00ul    /**< \brief (AON_GP_REGS_AON_BTRIM_RETENTION) Register MASK */

/* -------- AON_GP_REGS_AON_LPMCU_SCRATCH_PAD : (AON_GP_REGS Offset: 0x40) (R/W  8) Usage for the LPMCU for any sort of status it needs to store for itself before it goes to sleep -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  :8;                        /*!< bit:   0..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_LPMCU_SCRATCH_PAD_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_LPMCU_SCRATCH_PAD_OFFSET 0x40           /**<  \brief (AON_GP_REGS_AON_LPMCU_SCRATCH_PAD offset) Usage for the LPMCU for any sort of status it needs to store for itself before it goes to sleep */
#define AON_GP_REGS_AON_LPMCU_SCRATCH_PAD_RESETVALUE 0x00ul         /**<  \brief (AON_GP_REGS_AON_LPMCU_SCRATCH_PAD reset_value) Usage for the LPMCU for any sort of status it needs to store for itself before it goes to sleep */

#define AON_GP_REGS_AON_LPMCU_SCRATCH_PAD_MASK 0x00ul    /**< \brief (AON_GP_REGS_AON_LPMCU_SCRATCH_PAD) Register MASK */

/* -------- AON_GP_REGS_AON_LPMCU_COLD_BOOT : (AON_GP_REGS Offset: 0x44) (R/W  8) To be used by ARM to determine if it is a cold boot or not -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  :8;                        /*!< bit:   0..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_LPMCU_COLD_BOOT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_LPMCU_COLD_BOOT_OFFSET 0x44           /**<  \brief (AON_GP_REGS_AON_LPMCU_COLD_BOOT offset) To be used by ARM to determine if it is a cold boot or not */
#define AON_GP_REGS_AON_LPMCU_COLD_BOOT_RESETVALUE 0x78ul         /**<  \brief (AON_GP_REGS_AON_LPMCU_COLD_BOOT reset_value) To be used by ARM to determine if it is a cold boot or not */

#define AON_GP_REGS_AON_LPMCU_COLD_BOOT_MASK  0x00ul    /**< \brief (AON_GP_REGS_AON_LPMCU_COLD_BOOT) Register MASK */

/* -------- AON_GP_REGS_AON_BO_OUT_STATUS : (AON_GP_REGS Offset: 0x80) (R/  8) Brown Out Detected (must be cleared manually) -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  :8;                        /*!< bit:   0..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_AON_BO_OUT_STATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_AON_BO_OUT_STATUS_OFFSET  0x80           /**<  \brief (AON_GP_REGS_AON_BO_OUT_STATUS offset) Brown Out Detected (must be cleared manually) */
#define AON_GP_REGS_AON_BO_OUT_STATUS_RESETVALUE 0x00ul         /**<  \brief (AON_GP_REGS_AON_BO_OUT_STATUS reset_value) Brown Out Detected (must be cleared manually) */

#define AON_GP_REGS_AON_BO_OUT_STATUS_MASK    0x00ul    /**< \brief (AON_GP_REGS_AON_BO_OUT_STATUS) Register MASK */

/* -------- AON_GP_REGS_CLEAR_BROWN_OUT_REG : (AON_GP_REGS Offset: 0x84) (R/W  8) Set to 1 to clear (hold until 0 read at aon_bo_out_status and then this must be cleared to detect another brown out condition) -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  :8;                        /*!< bit:   0..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_CLEAR_BROWN_OUT_REG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_CLEAR_BROWN_OUT_REG_OFFSET 0x84           /**<  \brief (AON_GP_REGS_CLEAR_BROWN_OUT_REG offset) Set to 1 to clear (hold until 0 read at aon_bo_out_status and then this must be cleared to detect another brown out condition) */
#define AON_GP_REGS_CLEAR_BROWN_OUT_REG_RESETVALUE 0x00ul         /**<  \brief (AON_GP_REGS_CLEAR_BROWN_OUT_REG reset_value) Set to 1 to clear (hold until 0 read at aon_bo_out_status and then this must be cleared to detect another brown out condition) */

#define AON_GP_REGS_CLEAR_BROWN_OUT_REG_MASK  0x00ul    /**< \brief (AON_GP_REGS_CLEAR_BROWN_OUT_REG) Register MASK */

/* -------- AON_GP_REGS_RF_PMU_REGS_0 : (AON_GP_REGS Offset: 0x400) (R/W  32) RF PMU Registers -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint32_t REG1:8;                    /*!< bit:   0..7  RF PMU Register 1                        */
    uint32_t REG2:8;                    /*!< bit:  8..15  RF PMU Register 2                        */
    uint32_t REG3:8;                    /*!< bit: 16..23  RF PMU Register 3                        */
    uint32_t REG4:8;                    /*!< bit: 24..31  RF PMU Register 4                        */
  } bit;                                /*!< Structure used for bit  access                        */
  uint32_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_RF_PMU_REGS_0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_RF_PMU_REGS_0_OFFSET      0x400          /**<  \brief (AON_GP_REGS_RF_PMU_REGS_0 offset) RF PMU Registers */
#define AON_GP_REGS_RF_PMU_REGS_0_RESETVALUE  0xF600409ul    /**<  \brief (AON_GP_REGS_RF_PMU_REGS_0 reset_value) RF PMU Registers */

#define AON_GP_REGS_RF_PMU_REGS_0_REG1_Pos    0  /**< \brief (AON_GP_REGS_RF_PMU_REGS_0) RF PMU Register 1                */
#define AON_GP_REGS_RF_PMU_REGS_0_REG1_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_0_REG1_Pos)
#define AON_GP_REGS_RF_PMU_REGS_0_REG1(value) (AON_GP_REGS_RF_PMU_REGS_0_REG1_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_0_REG1_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_0_REG2_Pos    8  /**< \brief (AON_GP_REGS_RF_PMU_REGS_0) RF PMU Register 2                */
#define AON_GP_REGS_RF_PMU_REGS_0_REG2_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_0_REG2_Pos)
#define AON_GP_REGS_RF_PMU_REGS_0_REG2(value) (AON_GP_REGS_RF_PMU_REGS_0_REG2_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_0_REG2_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_0_REG3_Pos    16  /**< \brief (AON_GP_REGS_RF_PMU_REGS_0) RF PMU Register 3                */
#define AON_GP_REGS_RF_PMU_REGS_0_REG3_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_0_REG3_Pos)
#define AON_GP_REGS_RF_PMU_REGS_0_REG3(value) (AON_GP_REGS_RF_PMU_REGS_0_REG3_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_0_REG3_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_0_REG4_Pos    24  /**< \brief (AON_GP_REGS_RF_PMU_REGS_0) RF PMU Register 4                */
#define AON_GP_REGS_RF_PMU_REGS_0_REG4_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_0_REG4_Pos)
#define AON_GP_REGS_RF_PMU_REGS_0_REG4(value) (AON_GP_REGS_RF_PMU_REGS_0_REG4_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_0_REG4_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_0_MASK        0xFFFFFFFFul    /**< \brief (AON_GP_REGS_RF_PMU_REGS_0) Register MASK */

/* -------- AON_GP_REGS_RF_PMU_REGS_1 : (AON_GP_REGS Offset: 0x404) (R/W  32) RF PMU Registers -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint32_t REG5:8;                    /*!< bit:   0..7  RF PMU Register 5                        */
    uint32_t REG6:8;                    /*!< bit:  8..15  RF PMU Register 6                        */
    uint32_t REG7:8;                    /*!< bit: 16..23  RF PMU Register 7                        */
    uint32_t REG8:8;                    /*!< bit: 24..31  RF PMU Register 8                        */
  } bit;                                /*!< Structure used for bit  access                        */
  uint32_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_RF_PMU_REGS_1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_RF_PMU_REGS_1_OFFSET      0x404          /**<  \brief (AON_GP_REGS_RF_PMU_REGS_1 offset) RF PMU Registers */
#define AON_GP_REGS_RF_PMU_REGS_1_RESETVALUE  0x31888C82ul   /**<  \brief (AON_GP_REGS_RF_PMU_REGS_1 reset_value) RF PMU Registers */

#define AON_GP_REGS_RF_PMU_REGS_1_REG5_Pos    0  /**< \brief (AON_GP_REGS_RF_PMU_REGS_1) RF PMU Register 5                */
#define AON_GP_REGS_RF_PMU_REGS_1_REG5_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_1_REG5_Pos)
#define AON_GP_REGS_RF_PMU_REGS_1_REG5(value) (AON_GP_REGS_RF_PMU_REGS_1_REG5_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_1_REG5_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_1_REG6_Pos    8  /**< \brief (AON_GP_REGS_RF_PMU_REGS_1) RF PMU Register 6                */
#define AON_GP_REGS_RF_PMU_REGS_1_REG6_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_1_REG6_Pos)
#define AON_GP_REGS_RF_PMU_REGS_1_REG6(value) (AON_GP_REGS_RF_PMU_REGS_1_REG6_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_1_REG6_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_1_REG7_Pos    16  /**< \brief (AON_GP_REGS_RF_PMU_REGS_1) RF PMU Register 7                */
#define AON_GP_REGS_RF_PMU_REGS_1_REG7_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_1_REG7_Pos)
#define AON_GP_REGS_RF_PMU_REGS_1_REG7(value) (AON_GP_REGS_RF_PMU_REGS_1_REG7_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_1_REG7_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_1_REG8_Pos    24  /**< \brief (AON_GP_REGS_RF_PMU_REGS_1) RF PMU Register 8                */
#define AON_GP_REGS_RF_PMU_REGS_1_REG8_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_1_REG8_Pos)
#define AON_GP_REGS_RF_PMU_REGS_1_REG8(value) (AON_GP_REGS_RF_PMU_REGS_1_REG8_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_1_REG8_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_1_MASK        0xFFFFFFFFul    /**< \brief (AON_GP_REGS_RF_PMU_REGS_1) Register MASK */

/* -------- AON_GP_REGS_RF_PMU_REGS_2 : (AON_GP_REGS Offset: 0x408) (R/W  16) RF PMU Registers -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint16_t REG9:8;                    /*!< bit:   0..7  RF PMU Register 9                        */
    uint16_t REG10:8;                   /*!< bit:  8..15  RF PMU Register 10                       */
  } bit;                                /*!< Structure used for bit  access                        */
  uint16_t reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_RF_PMU_REGS_2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_RF_PMU_REGS_2_OFFSET      0x408          /**<  \brief (AON_GP_REGS_RF_PMU_REGS_2 offset) RF PMU Registers */
#define AON_GP_REGS_RF_PMU_REGS_2_RESETVALUE  0x05ul         /**<  \brief (AON_GP_REGS_RF_PMU_REGS_2 reset_value) RF PMU Registers */

#define AON_GP_REGS_RF_PMU_REGS_2_REG9_Pos    0  /**< \brief (AON_GP_REGS_RF_PMU_REGS_2) RF PMU Register 9                */
#define AON_GP_REGS_RF_PMU_REGS_2_REG9_Msk    (0xFFul << AON_GP_REGS_RF_PMU_REGS_2_REG9_Pos)
#define AON_GP_REGS_RF_PMU_REGS_2_REG9(value) (AON_GP_REGS_RF_PMU_REGS_2_REG9_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_2_REG9_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_2_REG10_Pos   8  /**< \brief (AON_GP_REGS_RF_PMU_REGS_2) RF PMU Register 10               */
#define AON_GP_REGS_RF_PMU_REGS_2_REG10_Msk   (0xFFul << AON_GP_REGS_RF_PMU_REGS_2_REG10_Pos)
#define AON_GP_REGS_RF_PMU_REGS_2_REG10(value) (AON_GP_REGS_RF_PMU_REGS_2_REG10_Msk & ((value) << AON_GP_REGS_RF_PMU_REGS_2_REG10_Pos))  
#define AON_GP_REGS_RF_PMU_REGS_2_MASK        0xFFFFul    /**< \brief (AON_GP_REGS_RF_PMU_REGS_2) Register MASK */

/* -------- AON_GP_REGS_MS_GPIO_MODE : (AON_GP_REGS Offset: 0x410) (R/W  8) Analog Mode Select of Mixed Signal GPIOs -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  ANALOG_ENABLE_44:1;        /*!< bit:      0  Active High Analog Mode Enable for Mixed Signal LP_GPIO_44 */
    uint8_t  ANALOG_ENABLE_45:1;        /*!< bit:      1  Active High Analog Mode Enable for Mixed Signal LP_GPIO_45 */
    uint8_t  ANALOG_ENABLE_46:1;        /*!< bit:      2  Active High Analog Mode Enable for Mixed Signal LP_GPIO_46 */
    uint8_t  ANALOG_ENABLE_47:1;        /*!< bit:      3  Active High Analog Mode Enable for Mixed Signal LP_GPIO_47 */
    uint8_t  :4;                        /*!< bit:   4..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  struct {
    uint8_t  ANALOG_ENABLE_:4;          /*!< bit:   0..3  Active High Analog Mode Enable for Mixed Signal LP_GPIO_47 */
    uint8_t  :4;                        /*!< bit:   4..7 Reserved                                  */
  } vec;                                /*!< Structure used for vec  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_MS_GPIO_MODE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_MS_GPIO_MODE_OFFSET       0x410          /**<  \brief (AON_GP_REGS_MS_GPIO_MODE offset) Analog Mode Select of Mixed Signal GPIOs */
#define AON_GP_REGS_MS_GPIO_MODE_RESETVALUE   0x0Ful         /**<  \brief (AON_GP_REGS_MS_GPIO_MODE reset_value) Analog Mode Select of Mixed Signal GPIOs */

#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_44_Pos 0  /**< \brief (AON_GP_REGS_MS_GPIO_MODE) Active High Analog Mode Enable for Mixed Signal LP_GPIO_44 */
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_44 (0x1ul << AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_44_Pos)  
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_45_Pos 1  /**< \brief (AON_GP_REGS_MS_GPIO_MODE) Active High Analog Mode Enable for Mixed Signal LP_GPIO_45 */
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_45 (0x1ul << AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_45_Pos)  
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_46_Pos 2  /**< \brief (AON_GP_REGS_MS_GPIO_MODE) Active High Analog Mode Enable for Mixed Signal LP_GPIO_46 */
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_46 (0x1ul << AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_46_Pos)  
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_47_Pos 3  /**< \brief (AON_GP_REGS_MS_GPIO_MODE) Active High Analog Mode Enable for Mixed Signal LP_GPIO_47 */
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_47 (0x1ul << AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_47_Pos)  
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE__Pos 0  /**< \brief (AON_GP_REGS_MS_GPIO_MODE) Active High Analog Mode Enable for Mixed Signal LP_GPIO_47 */
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE__Msk (0xFul << AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE__Pos)  
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_(value) (AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE__Msk & ((value) << AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE__Pos))  
#define AON_GP_REGS_MS_GPIO_MODE_MASK         0x0Ful    /**< \brief (AON_GP_REGS_MS_GPIO_MODE) Register MASK */

/* -------- AON_GP_REGS_IO_PADS_CONTROL : (AON_GP_REGS Offset: 0x414) (R/W  8) Controls behaviour of IO Pads in Sleep Mode -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { 
  struct {
    uint8_t  LATCH_ENABLE:1;            /*!< bit:      0  Active High Latch Enable of IO Pads configuration parameters. When Low, Pads retain their configured state during sleep mode. */
    uint8_t  :7;                        /*!< bit:   1..7  Reserved                                 */
  } bit;                                /*!< Structure used for bit  access                        */
  uint8_t  reg;                         /*!< Type used for register access                         */
} AON_GP_REGS_IO_PADS_CONTROL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define AON_GP_REGS_IO_PADS_CONTROL_OFFSET    0x414          /**<  \brief (AON_GP_REGS_IO_PADS_CONTROL offset) Controls behaviour of IO Pads in Sleep Mode */
#define AON_GP_REGS_IO_PADS_CONTROL_RESETVALUE 0x01ul         /**<  \brief (AON_GP_REGS_IO_PADS_CONTROL reset_value) Controls behaviour of IO Pads in Sleep Mode */

#define AON_GP_REGS_IO_PADS_CONTROL_LATCH_ENABLE_Pos 0  /**< \brief (AON_GP_REGS_IO_PADS_CONTROL) Active High Latch Enable of IO Pads configuration parameters. When Low, Pads retain their configured state during sleep mode. */
#define AON_GP_REGS_IO_PADS_CONTROL_LATCH_ENABLE (0x1ul << AON_GP_REGS_IO_PADS_CONTROL_LATCH_ENABLE_Pos)  
#define AON_GP_REGS_IO_PADS_CONTROL_MASK      0x01ul    /**< \brief (AON_GP_REGS_IO_PADS_CONTROL) Register MASK */

/** \brief AON_GP_REGS hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {  /* Always On General Purpose Registers */
  __IO AON_GP_REGS_AON_PINMUX_SEL_Type AON_PINMUX_SEL; /**< \brief Offset: 0x00 (R/W  16) Controls the pinmux selection for the AO GPIOs */
       RoReg8                         Reserved1[0x2];
  __IO AON_GP_REGS_AON_PMU_CTRL_Type  AON_PMU_CTRL;   /**< \brief Offset: 0x04 (R/W  32) Always On Misc Control */
  __IO AON_GP_REGS_AON_BLE_LP_CTRL_Type AON_BLE_LP_CTRL; /**< \brief Offset: 0x08 (R/W   8) Always On BLE LP Control */
       RoReg8                         Reserved2[0x3];
  __IO AON_GP_REGS_AON_MISC_CTRL_Type AON_MISC_CTRL;  /**< \brief Offset: 0x0C (R/W  32) Always On Misc Control */
  __IO AON_GP_REGS_AON_GLOBAL_RESET_Type AON_GLOBAL_RESET; /**< \brief Offset: 0x10 (R/W   8) Active Low Always On Reset Control */
       RoReg8                         Reserved3[0x3];
  __IO AON_GP_REGS_AON_PULL_ENABLE_Type AON_PULL_ENABLE; /**< \brief Offset: 0x14 (R/W   8) Active Low Always On Pull Enable Control */
       RoReg8                         Reserved4[0x7];
  __IO AON_GP_REGS_AON_RESET_CTRL_Type AON_RESET_CTRL; /**< \brief Offset: 0x1C (R/W  32) Reset Count Control for PD1, PD4, PD6 and PD7 */
  __IO AON_GP_REGS_AON_BTRIM_ACTIVE_Type AON_BTRIM_ACTIVE; /**< \brief Offset: 0x20 (R/W   8) BTRIM settings for active mode (i.e. not in retention) */
       RoReg8                         Reserved5[0x3];
  __IO AON_GP_REGS_AON_BTRIM_RETENTION_Type AON_BTRIM_RETENTION; /**< \brief Offset: 0x24 (R/W   8) BTRIM settings for retention mode */
       RoReg8                         Reserved6[0x1B];
  __IO AON_GP_REGS_AON_LPMCU_SCRATCH_PAD_Type AON_LPMCU_SCRATCH_PAD; /**< \brief Offset: 0x40 (R/W   8) Usage for the LPMCU for any sort of status it needs to store for itself before it goes to sleep */
       RoReg8                         Reserved7[0x3];
  __IO AON_GP_REGS_AON_LPMCU_COLD_BOOT_Type AON_LPMCU_COLD_BOOT; /**< \brief Offset: 0x44 (R/W   8) To be used by ARM to determine if it is a cold boot or not */
       RoReg8                         Reserved8[0x3B];
  __I  AON_GP_REGS_AON_BO_OUT_STATUS_Type AON_BO_OUT_STATUS; /**< \brief Offset: 0x80 (R/    8) Brown Out Detected (must be cleared manually) */
       RoReg8                         Reserved9[0x3];
  __IO AON_GP_REGS_CLEAR_BROWN_OUT_REG_Type CLEAR_BROWN_OUT_REG; /**< \brief Offset: 0x84 (R/W   8) Set to 1 to clear (hold until 0 read at aon_bo_out_status and then this must be cleared to detect another brown out condition) */
       RoReg8                         Reserved10[0x37B];
  __IO AON_GP_REGS_RF_PMU_REGS_0_Type RF_PMU_REGS_0;  /**< \brief Offset: 0x400 (R/W  32) RF PMU Registers */
  __IO AON_GP_REGS_RF_PMU_REGS_1_Type RF_PMU_REGS_1;  /**< \brief Offset: 0x404 (R/W  32) RF PMU Registers */
  __IO AON_GP_REGS_RF_PMU_REGS_2_Type RF_PMU_REGS_2;  /**< \brief Offset: 0x408 (R/W  16) RF PMU Registers */
       RoReg8                         Reserved11[0x6];
  __IO AON_GP_REGS_MS_GPIO_MODE_Type  MS_GPIO_MODE;   /**< \brief Offset: 0x410 (R/W   8) Analog Mode Select of Mixed Signal GPIOs */
       RoReg8                         Reserved12[0x3];
  __IO AON_GP_REGS_IO_PADS_CONTROL_Type IO_PADS_CONTROL; /**< \brief Offset: 0x414 (R/W   8) Controls behaviour of IO Pads in Sleep Mode */
} AonGpRegs;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMB11_AON_GP_REGS_COMPONENT_ */
