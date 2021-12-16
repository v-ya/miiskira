#include "graph.pri.h"

struct miiskira_graph_s *graph;

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
	if ((graph = inner_miiskira_graph_alloc($mlog, debug_level)))
		return NULL;
	return "miiskira.graph.initial";
}

void miiskira_graph_finally(void)
{
	refer_free(graph);
}
