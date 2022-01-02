#ifndef _miiskira_posky_gift_h_
#define _miiskira_posky_gift_h_

#include "posky.pri.h"

struct miiskira_candy__posky_proxy_s {
	posky_candy_s candy;
	const char *name;
	posky_candy_s *gift;
};

struct miiskira_candy__posky_trick_s {
	posky_candy_s candy;
	miiskira_posky_trick_f trick;
	refer_t lady;
};

struct miiskira_candy__posky_foster_care_s {
	posky_candy_s candy;
	posky_adorable_s *adorable;
	yaw_signal_s *signal;
	miiskira_posky_treat_f treat;
	refer_t lady;
};

const posky_candy_s* inner_miiskira_posky_gift__posky_proxy_adorable(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_proxy_s *restrict candy);
const posky_candy_s* inner_miiskira_posky_gift__posky_clear_lonely(struct miiskira_posky_lady_s *restrict lady, const posky_candy_s *restrict candy);
const posky_candy_s* inner_miiskira_posky_gift__posky_trick(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_trick_s *restrict candy);
const posky_candy_s* inner_miiskira_posky_gift__posky_foster_care(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_foster_care_s *restrict candy);

#endif
