#include "at_ble_api.h"
#include "platform.h"
#include "interface.h"

#include "dbg_task.h"

at_ble_status_t dbg_le_set_bd_addr_req_handler(uint8_t *addr)
{
    uint8_t u8Status = 0;
    uint16_t delay = 0;
    do
    {
        delay++;
    }
    while (delay < 0xFFFE);
    INTERFACE_MSG_INIT(DBG_LE_SET_BD_ADDR_REQ, TASK_DBG);
    INTERFACE_PACK_ARG_BLOCK(addr , AT_BLE_ADDR_LEN);
    INTERFACE_SEND_WAIT(DBG_LE_SET_BD_ADDR_CMP_EVT, TASK_DBG);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_DONE();
    return (at_ble_status_t)u8Status;
}

/**@brief Write data chunk to specific memory address
  *
  * @param[in] u32MemAddr Memory address [32Bit Address]
  * @param[in] pu8Data pointer to data buffer
  * @param[in] u8Size length of data
  * @param[in] u8WriteMode either 8Bit\32Bit Modes, selected by 8 or 32
  * @param[in] bDisWait if true this means function wont be blocking otherwise it will be blocked waiting for data
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
uint8_t dbg_wr_mem_req_handler(uint32_t u32MemAddr, uint8_t *pu8Data, uint8_t u8Size, uint8_t u8WriteMode, uint8_t bDisWait)
{
    uint8_t u8Status = 0;
    INTERFACE_MSG_INIT(DBG_WR_MEM_REQ, TASK_DBG);
    INTERFACE_PACK_ARG_UINT32(u32MemAddr);
    INTERFACE_PACK_ARG_UINT8(u8WriteMode);
    INTERFACE_PACK_ARG_UINT8(u8Size);
    INTERFACE_PACK_ARG_BLOCK(pu8Data, u8Size);
    switch (bDisWait)
    {
    default:
    case 0:
        INTERFACE_SEND_WAIT(DBG_WR_MEM_CMP_EVT, TASK_DBG);
        INTERFACE_UNPACK_UINT8(&u8Status);
        break;
    case 1:
        INTERFACE_SEND_NO_WAIT();
        break;
    }
    INTERFACE_DONE();
    return u8Status;
}

/**@brief Read data chunk from specific memory address
  *
  * @param[in] u32MemAddr Memory address [32Bit Address]
  * @param[in] pu8Data pointer to data buffer
  * @param[in] u8Size length of data
  * @param[in] u8ReadMode either 8Bit\32Bit Modes, selected by 8 or 32
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
uint8_t dbg_rd_mem_req_handler(uint32_t u32MemAddr, uint8_t *pu8Data, uint8_t u8Size, uint8_t u8ReadMode)
{
    uint8_t u8Status = 0;
    uint8_t u8DataSize = 0;
		/* Argused*/
		u8DataSize = u8DataSize;
	
    INTERFACE_MSG_INIT(DBG_RD_MEM_REQ, TASK_DBG);
    INTERFACE_PACK_ARG_UINT32(u32MemAddr);
    INTERFACE_PACK_ARG_UINT8(u8ReadMode);
    INTERFACE_PACK_ARG_UINT8(u8Size);
    INTERFACE_SEND_WAIT(DBG_RD_MEM_CMP_EVT, TASK_DBG);
    INTERFACE_UNPACK_UINT8(&u8Status);
    INTERFACE_UNPACK_UINT8(&u8DataSize);
    ASSERT_PRINT_ERR(u8DataSize != u8Size, "Data Size : %d\n", u8DataSize);
    INTERFACE_UNPACK_BLOCK(pu8Data, u8Size);
    INTERFACE_DONE();
    return u8Status;
}
