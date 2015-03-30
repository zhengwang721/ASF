
#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "dbg_task.h"

uint8_t dbg_le_set_bd_addr_req_handler(uint8_t* addr)
{
	uint8_t u8Status;
#ifndef __WINC3000__
	uint32_t chip_id = 0;
	uint32_t gap_dev_addr;
#endif
	do
	{
		INTERFACE_MSG_INIT(DBG_LE_SET_BD_ADDR_REQ, TASK_DBG);
		INTERFACE_PACK_ARG_BLOCK(addr , AT_BLE_ADDR_LEN);
		INTERFACE_SEND_WAIT(DBG_LE_SET_BD_ADDR_CMP_EVT, TASK_DBG);
		INTERFACE_UNPACK_UINT8(&u8Status);
		INTERFACE_DONE();

		if(u8Status != AT_BLE_SUCCESS)
			break;
#ifndef __WINC3000__
		u8Status = dbg_rd_mem_req_handler(0x4000B000,(uint8_t *)&chip_id,4);
		if(u8Status != AT_BLE_SUCCESS)
			break;

		chip_id&=0xFFFFFF00;
		switch(chip_id)
		{
			case 0x2000a000 :
				gap_dev_addr = 0x10000E26;
				break;
			case 0x2000a200 :
				gap_dev_addr = 0x10001096;
				break;
			case 0x2000a300 :
				gap_dev_addr = 0x10001156;
				break;
			case 0x2000a400 :
				gap_dev_addr = 0x1000085A;
				break;
		}
		if(chip_id != 0)
		{
			u8Status = dbg_wr_mem_req_handler(gap_dev_addr,addr,AT_BLE_ADDR_LEN);
		}
#endif //__WINC3000__
	}while(0);
	return u8Status;
}

uint8_t dbg_set_tx_pw_req_handler(uint16_t conn_handle, uint8_t level)
{
	uint8_t u8Status;
	
	INTERFACE_MSG_INIT(DBG_SET_TX_PW_REQ, TASK_DBG);
	INTERFACE_PACK_ARG_UINT16(conn_handle);
	INTERFACE_PACK_ARG_UINT8(level);
	INTERFACE_SEND_WAIT(DBG_SET_TX_PW_CMP_EVT, TASK_DBG);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();

	return u8Status;
}

uint8_t dbg_wr_mem_req_handler(uint32_t memAddr,uint8_t* data,uint8_t size)
{
	uint8_t u8Status;
	
	INTERFACE_MSG_INIT(DBG_WR_MEM_REQ, TASK_DBG);
	INTERFACE_PACK_ARG_UINT32(memAddr);
	INTERFACE_PACK_ARG_UINT8(8);
	INTERFACE_PACK_ARG_UINT8(size);
	INTERFACE_PACK_ARG_BLOCK(data,size);
	INTERFACE_SEND_WAIT(DBG_WR_MEM_CMP_EVT, TASK_DBG);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	
	return u8Status;
}

void dbg_wr_mem_req_handler32_reset(uint32_t memAddr,uint32_t * data,uint8_t size)
{
	INTERFACE_MSG_INIT(DBG_WR_MEM_REQ, TASK_DBG);
	INTERFACE_PACK_ARG_UINT32(memAddr);
	INTERFACE_PACK_ARG_UINT8(32);
	INTERFACE_PACK_ARG_UINT8(size);
	INTERFACE_PACK_ARG_BLOCK(data,size);
	INTERFACE_SEND_NO_WAIT();
	INTERFACE_DONE();	
}

uint8_t dbg_wr_mem_req_handler32(uint32_t memAddr,uint32_t* data,uint8_t size)
{
	uint8_t u8Status;
	
	INTERFACE_MSG_INIT(DBG_WR_MEM_REQ, TASK_DBG);
	INTERFACE_PACK_ARG_UINT32(memAddr);
	INTERFACE_PACK_ARG_UINT8(32);
	INTERFACE_PACK_ARG_UINT8(size);
	INTERFACE_PACK_ARG_BLOCK(data,size);
	INTERFACE_SEND_WAIT(DBG_WR_MEM_CMP_EVT, TASK_DBG);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_DONE();
	
	return u8Status;
}

uint8_t dbg_rd_mem_req_handler(uint32_t memAddr,uint8_t* data ,uint8_t size)
{
	uint8_t u8Status;
	
	INTERFACE_MSG_INIT(DBG_RD_MEM_REQ, TASK_DBG);
	INTERFACE_PACK_ARG_UINT32(memAddr);
	INTERFACE_PACK_ARG_UINT8(8);
	INTERFACE_PACK_ARG_UINT8(size);
	INTERFACE_SEND_WAIT(DBG_RD_MEM_CMP_EVT, TASK_DBG);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_UNPACK_BLOCK(data,size);
	INTERFACE_DONE();
	
	return u8Status;
}

uint8_t dbg_rd_mem_req_handler32(uint32_t memAddr,uint8_t* data ,uint8_t size)
{
	uint8_t u8Status;
	
	INTERFACE_MSG_INIT(DBG_RD_MEM_REQ, TASK_DBG);
	INTERFACE_PACK_ARG_UINT32(memAddr);
	INTERFACE_PACK_ARG_UINT8(32);
	INTERFACE_PACK_ARG_UINT8(size);
	INTERFACE_SEND_WAIT(DBG_RD_MEM_CMP_EVT, TASK_DBG);
	INTERFACE_UNPACK_UINT8(&u8Status);
	INTERFACE_UNPACK_BLOCK(data,size);
	INTERFACE_DONE();
	
	return u8Status;
}