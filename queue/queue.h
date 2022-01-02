#ifndef _miiskira_queue_h_
#define _miiskira_queue_h_

#include <queue/queue.h>
#include <queue/queue_ring.h>
#include <yaw.h>

typedef struct miiskira_queue_s {
	queue_s queue;
	queue_ring_s *ring;
	yaw_signal_s *signal;
} miiskira_queue_s;

miiskira_queue_s* miiskira_queue_alloc(uintptr_t queue_size, yaw_signal_s *signal);

#endif
