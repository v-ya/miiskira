#include "posky.pri.h"

struct miiskira_posky_queue_s {
	queue_s queue;
	queue_ring_s *ring;
	yaw_signal_s *signal;
};

static void inner_miiskira_posky_queue_free_func(struct miiskira_posky_queue_s *restrict r)
{
	if (r->ring) refer_free(r->ring);
	if (r->signal) refer_free(r->signal);
}

static queue_s* inner_miiskira_posky_queue_push(struct miiskira_posky_queue_s *restrict q, refer_t v)
{
	if (queue_ring_push(q->ring, v))
	{
		register yaw_signal_s *restrict s;
		s = q->signal;
		yaw_signal_inc(s);
		yaw_signal_wake(s, ~0);
		return &q->queue;
	}
	return NULL;
}

static refer_t inner_miiskira_posky_queue_pull(struct miiskira_posky_queue_s *restrict q)
{
	return queue_ring_pull(q->ring);
}

queue_s* inner_miiskira_posky_queue_alloc(uintptr_t queue_size, yaw_signal_s **restrict p_signal)
{
	struct miiskira_posky_queue_s *restrict r;
	if (queue_size && p_signal)
	{
		if ((r = (struct miiskira_posky_queue_s *) refer_alloz(sizeof(struct miiskira_posky_queue_s))))
		{
			refer_set_free(r, (refer_free_f) inner_miiskira_posky_queue_free_func);
			if (!*p_signal)
				r->signal = yaw_signal_alloc();
			else r->signal = (yaw_signal_s *) refer_save(*p_signal);
			if (r->signal && (r->ring = queue_ring_alloc(queue_size)))
			{
				r->queue.push = (queue_push_f) inner_miiskira_posky_queue_push;
				r->queue.pull = (queue_pull_f) inner_miiskira_posky_queue_pull;
				if (!*p_signal)
					*p_signal = (yaw_signal_s *) refer_save(r->signal);
				return &r->queue;
			}
		}
	}
	return NULL;
}
