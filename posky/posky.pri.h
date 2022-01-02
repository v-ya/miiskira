#ifndef _miiskira_posky_pri_h_
#define _miiskira_posky_pri_h_

#include "posky.h"
#include "../define.h"
#include "../log/log.h"
#include "../queue/queue.h"
#include <vattr.h>

struct miiskira_posky_s;

struct miiskira_posky_lady_s {
	struct miiskira_posky_s *p;
};

struct miiskira_posky_s {
	posky_s *posky;
	struct miiskira_posky_lady_s *lady;
	posky_adorable_s *adorable;
	yaw_signal_s *signal;
	vattr_s *pool;
	yaw_s *yaw;
	uint64_t address;
};

extern struct miiskira_posky_s *p;

extern uint32_t miiskira$type$posky_proxy_adorable;
extern uint32_t miiskira$type$posky_clear_lonely;
extern uint32_t miiskira$type$posky_trick;
extern uint32_t miiskira$type$posky_foster_care;

struct miiskira_posky_s* inner_miiskira_posky_alloc(uintptr_t adorable_slot_size, uintptr_t posky_size, uintptr_t adorable_posky_size);
const char* inner_miiskira_posky_get_name(posky_s *restrict posky, uint64_t address);

#endif
