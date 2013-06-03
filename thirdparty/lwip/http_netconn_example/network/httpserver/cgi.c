/**
 * \file
 *
 * \brief CGI request function handlers.
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
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

#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <lwip/ip.h>
#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/tcpip.h>
#include <lwip/stats.h>
#include <lwip/opt.h>
#include "httpd.h"
#include "cgi.h"
#include "led.h"
#include "sysclk.h"

#define CGI_MATCH_NONE   0
#define CGI_MATCH_WORD   1  /* Select item in table only if string match */
#define CGI_MATCH_EXT    2  /* Select item in table if the extention match */
#define CGI_MATCH_NAME   3  /* Select item in table if the string is content */

#define CGI_LED_ID_KEY    "n"
#define CGI_LED_CMD_KEY   "set"
#define GET_LED_STATUS(status, \
			led_id) (((status) & (1 << (led_id))) >> (led_id))
#define CLEAR_LED_STATUS(status, led_id)   ((status) &= ~(1 << (led_id)))
#define SET_LED_STATUS(status, led_id)     ((status) |= (1 << (led_id)))

#define CHIPID_NVTYP   (((CHIPID->CHIPID_CIDR) & \
	CHIPID_CIDR_NVPTYP_Msk) >>  CHIPID_CIDR_NVPTYP_Pos)
#define CHIPID_NVPSIZ  (((CHIPID->CHIPID_CIDR) & \
	CHIPID_CIDR_NVPSIZ_Msk) >>  CHIPID_CIDR_NVPSIZ_Pos)
#define CHIPID_SRAMSIZ (((CHIPID->CHIPID_CIDR) & \
	CHIPID_CIDR_SRAMSIZ_Msk) >>  CHIPID_CIDR_SRAMSIZ_Pos)
#define CHIPID_ARCH    (((CHIPID->CHIPID_CIDR) & \
	CHIPID_CIDR_ARCH_Msk) >>  CHIPID_CIDR_ARCH_Pos)
#define CHIPID_EPRCOC  (((CHIPID->CHIPID_CIDR) & \
	CHIPID_CIDR_EPROC_Msk) >>  CHIPID_CIDR_EPROC_Pos)

/* Macro to unpack the ip address from LwIP format into 4 integers. */
#define IP_ADDR_TO_INT_TUPLE(addr) \
	(int)((addr) >>  0 & 0xff), \
	(int)((addr) >>  8 & 0xff), \
	(int)((addr) >> 16 & 0xff), \
	(int)((addr) >> 24 & 0xff)

#define CGI_MSG_CMD_KEY   "msg"

static const char _unknown[] = "unknown";

static const char *const chip_id_eproc_names[] = {
	_unknown,                               /* 0 */
	"arm946es",                             /* 1 */
	"arm7tdmi",                             /* 2 */
	"cortex-m3",                            /* 3 */
	"arm920t",                              /* 4 */
	"arm926ejs",                            /* 5 */
	"cortex-a5",                            /* 6 */
	"cortex-m4"                             /* 7 */
};

static const char *const chip_id_nvpsize[] = {
	"none",                                 /*  0 */
	"8K bytes",                             /*  1 */
	"16K bytes",                            /*  2 */
	"32K bytes",                            /*  3 */
	_unknown,                               /*  4 */
	"64K bytes",                            /*  5 */
	_unknown,                               /*  6 */
	"128K bytes",                           /*  7 */
	_unknown,                               /*  8 */
	"256K bytes",                           /*  9 */
	"512K bytes",                           /* 10 */
	_unknown,                               /* 11 */
	"1024K bytes",                          /* 12 */
	_unknown,                               /* 13 */
	"2048K bytes",                          /* 14 */
	_unknown,                               /* 15 */
};

static const char *const chip_id_sramsize[] = {
	"48K bytes",                            /*  0 */
	"1K bytes",                             /*  1 */
	"2K bytes",                             /*  2 */
	"6K bytes",                             /*  3 */
	"112K bytes",                           /*  4 */
	"4K bytes",                             /*  5 */
	"80K bytes",                            /*  6 */
	"160K bytes",                           /*  7 */
	"8K bytes",                             /*  8 */
	"16K bytes",                            /*  9 */
	"32K bytes",                            /* 10 */
	"64K bytes",                            /* 11 */
	"128K bytes",                           /* 12 */
	"256K bytes",                           /* 13 */
	"96K bytes",                            /* 14 */
	"512K bytes",                           /* 15 */
};

static const struct archnames { unsigned value; const char *name; }
chip_id_archnames[] = {
	{ 0x19, "AT91SAM9xx Series" },
	{ 0x29, "AT91SAM9XExx Series" },
	{ 0x34, "AT91x34 Series" },
	{ 0x37, "CAP7 Series" },
	{ 0x39, "CAP9 Series" },
	{ 0x3B, "CAP11 Series" },
	{ 0x3C, "SAM4E Series" },
	{ 0x40, "AT91x40 Series" },
	{ 0x42, "AT91x42 Series" },
	{ 0x55, "AT91x55 Series" },
	{ 0x60, "AT91SAM7Axx Series" },
	{ 0x61, "AT91SAM7AQxx Series" },
	{ 0x63, "AT91x63 Series" },
	{ 0x70, "AT91SAM7Sxx Series" },
	{ 0x71, "AT91SAM7XCxx Series" },
	{ 0x72, "AT91SAM7SExx Series" },
	{ 0x73, "AT91SAM7Lxx Series" },
	{ 0x75, "AT91SAM7Xxx Series" },
	{ 0x76, "AT91SAM7SLxx Series" },
	{ 0x80, "ATSAM3UxC Series (100-pin version)" },
	{ 0x81, "ATSAM3UxE Series (144-pin version)" },
	{ 0x83, "ATSAM3AxC Series (100-pin version)" },
	{ 0x84, "ATSAM3XxC Series (100-pin version)" },
	{ 0x85, "ATSAM3XxE Series (144-pin version)" },
	{ 0x86, "ATSAM3X8H Series (217-pin version)" },
	{ 0x88, "ATSAM3SxA Series (48-pin version)" },
	{ 0x89, "ATSAM3SxB Series (64-pin version)" },
	{ 0x8A, "ATSAM3SxC Series (100-pin version)" },
	{ 0x92, "AT91x92 Series" },
	{ 0x95, "ATSAM3NxC Series (100-pin version)" },
	{ 0xF0, "AT75Cxx Series" },
	{ -1, NULL },
};

static const char *const chip_id_nvptype[] = {
	"rom", /* 0 */
	"romless or onchip flash", /* 1 */
	"embedded flash memory", /* 2 */
	"rom(nvpsiz) + embedded flash (nvpsiz2)", /* 3 */
	"sram emulating flash", /* 4 */
};

/* HTTP buffer to store key value. */
static char key_value[80];

/* FIFO tx buffer. */
#define CONFIG_AFSK_TX_BUFLEN			1024
static u8_t tx_buf[CONFIG_AFSK_TX_BUFLEN];

/* Function declarations */
static int cgi_echo(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_temp(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_uptime(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_resistor(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_led(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_ledStatus(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_error(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_status(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_chipInfo(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);
static int cgi_displayMsg(struct netconn *client, const char *name, char *recv_buf, size_t recv_len);

/*
 * CGI table where we associate one callback to one page.
 */
HttpCGI cgi_table[] = {
	{ CGI_MATCH_NAME, "echo", cgi_echo },
	{ CGI_MATCH_NAME, "get_temperature", cgi_temp },
	{ CGI_MATCH_NAME, "get_uptime", cgi_uptime },
	{ CGI_MATCH_NAME, "get_resistor", cgi_resistor },
	{ CGI_MATCH_NAME, "set_led", cgi_led },
	{ CGI_MATCH_NAME, "get_ledStatus", cgi_ledStatus },
	{ CGI_MATCH_NAME, "error", cgi_error },
	{ CGI_MATCH_NAME, "status", cgi_status },
	{ CGI_MATCH_NAME, "get_chipinfo", cgi_chipInfo },
	{ CGI_MATCH_NAME, "display", cgi_displayMsg },
	{ CGI_MATCH_NONE, NULL, NULL }
};

/**
 * \brief Send back the input string.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer to send back.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_echo(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)name;

	http_sendOk(client, HTTP_CONTENT_PLAIN);
	netconn_write(client, recv_buf, recv_len, NETCONN_COPY);
	return 0;
}

/**
 * \brief Send the internal core temperature in string format.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_temp(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	sprintf((char *)tx_buf, "[%d.%d]", status.internal_temp / 100,
			status.internal_temp % 100);

	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, (const char*)tx_buf, strlen((char *)tx_buf), NETCONN_COPY);

	return 0;
}

/**
 * \brief convert seconds to day/hour/minute/sec.
 *
 * \param sec_time Time value.
 * \param str Buffer to store date.
 * \param len Buffer length.
 *
 * \return 0 on success, -1 otherwise.
 */
static int sec_to_strDhms(u32_t sec_time, char *str, size_t len)
{
	u32_t h = (sec_time / 3600);
	u32_t d = h / 24;
	u32_t m = ((sec_time - (h * 3600)) / 60);
	u32_t s = (sec_time - (m * 60) - (h * 3600));

	if (len < sizeof("xxxxd xxh xxm xxs")) {
		return -1;
	}

	sprintf(str, "%ldd %ldh %ldm %lds", (long)d, (h >= 24) ? (long)(h - 24) : (long)h, (long)m, (long)s);

	return 0;
}

/**
 * \brief Send system uptime.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_uptime(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	sec_to_strDhms(status.up_time, (char *)tx_buf, sizeof(tx_buf));

	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, (const char*)tx_buf, strlen((char *)tx_buf), NETCONN_COPY);

	return 0;
}

/**
 * \brief Send the potentiometer voltage.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_resistor(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	u16_t volt;

	volt = 0;//afec_get_channel_value(AFEC0, AFE_CHANNEL_5) * 10000 / 4096;

	sprintf((char *)tx_buf, "[ \"%d.%dV\" ]", volt / 1000, volt % 1000);

	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, (const char*)tx_buf, strlen((char *)tx_buf), NETCONN_COPY);

	return 0;
}

/**
 * \brief Set the led status and send back the new status.
 *
 * \param name String containing the led status request.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_led(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	char *query_str = strstr(name, "?") + 1;
	size_t query_str_len = strlen(query_str);
	int led_id;
	int led_pio;
	int led_cmd;

	http_tokenizeGetRequest(query_str, query_str_len);

	if (http_getValue(query_str, query_str_len, CGI_LED_ID_KEY, key_value,
			sizeof(key_value)) < 0) {
		goto error;
	}

	led_id = atoi(key_value);

	if (http_getValue(query_str, query_str_len, CGI_LED_CMD_KEY, key_value,
			sizeof(key_value)) < 0) {
		goto error;
	}

	led_cmd = atoi(key_value);

	if (led_id == 0) {
		led_pio = LED2_GPIO;
	} else if (led_id == 1) {
		led_pio = LED1_GPIO;
	} else if (led_id == 2) {
		led_pio = LED0_GPIO;
	} else if (led_id == 3) {
		led_pio = LED3_GPIO;
	}

	if (led_cmd) {
	/////////////	ioport_set_pin_level(led_pio, LED2_ACTIVE_LEVEL);
	/////////////	SET_LED_STATUS(status.led_status, led_id);
	} else {
	/////////////	ioport_set_pin_level(led_pio, LED2_INACTIVE_LEVEL);
	/////////	CLEAR_LED_STATUS(status.led_status, led_id);
	}

	sprintf((char *)tx_buf, "{\"n\":%d, \"set\":,%d}", led_id, led_cmd);

	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, (const char*)tx_buf, strlen((char *)tx_buf), NETCONN_COPY);
	return 0;

error:
	http_sendInternalErr(client, HTTP_CONTENT_JSON);
	return 0;
}

/**
 * \brief Send the led status.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_ledStatus(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	sprintf((char *)tx_buf, "{ \"0\":\"%d\", \"1\":\"%d\", \"2\":\"%d\"}",
			GET_LED_STATUS(status.led_status, 0),
			GET_LED_STATUS(status.led_status, 1),
			GET_LED_STATUS(status.led_status, 2));

	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, (const char*)tx_buf, strlen((char *)tx_buf), NETCONN_COPY);

	return 0;
}

/**
 * \brief Used to handle unsupported CGI requests.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return -1.
 */
static int cgi_error(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)name;
	(void)recv_buf;
	(void)recv_len;

	return -1;
}

#define FREERTOS_STATS_SIZE 512
static char freertos_stats[FREERTOS_STATS_SIZE];

/**
 * \brief Send a JSON string representing the board status.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_status(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;
	uint32_t length = 0;
	uint32_t nb = 11;
	uint32_t i, count, new_entry;

#if LWIP_STATS
	extern uint32_t lwip_tx_rate;
	extern uint32_t lwip_rx_rate;
	volatile uint32_t tx_rate = lwip_tx_rate;
	volatile uint32_t rx_rate = lwip_rx_rate;
#else
	volatile uint32_t tx_rate = 0;
	volatile uint32_t rx_rate = 0;
#endif

	status.tot_req++;
	status.up_time = xTaskGetTickCount() / 1000;

	/* Update board status. */
	sprintf(status.last_connected_ip, "%d.%d.%d.%d", IP_ADDR_TO_INT_TUPLE(client->pcb.ip->remote_ip.addr));
	sprintf(status.local_ip, "%d.%d.%d.%d", IP_ADDR_TO_INT_TUPLE(client->pcb.ip->local_ip.addr));
	length += sprintf((char *)tx_buf, "{\"board_ip\":\"%s\",\"remote_ip\":\"%s\",\"download\":%d,\"upload\":%d",
								status.local_ip, status.last_connected_ip,
								rx_rate, tx_rate);

	/* Turn FreeRTOS stats into JSON. */
	vTaskGetRunTimeStats(freertos_stats);
	length += sprintf((char *)tx_buf + length, ",\"rtos\":{\"10");
	// i = 2 to skip first 13 10 sequence.
	for (i = 2, count = 0, new_entry = 0; i < FREERTOS_STATS_SIZE && freertos_stats[i]; ++i) {
		if (freertos_stats[i] == 13) {
			tx_buf[length++] = '\"';
			new_entry = 1;
			continue;
		}
		if (freertos_stats[i] == 10)
			continue;
		if (freertos_stats[i] == 9) {
			count += 1;
			if (count == 4) {
				tx_buf[length++] = '\"';
				tx_buf[length++] = ':';
				tx_buf[length++] = '\"';
				count = 0;
				continue;
			}
		}
		if (count != 0)
			continue;

		if (new_entry == 1) {
			new_entry = 0;
			tx_buf[length++] = ',';
			tx_buf[length++] = '\"';
			/* Append ID to task name since JSON id must be unique. */
			tx_buf[length++] = '0' + nb / 10;
			tx_buf[length++] = '0' + nb % 10;
			nb++;
		}
		tx_buf[length++] = freertos_stats[i];
	}
	tx_buf[length++] = '}';

	/* Remaining board status. */
	length += sprintf((char *)tx_buf + length, ",\"lwip_m_heap\":%d,\"lwip_m_heap_s\":%d,\"lwip_m_heap_m\":%d,\"lwip_pp\":%d,\"lwip_pp_s\":%d,\"lwip_pp_m\":%d,\"lwip_pr\":%d,\"lwip_pr_s\":%d,\"lwip_pr_m\":%d,\"lwip_tcp_pcb\":%d,\"lwip_tcp_pcb_s\":%d,\"lwip_tcp_pcb_m\":%d,\"lwip_tcp_seg\":%d,\"lwip_tcp_seg_s\":%d,\"lwip_tcp_seg_m\":%d,\"up_time\":%ld,\"tot_req\":%d}",
								lwip_stats.mem.used, lwip_stats.mem.avail, lwip_stats.mem.max,
								lwip_stats.memp[12].used, lwip_stats.memp[12].avail, lwip_stats.memp[12].max,
								lwip_stats.memp[11].used, lwip_stats.memp[11].avail, lwip_stats.memp[11].max,
								lwip_stats.memp[1].used, lwip_stats.memp[1].avail, lwip_stats.memp[1].max,
								lwip_stats.memp[3].used, lwip_stats.memp[3].avail, lwip_stats.memp[3].max,
								status.up_time, status.tot_req);


tx_buf[length] = 0;
	printf("%s\r\n", tx_buf);

	/* Send answer. */
	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, tx_buf, strlen((char *)tx_buf), NETCONN_COPY);

	return 0;
}

/**
 * \brief Return the chip ID name.
 *
 * \param idx Index in the chip_id_eproc_names table.
 *
 * \return The chip id name.
 */
static const char *chipid_eproc_name(int idx)
{
	if (idx <= 7) {
		return chip_id_eproc_names[idx];
	}

	return _unknown;
}

/**
 * \brief Return the non volatile programmable memory size.
 *
 * \param idx Index in the chip_id_nvpsize table.
 *
 * \return The chip non volatile programmable memory size.
 */
static const char *chipid_nvpsize(int idx)
{
	if (idx <= 15) {
		return chip_id_nvpsize[idx];
	}

	return _unknown;
}

/**
 * \brief Return the chip SRAM size.
 *
 * \param idx Index in the chip_id_sramsize table.
 *
 * \return The chip SRAM size.
 */
static const char *chipid_sramsize(int idx)
{
	if (idx <= 15) {
		return chip_id_sramsize[idx];
	}

	return _unknown;
}

/**
 * \brief Return the chip architecture name.
 *
 * \param idx Index in the chip_id_archnames table.
 *
 * \return The chip architecture name.
 */
static const char *chipid_archnames(unsigned value)
{
	for (int i = 0; chip_id_archnames[i].name; i++) {
		if (chip_id_archnames[i].value == value) {
			return chip_id_archnames[i].name;
		}
	}

	return _unknown;
}

/**
 * \brief Return the chip volatile programmable memory type.
 *
 * \param idx Index in the chip_id_nvptype table.
 *
 * \return The chip non volatile programmable memory type.
 */
static const char *chipid_nvptype(int idx)
{
	if (idx <= 4) {
		return chip_id_nvptype[idx];
	}

	return _unknown;
}

/**
 * \brief Send the chip ID information.
 *
 * \param name Not used.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_chipInfo(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	sprintf((char *)tx_buf,
			"{ \"core_name\":\"%s\", \"arch_name\":\"%s\", \"sram_size\":\"%s\",\"flash_size\":\"%s\", \"mem_boot_type\":\"%s\" }",
			chipid_eproc_name(CHIPID_EPRCOC),
			chipid_archnames(CHIPID_ARCH),
			chipid_sramsize(CHIPID_SRAMSIZ),
			chipid_nvpsize(CHIPID_NVPSIZ),
			chipid_nvptype(CHIPID_NVTYP));

	http_sendOk(client, HTTP_CONTENT_JSON);
	netconn_write(client, (const char*)tx_buf, strlen((char *)tx_buf), NETCONN_COPY);

	return 0;
}

/**
 * \brief Read a message.
 *
 * \param name Request containing the message.
 * \param recv_buf Receive buffer.
 * \param recv_len Receive buffer length.
 *
 * \return 0.
 */
static int cgi_displayMsg(struct netconn *client, const char *name, char *recv_buf, size_t recv_len)
{
	(void)recv_buf;
	(void)recv_len;
	(void)name;

	char *query_str = strstr(name, "?") + 1;
	size_t query_str_len = strlen(query_str);

	http_tokenizeGetRequest(query_str, query_str_len);

	if (http_getValue(query_str, query_str_len, CGI_MSG_CMD_KEY, key_value,
			sizeof(key_value)) > 0) {
		/*		gfx_bitmapClear(lcd_bitmap); */

		/*		text_style(lcd_bitmap, STYLEF_BOLD |
		 * STYLEF_UNDERLINE, STYLEF_BOLD | STYLEF_UNDERLINE); */

		/*		text_xprintf(lcd_bitmap, 0, 0, TEXT_CENTER |
		 * TEXT_FILL, "BeRTOS Simple Http Server"); */
		/*		text_style(lcd_bitmap, 0, STYLEF_MASK); */

		/*		text_xprintf(lcd_bitmap, 2, 0, TEXT_CENTER |
		 * TEXT_FILL, "Your message:"); */

		/*		text_xprintf(lcd_bitmap, 10, 0, TEXT_CENTER,
		 * "%s", key_value); */
		/*  */
		/*		lcd_hx8347_blitBitmap(lcd_bitmap); */

		http_sendOk(client, HTTP_CONTENT_JSON);
		return 0;
	}

	http_sendInternalErr(client, HTTP_CONTENT_JSON);
	return 0;
}

/**
 * \brief CGI request search engine.
 *
 * \param name CGI request name.
 * \param table CGI handler table.
 * \return A valid function handler for the specified CGI request, NULL otherwise.
 */
http_handler_t cgi_search(const char *name, HttpCGI *table)
{
	if (!table) {
		return NULL;
	}

	int i = 0;
	const char *ext = get_ext(name);

	while (table[i].name) {
		if (ext && table[i].type == CGI_MATCH_EXT) {
			if (!strcmp(table[i].name, ext)) {
				break;
			}
		} else if (table[i].type == CGI_MATCH_NAME) {
			if (strstr(name, table[i].name) != NULL) {
				break;
			}
		} else { /* (table[i].type == CGI_MATCH_WORD) */
			if (!strcmp(table[i].name, name)) {
				break;
			}
		}

		i++;
	}

	return table[i].handler;
}
