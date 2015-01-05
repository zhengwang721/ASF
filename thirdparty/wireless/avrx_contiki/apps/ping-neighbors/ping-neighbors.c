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

#include "thsq.h"
#include "ping-neighbors.h"

#include "net/rpl/rpl.h"

#include "net/ip/uip-debug.h"


#define INTERPING_TIME CLOCK_SECOND * 2
#define NUM_PINGS      32

PROCESS(ping_neighbors_process, "Ping neighbors process");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ping_neighbors_process, ev, data)
{
  static struct etimer periodtimer, pingtimer;
  static int i;
  static int num_neighbors;
  static int pings;
  PROCESS_BEGIN();

  for(pings = 0; pings < NUM_PINGS; pings++) {
    num_neighbors = uip_ds6_nbr_num();
    for(i = 0; i < num_neighbors; i++) {
      uip_ds6_nbr_t *nbr;
      int j;

      for(j = 0, nbr = nbr_table_head(ds6_neighbors);
          j < i && nbr != NULL;
          j++, nbr = nbr_table_next(ds6_neighbors, nbr));

      etimer_set(&periodtimer, INTERPING_TIME);
      etimer_set(&pingtimer, random_rand() % (INTERPING_TIME));
      if(nbr != NULL) {
        static const uip_ipaddr_t *ipaddr;

        ipaddr = uip_ds6_nbr_get_ipaddr(nbr);
        if(ipaddr != NULL) {
          /*          printf("Sending ping %d to ", pings);
          uip_debug_ipaddr_print(ipaddr);
          printf("\n");*/
          PROCESS_WAIT_UNTIL(etimer_expired(&pingtimer));
          uip_icmp6_send(ipaddr, ICMP6_ECHO_REQUEST, 0, 0);

          PROCESS_WAIT_UNTIL(etimer_expired(&periodtimer));
        }
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
ping_neighbors_start(void)
{
  process_start(&ping_neighbors_process, NULL);
}
/*---------------------------------------------------------------------------*/
