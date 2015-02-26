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
#include "propple-socket.h"


struct propple_hdr {
  uint8_t datalen[2];
  uint8_t seqno;
  uint8_t dupthreshold;
  uint8_t start_interval[3];
  uint8_t max_interval_scaling;
  uint8_t sender[16];
};

struct propple_msg {
  struct propple_hdr h;
  uint8_t data[PROPPLE_SOCKET_MAXDATALEN];
};

static int run_trickle(struct propple_socket *s);

#define LOLLIPOP_INIT_VALUE          0
#define LOLLIPOP_MAX_VALUE           255
#define LOLLIPOP_RESTART_VALUE       128

#define LOLLIPOP_INCREMENT(counter)             \
   do {                                         \
     if((counter) == LOLLIPOP_MAX_VALUE) {      \
       (counter) = LOLLIPOP_RESTART_VALUE;      \
     } else {                                   \
       (counter)++;                             \
     }                                          \
   } while(0)

/*---------------------------------------------------------------------------*/
static int
seqno_lollipop_lt(uint8_t a, uint8_t b)
{
  if(a < LOLLIPOP_RESTART_VALUE && b >= LOLLIPOP_RESTART_VALUE) {
    return 1;
  }
  if(a >= LOLLIPOP_RESTART_VALUE && b <= LOLLIPOP_RESTART_VALUE) {
    return 0;
  }
  if(a < LOLLIPOP_RESTART_VALUE && b < LOLLIPOP_RESTART_VALUE) {
    return a < b;
  }
  if(a >= LOLLIPOP_RESTART_VALUE && b >= LOLLIPOP_RESTART_VALUE) {
    if(b > a) {
      return (b - a) < (LOLLIPOP_MAX_VALUE - LOLLIPOP_RESTART_VALUE) / 2;
    } else {
      return (a - b) > (LOLLIPOP_MAX_VALUE - LOLLIPOP_RESTART_VALUE) / 2;
    }
  }

  /* Should not be reached. */
  return 0;
}
/*---------------------------------------------------------------------------*/
static void
send(void *ptr)
{
  struct propple_socket *s = ptr;
  struct propple_msg m;
  uip_ipaddr_t addr;
  const uip_ipaddr_t *senderaddr;

  m.h.datalen[0] = s->datalen >> 8;
  m.h.datalen[1] = s->datalen & 0xff;
  m.h.seqno = s->seqno;
  m.h.dupthreshold = s->dupthreshold;
  m.h.start_interval[0] = (s->start_interval >> 16) & 0xff;
  m.h.start_interval[1] = (s->start_interval >>  8) & 0xff;
  m.h.start_interval[2] = (s->start_interval >>  0) & 0xff;
  m.h.max_interval_scaling = s->max_interval_scaling;
  memcpy(m.data, s->data, s->datalen);

  senderaddr = simple_rpl_global_address();
  if(senderaddr == NULL) {
    memset(m.h.sender, 0, sizeof(m.h.sender));
  } else {
    memcpy(m.h.sender, senderaddr, sizeof(m.h.sender));
  }
  uip_create_linklocal_allnodes_mcast(&addr);

  /*  printf("propple-socket: send sending\n");*/
  if(udp_socket_sendto(&s->s,
                       &m, sizeof(struct propple_hdr) + s->datalen,
                       &addr, s->port) == -1) {
    /*    printf("propple-socket: send: failed\n");*/
  } else {
    /*    printf("propple-socket: send: sending %d bytes\n",
          sizeof(struct propple_hdr) + s->datalen);*/
  }
}
/*---------------------------------------------------------------------------*/
static void
timer_callback(void *ptr)
{
  struct propple_socket *s = ptr;
  run_trickle(s);
}
/*---------------------------------------------------------------------------*/
static void
reset_interval(struct propple_socket *s)
{
  PT_INIT(&s->pt);
  run_trickle(s);
}
/*---------------------------------------------------------------------------*/
static void
set_timer(struct propple_socket *s,
          struct ctimer *t, clock_time_t i)
{
  ctimer_set(t, i, timer_callback, s);
}
/*---------------------------------------------------------------------------*/
static int
run_trickle(struct propple_socket *s)
{
  clock_time_t interval;
  uint32_t interval_ms;
  PT_BEGIN(&s->pt);

  while(1) {
    interval_ms = s->start_interval << s->interval_scaling;
    interval = (CLOCK_SECOND * interval_ms) / 1000;
    set_timer(s, &s->interval_timer, interval);
    set_timer(s, &s->t, interval / 2 + (random_rand() % (interval / 2)));

    s->duplicates = 0;
    PT_YIELD(&s->pt); /* Wait until listen timeout */
    if(s->duplicates < s->dupthreshold) {
      send(s);
    }
    interval = s->start_interval << s->interval_scaling;
    if(s->interval_scaling < s->max_interval_scaling &&
       (s->start_interval << (s->interval_scaling + 1)) >
       s->start_interval << s->interval_scaling) {
      s->interval_scaling++;
    }
    PT_YIELD(&s->pt); /* Wait until interval timer expired. */
  }

  PT_END(&s->pt);
}
/*---------------------------------------------------------------------------*/
static void
recv(struct udp_socket *c, void *ptr,
     const uip_ipaddr_t *source_addr, uint16_t source_port,
     const uip_ipaddr_t *dest_addr, uint16_t dest_port,
     const uint8_t *data, uint16_t datalen)
{
  struct propple_socket *s = ptr;
  struct propple_msg *m = (struct propple_msg *)data;
  struct propple_hdr h;

  memcpy((uint8_t *)&h, (uint8_t *)&m->h, sizeof(h));

  uint16_t seqno = h.seqno;

  /*  printf("propple-socket: recv seqno %d our %d data len %d\n",
	 seqno,
         s->seqno,
	 datalen);*/

  if(seqno == s->seqno) {
    ++s->duplicates;
  } else if(seqno_lollipop_lt(seqno, s->seqno)) {
    /*    printf("we have higher\n");*/
    /* If we have a higher sequence number, we send out our sequence
       number. */
    s->interval_scaling = 0;
    s->duplicates = 0;
    reset_interval(s);
  } else { /* hdr->seqno > c->seqno */
    /*    printf("we have lower\n");*/
    /* Sanity check datalen */
    uint16_t len = (h.datalen[0] << 8) + h.datalen[1];
    /*    printf("datalen %d len %d\n", datalen, len);*/
    if(len <= PROPPLE_SOCKET_MAXDATALEN) {
      s->seqno = seqno;
      /* Remember the incoming interval properties */
      s->start_interval =
        ((uint32_t)h.start_interval[0] << 16) +
        (h.start_interval[1] << 8) +
        h.start_interval[2];
      s->max_interval_scaling = h.max_interval_scaling;
      s->dupthreshold = h.dupthreshold;
      /* Remember the incoming data */
      memcpy(s->data, m->data, datalen);
      s->datalen = len;

      /* Reset scaling and duplicates. */
      s->interval_scaling = 0;
      s->duplicates = 0;
      reset_interval(s);

      if(s->callback != NULL) {
        s->callback(s, s->ptr, s->seqno,
                    s->data, s->datalen);
      }
      /*    c->cb->recv(c);*/
    }
  }
}
/*---------------------------------------------------------------------------*/
int
propple_socket_open(struct propple_socket *s,
                    void *ptr,
                    propple_socket_input_callback_t callback,
                    uint16_t port)
{
  s->ptr = ptr;
  s->callback = callback;
  s->port = port;
  s->seqno = LOLLIPOP_INIT_VALUE;
  s->duplicates = 0;
  s->interval_scaling = 0;

  /* Set a default, fallback interval */
  s->start_interval = PROPPLE_SOCKET_DEFAULT_TIMEOUT / CLOCK_SECOND * 1000;
  s->max_interval_scaling = 8;
  s->dupthreshold = 1;
  s->datalen = 0;

  /* Register UDP socket callback */
  if(udp_socket_register(&s->s, s, recv) == -1) {
    printf("propple_socket_open: udp_socket_register failed\n");
    return -1;
  }

  /* Bind UDP socket to local port */
  if(udp_socket_bind(&s->s, port) == -1) {
    printf("propple_socket_open: udp_socket_bind failed\n");
    return -1;
  }

  /* Connect UDP socket to remote port */
  if(udp_socket_connect(&s->s, NULL, port) == -1) {
    printf("propple_socket_open: udp_socket_connect failed\n");
    return -1;
  }

  reset_interval(s);
  return 1;
}
/*---------------------------------------------------------------------------*/
int
propple_socket_send(struct propple_socket *s,
                    const uint8_t *data,
                    uint16_t datalen,
                    uint32_t start_interval,
                    uint8_t max_interval_scaling,
                    uint8_t dupthreshold)

{
  if(datalen > PROPPLE_SOCKET_MAXDATALEN) {
    datalen = PROPPLE_SOCKET_MAXDATALEN;
  }
  memcpy(s->data, data, datalen);
  s->datalen = datalen;
  LOLLIPOP_INCREMENT(s->seqno);

  s->start_interval = start_interval;
  s->max_interval_scaling = max_interval_scaling;
  s->dupthreshold = dupthreshold;
  reset_interval(s);
  ctimer_set(&s->first_transmission_timer, 0, send, s);
  /*  printf("propple_socket_send seqno %d, datalen %d data '%s'\n", s->seqno,
      s->datalen, data);*/
  return datalen;
}
/*---------------------------------------------------------------------------*/

