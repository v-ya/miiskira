#include "posky.gift.h"

static void inner_miiskira_posky_adorable_free_func(struct miiskira_posky_adorable_s *restrict r)
{
	if (r->yaw)
	{
		yaw_stop(r->yaw);
		yaw_signal_inc(r->signal);
		yaw_signal_wake(r->signal, ~0);
		yaw_wait(r->yaw);
		refer_free(r->yaw);
	}
	if (r->signal)
		refer_free(r->signal);
	if (r->adorable)
		refer_free(r->adorable);
}

static void inner_miiskira_posky_adorable_thread(yaw_s *restrict yaw)
{
	struct miiskira_posky_adorable_s *restrict r;
	uint32_t status;
	r = (struct miiskira_posky_adorable_s *) yaw->data;
	while (yaw->running)
	{
		status = yaw_signal_get(r->signal);
		posky_adorable_clear_gift(r->adorable);
		yaw_signal_wait(r->signal, status);
	}
}

static struct miiskira_posky_adorable_s* inner_miiskira_posky_adorable_alloc(posky_adorable_s *adorable, yaw_signal_s *signal)
{
	struct miiskira_posky_adorable_s *restrict r;
	if ((r = (struct miiskira_posky_adorable_s *) refer_alloz(sizeof(struct miiskira_posky_adorable_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_posky_adorable_free_func);
		r->adorable = (posky_adorable_s *) refer_save(adorable);
		r->signal = (yaw_signal_s *) refer_save(signal);
		r->address = posky_adorable_address(adorable);
		if ((r->yaw = yaw_alloc_and_start(inner_miiskira_posky_adorable_thread, NULL, r)))
			return r;
		refer_free(r);
	}
	return NULL;
}

const posky_candy_s* inner_miiskira_posky_gift__posky_return(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_return_s *restrict ret)
{
	struct miiskira_posky_s *restrict pri;
	const struct miiskira_posky_return_t *restrict r;
	const struct miiskira_posky_task_s *restrict t;
	struct miiskira_posky_adorable_s *restrict v;
	uintptr_t i;
	pri = lady->p;
	for (i = 0; i < ret->number; ++i)
	{
		r = ret->ret + i;
		if (r->status == miiskira_posky_task_status__okay && (t = r->task))
		{
			if (t->id == miiskira_posky_task_id__create_adorable)
			{
				if (t->adorable_name && r->adorable && r->signal)
				{
					if ((v = inner_miiskira_posky_adorable_alloc(r->adorable, r->signal)))
					{
						vattr_set(pri->pool, t->adorable_name, v);
						refer_free(v);
					}
				}
			}
			else if (t->id == miiskira_posky_task_id__delete_adorable)
			{
				if (t->adorable_name)
					vattr_delete(pri->pool, t->adorable_name);
				else
				{
					vattr_vlist_t *restrict vl, *restrict next;
					next = pri->pool->vattr;
					while ((vl = next))
					{
						if ((v = (struct miiskira_posky_adorable_s *) vl->value) && v->address == t->adorable_address)
							vattr_delete_vlist(vl);
						next = vl->vattr_next;
					}
				}
			}
		}
	}
	return &ret->candy;
}
