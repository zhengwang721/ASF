#include "dma.h"
#include "dma_if.h"

static unsigned int dma_complete_int = 0;
dma_req *active_req;
str_dma_tr_cmd cmd_list[4] __attribute__((at(0x10001000)));
//str_dmacmd cmd_list[4]; // __attribute__((at(0x10003F00)));
void dma_transfer_complete_handler()
{
	uint32 length = 0;
	if(active_req) {
		if(active_req->dma_cb) {
			length = (REG_RD((DMA_COUNT_REG + (0x100 * active_req->channel))) & 0xFFFF);
			if(length >= 1) {
				active_req->dma_cb(active_req->cb_context,active_req->desc.dest_config.length);
				active_req = NULL;
			}
		}
	}
}

static uint32 get_priority_channel_num(uint32 channel)
{
	uint32 channel_list = 0;
	uint32 channel_index = 0;
	//only first four channels in the design.
	channel_list = channel & 0xF;
	do {
		channel_list = channel_list >> 1;
		channel_index++;
	}while(channel_list);
	return(channel_index - 1);
}

void DMA_Handler(void)
{
	/* 
		Interrupt handling should be done as follows:
		- Read the correct INTX_STATUS register to figure out which channel caused the interrupt.
		- Read the channel's INT_STATUS_REG to figure out which interrupt to handle.
		- Clear the interrupt by writing to the corresponding INT_CLEAR_REG.
	*/
	unsigned int channel_num = 0;
	unsigned int channel = dma_get_interrupt_channel(0);
	while(channel != 0) {
		channel_num = get_priority_channel_num(channel);
		unsigned int int_status = dma_get_interrupt_status(channel_num);
		unsigned int enabled_int = dma_get_enabled_interrupt(channel_num);
		if(enabled_int & int_status) {
			// Got an interrupt that is enabled.
			// Clear the pending interrupt.
			//dma_clear_interrupt(int_status);
			if(int_status & DMA_INT_CH_END) {
				dma_complete_int = 1;
				//dma_transfer_complete_handler();
				dma_clear_interrupt(channel_num,DMA_INT_CH_END);
			}
		}
		channel &= ~(1 << channel_num);
	};
	NVIC_ClearPendingIRQ(DMA_IRQn);
}

void dma_enable_done_interrupt(uint8 channel)
{

	// Enable DMA interrupt
	dma_interrupt_enable(channel,DMA_INT_CH_END);
	NVIC_EnableIRQ(DMA_IRQn);
}

static void select_dma_interrupt_pinmux()
{
	volatile uint32 regval = 0;
	regval = REG_RD(0x4000B0CC);
	regval &= ~(0x1F << 24);
	regval |= (16 << 24);
	REG_WR(0x4000B0CC,regval);
}

void dma_init(uint8 channel)
{
	struct dma_static_reg0_cfg reg0_cfg;
	struct dma_static_reg1_cfg reg1_cfg;
	struct dma_static_reg2_cfg reg2_cfg;
	struct dma_static_reg4_cfg reg4_cfg;
	
	reg0_cfg.rd_burst_max_size = 4;
	reg0_cfg.rd_allow_full_burst = 0;
	reg0_cfg.rd_allow_full_fifo = 0;
	reg0_cfg.rd_tokens = 4;
	reg0_cfg.rd_outstanding = 0;
	reg0_cfg.rd_incr = 1;

	
	reg1_cfg.wr_burst_max_size = 4;
	reg1_cfg.wr_allow_full_burst = 0;
	reg1_cfg.wr_allow_full_fifo = 0;
	reg1_cfg.wr_tokens = 1;
	reg1_cfg.wr_outstanding = 0;
	reg1_cfg.wr_incr = 1;


	memset(&reg2_cfg,0x0,sizeof(reg2_cfg));
	memset(&reg4_cfg,0x0,sizeof(reg4_cfg));
	
	// Program the Static registers
	static_reg0_write(channel,&reg0_cfg);
	static_reg1_write(channel,&reg1_cfg);
	static_reg2_write(channel,&reg2_cfg);
	static_reg3_write(channel,0,0);
	static_reg4_write(channel,&reg4_cfg);	
	
	// Program CORE0_JOINT_MODE register
	REG_WR(DMA_CORE0_JOINT_MODE,0x1);	//Joint Mode
	// Program PERIPH_TX_CTRL register
	REG_WR(DMA_PERIPH_TX_CTRL,0xFFFF);
	REG_WR(DMA_PERIPH_TX_CTRL,0xFFFE);
}

void dma_test(void)
{
	int i;
	volatile unsigned int chipID;
	unsigned int sz = 0;
	unsigned int count = 0;
	unsigned int rd_addr = 0;
	unsigned int wr_addr = 0;
	int passed;
	//Memory Prefill
	//memset(((void *)0x10004000),0xFF,1024);
	for(i=0;i<1024;i++) {
		*(((unsigned char *)0x10004000)+i) = (unsigned char)(i & 0xFF);
	}
	memset(((void *)0x10008000),0x0,1024);
	rd_addr = 0x10004000;
	wr_addr = 0x10008000;
	sz = 277;
	dma_init(CHANNEL_0);
	dma_enable_done_interrupt(CHANNEL_0);
	//while((sz += 32)<=1024)
	{
		dma_single_cmd_setup(CHANNEL_0,rd_addr, wr_addr,sz,1);
		dma_channel_enable(CHANNEL_0,1);
		dma_channel_start(CHANNEL_0);
	}
	do {
		//Do some AHB transfers in order to contend with the DMA operation
		chipID = REG_RD(0x4000B000);
		count++;
	}while(!dma_complete_int /*&& (count < 1000)*/);

	if(dma_complete_int == 1) {
		dma_complete_int = 0;
		//check memory now.
		count = memcmp((const void *)rd_addr,(const void *)wr_addr,sz);
			if(count == 0) {
				//Passed.
				passed = 1;
			}
			else {
				//Failed
				passed = 0;
			}
	}
}

void dma_test2(void)
{
	memset(((void *)0x10018000),0xab,1024);

	/* Data to be transfered */
	REG_WR(0x10001008,0x23);
	REG_WR(0x1000100c,0x26);
	


	//REG_WR(0x40002854,0xFFFF);

	/* CMD */
	REG_WR(0x40002000,0x10001008); 	//CMD_REG0
	REG_WR(0x40002004,0x100010f0);	//CMD_REG1
	REG_WR(0x40002008,0x8);			//CMD_REG2
	REG_WR(0x4000200C,0x3);			//CMD_REG3
	/* CMD- END */

	
	REG_WR(0x40002010,0x80010004);		//Static_REG0
	REG_WR(0x40002014,0x80040004);		//Static_REG1
	REG_WR(0x4000201C,0x0);				//Static_REG3
	REG_WR(0x40002020,0x0);				//Static REG4


	//REG_WR(0x40002850,0xffff);
	REG_WR(0x40002040,0x1);			//Channel enable
	REG_WR(0x40002044,0x1);			//Channel start

	
	//REG_WR(0x40002004,0x100010F0);
	//REG_WR(0x40002014,0x80040004); 



}

static void program_static_regs(dma_req *req)
{
	struct dma_static_reg0_cfg reg0_cfg;
	struct dma_static_reg1_cfg reg1_cfg;
	struct dma_static_reg2_cfg reg2_cfg;
	struct dma_static_reg4_cfg reg4_cfg;
	//Program Static Reg 0
	reg0_cfg.rd_burst_max_size = req->desc.src_config.burst_size;
	reg0_cfg.rd_allow_full_burst = 0;
	reg0_cfg.rd_allow_full_fifo = 0;
	reg0_cfg.rd_tokens = 1;
	reg0_cfg.rd_outstanding = req->desc.src_config.outstanding_mode;
	reg0_cfg.rd_incr = req->desc.src_config.incr_addr;
	static_reg0_write(req->channel,&reg0_cfg);
	//Program Static Reg 1
	reg1_cfg.wr_burst_max_size = req->desc.dest_config.burst_size;
	reg1_cfg.wr_allow_full_burst = 0;
	reg1_cfg.wr_allow_full_fifo = 0;
	reg1_cfg.wr_tokens = 1;
	reg1_cfg.wr_outstanding = req->desc.dest_config.outstanding_mode;
	reg1_cfg.wr_incr = req->desc.dest_config.incr_addr;
	static_reg1_write(req->channel,&reg1_cfg);
	//Program Static Reg 2
	reg2_cfg.auto_retry = req->block_mode.auto_retry;
	reg2_cfg.block = req->block_mode.enable;
	reg2_cfg.end_swap = req->block_mode.swap;
	reg2_cfg.frame_width = req->block_mode.frame_width;
	reg2_cfg.int_num = 0;
	reg2_cfg.joint = req->joint_mode;
	static_reg2_write(req->channel,&reg2_cfg);
	//Program Static Reg 4
	reg4_cfg.rd_periph_num = req->desc.src_config.periph_num;
	reg4_cfg.rd_periph_delay = req->desc.src_config.periph_delay;
	reg4_cfg.rd_periph_block = (req->block_mode.enable);
	reg4_cfg.wr_periph_num = req->desc.dest_config.periph_num;
	reg4_cfg.wr_periph_delay = req->desc.dest_config.periph_delay;
	reg4_cfg.wr_periph_block = (req->block_mode.enable);
	static_reg4_write(req->channel,&reg4_cfg);
}

void dma_channel_init(void *channel_config)
{
	//dma_channel_config *p_ch_cfg = (dma_channel_config *)channel_config;
	
}

void submit_dma_transfer_req(dma_req *req)
{
	transfer_desc *curr_req;
	uint32 reg_val = 0;
	uint8 cmd_num = 0;
	uint8 no_intr = 0;
	if(req->desc.src_config.length != req->desc.dest_config.length)
		return;
	if(active_req != NULL)
		return;
	//active_req = req;
	program_static_regs(req);
	reg_val = REG_RD((DMA_INT_STATUS_REG + (0x100 * req->channel)));
	dma_clear_interrupt(req->channel,reg_val);
	dma_interrupt_disable(req->channel,0xff);
	select_dma_interrupt_pinmux();
	// Disabling watchdog interrupt and enabling rest of the interrupt.
	if(req->dma_cb != 0) {
		dma_interrupt_enable(req->channel,0x7f);
		NVIC_EnableIRQ(DMA_IRQn);
		active_req = req;
	}
	else {
		no_intr = 1;
	}
	curr_req = &req->desc;
	do {
		if(req->desc.next == 0) {
			//Single command transfer
			dma_single_cmd_setup(req->channel,req->desc.src_config.start_addr,req->desc.dest_config.start_addr,req->desc.dest_config.length,1);
			curr_req = 0;	
		}
		else {
			//Need to program the command list here.
			cmd_list[cmd_num].u32rd_startaddr = curr_req->src_config.start_addr;
			cmd_list[cmd_num].u32wr_startaddr = curr_req->dest_config.start_addr;
			cmd_list[cmd_num].u32buff_size = curr_req->src_config.length;
			if((curr_req->next == 0) || (cmd_num == 3)) {
				cmd_list[cmd_num].cmd.val = 0x3;
			}
			else {
				//cmd_list[cmd_num].cmd.str_bfields.cmd_next = (uint32)&cmd_list[cmd_num+1];
				//cmd_list[cmd_num].cmd.str_bfields.cmd_last = 0;
				//cmd_list[cmd_num].cmd.str_bfields.cmd_set_int = 0;
				cmd_list[cmd_num].cmd.val = (((uint32)&cmd_list[cmd_num+1]) & ~(0xF));
			}
			cmd_num++;
			curr_req = curr_req->next;
		}
	}while((curr_req != 0) && (cmd_num < 4));
	if(cmd_num > 0)
		REG_WR((DMA_CMD_REG3 + (0x100 * req->channel)),(uint32)&cmd_list[0]);
	dma_complete_int = 0;
	dma_channel_enable(req->channel,1);
	dma_channel_start(req->tr_start);
	//dma_enable_done_interrupt();
	while((!dma_complete_int) && (!no_intr));
	dma_complete_int = 0;
	if(req->dma_cb != 0) {
		NVIC_DisableIRQ(DMA_IRQn);
		dma_transfer_complete_handler();
	}
}
