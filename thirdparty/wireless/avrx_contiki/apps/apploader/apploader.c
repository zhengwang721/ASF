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

#include "contiki.h"
#include "dev/watchdog.h"
#include "dev/flash.h"
#include "leds.h"
#include "lib/crc16.h"

#include "apploader.h"

#include <stdio.h> /* For printf() */
#include <string.h> /* For memcpy() */

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static unsigned long appsize = 0; /* current size of app written to flash */
static int formatted = 0; /* flash was formatted and is ready for append() */

static int append_hascached = 0; /* flag indicating whether we have
                                    cached data that should be written
                                    to flash */
static unsigned char append_cached; /* cached data to be writted to flash */

static struct process **app_aprocesses;

static int apphash_has_cached = 0;
static char apphash_cached[4];

static int app_started = 0;

/*---------------------------------------------------------------------------*/
static void
copydown_data(char *copyfrom, char *copyto, int size)
{
  /* Load data section */
  memcpy(copyto, copyfrom, size);
}
/*---------------------------------------------------------------------------*/
static void
clear_bss(char* bssaddr, int bsssize)
{
  /* Clear bss section */
  if(bsssize != 0) {
    memset(bssaddr, 0, bsssize);
  }
}
/*---------------------------------------------------------------------------*/
static unsigned long
read_long(char* ptr)
{
  unsigned long l;
  l = 0;
  l += (unsigned long)(((unsigned long)0xff & ptr[0]) << 24);
  l += (unsigned long)(((unsigned long)0xff & ptr[1]) << 16);
  l += (unsigned long)(((unsigned long)0xff & ptr[2]) << 8);
  l += (unsigned long)(((unsigned long)0xff & ptr[3]));
  return l;
}
/*---------------------------------------------------------------------------*/
int
apploader_isformatted(void)
{
#ifndef APPLOADER_ERASED_16BITS
#define APPLOADER_ERASED_16BITS 0xffff
#endif /* APPLOADER_ERASED_16BITS */

  uint16_t data;
  unsigned long addr;
  unsigned long app_end = APPLOADER_ROM_ADDR + APPLOADER_ROM_SIZE;

  PRINTF("verify if formatted\n");

  /* Verify that flash is formatted */
  for(addr = APPLOADER_ROM_ADDR ; addr < app_end; addr += 2) {
    data = flash_read(addr);
    if(data != APPLOADER_ERASED_16BITS) {
      formatted = 0;
      PRINTF("verify if formatted: not formatted\n");
      return 0;
    }
    watchdog_periodic();
  }

  PRINTF("verify if formatted: formatted\n");
  formatted = 1;
  return 1;
}
/*---------------------------------------------------------------------------*/
void
apploader_format(void)
{
  unsigned long flashptr = (unsigned long)APPLOADER_ROM_ADDR;
  unsigned long i = 0;

#ifndef APPLOADER_ERASE_PAGESIZE
#define APPLOADER_ERASE_PAGESIZE 0x200
#endif /* APPLOADER_ERASE_PAGESIZE */

  /* TODO Make sure addresses are 512-byte aligned */

  PRINTF("formatting\n");
  flash_setup();
  i = 0;
  do {
    flash_clear(flashptr);
    flashptr += APPLOADER_ERASE_PAGESIZE;
    i += APPLOADER_ERASE_PAGESIZE;

    watchdog_periodic();
  } while(i < APPLOADER_ROM_SIZE);
  flash_done();

  formatted = 1;
  append_hascached = 0;
  appsize = 0;

  apphash_has_cached = 0;
}
/*---------------------------------------------------------------------------*/
int
apploader_append(unsigned char data)
{
  unsigned long flashptr;

  if(appsize >= APPLOADER_ROM_SIZE) {
    return APPLOADER_FAILED_TOOBIG;
  }
  if(appsize == 0 && !formatted) {
    return APPLOADER_FAILED_NEEDFORMAT;
  }
  formatted = 0;

  flashptr = (unsigned long)APPLOADER_ROM_ADDR;
  flashptr += (unsigned long)appsize;

  if((flashptr % 2) == 0) {
    append_cached = data;
    append_hascached = 1;
  } else {
    uint16_t word = (0xff & append_cached) + ((0xff & data) << 8);
    flash_setup();
    flash_write(flashptr - (unsigned long)1, word);
    flash_done();
    append_hascached = 0;
  }

  appsize++;

  apphash_has_cached = 0;
  return APPLOADER_OK;
}
/*---------------------------------------------------------------------------*/
static void
calculate_firmware_hash(char* outbuf)
{
  uint16_t crc1 = 0xFFFF;
  uint16_t crc2 = 0xFFFF;
  uint16_t crc3 = 0xFFFF;
  uint16_t crc4 = 0xFFFF;
  uint16_t crc5 = 0xFFFF;
  uint16_t crc6 = 0xFFFF;
  uint16_t crc7 = 0xFFFF;
  uint16_t crc8 = 0xFFFF;

  uint16_t data;
  unsigned char data1;
  unsigned char data2;
  unsigned long addr;


    printf("Computing fwhash from 0x%08lx to 0x%08lx\n",
           FIRMWARE_ROM_ADDR, FIRMWARE_ROM_END);
  /* TODO XXX This implementation replaces all 0x00 with 0xFF when calculating firmware hash */

  /* 16 bytes firmware hash calculated over a large memory area:
   *  2 bytes crc16 of every 8th byte, offset 0
   *  2 bytes crc16 of every 8th byte, offset 1
   *  2 bytes crc16 of every 8th byte, offset 2
   *  2 bytes crc16 of every 8th byte, offset 3
   *  2 bytes crc16 of every 8th byte, offset 4
   *  2 bytes crc16 of every 8th byte, offset 5
   *  2 bytes crc16 of every 8th byte, offset 6
   *  2 bytes crc16 of every 8th byte, offset 7
   */

  /* $ jacksum -a crc:16,1021,FFFF,true,true,0 -X [binarydump of section] */

  /* Near memory */
  watchdog_periodic();
  for(addr = FIRMWARE_ROM_ADDR + 0; addr < FIRMWARE_ROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc1 = crc16_add(data1, crc1);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc2 = crc16_add(data2, crc2);
  }
  watchdog_periodic();
  for(addr = FIRMWARE_ROM_ADDR + 2; addr < FIRMWARE_ROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc3 = crc16_add(data1, crc3);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc4 = crc16_add(data2, crc4);
  }
  watchdog_periodic();
  for(addr = FIRMWARE_ROM_ADDR + 4; addr < FIRMWARE_ROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc5 = crc16_add(data1, crc5);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc6 = crc16_add(data2, crc6);
  }
  watchdog_periodic();
  for(addr = FIRMWARE_ROM_ADDR + 6; addr < FIRMWARE_ROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc7 = crc16_add(data1, crc7);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc8 = crc16_add(data2, crc8);
  }
  watchdog_periodic();

#ifdef FIRMWARE_FARROM_ADDR
  /* Far memory */
  for(addr = FIRMWARE_FARROM_ADDR + 0; addr < FIRMWARE_FARROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc1 = crc16_add(data1, crc1);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc2 = crc16_add(data2, crc2);
  }
  watchdog_periodic();
  for(addr = FIRMWARE_FARROM_ADDR + 2; addr < FIRMWARE_FARROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc3 = crc16_add(data1, crc3);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc4 = crc16_add(data2, crc4);
  }
  watchdog_periodic();
  for(addr = FIRMWARE_FARROM_ADDR + 4; addr < FIRMWARE_FARROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc5 = crc16_add(data1, crc5);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc6 = crc16_add(data2, crc6);
  }
  watchdog_periodic();
  for(addr = FIRMWARE_FARROM_ADDR + 6; addr < FIRMWARE_FARROM_END ; addr += 8) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc7 = crc16_add(data1, crc7);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc8 = crc16_add(data2, crc8);
  }
  watchdog_periodic();
#endif /* FIRMWARE_FARROM_END */

  outbuf[0] = (unsigned char)0xff & (crc1 >> 8);
  outbuf[1] = (unsigned char)0xff & crc1;
  outbuf[2] = (unsigned char)0xff & (crc2 >> 8);
  outbuf[3] = (unsigned char)0xff & crc2;
  outbuf[4] = (unsigned char)0xff & (crc3 >> 8);
  outbuf[5] = (unsigned char)0xff & crc3;
  outbuf[6] = (unsigned char)0xff & (crc4 >> 8);
  outbuf[7] = (unsigned char)0xff & crc4;
  outbuf[8] = (unsigned char)0xff & (crc5 >> 8);
  outbuf[9] = (unsigned char)0xff & crc5;
  outbuf[10] = (unsigned char)0xff & (crc6 >> 8);
  outbuf[11] = (unsigned char)0xff & crc6;
  outbuf[12] = (unsigned char)0xff & (crc7 >> 8);
  outbuf[13] = (unsigned char)0xff & crc7;
  outbuf[14] = (unsigned char)0xff & (crc8 >> 8);
  outbuf[15] = (unsigned char)0xff & crc8;

  /*PRINTF("crc1 %04x\n", crc1);
  PRINTF("crc2 %04x\n", crc2);
  PRINTF("crc3 %04x\n", crc3);
  PRINTF("crc4 %04x\n", crc4);
  PRINTF("crc5 %04x\n", crc5);
  PRINTF("crc6 %04x\n", crc6);
  PRINTF("crc7 %04x\n", crc7);
  PRINTF("crc8 %04x\n", crc8);*/
}
/*---------------------------------------------------------------------------*/
void
apploader_firmwarehash(char* outbuf)
{
  /* 16 bytes hash */
  static int has_cached = 0;
  static char cached[16];

  if(!has_cached) {
    calculate_firmware_hash(cached);
    has_cached = 1;
  }

  memcpy(outbuf, cached, 16);
}
/*---------------------------------------------------------------------------*/
static void
calculate_app_hash(char* outbuf)
{
  uint16_t data;
  uint16_t crc;
  unsigned char data1;
  unsigned char data2;
  unsigned long addr;
  unsigned long app_end = APPLOADER_ROM_ADDR + APPLOADER_ROM_SIZE;

  PRINTF("calculating apphash\n");

  /* $ jacksum -a crc:16,1021,FFFF,true,true,0 -X [binarydump of section] */

  /* calculate hash over app area */
  crc = 0xFFFF;
  for (addr = APPLOADER_ROM_ADDR ; addr < app_end ; addr += 2) {
    data = flash_read(addr);
    data1 = (unsigned char)0xff & data;
    data2 = (unsigned char)0xff & (data >> 8);
    data1 = data1 == 0x00 ? 0xFF : data1;
    crc = crc16_add(data1, crc);
    data2 = data2 == 0x00 ? 0xFF : data2;
    crc = crc16_add(data2, crc);

    watchdog_periodic();
  }
  PRINTF("apphash %04x\n", crc);

  memset(outbuf, 0, 4);
  outbuf[0] = (unsigned char)0xff & (crc >> 8);
  outbuf[1] = (unsigned char)0xff & crc;
  outbuf[2] = 0;
  outbuf[3] = 0;
}
/*---------------------------------------------------------------------------*/
void
apploader_apphash(char* outbuf)
{
  if(apploader_isformatted()) {
    memset(outbuf, 0, 4);
    return;
  }

  if(!apphash_has_cached) {
    calculate_app_hash(apphash_cached);
    apphash_has_cached = 1;
  }

  memcpy(outbuf, apphash_cached, 4);
}
/*---------------------------------------------------------------------------*/
int
apploader_install(void)
{
  static unsigned long loadaddr;
  static unsigned long aprocesses;
  static unsigned long dataaddr;
  static unsigned long datasize;
  static unsigned long bssaddr;
  static unsigned long bsssize;
  static unsigned long dataloadptr;

  static char header[APPLOADER_HEADER_LEN];
  static char firmwarehash[16];
  static char apphash[16];

  unsigned long addr;
  int header_cnt = 0;
  int i;
  char* app;
  int force_installation; /* header states a firmwarehash of all zeroes */

  /* Read header from flash */
  header_cnt = 0;
  for(addr = APPLOADER_ROM_ADDR ; header_cnt < APPLOADER_HEADER_LEN; addr += 2) {
    uint16_t data;
    data = flash_read(addr);
    header[header_cnt++] = (unsigned char)0xff & data;
    header[header_cnt++] = (unsigned char)0xff & (data >> 8);
  }
  app = header;


  /* Flush flash buffer */
  if(append_hascached) {
    apploader_append(0x00);
  }

    printf("app header is at %p, read from 0x%08lx\n", app, APPLOADER_ROM_ADDR);
  /* Read header */
  PRINTF("app header signature: '%c%c%c%c'\n", app[0], app[1], app[2], app[3]);
  if(app[0] != 'T' || app[1] != 'H' || app[2] != 'S' || app[3] != 'Q') {
    return APPLOADER_FAILED_BADSIGNATURE;
  }
  loadaddr = read_long(&app[4]);
  PRINTF("app header loadaddr: 0x%08lx\n", loadaddr);
  if(loadaddr != APPLOADER_ROM_ADDR) {
    return APPLOADER_FAILED_BADLOADADDR;
  }
  datasize = read_long(&app[8]);
  PRINTF("app header datasize: 0x%08lx\n", datasize);
  dataaddr = read_long(&app[12]);
  PRINTF("app header dataaddr: 0x%08lx\n", dataaddr);
  dataloadptr = read_long(&app[16]);
  PRINTF("app header dataloadptr: 0x%08lx\n", dataloadptr);
  bsssize = read_long(&app[20]);
  PRINTF("app header bsssize: 0x%08lx\n", bsssize);
  bssaddr = read_long(&app[24]);
  PRINTF("app header bssaddr: 0x%08lx\n", bssaddr);
  aprocesses = read_long(&app[28]);
  PRINTF("app header aprocesses: 0x%08lx\n", aprocesses);
  memcpy(apphash, &app[32], sizeof(apphash));
  PRINTF("app header hash: ");
  force_installation = 1;
  for(i = 0; i < 16; i++) {
    if (apphash[i] != 0) {
      force_installation = 0;
    }
    PRINTF("%02x ", 0xff & apphash[i]);
  }
  PRINTF("\n");

  /* Verify our firmware hash with the app's firmware hash */
  calculate_firmware_hash(firmwarehash);
  PRINTF("firmware hash: ");
  for(i = 0; i < 16; i++) {
    PRINTF("%02x ", 0xff & firmwarehash[i]);
  }
  PRINTF("\n");

  if(!force_installation) {
    for(i = 0; i < 16; i++) {
      if (firmwarehash[i] != apphash[i]) {
        return APPLOADER_FAILED_WRONGFIRMWARE;
      }
    }
  } else {
    PRINTF("forced installation: skipped verifying firmware hash\n");
  }

  /* Prepare bss and data sections */
  PRINTF("clearing bss: %p-%p\n", (char *)(intptr_t)bssaddr,
         (char *)(intptr_t)(bssaddr + bsssize));
  clear_bss((char *)(intptr_t)bssaddr, (int)bsssize);
  PRINTF("copydown data: %p -> %p-%p\n", (char *)(intptr_t)dataloadptr,
         (char *)(intptr_t)dataaddr, (char *)(intptr_t)(dataaddr + datasize));
  copydown_data((char *)(intptr_t)dataloadptr,
                (char *)(intptr_t)dataaddr, (int)datasize);

  app_aprocesses = (struct process **)(intptr_t)aprocesses;
  return APPLOADER_OK;
}
/*---------------------------------------------------------------------------*/
int
apploader_startapp(void)
{
  app_started = 1;
  PRINTF("starting app processes: %p\n", (char*) app_aprocesses);
  autostart_start(app_aprocesses);
  return APPLOADER_OK;
}
/*---------------------------------------------------------------------------*/
int
apploader_appstarted(void)
{
  return app_started;
}
/*---------------------------------------------------------------------------*/
int
apploader_stopapp(void)
{
  /*app_started = 0;*/ /* we should never reset this */
  PRINTF("stopping app processes: %p\n", (char *)app_aprocesses);

  autostart_exit(app_aprocesses);
  app_started = 0;
  return APPLOADER_OK;
}
/*---------------------------------------------------------------------------*/
