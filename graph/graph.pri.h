#ifndef _miiskira_graph_pri_h_
#define _miiskira_graph_pri_h_

#include "graph.h"
#include "../log/log.h"
#include <posky.h>
#include <yaw.h>
#include <hashmap.h>
#include <vattr.h>
#include <exbuffer.h>
#include <graph/graph.h>
#include <graph/allocator.h>
#include <graph/support.h>
#include <graph/device.h>
#include <graph/image.h>
#include <graph/layout.h>
#include <graph/shader.h>
#include <graph/command.h>
#include <graph/buffer.h>
#include <fsys.h>

struct miiskira_graph_device_s {
	graph_dev_s *dev;
	graph_queue_t *q_graphics;
	graph_queue_t *q_compute;
	graph_queue_t *q_transfer;
	graph_pipe_cache_s *pipe_cache;
	// info
	const graph_devices_s *list_device;
	const graph_device_queues_s *list_queue;
	const graph_device_t *select_device;
	const graph_device_queue_t *select_queue_graphics;
	const graph_device_queue_t *select_queue_compute;
	const graph_device_queue_t *select_queue_transfer;
};

struct miiskira_graph_parser_s {
	hashmap_t render;
	hashmap_t model;
	hashmap_t layout_type;
	hashmap_t format_type;
	hashmap_t shader_type;
	hashmap_t image_layout_type;
	fsys_rpath_s *rpath;
};

struct miiskira_graph_range_s {
	uintptr_t offset;
	uintptr_t size;
	uintptr_t type;
	uintptr_t type_size;
};

// 数据布局 用于 vertex 和 uniform
struct miiskira_graph_layout_s {
	uintptr_t size;
	uintptr_t attr_number;
	vattr_s *area;   // name => miiskira_graph_range_s
	graph_vertex_input_description_s *vertex_desc;
};

// uniform
struct miiskira_graph_uniform_s {
	uint32_t binding;
	graph_shader_stage_flags_t shader_stage;
	struct miiskira_graph_layout_s *layout;
	uint32_t share_model;
	uint32_t share_pipe;
	uint32_t share_present;
};

// 渲染流程
struct miiskira_graph_render_pass_s {
	graph_render_pass_s *render;
	graph_render_pass_param_s *param;
	hashmap_t a2i; // attachment-name => index
	hashmap_t p2i; // subpass-name    => index
};

// 着色器
struct miiskira_graph_shader_s {
	graph_shader_type_t type;
	graph_shader_s *shader;
	struct miiskira_graph_layout_s *input;
	struct miiskira_graph_layout_s *output;
	vattr_s *uniform;  // layout-type => miiskira_graph_uniform_s
};

// 图形管线
struct miiskira_graph_gpipe_s {
	graph_pipe_s *pipe;
	struct miiskira_graph_layout_s *vertex;
	vattr_s *uniform;  // layout-type => miiskira_graph_uniform_s
	graph_gpipe_param_s *param;
	graph_pipe_cache_s *cache;
};

// 呈现
struct miiskira_graph_present_s {
	miiskira_graph_present_f present;
	refer_t pri;
	uint32_t width;
	uint32_t height;
	graph_format_t format;
};

struct miiskira_graph_s {
	graph_s *graph;
	struct miiskira_graph_device_s *device;
	struct miiskira_graph_parser_s *parser;
	graph_viewports_scissors_s *viewport;
	hashmap_t layout;   // name => miiskira_graph_layout_s
	hashmap_t blend;    // name => graph_pipe_color_blend_s
	hashmap_t render;   // name => miiskira_graph_render_pass_s
	hashmap_t shader;   // name => miiskira_graph_shader_s
	hashmap_t gpipe;    // name => miiskira_graph_gpipe_s
	hashmap_t present;  // name => miiskira_graph_present_s
};

struct pocket_s;
struct pocket_attr_t;

typedef struct miiskira_graph_s* (*miiskira_graph_parse_f)(struct miiskira_graph_s *restrict r, struct pocket_s *restrict p, const struct pocket_attr_t *restrict a);

extern uint32_t miiskira$type$graph_tag_render;
extern uint32_t miiskira$type$graph_tag_model;

// graph.alloc.c

void inner_miiskira_graph_hashmap_free_func(hashmap_vlist_t *restrict vl);

// debug_level:
//     0 none
//     1 error
//     2 warning
//     3 info
//     4 verbose
//     5 dump api
struct miiskira_graph_s* inner_miiskira_graph_alloc(mlog_s *ml, uint32_t debug_level);

// graph.posky.c

void inner_miiskira_graph_register_posky(struct miiskira_graph_s *restrict graph);

// graph.render.c

hashmap_t* inner_miiskira_graph_initial_render_parser(hashmap_t *restrict parser);

// inner.parser.c

struct miiskira_graph_s* inner_miiskira_graph_parse_pocket(struct miiskira_graph_s *restrict r, const hashmap_t *restrict parser, struct pocket_s *restrict pocket);

// inner.layout.c

hashmap_t* inner_miiskira_graph_initial_layout_type(hashmap_t *restrict layout_type);
struct miiskira_graph_layout_s* inner_miiskira_graph_layout_alloc(void);
struct miiskira_graph_layout_s* inner_miiskira_graph_layout_append(struct miiskira_graph_layout_s *restrict layout, const hashmap_t *restrict layout_type, const char *restrict name, const char *restrict type, const void *data, uintptr_t size);
graph_vertex_input_description_s* inner_miiskira_graph_layout_get_vertex_desc(struct miiskira_graph_layout_s *restrict layout);

// inner.format.c

hashmap_t* inner_miiskira_graph_initial_format_type(hashmap_t *restrict format_type);

// inner.image_layout.c

hashmap_t* inner_miiskira_graph_initial_image_layout_type(hashmap_t *restrict image_layout_type);

// inner.shader.c

hashmap_t* inner_miiskira_graph_initial_shader_type(hashmap_t *restrict shader_type);
struct miiskira_graph_shader_s* inner_miiskira_graph_shader_alloc(graph_dev_s *restrict dev, const hashmap_t *restrict shader_type, const char *restrict type, const void *spv_data, uintptr_t spv_size);
void inner_miiskira_graph_shader_set_input(struct miiskira_graph_shader_s *restrict shader, struct miiskira_graph_layout_s *restrict layout);
void inner_miiskira_graph_shader_set_output(struct miiskira_graph_shader_s *restrict shader, struct miiskira_graph_layout_s *restrict layout);
struct miiskira_graph_uniform_s* inner_miiskira_graph_shader_add_uniform_layout(struct miiskira_graph_shader_s *restrict shader, uint32_t binding, struct miiskira_graph_layout_s *restrict layout, uint32_t share_model, uint32_t share_pipe, uint32_t share_present);

// inner.gpipe.c

struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_alloc(struct miiskira_graph_s *restrict p, uintptr_t shader_number, const char *const shader_name[], const char *const shader_entry[]);
struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_assembly(struct miiskira_graph_gpipe_s *restrict r, graph_primitive_topology_t topology);
struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_viewport(struct miiskira_graph_gpipe_s *restrict r, graph_viewports_scissors_s *restrict viewport);
struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_blend(struct miiskira_graph_gpipe_s *restrict r, graph_pipe_color_blend_s *restrict blend);
struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_render(struct miiskira_graph_gpipe_s *restrict r, graph_render_pass_s *restrict render, uint32_t index);
struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_set_dynamic(struct miiskira_graph_gpipe_s *restrict r, uint32_t n, const graph_dynamic_t dynamic[]);
struct miiskira_graph_gpipe_s* inner_miiskira_graph_gpipe_okay(struct miiskira_graph_gpipe_s *restrict r);

#endif
