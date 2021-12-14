#ifndef _miiskira_posky_pri_h_
#define _miiskira_posky_pri_h_

#include "posky.h"
#include "../define.h"
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

extern struct miiskira_posky_s *p;

queue_s* inner_miiskira_posky_queue_alloc(uintptr_t queue_size, yaw_signal_s **restrict p_signal);

struct miiskira_posky_s* inner_miiskira_posky_alloc(uintptr_t adorable_slot_size, uintptr_t posky_size, uintptr_t adorable_posky_size);

#endif
