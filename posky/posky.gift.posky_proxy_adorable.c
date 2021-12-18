#include "posky.gift.h"

const posky_candy_s* inner_miiskira_posky_gift__posky_proxy_adorable(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_proxy_s *restrict candy)
{
	posky_adorable_s *restrict adorable;
	if (candy->name && candy->gift && (adorable = posky_find_adorable(lady->p->posky, candy->name, 0)))
	{
		if (posky_adorable_surprise(adorable, candy->gift))
		{
			log_verbose("[posky] posky_proxy_adorable okay (%s:%016lx) type: %u", candy->name, posky_adorable_address(adorable), candy->gift->type);
			return &candy->candy;
		}
	}
	log_warning("[posky] posky_proxy_adorable fail (%s)", candy->name?candy->name:"");
	return NULL;
}
