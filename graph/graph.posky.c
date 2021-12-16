#include "graph.posky.h"
#include "../posky/posky.h"
#include "../define.h"

static posky_adorable_s* inner_miiskira_graph_posky_initial_adorable(posky_adorable_s *restrict adorable, posky_s *restrict posky, struct miiskira_graph_s *restrict lady)
{
	if (
		#define d_type_way(_id)  posky_adorable_set_type_way(adorable, miiskira$type$graph_##_id, (posky_feeding_f) inner_miiskira_graph__##_id)
		d_type_way(present_create) &&
		d_type_way(present_destroy) &&
		d_type_way(present_resize) &&
		d_type_way(present_do)
		#undef d_type_way
	) return adorable;
	return NULL;
}

struct miiskira_graph_s* inner_miiskira_graph_register_posky(struct miiskira_graph_s *restrict graph, uintptr_t queue_size)
{
	struct miiskira_graph_s *r;
	struct miiskira_candy__posky_task_s *restrict candy;
	r = NULL;
	if ((candy = miiskira_candy_alloc__posky_task(1)))
	{
		if (miiskira_candy_append__posky_task(candy, miiskira_posky_task_id__create_adorable,
			miiskira$posky$graph, 0, NULL, 0, queue_size, graph,
			(miiskira_posky_initial_adorable_f) inner_miiskira_graph_posky_initial_adorable))
		{
			candy->candy.report = candy->candy.target;
			miiskira_posky_feeding(&candy->candy);
			r = graph;
		}
		refer_free(candy);
	}
	return r;
}
