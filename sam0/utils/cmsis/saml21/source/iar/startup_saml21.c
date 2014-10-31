/**
 * \file
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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

#if 0 // #ifndef SAML21_REV_A
#include "saml21.h"
#else // For RevA0 workaround
#include "saml21_internal.h"
#endif

<<<<<<< HEAD
#if 1 // SAML21_REV_A
// fuses_params.h BEGIN ===============================================
/** @file fuses_params.h
*/
//! @par  THIS FILE IS AUTO GENERATED! DO NOT EDIT!


//! @par  Parameters definition for CAL row
typedef struct {
  uint64_t CALIB_VREFTEMP           : 4; //!< Temperature Calibration of Bandgap
  uint64_t CALIB_VREFPL0            : 8; //!< VREF Calibration of VREG in PL0
  uint64_t CALIB_VREF0              : 8; //!< VREF0 Calibration
  uint64_t BOD33STARTLEVEL          : 6; //!< BOD33 Start Level
  uint64_t BOD12STARTLEVEL          : 6; //!< BOD12 Start Level
  uint64_t OSCULP32K                : 6; //!< OSCULP32K Calibration
  uint64_t OSC16M_CAL4M_TCAL        : 6; //!< OSC16M Temp Calibration (4MHz)
  uint64_t OSC16M_CAL4M_FCAL        : 6; //!< OSC16M Freq Calibration (4MHz)
  uint64_t BUVREG_BKUPCALIB         : 4; //!< BU VREG calibration in backup
  uint64_t BUVREG_NOBKUPCALIB       : 4; //!< BU VREG calibration in mode other than backup
  uint64_t NVMCTRL_MENSTARTUP_DIS   : 1; //!< NVM deep power down mode during PU1 disable fuse
  uint64_t                          : 5;
} FUSEMAP_CAL_type;

#define FUSES_CALIB_VREFTEMP_OFFSET           0   //!< [3:0] Temperature Calibration of Bandgap
#define FUSES_CALIB_VREFTEMP_SIZE             4
#define FUSES_CALIB_VREFTEMP_ADDR             0x00800000
#define FUSES_CALIB_VREFTEMP_MASK             0x0000000F

#define FUSES_CALIB_VREFPL0_OFFSET            4   //!< [11:4] VREF Calibration of VREG in PL0
#define FUSES_CALIB_VREFPL0_SIZE              8
#define FUSES_CALIB_VREFPL0_ADDR              0x00800000
#define FUSES_CALIB_VREFPL0_MASK              0x00000FF0

#define FUSES_CALIB_VREF0_OFFSET              12  //!< [19:12] VREF0 Calibration
#define FUSES_CALIB_VREF0_SIZE                8
#define FUSES_CALIB_VREF0_ADDR                0x00800000
#define FUSES_CALIB_VREF0_MASK                0x000FF000

#define FUSES_BOD33STARTLEVEL_OFFSET          20  //!< [25:20] BOD33 Start Level
#define FUSES_BOD33STARTLEVEL_SIZE            6
#define FUSES_BOD33STARTLEVEL_ADDR            0x00800000
#define FUSES_BOD33STARTLEVEL_MASK            0x03F00000

#define FUSES_BOD12STARTLEVEL_OFFSET          26  //!< [31:26] BOD12 Start Level
#define FUSES_BOD12STARTLEVEL_SIZE            6
#define FUSES_BOD12STARTLEVEL_ADDR            0x00800000
#define FUSES_BOD12STARTLEVEL_MASK            0xFC000000

#define FUSES_OSCULP32K_OFFSET                0   //!< [37:32] OSCULP32K Calibration
#define FUSES_OSCULP32K_SIZE                  6
#define FUSES_OSCULP32K_ADDR                  0x00800004
#define FUSES_OSCULP32K_MASK                  0x0000003F

#define FUSES_OSC16M_CAL4M_TCAL_OFFSET        6   //!< [43:38] OSC16M Temp Calibration (4MHz)
#define FUSES_OSC16M_CAL4M_TCAL_SIZE          6
#define FUSES_OSC16M_CAL4M_TCAL_ADDR          0x00800004
#define FUSES_OSC16M_CAL4M_TCAL_MASK          0x00000FC0

#define FUSES_OSC16M_CAL4M_FCAL_OFFSET        12  //!< [49:44] OSC16M Freq Calibration (4MHz)
#define FUSES_OSC16M_CAL4M_FCAL_SIZE          6
#define FUSES_OSC16M_CAL4M_FCAL_ADDR          0x00800004
#define FUSES_OSC16M_CAL4M_FCAL_MASK          0x0003F000

#define FUSES_BUVREG_BKUPCALIB_OFFSET         18  //!< [53:50] BU VREG calibration in backup
#define FUSES_BUVREG_BKUPCALIB_SIZE           4
#define FUSES_BUVREG_BKUPCALIB_ADDR           0x00800004
#define FUSES_BUVREG_BKUPCALIB_MASK           0x003C0000

#define FUSES_BUVREG_NOBKUPCALIB_OFFSET       22  //!< [57:54] BU VREG calibration in mode other than backup
#define FUSES_BUVREG_NOBKUPCALIB_SIZE         4
#define FUSES_BUVREG_NOBKUPCALIB_ADDR         0x00800004
#define FUSES_BUVREG_NOBKUPCALIB_MASK         0x03C00000

#define FUSES_NVMCTRL_MENSTARTUP_DIS_OFFSET   26  //!< [58] NVM deep power down mode during PU1 disable fuse
#define FUSES_NVMCTRL_MENSTARTUP_DIS_SIZE     1
#define FUSES_NVMCTRL_MENSTARTUP_DIS_ADDR     0x00800004
#define FUSES_NVMCTRL_MENSTARTUP_DIS_MASK     0x04000000

//! @par  Parameters definition for LOCKBIT row
typedef struct {
  uint64_t NVMCTRL_LOCKFIELD        : 8; //!< LOCK Region
  uint64_t                          :56;
} FUSEMAP_LOCKBIT_type;

#define FUSES_NVMCTRL_LOCKFIELD_OFFSET        0   //!< [7:0] LOCK Region
#define FUSES_NVMCTRL_LOCKFIELD_SIZE          8
#define FUSES_NVMCTRL_LOCKFIELD_ADDR          0x00802000
#define FUSES_NVMCTRL_LOCKFIELD_MASK          0x000000FF

//! @par  Parameters definition for OTP1 row
typedef struct {
  uint64_t NVMCTRL_NVM_LOCK         : 8; //!< NVM Lock
  uint64_t NVMCTRL_PSZ              : 2; //!< NVM Page Size
  uint64_t                          : 6;
  uint64_t NVMCTRL_NVMP             :13; //!< Number of NVM Pages
  uint64_t                          : 3;
  uint64_t NVMCTRL_WWREEP           : 8; //!< None
  uint64_t                          :24;
} FUSEMAP_OTP1_type;

#define FUSES_NVMCTRL_NVM_LOCK_OFFSET         0   //!< [7:0] NVM Lock
#define FUSES_NVMCTRL_NVM_LOCK_SIZE           8
#define FUSES_NVMCTRL_NVM_LOCK_ADDR           0x00806000
#define FUSES_NVMCTRL_NVM_LOCK_MASK           0x000000FF

#define FUSES_NVMCTRL_PSZ_OFFSET              8   //!< [9:8] NVM Page Size
#define FUSES_NVMCTRL_PSZ_SIZE                2
#define FUSES_NVMCTRL_PSZ_ADDR                0x00806000
#define FUSES_NVMCTRL_PSZ_MASK                0x00000300

#define FUSES_NVMCTRL_NVMP_OFFSET             16  //!< [28:16] Number of NVM Pages
#define FUSES_NVMCTRL_NVMP_SIZE               13
#define FUSES_NVMCTRL_NVMP_ADDR               0x00806000
#define FUSES_NVMCTRL_NVMP_MASK               0x1FFF0000

#define FUSES_NVMCTRL_WWREEP_OFFSET           0   //!< [39:32] None
#define FUSES_NVMCTRL_WWREEP_SIZE             8
#define FUSES_NVMCTRL_WWREEP_ADDR             0x00806004
#define FUSES_NVMCTRL_WWREEP_MASK             0x000000FF

//! @par  Parameters definition for OTP2 row
typedef struct {
  uint64_t DSU_DID_DEVSEL           : 8; //!< Device select
  uint64_t DSU_DID_SERIES           : 3; //!< Device series
  uint64_t DSU_HSRAM_SIZE           : 4; //!< System RAM Size
  uint64_t DSUSTANDBY_PACKAGE_TYPE  : 4; //!< Package Option
  uint64_t DSUSTANDBY_ME_ADC        : 1; //!< ADC is implemented
  uint64_t DSUSTANDBY_ME_AC         : 1; //!< AC is implemented
  uint64_t DSUSTANDBY_ME_DAC        : 1; //!< DAC is implemented
  uint64_t DSUSTANDBY_DAC_DUAL      : 1; //!< None
  uint64_t DSUSTANDBY_ME_SERCOM45   : 1; //!< SERCOM4/5 are implemented
  uint64_t DSUSTANDBY_ME_TC23       : 1; //!< TC2/3 are implemented
  uint64_t DSUSTANDBY_ME_PTC        : 1; //!< PTC is implemented
  uint64_t DSU_ME_USB               : 1; //!< USB is implemented
  uint64_t DSU_USB_DEVONLY_N        : 1; //!< None
  uint64_t DSUSTANDBY_SIP_TYPE      : 2; //!< SIP Type
  uint64_t DSUSTANDBY_ME_OPAMP      : 1; //!< OPAMP is implemented
  uint64_t DSUSTANDBY_ME_AES        : 1; //!< AES is implemented
  uint64_t DSUSTANDBY_ME_TRNG       : 1; //!< TRNG is implemented
  uint64_t DSUSTANDBY_ME_PPP_N      : 1; //!< PPP is implemented
  uint64_t                          : 1;
  uint64_t DSUSTANDBY_ME_RFCTRL     : 1; //!< RFCTRL is implemente
  uint64_t DSUSTANDBY_LPRAM_SIZE    : 2; //!< None
  uint64_t DSUSTANDBY_PD1_RAM_BIAS  : 3; //!< PD1 RAM Bias
  uint64_t DSUSTANDBY_PD1_RAM_RMARGIN: 4; //!< PD1 RAM Read Margin
  uint64_t DSUSTANDBY_PD2_RAM_BIAS  : 3; //!< PD2 RAM Bias
  uint64_t DSU_PD2_RAM_RMARGIN      : 4; //!< PD2 RAM Read Margin
  uint64_t                          :12;
} FUSEMAP_OTP2_type;

#define FUSES_DSU_DID_DEVSEL_OFFSET           0   //!< [7:0] Device select
#define FUSES_DSU_DID_DEVSEL_SIZE             8
#define FUSES_DSU_DID_DEVSEL_ADDR             0x00806008
#define FUSES_DSU_DID_DEVSEL_MASK             0x000000FF

#define FUSES_DSU_DID_SERIES_OFFSET           8   //!< [10:8] Device series
#define FUSES_DSU_DID_SERIES_SIZE             3
#define FUSES_DSU_DID_SERIES_ADDR             0x00806008
#define FUSES_DSU_DID_SERIES_MASK             0x00000700

#define FUSES_DSU_HSRAM_SIZE_OFFSET           11  //!< [14:11] System RAM Size
#define FUSES_DSU_HSRAM_SIZE_SIZE             4
#define FUSES_DSU_HSRAM_SIZE_ADDR             0x00806008
#define FUSES_DSU_HSRAM_SIZE_MASK             0x00007800

#define FUSES_DSUSTANDBY_PACKAGE_TYPE_OFFSET  15  //!< [18:15] Package Option
#define FUSES_DSUSTANDBY_PACKAGE_TYPE_SIZE    4
#define FUSES_DSUSTANDBY_PACKAGE_TYPE_ADDR    0x00806008
#define FUSES_DSUSTANDBY_PACKAGE_TYPE_MASK    0x00078000

#define FUSES_DSUSTANDBY_ME_ADC_OFFSET        19  //!< [19] ADC is implemented
#define SYSTEM_DCFG_ME_ADC_POS                19  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_ADC_SIZE          1
#define FUSES_DSUSTANDBY_ME_ADC_ADDR          0x00806008
#define FUSES_DSUSTANDBY_ME_ADC_MASK          0x00080000

#define FUSES_DSUSTANDBY_ME_AC_OFFSET         20  //!< [20] AC is implemented
#define SYSTEM_DCFG_ME_AC_POS                 20  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_AC_SIZE           1
#define FUSES_DSUSTANDBY_ME_AC_ADDR           0x00806008
#define FUSES_DSUSTANDBY_ME_AC_MASK           0x00100000

#define FUSES_DSUSTANDBY_ME_DAC_OFFSET        21  //!< [21] DAC is implemented
#define SYSTEM_DCFG_ME_DAC_POS                21  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_DAC_SIZE          1
#define FUSES_DSUSTANDBY_ME_DAC_ADDR          0x00806008
#define FUSES_DSUSTANDBY_ME_DAC_MASK          0x00200000

#define FUSES_DSUSTANDBY_DAC_DUAL_OFFSET      22  //!< [22] None
#define FUSES_DSUSTANDBY_DAC_DUAL_SIZE        1
#define FUSES_DSUSTANDBY_DAC_DUAL_ADDR        0x00806008
#define FUSES_DSUSTANDBY_DAC_DUAL_MASK        0x00400000

#define FUSES_DSUSTANDBY_ME_SERCOM45_OFFSET   23  //!< [23] SERCOM4/5 are implemented
#define SYSTEM_DCFG_ME_SERCOM45_POS           23  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_SERCOM45_SIZE     1
#define FUSES_DSUSTANDBY_ME_SERCOM45_ADDR     0x00806008
#define FUSES_DSUSTANDBY_ME_SERCOM45_MASK     0x00800000

#define FUSES_DSUSTANDBY_ME_TC23_OFFSET       24  //!< [24] TC2/3 are implemented
#define SYSTEM_DCFG_ME_TC23_POS               24  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_TC23_SIZE         1
#define FUSES_DSUSTANDBY_ME_TC23_ADDR         0x00806008
#define FUSES_DSUSTANDBY_ME_TC23_MASK         0x01000000

#define FUSES_DSUSTANDBY_ME_PTC_OFFSET        25  //!< [25] PTC is implemented
#define SYSTEM_DCFG_ME_PTC_POS                25  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_PTC_SIZE          1
#define FUSES_DSUSTANDBY_ME_PTC_ADDR          0x00806008
#define FUSES_DSUSTANDBY_ME_PTC_MASK          0x02000000

#define FUSES_DSU_ME_USB_OFFSET               26  //!< [26] USB is implemented
#define SYSTEM_DCFG_ME_USB_POS                26  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSU_ME_USB_SIZE                 1
#define FUSES_DSU_ME_USB_ADDR                 0x00806008
#define FUSES_DSU_ME_USB_MASK                 0x04000000

#define FUSES_DSU_USB_DEVONLY_N_OFFSET        27  //!< [27] None
#define FUSES_DSU_USB_DEVONLY_N_SIZE          1
#define FUSES_DSU_USB_DEVONLY_N_ADDR          0x00806008
#define FUSES_DSU_USB_DEVONLY_N_MASK          0x08000000

#define FUSES_DSUSTANDBY_SIP_TYPE_OFFSET      28  //!< [29:28] SIP Type
#define FUSES_DSUSTANDBY_SIP_TYPE_SIZE        2
#define FUSES_DSUSTANDBY_SIP_TYPE_ADDR        0x00806008
#define FUSES_DSUSTANDBY_SIP_TYPE_MASK        0x30000000

#define FUSES_DSUSTANDBY_ME_OPAMP_OFFSET      30  //!< [30] OPAMP is implemented
#define SYSTEM_DCFG_ME_OPAMP_POS              30  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_OPAMP_SIZE        1
#define FUSES_DSUSTANDBY_ME_OPAMP_ADDR        0x00806008
#define FUSES_DSUSTANDBY_ME_OPAMP_MASK        0x40000000

#define FUSES_DSUSTANDBY_ME_AES_OFFSET        31  //!< [31] AES is implemented
#define SYSTEM_DCFG_ME_AES_POS                31  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_AES_SIZE          1
#define FUSES_DSUSTANDBY_ME_AES_ADDR          0x00806008
#define FUSES_DSUSTANDBY_ME_AES_MASK          0x80000000

#define FUSES_DSUSTANDBY_ME_TRNG_OFFSET       0   //!< [32] TRNG is implemented
#define SYSTEM_DCFG_ME_TRNG_POS               32  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_TRNG_SIZE         1
#define FUSES_DSUSTANDBY_ME_TRNG_ADDR         0x0080600C
#define FUSES_DSUSTANDBY_ME_TRNG_MASK         0x00000001

#define FUSES_DSUSTANDBY_ME_PPP_N_OFFSET      1   //!< [33] PPP is implemented
#define SYSTEM_DCFG_ME_PPP_N_POS              33  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_PPP_N_SIZE        1
#define FUSES_DSUSTANDBY_ME_PPP_N_ADDR        0x0080600C
#define FUSES_DSUSTANDBY_ME_PPP_N_MASK        0x00000002

#define FUSES_DSUSTANDBY_ME_RFCTRL_OFFSET     3   //!< [35] RFCTRL is implemente
#define SYSTEM_DCFG_ME_RFCTRL_POS             35  //!< Duplicated param needed by apb_mapper generator to handle module_enable
#define FUSES_DSUSTANDBY_ME_RFCTRL_SIZE       1
#define FUSES_DSUSTANDBY_ME_RFCTRL_ADDR       0x0080600C
#define FUSES_DSUSTANDBY_ME_RFCTRL_MASK       0x00000008

#define FUSES_DSUSTANDBY_LPRAM_SIZE_OFFSET    4   //!< [37:36] None
#define FUSES_DSUSTANDBY_LPRAM_SIZE_SIZE      2
#define FUSES_DSUSTANDBY_LPRAM_SIZE_ADDR      0x0080600C
#define FUSES_DSUSTANDBY_LPRAM_SIZE_MASK      0x00000030

#define FUSES_DSUSTANDBY_PD1_RAM_BIAS_OFFSET  6   //!< [40:38] PD1 RAM Bias
#define FUSES_DSUSTANDBY_PD1_RAM_BIAS_SIZE    3
#define FUSES_DSUSTANDBY_PD1_RAM_BIAS_ADDR    0x0080600C
#define FUSES_DSUSTANDBY_PD1_RAM_BIAS_MASK    0x000001C0

#define FUSES_DSUSTANDBY_PD1_RAM_RMARGIN_OFFSET 9   //!< [44:41] PD1 RAM Read Margin
#define FUSES_DSUSTANDBY_PD1_RAM_RMARGIN_SIZE 4
#define FUSES_DSUSTANDBY_PD1_RAM_RMARGIN_ADDR 0x0080600C
#define FUSES_DSUSTANDBY_PD1_RAM_RMARGIN_MASK 0x00001E00

#define FUSES_DSUSTANDBY_PD2_RAM_BIAS_OFFSET  13  //!< [47:45] PD2 RAM Bias
#define FUSES_DSUSTANDBY_PD2_RAM_BIAS_SIZE    3
#define FUSES_DSUSTANDBY_PD2_RAM_BIAS_ADDR    0x0080600C
#define FUSES_DSUSTANDBY_PD2_RAM_BIAS_MASK    0x0000E000

#define FUSES_DSU_PD2_RAM_RMARGIN_OFFSET      16  //!< [51:48] PD2 RAM Read Margin
#define FUSES_DSU_PD2_RAM_RMARGIN_SIZE        4
#define FUSES_DSU_PD2_RAM_RMARGIN_ADDR        0x0080600C
#define FUSES_DSU_PD2_RAM_RMARGIN_MASK        0x000F0000

//! @par  Parameters definition for OTP3 row
typedef struct {
  uint64_t OTP_VREF1                : 8; //!< VREF1 Calibration
  uint64_t OTP_VREFPL1              : 8; //!< VREF Calibration of VREG in PL1
  uint64_t OTP_VREFPL2              : 8; //!< VREF Calibration of VREG in PL2
  uint64_t OTP_VREFPL3              : 8; //!< VREF Calibration of VREG in PL3
  uint64_t                          :32;
} FUSEMAP_OTP3_type;

#define FUSES_OTP_VREF1_OFFSET                0   //!< [7:0] VREF1 Calibration
#define FUSES_OTP_VREF1_SIZE                  8
#define FUSES_OTP_VREF1_ADDR                  0x00806010
#define FUSES_OTP_VREF1_MASK                  0x000000FF

#define FUSES_OTP_VREFPL1_OFFSET              8   //!< [15:8] VREF Calibration of VREG in PL1
#define FUSES_OTP_VREFPL1_SIZE                8
#define FUSES_OTP_VREFPL1_ADDR                0x00806010
#define FUSES_OTP_VREFPL1_MASK                0x0000FF00

#define FUSES_OTP_VREFPL2_OFFSET              16  //!< [23:16] VREF Calibration of VREG in PL2
#define FUSES_OTP_VREFPL2_SIZE                8
#define FUSES_OTP_VREFPL2_ADDR                0x00806010
#define FUSES_OTP_VREFPL2_MASK                0x00FF0000

#define FUSES_OTP_VREFPL3_OFFSET              24  //!< [31:24] VREF Calibration of VREG in PL3
#define FUSES_OTP_VREFPL3_SIZE                8
#define FUSES_OTP_VREFPL3_ADDR                0x00806010
#define FUSES_OTP_VREFPL3_MASK                0xFF000000

//! @par  Parameters definition for OTP4 row
typedef struct {
  uint64_t OSC16M_CAL8M_TCAL        : 6; //!< OSC16M Temp Calibration (8MHz)
  uint64_t OSC16M_CAL8M_FCAL        : 6; //!< OSC16M Freq Calibration (8MHz)
  uint64_t OSC16M_CAL12M_TCAL       : 6; //!< OSC16M Temp Calibration (12MHz)
  uint64_t OSC16M_CAL12M_FCAL       : 6; //!< OSC16M Freq Calibration (12MHz)
  uint64_t OSC16M_CAL16M_TCAL       : 6; //!< OSC16M Temp Calibration (16MHz)
  uint64_t OSC16M_CAL16M_FCAL       : 6; //!< OSC16M Freq Calibration (16MHz)
  uint64_t DFLL48M_TCAL             : 3; //!< DFLL48M Temperature Calibration
  uint64_t                          :25;
} FUSEMAP_OTP4_type;

#define FUSES_OSC16M_CAL8M_TCAL_OFFSET        0   //!< [5:0] OSC16M Temp Calibration (8MHz)
#define FUSES_OSC16M_CAL8M_TCAL_SIZE          6
#define FUSES_OSC16M_CAL8M_TCAL_ADDR          0x00806018
#define FUSES_OSC16M_CAL8M_TCAL_MASK          0x0000003F

#define FUSES_OSC16M_CAL8M_FCAL_OFFSET        6   //!< [11:6] OSC16M Freq Calibration (8MHz)
#define FUSES_OSC16M_CAL8M_FCAL_SIZE          6
#define FUSES_OSC16M_CAL8M_FCAL_ADDR          0x00806018
#define FUSES_OSC16M_CAL8M_FCAL_MASK          0x00000FC0

#define FUSES_OSC16M_CAL12M_TCAL_OFFSET       12  //!< [17:12] OSC16M Temp Calibration (12MHz)
#define FUSES_OSC16M_CAL12M_TCAL_SIZE         6
#define FUSES_OSC16M_CAL12M_TCAL_ADDR         0x00806018
#define FUSES_OSC16M_CAL12M_TCAL_MASK         0x0003F000

#define FUSES_OSC16M_CAL12M_FCAL_OFFSET       18  //!< [23:18] OSC16M Freq Calibration (12MHz)
#define FUSES_OSC16M_CAL12M_FCAL_SIZE         6
#define FUSES_OSC16M_CAL12M_FCAL_ADDR         0x00806018
#define FUSES_OSC16M_CAL12M_FCAL_MASK         0x00FC0000

#define FUSES_OSC16M_CAL16M_TCAL_OFFSET       24  //!< [29:24] OSC16M Temp Calibration (16MHz)
#define FUSES_OSC16M_CAL16M_TCAL_SIZE         6
#define FUSES_OSC16M_CAL16M_TCAL_ADDR         0x00806018
#define FUSES_OSC16M_CAL16M_TCAL_MASK         0x3F000000

#define FUSES_OSC16M_CAL16M_FCAL_OFFSET       30  //!< [35:30] OSC16M Freq Calibration (16MHz)
#define FUSES_OSC16M_CAL16M_FCAL_SIZE         6
#define FUSES_OSC16M_CAL16M_FCAL_ADDR         0x00806018
#define FUSES_OSC16M_CAL16M_FCAL_MASK         0x0000000FC0000000

#define FUSES_OSC16M_CAL16M_FCAL_0_OFFSET     30  //!< [31:30] OSC16M Freq Calibration (16MHz)
#define FUSES_OSC16M_CAL16M_FCAL_0_SIZE       2
#define FUSES_OSC16M_CAL16M_FCAL_0_ADDR       0x00806018
#define FUSES_OSC16M_CAL16M_FCAL_0_MASK       0xC0000000

#define FUSES_OSC16M_CAL16M_FCAL_1_OFFSET     0   //!< [35:32] OSC16M Freq Calibration (16MHz)
#define FUSES_OSC16M_CAL16M_FCAL_1_SIZE       4
#define FUSES_OSC16M_CAL16M_FCAL_1_ADDR       0x0080601C
#define FUSES_OSC16M_CAL16M_FCAL_1_MASK       0x0000000F

#define FUSES_DFLL48M_TCAL_OFFSET             4   //!< [38:36] DFLL48M Temperature Calibration
#define FUSES_DFLL48M_TCAL_SIZE               3
#define FUSES_DFLL48M_TCAL_ADDR               0x0080601C
#define FUSES_DFLL48M_TCAL_MASK               0x00000070

//! @par  Parameters definition for OTP5 row
typedef struct {
  uint64_t ADC_BIASREFBUF           : 3; //!< ADC Bias Reference Buffer Scaling
  uint64_t ADC_BIASCOMP             : 3; //!< ADC Comparator Scaling
  uint64_t OSC32K                   : 7; //!< OSC32K Calibration
  uint64_t USB_TRANSN               : 5; //!< USB pad Transn calibration
  uint64_t USB_TRANSP               : 5; //!< USB pad Transp calibration
  uint64_t USB_TRIM                 : 3; //!< USB pad Trim calibration
  uint64_t                          :38;
} FUSEMAP_OTP5_type;

#define FUSES_ADC_BIASREFBUF_OFFSET           0   //!< [2:0] ADC Bias Reference Buffer Scaling
#define FUSES_ADC_BIASREFBUF_SIZE             3
#define FUSES_ADC_BIASREFBUF_ADDR             0x00806020
#define FUSES_ADC_BIASREFBUF_MASK             0x00000007

#define FUSES_ADC_BIASCOMP_OFFSET             3   //!< [5:3] ADC Comparator Scaling
#define FUSES_ADC_BIASCOMP_SIZE               3
#define FUSES_ADC_BIASCOMP_ADDR               0x00806020
#define FUSES_ADC_BIASCOMP_MASK               0x00000038

#define FUSES_OSC32K_OFFSET                   6   //!< [12:6] OSC32K Calibration
#define FUSES_OSC32K_SIZE                     7
#define FUSES_OSC32K_ADDR                     0x00806020
#define FUSES_OSC32K_MASK                     0x00001FC0

#define FUSES_USB_TRANSN_OFFSET               13  //!< [17:13] USB pad Transn calibration
#define FUSES_USB_TRANSN_SIZE                 5
#define FUSES_USB_TRANSN_ADDR                 0x00806020
#define FUSES_USB_TRANSN_MASK                 0x0003E000

#define FUSES_USB_TRANSP_OFFSET               18  //!< [22:18] USB pad Transp calibration
#define FUSES_USB_TRANSP_SIZE                 5
#define FUSES_USB_TRANSP_ADDR                 0x00806020
#define FUSES_USB_TRANSP_MASK                 0x007C0000

#define FUSES_USB_TRIM_OFFSET                 23  //!< [25:23] USB pad Trim calibration
#define FUSES_USB_TRIM_SIZE                   3
#define FUSES_USB_TRIM_ADDR                   0x00806020
#define FUSES_USB_TRIM_MASK                   0x03800000

//! @par  Parameters definition for TEMP_LOG row
typedef struct {
  uint64_t ROOM_TEMP_VAL_INT        : 8; //!< Integer part of room temperature in oC
  uint64_t ROOM_TEMP_VAL_DEC        : 4; //!< Decimal part of room temperature
  uint64_t HOT_TEMP_VAL_INT         : 8; //!< Integer part of hot temperature in oC
  uint64_t HOT_TEMP_VAL_DEC         : 4; //!< Decimal part of hot temperature
  uint64_t ROOM_INT1V_VAL           : 8; //!< 2's complement of the internal 1V reference drift at room temperature (versus a 1.0 centered value)
  uint64_t HOT_INT1V_VAL            : 8; //!< 2's complement of the internal 1V reference drift at hot temperature (versus a 1.0 centered value)
  uint64_t ROOM_ADC_VAL             :12; //!< 12-bit ADC conversion at room temperature
  uint64_t HOT_ADC_VAL              :12; //!< 12-bit ADC conversion at hot temperature
} FUSEMAP_TEMP_LOG_type;

#define FUSES_ROOM_TEMP_VAL_INT_OFFSET        0   //!< [7:0] Integer part of room temperature in oC
#define FUSES_ROOM_TEMP_VAL_INT_SIZE          8
#define FUSES_ROOM_TEMP_VAL_INT_ADDR          0x00806030
#define FUSES_ROOM_TEMP_VAL_INT_MASK          0x000000FF

#define FUSES_ROOM_TEMP_VAL_DEC_OFFSET        8   //!< [11:8] Decimal part of room temperature
#define FUSES_ROOM_TEMP_VAL_DEC_SIZE          4
#define FUSES_ROOM_TEMP_VAL_DEC_ADDR          0x00806030
#define FUSES_ROOM_TEMP_VAL_DEC_MASK          0x00000F00

#define FUSES_HOT_TEMP_VAL_INT_OFFSET         12  //!< [19:12] Integer part of hot temperature in oC
#define FUSES_HOT_TEMP_VAL_INT_SIZE           8
#define FUSES_HOT_TEMP_VAL_INT_ADDR           0x00806030
#define FUSES_HOT_TEMP_VAL_INT_MASK           0x000FF000

#define FUSES_HOT_TEMP_VAL_DEC_OFFSET         20  //!< [23:20] Decimal part of hot temperature
#define FUSES_HOT_TEMP_VAL_DEC_SIZE           4
#define FUSES_HOT_TEMP_VAL_DEC_ADDR           0x00806030
#define FUSES_HOT_TEMP_VAL_DEC_MASK           0x00F00000

#define FUSES_ROOM_INT1V_VAL_OFFSET           24  //!< [31:24] 2's complement of the internal 1V reference drift at room temperature (versus a 1.0 centered value)
#define FUSES_ROOM_INT1V_VAL_SIZE             8
#define FUSES_ROOM_INT1V_VAL_ADDR             0x00806030
#define FUSES_ROOM_INT1V_VAL_MASK             0xFF000000

#define FUSES_HOT_INT1V_VAL_OFFSET            0   //!< [39:32] 2's complement of the internal 1V reference drift at hot temperature (versus a 1.0 centered value)
#define FUSES_HOT_INT1V_VAL_SIZE              8
#define FUSES_HOT_INT1V_VAL_ADDR              0x00806034
#define FUSES_HOT_INT1V_VAL_MASK              0x000000FF

#define FUSES_ROOM_ADC_VAL_OFFSET             8   //!< [51:40] 12-bit ADC conversion at room temperature
#define FUSES_ROOM_ADC_VAL_SIZE               12
#define FUSES_ROOM_ADC_VAL_ADDR               0x00806034
#define FUSES_ROOM_ADC_VAL_MASK               0x000FFF00

#define FUSES_HOT_ADC_VAL_OFFSET              20  //!< [63:52] 12-bit ADC conversion at hot temperature
#define FUSES_HOT_ADC_VAL_SIZE                12
#define FUSES_HOT_ADC_VAL_ADDR                0x00806034
#define FUSES_HOT_ADC_VAL_MASK                0xFFF00000

//! @par  Parameters definition for USER row
typedef struct {
  uint64_t NVMCTRL_BOOTPROT         : 3; //!< Bootloader Size
  uint64_t                          : 1;
  uint64_t NVMCTRL_EEPROM_SIZE      : 3; //!< EEPROM Size
  uint64_t                          : 1;
  uint64_t BOD33USERLEVEL           : 6; //!< BOD33 User Level
  uint64_t BOD33_DIS                : 1; //!< BOD33 Disable
  uint64_t BOD33_ACTION             : 2; //!< BOD33 Action
  uint64_t BOD12USERLEVEL           : 6; //!< BOD12 User Level
  uint64_t BOD12_DIS                : 1; //!< BOD12 Disable
  uint64_t BOD12_ACTION             : 2; //!< BOD12 Action
  uint64_t WDT_ENABLE               : 1; //!< WDT Enable
  uint64_t WDT_ALWAYSON             : 1; //!< WDT Always On
  uint64_t WDT_PER                  : 4; //!< WDT Period
  uint64_t WDT_WINDOW               : 4; //!< WDT Window
  uint64_t WDT_EWOFFSET             : 4; //!< WDT Early Warning Offset
  uint64_t WDT_WEN                  : 1; //!< WDT Window Mode Enable
  uint64_t BOD33_HYST               : 1; //!< BOD33 Hysteresis
  uint64_t BOD12_HYST               : 1; //!< BOD12 Hysteresis
  uint64_t                          : 5;
  uint64_t NVMCTRL_REGION_LOCKS     :16; //!< NVM Region Locks
} FUSEMAP_USER_type;

#define FUSES_NVMCTRL_BOOTPROT_OFFSET         0   //!< [2:0] Bootloader Size
#define FUSES_NVMCTRL_BOOTPROT_SIZE           3
#define FUSES_NVMCTRL_BOOTPROT_ADDR           0x00804000
#define FUSES_NVMCTRL_BOOTPROT_MASK           0x00000007

#define FUSES_NVMCTRL_EEPROM_SIZE_OFFSET      4   //!< [6:4] EEPROM Size
#define FUSES_NVMCTRL_EEPROM_SIZE_SIZE        3
#define FUSES_NVMCTRL_EEPROM_SIZE_ADDR        0x00804000
#define FUSES_NVMCTRL_EEPROM_SIZE_MASK        0x00000070

#define FUSES_BOD33USERLEVEL_OFFSET           8   //!< [13:8] BOD33 User Level
#define FUSES_BOD33USERLEVEL_SIZE             6
//#define FUSES_BOD33USERLEVEL_ADDR             0x00804000
#define FUSES_BOD33USERLEVEL_MASK             0x00003F00

#define FUSES_BOD33_DIS_OFFSET                14  //!< [14] BOD33 Disable
#define FUSES_BOD33_DIS_SIZE                  1
//#define FUSES_BOD33_DIS_ADDR                  0x00804000
#define FUSES_BOD33_DIS_MASK                  0x00004000

#define FUSES_BOD33_ACTION_OFFSET             15  //!< [16:15] BOD33 Action
#define FUSES_BOD33_ACTION_SIZE               2
//#define FUSES_BOD33_ACTION_ADDR               0x00804000
#define FUSES_BOD33_ACTION_MASK               0x00018000

#define FUSES_BOD12USERLEVEL_OFFSET           17  //!< [22:17] BOD12 User Level
#define FUSES_BOD12USERLEVEL_SIZE             6
#define FUSES_BOD12USERLEVEL_ADDR             0x00804000
#define FUSES_BOD12USERLEVEL_MASK             0x007E0000

#define FUSES_BOD12_DIS_OFFSET                23  //!< [23] BOD12 Disable
#define FUSES_BOD12_DIS_SIZE                  1
#define FUSES_BOD12_DIS_ADDR                  0x00804000
#define FUSES_BOD12_DIS_MASK                  0x00800000

#define FUSES_BOD12_ACTION_OFFSET             24  //!< [25:24] BOD12 Action
#define FUSES_BOD12_ACTION_SIZE               2
#define FUSES_BOD12_ACTION_ADDR               0x00804000
#define FUSES_BOD12_ACTION_MASK               0x03000000

#define FUSES_WDT_ENABLE_OFFSET               26  //!< [26] WDT Enable
#define FUSES_WDT_ENABLE_SIZE                 1
#define FUSES_WDT_ENABLE_ADDR                 0x00804000
#define FUSES_WDT_ENABLE_MASK                 0x04000000

#define FUSES_WDT_ALWAYSON_OFFSET             27  //!< [27] WDT Always On
#define FUSES_WDT_ALWAYSON_SIZE               1
#define FUSES_WDT_ALWAYSON_ADDR               0x00804000
#define FUSES_WDT_ALWAYSON_MASK               0x08000000

#define FUSES_WDT_PER_OFFSET                  28  //!< [31:28] WDT Period
#define FUSES_WDT_PER_SIZE                    4
#define FUSES_WDT_PER_ADDR                    0x00804000
#define FUSES_WDT_PER_MASK                    0xF0000000

#define FUSES_WDT_WINDOW_OFFSET               0   //!< [35:32] WDT Window
#define FUSES_WDT_WINDOW_SIZE                 4
#define FUSES_WDT_WINDOW_ADDR                 0x00804004
#define FUSES_WDT_WINDOW_MASK                 0x0000000F

#define FUSES_WDT_EWOFFSET_OFFSET             4   //!< [39:36] WDT Early Warning Offset
#define FUSES_WDT_EWOFFSET_SIZE               4
#define FUSES_WDT_EWOFFSET_ADDR               0x00804004
#define FUSES_WDT_EWOFFSET_MASK               0x000000F0

#define FUSES_WDT_WEN_OFFSET                  8   //!< [40] WDT Window Mode Enable
#define FUSES_WDT_WEN_SIZE                    1
#define FUSES_WDT_WEN_ADDR                    0x00804004
#define FUSES_WDT_WEN_MASK                    0x00000100

#define FUSES_BOD33_HYST_OFFSET               9   //!< [41] BOD33 Hysteresis
#define FUSES_BOD33_HYST_SIZE                 1
//#define FUSES_BOD33_HYST_ADDR                 0x00804004
#define FUSES_BOD33_HYST_MASK                 0x00000200

#define FUSES_BOD12_HYST_OFFSET               10  //!< [42] BOD12 Hysteresis
#define FUSES_BOD12_HYST_SIZE                 1
//#define FUSES_BOD12_HYST_ADDR                 0x00804004
#define FUSES_BOD12_HYST_MASK                 0x00000400

#define FUSES_NVMCTRL_REGION_LOCKS_OFFSET     16  //!< [63:48] NVM Region Locks
#define FUSES_NVMCTRL_REGION_LOCKS_SIZE       16
#define FUSES_NVMCTRL_REGION_LOCKS_ADDR       0x00804004
#define FUSES_NVMCTRL_REGION_LOCKS_MASK       0xFFFF0000
// fuses_params.h END ===============================================

static void load_calibrations(void)
{
    uint32_t * cal_row   = (uint32_t *)(NVMCTRL_CAL_ADDR);
    uint32_t * user_row  = (uint32_t *)(NVMCTRL_USER_ADDR);
    //uint32_t * otp1_row  = (uint32_t *)(NVMCTRL_OTP1_ADDR);
    uint32_t * otp2_row  = (uint32_t *)(NVMCTRL_OTP2_ADDR);
    uint32_t * otp3_row  = (uint32_t *)(NVMCTRL_OTP3_ADDR);
    uint32_t * otp4_row  = (uint32_t *)(NVMCTRL_OTP4_ADDR);

    uint32_t cal0;
    uint32_t cal1;
    uint32_t user_row0;
    uint32_t user_row1;
    //uint32_t otp1_0;
    //uint32_t otp1_1;
    uint32_t otp2_0;
    uint32_t otp2_1;
    uint32_t otp3_0;
    //uint32_t otp3_1;
    uint32_t otp4_0;
    uint32_t otp4_1;


    // Load CB in registers
    // -----------------------------
    // CB row
    // -----------------------------
    // read the CB row
    cal0 = cal_row[0];
    cal1 = cal_row[1];

    // CAL0
    SUPC->VREFCAL.bit.TEMP = ((FUSES_CALIB_VREFTEMP_MASK & cal0) >> FUSES_CALIB_VREFTEMP_OFFSET);     // VREFTEMP
    SUPC->MVREGCAL.bit.PL0 = (FUSES_CALIB_VREFPL0_MASK & cal0) >> FUSES_CALIB_VREFPL0_OFFSET;     // VREFPL0
    SUPC->VREFCAL.bit.VREF0 = (FUSES_CALIB_VREF0_MASK & cal0) >> FUSES_CALIB_VREF0_OFFSET;   // VREF0
    // Here, no BOD start level as this sequence is skipped when fresh from fab
    /*   SUPC->BOD33.bit.LEVEL = (FUSES_BOD33STARTLEVEL_MASK & cal0) >> FUSES_BOD33STARTLEVEL_OFFSET;       // BOD33 LEVEL */
    /*   SUPC->BOD12.bit.LEVEL = (FUSES_BOD12STARTLEVEL_MASK & cal0) >> FUSES_BOD12STARTLEVEL_OFFSET;       // BOD12 LEVEL */

    //CAL1
    OSC32KCTRL->OSCULP32K.bit.CALIB = (FUSES_OSCULP32K_MASK & cal1) >> FUSES_OSCULP32K_OFFSET; // OSCULP32K Calib
    OSCCTRL->CAL4M.bit.TCAL = (FUSES_OSC16M_CAL4M_TCAL_MASK & cal1) >> FUSES_OSC16M_CAL4M_TCAL_OFFSET;
    OSCCTRL->CAL4M.bit.FCAL = (FUSES_OSC16M_CAL4M_FCAL_MASK & cal1) >> FUSES_OSC16M_CAL4M_FCAL_OFFSET;
    /* FIXME: Adjust FCAL (0x0 ~ 0x3F) according to specific board:
     * - 0x29 for XPRO board SN: 0200000038
     */
    OSCCTRL->CAL4M.bit.FCAL = 0x30;
    SUPC->BUVREGCAL.bit.BKUP = (FUSES_BUVREG_BKUPCALIB_MASK & cal1) >> FUSES_BUVREG_BKUPCALIB_OFFSET;
    SUPC->BUVREGCAL.bit.NOTINBKUP = (FUSES_BUVREG_NOBKUPCALIB_MASK & cal1) >> FUSES_BUVREG_NOBKUPCALIB_OFFSET;

    // -----------------------------
    // AUX-0  User row
    // -----------------------------
    user_row0 = user_row[0];
    user_row1 = user_row[1];

    // Here, test mode enable for NVMCTRL is set to change the BOOTPROT and EEPROM
    NVMCTRL->TMCTRL.reg = NVMCTRL_TMCTRL_TKEY(0xD5) | NVMCTRL_TMCTRL_TMEN;
    NVMCTRL->TUSER.bit.BOOTPROT = (FUSES_NVMCTRL_BOOTPROT_MASK & user_row0) >> FUSES_NVMCTRL_BOOTPROT_OFFSET;
    NVMCTRL->TUSER.bit.EEPROM = (FUSES_NVMCTRL_EEPROM_SIZE_MASK & user_row0) >> FUSES_NVMCTRL_EEPROM_SIZE_OFFSET;
    // Disable test mode
    NVMCTRL->TMCTRL.reg = NVMCTRL_TMCTRL_TKEY(0xD5);

    // HERE, there is no way to configure by SW the BOD33 register when fresh from fab
    // Reason: this register is writtable only if fuse update has been executed before
    /*   // BOD is disabled before configuring it */
    /*   SUPC->BOD33.bit.ENABLE = 0; */
    /*   while (!SUPC->STATUS.bit.B33SRDY); */
    /*   SUPC->BOD33.bit.LEVEL = (FUSES_BOD33USERLEVEL_MASK & user_row0) >> FUSES_BOD33USERLEVEL_OFFSET; */
    /*   SUPC_BOD33_Type BOD33; */
    /*   // disable the BOD33 */
    /*   SUPC->BOD33.bit.ENABLE = 0; */
    /*   while (!SUPC->STATUS.bit.B33SRDY); */
    /*   // configure BOD33 */
    /*   BOD33 = SUPC->BOD33; */
    /*   BOD33.bit.HYST   = (FUSES_BOD33_HYST_MASK & user_row1) >> FUSES_BOD33_HYST_OFFSET; */
    /*   BOD33.bit.ACTION = (FUSES_BOD33_ACTION_MASK & user_row0) >> FUSES_BOD33_ACTION_OFFSET; */
    /*   SUPC->BOD33 = BOD33; */
    /*   SUPC->BOD33.bit.ENABLE = !((FUSES_BOD33_DIS_MASK & user_row0) >> FUSES_BOD33_DIS_OFFSET); */
    /*   while (!SUPC->STATUS.bit.B33SRDY); */

    // BOD is disabled before configuring it
    SUPC->BOD12.bit.ENABLE = 0;
    while (!SUPC->STATUS.bit.B12SRDY);
    SUPC->BOD12.bit.LEVEL = (FUSES_BOD12USERLEVEL_MASK & user_row0) >> FUSES_BOD12USERLEVEL_OFFSET;
    SUPC_BOD12_Type BOD12;
    // disable the BOD12
    SUPC->BOD12.bit.ENABLE = 0;
    while (!SUPC->STATUS.bit.B12SRDY);
    // configure BOD12
    BOD12 = SUPC->BOD12;
    BOD12.bit.HYST   = (FUSES_BOD12_HYST_MASK & user_row1) >> FUSES_BOD12_HYST_OFFSET;
    BOD12.bit.ACTION = (FUSES_BOD12_ACTION_MASK & user_row0) >> FUSES_BOD12_ACTION_OFFSET;
    SUPC->BOD12 = BOD12;
    SUPC->BOD12.bit.ENABLE = !((FUSES_BOD12_DIS_MASK & user_row0) >> FUSES_BOD12_DIS_OFFSET);
    while (!SUPC->STATUS.bit.B12SRDY);


    WDT->CTRLA.bit.ALWAYSON = (FUSES_WDT_ALWAYSON_MASK & user_row0) >> FUSES_WDT_ALWAYSON_OFFSET;
    while (WDT->SYNCBUSY.bit.ALWAYSON == 1);

    WDT->CTRLA.bit.ENABLE = (FUSES_WDT_ENABLE_MASK & user_row0) >> FUSES_WDT_ENABLE_OFFSET;
    while (WDT->SYNCBUSY.bit.ENABLE == 1);


    WDT_CONFIG_Type wdtconfig;
    WDT_EWCTRL_Type wdtewctrl;
    wdtconfig.bit.PER = (FUSES_WDT_PER_MASK & user_row0) >> FUSES_WDT_PER_OFFSET;
    wdtconfig.bit.WINDOW = (FUSES_WDT_WINDOW_MASK & user_row1) >> FUSES_WDT_WINDOW_OFFSET;
    wdtewctrl.bit.EWOFFSET = (FUSES_WDT_EWOFFSET_MASK & user_row1) >> FUSES_WDT_EWOFFSET_OFFSET;
    if ( WDT->CTRLA.bit.ALWAYSON == 1 || WDT->CTRLA.bit.ENABLE == 0 ) {
    WDT->CTRLA.bit.WEN = (FUSES_WDT_WEN_MASK & user_row1) >> FUSES_WDT_WEN_OFFSET;
    while (WDT->SYNCBUSY.bit.WEN == 1);
    }
    if ( WDT->CTRLA.bit.ALWAYSON == 0 && WDT->CTRLA.bit.ENABLE == 0 ) {
    WDT->CONFIG = wdtconfig;
    WDT->EWCTRL = wdtewctrl;
    }

    /*   tmp = NVMCTRL->LOCK.bit.LOCK; */
    /*   NVMCTRL->LOCK.bit.LOCK = (FUSES_NVMCTRL_REGION_LOCKS_MASK & user_row1) >> FUSES_NVMCTRL_REGION_LOCKS_OFFSET; */
    /*   if (NVMCTRL->LOCK.bit.LOCK != (FUSES_NVMCTRL_REGION_LOCKS_MASK & user_row1) >> FUSES_NVMCTRL_REGION_LOCKS_OFFSET) { */
    /*     printk("debug36\n"); */
    /*   } */
    /*   if (NVMCTRL->LOCK.bit.LOCK == tmp) { */
    /*     printk("debug37\n"); */
    /*   } */


    // -----------------------------
    // AUX-1  OTP1
    // -----------------------------
    // Here, a MUX should be implemented to emulate the fuse udpate
    // given the fact that if the flash is erased (all one), "all-to-one" value is not downloaded into the flash
    // instead, the reset value (of the shadow register) is set

    //otp1_0 = otp1_row[0];
    //otp1_1 = otp1_row[1];


    /*   // Here, test mode enable for NVMCTRL is set to change the BOOTPROT and EEPROM */
    /*   NVMCTRL->TMCTRL.reg = NVMCTRL_TMCTRL_TKEY(0xD5) | NVMCTRL_TMCTRL_TMEN; */
    /*   tmp = NVMCTRL->PARAM.bit.PSZ;*/
    //   Security word: already with the good value
    /*   NVMCTRL->LOCK.bit.LOCK = (FUSES_NVMCTRL_NVM_LOCK_MASK & user_row1) >> FUSES_NVMCTRL_NVM_LOCK_OFFSET; */
    /*   NVMCTRL->PARAM.bit.PSZ = (FUSES_NVMCTRL_PSZ_MASK & otp1_0) >> FUSES_NVMCTRL_PSZ_OFFSET; // !!! 2 bits instead of 3 ?// */
    /*   if (NVMCTRL->PARAM.bit.PSZ != (FUSES_NVMCTRL_PSZ_MASK & otp1_0) >> FUSES_NVMCTRL_PSZ_OFFSET) { */
    /*     printk("debug42\n"); */
    /*   } */
    /*   if (NVMCTRL->PARAM.bit.PSZ == tmp) { */
    /*     printk("debug43\n"); */
    /*   } */

    /*   tmp = NVMCTRL->PARAM.bit.NVMP; */
    /*   NVMCTRL->PARAM.bit.NVMP = (FUSES_NVMCTRL_NVMP_MASK & otp1_0) >> FUSES_NVMCTRL_NVMP_OFFSET; */
    /*   if (NVMCTRL->PARAM.bit.NVMP != (FUSES_NVMCTRL_NVMP_MASK & otp1_0) >> FUSES_NVMCTRL_NVMP_OFFSET) { */
    /*     printk("debug44\n"); */
    /*   } */
    /*   if (NVMCTRL->PARAM.bit.NVMP == tmp) { */
    /*     printk("debug45\n"); */
    /*   } */

    /*   tmp = NVMCTRL->PARAM.bit.WWREEP; */
    /*   NVMCTRL->PARAM.bit.WWREEP = (FUSES_NVMCTRL_WWREEP_MASK & otp1_1) >> FUSES_NVMCTRL_WWREEP_OFFSET; */
    /*   if (NVMCTRL->PARAM.bit.WWREEP != (FUSES_NVMCTRL_WWREEP_MASK & otp1_1) >> FUSES_NVMCTRL_WWREEP_OFFSET) { */
    /*     printk("debug46\n"); */
    /*   } */
    /*   if (NVMCTRL->PARAM.bit.WWREEP == tmp) { */
    /*     printk("debug47\n"); */
    /*   } */
    /*   // Disable test mode */
    /*   NVMCTRL->TMCTRL.reg = NVMCTRL_TMCTRL_TKEY(0xD5); */

    // -----------------------------
    // AUX-1  OTP2 : Device Configuration
    // -----------------------------
    otp2_0 = otp2_row[0];
    otp2_1 = otp2_row[1];

    DSU->DCFG[0].reg = otp2_0;
    DSU->DCFG[0].reg = otp2_0;
    DSUSTANDBY->DCFG[0].reg = otp2_0;
    DSUSTANDBY->DCFG[1].reg = otp2_1;

    // -----------------------------
    // AUX-1  OTP3 :
    // -----------------------------
    otp3_0 = otp3_row[0];
    //otp3_1 = otp3_row[1];
    SUPC->VREFCAL.bit.VREF1 = (FUSES_OTP_VREF1_MASK & otp3_0) >> FUSES_OTP_VREF1_OFFSET;
    SUPC->MVREGCAL.bit.PL1 = (FUSES_OTP_VREFPL1_MASK & otp3_0) >> FUSES_OTP_VREFPL1_OFFSET;
    SUPC->MVREGCAL.bit.PL2 = (FUSES_OTP_VREFPL2_MASK & otp3_0) >> FUSES_OTP_VREFPL2_OFFSET;
    SUPC->MVREGCAL.bit.PL3 = (FUSES_OTP_VREFPL3_MASK & otp3_0) >> FUSES_OTP_VREFPL3_OFFSET;

    // -----------------------------
    // AUX-1  OTP4 :
    // -----------------------------
    otp4_0 = otp4_row[0];
    otp4_1 = otp4_row[1];
    OSCCTRL->CAL8M.bit.TCAL = (FUSES_OSC16M_CAL8M_TCAL_MASK & otp4_0) >> FUSES_OSC16M_CAL8M_TCAL_OFFSET;
    OSCCTRL->CAL8M.bit.FCAL = (FUSES_OSC16M_CAL8M_FCAL_MASK & otp4_0) >> FUSES_OSC16M_CAL8M_FCAL_OFFSET;
    OSCCTRL->CAL12M.bit.TCAL = (FUSES_OSC16M_CAL12M_TCAL_MASK & otp4_0) >> FUSES_OSC16M_CAL12M_TCAL_OFFSET;
    OSCCTRL->CAL12M.bit.FCAL = (FUSES_OSC16M_CAL12M_FCAL_MASK & otp4_0) >> FUSES_OSC16M_CAL12M_FCAL_OFFSET;
    OSCCTRL->CAL16M.bit.TCAL = (FUSES_OSC16M_CAL16M_TCAL_MASK & otp4_0) >> FUSES_OSC16M_CAL16M_TCAL_OFFSET;
    OSCCTRL->CAL16M.bit.FCAL = ((FUSES_OSC16M_CAL16M_FCAL_0_MASK & otp4_0) >> FUSES_OSC16M_CAL16M_FCAL_0_OFFSET) |
    (((FUSES_OSC16M_CAL16M_FCAL_1_MASK & otp4_1) >> FUSES_OSC16M_CAL16M_FCAL_1_OFFSET) << FUSES_OSC16M_CAL16M_FCAL_0_SIZE);

    OSCCTRL->DFLLTEST.bit.TCAL = (FUSES_DFLL48M_TCAL_MASK & otp4_0) >> FUSES_DFLL48M_TCAL_OFFSET;
}

#define DEFAULT_DCFG0_STD           0xFFFF5900
#define DEFAULT_DCFG1_STD           0x000F5EF1
#define MAGIC_CALIBRATIONS_WRITTEN  0x0000A5A5

//Default workaround for fresh part without calibrations :
// Consist in writing shadow register or testmode register with calibration
// values found in the calibration pages of the nvm.
static void software_fuse_update(void)
{
    uint32_t u32CalPresentWord;
    //if ((REG_DSU_DID & 0xFFFFFF00) != 0x10870000) {           return;
        //}
        // DID matches SAML21 DieA(0) RevA(0)
        MCLK->APBEMASK.reg = 5; //Disable PPP, Enable PAC & HMATRIXLP

    //Set protection off for DSUSTANDBY & DSU
        PAC->WRCTRL.reg = PAC_WRCTRL_KEY(PAC_WRCTRL_KEY_CLR_Val)|PAC_WRCTRL_PERID(ID_DSUSTANDBY);
        while((PAC->STATUSE.reg&PAC_STATUSA_DSUSTANDBY) > 0);
        PAC->WRCTRL.reg = PAC_WRCTRL_KEY(PAC_WRCTRL_KEY_CLR_Val)|PAC_WRCTRL_PERID(ID_DSU);
        while((PAC->STATUSB.reg&PAC_STATUSB_DSU) > 0);

    //Enable testmode to access shadow registers,
    // this is only possible when the debugger is connected
    DSUSTANDBY->TESTMODE.reg |= 0x1;
    if(!((DSU->TESTMODE.reg & 0x1) || (DSUSTANDBY->TESTMODE.reg & 0x1)))
    {
        while(1)// Error dsu_test_enable not set
        {
        }
    }

    //Load default configuration for the device, will be changed later
    DSU->DCFG[0].reg = DEFAULT_DCFG0_STD;
    DSU->DCFG[1].reg = DEFAULT_DCFG1_STD;
    DSUSTANDBY->DCFG[0].reg = DEFAULT_DCFG0_STD;
    DSUSTANDBY->DCFG[1].reg = DEFAULT_DCFG1_STD;

    //Disable WDT
    WDT->CTRLA.bit.ALWAYSON = 0 >> FUSES_WDT_ALWAYSON_OFFSET;
    while (WDT->SYNCBUSY.bit.ALWAYSON == 1);
    WDT->CTRLA.bit.ENABLE = 0 >> FUSES_WDT_ENABLE_OFFSET;
    while (WDT->SYNCBUSY.bit.ENABLE == 1);

    //Check if the part has received the factory calibration, if yes, apply them
    u32CalPresentWord=*((volatile uint32_t*)NVMCTRL_AUX3_ADDRESS);
    if(u32CalPresentWord==MAGIC_CALIBRATIONS_WRITTEN)
    {
        load_calibrations();
    }

    //Disable testmode
    DSUSTANDBY->TESTMODE.reg = 0;
}

 #endif // end of SAML21_REV_A


=======
>>>>>>> 5e0d620476d40c7102da64ac29893d332cd2c691
typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M0+ core handlers */
#pragma weak NMI_Handler              = Dummy_Handler
#pragma weak HardFault_Handler        = Dummy_Handler
#pragma weak SVC_Handler              = Dummy_Handler
#pragma weak PendSV_Handler           = Dummy_Handler
#pragma weak SysTick_Handler          = Dummy_Handler

/* Peripherals handlers */
#pragma weak SYSTEM_Handler           = Dummy_Handler /* MCLK, OSCCTRL, OSC32KCTRL, PAC, PM, SUPC, TAL */
#pragma weak WDT_Handler              = Dummy_Handler
#pragma weak RTC_Handler              = Dummy_Handler
#pragma weak EIC_Handler              = Dummy_Handler
#pragma weak NVMCTRL_Handler          = Dummy_Handler
#pragma weak DMAC_Handler             = Dummy_Handler
#ifdef       ID_USB
#pragma weak USB_Handler              = Dummy_Handler
#endif
#pragma weak EVSYS_Handler            = Dummy_Handler
#pragma weak SERCOM0_Handler          = Dummy_Handler
#pragma weak SERCOM1_Handler          = Dummy_Handler
#pragma weak SERCOM2_Handler          = Dummy_Handler
#pragma weak SERCOM3_Handler          = Dummy_Handler
#ifdef       ID_SERCOM4
#pragma weak SERCOM4_Handler          = Dummy_Handler
#endif
#ifdef       ID_SERCOM5
#pragma weak SERCOM5_Handler          = Dummy_Handler
#endif
#pragma weak TCC0_Handler             = Dummy_Handler
#pragma weak TCC1_Handler             = Dummy_Handler
#pragma weak TCC2_Handler             = Dummy_Handler
#pragma weak TC0_Handler              = Dummy_Handler
#pragma weak TC1_Handler              = Dummy_Handler
#ifdef       ID_TC2
#pragma weak TC2_Handler              = Dummy_Handler
#endif
#ifdef       ID_TC3
#pragma weak TC3_Handler              = Dummy_Handler
#endif
#pragma weak TC4_Handler              = Dummy_Handler
#ifdef       ID_ADC
#pragma weak ADC_Handler              = Dummy_Handler
#endif
#ifdef       ID_AC
#pragma weak AC_Handler               = Dummy_Handler
#endif
#ifdef       ID_DAC
#pragma weak DAC_Handler              = Dummy_Handler
#endif
#ifdef       ID_PTC
#pragma weak PTC_Handler              = Dummy_Handler
#endif
#ifdef       ID_AES
#pragma weak AES_Handler              = Dummy_Handler
#endif
#ifdef       ID_TRNG
#pragma weak TRNG_Handler             = Dummy_Handler
#endif
#ifdef       ID_PICOP
#pragma weak PICOP_Handler            = Dummy_Handler
#endif

/* Exception Table */
#pragma language = extended
#pragma segment  = "CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section  = ".intvec"
#pragma location = ".intvec"
const DeviceVectors __vector_table[] = {
        __sfe("CSTACK"),
        (void*) Reset_Handler,
        (void*) NMI_Handler,
        (void*) HardFault_Handler,
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) SVC_Handler,
        (void*) (0UL), /* Reserved */
        (void*) (0UL), /* Reserved */
        (void*) PendSV_Handler,
        (void*) SysTick_Handler,

        /* Configurable interrupts */
        (void*) SYSTEM_Handler,         /*  0 Main Clock, Oscillators Control, 32k Oscillators Control, Peripheral Access Controller, Power Manager, Supply Controller, Trigger Allocator */
        (void*) WDT_Handler,            /*  1 Watchdog Timer */
        (void*) RTC_Handler,            /*  2 Real-Time Counter */
        (void*) EIC_Handler,            /*  3 External Interrupt Controller */
        (void*) NVMCTRL_Handler,        /*  4 Non-Volatile Memory Controller */
        (void*) DMAC_Handler,           /*  5 Direct Memory Access Controller */
#ifdef ID_USB
        (void*) USB_Handler,            /*  6 Universal Serial Bus */
#else
        (void*) (0UL), /* Reserved*/
#endif
        (void*) EVSYS_Handler,          /*  7 Event System Interface */
        (void*) SERCOM0_Handler,        /*  8 Serial Communication Interface 0 */
        (void*) SERCOM1_Handler,        /*  9 Serial Communication Interface 1 */
        (void*) SERCOM2_Handler,        /* 10 Serial Communication Interface 2 */
        (void*) SERCOM3_Handler,        /* 11 Serial Communication Interface 3 */
#ifdef ID_SERCOM4
        (void*) SERCOM4_Handler,        /* 12 Serial Communication Interface 4 */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_SERCOM5
        (void*) SERCOM5_Handler,        /* 13 Serial Communication Interface 5 */
#else
        (void*) (0UL), /* Reserved*/
#endif
        (void*) TCC0_Handler,           /* 14 Timer Counter Control 0 */
        (void*) TCC1_Handler,           /* 15 Timer Counter Control 1 */
        (void*) TCC2_Handler,           /* 16 Timer Counter Control 2 */
        (void*) TC0_Handler,            /* 17 Basic Timer Counter 0 */
        (void*) TC1_Handler,            /* 18 Basic Timer Counter 1 */
#ifdef ID_TC2
        (void*) TC2_Handler,            /* 19 Basic Timer Counter 2 */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_TC3
        (void*) TC3_Handler,            /* 20 Basic Timer Counter 3 */
#else
        (void*) (0UL), /* Reserved*/
#endif
        (void*) TC4_Handler,            /* 21 Basic Timer Counter 4 */
#ifdef ID_ADC
        (void*) ADC_Handler,            /* 22 Analog Digital Converter */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_AC
        (void*) AC_Handler,             /* 23 Analog Comparators */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_DAC
        (void*) DAC_Handler,            /* 24 Digital-to-Analog Converter */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_PTC
        (void*) PTC_Handler,            /* 25 Peripheral Touch Controller */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_AES
        (void*) AES_Handler,            /* 26 Advanced Encryption Standard */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_TRNG
        (void*) TRNG_Handler,           /* 27 True Random Generator */
#else
        (void*) (0UL), /* Reserved*/
#endif
#ifdef ID_PICOP
        (void*) PICOP_Handler           /* 28 PicoProcessor */
#else
        (void*) (0UL)  /* Reserved*/
#endif
};

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
        uint32_t *pSrc = __section_begin(".intvec");

        SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        return 1; /* if return 0, the data sections will not be initialized */
}

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
void Reset_Handler(void)
{
#if 1 // SAML21_REV_A
        software_fuse_update();
#endif
        __iar_program_start();
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}

void set_perf_level(uint32_t Perflevel)
{
	//Switch to PLx for this test

		//Enable testmode to access shadow registers
		DSUSTANDBY->TESTMODE.reg |= 0x1;
		if(!((DSU->TESTMODE.reg & 0x1) || (DSUSTANDBY->TESTMODE.reg & 0x1)))
		{
		  while(1);// Error dsu_test_enable not set
		}
		
	
		if(Perflevel==0)
			SUPC->VREFCAL.bit.VREFVREG = 0x37;	 // Value : 0.96V
	
		if(Perflevel==1)
			SUPC->VREFCAL.bit.VREFVREG = 0x4E;	 // Value : 1.05V
	
		if(Perflevel==2)
			SUPC->VREFCAL.bit.VREFVREG = 0x6B;	 // Value : 1.20V
		  
		SUPC->VREGTEST.reg = SUPC_VREGTEST_LDOEN |	SUPC_VREGTEST_SCEN;
		SUPC->VREGTEST.bit.FVREFVREG = 1;
		SUPC->VREGTEST.bit.TESTEN = 1;	  
		
		DSUSTANDBY->TESTMODE.reg = 0;	 

}

