#include "posky.pri.h"

struct miiskira_posky_s *p;

const char* miiskira_posky_initial(uintptr_t argc, const char *const argv[])
{
	miiskira$type$posky_task = miiskira_posky_gen_type();
	miiskira$type$posky_return = miiskira_posky_gen_type();
	miiskira$type$posky_clear_lonely = miiskira_posky_gen_type();
	if ((p = inner_miiskira_posky_alloc(16 << 10, 64 << 10, 1 << 10)))
		return NULL;
	return "miiskira.posky.initial";
}

void miiskira_posky_finally(void)
{
	refer_free(p);
}
