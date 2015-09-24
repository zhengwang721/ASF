/*
 * dbg_task.h
 *
 *  Created on: Oct 28, 2014
 *      Author: lali
 */
#ifndef DBG_TASK_H_
#define DBG_TASK_H_

#include "cmn_defs.h"

/**
* @defgroup dbg_task_group DEBUG APIs
* This group includes all debug APIs; read reg and write reg functions
* @{
*/
/** @}*/

///@cond IGNORE_DOXYGEN
#define RPL_POWER_MIN 1
#define RPL_POWER_MAX 6
#define RPL_PWR_TBL_SIZE 0x0f

/// dbg Task messages
enum dbg_msg_id
{
    // Debug commands description.
    DBG_MSG_ID_CMD_FIRST = ((uint16_t)(TASK_DBG << 8)),
    DBG_RD_MEM_REQ = DBG_MSG_ID_CMD_FIRST,
    DBG_WR_MEM_REQ,
    DBG_DEL_PARAM_REQ,
    DBG_RD_PARAM_REQ,
    DBG_WR_PARAM_REQ,
    DBG_PLF_RESET_REQ,
    DBG_FLASH_IDENT_REQ,
    DBG_FLASH_ERASE_REQ,
    DBG_FLASH_WRITE_REQ,
    DBG_FLASH_READ_REQ,

    DBG_RD_KE_STATS_REQ,
    DBG_RF_REG_RD_REQ,
    DBG_RF_REG_WR_REQ,
    DBG_HW_REG_RD_REQ,
    DBG_HW_REG_WR_REQ,

    DBG_LE_SET_BD_ADDR_REQ,
    DBG_LE_SET_TYPE_PUB_REQ,
    DBG_LE_SET_TYPE_RAND_REQ,
    DBG_LE_SET_CRC_REQ,
    DBG_LE_LLCP_DISCARD_REQ,
    DBG_LE_RESET_RX_CNT_REQ,
    DBG_LE_RESET_TX_CNT_REQ,
    DBG_SET_TX_PW_REQ,

    DBG_WLAN_COEX_REQ,

    DBG_RD_MEM_INFO_REQ,

    DBG_MSG_ID_EVT_FIRST,
};

enum dbg_evt_id
{
    DBG_RD_MEM_CMP_EVT = DBG_MSG_ID_EVT_FIRST,
    DBG_WR_MEM_CMP_EVT,
    DBG_DEL_PARAM_CMP_EVT,
    DBG_RD_PARAM_CMP_EVT,
    DBG_WR_PARAM_CMP_EVT,
    DBG_FLASH_IDENT_CMP_EVT,
    DBG_FLASH_ERASE_CMP_EVT,
    DBG_FLASH_WRITE_CMP_EVT,
    DBG_FLASH_READ_CMP_EVT,

    DBG_RD_KE_STATS_CMP_EVT,
    DBG_TRACE_WARNING_EVT,
    DBG_RF_REG_RD_CMP_EVT,
    DBG_RF_REG_WR_CMP_EVT,
    DBG_PLF_RESET_CMP_EVT,
    DBG_HW_REG_RD_CMP_EVT,
    DBG_HW_REG_WR_CMP_EVT,

    DBG_LE_SET_BD_ADDR_CMP_EVT,
    DBG_LE_SET_TYPE_PUB_CMP_EVT,
    DBG_LE_SET_TYPE_RAND_CMP_EVT,
    DBG_LE_SET_CRC_CMP_EVT,
    DBG_LE_LLCP_DISCARD_CMP_EVT,
    DBG_LE_RESET_RX_CNT_CMP_EVT,
    DBG_LE_RESET_TX_CNT_CMP_EVT,
    DBG_SET_TX_PW_CMP_EVT,

    DBG_WLAN_COEX_CMP_EVT,

    DBG_RD_MEM_INFO_CMP_EVT,

    DBG_MSG_ID_EVT_LAST
};

at_ble_status_t dbg_le_set_bd_addr_req_handler(uint8_t *addr);

uint8_t dbg_set_tx_pw_req_handler(uint16_t conn_handle, uint8_t level);
///@endcond

/** @ingroup dbg_task_group
  *@brief Write data chunk to specific memory address
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
///@cond IGNORE_DOXYGEN
AT_BLE_API
///@endcond
uint8_t dbg_wr_mem_req_handler(uint32_t u32MemAddr, uint8_t *pu8Data,
                               uint8_t u8Size, uint8_t u8WriteMode, uint8_t bDisWait);

/** @ingroup dbg_task_group
  *@brief Read data chunk from specific memory address
  *
  * @param[in] u32MemAddr Memory address [32Bit Address]
  * @param[in] pu8Data pointer to data buffer
  * @param[in] u8Size length of data
  * @param[in] u8ReadMode either 8Bit\32Bit Modes, selected by 8 or 32
  *
  * @return @ref AT_BLE_SUCCESS operation completed successfully
  * @return @ref AT_BLE_FAILURE Generic error.
  */
///@cond IGNORE_DOXYGEN
AT_BLE_API
///@endcond
uint8_t dbg_rd_mem_req_handler(uint32_t u32MemAddr, uint8_t *pu8Data,
                               uint8_t u8Size, uint8_t u8ReadMode);

#endif

