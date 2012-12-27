/**
 * @file node_config.h
 *
 * @brief These are application-specific resources which are used
 *        in the example application of the coordinator in addition to the
 *        underlaying stack.
 *
 * $Id: node_config.h 33865 2012-12-26 12:39:30Z agasthian.s $
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
#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

/* === Includes ============================================================= */

#include "nwk_config.h"
#include "rf4ce.h"
#include "zrc.h"

/* === Macros =============================================================== */



/* === Types ================================================================ */


/* Configure RF4CE node capabilities */
/* The capabilities of this node. Implementation specific according to the format
 * illustrated in Figure 26.
 * Implementation specific
 * NodeTypeTarget
 * NodeTypeController
 * PowerSourceMainsTrue
 * PowerSourceMainsFalse
 * SecurityCapableTrue
 * SecurityCapableFalse
 * ChannelNormalizationCapableTrue
 * ChannelNormalizationCapableFalse    */
#ifdef RF4CE_SECURITY
#define NWKC_NODE_CAPABILITIES   (NodeTypeController | PowerSourceMainsFalse \
                                  | SecurityCapableTrue | ChannelNormalizationCapableTrue)
#else
#define NWKC_NODE_CAPABILITIES   (NodeTypeController | PowerSourceMainsFalse \
                                  | SecurityCapableFalse | ChannelNormalizationCapableTrue)
#endif

#define NWKC_MAX_PAIRING_TABLE_ENTRIES    (5)
#define NWKC_VENDOR_IDENTIFIER           (0x1014)
#define NWKC_VENDOR_STRING               "ATMEL  "          /* 7 characters ! */


/* Implementation specific ZRC constant defines */
#ifdef ENABLE_PWR_SAVE_MODE
#define NWK_ACTIVE_PERIOD           nwkcMinActivePeriod /* 16.8 ms */
#else
#define NWK_ACTIVE_PERIOD           (0) /* 16.8 ms */
#endif
#define NWK_DUTY_CYCLE              nwkcMaxDutyCycle   /* 62500 symbols = 1s */
#define NWK_DISCOVERY_LQI_THRESHOLD (0x00)


/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NODE_CONFIG_H */
/* EOF */
