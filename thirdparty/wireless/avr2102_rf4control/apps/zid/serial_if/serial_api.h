/**
 * @file sio_handler.c
 *
 * @brief RF4CE Serial API
 *
 * $Id: serial_api.h 33723 2014-12-04 12:17:21Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2014, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef SERIAL_API_H
#define SERIAL_API_H

/* === Includes ============================================================= */

/* === Macros =============================================================== */

/* === Types ================================================================ */

/* === Externals ============================================================ */

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

void serial_interface_init(void);
void serial_data_handler(void);
/* Protocol Identifier of the RF4Control */
#define RF4CONTROL_PID                         (2)

/* RF4Control Protocol Identifier Length */
#define RF4CONTROL_PID_LEN                     (1)

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SERIAL_API_H */

/* EOF */
