#ifndef _miiskira_graph_pri_h_
#define _miiskira_graph_pri_h_

#include "graph.h"
#include <posky.h>
#include <yaw.h>
#include <hashmap.h>
#include <graph/graph.h>
#include <graph/allocator.h>
#include <graph/support.h>
#include <graph/device.h>
#include <graph/image.h>
#include <graph/layout.h>
#include <graph/shader.h>
#include <graph/command.h>
#include <graph/buffer.h>

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
	hashmap_t present;  // name => miiskira_graph_present_s
};

void inner_miiskira_graph_hashmap_free_func(hashmap_vlist_t *restrict vl);

// debug_level:
//     0 none
//     1 error
//     2 warning
//     3 info
//     4 verbose
//     5 dump api
struct miiskira_graph_s* inner_miiskira_graph_alloc(mlog_s *ml, uint32_t debug_level);

struct miiskira_graph_s* inner_miiskira_graph_register_posky(struct miiskira_graph_s *restrict graph, uintptr_t queue_size);

#endif
