/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

 /**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

void system_board_init(void);

/** Name string macro */
#define BOARD_NAME                "USER_BOARD"

// Uncomment and edit according to your hardware

/** \name Resonator definitions
 *  @{ */
//#define BOARD_FREQ_SLCK_XTAL      (32768U)
//#define BOARD_FREQ_SLCK_BYPASS    (32768U)
//#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
//#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
//#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
//#define BOARD_OSC_STARTUP_US      15625
/** @} */

 /** @} */

#endif // USER_BOARD_H
