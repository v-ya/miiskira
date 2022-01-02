#include "graph.pri.h"
#include "graph.tag.h"
#include "../posky/posky.h"
#include <stdlib.h>

struct miiskira_graph_s *graph;
uintptr_t graph_posky_adorable_size;

// type
uint32_t miiskira$type$graph_present_create;
uint32_t miiskira$type$graph_present_destroy;
uint32_t miiskira$type$graph_present_resize;
uint32_t miiskira$type$graph_present_do;
// inner tag parse
uint32_t miiskira$type$graph_tag_render;
uint32_t miiskira$type$graph_tag_model;

static void miiskira_graph_initial_type(void)
{
	miiskira$type$graph_present_create = miiskira_posky_gen_type();
	miiskira$type$graph_present_destroy = miiskira_posky_gen_type();
	miiskira$type$graph_present_resize = miiskira_posky_gen_type();
	miiskira$type$graph_present_do = miiskira_posky_gen_type();
	miiskira$type$graph_tag_render = miiskira_posky_gen_type();
	miiskira$type$graph_tag_model = miiskira_posky_gen_type();
}

static uint32_t miiskira_graph_initial_get_debug_level(uintptr_t argc, const char *const argv[])
{
	if (argc >= 2)
		return (uint32_t) strtoul(argv[1], NULL, 10);
	return 0;
}

const char* miiskira_graph_initial(uintptr_t argc, const char *const argv[])
{
	uint32_t debug_level;
	debug_level = miiskira_graph_initial_get_debug_level(argc, argv);
	graph_posky_adorable_size = 1024;
	miiskira_graph_initial_type();
	if ((graph = inner_miiskira_graph_alloc(miiskira$log$verbose, debug_level)))
	{
		inner_miiskira_graph_register_posky(graph);
		if (miiskira_graph_initial_tag())
			return NULL;
		refer_free(graph);
	}
	return "miiskira.graph.initial";
}

void miiskira_graph_finally(void)
{
	miiskira_graph_finally_tag();
	refer_free(graph);
}
