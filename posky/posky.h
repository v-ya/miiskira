#ifndef _miiskira_posky_h_
#define _miiskira_posky_h_

#include <posky.h>
#include <yaw.h>

enum miiskira_posky_task_id_t {
	miiskira_posky_task_id__create_adorable,
	miiskira_posky_task_id__create_group,
	miiskira_posky_task_id__queue_adorable,
	miiskira_posky_task_id__queue_group,
	miiskira_posky_task_id__delete_adorable,
	miiskira_posky_task_id__delete_group,
	miiskira_posky_task_id__join_group,
	miiskira_posky_task_id__leave_group,
};

enum miiskira_posky_task_status_t {
	miiskira_posky_task_status__okay,
	miiskira_posky_task_status__fail,
};

typedef posky_adorable_s* (*miiskira_posky_initial_adorable_f)(posky_adorable_s *restrict adorable, posky_s *restrict posky, refer_t lady);

struct miiskira_posky_task_s {
	enum miiskira_posky_task_id_t id;
	const char *adorable_name;
	uint64_t adorable_address;
	const char *group_name;
	uint64_t group_address;
	uintptr_t queue_size;
	refer_t lady;
	miiskira_posky_initial_adorable_f initial;
};

struct miiskira_posky_return_t {
	enum miiskira_posky_task_status_t status;
	const struct miiskira_posky_task_s *task;
	posky_adorable_s *adorable;
	uint64_t address;
	yaw_signal_s *signal;
};

struct miiskira_candy__posky_task_s {
	posky_candy_s candy;
	uintptr_t size;
	uintptr_t number;
	struct miiskira_posky_task_s *task[];
};

struct miiskira_candy__posky_return_s {
	posky_candy_s candy;
	uintptr_t size;
	uintptr_t number;
	struct miiskira_posky_return_t ret[];
};

extern uint32_t miiskira$type$posky_task;
extern uint32_t miiskira$type$posky_return;
extern uint32_t miiskira$type$posky_clear_lonely;

uint32_t miiskira_posky_gen_type(void);

void miiskira_posky_feeding(posky_candy_s *restrict candy);

struct miiskira_candy__posky_task_s* miiskira_candy_alloc__posky_task(uintptr_t size);
struct miiskira_candy__posky_return_s* miiskira_candy_alloc__posky_return(uintptr_t size);
struct miiskira_candy__posky_task_s* miiskira_candy_append__posky_task(struct miiskira_candy__posky_task_s *restrict candy, enum miiskira_posky_task_id_t id, const char *adorable_name, uint64_t adorable_address, const char *group_name, uint64_t group_address, uintptr_t queue_size, refer_t lady, miiskira_posky_initial_adorable_f initial);
struct miiskira_candy__posky_return_s* miiskira_candy_append__posky_return(struct miiskira_candy__posky_return_s *restrict candy, enum miiskira_posky_task_status_t status, const struct miiskira_posky_task_s *task, posky_adorable_s *adorable, uint64_t address, yaw_signal_s *signal);

#endif
