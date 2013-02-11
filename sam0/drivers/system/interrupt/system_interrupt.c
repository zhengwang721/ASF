#include "system_interrupt.h"

bool system_interrupt_is_pending(enum system_interrupt_vector vector)
{
	bool result;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		result = (bool)((NVIC->ISPR[0] >> vector) & 0x00000001);

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {

		result = (bool)((SCB->ICSR >> SCB_ICSR_PENDSTSET_Pos) & 0x00000001);

	} else {

		Assert(false);
		result = false;
	}

	return result;
}

enum status_code system_interrupt_set_pending(enum system_interrupt_vector vector)
{
	enum status_code status = STATUS_OK;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		NVIC->ISPR[0] = 1 << vector;

	} else if((vector == SYSTEM_INTERRUPT_SYSTICK) || (vector == SYSTEM_INTERRUPT_NON_MASKABLE)) {
		/* Because NMI has highest priority it will be executed immediately after it has been set pending */

		uint8_t vector_to_pos_translater[] = {(uint8_t)(1 << SCB_ICSR_NMIPENDSET_Pos),(uint8_t)( 1 << SCB_ICSR_PENDSTSET_Pos)};

		SCB->ICSR = vector_to_pos_translater[vector == SYSTEM_INTERRUPT_SYSTICK];

	} else {

		/* The user want to set something unsopported as pending */
		Assert(false);
		status = STATUS_ERR_INVALID_ARG;

	}

	return status;
}

enum status_code system_interrupt_clear_pending(enum system_interrupt_vector vector)
{

	enum status_code status = STATUS_OK;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		NVIC->ICPR[0] = 1 << vector;

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {
		/* Clearing of NMI pending interrupts does not make sense and is not supported by the
		   device because it has the highest priority and will executed at the moment it is set */

		SCB->ICSR = (uint32_t)(1 << SCB_ICSR_PENDSTCLR_Pos);

	} else {

		Assert(false);
		status = STATUS_ERR_INVALID_ARG;
	}

	return status;
}

enum status_code system_interrupt_set_priority(enum system_interrupt_vector vector,
		enum system_interrupt_priority_level priority_level)
{
	enum status_code status = STATUS_OK;

	if(vector >= _SYSTEM_INTERRUPT_EXTERNAL_VECTOR_START) {

		uint8_t register_num = vector / 4;
		uint8_t priority_pos = ((vector % 4) * 8) + 5;

		NVIC->IP[register_num] = priority_level << priority_pos;

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {

		SCB->SHP[1] = priority_level << _SYSTEM_INTERRUPT_SYSTICK_PRI_POS;

	} else {

		Assert(false);
		status = STATUS_ERR_INVALID_ARG;

	}

	return status;
}

enum system_interrupt_priority_level system_interrupt_get_priority(enum system_interrupt_vector vector)
{
	uint8_t register_num = vector / 4;
	uint8_t priority_pos = ((vector % 4) * 8) + 5;
	enum system_interrupt_priority_level priority = SYSTEM_INTERRUPT_PRIORITY_LEVEL_0;

	if (vector >= 0) {

		priority = (NVIC->IP[register_num] >> priority_pos) & _SYSTEM_INTERRUPT_PRIORITY_MASK;

	} else if (vector == SYSTEM_INTERRUPT_SYSTICK) {

		priority = (SCB->SHP[1] >> _SYSTEM_INTERRUPT_SYSTICK_PRI_POS) & _SYSTEM_INTERRUPT_PRIORITY_MASK;

	}

	return priority;
}

