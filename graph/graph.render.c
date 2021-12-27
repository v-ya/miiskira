#include "graph.posky.h"
#include <pocket/pocket.h>
#include <stdlib.h>
#include <alloca.h>

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

// blend

static graph_logic_op_t inner_miiskira_graph_parse_render_blend_get_logic_op(const char *restrict s)
{
	uint32_t c, n;
	c = n = 0;
	if (!s) goto label_fail;
	while (*s) c = (c << 8) | *(const uint8_t *) s++, ++n;
	if (n < 3)
	{
		switch (c)
		{
			case ('0'):            return graph_logic_op_clear;
			case ('&'):            return graph_logic_op_and;
			case ('&' << 8 | '~'): return graph_logic_op_and_reverse;
			case ('s'):            return graph_logic_op_copy;
			case ('~' << 8 | '&'): return graph_logic_op_and_inverted;
			case ('d'):            return graph_logic_op_no_op;
			case ('^'):            return graph_logic_op_xor;
			case ('|'):            return graph_logic_op_or;
			case ('!' << 8 | '|'): return graph_logic_op_nor;
			case ('!' << 8 | '^'): return graph_logic_op_equivalent;
			case ('~' << 8 | 'd'): return graph_logic_op_invert;
			case ('|' << 8 | '~'): return graph_logic_op_or_reverse;
			case ('~' << 8 | 's'): return graph_logic_op_copy_inverted;
			case ('~' << 8 | '|'): return graph_logic_op_or_inverted;
			case ('!' << 8 | '&'): return graph_logic_op_nand;
			case ('1'):            return graph_logic_op_set;
		}
	}
	label_fail:
	return graph_logic_op$number;
}

static graph_blend_factor_t inner_miiskira_graph_parse_render_blend_get_factor(const char *restrict s)
{
	uint32_t c, n;
	c = n = 0;
	if (!s) goto label_fail;
	while (*s) c = (c << 8) | *(const uint8_t *) s++, ++n;
	if (n < 5)
	{
		switch (c)
		{
			case ('0'):                                    return graph_blend_factor_zero;
			case ('1'):                                    return graph_blend_factor_one;
			case ('s' << 8 | 'c'):                         return graph_blend_factor_src_color;
			case ('1' << 24 | '-' << 16 | 's' << 8 | 'c'): return graph_blend_factor_one_minus_src_color;
			case ('d' << 8 | 'c'):                         return graph_blend_factor_dst_color;
			case ('1' << 24 | '-' << 16 | 'd' << 8 | 'c'): return graph_blend_factor_one_minus_dst_color;
			case ('s' << 8 | 'a'):                         return graph_blend_factor_src_alpha;
			case ('1' << 24 | '-' << 16 | 's' << 8 | 'a'): return graph_blend_factor_one_minus_src_alpha;
			case ('d' << 8 | 'a'):                         return graph_blend_factor_dst_alpha;
			case ('1' << 24 | '-' << 16 | 'd' << 8 | 'a'): return graph_blend_factor_one_minus_dst_alpha;
			case ('c' << 8 | 'c'):                         return graph_blend_factor_constant_color;
			case ('1' << 24 | '-' << 16 | 'c' << 8 | 'c'): return graph_blend_factor_one_minus_constant_color;
			case ('c' << 8 | 'a'):                         return graph_blend_factor_constant_alpha;
			case ('1' << 24 | '-' << 16 | 'c' << 8 | 'a'): return graph_blend_factor_one_minus_constant_alpha;
		}
	}
	label_fail:
	return graph_blend_factor$number;
}

static graph_blend_op_t inner_miiskira_graph_parse_render_blend_get_op(const char *restrict s)
{
	uint32_t c, n;
	c = n = 0;
	if (!s) goto label_fail;
	while (*s) c = (c << 8) | *(const uint8_t *) s++, ++n;
	if (n < 4)
	{
		switch (c)
		{
			case ('+'):                        return graph_blend_op_add;
			case ('-'):                        return graph_blend_op_subtract;
			case ('-' << 8 | '+'):             return graph_blend_op_reverse_subtract;
			case ('m' << 16 | 'i' << 8 | 'n'): return graph_blend_op_min;
			case ('m' << 16 | 'a' << 8 | 'x'): return graph_blend_op_max;
		}
	}
	label_fail:
	return graph_blend_op$number;
}

static graph_pipe_color_blend_s* inner_miiskira_graph_parse_render_blend_create(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	const pocket_attr_t *restrict attach, *restrict v;
	graph_pipe_color_blend_s *restrict blend;
	uint64_t n;
	blend = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	// attachment
	attach = pocket_find(p, a, "attachment");
	blend = graph_pipe_color_blend_alloc(attach?((uint32_t) attach->size):0);
	if (!blend)
		goto label_fail;
	if (attach)
	{
		graph_blend_factor_t s_color, d_color, s_alpha, d_alpha;
		graph_blend_op_t op_color, op_alpha;
		graph_color_component_mask_t mask;
		if (!pocket_is_tag(p, attach, pocket_tag$index, NULL))
			goto label_fail;
		n = attach->size;
		attach = (const pocket_attr_t *) attach->data.ptr;
		while (n)
		{
			--n;
			mask = graph_color_component_mask_all;
			// mask
			if ((v = pocket_find(p, attach, "mask")))
			{
				if (!pocket_is_tag(p, v, pocket_tag$u8, NULL))
					goto label_fail;
				if (v->size != (sizeof(uint8_t) * 4))
					goto label_fail;
				mask = 0;
				if (((uint8_t *) v->data.ptr)[0]) mask |= graph_color_component_mask_r;
				if (((uint8_t *) v->data.ptr)[1]) mask |= graph_color_component_mask_g;
				if (((uint8_t *) v->data.ptr)[2]) mask |= graph_color_component_mask_b;
				if (((uint8_t *) v->data.ptr)[3]) mask |= graph_color_component_mask_a;
			}
			// src-color
			if (!(v = pocket_find_tag(p, attach, "src-color", pocket_tag$string, NULL)))
				goto label_fail;
			if ((s_color = inner_miiskira_graph_parse_render_blend_get_factor((const char *) v->data.ptr)) >= graph_blend_factor$number)
				goto label_fail;
			// dst-color
			if (!(v = pocket_find_tag(p, attach, "dst-color", pocket_tag$string, NULL)))
				goto label_fail;
			if ((d_color = inner_miiskira_graph_parse_render_blend_get_factor((const char *) v->data.ptr)) >= graph_blend_factor$number)
				goto label_fail;
			// op-color
			if (!(v = pocket_find_tag(p, attach, "op-color", pocket_tag$string, NULL)))
				goto label_fail;
			if ((op_color = inner_miiskira_graph_parse_render_blend_get_op((const char *) v->data.ptr)) >= graph_blend_op$number)
				goto label_fail;
			// src-alpha
			if (!(v = pocket_find_tag(p, attach, "src-alpha", pocket_tag$string, NULL)))
				goto label_fail;
			if ((s_alpha = inner_miiskira_graph_parse_render_blend_get_factor((const char *) v->data.ptr)) >= graph_blend_factor$number)
				goto label_fail;
			// dst-alpha
			if (!(v = pocket_find_tag(p, attach, "dst-alpha", pocket_tag$string, NULL)))
				goto label_fail;
			if ((d_alpha = inner_miiskira_graph_parse_render_blend_get_factor((const char *) v->data.ptr)) >= graph_blend_factor$number)
				goto label_fail;
			// op-alpha
			if (!(v = pocket_find_tag(p, attach, "op-alpha", pocket_tag$string, NULL)))
				goto label_fail;
			if ((op_alpha = inner_miiskira_graph_parse_render_blend_get_op((const char *) v->data.ptr)) >= graph_blend_op$number)
				goto label_fail;
			// append
			if (!graph_pipe_color_blend_append_attachment(blend, 1, mask, s_color, d_color, op_color, s_alpha, d_alpha, op_alpha))
				goto label_fail;
			++attach;
		}
	}
	// logic
	if ((v = pocket_find(p, a, "logic")))
	{
		graph_logic_op_t logic_op;
		if (!pocket_is_tag(p, v, pocket_tag$string, NULL))
			goto label_fail;
		if ((logic_op = inner_miiskira_graph_parse_render_blend_get_logic_op((const char *) v->data.ptr)) >= graph_logic_op$number)
			goto label_fail;
		graph_pipe_color_blend_set_logic(blend, 1, logic_op);
	}
	// color
	if ((v = pocket_find(p, a, "color")))
	{
		if (!pocket_is_tag(p, v, pocket_tag$f32, NULL))
			goto label_fail;
		if (v->size != (sizeof(float) * 4))
			goto label_fail;
		graph_pipe_color_blend_set_constants(blend, (float *) v->data.ptr);
	}
	return blend;
	label_fail:
	if (blend) refer_free(blend);
	return NULL;
}

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_blend(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	graph_pipe_color_blend_s *restrict blend;
	uint64_t n;
	blend = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	n = a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	while (n)
	{
		--n;
		if (!a->name.string)
			goto label_fail;
		if (hashmap_find_name(&r->blend, a->name.string))
			goto label_fail;
		if (!(blend = inner_miiskira_graph_parse_render_blend_create(r, p, a)))
			goto label_fail;
		if (!hashmap_set_name(&r->blend, a->name.string, blend, inner_miiskira_graph_hashmap_free_func))
			goto label_fail;
		blend = NULL;
		log_info("[graph] load render.blend (%s)", a->name.string);
		++a;
	}
	return r;
	label_fail:
	if (blend) refer_free(blend);
	log_error("[graph] load render.blend (%s) fail", a->name.string?a->name.string:"");
	return NULL;
}

// render

static struct miiskira_graph_render_pass_s* inner_miiskira_graph_parse_render_render_create(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	struct miiskira_graph_render_pass_s *restrict render;
	render = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	// graph_render_pass_param_s* graph_render_pass_param_alloc(uint32_t attachment_number, uint32_t subpass_number, uint32_t dependency_number, uint32_t aref_number);
	// graph_render_pass_param_s* graph_render_pass_param_set_attachment(graph_render_pass_param_s *restrict r, uint32_t index, graph_format_t format, graph_sample_count_t sample, graph_attachment_load_op_t load, graph_attachment_store_op_t store, graph_attachment_load_op_t stencil_load, graph_attachment_store_op_t stencil_store, graph_image_layout_t initial, graph_image_layout_t final);
	// graph_render_pass_param_s* graph_render_pass_param_set_subpass(graph_render_pass_param_s *restrict r, uint32_t index, graph_pipeline_bind_point_t type);
	// graph_render_pass_param_s* graph_render_pass_param_set_subpass_color(graph_render_pass_param_s *restrict r, uint32_t index, uint32_t n, uint32_t at_index[], graph_image_layout_t layout[]);
	// graph_render_pass_s* graph_render_pass_alloc(graph_render_pass_param_s *restrict param, struct graph_dev_s *restrict dev);
	return render;
	label_fail:
	if (render) refer_free(render);
	return NULL;
}

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_render(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	struct miiskira_graph_render_pass_s *restrict render;
	uint64_t n;
	render = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	n = a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	while (n)
	{
		--n;
		if (!a->name.string)
			goto label_fail;
		if (hashmap_find_name(&r->render, a->name.string))
			goto label_fail;
		if (!(render = inner_miiskira_graph_parse_render_render_create(r, p, a)))
			goto label_fail;
		if (!hashmap_set_name(&r->render, a->name.string, render, inner_miiskira_graph_hashmap_free_func))
			goto label_fail;
		render = NULL;
		log_info("[graph] load render.render (%s)", a->name.string);
		++a;
	}
	return r;
	label_fail:
	if (render) refer_free(render);
	log_error("[graph] load render.render (%s) fail", a->name.string?a->name.string:"");
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

// g-pipe

static graph_primitive_topology_t inner_miiskira_graph_parse_render_gpipe_get_topology(const char *restrict s)
{
	uint32_t c, n;
	c = n = 0;
	if (!s) goto label_fail;
	while (*s) c = (c << 8) | *(const uint8_t *) s++, ++n;
	if (n < 4)
	{
		switch (c)
		{
			case ('p' << 8 | 'l'): return graph_primitive_topology_point_list;
			case ('l' << 8 | 'l'): return graph_primitive_topology_line_list;
			case ('l' << 8 | 's'): return graph_primitive_topology_line_strip;
			case ('t' << 8 | 'l'): return graph_primitive_topology_triangle_list;
			case ('t' << 8 | 's'): return graph_primitive_topology_triangle_strip;
			case ('t' << 8 | 'f'): return graph_primitive_topology_triangle_fan;
			case ('l' << 16 | 'l' << 8 | 'a'): return graph_primitive_topology_line_list_with_adjacency;
			case ('l' << 16 | 's' << 8 | 'a'): return graph_primitive_topology_line_strip_with_adjacency;
			case ('t' << 16 | 'l' << 8 | 'a'): return graph_primitive_topology_triangle_list_with_adjacency;
			case ('t' << 16 | 's' << 8 | 'a'): return graph_primitive_topology_triangle_strip_with_adjacency;
		}
	}
	label_fail:
	return graph_primitive_topology$number;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_parse_render_gpipe_create_shader(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	uintptr_t i, n;
	const char **s;
	if (!(a = pocket_find_tag(p, a, "shader", pocket_tag$index, NULL)))
		goto label_fail;
	n = (uintptr_t) a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	if (!(s = (const char **) alloca(sizeof(*s) * 2 * n)))
		goto label_fail;
	for (i = 0; i < n; ++i)
	{
		if (!pocket_is_tag(p, a + i, pocket_tag$string, NULL))
			goto label_fail;
		s[i] = a[i].name.string;
		s[n + i] = (const char *) a[i].data.ptr;
		log_verbose("[graph] render.g-pipe shader(%s) entry(%s)", s[i]?s[i]:"", s[n + i]?s[n + i]:"");
	}
	return inner_miiskira_graph_gpipe_alloc(r, n, s, s + n);
	label_fail:
	log_warning("[graph] render.g-pipe shader fail");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_parse_render_gpipe_create_topology(struct miiskira_graph_gpipe_s *restrict gpipe, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	graph_primitive_topology_t tp;
	if (!(a = pocket_find_tag(p, a, "topology", pocket_tag$string, NULL)))
		goto label_fail;
	if ((uint32_t) (tp = inner_miiskira_graph_parse_render_gpipe_get_topology((const char *) a->data.ptr)) >= graph_primitive_topology$number)
		goto label_fail;
	if (!inner_miiskira_graph_gpipe_set_assembly(gpipe, tp))
		goto label_fail;
	log_verbose("[graph] render.g-pipe topology(%s)", (const char *) a->data.ptr);
	return gpipe;
	label_fail:
	log_warning("[graph] render.g-pipe topology fail");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_parse_render_gpipe_create_blend(struct miiskira_graph_gpipe_s *restrict gpipe, struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	graph_pipe_color_blend_s *restrict blend;
	if (!(a = pocket_find_tag(p, a, "blend", pocket_tag$string, NULL)))
		goto label_fail;
	if (!a->data.ptr)
		goto label_fail;
	if (!(blend = (graph_pipe_color_blend_s *) hashmap_get_name(&r->blend, (const char *) a->data.ptr)))
		goto label_fail;
	if (!inner_miiskira_graph_gpipe_set_blend(gpipe, blend))
		goto label_fail;
	log_verbose("[graph] render.g-pipe blend(%s)", (const char *) a->data.ptr);
	return gpipe;
	label_fail:
	log_warning("[graph] render.g-pipe blend fail");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_parse_render_gpipe_create_dynamic(struct miiskira_graph_gpipe_s *restrict gpipe, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	graph_dynamic_t dynamic[graph_dynamic$number];
	uint32_t n;
	n = 0;
	dynamic[n++] = graph_dynamic_viewport;
	dynamic[n++] = graph_dynamic_scissor;
	if ((a = pocket_find(p, a, "dynamic")))
	{
		if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
			goto label_fail;
		if (pocket_find(p, a, "line_width"))           dynamic[n++] = graph_dynamic_line_width;
		if (pocket_find(p, a, "depth_bias"))           dynamic[n++] = graph_dynamic_depth_bias;
		if (pocket_find(p, a, "blend_constants"))      dynamic[n++] = graph_dynamic_blend_constants;
		if (pocket_find(p, a, "depth_bounds"))         dynamic[n++] = graph_dynamic_depth_bounds;
		if (pocket_find(p, a, "stencil_compare_mask")) dynamic[n++] = graph_dynamic_stencil_compare_mask;
		if (pocket_find(p, a, "stencil_write_mask"))   dynamic[n++] = graph_dynamic_stencil_write_mask;
		if (pocket_find(p, a, "stencil_reference"))    dynamic[n++] = graph_dynamic_stencil_reference;
	}
	if (!inner_miiskira_graph_gpipe_set_dynamic(gpipe, n, dynamic))
		goto label_fail;
	return gpipe;
	label_fail:
	log_warning("[graph] render.g-pipe dynamic fail");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_parse_render_gpipe_create(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	struct miiskira_graph_gpipe_s *restrict gpipe;
	gpipe = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	if (!(gpipe = inner_miiskira_graph_parse_render_gpipe_create_shader(r, p, a)))
		goto label_fail;
	if (!inner_miiskira_graph_parse_render_gpipe_create_topology(gpipe, p, a))
		goto label_fail;
	if (!inner_miiskira_graph_parse_render_gpipe_create_blend(gpipe, r, p, a))
		goto label_fail;
	if (!inner_miiskira_graph_parse_render_gpipe_create_dynamic(gpipe, p, a))
		goto label_fail;
	if (!inner_miiskira_graph_gpipe_okay(gpipe))
		goto label_fail;
	return gpipe;
	label_fail:
	if (gpipe) refer_free(gpipe);
	return NULL;
}

static struct miiskira_graph_s* inner_miiskira_graph_parse_render_gpipe(struct miiskira_graph_s *restrict r, pocket_s *restrict p, const pocket_attr_t *restrict a)
{
	struct miiskira_graph_gpipe_s *restrict gpipe;
	uint64_t n;
	gpipe = NULL;
	if (!pocket_is_tag(p, a, pocket_tag$index, NULL))
		goto label_fail;
	n = a->size;
	a = (const pocket_attr_t *) a->data.ptr;
	while (n)
	{
		--n;
		if (!a->name.string)
			goto label_fail;
		if (hashmap_find_name(&r->gpipe, a->name.string))
			goto label_fail;
		if (!(gpipe = inner_miiskira_graph_parse_render_gpipe_create(r, p, a)))
			goto label_fail;
		if (!hashmap_set_name(&r->gpipe, a->name.string, gpipe, inner_miiskira_graph_hashmap_free_func))
			goto label_fail;
		gpipe = NULL;
		log_info("[graph] load render.g-pipe (%s)", a->name.string);
		++a;
	}
	return r;
	label_fail:
	if (gpipe) refer_free(gpipe);
	log_error("[graph] load render.g-pipe (%s) fail", a->name.string?a->name.string:"");
	return NULL;
}

// initial

hashmap_t* inner_miiskira_graph_initial_render_parser(hashmap_t *restrict parser)
{
	if (hashmap_set_name(parser, "layout", inner_miiskira_graph_parse_render_layout, NULL) &&
		hashmap_set_name(parser, "blend", inner_miiskira_graph_parse_render_blend, NULL) &&
		hashmap_set_name(parser, "render", inner_miiskira_graph_parse_render_render, NULL) &&
		hashmap_set_name(parser, "shader", inner_miiskira_graph_parse_render_shader, NULL) &&
		hashmap_set_name(parser, "g-pipe", inner_miiskira_graph_parse_render_gpipe, NULL))
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
