/**
 * \file
 *
 * \brief OHCI Host Controller Driver.
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

#include "ohci_hcd.h"
#include <string.h>

static struct ohci_hcca hcca;
struct ohci_ed control_ed;

static ohci_callback_t ohci_callback_pointer[OHCI_NUM_OF_INTERRUPT_SOURCE];
static uint32_t callback_para;
static uint32_t bus_reset_flag;

void ohci_init(void)
{
	uint32_t i, temp_value;

	for (i = 0; i < OHCI_NUM_OF_INTERRUPT_SOURCE; i++) {
		ohci_callback_pointer[i] = 0;
	}

	memset((void *)&hcca, 0, sizeof(hcca));
	memset((void *)&control_ed, 0, sizeof(control_ed));

	UHP->HcHCCA = &hcca;
	UHP->HcControlHeadED = &control_ed;

	// Setup Host Controller to issue a software reset
	UHP->HcCommandStatus |= HC_COMMANDSTATUS_HCR;
	while (UHP->HcCommandStatus & HC_COMMANDSTATUS_HCR);

	// Enable all interrupts
	UHP->HcInterruptEnable = HC_INTERRUPT_SO | HC_INTERRUPT_WDH | HC_INTERRUPT_SF
			| HC_INTERRUPT_RD | HC_INTERRUPT_UE
			| HC_INTERRUPT_FNO | HC_INTERRUPT_RHSC
			| HC_INTERRUPT_OC | HC_INTERRUPT_MIE;

	// Enable all queues
	UHP->HcControl |= HC_CONTROL_PLE | HC_CONTROL_IE | HC_CONTROL_CLE | HC_CONTROL_BLE;

	//Not sure if FSLargestDataPacket in UHP->HcFmInterval need to be set

	// Set HcPeriodicStart value to 90% if frame interval
	UHP->HcPeriodicStart = ((UHP->HcFmInterval & HC_FMINTERVAL_FI) * 9) / 10;

	// Begin sending SOF
	temp_value = UHP->HcControl;
	temp_value &= ~HC_CONTROL_HCFS;
	temp_value |= HC_CONTROL_HCFS_USBOPERATIONAL;
	UHP->HcControl = temp_value;

	//delay some time to access the port

	// enable port
	UHP->HcRhPortStatus |= RH_PS_PES;

}

void ohci_deinit(void)
{
	uint32_t i;
	struct ohci_td_general *td_general_header;
	struct ohci_td_general *td_general_free_header;
	struct ohci_ed *ed_header;
	struct ohci_ed *ed_free_header;

	// Change to reset state
	UHP->HcControl &= ~HC_CONTROL_HCFS;;
	
	// Free all allocated EDs and TDs

	/* TDs in control endpoint. */
	if (control_ed.p_td_head != control_ed.p_td_tail) {
		td_general_free_header = (struct ohci_td_general *)control_ed.p_td_head;
		while (td_general_free_header != NULL) {
			td_general_header = td_general_free_header->p_next_td;
			free(td_general_free_header);
			td_general_free_header = td_general_header;
		}
	}

	/* TDs in Bulk endpoints. */
	ed_header = (struct ohci_ed *)UHP->HcBulkHeadED;
	while (ed_header != NULL) {
		if (ed_header->p_td_head != ed_header->p_td_tail) {
			td_general_free_header = (struct ohci_td_general *)ed_header->p_td_head;
			while (td_general_free_header != NULL) {
				td_general_header = td_general_free_header->p_next_td;
				free(td_general_free_header);
				td_general_free_header = td_general_header;
			}
		}
	}

	/* TDs in Int/ISO endpoints. */
	for (i = 0; i < 32; i++) {
		ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
		while (ed_header != NULL) {
			if (ed_header->p_td_head != ed_header->p_td_tail) {
				td_general_free_header = (struct ohci_td_general *)ed_header->p_td_head;
				while (td_general_free_header != NULL) {
					td_general_header = td_general_free_header->p_next_td;
					free(td_general_free_header);
					td_general_free_header = td_general_header;
				}
			}
			ed_header = ed_header->p_next_ed;
		}
	}

	/* control endpoint. */	
	memset((void *)&control_ed, 0, sizeof(control_ed));


	/* Bulk endpoints. */
	ed_header = (struct ohci_ed *)UHP->HcBulkHeadED;
	while (ed_header != NULL) {
		ed_free_header = ed_header;
		ed_header = ed_header->p_next_ed;
		free(ed_free_header);
	}

	/* Int/ISO endpoints. */
	for (i = 0; i < 32; i++) {
		ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
		while (ed_header != NULL) {
			ed_free_header = ed_header;
			ed_header = ed_header->p_next_ed;
			free(ed_free_header);
		}
	}
}

uint32_t ohci_get_device_speed (void)
{
	return UHP->HcRhPortStatus & RH_PS_LSDA;
}

uint16_t ohci_get_frame_number (void)
{
	// not sure if need check the Pad1 status
	return hcca.FrameNumber;
}

void ohci_bus_reset(void)
{
	Assert(!(UHP->HcRhPortStatus & RH_PS_CCS));

	UHP->HcRhPortStatus |= RH_PS_PRS;
	bus_reset_flag = true;
}

void ohci_bus_suspend(void)
{
	uint32_t temp_value;

	UHP->HcRhPortStatus |= RH_PS_PSS;

	/* First stop any processing */
	UHP->HcControl &= ~(HC_CONTROL_CLE|HC_CONTROL_BLE|HC_CONTROL_PLE|HC_CONTROL_IE);

	UHP->HcControl |= HC_CONTROL_RWE;

	/* Suspend hub ... this is the "global (to this bus) suspend" mode,
	 * which doesn't imply ports will first be individually suspended.
	 */
	temp_value = UHP->HcControl;
	temp_value &= ~HC_CONTROL_HCFS;
	temp_value |= HC_CONTROL_HCFS_USBSUSPEND;
	UHP->HcControl = temp_value;

	// device remote wakeup enable
	UHP->HcRhStatus |= RH_HS_DRWE;
//	UHP->HcInterruptDisable |= HC_INTERRUPT_RHSC;
}

uint32_t ohci_is_suspend(void)
{
	return ((UHP->HcControl & HC_CONTROL_HCFS) == HC_CONTROL_HCFS_USBSUSPEND);
}

void ohci_bus_resume(void)
{
	uint32_t temp_value;

	temp_value = UHP->HcControl;
	temp_value &= ~HC_CONTROL_HCFS;
	temp_value |= HC_CONTROL_HCFS_USBRESUME;
	UHP->HcControl = temp_value;

	/* disable old schedule state, reinit from scratch */
	UHP->HcControlCurrentED = 0;
	UHP->HcBulkCurrentED = 0;

	UHP->HcHCCA = &hcca;
	/* interrupts might have been disabled */
	UHP->HcInterruptEnable = HC_INTERRUPT_SO | HC_INTERRUPT_WDH | HC_INTERRUPT_SF
			| HC_INTERRUPT_RD | HC_INTERRUPT_UE
			| HC_INTERRUPT_FNO | HC_INTERRUPT_RHSC
			| HC_INTERRUPT_OC | HC_INTERRUPT_MIE;

	/* Then re-enable operations */
	temp_value = UHP->HcControl;
	temp_value &= ~HC_CONTROL_HCFS;
	temp_value |= HC_CONTROL_HCFS_USBOPERATIONAL;
	UHP->HcControl = temp_value;

	// device remote wakeup disable
	UHP->HcRhStatus |= RH_HS_CRWE;

	//port resume
	UHP->HcRhPortStatus |= RH_PS_POCI;
}

bool ohci_add_ed_control(ed_info_t *ed_info)
{
	control_ed.ed_info.ul_ed_info = ed_info->ul_ed_info;
	return true;
}

bool ohci_add_ed_bulk(ed_info_t *ed_info)
{
	struct ohci_ed *bulk_ed;

	bulk_ed = (struct ohci_ed *)UHP->HcBulkHeadED;

	if (bulk_ed == NULL) {
		bulk_ed = malloc(sizeof(struct ohci_ed));
		if (bulk_ed == NULL) {
			return false;
		}
		bulk_ed->ed_info.ul_ed_info = ed_info->ul_ed_info;
		UHP->HcBulkHeadED = bulk_ed;
	} else {
		/* Check if the endpoint has been allocated */
		while (bulk_ed != NULL) {
			if (bulk_ed->ed_info.ed_info_s.bEndpointNumber ==
					ed_info->ed_info_s.bEndpointNumber) {
				return false;
			}
			if (bulk_ed->p_next_ed == NULL) {
				break;
			} else {
				bulk_ed = bulk_ed->p_next_ed;
			}
		};

		bulk_ed->p_next_ed = malloc(sizeof(struct ohci_ed));
		if (bulk_ed->p_next_ed == NULL) {
			return false;
		}
		bulk_ed->ed_info.ul_ed_info = ed_info->ul_ed_info;
	}
	return true;
}

/* Simplify the management, only 32 interrupt ED support, no list on each entry. */
bool ohci_add_ed_period(ed_info_t *ed_info)
{
	uint32_t i;
	struct ohci_ed *period_ed;
	struct ohci_ed *period_ed_header;

	if (ed_info->ed_info_s.bFormat) {
		/* isochronous ED */
		period_ed = malloc(sizeof(struct ohci_ed));
		if (period_ed == NULL) {
			return false;
		}
		period_ed->ed_info.ul_ed_info = ed_info->ul_ed_info;
		for (i = 0; i < 32; i++) {
			period_ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
			if (period_ed_header == NULL) {
				hcca.InterruptTable[i] = period_ed;
			} else {
				period_ed_header->p_next_ed = period_ed;
			}
		}
	} else {
		/* interrupt ED */
		for (i = 0; i < 32; i++) {
			period_ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
			if (period_ed_header == NULL) {
				period_ed = malloc(sizeof(struct ohci_ed));
				if (period_ed == NULL) {
					return false;
				}
				period_ed->ed_info.ul_ed_info = ed_info->ul_ed_info;
				hcca.InterruptTable[i] = period_ed;
				return true;
			} else {
				if (period_ed_header->ed_info.ed_info_s.bEndpointNumber
						== ed_info->ed_info_s.bEndpointNumber) {
					return false;
				}
			}
		}
	}
	return true;
}

void ohci_remove_ed(uint8_t ep_number)
{
	uint32_t i;
	struct ohci_ed *ed_header;
	struct ohci_ed *ed_free_header;

	/* Control endpoints. */
	if (ep_number == 0) {
		memset((void *)&control_ed, 0, sizeof(control_ed));
	} 

	/* Bulk/Int/ISO endpoints. */
	if (ep_number != 0xFF) {
		/* Bulk endpoints. */
		ed_header = (struct ohci_ed *)UHP->HcBulkHeadED;
		if (ed_header != NULL) {
			if (ed_header->ed_info.ed_info_s.bEndpointNumber == ep_number) {
				UHP->HcBulkHeadED = ed_header->p_next_ed;
				ed_free_header = ed_header;
				free(ed_free_header);
			} else {
				/* Check the list */
				while ((ed_header != NULL) && (ed_header->p_next_ed != NULL)) {
					if (ed_header->p_next_ed->ed_info.ed_info_s.bEndpointNumber
							==	ep_number) {
						ed_free_header = ed_header->p_next_ed;
						ed_header->p_next_ed = ed_header->p_next_ed->p_next_ed;
						free(ed_free_header);
						break;
					}
					ed_header = ed_header->p_next_ed;
				};
			}
		}

		/* Int/ISO endpoints. */
		for (i = 0; i < 32; i++) {
			ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
			while (ed_header != NULL) {
				if (ed_header->ed_info.ed_info_s.bEndpointNumber == ep_number) {
					ed_free_header = ed_header;
					free(ed_free_header);
					break;
				}
				ed_header = ed_header->p_next_ed;
			}
		}
	}

	/* All endpoints. */
	if (ep_number == 0xFF) {
		/* Bulk endpoints. */
		ed_header = (struct ohci_ed *)UHP->HcBulkHeadED;
		while (ed_header != NULL) {
			ed_free_header = ed_header;
			ed_header = ed_header->p_next_ed;
			free(ed_free_header);
		}

		/* Int/ISO endpoints. */
		for (i = 0; i < 32; i++) {
			ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
			while (ed_header != NULL) {
				if (ed_header->ed_info.ed_info_s.bEndpointNumber == ep_number) {
					ed_free_header = ed_header;
					ed_header = ed_header->p_next_ed;
					free(ed_free_header);
				} else {
					ed_header = ed_header->p_next_ed;
				}
			}
		}
	};
}

bool ohci_add_td_control(enum pid pid, uint8_t *buf, uint16_t buf_size)
{
	struct ohci_td_general *control_td;
	struct ohci_td_general *td_general_header;

	control_td = malloc(sizeof(struct ohci_td_general));
	if (control_td == NULL) {
		return false;
	}
	control_td->td_info.bBufferRounding = 0;
	control_td->td_info.bDirectionPID = pid;
	control_td->td_info.bDelayInterrupt = 0;
	if (pid == TD_PID_SETUP) {
		control_td->td_info.bDataToggle = 2;
	} else {
		control_td->td_info.bDataToggle = 3;
	}
	control_td->td_info.bErrorCount = 0;
	control_td->td_info.bConditionCode = 0;
	control_td->pCurrentBufferPointer= buf;
	control_td->p_next_td = NULL;
	control_td->pBufferEnd = buf + buf_size - 1;

	/* Check the halt status. */
	if ((uint32_t)control_ed.p_td_head & 0x01) {
		return false;
	}

	if (control_ed.p_td_head != control_ed.p_td_tail) {
		// set the skip
		control_ed.ed_info.ed_info_s.bSkip = 1;

		td_general_header = (struct ohci_td_general *)control_ed.p_td_head;
		if (td_general_header == NULL) {
			control_ed.p_td_head = control_td;
		}
		while (td_general_header->p_next_td != NULL) {
			td_general_header = td_general_header->p_next_td;
		}
		td_general_header->p_next_td = control_td;

		// clear the skip
		control_ed.ed_info.ed_info_s.bSkip = 0;

		UHP->HcCommandStatus |= HC_COMMANDSTATUS_CLF;

		return true;
	}

	return true;
}


bool ohci_add_td_non_control(uint8_t ep_number, uint8_t *buf, uint32_t buf_size)
{
	struct ohci_td_general *general_td;
	struct ohci_td_general *td_general_header;
	struct ohci_td_iso *iso_td;
	struct ohci_ed *ed_header;
	uint32_t i;

	/* Bulk endpoints. */
	ed_header = (struct ohci_ed *)UHP->HcBulkHeadED;
	while (ed_header != NULL) {
		if (ed_header->ed_info.ed_info_s.bEndpointNumber == ep_number) {
			general_td = malloc(sizeof(struct ohci_td_general));
			if (general_td == NULL) {
				return false;
			}
			general_td->td_info.bBufferRounding = 0;
			general_td->td_info.bDirectionPID = 3;
			general_td->td_info.bDelayInterrupt = 0;
			general_td->td_info.bDataToggle = 0;
			general_td->td_info.bErrorCount = 0;
			general_td->td_info.bConditionCode = 0;
			general_td->pCurrentBufferPointer= buf;
			general_td->p_next_td = NULL;
			general_td->pBufferEnd = buf + buf_size - 1;

			/* Check the halt status. */
			if ((uint32_t)ed_header->p_td_head & 0x01) {
				return false;
			}

			if (ed_header->p_td_head != ed_header->p_td_tail) {
				// set the skip
				ed_header->ed_info.ed_info_s.bSkip = 1;

				td_general_header = (struct ohci_td_general *)ed_header->p_td_head;

				if (td_general_header == NULL) {
					ed_header->p_td_head = general_td;
				}

				while (td_general_header->p_next_td != NULL) {
					td_general_header = td_general_header->p_next_td;
				}
				td_general_header->p_next_td = general_td;

				// clear the skip
				ed_header->ed_info.ed_info_s.bSkip = 0;

				UHP->HcCommandStatus |= HC_COMMANDSTATUS_BLF;

				return true;
			}
		}
	}

	/* Int/ISO endpoints. */
	for (i = 0; i < 32; i++) {
		ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
		if (ed_header->ed_info.ed_info_s.bEndpointNumber == ep_number) {
			if (ed_header->ed_info.ed_info_s.bFormat = 0) {
				general_td = malloc(sizeof(struct ohci_td_general));
				if (general_td == NULL) {
					return false;
				}
				general_td->td_info.bBufferRounding = 0;
				general_td->td_info.bDirectionPID = 3;
				general_td->td_info.bDelayInterrupt = 0;
				general_td->td_info.bDataToggle = 0;
				general_td->td_info.bErrorCount = 0;
				general_td->td_info.bConditionCode = 0;
				general_td->pCurrentBufferPointer= buf;
				general_td->p_next_td = NULL;
				general_td->pBufferEnd = buf + buf_size - 1;

				/* Check the halt status. */
				if ((uint32_t)ed_header->p_td_head & 0x01) {
					return false;
				}

				if (ed_header->p_td_head != ed_header->p_td_tail) {
					// set the skip
					ed_header->ed_info.ed_info_s.bSkip = 1;

					td_general_header = (struct ohci_td_general *)ed_header->p_td_head;

					if (td_general_header == NULL) {
						ed_header->p_td_head = general_td;
					}

					while (td_general_header->p_next_td != NULL) {
						td_general_header = td_general_header->p_next_td;
					}
					td_general_header->p_next_td = general_td;

					// clear the skip
					ed_header->ed_info.ed_info_s.bSkip = 0;

					UHP->HcCommandStatus |= HC_COMMANDSTATUS_BLF;

					return true;
				}
			} else {
				iso_td = malloc(sizeof(struct ohci_td_iso));
				if (iso_td == NULL) {
					return false;
				}
				// start after 3 frame
				iso_td->td_info.bStartingFrame = ohci_get_frame_number() + 3;
				iso_td->td_info.bDelayInterrupt = 0;
				iso_td->td_info.FrameCount = 0;				// one frame transaction
				iso_td->td_info.bConditionCode = 0;
				iso_td->pBufferPage0= buf;
				iso_td->p_next_td = NULL;
				iso_td->pBufferEnd = buf + buf_size - 1;
				iso_td->offset_psw[0] = 0;

				/* Check the halt status. */
				if ((uint32_t)ed_header->p_td_head & 0x01) {
					return false;
				}

				if (ed_header->p_td_head != ed_header->p_td_tail) {
					// set the skip
					ed_header->ed_info.ed_info_s.bSkip = 1;

					td_general_header = (struct ohci_td_iso *)ed_header->p_td_head;

					if (td_general_header == NULL) {
						ed_header->p_td_head = general_td;
					}

					while (td_general_header->p_next_td != NULL) {
						td_general_header = td_general_header->p_next_td;
					}
					td_general_header->p_next_td = iso_td;

					// clear the skip
					ed_header->ed_info.ed_info_s.bSkip = 0;

					return true;
				}
			}
		}
	}
	return false;
}

void ohci_remove_td(uint8_t ep_number)
{
	uint32_t i;
	struct ohci_ed *ed_header;
	struct ohci_td_general *td_general_header;
	struct ohci_td_general *td_general_free_header;

	/* Control endpoints. */
	if (ep_number == 0) {
		if (control_ed.p_td_head != control_ed.p_td_tail) {
			td_general_free_header = (struct ohci_td_general *)control_ed.p_td_head;
			while (td_general_free_header != NULL) {
				td_general_header = td_general_free_header->p_next_td;
				free(td_general_free_header);
				td_general_free_header = td_general_header;
			}
		}
		return;
	} 

	/* Bulk endpoints. */
	ed_header = (struct ohci_ed *)UHP->HcBulkHeadED;
	while (ed_header != NULL) {
		if (ed_header->ed_info.ed_info_s.bEndpointNumber ==	ep_number) {
			if (ed_header->p_td_head != ed_header->p_td_tail) {
				td_general_free_header = (struct ohci_td_general *)ed_header->p_td_head;
				while (td_general_free_header != NULL) {
					td_general_header = td_general_free_header->p_next_td;
					free(td_general_free_header);
					td_general_free_header = td_general_header;
				}
			}
			return;
		}
	}

	/* Int/ISO endpoints. */
	for (i = 0; i < 32; i++) {
		ed_header = (struct ohci_ed *)hcca.InterruptTable[i];
		while (ed_header != NULL) {
			if (ed_header->ed_info.ed_info_s.bEndpointNumber == ep_number) {
				if (ed_header->p_td_head != ed_header->p_td_tail) {
					td_general_free_header = (struct ohci_td_general *)ed_header->p_td_head;
					while (td_general_free_header != NULL) {
						td_general_header = td_general_free_header->p_next_td;
						free(td_general_free_header);
						td_general_free_header = td_general_header;
					}
				}
				return;
			}
			ed_header = ed_header->p_next_ed;
		}
	}
}

void ohci_enable_interrupt(enum ohci_interrupt_source int_source)
{
	if (int_source = OHCI_INTERRUPT_SO) {
		UHP->HcInterruptEnable = HC_INTERRUPT_SO;
	}
	if (int_source = OHCI_INTERRUPT_WDH) {
		UHP->HcInterruptEnable = HC_INTERRUPT_WDH;
	}
	if (int_source = OHCI_INTERRUPT_SF) {
		UHP->HcInterruptEnable = HC_INTERRUPT_SF;
	}
	if (int_source = OHCI_INTERRUPT_RD) {
		UHP->HcInterruptEnable = HC_INTERRUPT_RD;
	}
	if (int_source = OHCI_INTERRUPT_UE) {
		UHP->HcInterruptEnable = HC_INTERRUPT_UE;
	}
	if (int_source = OHCI_INTERRUPT_FNO) {
		UHP->HcInterruptEnable = HC_INTERRUPT_FNO;
	}
	if (int_source = OHCI_INTERRUPT_RHSC) {
		UHP->HcInterruptEnable = HC_INTERRUPT_RHSC;
	}
	if (int_source = OHCI_INTERRUPT_OC) {
		UHP->HcInterruptEnable = HC_INTERRUPT_OC;
	}
}
void ohci_disable_interrupt(enum ohci_interrupt_source int_source)
{
	if (int_source = OHCI_INTERRUPT_SO) {
		UHP->HcInterruptDisable = HC_INTERRUPT_SO;
	}
	if (int_source = OHCI_INTERRUPT_WDH) {
		UHP->HcInterruptDisable = HC_INTERRUPT_WDH;
	}
	if (int_source = OHCI_INTERRUPT_SF) {
		UHP->HcInterruptDisable = HC_INTERRUPT_SF;
	}
	if (int_source = OHCI_INTERRUPT_RD) {
		UHP->HcInterruptDisable = HC_INTERRUPT_RD;
	}
	if (int_source = OHCI_INTERRUPT_UE) {
		UHP->HcInterruptDisable = HC_INTERRUPT_UE;
	}
	if (int_source = OHCI_INTERRUPT_FNO) {
		UHP->HcInterruptDisable = HC_INTERRUPT_FNO;
	}
	if (int_source = OHCI_INTERRUPT_RHSC) {
		UHP->HcInterruptDisable = HC_INTERRUPT_RHSC;
	}
	if (int_source = OHCI_INTERRUPT_OC) {
		UHP->HcInterruptDisable = HC_INTERRUPT_OC;
	}
}

void ohci_register_callback(enum ohci_interrupt_source int_source, void *call_back)
{
	ohci_callback_pointer[int_source] = call_back;
}

void ohci_unregister_callback(enum ohci_interrupt_source int_source)
{
	ohci_callback_pointer[int_source] = NULL;
}

void UHP_Handler()
{
	uint32_t int_status;
	uint32_t rh_status;
	uint32_t rh_port_status;
	struct ohci_td_general *td_general_header;

	rh_status = UHP->HcInterruptStatus;
	rh_port_status = UHP->HcRhPortStatus;

	/* Read interrupt status (and flush pending writes).  We ignore the
	 * optimization of checking the LSB of hcca->done_head; it doesn't
	 * work on all systems (edge triggering for OHCI can be a factor).
	 */
	int_status = UHP->HcInterruptStatus;

	/* We only care about interrupts that are enabled */
	int_status &= UHP->HcInterruptEnable;

	if (int_status & HC_INTERRUPT_WDH) {
		td_general_header = (struct ohci_td_general *)hcca.pDoneHead;
//		callback_para = td_general_header->pCurrentBufferPointer;    //byte transferred if no error
		callback_para = td_general_header->td_info.bConditionCode;  //
		free(td_general_header);
		ohci_callback_pointer[OHCI_INTERRUPT_WDH](&callback_para);
		UHP->HcInterruptStatus &= ~HC_INTERRUPT_WDH;
	}

	if (int_status & HC_INTERRUPT_RHSC) {
		//rhsc
		UHP->HcInterruptStatus |=  HC_INTERRUPT_RD | HC_INTERRUPT_RHSC;
		if (bus_reset_flag) {
			if (rh_status & RH_PS_PRSC) {
				bus_reset_flag = false;
				callback_para = BUS_RESET;
				ohci_callback_pointer[OHCI_INTERRUPT_RHSC](&callback_para);
			}
		} else if (!(rh_status & RH_HS_DRWE)) {
			if (rh_port_status & RH_PS_CCS) {
				callback_para = BUS_CONNECT;
			} else {
				callback_para = BUS_DISCONNECT;
			}
			ohci_callback_pointer[OHCI_INTERRUPT_RHSC](&callback_para);
		} else {
			callback_para = 0xff;
		}
	}

	/* For connect and disconnect events, we expect the controller
	 * to turn on RHSC along with RD.  But for remote wakeup events
	 * this might not happen.
	 */
	else if (int_status & HC_INTERRUPT_RD) {
		// resume detect
		UHP->HcInterruptStatus |= HC_INTERRUPT_RD;

		ohci_bus_resume();
		ohci_callback_pointer[OHCI_INTERRUPT_RD](&callback_para);
	}

	if (int_status & HC_INTERRUPT_SF) {
		ohci_callback_pointer[OHCI_INTERRUPT_SF](&callback_para);
	}

	if (int_status & HC_INTERRUPT_SO) {
		ohci_callback_pointer[OHCI_INTERRUPT_SO](&callback_para);
	}

	if (int_status & HC_INTERRUPT_UE) {
		ohci_callback_pointer[OHCI_INTERRUPT_UE](&callback_para);
	}

}

