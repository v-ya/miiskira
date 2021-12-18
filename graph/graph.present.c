#include "graph.posky.h"

static void inner_miiskira_graph_present_free_func(struct miiskira_graph_present_s *restrict r)
{
	if (r->pri) refer_free(r->pri);
}

static inline struct miiskira_graph_present_s* inner_miiskira_graph_present_find(struct miiskira_graph_s *restrict lady, const char *restrict name)
{
	return (struct miiskira_graph_present_s *) hashmap_get_name(&lady->present, name);
}

const posky_candy_s* inner_miiskira_graph__present_create(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_create_s *restrict candy)
{
	struct miiskira_graph_present_s *restrict r;
	if (candy->name && candy->present)
	{
		if (!hashmap_find_name(&lady->present, candy->name) &&
			(r = (struct miiskira_graph_present_s *) refer_alloz(sizeof(struct miiskira_graph_present_s))))
		{
			refer_set_free(r, (refer_free_f) inner_miiskira_graph_present_free_func);
			r->present = candy->present;
			r->pri = refer_save(candy->pri);
			r->format = candy->format;
			if (hashmap_set_name(&lady->present, candy->name, r, inner_miiskira_graph_hashmap_free_func))
			{
				log_info("[graph] create present (%s)", candy->name);
				return &candy->candy;
			}
			refer_free(r);
		}
	}
	log_warning("[graph] create present (%s) fail", candy->name?candy->name:"");
	return NULL;
}

const posky_candy_s* inner_miiskira_graph__present_destroy(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_destroy_s *restrict candy)
{
	if (candy->name)
	{
		log_info("[graph] destroy present (%s)", candy->name);
		hashmap_delete_name(&lady->present, candy->name);
		return &candy->candy;
	}
	return NULL;
}

const posky_candy_s* inner_miiskira_graph__present_resize(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_resize_s *restrict candy)
{
	struct miiskira_graph_present_s *restrict r;
	if (candy->name && (r = inner_miiskira_graph_present_find(lady, candy->name)))
	{
		log_verbose("[graph] resize present (%s) %ux%u => %ux%u", candy->name, r->width, r->height, candy->width, candy->height);
		r->width = candy->width;
		r->height = candy->height;
		return &candy->candy;
	}
	return NULL;
}

const posky_candy_s* inner_miiskira_graph__present_do(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_do_s *restrict candy)
{
	struct miiskira_graph_present_s *restrict r;
	if (candy->name && (r = inner_miiskira_graph_present_find(lady, candy->name)))
	{
		/// TODO: call render
		/// TODO: send report
		return &candy->candy;
	}
	log_warning("[graph] do present (%s) fail", candy->name?candy->name:"");
	return NULL;
}
