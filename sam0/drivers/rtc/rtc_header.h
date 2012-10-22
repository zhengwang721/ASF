#ifndef _RTC_H
#define _RTC_H

#define LOCAL

#ifdef LOCAL
  #include <stdint.h>
  #include <stdbool.h>
#endif

#define __I  volatile const
#define __O  volatile
#define __IO volatile

/* Default register macro for sub access of 32-bit registers */
#define REG64(name, type) union { \
	type uint64_t name; \
	type uint32_t name ## _UINT16_T[2]; \
	type uint16_t name ## _UINT16_T[4]; \
	type uint8_t name ## _UINT8_T[8]; \
}

/* Default register macro for sub access of 32-bit registers */
#define REG32(name, type) union { \
	type uint32_t name; \
	type uint16_t name ## _UINT16_T[2]; \
	type uint8_t name ## _UINT8_T[4]; \
}

/* Default register macro for sub access of 16-bit registers */
#define REG16(name, type) union { \
	type uint16_t name; \
	type uint8_t name ## _UINT8_T[2]; \
}

/* Default register definition for 8-bit registers */
#define REG8(name, type) type uint8_t name

/* RTC Register Map in COUNT16 mode */
typedef struct {
	REG16(COUNT, __IO);
	__I uint8_t reserved_0x12;
	__I uint8_t reserved_0x13;
	REG16(PER, __IO);
	__I uint8_t reserved_0x16;
	__I uint8_t reserved_0x17;
	__IO uint16_t COMP[6];
	__I uint8_t reserved_0x24;
	__I uint8_t reserved_0x25;
	__I uint8_t reserved_0x26;
	__I uint8_t reserved_0x27;
} RTC_COUNT16_t;

/* RTC Register Map in COUNT32 mode */
typedef struct {
	REG32(COUNT, __IO);
	__I uint8_t reserved_0x14;
	__I uint8_t reserved_0x15;
	__I uint8_t reserved_0x16;
	__I uint8_t reserved_0x17;
	__IO uint32_t COMP[4];
} RTC_COUNT32_t;

/* RTC Register Map in CLOCK mode */
typedef struct {
	REG32(CLOCK, __IO);
	__I uint8_t reserved_0x14;
	__I uint8_t reserved_0x15;
	__I uint8_t reserved_0x16;
	__I uint8_t reserved_0x17;
	__IO uint32_t ALARM[4];
} RTC_CLOCK_t;

typedef struct {
	REG16(CTRL, __IO);
	REG16(READREQ, __IO);
	REG16(EVCTRL, __IO);
	REG8(INTENCLR, __IO);
	REG8(INTENSET, __IO);
	REG8(INTFLAG, __IO);
	__I uint8_t reserved_0x09;
	REG8(STATUS, __O);
	REG8(DEBUG, __IO);
	REG8(FREQCORR, __IO);
	__I uint8_t reserved_0x0D;
	__I uint8_t reserved_0x0E;
	__I uint8_t reserved_0x0F;
	union {
		RTC_COUNT32_t COUNT32;
		RTC_COUNT16_t COUNT16;
		RTC_CLOCK_t CALENDAR;
	};
} RTC_t;

#define RTC_SWRST_bm                  0x0001

#define RTC_CLOCK_YEAR_bp 0
#define RTC_CLOCK_YEAR_bm 0
#define RTC_CLOCK_MONTH_bp 0
#define RTC_CLOCK_MONTH_bm 0
#define RTC_CLOCK_DAY_bp 0
#define RTC_CLOCK_DAY_bm 0
#define RTC_CLOCK_HOUR_bp 0
#define RTC_CLOCK_HOUR_bm 0
#define RTC_CLOCK_24HOUR_bm 0
#define RTC_CLOCK_24HOUR_bp 0
#define RTC_CLOCK_12HOUR_bm 0
#define RTC_CLOCK_12HOUR_bp 0
#define RTC_CLOCK_PM_bp 0
#define RTC_CLOCK_PM_bm 0
#define RTC_CLOCK_MINUTE_bp 0
#define RTC_CLOCK_MINUTE_bm 0
#define RTC_CLOCK_SECOND_bp 0
#define RTC_CLOCK_SECOND_bm 0

#define RTC_FREQCORR_SIGN_bp 0
#define RTC_MODE_gp		2
#define RTC_MODE_COUNT32_gc	0x0
#define RTC_MODE_COUNT16_gc	0x1
#define RTC_MODE_CLOCK_gc	0x2
#define RTC_MODE_CLOCK_bm 0
#define RTC_CLKREP_bm 0
#define RTC_RCONT_bp 0
#define RTC_RREQ_bm 0

#define RTC_PRESCALER_gp            8
#define RTC_PRESCALER_DIV1_gc     0x0
#define RTC_PRESCALER_DIV2_gc     0x1
#define RTC_PRESCALER_DIV4_gc     0x2
#define RTC_PRESCALER_DIV8_gc     0x3
#define RTC_PRESCALER_DIV16_gc    0x4
#define RTC_PRESCALER_DIV32_gc    0x5
#define RTC_PRESCALER_DIV64_gc    0x6
#define RTC_PRESCALER_DIV128_gc   0x7
#define RTC_PRESCALER_DIV256_gc   0x8
#define RTC_PRESCALER_DIV512_gc   0x9
#define RTC_PRESCALER_DIV1024_gc  0xA

#define RTC_PRESCALER_DIV1024_bm  0xA


#define RTC_ENABLE_bm             0x0002
#define RTC_MATCHCLR_bm           0x0080

#define RTC_INTFLAG_CMP0_bm     0x01
#define RTC_INTFLAG_CMP1_bm     0x02
#define RTC_INTFLAG_CMP2_bm     0x04
#define RTC_INTFLAG_CMP3_bm     0x08
#define RTC_INTFLAG_CMP4_bm     0x10
#define RTC_INTFLAG_CMP5_bm     0x20

#define RTC_INTFLAG_READY_bm    0x40
#define RTC_INTFLAG_OVF_bm      0x80

#define RTC_INTFLAG_OVF_bp 		7;
#define RTC_INTFLAG_CMP0_bp	0;
#define RTC_INTFLAG_CMP1_bp	1;
#define RTC_INTFLAG_CMP2_bp	2;
#define RTC_INTFLAG_CMP3_bp	3;
#define RTC_INTFLAG_CMP4_bp	4;
#define RTC_INTFLAG_CMP5_bp	5;

#define RTC_EVENT_OVF_bm 0
#define RTC_EVENT_ALARM0_bm 0
#define RTC_EVENT_ALARM1_bm 0
#define RTC_EVENT_ALARM2_bm 0
#define RTC_EVENT_ALARM3_bm 0
#define RTC_EVENT_PER0_bm 0
#define RTC_EVENT_PER1_bm 0
#define RTC_EVENT_PER2_bm 0
#define RTC_EVENT_PER3_bm 0
#define RTC_EVENT_PER4_bm 0
#define RTC_EVENT_PER5_bm 0
#define RTC_EVENT_PER6_bm 0
#define RTC_EVENT_PER7_bm 0


#define RTC_SYNC_BUSY_bm         0x01

#define RTC (*(RTC_t*) 0x40000000)

enum status_code {
	STATUS_OK = 0,
	STATUS_ERR_INVALID_ARG 	= -8,
	STATUS_ERR_NOT_INITIALIZED = 0x14,
};

enum RTC_MODE {
	RTC_MODE_COUNT32 	= (RTC_MODE_COUNT32_gc << RTC_MODE_gp),
	RTC_MODE_COUNT16 	= (RTC_MODE_COUNT16_gc << RTC_MODE_gp),
	RTC_MODE_CLOCK 		= (RTC_MODE_CLOCK_gc << RTC_MODE_gp),
};

enum RTC_PRESCALER {
	RTC_PRESCALER_DIV1 	= (RTC_PRESCALER_DIV1_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV2 	= (RTC_PRESCALER_DIV2_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV4 	= (RTC_PRESCALER_DIV4_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV8 	= (RTC_PRESCALER_DIV8_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV16 	= (RTC_PRESCALER_DIV16_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV32 	= (RTC_PRESCALER_DIV32_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV64	= (RTC_PRESCALER_DIV64_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV128	= (RTC_PRESCALER_DIV128_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV256	= (RTC_PRESCALER_DIV256_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV512	= (RTC_PRESCALER_DIV512_gc << RTC_PRESCALER_gp),
	RTC_PRESCALER_DIV1024	= (RTC_PRESCALER_DIV1024_gc << RTC_PRESCALER_gp),
};



enum RTC_COUNT_MODE {
	RTC_COUNT_MODE_32BIT_bm,
	RTC_COUNT_MODE_16BIT_bm,
};

#endif /* _RTC_H_*/
