/* **************************** */
#define AON_GP_REGS_AON_PINMUX_SEL                                               (AON_GP_REGS_BASE + 0x0)
#define AON_GP_REGS_AON_PINMUX_SEL_DEFAULT                                       0x1
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL                                 (Bit0+Bit1)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_MASK                            (Bit1+Bit0)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_0_SEL_SHIFT                           (0)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL                                 (Bit4+Bit5)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_MASK                            (Bit1+Bit0)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_1_SEL_SHIFT                           (4)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL                                 (Bit8+Bit9)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_MASK                            (Bit1+Bit0)
#define AON_GP_REGS_AON_PINMUX_SEL_AO_GPIO_2_SEL_SHIFT                           (8)


#define AON_GP_REGS_AON_PMU_CTRL                                                 (AON_GP_REGS_BASE + 0x4)
#define AON_GP_REGS_AON_PMU_CTRL_DEFAULT                                         0xA0022
#define AON_GP_REGS_AON_PMU_CTRL_PMU_REGS_4TO1_SEL                               (Bit0)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RTC_CLK_EN                                  (Bit1)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL                                (Bit4+Bit5)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_MASK                           (Bit1+Bit0)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_RETN_VAL_SEL_SHIFT                          (4)
#define AON_GP_REGS_AON_PMU_CTRL_EFUSE_LDO_EN                                    (Bit7)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_EN                                      (Bit8)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A                                       (Bit9+Bit10+Bit11)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_MASK                                  (Bit2+Bit1+Bit0)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_A_SHIFT                                 (9)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL                                     (Bit12+Bit13+Bit14+Bit15)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_MASK                                (Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_MUX_SEL_SHIFT                               (12)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_EN                                 (Bit16)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_SENS_ADC_RST                                (Bit17)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_BGR_EN                                      (Bit18)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_2MHZ_CLK_EN                                 (Bit19)
#define AON_GP_REGS_AON_PMU_CTRL_PMU_26MHZ_CLK_FORCE_OFF                         (Bit20)


#define AON_GP_REGS_AON_BLE_LP_CTRL                                              (AON_GP_REGS_BASE + 0x8)
#define AON_GP_REGS_AON_BLE_LP_CTRL_DEFAULT                                      0x0
#define AON_GP_REGS_AON_BLE_LP_CTRL_WAKEUP_LP_LATCH                              (Bit0)
#define AON_GP_REGS_AON_BLE_LP_CTRL_DEEP_SLEEP_STAT_SYNC_EN                      (Bit1)


#define AON_GP_REGS_AON_MISC_CTRL                                                (AON_GP_REGS_BASE + 0xc)
#define AON_GP_REGS_AON_MISC_CTRL_DEFAULT                                        0x10000
#define AON_GP_REGS_AON_MISC_CTRL_USE_RTC_32KHZ_CLK_SLEEP_TIMER                  (Bit1)
#define AON_GP_REGS_AON_MISC_CTRL_USE_EXT_32KHZ_CLK_SLEEP_TIMER                  (Bit2)
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_BOOT_RESET_MUXSEL                        (Bit3)
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_USE_BOOT_REGS                            (Bit4)
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_EN                    (Bit5)
#define AON_GP_REGS_AON_MISC_CTRL_LPMCU_CPU_RESET_OVERRIDE_VAL                   (Bit6)
#define AON_GP_REGS_AON_MISC_CTRL_USE_2M_AON_PWR_SEQ_CLK                         (Bit14)
#define AON_GP_REGS_AON_MISC_CTRL_USE_OSC2M_AS_TB_CLK                            (Bit15)
#define AON_GP_REGS_AON_MISC_CTRL_AON_SLEEP_TIMER_CLK_EN                         (Bit16)
#define AON_GP_REGS_AON_MISC_CTRL_AON_EXT_32KHZ_OUT_EN                           (Bit17)
#define AON_GP_REGS_AON_MISC_CTRL_USE_RTC_AON_PWR_SEQ_CLK                        (Bit18)
#define AON_GP_REGS_AON_MISC_CTRL_INVERT_WAKEUP_GPIO_0                           (Bit19)
#define AON_GP_REGS_AON_MISC_CTRL_FORCE_OFF_XO                                   (Bit20)
#define AON_GP_REGS_AON_MISC_CTRL_FORCE_XO_TO_BYPASS_MODE                        (Bit21)


#define AON_GP_REGS_AON_GLOBAL_RESET                                             (AON_GP_REGS_BASE + 0x10)
#define AON_GP_REGS_AON_GLOBAL_RESET_DEFAULT                                     0x1B
#define AON_GP_REGS_AON_GLOBAL_RESET_GLOBAL_RSTN                                 (Bit0)
#define AON_GP_REGS_AON_GLOBAL_RESET_SLEEP_TIMER_RSTN                            (Bit1)
#define AON_GP_REGS_AON_GLOBAL_RESET_BLE_LP_RSTN                                 (Bit3)
#define AON_GP_REGS_AON_GLOBAL_RESET_PD4_RSTN                                    (Bit4)


#define AON_GP_REGS_AON_PULL_ENABLE                                              (AON_GP_REGS_BASE + 0x14)
#define AON_GP_REGS_AON_PULL_ENABLE_DEFAULT                                      0x0
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_0                                    (Bit0)
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_1                                    (Bit1)
#define AON_GP_REGS_AON_PULL_ENABLE_AO_GPIO_2                                    (Bit2)


#define AON_GP_REGS_AON_RESET_CTRL                                               (AON_GP_REGS_BASE + 0x1c)
#define AON_GP_REGS_AON_RESET_CTRL_DEFAULT                                       0x3A0A0
#define AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT                                     (Bit4+Bit5+Bit6+Bit7)
#define AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_MASK                                (Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_AON_RESET_CTRL_PD1_COUNT_SHIFT                               (4)
#define AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT                                     (Bit12+Bit13+Bit14+Bit15)
#define AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_MASK                                (Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_AON_RESET_CTRL_PD4_COUNT_SHIFT                               (12)
#define AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT                                     (Bit16+Bit17+Bit18+Bit19)
#define AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_MASK                                (Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_AON_RESET_CTRL_PD6_COUNT_SHIFT                               (16)


#define AON_GP_REGS_AON_BTRIM_ACTIVE                                             (AON_GP_REGS_BASE + 0x20)
#define AON_GP_REGS_AON_BTRIM_ACTIVE_DEFAULT                                     0x8


#define AON_GP_REGS_AON_BTRIM_RETENTION                                          (AON_GP_REGS_BASE + 0x24)
#define AON_GP_REGS_AON_BTRIM_RETENTION_DEFAULT                                  0x8


#define AON_GP_REGS_AON_LPMCU_SCRATCH_PAD                                        (AON_GP_REGS_BASE + 0x40)
#define AON_GP_REGS_AON_LPMCU_SCRATCH_PAD_DEFAULT                                0x0


#define AON_GP_REGS_AON_LPMCU_COLD_BOOT                                          (AON_GP_REGS_BASE + 0x44)
#define AON_GP_REGS_AON_LPMCU_COLD_BOOT_DEFAULT                                  0x78


#define AON_GP_REGS_AON_BO_OUT_STATUS                                            (AON_GP_REGS_BASE + 0x80)
#define AON_GP_REGS_AON_BO_OUT_STATUS_DEFAULT                                    0x0


#define AON_GP_REGS_CLEAR_BROWN_OUT_REG                                          (AON_GP_REGS_BASE + 0x84)
#define AON_GP_REGS_CLEAR_BROWN_OUT_REG_DEFAULT                                  0x0


#define AON_GP_REGS_RF_PMU_REGS_0                                                (AON_GP_REGS_BASE + 0x400)
#define AON_GP_REGS_RF_PMU_REGS_0_DEFAULT                                        0xF600409
#define AON_GP_REGS_RF_PMU_REGS_0_REG1                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define AON_GP_REGS_RF_PMU_REGS_0_REG1_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_0_REG1_SHIFT                                     (0)
#define AON_GP_REGS_RF_PMU_REGS_0_REG2                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define AON_GP_REGS_RF_PMU_REGS_0_REG2_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_0_REG2_SHIFT                                     (8)
#define AON_GP_REGS_RF_PMU_REGS_0_REG3                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define AON_GP_REGS_RF_PMU_REGS_0_REG3_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_0_REG3_SHIFT                                     (16)
#define AON_GP_REGS_RF_PMU_REGS_0_REG4                                           (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define AON_GP_REGS_RF_PMU_REGS_0_REG4_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_0_REG4_SHIFT                                     (24)


#define AON_GP_REGS_RF_PMU_REGS_1                                                (AON_GP_REGS_BASE + 0x404)
#define AON_GP_REGS_RF_PMU_REGS_1_DEFAULT                                        0x31888C82
#define AON_GP_REGS_RF_PMU_REGS_1_REG5                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define AON_GP_REGS_RF_PMU_REGS_1_REG5_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_1_REG5_SHIFT                                     (0)
#define AON_GP_REGS_RF_PMU_REGS_1_REG6                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define AON_GP_REGS_RF_PMU_REGS_1_REG6_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_1_REG6_SHIFT                                     (8)
#define AON_GP_REGS_RF_PMU_REGS_1_REG7                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define AON_GP_REGS_RF_PMU_REGS_1_REG7_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_1_REG7_SHIFT                                     (16)
#define AON_GP_REGS_RF_PMU_REGS_1_REG8                                           (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define AON_GP_REGS_RF_PMU_REGS_1_REG8_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_1_REG8_SHIFT                                     (24)


#define AON_GP_REGS_RF_PMU_REGS_2                                                (AON_GP_REGS_BASE + 0x408)
#define AON_GP_REGS_RF_PMU_REGS_2_DEFAULT                                        0x5
#define AON_GP_REGS_RF_PMU_REGS_2_REG9                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define AON_GP_REGS_RF_PMU_REGS_2_REG9_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_2_REG9_SHIFT                                     (0)
#define AON_GP_REGS_RF_PMU_REGS_2_REG10                                          (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define AON_GP_REGS_RF_PMU_REGS_2_REG10_MASK                                     (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_GP_REGS_RF_PMU_REGS_2_REG10_SHIFT                                    (8)


#define AON_GP_REGS_MS_GPIO_MODE                                                 (AON_GP_REGS_BASE + 0x410)
#define AON_GP_REGS_MS_GPIO_MODE_DEFAULT                                         0xF
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_44                                (Bit0)
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_45                                (Bit1)
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_46                                (Bit2)
#define AON_GP_REGS_MS_GPIO_MODE_ANALOG_ENABLE_47                                (Bit3)


#define AON_GP_REGS_IO_PADS_CONTROL                                              (AON_GP_REGS_BASE + 0x414)
#define AON_GP_REGS_IO_PADS_CONTROL_DEFAULT                                      0x1
#define AON_GP_REGS_IO_PADS_CONTROL_LATCH_ENABLE                                 (Bit0)

