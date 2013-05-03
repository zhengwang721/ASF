/**
 * @file gdp.h
 *
 * @brief API for ZID profile
 *
 * $Id: gdp_config.h 33949 2013-01-17 10:18:08Z agasthian.s $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#if ((defined GDP_PROFILE) || (defined DOXYGEN))

/* Prevent double inclusion */
#ifndef GDP_CONFIG_H
#define GDP_CONFIG_H


/* === Includes ============================================================= */



/* === Macros =============================================================== */


/* === Types ================================================================ */

#ifdef ZID_PROFILE
#ifdef ZID_ADAPTOR
// need to enable the adaptor specific gdp commands
#define GDP_HEARTBEAT_IND   (1)
#define GDP_HEARTBEAT_REQ   (0)
#define GDP_HEARTBEAT_CONF  (0)
#define GDP_PUSH_ATTR_IND   (1)
#define GDP_PUSH_ATTR_REQ   (0)
#define GDP_PUSH_ATTR_CONF  (0)
#define GDP_CONF_COMPLETE_IND (1)
#else
#define GDP_HEARTBEAT_IND   (0)
#define GDP_HEARTBEAT_REQ   (1)
#define GDP_HEARTBEAT_CONF  (1)
#define GDP_PUSH_ATTR_IND   (0)
#define GDP_PUSH_ATTR_REQ   (1)
#define GDP_PUSH_ATTR_CONF  (1)
#define GDP_CONF_COMPLETE_IND (0)
// need to enable the zid class device specific gdp commands
#endif

#endif



/* === Externals ============================================================ */

/* === Prototypes =========================================================== */


#endif /* GDP_CONFIG_H */

#endif  /* #if ((defined GDP_PROFILE) || (defined DOXYGEN)) */

/* EOF */
