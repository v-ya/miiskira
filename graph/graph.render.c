#include "graph.posky.h"
#include <pocket/pocket.h>

// layout

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_layout_append(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a, struct miiskira_graph_layout_s *restrict layout)
{
	const struct miiskira_graph_parser_s *restrict parser;
	const char *restrict name;
	uintptr_t n;
	parser = r->parser;
	n = a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	while (n)
	{
		--n;
		if ((name = a->name.string) && *name == '.')
			name = NULL;
		if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		{
			if (name && !(name = fsys_rpath_get_full_path(parser->rpath, name)))
				goto label_fail;
			if (!inner_miiskira_graph_layout_append(layout, &parser->layout_type, name, a->tag.string, a->data.ptr, (uintptr_t) a->size))
				goto label_fail;
			log_verbose("[graph] render.layout: %s %s[], size = %zu", a->tag.string, name, (uintptr_t) a->size);
		}
		else
		{
			if (name)
			{
				if (!fsys_rpath_push(parser->rpath, name))
					goto label_fail;
			}
			r = inner_miiskira_graph_parse_render_layout_append(r, p, a, layout);
			if (name) fsys_rpath_pop(parser->rpath);
			if (!r) goto label_fail;
		}
		++a;
	}
	return r;
	label_fail:
	return NULL;
}

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_layout(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	struct miiskira_graph_layout_s *restrict layout;
	uintptr_t n;
	layout = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	n = a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	while (n)
	{
		--n;
		if (!a->name.string)
			goto label_fail;
		if (hashmap_find_name(&r->layout, a->name.string))
			goto label_fail;
		if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
			goto label_fail;
		if (!(layout = inner_miiskira_graph_layout_alloc()))
			goto label_fail;
		log_verbose("[graph] load render.layout (%s) begin", a->name.string);
		if (!inner_miiskira_graph_parse_render_layout_append(r, p, a, layout))
			goto label_fail;
		log_verbose("[graph] load render.layout (%s) end", a->name.string);
		if (!hashmap_set_name(&r->layout, a->name.string, layout, inner_miiskira_graph_hashmap_free_func))
			goto label_fail;
		log_info("[graph] load render.layout (%s)", a->name.string);
		layout = NULL;
		++a;
	}
	return r;
	label_fail:
	if (!layout) refer_free(layout);
	log_error("[graph] load render.layout (%s) fail", a->name.string?a->name.string:"");
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
	log_error("[graph] load render fail");
	return NULL;
}
