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

static posky_s* inner_miiskira_posky_alloc_create_posky(uintptr_t adorable_slot_size, uintptr_t posky_size, yaw_signal_s **restrict signal)
{
	queue_s *restrict q;
	posky_s *restrict p;
	p = NULL;
	if ((q = inner_miiskira_posky_queue_alloc(posky_size, signal)))
	{
		p = posky_alloc(q, adorable_slot_size);
		refer_free(q);
	}
	return p;
}

static posky_adorable_s* inner_miiskira_posky_alloc_create_adorable(posky_s *restrict posky, const char *restrict name, refer_t lady, uintptr_t adorable_posky_size, yaw_signal_s **restrict signal)
{
	queue_s *restrict q;
	posky_adorable_s *restrict a;
	a = NULL;
	if ((q = inner_miiskira_posky_queue_alloc(adorable_posky_size, signal)))
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
		if (!(r->posky = inner_miiskira_posky_alloc_create_posky(adorable_slot_size, posky_size, &r->signal)))
			goto label_fail;
		if (!(r->lady = (struct miiskira_posky_lady_s *) refer_alloc(sizeof(struct miiskira_posky_lady_s))))
			goto label_fail;
		r->lady->p = r;
		if (!(r->adorable = inner_miiskira_posky_alloc_create_adorable(r->posky, miiskira$posky$posky, r->lady, adorable_posky_size, &r->signal)))
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

static const posky_candy_s* inner_miiskira_posky_type__posky_create(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_task_s *restrict task)
{
	posky_s *restrict posky;
	struct miiskira_candy__posky_return_s *restrict r;
	const struct miiskira_posky_task_s *restrict t;
	posky_adorable_s *restrict adorable;
	uint64_t address;
	yaw_signal_s *signal;
	uintptr_t i;
	posky = lady->p->posky;
	r = NULL;
	adorable = NULL;
	if (task->candy.report)
		r = miiskira_candy_alloc__posky_return(task->number);
	for (i = 0; i < task->number; ++i)
	{
		switch ((t = task->task[i])->id)
		{
			case miiskira_posky_task_id__create_adorable:
				signal = NULL;
				if (!t->adorable_name)
					adorable = posky_create_adorable_ring(posky, t->adorable_name, t->lady, t->queue_size);
				else
				{
					adorable = inner_miiskira_posky_alloc_create_adorable(posky, t->adorable_name, t->lady, t->queue_size, &signal);
					if (signal) refer_free(signal);
				}
				if (adorable)
				{
					refer_free(adorable);
					address = posky_adorable_address(adorable);
					if (!t->initial || t->initial(adorable, posky, t->lady))
					{
						if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, adorable, address, signal);
						break;
					}
					posky_delete_adorable(posky, NULL, address);
					adorable = NULL;
				}
				goto label_fail;
			case miiskira_posky_task_id__create_group:
				if (t->group_name)
				{
					if ((address = posky_create_group(posky, t->group_name)))
					{
						if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, NULL, address, NULL);
						break;
					}
				}
				goto label_fail;
			case miiskira_posky_task_id__queue_adorable:
				if (t->adorable_name || t->adorable_address)
				{
					if ((adorable = posky_find_adorable(posky, t->adorable_name, t->adorable_address)))
					{
						address = posky_adorable_address(adorable);
						if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, adorable, address, NULL);
						break;
					}
				}
				goto label_fail;
			case miiskira_posky_task_id__queue_group:
				if (t->group_name)
				{
					if ((address = posky_address_group(posky, t->group_name)))
					{
						if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, NULL, address, NULL);
						break;
					}
				}
				goto label_fail;
			case miiskira_posky_task_id__delete_adorable:
				if (t->adorable_name || t->adorable_address)
				{
					posky_delete_adorable(posky, t->adorable_name, t->adorable_address);
					if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, NULL, 0, NULL);
					break;
				}
				goto label_fail;
			case miiskira_posky_task_id__delete_group:
				if (t->group_name || t->group_address)
				{
					posky_delete_group(posky, t->group_name, t->group_address);
					if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, NULL, 0, NULL);
					break;
				}
				goto label_fail;
			case miiskira_posky_task_id__join_group:
				address = t->adorable_address;
				if ((t->adorable_name || address || adorable) && (t->group_name || t->group_address))
				{
					if (!t->adorable_name && !address)
						address = posky_adorable_address(adorable);
					if (posky_join_group(posky, t->group_name, t->group_address, t->adorable_name, address))
					{
						if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, NULL, 0, NULL);
						break;
					}
				}
				goto label_fail;
			case miiskira_posky_task_id__leave_group:
				address = t->adorable_address;
				if ((t->adorable_name || address || adorable) && (t->group_name || t->group_address))
				{
					if (!t->adorable_name && !address)
						address = posky_adorable_address(adorable);
					posky_leave_group(posky, t->group_name, t->group_address, t->adorable_name, address);
					if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__okay, t, NULL, 0, NULL);
					break;
				}
				goto label_fail;
			default:
				label_fail:
				if (r) miiskira_candy_append__posky_return(r, miiskira_posky_task_status__fail, t, NULL, 0, NULL);
				break;
		}
	}
	if (r)
	{
		r->candy.target = task->candy.report;
		r->candy.stamp = task->candy.stamp;
		posky_feeding(posky, &r->candy);
		refer_free(r);
	}
	return &task->candy;
}

static const posky_candy_s* inner_miiskira_posky_type__posky_return(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_return_s *restrict ret)
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

static const posky_candy_s* inner_miiskira_posky_type__posky_clear_lonely(struct miiskira_posky_lady_s *restrict lady, const posky_candy_s *restrict candy)
{
	posky_clear_lonely_adorable(lady->p->posky);
	return candy;
}

static struct miiskira_posky_s* inner_miiskira_posky_initial_adorable(struct miiskira_posky_s *restrict r)
{
	if (posky_adorable_set_type_way(r->adorable, miiskira$type$posky_task, (posky_feeding_f) inner_miiskira_posky_type__posky_create) &&
		posky_adorable_set_type_way(r->adorable, miiskira$type$posky_return, (posky_feeding_f) inner_miiskira_posky_type__posky_return) &&
		posky_adorable_set_type_way(r->adorable, miiskira$type$posky_clear_lonely, (posky_feeding_f) inner_miiskira_posky_type__posky_clear_lonely))
		return r;
	return NULL;
}
