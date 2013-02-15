/**
 * \file
 *
 * \brief Top header file for SAMD20
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

#ifndef _SAMD20_
#define _SAMD20_

#if   defined(__SAMD20E14A__) || defined(__ATSAMD20E14A__)
  #include "samd20e14a.h"
#elif defined(__SAMD20E15A__) || defined(__ATSAMD20E15A__)
  #include "samd20e15a.h"
#elif defined(__SAMD20E16A__) || defined(__ATSAMD20E16A__)
  #include "samd20e16a.h"
#elif defined(__SAMD20E17A__) || defined(__ATSAMD20E17A__)
  #include "samd20e17a.h"
#elif defined(__SAMD20E18A__) || defined(__ATSAMD20E18A__)
  #include "samd20e18a.h"
#elif defined(__SAMD20G14A__) || defined(__ATSAMD20G14A__)
  #include "samd20g14a.h"
#elif defined(__SAMD20G15A__) || defined(__ATSAMD20G15A__)
  #include "samd20g15a.h"
#elif defined(__SAMD20G16A__) || defined(__ATSAMD20G16A__)
  #include "samd20g16a.h"
#elif defined(__SAMD20G17A__) || defined(__ATSAMD20G17A__)
  #include "samd20g17a.h"
#elif defined(__SAMD20G18A__) || defined(__ATSAMD20G18A__)
  #include "samd20g18a.h"
#elif defined(__SAMD20J14A__) || defined(__ATSAMD20J14A__)
  #include "samd20j14a.h"
#elif defined(__SAMD20J15A__) || defined(__ATSAMD20J15A__)
  #include "samd20j15a.h"
#elif defined(__SAMD20J16A__) || defined(__ATSAMD20J16A__)
  #include "samd20j16a.h"
#elif defined(__SAMD20J17A__) || defined(__ATSAMD20J17A__)
  #include "samd20j17a.h"
#elif defined(__SAMD20J18A__) || defined(__ATSAMD20J18A__)
  #include "samd20j18a.h"
#else
  #error Library does not support the specified device.
#endif

#endif /* _SAMD20_ */
