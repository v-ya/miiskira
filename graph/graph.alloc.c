#include "graph.pri.h"

void inner_miiskira_graph_hashmap_free_func(hashmap_vlist_t *restrict vl)
{
	if (vl->value) refer_free(vl->value);
}

// create

static graph_s* inner_miiskira_graph_create_graph(mlog_s *ml, uint32_t debug_level)
{
	static const char name[] = "miiskira";
	graph_s *restrict r;
	graph_layer_t layer[3];
	graph_extension_t extension[2];
	graph_debug_message_level_t dm_level;
	graph_debug_message_type_t dm_type;
	if ((r = graph_alloc(ml, NULL)))
	{
		if (!debug_level)
		{
			layer[0] = 0;
			extension[0] = 0;
		}
		else
		{
			if (debug_level < 5)
			{
				layer[0] = graph_layer_validation;
				layer[1] = 0;
			}
			else
			{
				layer[0] = graph_layer_validation;
				layer[1] = graph_layer_api_dump;
				layer[2] = 0;
			}
			extension[0] = graph_extension_ext_debug_utils;
			extension[1] = 0;
		}
		if (graph_init_instance(r, name, name, layer, extension))
		{
			dm_level = 0;
			dm_type = graph_debug_message_type_general |
				graph_debug_message_type_validation |
				graph_debug_message_type_performance;
			switch (debug_level)
			{
				default: dm_level |= graph_debug_message_level_verbose;
					// fall through
				case 3: dm_level |= graph_debug_message_level_info;
					// fall through
				case 2: dm_level |= graph_debug_message_level_warning;
					// fall through
				case 1: dm_level |= graph_debug_message_level_error;
					// fall through
				case 0: break;
			}
			if (!debug_level || graph_instance_enable_debug_message(r, dm_level, dm_type))
				return r;
		}
		refer_free(r);
	}
	return NULL;
}

static const graph_device_t* inner_miiskira_graph_device_select_device(const graph_devices_s *restrict list_device, uint32_t debug_level)
{
	const graph_device_t *d, *d_gpu_d, *d_gpu_i, *d_cpu;
	uint32_t i, n;
	d_gpu_d = d_gpu_i = d_cpu = NULL;
	n = graph_devices_number(list_device);
	for (i = 0; i < n; ++i)
	{
		if ((d = graph_devices_index(list_device, i)))
		{
			switch (graph_device_type(d))
			{
				case graph_physical_device_type_discrete_gpu:
					if (!d_gpu_d) d_gpu_d = d;
					break;
				case graph_physical_device_type_integrated_gpu:
					if (!d_gpu_i) d_gpu_i = d;
					break;
				case graph_physical_device_type_cpu:
					if (!d_cpu) d_cpu = d;
					break;
				default: break;
			}
		}
	}
	d = d_gpu_d?d_gpu_d:(d_gpu_i?d_gpu_i:d_cpu);
	if (d && debug_level)
	{
		if (debug_level <= 3)
			graph_device_properties_header_dump(d);
		else graph_device_dump(d);
	}
	return d;
}

static void inner_miiskira_graph_device_select_queue(const graph_device_queues_s *restrict list_queue, const graph_device_queue_t **restrict q_graphics, const graph_device_queue_t **restrict q_compute, const graph_device_queue_t **restrict q_transfer)
{
	const graph_device_queue_t *q, *qg, *qc, *qt;
	graph_queue_flags_t flags;
	uint32_t i, n;
	qg = qc = qt = NULL;
	n = graph_device_queues_number(list_queue);
	for (i = 0; i < n; ++i)
	{
		if ((q = graph_device_queues_index(list_queue, i)))
		{
			flags = graph_device_queue_flags(q);
			if (!qg && (flags & graph_queue_flags_graphics))
				qg = q;
			if (!qc && (flags & graph_queue_flags_compute))
				qc = q;
			if (!qt && (flags & graph_queue_flags_transfer))
				qt = q;
		}
	}
	if (q_graphics) *q_graphics = qg;
	if (q_compute) *q_compute = qc;
	if (q_transfer) *q_transfer = qt;
}

static struct miiskira_graph_device_s* inner_miiskira_graph_device_create_dev(struct miiskira_graph_device_s *restrict r, graph_s *restrict graph, uint32_t debug_level)
{
	graph_dev_param_s *restrict param;
	const graph_device_queue_t *queue_array[3];
	uint32_t i, queue_number;
	queue_number = 0;
	#define d_push_queue(_type)  \
		if (r->select_queue_##_type)\
		{\
			for (i = 0; i < queue_number; ++i)\
			if (queue_array[i] == r->select_queue_##_type)\
				break;\
			if (i == queue_number)\
				queue_array[queue_number++] = r->select_queue_##_type;\
		}
	d_push_queue(graphics)
	d_push_queue(compute)
	d_push_queue(transfer)
	#undef d_push_queue
	if (queue_number)
	{
		param = graph_dev_param_alloc(queue_number);
		if (param)
		{
			if (graph_dev_param_set_layer(param, (graph_layer_t []) {0}) &&
				graph_dev_param_set_extension(param, (graph_extension_t []) {0}))
			{
				for (i = 0; i < queue_number; ++i)
				{
					if (debug_level) graph_device_queue_dump(queue_array[i]);
					if (!graph_dev_param_set_queue(param, i, queue_array[i], 1, NULL, 0))
						goto label_fail;
				}
				graph_dev_param_feature_enable(param, graph_device_feature_samplerAnisotropy);
				if ((r->dev = graph_dev_alloc(param, graph, r->select_device)))
				{
					if (r->select_queue_graphics)
						r->q_graphics = graph_dev_queue(r->dev, r->select_queue_graphics, 0);
					if (r->select_queue_compute)
						r->q_compute = graph_dev_queue(r->dev, r->select_queue_compute, 0);
					if (r->select_queue_transfer)
						r->q_transfer = graph_dev_queue(r->dev, r->select_queue_transfer, 0);
				}
			}
			label_fail:
			refer_free(param);
		}
	}
	if (r->dev) return r;
	return NULL;
}

// alloc device

static void inner_miiskira_graph_device_free_func(struct miiskira_graph_device_s *restrict r)
{
	if (r->dev) refer_free(r->dev);
	if (r->pipe_cache) refer_free(r->pipe_cache);
	if (r->list_queue) refer_free(r->list_queue);
	if (r->list_device) refer_free(r->list_device);
}

static struct miiskira_graph_device_s* inner_miiskira_graph_device_alloc(graph_s *restrict graph, uint32_t debug_level)
{
	struct miiskira_graph_device_s *restrict r;
	if ((r = (struct miiskira_graph_device_s *) refer_alloz(sizeof(struct miiskira_graph_device_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_graph_device_free_func);
		if (!(r->list_device = graph_instance_devices_get(graph)))
			goto label_fail;
		if (!(r->select_device = inner_miiskira_graph_device_select_device(r->list_device, debug_level)))
			goto label_fail;
		if (!(r->list_queue = graph_device_queues_get(r->select_device)))
			goto label_fail;
		inner_miiskira_graph_device_select_queue(r->list_queue, &r->select_queue_graphics, &r->select_queue_compute, &r->select_queue_transfer);
		if (!r->select_queue_graphics || !r->select_queue_transfer)
			goto label_fail;
		if (!inner_miiskira_graph_device_create_dev(r, graph, debug_level))
			goto label_fail;
		if (!(r->pipe_cache = graph_pipe_cache_alloc(r->dev)))
			goto label_fail;
		return r;
		label_fail:
		refer_free(r);
	}
	return NULL;
}

// alloc parser

static void inner_miiskira_graph_parser_free_func(struct miiskira_graph_parser_s *restrict r)
{
	if (r->rpath) refer_free(r->rpath);
	hashmap_uini(&r->shader_type);
	hashmap_uini(&r->layout_type);
	hashmap_uini(&r->model);
	hashmap_uini(&r->render);
}

static struct miiskira_graph_parser_s* inner_miiskira_graph_parser_alloc(void)
{
	struct miiskira_graph_parser_s *restrict r;
	if ((r = (struct miiskira_graph_parser_s *) refer_alloz(sizeof(struct miiskira_graph_parser_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_graph_parser_free_func);
		if (!hashmap_init(&r->render)) goto label_fail;
		if (!hashmap_init(&r->model)) goto label_fail;
		if (!hashmap_init(&r->layout_type)) goto label_fail;
		if (!hashmap_init(&r->shader_type)) goto label_fail;
		if (!(r->rpath = fsys_rpath_alloc(NULL, 256)))
			goto label_fail;
		fsys_rpath_set_delimiter(r->rpath, '.');
		if (!inner_miiskira_graph_initial_render_parser(&r->render))
			goto label_fail;
		if (!inner_miiskira_graph_initial_layout_type(&r->layout_type))
			goto label_fail;
		if (!inner_miiskira_graph_initial_shader_type(&r->shader_type))
			goto label_fail;
		return r;
		label_fail:
		refer_free(r);
	}
	return NULL;
}

// alloc

static void inner_miiskira_graph_free_func(struct miiskira_graph_s *restrict r)
{
	hashmap_uini(&r->present);
	hashmap_uini(&r->gpipe);
	hashmap_uini(&r->shader);
	hashmap_uini(&r->blend);
	hashmap_uini(&r->layout);
	if (r->parser) refer_free(r->parser);
	if (r->device) refer_free(r->device);
	if (r->graph) refer_free(r->graph);
}

struct miiskira_graph_s* inner_miiskira_graph_alloc(mlog_s *ml, uint32_t debug_level)
{
	struct miiskira_graph_s *restrict r;
	if ((r = (struct miiskira_graph_s *) refer_alloz(sizeof(struct miiskira_graph_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_graph_free_func);
		if (!hashmap_init(&r->layout)) goto label_fail;
		if (!hashmap_init(&r->blend)) goto label_fail;
		if (!hashmap_init(&r->shader)) goto label_fail;
		if (!hashmap_init(&r->gpipe)) goto label_fail;
		if (!hashmap_init(&r->present)) goto label_fail;
		if (!(r->parser = inner_miiskira_graph_parser_alloc()))
			goto label_fail;
		if (!(r->graph = inner_miiskira_graph_create_graph(ml, debug_level)))
			goto label_fail;
		if (!(r->device = inner_miiskira_graph_device_alloc(r->graph, debug_level)))
			goto label_fail;
		return r;
		label_fail:
		refer_free(r);
	}
	return NULL;
}
