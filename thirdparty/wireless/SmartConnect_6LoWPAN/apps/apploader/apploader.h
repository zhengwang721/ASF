/*
 * Copyright (c) 2013, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef APPLOADER_H
#define APPLOADER_H

#include "apploader-arch.h"

#define APPLOADER_HEADER_LEN 48

#define APPLOADER_OK 1
#define APPLOADER_FAILED_TOOSMALL 2
#define APPLOADER_FAILED_TOOBIG 3
#define APPLOADER_FAILED_BADSIGNATURE 4
#define APPLOADER_FAILED_BADLOADADDR 5
#define APPLOADER_FAILED_NEEDFORMAT 6
#define APPLOADER_FAILED_WRONGFIRMWARE 7

void apploader_format(void);

/* returns: 1, 0 */
int apploader_isformatted(void);

/* returns: APPLOADER_OK, APPLOADER_FAILED_TOOBIG,
 * APPLOADER_FAILED_NEEDFORMAT */
int apploader_append(unsigned char data);

/* returns: APPLOADER_FAILED_TOOSMALL, APPLOADER_FAILED_TOOBIG,
 * APPLOADER_FAILED_BADSIGNATURE, APPLOADER_FAILED_BADLOADADDR, APPLOADER_OK */
int apploader_install(void);

/* returns: APPLOADER_OK */
int apploader_startapp(void);

/* returns: APPLOADER_OK */
int apploader_stopapp(void);

int apploader_appstarted(void);

void apploader_firmwarehash(char *buf); /* 16 bytes */
void apploader_apphash(char *buf); /* 4 bytes */

#endif /* APPLOADER_H */
