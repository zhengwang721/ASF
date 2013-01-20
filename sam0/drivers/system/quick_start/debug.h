#ifndef DEBUG_H
#  define DEBUG_G

#include <asf.h>



#define LED1 30
#define LED2 31
#define LED3 32 + 13
#define LED4 40

#define BUTTON1 32 + 16
#define BUTTON2 6
#define BUTTON3 7
#define BUTTON4 41

/* EXT1 pin 3 - 6 */
#define IOPIN1 4
#define IOPIN2 5
#define IOPIN3 32 + 4
#define IOPIN4 32 + 5


void init_debug_pins(void);

void debug_set_val(uint8_t val);


#endif /* DEBUG_H */
