#ifndef I2C_COMMON_H_INCLUDED
#define I2C_COMMON_H_INCLUDED

#include <asf.h>

#define I2C_WRITE_TO_SLAVE	0
#define I2C_READ_FROM_SLAVE	1

static inline void _i2c_disable(I2C *const i2c_module)
{
	if(i2c_module == NULL)
		return;
	//i2c_module->ENABLE = I2C_DISABLE;
}

static inline void _i2c_enable(I2C *const i2c_module)
{
	if(i2c_module == NULL)
		return;
	//i2c_module->ENABLE = I2C_ENABLE;
}

enum i2c_status_code _i2c_set_config(
									I2C *const i2c_module,
									uint16_t *pinmux_pad,
									uint32_t baud_rate,
									uint8_t clock_source);
									
enum i2c_status_code _i2c_reset(I2C *const i2c_module);
									
#endif	//I2C_COMMON_H_INCLUDED
