#ifndef _miiskira_posky_h_
#define _miiskira_posky_h_

#include <posky.h>
#include <yaw.h>

typedef void (*miiskira_posky_trick_f)(posky_s *restrict posky, refer_t lady);
typedef void (*miiskira_posky_treat_f)(posky_adorable_s *restrict adorable, refer_t lady);

extern uint64_t miiskira$posky$stamp;

uint32_t miiskira_posky_gen_type(void);

void miiskira_posky_feeding(posky_candy_s *restrict candy);
void miiskira_posky_feeding_proxy_adorable(posky_candy_s *restrict candy, const char *restrict name);
void miiskira_posky_feeding_clear_lonely(void);
void miiskira_posky_feeding_trick(miiskira_posky_trick_f trick, refer_t lady, uint64_t report);
void miiskira_posky_feeding_foster_care(posky_adorable_s *adorable, yaw_signal_s *signal, miiskira_posky_treat_f treat, refer_t lady, uint64_t report);

#endif
