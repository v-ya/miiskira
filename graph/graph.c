#include "graph.posky.h"
#include "../posky/posky.h"

static inline void inner_graph_candy_initial(posky_candy_s *restrict candy, uint64_t target, uint64_t report, uint32_t type)
{
	candy->target = target;
	candy->report = report;
	candy->type = type;
	candy->level = 0;
	candy->stamp = miiskira$posky$stamp;
}

// graph_present_create

static void inner_graph_present_create_free_func(struct miiskira_candy__graph_present_create_s *restrict r)
{
	if (r->name) refer_free(r->name);
	if (r->pri) refer_free(r->pri);
}

static void inner_miiskira_graph_present_create(uint64_t address, refer_string_t name, miiskira_graph_present_f present_func, refer_t pri, graph_format_t format)
{
	struct miiskira_candy__graph_present_create_s *restrict r;
	r = (struct miiskira_candy__graph_present_create_s *) refer_alloc(sizeof(struct miiskira_candy__graph_present_create_s));
	if (r)
	{
		inner_graph_candy_initial(&r->candy, address, 0, miiskira$type$graph_present_create);
		r->name = (refer_string_t) refer_save(name);
		r->present = present_func;
		r->pri = refer_save(pri);
		r->format = format;
		refer_set_free(r, (refer_free_f) inner_graph_present_create_free_func);
		miiskira_posky_feeding(&r->candy);
		refer_free(r);
	}
}

void miiskira_graph_present_create_bgra(uint64_t address, refer_string_t name, miiskira_graph_present_f present_func, refer_t pri)
{
	inner_miiskira_graph_present_create(address, name, present_func, pri, graph_format_b8g8r8a8_unorm);
}

// graph_present_destroy

static void inner_graph_present_destroy_free_func(struct miiskira_candy__graph_present_destroy_s *restrict r)
{
	if (r->name) refer_free(r->name);
}

void miiskira_graph_present_destroy(uint64_t address, refer_string_t name)
{
	struct miiskira_candy__graph_present_destroy_s *restrict r;
	r = (struct miiskira_candy__graph_present_destroy_s *) refer_alloc(sizeof(struct miiskira_candy__graph_present_destroy_s));
	if (r)
	{
		inner_graph_candy_initial(&r->candy, address, 0, miiskira$type$graph_present_destroy);
		r->name = (refer_string_t) refer_save(name);
		refer_set_free(r, (refer_free_f) inner_graph_present_destroy_free_func);
		miiskira_posky_feeding(&r->candy);
		refer_free(r);
	}
}

// graph_present_resize

static void inner_graph_present_resize_free_func(struct miiskira_candy__graph_present_resize_s *restrict r)
{
	if (r->name) refer_free(r->name);
}

void miiskira_graph_present_resize(uint64_t address, refer_string_t name, uint32_t width, uint32_t height)
{
	struct miiskira_candy__graph_present_resize_s *restrict r;
	r = (struct miiskira_candy__graph_present_resize_s *) refer_alloc(sizeof(struct miiskira_candy__graph_present_resize_s));
	if (r)
	{
		inner_graph_candy_initial(&r->candy, address, 0, miiskira$type$graph_present_resize);
		r->name = (refer_string_t) refer_save(name);
		r->width = width;
		r->height = height;
		refer_set_free(r, (refer_free_f) inner_graph_present_resize_free_func);
		miiskira_posky_feeding(&r->candy);
		refer_free(r);
	}
}

// graph_present_do

static void inner_graph_present_do_free_func(struct miiskira_candy__graph_present_do_s *restrict r)
{
	if (r->name) refer_free(r->name);
}

void miiskira_graph_present_do(uint64_t address, refer_string_t name, uint64_t report_address)
{
	struct miiskira_candy__graph_present_do_s *restrict r;
	r = (struct miiskira_candy__graph_present_do_s *) refer_alloc(sizeof(struct miiskira_candy__graph_present_do_s));
	if (r)
	{
		inner_graph_candy_initial(&r->candy, address, report_address, miiskira$type$graph_present_do);
		r->name = (refer_string_t) refer_save(name);
		refer_set_free(r, (refer_free_f) inner_graph_present_do_free_func);
		miiskira_posky_feeding(&r->candy);
		refer_free(r);
	}
}
