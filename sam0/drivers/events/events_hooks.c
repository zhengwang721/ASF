extern struct _events_inst;

enum status_code events_add_hook(struct events_resource *resource, events_interrupt_hook hook)
{
}

enum status_code events_del_hook(struct events_resource *resource, events_interrput_hook hook)
{
}

enum events_interrput_source  events_get_interrupt_source(struct events_resource resource);
{
}

enum status_code events_ack_interrupt_source(struct events_resource, enum events_interrput_source source);
{
}


