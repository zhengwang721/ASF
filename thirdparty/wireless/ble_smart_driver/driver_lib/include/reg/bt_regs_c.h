/* **************************** */
#define BT_REGS_VERSION                                                                                  (BT_REGS_BASE + 0x0)
#define BT_REGS_VERSION_DEFAULT                                                                          0x0
#define BT_REGS_VERSION_BT_SUBDAY                                                                        (Bit0+Bit1+Bit2+Bit3)
#define BT_REGS_VERSION_BT_SUBDAY_MASK                                                                   (Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_VERSION_BT_SUBDAY_SHIFT                                                                  (0)
#define BT_REGS_VERSION_BT_DAY                                                                           (Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11)
#define BT_REGS_VERSION_BT_DAY_MASK                                                                      (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_VERSION_BT_DAY_SHIFT                                                                     (4)
#define BT_REGS_VERSION_BT_MONTH                                                                         (Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19)
#define BT_REGS_VERSION_BT_MONTH_MASK                                                                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_VERSION_BT_MONTH_SHIFT                                                                   (12)
#define BT_REGS_VERSION_BT_YEAR                                                                          (Bit20+Bit21+Bit22+Bit23)
#define BT_REGS_VERSION_BT_YEAR_MASK                                                                     (Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_VERSION_BT_YEAR_SHIFT                                                                    (20)
#define BT_REGS_VERSION_BT_VERSION                                                                       (Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30+Bit31)
#define BT_REGS_VERSION_BT_VERSION_MASK                                                                  (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_VERSION_BT_VERSION_SHIFT                                                                 (24)


#define BT_REGS_TOP_CTL                                                                                  (BT_REGS_BASE + 0x4)
#define BT_REGS_TOP_CTL_DEFAULT                                                                          0x12A0D2
#define BT_REGS_TOP_CTL_CTL_MMX_ENABLE                                                                   (Bit0)
#define BT_REGS_TOP_CTL_CTL_DFE_BYPASS                                                                   (Bit1)
#define BT_REGS_TOP_CTL_CTL_COR_CLKSEL                                                                   (Bit4+Bit5+Bit6+Bit7+Bit8+Bit9)
#define BT_REGS_TOP_CTL_CTL_COR_CLKSEL_MASK                                                              (Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_TOP_CTL_CTL_COR_CLKSEL_SHIFT                                                             (4)
#define BT_REGS_TOP_CTL_CTL_COR_PHASE52_13_SEL                                                           (Bit12+Bit13)
#define BT_REGS_TOP_CTL_CTL_COR_PHASE52_13_SEL_MASK                                                      (Bit1+Bit0)
#define BT_REGS_TOP_CTL_CTL_COR_PHASE52_13_SEL_SHIFT                                                     (12)
#define BT_REGS_TOP_CTL_CTL_FE_PHASE52_13_SEL                                                            (Bit14+Bit15)
#define BT_REGS_TOP_CTL_CTL_FE_PHASE52_13_SEL_MASK                                                       (Bit1+Bit0)
#define BT_REGS_TOP_CTL_CTL_FE_PHASE52_13_SEL_SHIFT                                                      (14)
#define BT_REGS_TOP_CTL_CTL_COR_PHASE_MATCH_P_SEL                                                        (Bit16+Bit17)
#define BT_REGS_TOP_CTL_CTL_COR_PHASE_MATCH_P_SEL_MASK                                                   (Bit1+Bit0)
#define BT_REGS_TOP_CTL_CTL_COR_PHASE_MATCH_P_SEL_SHIFT                                                  (16)
#define BT_REGS_TOP_CTL_CTL_WAKEUP_LP_LATCH                                                              (Bit20)
#define BT_REGS_TOP_CTL_CTL_DEEP_SLEEP_STAT_OUT_REG13M                                                   (Bit21)
#define BT_REGS_TOP_CTL_CTL_DEEP_SLEEP_STAT_32K_IN_REG13M                                                (Bit22+Bit23)
#define BT_REGS_TOP_CTL_CTL_DEEP_SLEEP_STAT_32K_IN_REG13M_MASK                                           (Bit1+Bit0)
#define BT_REGS_TOP_CTL_CTL_DEEP_SLEEP_STAT_32K_IN_REG13M_SHIFT                                          (22)
#define BT_REGS_TOP_CTL_CTL_FSMFIX4_CNT_SAMP                                                             (Bit24)
#define BT_REGS_TOP_CTL_CTL_MDM_DL_FLUSH_EN                                                              (Bit25)
#define BT_REGS_TOP_CTL_CTL_USE_NMI_LP                                                                   (Bit26)
#define BT_REGS_TOP_CTL_CTL_DISABLE_BLE_REG_HCLK                                                         (Bit27)
#define BT_REGS_TOP_CTL_CTL_USE_PSD_CORDIC_8_ITS                                                         (Bit28)
#define BT_REGS_TOP_CTL_CTL_USE_FSC_CORDIC_4_OR_8_ITS                                                    (Bit29+Bit30)
#define BT_REGS_TOP_CTL_CTL_USE_FSC_CORDIC_4_OR_8_ITS_MASK                                               (Bit1+Bit0)
#define BT_REGS_TOP_CTL_CTL_USE_FSC_CORDIC_4_OR_8_ITS_SHIFT                                              (29)


#define BT_REGS_CLK_FORCE_ON                                                                             (BT_REGS_BASE + 0x8)
#define BT_REGS_CLK_FORCE_ON_DEFAULT                                                                     0x0
#define BT_REGS_CLK_FORCE_ON_CTL_COR_FORCE_CLK_ON                                                        (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12)
#define BT_REGS_CLK_FORCE_ON_CTL_COR_FORCE_CLK_ON_MASK                                                   (Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_CLK_FORCE_ON_CTL_COR_FORCE_CLK_ON_SHIFT                                                  (0)
#define BT_REGS_CLK_FORCE_ON_CTL_MDM_FORCE_CLK_ON                                                        (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30)
#define BT_REGS_CLK_FORCE_ON_CTL_MDM_FORCE_CLK_ON_MASK                                                   (Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_CLK_FORCE_ON_CTL_MDM_FORCE_CLK_ON_SHIFT                                                  (16)


#define BT_REGS_CLK_FORCE_OFF                                                                            (BT_REGS_BASE + 0xc)
#define BT_REGS_CLK_FORCE_OFF_DEFAULT                                                                    0x2000000
#define BT_REGS_CLK_FORCE_OFF_CTL_COR_FORCE_CLK_OFF                                                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12)
#define BT_REGS_CLK_FORCE_OFF_CTL_COR_FORCE_CLK_OFF_MASK                                                 (Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_CLK_FORCE_OFF_CTL_COR_FORCE_CLK_OFF_SHIFT                                                (0)
#define BT_REGS_CLK_FORCE_OFF_CTL_MDM_FORCE_CLK_OFF                                                      (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29+Bit30)
#define BT_REGS_CLK_FORCE_OFF_CTL_MDM_FORCE_CLK_OFF_MASK                                                 (Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_CLK_FORCE_OFF_CTL_MDM_FORCE_CLK_OFF_SHIFT                                                (16)


#define BT_REGS_DATA_WHITENING                                                                           (BT_REGS_BASE + 0x14)
#define BT_REGS_DATA_WHITENING_DEFAULT                                                                   0x0
#define BT_REGS_DATA_WHITENING_CTL_COR_DW_LFSR_INIT                                                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6)
#define BT_REGS_DATA_WHITENING_CTL_COR_DW_LFSR_INIT_MASK                                                 (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_DATA_WHITENING_CTL_COR_DW_LFSR_INIT_SHIFT                                                (0)
#define BT_REGS_DATA_WHITENING_CTL_COR_DW_OVERRIDE                                                       (Bit8)


#define BT_REGS_TUNER_INTF                                                                               (BT_REGS_BASE + 0x18)
#define BT_REGS_TUNER_INTF_DEFAULT                                                                       0x1D5A
#define BT_REGS_TUNER_INTF_CTL_RAD_RSSI_TO_FORCE                                                         (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7)
#define BT_REGS_TUNER_INTF_CTL_RAD_RSSI_TO_FORCE_MASK                                                    (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_TUNER_INTF_CTL_RAD_RSSI_TO_FORCE_SHIFT                                                   (0)
#define BT_REGS_TUNER_INTF_CTL_RAD_USE_NMI_RSSI                                                          (Bit8)
#define BT_REGS_TUNER_INTF_CTL_RAD_FORCE_RSSI                                                            (Bit9)
#define BT_REGS_TUNER_INTF_CTL_RAD_EN_UPDATE_PULSE                                                       (Bit10+Bit11+Bit12)
#define BT_REGS_TUNER_INTF_CTL_RAD_EN_UPDATE_PULSE_MASK                                                  (Bit2+Bit1+Bit0)
#define BT_REGS_TUNER_INTF_CTL_RAD_EN_UPDATE_PULSE_SHIFT                                                 (10)
#define BT_REGS_TUNER_INTF_CTL_RAD_FORCE_BT2RF_TX_EDR                                                    (Bit13)
#define BT_REGS_TUNER_INTF_CTL_RAD_BT2RF_TX_EDR_TO_FORCE                                                 (Bit14)
#define BT_REGS_TUNER_INTF_CTL_RAD_UPDATE_LOC                                                            (Bit15+Bit16)
#define BT_REGS_TUNER_INTF_CTL_RAD_UPDATE_LOC_MASK                                                       (Bit1+Bit0)
#define BT_REGS_TUNER_INTF_CTL_RAD_UPDATE_LOC_SHIFT                                                      (15)


#define BT_REGS_FPGA                                                                                     (BT_REGS_BASE + 0x1c)
#define BT_REGS_FPGA_DEFAULT                                                                             0x0
#define BT_REGS_FPGA_CTL_FPGA_RIP_CAP_NEGEDGE                                                            (Bit0)
#define BT_REGS_FPGA_CTL_FPGA_RX_NMI                                                                     (Bit1)
#define BT_REGS_FPGA_CTL_FPGA_TX_NMI                                                                     (Bit2)
#define BT_REGS_FPGA_CTL_FPGA_MDMACC_APB                                                                 (Bit3)
#define BT_REGS_FPGA_CTL_FPGA_MISC                                                                       (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23)
#define BT_REGS_FPGA_CTL_FPGA_MISC_MASK                                                                  (Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_FPGA_CTL_FPGA_MISC_SHIFT                                                                 (16)


#define BT_REGS_DBG_OUT                                                                                  (BT_REGS_BASE + 0x20)
#define BT_REGS_DBG_OUT_DEFAULT                                                                          0x0
#define BT_REGS_DBG_OUT_CTL_DBG_SIG_SEL                                                                  (Bit0+Bit1)
#define BT_REGS_DBG_OUT_CTL_DBG_SIG_SEL_MASK                                                             (Bit1+Bit0)
#define BT_REGS_DBG_OUT_CTL_DBG_SIG_SEL_SHIFT                                                            (0)
#define BT_REGS_DBG_OUT_CTL_DBG_BYTE_SHIFT                                                               (Bit2+Bit3)
#define BT_REGS_DBG_OUT_CTL_DBG_BYTE_SHIFT_MASK                                                          (Bit1+Bit0)
#define BT_REGS_DBG_OUT_CTL_DBG_BYTE_SHIFT_SHIFT                                                         (2)


#define BT_REGS_SLEEP_INTF                                                                               (BT_REGS_BASE + 0x24)
#define BT_REGS_SLEEP_INTF_DEFAULT                                                                       0x0
#define BT_REGS_SLEEP_INTF_STS_SLP_WAKEUP_REQ                                                            (Bit0)
#define BT_REGS_SLEEP_INTF_STS_SLP_CLK_STATUS                                                            (Bit1)
#define BT_REGS_SLEEP_INTF_STS_SLP_OSC_EN                                                                (Bit2)
#define BT_REGS_SLEEP_INTF_STS_SLP_RADIO_EN                                                              (Bit3)
#define BT_REGS_SLEEP_INTF_CTL_SLP_EN_FORCE_OSC_EN                                                       (Bit4)
#define BT_REGS_SLEEP_INTF_CTL_SLP_FORCE_OSC_EN                                                          (Bit5)
#define BT_REGS_SLEEP_INTF_CTL_SLP_EN_FORCE_RADIO_EN                                                     (Bit6)
#define BT_REGS_SLEEP_INTF_CTL_SLP_FORCE_RADIO_EN                                                        (Bit7)
#define BT_REGS_SLEEP_INTF_CTL_SLP_EN_FORCE_WAKEUP_REQ                                                   (Bit8)
#define BT_REGS_SLEEP_INTF_CTL_SLP_FORCE_WAKEUP_REQ                                                      (Bit9)


#define BT_REGS_COEX_INTF                                                                                (BT_REGS_BASE + 0x28)
#define BT_REGS_COEX_INTF_DEFAULT                                                                        0x5000
#define BT_REGS_COEX_INTF_STS_COEX_WLAN_TX                                                               (Bit0)
#define BT_REGS_COEX_INTF_STS_COEX_WLAN_RX                                                               (Bit1)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_TX                                                                (Bit2)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_RX                                                                (Bit3)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_IN_PROCESS                                                        (Bit4)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_MBSY                                                              (Bit5)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_SYNC                                                              (Bit6)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_RXNTX                                                             (Bit7)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_PTI                                                               (Bit8+Bit9+Bit10+Bit11)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_PTI_MASK                                                          (Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_COEX_INTF_STS_COEX_BLE_PTI_SHIFT                                                         (8)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_WLAN_TX                                                      (Bit12)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_WLAN_TX                                                         (Bit13)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_WLAN_RX                                                      (Bit14)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_WLAN_RX                                                         (Bit15)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_BLE_TX                                                       (Bit16)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_BLE_TX                                                          (Bit17)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_BLE_RX                                                       (Bit18)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_BLE_RX                                                          (Bit19)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_BLE_IN_PROCESS                                               (Bit20)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_BLE_IN_PROCESS                                                  (Bit21)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_BLE_MBSY                                                     (Bit22)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_BLE_MBSY                                                        (Bit23)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_BLE_SYNC                                                     (Bit24)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_BLE_SYNC                                                        (Bit25)
#define BT_REGS_COEX_INTF_CTL_COEX_EN_FORCE_BLE_RXNTX                                                    (Bit26)
#define BT_REGS_COEX_INTF_CTL_COEX_FORCE_BLE_RXNTX                                                       (Bit27)


#define BT_REGS_COEX_INTF_2                                                                              (BT_REGS_BASE + 0x2c)
#define BT_REGS_COEX_INTF_2_DEFAULT                                                                      0x3C
#define BT_REGS_COEX_INTF_2_CTL_COEX_EN_FORCE_DNABORT                                                    (Bit0)
#define BT_REGS_COEX_INTF_2_CTL_COEX_FORCE_DNABORT                                                       (Bit1)
#define BT_REGS_COEX_INTF_2_CTL_COEX_EN_FORCE_RXBSYENA                                                   (Bit2)
#define BT_REGS_COEX_INTF_2_CTL_COEX_FORCE_RXBSYENA                                                      (Bit3)
#define BT_REGS_COEX_INTF_2_CTL_COEX_EN_FORCE_TXBSYENA                                                   (Bit4)
#define BT_REGS_COEX_INTF_2_CTL_COEX_FORCE_TXBSYENA                                                      (Bit5)
#define BT_REGS_COEX_INTF_2_CTL_COEX_EN_FORCE_BLE_PTI                                                    (Bit8)
#define BT_REGS_COEX_INTF_2_CTL_COEX_FORCE_BLE_PTI                                                       (Bit9+Bit10+Bit11+Bit12)
#define BT_REGS_COEX_INTF_2_CTL_COEX_FORCE_BLE_PTI_MASK                                                  (Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_COEX_INTF_2_CTL_COEX_FORCE_BLE_PTI_SHIFT                                                 (9)


#define BT_REGS_COEX_LEGACY_CFG                                                                          (BT_REGS_BASE + 0x30)
#define BT_REGS_COEX_LEGACY_CFG_DEFAULT                                                                  0x30
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCMODE                                                              (Bit0+Bit1)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCMODE_MASK                                                         (Bit1+Bit0)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCMODE_SHIFT                                                        (0)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPRIOMODE                                                          (Bit2+Bit3)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPRIOMODE_MASK                                                     (Bit1+Bit0)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPRIOMODE_SHIFT                                                    (2)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCTXRXPOL                                                           (Bit4)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCTXRXPOL_LTCH                                                      (Bit5)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCMBSYACTIVE                                                        (Bit6)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPDELAY                                                            (Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPDELAY_MASK                                                       (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPDELAY_SHIFT                                                      (8)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPDURATION                                                         (Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPDURATION_MASK                                                    (Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPDURATION_SHIFT                                                   (16)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPTHRESHOLD                                                        (Bit24+Bit25+Bit26+Bit27+Bit28)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPTHRESHOLD_MASK                                                   (Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_COEX_LEGACY_CFG_CTL_WLCPTHRESHOLD_SHIFT                                                  (24)


#define BT_REGS_FE_CLK_FORCE_ON                                                                          (BT_REGS_BASE + 0x40)
#define BT_REGS_FE_CLK_FORCE_ON_DEFAULT                                                                  0x0
#define BT_REGS_FE_CLK_FORCE_ON_CTL_FE_FORCE_CLK_ON                                                      (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BT_REGS_FE_CLK_FORCE_ON_CTL_FE_FORCE_CLK_ON_MASK                                                 (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_FE_CLK_FORCE_ON_CTL_FE_FORCE_CLK_ON_SHIFT                                                (0)


#define BT_REGS_FE_CLK_FORCE_OFF                                                                         (BT_REGS_BASE + 0x44)
#define BT_REGS_FE_CLK_FORCE_OFF_DEFAULT                                                                 0x0
#define BT_REGS_FE_CLK_FORCE_OFF_CTL_FE_FORCE_CLK_OFF                                                    (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15)
#define BT_REGS_FE_CLK_FORCE_OFF_CTL_FE_FORCE_CLK_OFF_MASK                                               (Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_FE_CLK_FORCE_OFF_CTL_FE_FORCE_CLK_OFF_SHIFT                                              (0)


#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_STS                                                             (BT_REGS_BASE + 0x50)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_STS_DEFAULT                                                     0x0
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_STS_STS_DEEP_SLEEP_HW_TIMER_CORR_BUSY                           (Bit0)


#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME                                                        (BT_REGS_BASE + 0x54)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME_DEFAULT                                                0x0
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME_STS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME                  (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26+Bit27+Bit28+Bit29)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME_STS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME_MASK             (Bit29+Bit28+Bit27+Bit26+Bit25+Bit24+Bit23+Bit22+Bit21+Bit20+Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME_STS_DEEP_SLEEP_HW_TIMER_CORR_BASETIME_SHIFT            (0)


#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME                                                        (BT_REGS_BASE + 0x58)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME_DEFAULT                                                0x0
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME_STS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME                  (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME_STS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME_MASK             (Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME_STS_DEEP_SLEEP_HW_TIMER_CORR_FINETIME_SHIFT            (0)


#define BT_REGS_FINE_TIMER_LOAD_VALUE                                                                    (BT_REGS_BASE + 0x60)
#define BT_REGS_FINE_TIMER_LOAD_VALUE_DEFAULT                                                            0x0
#define BT_REGS_FINE_TIMER_LOAD_VALUE_CTL_FINE_TIMER_LOAD_VALUE                                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9)
#define BT_REGS_FINE_TIMER_LOAD_VALUE_CTL_FINE_TIMER_LOAD_VALUE_MASK                                     (Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_FINE_TIMER_LOAD_VALUE_CTL_FINE_TIMER_LOAD_VALUE_SHIFT                                    (0)


#define BT_REGS_BASE_TIMER_LOAD_VALUE                                                                    (BT_REGS_BASE + 0x64)
#define BT_REGS_BASE_TIMER_LOAD_VALUE_DEFAULT                                                            0x0
#define BT_REGS_BASE_TIMER_LOAD_VALUE_CTL_BASE_TIMER_LOAD_VALUE                                          (Bit0+Bit1+Bit2+Bit3+Bit4+Bit5+Bit6+Bit7+Bit8+Bit9+Bit10+Bit11+Bit12+Bit13+Bit14+Bit15+Bit16+Bit17+Bit18+Bit19+Bit20+Bit21+Bit22+Bit23+Bit24+Bit25+Bit26)
#define BT_REGS_BASE_TIMER_LOAD_VALUE_CTL_BASE_TIMER_LOAD_VALUE_MASK                                     (Bit26+Bit25+Bit24+Bit23+Bit22+Bit21+Bit20+Bit19+Bit18+Bit17+Bit16+Bit15+Bit14+Bit13+Bit12+Bit11+Bit10+Bit9+Bit8+Bit7+Bit6+Bit5+Bit4+Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_BASE_TIMER_LOAD_VALUE_CTL_BASE_TIMER_LOAD_VALUE_SHIFT                                    (0)


#define BT_REGS_LOAD_TIMER                                                                               (BT_REGS_BASE + 0x68)
#define BT_REGS_LOAD_TIMER_DEFAULT                                                                       0x0
#define BT_REGS_LOAD_TIMER_STS_LOAD_TIMER_FINE                                                           (Bit0)
#define BT_REGS_LOAD_TIMER_STS_LOAD_TIMER_BASE                                                           (Bit1)


#define BT_REGS_TEST                                                                                     (BT_REGS_BASE + 0x80)
#define BT_REGS_TEST_DEFAULT                                                                             0x0
#define BT_REGS_TEST_CTL_TEST_SEL                                                                        (Bit0+Bit1+Bit2+Bit3)
#define BT_REGS_TEST_CTL_TEST_SEL_MASK                                                                   (Bit3+Bit2+Bit1+Bit0)
#define BT_REGS_TEST_CTL_TEST_SEL_SHIFT                                                                  (0)
#define BT_REGS_TEST_CTL_TEST_DAC_SEL                                                                    (Bit12+Bit13+Bit14)
#define BT_REGS_TEST_CTL_TEST_DAC_SEL_MASK                                                               (Bit2+Bit1+Bit0)
#define BT_REGS_TEST_CTL_TEST_DAC_SEL_SHIFT                                                              (12)


