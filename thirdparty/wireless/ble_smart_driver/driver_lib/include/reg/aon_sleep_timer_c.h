#ifndef __AON_SLEEP_TIMER_C_H__
#define __AON_SLEEP_TIMER_C_H__

/* **************************** */
#define AON_SLEEP_TIMER_CONTROL                                                    (AON_SLEEP_TIMER_BASE + 0x0)
#define AON_SLEEP_TIMER_CONTROL_DEFAULT                                            0x0
#define AON_SLEEP_TIMER_CONTROL_RELOAD_ENABLE                                      (Bit0)
#define AON_SLEEP_TIMER_CONTROL_SINGLE_COUNT_ENABLE                                (Bit1)
#define AON_SLEEP_TIMER_CONTROL_IRQ_CLEAR                                          (Bit4)
#define AON_SLEEP_TIMER_CONTROL_SLP_TIMER_CLK_RELOAD_DLY                           (Bit8+Bit9+Bit10)
#define AON_SLEEP_TIMER_CONTROL_SLP_TIMER_CLK_RELOAD_DLY_MASK                      (Bit2+Bit1+Bit0)
#define AON_SLEEP_TIMER_CONTROL_SLP_TIMER_CLK_RELOAD_DLY_SHIFT                     (8)
#define AON_SLEEP_TIMER_CONTROL_SLP_TIMER_SINGLE_COUNT_ENABLE_DLY                  (Bit12+Bit13+Bit14)
#define AON_SLEEP_TIMER_CONTROL_SLP_TIMER_SINGLE_COUNT_ENABLE_DLY_MASK             (Bit2+Bit1+Bit0)
#define AON_SLEEP_TIMER_CONTROL_SLP_TIMER_SINGLE_COUNT_ENABLE_DLY_SHIFT            (12)
#define AON_SLEEP_TIMER_CONTROL_SLEEP_TIMER_ACTIVE                                 (Bit30)
#define AON_SLEEP_TIMER_CONTROL_SLEEP_TIMER_NOT_ACTIVE                             (Bit31)


#define AON_SLEEP_TIMER_SINGLE_COUNT_DURATION                                      (AON_SLEEP_TIMER_BASE + 0x4)
#define AON_SLEEP_TIMER_SINGLE_COUNT_DURATION_DEFAULT                              0x0


#define AON_SLEEP_TIMER_RELOAD_DURATION                                            (AON_SLEEP_TIMER_BASE + 0x8)
#define AON_SLEEP_TIMER_RELOAD_DURATION_DEFAULT                                    0x0


#define AON_SLEEP_TIMER_CURRENT_COUNT_VALUE                                        (AON_SLEEP_TIMER_BASE + 0xc)
#define AON_SLEEP_TIMER_CURRENT_COUNT_VALUE_DEFAULT                                0x0

#endif
