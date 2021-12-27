#include "graph.pri.h"
#include <alloca.h>

static void inner_miiskira_graph_gpipe_free_func(struct miiskira_graph_gpipe_s *restrict r)
{
	if (r->pipe) refer_free(r->pipe);
	if (r->vertex) refer_free(r->vertex);
	if (r->uniform) refer_free(r->uniform);
	if (r->param) refer_free(r->param);
	if (r->cache) refer_free(r->cache);
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_alloc_empty(graph_dev_s *restrict dev, graph_pipe_cache_s *restrict cache, uintptr_t shader_number)
{
	struct miiskira_graph_gpipe_s *restrict r;
	if ((r = (struct miiskira_graph_gpipe_s *) refer_alloz(sizeof(struct miiskira_graph_gpipe_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_graph_gpipe_free_func);
		r->cache = (graph_pipe_cache_s *) refer_save(cache);
		if ((r->uniform = vattr_alloc()) &&
			(r->param = graph_gpipe_param_alloc(dev, shader_number)))
			return r;
		refer_free(r);
	}
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_append_shader(struct miiskira_graph_gpipe_s *restrict r, uintptr_t n, struct miiskira_graph_shader_s *const shader_array[], const char *const shader_entry[])
{
	struct miiskira_graph_layout_s *last_layout;
	graph_shader_type_t last_type;
	uintptr_t i;
	if (shader_array[0]->type != graph_shader_type_vertex)
		goto label_fail;
	if (shader_array[n - 1]->type != graph_shader_type_fragment)
		goto label_fail;
	last_layout = NULL;
	last_type = 0;
	r->vertex = (struct miiskira_graph_layout_s *) refer_save(shader_array[0]->input);
	for (i = 0; i < n; ++i)
	{
		if (shader_array[i]->type <= last_type)
			goto label_fail;
		if (last_layout && shader_array[i]->input != last_layout)
			goto label_fail;
		if (!shader_entry[i])
			goto label_fail;
		if (!graph_gpipe_param_append_shader(r->param, shader_array[i]->shader, shader_array[i]->type, shader_entry[i], NULL))
			goto label_fail;
		last_type = shader_array[i]->type;
		last_layout = shader_array[i]->output;
	}
	return r;
	label_fail:
	log_warning("[graph] create gpipe: append shader fail");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_append_uniform(struct miiskira_graph_gpipe_s *restrict r, uintptr_t n, vattr_vlist_t *uniform_array[])
{
	struct miiskira_graph_uniform_s *restrict uniform;
	uintptr_t i, min_i, have_n;
	uint32_t min_bind, last_bind;
	last_bind = ~0;
	do {
		have_n = 0;
		min_bind = ~0;
		for (i = 0; i < n; ++i)
		{
			if (uniform_array[i])
			{
				++have_n;
				uniform = (struct miiskira_graph_uniform_s *) uniform_array[i]->value;
				if (uniform->binding < min_bind)
				{
					min_bind = uniform->binding;
					min_i = i;
				}
			}
		}
		if (~min_bind)
		{
			if (~last_bind && min_bind <= last_bind)
				goto label_fail;
			if (!vattr_insert_tail(r->uniform, uniform_array[min_i]->vslot->key, uniform_array[min_i]->value))
				goto label_fail;
			uniform_array[min_i] = uniform_array[min_i]->vattr_next;
			last_bind = min_bind;
		}
		else if (have_n)
			goto label_fail;
	} while (have_n);
	return r;
	label_fail:
	log_warning("[graph] create gpipe: append uniform fail, maybe binding same?");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_vertex(struct miiskira_graph_gpipe_s *restrict r)
{
	graph_vertex_input_description_s *restrict vid;
	if (r->vertex)
	{
		if (!(vid = inner_miiskira_graph_layout_get_vertex_desc(r->vertex)))
			goto label_fail;
	}
	else vid = NULL;
	graph_gpipe_param_set_vertex(r->param, vid);
	return r;
	label_fail:
	log_warning("[graph] create gpipe: set vertex input desc fail");
	return NULL;
}

static struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_layout(struct miiskira_graph_gpipe_s *restrict r, graph_dev_s *restrict dev)
{
	graph_descriptor_set_layout_param_s *restrict set_param;
	graph_descriptor_set_layout_s *restrict set_layout;
	struct miiskira_graph_uniform_s *restrict uniform;
	vattr_vlist_t *restrict vl;
	graph_pipe_layout_param_s *restrict param;
	graph_pipe_layout_s *restrict layout;
	struct miiskira_graph_gpipe_s *ret;
	uint32_t index;
	ret = NULL;
	set_param = graph_descriptor_set_layout_param_alloc(
		(uint32_t) vattr_get_vslot_number(r->uniform, "layout"), 0, 0);
	if (set_param)
	{
		index = 0;
		for (vl = r->uniform->vattr; vl; vl = vl->vattr_next)
		{
			// 这里或许可以更改成 graph_desc_type_uniform_buffer_dynamic ?
			// 可以一次性批量提交绘制
			// 但 graph_desc_type_uniform_buffer_dynamic 缓冲区的数量好像有限制
			// 目前不太清楚是 单管线 还是 单设备或是实例 限制数量
			uniform = (struct miiskira_graph_uniform_s *) vl->value;
			if (!graph_descriptor_set_layout_param_set_binding(
					set_param, index++, uniform->binding,
					graph_desc_type_uniform_buffer, 1,
					uniform->shader_stage))
				goto label_fail;
		}
		if ((set_layout = graph_descriptor_set_layout_alloc(set_param, dev)))
		{
			if ((param = graph_pipe_layout_param_alloc(1, 0)))
			{
				if (graph_pipe_layout_param_append_set_layout(param, set_layout) &&
					(layout = graph_pipe_layout_alloc(dev, param)))
				{
					graph_gpipe_param_set_layout(r->param, layout);
					ret = r;
					refer_free(layout);
				}
				refer_free(param);
			}
			refer_free(set_layout);
		}
		label_fail:
		refer_free(set_param);
	}
	if (!ret)
		log_warning("[graph] create gpipe: set layout fail");
	return ret;
}

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_alloc(struct miiskira_graph_s *restrict p, uintptr_t shader_number, const char *const shader_name[], const char *const shader_entry[])
{
	struct miiskira_graph_gpipe_s *restrict r;
	struct miiskira_graph_shader_s **shader_array;
	vattr_vlist_t **uniform_array;
	uintptr_t i;
	if (shader_number && (r = inner_miiskira_graph_gpipe_alloc_empty(p->device->dev, p->device->pipe_cache, shader_number)))
	{
		shader_array = (struct miiskira_graph_shader_s **) alloca(sizeof(*shader_array) * shader_number);
		uniform_array = (vattr_vlist_t **) alloca(sizeof(*uniform_array) * shader_number);
		if (!shader_array || !uniform_array)
			goto label_fail;
		for (i = 0; i < shader_number; ++i)
		{
			if (!shader_name[i])
				goto label_fail;
			shader_array[i] = (struct miiskira_graph_shader_s *) hashmap_get_name(&p->shader, shader_name[i]);
			if (!shader_array[i])
				goto label_fail;
			uniform_array[i] = shader_array[i]->uniform->vattr;
		}
		if (!inner_miiskira_graph_gpipe_append_shader(r, shader_number, shader_array, shader_entry))
			goto label_fail;
		if (!inner_miiskira_graph_gpipe_append_uniform(r, shader_number, uniform_array))
			goto label_fail;
		if (!inner_miiskira_graph_gpipe_set_vertex(r))
			goto label_fail;
		if (!inner_miiskira_graph_gpipe_set_layout(r, p->device->dev))
			goto label_fail;
		return r;
		label_fail:
		refer_free(r);
	}
	return NULL;
}

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_assembly(struct miiskira_graph_gpipe_s *restrict r, graph_primitive_topology_t topology)
{
	if (graph_gpipe_param_set_assembly(r->param, topology, 1))
		return r;
	return NULL;
}

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_blend(struct miiskira_graph_gpipe_s *restrict r, graph_pipe_color_blend_s *restrict blend)
{
	if (graph_gpipe_param_set_color_blend(r->param, blend))
		return r;
	return NULL;
}

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_render(struct miiskira_graph_gpipe_s *restrict r, graph_render_pass_s *restrict render, uint32_t index)
{
	graph_gpipe_param_set_render(r->param, render, index);
	return r;
}

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_dynamic(struct miiskira_graph_gpipe_s *restrict r, uint32_t n, const graph_dynamic_t dynamic[])
{
	if (graph_gpipe_param_set_dynamic(r->param, n, dynamic))
		return r;
	return NULL;
}

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_okay(struct miiskira_graph_gpipe_s *restrict r)
{
	if (r->pipe) goto label_okay;
	// graph_gpipe_param_set_rasterization_depth_clamp();
	// graph_gpipe_param_set_rasterization_discard();
	// graph_gpipe_param_set_rasterization_polygon();
	// graph_gpipe_param_set_rasterization_cull();
	// graph_gpipe_param_set_rasterization_front_face();
	// graph_gpipe_param_set_rasterization_depth_bias();
	// graph_gpipe_param_set_rasterization_line_width();
	// graph_gpipe_param_set_multisample_sample_shading();
	// graph_gpipe_param_set_multisample_min_sample_shading();
	// graph_gpipe_param_set_multisample_alpha2coverage();
	// graph_gpipe_param_set_multisample_alpha2one();
	if (graph_gpipe_param_ok(r->param) &&
		(r->pipe = graph_pipe_alloc_graphics(r->param, r->cache)))
	{
		label_okay:
		return r;
	}
	return NULL;
}
