/* **************************** */
#define BLE_FE_REGS_MISC_CTL                                                     (BLE_FE_REGS_BASE + 0x0)
#define BLE_FE_REGS_MISC_CTL_DEFAULT                                             0x410041A0
#define BLE_FE_REGS_MISC_CTL_SOFT_RESET                                          (Bit0)
#define BLE_FE_REGS_MISC_CTL_RX_TX_EN_FORCE                                      (Bit1)
#define BLE_FE_REGS_MISC_CTL_RX_EN                                               (Bit2)
#define BLE_FE_REGS_MISC_CTL_TX_EN                                               (Bit3)
#define BLE_FE_REGS_MISC_CTL_RX_IN_GATED_ON_RX_EN                                (Bit4)
#define BLE_FE_REGS_MISC_CTL_RX_SAMP_EDGE                                        (Bit5)
#define BLE_FE_REGS_MISC_CTL_RX_MUX_I                                            (Bit7)
#define BLE_FE_REGS_MISC_CTL_RX_MUX_Q                                            (Bit8)
#define BLE_FE_REGS_MISC_CTL_RX_NEG_I                                            (Bit9)
#define BLE_FE_REGS_MISC_CTL_RX_NEG_Q                                            (Bit10)
#define BLE_FE_REGS_MISC_CTL_RX_CORDIC_MUX_I                                     (Bit11)
#define BLE_FE_REGS_MISC_CTL_RX_CORDIC_MUX_Q                                     (Bit12)
#define BLE_FE_REGS_MISC_CTL_RX_TX_EN_SRC                                        (Bit14)
#define BLE_FE_REGS_MISC_CTL_TX_MUX_I                                            (Bit15)
#define BLE_FE_REGS_MISC_CTL_TX_MUX_Q                                            (Bit16)
#define BLE_FE_REGS_MISC_CTL_TX_MSB_FLIP1_I                                      (Bit17)
#define BLE_FE_REGS_MISC_CTL_TX_MSB_FLIP1_Q                                      (Bit18)
#define BLE_FE_REGS_MISC_CTL_TX_NEG_I                                            (Bit19)
#define BLE_FE_REGS_MISC_CTL_TX_NEG_Q                                            (Bit20)
#define BLE_FE_REGS_MISC_CTL_TX_MSB_FLIP2_I                                      (Bit21)
#define BLE_FE_REGS_MISC_CTL_TX_MSB_FLIP2_Q                                      (Bit22)
#define BLE_FE_REGS_MISC_CTL_TX_FIFO_BYPASS                                      (Bit23)
#define BLE_FE_REGS_MISC_CTL_SOFT_RST_RX_START                                   (Bit24)
#define BLE_FE_REGS_MISC_CTL_SOFT_RST_RX_END                                     (Bit25)
#define BLE_FE_REGS_MISC_CTL_USE_EXT_SOFT_RESET                                  (Bit27)
#define BLE_FE_REGS_MISC_CTL_SOFT_RST_TX_START                                   (Bit30)
#define BLE_FE_REGS_MISC_CTL_SOFT_RST_TX_END                                     (Bit31)


#define BLE_FE_REGS_RX_TX_DELAY                                                  (BLE_FE_REGS_BASE + 0x8)
#define BLE_FE_REGS_RX_TX_DELAY_DEFAULT                                          0x0
#define BLE_FE_REGS_RX_TX_DELAY_RX                                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RX_TX_DELAY_RX_MASK                                          (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_TX_DELAY_RX_SHIFT                                         (0)
#define BLE_FE_REGS_RX_TX_DELAY_TX                                               (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define BLE_FE_REGS_RX_TX_DELAY_TX_MASK                                          (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_TX_DELAY_TX_SHIFT                                         (16)


#define BLE_FE_REGS_SOFT_RESET_CTL                                               (BLE_FE_REGS_BASE + 0xc)
#define BLE_FE_REGS_SOFT_RESET_CTL_DEFAULT                                       0x3
#define BLE_FE_REGS_SOFT_RESET_CTL_DURATION                                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_SOFT_RESET_CTL_DURATION_MASK                                 (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_SOFT_RESET_CTL_DURATION_SHIFT                                (0)


#define BLE_FE_REGS_ADC_STS                                                      (BLE_FE_REGS_BASE + 0x14)
#define BLE_FE_REGS_ADC_STS_DEFAULT                                              0x0
#define BLE_FE_REGS_ADC_STS_REAL                                                 (Bit0)
#define BLE_FE_REGS_ADC_STS_IMAG                                                 (Bit1)


#define BLE_FE_REGS_DAC_STS                                                      (BLE_FE_REGS_BASE + 0x18)
#define BLE_FE_REGS_DAC_STS_DEFAULT                                              0x0
#define BLE_FE_REGS_DAC_STS_REAL                                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_DAC_STS_REAL_MASK                                            (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_DAC_STS_REAL_SHIFT                                           (0)
#define BLE_FE_REGS_DAC_STS_IMAG                                                 (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_DAC_STS_IMAG_MASK                                            (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_DAC_STS_IMAG_SHIFT                                           (8)


#define BLE_FE_REGS_ADC_FORCE                                                    (BLE_FE_REGS_BASE + 0x1c)
#define BLE_FE_REGS_ADC_FORCE_DEFAULT                                            0x0
#define BLE_FE_REGS_ADC_FORCE_REAL                                               (Bit0)
#define BLE_FE_REGS_ADC_FORCE_IMAG                                               (Bit1)
#define BLE_FE_REGS_ADC_FORCE_ENABLE                                             (Bit2)


#define BLE_FE_REGS_DAC_FORCE                                                    (BLE_FE_REGS_BASE + 0x20)
#define BLE_FE_REGS_DAC_FORCE_DEFAULT                                            0x0
#define BLE_FE_REGS_DAC_FORCE_REAL                                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_DAC_FORCE_REAL_MASK                                          (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_DAC_FORCE_REAL_SHIFT                                         (0)
#define BLE_FE_REGS_DAC_FORCE_IMAG                                               (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_DAC_FORCE_IMAG_MASK                                          (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_DAC_FORCE_IMAG_SHIFT                                         (8)
#define BLE_FE_REGS_DAC_FORCE_ENABLE                                             (Bit16)


#define BLE_FE_REGS_RX_PDBRAW_AVG                                                (BLE_FE_REGS_BASE + 0x60)
#define BLE_FE_REGS_RX_PDBRAW_AVG_DEFAULT                                        0x1002
#define BLE_FE_REGS_RX_PDBRAW_AVG_PROG_GAIN                                      (Bit0+Bit1)
#define BLE_FE_REGS_RX_PDBRAW_AVG_PROG_GAIN_MASK                                 (Bit1+Bit0)
#define BLE_FE_REGS_RX_PDBRAW_AVG_PROG_GAIN_SHIFT                                (0)
#define BLE_FE_REGS_RX_PDBRAW_AVG_THR_HP                                         (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define BLE_FE_REGS_RX_PDBRAW_AVG_THR_HP_MASK                                    (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_PDBRAW_AVG_THR_HP_SHIFT                                   (8)


#define BLE_FE_REGS_RX_PDBRAW_AVG_STS                                            (BLE_FE_REGS_BASE + 0x64)
#define BLE_FE_REGS_RX_PDBRAW_AVG_STS_DEFAULT                                    0x0
#define BLE_FE_REGS_RX_PDBRAW_AVG_STS_STS_RW_PDBRAW_AVG                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10)
#define BLE_FE_REGS_RX_PDBRAW_AVG_STS_STS_RW_PDBRAW_AVG_MASK                     (Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_PDBRAW_AVG_STS_STS_RW_PDBRAW_AVG_SHIFT                    (0)


#define BLE_FE_REGS_RX_DC_EST_STS                                                (BLE_FE_REGS_BASE + 0x7c)
#define BLE_FE_REGS_RX_DC_EST_STS_DEFAULT                                        0x0
#define BLE_FE_REGS_RX_DC_EST_STS_DC_I                                           (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13)
#define BLE_FE_REGS_RX_DC_EST_STS_DC_I_MASK                                      (Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_DC_EST_STS_DC_I_SHIFT                                     (0)
#define BLE_FE_REGS_RX_DC_EST_STS_DC_Q                                           (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define BLE_FE_REGS_RX_DC_EST_STS_DC_Q_MASK                                      (Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_DC_EST_STS_DC_Q_SHIFT                                     (16)


#define BLE_FE_REGS_RX_DC_EST_CORR                                               (BLE_FE_REGS_BASE + 0x80)
#define BLE_FE_REGS_RX_DC_EST_CORR_DEFAULT                                       0x150D0
#define BLE_FE_REGS_RX_DC_EST_CORR_AVG_FACTOR                                    (Bit0+Bit1)
#define BLE_FE_REGS_RX_DC_EST_CORR_AVG_FACTOR_MASK                               (Bit1+Bit0)
#define BLE_FE_REGS_RX_DC_EST_CORR_AVG_FACTOR_SHIFT                              (0)
#define BLE_FE_REGS_RX_DC_EST_CORR_AVG_EN                                        (Bit2)
#define BLE_FE_REGS_RX_DC_EST_CORR_OPEN_LOOP                                     (Bit3)
#define BLE_FE_REGS_RX_DC_EST_CORR_CALC_STATES                                   (Bit4+Bit5+Bit6)
#define BLE_FE_REGS_RX_DC_EST_CORR_CALC_STATES_MASK                              (Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_DC_EST_CORR_CALC_STATES_SHIFT                             (4)
#define BLE_FE_REGS_RX_DC_EST_CORR_FLUSH_CPX_VALID                               (Bit7)
#define BLE_FE_REGS_RX_DC_EST_CORR_START_DELAY_CNT                               (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RX_DC_EST_CORR_START_DELAY_CNT_MASK                          (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_DC_EST_CORR_START_DELAY_CNT_SHIFT                         (8)
#define BLE_FE_REGS_RX_DC_EST_CORR_PROG_GAIN                                     (Bit16+Bit17)
#define BLE_FE_REGS_RX_DC_EST_CORR_PROG_GAIN_MASK                                (Bit1+Bit0)
#define BLE_FE_REGS_RX_DC_EST_CORR_PROG_GAIN_SHIFT                               (16)


#define BLE_FE_REGS_RX_AGC_TIME                                                  (BLE_FE_REGS_BASE + 0x84)
#define BLE_FE_REGS_RX_AGC_TIME_DEFAULT                                          0xA1E
#define BLE_FE_REGS_RX_AGC_TIME_WINDOW_SEARCH_TIME                               (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_RX_AGC_TIME_WINDOW_SEARCH_TIME_MASK                          (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_TIME_WINDOW_SEARCH_TIME_SHIFT                         (0)
#define BLE_FE_REGS_RX_AGC_TIME_SETTLING_TIME                                    (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RX_AGC_TIME_SETTLING_TIME_MASK                               (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_TIME_SETTLING_TIME_SHIFT                              (8)


#define BLE_FE_REGS_RX_AGC_THRESHOLD                                             (BLE_FE_REGS_BASE + 0x88)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_DEFAULT                                     0x1E1E0101
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_ULTRA_HI                                 (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_ULTRA_HI_MASK                            (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_ULTRA_HI_SHIFT                           (0)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_HI                                       (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_HI_MASK                                  (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_HI_SHIFT                                 (8)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_ULTRA_LOW                                (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_ULTRA_LOW_MASK                           (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_ULTRA_LOW_SHIFT                          (16)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_LOW                                      (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_LOW_MASK                                 (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_THRESHOLD_TH_LOW_SHIFT                                (24)


#define BLE_FE_REGS_RX_AGC_STEP                                                  (BLE_FE_REGS_BASE + 0x8c)
#define BLE_FE_REGS_RX_AGC_STEP_DEFAULT                                          0x1111
#define BLE_FE_REGS_RX_AGC_STEP_STEP_ULTRA_HI                                    (Bit0+Bit1)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_ULTRA_HI_MASK                               (Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_ULTRA_HI_SHIFT                              (0)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_HI                                          (Bit4+Bit5)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_HI_MASK                                     (Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_HI_SHIFT                                    (4)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_ULTRA_LOW                                   (Bit8+Bit9)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_ULTRA_LOW_MASK                              (Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_ULTRA_LOW_SHIFT                             (8)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_LOW                                         (Bit12+Bit13)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_LOW_MASK                                    (Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_STEP_STEP_LOW_SHIFT                                   (12)


#define BLE_FE_REGS_RX_AGC_GAIN_FORCE                                            (BLE_FE_REGS_BASE + 0x90)
#define BLE_FE_REGS_RX_AGC_GAIN_FORCE_DEFAULT                                    0x0
#define BLE_FE_REGS_RX_AGC_GAIN_FORCE_GAIN_FORCE_EN                              (Bit0)
#define BLE_FE_REGS_RX_AGC_GAIN_FORCE_GAIN_FORCE_IDX                             (Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_RX_AGC_GAIN_FORCE_GAIN_FORCE_IDX_MASK                        (Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_GAIN_FORCE_GAIN_FORCE_IDX_SHIFT                       (4)


#define BLE_FE_REGS_RX_AGC_CALC                                                  (BLE_FE_REGS_BASE + 0x94)
#define BLE_FE_REGS_RX_AGC_CALC_DEFAULT                                          0x81
#define BLE_FE_REGS_RX_AGC_CALC_FREEZE_AFTER_SYNC                                (Bit0)
#define BLE_FE_REGS_RX_AGC_CALC_SWAP                                             (Bit1)
#define BLE_FE_REGS_RX_AGC_CALC_INV                                              (Bit2)
#define BLE_FE_REGS_RX_AGC_CALC_HI_INV                                           (Bit3)
#define BLE_FE_REGS_RX_AGC_CALC_LO_INV                                           (Bit4)
#define BLE_FE_REGS_RX_AGC_CALC_CONTRADICT_OP                                    (Bit5+Bit6)
#define BLE_FE_REGS_RX_AGC_CALC_CONTRADICT_OP_MASK                               (Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_CALC_CONTRADICT_OP_SHIFT                              (5)
#define BLE_FE_REGS_RX_AGC_CALC_CLK_GATE_AFTER_RSSI                              (Bit7)


#define BLE_FE_REGS_RX_AGC_STATUS                                                (BLE_FE_REGS_BASE + 0x98)
#define BLE_FE_REGS_RX_AGC_STATUS_DEFAULT                                        0x0
#define BLE_FE_REGS_RX_AGC_STATUS_STS_HI_LOW_BOTH_ASSERTED                       (Bit0)
#define BLE_FE_REGS_RX_AGC_STATUS_STS_GAIN_CONTRADICT                            (Bit1)


#define BLE_FE_REGS_RX_AGC_GAIN_CODE_STS                                         (BLE_FE_REGS_BASE + 0x9c)
#define BLE_FE_REGS_RX_AGC_GAIN_CODE_STS_DEFAULT                                 0x1FF
#define BLE_FE_REGS_RX_AGC_GAIN_CODE_STS_STS_AGC_RF_GAIN_CODE                    (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8)
#define BLE_FE_REGS_RX_AGC_GAIN_CODE_STS_STS_AGC_RF_GAIN_CODE_MASK               (Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RX_AGC_GAIN_CODE_STS_STS_AGC_RF_GAIN_CODE_SHIFT              (0)


#define BLE_FE_REGS_RSSI_GAIN_SET_0                                              (BLE_FE_REGS_BASE + 0x114)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_DEFAULT                                      0xCCDCF0FF
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE0                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE0_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE0_SHIFT                       (0)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE1                             (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE1_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE1_SHIFT                       (8)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE2                             (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE2_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE2_SHIFT                       (16)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE3                             (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE3_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_0_RSSI_GAIN_STAGE3_SHIFT                       (24)


#define BLE_FE_REGS_RSSI_GAIN_SET_1                                              (BLE_FE_REGS_BASE + 0x118)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_DEFAULT                                      0x7289A1B8
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE4                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE4_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE4_SHIFT                       (0)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE5                             (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE5_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE5_SHIFT                       (8)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE6                             (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE6_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE6_SHIFT                       (16)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE7                             (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE7_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_1_RSSI_GAIN_STAGE7_SHIFT                       (24)


#define BLE_FE_REGS_RSSI_GAIN_SET_2                                              (BLE_FE_REGS_BASE + 0x11c)
#define BLE_FE_REGS_RSSI_GAIN_SET_2_DEFAULT                                      0x5A59
#define BLE_FE_REGS_RSSI_GAIN_SET_2_RSSI_GAIN_STAGE8                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_RSSI_GAIN_SET_2_RSSI_GAIN_STAGE8_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_2_RSSI_GAIN_STAGE8_SHIFT                       (0)
#define BLE_FE_REGS_RSSI_GAIN_SET_2_RSSI_GAIN_STAGE9                             (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_RSSI_GAIN_SET_2_RSSI_GAIN_STAGE9_MASK                        (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_GAIN_SET_2_RSSI_GAIN_STAGE9_SHIFT                       (8)


#define BLE_FE_REGS_RSSI_CONSTANT_OFFSET                                         (BLE_FE_REGS_BASE + 0x120)
#define BLE_FE_REGS_RSSI_CONSTANT_OFFSET_DEFAULT                                 0x25C
#define BLE_FE_REGS_RSSI_CONSTANT_OFFSET_RSSI_OFFSET                             (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9)
#define BLE_FE_REGS_RSSI_CONSTANT_OFFSET_RSSI_OFFSET_MASK                        (Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_CONSTANT_OFFSET_RSSI_OFFSET_SHIFT                       (0)


#define BLE_FE_REGS_RSSI_CALC                                                    (BLE_FE_REGS_BASE + 0x124)
#define BLE_FE_REGS_RSSI_CALC_DEFAULT                                            0x0
#define BLE_FE_REGS_RSSI_CALC_POWDB_INV                                          (Bit0)


#define BLE_FE_REGS_RSSI_OUT                                                     (BLE_FE_REGS_BASE + 0x128)
#define BLE_FE_REGS_RSSI_OUT_DEFAULT                                             0x0
#define BLE_FE_REGS_RSSI_OUT_STS_CURRENT_RSSI                                    (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9)
#define BLE_FE_REGS_RSSI_OUT_STS_CURRENT_RSSI_MASK                               (Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_RSSI_OUT_STS_CURRENT_RSSI_SHIFT                              (0)


#define BLE_FE_REGS_FECE_CTL_I                                                   (BLE_FE_REGS_BASE + 0x12c)
#define BLE_FE_REGS_FECE_CTL_I_DEFAULT                                           0x18000
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_RX_EN                                    (Bit0)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TX_EN                                    (Bit1)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_REG_TRIG                                 (Bit2)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_CAPTURE_ACK                              (Bit3)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TRIGGER_SOURCE                           (Bit4+Bit5)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TRIGGER_SOURCE_MASK                      (Bit1+Bit0)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TRIGGER_SOURCE_SHIFT                     (4)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_CAPTURE_POINT                            (Bit8+Bit9+Bit10)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_CAPTURE_POINT_MASK                       (Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_CAPTURE_POINT_SHIFT                      (8)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TX_ONESHOT                               (Bit11)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TX_EMIT_PT                               (Bit12)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TX_START_DLY                             (Bit15+Bit16+Bit17)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TX_START_DLY_MASK                        (Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_CTL_I_CTL_FECE_TX_START_DLY_SHIFT                       (15)


#define BLE_FE_REGS_FECE_HISTORY_DEPTH                                           (BLE_FE_REGS_BASE + 0x130)
#define BLE_FE_REGS_FECE_HISTORY_DEPTH_DEFAULT                                   0x0
#define BLE_FE_REGS_FECE_HISTORY_DEPTH_CTL_FECE_HISTORY_DEPTH                    (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12)
#define BLE_FE_REGS_FECE_HISTORY_DEPTH_CTL_FECE_HISTORY_DEPTH_MASK               (Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_HISTORY_DEPTH_CTL_FECE_HISTORY_DEPTH_SHIFT              (0)


#define BLE_FE_REGS_FECE_TX_LAST_ADDR                                            (BLE_FE_REGS_BASE + 0x134)
#define BLE_FE_REGS_FECE_TX_LAST_ADDR_DEFAULT                                    0x0
#define BLE_FE_REGS_FECE_TX_LAST_ADDR_CTL_FECE_TX_RD_LAST_ADDR                   (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12)
#define BLE_FE_REGS_FECE_TX_LAST_ADDR_CTL_FECE_TX_RD_LAST_ADDR_MASK              (Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_TX_LAST_ADDR_CTL_FECE_TX_RD_LAST_ADDR_SHIFT             (0)


#define BLE_FE_REGS_FECE_STS_I                                                   (BLE_FE_REGS_BASE + 0x13c)
#define BLE_FE_REGS_FECE_STS_I_DEFAULT                                           0x0
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_CAP_DONE                                 (Bit0)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_HIST_CAP_DONE                            (Bit1)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_RX_STATE                                 (Bit2+Bit3+Bit4)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_RX_STATE_MASK                            (Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_RX_STATE_SHIFT                           (2)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_TX_STATE                                 (Bit5+Bit6+Bit7)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_TX_STATE_MASK                            (Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_STS_I_STS_FECE_TX_STATE_SHIFT                           (5)


#define BLE_FE_REGS_FECE_FIRST_SAMP_ADDR                                         (BLE_FE_REGS_BASE + 0x140)
#define BLE_FE_REGS_FECE_FIRST_SAMP_ADDR_DEFAULT                                 0x0
#define BLE_FE_REGS_FECE_FIRST_SAMP_ADDR_STS_FECE_ADDR_OF_SAMP1                  (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12)
#define BLE_FE_REGS_FECE_FIRST_SAMP_ADDR_STS_FECE_ADDR_OF_SAMP1_MASK             (Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_FIRST_SAMP_ADDR_STS_FECE_ADDR_OF_SAMP1_SHIFT            (0)


#define BLE_FE_REGS_FECE_TX_LOOP_CNT                                             (BLE_FE_REGS_BASE + 0x144)
#define BLE_FE_REGS_FECE_TX_LOOP_CNT_DEFAULT                                     0x0
#define BLE_FE_REGS_FECE_TX_LOOP_CNT_STS_FECE_TX_LOOP_CNT                        (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_FECE_TX_LOOP_CNT_STS_FECE_TX_LOOP_CNT_MASK                   (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_TX_LOOP_CNT_STS_FECE_TX_LOOP_CNT_SHIFT                  (0)


#define BLE_FE_REGS_FECE_WAKEUP_DLY                                              (BLE_FE_REGS_BASE + 0x148)
#define BLE_FE_REGS_FECE_WAKEUP_DLY_DEFAULT                                      0x1
#define BLE_FE_REGS_FECE_WAKEUP_DLY_CTL_FECE_WAKEUP_DLY                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define BLE_FE_REGS_FECE_WAKEUP_DLY_CTL_FECE_WAKEUP_DLY_MASK                     (Bit23+Bit22+Bit21+Bit20+Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FECE_WAKEUP_DLY_CTL_FECE_WAKEUP_DLY_SHIFT                    (0)


#define BLE_FE_REGS_TX_GAIN                                                      (BLE_FE_REGS_BASE + 0x2ac)
#define BLE_FE_REGS_TX_GAIN_DEFAULT                                              0xE090
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_I                                            (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_I_MASK                                       (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_I_SHIFT                                      (0)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_Q                                            (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_Q_MASK                                       (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_Q_SHIFT                                      (8)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_I_MUX_SEL                                    (Bit16)
#define BLE_FE_REGS_TX_GAIN_TX_GAIN_Q_MUX_SEL                                    (Bit17)


#define BLE_FE_REGS_FE_IRQ_CTL                                                   (BLE_FE_REGS_BASE + 0x3e8)
#define BLE_FE_REGS_FE_IRQ_CTL_DEFAULT                                           0xF
#define BLE_FE_REGS_FE_IRQ_CTL_MASK                                              (Bit0+Bit1+Bit2+Bit3)
#define BLE_FE_REGS_FE_IRQ_CTL_MASK_MASK                                         (Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FE_IRQ_CTL_MASK_SHIFT                                        (0)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL0                                          (Bit4+Bit5+Bit6+Bit7+Bit8)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL0_MASK                                     (Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL0_SHIFT                                    (4)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL1                                          (Bit9+Bit10+Bit11+Bit12+Bit13)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL1_MASK                                     (Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL1_SHIFT                                    (9)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL2                                          (Bit14+Bit15+Bit16+Bit17+Bit18)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL2_MASK                                     (Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL2_SHIFT                                    (14)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL3                                          (Bit19+Bit20+Bit21+Bit22+Bit23)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL3_MASK                                     (Bit4+Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FE_IRQ_CTL_SRC_SEL3_SHIFT                                    (19)


#define BLE_FE_REGS_FE_IRQ                                                       (BLE_FE_REGS_BASE + 0x3ec)
#define BLE_FE_REGS_FE_IRQ_DEFAULT                                               0x0
#define BLE_FE_REGS_FE_IRQ_IRQ                                                   (Bit0+Bit1+Bit2+Bit3)
#define BLE_FE_REGS_FE_IRQ_IRQ_MASK                                              (Bit3+Bit2+Bit1+Bit0)
#define BLE_FE_REGS_FE_IRQ_IRQ_SHIFT                                             (0)


