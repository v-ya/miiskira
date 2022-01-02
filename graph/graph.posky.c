#include "graph.posky.h"
#include "../posky/posky.h"
#include "../queue/queue.h"
#include "../define.h"

static posky_adorable_s* inner_miiskira_graph_posky_initial_adorable(posky_adorable_s *restrict adorable)
{
	if (
		#define d_type_way(_id)  posky_adorable_set_type_way(adorable, miiskira$type$graph_##_id, (posky_feeding_f) inner_miiskira_graph__##_id)
		d_type_way(present_create) &&
		d_type_way(present_destroy) &&
		d_type_way(present_resize) &&
		d_type_way(present_do) &&
		d_type_way(tag_render) &&
		d_type_way(tag_model)
		#undef d_type_way
	) return adorable;
	return NULL;
}

static void inner_miiskira_graph_posky_trick(posky_s *restrict posky, refer_t lady)
{
	extern uintptr_t graph_posky_adorable_size;
	posky_adorable_s *restrict adorable;
	miiskira_queue_s *restrict queue;
	posky_adorable_s *trick_okay;
	trick_okay = NULL;
	if ((queue = miiskira_queue_alloc(graph_posky_adorable_size, NULL)))
	{
		if ((adorable = posky_create_adorable(posky, miiskira$posky$graph, &queue->queue, lady)))
		{
			if ((trick_okay = inner_miiskira_graph_posky_initial_adorable(adorable)))
				miiskira_posky_feeding_foster_care(adorable, queue->signal, NULL, NULL, posky_adorable_address(adorable));
			refer_free(adorable);
		}
		refer_free(queue);
	}
	if (!trick_okay)
		log_warning("[graph] posky trick register self fail");
}

void inner_miiskira_graph_register_posky(struct miiskira_graph_s *restrict graph)
{
	miiskira_posky_feeding_trick(inner_miiskira_graph_posky_trick, graph, 0);
}
