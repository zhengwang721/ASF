#include "dbg_logs.h"

/*******************************************************
 ********************* VARIABLES ***********************
 *******************************************************
 */
#if USE_TRACE_LOGS_VER  == VER_1_1
#if (AT_BLE_DBG != TRACE_LVL_DISABLE)
static int __printingNullFun(const char *_Format, ...)
{
    return 0;
}

//Genral pointer to printing function
static tpfTrace pfTrace = &__printingNullFun;
#endif //AT_BLE_DBG != TRACE_LVL_DISABLE

#if (AT_BLE_DBG & TRACE_LVL_INFO) == TRACE_LVL_INFO
tpfTrace tpfAtBlePrintInfo = (tpfTrace) &__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_INFO

#if (AT_BLE_DBG & TRACE_LVL_ERR) == TRACE_LVL_ERR
tpfTrace tpfAtBlePrintErr = (tpfTrace) &__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_ERR

#if (AT_BLE_DBG & TRACE_LVL_DBG) == TRACE_LVL_DBG
tpfTrace tpfAtBlePrintDbg = (tpfTrace) &__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_DBG

#if (AT_BLE_DBG & TRACE_LVL_BUS) == TRACE_LVL_BUS
tpfTrace tpfAtBlePrintBus = (tpfTrace) &__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_BUS

#if (AT_BLE_DBG & TRACE_LVL_FUN) == TRACE_LVL_FUN
tpfTrace tpfAtBlePrintFns = (tpfTrace) &__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_FUN

#elif USE_TRACE_LOGS_VER == VER_1_0 //USE_TRACE_LOGS_VER

uint8_t u8AT_BLE_Tracelevel;
tpfTrace pfTrace = '\0';

#endif //USE_TRACE_LOGS_VER


/*******************************************************
 ********************* FUNCTIONS ***********************
 *******************************************************
 */
void trace_register_printFn(void *pf)
{
#if USE_TRACE_LOGS_VER  == VER_1_1
#if (AT_BLE_DBG != TRACE_LVL_DISABLE)
    if (0 != pf)
    {
        pfTrace = pf;
    }
    else
    {
        pfTrace = &__printingNullFun;
    }
#endif //AT_BLE_DBG != TRACE_LVL_DISABLE
#elif USE_TRACE_LOGS_VER == VER_1_0 //USE_TRACE_LOGS_VER
#if (AT_BLE_DBG != 0)
    pfTrace = pf;
#endif //(AT_BLE_DBG == 1)
#endif //USE_TRACE_LOGS_VER
    return;
}

void trace_set_level(uint8_t lvl)
{
#if USE_TRACE_LOGS_VER  == VER_1_1
#if (AT_BLE_DBG & TRACE_LVL_INFO) == TRACE_LVL_INFO
    tpfAtBlePrintInfo = (lvl & TRACE_LVL_INFO) ? pfTrace : (tpfTrace)&__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_INFO
#if (AT_BLE_DBG & TRACE_LVL_DBG) == TRACE_LVL_DBG
    tpfAtBlePrintDbg  = (lvl & TRACE_LVL_DBG)  ? pfTrace : (tpfTrace)&__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_DBG
#if (AT_BLE_DBG & TRACE_LVL_ERR) == TRACE_LVL_ERR
    tpfAtBlePrintErr  = (lvl & TRACE_LVL_ERR)  ? pfTrace : (tpfTrace)&__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_ERR
#if (AT_BLE_DBG & TRACE_LVL_BUS) == TRACE_LVL_BUS
    tpfAtBlePrintBus  = (lvl & TRACE_LVL_BUS)  ? pfTrace : (tpfTrace)&__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_BUS
#if (AT_BLE_DBG & TRACE_LVL_FUN) == TRACE_LVL_FUN
    tpfAtBlePrintFns  = (lvl & TRACE_LVL_FUN)  ? pfTrace : (tpfTrace)&__printingNullFun;
#endif //AT_BLE_DBG & TRACE_LVL_FUN
#elif USE_TRACE_LOGS_VER == VER_1_0 //USE_TRACE_LOGS_VER
#if (AT_BLE_DBG != TRACE_LVL_DISABLE)
    u8AT_BLE_Tracelevel = lvl;
#endif //(AT_BLE_DBG != TRACE_LVL_DISABLE)
#endif //USE_TRACE_LOGS_VER
    return;
}
