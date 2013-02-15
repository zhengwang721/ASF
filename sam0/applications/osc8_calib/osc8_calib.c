/**
 * \file
 *
 * \brief SAM0+ OSC8MHz Calibration Application
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
#include <asf.h>
#include <math.h>

#define RES 255
#define SCALE 32768/RES

static struct usart_dev_inst usart_edbg;

void debug_write_string(struct usart_dev_inst *const dev, uint8_t const *string);

void debug_int_to_string(uint8_t *ret_val, uint8_t size, uint32_t integer);

void setup_tc_channels(struct tc_module *const calib_chan, struct tc_module *const comp_chan);

void setup_usart_channel(void);

uint32_t debug_get_freq(struct tc_module *calib_chan, struct tc_module *comp_chan);

void debug_write_string(struct usart_dev_inst *const dev, uint8_t const *string)
{
	do {
		while (usart_write(dev, *string) != STATUS_OK) {
		}
	} while (*(++string) != 0);
}

void debug_int_to_string(uint8_t *ret_val, uint8_t size, uint32_t integer)
{
    uint32_t cnt = 10;
    uint32_t left;
    uint8_t counter=0;

    for(counter = 0; counter < size; counter++){
        ret_val[counter] = 0;
    }

    counter = 0;

    while(1) {
        if(integer / cnt != 0){
            cnt *= 10;
        } else {
            cnt /= 10;
            break;
        }
        counter++;
    }

    if (counter > size) {
    	return;
    }

    counter = 0;

    while (1) {
        left = integer/cnt;
        integer -= left*cnt;
        ret_val[counter++] = left+48;

        cnt /= 10;
        if (cnt < 1) break;
    }
}

void setup_tc_channels(struct tc_module *const calib_chan, struct tc_module *const comp_chan)
{
	// TODO: Update with event system
	struct tc_conf config;
	tc_get_config_defaults(&config);

	config.counter_size = TC_COUNTER_SIZE_32BIT;
	config.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;

	tc_init(calib_chan, TC0, &config);

	config.counter_size = TC_COUNTER_SIZE_16BIT;
	config.clock_source = GCLK_GENERATOR_3;
	config.size_specific.size_16_bit.compare_capture_channel[0] = RES;
	tc_init(comp_chan, TC2, &config);

	tc_enable(calib_chan);
	tc_enable(comp_chan);
}

void setup_usart_channel(void)
{
	struct usart_conf config_struct;

	usart_get_config_defaults(&config_struct);
	config_struct.mux_settings = USART_RX_3_TX_2_XCK_3;
	config_struct.pinout_pad3 = EDBG_CDC_RX_PINMUX;
	config_struct.pinout_pad2 = EDBG_CDC_TX_PINMUX;

	while (usart_init(&usart_edbg, EDBG_CDC_MODULE,
			&config_struct) != STATUS_OK) {
	}

	usart_enable(&usart_edbg);

	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_edbg, USART_TRANSCEIVER_RX);
}

uint32_t debug_get_freq(struct tc_module *calib_chan, struct tc_module *comp_chan)
{
	uint32_t tmp;

	tc_clear_interrupt_flag(comp_chan, TC_INTERRUPT_FLAG_CHANNEL_0);

	calib_chan->hw_dev->COUNT32.COUNT.reg = 0;
	comp_chan->hw_dev->COUNT16.COUNT.reg = 0;

	while (!tc_is_interrupt_flag_set(comp_chan, TC_INTERRUPT_FLAG_CHANNEL_0));
	tmp = calib_chan->hw_dev->COUNT32.COUNT.reg;

	return tmp * SCALE;
}

int main(void)
{
	uint8_t string[10];

	system_init();

	uint32_t tmp;

	/* Structures for config and software device instance */
	struct tc_module calib_chan;
	struct tc_module comp_chan;

	/* Set up tc modules to be used. */
	setup_tc_channels(&calib_chan, &comp_chan);

	/* Values for calculating current frequency of osc. */
	uint8_t comm_cal;
	uint8_t frange_cal;
	uint8_t comm_best=0;
	uint8_t frange_best=0;
	uint32_t freq_best = 0xffffffff;
	uint32_t freq_before = debug_get_freq(&calib_chan, &comp_chan);
	uint32_t gen_freq = system_gclk_chan_get_hz(SERCOM_GCLK_ID);

	for (frange_cal = 1; frange_cal < 4; frange_cal++) {
		for (comm_cal = 0; comm_cal < 127; comm_cal++) {
			system_clock_source_write_calibration(SYSTEM_CLOCK_SOURCE_OSC8M, comm_cal | 8 << 7, frange_cal);

			tmp = debug_get_freq(&calib_chan, &comp_chan);

			if (abs(tmp - gen_freq) <
				abs(freq_best - gen_freq))
			{
				freq_best = tmp;
				comm_best = comm_cal;
				frange_best = frange_cal;
			}
		}
	}
	/* Set the found best calibration. */
	system_clock_source_write_calibration(SYSTEM_CLOCK_SOURCE_OSC8M, comm_best | 8 << 7, frange_best);

	/* Setup usart module to give information back. */
	setup_usart_channel();

	/* Get calibrated value. */
	tmp = debug_get_freq(&calib_chan, &comp_chan);

	/* Write freq. before and now, together with calibration values to usart. */
	debug_int_to_string(string, 10, freq_before);
	debug_write_string(&usart_edbg, (uint8_t*)"Before: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	debug_int_to_string(string, 10, tmp);
	debug_write_string(&usart_edbg, (uint8_t*)"Now: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	debug_int_to_string(string, 10, frange_best);
	debug_write_string(&usart_edbg, (uint8_t*)"Frequency range: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	debug_int_to_string(string, 10, comm_best | 8 << 7);
	debug_write_string(&usart_edbg, (uint8_t*)"Calibration value: ");
	debug_write_string(&usart_edbg, string);
	debug_write_string(&usart_edbg, (uint8_t*)"\r\n");

	/* Deactivate tc modules. */
	tc_disable(&calib_chan);
	tc_disable(&comp_chan);
}
