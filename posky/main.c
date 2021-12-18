#include "posky.pri.h"

struct miiskira_posky_s *p;

uint64_t miiskira$posky$stamp;
uint32_t miiskira$type$posky_task;
uint32_t miiskira$type$posky_return;
uint32_t miiskira$type$posky_clear_lonely;
uint32_t miiskira$type$posky_proxy_adorable;

static inline void miiskira_posky_initial_type(void)
{
	miiskira$type$posky_task = miiskira_posky_gen_type();
	miiskira$type$posky_return = miiskira_posky_gen_type();
	miiskira$type$posky_clear_lonely = miiskira_posky_gen_type();
	miiskira$type$posky_proxy_adorable = miiskira_posky_gen_type();
}

const char* miiskira_posky_initial(uintptr_t argc, const char *const argv[])
{
	uintptr_t adorable_slot_size, posky_size, adorable_posky_size;
	adorable_slot_size  = 16 << 10; // 16 KiB
	posky_size          = 64 << 10; // 64 KiB
	adorable_posky_size =  1 << 10; //  1 KiB
	miiskira_posky_initial_type();
	log_verbose("[posky] posky create param: a_limit: %zu, pq_size: %zu, apq_size: %zu", adorable_slot_size, posky_size, adorable_posky_size);
	if ((p = inner_miiskira_posky_alloc(adorable_slot_size, posky_size, adorable_posky_size)))
	{
		log_info("[posky] create adorable (%s:%016lx)", miiskira$posky$posky, p->address);
		return NULL;
	}
	return "miiskira.posky.initial";
}

void miiskira_posky_finally(void)
{
	refer_free(p);
}
