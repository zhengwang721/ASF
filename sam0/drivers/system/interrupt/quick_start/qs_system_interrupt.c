#include "asf.h"

int main(void)
{
	system_init();

	system_interrupt_enter_critical_section();

	if(!system_interrupt_is_enabled(SYSTEM_INTERRUPT_MODULE_SERCOM0)) {

		system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_SERCOM0);

	}

	system_interrupt_leave_critical_section();


	while(!system_interrupt_is_global_enabled()) {
		system_interrupt_leave_critical_section();
	}

	system_interrupt_enter_critical_section();

	if(system_interrupt_is_enabled(SYSTEM_INTERRUPT_MODULE_SERCOM0)) {

		system_interrupt_disable(SYSTEM_INTERRUPT_MODULE_SERCOM0);

	}

	system_interrupt_leave_critical_section();

	while(1) {
	}

	return 0;
}
