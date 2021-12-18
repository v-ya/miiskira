#include "graph.posky.h"
#include <pocket/pocket.h>

// layout

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_layout(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	return NULL;
}

// initial

hashmap_t* inner_miiskira_graph_initial_render_parser(hashmap_t *restrict parser)
{
	if (hashmap_set_name(parser, "layout", inner_miiskira_graph_parse_render_layout, NULL))
		return parser;
	return NULL;
}

// posky candy

const posky_candy_s* inner_miiskira_graph__tag_render(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_pocket_s *restrict candy)
{
	if (candy->pocket && inner_miiskira_graph_parse_pocket(lady, &lady->parser->render, candy->pocket))
		return &candy->candy;
	return NULL;
}
