#include "graph.tag.h"
#include "graph.posky.h"
#include "../posky/posky.h"
#include "../define.h"

static void inner_graph_candy_tag_free_func(struct miiskira_candy__graph_pocket_s *restrict r)
{
	if (r->pocket)
		refer_free(r->pocket);
}

static struct pocket_s* miiskira_graph_tag_posky_feeding(struct pocket_s *restrict pocket, uint32_t type)
{
	struct miiskira_candy__graph_pocket_s *restrict r;
	if ((r = (struct miiskira_candy__graph_pocket_s *) refer_alloz(sizeof(struct miiskira_candy__graph_pocket_s))))
	{
		refer_set_free(r, (refer_free_f) inner_graph_candy_tag_free_func);
		r->candy.type = type;
		r->pocket = (struct pocket_s *) refer_save(pocket);
		miiskira_posky_feeding_proxy_adorable(&r->candy, miiskira$posky$graph);
		refer_free(r);
		return pocket;
	}
	return NULL;
}

static struct pocket_s* miiskira_graph_tag_render(struct pocket_s *restrict pocket)
{
	return miiskira_graph_tag_posky_feeding(pocket, miiskira$type$graph_tag_render);
}

static struct pocket_s* miiskira_graph_tag_model(struct pocket_s *restrict pocket)
{
	return miiskira_graph_tag_posky_feeding(pocket, miiskira$type$graph_tag_model);
}

static const char tag_render[] = "miiskira.render";
static const char tag_model[] = "miiskira.model";

hashmap_t* miiskira_graph_initial_tag(void)
{
	extern hashmap_t *$tag;
	hashmap_vlist_t *restrict vl;
	void *value;
	if ((vl = hashmap_put_name($tag, tag_render, value = miiskira_graph_tag_render, NULL)) && vl->value == value)
	{
		if ((vl = hashmap_put_name($tag, tag_model, value = miiskira_graph_tag_model, NULL)) && vl->value == value)
			return $tag;
		hashmap_delete_name($tag, tag_render);
	}
	return NULL;
}

void miiskira_graph_finally_tag(void)
{
	extern hashmap_t *$tag;
	hashmap_delete_name($tag, tag_model);
	hashmap_delete_name($tag, tag_render);
}
