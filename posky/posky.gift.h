#ifndef _miiskira_posky_gift_h_
#define _miiskira_posky_gift_h_

#include "posky.pri.h"

const posky_candy_s* inner_miiskira_posky_gift__posky_task(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_task_s *restrict task);
const posky_candy_s* inner_miiskira_posky_gift__posky_return(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_return_s *restrict ret);
const posky_candy_s* inner_miiskira_posky_gift__posky_clear_lonely(struct miiskira_posky_lady_s *restrict lady, const posky_candy_s *restrict candy);
const posky_candy_s* inner_miiskira_posky_gift__posky_proxy_adorable(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_proxy_s *restrict candy);

#endif
