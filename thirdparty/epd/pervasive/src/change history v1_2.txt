/*
 * change_history_v1_2.c
 *
 * Created: 8/14/2014 3:03:10 PM
 *  Author: arunkarkki.eswaran
 */ 

/*
Revision history V1_4

>> create new ASF board project for ATMega256RFR2 Xplained pro board
>> Add the drivers and services from ASF wizard.
   1, Sys clock control - Mega RF A1 implementation.
   2, IOPORT - General purpose I/O service.
   3, GPIO - General purpose Input/Output
   4, Delay routines.
   5, ADC 
   6, Watchdog timer for megaRF
   7, SPI - standard SPI
   8, Sleep manager - MegaRF implementation.
>> Copy the EPD driver package src >> ASF >> thirdparty >> wireless >> EPD 
>> Copy conf_EPD_hw_timer.h, conf_EPD.h into config directory.
>> Click the show all files icon and add the necesary files for EPD
   1, thirdparty >> include in project.
   2, Config >> conf_EPD_hw_timer.h, conf_EPD.h >> include in project
>> define the include directory path for the EPD files.
   1, Goto >> Project properties >> Toolchain >> AVR/GNU C Compiler >> Directories >> add item.
   2, ../src/ASF/thirdparty/wireless/EPD/inc
   3, ../src/ASF/thirdparty/wireless/EPD/src
   4, ../src/ASF/thirdparty/wireless/EPD/src/EPD_Kit_Tool/inc
   5, ../src/ASF/thirdparty/wireless/EPD/src/EPD_Kit_Tool/Drivers/inc
   6, ../src/ASF/thirdparty/wireless/EPD/src/Pervasive_Displays_small_EPD/inc
   7, ../src/ASF/thirdparty/wireless/EPD/src/services
   8, ../src/ASF/thirdparty/wireless/EPD/src/services/EPD_TC_TIMER_ID
   9, ../src/ASF/thirdparty/wireless/EPD/src/services/mega/drivers/tc
>> Goto asf.h include the following header file 
   // From module: TC - MEGARF driver
   #include <tc_megarf.h>

   // From module: TC - MEGARF implementation
   #include "EPD_hw_timer.h"
   #include "common_EPD_hw_timer.h"
   
>> Goto main.c include the following header file 
   #include "EPD_demo.h"
>> Goto main.c add the following function in the main function 
   EPD_example_main();
>> Goto conf_clock.h change the CONFIG_SYSCLK_PSDIV as SYSCLK_PSDIV_1
   // ===== System Clock Bus Division Options 
   #define CONFIG_SYSCLK_PSDIV         SYSCLK_PSDIV_1
>> 

Revision history V1_3:-

>> TC3 CTC mode used for EPD base timer to check the stage time 1ms tick.
>> TC3 Fast PWM ICRC used for EPD PWM generation 100KHz  
>> SPI used for EPD module configuration 
>> flash cs pin has used as debug tgl pin temporarily. 
>> internal temperature sensor used and initiated for get temp function.

>> bug found in tcmegarf.h file 
issue:- '|' used here will add the new OCCRx value with the older OCCRx value.
function :- 
static inline void tc_write_cc(volatile void *tc,
		enum tc_cc_channel_t channel_index, uint16_t value)		
		{
	uint8_t *reg = (uint8_t *)tc;
	// *(reg + channel_index + 1) |=  (value >> 8);
	// *(reg + channel_index) |=  value;
	
	fix:-  	remove the '|' operator. or OCCRx should be cleared before loading the 
			new OCCRx value.
	*(reg + channel_index + 1) =  (value >> 8);
	*(reg + channel_index) =  value;
}
	
>> bug found in tcmegarf.h file 
issue:-	'|' wont configure the Clock properly 
for example :- 	if the Previous mode is 001 new mode is 010. but we would get the
				wrong configuration result 011.  
function :- 
static inline void tc_write_clock_source(volatile void *tc,
		TC_CLKSEL_t TC_CLKSEL_enum)
{
	// if ((uintptr_t)tc == (uintptr_t)&TCCR1A) {
		// TCCR1B |=  TC_CLKSEL_enum;
	// } else if ((uintptr_t)tc == (uintptr_t)&TCCR3A) {
		// TCCR3B |=  TC_CLKSEL_enum;
	// } else if ((uintptr_t)tc == (uintptr_t)&TCCR4A) {
		// TCCR4B |=  TC_CLKSEL_enum;
	// } else if ((uintptr_t)tc == (uintptr_t)&TCCR5A) {
		// TCCR5B |=  TC_CLKSEL_enum;
	// } else {}
	
	fix:-  	remove the '|' operator. or clock select bits should be cleared 
			before loading the new value.
			
	uint8_t *reg = (uint8_t *)tc;
	
	*(reg + TCCRB_OFFSET) &= (~(0x07 << CS10));
	*(reg + TCCRB_OFFSET) |= (TC_CLKSEL_enum);
}






















*/