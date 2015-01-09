/**
* \file
*
* \brief The initialization and configuration of COG hardware driver
*
* Copyright (c) 2012-2013 Pervasive Displays Inc. All rights reserved.
*
*  Authors: Pervasive Displays Inc.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <math.h>
#include "EPD_hardware_driver.h"

// From module: TC - MEGARF implementation
#include "EPD_hw_timer.h"
//#include "common_EPD_hw_timer.h"


static volatile uint32_t EPD_Counter;
//static uint8_t spi_flag=FALSE;
#if (SAM4L)
///* The first one is meaningless */
static const uint32_t divisors[5] = { 0, 2, 8, 32, 128};
#elif (MEGA_RF)
///* The last one is meaningless */
static const uint32_t divisors[7] = { 0, 1, 8, 64, 256, 1024};
///* The last one is meaningless */
#else 
static const uint32_t divisors[5] = { 2, 8, 32, 128, 0};
#endif

/**
* \brief Set up EPD Timer interrupts handler
*
* \note
* desired value: 1mSec
* actual value:  1.000mSec
*/


/**
* \brief Set up EPD Timer for 1 mSec interrupts
*
* \note
* desired value: 1mSec
* actual value:  1.000mSec
*/

void EPD_timer_handler(void){
	
	////if ( tc_is_overflow(EPD_TC_TIMER_ID) == true) {
		//EPD_Counter++;	
		//
		//// test //	
		////debug_tgl_pin();
		//// test //
		//
		//
		//// test //
		////uint32_t rc =0;
		////rc = (sysclk_get_peripheral_bus_hz(EPD_TC_TIMER_ID) / divisors[EPD_TC_ClockSignalSel] );
		////rc = (rc/10000);
		////rc = 65536 - rc;
		//// test//
	////}
	
	//volatile uint32_t status;
	//status = REG_TC0_SR0 ;
	//if ( (status & TC_SR_CPCS) == TC_SR_CPCS ) {
		EPD_Counter++;
	//}
	
	
}

/**
 * \internal
 * \brief Interrupt handler for Timer Counter  overflow
 */
//ISR(TIMER1_OVF_vect)
//{
	//if ( tc_is_overflow(EPD_TC_TIMER_ID) == true) {
		//EPD_Counter++;
	//}
//}


static void initialize_EPD_timer(void) {
		
	//***********************SAM4L ***********************************//
	//uint32_t rc;
//
	///* Configure the PMC to enable the Timer/Counter (TC) module. */
	//sysclk_enable_peripheral_clock(EPD_TC_TIMER_ID);
//
	//tc_init(EPD_TC_TIMER_ID, EPD_TC_TIMER_CHANNEL,  EPD_TC_ClockSignalSel | TC_CMR_CPCTRG );
	//rc = (sysclk_get_peripheral_bus_hz(EPD_TC_TIMER_ID) /
	//divisors[EPD_TC_ClockSignalSel]) /
	//1000;
//
	//tc_write_rc(EPD_TC_TIMER_ID, EPD_TC_TIMER_CHANNEL, rc);
//
	//// Configure and enable interrupt on RC compare
	//NVIC_EnableIRQ(EPD_TC_TIMER_IRQn);
	//tc_enable_interrupt(EPD_TC_TIMER_ID,EPD_TC_TIMER_CHANNEL, TC_IER_CPCS);
	//tc_start(EPD_TC_TIMER_ID, EPD_TC_TIMER_CHANNEL);
	//EPD_Counter=0;
	
	
	//************* MEGARF *********************//
	uint32_t rc;
	/* Configure the PMC to enable the Timer/Counter (TC) module. */
	sysclk_enable_peripheral_clock(EPD_TC_TIMER_ID);
	
	/** TC  Configuration structure. */
	struct tc_control_reg tc_control_par = {
	/** TC Compare Output Mode  */
	.co_mode = CO_NORMAL,
	/** TC Waveform Generation Mode */	
	//.wg_mode = NORMAL,
	.wg_mode = CTC_Mode1,
	
	///** Prescalar Rate selection */
	//uint8_t prescal;
	///** TC Clock Selection, Prescalar select */
	.cs_select = EPD_TC_ClockSignalSel
	};
	
	/* Init TC to timer ctc mode. */	
	tc_initc(EPD_TC_TIMER_ID, EPD_TC_TIMER_CHANNEL, &tc_control_par);

	///* Configure OVF value */
	rc = (sysclk_get_peripheral_bus_hz(EPD_TC_TIMER_ID) / 
		 divisors[EPD_TC_ClockSignalSel] ) /
	     1000 ;
	
	//tc_write_count(EPD_TC_TIMER_ID,rc);
	//
	////// Configure and enable interrupt on EPD_TC_TIMER_ID Over flow
	//tc_set_overflow_interrupt_callback(EPD_TC_TIMER_ID, EPD_timer_handler);
	//tc_enable_ovf_int(EPD_TC_TIMER_ID);
		
	tc_write_cc(EPD_TC_TIMER_ID, EPD_TC_TIMER_CHANNEL, rc);
	
	// Configure and enable interrupt on TC CTC compare match
	tc_set_compa_interrupt_callback(EPD_TC_TIMER_ID, EPD_timer_handler);
	tc_enable_compa_int(EPD_TC_TIMER_ID);
	
	cpu_irq_enable();
	tc_start(EPD_TC_TIMER_ID, &tc_control_par);
	EPD_Counter=0;
	
	/* Configure the PMC to enable the Timer/Counter (TC) module. */
	sysclk_enable_peripheral_clock(EPD_TC_TIMER_ID);
	
	
}

/**
* \brief Start Timer
*/
void start_EPD_timer(void) {
	initialize_EPD_timer();
}

/**
* \brief Stop Timer
*/
void stop_EPD_timer(void) {
	tc_stop(EPD_TC_TIMER_ID);
	//tc_disable_ovf_int(EPD_TC_TIMER_ID);
	//NVIC_DisableIRQ(EPD_TC_TIMER_IRQn);
}

/**
* \brief Get current Timer after starting a new one
*/
uint32_t get_current_time_tick(void) {
	return EPD_Counter;
}

/**
* \brief Interrupt Service Routine for system tick counter
*/
void SysTick_Handler(void) {
	EPD_Counter++;
}

/**
* \brief Delay mini-seconds
* \param ms The number of mini-seconds
*/
void sys_delay_ms(unsigned int ms) {
	uint16_t curTicks;
	start_EPD_timer();
	curTicks = EPD_Counter;
	while ((EPD_Counter - curTicks) < ms);
	stop_EPD_timer();
}

/**
* \brief Interrupt Service Routine for TC0 tick counter
*/
//void TC00_Handler(void) {
	////uint32_t status ;
	////status = REG_TC0_SR0 ;
	////if ( (status & TC_SR_CPCS) == TC_SR_CPCS ) {
		////EPD_Counter++;
	////}
//}
//

static void Wait_10us(void) {
	delay_us(10);
}

//******************************************************************
//* PWM  Configuration/Control //PWM output : PD3
//******************************************************************

/**
* \brief The PWM signal starts toggling
*/
void PWM_start_toggle(void) {
	uint16_t ra, rc;
	/* Configure the PMC to enable the Timer/Counter (TC) module. */
	sysclk_enable_peripheral_clock(EPD_TC_WAVEFORM_ID);

	/* Configure PIO Pins for TC */
	#if (SAM4L || SAM4E)
		ioport_set_pin_mode(EPD_TC_WAVEFORM_PIN, EPD_TC_WAVEFORM_PIN_FLAGS);
		ioport_disable_pin(EPD_TC_WAVEFORM_PIN); // Disable IO (but enable peripheral mode)
	#else
		gpio_configure_pin(EPD_TC_WAVEFORM_PIN, EPD_TC_WAVEFORM_PIN_FLAGS);
	#endif
	
	/** TC  Configuration structure. */
	struct tc_control_reg tc_control_par = {
		/** TC Compare Output Mode  */
		.co_mode = CO_SET,
		/** TC Waveform Generation Mode */
		.wg_mode = PWM_Mode11,
		///** Prescalar Rate selection */
		//uint8_t prescal;
		///** TC Clock Selection, Prescalar select */
		.cs_select = EPD_TC_ClockSignalSel
	};
	/* Init TC to waveform mode. */
	tc_initc(EPD_TC_WAVEFORM_ID, EPD_TC_WAVEFORM_CHANNEL, &tc_control_par);
	       
	/* Configure waveform frequency and duty cycle. */
#if (SAM4L)
	rc = (sysclk_get_peripheral_bus_hz(EPD_TC_TIMER_ID) /
	      divisors[EPD_TC_ClockSignalSel]) /
	     EPD_TC_WAVEFORM_PWM_FREQUENCY;
#else
	rc = (sysclk_get_peripheral_bus_hz(EPD_TC_TIMER_ID) /			//sysclk_get_peripheral_hz
	     EPD_TC_ClockSignalSel) /
		 EPD_TC_WAVEFORM_PWM_FREQUENCY;
#endif
	tc_clear_ic(EPD_TC_WAVEFORM_ID, EPD_TC_WAVEFORM_CHANNEL, 0);
	tc_write_ic(EPD_TC_WAVEFORM_ID, EPD_TC_WAVEFORM_CHANNEL, rc);
	ra = (100 - EPD_TC_WAVEFORM_PWM_DUTY_CYCLE) * rc / 100;
	tc_clear_cc(EPD_TC_WAVEFORM_ID, EPD_TC_WAVEFORM_CHANNEL, 0);
	tc_write_cc(EPD_TC_WAVEFORM_ID, EPD_TC_WAVEFORM_CHANNEL, ra);

	/* Enable TC EPD_TC_WAVEFORM_CHANNEL. */
	tc_start(EPD_TC_WAVEFORM_ID, &tc_control_par);
	
		
}

/**
* \brief The PWM signal stops toggling.
*/
void PWM_stop_toggle(void) {
	tc_stop(EPD_TC_WAVEFORM_ID);
}

/**
* \brief PWM toggling.
*
* \param ms The interval of PWM toggling (mini seconds)
*/
void PWM_run(uint16_t ms) {
	PWM_start_toggle();
	while(ms--)delay_ms(1);
	PWM_stop_toggle();
}

////******************************************************************
////* SPI  Configuration
////******************************************************************
#if (SAM4L)
///**
 //* \brief The usart device struct that should be initialized */
//struct usart_spi_device epd_device_conf = {
	//.id =0,
//};
#else
//* \brief The SPI device struct that should be initialized */
struct spi_device epd_device_conf = {
	//.id = IOPORT_CREATE_PIN(PORTB, 0)
	.id = 0
};


#endif



////******************************************************************
////* SPI  Configuration
////******************************************************************
///**
//
//// taken direct declaration 
 //* \brief The usart device struct that should be initialized */
////! \brief Polled SPI device definition.
//
	////struct spi_device epd_device_conf = {
		////.id = IOPORT_CREATE_PIN(PORTB, 0)
	////};
	//
	//struct usart_spi_device epd_device_conf = {
		//.id = 0
	//};

// taken direct declaration 

//struct usart_spi_device epd_device_conf = {
	//.id =0,
//};


/**
* \brief Configure SPI
*/
void spi_init(void) {
	//if(spi_flag) return;
	//spi_flag=TRUE;
	//if((PRR0 & PRSPI_bm) == 0)return;
	sysclk_enable_peripheral_clock(EPD_SPI_ID);
	gpio_configure_pin(EPD_SPI_MISO_PIN, EPD_SPI_MISO_MUX);
	ioport_disable_pin(EPD_SPI_MISO_PIN); // Disable IO (but enable peripheral mode)
	gpio_configure_pin(EPD_SPI_MOSI_PIN, EPD_SPI_MOSI_MUX);
	ioport_disable_pin(EPD_SPI_MOSI_PIN); // Disable IO (but enable peripheral mode)
	gpio_configure_pin(EPD_SPI_CLK_PIN, EPD_SPI_CLK_MUX);
	ioport_disable_pin(EPD_SPI_CLK_PIN); // Disable IO (but enable peripheral mode)
	gpio_configure_pin(EPD_SPI_SS_PIN, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	spi_master_init(EPD_SPI_ID);
	spi_master_setup_device(EPD_SPI_ID, &epd_device_conf, SPI_MODE_0, EPD_SPI_baudrate, 0);
	spi_enable(EPD_SPI_ID);
	delay_ms(5);
}

/**
* \brief Initialize SPI
*/
void spi_attach (void) {
	
	EPD_flash_cs_high();
	EPD_cs_high();
	spi_init();
}

/**
* \brief Disable SPI and change to GPIO
*/
void spi_detach (void) {
	spi_disable(EPD_SPI_ID);
	ioport_enable_pin(EPD_SPI_CLK_PIN);
	ioport_enable_pin(EPD_SPI_MOSI_PIN);
	ioport_enable_pin(EPD_SPI_MISO_PIN);

	config_gpio_dir_o(EPD_SPI_CLK_PIN);
	config_gpio_dir_o(EPD_SPI_MOSI_PIN);
	config_gpio_dir_o(EPD_SPI_MISO_PIN);

	set_gpio_low(EPD_SPI_CLK_PIN);
	set_gpio_low(EPD_SPI_MOSI_PIN);
	set_gpio_low(EPD_SPI_MISO_PIN);
	//spi_flag=FALSE;
}

/**
 * \brief Send data to SPI
 *
 * \param data The data to be sent out
 */
void SPI_write (unsigned char Data) {
	//// test	//
	//volatile uint8_t b;
	//spi_write_single(EPD_SPI_ID, Data);
	//b=spi_is_tx_empty(EPD_SPI_ID);
	//while (!b);
	//// test	//
	
	spi_write_single(EPD_SPI_ID, Data);
	while (!spi_is_tx_empty(EPD_SPI_ID));
	
}

/**
 * \brief SPI synchronous read
 */
uint8_t SPI_read(unsigned char RDATA) {
	spi_read_single(EPD_SPI_ID, &RDATA);
	return RDATA;
}

/**
 * \brief Send data to SPI with time out feature
 *
 * \param data The data to be sent out
 */
uint8_t SPI_write_ex (unsigned char Data) {
	//uint8_t cnt=200;
	uint8_t flag=1;
	//while (!(EPD_SPI_ID->US_CSR & US_CSR_TXRDY)) {
		//if((cnt--)==0) {
			//flag=0;
			//break;
		//}
	//}
	//EPD_SPI_ID->US_THR = US_THR_TXCHR(Data);
	//while (!usart_spi_is_tx_empty(EPD_SPI_ID));
	return flag;
}

#if (defined COG_V110_G2) || (defined COG_V230)
/**
* \brief SPI command
*
* \param register_index The Register Index as SPI Data to COG
* \param register_data The Register Data for sending command data to COG
* \return the SPI read value
*/
uint8_t SPI_R(uint8_t Register, uint8_t Data) {
	uint8_t result;
	EPD_cs_low ();
	SPI_write (0x70); // header of Register Index
	SPI_write (Register);

	EPD_cs_high ();
	Wait_10us ();
	EPD_cs_low ();

	SPI_write (0x73); // header of Register Data of read command
	result=SPI_read (Data);

	EPD_cs_high ();

	return result;
}
#endif

/**
* \brief SPI command if register data is larger than two bytes
*
* \param register_index The Register Index as SPI command to COG
* \param register_data The Register Data for sending command data to COG
* \param length The number of bytes of Register Data which depends on which
* Register Index is selected.
*/
void SPI_send (unsigned char register_index, unsigned char *register_data,
               unsigned length) {
	EPD_cs_low ();
	SPI_write (0x70); // header of Register Index
	SPI_write (register_index);

	EPD_cs_high ();
	Wait_10us ();
	EPD_cs_low ();

	SPI_write (0x72); // header of Register Data of write command
	while(length--) {
		SPI_write (*register_data++);
	}
	EPD_cs_high ();
}

/**
* \brief SPI command
*
* \param register_index The Register Index as SPI command to COG
* \param register_data The Register Data for sending command data to COG
*/
void SPI_send_byte (uint8_t register_index, uint8_t register_data) {
	EPD_cs_low ();
	SPI_write (0x70); // header of Register Index
	SPI_write (register_index);

	EPD_cs_high ();
	Wait_10us ();
	EPD_cs_low ();
	SPI_write (0x72); // header of Register Data
	SPI_write (register_data);
	EPD_cs_high ();
}

//******************************************************************
//* Temperature sensor  Configuration
//******************************************************************
/** ADC instance */

// taken direct declaration 

struct adc_dev_inst {
	/** Base address of the ADC module. */
	//Adcife *hw_dev;
	/** Pointer to ADC configuration structure. */
	struct adc_config  *adc_cfg;
};

// taken direct declaration 

struct adc_dev_inst g_adc_inst;
/** Conversion done flag */
volatile uint8_t g_uc_condone_flag = 0;
/** Conversion value */
uint16_t g_adc_sample_data;

/**
* \brief Callback function for ADCIFE interrupt.
*/
static void adcife_read_conv_result(void) {
	//if ((adc_get_status(&g_adc_inst) & ADCIFE_SR_SEOC) == ADCIFE_SR_SEOC) {
		//g_adc_sample_data=adc_get_last_conv_value(&g_adc_inst);
		//g_uc_condone_flag = 1;
		//adc_clear_status(&g_adc_inst, ADCIFE_SCR_SEOC);
	//}
}

/**
* \brief ADC trigger conversion
*
* \return the ADC conversion value
*/
static uint16_t get_ADC_value(void) {
	//adc_start_software_conversion(&g_adc_inst);
	//g_uc_condone_flag=0;
	//while(!g_uc_condone_flag);
	return g_adc_sample_data;
}

/**
* \brief Get temperature value from ADC
*
* \return the Celsius temperature
*/
int16_t get_temperature(void) {
	//uint8_t	i;
	//long ADC_value;
	//float Vadc=0.0;
	//float degC=0.0;
	//adc_enable(&g_adc_inst);
	//for(i=0; i<10; i++)get_ADC_value();
	//ADC_value=0;
	//for(i=0; i<EPD_ADCSampleCount; i++) {
		//ADC_value+=get_ADC_value();
	//}
	//ADC_value=ADC_value/EPD_ADCSampleCount;
	//adc_disable(&g_adc_inst);
	//Vadc=(EPD_ADCRefVcc/EPD_ADCres)*ADC_value*EPD_TempeScaled;
	//degC=(100.0f+EPD_DegCOffset)-(float)(((Vadc-1.199f)*1000.0f)/10.77f);
	//return   (int16_t)degC;
	
#if 0	
	/* Configure the PMC to enable the Timer/Counter (TC) module. */
	sysclk_enable_peripheral_clock(&ADC);
	

	uint32_t F_CPU_VAL;
       F_CPU_VAL = F_CPU;
       float temp_result, temp_result_cal;
       volatile float offset;
       double result , result_cal;
	   
	   #define NUM_SAMPLES 1
#if NUM_SAMPLES > 255
       uint16_t i;
#else
       uint8_t i;
#endif
       uint16_t adc_value;

       uint32_t temp_result32 = 0;
       int32_t offset32 = 0;

       ADCSRA = (1 << ADEN); /* Enable ADC */

       /*
       * Analog channel and gain selection
       * The MUX5 bit has to written first followed by a
       * write access to the MUX4:0 bits which triggers the update of the
       *internal buffer.
       */
       ADCSRB = (1 << MUX5);

       /*
       * Select internal 1.6V reference voltage
       * Select temperature sensor
       */
       ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3) | (1 << MUX0);

       /* Dummy conversion to clear PGA */
       if ((F_CPU_VAL == 16000000UL) || (F_CPU_VAL == 15384600UL)) {
             ADCSRA
                    = (1 <<
                           ADEN) |
                           (1 <<
                           ADSC) |
                           (1 << ADIF) | (1 << ADPS2) | (1 << ADPS0);
       } else if (F_CPU_VAL == 8000000UL) {
             ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2);
       } else if (F_CPU_VAL == 4000000UL) {
             ADCSRA
                    = (1 <<
                           ADEN) |
                           (1 <<
                           ADSC) |
                           (1 << ADIF) | (1 << ADPS1) | (1 << ADPS0);
       } else {
       }

       /* Wait for conversion to be completed */
       do {
       } while ((ADCSRA & (1 << ADIF)) == 0x00);

       /* Sample */
       for (i = 0; i < NUM_SAMPLES; i++) {
             ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIF);

             /*
             * ADC Control and Status Register A:
             * ADC Enable
             * ADC Start Conversion
             * Clear ADIF
             * Prescaler = 32 (500kHz) for 16 MHz main clock
             */

             /* Wait for conversion to be completed */
             do {
             } while ((ADCSRA & (1 << ADIF)) == 0x00);

             adc_value = ADC;

             /* Averaging */
             temp_result32 += adc_value;
       }
       temp_result = (float)temp_result32 / NUM_SAMPLES;

       ADCSRA ^= (1 << ADEN); /* Disable ADC for channel change */

       /* Get offset value */
       ADCSRB = 0x00;
       //ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3);
       ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX4) | (1 << MUX0);

       for (i = 0; i < NUM_SAMPLES; i++) {
             ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIF);

             /* Wait for conversion to be completed */
             do {
             } while ((ADCSRA & (1 << ADIF)) == 0x00);

             adc_value = ADC;

             /* Averaging */
             if (adc_value > 0x1FF) {
                    offset32 -= 0x400 - adc_value;
             } else {
                    offset32 += adc_value;
             }
       }
       offset = (float)offset32 / NUM_SAMPLES;

       ADCSRA &= ~(1 << ADEN); /* Disable ADC */
       
       temp_result -= offset;
      //ADCSRB = (1<<MUX5);
      //ADMUX = 0xC9;
      //ADCSRA |= (1<<ADEN);
       //for(i = 0; i < 16; i++){
      //ADCSRA |= (1<<ADSC);
      //do {
            //} while ((ADCSRA & (1 << ADIF)) == 0x00);
      //temp_result = ADC;
       //}
       result = ((double)temp_result * 1.13) - 272.8;
       //printf ("ADC value = %2.1f \r\n",temp_result);
       //temp_result_cal = (temp_result - 5.2781)/0.8732 - 273;
       //result_cal = ((double)temp_result_cal * 1.13) - 272.8;
       //printf ("temperature value after calibration = %2.1f \r\n",temp_result_cal);    
       F_CPU_VAL = F_CPU_VAL; /* Ignore Warnings */
	   
	   /* Configure the PMC to enable the Timer/Counter (TC) module. */
	   sysclk_disable_peripheral_clock(&ADC);
       return (int16_t)result;
	   
#endif
	   
	return   (int16_t)25;
}

/**
* \brief Initialize the temperature sensor
*/
void initialize_temperature(void) {
	//struct adc_config adc_cfg = {
		///* System clock division factor is 16 */
		//.prescal = ADC_PRESCAL_DIV16,
		///* The APB clock is used */
		//.clksel = ADC_CLKSEL_APBCLK,
		///* Max speed is 150K */
		//.speed = ADC_SPEED_150K,
		///* ADC Reference voltage is VCC/2 */
		//.refsel =ADC_REFSEL_4,
		///* Enables the Startup time */
		//.start_up = CONFIG_ADC_STARTUP
	//};
	//struct adc_seq_config adc_seq_cfg = {
		///* Select Vref for shift cycle */
		//.zoomrange = ADC_ZOOMRANGE_0,
		///* Pad Ground */
		//.muxneg = ADC_MUXNEG_1,
		///* Temperature sensor */
		//.muxpos = EPD_Temperature_Sensor_ADC,
		///* Enables the internal voltage sources */
		//.internal =ADC_INTERNAL_2,
		///* Disables the ADC gain error reduction */
		//.gcomp = ADC_GCOMP_DIS,
		///* Disables the HWLA mode */
		//.hwla = ADC_HWLA_DIS,
		//.gain=ADC_GAIN_1X,
		///* 12-bits resolution */
		//.res = ADC_RES_12_BIT,
		///* Enables the single-ended mode */
		//.bipolar = ADC_BIPOLAR_SINGLEENDED
	//};
	//struct adc_ch_config adc_ch_cfg = {
		//.seq_cfg = &adc_seq_cfg,
		///* Internal Timer Max Counter */
		//.internal_timer_max_count = 60,
		///* Window monitor mode is off */
		//.window_mode = 0,
		//.low_threshold = 0,
		//.high_threshold = 0,
	//};
//
	//adc_init(&g_adc_inst, ADCIFE, &adc_cfg);
	//adc_enable(&g_adc_inst);
	//adc_ch_set_config(&g_adc_inst, &adc_ch_cfg);
	//adc_set_callback(&g_adc_inst, ADC_SEQ_SEOC, adcife_read_conv_result, ADCIFE_IRQn, 1);
	//adc_configure_trigger(&g_adc_inst,ADC_TRIG_SW);
	//adc_configure_gain(&g_adc_inst, ADC_GAIN_1X);
}


/**
* \brief Initialize the EPD hardware setting
*/
void EPD_display_hardware_init (void) {
	EPD_initialize_gpio();
	EPD_Vcc_turn_off();
	spi_init();
	initialize_temperature();
	EPD_cs_low();
	EPD_pwm_low();
	EPD_rst_low();
	EPD_discharge_low();
	EPD_border_low();
	//initialize_EPD_timer();
}