#ifndef __SERCOM_HEADER_INCLUDED
#define __SERCOM_HEADER_INCLUDED

#include <stdint.h>

#define SERCOM_INST 6

//TODO: remove
enum status_code {
	STATUS_OK,
	STATUS_ERR_INVALID_ARG,
	STATUS_ERR_BUSY,
	STATUS_ERR_IO,
	STATUS_ERR_TIMEOUT,
	STATUS_ERR_UNSUPPORTED_DEV,
	STATUS_ERR_DENIED,
};
#define Assert(expr) \
{\
	if (!(expr)) while (true);\
}

#define SHIFT 0

#define __I  volatile const
#define __O  volatile
#define __IO volatile

/* Default register macro for sub access of 32-bit registers */
#define __REG32(name, type) union { \
		type uint32_t name; \
		type uint16_t name ## _UINT16_T[2]; \
		type uint8_t name ## _UINT8_T[4]; \
}

/* Default register macro for sub access of arrays of 32-bit registers */
#define __REG32_ARRAY(name, elements, type) union { \
		type uint32_t name[elements]; \
		type uint16_t name ## _UINT16_T[elements][2]; \
		type uint8_t name ## _UINT8_T[elements][4]; \
}

/* Default register macro for sub access of 16-bit registers */
#define __REG16(name, type) union { \
		type uint16_t name; \
		type uint8_t name ## _UINT8_T[2]; \
}

/* Default register macro for sub access of arrays of 16-bit registers */
#define __REG16_ARRAY(name, elements, type) union { \
		type uint16_t name[elements]; \
		type uint8_t name ## _UINT8_T[elements][2]; \
}

/* Default register definition for 8-bit registers */
#define __REG8(name, type) type uint8_t name

/* Default register definition for arrays of 8-bit registers */
#define __REG8_ARRAY(name, elements, type) type uint8_t name[elements]

/* SERCOM Register Map in USART mode */
typedef struct {
	__REG32(CTRLA, __IO);
	__REG32(CTRLB, __IO);
	__I uint16_t reserved_0x08;
	__REG16(BAUD, __IO);
	__I uint16_t reserved_0x0C;
	__I uint16_t reserved_0x0E;
	__REG8(INTENCLR, __O);
	__REG8(INTENSET, __IO);
	__REG16(INTFLAGS, __IO);
	__REG16(STATUS, __IO);
	__I uint16_t reserved_0x16;
	__REG16(DATA, __IO);
	__I uint16_t reserved_0x1A;
	__I uint16_t reserved_0x1C;
	__I uint16_t reserved_0x1E;
} SERCOM_USART_t;

/* SERCOM Register Map in SPI mode */
typedef struct {
	__REG32(CTRLA, __IO);
	__REG32(CTRLB, __IO);
	__I uint16_t reserved_0x08;
	__REG8(BAUD, __IO);
	__I uint8_t reserved_0x0B;
	__REG32(ADDR, __IO);
	__REG8(INTENCLR, __O);
	__REG8(INTENSET, __IO);
	__REG16(INTFLAGS, __IO);
	__REG16(STATUS, __I);
	__I uint16_t reserved_0x16;
	__REG16(DATA, __IO);
	__I uint16_t reserved_0x1A;
	__I uint16_t reserved_0x1C;
	__I uint16_t reserved_0x1E;
} SPI_t;

/* SERCOM Register Map in I2C mode */
typedef struct {
	__REG32(CTRLA, __IO);
	__REG32(CTRLB, __IO);
	__I uint16_t reserved_0x08;
	__I uint16_t reserved_0x0A;
	__REG32(ADDR, __IO);
	__REG8(INTENCLR, __O);
	__REG8(INTENSET, __IO);
	__REG16(INTFLAGS, __IO);
	__REG16(STATUS, __I);
	__I uint16_t reserved_0x16;
	__REG16(DATA, __IO);
	__I uint16_t reserved_0x1A;
	__I uint16_t reserved_0x1C;
	__I uint16_t reserved_0x1E;

} SERCOM_I2C_SLAVE_t;

/* SERCOM Register Map in I2C mode */
typedef struct {
	__REG32(CTRLA, __IO);
	__REG32(CTRLB, __IO);
	__I uint16_t reserved_0x08;
	__REG8(BAUD, __IO);
	__I uint8_t reserved_0x0B;
	__REG16(ADDR, __IO);
	__I uint16_t reserved_0x0E;
	__REG8(INTENCLR, __O);
	__REG8(INTENSET, __IO);
	__REG16(INTFLAGS, __IO);
	__REG16(STATUS, __I);
	__I uint16_t reserved_0x16;
	__REG16(DATA, __IO);
	__I uint16_t reserved_0x1A;
	__I uint16_t reserved_0x1C;
	__I uint16_t reserved_0x1E;

} SERCOM_I2C_MASTER_t;



/* SERCOM module */
typedef struct {
	union {
		SERCOM_I2C_SLAVE_t  I2C_SLAVE;
		SERCOM_I2C_MASTER_t I2C_MASTER;
		SPI_t               SPI;
		SERCOM_USART_t      USART;
	};
} SERCOM_t;

#define SERCOM_MODE_gp 2
#define SERCOM_MODE_USART_gc (0 << SERCOM_MODE_gp)
#define SERCOM_MODE_SPI_gc (1 <<  SERCOM_MODE_gp)
#define SERCOM_MODE_I2C_gc (2 << SERCOM_MODE_gp)

/* SERCOM Mode Selector */
typedef enum {
	SERCOM_MODE_USART = SERCOM_MODE_USART_gc,
	SERCOM_MODE_SPI   = SERCOM_MODE_SPI_gc,
	SERCOM_MODE_I2C   = SERCOM_MODE_I2C_gc,
} SERCOM_MODE_t;

/* Default reset bit on location bit 0 for all modules */
#define SWRST_bm 0x00000001




#define SERCOM_USART_ENABLE_bm 0x00000002
#define SERCOM_USART_RESET_bm  0x00000001
#define SERCOM_USART_CSRC_bm (1 << 31)
#define SERCOM_USART_DORD_bm (1 << 30)
#define SERCOM_USART_CPOL_bm (1 << 29)
#define SERCOM_USART_CMODE_bm (1 << 28)
#define SERCOM_USART_TXEN_bm (1 << 16)
#define SERCOM_USART_RXEN_bm (1 << 17)


// Interrupt flags
#define SERCOM_USART_DREIF_bm (1 << 0)
#define SERCOM_USART_TXCIF_bm (1 << 1)
#define SERCOM_USART_RXCIF_bm (1 << 2)
#define SERCOM_USART_RXSIF_bm (1 << 3)

// Error flags
#define SERCOM_USART_PERR_bm   (1 << 0)
#define SERCOM_USART_FERR_bm   (1 << 1)
#define SERCOM_USART_BUFOVF_bm (1 << 2)


#define SERCOM_USART_DATA_gm  (0x1FF << 1)
#define SERCOM_USART_SYNCBUSY_bp 7
#define SERCOM_USART_SYNCBUSY_bm (1 << SERCOM_USART_SYNCBUSY_bp)


/*TODO: Replace with correct values */
#define SERCOM_USART_FORM_1_gc 0
#define SERCOM_USART_FORM_5_gc 0


#define SERCOM_USART_CHSIZE_5BIT_gc 0
#define SERCOM_USART_CHSIZE_6BIT_gc 0
#define SERCOM_USART_CHSIZE_7BIT_gc 0
#define SERCOM_USART_CHSIZE_8BIT_gc 0
#define SERCOM_USART_CHSIZE_9BIT_gc 0

#define SERCOM_USART_PMODE_bm  0
#define SERCOM_USART_BMODE_bm  0


/* SPI defines */
#define SPI_MASTER_bm          (1 << 31)

#define SPI_DORD_bm            (1 << 30)

#define SPI_FORM_gp            24
#define SPI_FORM_WITH_ADDR_bm  (1 << 1)
#define SPI_FORM_HALF_DUP_bm   (1 << 2)

#define SPI_AMODE_gp           14
#define SPI_ADDRMODE_UNIQUE_bm (1 << 14)
#define SPI_ADDRMODE_RANGE_bm  (2 << 14)

#define SPI_DREIF_bm           (1 << 0)
#define SPI_TXCIF_bm           (1 << 1)
#define SPI_RXCIF_bm           (1 << 2)

#define SPI_CPHA_bm            (1 << 28)
#define SPI_CPOL_bm            (1 << 29)

#define SPI_SYNCBUSY_bm        (1 << 15)

#define SPI_RXEN_bm     (1 << 17)
#define SPI_ENABLE_bm        (1 << 1)
#define SPI_SLEEPEN_bm       (1 << 7)
#define SPI_SWRST_bm         (1 << 0)
#define SPI_PLOADEN_bm       (1 << 6)
#define SPI_CHSIZE_bm        (1 << 1)
#define SPI_CHSIZE_gp        0

#define SPI_DATA_BIT_8_bm   (1 << 8)
#define SPI_DATA_gm         0x1FF

#define SPI_ADDR_gp         0
#define SPI_ADDRMASK_gp     16

enum RXPO_group {
	SERCOM_USART_RXPO_0_gc = (0 << 18),
	SERCOM_USART_RXPO_1_gc = (1 << 18),
	SERCOM_USART_RXPO_2_gc = (2 << 18),
	SERCOM_USART_RXPO_3_gc = (3 << 18),
};

enum TXPO_group {
	SERCOM_USART_TXPO_0_gc = (0 << 23),
	SERCOM_USART_TXPO_2_gc = (1 << 23),
};


enum DIPO_group {
	SPI_DIPO_PIN0 = (0 << 20),
	SPI_DIPO_PIN1 = (1 << 20),
	SPI_DIPO_PIN2 = (2 << 20),
	SPI_DIPO_PIN3 = (3 << 20),
};

enum DOPO_group {
	SPI_DOPO_PIN0_PIN1_PIN2 = (0 << 16),
	SPI_DOPO_PIN2_PIN3_PIN1 = (1 << 16),
};



#define SERCOM0 (*(SERCOM_t * ) 0x2000)
#define SERCOM1 (*(SERCOM_t * ) 0x3000)
#define SERCOM2 (*(SERCOM_t * ) 0x4000)
#define SERCOM3 (*(SERCOM_t * ) 0x5000)
/* #define SAM0P_SERCOM0 (*(SAM0P_SERCOM_Typedef * ) 0x2000) */

//#define SERCOM1 (*(SERCOM_Typedef * ) 0x2200)

#endif /* __SERCOM_HEADER_INCLUDED */
