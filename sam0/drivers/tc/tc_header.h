#ifndef TC_HEADER_H_INCLUDED
#define TC_HEADER_H_INCLUDED
#include <stdint.h>

#define Assert(check)

#define __I  volatile const
#define __O  volatile
#define __IO volatile

/* Default register macro for sub access of 32-bit registers */
#define REG32(name, type) union { \
	type volatile uint32_t name; \
	type volatile uint16_t name ## _UINT16_T[2]; \
	type volatile uint8_t name ## _UINT8_T[4]; \
}

/* Default register macro for sub access of 16-bit registers */
#define REG16(name, type) union { \
	type volatile uint16_t name; \
	type volatile uint8_t name ## _UINT8_T[2]; \
}

/* Default register definition for 8-bit registers */
#define REG8(name, type) type volatile uint8_t name

// There is no reg map specified for TC_LINK mode


#ifdef USE_ALL_MODE_DRIVER
/* TC Register Map */
typedef struct {
	REG8(COUNT, __IO);
	__I uint8_t reserved_0x11;
	__I uint8_t reserved_0x12;
	__I uint8_t reserved_0x13;
	REG8(PER, __IO);
	__I uint8_t reserved_0x15;
	__I uint8_t reserved_0x16;
	__I uint8_t reserved_0x17;
	REG8(CC0, __IO);
	REG8(CC1, __IO);
	REG8(CC2, __IO);
	REG8(CC3, __IO);
	__I uint8_t reserved_0x1C;
	__I uint8_t reserved_0x1D;
	__I uint8_t reserved_0x1E;
	__I uint8_t reserved_0x1F;
} TC_COUNT8_t;

/* TC Register Map */
typedef struct {
	REG16(COUNT, __IO);
	__I uint8_t reserved_0x12;
	__I uint8_t reserved_0x13;
	__I uint8_t reserved_0x14;
	__I uint8_t reserved_0x15;
	__I uint8_t reserved_0x16;
	__I uint8_t reserved_0x17;
	REG16(CC0, __IO);
	REG16(CC1, __IO);
	REG16(CC2, __IO);
	REG16(CC3, __IO);
} TC_COUNT16_t;

/* TC Register Map */
typedef struct {
	REG32(COUNT, __IO);
	__I uint8_t reserved_0x14;
	__I uint8_t reserved_0x15;
	__I uint8_t reserved_0x16;
	__I uint8_t reserved_0x17;
	REG32(CC0, __IO);
	REG32(CC1, __IO);
} TC_COUNT32_t;

typedef struct {
	REG16(CTRLA, __IO);
	REG16(READREQ, __IO);
	REG8(CTRLBCLR, __IO);
	REG8(CTRLBSET, __IO);
	REG8(CTRLC, __IO);
	__I uint8_t reserved_0x07;
	REG8(DEBUG, __IO);
	__I uint8_t reserved_0x09;
	REG16(EVCTRL, __IO);
	REG8(INTENCLR, __IO);
	REG8(INTENSET, __IO);
	REG8(INTFLAG, __IO);
	REG8(STATUS, __IO);

	/* Part of register map which is unique depending on the mode */
	union {
		TC_COUNT8_t  TC_COUNT8;
		TC_COUNT16_t TC_COUNT16;
		TC_COUNT32_t TC_COUNT32;
	};
} TC_t;

#else
/* TC Register Map */
typedef struct {
		REG16(CTRLA, __IO);
		REG16(READREQ, __IO);
		REG8(CTRLBCLR, __IO);
		REG8(CTRLBSET, __IO);
		REG8(CTRLC, __IO);
		__I uint8_t reserved_0x07;
		REG8(DEBUG, __IO);
		__I uint8_t reserved_0x09;
		REG16(EVCTRL, __IO);
		REG8(INTENCLR, __IO);
		REG8(INTENSET, __IO);
		REG8(INTFLAG, __IO);
		REG8(STATUS, __IO);
		REG8(COUNT, __IO);
		__I uint8_t reserved_0x11;
		__I uint8_t reserved_0x12;
		__I uint8_t reserved_0x13;
		REG8(PER, __IO);
		__I uint8_t reserved_0x15;
		__I uint8_t reserved_0x16;
		__I uint8_t reserved_0x17;
		REG8(CC0, __IO);
		REG8(CC1, __IO);
		REG8(CC2, __IO);
		REG8(CC3, __IO);
		__I uint8_t reserved_0x1C;
		__I uint8_t reserved_0x1D;
		__I uint8_t reserved_0x1E;
		__I uint8_t reserved_0x1F;
} TC_COUNT8_t;

/* TC Register Map */
typedef struct {
		REG16(CTRLA, __IO);
		REG16(READREQ, __IO);
		REG8(CTRLBCLR, __IO);
		REG8(CTRLBSET, __IO);
		REG8(CTRLC, __IO);
		__I uint8_t reserved_0x07;
		REG8(DEBUG, __IO);
		__I uint8_t reserved_0x09;
		REG16(EVCTRL, __IO);
		REG8(INTENCLR, __IO);
		REG8(INTENSET, __IO);
		REG8(INTFLAG, __IO);
		REG8(STATUS, __IO);
		REG16(COUNT, __IO);
		__I uint8_t reserved_0x12;
		__I uint8_t reserved_0x13;
		__I uint8_t reserved_0x14;
		__I uint8_t reserved_0x15;
		__I uint8_t reserved_0x16;
		__I uint8_t reserved_0x17;
		REG16(CC0, __IO);
		REG16(CC1, __IO);
		REG16(CC2, __IO);
		REG16(CC3, __IO);
} TC_COUNT16_t;

/* TC Register Map */
typedef struct {
		REG16(CTRLA, __IO);
		REG16(READREQ, __IO);
		REG8(CTRLBCLR, __IO);
		REG8(CTRLBSET, __IO);
		REG8(CTRLC, __IO);
		__I uint8_t reserved_0x07;
		REG8(DEBUG, __IO);
		__I uint8_t reserved_0x09;
		REG16(EVCTRL, __IO);
		REG8(INTENCLR, __IO);
		REG8(INTENSET, __IO);
		REG8(INTFLAG, __IO);
		REG8(STATUS, __IO);
		REG32(COUNT, __IO);
		__I uint8_t reserved_0x14;
		__I uint8_t reserved_0x15;
		__I uint8_t reserved_0x16;
		__I uint8_t reserved_0x17;
		REG32(CC0, __IO);
		REG32(CC1, __IO);
} TC_COUNT32_t;

typedef struct {
	/* Part of register map which is unique depending on the mode */
	union {
		TC_COUNT8_t  TC_COUNT8;
		TC_COUNT16_t TC_COUNT16;
		TC_COUNT32_t TC_COUNT32;
	};
} TC_t;
#endif /* Registermap for different drivers */


#define TC_MODE_COUNT16_bm           (0x00 << 2)
#define TC_MODE_COUNT8_bm            (0x01 << 2)
#define TC_MODE_COUNT32_bm           (0x02 << 2)

#define TC_ENABLE_bm                 (0x0001 << 1)
#define TC_RESET_bm                  (0x0001 << 0)
#define TC_SYNCBUSY_bm               (0x0001 << 7)
#define TC_SLAVE_bm                  (0x01   << 4)

#define TC_ONESHOT_ENABLED_bm        (0x01 << 2)
#define TC_ONESHOT_DISABLED_bm       (0x00 << 2)

#define TC_EVGEN_NONE_gc             (0x00 << 8)
#define TC_EVGEN_OVF_gc              (0x01 << 8)
#define TC_EVGEN_CC0_gc              (0x01 << 12)
#define TC_EVGEN_CC1_gc              (0x02 << 12)
#define TC_EVGEN_CC2_gc              (0x03 << 12)
#define TC_EVGEN_CC3_gc              (0x04 << 12)
#define TC_EVGEN_ALL_gc              (0xF1 << 8)

#define TC_PRESCSYNC_GCLK_bm           (0x00 << 12)
#define TC_PRESCSYNC_PRESC_bm          (0x01 << 12)
#define TC_PRESCSYNC_RESYNC_bm         (0x02 << 12)

#define TC_PRESCALER_DIV1_bm         (0x00 << 8)
#define TC_PRESCALER_DIV2_bm         (0x01 << 8)
#define TC_PRESCALER_DIV4_bm         (0x02 << 8)
#define TC_PRESCALER_DIV8_bm         (0x03 << 8)
#define TC_PRESCALER_DIV16_bm        (0x04 << 8)
#define TC_PRESCALER_DIV64_bm        (0x05 << 8)
#define TC_PRESCALER_DIV256_bm       (0x06 << 8)
#define TC_PRESCALER_DIV1024_bm      (0x07 << 8)

#define TC_WAVEGEN_NFRQ_bm           (0x00 << 5)
#define TC_WAVEGEN_MFRQ_bm           (0x01 << 5)
#define TC_WAVEGEN_NPWM_bm           (0x02 << 5)
#define TC_WAVEGEN_MPWM_bm           (0x03 << 5)

#define TC_COMMAND_NONE_bm           (0x00 << 6)
#define TC_COMMAND_START_bm          (0x00 << 6)
#define TC_COMMAND_STOP_bm           (0x00 << 6)

#define TC_DIR_COUNT_UP_bm           (0x01 << 0)
#define TC_DIR_COUNT_DOWN_bm         (0x00 << 0)

#define TC_CPTEN_CC0_bm              (0x01 << 4)
#define TC_CPTEN_CC1_bm              (0x01 << 5)
#define TC_CPTEN_CC2_bm              (0x01 << 6)
#define TC_CPTEN_CC3_bm              (0x01 << 7)

#define TC_INVEN_CC0_bm              (0x01 << 0)
#define TC_INVEN_CC1_bm              (0x01 << 1)
#define TC_INVEN_CC2_bm              (0x01 << 2)
#define TC_INVEN_CC3_bm              (0x01 << 3)

#define TC_EVACT_OFF_bm              (0x00 << 0)
#define TC_EVACT_RETRIGGER_bm        (0x01 << 0)
#define TC_EVACT_COUNT_bm            (0x02 << 0)
#define TC_EVACT_PPW_bm              (0x05 << 0)
#define TC_EVACT_PWP_bm              (0x06 << 0)

#define TC_CPTEN_CC0_bp              4
#define TC_INVEN_CC0_bp              0
#define TC_EVGEN_CCEO_bp             12
#define TC_COUNT_DIRECTION_bp        1

#define TC0 (*( TC_t * ) 0x40000000)

#endif /* TC_HEADER_H_INCLUDED */
