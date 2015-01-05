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

#ifndef PROPPLE_SOCKET_H
#define PROPPLE_SOCKET_H

#include "udp-socket.h"

struct propple_socket;

typedef void (* propple_socket_input_callback_t)(struct propple_socket *c,
                                                 void *ptr,
                                                 uint8_t seqno,
                                                 const uint8_t *data,
                                                 uint16_t datalen);
#define PROPPLE_SOCKET_MAXDATALEN 120

#define PROPPLE_SOCKET_DEFAULT_TIMEOUT (CLOCK_SECOND * 10)

struct propple_socket {
  struct udp_socket s;
  uint16_t port;
  propple_socket_input_callback_t callback;
  void *ptr;
  struct ctimer t, interval_timer, first_transmission_timer;
  struct pt pt;
  uint32_t start_interval;
  uint8_t max_interval_scaling;
  uint8_t dupthreshold;
  uint8_t seqno;
  uint8_t interval_scaling;
  uint8_t duplicates;
  uint16_t datalen;
  uint8_t data[PROPPLE_SOCKET_MAXDATALEN];
};

/**
 * \brief      Open a propple socket on a specific UDP port
 * \param s    A pointer to a propple_socket struct
 * \param ptr  An opaque pointer that is passed to the callback
 * \param callback The receiver callback function
 * \param port The UDP port number, in host byte order
 * \return     Less than zero on error
 *
 *             This function opens a propple socket and starts to
 *             retransmit propple messages from others.
 */
int propple_socket_open(struct propple_socket *s,
                        void *ptr,
                        propple_socket_input_callback_t callback,
                        uint16_t port);

/**
 * \brief      Start sending a propple message on a propple socket
 * \param s    A pointer to a propple_socket struct that has previously been opened with propple_socket_open()
 * \param data The data to send
 * \param datalen The length of the data to send, which which be cropped to be no longer than PROPLE_SOCKET_MAXDATALEN
 * \param start_interval The initial (minimum) propple interval, in milliseconds
 * \param max_interval_scaling The maximum number of interval doublings
 * \param dupthreshold The number of duplicates needed to suppress a retransmission
 *
 *             This function starts sending a propple message to the
 *             network. Only one sender should sent propple messages
 *             on a given port in a single network, to avoid
 *             ambiguity.
 */
int propple_socket_send(struct propple_socket *s,
                        const uint8_t *data,
                        uint16_t datalen,
                        uint32_t start_interval,
                        uint8_t max_interval_scaling,
                        uint8_t dupthreshold);

#endif /* PROPPLE_SOCKET_H */
