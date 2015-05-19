/**
 * \file
 *
 * \brief Top header file for SAMDA0
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
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

#ifndef _SAMDA0_
#define _SAMDA0_

/**
 * \defgroup SAMDA0_definitions SAMDA0 Device Definitions
 * \brief SAMDA0 CMSIS Definitions.
 */

#if   defined(__SAMDA0E14A__) || defined(__ATSAMDA0E14A__)
  #include "samda0e14a.h"
#elif defined(__SAMDA0E15A__) || defined(__ATSAMDA0E15A__)
  #include "samda0e15a.h"
#elif defined(__SAMDA0E16A__) || defined(__ATSAMDA0E16A__)
  #include "samda0e16a.h"
#elif defined(__SAMDA0G14A__) || defined(__ATSAMDA0G14A__)
  #include "samda0g14a.h"
#elif defined(__SAMDA0G15A__) || defined(__ATSAMDA0G15A__)
  #include "samda0g15a.h"
#elif defined(__SAMDA0G16A__) || defined(__ATSAMDA0G16A__)
  #include "samda0g16a.h"
#elif defined(__SAMDA0J14A__) || defined(__ATSAMDA0J14A__)
  #include "samda0j14a.h"
#elif defined(__SAMDA0J15A__) || defined(__ATSAMDA0J15A__)
  #include "samda0j15a.h"
#elif defined(__SAMDA0J16A__) || defined(__ATSAMDA0J16A__)
  #include "samda0j16a.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAMDA0_ */
