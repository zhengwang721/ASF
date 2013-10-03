/**
 * \file
 *
 * \brief Top header file for SAMD21
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
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

#ifndef _SAMD21_
#define _SAMD21_

/**
 * \defgroup SAMD21_definitions SAMD21 Device Definitions
 * \brief SAMD21 CMSIS Definitions.
 */

#if   defined(__SAMD21E14__) || defined(__ATSAMD21E14__)
  #include "samd21e14.h"
#elif defined(__SAMD21E15__) || defined(__ATSAMD21E15__)
  #include "samd21e15.h"
#elif defined(__SAMD21E16__) || defined(__ATSAMD21E16__)
  #include "samd21e16.h"
#elif defined(__SAMD21E17__) || defined(__ATSAMD21E17__)
  #include "samd21e17.h"
#elif defined(__SAMD21E18__) || defined(__ATSAMD21E18__)
  #include "samd21e18.h"
#elif defined(__SAMD21G14__) || defined(__ATSAMD21G14__)
  #include "samd21g14.h"
#elif defined(__SAMD21G15__) || defined(__ATSAMD21G15__)
  #include "samd21g15.h"
#elif defined(__SAMD21G16__) || defined(__ATSAMD21G16__)
  #include "samd21g16.h"
#elif defined(__SAMD21G17__) || defined(__ATSAMD21G17__)
  #include "samd21g17.h"
#elif defined(__SAMD21G18__) || defined(__ATSAMD21G18__)
  #include "samd21g18.h"
#elif defined(__SAMD21J14__) || defined(__ATSAMD21J14__)
  #include "samd21j14.h"
#elif defined(__SAMD21J15__) || defined(__ATSAMD21J15__)
  #include "samd21j15.h"
#elif defined(__SAMD21J16__) || defined(__ATSAMD21J16__)
  #include "samd21j16.h"
#elif defined(__SAMD21J17__) || defined(__ATSAMD21J17__)
  #include "samd21j17.h"
#elif defined(__SAMD21J18__) || defined(__ATSAMD21J18__)
  #include "samd21j18.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAMD21_ */
