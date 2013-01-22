#include "compiler.h"
#include "api_parser.h"
#include "sio2ncp.h"
#include "common_sw_timer.h"



#define SIO_RX_BUF_SIZE 156
 uint8_t data[SIO_RX_BUF_SIZE];
 uint8_t data_length = 0;

static uint8_t rx_index = 0;

static uint8_t rcv_buffer[RX_BUFFER_LENGTH] = {0};
static uint8_t *rcv_buff_ptr;
static uint8_t *rcv_frame_ptr = rcv_buffer+1;
static uint8_t rcv_state = UART_RX_STATE_SOT, rcv_length = 0;


void handle_rx_frame(void)
{
  
    
	switch (*(rcv_frame_ptr + 1))  /* message type */
    {
        case MCPS_DATA_CONFIRM:
#if defined(ENABLE_TSTAMP)
			usr_mcps_data_conf(*(rcv_frame_ptr + 2), *(rcv_frame_ptr + 3),
							   *((uint32_t*)(rcv_frame_ptr + 4)));
#else
			usr_mcps_data_conf(*(rcv_frame_ptr + 2), *(rcv_frame_ptr + 3));
#endif  /* ENABLE_TSTAMP */
				break;

		case MCPS_DATA_INDICATION:
			usr_mcps_data_ind((wpan_addr_spec_t*)(rcv_frame_ptr + 2),
							  (wpan_addr_spec_t*)(rcv_frame_ptr + 2 + sizeof(wpan_addr_spec_t)),
							  *(rcv_frame_ptr + 8 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t)),
							  (rcv_frame_ptr + 10 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t)),
							  *(rcv_frame_ptr + 2 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t)),
    #ifdef ENABLE_TSTAMP
							  *(rcv_frame_ptr + 3 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t)),
							  *((uint32_t *)(rcv_frame_ptr + 4 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t))));
    #else
							  *(rcv_frame_ptr + 3 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t)),
    #endif  /* ENABLE_TSTAMP */
				break;

        case MCPS_PURGE_CONFIRM:
#if ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1))
			usr_mcps_purge_conf(*(rcv_frame_ptr + 2), *(rcv_frame_ptr + 3));
#endif  /* ((MAC_PURGE_REQUEST_CONFIRM == 1) && (MAC_INDIRECT_DATA_BASIC == 1)) */
			break;

        case MLME_ASSOCIATE_INDICATION:
#if (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)
			usr_mlme_associate_ind(*((uint64_t *)(rcv_frame_ptr + 2)),
								   *(rcv_frame_ptr + 2 + sizeof(uint64_t)));
#endif  /* (MAC_ASSOCIATION_INDICATION_RESPONSE == 1) */
			break;

        case MLME_ASSOCIATE_CONFIRM:
#if (MAC_ASSOCIATION_REQUEST_CONFIRM == 1)
			usr_mlme_associate_conf(*((uint16_t *)(rcv_frame_ptr + 2)),
									*(rcv_frame_ptr + 2 + sizeof(uint16_t)));
#endif  /* (MAC_ASSOCIATION_REQUEST_CONFIRM == 1) */
			break;

        case MLME_DISASSOCIATE_INDICATION:
#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)
			usr_mlme_disassociate_ind(*((uint64_t *)(rcv_frame_ptr + 2)),
									  *(rcv_frame_ptr + 2 + sizeof(uint64_t)));
#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */
			break;

        case MLME_DISASSOCIATE_CONFIRM:
#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)
			usr_mlme_disassociate_conf(*(rcv_frame_ptr + 2),
									  (wpan_addr_spec_t *)(rcv_frame_ptr + 3));
#endif /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */
			break;

        case MLME_BEACON_NOTIFY_INDICATION:
#if (MAC_BEACON_NOTIFY_INDICATION == 1)
			usr_mlme_beacon_notify_ind(*(rcv_frame_ptr + 2),
									   (wpan_pandescriptor_t *)(rcv_frame_ptr + 3),
									   *(rcv_frame_ptr + 3 + WPAN_DESCRIPTOR_LEN),
									   (rcv_frame_ptr + 4 + WPAN_DESCRIPTOR_LEN),
									   *(rcv_frame_ptr + 4 + WPAN_DESCRIPTOR_LEN),
									   (rcv_frame_ptr + 5 + WPAN_DESCRIPTOR_LEN));
#endif  /* (MAC_BEACON_NOTIFY_INDICATION == 1) */
			break;

        case MLME_ORPHAN_INDICATION:
#if (MAC_ORPHAN_INDICATION_RESPONSE == 1)
			usr_mlme_orphan_ind(*((uint64_t *)(rcv_frame_ptr + 2)));
#endif  /* (MAC_ORPHAN_INDICATION_RESPONSE == 1) */
			break;

        case MLME_SCAN_CONFIRM:
#if ((MAC_SCAN_ED_REQUEST_CONFIRM == 1)      || \
     (MAC_SCAN_ACTIVE_REQUEST_CONFIRM == 1)  || \
     (MAC_SCAN_PASSIVE_REQUEST_CONFIRM == 1) || \
     (MAC_SCAN_ORPHAN_REQUEST_CONFIRM == 1))
			usr_mlme_scan_conf(*(rcv_frame_ptr + 2),
                        *(rcv_frame_ptr + 3),
                        *(rcv_frame_ptr + 4),
                        *((uint32_t *)(rcv_frame_ptr + 5)),
                        *(rcv_frame_ptr + 5 + sizeof(uint32_t)),
                        (void *)((rcv_frame_ptr + 7 + sizeof(uint32_t))));
#endif
			break;

        case MLME_COMM_STATUS_INDICATION:
#if ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || (MAC_ASSOCIATION_INDICATION_RESPONSE == 1))
			usr_mlme_comm_status_ind((wpan_addr_spec_t *)(rcv_frame_ptr + 2),
                              (wpan_addr_spec_t *)(rcv_frame_ptr + 2 + sizeof(wpan_addr_spec_t)),
                              *(rcv_frame_ptr + 2 + sizeof(wpan_addr_spec_t) + sizeof(wpan_addr_spec_t)));
#endif  /* ((MAC_ORPHAN_INDICATION_RESPONSE == 1) || (MAC_ASSOCIATION_INDICATION_RESPONSE == 1)) */
			break;

        case MLME_SYNC_LOSS_INDICATION:
			usr_mlme_sync_loss_ind(*(rcv_frame_ptr + 2),
                            *((uint16_t *)(rcv_frame_ptr + 3)),
                            *(rcv_frame_ptr + 3 + sizeof(uint16_t)),
                            *(rcv_frame_ptr + 4 + sizeof(uint16_t)));
			break;

        case MLME_GET_CONFIRM:
#if (MAC_GET_SUPPORT == 1)
			usr_mlme_get_conf(*(rcv_frame_ptr + 2),
							  *(rcv_frame_ptr + 3),
	                         (rcv_frame_ptr + 5));
#endif  /* (MAC_GET_SUPPORT == 1) */
			break;

        case MLME_SET_CONFIRM:
			usr_mlme_set_conf(*(rcv_frame_ptr + 2),
							  *(rcv_frame_ptr + 3));
			break;

        case MLME_RESET_CONFIRM:
			usr_mlme_reset_conf(*(rcv_frame_ptr + 2));
			break;

        case MLME_RX_ENABLE_CONFIRM:
#if (MAC_RX_ENABLE_SUPPORT == 1)
			usr_mlme_rx_enable_conf(*(rcv_frame_ptr + 2));
#endif  /* (MAC_RX_ENABLE_SUPPORT == 1) */
			break;

        case MLME_START_CONFIRM:
#if (MAC_START_REQUEST_CONFIRM == 1)
			usr_mlme_start_conf(*(rcv_frame_ptr + 2));
#endif  /* (MAC_START_REQUEST_CONFIRM == 1) */
			break;

        case MLME_POLL_CONFIRM:
#if (MAC_INDIRECT_DATA_BASIC == 1)
			usr_mlme_poll_conf(*(rcv_frame_ptr + 2));
#endif  /* (MAC_INDIRECT_DATA_BASIC == 1) */
			break;

		default:
		
			break;
	}
}


void process_rf_serial_byte(uint8_t byte)
{
    switch (rcv_state)
    {
        case UART_RX_STATE_SOT:
            rcv_buff_ptr = rcv_buffer;
            if (SOT == byte)
            {
                rcv_state = UART_RX_STATE_LENGTH;
            }
            break;

        case UART_RX_STATE_LENGTH:
            rcv_length = byte;
            if (rcv_length)
            {
                rcv_state = UART_RX_STATE_DATA;
                *rcv_buff_ptr = rcv_length;
                rcv_buff_ptr++;
            }
            else
            {
                /* NULL message */
                rcv_buff_ptr = rcv_buffer;
                rcv_state = UART_RX_STATE_SOT;
            }
            break;

        case UART_RX_STATE_DATA:
            *rcv_buff_ptr = byte;
            rcv_buff_ptr++;
            rcv_length--;
            if (!rcv_length)
            {
                rcv_state = UART_RX_STATE_EOT;
            }
            break;

        case UART_RX_STATE_EOT:
            if (EOT == byte)
            {
                /* Message received successfully */
                handle_rx_frame();
            }
            /* Make rx buffer ready for next reception before handling received data. */
            rcv_buff_ptr = rcv_buffer;
            rcv_state = UART_RX_STATE_SOT;
            break;

        default:
            rcv_buff_ptr = rcv_buffer;
            rcv_state = UART_RX_STATE_SOT;
            break;
    }
}


bool wpan_task(void)
{
      /* Rx processing */
    if (data_length == 0)
    {
        /* No data to process, read the stream IO */
        rx_index = 0;
        data_length = sio2ncp_rx(data, SIO_RX_BUF_SIZE);    // @ToDo 20 ?, different values for USB and UART ?
    }
    else    /* Data has been received, process the data */
    {
        /* Process each single byte */
        process_incoming_sio_data();
        data_length--;
        rx_index++;
    }
    
    sw_timer_service();


	return true;
}

/**
 * @brief Process data received from SIO
 */
static void process_incoming_sio_data(void)
{
    
     switch (rcv_state)
    {
        case UART_RX_STATE_SOT:
            rcv_buff_ptr = rcv_buffer;
             if (SOT == data[rx_index])
            {
                rcv_state = UART_RX_STATE_LENGTH;
            }
            
            break;

        case UART_RX_STATE_LENGTH:
           rcv_length = data[rx_index];
            if (rcv_length)
            {
            
                rcv_state = UART_RX_STATE_DATA;
                *rcv_buff_ptr = rcv_length;
                rcv_buff_ptr++;
            }
            else
            {
                /* NULL message */
                rcv_buff_ptr = rcv_buffer;
                rcv_state = UART_RX_STATE_SOT;
            }
            break;

        case UART_RX_STATE_DATA:
            *rcv_buff_ptr = data[rx_index];
            rcv_buff_ptr++;
            rcv_length--;
            if (!rcv_length)
            {
                rcv_state = UART_RX_STATE_EOT;
            }
            break;
            


        case UART_RX_STATE_EOT:
             if (EOT == data[rx_index])
            {
                /* Message received successfully */
                handle_rx_frame();
            }
            /* Make rx buffer ready for next reception before handling received data. */
            rcv_buff_ptr = rcv_buffer;
            rcv_state = UART_RX_STATE_SOT;
            break;

        default:
            rcv_buff_ptr = rcv_buffer;
            rcv_state = UART_RX_STATE_SOT;
            break;
    }
    
}
