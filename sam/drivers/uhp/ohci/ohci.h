/**
 * \file
 *
 * \brief OHCI header file and APIs for UHD interface.
 *
 * Copyright (C) 2014 Atmel Corporation. All rights reserved.
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

#include "compiler.h"
#include "uhp.h"

#define BUS_CONNECT     0
#define BUS_DISCONNECT  1
#define BUS_RESET       2

enum ohci_interrupt_source {
	OHCI_INTERRUPT_SO = 0,
	OHCI_INTERRUPT_WDH,
	OHCI_INTERRUPT_SF,
	OHCI_INTERRUPT_RD,
	OHCI_INTERRUPT_UE,
	OHCI_INTERRUPT_FNO,
	OHCI_INTERRUPT_RHSC,
	OHCI_INTERRUPT_OC,
	OHCI_NUM_OF_INTERRUPT_SOURCE,
};

typedef void (*ohci_callback_t)(void *pointer);

/*
 * OHCI Endpoint Descriptor (ED) ... holds TD queue
 * See OHCI spec, section 4.2
 */
typedef union {
	struct {
		uint32_t  bFunctionAddress:7;    //!< Device address on the bus
		uint32_t  bEndpointNumber:4;     //!< Endpoint number
		uint32_t  bDirection:2;          //!< Endpoint direction
		uint32_t  bSpeed:1;              //!< Full- or low-speed
		uint32_t  bSkip:1;               //!< Process TDs on this endpoint ?
		uint32_t  bFormat:1;             //!< TD format
		uint32_t  bMaximumPacketSize:11; //!< Maximum packet size
		uint32_t  bReserved:5;
	} ed_info_s;
	uint32_t ul_ed_info;
} ed_info_t;

struct ohci_ed {
	ed_info_t       ed_info;
    void           *p_td_tail;                //!< Pointer to the tail TD
    void           *p_td_head;                //!< Pointer to the head TD
    struct ohci_ed  *p_next_ed;               //!< Pointer to next ED
} __attribute__ ((aligned(16)));


enum pid {
	TD_PID_SETUP = 0,
	TD_PID_OUT   = 1,
	TD_PID_IN    = 2,
};
//! \brief  Transfer descriptor directions
#define TD_DIRECTION_SETUP              0
#define TD_DIRECTION_OUT                1
#define TD_DIRECTION_IN                 2

//! \brief  Transfer descriptor data toggle values
#define TD_TOGGLE_AUTO                  0
#define TD_TOGGLE_DATA0                 2
#define TD_TOGGLE_DATA1                 3

//! \brief  Condition code field values
// Completion Codes
// General TD or isochronous data packet processing completed with 
// no detected errors
#define TD_CONDITIONCODE_NOERROR            0x0 // 0000
// Last data packet from endpoint contained a CRC error.
#define TD_CONDITIONCODE_CRC                0x1 // 0001
// Last data packet from endpoint contained a bit stuffing violation
#define TD_CONDITIONCODE_BITSTUFFING        0x2 // 0010
// Last packet from endpoint had data toggle PID that didnot match 
// the expected value.
#define TD_CONDITIONCODE_DATATOGGLEMISMATCH 0x3 // 0011
// STALL: TD was moved to the Done Queue because the endpoint returned 
// a STALL PID
#define TD_CONDITIONCODE_STALL              0x4 // 0100
// Device did not respond to token (IN) or did not provide a handshake (OUT)
#define TD_CONDITIONCODE_NOTRESPONDING      0x5 // 0101
// Check bits on PID from endpoint failed on data PID (IN) or handshake (OUT)
#define TD_CONDITIONCODE_PIDCHECKFAILURE    0x6 // 0110
// Receive PID was not valid when encountered or PID value is not defined.
#define TD_CONDITIONCODE_UNEXPECTEDPID      0x7 // 0111
// The amount of data returned by the endpoint exceeded either the size of 
// the maximum data packet allowed from the endpoint (found in 
// MaximumPacketSize field of ED) or the remaining buffer size.
#define TD_CONDITIONCODE_DATAOVERRUN        0x8 // 1000
// The endpoint returned less than MaximumPacketSize and that amount was not
// sufficient to fill the specified buffer
#define TD_CONDITIONCODE_DATAUNDERRUN       0x9 // 1001
// Reserved                                 0xA // 1010
// Reserved                                 0xB // 1011
// During an IN, HC received data from endpoint faster than it could be written
// to system memory
#define TD_CONDITIONCODE_BUFFEROVERRUN      0xC // 1100
// During an OUT, HC could not retrieve data from system memory fast enough 
// to keep up with data USB data rate.
#define TD_CONDITIONCODE_BUFFERUNDERRUN     0xD // 1101
// This code is set by software before the TD is placed on a list to be 
// processed by the HC.
#define TD_CONDITIONCODE_NOTACCESSED        0xE // 111x

/*
 * OHCI Transfer Descriptor (TD) ... one per transfer segment
 * See OHCI spec, sections 4.3.1 (general = control/bulk/interrupt)
 * and 4.3.2 (iso)
 */
struct ohci_td_general {
	struct {
	  uint32_t  bReserved1:18;
	  uint32_t  bBufferRounding:1;      //!< Last data packet size policy
	  uint32_t  bDirectionPID:2;        //!< Packet PID
	  uint32_t  bDelayInterrupt:3;      //!< Interrupt delay count
	  uint32_t  bDataToggle:2;          //!< Data toggle value
	  uint32_t  bErrorCount:2;          //!< Transmission error count
	  uint32_t  bConditionCode:4;       //!< Status of last transaction
	} td_info;
	void           *pCurrentBufferPointer; //!< Pointer to the data buffer
	struct ohci_td_general *p_next_td;               //!< Pointer to next TD
	void           *pBufferEnd;            //!< Address of data buffer last byte
}__attribute__ ((aligned(16)));

struct ohci_td_iso {
	struct {
	  uint32_t  bStartingFrame:16;      //!< Start frame number
	  uint32_t  bReserved1:5;
	  uint32_t  bDelayInterrupt:3;      //!< Interrupt delay count
	  uint32_t  FrameCount:3;          //!< Number of data packets (frames)
	  uint32_t  bReserved2:1;
	  uint32_t  bConditionCode:4;       //!< Status of last transaction
	} td_info;
	void           *pBufferPage0;       //!< Pointer to the data buffer
	struct ohci_td_iso *p_next_td;               //!< Pointer to next TD
	void           *pBufferEnd;            //!< Address of data buffer last byte
	uint16_t    offset_psw[8];            //!< Offset, packet status word
}__attribute__ ((aligned(32)));

/*
 * The HCCA (Host Controller Communications Area) is a 256 byte
 * structure defined section 4.4.1 of the OHCI spec. The HC is
 * told the base address of it.  It must be 256-byte aligned.
 */
struct ohci_hcca {
    uint32_t InterruptTable[32];
    uint16_t FrameNumber;
    uint16_t Pad1;
    void         *pDoneHead;
    uint32_t Reserved[29];
	uint32_t what;		/* spec only identifies 252 bytes :) */
} __attribute__ ((aligned(256)));;

#define	FI			0x2edf		/* 12000 bits per frame (-1) */
#define	FSMP(fi)		(0x7fff & ((6 * ((fi) - 210)) / 7))
#define	FIT			(1 << 31)
#define LSTHRESH		0x628		/* lowspeed bit threshold */

void ohci_init(void);
void ohci_deinit(void);
uint32_t ohci_get_device_speed (void);
uint16_t ohci_get_frame_number (void);
void ohci_bus_reset(void);
void ohci_bus_suspend(void);
uint32_t ohci_is_suspend(void);
void ohci_bus_resume(void);
bool ohci_add_ed_control(ed_info_t * ed_info);
bool ohci_add_ed_bulk(ed_info_t * ed_info);
bool ohci_add_ed_period(ed_info_t * ed_info);
void ohci_remove_ed(uint8_t ep_number);
bool ohci_add_td_control(enum pid pid, uint8_t *buf, uint16_t buf_size);
bool ohci_add_td_non_control(uint8_t ep_number, uint8_t *buf, uint32_t buf_size);

void ohci_remove_td(uint8_t ep_number);
void ohci_enable_interrupt(enum ohci_interrupt_source int_source);
void ohci_disable_interrupt(enum ohci_interrupt_source int_source);
void ohci_register_callback(enum ohci_interrupt_source int_source, void *call_back);
void ohci_unregister_callback(enum ohci_interrupt_source int_source);

