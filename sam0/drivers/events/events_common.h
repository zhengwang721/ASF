#ifndef _EVENTS_COMMON_H_INCLUDED_
#define _EVENTS_COMMON_H_INCLUDED_

struct _events_module {
        volatile uint32_t allocated_channels;
        uint8_t           free_channels;

#if EVENTS_INTERRUPT_HOOKS_MODE == true
        volatile uint32_t interrupt_flag_buffer;
        volatile uint32_t interrupt_flag_ack_buffer;

	struct events_hook *hook_list;
#endif
};

#endif
