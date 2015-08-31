/* **************************** */
#define PERIPH_BT_GLOBAL_REGISTERS_MISC                                                           (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x4)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_DEFAULT                                                   0x0
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_ENABLE_SXPLL_UPDATE                                       (Bit0)


#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL                                                      (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_DEFAULT                                              0x3A400D
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_ALLOW_TRIG_BT_CTRL_SXPLL                             (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_USE_PS_RX_EN_TRIGGER_PLL                             (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_USE_PS_TX_EN_TRIGGER_PLL                             (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_I2C_DELAY_SEL                                        (Bit6+Bit7+Bit8+Bit9+Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_I2C_DELAY_SEL_MASK                                   (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_I2C_DELAY_SEL_SHIFT                                  (6)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_SXPLL_CONTROL_CLOCK_SEL                              (Bit12+Bit13)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_SXPLL_CONTROL_CLOCK_SEL_MASK                         (Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_SXPLL_CONTROL_CLOCK_SEL_SHIFT                        (12)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_USE_TX_REG1_FROM_BT_PLL_TRIG                         (Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_USE_13M_CLK_FOR_AGC_CLK                              (Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_RXTX_SEQ_COUNTER_CLK_SEL                             (Bit16+Bit17)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_RXTX_SEQ_COUNTER_CLK_SEL_MASK                        (Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_RXTX_SEQ_COUNTER_CLK_SEL_SHIFT                       (16)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_RXTX_SEQ_MAIN_CLK_SEL                                (Bit18+Bit19)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_RXTX_SEQ_MAIN_CLK_SEL_MASK                           (Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_RXTX_SEQ_MAIN_CLK_SEL_SHIFT                          (18)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_USE_PS_RX_EN_TRIGGER_AHB                             (Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_MISC_CTRL_USE_PS_TX_EN_TRIGGER_AHB                             (Bit21)


#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0xc)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_DEFAULT                                       0x5C4CCE8
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_1_                                          (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_2_                                          (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_3_                                          (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_4_                                          (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_5_                                          (Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_6_                                          (Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_7_                                          (Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_8_                                          (Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_9_                                          (Bit8)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_10_                                         (Bit9)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_11_                                         (Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_12_                                         (Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_13_                                         (Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_14_                                         (Bit13)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_15_                                         (Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_16_                                         (Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_17_                                         (Bit16)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_18_                                         (Bit17)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_F_19_                                         (Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_0_                                          (Bit19)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_1_                                          (Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_2_                                          (Bit21)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_3_                                          (Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_4_                                          (Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_5_                                          (Bit24)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_6_                                          (Bit25)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_7_                                          (Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_N_8_                                          (Bit27)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_RESERVED_28                                   (Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_RESERVED_29                                   (Bit29)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_RESERVED_30                                   (Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_1_RESERVED_31                                   (Bit31)


#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x10)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_DEFAULT                                       0xF141607
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_VCO_BIAS_CONT_0_                              (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_VCO_BIAS_CONT_1_                              (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_VCO_BIAS_CONT_2_                              (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_TX_CLK_EDGE_SEL                               (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_BYP_RETIMER                                   (Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_DPLL_ADJ_0_                                   (Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_DPLL_ADJ_1_                                   (Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_DPLL_ADJ_2_                                   (Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_TDZ_CONT_0_                                   (Bit8)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_TDZ_CONT_1_                                   (Bit9)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_CONT_0_                                    (Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_CONT_1_                                    (Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_CONT_2_                                    (Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_RESERVED_13                                   (Bit13)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_RESERVED_14                                   (Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_RESERVED_15                                   (Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_FOURTH_POLE_ENA                               (Bit16)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_OFST_0_                                    (Bit17)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_OFST_1_                                    (Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_OFST_2_                                    (Bit19)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_BYP_NDIV_CLK_GATE                             (Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_BYP_PFD_RST_ON_2_REDGES                       (Bit21)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CP_TRI_LOGIC_BYPASS                           (Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_RESERVED_23                                   (Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_MASH3_ENA                                     (Bit24)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_SD_CLK_ENA                                    (Bit25)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_FRACN_ENA                                     (Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_VCO_ENA                                       (Bit27)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_RESERVED_28                                   (Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_SX_CONT_0                                     (Bit29)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_SX_CONT_1                                     (Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_2_CAL_MUX_SEL                                   (Bit31)


#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x14)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_DEFAULT                                       0x6A0091C7
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_VCO_LDO_CONT_0                                (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_VCO_LDO_CONT_1                                (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_VCO_LDO_CONT_2                                (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_VCO_LDO_CONT_3                                (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_BUF_LDO_CONT_0                                (Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_BUF_LDO_CONT_1                                (Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_BUF_LDO_CONT_2                                (Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_RESERVED_7                                    (Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_SX_REF_CONT_0_                                (Bit8)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_SX_REF_CONT_1_                                (Bit9)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_RESERVED_10                                   (Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_RESERVED_11_                                  (Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_ENABLE_AUTO_RESET                             (Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_SX_RELOCK_TH_0_                               (Bit13)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_SX_RELOCK_TH_1_                               (Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_RELOCK_ENA_1P2                                (Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_TWO_PT_DELAY_CONT_0                           (Bit16)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_TWO_PT_DELAY_CONT_1                           (Bit17)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_RESERVED_18                                   (Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_RESERVED_19                                   (Bit19)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_0_                                (Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_1_                                (Bit21)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_2_                                (Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_3_                                (Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_4_                                (Bit24)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_5_                                (Bit25)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_6_                                (Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_7_                                (Bit27)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_8_                                (Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_9_                                (Bit29)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_COARSE_PROG_10_                               (Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_SXPLL_INTERNAL_3_DPLL_REG_WRITE_ENA                            (Bit31)


#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL                                                   (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1c)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_DEFAULT                                           0x82
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_ENABLE_AHBS_ACCESS                                (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_ENABLE_WAIT_FOR_PLL_UPDATE_DONE                   (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_PLL_UPDATE_CYCLES                                 (Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_PLL_UPDATE_CYCLES_MASK                            (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_PLL_UPDATE_CYCLES_SHIFT                           (4)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_PLL_PROG_EN                                       (Bit12+Bit13+Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_PLL_PROG_EN_MASK                                  (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_PLL_PROG_EN_SHIFT                                 (12)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_AHB_PROG_EN                                       (Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_AHB_PROG_EN_MASK                                  (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_CTRL_AHB_PROG_EN_SHIFT                                 (20)


#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x20)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_DEFAULT                                       0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_PLL_MEM_LOC_START                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_PLL_MEM_LOC_START_MASK                     (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_PLL_MEM_LOC_START_SHIFT                    (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_AHB_MEM_LOC_START                          (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_AHB_MEM_LOC_START_MASK                     (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_AHB_MEM_LOC_START_SHIFT                    (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_PLL_TRIG                                   (Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW1_VALS_SW_AHB_TRIG                                   (Bit29)


#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW2_VALS                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x24)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW2_VALS_DEFAULT                                       0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW2_VALS_SW_AHB_MEM_LOC_START                          (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW2_VALS_SW_AHB_MEM_LOC_START_MASK                     (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW2_VALS_SW_AHB_MEM_LOC_START_SHIFT                    (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_SW2_VALS_SW_AHB_TRIG                                   (Bit29)


#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1                                      (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x28)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_DEFAULT                              0x0
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_RX_EN_FALLING                        (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_RX_EN_FALLING_MASK                   (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_RX_EN_FALLING_SHIFT                  (0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_TX_EN_FALLING                        (Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_TX_EN_FALLING_MASK                   (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_ONLY_MEM_LOC_STARTS_1_TX_EN_FALLING_SHIFT                  (12)


#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS                                                 (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x30)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_DEFAULT                                         0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_TRIG_BT_PLL_STATE                               (Bit0+Bit1+Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_TRIG_BT_PLL_STATE_MASK                          (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_TRIG_BT_PLL_STATE_SHIFT                         (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_TRIG_BT_AHB_STATE                               (Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_TRIG_BT_AHB_STATE_MASK                          (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_BT_STATUS_TRIG_BT_AHB_STATE_SHIFT                         (4)


#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x34)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_DEFAULT                                    0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_SW_RX_AHB_LOC                              (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_SW_RX_AHB_LOC_MASK                         (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_SW_RX_AHB_LOC_SHIFT                        (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_SW_TX_AHB_LOC                              (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_SW_TX_AHB_LOC_MASK                         (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TRIG_AHB_TX_RX_CTRL_SW_TX_AHB_LOC_SHIFT                        (16)


#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES                                                  (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x180)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_DEFAULT                                          0xDFA1FFF
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_CLK52_EN                                      (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_MDM_CLK52_EN                                  (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_CLK26_EN                                      (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_HCLK_EN                                       (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_CLK13_EN                                      (Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_MDM_CLK13_EN                                  (Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_EM_MEM_CLK_EN                                 (Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_EM_RT_MEM_CLK_EN                              (Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_TRIG_BT_AHB_CLK_EN                               (Bit8)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_TRIG_BT_AHB_MEM_CLK                              (Bit9)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_TRIG_BT_PLL_CLK_EN                               (Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_TRIG_BT_PLL_MEM_CLK_EN                           (Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_APB_CLK_EN                                    (Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_HOST_I2C_CLK_EN                                  (Bit17)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_PMU_CLK_EN                                       (Bit19)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_RXTX_SEQ_REGS_CLK_EN                             (Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_SXPLL_CONTROL_CLK_EN                             (Bit21)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_RXTX_SEQ_MAIN_CLK_EN                             (Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_DL_MEM_CLK_EN                                 (Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_BT_MDM_CLK26_EN                                  (Bit24)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_MTTICK_LOGIC_BT_EN                               (Bit25)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_TRIG_BT_AHB_FREE_CLK_EN                          (Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_CLOCK_ENABLES_TRIG_BT_PLL_FREE_CLK_EN                          (Bit27)


#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET                                             (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x184)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_DEFAULT                                     0x7F87F
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_GLOBAL_RSTN                                 (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_BT_CORE_RSTN                                (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_BT_HRESETN_RSTN                             (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_BT_MASTER_NRST_RSTN                         (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_BT_CRYPT_NRST_RSTN                          (Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_BT_RST_N_MDM_RSTN                           (Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_BT_APB_RSTN                                 (Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_HOST_I2C_RSTN                               (Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_MBIST_RSTN                                  (Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_RXTX_SEQ_RSTN                               (Bit13)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_RXTX_SEQ_CNTR_RSTN                          (Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_FE_MEM_RSTN                                 (Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_FE_ASYNC_RSTN                               (Bit16)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_TRIG_PROG_BT_AHB_RSTN                       (Bit17)
#define PERIPH_BT_GLOBAL_REGISTERS_BTMCU_GLOBAL_RESET_TRIG_PROG_BT_PLL_RSTN                       (Bit18)


#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM                                                      (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x188)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_DEFAULT                                              0x30
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_BTRIM_FOR_BTMCU_PD4A                                 (Bit0+Bit1+Bit2+Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_BTRIM_FOR_BTMCU_PD4A_MASK                            (Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_BTRIM_FOR_BTMCU_PD4A_SHIFT                           (0)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_STDBY_FOR_BTMCU_PD4A                                 (Bit4+Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_STDBY_FOR_BTMCU_PD4A_MASK                            (Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_STDBY_FOR_BTMCU_PD4A_SHIFT                           (4)


#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_CONTROL                                                  (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x18c)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_CONTROL_DEFAULT                                          0x0


#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS                                                   (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x190)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_DEFAULT                                           0x0
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_BUS                                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_BUS_MASK                                    (Bit20+Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_BUS_SHIFT                                   (0)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_BLOCK_SEL                                   (Bit21+Bit22+Bit23+Bit24+Bit25)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_BLOCK_SEL_MASK                              (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_BLOCK_SEL_SHIFT                             (21)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_WRAPPER_SEL                                 (Bit26+Bit27+Bit28+Bit29+Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_WRAPPER_SEL_MASK                            (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_WRAPPER_SEL_SHIFT                           (26)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_STATUS_MBIST_MODE                                        (Bit31)


#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL                                             (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x194)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_DEFAULT                                     0x4F
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_MARCH2_ENABLE                               (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_UNIQUE_ENABLE                               (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_CHECKERBOARD_ENABLE                         (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_DIAGONAL_ENABLE                             (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_ADDRDEC0_ENABLE                             (Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_ADDRDEC1_ENABLE                             (Bit5)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_COLMARCH1_ENABLE                            (Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_FILLWITH0_ENABLE                            (Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_READ_ZEROS_ENABLE                       (Bit8)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_WRITE_ZEROS_ENABLE                      (Bit9)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_READ_ONES_ENABLE                        (Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_WRITE_ONES_ENABLE                       (Bit11)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_READ_CB_ENABLE                          (Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_WRITE_CB_ENABLE                         (Bit13)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_READ_INVCB_ENABLE                       (Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_SRAM_ALG_SEL_RET_WRITE_INVCB_ENABLE                      (Bit15)


#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_BG_0                                                     (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x198)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_BG_0_DEFAULT                                             0x0


#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_BG_1                                                     (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x19c)
#define PERIPH_BT_GLOBAL_REGISTERS_MBIST_BG_1_DEFAULT                                             0x0


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1a0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_DEFAULT                                       0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_ENABLE_TOP                               (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_ENABLE_BTMCU                             (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_ENABLE_TRIG_PROG_BT_PLL                  (Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_ENABLE_TRIG_PROG_BT_AHB                  (Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_BUS_STATUS                               (Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_BUS_STATUS_MASK                          (Bit20+Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_CONTROL_TEST_BUS_STATUS_SHIFT                         (11)


#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1c0)
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_DEFAULT                                        0x0
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_RXTX_SEQ_STATE                                 (Bit0+Bit1+Bit2+Bit3)
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_RXTX_SEQ_STATE_MASK                            (Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_RXTX_SEQ_STATE_SHIFT                           (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_RXTX_SEQ_LAST_STATE                            (Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_RXTX_SEQ_LAST_STATE_MASK                       (Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RXTX_SEQ_STATUS_RXTX_SEQ_LAST_STATE_SHIFT                      (4)


#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_CTRL                                                 (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1c4)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_CTRL_DEFAULT                                         0x0
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_CTRL_EM_MEM_RWM_EN                                   (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_CTRL_EM_MEM_WM_EN                                    (Bit1)
#define PERIPH_BT_GLOBAL_REGISTERS_MSEMI_MEM_CTRL_EM_MEM_RM_EN                                    (Bit2)


#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1                                                  (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1d4)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_DEFAULT                                          0x52000E2
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_SXPLL_ENABLE_DELAY_COUNT                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_SXPLL_ENABLE_DELAY_COUNT_MASK                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_SXPLL_ENABLE_DELAY_COUNT_SHIFT                   (0)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_SXPLL_LOCK_COUNT                                 (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_SXPLL_LOCK_COUNT_MASK                            (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_1_SXPLL_LOCK_COUNT_SHIFT                           (16)


#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_2                                                  (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1d8)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_2_DEFAULT                                          0x10F
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_2_SXPLL_RESETN_RISE_COUNT                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_2_SXPLL_RESETN_RISE_COUNT_MASK                     (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_PLL_CONTROL_2_SXPLL_RESETN_RISE_COUNT_SHIFT                    (0)


#define PERIPH_BT_GLOBAL_REGISTERS_MTTICK_LOGIC                                                   (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x1e8)
#define PERIPH_BT_GLOBAL_REGISTERS_MTTICK_LOGIC_DEFAULT                                           0xF000
#define PERIPH_BT_GLOBAL_REGISTERS_MTTICK_LOGIC_BT_LOGIC_ENABLE                                   (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MTTICK_LOGIC_BT_LOGIC_COUNT_VAL                                (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_MTTICK_LOGIC_BT_LOGIC_COUNT_VAL_MASK                           (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_MTTICK_LOGIC_BT_LOGIC_COUNT_VAL_SHIFT                          (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RFREVID                                                        (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x200)
#define PERIPH_BT_GLOBAL_REGISTERS_RFREVID_DEFAULT                                                0x0


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x240)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_DEFAULT                                    0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_0_SEL                                  (Bit0+Bit1+Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_0_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_0_SEL_SHIFT                            (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_1_SEL                                  (Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_1_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_1_SEL_SHIFT                            (4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_2_SEL                                  (Bit8+Bit9+Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_2_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_2_SEL_SHIFT                            (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_3_SEL                                  (Bit12+Bit13+Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_3_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_3_SEL_SHIFT                            (12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_4_SEL                                  (Bit16+Bit17+Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_4_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_4_SEL_SHIFT                            (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_5_SEL                                  (Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_5_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_5_SEL_SHIFT                            (20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_6_SEL                                  (Bit24+Bit25+Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_6_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_6_SEL_SHIFT                            (24)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_7_SEL                                  (Bit28+Bit29+Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_7_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_0_BIT_7_SEL_SHIFT                            (28)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x244)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_DEFAULT                                    0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_8_SEL                                  (Bit0+Bit1+Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_8_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_8_SEL_SHIFT                            (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_9_SEL                                  (Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_9_SEL_MASK                             (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_9_SEL_SHIFT                            (4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_10_SEL                                 (Bit8+Bit9+Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_10_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_10_SEL_SHIFT                           (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_11_SEL                                 (Bit12+Bit13+Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_11_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_11_SEL_SHIFT                           (12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_12_SEL                                 (Bit16+Bit17+Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_12_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_12_SEL_SHIFT                           (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_13_SEL                                 (Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_13_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_13_SEL_SHIFT                           (20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_14_SEL                                 (Bit24+Bit25+Bit26)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_14_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_14_SEL_SHIFT                           (24)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_15_SEL                                 (Bit28+Bit29+Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_15_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_1_BIT_15_SEL_SHIFT                           (28)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x248)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_DEFAULT                                    0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_16_SEL                                 (Bit0+Bit1+Bit2)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_16_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_16_SEL_SHIFT                           (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_17_SEL                                 (Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_17_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_17_SEL_SHIFT                           (4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_18_SEL                                 (Bit8+Bit9+Bit10)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_18_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_18_SEL_SHIFT                           (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_19_SEL                                 (Bit12+Bit13+Bit14)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_19_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_19_SEL_SHIFT                           (12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_20_SEL                                 (Bit16+Bit17+Bit18)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_20_SEL_MASK                            (Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SEL_CTRL_2_BIT_20_SEL_SHIFT                           (16)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0                                          (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x250)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_DEFAULT                                  0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_0_SHIFT                              (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_0_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_0_SHIFT_SHIFT                        (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_1_SHIFT                              (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_1_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_1_SHIFT_SHIFT                        (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_2_SHIFT                              (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_2_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_2_SHIFT_SHIFT                        (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_3_SHIFT                              (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_3_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_0_BUS_3_SHIFT_SHIFT                        (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1                                          (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x254)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_DEFAULT                                  0x0
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_4_SHIFT                              (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_4_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_4_SHIFT_SHIFT                        (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_5_SHIFT                              (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_5_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_5_SHIFT_SHIFT                        (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_6_SHIFT                              (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_6_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_6_SHIFT_SHIFT                        (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_7_SHIFT                              (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_7_SHIFT_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_SHIFT_CTRL_1_BUS_7_SHIFT_SHIFT                        (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0                                         (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x260)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_DEFAULT                                 0x3020100
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_0_SEL                               (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_0_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_0_SEL_SHIFT                         (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_1_SEL                               (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_1_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_1_SEL_SHIFT                         (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_2_SEL                               (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_2_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_2_SEL_SHIFT                         (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_3_SEL                               (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_3_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_0_BIT_3_SEL_SHIFT                         (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1                                         (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x264)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_DEFAULT                                 0x7060504
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_4_SEL                               (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_4_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_4_SEL_SHIFT                         (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_5_SEL                               (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_5_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_5_SEL_SHIFT                         (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_6_SEL                               (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_6_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_6_SEL_SHIFT                         (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_7_SEL                               (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_7_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_1_BIT_7_SEL_SHIFT                         (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2                                         (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x268)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_DEFAULT                                 0xB0A0908
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_8_SEL                               (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_8_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_8_SEL_SHIFT                         (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_9_SEL                               (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_9_SEL_MASK                          (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_9_SEL_SHIFT                         (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_10_SEL                              (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_10_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_10_SEL_SHIFT                        (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_11_SEL                              (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_11_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_2_BIT_11_SEL_SHIFT                        (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3                                         (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x26c)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_DEFAULT                                 0xF0E0D0C
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_12_SEL                              (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_12_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_12_SEL_SHIFT                        (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_13_SEL                              (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_13_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_13_SEL_SHIFT                        (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_14_SEL                              (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_14_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_14_SEL_SHIFT                        (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_15_SEL                              (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_15_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_3_BIT_15_SEL_SHIFT                        (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4                                         (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x270)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_DEFAULT                                 0x13121110
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_16_SEL                              (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_16_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_16_SEL_SHIFT                        (0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_17_SEL                              (Bit8+Bit9+Bit10+Bit11+Bit12)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_17_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_17_SEL_SHIFT                        (8)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_18_SEL                              (Bit16+Bit17+Bit18+Bit19+Bit20)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_18_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_18_SEL_SHIFT                        (16)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_19_SEL                              (Bit24+Bit25+Bit26+Bit27+Bit28)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_19_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_4_BIT_19_SEL_SHIFT                        (24)


#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_5                                         (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x274)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_5_DEFAULT                                 0x14
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_5_BIT_20_SEL                              (Bit0+Bit1+Bit2+Bit3+Bit4)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_5_BIT_20_SEL_MASK                         (Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_TEST_BUS_ASSIGN_CTRL_5_BIT_20_SEL_SHIFT                        (0)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX                                                 (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x800)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_DEFAULT                                         0x2A02
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_REG1                                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_REG1_MASK                                       (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_REG1_SHIFT                                      (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_REG2                                            (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_REG2_MASK                                       (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_RX_REG2_SHIFT                                      (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x804)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_DEFAULT                                        0xB248EE13
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG1                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG1_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG1_SHIFT                                     (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG2                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG2_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG2_SHIFT                                     (8)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG3                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG3_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG3_SHIFT                                     (16)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG4                                           (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG4_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_RX_REG4_SHIFT                                     (24)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x808)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_DEFAULT                                        0xEEC
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG5                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG5_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG5_SHIFT                                     (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG6                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG6_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG6_SHIFT                                     (8)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG7                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG7_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_RX_REG7_SHIFT                                     (16)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX                                                  (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x80c)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_DEFAULT                                          0x800F
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_REG1                                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_REG1_MASK                                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_REG1_SHIFT                                       (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_REG2                                             (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_REG2_MASK                                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_RX_REG2_SHIFT                                       (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x810)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_DEFAULT                                        0x8800
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_REG1                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_REG1_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_REG1_SHIFT                                     (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_REG2                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_REG2_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_RX_REG2_SHIFT                                     (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS                                                     (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x814)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_DEFAULT                                             0x14148E
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG1                                                (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG1_MASK                                           (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG1_SHIFT                                          (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG2                                                (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG2_MASK                                           (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG2_SHIFT                                          (8)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG3                                                (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG3_MASK                                           (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_XO_REGS_REG3_SHIFT                                          (16)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_MUX_CNTL_REGS                                               (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x818)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MUX_CNTL_REGS_DEFAULT                                       0x10000


#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX                                                 (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x840)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_DEFAULT                                         0x2A02
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_REG1                                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_REG1_MASK                                       (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_REG1_SHIFT                                      (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_REG2                                            (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_REG2_MASK                                       (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_ADC_REGS_TX_REG2_SHIFT                                      (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x844)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_DEFAULT                                        0xB248EE13
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG1                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG1_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG1_SHIFT                                     (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG2                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG2_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG2_SHIFT                                     (8)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG3                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG3_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG3_SHIFT                                     (16)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG4                                           (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG4_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_0_TX_REG4_SHIFT                                     (24)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x848)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_DEFAULT                                        0xEEC
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG5                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG5_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG5_SHIFT                                     (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG6                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG6_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG6_SHIFT                                     (8)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG7                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG7_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_RX_REGS_1_TX_REG7_SHIFT                                     (16)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX                                                  (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x84c)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_DEFAULT                                          0x800F
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_REG1                                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_REG1_MASK                                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_REG1_SHIFT                                       (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_REG2                                             (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_REG2_MASK                                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_TX_REGS_TX_REG2_SHIFT                                       (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x850)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_DEFAULT                                        0x8800
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_REG1                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_REG1_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_REG1_SHIFT                                     (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_REG2                                           (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_REG2_MASK                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_MISC_REGS_TX_REG2_SHIFT                                     (8)


#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS                                                   (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8a0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_DEFAULT                                           0x0
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG1                                              (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG1_MASK                                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG1_SHIFT                                        (0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG2                                              (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG2_MASK                                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG2_SHIFT                                        (8)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG3                                              (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG3_MASK                                         (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_RF_READ_REGS_REG3_SHIFT                                        (16)


#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_CONTROL                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8a4)
#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_CONTROL_DEFAULT                                    0x0
#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_CONTROL_ENABLE                                     (Bit0)


#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_STATUS                                             (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8a8)
#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_STATUS_DEFAULT                                     0x0
#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_STATUS_VALUE                                       (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19)
#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_STATUS_VALUE_MASK                                  (Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_LO_FREQ_EST_STATUS_VALUE_SHIFT                                 (0)


#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_TIMEOUT                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8c0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_TIMEOUT_DEFAULT                                    0x7FFFFFFF
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_TIMEOUT_VALUE                                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_TIMEOUT_VALUE_MASK                                 (Bit30+Bit29+Bit28+Bit27+Bit26+Bit25+Bit24+Bit23+Bit22+Bit21+Bit20+Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_TIMEOUT_VALUE_SHIFT                                (0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_TIMEOUT_ENABLE                                     (Bit31)


#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL                                            (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8c4)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_DEFAULT                                    0x0
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_TRIG_CLEAR_TIMEOUT                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_TRIG_CLEAR_TIMEOUT_MASK                    (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_TRIG_CLEAR_TIMEOUT_SHIFT                   (0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_TRIG_READ_UNTIL_ZERO                       (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_TRIG_READ_UNTIL_ZERO_MASK                  (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_CONTROL_TRIG_READ_UNTIL_ZERO_SHIFT                 (16)


#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS                                             (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8c8)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_DEFAULT                                     0x0
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_TIMEOUT_TRIGGER                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_TIMEOUT_TRIGGER_MASK                        (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_TIMEOUT_TRIGGER_SHIFT                       (0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_ACTIVE_TRIGGERS                             (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_ACTIVE_TRIGGERS_MASK                        (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_STATUS_ACTIVE_TRIGGERS_SHIFT                       (16)


#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS                                      (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8cc)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_DEFAULT                              0x0
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_CURRENT_MEM_LOC                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_CURRENT_MEM_LOC_MASK                 (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_CURRENT_MEM_LOC_SHIFT                (0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_TIMEOUT_MEM_LOC                      (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_TIMEOUT_MEM_LOC_MASK                 (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_MEMORY_STATUS_TIMEOUT_MEM_LOC_SHIFT                (16)


#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_IRQ                                                (PERIPH_BT_GLOBAL_REGISTERS_BASE + 0x8d0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_IRQ_DEFAULT                                        0x0
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_IRQ_CONTROL_IRQ                                    (Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_IRQ_CONTROL_CODE                                   (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_IRQ_CONTROL_CODE_MASK                              (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define PERIPH_BT_GLOBAL_REGISTERS_AHB_TRIGGER_IRQ_CONTROL_CODE_SHIFT                             (24)


