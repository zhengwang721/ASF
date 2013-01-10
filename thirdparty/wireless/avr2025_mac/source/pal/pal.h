/**
 * @file pal.h
 *
 * @brief PAL related APIs
 *
 *  Copyright (c) 2009 Atmel Corporation. All rights reserved.
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
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_H
#define PAL_H

/*
 * NOTE:- Include 'return_val.h' before this file, as return_val.h has the
 *        all return value enums.
 */

/* === Includes ============================================================ */

#include "compiler.h"
#include "pal_generic.h"
#include "interrupt.h"
#include "common_sw_timer.h"
#include "delay.h"
#include "ioport.h"
#include "return_val.h"
/* === Macros =============================================================== */

    /**
     * @brief Generates blocking delay
     *
     * This functions generates a blocking delay of specified time.
     *
     * @param delay in microseconds
     * @ingroup apiPalApi
     */
#define pal_timer_delay(delay)    delay_us(delay)

/*
 * Write access command of the transceiver
 */
#define WRITE_ACCESS_COMMAND            (0xC0)

/*
 * Read access command to the tranceiver
 */
#define READ_ACCESS_COMMAND             (0x80)

/*
 * Frame write command of transceiver
 */
#define TRX_CMD_FW                      (0x60)

/*
 * Frame read command of transceiver
 */
#define TRX_CMD_FR                      (0x20)

/*
 * SRAM write command of transceiver
 */
#define TRX_CMD_SW                      (0x40)

/*
 * SRAM read command of transceiver
 */
#define TRX_CMD_SR                      (0x00)

/*
 * Set TRX GPIO pins.
 */
#define RST_HIGH()                      ioport_set_pin_level(AT86RFX_RST_PIN, HIGH)
#define RST_LOW()                       ioport_set_pin_level(AT86RFX_RST_PIN, LOW)
#define SLP_TR_HIGH()                   ioport_set_pin_level(AT86RFX_SLP_PIN, HIGH)
#define SLP_TR_LOW()                    ioport_set_pin_level(AT86RFX_SLP_PIN, LOW)
# define IRQ_PINGET()                    ioport_get_value(AT86RFX_IRQ_PIN)
//#define TRX_PORT1                       (PORTC)
///*
// * RESET pin of transceiver
// */
//#define TRX_RST                         (0)
//
///*
// * Sleep Transceiver pin
// */
//#define SLP_TR                          (3)
//
//
//#define RST_HIGH()                      (TRX_PORT1.OUTSET = _BV(TRX_RST))
//#define RST_LOW()                       (TRX_PORT1.OUTCLR = _BV(TRX_RST))
//#define SLP_TR_HIGH()                   (TRX_PORT1.OUTSET = _BV(SLP_TR))
//#define SLP_TR_LOW()                    (TRX_PORT1.OUTCLR = _BV(SLP_TR))


//#if(ANTENNA_DIVERSITY == 1)
//#define TIMER_SRC_DURING_TRX_SLEEP() {TCC0_CTRLA = TC0_CLKSEL3_bm;}
//#else
//#define TIMER_SRC_DURING_TRX_SLEEP() {TCC0_CTRLA = TC0_CLKSEL3_bm; TCC1_CTRLA = TC1_CLKSEL3_bm;}
//#endif

/* Wait for 1 us. */
#define PAL_WAIT_1_US()               delay_us(1);

/* Wait for 65 ns. */
#define  PAL_WAIT_65_NS()  {nop(); nop();}

/**
 * This macro is used for handling endianness among the different CPUs.
 */
#if (UC3)
#define U16_TO_TARGET(x) ((((x) << 8) & 0xFF00) | (((x) >> 8) & 0x00FF))
#else
#define U16_TO_TARGET(x) (x)
#endif

/* Enables the global interrupt */
#define ENABLE_GLOBAL_IRQ()                  Enable_global_interrupt()

/* Disables the global interrupt */
#define DISABLE_GLOBAL_IRQ()                 Disable_global_interrupt()

/* This macro saves the global interrupt status */
#define ENTER_CRITICAL_REGION()              {uint8_t flags = cpu_irq_save();

/* This macro restores the global interrupt status */
#define LEAVE_CRITICAL_REGION()              cpu_irq_restore(flags);}

#define STACK_FLASH_SIZE (1024)


/* === Types =============================================================== */

/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)


/**
 * Timer clock source type
 */
typedef enum source_type_tag
{
    /** Identifier for timer clock source during sleep */
    TMR_CLK_SRC_DURING_TRX_SLEEP,
    /** Identifier for timer clock source while being awake */
    TMR_CLK_SRC_DURING_TRX_AWAKE
} SHORTENUM source_type_t;


/**
 * Timer clock source while radio is awake.
 *
 * T0 & T1 of PORTC
 * clk source is event channel 2 triggered by transceiver clock (CLKM, 1MHz)
 */
#define TIMER_SRC_DURING_TRX_AWAKE()

/**
 * Timer clock source while radio is sleeping.
 *
 * T0 & T1 of PORTC
 * clk source is event channel 0 triggered by system clock with corresponding
 * event system prescaler (see function event_system_init()).
 */
#define TIMER_SRC_DURING_TRX_SLEEP()

/**
 * @brief IDs for persistence storage access
 */
typedef enum ps_id_tag
{
    PS_IEEE_ADDR,
    PS_XTAL_TRIM
} SHORTENUM ps_id_t;


/**
 * @brief Power modes
 */
typedef enum pwr_mode_tag
{
    SYSTEM_SLEEP
} pwr_mode_t;




/**
 * @brief Transceiver ISR handler
 */
typedef void (*irq_handler_t)(void);


#ifdef TEST_HARNESS
#if (DEBUG > 0)
/**
 * @brief This is the ALERT.indication message structure.
 */
typedef struct assert_tag
{
    /**< The total length of this message. */
    uint8_t size            ALIGN8BIT;
    /**< This identifies the message as ALERT_INDICATION */
    uint8_t assert_cmdcode  ALIGN8BIT;
    /**< Bytes to read to frame, data[0] gives the length */
    uint8_t data[1]         ALIGN8BIT;
} assert_t;
#endif  /* (DEBUG > 0) */
#endif  /* TEST_HARNESS */

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Initialization of PAL
     *
     * This function initializes the PAL. The RC Oscillator is calibrated.
     *
     * @return MAC_SUCCESS  if PAL initialization is successful, FAILURE otherwise
     * @ingroup apiPalApi
     */
    retval_t pal_init(void);



    /**
     * @brief Initializes the transceiver timestamp interrupt
     *
     * This function sets the microcontroller specific registers
     * responsible for handling the transceiver timestamp interrupt
     *
     * @param trx_irq_cb Callback function for the transceiver timestamp interrupt
     * @ingroup apiPalApi
     */
    void pal_trx_irq_init_tstamp(FUNC_PTR trx_irq_cb);

    /**
     * \brief Enables the transceiver main interrupt
     *
     * This macro is only available for non-single chip transceivers, since
     * in single chip transceivers there is no separation between enabling
     * transceiver interrupts at the transceiver, and setting the IRQ mask
     * at the MCU. Therefore the transceiver interrupts in single chips are
     * enabled by setting the MCU IRQ mask.
     *
     */
#define pal_trx_irq_en()                ENABLE_TRX_IRQ()

    /**
     * \brief Enables the transceiver timestamp interrupt
     *
     * This macro is only available for non-single chip transceivers, since
     * in single chip transceivers there is no separation between enabling
     * transceiver interrupts at the transceiver, and setting the IRQ mask
     * at the MCU. Therefore the transceiver interrupts in single chips are
     * enabled by setting the MCU IRQ mask.
     *
     */
#define pal_trx_irq_en_tstamp()         ENABLE_TRX_IRQ_TSTAMP()

    /**
     * \brief Disables the transceiver main interrupt
     *
     * This macro is only available for non-single chip transceivers, since
     * in single chip transceivers there is no separation between disabling
     * transceiver interrupts at the transceiver, and clearing the IRQ mask
     * at the MCU. Therefore the transceiver interrupts in single chips are
     * disabled by clearing the MCU IRQ mask.
     *
     */
#define pal_trx_irq_dis()               DISABLE_TRX_IRQ()

    /**
     * \brief Disables the transceiver timestamp interrupt
     *
     * This macro is only available for non-single chip transceivers, since
     * in single chip transceivers there is no separation between disabling
     * transceiver interrupts at the transceiver, and clearing the IRQ mask
     * at the MCU. Therefore the transceiver interrupts in single chips are
     * disabled by clearing the MCU IRQ mask.
     *
     */
#define pal_trx_irq_dis_tstamp()        DISABLE_TRX_IRQ_TSTAMP()

    /**
     * @brief Clears the transceiver main interrupt
     *
     * @ingroup apiPalApi
     */
#define pal_trx_irq_flag_clr()          CLEAR_TRX_IRQ()

    /**
     * @brief Clears the transceiver timestamp interrupt
     *
     * @ingroup apiPalApi
     */
#define pal_trx_irq_flag_clr_tstamp()   CLEAR_TRX_IRQ_TSTAMP()



    /**
     * @brief Enables the global interrupt
     * @ingroup apiPalApi
     */
    static inline void pal_global_irq_enable(void)
    {
        ENABLE_GLOBAL_IRQ();
    }


    /**
     * @brief Disables the global interrupt
     * @ingroup apiPalApi
     */
    static inline void pal_global_irq_disable(void)
    {
        DISABLE_GLOBAL_IRQ();
    }






    /*
     * Macros for TRX GPIO pins access.
     */
    /** Macro to set Reset pin to high */
#define PAL_RST_HIGH()                  RST_HIGH()
    /** Macro to set Reset pin to low */
#define PAL_RST_LOW()                   RST_LOW()
    /** Macro to set SLP_TR pin to high */
#define PAL_SLP_TR_HIGH()               SLP_TR_HIGH()
    /** Macro to set SLP_TR pin to low */
#define PAL_SLP_TR_LOW()                SLP_TR_LOW()
    /** Macro to get the transceiver's main IRQ status */
#define PAL_TRX_IRQ_HIGH()              IRQ_PINGET()

    /**
     * \brief Initializes the transceiver main interrupt
     *
     * This function sets the microcontroller specific registers
     * responsible for handling the transceiver main interrupt
     *
     * \param trx_irq_cb Callback function for the transceiver main interrupt
     * \ingroup group_pal_interrupt
     */
    void pal_trx_irq_init(FUNC_PTR trx_irq_cb);

    /**
     * @brief Provides timestamp of the last received frame
     *
     * This function provides the timestamp (in microseconds)
     * of the last received frame.
     *
     * @param[out] timestamp in microseconds
     * @ingroup apiPalApi
     */
    void pal_trx_read_timestamp(uint32_t *timestamp);

#ifdef SNIFFER
    /**
     * @brief write the timestamp value
     *
     * This function writes the timestamp in the register
     *
     * @param[In] Timestamp in microseconds
     * @ingroup apiPalApi
     */
    void pal_trx_write_timestamp(uint32_t *timestamp);
#endif  /* SNIFFER */

    /*
     * Prototypes for transceiver access.  Some PALs define these as
     * macros instead of implementing them as functions, so only declare
     * them here if they are not implemented as macros.
     */
#if defined(PAL_USE_SPI_TRX) || defined(DOXYGEN)
    /**
     * @brief Reads frame buffer of the transceiver
     *
     * This function reads the frame buffer of the transceiver.
     *
     * @param[out] data Pointer to the location to store frame
     * @param[in] length Number of bytes to be read from the frame
     * buffer.
     * @ingroup apiPalApi
     */
    void pal_trx_frame_read(uint8_t *data, uint8_t length);

    /**
     * @brief Writes data into frame buffer of the transceiver
     *
     * This function writes data into the frame buffer of the transceiver
     *
     * @param[in] data Pointer to data to be written into frame buffer
     * @param[in] length Number of bytes to be written into frame buffer
     * @ingroup apiPalApi
     */
    void pal_trx_frame_write(uint8_t *data, uint8_t length);

    /**
     * @brief Reads current value from a transceiver register
     *
     * This function reads the current value from a transceiver register.
     *
     * @param addr Specifies the address of the trx register
     * from which the data shall be read
     *
     * @return value of the register read
     * @ingroup apiPalApi
     */
    uint8_t pal_trx_reg_read(uint8_t addr);

    /**
     * @brief Writes data into a transceiver register
     *
     * This function writes a value into transceiver register.
     *
     * @param addr Address of the trx register
     * @param data Data to be written to trx register
     *
     * @ingroup apiPalApi
     */
    void pal_trx_reg_write(uint8_t addr, uint8_t data);

    /**
     * @brief Subregister read
     *
     * @param   addr  offset of the register
     * @param   mask  bit mask of the subregister
     * @param   pos   bit position of the subregister
     *
     * @return  value of the read bit(s)
     * @ingroup apiPalApi
     */
    uint8_t pal_trx_bit_read(uint8_t addr, uint8_t mask, uint8_t pos);

    /**
     * @brief Subregister write
     *
     * @param[in]   reg_addr  Offset of the register
     * @param[in]   mask  Bit mask of the subregister
     * @param[in]   pos   Bit position of the subregister
     * @param[out]  new_value  Data, which is muxed into the register
     * @ingroup apiPalApi
     */
    void pal_trx_bit_write(uint8_t reg_addr, uint8_t mask, uint8_t pos, uint8_t new_value);

    /**
     * @brief Reads data from SRAM of the transceiver
     *
     * This function reads from the SRAM of the transceiver
     *
     * @param[in] addr Start address in SRAM for read operation
     * @param[out] data Pointer to the location where data stored
     * @param[in] length Number of bytes to be read from SRAM
     * @ingroup apiPalApi
     */
    void pal_trx_sram_read(uint8_t addr, uint8_t *data, uint8_t length);

    /**
     * @brief Writes data into SRAM of the transceiver
     *
     * This function writes data into the SRAM of the transceiver
     *
     * @param addr Start address in the SRAM for the write operation
     * @param data Pointer to the data to be written into SRAM
     * @param length Number of bytes to be written into SRAM
     * @ingroup apiPalApi
     */
    void pal_trx_sram_write(uint8_t addr, uint8_t *data, uint8_t length);

    /**
     * @brief Writes and reads data into/from SRAM of the transceiver
     *
     * This function writes data into the SRAM of the transceiver and
     * simultaneously reads the bytes.
     *
     * @param addr Start address in the SRAM for the write operation
     * @param idata Pointer to the data written/read into/from SRAM
     * @param length Number of bytes written/read into/from SRAM
     * @ingroup apiPalApi
     */
    void pal_trx_aes_wrrd(uint8_t addr, uint8_t *idata, uint8_t length);

#endif  /* defined(PAL_USE_SPI_TRX) */


    /**
     * @brief Calibrates the internal RC oscillator
     *
     * @return True if calibration is successful, false otherwise.
     * @ingroup apiPalApi
     */
    bool pal_calibrate_rc_osc(void);

    /**
     * @brief Selects timer clock source
     *
     * This function selects the clock source of the timer.
     *
     * @param source
     * - @ref TMR_CLK_SRC_DURING_TRX_SLEEP if clock source during sleep is to be selected, and
     * - @ref TMR_CLK_SRC_DURING_TRX_AWAKE if clock source while being awake is selected.
     * @ingroup apiPalApi
     */
    void pal_timer_source_select(source_type_t source);
	
	/**
     * @brief Adds two time values
     *
     * @param a Time value 1
     * @param b Time value 2
     *
     * @return Addition of a and b
     * @ingroup apiPalApi
     */
    static inline uint32_t pal_add_time_us(uint32_t a, uint32_t b)
    {
        return (ADD_TIME(a, b));
    }


    /**
     * @brief Subtracts two time values
     *
     * @param a Time value 1
     * @param b Time value 2
     *
     * @return Difference between a and b
     * @ingroup apiPalApi
     */
    static inline uint32_t pal_sub_time_us(uint32_t a, uint32_t b)
    {
        return (SUB_TIME(a, b));
    }



    /**
     * @brief Sleep/Power handling
     *
     * Handles sleep/power modes for entire system
     *
     * @param sleep_mode desired sleep/power mode, see sleep.h
     *
     * @ingroup apiPalApi
     */
    void pal_sleep_mode(uint8_t sleep_mode);


#if defined(NON_BLOCKING_SPI) || defined(DOXYGEN)
    /**
     * @brief SPI done callback initialization
     *
     * @param spi_done_cb Pointer to SPI done callback function
     * @ingroup apiPalApi
     */
    void pal_spi_done_cb_init(void *spi_done_cb);
#endif

#if defined(WATCHDOG) || defined(SLEEPING_TIMER) || defined(DOXYGEN)
#if (XMEGA)

    /**
     * @brief Initialize the watchdog timer of the ATxmega1281
     */
    void wdt_init(void);

    /**
     * @brief Parallel Software timer initialization if the Sleeping timer (RTC) is
     * not enabled to reset the Watchdog timer periodically.
     */
    void wdt_parallel_timer_init(void);

    /**
     * @brief Initialization of Sleeping timer (RTC) of the Atxmega1281 when Watchdog Timer
     * is not enabled. The period for the Sleeping timer should be given by the user in this case.
     */
    void sleeping_timer_without_wdt(void);

    /**
     * @brief Initialization of Sleeping timer (RTC) of the Atxmega1281 when Watchdog Timer
     * is enabled. The period for the Sleeping timer should be given by the user in this case
     * and the period of the sleeping timer should be less than the time-out period of the
     * watchdog timer.
     * @note It is required that the period of the Sleeping Timer (RTC) is less than the Time-out period
     * of Watchdog Timer (If enabled) to avoid the unintentional reset of the device.
     */
    void sleeping_timer_with_wdt(void);

    /**
     * @brief Initialization of sleeping timer (RTC) of the Atxmega1281
     */
    void sleeping_timer_init(void);

    /**
     * @brief Resets the watchdog timer of the ATxmega1281 before it
     * reaches its time-out period
     */
    void wdt_clear(void);

    /** @brief Enable Watchdog and set prescaler.
     *
     *  This function enables the Watchdog and applies prescaler settings.
     *  The Watchdog will be reset automatically.
     *
     *  The function writes the correct signature to the Configuration
     *  Change Protection register before writing the CTRL register. Interrupts are
     *  automatically ignored during the change enable period. The function will
     *  wait for the watchdog to be synchronized to the other clock domains before
     *  proceeding
     *
     *  @param  period  Watchdog Timer timeout period
     */
    void pal_wdt_enable( WDT_PER_t period );

    /** @brief This function initializes the RTC with period, initial count,
     *         compare value and clock prescaler.
     *
     *  All the synchronized registers are written at the same time to save time.
     *  The program code need to check if the RTC is busy before calling this
     *  function. The RTC_Busy() function can be used to do this.
     *
     *  @param period         RTC period. Topvalue = Period - 1.
     *  @param count          Initial RTC count.
     *  @param compareValue   Compare value.
     *  @param prescaler      Clock prescaler setting.
     *  @note It is required that the period of the Sleeping Timer (RTC) is less than the Time-out period
     *  of Watchdog Timer (If enabled) to avoid the unintentional reset of the device.
     */
    void RTC_Initialize( uint16_t period,
                         uint16_t count,
                         uint16_t compareValue,
                         RTC_PRESCALER_t prescaler );

    /** @brief This function sets both compare and overflow interrupt levels in
     *         one go.
     *
     *  @param ovfIntLevel  The overflow interrupt level.
     *  @param compIntLevel The compare interrupt level.
     */
    void RTC_SetIntLevels( RTC_OVFINTLVL_t ovfIntLevel,
                           RTC_COMPINTLVL_t compIntLevel );
#endif /* (PAL_GENERIC_TYPE == XMEGA) */
#endif /* (defined(WATCHDOG) || defined(SLEEPING_TIMER) || defined(DOXYGEN)) */

    /**
     * @brief Generation of random seed for function srand() in case this
     *        is not supported by the transceiver (e.g. AT86RF230)
     *
     * @return uint16_t Random number to be used as seed for function srand()
     * @ingroup apiPalApi
     */
    uint16_t pal_generate_rand_seed(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

void timer_overflow_cb(void);
void timer_compare_cb(void);

#endif  /* PAL_H */
/* EOF */
