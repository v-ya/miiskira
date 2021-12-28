#include "graph.pri.h"
#include <pocket/pocket.h>

struct miiskira_graph_s* inner_miiskira_graph_parse_pocket(struct miiskira_graph_s *restrict r, const hashmap_t *restrict parser, struct pocket_s *restrict pocket)
{
	const pocket_attr_t *restrict a;
	uint64_t n;
	miiskira_graph_parse_f func;
	if ((a = pocket_user(pocket)))
	{
		if (!pocket_is_tag(pocket, a, pocket_tag$index, NULL))
			goto label_fail;
		n = a->size;
		a = (const pocket_attr_t *) a->data.ptr;
		while (n)
		{
			--n;
			if (!a->name.string)
				goto label_fail;
			if (!(func = (miiskira_graph_parse_f) hashmap_get_name(parser, a->name.string)))
				goto label_fail;
			if (!func(r, pocket, a))
				goto label_fail;
			++a;
		}
	}
	return r;
	label_fail:
	log_warning("[graph] load pocket[%s] fail", a->name.string?a->name.string:"");
	return NULL;
}
