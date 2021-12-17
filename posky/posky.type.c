#include "posky.h"

uint32_t miiskira_posky_gen_type(void)
{
	static uint32_t type_last;
	return __sync_add_and_fetch(&type_last, 1);
}
