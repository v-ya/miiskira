#include "posky.gift.h"

const posky_candy_s* inner_miiskira_posky_gift__posky_trick(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_trick_s *restrict candy)
{
	posky_s *restrict posky;
	posky = lady->p->posky;
	if (candy->trick)
	{
		candy->trick(posky, candy->lady);
		return &candy->candy;
	}
	log_warning("[posky] posky_trick fail by (%s:%016lx)", inner_miiskira_posky_get_name(posky, candy->candy.report), candy->candy.report);
	return NULL;
}
