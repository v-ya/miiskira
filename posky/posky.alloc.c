#include "posky.pri.h"

static void inner_miiskira_posky_free_func(struct miiskira_posky_s *restrict r)
{
	if (r->yaw)
	{
		yaw_stop(r->yaw);
		yaw_signal_inc(r->signal);
		yaw_signal_wake(r->signal, ~0);
		yaw_wait(r->yaw);
		refer_free(r->yaw);
	}
	if (r->pool)
		refer_free(r->pool);
	if (r->signal)
		refer_free(r->signal);
	if (r->adorable)
		refer_free(r->adorable);
	if (r->lady)
		refer_free(r->lady);
	if (r->posky)
		refer_free(r->posky);
}

static void inner_miiskira_posky_thread(yaw_s *restrict yaw)
{
	struct miiskira_posky_s *restrict r;
	uint32_t status;
	r = (struct miiskira_posky_s *) yaw->data;
	while (yaw->running)
	{
		status = yaw_signal_get(r->signal);
		posky_clear_gift(r->posky);
		posky_adorable_clear_gift(r->adorable);
		yaw_signal_wait(r->signal, status);
	}
}

static posky_s* inner_miiskira_posky_alloc_create_posky(uintptr_t adorable_slot_size, uintptr_t posky_size, yaw_signal_s *signal)
{
	queue_s *restrict q;
	posky_s *restrict p;
	p = NULL;
	if ((q = &miiskira_queue_alloc(posky_size, signal)->queue))
	{
		p = posky_alloc(q, adorable_slot_size);
		refer_free(q);
	}
	return p;
}

static posky_adorable_s* inner_miiskira_posky_alloc_create_adorable(posky_s *restrict posky, const char *restrict name, refer_t lady, uintptr_t adorable_posky_size, yaw_signal_s *signal)
{
	queue_s *restrict q;
	posky_adorable_s *restrict a;
	a = NULL;
	if ((q = &miiskira_queue_alloc(adorable_posky_size, signal)->queue))
	{
		a = posky_create_adorable(posky, name, q, lady);
		refer_free(q);
	}
	return a;
}

static struct miiskira_posky_s* inner_miiskira_posky_initial_adorable(struct miiskira_posky_s *restrict r);

struct miiskira_posky_s* inner_miiskira_posky_alloc(uintptr_t adorable_slot_size, uintptr_t posky_size, uintptr_t adorable_posky_size)
{
	struct miiskira_posky_s *restrict r;
	if ((r = (struct miiskira_posky_s *) refer_alloz(sizeof(struct miiskira_posky_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_posky_free_func);
		if (!(r->signal = yaw_signal_alloc()))
			goto label_fail;
		if (!(r->posky = inner_miiskira_posky_alloc_create_posky(adorable_slot_size, posky_size, r->signal)))
			goto label_fail;
		if (!(r->lady = (struct miiskira_posky_lady_s *) refer_alloc(sizeof(struct miiskira_posky_lady_s))))
			goto label_fail;
		r->lady->p = r;
		if (!(r->adorable = inner_miiskira_posky_alloc_create_adorable(r->posky, miiskira$posky$posky, r->lady, adorable_posky_size, r->signal)))
			goto label_fail;
		r->address = posky_adorable_address(r->adorable);
		if (!inner_miiskira_posky_initial_adorable(r))
			goto label_fail;
		if (!(r->pool = vattr_alloc()))
			goto label_fail;
		if (!(r->yaw = yaw_alloc_and_start(inner_miiskira_posky_thread, NULL, r)))
			goto label_fail;
		return r;
		label_fail:
		refer_free(r);
	}
	return NULL;
}

#include "posky.gift.h"

static struct miiskira_posky_s* inner_miiskira_posky_initial_adorable(struct miiskira_posky_s *restrict r)
{
	#define d_set_way(_id)  posky_adorable_set_type_way(r->adorable, miiskira$type$##_id, (posky_feeding_f) inner_miiskira_posky_gift__##_id)
	if (d_set_way(posky_proxy_adorable) &&
		d_set_way(posky_clear_lonely) &&
		d_set_way(posky_trick) &&
		d_set_way(posky_foster_care))
		return r;
	return NULL;
}
