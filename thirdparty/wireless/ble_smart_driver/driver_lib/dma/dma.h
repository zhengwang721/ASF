#ifndef __DMA_CORE_H__
#define __DMA_CORE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "CMSDK_CM0.h"
#include "core_cm0.h"
#include "compiler.h"
#include "typedefs.h"

#define REG_WR(addr, value)      (*(volatile unsigned int *)(addr)) = (value)
#define REG_RD(addr)             (*(volatile unsigned int *)(addr))


#define DMA_BASE_ADD	0x40002000

#define DMA_CMD_RD_START_ADD		(DMA_BASE_ADD+0x00)
#define DMA_CMD_WR_START_ADD		(DMA_BASE_ADD+0x04)
#define DMA_CMD_BUFFER_SZ				(DMA_BASE_ADD+0x08)
#define DMA_CMD_REG3						(DMA_BASE_ADD+0x0C)

#define DMA_STATIC_REG0 				(DMA_BASE_ADD+0x10)
#define DMA_STATIC_REG1 				(DMA_BASE_ADD+0x14)
#define DMA_STATIC_REG2 				(DMA_BASE_ADD+0x18)
#define DMA_STATIC_REG3 				(DMA_BASE_ADD+0x1c)
#define DMA_STATIC_REG4 				(DMA_BASE_ADD+0x20)

#define DMA_RSTRICT_REG 				(DMA_BASE_ADD+0x2C)
#define DMA_READ_OFFSET_REG 		(DMA_BASE_ADD+0x30)
#define DMA_WRITE_OFFSET_REG	 	(DMA_BASE_ADD+0x34)
#define DMA_FIFO_FULLNESS_REG 	(DMA_BASE_ADD+0x38)

#define DMA_CHANNEL_ENABLE_REG	(DMA_BASE_ADD+0x40)
#define DMA_CHANNEL_START_REG		(DMA_BASE_ADD+0x44)
#define DMA_CHANNEL_ACTIVE_REG	(DMA_BASE_ADD+0x48)
#define DMA_COUNT_REG						(DMA_BASE_ADD+0x50)

#define DMA_INT_RAWSTAT_REG			(DMA_BASE_ADD+0xA0)
#define DMA_INT_CLEAR_REG				(DMA_BASE_ADD+0xA4)
#define DMA_INT_ENABLE_REG			(DMA_BASE_ADD+0xA8)
#define DMA_INT_STATUS_REG			(DMA_BASE_ADD+0xAC)


/* Shared registers */
#define DMA_INT0_STATUS		(DMA_BASE_ADD+0x800)
#define DMA_INT1_STATUS		(DMA_BASE_ADD+0x804)
#define DMA_INT2_STATUS		(DMA_BASE_ADD+0x808)
#define DMA_INT3_STATUS		(DMA_BASE_ADD+0x80C)
#define DMA_INT4_STATUS		(DMA_BASE_ADD+0x810)
#define DMA_INT5_STATUS		(DMA_BASE_ADD+0x814)
#define DMA_INT6_STATUS		(DMA_BASE_ADD+0x818)
#define DMA_INT7_STATUS		(DMA_BASE_ADD+0x81C)
#define DMA_CORE0_JOINT_MODE	(DMA_BASE_ADD + 0x830)
#define DMA_CORE0_CHANNEL_START (DMA_BASE_ADD + 0x848)
#define DMA_PERIPH_TX_CTRL	(DMA_BASE_ADD + 0x854)

#define DMA_INT_CH_END			(1<<0)
#define DMA_INT_RD_SLVERR		(1<<1)
#define DMA_INT_WR_SLVERR		(1<<2)
#define DMA_INT_OVERFLOW		(1<<3)
#define DMA_INT_UNDERFLOW		(1<<4)
#define DMA_INT_TIMEOUT_RD	(1<<5)
#define DMA_INT_TIMEOUT_WR	(1<<6)	
#define DMA_INT_WDT			(1<<7)

struct dma_static_reg0_cfg{
	unsigned rd_burst_max_size;
	unsigned rd_allow_full_burst;
	unsigned rd_allow_full_fifo;
	unsigned rd_tokens;
	unsigned rd_outstanding;
	unsigned rd_incr;
};

__INLINE void static_reg0_write(uint8 channel, struct dma_static_reg0_cfg* str_cfg)
{

	unsigned int reg_val = 0;
	reg_val |= str_cfg->rd_burst_max_size;
	reg_val |= str_cfg->rd_allow_full_burst << 12;
	reg_val |= str_cfg->rd_allow_full_fifo << 13;
	reg_val |= str_cfg->rd_tokens <<16;
	reg_val &= ~(1<<30);	//recommended to be 0
	reg_val |= str_cfg->rd_incr <<31; //set to 1 for all mem copy, 0 for peripheral

	REG_WR((DMA_STATIC_REG0 + (0x100 * channel)),reg_val);	
}


struct dma_static_reg1_cfg{
	unsigned wr_burst_max_size;
	unsigned wr_allow_full_burst;
	unsigned wr_allow_full_fifo;
	unsigned wr_tokens;
	unsigned wr_outstanding;
	unsigned wr_incr;
};
__INLINE void static_reg1_write(uint8 channel, struct dma_static_reg1_cfg* str_cfg)
{

	unsigned int reg_val = 0;
	reg_val |= str_cfg->wr_burst_max_size;
	reg_val |= str_cfg->wr_allow_full_burst << 12;
	reg_val |= str_cfg->wr_allow_full_burst << 13;
	reg_val |= str_cfg->wr_tokens <<16;
	reg_val &= ~(1<<30);	//recommended to be 0
	reg_val |= str_cfg->wr_incr <<31; //set to 1 for all mem copy, 0 for peripheral

	REG_WR((DMA_STATIC_REG1 + (0x100 * channel)),reg_val);	
}

struct dma_static_reg2_cfg{
	unsigned frame_width;
	unsigned block;
	unsigned joint;
	unsigned auto_retry;
	unsigned rd_cmd_port_num;
	unsigned rd_port_num;
	unsigned wr_port_num;
	unsigned int_num;
	unsigned end_swap;	
};

__INLINE void static_reg2_write(uint8 channel, struct dma_static_reg2_cfg* str_cfg)
{

	unsigned int reg_val = 0;
	reg_val |= str_cfg->frame_width;
	reg_val |= str_cfg->block << 15;
	reg_val |= str_cfg->joint<< 16;
	reg_val |= str_cfg->auto_retry<<17;
	reg_val |= str_cfg->rd_cmd_port_num<<20;
	reg_val |= str_cfg->rd_port_num<<21;
	reg_val |= str_cfg->wr_port_num<<22;
	reg_val |= str_cfg->int_num<<24;
	reg_val |= str_cfg->end_swap<<28;

	REG_WR((DMA_STATIC_REG2 + (0x100 * channel)),reg_val);	
}

__INLINE void static_reg3_write(uint8 channel, unsigned int  rd_wait_limit, unsigned int wr_wait_limit)
{
	REG_WR((DMA_STATIC_REG3 + (0x100 * channel)),(rd_wait_limit | (wr_wait_limit << 16)));	
}


struct dma_static_reg4_cfg{
	unsigned rd_periph_num;
	unsigned rd_periph_delay;
	unsigned rd_periph_block;
	unsigned wr_periph_num;
	unsigned wr_periph_delay;
	unsigned wr_periph_block;
};

__INLINE void static_reg4_write(uint8 channel, struct dma_static_reg4_cfg* str_cfg)
{
	unsigned int reg_val = 0;
	reg_val |= str_cfg->rd_periph_num;
	reg_val |= str_cfg->rd_periph_delay<< 8;
	reg_val |= str_cfg->rd_periph_block<< 15;
	reg_val |= str_cfg->wr_periph_num<<16;
	reg_val |= str_cfg->wr_periph_delay<<24;
	reg_val |= str_cfg->wr_periph_block<<31;

	REG_WR((DMA_STATIC_REG4 + (0x100 * channel)),reg_val);
}

typedef union _u_chcmd {
	struct _bfields {
		uint32 cmd_set_int:1;
		uint32 cmd_last:1;
		uint32 rsvd1:2;
		uint32 cmd_next:28;
	}str_bfields;
	uint32	val;
}u_chcmd;

typedef struct _str_dma_cmd {
	uint32	u32rd_startaddr;
	uint32 	u32wr_startaddr;
	uint32	u32buff_size;
	u_chcmd	cmd;
}str_dma_tr_cmd;


__INLINE void dma_single_cmd_setup(uint8 channel, unsigned int rd_add, unsigned int wr_add, unsigned int len,unsigned char int_enable)
{
	unsigned int reg3_val = 0;
	REG_WR((DMA_CMD_RD_START_ADD + (0x100 * channel)), rd_add);
	REG_WR((DMA_CMD_WR_START_ADD + (0x100 * channel)), wr_add);
	REG_WR((DMA_CMD_BUFFER_SZ + (0x100 * channel)), len);
	reg3_val = (int_enable == 0)?0x2:0x3;
	REG_WR((DMA_CMD_REG3 + (0x100 * channel)), reg3_val);		
}

__INLINE void dma_channel_enable(uint8 channel, unsigned int en_dis)
{
	REG_WR((DMA_CHANNEL_ENABLE_REG + (0x100 * channel)),en_dis);
}

__INLINE void dma_channel_start(uint8 channel_list)
{
	//REG_WR((DMA_CHANNEL_START_REG + (0x100 * channel)),1);
	REG_WR(DMA_CORE0_CHANNEL_START,channel_list);
}

/* Enable DMA interrupt */
__INLINE void dma_interrupt_enable(uint8 channel, unsigned int int_en_lst)
{
	unsigned int int_val = REG_RD((DMA_INT_ENABLE_REG + (0x100 * channel)));
	//int_val = 0x0;
	int_val |= int_en_lst;
	REG_WR((DMA_INT_ENABLE_REG + (0x100 * channel)),int_val);
}

__INLINE unsigned int dma_get_enabled_interrupt(uint8 channel)
{
	return  REG_RD((DMA_INT_ENABLE_REG + (0x100 * channel)));
}

__INLINE void dma_interrupt_disable(uint8 channel, unsigned int int_dis_lst)
{
	unsigned int int_val = REG_RD((DMA_INT_ENABLE_REG + (0x100 * channel)));
	int_val &= ~(int_dis_lst);
	REG_WR((DMA_INT_ENABLE_REG + (0x100 * channel)),int_val);
}

__INLINE unsigned int dma_get_interrupt_status(uint8 channel)
{
	return REG_RD((DMA_INT_STATUS_REG + (0x100 * channel)));
}

/*
	return the channel number causing the interrupt.
*/

__INLINE unsigned int dma_get_interrupt_channel(unsigned int int_num)
{
	unsigned int int_idx = 0;
	unsigned int l_int_num = int_num;
	unsigned int add = 0;
	while((l_int_num = l_int_num >>1))		
		int_idx++;
	add = (DMA_INT0_STATUS + (int_idx * 4));
	return REG_RD(add);
	
}

__INLINE void dma_clear_interrupt(uint8 channel, unsigned int int_status)
{
	REG_WR((DMA_INT_CLEAR_REG + (0x100 * channel)),int_status);
}


#endif
