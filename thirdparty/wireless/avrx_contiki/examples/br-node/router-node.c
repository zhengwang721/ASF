/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the WSN Demo Application Application
 * The WSNDemo application implements a typical wireless sensor network
 *scenario,
 * in which one central node collects the data from a network of sensors and
 *passes this data over a serial connection for further processing.
 * In the case of the WSNDemo this processing is performed by the WSNMonitor PC
 *application. The BitCloud® Quick Start Guide  provides a detailed description
 *of the WSNDemo application scenario, and instructions on how to use
 *WSNMonitor.
 *  However since BitCloud is a ZigBee® PRO stack, there are a few differences
 *in the protocol:
 * • Device types (Coordinator, Router and End Device) are simulated on the
 *application level; there is no such separation in Lightweight Mesh on the
 *stack level
 * • The value of the extended address field is set equal to the value of the
 *short address field
 * • For all frames, the LQI and RSSI fields are filled in by the coordinator
 *with the values of LQI and RSSI from the received frame. This means that nodes
 *that are not connected to the coordinator directly will have the same values
 *as the last node on the route to the coordinator
 * • Sensor data values are generated randomly on all platforms
 * • Sending data to the nodes on the network is not implemented and not
 *supported in this demo application
 */


#include "thsq.h"
#include "ip64.h"
#if (ENABLE_WEBSERVER == 1)
#include "ip64-webserver.h"
#endif
/*---------------------------------------------------------------------------*/
PROCESS(router_node_process, "Router node");
AUTOSTART_PROCESSES(&router_node_process
#if (ENABLE_WEBSERVER == 1)
,&blinker_process
#endif
);
#if (ENABLE_WEBSERVER == 1)
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blinker_process, ev, data)
{
	static struct etimer et;
	static uint8_t red, green;
	PROCESS_BEGIN();

	etimer_set(&et, CLOCK_SECOND / 2);
	while(1) {
		PROCESS_WAIT_UNTIL(etimer_expired(&et));
		etimer_reset(&et);
		if(0) {
			leds_on(LEDS_RED);
			red = 1;
			} else {
			red = 0;
		}
		if(!ip64_hostaddr_is_configured()) {
			leds_on(LEDS_GREEN);
			green = 1;
			} else {
			green = 0;
		}
		PROCESS_WAIT_UNTIL(etimer_expired(&et));
		etimer_reset(&et);
		if(red) {
			leds_off(LEDS_RED);
		}
		if(green) {
			leds_off(LEDS_GREEN);
		}
	}
	PROCESS_END();
}
#endif
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(router_node_process, ev, data)
{
	PROCESS_BEGIN();

	/* Set us up as a RPL root node. */
	simple_rpl_init_dag();

	/* Initialize the IP64 module so we'll start translating packets */
	ip64_init();
#if (ENABLE_WEBSERVER == 1)
	/* Initialize the IP64 webserver */
	ip64_webserver_init();
#endif
	NETSTACK_RDC.off(1);

	/* ... and do nothing more. */
	while(1) {
		PROCESS_WAIT_EVENT();
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
