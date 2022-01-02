#include "queue.h"
#include "../log/log.h"

static void inner_miiskira_queue_free_func(miiskira_queue_s *restrict r)
{
	if (r->ring) refer_free(r->ring);
	if (r->signal) refer_free(r->signal);
}

static queue_s* inner_miiskira_queue_push(miiskira_queue_s *restrict q, refer_t v)
{
	if (queue_ring_push(q->ring, v))
	{
		register yaw_signal_s *restrict s;
		s = q->signal;
		yaw_signal_inc(s);
		yaw_signal_wake(s, ~0);
		return &q->queue;
	}
	log_warning("[queue] push fail: have %zu/%zu", queue_ring_number(q->ring), queue_ring_size(q->ring));
	return NULL;
}

static refer_t inner_miiskira_queue_pull(miiskira_queue_s *restrict q)
{
	return queue_ring_pull(q->ring);
}

miiskira_queue_s* miiskira_queue_alloc(uintptr_t queue_size, yaw_signal_s *signal)
{
	miiskira_queue_s *restrict r;
	if (queue_size)
	{
		if ((r = (miiskira_queue_s *) refer_alloz(sizeof(miiskira_queue_s))))
		{
			refer_set_free(r, (refer_free_f) inner_miiskira_queue_free_func);
			if (!signal) r->signal = yaw_signal_alloc();
			else r->signal = (yaw_signal_s *) refer_save(signal);
			if (r->signal && (r->ring = queue_ring_alloc(queue_size)))
			{
				r->queue.push = (queue_push_f) inner_miiskira_queue_push;
				r->queue.pull = (queue_pull_f) inner_miiskira_queue_pull;
				return r;
			}
		}
	}
	return NULL;
}
