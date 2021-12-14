#include "posky.pri.h"

void miiskira_posky_feeding(posky_candy_s *restrict candy)
{
	posky_feeding(p->posky, candy);
}

static void inner_miiskira_candy__posky_task_free_func(struct miiskira_candy__posky_task_s *restrict r)
{
	uintptr_t i;
	for (i = 0; i < r->number; ++i)
	{
		if (r->task[i])
			refer_free(r->task[i]);
	}
}

static void inner_miiskira_candy__posky_return_free_func(struct miiskira_candy__posky_return_s *restrict r)
{
	uintptr_t i;
	for (i = 0; i < r->number; ++i)
	{
		if (r->ret[i].task)
			refer_free(r->ret[i].task);
		if (r->ret[i].adorable)
			refer_free(r->ret[i].adorable);
	}
}

struct miiskira_candy__posky_task_s* miiskira_candy_alloc__posky_task(uintptr_t size)
{
	struct miiskira_candy__posky_task_s *restrict r;
	if ((r = (struct miiskira_candy__posky_task_s *) refer_alloz(sizeof(struct miiskira_candy__posky_task_s) + sizeof(r->task[0]) * size)))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_candy__posky_task_free_func);
		r->candy.target = p->address;
		r->candy.type = miiskira$type$posky_task;
		r->size = size;
	}
	return r;
}

struct miiskira_candy__posky_return_s* miiskira_candy_alloc__posky_return(uintptr_t size)
{
	struct miiskira_candy__posky_return_s *restrict r;
	if ((r = (struct miiskira_candy__posky_return_s *) refer_alloz(sizeof(struct miiskira_candy__posky_return_s) + sizeof(r->ret[0]) * size)))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_candy__posky_return_free_func);
		r->candy.report = p->address;
		r->candy.type = miiskira$type$posky_return;
		r->size = size;
	}
	return r;
}

static void inner_miiskira_posky_task_free_func(struct miiskira_posky_task_s *restrict r)
{
	if (r->lady)
		refer_free(r->lady);
}

struct miiskira_candy__posky_task_s* miiskira_candy_append__posky_task(struct miiskira_candy__posky_task_s *restrict candy, enum miiskira_posky_task_id_t id, const char *adorable_name, uint64_t adorable_address, const char *group_name, uint64_t group_address, uintptr_t queue_size, refer_t lady, miiskira_posky_initial_adorable_f initial)
{
	if (candy->number < candy->size)
	{
		struct miiskira_posky_task_s *restrict task;
		if ((task = (struct miiskira_posky_task_s *) refer_alloc(sizeof(struct miiskira_posky_task_s))))
		{
			task->id = id;
			task->adorable_name = adorable_name;
			task->adorable_address = adorable_address;
			task->group_name = group_name;
			task->group_address = group_address;
			task->queue_size = queue_size;
			task->lady = refer_save(lady);
			task->initial = initial;
			refer_set_free(task, (refer_free_f) inner_miiskira_posky_task_free_func);
			candy->task[candy->number++] = task;
			return candy;
		}
	}
	return NULL;
}

struct miiskira_candy__posky_return_s* miiskira_candy_append__posky_return(struct miiskira_candy__posky_return_s *restrict candy, enum miiskira_posky_task_status_t status, const struct miiskira_posky_task_s *task, posky_adorable_s *adorable, uint64_t address, yaw_signal_s *signal)
{
	if (candy->number < candy->size)
	{
		struct miiskira_posky_return_t *restrict ret;
		ret = candy->ret + candy->number++;
		ret->status = status;
		ret->task = (const struct miiskira_posky_task_s *) refer_save(task);
		ret->adorable = (posky_adorable_s *) refer_save(adorable);
		ret->address = address;
		ret->signal = (yaw_signal_s *) refer_save(signal);
		return candy;
	}
	return NULL;
}
