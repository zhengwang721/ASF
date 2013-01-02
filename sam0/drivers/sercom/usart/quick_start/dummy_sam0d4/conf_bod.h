/**
 * \file
 *
 * \brief SAM0+ BOD configuration
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef BOD_CONFIG_H
#  define BOD_CONFIG_H

/* BOD33 Configuration
 * ------------------------------------------------------*/

/* Enable BOD33 */
#define CONF_BOD33_ENABLE false

#define CONF_BOD33_ACTION SYSTEM_BOD_ACTION_RESET
//#define BOD33_ACTION SYSTEM_BOD_ACTION_INTERRUPT

#define CONF_BOD33_MODE SYSTEM_BOD_MODE_SAMPLED
//#define BOD33_MODE SYSTEM_BOD_MODE_CONTINIOUS

#define CONF_BOD33_LEVEL 10
#define CONF_BOD33_HYSTERESIS true


/* BOD12 Configuration
 * ------------------------------------------------------*/

/* Enable BOD12 */
#define CONF_BOD12_ENABLE false

/* Action on bod timeout; reset or interrupt */
#define CONF_BOD12_ACTION SYSTEM_BOD_ACTION_RESET
//#define CONF_BOD12_ACTION SYSTEM_BOD_ACTION_INTERRUPT

/* Sampled or continious monitoring */
#define CONF_BOD12_MODE SYSTEM_BOD_MODE_SAMPLED
//#define CONF_BOD12_MODE SYSTEM_BOD_MODE_CONTINIOUS

#define CONF_BOD12_HYSTERESIS true


#endif /* BOD_CONFIG_H */
