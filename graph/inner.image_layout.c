#include "graph.pri.h"

hashmap_t* inner_miiskira_graph_initial_image_layout_type(hashmap_t *restrict image_layout_type)
{
	if (
		#define d_type(_id)  hashmap_set_name(image_layout_type, #_id, (void *) (uintptr_t) graph_image_layout_##_id, NULL)
		d_type(undefined) &&
		d_type(general) &&
		d_type(color_attachment_optimal) &&
		d_type(depth_stencil_attachment_optimal) &&
		d_type(depth_stencil_read_only_optimal) &&
		d_type(shader_read_only_optimal) &&
		d_type(transfer_src_optimal) &&
		d_type(transfer_dst_optimal) &&
		d_type(preinitialized) &&
		d_type(khr_present_src)
		#undef d_type
	) return image_layout_type;
	return NULL;
}
