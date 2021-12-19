#include "graph.pri.h"

hashmap_t* inner_miiskira_graph_initial_shader_type(hashmap_t *restrict shader_type)
{
	if (
		#define d_type(_id, _name)  hashmap_set_name(shader_type, #_name, (void *) (uintptr_t) graph_shader_type_##_id, NULL)
		d_type(vertex,                  vertex) &&
		d_type(tessellation_control,    tess-ctrl) &&
		d_type(tessellation_evaluation, tess-eval) &&
		d_type(geometry,                geometry) &&
		d_type(fragment,                fragment) &&
		d_type(compute,                 compute)
		#undef d_type
	) return shader_type;
	return NULL;
}

static void inner_miiskira_graph_shader_free_func(struct miiskira_graph_shader_s *restrict r)
{
	if (r->uniform) refer_free(r->uniform);
	if (r->output) refer_free(r->output);
	if (r->input) refer_free(r->input);
	if (r->shader) refer_free(r->shader);
}

static struct miiskira_graph_shader_s* inner_miiskira_graph_shader_alloc_empty(void)
{
	struct miiskira_graph_shader_s *restrict r;
	if ((r = (struct miiskira_graph_shader_s *) refer_alloz(sizeof(struct miiskira_graph_shader_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_graph_shader_free_func);
		if ((r->uniform = vattr_alloc()))
			return r;
		refer_free(r);
	}
	return NULL;
}

struct miiskira_graph_shader_s* inner_miiskira_graph_shader_alloc(graph_dev_s *restrict dev, const hashmap_t *restrict shader_type, const char *restrict type, const void *spv_data, uintptr_t spv_size)
{
	struct miiskira_graph_shader_s *restrict r;
	hashmap_vlist_t *restrict vl;
	if (type && spv_size && (vl = hashmap_find_name(shader_type, type)) &&
		(r = inner_miiskira_graph_shader_alloc_empty()))
	{
		r->type = (graph_shader_type_t) (uintptr_t) vl->value;
		r->shader = graph_shader_alloc(dev, spv_data, spv_size);
		if (r->shader)
			return r;
		refer_free(r);
	}
	return NULL;
}
