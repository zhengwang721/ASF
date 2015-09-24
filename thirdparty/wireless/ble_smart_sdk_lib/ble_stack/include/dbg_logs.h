#ifndef __DBG_LOGS_H__
#define __DBG_LOGS_H__

#include "at_ble_api.h"
#include "at_ble_trace.h"
#include "string.h"

#define VER_1_0 1       //Optimized for Speed
#define VER_1_1 2       //Optimized for Code Size

#define USE_TRACE_LOGS_VER VER_1_1
#define __SHORT_FILE__ strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__
#define HDR_TEXT(Type, End) "LIB("Type") %-16s(%03d) %32s()"End
typedef int (*tpfTrace)(const char *_Format, ...);

#if USE_TRACE_LOGS_VER  == VER_1_1

#if (AT_BLE_DBG & TRACE_LVL_INFO) == TRACE_LVL_INFO
extern tpfTrace tpfAtBlePrintInfo;

/// Print info message; ex: PRINT_INFO(MESSAGE) -> (INF)(FUNCTON_NAME)(LINE_NUMBER)MESSAGE
#define PRINT_INFO(...)     tpfAtBlePrintInfo(HDR_TEXT("I", " : "), __SHORT_FILE__, __LINE__, __FUNCTION__);tpfAtBlePrintInfo(__VA_ARGS__)
#else //AT_BLE_DBG & TRACE_LVL_INFO
#define PRINT_INFO(...)
#endif //AT_BLE_DBG & TRACE_LVL_INFO

#if (AT_BLE_DBG & TRACE_LVL_ERR) == TRACE_LVL_ERR
extern tpfTrace tpfAtBlePrintErr;
/// Print info message; ex: PRINT_ERR(MESSAGE) -> (ERR)(FUNCTON_NAME)(LINE_NUMBER)MESSAGE
#define PRINT_ERR(...)      tpfAtBlePrintErr(HDR_TEXT("E", " : "), __SHORT_FILE__, __LINE__, __FUNCTION__);tpfAtBlePrintErr(__VA_ARGS__)
#define ASSERT_PRINT_ERR(c,...) do{if(c){PRINT_ERR(__VA_ARGS__);}}while(0)
#else //AT_BLE_DBG & TRACE_LVL_ERR
#define PRINT_ERR(...)
#define ASSERT_PRINT_ERR(c,...) do{}while(0)
#endif //AT_BLE_DBG & TRACE_LVL_ERR

#if (AT_BLE_DBG & TRACE_LVL_DBG) == TRACE_LVL_DBG
extern tpfTrace tpfAtBlePrintDbg;
/// Print info message; ex: PRINT_DBG(MESSAGE) -> (DBG)(FUNCTON_NAME)(LINE_NUMBER)MESSAGE
#define PRINT_DBG(...)      tpfAtBlePrintDbg(HDR_TEXT("D", " : "), __SHORT_FILE__, __LINE__, __FUNCTION__);tpfAtBlePrintDbg(__VA_ARGS__)
#else //AT_BLE_DBG & TRACE_LVL_DBG
#define PRINT_DBG(...)
#endif //AT_BLE_DBG & TRACE_LVL_DBG

#if (AT_BLE_DBG & TRACE_LVL_BUS) == TRACE_LVL_BUS
extern tpfTrace tpfAtBlePrintBus;
/// Print trasfered message on bus; ex: PRINT_BUS("TAG: ", arr, 2) -> TAG: arr[0] arr[1]
#define PRINT_BUS(TAG,BUFF,LEN)     \
    do{ \
        uint16_t counter=0; \
        tpfAtBlePrintBus(TAG); \
        for(counter=0; ((counter<LEN) && (0 != (BUFF))); counter++) \
        { \
            tpfAtBlePrintBus("%02X ", ((uint8_t *)BUFF)[counter]); \
        } \
    }while(0)

#else //AT_BLE_DBG & TRACE_LVL_BUS
#define PRINT_BUS(TAG,BUFF,len)
#endif //AT_BLE_DBG & TRACE_LVL_BUS

#if (AT_BLE_DBG & TRACE_LVL_FUN) == TRACE_LVL_FUN
extern tpfTrace tpfAtBlePrintFns;
/// Print FN_IN message; ex: FN_IN() -> >>FUNCTON_NAME
#define FN_IN()             tpfAtBlePrintFns(HDR_TEXT("F", "{\n"), __SHORT_FILE__, __LINE__, __FUNCTION__)
/// Print FN_OUT message; ex: FN_IN(5) -> <<FUNCTON_NAME (0x5)
#define FN_OUT(RET)         tpfAtBlePrintFns(HDR_TEXT("F", "}=0x%02X\n"), __SHORT_FILE__, __LINE__, __FUNCTION__, RET)
#else //AT_BLE_DBG & TRACE_LVL_FUN
#define FN_IN()
#define FN_OUT(RET)
#endif //AT_BLE_DBG & TRACE_LVL_FUN

#elif USE_TRACE_LOGS_VER == VER_1_0 //USE_TRACE_LOGS_VER

extern tpfTrace pfTrace;
extern uint8_t u8AT_BLE_Tracelevel;

#if (AT_BLE_DBG != TRACE_LVL_DISABLE)
/// Generic print function; ex:PRINT(MESSAGE) -> MESSAGE
#define PRINT(...)          do{if(pfTrace){pfTrace(__VA_ARGS__);}}while(0)
/// Print info message; ex: PRINT_INFO(MESSAGE) -> (INF)(FUNCTON_NAME)(LINE_NUMBER)MESSAGE
#define PRINT_INFO(...)     do{if(pfTrace && (u8AT_BLE_Tracelevel & TRACE_LVL_INFO)){pfTrace("(INF)"/**/__VA_ARGS__);}}while(0)
/// Print info message; ex: PRINT_ERR(MESSAGE) -> (ERR)(FUNCTON_NAME)(LINE_NUMBER)MESSAGE
#define PRINT_ERR(...)      do{if(pfTrace && (u8AT_BLE_Tracelevel & TRACE_LVL_ERR)){pfTrace("(ERR)%s[%d]",__FUNCTION__,__LINE__);pfTrace(__VA_ARGS__);}}while(0)
/// Print info message; ex: PRINT_DBG(MESSAGE) -> (DBG)(FUNCTON_NAME)(LINE_NUMBER)MESSAGE
#define PRINT_DBG(...)      do{if(pfTrace && (u8AT_BLE_Tracelevel & TRACE_LVL_DBG)){pfTrace("(DBG)"/**/__VA_ARGS__);}}while(0)
/// Print trasfered message on bus; ex: PRINT_BUS("TAG: ", arr, 2) -> TAG: arr[0] arr[1]
#define PRINT_BUS(TAG,BUFF,LEN)     \
    do{ \
        if(pfTrace && (u8AT_BLE_Tracelevel & TRACE_LVL_BUS)) \
        { \
            uint8_t counter=0; \
            PRINT(TAG); \
            for(counter=0; ((counter<LEN) && (0 != (BUFF))); counter++) \
            { \
                PRINT("%02X ", ((uint8_t *)BUFF)[counter]); \
            } \
        } \
    }while(0)

/// Print FN_IN message; ex: FN_IN() -> >>FUNCTON_NAME
#define FN_IN()             do{if(pfTrace && (u8AT_BLE_Tracelevel & TRACE_LVL_FUN)){pfTrace(">>%s\r\n",__FUNCTION__);}}while(0)
/// Print FN_OUT message; ex: FN_IN(5) -> <<FUNCTON_NAME (0x5)
#define FN_OUT(RET)         do{if(pfTrace && (u8AT_BLE_Tracelevel & TRACE_LVL_FUN)){pfTrace("<<%s[%d](0x%X)\r\n",__FUNCTION__,__LINE__,RET);}}while(0)
#else //(AT_BLE_DBG != TRACE_LVL_DISABLE)

#define PRINT(...)
#define PRINT_INFO(...)
#define PRINT_ERR(...)
#define ASSERT_PRINT_ERR(c,...) do{}while(0)
#define PRINT_DBG(...)
#define PRINT_BUS(TAG,BUFF,len)
#define FN_IN()
#define FN_OUT(RET)
#endif //(AT_BLE_DBG != TRACE_LVL_DISABLE)

#endif //USE_TRACE_LOGS_VER
#endif /* __DBG_LOGS_H__ */
