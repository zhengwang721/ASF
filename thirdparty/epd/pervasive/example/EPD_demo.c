/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
* -# Include the ASF header files (through asf.h)
* -# Include Pervasive_Displays_small_EPD.h: EPD definitions
* -# Include if defined EPD_KIT_TOOL_FUNCTIONS, include EPD_Kit_Tool_Process.h
*            for working with EPD Kit Tool, or include image_data.h to load image
*            data array
*/
								
#include "EPD_demo.h"
				
/**
* \brief The main function will start showing two images alternatively on
* corresponding EPD depends on specified EPD size or work with EPD Kit Tool
*
* \note
* EPD size: EPD_144 = 1.44 inch, EPD_200 = 2 inch, EPD_270 = 2.7 inch
*/
int main(void){
	/* Initialize system clock and Xplained pro board */
	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	//sleepmgr_init(); // Optional
	board_init();
	/* Initialize EPD hardware */
	EPD_display_init();
	
#if (defined EPD_KIT_TOOL_FUNCTIONS) /** if working with EPD Kit Tool (default)***************/
	LED_init();
	EPD_Kit_Tool_process_init();
	for(;;) {
		EPD_Kit_tool_process_task();
	}
#else /** if showing two images alternatively ***************************************/

//tc_stop(EPD_TC_TIMER_ID);

for(;;) {
		//* User selects which EPD size to run demonstration by changing the
		//* USE_EPD_Type in image_data.h
		//* The Image data arrays for each EPD size are defined at image_data.c */
		#if(USE_EPD_Type==USE_EPD144)
				EPD_display_from_pointer(EPD_144,(uint8_t *)&image_array_144_2,(uint8_t *)&image_array_144_1);
		#elif(USE_EPD_Type==USE_EPD200)
				EPD_display_from_pointer(EPD_200,(uint8_t *)&image_array_200_2,(uint8_t *)&image_array_200_1);
		#elif(USE_EPD_Type==USE_EPD270)
				EPD_display_from_pointer(EPD_270,(uint8_t *)&image_array_270_2,(uint8_t *)&image_array_270_1);
		#endif


		/* The interval of two images alternatively change is 10 seconds */
		delay_ms(10000);


		#if(USE_EPD_Type==USE_EPD144)
		EPD_display_from_pointer(EPD_144,(uint8_t *)&image_array_144_1,(uint8_t *)&image_array_144_2);
		#elif(USE_EPD_Type==USE_EPD200)
		EPD_display_from_pointer(EPD_200,(uint8_t *)&image_array_200_1,(uint8_t *)&image_array_200_2);
		#elif(USE_EPD_Type==USE_EPD270)
		EPD_display_from_pointer(EPD_270,(uint8_t *)&image_array_270_1,(uint8_t *)&image_array_270_2);
		#endif

		/* The interval of two images alternatively change is 10 seconds */
		delay_ms(10000);
		
	}

#endif
	
}


/**
 * \page src/ASF/sam/components/display/Pervasive_Displays_small_EPD Quick Start Guide
 *
 * This is the quick start guide for the EPD Extension board made by Pervasive Displays Inc. 
 * with its small size EPDs on how to setup the kit to Atmel Xplained Pro. 
 * The code example in main.c shows two ways for EPD updates, images change 
 * alternately from image array and work with PDi's EPD Kit Tool by graphic 
 * user interface. It also instructs how to use the display functions.
 * Please open the index.html in Doxygen html folder to better read this guide.
 *
 * \note
 * - PDi = Pervasive Displays Inc. http://www.pervasivedisplays.com
 * - EPD = Electronic Paper Display (Electrophoretic Display)
 * - EPD Extension Board/Kit = The driving board to EPD features 20 pin out bridges to 
 *   your product or development kit
 * - EPD Kit Tool = The application(user graphic interface) to work with EPD 
 *   Extension kit. You can download the installer and source code at here: 
 *   http://www.pervasivedisplays.com/kits/ext_kit
 * - COG = Chip on Glass, the driver IC on EPD module
 * - For driving PDi's small size EPDs, please read the "COG Driver Interface
 *   Timing" document(hereinafter COG Document) first. It explains the interface
 *   to the COG driver of EPD for a MCU based solution.
 * - Document number: 4P008-00
 *   Download URL: http://www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=138408
 * - This project code supports EPD size: 1.44 inch, 2 inch and 2.7 inch
 * - Supports Atmel Xplained PRO: SAM4L Xplained PRO
 * - For optical performance and the best contrast ratio quality, PDi recommends
 *   following the standard stage time and running four stages to complete an EPD
 *   update.
 * - Every EPD update, have to do EPD initialization first. After updating EPD,
 *   have to do the power off stage.
 *
 * \section file_explanation File explanation
 * - <b>image_data:</b>\n
 *   It defines the image arrays of each EPD size. User can use the array without 
 *   application input.
 * - <b>EPD_Kit_Tool</b> folder:\n
 *   If you will work with PDi's EPD Extension Kit, the commands and definitions 
 *   are located in this folder.
 *     -# <b>Mem_Flash:</b>\n
 *        The functions of working with flash memory including saving image, easing 
 *        images and get/set parameters.
 *     -# <b>Uart_Controller:</b>\n
 *        The functions to control and return system packets with EPD Kit Tool.
 *     -# <b>Char:</b>\n
 *        The definition of ASCII characters.
 *     -# <b>EPD_Kit_Tool_Process:</b>\n
 *        All of the functions that are provided on EPD Kit Tool.
 *     -# <b>Drivers folder:</b>\n
 *        The driver of UART and LED.
 * - <b>Config</b> folder:\n
 *     -# <b>conf_EPD:</b> The EPD configurations.\n 
 *        - EPD_KIT_TOOL_FUNCTIONS: define it if you will work with EPD Kit Tool
 *          or comment out this define if just load two images
 *        - COG_VXXX: define which COG driving waveform you will use
 *        - COG_SPI_baudrate: SPI speed
 *        - EPD_KIT_TOOL_VERSION: the firmware version of this project code
 *        - EPD_KIT_TOOL_KIT_ID: define this kit ID, 0x0501: Atmel=05, Atmel SAM4L Xplained PRO=01
 * - <b>Pervasive_Displays_small_EPD</b> folder:\n
 *   All of the COG driving waveforms are located in this folder. Logically developer 
 *   doesn't need to change the codes in this folder in order to keep correct driving 
 *   the EPDs.\n\n
 *   <b><em>Software architecture:</em></b>\n
 *   [Application (ex. EPD Kit Tool)] <-- [COG Interface (<em>EPD_controller</em>)] <--
 *   [COG Process (<em>EPD_COG_process</em>)] <-- [Hardware Driver (<em>EPD_hardware_driver 
 *   & EPD_hardware_gpio</em>)]\n\n
 *    -# <b>EPD_hardware_driver:</b>\n
 *       Most of the COG hardware initialization and configuration. User can implement 
 *       the driver layer of EPD if some variables need to be adjusted. The provided 
 *       settings and functions are Timer, SPI, PWM, temperature and EPD hardware initialization.
 *    -# <b>EPD_hardware_gpio:</b>\n
 *       GPIO pins configuration.
 *    -# <b>EPD_COG_process.h:</b>\n
 *       The common definition of COG driving process.
 *    -# <b>EPD_controller:</b>\n
 *       The interface for external function to work with EPD.
 *    -# <b>EPD_COG:</b>\n
 *       The link source of different COG and EPD switching to be used.
 *    -# <b>COG</b> folder:\n
 *       Each COG driving file presents the different waveform driving processes of COG 
 *       and updating stages.
 *       The parameters of driving different EPD is defined at COG_parameters_t structure
 *       which is easy for developer adjusting initial parameters, resolution, frame time
 *       of MCU and the size of data line.
 *       - <b>EPD_COG_process_Vxxx:</b>\n
 *         The waveform driving processes and updating stages of COG v? with VXXX EPD.
 *       - <b>EPD_COG_partial_update_Vxxx:</b>\n
 *         The partial update waveform of driving processes and updating stages 
 *         of COG v? with VXXX EPD.
 *
 *
 * \section  use_case Use Case
 * -# <b>EPD_display_from_pointer</b>: Load two image data arrays from image_data.c
 *   according to predefined EPD size.
 * -# <b>EPD_display_from_flash</b>:
 *   Load image data from flash memory according to the command by EPD Kit Tool.
 *
 * \section Steps
 * -# Ensure the EPD is connected correctly on the EPD Extension board
 * -# Connect the EPD Extension board to Atmel SAM4L Xplained Pro header marked as EXT3
 *    via the 20 pins cable. The pin assigment is listed below
 * -# Connect the Atmel SAM4L Xplained Pro to computer's USB ports via 2 USB cables
 *    One is for programming code to MCU, the other is for EPD Kit Tool
 *    - <b>Run two images only:</b>\n
 *         -# Ensure what the EPD size you are connecting. Open image_data.h file and find
 *            "#define USE_EPD_Type USE_EPD200". Change the USE_EPDXXX to the correct size.
 *         -# Find "conf_EPD.h" file in config folder. Comment out "#define EPD_KIT_TOOL_FUNCTIONS"
 *         -# Start debugging to program the driving code to Atmel MCU. The EPD will show 
 *            two images change alternately every 10 seconds (default).
 *    - <b>Work with EPD Kit Tool:</b>\n
 *         -# Find "conf_EPD.h" file in config folder. Ensure "#define EPD_KIT_TOOL_FUNCTIONS".
 *         -# Start debugging to program the driving code to Atmel MCU.
 *         -# Install EPD Kit Tool and then execute it. On the EPD Kit Tool, click [Scan]
 *            button to search "Atmel Xplained PRO" kit. If found, click [Connect].
 *         -# Refer to EPD Extension Kit_User manual which can be downloaded in PDi website.
 *
 *
 * \section Pin_Assignment The pin assignment of EPD Extension board to Atmel Xplained Pro
 * - The table below give the pin connections of the 20-pin male connector on Extension board.
 * - Looking at the board oriented display side up (as front view above) and connector 
 *   on the left. Pin 1 is the top left pin (the left column are all odd numbered pins) 
 *   and pin 2 is immediately to its right (right hand column is all the even pins).\n
 *
 * ===============================================================================================
 * |Pin| Function       | SAM4L pin | Kit pin | Color  | Description                             |
 * |---|----------------|-----------|---------|--------|-----------------------------------------|
 * | 1 | VCC            | VCC       | EXT3_20 | Red    | Target supply voltage                   |
 * | 2 | -              | -         | -       | White  |                                         |
 * | 3 | -              | -         | -       | Grey   |                                         |
 * | 4 | -              | -         | -       | Purple |                                         |
 * | 5 | -              | -         | -       | Blue   |                                         |
 * | 6 | Temperature    | PB03      | EXT3_3  | Green  | On board temperature sensor output (ADC)|
 * | 7 | SPI_CLK        | PA18	    | EXT3_18 |	Yellow | Clock for SPI                           |
 * | 8 | BUSY           | PA06      | EXT3_9  | Orange | COG busy pin (GPIO)                     |
 * | 9 | PWM            | PA08      | EXT3_7  | Brown  | Square wave when EPD power on (PWM)     |
 * |10 | /RESET         | PC16      | EXT3_6  | Black  | Reset signal. Low enable (GPIO)         |
 * |11 | PANEL_ON       | PA09      | EXT3_8  | Red    | COG driver power control pin (GPIO)     |
 * |12 | DISCHARGE      | PC15      | EXT3_5  | White  | EPD discharge when EPD power off (GPIO) |
 * |13 | BORDER_CONTROL | PB04      | EXT3_4  | Grey   | Border control pin (GPIO)               |
 * |14 | SPI_MISO       | PA19      | EXT3_17 | Purple | Serial output from EPD to host MCU      |
 * |15 | SPI_MOSI       | PA20      | EXT3_16 | Blue   | Serial input from host MCU to EPD       |
 * |16 | -              | -         | -       | Green  |                                         |
 * |17 | -              | -         | -       | Yellow |                                         |
 * |18 | FLASH_CS       | PA10      | EXT3_10 | Orange | On board flash chip select (GPIO)       |
 * |19 | /EPD_CS        | PA17      | EXT3_15 | Brown  | Chip Select. Low enable (GPIO)          |
 * |20 | GND            | GND       | EXT3_19 | Black  |                                         |
 *
 * \section PDi EPD displays
 * =====================
 * | Size | PDi Model  |
 * |------|------------|
 * | 1.44 | EK014AS014 |
 * | 2.0  | EG020AS012 |
 * | 2.7  | EM027AS012 |
 */