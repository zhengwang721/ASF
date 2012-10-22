#ifndef PAC_HEADER_H_INCLUDED
#define PAC_HEADER_H_INCLUDED
#include <stdint.h>
#include <stdbool.h>
#define __I  volatile const
#define __O  volatile
#define __IO volatile

#define Assert(check)

#undef __always_inline
#define __always_inline inline __attribute__((__always_inline__))
#define __no_inline     __attribute__((noinline))

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


/* PAC Register Map */
typedef struct {
	REG32(WPCLR, __IO);
	REG32(WPSET, __IO);
} PAC_t;

enum status_code{
	STATUS_OK		= 0x00,
	STATUS_ERR_INVALID_ARG	= 0x17,
};

typedef enum status_code status_code_t;

#define PAC1 (*( PAC_t * ) 0x40000000)
#define PAC2 (*( PAC_t * ) 0x40000001)
#define PAC3 (*( PAC_t * ) 0x40000002)

#endif /* PAC_HEADER_H_INCLUDED */
