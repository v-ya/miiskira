#include <console.h>
#include <mlog.h>
#include <stdlib.h>
#include "../log/log.h"

static console_s* log_main(console_command_s *restrict cc, mlog_s *mlog, uintptr_t argc, const char *const argv[])
{
	uintptr_t level_limit = ~(uintptr_t) 0;
	if (argc > 1)
		level_limit = (uintptr_t) strtoul(argv[1], NULL, 10);
	miiskira_log_dump(mlog, level_limit);
	return NULL;
}

static void log_help(console_command_s *restrict cc, mlog_s *mlog, uintptr_t argc, const char *const argv[])
{
	mlog_printf(mlog, "%s [level-limit]\n", argv[0]);
	mlog_printf(mlog, "    level-limit:\n");
	mlog_printf(mlog, "        0: error\n");
	mlog_printf(mlog, "        1: warning\n");
	mlog_printf(mlog, "        2: info\n");
	mlog_printf(mlog, "        3: verbose\n");
}

console_command_s* cmd_alloc_log(void)
{
	console_command_s *restrict r;
	r = (console_command_s *) refer_alloz(sizeof(console_command_s));
	if (r)
	{
		r->main = log_main;
		r->help = log_help;
		r->name = "miiskira.console.log";
		r->desc = "显示 miiskira 的日志信息";
	}
	return r;
}

static console_s* log_clear_main(console_command_s *restrict cc, mlog_s *mlog, uintptr_t argc, const char *const argv[])
{
	miiskira_log_clear();
	return NULL;
}

static void log_clear_help(console_command_s *restrict cc, mlog_s *mlog, uintptr_t argc, const char *const argv[])
{
	mlog_printf(mlog, "%s\n", argv[0]);
	mlog_printf(mlog, "    level-limit:\n");
	mlog_printf(mlog, "        0: error\n");
	mlog_printf(mlog, "        1: warning\n");
	mlog_printf(mlog, "        2: info\n");
	mlog_printf(mlog, "        3: verbose\n");
}

console_command_s* cmd_alloc_log_clear(void)
{
	console_command_s *restrict r;
	r = (console_command_s *) refer_alloz(sizeof(console_command_s));
	if (r)
	{
		r->main = log_clear_main;
		r->help = log_clear_help;
		r->name = "miiskira.console.log.clear";
		r->desc = "清除 miiskira 的日志信息";
	}
	return r;
}
