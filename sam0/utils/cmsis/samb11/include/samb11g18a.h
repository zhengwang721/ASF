/**
 * \file
 *
 * \brief Header file for SAMB11G18A
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

#ifndef _SAMB11G18A_
#define _SAMB11G18A_

/**
 * \ingroup SAMB11_definitions
 * \addtogroup SAMB11G18A_definitions SAMB11G18A definitions
 * This file defines all structures and symbols for SAMB11G18A:
 *   - registers and bitfields
 *   - peripheral base address
 *   - peripheral ID
 *   - PIO definitions
*/
/*@{*/

#ifdef __cplusplus
 extern "C" {
#endif

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#include <stdint.h>
#ifndef __cplusplus
typedef volatile const uint32_t RoReg;   /**< Read only 32-bit register (volatile const unsigned int) */
typedef volatile const uint16_t RoReg16; /**< Read only 16-bit register (volatile const unsigned int) */
typedef volatile const uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */
#else
typedef volatile       uint32_t RoReg;   /**< Read only 32-bit register (volatile const unsigned int) */
typedef volatile       uint16_t RoReg16; /**< Read only 16-bit register (volatile const unsigned int) */
typedef volatile       uint8_t  RoReg8;  /**< Read only  8-bit register (volatile const unsigned int) */
#endif
typedef volatile       uint32_t WoReg;   /**< Write only 32-bit register (volatile unsigned int) */
typedef volatile       uint16_t WoReg16; /**< Write only 16-bit register (volatile unsigned int) */
typedef volatile       uint32_t WoReg8;  /**< Write only  8-bit register (volatile unsigned int) */
typedef volatile       uint32_t RwReg;   /**< Read-Write 32-bit register (volatile unsigned int) */
typedef volatile       uint16_t RwReg16; /**< Read-Write 16-bit register (volatile unsigned int) */
typedef volatile       uint8_t  RwReg8;  /**< Read-Write  8-bit register (volatile unsigned int) */
#define CAST(type, value) ((type *)(value))
#define REG_ACCESS(type, address) (*(type*)(address)) /**< C code: Register value */
#else
#define CAST(type, value) (value)
#define REG_ACCESS(type, address) (address) /**< Assembly code: Register address */
#endif

/* ************************************************************************** */
/*   CMSIS DEFINITIONS FOR SAMB11G18A */
/* ************************************************************************** */
/** \addtogroup SAMB11G18A_cmsis CMSIS Definitions */
/*@{*/

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  CORTEX-M0 Processor Exceptions Numbers ******************************/
  NonMaskableInt_IRQn       = -14, /**< 2   Non Maskable Interrupt               */
  HardFault_IRQn            = -13, /**< 3   Cortex-M4 Hard Fault Interrupt       */
  MemoryManagement_IRQn     = -12, /**< 4   Cortex-M4 Memory Management Interrupt  */
  BusFault_IRQn             = -11, /**< 5   Cortex-M4 Bus Fault Interrupt        */
  UsageFault_IRQn           = -10, /**< 6   Cortex-M4 Usage Fault Interrupt      */
  SVCall_IRQn               = -5 , /**< 11  Cortex-M4 SV Call Interrupt          */
  DebugMonitor_IRQn         = -4 , /**< 12  Cortex-M4 Debug Monitor Interrupt    */
  PendSV_IRQn               = -2 , /**< 14  Cortex-M4 Pend SV Interrupt          */
  SysTick_IRQn              = -1 , /**< 15  Cortex-M4 System Tick Interrupt      */
/******  SAMB11 specific Interrupt Numbers ***********************************/
  UART0_RX_IRQn             = 0  , /**< 0   Not present in SAMB11 (UART)        */
  UART0_TX_IRQn             = 1  , /**< 1   Not present in SAMB11 (UART)        */
  UART1_RX_IRQn             = 2  , /**< 2   Not present in SAMB11 (UART)        */
  UART1_TX_IRQn             = 3  , /**< 3   Not present in SAMB11 (UART)        */
  SPI0_RX_IRQn              = 4  , /**< 4   Not present in SAMB11 (SPI)         */
  SPI0_TX_IRQn              = 5  , /**< 5   Not present in SAMB11 (SPI)         */
  SPI1_RX_IRQn              = 6  , /**< 6   Not present in SAMB11 (SPI)         */
  SPI1_TX_IRQn              = 7  , /**< 7   Not present in SAMB11 (SPI)         */
  I2C0_RX_IRQn              = 8  , /**< 8   Not present in SAMB11 (I2C)         */
  I2C0_TX_IRQn              = 9  , /**< 9   Not present in SAMB11 (I2C)         */
  I2C1_RX_IRQn              = 10 , /**< 10  Not present in SAMB11 (I2C)         */
  I2C1_TX_IRQn              = 11 , /**< 11  Not present in SAMB11 (I2C)         */
  WDT0_IRQn                 = 12 , /**< 12  Not present in SAMB11 (WDT)         */
  WDT1_IRQn                 = 13 , /**< 13  Not present in SAMB11 (WDT)         */
  DUALTIMER0_IRQn           = 14 , /**< 14  Not present in SAMB11 (DUALTIMER)   */
  PROV_DMA_CTRL0_IRQn       = 15 , /**< 15  Not present in SAMB11 (PROV_DMA_CTRL) */
  GPIO0_IRQn                = 23 , /**< 23  Not present in SAMB11 (GPIO)        */
  GPIO1_IRQn                = 24 , /**< 24  Not present in SAMB11 (GPIO)        */
  GPIO2_IRQn                = 25 , /**< 25  Not present in SAMB11 (GPIO)        */
  TIMER0_IRQn               = 26 , /**< 26  Not present in SAMB11 (TIMER)       */

  PERIPH_COUNT_IRQn        = 27  /**< Number of peripheral IDs */
} IRQn_Type;

typedef struct _DeviceVectors
{
  /* Stack pointer */
  void* pvStack;

  /* Cortex-M handlers */
  void* pfnReset_Handler;                        /* 0   Reset handler                        */
  void* pfnNonMaskableInt_Handler;               /* 2   Non Maskable Interrupt               */
  void* pfnHardFault_Handler;                    /* 3   Cortex-M4 Hard Fault Interrupt       */
  void* pfnMemoryManagement_Handler;             /* 4   Cortex-M4 Memory Management Interrupt  */
  void* pfnBusFault_Handler;                     /* 5   Cortex-M4 Bus Fault Interrupt        */
  void* pfnUsageFault_Handler;                   /* 6   Cortex-M4 Usage Fault Interrupt      */
  void* pfnReservedM9;
  void* pfnReservedM8;
  void* pfnReservedM7;
  void* pfnReservedM6;
  void* pfnSVCall_Handler;                       /* 11  Cortex-M4 SV Call Interrupt          */
  void* pfnDebugMonitor_Handler;                 /* 12  Cortex-M4 Debug Monitor Interrupt    */
  void* pfnReservedM3;
  void* pfnPendSV_Handler;                       /* 14  Cortex-M4 Pend SV Interrupt          */
  void* pfnSysTick_Handler;                      /* 15  Cortex-M4 System Tick Interrupt      */

  /* Peripheral handlers */
  void* pfnUART0_RX_Handler;                     /* 0   Not present in SAMB11 (UART)   */
  void* pfnUART0_TX_Handler;                     /* 1   Not present in SAMB11 (UART)   */
  void* pfnUART1_RX_Handler;                     /* 2   Not present in SAMB11 (UART)   */
  void* pfnUART1_TX_Handler;                     /* 3   Not present in SAMB11 (UART)   */
  void* pfnSPI0_RX_Handler;                      /* 4   Not present in SAMB11 (SPI)    */
  void* pfnSPI0_TX_Handler;                      /* 5   Not present in SAMB11 (SPI)    */
  void* pfnSPI1_RX_Handler;                      /* 6   Not present in SAMB11 (SPI)    */
  void* pfnSPI1_TX_Handler;                      /* 7   Not present in SAMB11 (SPI)    */
  void* pfnI2C0_RX_Handler;                      /* 8   Not present in SAMB11 (I2C)    */
  void* pfnI2C0_TX_Handler;                      /* 9   Not present in SAMB11 (I2C)    */
  void* pfnI2C1_RX_Handler;                      /* 10  Not present in SAMB11 (I2C)    */
  void* pfnI2C1_TX_Handler;                      /* 11  Not present in SAMB11 (I2C)    */
  void* pfnWDT0_Handler;                         /* 12  Not present in SAMB11 (WDT)    */
  void* pfnWDT1_Handler;                         /* 13  Not present in SAMB11 (WDT)    */
  void* pfnDUALTIMER0_Handler;                   /* 14  Not present in SAMB11 (DUALTIMER) */
  void* pfnPROV_DMA_CTRL0_Handler;               /* 15  Not present in SAMB11 (PROV_DMA_CTRL) */
  void* pfnReserved16;
  void* pfnReserved17;
  void* pfnReserved18;
  void* pfnReserved19;
  void* pfnReserved20;
  void* pfnReserved21;
  void* pfnReserved22;
  void* pfnGPIO0_Handler;                        /* 23  Not present in SAMB11 (GPIO)   */
  void* pfnGPIO1_Handler;                        /* 24  Not present in SAMB11 (GPIO)   */
  void* pfnGPIO2_Handler;                        /* 25  Not present in SAMB11 (GPIO)   */
  void* pfnTIMER0_Handler;                       /* 26  Not present in SAMB11 (TIMER)  */
  void* pfnReserved27;
} DeviceVectors;

/* CORTEX-M0 core handlers */
void Reset_Handler        ( void );
void NonMaskableInt_Handler ( void );
void HardFault_Handler    ( void );
void MemoryManagement_Handler ( void );
void BusFault_Handler     ( void );
void UsageFault_Handler   ( void );
void SVCall_Handler       ( void );
void DebugMonitor_Handler ( void );
void PendSV_Handler       ( void );
void SysTick_Handler      ( void );

/* Peripherals handlers */
void UART0_RX_Handler    ( void );
void UART0_TX_Handler    ( void );
void UART1_RX_Handler    ( void );
void UART1_TX_Handler    ( void );
void SPI0_RX_Handler     ( void );
void SPI0_TX_Handler     ( void );
void SPI1_RX_Handler     ( void );
void SPI1_TX_Handler     ( void );
void I2C0_RX_Handler     ( void );
void I2C0_TX_Handler     ( void );
void I2C1_RX_Handler     ( void );
void I2C1_TX_Handler     ( void );
void WDT0_Handler        ( void );
void WDT1_Handler        ( void );
void DUALTIMER0_Handler  ( void );
void PROV_DMA_CTRL0_Handler( void );
void GPIO0_Handler       ( void );
void GPIO1_Handler       ( void );
void GPIO2_Handler       ( void );
void TIMER0_Handler      ( void );

/*
 * \brief Configuration of the CORTEX-M0 Processor and Core Peripherals
 */

#define __CM0_REV               0
#define __NVIC_PRIO_BITS        2
#define __Vendor_SysTickConfig  0
#define __FPU_PRESENT           0
#define LITTLE_ENDIAN           1
/*
 * \brief CMSIS includes
 */

#include <core_cm0.h>
#if !defined DONT_USE_CMSIS_INIT
#include "system_samb11.h"
#endif /* DONT_USE_CMSIS_INIT */

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAMB11G18A */
/* ************************************************************************** */
/** \defgroup SAMB11G18A_api Peripheral Software API */
/*@{*/
#include "component/timer.h"
#include "component/dualtimer.h"
#include "component/prov_dma_ctrl.h"
#include "component/i2c.h"
#include "component/uart.h"
#include "component/spi.h"
#include "component/wdt.h"
#include "component/lpmcu_misc_regs.h"
#include "component/gpio.h"
#include "component/spi_flash.h"
#include "component/arm_sysctrl.h"
#include "component/arm_bpu.h"
#include "component/arm_dwt.h"
#include "component/arm_rom.h"
/*@}*/
/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAMB11G18A */
/* ************************************************************************** */
/** \addtogroup SAMB11G18A_reg Registers Access Definitions */
/*@{*/
#include "instance/timer0.h"
#include "instance/dualtimer0.h"
#include "instance/prov_dma_ctrl0.h"
#include "instance/i2c0.h"
#include "instance/i2c1.h"
#include "instance/uart0.h"
#include "instance/uart1.h"
#include "instance/spi0.h"
#include "instance/spi1.h"
#include "instance/wdt0.h"
#include "instance/wdt1.h"
#include "instance/lpmcu_misc_regs0.h"
#include "instance/gpio0.h"
#include "instance/gpio1.h"
#include "instance/gpio2.h"
#include "instance/spi_flash0.h"
#include "instance/arm_sysctrl0.h"
#include "instance/arm_bpu0.h"
#include "instance/arm_dwt0.h"
#include "instance/arm_rom0.h"
/*@}*/



/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAMB11G18A */
/* ************************************************************************** */

/** \addtogroup SAMB11G18A_base Peripheral Base Address Definitions 
 * @{*/


#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define TIMER0                 (0x40000000UL)                 /**< \brief (TIMER0    ) Base Address */
#define DUALTIMER0             (0x40001000UL)                 /**< \brief (DUALTIMER0) Base Address */
#define PROV_DMA_CTRL0         (0x40002000UL)                 /**< \brief (PROV_DMA_CTRL0) Base Address */
#define I2C0                   (0x40003000UL)                 /**< \brief (I2C0      ) Base Address */
#define I2C1                   (0x40003400UL)                 /**< \brief (I2C1      ) Base Address */
#define UART0                  (0x40004000UL)                 /**< \brief (UART0     ) Base Address */
#define UART1                  (0x40005000UL)                 /**< \brief (UART1     ) Base Address */
#define SPI0                   (0x40006000UL)                 /**< \brief (SPI0      ) Base Address */
#define SPI1                   (0x40007000UL)                 /**< \brief (SPI1      ) Base Address */
#define WDT0                   (0x40008000UL)                 /**< \brief (WDT0      ) Base Address */
#define WDT1                   (0x40009000UL)                 /**< \brief (WDT1      ) Base Address */
#define LPMCU_MISC_REGS0       (0x4000B000UL)                 /**< \brief (LPMCU_MISC_REGS0) Base Address */
#define GPIO0                  (0x40010000UL)                 /**< \brief (GPIO0     ) Base Address */
#define GPIO1                  (0x40011000UL)                 /**< \brief (GPIO1     ) Base Address */
#define GPIO2                  (0x40013000UL)                 /**< \brief (GPIO2     ) Base Address */
#define SPI_FLASH0             (0x40012000UL)                 /**< \brief (SPI_FLASH0) Base Address */
#define ARM_SYSCTRL0           (0xE000E000UL)                 /**< \brief (ARM_SYSCTRL0) Base Address */
#define ARM_BPU0               (0xE0002000UL)                 /**< \brief (ARM_BPU0  ) Base Address */
#define ARM_DWT0               (0xE0001000UL)                 /**< \brief (ARM_DWT0  ) Base Address */
#define ARM_ROM0               (0xE00FF000UL)                 /**< \brief (ARM_ROM0  ) Base Address */
#else
#define TIMER0                 ((Timer *)0x40000000UL)        /**< \brief (TIMER0    ) Base Address */
#define TIMER_INST_NUM         1
#define TIMER_INSTS            { TIMER0 }

#define DUALTIMER0             ((Dualtimer *)0x40001000UL)    /**< \brief (DUALTIMER0) Base Address */
#define DUALTIMER_INST_NUM     1
#define DUALTIMER_INSTS        { DUALTIMER0 }

#define PROV_DMA_CTRL0         ((Prov_Dma_Ctrl *)0x40002000UL) /**< \brief (PROV_DMA_CTRL0) Base Address */
#define PROV_DMA_CTRL_INST_NUM 1
#define PROV_DMA_CTRL_INSTS    { PROV_DMA_CTRL0 }

#define I2C0                   ((I2C *)0x40003000UL)          /**< \brief (I2C0      ) Base Address */
#define I2C1                   ((I2C *)0x40003400UL)          /**< \brief (I2C1      ) Base Address */
#define I2C_INST_NUM           2
#define I2C_INSTS              { I2C0, I2C1 }

#define UART0                  ((Uart *)0x40004000UL)         /**< \brief (UART0     ) Base Address */
#define UART1                  ((Uart *)0x40005000UL)         /**< \brief (UART1     ) Base Address */
#define UART_INST_NUM          2
#define UART_INSTS             { UART0, UART1 }

#define SPI0                   ((Spi *)0x40006000UL)          /**< \brief (SPI0      ) Base Address */
#define SPI1                   ((Spi *)0x40007000UL)          /**< \brief (SPI1      ) Base Address */
#define SPI_INST_NUM           2
#define SPI_INSTS              { SPI0, SPI1 }

#define WDT0                   ((Wdt *)0x40008000UL)          /**< \brief (WDT0      ) Base Address */
#define WDT1                   ((Wdt *)0x40009000UL)          /**< \brief (WDT1      ) Base Address */
#define WDT_INST_NUM           2
#define WDT_INSTS              { WDT0, WDT1 }

#define LPMCU_MISC_REGS0       ((LpmcuMiscRegs *)0x4000b000UL) /**< \brief (LPMCU_MISC_REGS0) Base Address */
#define LPMCU_MISC_REGS_INST_NUM 1
#define LPMCU_MISC_REGS_INSTS  { LPMCU_MISC_REGS0 }

#define GPIO0                  ((Gpio *)0x40010000UL)         /**< \brief (GPIO0     ) Base Address */
#define GPIO1                  ((Gpio *)0x40011000UL)         /**< \brief (GPIO1     ) Base Address */
#define GPIO2                  ((Gpio *)0x40013000UL)         /**< \brief (GPIO2     ) Base Address */
#define GPIO_INST_NUM          3
#define GPIO_INSTS             { GPIO0, GPIO1, GPIO2 }

#define SPI_FLASH0             ((Spi_Flash *)0x40012000UL)    /**< \brief (SPI_FLASH0) Base Address */
#define SPI_FLASH_INST_NUM     1
#define SPI_FLASH_INSTS        { SPI_FLASH0 }

#define ARM_SYSCTRL0           ((Arm_Sysctrl *)0xe000e000LUL) /**< \brief (ARM_SYSCTRL0) Base Address */
#define ARM_SYSCTRL_INST_NUM   1
#define ARM_SYSCTRL_INSTS      { ARM_SYSCTRL0 }

#define ARM_BPU0               ((Arm_Bpu *)0xe0002000LUL)     /**< \brief (ARM_BPU0  ) Base Address */
#define ARM_BPU_INST_NUM       1
#define ARM_BPU_INSTS          { ARM_BPU0 }

#define ARM_DWT0               ((Arm_Dwt *)0xe0001000LUL)     /**< \brief (ARM_DWT0  ) Base Address */
#define ARM_DWT_INST_NUM       1
#define ARM_DWT_INSTS          { ARM_DWT0 }

#define ARM_ROM0               ((Arm_Rom *)0xe00ff000LUL)     /**< \brief (ARM_ROM0  ) Base Address */
#define ARM_ROM_INST_NUM       1
#define ARM_ROM_INSTS          { ARM_ROM0 }

#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** @}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAMB11G18A*/
/* ************************************************************************** */

#define BOOTROM_SIZE             0x00020000UL       /*  128kB Memory segment type: rom */
#define IDRAM_SIZE               0x00020000UL       /*  128kB Memory segment type: ram */
#define BLERAM_SIZE              0x00002000UL       /*    8kB Memory segment type: ram */

#define BOOTROM_ADDR             0x00000000UL       /* Memory segment type: rom */
#define IDRAM_ADDR               0x10000000UL       /* Memory segment type: ram */
#define BLERAM_ADDR              0x10040000UL       /* Memory segment type: ram */

/* ************************************************************************** */
/**  DEVICE SIGNATURES FOR SAMB11G18A */
/* ************************************************************************** */
#define LPMCU_CHIP_ID_REV_ID     0x002000b0UL


#ifdef __cplusplus
}
#endif

#endif /* _SAMB11_ */
