
/* Prevent double inclusion */
#ifndef COMMON_HW_TIMER_H
#define COMMON_HW_TIMER_H

#include "tc.h"
#include "conf_common_hw_timer.h"

#define DEF_1MHZ	(1000000)
#define TIMER_PERIOD  UINT16_MAX

void common_tc_init(void);
uint16_t common_tc_read_count(void);
void common_tc_delay(uint16_t value);
void common_tc_compare_stop(void);
void common_tc_overflow_stop(void);
void common_tc_stop(void);
void set_common_tc_overflow_callback(tc_callback_t callback);
void set_common_tc_expiry_callback(tc_callback_t callback);

#endif /* COMMON_HW_TIMER_H */
/* EOF */
