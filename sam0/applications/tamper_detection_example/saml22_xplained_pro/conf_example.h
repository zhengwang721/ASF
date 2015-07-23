/*
 * example.h
 *
 * Created: 16-07-2015 03:12:10
 *  Author: sivashankari.selvara
 */ 

#ifndef EXAMPLE_H
#define EXAMPLE_H

/* set the value to zero*/
#define SET_TO_ZERO                    0

/*Set the baud rate*/
#define SET_BAUD_RATE                  38400

/* define slcd blink speed*/
#define SLCD_BLINK_SPEED               2500

/* define the tamper input*/
#define TAMPER_INPUT_PIN               4

/* turn off lcd backlight*/
#define LCD_BACKLIGHT_OFF              LED_On(LED_0_PIN)

/* turn on lcd backlight*/
#define LCD_BACKLIGHT_ON               LED_Off(LED_0_PIN)

/*maximum characters displayed on LCD */
#define SLCD1_MAX_CHAR                 5
/*the alpha-numerical character starts with segment 4*/
#define SLCD1_TXT_SEG_INDEX_S          4

/*frame count overflow value*/
#define FRAME_COUNT_OVERFLOW           0x2

/*Set LCD contrast*/
#define SLCD_CONTRAST                  0xf

/* error message on SLCD during wake mode*/
#define ERROR_STRING                  "TAMPR"

/*macros for different user input*/
#define DISABLE_TAMPER                'a'
#define ENABLE_TAMPER_IN_WAKE_MODE    'b'
#define ENABLE_TAMPER_IN_CAPTURE_MODE 'c'
#define READ_TAMPER_HISTORY           'd'
#define ERASE_TAMPER_HISTORY          'e'

/* Define current time*/
#define YEAR                           2012
#define MONTH                          12
#define DAY                            31 
#define HOUR                           11
#define MINUTE                         59
#define SECOND                         50
#define TIME_PM                        false     

/* Define the length of the structure time*/
#define LENGTH_OF_TIME_STRUCT          7         

/* Define the length of the memory index*/
#define LENGTH_OF_MEMORY_INDEX         2  

#define EEPROM_EMPTY_CHAR              0xFFFF

/*maximum entries stored on RWW EEPROM section*/
#define MAXIMUM_ENTRIES_STORED         128

/*enum for defining mode*/
enum 
{		
	SET_TO_DISABLE = 0,
	SET_TO_WAKE  = 1,
	SET_TO_CAPTURE = 2
}mode, previous_mode;
	

/*Function prototypes for SLCD  */
static void configure_usart(void);
static void display_menu(void);
void lcd_init(void);
static void config_slcd(void);
status_code_genare_t slcd1_init(void);
void slcd1_show_text(const char *data);
void lcd_display_time(void);

/*Function prototypes for DMA */
void config_dma(void);
void configure_dma_resource(struct dma_resource *resource);
void setup_transfer_descriptor(DmacDescriptor *descriptor);

/*Function prototypes for RTC */
void rtc_match_callback(void);
void configure_rtc_callbacks(void);
void configure_rtc_calendar(void);
void configure_tamper_off_mode(void);
void configure_tamper_capture_mode(void);
void configure_tamper_wake_mode(void);
void rtc_tamper_callback(void);
void configure_rtc_calender_time(void);

/*Function prototypes for TCC */
void configure_tcc(void);
void configure_tcc_callbacks(void);
void tcc_callback_to_toggle_led(struct tcc_module *const module_inst);

/*Function prototypes for RWW EEPROM */
void configure_eeprom(void);
void write_tamper_data(void);
void read_tamper_history(void);
void erase_tamper_history(void);
void memory_index_init(void);
void enable_tamper(void);

/*lookup table with segment values for displaying different character in LCD */
uint16_t DIGI_LUT[] = {
	
	0b0000000000000000, //32 - Space
	0b0000000000000000, //33 - !
	0b0000000000000000, //34 - "
	0b0000000000000000, //35 - #
	0b0000000000000000, //36 - $
	0b0000000000000000, //37 - %
	0b0000000000000000, //38 - &
	0b0000000000000000, //39 - '
	0b0000000000000000, //40 - (
	0b0000000000000000, //41 - )
	0b1001100111011001, //42 - *
	0b1000000110000001, //43 - +
	0b0000000000000000, //44 - ,
	0b0000000100000000, //45 - -
	0b0000000000000000, //46 - .
	0b0000100000010000, //47 - /
	0b0010111001110100, //48 - 0
	0b0000010001000000, //49 - 1
	0b0010001111000100, //50 - 2
	0b0010010111000100, //51 - 3
	0b0000010111100000, //52 - 4
	0b0010010110100100, //53 - 5
	0b0010011110100100, //54 - 6
	0b0000010001000100, //55 - 7
	0b0010011111100100, //56 - 8
	0b0010010111100100, //57 - 9
	0b0000000000000000, //58 - :
	0b0000000000000000, //59 - ;
	0b0001000000010000, //60 - <
	0b0000000000000000, //61 - =
	0b0000100000001000, //62 - >
	0b0000000000000000, //63 - ?
	0b0000000000000000, //64 - @
	0b0000011111100100, //65 - A
	0b1010010101000101, //66 - B
	0b0010001000100100, //67 - C
	0b1010010001000101, //68 - D
	0b0010001110100100, //69 - E
	0b0000001110100100, //70 - F
	0b0010011100100100, //71 - G
	0b0000011111100000, //72 - H
	0b1010000000000101, //73 - I
	0b0010011001000000, //74 - J
	0b0001001010110000, //75 - K
	0b0010001000100000, //76 - L
	0b0000011001111000, //77 - M
	0b0001011001101000, //78 - N
	0b0010011001100100, //79 - O
	0b0000001111100100, //80 - P
	0b0011011001100100, //81 - Q
	0b0001001111100100, //82 - R
	0b0010010110100100, //83 - S
	0b1000000000000101, //84 - T
	0b0010011001100000, //85 - U
	0b0000101000110000, //86 - V
	0b0001111001100000, //87 - W
	0b0001100000011000, //88 - X
	0b1000000000011000, //89 - Y
	0b0010100000010100, //90 - Z

};



#endif