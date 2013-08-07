#include "asf.h"

int main(void)
{
	struct wing_list list;
	struct wing_object oled;

	wing_probe(&list);

	wing_spawn(&list, WING_TYPE_OLED1, &oled);

	return 0;
}
