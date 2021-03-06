/**
 * \file
 *
 * \brief Functions for drawing bitmaps through FatFS.
 *
 * Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "ff.h"
#include "demo.h"
#include "string.h"

/** FatFS read buffer size */
#define FF_BUFF_SIZE    1536

COMPILER_PACK_SET(1)

/* BMP (Windows) Header Format */
struct bmpfile_header {
	uint16_t type; /* signature, must be 4D42 hex */
	uint32_t file_size; /* size of BMP file in bytes (unreliable) */
	uint16_t reserved1; /* reserved, must be zero */
	uint16_t reserved2; /* reserved, must be zero */
	uint32_t offset; /* offset to start of image data in bytes */
	uint32_t header_size; /* size of BITMAPINFOHEADER structure, must be 40 */
	uint32_t width; /* image width in pixels */
	uint32_t height; /* image height in pixels */
	uint16_t planes; /* number of planes in the image, must be 1 */
	uint16_t bits; /* number of bits per pixel (1, 4, 8, 16, 24, 32) */
	uint32_t compression; /* compression type (0=none, 1=RLE-8, 2=RLE-4) */
	uint32_t inage_size; /* size of image data in bytes (including padding) */
	uint32_t h_resolution; /* horizontal resolution in pixels per meter
	                        * (unreliable) */
	uint32_t v_resolution; /* vertical resolution in pixels per meter
	                        * (unreliable) */
	uint32_t colours; /* number of colors in image, or zero */
	uint32_t important_colors; /* number of important colors, or zero */
};

COMPILER_PACK_RESET()

/** Bitmap buffer read from file */
static uint8_t demo_bmp_filedata[FF_BUFF_SIZE];

static void demo_draw_bmpfile( struct gfx_bitmap const *bmp,
		gfx_coord_t map_x, gfx_coord_t map_y,
		gfx_coord_t x, gfx_coord_t y);

/**
 * \brief Initialize FatFS and draw interface.
 */
uint8_t demo_draw_bmpfile_init(void)
{
	static FATFS fs;
	FRESULT res;
	DIR dirs;
	/* Mount disk*/
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_0, &fs);

	if (res != FR_OK) {
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 1;
	}

	/* Test if the disk is formatted */
	res = f_opendir(&dirs, "");

	if (res == FR_NO_FILESYSTEM) {
		/* Format disk */
		printf("-I- Format disk %d\n\r", LUN_ID_0);
		puts("-I- Please wait a moment during formatting...\r");
		res = f_mkfs(LUN_ID_0, /* Drv */
				0, /* FDISK partition */
				512); /* AllocSize */
		puts("-I- Disk format finished !\r");
		if (res != FR_OK) {
			printf("-E- f_mkfs pb: 0x%X\n\r", res);
			return 1;
		}
	}

	/* Set gfx draw bitmap function */
	gfx_set_ext_handler( demo_draw_bmpfile );

	return 0;
}

/**
 * \brief Draw bmp file read from FatFS.
 *
 * \param bmp Pointer to the bitmap.
 * \param map_x Start pos x.
 * \param map_y Start pos y.
 * \param x Width length.
 * \param y Height length.
 */
static void demo_draw_bmpfile(  struct gfx_bitmap const *bmp,
		gfx_coord_t map_x, gfx_coord_t map_y,
		gfx_coord_t x, gfx_coord_t y)
{
	FIL fp;
	struct bmpfile_header bmp_header;
	uint32_t length;
	uint8_t i = 0;
	uint32_t line_length;
	uint32_t min_length;
	volatile uint32_t read_line_num = 0;
	volatile uint32_t offset;
	uint32_t start_y = map_y;
	uint32_t offset_y;
	uint32_t read_length;
	gfx_coord_t width = bmp->width;
	uint32_t lcd_type = ili93xx_get_lcd_type();

	if (lcd_type == 1)
	{
		gfx_coord_t width = bmp->width;

		if (f_open(&fp, (const char *)bmp->data.custom, FA_OPEN_EXISTING |
				FA_READ) == FR_OK) {
			if (f_read(&fp, &bmp_header, sizeof(bmp_header), &length) == FR_OK) {
				switch (bmp_header.bits) {
				case 24:
					/* Set window */
					gfx_set_limits(map_x, map_y, map_x + x, map_y + y);

					line_length = (((bmp_header.width * bmp_header.bits) +31) / 32) * 4;

					min_length = width * 3;
					if (min_length == line_length) {
						/* Read buffer and write it on backend */
						do {
							f_read(&fp, demo_bmp_filedata, sizeof(demo_bmp_filedata), &length);
							gfx_copy_progmem_pixels_to_screen(demo_bmp_filedata, length /3);
						} while (length == sizeof(demo_bmp_filedata));
					} else {
						read_line_num = FF_BUFF_SIZE / line_length;
						do {
							f_read( &fp, demo_bmp_filedata, read_line_num * line_length, &length );
							offset = 0;
							for (i = 0; i < length / line_length; i++) {
								gfx_copy_progmem_pixels_to_screen(&demo_bmp_filedata[offset], width);
								offset += line_length;
							}
						} while (length == read_line_num * line_length);
					}
					break;
				default:
					break;
				}
			}
			f_close(&fp);
		}
	} else {
		gfx_set_orientation(GFX_FLIP_Y);

		offset_y = FF_BUFF_SIZE/(x+1)/3;
		read_length = offset_y*(x+1)*3;

		if (f_open(&fp, (const char *)bmp->data.custom, FA_OPEN_EXISTING |
				FA_READ) == FR_OK) {
			if (f_read( &fp, &bmp_header, sizeof(bmp_header), &length ) == FR_OK) {
				switch (bmp_header.bits) {
				case 24:
					/* Set window */
					gfx_set_limits(map_x, 320- start_y - y, map_x + x, 320 - map_y);

					line_length = (((bmp_header.width * bmp_header.bits) +31) / 32) * 4;

					min_length = width * 3;
					if (min_length == line_length) {
						/* Read buffer and write it on backend */
						do {
							f_read(&fp, demo_bmp_filedata, read_length, &length );
							gfx_set_limits(map_x, 320-start_y-y, map_x+x, 320- map_y);
							gfx_copy_progmem_pixels_to_screen(demo_bmp_filedata, length/3);
							start_y -= offset_y;
						} while (length == (read_length));
					} else {
						read_line_num = FF_BUFF_SIZE / line_length;
						do {
							f_read( &fp, demo_bmp_filedata, read_line_num * line_length, &length );
							offset = 0;
							for (i = 0; i < length / line_length; i++) {
								gfx_copy_progmem_pixels_to_screen(&demo_bmp_filedata[offset], width);
								offset += line_length;
							}
						} while (length == read_line_num * line_length);
					}
					break;
				default:
					break;
				}
			}
			f_close( &fp );
		}
		gfx_set_orientation(0);
	}
}
