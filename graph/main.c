#include "graph.pri.h"
#include "../posky/posky.h"

struct miiskira_graph_s *graph;

// type
uint32_t miiskira$type$graph_present_create;
uint32_t miiskira$type$graph_present_destroy;
uint32_t miiskira$type$graph_present_resize;
uint32_t miiskira$type$graph_present_do;

static void miiskira_graph_initial_type(void)
{
	miiskira$type$graph_present_create = miiskira_posky_gen_type();
	miiskira$type$graph_present_destroy = miiskira_posky_gen_type();
	miiskira$type$graph_present_resize = miiskira_posky_gen_type();
	miiskira$type$graph_present_do = miiskira_posky_gen_type();
}

static uint32_t miiskira_graph_initial_get_debug_level(uintptr_t argc, const char *const argv[])
{
	char c;
	if (argc >= 2 && (c = *argv[1]) >= '0' && c <= '5')
		return (uint32_t) (c - '0');
	return 0;
}

const char* miiskira_graph_initial(uintptr_t argc, const char *const argv[])
{
	extern mlog_s *$mlog;
	uint32_t debug_level;
	debug_level = miiskira_graph_initial_get_debug_level(argc, argv);
	miiskira_graph_initial_type();
	if ((graph = inner_miiskira_graph_alloc($mlog, debug_level)))
	{
		if (inner_miiskira_graph_register_posky(graph, 1024))
			return NULL;
		refer_free(graph);
	}
	return "miiskira.graph.initial";
}

void miiskira_graph_finally(void)
{
	refer_free(graph);
}
