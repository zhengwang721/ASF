/**
 * \file
 *
 * \brief BNO055 extension board example for SAME70.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "bno055_porting_i2c.h"

#ifdef	BNO055_API

/*--------------------------------------------------------------------------*
*	The following function is used to map the I2C bus read, write, delay and
*	device address with global structure bno055_t
*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 *  By using bno055 the following structure parameter can be accessed
 *	Bus write function pointer: BNO055_WR_FUNC_PTR
 *	Bus read function pointer: BNO055_RD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *--------------------------------------------------------------------------*/
 s8 I2C_routine(struct bno055_t *stru_bno055) {

	stru_bno055->bus_write = BNO055_I2C_bus_write;
	stru_bno055->bus_read = BNO055_I2C_bus_read;
	stru_bno055->delay_msec = BNO055_delay_msek;
	stru_bno055->dev_addr = BNO055_I2C_SLAVE_ADDRESS;

	return BNO055_ZERO_U8X;
}

/*	
 *  \Brief: The function is used as I2C bus init
 */
s8 BNO055_I2C_bus_init(void)
{
	twihs_options_t bno055_option;
	pmc_enable_periph_clk(BOARD_ID_TWIHS_EEPROM);

	/* Configure the options of TWI driver */
	bno055_option.master_clk = sysclk_get_cpu_hz();
	bno055_option.speed      = TWIHS_CLK;
	twihs_master_init(BNO055_IIC_INSRANCE, &bno055_option);
}

/*	\Brief: The function is used as I2C bus write
 *	\Return : Status of the I2C write
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, will data is going to be written
 *	\param reg_data : It is a value hold in the array,
 *		will be used for write the value into the register
 *	\param cnt : The no of byte of data to be write
 */
s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 iError = BNO055_ZERO_U8X;
	u8 array[I2C_BUFFER_LEN];
	u8 stringpos = BNO055_ZERO_U8X;
	array[BNO055_ZERO_U8X] = reg_addr;
	for (stringpos = BNO055_ZERO_U8X; stringpos < cnt; stringpos++) {
		array[stringpos + BNO055_ONE_U8X] = *(reg_data + stringpos);
	}
	/*
	* Please take the below function as your reference for
	* write the data using I2C communication
	* "IERROR = I2C_WRITE_STRING(DEV_ADDR, ARRAY, CNT+1)"
	* add your I2C write function here
	* iError is an return value of I2C read function
	* Please select your valid return value
	* In the driver SUCCESS defined as 0
    * and FAILURE defined as -1
	* Note :
	* This is a full duplex operation,
	* The first read data is discarded, for that extra write operation
	* have to be initiated. For that cnt+1 operation done in the I2C write string function
	* For more information please refer data sheet SPI communication:
	*/
	
	twihs_packet_t p_packet;
	p_packet.chip = dev_addr;
	p_packet.addr[0] = reg_addr >> 8;
	p_packet.addr[1] = reg_addr;
	p_packet.addr_length = 2;
	p_packet.buffer = reg_data;
	p_packet.length = cnt+1;
	
	iError = twihs_master_write(BNO055_IIC_INSRANCE, &p_packet);

	return (s8)iError;
}

 /*	\Brief: The function is used as I2C bus read
 *	\Return : Status of the I2C read
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, will data is going to be read
 *	\param reg_data : This data read from the sensor, which is hold in an array
 *	\param cnt : The no of byte of data to be read
 */
s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	s32 iError = BNO055_ZERO_U8X;
	u8 array[I2C_BUFFER_LEN] = {BNO055_ZERO_U8X};
	u8 stringpos = BNO055_ZERO_U8X;
	array[BNO055_ZERO_U8X] = reg_addr;
	/* Please take the below function as your reference
	 * for read the data using I2C communication
	 * add your I2C read function here.
	 * "IERROR = I2C_WRITE_READ_STRING(DEV_ADDR, ARRAY, ARRAY, 1, CNT)"
	 * iError is an return value of I2C write function
	 * Please select your valid return value
     * In the driver SUCCESS defined as 0
     * and FAILURE defined as -1
	 */
	
	twihs_packet_t p_packet;
	p_packet.chip = dev_addr;
	p_packet.addr[0] = reg_addr >> 8;
	p_packet.addr[1] = reg_addr;
	p_packet.addr_length = 2;
	p_packet.buffer = reg_data;
	p_packet.length = cnt;
	
	iError = twihs_master_read(BNO055_IIC_INSRANCE, &p_packet);
	
	for (stringpos = BNO055_ZERO_U8X; stringpos < cnt; stringpos++) {
		*(reg_data + stringpos) = array[stringpos];
	}
	return (s8)iError;
}
/*	Brief : The delay routine
 *	\param : delay in ms
*/
void BNO055_delay_msek(u32 msek)
{
	/*Here you can write your own delay routine*/
	delay_ms(msek);
}
#endif

