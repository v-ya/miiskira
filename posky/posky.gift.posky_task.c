#include "posky.gift.h"

const posky_candy_s* inner_miiskira_posky_gift__posky_task(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_task_s *restrict task)
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
