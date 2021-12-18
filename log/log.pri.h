#ifndef _miiskira_log_pri_h_
#define _miiskira_log_pri_h_

#include "log.h"
#include <exbuffer.h>
#include <yaw.h>

struct miiskira_log_s {
	exbuffer_t msg;
	exbuffer_t attr;
	yaw_lock_s *metux;
	uint64_t stamp_start;
};

struct miiskira_log_bypass_s {
	const char *name;
	struct miiskira_log_s *inst;
};

struct miiskira_log_attr_t {
	const char *name;
	uintptr_t msg_pos;
	uint64_t stamp_by_start;
};

extern struct miiskira_log_s *inst;

struct miiskira_log_s* inner_miiskira_log_alloc(void);
struct mlog_s* inner_miiskira_log_create_bypass(struct miiskira_log_s *restrict r, const char *restrict name);
void inner_miiskira_log_clear(struct miiskira_log_s *restrict r);
void inner_miiskira_log_dump(struct miiskira_log_s *restrict r, mlog_s *restrict ml);

#endif
