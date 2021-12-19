#include "graph.posky.h"
#include <pocket/pocket.h>
#include <stdlib.h>

// layout

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_layout_append(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a, struct miiskira_graph_layout_s *restrict layout)
{
	const struct miiskira_graph_parser_s *restrict parser;
	const char *restrict name;
	uint64_t n;
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
	uint64_t n;
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
		layout = NULL;
		log_info("[graph] load render.layout (%s)", a->name.string);
		++a;
	}
	return r;
	label_fail:
	if (layout) refer_free(layout);
	log_error("[graph] load render.layout (%s) fail", a->name.string?a->name.string:"");
	return NULL;
}

// shader

static struct miiskira_graph_layout_s* inner_miiskira_graph_parse_render_shader_get_layout(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	if (!pocket_is_tag(p, a, pocket_tag$string, NULL))
		goto label_fail;
	if (!a->data.ptr)
		goto label_fail;
	return (struct miiskira_graph_layout_s *) hashmap_get_name(&r->layout, (const char *) a->data.ptr);
	label_fail:
	return NULL;
}

static struct miiskira_graph_shader_s* inner_miiskira_graph_parse_render_shader_add_uniform(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a, struct miiskira_graph_shader_s *restrict shader)
{
	const pocket_attr_t *restrict v;
	struct miiskira_graph_layout_s *restrict layout;
	char *binding_str_end;
	uint32_t binding, share_model, share_pipe, share_present;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	if (!a->name.string)
		goto label_fail;
	binding = (uint32_t) strtoul(a->name.string, &binding_str_end, 0);
	if (*binding_str_end)
		goto label_fail;
	if ((v = pocket_find(p, a, "layout")))
	{
		// layout
		if (!(layout = inner_miiskira_graph_parse_render_shader_get_layout(r, p, v)))
			goto label_fail;
		share_model = !!pocket_find(p, a, "share-model");
		share_pipe = !!pocket_find(p, a, "share-pipe");
		share_present = !!pocket_find(p, a, "share-present");
		if (!inner_miiskira_graph_shader_add_uniform_layout(shader, binding, layout, share_model, share_pipe, share_present))
			goto label_fail;
		log_verbose("[graph] render.shader add uniform [%u]{layout:(%s), share-model:%u, share-pipe:%u, share-present:%u}",
			binding, (const char *) v->data.ptr, share_model, share_pipe, share_present);
	}
	else goto label_fail;
	return shader;
	label_fail:
	return NULL;
}

static struct miiskira_graph_shader_s* inner_miiskira_graph_parse_render_shader_create(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	const pocket_attr_t *restrict v;
	struct miiskira_graph_shader_s *restrict shader;
	struct miiskira_graph_layout_s *restrict layout;
	uint64_t n;
	shader = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	if (!(v = pocket_find(p, a, "shader")))
		goto label_fail;
	// shader
	shader = inner_miiskira_graph_shader_alloc(r->device->dev, &r->parser->shader_type, v->tag.string, v->data.ptr, v->size);
	if (!shader)
		goto label_fail;
	log_verbose("[graph] render.shader create (%s)", a->name.string);
	// input
	if ((v = pocket_find(p, a, "input")))
	{
		if (!(layout = inner_miiskira_graph_parse_render_shader_get_layout(r, p, v)))
			goto label_fail;
		inner_miiskira_graph_shader_set_input(shader, layout);
		log_verbose("[graph] render.shader input = (%s)", (const char *) v->data.ptr);
	}
	// output
	if ((v = pocket_find(p, a, "output")))
	{
		if (!(layout = inner_miiskira_graph_parse_render_shader_get_layout(r, p, v)))
			goto label_fail;
		inner_miiskira_graph_shader_set_output(shader, layout);
		log_verbose("[graph] render.shader output = (%s)", (const char *) v->data.ptr);
	}
	// uniform
	if ((v = pocket_find(p, a, "uniform")))
	{
		if (!pocket_is_tag(p, v, pocket_tag$index, NULL))
			goto label_fail;
		n = v->size;
		v = (const pocket_attr_t *) v->data.ptr;
		while (n)
		{
			--n;
			if (!inner_miiskira_graph_parse_render_shader_add_uniform(r, p, v, shader))
				goto label_fail;
			++v;
		}
	}
	return shader;
	label_fail:
	if (shader) refer_free(shader);
	return NULL;
}

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_shader(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	struct miiskira_graph_shader_s *restrict shader;
	uint64_t n;
	shader = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	n = a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	while (n)
	{
		--n;
		if (!a->name.string)
			goto label_fail;
		if (hashmap_find_name(&r->shader, a->name.string))
			goto label_fail;
		if (!(shader = inner_miiskira_graph_parse_render_shader_create(r, p, a)))
			goto label_fail;
		if (!hashmap_set_name(&r->shader, a->name.string, shader, inner_miiskira_graph_hashmap_free_func))
			goto label_fail;
		shader = NULL;
		log_info("[graph] load render.shader (%s)", a->name.string);
		++a;
	}
	return r;
	label_fail:
	if (shader) refer_free(shader);
	log_error("[graph] load render.shader (%s) fail", a->name.string?a->name.string:"");
	return NULL;
}

// initial

hashmap_t* inner_miiskira_graph_initial_render_parser(hashmap_t *restrict parser)
{
	if (hashmap_set_name(parser, "layout", inner_miiskira_graph_parse_render_layout, NULL) &&
		hashmap_set_name(parser, "shader", inner_miiskira_graph_parse_render_shader, NULL))
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
