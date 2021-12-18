#ifndef _miiskira_graph_posky_h_
#define _miiskira_graph_posky_h_

#include "graph.pri.h"

struct miiskira_candy__graph_present_create_s {
	posky_candy_s candy;
	refer_string_t name;
	miiskira_graph_present_f present;
	refer_t pri;
	graph_format_t format;
};

struct miiskira_candy__graph_present_destroy_s {
	posky_candy_s candy;
	refer_string_t name;
};

struct miiskira_candy__graph_present_resize_s {
	posky_candy_s candy;
	refer_string_t name;
	uint32_t width;
	uint32_t height;
};

struct miiskira_candy__graph_present_do_s {
	posky_candy_s candy;
	refer_string_t name;
};

struct miiskira_candy__graph_pocket_s {
	posky_candy_s candy;
	struct pocket_s *pocket;
};

const posky_candy_s* inner_miiskira_graph__present_create(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_create_s *restrict candy);
const posky_candy_s* inner_miiskira_graph__present_destroy(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_destroy_s *restrict candy);
const posky_candy_s* inner_miiskira_graph__present_resize(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_resize_s *restrict candy);
const posky_candy_s* inner_miiskira_graph__present_do(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_present_do_s *restrict candy);
const posky_candy_s* inner_miiskira_graph__tag_render(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_pocket_s *restrict candy);
const posky_candy_s* inner_miiskira_graph__tag_model(struct miiskira_graph_s *restrict lady, const struct miiskira_candy__graph_pocket_s *restrict candy);

#endif
