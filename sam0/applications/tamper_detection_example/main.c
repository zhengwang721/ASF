/**
 * \file
 *
 * \brief SAM RTC Calendar Callback Quick Start
 *
 * Copyright (C) 2013-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <string.h>
#include <asf.h>
#include <stdio_serial.h>
#include <conf_example.h>

/** Create DMA descriptor */
COMPILER_ALIGNED(32)
DmacDescriptor example_descriptor;

/** Peripheral instance structures **/
struct rtc_module rtc_instance;
struct usart_module usart_instance;
struct rtc_calendar_time time;
struct rtc_calendar_alarm_time alarm;
struct rtc_calendar_time tamper_time;
struct rtc_calendar_time read_time;
struct tcc_module tcc_instance;
struct dma_resource example_resource;

/* Global variable declaration*/

/*status flag to detect whether tamper occurred or not*/
static volatile bool tamper_flag = false;
/*Array to get time value to print in LCD*/
char time_value[5];
/*destination memory of DMA to store timestamp value*/
volatile uint32_t buffer_rtc_tamper;
/*index which stores the number tamper occurrence in the emulated EEPROM*/
volatile uint16_t memory_index;

/*Tamper callback*/
/******************************************************************************
* Function Name : rtc_tamper_callback
* Function Description : Perform action to be done when tamper occurs
* Arguments : None
* Returns : None
******************************************************************************/
void rtc_tamper_callback(void)
{
	struct rtc_module *module = &rtc_instance;
	Rtc *const rtc_module = module->hw;
    /*turn on LED*/
	LCD_BACKLIGHT_OFF;
	/*clear the LCD display*/
	slcd_clear_display_memory();
	/*Turn off the LED*/
	LCD_BACKLIGHT_ON;
	/*enable alarm callback*/
	rtc_calendar_enable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_ALARM_0);

	/*setup enable TCC*/
	tcc_enable(&tcc_instance);

	/*Get current time from clock register*/
	rtc_calendar_get_time(&rtc_instance, &time);

	/*copy the time values to alarm structure*/
	alarm.time.second = time.second;

	/* Set alarm in 5 seconds */
	alarm.mask = RTC_CALENDAR_ALARM_MASK_SEC;
	alarm.time.second += 4;
	alarm.time.second = alarm.time.second % 60;

	/*set alarm*/
	rtc_calendar_set_alarm(&rtc_instance, &alarm, RTC_CALENDAR_ALARM_0);

	/*clear tamper flag and tamper_id*/
	rtc_module->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_TAMPER;
	rtc_tamper_clear_detect_flag(&rtc_instance,
	rtc_tamper_get_detect_flag(&rtc_instance));
	/*set the tamper flag*/
	tamper_flag = true;
}

/* alarm_callback*/
/******************************************************************************
* Function Name : rtc_match_callback
* Function Description : Perform action to be done when alarm occurs
* Arguments : None
* Returns : None
******************************************************************************/
void rtc_match_callback(void)
{
	/*turn Off LCD backlight*/
	LCD_BACKLIGHT_OFF;

	/*disable alarm callback*/
	rtc_calendar_disable_callback(&rtc_instance,RTC_CALENDAR_CALLBACK_ALARM_0);

	/*clear LCD display*/
	slcd_clear_display_memory();

	/*disable_TCC*/
	tcc_disable(&tcc_instance);
}

/******************************************************************************
* Function Name : configure_tamper_capture_mode
* Function Description : Configure the RTC tamper detection in capture mode
* Arguments : None
* Returns : None
******************************************************************************/
void configure_tamper_capture_mode(void)
{
	struct rtc_module *module = &rtc_instance;
	Rtc *const rtc_module = module->hw;
	     
	/* Initialize tamper in capture mode. */
	struct rtc_tamper_config config_rtc_tamper;
	rtc_tamper_get_config_defaults(&config_rtc_tamper);

	/* disable GP0*/
	config_rtc_tamper. gp0_enable = false;

	/*enable DMA on tamper detection*/
	config_rtc_tamper.dma_tamper_enable = true;

	/*configure the input action,pin and level*/
	config_rtc_tamper.in_cfg[TAMPER_INPUT_PIN].level = RTC_TAMPER_LEVEL_RISING;
	config_rtc_tamper.in_cfg[TAMPER_INPUT_PIN].action = 
			RTC_TAMPER_INPUT_ACTION_CAPTURE;

	/*disable_RTC*/
	rtc_calendar_disable(&rtc_instance);

	/*set tamper configuration*/
	rtc_tamper_set_config(&rtc_instance, &config_rtc_tamper);

	/*enable_RTC*/
	rtc_calendar_enable(&rtc_instance);

	/*dummy read timestamp value*/
	volatile uint32_t time_stamp = rtc_module->MODE2.TIMESTAMP.reg;
	
	/*dummy write to avoid warning*/
	time_stamp = time_stamp;

	/*configure DMA*/
	config_dma();
	
	/*start the DMA transfer*/
	dma_start_transfer_job(&example_resource);
		
	/*enable callback for tamper*/
	rtc_calendar_enable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_TAMPER);

	/*set the mode flag to two*/
	mode = SET_TO_CAPTURE;
	previous_mode = mode;
	printf("\r\nTamper enabled in CAPTURE mode\r\n");
}

/******************************************************************************
* Function Name : configure_tamper_wake_mode
* Function Description : Configure the RTC tamper detection in wake mode
* Arguments : None
* Returns : None
******************************************************************************/
void configure_tamper_wake_mode(void)
{
	/* Initialize tamper */
	struct rtc_tamper_config config_rtc_tamper;
	rtc_tamper_get_config_defaults(&config_rtc_tamper);

	/* disable GP0*/
	config_rtc_tamper. gp0_enable = false;

	/*configure the input action,pin and level*/
	config_rtc_tamper.in_cfg[TAMPER_INPUT_PIN].level = RTC_TAMPER_LEVEL_RISING;
	config_rtc_tamper.in_cfg[TAMPER_INPUT_PIN].action = 
			RTC_TAMPER_INPUT_ACTION_WAKE;

	/*disable_RTC*/
	rtc_calendar_disable(&rtc_instance);

	/*set tamper configuration*/
	rtc_tamper_set_config(&rtc_instance, &config_rtc_tamper);

	/*enable_RTC*/
	rtc_calendar_enable(&rtc_instance);

	/*enable callback for tamper*/
	rtc_calendar_enable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_TAMPER);

	/*set the mode flag to one*/
	mode = SET_TO_WAKE;
	previous_mode = mode;
	printf("\r\nTamper enabled in WAKE mode\r\n");
}

/******************************************************************************
* Function Name : configure_tamper_off_mode
* Function Description : Configure the RTC tamper in capture mode
* Arguments : None
* Returns : None
******************************************************************************/
void configure_tamper_off_mode(void)
{
	struct rtc_module *module = &rtc_instance;
	Rtc *const rtc_module = module->hw;
	/* Initialize tamper */
	struct rtc_tamper_config config_rtc_tamper;
	rtc_tamper_get_config_defaults(&config_rtc_tamper);

	/* disable GP0*/
	config_rtc_tamper. gp0_enable = false;

	/*disable_RTC*/
	rtc_calendar_disable(&rtc_instance);

	/*set tamper configuration*/
	rtc_tamper_set_config(&rtc_instance, &config_rtc_tamper);

	/*enable_RTC*/
	rtc_calendar_enable(&rtc_instance);

	/*enable callback for tamper*/
	rtc_calendar_disable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_TAMPER);

	/*clear tamper flag and tamper_id*/
	rtc_module->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_TAMPER;
			rtc_tamper_clear_detect_flag(&rtc_instance,
	rtc_tamper_get_detect_flag(&rtc_instance));

	mode = SET_TO_DISABLE;
	printf("\r\nTamper disabled\r\n");
	/*turn off SLCD back light*/
	LCD_BACKLIGHT_OFF;
}

/******************************************************************************
* Function Name : configure_rtc_calender_time
* Function Description : Set the given time in the clock register
* Arguments : None
* Returns : None
******************************************************************************/
void configure_rtc_calender_time(void)
{
	rtc_calendar_get_time_defaults(&time);
	/*set time*/
	time.year = YEAR;
	time.month = MONTH;
	time.day = DAY;
	time.hour = HOUR;
	time.minute = MINUTE;
	time.second = SECOND;
	time.pm = TIME_PM;

	/*set calender time*/
	rtc_calendar_set_time(&rtc_instance, &time);
}

/******************************************************************************
* Function Name : configure_rtc_calender
* Function Description : Configure the RTC in calender mode
* Arguments : None
* Returns : None
******************************************************************************/
void configure_rtc_calendar(void)
{
	/* Initialize RTC in calendar mode. */
	struct rtc_calendar_config config_rtc_calendar;
	rtc_calendar_get_config_defaults(&config_rtc_calendar);

	/*configure for 12 hours time mode*/
	config_rtc_calendar.clock_24h = false;

	/*set alarm time to alarm mask register*/
	config_rtc_calendar.alarm[0].time = alarm.time;

	/*mask the alarm for entire year*/
	config_rtc_calendar.alarm[0].mask = RTC_CALENDAR_ALARM_MASK_YEAR;

	/*initialize rtc*/
	rtc_calendar_init(&rtc_instance, RTC, &config_rtc_calendar);
	
	/*enable_RTC*/
	rtc_calendar_enable(&rtc_instance);
}

/******************************************************************************
* Function Name : configure_rtc_callbacks
* Function Description : Configure RTC callback for tamper and alarm function
* Arguments : None
* Returns : None
******************************************************************************/
void configure_rtc_callbacks(void)
{
	/*register callback for tamper*/
	rtc_calendar_register_callback(
	&rtc_instance, rtc_tamper_callback, RTC_CALENDAR_CALLBACK_TAMPER);

	/*register callback for alarm*/
	rtc_calendar_register_callback(
	&rtc_instance, rtc_match_callback, RTC_CALENDAR_CALLBACK_ALARM_0);
}

/******************************************************************************
* Function Name : tcc_callback_to_toggle_led
* Function Description : TCC callback function for toggling LED
* Arguments : structure which defines timer module
* Returns : None
******************************************************************************/
void tcc_callback_to_toggle_led(struct tcc_module *const module_inst)
{
	/*toggle the LED*/
	port_pin_toggle_output_level(LED0_PIN);
}

/******************************************************************************
* Function Name : configure_tcc
* Function Description : Configure the TCC for SLCD back light blinking
* Arguments : None
* Returns : None
******************************************************************************/
void configure_tcc(void)
{
	struct tcc_config config_tcc;
	tcc_get_config_defaults(&config_tcc, TCC0);

	config_tcc.counter.clock_source = GCLK_GENERATOR_0;
	config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV64;

	/*set TCC overflow value*/
	config_tcc.counter.period = SLCD_BLINK_SPEED;

	/*set TCC configuration*/
	tcc_init(&tcc_instance, TCC0, &config_tcc);
}

/******************************************************************************
* Function Name : configure_tcc_callbacks
* Function Description : Configure and enables the TCC callback
* Arguments : None
* Returns : None
******************************************************************************/
void configure_tcc_callbacks(void)
{
	/*register callback for TCC*/
	tcc_register_callback(&tcc_instance, tcc_callback_to_toggle_led,
	TCC_CALLBACK_OVERFLOW);

	/*enable callback for TCC*/
	tcc_enable_callback(&tcc_instance, TCC_CALLBACK_OVERFLOW);
}


/******************************************************************************
* Function Name : config_slcd
* Function Description : Pinmux configuration for SLCD pins
* Arguments : None
* Returns : None
******************************************************************************/
static void config_slcd(void)
{
	#if 1
		struct system_pinmux_config pin_conf;
		system_pinmux_get_config_defaults(&pin_conf);

		pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
		pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

		pin_conf.mux_position = MUX_PA06B_SLCD_LP6;
		system_pinmux_pin_set_config(PIN_PA06B_SLCD_LP6, &pin_conf);

		pin_conf.mux_position = MUX_PA07B_SLCD_LP7;
		system_pinmux_pin_set_config(PIN_PA07B_SLCD_LP7, &pin_conf);

		pin_conf.mux_position = MUX_PC05B_SLCD_LP8;
		system_pinmux_pin_set_config(PIN_PC05B_SLCD_LP8, &pin_conf);

		pin_conf.mux_position = MUX_PC06B_SLCD_LP9;
		system_pinmux_pin_set_config(PIN_PC06B_SLCD_LP9, &pin_conf);

		pin_conf.mux_position = MUX_PA11B_SLCD_LP14;
		system_pinmux_pin_set_config(PIN_PA11B_SLCD_LP14, &pin_conf);

		pin_conf.mux_position = MUX_PC08B_SLCD_LP15;
		system_pinmux_pin_set_config(PIN_PC08B_SLCD_LP15, &pin_conf);

		pin_conf.mux_position = MUX_PC09B_SLCD_LP16;
		system_pinmux_pin_set_config(PIN_PC09B_SLCD_LP16, &pin_conf);

		pin_conf.mux_position = MUX_PC10B_SLCD_LP17;
		system_pinmux_pin_set_config(PIN_PC10B_SLCD_LP17, &pin_conf);

		pin_conf.mux_position = MUX_PC11B_SLCD_LP18;
		system_pinmux_pin_set_config(PIN_PC11B_SLCD_LP18, &pin_conf);

		pin_conf.mux_position = MUX_PC12B_SLCD_LP19;
		system_pinmux_pin_set_config(PIN_PC12B_SLCD_LP19, &pin_conf);

		pin_conf.mux_position = MUX_PC13B_SLCD_LP20;
		system_pinmux_pin_set_config(PIN_PC13B_SLCD_LP20, &pin_conf);

		pin_conf.mux_position = MUX_PB11B_SLCD_LP21;
		system_pinmux_pin_set_config(PIN_PB11B_SLCD_LP21, &pin_conf);

		pin_conf.mux_position = MUX_PB12B_SLCD_LP22;
		system_pinmux_pin_set_config(PIN_PB12B_SLCD_LP22, &pin_conf);

		pin_conf.mux_position = MUX_PB13B_SLCD_LP23;
		system_pinmux_pin_set_config(PIN_PB13B_SLCD_LP23, &pin_conf);

		pin_conf.mux_position = MUX_PB14B_SLCD_LP24;
		system_pinmux_pin_set_config(PIN_PB14B_SLCD_LP24, &pin_conf);

		pin_conf.mux_position = MUX_PB15B_SLCD_LP25;
		system_pinmux_pin_set_config(PIN_PB15B_SLCD_LP25, &pin_conf);

		pin_conf.mux_position = MUX_PC14B_SLCD_LP26;
		system_pinmux_pin_set_config(PIN_PC14B_SLCD_LP26, &pin_conf);

		pin_conf.mux_position = MUX_PC15B_SLCD_LP27;
		system_pinmux_pin_set_config(PIN_PC15B_SLCD_LP27, &pin_conf);

		pin_conf.mux_position = MUX_PA14B_SLCD_LP30;
		system_pinmux_pin_set_config(PIN_PA14B_SLCD_LP30, &pin_conf);

		pin_conf.mux_position = MUX_PA15B_SLCD_LP31;
		system_pinmux_pin_set_config(PIN_PA15B_SLCD_LP31, &pin_conf);

		pin_conf.mux_position = MUX_PC16B_SLCD_LP36;
		system_pinmux_pin_set_config(PIN_PC16B_SLCD_LP36, &pin_conf);

		pin_conf.mux_position = MUX_PC17B_SLCD_LP37;
		system_pinmux_pin_set_config(PIN_PC17B_SLCD_LP37, &pin_conf);

		pin_conf.mux_position = MUX_PC18B_SLCD_LP38;
		system_pinmux_pin_set_config(PIN_PC18B_SLCD_LP38, &pin_conf);

		pin_conf.mux_position = MUX_PC19B_SLCD_LP39;
		system_pinmux_pin_set_config(PIN_PC19B_SLCD_LP39, &pin_conf);

		pin_conf.mux_position = MUX_PC20B_SLCD_LP40;
		system_pinmux_pin_set_config(PIN_PC20B_SLCD_LP40, &pin_conf);

		pin_conf.mux_position = MUX_PC21B_SLCD_LP41;
		system_pinmux_pin_set_config(PIN_PC21B_SLCD_LP41, &pin_conf);

		pin_conf.mux_position = MUX_PB16B_SLCD_LP42;
		system_pinmux_pin_set_config(PIN_PB16B_SLCD_LP42, &pin_conf);

		pin_conf.mux_position = MUX_PB17B_SLCD_LP43;
		system_pinmux_pin_set_config(PIN_PB17B_SLCD_LP43, &pin_conf);

		pin_conf.mux_position = MUX_PB18B_SLCD_LP44;
		system_pinmux_pin_set_config(PIN_PB18B_SLCD_LP44, &pin_conf);
	#endif
}

/******************************************************************************
* Function Name : slcd1_init
* Function Description : Initializes the SLCD
* Arguments : None
* Returns : Status of initialization
******************************************************************************/
status_code_genare_t slcd1_init(void)
{
	/* configure SLCD */
	struct slcd_config config;
	slcd_get_config_defaults(&config);

	/*SLCD_STANDARD_WAVEFORM_MODE*/
	config.waveform_mode = SLCD_STANDARD_WAVEFORM_MODE;

	/*Initialize SLCD*/
	slcd_init(&config);

	/*set frame count of SLCD*/
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_0,false,FRAME_COUNT_OVERFLOW);
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_1,false,FRAME_COUNT_OVERFLOW);
	slcd_set_frame_counter(SLCD_FRAME_COUNTER_2,false,FRAME_COUNT_OVERFLOW);
	/*set maximum contrast of SLCD*/
	slcd_set_contrast(SLCD_CONTRAST);

	return STATUS_OK;
}

/******************************************************************************
* Function Name : lcd_init
* Function Description : Initializes the SLCD with some basic configuration
* Arguments : None
* Returns : None
******************************************************************************/
void lcd_init(void)
{
         uint8_t com, seg;
	/*configure slcd*/
	config_slcd();

	/*initialize slcd*/
	slcd1_init();

	/*enable slcd*/
	slcd_enable();

	/*clear display memory*/
	slcd_clear_display_memory();

	/*Set the connected pixels*/
	for(com = 0; com <= 3; com++)
	{
		for (seg = 0; seg <= 23; seg++)
		{
			slcd_set_pixel(com,seg);
			delay_ms(10);
		}
	}
	/*clear display memory*/
	slcd_clear_display_memory();

	LCD_BACKLIGHT_OFF;
}

/******************************************************************************
* Function Name : slcd1_show_text
* Function Description : Displays the string on the SLCD
* Arguments : String to be displayed on SLCD
* Returns : None
******************************************************************************/
void slcd1_show_text(const char *data)
{
	Assert(data);
	uint32_t len = (uint32_t)strlen((char *) data);

	len = (len > SLCD1_MAX_CHAR) ? SLCD1_MAX_CHAR : len;

	slcd_character_map_set(SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT,3);
	for(uint32_t i = 0 ; *data != '\0' ; i++)		
	{
		slcd_character_write_data(0,SLCD1_TXT_SEG_INDEX_S+i*4,
		DIGI_LUT[*(data++) - 32],0x4002);
	}
}

/******************************************************************************
* Function Name : lcd_display_time
* Function Description : Displays the tamper occurrence time on SLCD
* Arguments : None
* Returns : None
******************************************************************************/
void lcd_display_time(void)
{
	/*converts time value to string for SLCD display*/
	snprintf(time_value,sizeof(time_value),"%02d%02d ",tamper_time.hour,
			tamper_time.minute);
	/*checks whether time is AM or PM*/
	if(tamper_time.pm)
	{
		/*displays PM on SLCD*/
		slcd_set_pixel(0,3);
	}
	else
	{
		/*displays PM on SLCD*/
		slcd_set_pixel(0,2);
	}
	/*Displays colon(:) on SLCD*/
	slcd_set_pixel(3,1);
	/*displays the tamper time on SLCD*/
	slcd1_show_text(time_value);
	/*writes the tamper time to RWW EEPROM memory*/
}

/******************************************************************************
* Function Name : config_DMA
* Function Description : Configure the DMA to trigger memory to memory transfer
* on tamper occurrence
* Arguments : None
* Returns : None
******************************************************************************/
void config_dma(void)			
{
	/*setup_dma_resource*/
	configure_dma_resource(&example_resource);

	/*setup_transfer_descriptor*/
	setup_transfer_descriptor(&example_descriptor);

	/*add_descriptor_to_resource*/
	dma_add_descriptor(&example_resource, &example_descriptor);
}

/*setup_dma_transfer_descriptor*/
/******************************************************************************
* Function Name : configure_tamper_capture
* Function Description : Configure the RTC tamper detection in capture mode
* Arguments : None
* Returns : None
******************************************************************************/
void setup_transfer_descriptor(DmacDescriptor *descriptor )
{
	/*initialize DMA*/
	struct dma_descriptor_config descriptor_config;
	dma_descriptor_get_config_defaults(&descriptor_config);

	/*configure DMA */
	descriptor_config.beat_size = DMA_BEAT_SIZE_WORD;
	descriptor_config.dst_increment_enable = false;
	descriptor_config.src_increment_enable = false;
	descriptor_config.block_transfer_count = sizeof(uint32_t);
	descriptor_config.source_address = 
			(uint32_t)(&(rtc_instance.hw->MODE0.TIMESTAMP));
	descriptor_config.destination_address = (uint32_t)&(buffer_rtc_tamper);
	descriptor_config.next_descriptor_address = (uint32_t)&example_descriptor;

	/*create DMA descriptor*/
	dma_descriptor_create(descriptor, &descriptor_config);
}

/*config dma resource*/
/******************************************************************************
* Function Name : configure_tamper_capture
* Function Description : Configure the RTC tamper detection in capture mode
* Arguments : None
* Returns : None
******************************************************************************/
void configure_dma_resource(struct dma_resource *resource)
{
	/*initialize DMA resources*/
	struct dma_resource_config config;
	dma_get_config_defaults(&config);

	/*configure TImestamp event as trigger source for DMA*/
	config.peripheral_trigger = RTC_DMAC_ID_TIMESTAMP;

	/*allocate DMA resource*/
	dma_allocate(resource, &config);
}

/*configure USART*/
/******************************************************************************
* Function Name : configure_tamper_capture
* Function Description : Configure the RTC tamper detection in capture mode
* Arguments : None
* Returns : None
******************************************************************************/
static void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate = SET_BAUD_RATE;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart);
	usart_enable(&usart_instance);
}

/******************************************************************************
* Function Name :  display_menu
* Function Description : Display menu printed on the terminal window
* Arguments : None
* Returns : None
******************************************************************************/
static void display_menu(void)
{
	printf("\n\r");
	printf("============================================================\n\r");
	printf("Menu: press any key to continue.\n\r");
	printf("============================================================\n\r");

	printf(" a : OFF mode: To disable tamper detection\n\r");
	printf(" b : WAKE mode: Wake on tamper detection\n\r");
	printf(" c : CAPTURE mode: Wake on tamper and capture timestamp\n\r");
	printf(" d : Show Tamper History\n\r");
	printf(" e : Clear Tamper History\n\r");

	printf("============================================================\n\r");
	printf(">> ");
}

/******************************************************************************
* Function Name : configureeeprom
* Function Description : Configure the EEPROM
* Arguments : None
* Returns : None
******************************************************************************/
void configure_eeprom(void)
{
	/* Setup EEPROM emulator service */
	enum status_code error_code = rww_eeprom_emulator_init();

	/*check_init_ok*/
	if (error_code == STATUS_ERR_NO_MEMORY)
	{
		while (true)
		{
			/* No EEPROM section has been set in the device's fuses */
		}
	}
	/*check_re-init*/
	else if (error_code != STATUS_OK)
	{
		/* Erase the emulated EEPROM memory (assume it is unformatted or
		* irrecoverably corrupt) */
		rww_eeprom_emulator_erase_memory();
		rww_eeprom_emulator_init();
	}
}

#if (SAMD21) || (SAMDA1)
	void SYSCTRL_Handler(void)
	{
		if (SYSCTRL->INTFLAG.reg & SYSCTRL_INTFLAG_BOD33DET)
		{
			SYSCTRL->INTFLAG.reg |= SYSCTRL_INTFLAG_BOD33DET;
			rww_eeprom_emulator_commit_page_buffer();
		}
	}
#endif

/******************************************************************************
* Function Name : configure_bod
* Function Description : Configures BOD
* Arguments : None
* Returns : None
******************************************************************************/
static void configure_bod(void)
{
	#if (SAMD21) || (SAMDA1)
		struct bod_config config_bod33;
		bod_get_config_defaults(&config_bod33);
		config_bod33.action = BOD_ACTION_INTERRUPT;
		/* BOD33 threshold level is about 3.2V */
		config_bod33.level = 48;
		bod_set_config(BOD_BOD33, &config_bod33);
		bod_enable(BOD_BOD33);

		SYSCTRL->INTENSET.reg |= SYSCTRL_INTENCLR_BOD33DET;
		system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_SYSCTRL);
	#endif
}

/******************************************************************************
* Function Name : write_tamper_data
* Function Description : writes the timestamp value to the RWW EEPROM memory
* Arguments : None
* Returns : None
******************************************************************************/
void write_tamper_data(void)
{
	/*offset pointing to next read location*/
	volatile uint8_t off_set = 2 ; 
	/*RWW EEPRM read memory index*/
	rww_eeprom_emulator_read_buffer(SET_TO_ZERO,(uint8_t *)&memory_index,
			LENGTH_OF_MEMORY_INDEX);
	if(memory_index<MAXIMUM_ENTRIES_STORED)
	{
		off_set = (memory_index*LENGTH_OF_TIME_STRUCT)+LENGTH_OF_MEMORY_INDEX;
		/*RWW EEPRM write page buffer*/
		rww_eeprom_emulator_write_buffer(off_set,(const uint8_t *)&tamper_time,
				LENGTH_OF_TIME_STRUCT);
		memory_index++;
		/*RWW EEPRM write memory index*/
		rww_eeprom_emulator_write_buffer(SET_TO_ZERO,(const uint8_t *)&memory_index,
				LENGTH_OF_MEMORY_INDEX);
		/*RWW EEPRM commit page buffer*/
		rww_eeprom_emulator_commit_page_buffer();
	}
	else
	{
		printf("\r\nMemory Full\n\r");
		printf("\r\nNext tamper time will not be recorded\n\r");
		printf("\r\nClear tamper history\n\r");
	}
}

/******************************************************************************
* Function Name : read_tamper_history
* Function Description : reads the timestamp value stored in the RWW EEPROM
* memory
* Arguments : None
* Returns : None
******************************************************************************/
void read_tamper_history(void)
{
	/*offset pointing to next read location*/
	volatile uint8_t read_off_set ; 
	/*initialize read offset*/
	
	
	/*Disable Tamper*/
	configure_tamper_off_mode();
	/*clear the LCD display*/
	slcd_clear_display_memory();
	printf("\r\nRead Tamper History\r\n");
	/*Read the entire EEPROM memory*/
	
	
	read_off_set = LENGTH_OF_MEMORY_INDEX;
	/*RWW EEPRM read memory index*/
	rww_eeprom_emulator_read_buffer(SET_TO_ZERO,(uint8_t *)&memory_index,
	LENGTH_OF_MEMORY_INDEX);
	if(memory_index == SET_TO_ZERO)
	{
		printf("\r\n No Tamper History available\r\n");
	}
	else
	{
		printf("\r\nTamper Detection Disabled\r\n");
		printf("\r\nShowing tamper History:\r\n");
		for(uint8_t i = 0; i<memory_index; i++)
		{
			/*RWW EEPRM read page buffer*/
			rww_eeprom_emulator_read_buffer(read_off_set,(uint8_t *)&read_time,LENGTH_OF_TIME_STRUCT);
			read_off_set = read_off_set+LENGTH_OF_TIME_STRUCT;
			printf("\r\nTamper occurred at[HH:MM:SS] = [%02d:%02d:%02d] %s\r\n",
					read_time.hour,read_time.minute,read_time.second,
					read_time.pm?"PM":"AM");
		}
	}
	enable_tamper();
}

/******************************************************************************
* Function Name : erase_tamper_history
* Function Description : erases the timestamp value stored in the RWW EEPROM
* memory
* Arguments : None
* Returns : None
******************************************************************************/
void erase_tamper_history (void)
{
	/*RWW EEPRM erase page buffer*/
	rww_eeprom_emulator_erase_memory();
	/*initialize memory buffer*/
	memory_index = SET_TO_ZERO;
	/*RWW EEPRM write page buffer*/
	rww_eeprom_emulator_write_buffer(0,(const uint8_t *)&memory_index,
			LENGTH_OF_MEMORY_INDEX);
	/*RWW EEPRM commit page buffer*/
	rww_eeprom_emulator_commit_page_buffer();
	printf("\r\nTamper History erased\r\n");
}

/******************************************************************************
* Function Name : memory_index_init
* Function Description : configures the memory_index during the startup
* memory
* Arguments : None
* Returns : None
******************************************************************************/
void memory_index_init(void)
{
	/*RWW EEPRM read page buffer*/
	rww_eeprom_emulator_read_buffer(SET_TO_ZERO,(uint8_t *)&memory_index,
	LENGTH_OF_MEMORY_INDEX);

	if(memory_index == EEPROM_EMPTY_CHAR)
	{
		/*initialize memory index*/
		memory_index = SET_TO_ZERO;
		/*RWW EEPRM write page buffer*/
		rww_eeprom_emulator_write_buffer(SET_TO_ZERO,
				(const uint8_t *)&memory_index,LENGTH_OF_MEMORY_INDEX);
		/*RWW EEPRM commit page buffer*/
		rww_eeprom_emulator_commit_page_buffer();
	}
}

/******************************************************************************
* Function Name : enable_tamper
* Function Description : configures the tamper in previous enabled mode after
*                        read operation
* Arguments : None
* Returns : None
******************************************************************************/
void enable_tamper(void)
{
	/*checks whether previous mode is capture mode*/
	if(previous_mode == SET_TO_CAPTURE)
	/*enable tamper in capture mode*/
	configure_tamper_capture_mode();
	
	/*checks whether previous mode is wake mode*/
	if(previous_mode == SET_TO_WAKE)
	/*enable tamper in wake mode*/
	configure_tamper_wake_mode();
}

/******************************************************************************
* Function Name : main
* Function Description : Detect the tamper in the configured mode.
* Records the tamper history on RWW EEPROM memory
* Displays the tamper history on user request
* Erase the tamper history on user request
* memory
* Arguments : None
* Returns : None
******************************************************************************/
int main(void)
{
	/*initialize the key input from user*/
	uint16_t key = 0;
	
	/*system_init*/
	system_init();

	/*setup_bod*/
	configure_bod();

	/*setup_init*/
	configure_eeprom();

	/*Configure and enable RTC */
	configure_rtc_calendar();

	/*configure RTC callback*/
	configure_rtc_callbacks();

	/*configure current time to RTC*/
	configure_rtc_calender_time();

	/*configure TCC*/
	configure_tcc();

	/*configure TCC callback*/
	configure_tcc_callbacks();

	/*configure usart*/
	configure_usart();//configure USART

	/* Initialize SLCD*/
	lcd_init();

	/*configure the memory index for tamper history storage*/
	memory_index_init();

	/*enable_global_interrupts*/
	system_interrupt_enable_global();

	/*Display menu*/
	display_menu();

	/* while*/
	while (1)
	{
		/*checks whether key is pressed*/
		if(!usart_read_wait(&usart_instance, &key))
		{
			printf("\r\nKey Entered: %c\r\n",key);
			switch (key)
			{
				case DISABLE_TAMPER:
				{
					/*enable tamper in capture mode*/
					configure_tamper_off_mode();
				}
				break;

				case ENABLE_TAMPER_IN_WAKE_MODE:
				{
					/*enable tamper in wake mode*/
					configure_tamper_wake_mode();
					/*enable standby sleep mode*/
					system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
					/*put the device in sleep*/
					system_sleep();
				}
				break;

				case ENABLE_TAMPER_IN_CAPTURE_MODE:
				{
					/*enable tamper in capture mode*/
					configure_tamper_capture_mode();
					/*enable standby sleep mode*/
					system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
					/*put the device in sleep*/
					system_sleep();
				}
				break;

				case READ_TAMPER_HISTORY:
				{
					/*read RWW EEPROM memory*/
					read_tamper_history();
				}
				break;

				case ERASE_TAMPER_HISTORY:
				{
					/*erase RWW EEPROM memory*/
					erase_tamper_history();
				}
				break;

				default:
				break;
			}
		}
		/*checks for the tamper occurrence*/
		if(tamper_flag == true)
		{
			/*checks whether capture or wake mode*/
			if(mode == SET_TO_CAPTURE)
			{
				rtc_tamper_get_stamp(&rtc_instance, &tamper_time);
				printf("\r\nTamper occurred at[HH:MM:SS] = [%02d:%02d:%02d] %s\r\n",
						tamper_time.hour,tamper_time.minute,tamper_time.second,
						tamper_time.pm?"PM":"AM");
				lcd_display_time();
				write_tamper_data();
				/*clear tamper flag*/
				tamper_flag = false;
			}
			else
			{
				printf("\r\nTamper occurred.\n\r");
				/*displays the tamper error message on SLCD*/
				slcd1_show_text(ERROR_STRING);
				/*clear tamper flag*/
				tamper_flag = false;
			}
		}
	}
}
