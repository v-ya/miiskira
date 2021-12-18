#ifndef _miiskira_graph_pri_h_
#define _miiskira_graph_pri_h_

#include "graph.h"
#include <posky.h>
#include <yaw.h>
#include <hashmap.h>
#include <vattr.h>
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
	fsys_rpath_s *rpath;
};

// 数据布局 用于 vertex 和 uniform
struct miiskira_graph_layout_s {
	vattr_s *area;
	uintptr_t size;
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
	hashmap_t layout;   // name => miiskira_graph_layout_s
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

struct miiskira_graph_s* inner_miiskira_graph_register_posky(struct miiskira_graph_s *restrict graph, uintptr_t queue_size);

// graph.render.c

hashmap_t* inner_miiskira_graph_initial_render_parser(hashmap_t *restrict parser);

// inner.parser.c

struct miiskira_graph_s* inner_miiskira_graph_parse_pocket(struct miiskira_graph_s *restrict r, const hashmap_t *restrict parser, struct pocket_s *restrict pocket);

#endif
