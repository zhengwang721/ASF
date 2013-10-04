/**
 * \file per_mode_receptor.c
 *
 * \brief Receptor functionalities in PER Measurement mode - Performance
 * Analyzer
 * application
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
 */

/*
 * Copyright(c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#include "app_init.h"
#include "tal_pib.h"
#include "tal_helper.h"
#include "ieee_const.h"
#include "app_frame_format.h"
#include "app_per_mode.h"
#include "conf_board.h"
#include "led.h"

/**
 * \addtogroup group_per_mode_receptor
 * \{
 */

/* === TYPES =============================================================== */

/*=====EXTERBNALS============================================================*/

/* === MACROS ============================================================== */

/* LED Blink count for identify command */
#define LED_BLINK_COUNT_FOR_IDENTIFY          (20)
#define LED_BLINK_COUNT_FOR_BUTTON_PRESS      (10)

/* At every LED_TOGGLE_COUNT_FOR_PER count the Led toggles when PER test
 * is in progress */
#define DELAY_BEFORE_APP_RESET_IN_MICRO_SEC   (5000)

/* === PROTOTYPES ========================================================== */
static void identify_timer_handler_cb(void *parameter);
static void app_reset_cb(void *parameter);
static void send_result_rsp(trx_id_t trx);
static void send_peer_info_rsp(trx_id_t trx);
static void set_paramter_on_recptor_node(trx_id_t trx, app_payload_t *msg);
static void set_default_configuration_peer_node(trx_id_t trx);
static void get_node_info(trx_id_t trx, peer_info_rsp_t *data);

#if (ANTENNA_DIVERSITY == 1)
static void send_diversity_status_rsp(trx_id_t trx);
#endif /* End of ANTENNA_DIVERSITY */

static void send_range_test_rsp(trx_id_t trx,uint8_t seq_num, uint32_t frame_count,
		int8_t ed, uint8_t lqi);
static bool send_range_test_marker_cmd(trx_id_t trx);

#ifdef CRC_SETTING_ON_REMOTE_NODE
static void send_crc_status_rsp(trx_id_t trx);
static bool crc_check_ok(frame_info_t *mac_frame_info);
static uint16_t crc_test(uint16_t crc, uint8_t data);
#endif /* End of CRC_SETTING_ON_REMOTE_NODE */

static bool range_test_in_progress[NO_TRX] = {false,false};
/* === GLOBALS ============================================================= */

static uint32_t number_rx_frames[NO_TRX];
static uint32_t aver_lqi[NO_TRX];
static uint32_t aver_rssi[NO_TRX];
static uint8_t seq_num_receptor[NO_TRX];
#ifdef CRC_SETTING_ON_REMOTE_NODE
uint32_t frames_with_wrong_crc[NO_TRX];
bool manual_crc[NO_TRX]= {false,false};
#endif
static uint8_t marker_seq_num[NO_TRX] = {0,0};

/* ! \} */

/* === IMPLEMENTATION ====================================================== */

/*
 * \brief Initialize the application in PER Measurement mode as Receptor
 *
 */
void per_mode_receptor_init(trx_id_t trx, void *parameter)
{

	/* PER TEST Receptor sequence number */
	seq_num_receptor[trx] = rand();

	
	printf("\r\n Starting PER Measurement mode as Reflector");
	
	/* keep the compiler happy */
	parameter = parameter;
}

/*
 * \brief The application task when the node is in PER_TEST_RECEPTOR state
 * i.e.PER Measurement mode as Receptor
 *
 */
void per_mode_receptor_task(trx_id_t trx)
{
	/* For Range Test  in PER Mode the receptor has to poll for a button
	 * press to initiate marker transmission */
	if (range_test_in_progress[trx]) {
		static uint8_t key_press;
		/* Check for any key press */
		key_press = app_debounce_button();

		if (key_press != 0) {
			printf("\r\n\nButton Pressed...");
			if (send_range_test_marker_cmd(trx)) {
				printf("\r\nInitiating Marker Transmission...");
				/* Timer for LED Blink for Marker Transmission*/
				sw_timer_start(APP_TIMER_TO_TX,
						LED_BLINK_RATE_IN_MICRO_SEC,
						SW_TIMEOUT_RELATIVE,
						(FUNC_PTR)marker_tx_timer_handler_cb,
						NULL);
			}
		}
	}
}

/**
 * \brief Function to send the range test marker command to the initiator node
 */
static bool send_range_test_marker_cmd(trx_id_t trx)
{
	uint8_t payload_length;
	app_payload_t msg;
	result_req_t *data;

	/* Create the payload */
	msg.cmd_id = RANGE_TEST_MARKER_CMD;
	seq_num_receptor[trx]++;
	msg.seq_num = marker_seq_num[trx]++;
	data = (result_req_t *)&msg.payload;
	/* Just a dummy value */
	data->cmd = DUMMY_PAYLOAD;

	payload_length = ((sizeof(app_payload_t) -
			sizeof(general_pkt_t)) +
			sizeof(result_req_t));

	/* Send the frame to Peer node */
	if (MAC_SUCCESS == transmit_frame(trx,FCF_SHORT_ADDR,
			(uint8_t *)&(node_info[trx].peer_short_addr),
			FCF_SHORT_ADDR,
			seq_num_receptor[trx],
			(uint8_t *)&msg,
			payload_length,
			true)
			) {
		return(true);
	}

	return(false);
}

/*
 * \brief Callback that is called once tx is done in PER_TEST_RECEPTOR state
 *        This call back is used when the node is operating as
 *        PER Measurement mode Receptor
 *
 * \param status    Status of the transmission procedure
 * \param frame     Pointer to the transmitted frame structure
 */
void per_mode_receptor_tx_done_cb(trx_id_t trx, retval_t status, frame_info_t *frame)
{
	status = status;
	frame = frame;
	/* Allow the next transmission to happen */
	node_info[trx].transmitting = false;
}

/*
 * \brief Callback that is called if data has been received by trx
 * in the PER_TEST_RECEPTOR state
 * This call back is used when the node is operating as
 * PER Measurement mode Receptor
 * \param frame Pointer to received frame
 */
void per_mode_receptor_rx_cb(trx_id_t trx, frame_info_t *mac_frame_info)
{
    app_payload_t *msg;
    static uint8_t rx_count;
    uint8_t expected_frame_size;

    /* Point to the message : 1 =>size is first byte and 2=>FCS*/
    msg = (app_payload_t *)(mac_frame_info->mpdu + FRAME_OVERHEAD);

#ifdef CRC_SETTING_ON_REMOTE_NODE
    /* If counting of wrong CRC packets is enabled on receptor node */
    if (manual_crc[trx])
    {
        uint16_t my_addr;
        uint16_t dest_addr;
        memcpy(&dest_addr, &mac_frame_info->mpdu[PL_POS_DST_ADDR_START], SHORT_ADDR_LEN);

			tal_pib_get(trx,macShortAddress, (uint8_t *)&my_addr);

        /* Check the destination address of the packet is my address  */
        if (dest_addr != my_addr)
        {
            return;
        }

        /* Counting of wrong crc packets option enabled and received crc is not OK */
        if (false == crc_check_ok(mac_frame_info))
        {
            if ( msg->cmd_id != PER_TEST_PKT )
            {
                /* Don't let in any packets with wrong CRC unless it is a PER
                test packet */
                return;
            }
            frames_with_wrong_crc[trx]++;
        }
    }
#endif /* #ifdef CRC_SETTING_ON_REMOTE_NODE */

    switch ((msg->cmd_id))
    {
        case SET_PARAM:
            {
                /* Calculate the expected frame size in case of SET_PARAM cmd */
                expected_frame_size = (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                          sizeof(general_pkt_t)) +
                                                         sizeof(set_parm_req_t)));
                if (*(mac_frame_info->mpdu) == expected_frame_size)
                {
                    /* Extract and process the cmd received */
                    set_paramter_on_recptor_node(trx, msg);
                }
                break;
            } /* case SET_PARAM */

        case PER_TEST_PKT:
            {
                static uint8_t cur_seq_no, prev_seq_no;
                /* if PER test frames received then increment number_rx_frames */
                if (number_rx_frames[trx] == 0)
                {
                    
                        printf("\r\nReceiving..");
                    aver_lqi[trx] += mac_frame_info->mpdu[mac_frame_info->mpdu[0] + LQI_LEN];
                    aver_rssi[trx] += mac_frame_info->mpdu[mac_frame_info->mpdu[0] + LQI_LEN + 1];
#ifdef CRC_SETTING_ON_REMOTE_NODE
                    frames_with_wrong_crc[trx] = 0;
#endif /* #ifdef CRC_SETTING_ON_REMOTE_NODE */
                    number_rx_frames[trx]++;
                    /* Get the seq no. of the first packet */
                    prev_seq_no = mac_frame_info->mpdu[PL_POS_SEQ_NUM];
                }
                else
                {
                    cur_seq_no = mac_frame_info->mpdu[PL_POS_SEQ_NUM];
                    /* Check for the duplicate packets */
                    if (prev_seq_no != cur_seq_no)
                    {
                        number_rx_frames[trx]++;
                        prev_seq_no = cur_seq_no;
                        /* Extract LQI and  RSSI */
                        aver_lqi[trx] += mac_frame_info->mpdu[mac_frame_info->mpdu[0] + LQI_LEN];
                        aver_rssi[trx] += mac_frame_info->mpdu[mac_frame_info->mpdu[0] + LQI_LEN + 1];
                    }

                }
                /* Led is toggled indicating the test in progress at the count of
                 * LED_TOGGLE_COUNT_FOR_PER
                 */
                if (rx_count >= LED_TOGGLE_COUNT_FOR_PER)
                {
                    app_led_event(LED_EVENT_RX_FRAME);
                    rx_count = 0;
                }
                else
                {
                    rx_count++;
                }
                break;
            } /* case PER_TEST_PKT */

        case RESULT_REQ:
            {
                /* Calculate the expected frame size in case of RESULT_REQ cmd */
                expected_frame_size =  (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                           sizeof(general_pkt_t)) +
                                                          sizeof(result_req_t)));
                if (*(mac_frame_info->mpdu) == expected_frame_size)
                {
                    if (number_rx_frames[trx] != 0)
                    {
                        aver_lqi[trx] = aver_lqi[trx] / number_rx_frames[trx];
                        aver_rssi[trx] = aver_rssi[trx] / number_rx_frames[trx];
                    }
                    send_result_rsp(trx);

                    
                        int8_t rssi_val = tal_get_rssi_base_val(trx);
                        rssi_val += aver_rssi[trx];
                        printf("\r\nNumber of received frames = %"
                               PRIu32 "; average LQI = %d, average RSSI = %d dBm",
                               number_rx_frames[trx], (uint8_t)aver_lqi[trx], (int8_t)rssi_val );
#ifdef CRC_SETTING_ON_REMOTE_NODE
                        if (manual_crc[trx])
                        {
                            printf("\r\nNumber of received frames with wrong CRC = %" PRIu32,
                                   frames_with_wrong_crc[trx]);
                        }
#endif /* #ifdef CRC_SETTING_ON_REMOTE_NODE */
                    
                    number_rx_frames[trx] = 0;
                    aver_lqi[trx] = 0;
                    aver_rssi[trx] = 0;
                    rx_count = 0;
                    app_led_event(LED_EVENT_PEER_SEARCH_DONE);
                }
                break;
            } /* RESULT_REQ */

        case PER_TEST_START_PKT:
            {
#ifdef CRC_SETTING_ON_REMOTE_NODE
                frames_with_wrong_crc[trx] = 0;
#endif /* #ifdef CRC_SETTING_ON_REMOTE_NODE */
                number_rx_frames[trx] = 0;
                aver_lqi[trx] = 0;
                aver_rssi[trx] = 0;
                rx_count = 0;
            }
            break;

#if (ANTENNA_DIVERSITY == 1)
        case DIV_STAT_REQ:
            {
                /* Calculate the expected frame size in case of DIV_STAT_REQ cmd */
                expected_frame_size = (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                          sizeof(general_pkt_t)) +
                                                         sizeof(div_stat_req_t)));
                if (*(mac_frame_info->mpdu) == expected_frame_size)
                {
                    send_diversity_status_rsp(trx);
                }
                break;
            } /* DIV_STAT_REQ */

        case DIV_SET_REQ:
            {
                /* Calculate the expected frame size in case of DIV_SET_REQ cmd */
                expected_frame_size = (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                          sizeof(general_pkt_t)) +
                                                         sizeof(div_set_req_t)));
                if (*(mac_frame_info->mpdu) == expected_frame_size)
                {
                    /* Antenna diversity need to be enabled */
                    if (msg->payload.div_set_req_data.status)
                    {
                        tal_ant_div_config(trx, ANT_DIVERSITY_ENABLE,ANTENNA_DEFAULT);
                        printf("\r\n Antenna diversity enabled");
                    }
                    else /* Antenna diversity need to be disabled */
                    {
                        /* Set the selected antenna & disable the antenna diversity */
                         tal_ant_div_config(trx,ANT_DIVERSITY_DISABLE, msg->payload.div_set_req_data.ant_sel);
                        printf("\r\n Antenna diversity disabled and antenna");
                        if (msg->payload.div_set_req_data.ant_sel == ANT_CTRL_1)
                        {
                            printf(" selected is A1/X2");
                        }
                        else if (msg->payload.div_set_req_data.ant_sel == ANT_CTRL_2)
                        {
                            printf(" selected is A2/X3");
                        }
                        else
                        {
                            printf(" selected is invalid");
                        }
                    }

                }
                break;
            } /* DIV_SET_REQ */
#endif /* #if (ANTENNA_DIVERSITY == 1) */

#ifdef CRC_SETTING_ON_REMOTE_NODE
        case CRC_STAT_REQ:
            {
                /* Calculate the expected frame size in case of CRC_STAT_REQ cmd */
                expected_frame_size =  (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                           sizeof(general_pkt_t)) +
                                                          sizeof(crc_stat_req_t)));
                if (*(mac_frame_info->mpdu) == expected_frame_size)
                {
                    send_crc_status_rsp(trx);
                }
                break;
            } /* CRC_STAT_REQ */

        case CRC_SET_REQ:
            {
                /* Calculate the expected frame size in case of CRC_SET_REQ cmd */
                expected_frame_size = (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                          sizeof(general_pkt_t)) +
                                                         sizeof(crc_set_req_t)));
                if (*(mac_frame_info->mpdu) == expected_frame_size)
                {
                    if (msg->payload.crc_set_req_data.status)
                    {
                        /* Enable the Promiscuous Mode */
                        tal_rxaack_prom_mode_ctrl(trx,AACK_PROM_ENABLE);

                        
                            printf("\r\n Counting packets with CRC error enabled");
                        
                        manual_crc[trx] = true;
                    }
                    else
                    {
                        /* Disable the Promiscuous Mode */
                        tal_rxaack_prom_mode_ctrl(trx,AACK_PROM_DISABLE);

                            printf("\r\n Counting packets with CRC error disabled");
                        
                        manual_crc[trx] = false;
                    }

                }
                break;
            } /* CRC_SET_REQ */
#endif /* #ifdef CRC_SETTING_ON_REMOTE_NODE */

        case IDENTIFY_NODE:
            {

                    printf(" \r\n Blinking LED: identify frame received");
                
                sw_timer_start(APP_TIMER_TO_TX,
                                LED_BLINK_RATE_IN_MICRO_SEC,
                                SW_TIMEOUT_RELATIVE,
                                (FUNC_PTR)identify_timer_handler_cb,
                                NULL);
                break;
            }
        case DISCONNECT_NODE:
            {
                /* app reset on remote node as the disconnect peer is successful
                   This is to make the node to restart as boot up and
                   open for fresh peer search */
                sw_timer_start(T_APP_TIMER,
                                DELAY_BEFORE_APP_RESET_IN_MICRO_SEC,
                                SW_TIMEOUT_RELATIVE,
                                (FUNC_PTR)app_reset_cb,
                                (void*) trx );


            }
            break;
        case SET_DEFAULT_REQ:
            {
                set_default_configuration_peer_node(trx);
            }
            break;
        case PEER_INFO_REQ:
            {
                send_peer_info_rsp(trx);
            }
            break;
	
	case RANGE_TEST_START_PKT:
	{
		/* set the flag to indicate that the receptor node is in range
		 * test mode */
		range_test_in_progress[trx] = true;
		printf("\r\nStarting Range Test in PER Mode...");
	}
	break;

	case RANGE_TEST_STOP_PKT:
	{
		/* reset the flag to indicate that the range test mode is
		 * stopped*/
		range_test_in_progress[trx] = false;
		marker_seq_num[trx] = 0;
		printf("\r\nStopping Range Test...");
		app_led_event(LED_EVENT_PEER_SEARCH_DONE);
	}
	break;

	case RANGE_TEST_PKT:
	{
		/* On reception of the range test packet calculate the ed and
		 * lqi values of
		 * the received pkt and add it as the payload of the response
		 * frame*/
		uint8_t phy_frame_len = mac_frame_info->mpdu[0];
		uint32_t frame_count;
		/* Get the frame count in correct format */
		frame_count
			= (CCPU_ENDIAN_TO_LE32(msg->payload.range_tx_data.
				frame_count));
		int8_t rssi_base_val, ed_value;
		rssi_base_val = tal_get_rssi_base_val(trx);
		app_led_event(LED_EVENT_RX_FRAME);
		/* Map the register ed value to dbm values */
		ed_value
			= mac_frame_info->mpdu[phy_frame_len + LQI_LEN +
				ED_VAL_LEN] + rssi_base_val;

		/* Send Response cmd to the received Range Test packet with the
		 * lqi and ed values */
		send_range_test_rsp(trx,msg->seq_num,
				msg->payload.range_tx_data.frame_count,	\
				ed_value,
				mac_frame_info->mpdu[phy_frame_len +
				LQI_LEN]);
		/* Print the received values to the terminal */
		printf(
				"\r\nRange Test Packet Received...\tFrame No : %" PRIu32 "\tLQI : %d\tED : %d",
				frame_count,
				mac_frame_info->mpdu[phy_frame_len + LQI_LEN],
				ed_value);
	}
	break;

	case RANGE_TEST_MARKER_RSP:
	{
		/* On reception of the Response frame to the Marker cmd sent ,
		 * get the lqi and ed values and print it on the terminal */
		int8_t rssi_base_val, ed_value;
		rssi_base_val = tal_get_rssi_base_val(trx);
		uint8_t phy_frame_len = mac_frame_info->mpdu[0];
		/* Map the register ed value to dbm values */
		ed_value
			= mac_frame_info->mpdu[phy_frame_len + LQI_LEN +
				ED_VAL_LEN] + rssi_base_val;
		printf("\r\nMarker Response Received... LQI : %d\t ED %d \n",
				mac_frame_info->mpdu[phy_frame_len + LQI_LEN],
				ed_value);
		/* Timer for LED Blink for Reception of Marker Response*/
		sw_timer_start(T_APP_TIMER,
				LED_BLINK_RATE_IN_MICRO_SEC,
				SW_TIMEOUT_RELATIVE,
				(FUNC_PTR)marker_rsp_timer_handler_cb,
				NULL);
	}
	break;


	default:
		break;
	}
}

/**
  * \brief Extract the received command frame and set the parameters like
  * channel, tx power etc on Receptor node
  * \param pointer to cmd payload
 */
static void set_paramter_on_recptor_node(trx_id_t trx, app_payload_t *msg)
{
    uint8_t param_val;

    switch (msg->payload.set_parm_req_data.param_type)
    {
        case CHANNEL: /* Parameter = channel */
            {

                param_val = msg->payload.set_parm_req_data.param_value;

					/* set the channel on receptor with the received value */
					tal_pib_set(trx,phyCurrentChannel, (pib_value_t *)&param_val);

                    printf("\r\n Channel changed to %d", param_val);
                

            }
            break;
			
        case CHANNEL_PAGE:
            {
				retval_t status;
                param_val = msg->payload.set_parm_req_data.param_value;

					status  = tal_pib_set(trx,phyCurrentPage, (pib_value_t *)&param_val);

                if (status == MAC_SUCCESS)
                {
                    
                        printf("\r\n Channel page changed to %d", param_val);
                    
                }
                else
                {

                    
                        printf("\r\n Channel page change failed");
                   

                }
                
            }
            break;

            /* Handle Tx power value in dBm */
        case TX_POWER_DBM: /* parameter = Tx power in dBm */
            {
                uint8_t temp_var;
                /* Get the the received tx power in dBm */
                param_val = msg->payload.set_parm_req_data.param_value;
                temp_var = CONV_DBM_TO_phyTransmitPower((int8_t)param_val);

				tal_pib_set(trx,phyTransmitPower, (pib_value_t *)&temp_var);

			}
            break;

            /* Handle Tx power value in dBm */
        case TX_POWER_REG: /* Parameter = TX power in Reg value */
            {

                int8_t tx_pwr_dbm = 0 ;

                /* get the the received tx power as reg value */
                param_val = msg->payload.set_parm_req_data.param_value;
                if (MAC_SUCCESS == tal_convert_reg_value_to_dBm(trx,param_val, &tx_pwr_dbm))
                {
                    uint8_t temp_var = CONV_DBM_TO_phyTransmitPower(tx_pwr_dbm);
                   
                    tal_pib_set(trx,phyTransmitPower, (pib_value_t *)&temp_var);
                    tal_set_tx_pwr(trx,REGISTER_VALUE,param_val);

                }
            }
            break;

        default:
            printf(" \r\nUnsupported Parameter");
            break;

    }
}

/**
 * \brief Timer Callback function  if identify command is received on air
 *        by a node in reflector configuration. This is used to blink the LED
 *        and thus identify itself
 *
 * \param parameter pass parameters to timer handler
 */
static void identify_timer_handler_cb(void *parameter)
{
	static uint8_t led_count;
	parameter = parameter;
	/* LED Blinking sequence is completed */
	if (led_count > LED_BLINK_COUNT_FOR_IDENTIFY) {
		led_count = 0;
		app_led_event(LED_EVENT_PEER_SEARCH_DONE);
	} else { /* Blink count is not completed  */
		 /* For every timeout switch off and on all LEDs alternatively
		  **/
		if (led_count & 0x01) {
			led_count++;
			app_led_event(LED_EVENT_ALL_OFF);
			sw_timer_stop(APP_TIMER_TO_TX);
		} else {
			led_count++;
			app_led_event(LED_EVENT_ALL_ON);
		}

		sw_timer_start(APP_TIMER_TO_TX,
				LED_BLINK_RATE_IN_MICRO_SEC,
				SW_TIMEOUT_RELATIVE,
				(FUNC_PTR)identify_timer_handler_cb,
				NULL);
	}

	return;
}

/**
 * \brief Timer Callback function  if marker response command is transmitted on
 * air
 *  This is used to blink the LED and thus identify that the transmission is
 * done
 * \param parameter pass parameters to timer handler
 */
void marker_tx_timer_handler_cb(void *parameter)
{
	static uint8_t led_count;
	parameter = parameter;
	/* LED Blinking sequence is completed */
	if (led_count > LED_BLINK_COUNT_FOR_BUTTON_PRESS) {
		led_count = 0;
		app_led_event(LED_EVENT_PEER_SEARCH_DONE);
	} else { /* Blink count is not completed  */
		 /* For every timeout switch off and on all LEDs alternatively
		  **/
		if (led_count & 0x01) {
			led_count++;
			LED_Off(TX_LED);
			sw_timer_stop(APP_TIMER_TO_TX);
		} else {
			led_count++;
			LED_On(TX_LED);
		}

		sw_timer_start(APP_TIMER_TO_TX,
				LED_BLINK_RATE_IN_MICRO_SEC,
				SW_TIMEOUT_RELATIVE,
				(FUNC_PTR)marker_tx_timer_handler_cb,
				NULL);
	}

	return;
}

/**
 * \brief Timer Callback function  if marker command is received on air
 * This is used to blink the LED and thus identify that the marker frame is
 * received
 * \param parameter pass parameters to timer handler
 */
void marker_rsp_timer_handler_cb(void *parameter)
{
	static uint8_t led_count;
	parameter = parameter;
	/* LED Blinking sequence is completed */
	if (led_count > LED_BLINK_COUNT_FOR_BUTTON_PRESS) {
		led_count = 0;
		app_led_event(LED_EVENT_PEER_SEARCH_DONE);
	} else { /* Blink count is not completed  */
		 /* For every timeout switch off and on all LEDs alternatively
		  **/
		if (led_count & 0x01) {
			led_count++;
			LED_Off(RX_LED);
			sw_timer_stop(T_APP_TIMER);
		} else {
			led_count++;
			LED_On(RX_LED);
		}

		sw_timer_start(T_APP_TIMER,
				LED_BLINK_RATE_IN_MICRO_SEC,
				SW_TIMEOUT_RELATIVE,
				(FUNC_PTR)marker_rsp_timer_handler_cb,
				NULL);
	}

	return;
}

/**
 * \brief Timer Callback function  if disconnect command is received on air
 *  by a node in reflector configuration.This is used to do a software reset
 *  on the reflector side.
 *
 * \param parameter pass parameters to timer handler
 */
static void app_reset_cb(void *parameter)
{
	trx_id_t trx = (trx_id_t)parameter;
	app_reset(trx);

	
	printf("\r\nDisconnected from Transmitter node");
}

#ifdef CRC_SETTING_ON_REMOTE_NODE

/**
 * \brief Calculates CRC for a given byte of data
 */
static uint16_t crc_test(uint16_t crc, uint8_t data)
{
	data ^= crc & 0xFF;
	data ^= data << 4;
	return ((((uint16_t)data << 8) | ((crc & 0xFF00) >> 8)) ^ \
	       ((uint8_t)(data >> 4)) ^	\
	       ((uint16_t)data << 3));
}

/**
 * \brief Calculates CRC manually and compares with the received
 * and returns true if both are same,false otherwise.
 */
static bool crc_check_ok(frame_info_t *frame_info)
{
	/* Calculate CRC manually since we are bypassing hardware CRC */
	uint8_t number_of_bytes_rec = (frame_info->mpdu)[0];
	uint16_t cal_crc = 0;
	uint16_t *rec_crc_ptr
		= (uint16_t *)&(frame_info->mpdu)[number_of_bytes_rec - 1 ];
	uint16_t rec_crc = CCPU_ENDIAN_TO_LE16(*rec_crc_ptr);
	uint8_t i;
	for (i = 1; i <= (number_of_bytes_rec - FCS_LEN); i++) {
		cal_crc = crc_test(cal_crc, (frame_info->mpdu)[i]);
	}
	if (rec_crc != cal_crc) {
		return(false);
	}

	return(true);
}

#endif /* End of #ifdef CRC_SETTING_ON_REMOTE_NODE */

/**
 * \brief Function used to send PER test result.
 *
 */
static void send_result_rsp(trx_id_t trx)
{
	uint8_t payload_length;
	app_payload_t msg;
	result_rsp_t *data;


	/* Create the payload */
	msg.cmd_id = RESULT_RSP;
	seq_num_receptor[trx]++;
	msg.seq_num = seq_num_receptor[trx];
	data = (result_rsp_t *)&msg.payload;
	data->num_of_frames_rx = CCPU_ENDIAN_TO_LE32(number_rx_frames[trx]);
	data->lqi_avrg_rx = CCPU_ENDIAN_TO_LE32(aver_lqi[trx]);
	data->rssi_avrg_rx = CCPU_ENDIAN_TO_LE32(aver_rssi[trx]);

	#ifdef CRC_SETTING_ON_REMOTE_NODE
	if ( manual_crc[trx] )
	{
		data->frames_with_wrong_crc = CCPU_ENDIAN_TO_LE32(frames_with_wrong_crc[trx]);
	}
	else
	#endif /* #ifdef CRC_SETTING_ON_REMOTE_NODE */
	{
		/* Set a value of 0xffffffff if we are not counting CRC errors */
		data->frames_with_wrong_crc = CCPU_ENDIAN_TO_LE32((uint32_t)(-1));
	}
	/* Calculate the payload length */
	payload_length = ((sizeof(app_payload_t) -
	sizeof(general_pkt_t)) +
	sizeof(result_rsp_t));

	/* Send the frame to Peer node */
	transmit_frame(trx, FCF_SHORT_ADDR,
	(uint8_t *) & (node_info[trx].peer_short_addr),
	FCF_SHORT_ADDR,
	seq_num_receptor[trx],
	(uint8_t *) &msg,
	payload_length,
	true);
}

/**
 * \brief Function used to send peer_info_rsp command
 *
 */
static void send_peer_info_rsp(trx_id_t trx)
{
    uint8_t payload_length;
    app_payload_t msg;
    peer_info_rsp_t *data;

    /* Create the payload */
    msg.cmd_id = PEER_INFO_RSP;
    seq_num_receptor[trx]++;
    msg.seq_num = seq_num_receptor[trx];
    data = (peer_info_rsp_t *)&msg.payload;
    get_node_info(trx, data);

    payload_length = ((sizeof(app_payload_t) -
                       sizeof(general_pkt_t)) +
                      sizeof(peer_info_rsp_t));

    /* Send the frame to Peer node */
    transmit_frame(trx, FCF_SHORT_ADDR,
                   (uint8_t *) & (node_info[trx].peer_short_addr),
                   FCF_SHORT_ADDR,
                   seq_num_receptor[trx],
                   (uint8_t *) &msg,
                   payload_length,
                   true);

}

/**
 * \brief Function used to send response to the received range test packet
 * \param seq_num sequence number of the range test packet received
 * \param frame_count Count of the received Range Test Packet
 * \param ed ED value of the received range test packet which has to be uploaded
 * into the response payload
 * \param lqi LQI value of the received range test packet which has to be
 * uploaded into the response payload
 */
static void send_range_test_rsp(trx_id_t trx,uint8_t seq_num, uint32_t frame_count,
		int8_t ed, uint8_t lqi)
{
	uint8_t payload_length;
	app_payload_t msg;
	range_tx_t *data;

	/* Create the payload */
	msg.cmd_id = RANGE_TEST_RSP;
	seq_num_receptor[trx]++;
	msg.seq_num = seq_num;
	data = (range_tx_t *)&msg.payload;
	data->frame_count = frame_count;
	data->ed = ed;
	data->lqi = lqi;
	app_led_event(LED_EVENT_TX_FRAME);
	payload_length = ((sizeof(app_payload_t) -
			sizeof(general_pkt_t)) +
			sizeof(range_tx_t));

	/* Send the frame to Peer node */
	transmit_frame(trx,FCF_SHORT_ADDR,
			(uint8_t *)&(node_info[trx].peer_short_addr),
			FCF_SHORT_ADDR,
			seq_num_receptor[trx],
			(uint8_t *)&msg,
			payload_length,
			true);
}

/**
 * \brief Function used to set default configurations on peer node on reception of
 * set_default req
 *
 */
static void set_default_configuration_peer_node(trx_id_t trx)
{
    uint8_t temp_ch,temp_pg,temp;

    /* Channel default configuration  */
	if(trx == RF24)
	{
		temp_ch = DEFAULT_CHANNEL_RF24;
		temp_pg = TAL_CURRENT_PAGE_DEFAULT_RF24;
	}
	else
	{
		temp_ch = DEFAULT_CHANNEL_RF09;
		temp_pg = TAL_CURRENT_PAGE_DEFAULT_RF09;
	}
	
	tal_pib_set(trx,phyCurrentChannel, (pib_value_t *)&temp_ch);

	 /* Channel page default configuration*/

	tal_pib_set(trx,phyCurrentPage, (pib_value_t *)&temp_pg);


		/* Tx power default configurations */
	temp = TAL_TRANSMIT_POWER_DEFAULT;
	tal_pib_set(trx,phyTransmitPower, (pib_value_t *)&temp);
	    /* antenna diversity default configurations */
#if(ANTENNA_DIVERSITY == 1)

		/* Disable antenna diversity by default */
	    tal_ant_div_config(trx,ANT_DIVERSITY_DISABLE,ANT_CTRL_1); /* Enable A1/X2 */

#endif

    /* CRC default configuration */
#ifdef CRC_SETTING_ON_REMOTE_NODE
    manual_crc[trx] = false;
    /* Disable the Promiscuous Mode */
     tal_rxaack_prom_mode_ctrl(trx,AACK_PROM_DISABLE);

#endif
}
/**
 * \brief Function used to get the board details of peer node
 *
 * \param pointer to the peer_info_rsp_t structure
 */
static void get_node_info(trx_id_t trx, peer_info_rsp_t *data)
{

    if(trx == RF09)
	{
		strcpy(data->trx_name, "AT86RF215-LT(RF09)"); 
	}
	else 
	{
		strcpy(data->trx_name, "AT86RF215-LT(RF24)");
	}
	
    data->ic_type = IC_TYPE;
    strcpy(data->soc_mcu_name, MCU_SOC_NAME);
    strcpy(data->board_name, BOARD_NAME);
    /* Get the MAC address of the node */
    data->mac_address = tal_pib[trx].IeeeAddress;
}



#if (ANTENNA_DIVERSITY == 1)
/**
 * \brief Function used to send diversity settings on remote node to host node
 */
static void send_diversity_status_rsp(trx_id_t trx)
{
    uint8_t payload_length;
    app_payload_t msg;
	uint8_t temp;
    div_stat_rsp_t *data;

    /* Create the payload */
    msg.cmd_id = DIV_STAT_RSP;
    seq_num_receptor[trx]++;
    msg.seq_num = seq_num_receptor[trx];
    data = (div_stat_rsp_t *)&msg.payload;

    uint8_t curr_ant_div;
    tal_get_curr_trx_config(trx,ANT_DIVERSITY,&curr_ant_div);
    if(curr_ant_div)
    {
        data->status = 1;
        data->ant_sel = 0;
    }
    else
    {
        data->status = 0;
        tal_get_curr_trx_config(trx,ANT_CTRL,&temp);
		data->ant_sel = temp;
    }
    /* Calculate the payload length */
    payload_length = ((sizeof(app_payload_t) -
                       sizeof(general_pkt_t)) +
                      sizeof(div_stat_rsp_t));
    /* Send the frame to Peer node */
    transmit_frame(trx, FCF_SHORT_ADDR,
                   (uint8_t *) & (node_info[trx].peer_short_addr),
                   FCF_SHORT_ADDR,
                   seq_num_receptor[trx],
                   (uint8_t *) &msg,
                   payload_length,
                   true);
}
#endif /* End of (ANTENNA_DIVERSITY == 1) */


#ifdef CRC_SETTING_ON_REMOTE_NODE
/**
 * \brief Function used to send crc settings on remote node to host node
 */
static void send_crc_status_rsp(trx_id_t trx)
{
    uint8_t payload_length;
    app_payload_t msg;
    crc_stat_rsp_t *data;

    /* Create the payload */
    msg.cmd_id = CRC_STAT_RSP;
    seq_num_receptor[trx]++;
    msg.seq_num = seq_num_receptor[trx];
    data = (crc_stat_rsp_t *)&msg.payload;

    tal_get_curr_trx_config(trx,AACK_PROMSCS_MODE,&(data->status));

    /* Calculate the payload length */
    payload_length = ((sizeof(app_payload_t) -
                       sizeof(general_pkt_t)) +
                      sizeof(crc_stat_rsp_t));

    /* Send the frame to Peer node */
    transmit_frame(trx, FCF_SHORT_ADDR,
                   (uint8_t *) & (node_info[trx].peer_short_addr),
                   FCF_SHORT_ADDR,
                   seq_num_receptor[trx],
                   (uint8_t *) &msg,
                   payload_length,
                   true);
}
#endif /* End of #ifdef CRC_SETTING_ON_REMOTE_NODE */

/*EOF */