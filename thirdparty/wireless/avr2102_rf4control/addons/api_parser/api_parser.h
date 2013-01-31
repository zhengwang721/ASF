/**
 * @file sio_handler.c
 *
 * @brief RF4CE Serial API
 *
 * $Id: serial_api.h 28666 2011-09-25 17:59:19Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef API_PARSER_H
#define API_PARSER_H

/* === Includes ============================================================= */

/* === Macros =============================================================== */

/* === Types ================================================================ */

/* === Externals ============================================================ */

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

    void serial_api_init(void);
    bool nwk_task(void);
#define RF4CONTROL_PID                         (2)

/* MAC Protocol Identifier Length */
#define RF4CONTROL_PID_LEN                     (1)


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SERIAL_API_H */

/* EOF */
