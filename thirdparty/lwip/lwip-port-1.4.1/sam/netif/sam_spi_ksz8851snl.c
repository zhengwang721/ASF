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
#include "netif/sam_spi_ksz8851snl.h"
#include "sysclk.h"
#include "conf_eth.h"

/** Define those to better describe your network interface */
#define IFNAME0								'e'
#define IFNAME1								'n'

/** Maximum transfer unit */
#define NET_MTU								1500

/** Network link speed */
#define NET_LINK_SPEED						100000000

#if NO_SYS == 0
/* Interrupt priorities. (lowest value = highest priority) */
/* ISRs using FreeRTOS *FromISR APIs must have priorities below or equal to */
/* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY. */
#define INT_PRIORITY_MAC					12

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
//	ksz8851snl_rx_descriptor_t rx_desc[ksz8851snl_RX_BUFFERS];
	/** Pointer to Tx descriptor list (must be 8-byte aligned) */
//	ksz8851snl_tx_descriptor_t tx_desc[ksz8851snl_TX_BUFFERS];
	/** RX pbuf pointer list */
	struct pbuf *rx_pbuf[NETIF_RX_BUFFERS];
	/** TX pbuf pointer list */
	struct pbuf *tx_pbuf[NETIF_TX_BUFFERS];

	/** RX index for current processing TD */
	uint32_t us_rx_idx;
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

	/* Initialize SPI link. */
	ksz8851snl_init();


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
	struct pbuf *p = 0;


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
