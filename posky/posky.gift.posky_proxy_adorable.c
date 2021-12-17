#include "posky.gift.h"

const posky_candy_s* inner_miiskira_posky_gift__posky_proxy_adorable(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_proxy_s *restrict candy)
{
	posky_adorable_s *restrict adorable;
	if (candy->name && candy->gift && (adorable = posky_find_adorable(lady->p->posky, candy->name, 0)))
	{
		if (posky_adorable_surprise(adorable, candy->gift))
			return &candy->candy;
	}
	return NULL;
}
