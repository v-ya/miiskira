#include "graph.pri.h"

enum miiskira_graph_layout_type_t {
	// float[1, 2, 3, 4]
	miiskira_graph_layout_type__float,
	miiskira_graph_layout_type__vec2,
	miiskira_graph_layout_type__vec3,
	miiskira_graph_layout_type__vec4,
	// double[1, 2, 3, 4]
	miiskira_graph_layout_type__double,
	miiskira_graph_layout_type__dvec2,
	miiskira_graph_layout_type__dvec3,
	miiskira_graph_layout_type__dvec4,
	// int[1, 2, 3, 4]
	miiskira_graph_layout_type__int,
	miiskira_graph_layout_type__ivec2,
	miiskira_graph_layout_type__ivec3,
	miiskira_graph_layout_type__ivec4,
	// uint[1, 2, 3, 4]
	miiskira_graph_layout_type__uint,
	miiskira_graph_layout_type__uvec2,
	miiskira_graph_layout_type__uvec3,
	miiskira_graph_layout_type__uvec4,
	// float[2x2, 3x3, 4x4]
	miiskira_graph_layout_type__mat2,
	miiskira_graph_layout_type__mat3,
	miiskira_graph_layout_type__mat4,
	// double[2x2, 3x3, 4x4]
	miiskira_graph_layout_type__dmat2,
	miiskira_graph_layout_type__dmat3,
	miiskira_graph_layout_type__dmat4,
	// number
	miiskira_graph_layout_type_number
};

static inline uintptr_t inner_miiskira_graph_layout_type_size(enum miiskira_graph_layout_type_t type)
{
	static const uintptr_t t2s[miiskira_graph_layout_type_number] = {
		[miiskira_graph_layout_type__float]  = sizeof(float),
		[miiskira_graph_layout_type__vec2]   = sizeof(float) * 2,
		[miiskira_graph_layout_type__vec3]   = sizeof(float) * 3,
		[miiskira_graph_layout_type__vec4]   = sizeof(float) * 4,
		[miiskira_graph_layout_type__double] = sizeof(double),
		[miiskira_graph_layout_type__dvec2]  = sizeof(double) * 2,
		[miiskira_graph_layout_type__dvec3]  = sizeof(double) * 3,
		[miiskira_graph_layout_type__dvec4]  = sizeof(double) * 4,
		[miiskira_graph_layout_type__int]    = sizeof(int32_t),
		[miiskira_graph_layout_type__ivec2]  = sizeof(int32_t) * 2,
		[miiskira_graph_layout_type__ivec3]  = sizeof(int32_t) * 3,
		[miiskira_graph_layout_type__ivec4]  = sizeof(int32_t) * 4,
		[miiskira_graph_layout_type__uint]   = sizeof(uint32_t),
		[miiskira_graph_layout_type__uvec2]  = sizeof(uint32_t) * 2,
		[miiskira_graph_layout_type__uvec3]  = sizeof(uint32_t) * 3,
		[miiskira_graph_layout_type__uvec4]  = sizeof(uint32_t) * 4,
		[miiskira_graph_layout_type__mat2]   = sizeof(float) * 4,
		[miiskira_graph_layout_type__mat3]   = sizeof(float) * 9,
		[miiskira_graph_layout_type__mat4]   = sizeof(float) * 16,
		[miiskira_graph_layout_type__dmat2]  = sizeof(double) * 4,
		[miiskira_graph_layout_type__dmat3]  = sizeof(double) * 9,
		[miiskira_graph_layout_type__dmat4]  = sizeof(double) * 16,
	};
	if ((uint32_t) type < miiskira_graph_layout_type_number)
		return t2s[type];
	return 0;
}

hashmap_t* inner_miiskira_graph_initial_layout_type(hashmap_t *restrict layout_type)
{
	if (
		#define d_type(_id)  hashmap_set_name(layout_type, #_id, (void *) (uintptr_t) miiskira_graph_layout_type__##_id, NULL)
		d_type(float) &&
		d_type(vec2) &&
		d_type(vec3) &&
		d_type(vec4) &&
		d_type(double) &&
		d_type(dvec2) &&
		d_type(dvec3) &&
		d_type(dvec4) &&
		d_type(int) &&
		d_type(ivec2) &&
		d_type(ivec3) &&
		d_type(ivec4) &&
		d_type(uint) &&
		d_type(uvec2) &&
		d_type(uvec3) &&
		d_type(uvec4) &&
		d_type(mat2) &&
		d_type(mat3) &&
		d_type(mat4) &&
		d_type(dmat2) &&
		d_type(dmat3) &&
		d_type(dmat4)
		#undef d_type
	) return layout_type;
	return NULL;
}

static void inner_miiskira_graph_layout_free_func(struct miiskira_graph_layout_s *restrict r)
{
	if (r->area) refer_free(r->area);
	exbuffer_uini(&r->default_data);
}

struct miiskira_graph_layout_s* inner_miiskira_graph_layout_alloc(void)
{
	struct miiskira_graph_layout_s *restrict r;
	if ((r = (struct miiskira_graph_layout_s *) refer_alloz(sizeof(struct miiskira_graph_layout_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_graph_layout_free_func);
		if (exbuffer_init(&r->default_data, 64) &&
			(r->area = vattr_alloc()))
			return r;
		refer_free(r);
	}
	return NULL;
}

static inline uintptr_t inner_miiskira_graph_layout_check_type_size(uintptr_t size, enum miiskira_graph_layout_type_t type)
{
	uintptr_t type_size;
	if ((type_size = inner_miiskira_graph_layout_type_size(type)) && !(size % type_size))
		return type_size;
	return 0;
}

struct miiskira_graph_layout_s* inner_miiskira_graph_layout_append(struct miiskira_graph_layout_s *restrict layout, const hashmap_t *restrict layout_type, const char *restrict name, const char *restrict type, const void *data, uintptr_t size)
{
	hashmap_vlist_t *restrict vl;
	uintptr_t offset, type_size;
	offset = layout->default_data.used;
	if (type && (vl = hashmap_find_name(layout_type, type)) &&
		(type_size = inner_miiskira_graph_layout_check_type_size(size,
			(enum miiskira_graph_layout_type_t) (uintptr_t) vl->value)) &&
		exbuffer_append(&layout->default_data, data, size))
	{
		layout->size += size;
		if (name)
		{
			struct miiskira_graph_range_s *restrict range;
			vattr_vlist_t *restrict vlist;
			if (!(range = (struct miiskira_graph_range_s *) refer_alloc(sizeof(struct miiskira_graph_range_s))))
				goto label_fail;
			range->offset = offset;
			range->size = size;
			range->type = (uintptr_t) vl->value;
			range->type_size = type_size;
			vlist = vattr_insert_tail(layout->area, name, range);
			refer_free(range);
			if (!vlist) goto label_fail;
		}
		return layout;
	}
	label_fail:
	return NULL;
}
