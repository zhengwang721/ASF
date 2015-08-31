/* **************************** */
#define AON_PWR_SEQ_REGS_GPIO_WAKEUP_CTRL                                           (AON_PWR_SEQ_REGS_BASE + 0x0)
#define AON_PWR_SEQ_REGS_GPIO_WAKEUP_CTRL_DEFAULT                                   0x2
#define AON_PWR_SEQ_REGS_GPIO_WAKEUP_CTRL_BLE_ENABLE                                (Bit1)


#define AON_PWR_SEQ_REGS_AON_ST_WAKEUP_CTRL                                         (AON_PWR_SEQ_REGS_BASE + 0xc)
#define AON_PWR_SEQ_REGS_AON_ST_WAKEUP_CTRL_DEFAULT                                 0x0
#define AON_PWR_SEQ_REGS_AON_ST_WAKEUP_CTRL_ARM_ENABLE                              (Bit0)
#define AON_PWR_SEQ_REGS_AON_ST_WAKEUP_CTRL_BLE_ENABLE                              (Bit1)


#define AON_PWR_SEQ_REGS_LPMCU_WAKEUP_CTRL                                          (AON_PWR_SEQ_REGS_BASE + 0x10)
#define AON_PWR_SEQ_REGS_LPMCU_WAKEUP_CTRL_DEFAULT                                  0x0
#define AON_PWR_SEQ_REGS_LPMCU_WAKEUP_CTRL_BLE_ENABLE                               (Bit6)
#define AON_PWR_SEQ_REGS_LPMCU_WAKEUP_CTRL_PD7_ENABLE                               (Bit9)


#define AON_PWR_SEQ_REGS_BLE_ST_WAKEUP_CTRL                                         (AON_PWR_SEQ_REGS_BASE + 0x14)
#define AON_PWR_SEQ_REGS_BLE_ST_WAKEUP_CTRL_DEFAULT                                 0x0
#define AON_PWR_SEQ_REGS_BLE_ST_WAKEUP_CTRL_ARM_ENABLE                              (Bit0)
#define AON_PWR_SEQ_REGS_BLE_ST_WAKEUP_CTRL_BLE_ENABLE                              (Bit1)


#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL                                         (AON_PWR_SEQ_REGS_BASE + 0x20)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_DEFAULT                                 0x0
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_ARM_SLEEP_EN                            (Bit0)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_BLE_SLEEP_EN                            (Bit6)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD6_SLEEP_EN                            (Bit8)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD7_SLEEP_EN                            (Bit9)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD2A_RETENTION_EN                       (Bit16)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD2B_RETENTION_EN                       (Bit17)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD3_RETENTION_EN                        (Bit18)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD5_RETENTION_EN                        (Bit21)
#define AON_PWR_SEQ_REGS_LPMCU_SLEEP_1_CTRL_PD8_RETENTION_EN                        (Bit22)


#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL                                           (AON_PWR_SEQ_REGS_BASE + 0x40)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_DEFAULT                                   0x4030001
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_CLOCK_EN_EARLY                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_CLOCK_EN_EARLY_MASK                       (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_CLOCK_EN_EARLY_SHIFT                      (0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_ISOLATE                                   (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_ISOLATE_MASK                              (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_ISOLATE_SHIFT                             (16)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_RESETN                                    (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_RESETN_MASK                               (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_0_CTRL_RESETN_SHIFT                              (24)


#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL                                           (AON_PWR_SEQ_REGS_BASE + 0x44)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_DEFAULT                                   0x8070605
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_BTRIM                                     (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_BTRIM_MASK                                (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_BTRIM_SHIFT                               (0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_STDBY_N                                   (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_STDBY_N_MASK                              (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_STDBY_N_SHIFT                             (8)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_POWER_VDD                                 (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_POWER_VDD_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_POWER_VDD_SHIFT                           (16)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_POWER_VDM                                 (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_POWER_VDM_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_OFF_DELAY_1_CTRL_POWER_VDM_SHIFT                           (24)


#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL                                            (AON_PWR_SEQ_REGS_BASE + 0x48)
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_DEFAULT                                    0x2727
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_CLOCK_EN_EARLY_ARM                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_CLOCK_EN_EARLY_ARM_MASK                    (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_CLOCK_EN_EARLY_ARM_SHIFT                   (0)
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_CLOCK_EN_EARLY_BLE                         (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_CLOCK_EN_EARLY_BLE_MASK                    (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_ON_DELAY_0_CTRL_CLOCK_EN_EARLY_BLE_SHIFT                   (8)


#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL                                            (AON_PWR_SEQ_REGS_BASE + 0x4c)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_DEFAULT                                    0x1020304
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_BTRIM                                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_BTRIM_MASK                                 (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_BTRIM_SHIFT                                (0)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_STDBY_N                                    (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_STDBY_N_MASK                               (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_STDBY_N_SHIFT                              (8)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_POWER_VDD                                  (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_POWER_VDD_MASK                             (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_POWER_VDD_SHIFT                            (16)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_POWER_VDM                                  (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_POWER_VDM_MASK                             (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_ON_DELAY_1_CTRL_POWER_VDM_SHIFT                            (24)


#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL                                     (AON_PWR_SEQ_REGS_BASE + 0x50)
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_DEFAULT                             0x2106
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_OFF                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_OFF_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_OFF_SHIFT                           (0)
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_ON                                  (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_ON_MASK                             (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_VDD_DCDC_EN_DELAY_CTRL_ON_SHIFT                            (8)


#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL                                         (AON_PWR_SEQ_REGS_BASE + 0x218)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_DEFAULT                                 0x88
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_PD0_LOW_VOLTAGE_EN_VAL                  (Bit2)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_PD0_LOW_VOLTAGE_EN_EN                   (Bit3)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_ENABLE_CLOCK_GATE                       (Bit4)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_VDD_DCDC_EN_VAL                         (Bit6)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_VDD_DCDC_EN_EN                          (Bit7)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_FORCE_ON_BLE_CORE                       (Bit8)
#define AON_PWR_SEQ_REGS_MISC_BYPASS_0_CTRL_FORCE_ON_BLE_MEMORIES                   (Bit9)


#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS                                            (AON_PWR_SEQ_REGS_BASE + 0x300)
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_DEFAULT                                    0x0
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_GPIO_0                                     (Bit0)
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_GPIO_1                                     (Bit1)
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_GPIO_2                                     (Bit2)
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_AON_ST                                     (Bit3)
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_LPMCU                                      (Bit4)
#define AON_PWR_SEQ_REGS_RAW_WAKEUP_BITS_BLE_ST                                     (Bit5)


#define AON_PWR_SEQ_REGS_PD_WAKEUP_BITS                                             (AON_PWR_SEQ_REGS_BASE + 0x304)
#define AON_PWR_SEQ_REGS_PD_WAKEUP_BITS_DEFAULT                                     0x0
#define AON_PWR_SEQ_REGS_PD_WAKEUP_BITS_ARM                                         (Bit0)
#define AON_PWR_SEQ_REGS_PD_WAKEUP_BITS_BLE                                         (Bit6)
#define AON_PWR_SEQ_REGS_PD_WAKEUP_BITS_PD6                                         (Bit8)
#define AON_PWR_SEQ_REGS_PD_WAKEUP_BITS_PD7                                         (Bit9)


#define AON_PWR_SEQ_REGS_SERVICED_REQUEST                                           (AON_PWR_SEQ_REGS_BASE + 0x308)
#define AON_PWR_SEQ_REGS_SERVICED_REQUEST_DEFAULT                                   0x0
#define AON_PWR_SEQ_REGS_SERVICED_REQUEST_GPIO_WAKEUP                               (Bit0)
#define AON_PWR_SEQ_REGS_SERVICED_REQUEST_AON_ST_WAKEUP                             (Bit3)
#define AON_PWR_SEQ_REGS_SERVICED_REQUEST_LPMCU_WAKEUP                              (Bit4)
#define AON_PWR_SEQ_REGS_SERVICED_REQUEST_BLE_ST_WAKEUP                             (Bit5)
#define AON_PWR_SEQ_REGS_SERVICED_REQUEST_SLEEP_1                                   (Bit8)


#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST                                             (AON_PWR_SEQ_REGS_BASE + 0x30c)
#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST_DEFAULT                                     0x0
#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST_GPIO_WAKEUP                                 (Bit0)
#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST_AON_ST_WAKEUP                               (Bit3)
#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST_LPMCU_WAKEUP                                (Bit4)
#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST_BLE_ST_WAKEUP                               (Bit5)
#define AON_PWR_SEQ_REGS_ACTIVE_REQUEST_SLEEP_1                                     (Bit8)


#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES                                           (AON_PWR_SEQ_REGS_BASE + 0x3f0)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_DEFAULT                                   0x0
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_MAIN_FSM_STATE                            (Bit0+Bit1+Bit2)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_MAIN_FSM_STATE_MASK                       (Bit2+Bit1+Bit0)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_MAIN_FSM_STATE_SHIFT                      (0)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_BLE_ON_STATE                              (Bit8)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_BLE_OFF_STATE                             (Bit9)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_PD6_ON_STATE                              (Bit12)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_PD6_OFF_STATE                             (Bit13)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_PD7_ON_STATE                              (Bit16)
#define AON_PWR_SEQ_REGS_LOGIC_FSM_STATES_PD7_OFF_STATE                             (Bit17)

