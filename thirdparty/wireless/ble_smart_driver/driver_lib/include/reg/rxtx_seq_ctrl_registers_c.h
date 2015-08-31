/* **************************** */
#define RXTX_SEQ_CTRL_REGISTERS_MODE_CTRL                                                      (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x0)
#define RXTX_SEQ_CTRL_REGISTERS_MODE_CTRL_DEFAULT                                              0x0
#define RXTX_SEQ_CTRL_REGISTERS_MODE_CTRL_ENABLE                                               (Bit0)


#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL                                                      (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x4)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_DEFAULT                                              0x14800
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_SX_ENA_VAL_IDLE_TO_SLEEP                             (Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_SX_ENA_VAL_SLEEP_TO_IDLE                             (Bit1)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_SX_ENA_VAL_ACTIVE_TO_IDLE                            (Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_SX_ENA_VAL_RESET_TO_IDLE                             (Bit3)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_SX_ENA_VAL_ANY_TO_RESET                              (Bit4)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_TUNER_ENA_VAL_SLEEP_TO_IDLE                          (Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_TUNER_ENA_VAL_ACTIVE_TO_IDLE                         (Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_TUNER_ENA_VAL_RESET_TO_IDLE                          (Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_TUNER_ENA_VAL_ANY_TO_RESET                           (Bit8)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_TUNER_ENA_VAL_IDLE_TO_SLEEP                          (Bit9)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_SXPLL_RESETN_VAL_IDLE_TO_SLEEP                       (Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_BT_RX_ACTIVE_VAL_IDLE_TO_RX                          (Bit14)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_BT_RX_ACTIVE_VAL_RX_TO_IDLE                          (Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_BT_TX_ACTIVE_VAL_IDLE_TO_TX                          (Bit16)
#define RXTX_SEQ_CTRL_REGISTERS_MISC_CTRL_BT_TX_ACTIVE_VAL_TX_TO_IDLE                          (Bit17)


#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL                                                        (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x8)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_DEFAULT                                                0x1C0
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_TX_HAS_PRIORITY_IN_IDLE_STATE                          (Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_TX_HAS_PRIORITY_IN_RX_STATE                            (Bit1)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_TX_HAS_PRIORITY_IN_TX_STATE                            (Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_ENABLE_WAKEUP_ARM                                      (Bit3)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_ENABLE_WAKEUP_SW                                       (Bit4)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_ENABLE_WAKEUP_HW                                       (Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_ONLY_WAKEUP_SIGNAL_FROM_SLEEP                          (Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_IDLE_WHEN_RX_NO_ACTIVE_SIGS                            (Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_IDLE_WHEN_TX_NO_ACTIVE_SIGS                            (Bit8)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_IDLE_TO_RX                                             (Bit9)
#define RXTX_SEQ_CTRL_REGISTERS_OP_CTRL_IDLE_TO_TX                                             (Bit10)


#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL                                                  (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xc)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_DEFAULT                                          0x0
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_RXADC_ENA                                (Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_RXADC_ENA                                 (Bit3)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_RXLNA_ENA                                (Bit4)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_RXLNA_ENA                                 (Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_RXMIXER_ENA                              (Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_RXMIXER_ENA                               (Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_SX_ENA                                   (Bit8)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_SX_ENA                                    (Bit9)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_TUNER_ENA                                (Bit10)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_TUNER_ENA                                 (Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_CLK_ENABLE                               (Bit12)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_CLK_ENABLE                                (Bit13)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_PMA_ENA                                  (Bit14)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_PMA_ENA                                   (Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_TXPA_ENA                                 (Bit16)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_TXPA_ENA                                  (Bit17)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_SXPLL_RESETN                             (Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_SXPLL_RESETN                              (Bit24)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_FE_SOFT_RESET_TOGGLE                     (Bit25)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_FE_SOFT_RESET_TOGGLE                      (Bit26)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_FE_RX_EN                                 (Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_FE_RX_EN                                  (Bit28)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_VAL_FE_TX_EN                                 (Bit29)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_0_CTRL_BYP_EN_FE_TX_EN                                  (Bit30)


#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL                                                  (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x10)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_DEFAULT                                          0x0
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_USE_RF_TX_REGS                           (Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_USE_RF_TX_REGS                            (Bit1)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_GP_TRIGGER_TOGGLE                        (Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_GP_TRIGGER_TOGGLE                         (Bit3)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_BBLNA_ENA                                (Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_BBLNA_ENA                                 (Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_BBLI_ENA                                 (Bit8)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_BBLI_ENA                                  (Bit9)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_BT_RX_ACTIVE                             (Bit10)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_BT_RX_ACTIVE                              (Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_BT_TX_ACTIVE                             (Bit12)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_BT_TX_ACTIVE                              (Bit13)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_VAL_BT_PLL_TRIGGER                           (Bit14)
#define RXTX_SEQ_CTRL_REGISTERS_BYPASS_1_CTRL_BYP_EN_BT_PLL_TRIGGER                            (Bit15)


#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL                                                  (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x14)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_DEFAULT                                          0x0
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_SEL                                              (Bit0+Bit1+Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_SEL_MASK                                         (Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_SEL_SHIFT                                        (0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_STATUS                                           (Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_STATUS_MASK                                      (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_CTRL_STATUS_SHIFT                                     (20)


#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL                                      (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x1c)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL_DEFAULT                              0x70
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL_ENABLE_IDLE_TO_RX                    (Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL_ENABLE_IDLE_TO_TX                    (Bit1)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL_HOLD                                 (Bit4+Bit5+Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL_HOLD_MASK                            (Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_CTRL_HOLD_SHIFT                           (4)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL                                         (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x20)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_DEFAULT                                 0x70000
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_ENABLE_SLEEP_TO_IDLE                    (Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_ENABLE_IDLE_TO_SLEEP                    (Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_ENABLE_IDLE_TO_RX                       (Bit4)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_ENABLE_IDLE_TO_TX                       (Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_ENABLE_RX_TO_IDLE                       (Bit10)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_ENABLE_TX_TO_IDLE                       (Bit14)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_HOLD                                    (Bit16+Bit17+Bit18)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_HOLD_MASK                               (Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_CTRL_HOLD_SHIFT                              (16)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x50)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_DEFAULT                               0x0
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_SLEEP_TO_IDLE                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_SLEEP_TO_IDLE_MASK                    (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_SLEEP_TO_IDLE_SHIFT                   (0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_IDLE_TO_SLEEP                         (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_IDLE_TO_SLEEP_MASK                    (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_0_ADDR_IDLE_TO_SLEEP_SHIFT                   (16)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x54)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_DEFAULT                               0x0
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_IDLE_TO_RX                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_IDLE_TO_RX_MASK                       (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_IDLE_TO_RX_SHIFT                      (0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_IDLE_TO_TX                            (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_IDLE_TO_TX_MASK                       (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_1_ADDR_IDLE_TO_TX_SHIFT                      (16)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x58)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_DEFAULT                               0x0
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_RX_TO_IDLE                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_RX_TO_IDLE_MASK                       (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_RX_TO_IDLE_SHIFT                      (0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_TX_TO_IDLE                            (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_TX_TO_IDLE_MASK                       (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_OFFSET_2_ADDR_TX_TO_IDLE_SHIFT                      (16)


#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL                                           (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xa4)
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_DEFAULT                                   0x10051
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_IDLE_TO_RX                                (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                           (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                          (0)
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_RX_TO_IDLE                                (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_RX_TO_IDLE_MASK                           (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_PMA_ENA_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                          (16)


#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL                                         (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xa8)
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_DEFAULT                                 0x10051
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_IDLE_TO_RX                              (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                         (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                        (0)
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_RX_TO_IDLE                              (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_RX_TO_IDLE_MASK                         (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BBLNA_ENA_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                        (16)


#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xac)
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_DEFAULT                                  0x10051
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_IDLE_TO_RX                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                          (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                         (0)
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_RX_TO_IDLE                               (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_RX_TO_IDLE_MASK                          (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BBLI_ENA_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                         (16)


#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL                                         (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xb4)
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_DEFAULT                                 0x10054
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_IDLE_TO_RX                              (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                         (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                        (0)
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_RX_TO_IDLE                              (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_RX_TO_IDLE_MASK                         (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_RXADC_ENA_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                        (16)


#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL                                         (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xbc)
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_DEFAULT                                 0x10051
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_IDLE_TO_RX                              (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                         (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                        (0)
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_RX_TO_IDLE                              (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_RX_TO_IDLE_MASK                         (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_RXLNA_ENA_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                        (16)


#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xc4)
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_DEFAULT                               0x10001
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_IDLE_TO_RX                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                       (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                      (0)
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_RX_TO_IDLE                            (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_RX_TO_IDLE_MASK                       (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_RXMIXER_ENA_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                      (16)


#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL                                            (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xd0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_DEFAULT                                    0x10001
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_IDLE_TO_RX                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_IDLE_TO_RX_MASK                            (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_IDLE_TO_RX_SHIFT                           (0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_IDLE_TO_TX                                 (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_IDLE_TO_TX_MASK                            (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_1_CTRL_IDLE_TO_TX_SHIFT                           (16)


#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL                                            (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xd4)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_DEFAULT                                    0x101
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_RX_TO_IDLE                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_RX_TO_IDLE_MASK                            (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_RX_TO_IDLE_SHIFT                           (0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_TX_TO_IDLE                                 (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_TX_TO_IDLE_MASK                            (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SX_ENA_DELAY_2_CTRL_TX_TO_IDLE_SHIFT                           (8)


#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL                                         (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xdc)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_DEFAULT                                 0x0
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_IDLE_TO_RX                              (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_IDLE_TO_RX_MASK                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                        (16)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_IDLE_TO_TX                              (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_IDLE_TO_TX_MASK                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                        (24)


#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL                                         (RXTX_SEQ_CTRL_REGISTERS_BASE + 0xe0)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_DEFAULT                                 0x303
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_RX_TO_IDLE                              (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_RX_TO_IDLE_MASK                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_RX_TO_IDLE_SHIFT                        (0)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_TX_TO_IDLE                              (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_TX_TO_IDLE_MASK                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TUNER_ENA_DELAY_1_CTRL_TX_TO_IDLE_SHIFT                        (8)


#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_0_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x104)
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_0_CTRL_DEFAULT                                  0x60
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_0_CTRL_IDLE_TO_TX                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_0_CTRL_IDLE_TO_TX_MASK                          (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                         (0)


#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_1_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x108)
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_1_CTRL_DEFAULT                                  0x2
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_1_CTRL_TX_TO_IDLE                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_1_CTRL_TX_TO_IDLE_MASK                          (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TXPA_ENA_DELAY_1_CTRL_TX_TO_IDLE_SHIFT                         (0)


#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL                                        (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x1d0)
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_DEFAULT                                0x101
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_IDLE_TO_SLEEP                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_IDLE_TO_SLEEP_MASK                     (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_IDLE_TO_SLEEP_SHIFT                    (0)
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_SLEEP_TO_IDLE                          (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_SLEEP_TO_IDLE_MASK                     (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_CLK_ENABLE_DELAY_0_CTRL_SLEEP_TO_IDLE_SHIFT                    (8)


#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL                                      (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x200)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_DEFAULT                              0x2020D02
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_SLEEP_TO_IDLE                        (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_SLEEP_TO_IDLE_MASK                   (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_SLEEP_TO_IDLE_SHIFT                  (0)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_SLEEP                        (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_SLEEP_MASK                   (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_SLEEP_SHIFT                  (8)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_RX                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_RX_MASK                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                     (16)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_TX                           (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_TX_MASK                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_SXPLL_RESETN_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                     (24)


#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL                              (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x210)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_DEFAULT                      0x101
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_IDLE_TO_RX                   (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_IDLE_TO_RX_MASK              (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_IDLE_TO_RX_SHIFT             (0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_IDLE_TO_TX                   (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_IDLE_TO_TX_MASK              (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_SOFT_RESET_TOGGLE_DELAY_0_CTRL_IDLE_TO_TX_SHIFT             (8)


#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_0_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x228)
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_0_CTRL_DEFAULT                                  0x58
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_0_CTRL_IDLE_TO_RX                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_0_CTRL_IDLE_TO_RX_MASK                          (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                         (0)


#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_1_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x230)
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_1_CTRL_DEFAULT                                  0x1
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_1_CTRL_FROM_RX                                  (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_1_CTRL_FROM_RX_MASK                             (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_RX_EN_DELAY_1_CTRL_FROM_RX_SHIFT                            (0)


#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_0_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x240)
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_0_CTRL_DEFAULT                                  0x5E
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_0_CTRL_IDLE_TO_TX                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_0_CTRL_IDLE_TO_TX_MASK                          (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                         (0)


#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_1_CTRL                                          (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x250)
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_1_CTRL_DEFAULT                                  0x2
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_1_CTRL_FROM_TX                                  (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_1_CTRL_FROM_TX_MASK                             (Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_FE_TX_EN_DELAY_1_CTRL_FROM_TX_SHIFT                            (0)


#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_0_CTRL                                    (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x260)
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_0_CTRL_DEFAULT                            0x0
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_0_CTRL_IDLE_TO_TX                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_0_CTRL_IDLE_TO_TX_MASK                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                   (0)


#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_1_CTRL                                    (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x270)
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_1_CTRL_DEFAULT                            0x2
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_1_CTRL_TX_TO_IDLE                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_1_CTRL_TX_TO_IDLE_MASK                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_USE_RF_TX_REGS_DELAY_1_CTRL_TX_TO_IDLE_SHIFT                   (0)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_0_CTRL                                 (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x2a0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_0_CTRL_DEFAULT                         0xF
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_0_CTRL_SLEEP_TO_IDLE                   (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_0_CTRL_SLEEP_TO_IDLE_MASK              (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_0_CTRL_SLEEP_TO_IDLE_SHIFT             (0)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL                                 (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x2a4)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_DEFAULT                         0xF0F000F
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_SLEEP                   (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_SLEEP_MASK              (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_SLEEP_SHIFT             (0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_RX                      (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_RX_MASK                 (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_RX_SHIFT                (16)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_TX                      (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_TX_MASK                 (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_1_CTRL_IDLE_TO_TX_SHIFT                (24)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_2_CTRL                                 (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x2ac)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_2_CTRL_DEFAULT                         0xF
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_2_CTRL_RX_TO_IDLE                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_2_CTRL_RX_TO_IDLE_MASK                 (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_2_CTRL_RX_TO_IDLE_SHIFT                (0)


#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_3_CTRL                                 (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x2b4)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_3_CTRL_DEFAULT                         0xF
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_3_CTRL_TX_TO_IDLE                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_3_CTRL_TX_TO_IDLE_MASK                 (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_GP_TRIGGER_TOGGLE_DELAY_3_CTRL_TX_TO_IDLE_SHIFT                (0)


#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL                                      (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x2b8)
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_DEFAULT                              0x0
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_IDLE_TO_RX                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_IDLE_TO_RX_MASK                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                     (0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_RX_TO_IDLE                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_RX_TO_IDLE_MASK                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_RX_ACTIVE_DELAY_0_CTRL_RX_TO_IDLE_SHIFT                     (8)


#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL                                      (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x2bc)
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_DEFAULT                              0x0
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_IDLE_TO_TX                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_IDLE_TO_TX_MASK                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                     (0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_TX_TO_IDLE                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_TX_TO_IDLE_MASK                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_TX_ACTIVE_DELAY_0_CTRL_TX_TO_IDLE_SHIFT                     (8)


#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x380)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_DEFAULT                               0x0
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_0                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_0_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_0_SHIFT                           (0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_1                                 (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_1_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_1_SHIFT                           (8)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_2                                 (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_2_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_2_SHIFT                           (16)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_3                                 (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_3_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_0_CTRL_BIT_3_SHIFT                           (24)


#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x384)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_DEFAULT                               0x0
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_4                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_4_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_4_SHIFT                           (0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_5                                 (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_5_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_5_SHIFT                           (8)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_6                                 (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_6_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_6_SHIFT                           (16)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_7                                 (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_7_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_1_CTRL_BIT_7_SHIFT                           (24)


#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL                                       (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x388)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_DEFAULT                               0x0
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_8                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_8_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_8_SHIFT                           (0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_9                                 (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_9_MASK                            (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_9_SHIFT                           (8)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_10                                (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_10_MASK                           (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_10_SHIFT                          (16)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_11                                (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_11_MASK                           (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_TEST_BUS_MEGA_MUX_2_CTRL_BIT_11_SHIFT                          (24)


#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL                                            (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x390)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_DEFAULT                                    0x0
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_SLEEP_TO_IDLE                       (Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_IDLE_TO_SLEEP                       (Bit1)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_IDLE_TO_RX                          (Bit2)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_IDLE_TO_TX                          (Bit3)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_RX_TO_IDLE                          (Bit4)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_TX_TO_IDLE                          (Bit5)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_RESET_TO_IDLE                       (Bit6)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_CTRL_ENABLE_ANY_TO_RESET                        (Bit7)


#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0                                        (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x394)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_DEFAULT                                0x0
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_ANY_TO_INACTIVE                        (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_ANY_TO_INACTIVE_MASK                   (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_ANY_TO_INACTIVE_SHIFT                  (0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_IDLE_TO_RX                             (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_IDLE_TO_RX_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_IDLE_TO_RX_SHIFT                       (8)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_IDLE_TO_TX                             (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_IDLE_TO_TX_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_0_IDLE_TO_TX_SHIFT                       (16)


#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL                                    (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x398)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_DEFAULT                            0x0
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_MASK                          (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_SHIFT                         (0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_TO_RX                         (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_TO_RX_MASK                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_TO_RX_SHIFT                   (8)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_TO_TX                         (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_TO_TX_MASK                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_DELAY_0_CTRL_IDLE_TO_TX_SHIFT                   (16)


#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL                             (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x39c)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_DEFAULT                     0x20202
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE                        (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_MASK                   (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_SHIFT                  (0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_TO_RX                  (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_TO_RX_MASK             (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_TO_RX_SHIFT            (8)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_TO_TX                  (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_TO_TX_MASK             (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_BT_PLL_TRIGGER_OFFSET_DELAY_0_CTRL_IDLE_TO_TX_SHIFT            (16)


#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES                                                 (RXTX_SEQ_CTRL_REGISTERS_BASE + 0x3c0)
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_DEFAULT                                         0x0
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_RXTX_SEQ_STATE                                  (Bit0+Bit1+Bit2+Bit3+Bit4)
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_RXTX_SEQ_STATE_MASK                             (Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_RXTX_SEQ_STATE_SHIFT                            (0)
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_LAST_RXTX_SEQ_STATE                             (Bit8+Bit9+Bit10+Bit11+Bit12)
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_LAST_RXTX_SEQ_STATE_MASK                        (Bit4+Bit3+Bit2+Bit1+Bit0)
#define RXTX_SEQ_CTRL_REGISTERS_CURRENT_STATES_LAST_RXTX_SEQ_STATE_SHIFT                       (8)


