#include "asf.h"


void configure_dac(void);
void configure_dac_channel(void);

struct dac_module dac_instance;
struct events_resource event_dac;
struct rtc_module rtc_instance;
struct events_config event_config;

void rtc_overflow_callback(void)
{
	
}

void configure_rtc_count(void)
{

	struct rtc_count_events  rtc_event;
	struct rtc_count_config config_rtc_count;
	rtc_count_get_config_defaults(&config_rtc_count);

	config_rtc_count.prescaler           = RTC_COUNT_PRESCALER_DIV_1;
	config_rtc_count.mode                = RTC_COUNT_MODE_16BIT;
	config_rtc_count.continuously_update = true;

	rtc_count_init(&rtc_instance, RTC, &config_rtc_count);
	rtc_event.generate_event_on_overflow = true;
	rtc_count_enable_events(&rtc_instance, &rtc_event);
	rtc_count_enable(&rtc_instance);

}


void configure_rtc_callbacks(void)
{
	rtc_count_register_callback(&rtc_instance,
	rtc_overflow_callback, RTC_COUNT_CALLBACK_OVERFLOW);
	rtc_count_enable_callback(&rtc_instance,RTC_COUNT_CALLBACK_OVERFLOW);
}

void dac_callback(uint8_t channel)
{
	static uint16_t i;

	DAC->DATABUF.reg = i;
	i+=20;
	if (i >= 0x3FF) {
		i = 0;
	}
}

void configure_dac(void)
{
	struct dac_config config_dac;
	dac_get_config_defaults(&config_dac);
	config_dac.reference      = DAC_REFERENCE_AVCC;
	dac_instance.start_on_event = true;
	dac_init(&dac_instance, DAC, &config_dac);
	
		/* Enable interrupt */
	DAC->INTENSET.reg = DAC_INTFLAG_UNDERRUN | DAC_INTFLAG_EMPTY;
	DAC->EVCTRL.reg = DAC_EVCTRL_STARTEI;
	dac_enable(&dac_instance);
}

void configure_dac_channel(void)
{

	struct dac_chan_config config_dac_chan;
	dac_chan_get_config_defaults(&config_dac_chan);
	dac_chan_set_config(&dac_instance, DAC_CHANNEL_0, &config_dac_chan);
	dac_chan_enable(&dac_instance, DAC_CHANNEL_0);

}


int main(void)
{

	system_clock_init();
//	_system_events_init();
	/* Enable EVSYS register interface */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, PM_APBCMASK_EVSYS);

	/* Make sure the EVSYS module is properly reset */
	EVSYS->CTRL.reg = EVSYS_CTRL_SWRST;

	while (EVSYS->CTRL.reg & EVSYS_CTRL_SWRST) {
	}

	configure_rtc_count();
	configure_rtc_callbacks();
	rtc_count_set_period(&rtc_instance,1);

	configure_dac();
	configure_dac_channel();
	dac_register_callback(&dac_instance,dac_callback,DAC_CALLBACK_DATA_UNDERRUN);
	dac_chan_enable_callback(&dac_instance,DAC_CHANNEL_0,DAC_CALLBACK_DATA_UNDERRUN);

	system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_DAC);


	events_get_config_defaults(&event_config);
	event_config.generator      = EVSYS_ID_GEN_RTC_OVF;
	event_config.edge_detect    = EVENTS_EDGE_DETECT_RISING;
	event_config.path           = EVENTS_PATH_ASYNCHRONOUS;
	event_config.clock_source   = GCLK_GENERATOR_0;

	events_allocate(&event_dac, &event_config);
	events_attach_user(&event_dac, EVSYS_ID_USER_DAC_START);

	while (1) {
	}

}

