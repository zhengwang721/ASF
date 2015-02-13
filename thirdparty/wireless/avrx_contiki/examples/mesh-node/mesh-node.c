#include "thsq.h"

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

#include "net/ip/uip-debug.h"
#if (MESH_NODE == 0)
/*---------------------------------------------------------------------------*/
PROCESS(mesh_node_process, "Mesh node");
AUTOSTART_PROCESSES(&mesh_node_process);
/*---------------------------------------------------------------------------*/
static void
route_callback(int event, uip_ipaddr_t *route, uip_ipaddr_t *ipaddr,
               int numroutes)
{
    leds_off(LEDS_ALL);
    printf("Got a RPL route event: %d; Route : ");
	uip_debug_ipaddr_print(route);
	printf("; Ipaddr:");
	uip_debug_ipaddr_print(ipaddr);
	printf("; Num of routes: %d\r\n");
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(mesh_node_process, ev, data)
{
  static struct uip_ds6_notification n;
  PROCESS_BEGIN();

  leds_on(LEDS_ALL);
  uip_ds6_notification_add(&n, route_callback);

  while(1) {
    PROCESS_WAIT_EVENT();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
PROCESS(mesh_root_process, "Mesh root");
AUTOSTART_PROCESSES(&mesh_root_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(mesh_root_process, ev, data)
{
  PROCESS_BEGIN();

  /* Set us up as a RPL root root. */
  simple_rpl_init_dag();

  while(1) {
    PROCESS_WAIT_EVENT();
  }

  PROCESS_END();
}
#endif