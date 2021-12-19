#include "log.pri.h"

void miiskira_log_clear(void)
{
	inner_miiskira_log_clear(inst);
}

void miiskira_log_dump(mlog_s *mlog, uintptr_t level_limit)
{
	inner_miiskira_log_dump(inst, mlog, level_limit);
}
