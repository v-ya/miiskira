#ifndef _miiskira_log_h_
#define _miiskira_log_h_

#include <mlog.h>
#include "../flag.h"

#if (miiskira_log_level >= 1)
#define log_error(_format, ...) mlog_printf(miiskira$log$error, _format, ##__VA_ARGS__)
#else
#define log_error(_format, ...) (void) 0
#endif

#if (miiskira_log_level >= 2)
#define log_warning(_format, ...) mlog_printf(miiskira$log$warning, _format, ##__VA_ARGS__)
#else
#define log_warning(_format, ...) (void) 0
#endif

#if (miiskira_log_level >= 3)
#define log_info(_format, ...) mlog_printf(miiskira$log$info, _format, ##__VA_ARGS__)
#else
#define log_info(_format, ...) (void) 0
#endif

#if (miiskira_log_level >= 4)
#define log_verbose(_format, ...) mlog_printf(miiskira$log$verbose, _format, ##__VA_ARGS__)
#else
#define log_verbose(_format, ...) (void) 0
#endif

extern mlog_s *miiskira$log$error;
extern mlog_s *miiskira$log$warning;
extern mlog_s *miiskira$log$info;
extern mlog_s *miiskira$log$verbose;

// 清除所有级别的日志
void miiskira_log_clear(void);

// 打印日志
// level_limit:
//     0 : error
//     1 : error warning
//     2 : error warning info
//     3+: error warning info verbose
void miiskira_log_dump(mlog_s *mlog, uintptr_t level_limit);

#endif
