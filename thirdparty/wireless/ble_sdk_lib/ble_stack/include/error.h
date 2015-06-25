/*
 * dbg_task.h
 *
 *  Created on: Oct 28, 2014
 *      Author: lali
 */
#ifndef ERROR_H_
#define ERROR_H_

#include "cmn_defs.h"

at_ble_status_t at_ble_att_error(uint8_t att_error_code);

at_ble_status_t at_ble_gap_error(uint8_t gap_error_code);


#define IS_ERR(__status__) (__status__ != AT_BLE_SUCCESS)

#define ERRORCHECK(__status__) do{\
	if(IS_ERR(__status__))\
	{\
		printf("ERRORCHECK(%d)\n", __status__);\
		goto ERRORHANDLER;\
	}\
}while(0)

#define ERRORREPORT(__status__, __err__) do{\
	printf("NMI_ERRORREPORT(%d)\n", __err__);\
	__status__ = __err__;\
	goto ERRORHANDLER;\
}while(0)

#define  NULLCHECK(__status__, __ptr__)	do{\
	if(__ptr__ == NULL)\
	{\
		ERRORREPORT(__status__, AT_BLE_FAILURE);\
	}\
}while(0)

#define CATCH(__status__) \
ERRORHANDLER :\
if(IS_ERR(__status__)) \

#endif

