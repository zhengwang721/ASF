/**
 * \file
 *
 * \brief KSZ8851SNL SAM driver for LwIP.
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

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include <string.h>
#include "ksz8851snl.h"
#include "ksz8851snl_reg.h"
#include "netif/sam_spi_ksz8851snl.h"
#include "sysclk.h"
#include "spi.h"
#include "pdc.h"
#include "conf_eth.h"

/** Define those to better describe your network interface */
#define IFNAME0								'e'
#define IFNAME1								'n'

/** Maximum transfer unit */
#define NET_MTU								1500

/** Network link speed */
#define NET_LINK_SPEED						100000000

/* Interrupt priorities. (lowest value = highest priority) */
/* ISRs using FreeRTOS *FromISR APIs must have priorities below or equal to */
/* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY. */
#define INT_PRIORITY_SPI					12

#if NO_SYS == 0
/** The ksz8851snl interrupts to enable */
//#define ksz8851snl_INT_GROUP (ksz8851snl_ISR_RCOMP | ksz8851snl_ISR_ROVR | ksz8851snl_ISR_HRESP | ksz8851snl_ISR_TCOMP | ksz8851snl_ISR_TUR | ksz8851snl_ISR_TFC)

/** The ksz8851snl TX errors to handle */
//#define ksz8851snl_TX_ERRORS (ksz8851snl_TSR_TFC | ksz8851snl_TSR_UND | ksz8851snl_TSR_HRESP)

/** The ksz8851snl RX errors to handle */
/*#define ksz8851snl_RX_ERRORS (ksz8851snl_RSR_RXOVR | ksz8851snl_RSR_HNO)
#else
#define INT_PRIORITY_ksz8851snl    0
#define ksz8851snl_INT_GROUP       0
#define ksz8851snl_TX_ERRORS       0
#define ksz8851snl_RX_ERRORS       0*/
#endif




/*
NPCS3 (PA22) for chip select
PA15 for INTRN
PA16 for RSTN



*/










/**
 * ksz8851snl driver structure.
 */
struct ksz8851snl_device {
	/**
	 * Pointer to allocated TX buffer.
	 * Section 3.6 of AMBA 2.0 spec states that burst should not cross
	 * 1K Boundaries.
	 * Receive buffer manager writes are burst of 2 words => 3 lsb bits
	 * of the address shall be set to 0.
	 */
	/** Pointer to Rx descriptor list (must be 8-byte aligned) */
	uint32_t rx_desc[NETIF_RX_BUFFERS];
	/** Set to 1 when corresponding pbuf can be freed. */
	uint32_t tx_desc[NETIF_RX_BUFFERS];
	/** RX pbuf pointer list */
	struct pbuf *rx_pbuf[NETIF_RX_BUFFERS];
	/** TX pbuf pointer list */
	struct pbuf *tx_pbuf[NETIF_TX_BUFFERS];
	struct pbuf *tx_cur_pbuf;

	/** Circular buffer head pointer for packet received */
	uint32_t us_rx_head;
	/** Circular buffer tail pointer for packet to be read */
	uint32_t us_rx_tail;
	/** Circular buffer head pointer by upper layer (buffer to be sent) */
	uint32_t us_tx_head;
	/** Circular buffer tail pointer incremented by handlers (buffer sent) */
	uint32_t us_tx_tail;

	/** Reference to LwIP netif structure */
	struct netif *netif;

#if NO_SYS == 0
	/** RX task notification semaphore */
	sys_sem_t rx_sem;
#endif
};

/**
 * ksz8851snl driver instance.
 */
COMPILER_ALIGNED(8)
static struct ksz8851snl_device gs_ksz8851snl_dev;

/**
 * MAC address to use
 */
static uint8_t gs_uc_mac_address[] =
{
	ETHERNET_CONF_ETHADDR0,
	ETHERNET_CONF_ETHADDR1,
	ETHERNET_CONF_ETHADDR2,
	ETHERNET_CONF_ETHADDR3,
	ETHERNET_CONF_ETHADDR4,
	ETHERNET_CONF_ETHADDR5
};

#if LWIP_STATS
/** Used to compute LwIP bandwidth */
uint32_t lwip_tx_count = 0;
uint32_t lwip_rx_count = 0;
uint32_t lwip_tx_rate = 0;
uint32_t lwip_rx_rate = 0;
#endif

volatile uint32_t g_intn_flag = 0;
volatile uint32_t g_spi_pdc_flag = 0;
extern Pdc *g_p_spi_pdc;

#define SPI_PDC_IDLE		0
#define SPI_PDC_RX_START	1
#define SPI_PDC_RX_COMPLETE	2
#define SPI_PDC_TX_START	3
#define SPI_PDC_TX_COMPLETE	4

void SPI_Handler(void)
{
	uint32_t status;

	status = spi_read_status(KSZ8851SNL_SPI);
	status &= spi_read_interrupt_mask(KSZ8851SNL_SPI);

	if (SPI_PDC_RX_START == g_spi_pdc_flag) {
		if (status & SPI_SR_RXBUFF) {
			////printf("SPI_Handler SPI_SR_RXBUFF!\n");
			/* Disable SPI interrupt. */
			spi_disable_interrupt(KSZ8851SNL_SPI, SPI_IDR_RXBUFF);
			g_spi_pdc_flag = SPI_PDC_RX_COMPLETE;
		}
	}
	else if (SPI_PDC_TX_START == g_spi_pdc_flag) {
	////printf("---\n");
		if (status & SPI_SR_ENDTX) {
			////printf("SPI_Handler SPI_SR_ENDTX!\n");
			////printf("gs_ksz8851snl_dev.tx_cur_pbuf=%p\n", gs_ksz8851snl_dev.tx_cur_pbuf);
			////printf("gs_ksz8851snl_dev.tx_cur_pbuf->next=%p\n", gs_ksz8851snl_dev.tx_cur_pbuf->next);
			/* Fetch next pbuf in the pbuf chain. */
			if (gs_ksz8851snl_dev.tx_cur_pbuf)
				gs_ksz8851snl_dev.tx_cur_pbuf = gs_ksz8851snl_dev.tx_cur_pbuf->next;

			/* If next pbuf is available, enqueue for transfer. */
			if (gs_ksz8851snl_dev.tx_cur_pbuf) {
				pdc_packet_t g_pdc_spi_tx_npacket;
				pdc_packet_t g_pdc_spi_rx_npacket;

				////printf("SPI_Handler SPI_SR_ENDTX has next...\n");
				g_pdc_spi_tx_npacket.ul_addr = (uint32_t) gs_ksz8851snl_dev.tx_cur_pbuf->payload;
				g_pdc_spi_tx_npacket.ul_size = gs_ksz8851snl_dev.tx_cur_pbuf->len;
				g_pdc_spi_rx_npacket.ul_addr = (uint32_t) gs_ksz8851snl_dev.tx_cur_pbuf->payload;
				g_pdc_spi_rx_npacket.ul_size = gs_ksz8851snl_dev.tx_cur_pbuf->len;

				pdc_tx_init(g_p_spi_pdc, NULL, &g_pdc_spi_tx_npacket);
				pdc_rx_init(g_p_spi_pdc, NULL, &g_pdc_spi_rx_npacket);
			}
			else {
				////printf("SPI_Handler SPI_SR_ENDTX no next!\n");
				/* Disable SPI interrupt. */
				spi_disable_interrupt(KSZ8851SNL_SPI, SPI_IDR_ENDTX);
				spi_enable_interrupt(KSZ8851SNL_SPI, SPI_IER_TXBUFE);
			}
		}

		if (status & SPI_SR_TXBUFE) {
			////printf("SPI_Handler SPI_SR_TXBUFE!\n");
			/* Disable SPI interrupt. */
			spi_disable_interrupt(KSZ8851SNL_SPI, SPI_IDR_TXBUFE);
			g_spi_pdc_flag = SPI_PDC_TX_COMPLETE;
		}
	////printf("---\n");
	}
}

/**
 *  \brief Handler for INTN falling edge interrupt.
 */
static void INTN_Handler(uint32_t id, uint32_t mask)
{
	if ((INTN_ID == id) && (INTN_PIN_MSK == mask)) {
		/* Clear the PIO interrupt flags. */
		pio_get_interrupt_status(INTN_PIO);

		/* Set the INTN flag. */
		g_intn_flag = 1;
	}
}

static void ksz8851snl_update(struct netif *netif)
{
	struct ksz8851snl_device *ps_ksz8851snl_dev = netif->state;
	uint16_t status;
	uint16_t len;

	/* Check for free PDC. */
	if (SPI_PDC_IDLE == g_spi_pdc_flag) {

		/* Handle RX. */
		if (g_intn_flag)
		{
			g_intn_flag = 0;
			status = ksz8851_reg_read(REG_INT_STATUS);

			/* Check for errors. */
			if (status & INT_RX_OVERRUN) {
				status &= ~INT_RX_OVERRUN;
			}

			/* Check for link up. */
			if (status & INT_RX_WOL_LINKUP) {
				status &= ~INT_RX_WOL_LINKUP;
			}

			/* Clear interrupt flags. */
			ksz8851_reg_write(REG_INT_STATUS, status);

			/* Check for RX packet. */
			if (status & INT_RX) {
				ksz8851_reg_setbits(REG_INT_STATUS, INT_RX);

				//printf("rx frame count %d\n", ksz8851_reg_read(REG_RX_FRAME_CNT_THRES) >> 8);
ksz8851_reg_read(REG_RX_FRAME_CNT_THRES);
				/* Get RX packet status. */
				status = ksz8851_reg_read(REG_RX_FHR_STATUS);
				if (!(status >> 0x10 == 0) || ((status & 0x3C17) == 0x3C17)) {//(ksz8851_reg_read(REG_RX_FHR_STATUS) & RX_ERRORS) {
					ksz8851_reg_setbits(REG_RXQ_CMD, RXQ_CMD_FREE_PACKET);
					//printf("ksz8851snl_update: packet error!\n");

				}
				else {
					/* Read byte count. */
					len = ksz8851_reg_read(REG_RX_FHR_BYTE_CNT) & RX_BYTE_CNT_MASK;
					//len = ksz8851_reg_read(REG_RX_FHR_BYTE_CNT) & RX_BYTE_CNT_MASK;

					/* Drop packet is len is invalid or no descriptor available. */
					if (0 == len || ps_ksz8851snl_dev->rx_desc[ps_ksz8851snl_dev->us_rx_head]) {
						ksz8851_reg_setbits(REG_RXQ_CMD, RXQ_CMD_FREE_PACKET);
						//printf("ksz8851snl_update: len or desc is invalid!\n");
					}
					else {
						//printf("ksz8851snl_update: start packet receive [len=%d]\n", len);
						/* Clear rx frame pointer. */
						ksz8851_reg_clrbits(REG_RX_ADDR_PTR, ADDR_PTR_MASK);

						/* Enable RXQ read access. */
						ksz8851_reg_setbits(REG_RXQ_CMD, RXQ_START);

						/* Start asynchronous FIFO read operation. */
						g_spi_pdc_flag = SPI_PDC_RX_START;
						gpio_set_pin_low(KSZ8851SNL_CSN_GPIO);
						ksz8851_fifo_read(ps_ksz8851snl_dev->rx_pbuf[ps_ksz8851snl_dev->us_rx_head]->payload, len);

						/* Remove CRC and update pbuf length. */
						len -= 4;
						ps_ksz8851snl_dev->rx_pbuf[ps_ksz8851snl_dev->us_rx_head]->len = len;
						ps_ksz8851snl_dev->rx_pbuf[ps_ksz8851snl_dev->us_rx_head]->tot_len = len;
					}
				}
			}
		}
		/* Handle TX. */
		else {
			uint16_t txmir;
			uint16_t isr;

			/* Check for packet ready to be sent. */
			if (ps_ksz8851snl_dev->us_tx_tail != ps_ksz8851snl_dev->us_tx_head) {

				len = ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail]->tot_len;

				/* Check if TXQ memory size is available for this transmit packet */
				txmir = ksz8851_reg_read(REG_TX_MEM_INFO) & TX_MEM_AVAILABLE_MASK;
				//printf("us_tx_tail=%d size available=%d, required=%d\n", ps_ksz8851snl_dev->us_tx_tail, txmir, len+4);
				if (txmir < len + 4) {
					/* Not enough space to send packet. */

					/* Enable TXQ memory available monitor */
					ksz8851_reg_write(REG_TX_TOTAL_FRAME_SIZE, len + 4);

					//spi_setbits(REG_TXQ_CMD, TXQ_MEM_AVAILABLE_INT);

					/* When the isr register has the TXSAIS bit set, there's
					* enough room for the packet.
					*/
					do {
						isr = ksz8851_reg_read(REG_INT_STATUS);
					} while (!(isr & INT_TX_SPACE));

					/* Disable TXQ memory available monitor */
					ksz8851_reg_clrbits(REG_TXQ_CMD, TXQ_MEM_AVAILABLE_INT);

					/* Clear the flag */
					isr &= ~INT_TX_SPACE;
					ksz8851_reg_write(REG_INT_STATUS, isr);
				}

				//printf("ksz8851snl_update: start packet transmit [len=%d]\n", len);

				/* Enable TXQ write access */
				ksz8851_reg_setbits(REG_RXQ_CMD, RXQ_START);

				/* Start FIFO write operation. */
				g_spi_pdc_flag = SPI_PDC_TX_START;
				ps_ksz8851snl_dev->tx_cur_pbuf = ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail];
				gpio_set_pin_low(KSZ8851SNL_CSN_GPIO);
				ksz8851_fifo_write(ps_ksz8851snl_dev->tx_cur_pbuf->payload, ps_ksz8851snl_dev->tx_cur_pbuf->len);
			}
		}
	}
	else if (SPI_PDC_RX_COMPLETE == g_spi_pdc_flag) {
		/* Pad with dummy data to keep dword alignment. */
		len = ps_ksz8851snl_dev->rx_pbuf[ps_ksz8851snl_dev->us_rx_head]->tot_len & 3;
		if (len) {
			//printf("ksz8851snl_update: padding %d bytes\n", len);
			ksz8851_fifo_dummy(len);
		}

		/* End RX transfer. */
		gpio_set_pin_high(KSZ8851SNL_CSN_GPIO);

		/* Disable asynchronous transfer mode. */
		g_spi_pdc_flag = SPI_PDC_IDLE;

		/* End RXQ read access. */
		ksz8851_reg_clrbits(REG_RXQ_CMD, RXQ_START);

		/* Mark descriptor ready to be read. */
		ps_ksz8851snl_dev->rx_desc[ps_ksz8851snl_dev->us_rx_head] = 1;
		ps_ksz8851snl_dev->us_rx_head = (ps_ksz8851snl_dev->us_rx_head + 1) % NETIF_RX_BUFFERS;
	}
	else if (SPI_PDC_TX_COMPLETE == g_spi_pdc_flag) {
		/* Pad with dummy data to keep dword alignment. */
		len = ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail]->tot_len & 3;
		if (len) {
			//printf("ksz8851snl_update: padding %d bytes\n", len);
			ksz8851_fifo_dummy(len);
		}

		/* End TX transfer. */
		gpio_set_pin_high(KSZ8851SNL_CSN_GPIO);

		/* Disable asynchronous transfer mode. */
		g_spi_pdc_flag = SPI_PDC_IDLE;

		/* Disable TXQ write access. */
		ksz8851_reg_clrbits(REG_RXQ_CMD, RXQ_START);

		/* Issue transmit command to the TXQ. */
		ksz8851_reg_setbits(REG_TXQ_CMD, TXQ_ENQUEUE);

		/* Wait until transmit command clears. */
		while (ksz8851_reg_read(REG_TXQ_CMD) & TXQ_ENQUEUE)
			;

		/* Mark packet ready to be freed. */
	//	ps_ksz8851snl_dev->tx_desc[ps_ksz8851snl_dev->us_tx_tail] = 1;





			/* Buffer sent, free the corresponding buffer if any. */
			pbuf_free(ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail]);
			ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail] = NULL;


		//printf("ksz8851snl_update: freeing... us_tx_tail=%d\r\n", ps_ksz8851snl_dev->us_tx_tail);
		ps_ksz8851snl_dev->us_tx_tail = (ps_ksz8851snl_dev->us_tx_tail + 1) % NETIF_TX_BUFFERS;









//		ps_ksz8851snl_dev->us_tx_tail = (ps_ksz8851snl_dev->us_tx_tail + 1) % NETIF_TX_BUFFERS;
	}
}

/**
 * \brief Populate the RX descriptor ring buffers with pbufs.
 *
 * \param p_ksz8851snl_dev Pointer to driver data structure.
 */
static void ksz8851snl_rx_populate_queue(struct ksz8851snl_device *p_ksz8851snl_dev)
{
	uint32_t ul_index = 0;
	struct pbuf *p = 0;

	/* Set up the RX descriptors */
	for (ul_index = 0; ul_index < NETIF_RX_BUFFERS; ul_index++) {
		if (p_ksz8851snl_dev->rx_pbuf[ul_index] == 0) {

			/* Allocate a new pbuf with the maximum size. */
			p = pbuf_alloc(PBUF_RAW, (u16_t) NET_MTU, PBUF_POOL);
			if (p == NULL) {
				LWIP_DEBUGF(NETIF_DEBUG, ("ksz8851snl_rx_populate_queue: pbuf allocation failure\n"));
				break;
			}

			/* Make sure LwIP is well configured so one pbuf can contain the maximum packet size. */
			LWIP_ASSERT("ksz8851snl_rx_populate_queue: pbuf size too small!", pbuf_clen(p) <= 1);

			/* Reset status value. */
			p_ksz8851snl_dev->rx_desc[ul_index] = 0;

			/* Save pbuf pointer to be sent to LwIP upper layer. */
			p_ksz8851snl_dev->rx_pbuf[ul_index] = p;

			LWIP_DEBUGF(NETIF_DEBUG,
					("ksz8851snl_rx_populate_queue: new pbuf allocated: %p [idx=%u]\n",
					p, ul_index));
		}
	}
}

/**
 * \brief Set up the RX descriptor ring buffers.
 *
 * This function sets up the descriptor list used for receive packets.
 *
 * \param ps_ksz8851snl_dev Pointer to driver data structure.
 */
static void ksz8851snl_rx_init(struct ksz8851snl_device *ps_ksz8851snl_dev)
{
	uint32_t ul_index = 0;

	/* Init pointer index. */
	ps_ksz8851snl_dev->us_rx_head = 0;
	ps_ksz8851snl_dev->us_rx_tail = 0;

	/* Set up the RX descriptors. */
	for (ul_index = 0; ul_index < NETIF_RX_BUFFERS; ul_index++) {
		ps_ksz8851snl_dev->rx_pbuf[ul_index] = 0;
		ps_ksz8851snl_dev->rx_desc[ul_index] = 0;
	}

	/* Build RX buffer and descriptors. */
	ksz8851snl_rx_populate_queue(ps_ksz8851snl_dev);
}

/**
 * \brief Set up the TX descriptor ring buffers.
 *
 * This function sets up the descriptor list used for receive packets.
 *
 * \param ps_ksz8851snl_dev Pointer to driver data structure.
 */
static void ksz8851snl_tx_init(struct ksz8851snl_device *ps_ksz8851snl_dev)
{
	uint32_t ul_index = 0;

	/* Init TX index pointer. */
	ps_ksz8851snl_dev->us_tx_head = 0;
	ps_ksz8851snl_dev->us_tx_tail = 0;

	/* Set up the TX descriptors */
	for (ul_index = 0; ul_index < NETIF_TX_BUFFERS; ul_index++) {
		ps_ksz8851snl_dev->tx_desc[ul_index] = 0;
	}
}

/**
 * \brief Free TX buffers that have successfully been sent.
 *
 * \param ps_ksz8851snl_dev Pointer to driver data structure.
 */
static void ksz8851snl_tx_reclaim(struct ksz8851snl_device *ps_ksz8851snl_dev)
{
	////printf("gmac_tx_reclaim: cur us_tx_tail=%d\r\n", ps_ksz8851snl_dev->us_tx_tail);

	while (ps_ksz8851snl_dev->us_tx_tail != ps_ksz8851snl_dev->us_tx_head) {
		if (ps_ksz8851snl_dev->tx_desc[ps_ksz8851snl_dev->us_tx_tail]) {
//printf("gmac_tx_reclaim: Freeing tx_pbuf[%d]=%p\n", ps_ksz8851snl_dev->us_tx_tail, ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail]);
			/* Buffer sent, free the corresponding buffer if any. */
			LWIP_DEBUGF(LWIP_DBG_TRACE,
				("gmac_tx_reclaim: Freeing tx_pbuf[%d]=%p\n",
				ps_ksz8851snl_dev->us_tx_tail,
				ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail]));
			pbuf_free(ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail]);
			ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_tail] = NULL;
		}
		else {
			break;
		}

		////printf("gmac_tx_reclaim: freeing... us_tx_tail=%d\r\n", ps_ksz8851snl_dev->us_tx_tail);
		ps_ksz8851snl_dev->us_tx_tail = (ps_ksz8851snl_dev->us_tx_tail + 1) % NETIF_TX_BUFFERS;
	}

////printf("gmac_tx_reclaim: done!\r\n");
}

/**
 * \brief Initialize ksz8851snl and PHY.
 *
 * \note Called from ethernetif_init().
 *
 * \param netif the already initialized lwip network interface structure
 *        for this ethernetif.
 */
static void ksz8851snl_low_level_init(struct netif *netif)
{
	volatile uint32_t ul_delay;

	/* Set MAC hardware address length. */
	netif->hwaddr_len = sizeof(gs_uc_mac_address);
	/* Set MAC hardware address. */
	netif->hwaddr[0] = gs_uc_mac_address[0];
	netif->hwaddr[1] = gs_uc_mac_address[1];
	netif->hwaddr[2] = gs_uc_mac_address[2];
	netif->hwaddr[3] = gs_uc_mac_address[3];
	netif->hwaddr[4] = gs_uc_mac_address[4];
	netif->hwaddr[5] = gs_uc_mac_address[5];

	/* Set maximum transfer unit. */
	netif->mtu = NET_MTU;

	/* Device capabilities. */
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP
#if defined(DHCP_USED)
			| NETIF_FLAG_DHCP
#endif
	;

	ksz8851snl_rx_init(&gs_ksz8851snl_dev);
	ksz8851snl_tx_init(&gs_ksz8851snl_dev);

	/* Enable NVIC SPI interrupt. */
	NVIC_SetPriority(SPI_IRQn, INT_PRIORITY_SPI);
	NVIC_EnableIRQ(SPI_IRQn);

	/* Initialize SPI link. */
	ksz8851snl_init();

	/* Initialize interrupt line INTN. */
	configure_intn(INTN_Handler);
}

/**
 * \brief This function should do the actual transmission of the packet. The
 * packet is contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * \param netif the lwip network interface structure for this ethernetif.
 * \param p the MAC packet to send (e.g. IP packet including MAC addresses and type).
 *
 * \return ERR_OK if the packet could be sent.
 *         an err_t value if the packet couldn't be sent.
 */
static err_t ksz8851snl_low_level_output(struct netif *netif, struct pbuf *p)
{
	struct ksz8851snl_device *ps_ksz8851snl_dev = netif->state;
	struct pbuf *q = NULL;

	uint32_t ul_index = 0;

	/* Make sure a descriptor is free. */
	/*while (((ps_ksz8851snl_dev->us_tx_head + 1) % NETIF_TX_BUFFERS) ==
			ps_ksz8851snl_dev->us_tx_tail)
		ksz8851snl_tx_reclaim(ps_ksz8851snl_dev);
*/
	/* Ensure LwIP won't free this pbuf before GMAC actually sends it. */
	pbuf_ref(p);

	/* Enqueue pbuf packet. */
	ps_ksz8851snl_dev->tx_desc[ps_ksz8851snl_dev->us_tx_head] = 0;
	ps_ksz8851snl_dev->tx_pbuf[ps_ksz8851snl_dev->us_tx_head] = p;

	//printf("ksz8851snl_low_level_output: pbuf to send, chain#=%d,"" size=%d (head=%d tail=%d)\r\n", pbuf_clen(p), p->tot_len, ps_ksz8851snl_dev->us_tx_head, ps_ksz8851snl_dev->us_tx_tail);

	LWIP_DEBUGF(LWIP_DBG_TRACE,
			("gmac_low_level_output: DMA buffer %p sent [clen=%d size=%d us_tx_head=%d]\n",
			q->payload, clen, q->len, ps_ksz8851snl_dev->us_tx_head));

	ps_ksz8851snl_dev->us_tx_head = (ps_ksz8851snl_dev->us_tx_head + 1) % NETIF_TX_BUFFERS;

#if LWIP_STATS
	lwip_tx_count += p->tot_len;
#endif
	LINK_STATS_INC(link.xmit);

	/* Free pbufs that have been sent by GMAC. */
//	ksz8851snl_tx_reclaim(ps_ksz8851snl_dev);

	return ERR_OK;
}

/**
 * \brief Use pre-allocated pbuf as DMA source and return the incoming packet.
 *
 * \param netif the lwip network interface structure for this ethernetif.
 *
 * \return a pbuf filled with the received packet (including MAC header).
 *         0 on memory error.
 */
static struct pbuf *ksz8851snl_low_level_input(struct netif *netif)
{
	struct ksz8851snl_device *ps_ksz8851snl_dev = netif->state;
	struct pbuf *p = 0;

	/* Check that a packet has been received. */
	if (ps_ksz8851snl_dev->rx_desc[ps_ksz8851snl_dev->us_rx_tail]) {

		/* Fetch pre-allocated pbuf. */
		p = ps_ksz8851snl_dev->rx_pbuf[ps_ksz8851snl_dev->us_rx_tail];

		/* Remove this pbuf from its descriptor. */
		ps_ksz8851snl_dev->rx_pbuf[ps_ksz8851snl_dev->us_rx_tail] = 0;

		//printf("ksz8851snl_low_level_input: DMA buffer 0x%p received, size=%u [tail=%u head=%u]\n", p, p->tot_len, ps_ksz8851snl_dev->us_rx_tail, ps_ksz8851snl_dev->us_rx_head);

		LWIP_DEBUGF(NETIF_DEBUG,
				("ksz8851snl_low_level_input: DMA buffer 0x%p received, size=%u [tail=%u head=%u]\n",
				p, p->tot_len, ps_ksz8851snl_dev->us_rx_tail, ps_ksz8851snl_dev->us_rx_head));

		/* Set pbuf total packet size. */
		LINK_STATS_INC(link.recv);

		/* Fill empty descriptors with new pbufs. */
		ksz8851snl_rx_populate_queue(ps_ksz8851snl_dev);

		ps_ksz8851snl_dev->us_rx_tail = (ps_ksz8851snl_dev->us_rx_tail + 1) % NETIF_RX_BUFFERS;

#if LWIP_STATS
		lwip_rx_count += p->length;
#endif
	}

	return p;
}

#if NO_SYS == 0
/**
 * \brief ksz8851snl task function. This function waits for the notification
 * semaphore from the interrupt, processes the incoming packet and then
 * passes it to the LwIP stack.
 *
 * \param pvParameters A pointer to the ksz8851snl_device instance.
 */
static void ksz8851snl_task(void *pvParameters)
{
	struct ksz8851snl_device *ps_ksz8851snl_dev = pvParameters;

	while (1) {
		/* Wait for the RX notification semaphore. */
		sys_arch_sem_wait(&ps_ksz8851snl_dev->rx_sem, 0);

		/* Process the incoming packet. */
		ethernetif_input(ps_ksz8851snl_dev->netif);
	}
}
#endif

/**
 * \brief This function should be called when a packet is ready to be
 * read from the interface. It uses the function ksz8851snl_low_level_input()
 * that handles the actual reception of bytes from the network interface.
 * Then the type of the received packet is determined and the appropriate
 * input function is called.
 *
 * \param pv_parameters the LwIP network interface structure for this
 * ethernetif.
 */
void ethernetif_input(struct netif *netif)
{
	struct eth_hdr *ethhdr;
	struct pbuf *p;

	/* Update driver state machine. */
	ksz8851snl_update(netif);

	/* Move received packet into a new pbuf. */
	p = ksz8851snl_low_level_input(netif);
	if (p == NULL)
		return;

	/* Points to packet payload, which starts with an Ethernet header. */
	ethhdr = p->payload;

	switch (htons(ethhdr->type)) {
		case ETHTYPE_IP:
		case ETHTYPE_ARP:
#if PPPOE_SUPPORT
		case ETHTYPE_PPPOEDISC:
		case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
			/* Send packet to LwIP for processing. */
			if (netif->input(p, netif) != ERR_OK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
				/* Free buffer. */
				pbuf_free(p);
			}
			break;

		default:
			/* Free buffer. */
			pbuf_free(p);
			break;
	}
}

/**
 * \brief Should be called at the beginning of the program to set up the
 * network interface. It calls the function ksz8851snl_low_level_init() to do the
 * actual setup of the hardware.
 *
 * \param netif the LwIP network interface structure for this ethernetif.
 *
 * \return ERR_OK if the loopif is initialized.
 *         ERR_MEM if private data couldn't be allocated.
 *         any other err_t on error.
 */
err_t ethernetif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

	gs_ksz8851snl_dev.netif = netif;

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "ksz8851snldev";
#endif /* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
#if LWIP_SNMP
	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, NET_LINK_SPEED);
#endif /* LWIP_SNMP */

	netif->state = &gs_ksz8851snl_dev;
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
	netif->linkoutput = ksz8851snl_low_level_output;
	/* Initialize the hardware */
	ksz8851snl_low_level_init(netif);

#if NO_SYS == 0
	err_t err;
	sys_thread_t id;

	/* Incoming packet notification semaphore. */
	err = sys_sem_new(&gs_ksz8851snl_dev.rx_sem, 0);
	LWIP_ASSERT("ethernetif_init: ksz8851snl RX semaphore allocation ERROR!\n",
			(err == ERR_OK));
	if (err == ERR_MEM)
		return ERR_MEM;

	id = sys_thread_new("ksz8851snl", ksz8851snl_task, &gs_ksz8851snl_dev,
			netifINTERFACE_TASK_STACK_SIZE, netifINTERFACE_TASK_PRIORITY);
	LWIP_ASSERT("ethernetif_init: ksz8851snl Task allocation ERROR!\n",
			(id != 0));
	if (id == 0)
		return ERR_MEM;
#endif

	return ERR_OK;
}
