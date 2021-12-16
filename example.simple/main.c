#include <xwindow.h>
#include <yaw.h>
#include "../posky/posky.h"
#include "../graph/graph.h"
#include "../define.h"

typedef struct inst_s inst_s;

struct inst_s {
	xwindow_s *xw;
	refer_string_t name;
	yaw_s *yaw;
	uintptr_t is_fullscreen;
	uint64_t graph_address;
};

static inst_s *__inst;

static void inst_free_func(inst_s *restrict r)
{
	if (r->yaw)
	{
		yaw_stop_and_wait(r->yaw);
		refer_free(r->yaw);
	}
	if (r->name) refer_free(r->name);
}

static inst_s* inst_alloc(xwindow_s *restrict xw)
{
	inst_s *restrict r;
	if ((r = (inst_s *) refer_alloz(sizeof(inst_s))))
	{
		refer_set_free(r, (refer_free_f) inst_free_func);
		r->xw = xw;
		r->name = refer_dump_string("example");
		if (r->name)
			return r;
		refer_free(r);
	}
	return NULL;
}

static void inst_present_func(xwindow_s *restrict xw, const uint32_t *restrict pixel_bgra, uint32_t width, uint32_t height)
{
	xwindow_update(xw, pixel_bgra, width, height, 0, 0);
}

inst_s* get_inst(xwindow_s *restrict xw)
{
	if (!__inst) __inst = inst_alloc(xw);
	return (inst_s *) refer_save(__inst);
}

void event_close(inst_s *restrict inst, xwindow_s *w)
{
	miiskira_graph_present_destroy(inst->graph_address, inst->name);
	xwindow_unmap(w);
}

void event_expose(inst_s *restrict inst, xwindow_s *w, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	miiskira_graph_present_do(inst->graph_address, inst->name, 0);
}

void event_key(inst_s *restrict inst, xwindow_s *w, xwindow_key_t key, uint32_t press, xwindow_event_state_t *restrict state)
{
	if (key == xwindow_key_f11 && !press)
	{
		inst->is_fullscreen = !inst->is_fullscreen;
		xwindow_set_fullscreen(w, !!inst->is_fullscreen);
	}
}

void event_button(inst_s *restrict inst, xwindow_s *w, xwindow_button_t button, uint32_t press, xwindow_event_state_t *restrict state)
{
	;
}

void event_pointer(inst_s *restrict inst, xwindow_s *w, xwindow_event_state_t *restrict state)
{
	;
}

void event_config(inst_s *restrict inst, xwindow_s *w, int32_t x, int32_t y, uint32_t width, uint32_t height)
{
	miiskira_graph_present_resize(inst->graph_address, inst->name, width, height);
}

static void thread(yaw_s *restrict yaw)
{
	inst_s *restrict inst;
	inst = (inst_s *) yaw->data;
	while (yaw->running)
	{
		miiskira_graph_present_do(inst->graph_address, inst->name, 0);
		yaw_msleep(15);
	}
}

static posky_adorable_s* initial_adorable(posky_adorable_s *restrict adorable, posky_s *restrict posky, inst_s *restrict inst)
{
	uint32_t width, height;
	inst->graph_address = posky_address_adorable(posky, miiskira$posky$graph);
	if (inst->graph_address && xwindow_get_geometry(inst->xw, &width, &height, NULL, NULL, NULL))
	{
		miiskira_graph_present_create_bgra(inst->graph_address, inst->name, (miiskira_graph_present_f) inst_present_func, inst->xw);
		miiskira_graph_present_resize(inst->graph_address, inst->name, width, height);
		miiskira_graph_present_do(inst->graph_address, inst->name, 0);
		if ((inst->yaw = yaw_alloc_and_start(thread, NULL, inst)))
			return adorable;
	}
	return NULL;
}

static inst_s* initial_create_adorable(inst_s *restrict inst)
{
	struct inst_s *r;
	struct miiskira_candy__posky_task_s *restrict candy;
	r = NULL;
	if ((candy = miiskira_candy_alloc__posky_task(1)))
	{
		if (miiskira_candy_append__posky_task(candy, miiskira_posky_task_id__create_adorable,
			NULL, 0, NULL, 0, 1024, inst,
			(miiskira_posky_initial_adorable_f) initial_adorable))
		{
			miiskira_posky_feeding(&candy->candy);
			r = inst;
		}
		refer_free(candy);
	}
	return r;
}

const char* initial(uintptr_t argc, const char *const argv[])
{
	if (__inst)
	{
		if (initial_create_adorable(__inst))
			return NULL;
		refer_free(__inst);
	}
	return "miiskira.example.simple.initial";
}

void finally(void)
{
	refer_free(__inst);
}
