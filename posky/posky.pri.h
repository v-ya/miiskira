#ifndef _miiskira_posky_pri_h_
#define _miiskira_posky_pri_h_

#include "posky.h"
#include "../define.h"
#include "../log/log.h"
#include <queue/queue_ring.h>
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

struct miiskira_posky_adorable_s {
	posky_adorable_s *adorable;
	yaw_signal_s *signal;
	yaw_s *yaw;
	uint64_t address;
};

struct miiskira_candy__posky_proxy_s {
	posky_candy_s candy;
	const char *name;
	posky_candy_s *gift;
};

extern struct miiskira_posky_s *p;

extern uint32_t miiskira$type$posky_proxy_adorable;

queue_s* inner_miiskira_posky_queue_alloc(uintptr_t queue_size, yaw_signal_s **restrict p_signal);

posky_adorable_s* inner_miiskira_posky_alloc_create_adorable(posky_s *restrict posky, const char *restrict name, refer_t lady, uintptr_t adorable_posky_size, yaw_signal_s **restrict signal);

struct miiskira_posky_s* inner_miiskira_posky_alloc(uintptr_t adorable_slot_size, uintptr_t posky_size, uintptr_t adorable_posky_size);

#endif
