
#include "return_val.h"
#include "mac_api.h"
#include "ieee_const.h"
#include "mac_msg_const.h"

#define MAX_BUFFER_LENGTH    160

#define RX_BUFFER_LENGTH    MAX_BUFFER_LENGTH
#define TX_BUFFER_LENGTH    MAX_BUFFER_LENGTH

#define SOT_POS	         0x00
#define LEN_POS          0x01
#define PROTOCOL_ID_POS  0x02
#define CMD_POS          0x03

#define PROTOCOL_ID  0X01
/**
 * A UART state that expects a \ref SOT to be received as the next character.
 */
#define UART_RX_STATE_SOT               (1)

/**
 * A UART state that expects the length to be received as the next character.
 */
#define UART_RX_STATE_LENGTH            (2)

/**
 * A UART state that expects the next data character to be received.
 */
#define UART_RX_STATE_DATA              (3)

/**
 * A UART state that expects a \ref EOT to be received as the next character.
 */
#define UART_RX_STATE_EOT               (4)



/* UART communication framing */

/** The start of transmission delimiter. */
#define SOT                             (1)

/** The end of transmission delimiter. */
#define EOT                             (4)

extern uint8_t *rcv_frame_ptr;
extern uint8_t rcv_buffer[];

void process_rf_serial_byte(uint8_t byte);
void handle_rx_frame(void);

