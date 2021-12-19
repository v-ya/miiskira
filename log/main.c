#include "log.pri.h"

struct miiskira_log_s *inst;

mlog_s *miiskira$log$error;
mlog_s *miiskira$log$warning;
mlog_s *miiskira$log$info;
mlog_s *miiskira$log$verbose;

void miiskira_log_finally(void)
{
	if (miiskira$log$verbose)
	{
		refer_free(miiskira$log$verbose);
		miiskira$log$verbose = NULL;
	}
	if (miiskira$log$info)
	{
		refer_free(miiskira$log$info);
		miiskira$log$info = NULL;
	}
	if (miiskira$log$warning)
	{
		refer_free(miiskira$log$warning);
		miiskira$log$warning = NULL;
	}
	if (miiskira$log$error)
	{
		refer_free(miiskira$log$error);
		miiskira$log$error = NULL;
	}
	if (inst)
	{
		refer_free(inst);
		inst = NULL;
	}
}

const char* miiskira_log_initial(uintptr_t argc, const char *const argv[])
{
	static const char name_error[]   = "\e[31m" "error  " "\e[0m";
	static const char name_warning[] = "\e[33m" "warning" "\e[0m";
	static const char name_info[]    = "\e[34m" "info   " "\e[0m";
	static const char name_verbose[] = "\e[37m" "verbose" "\e[0m";
	if (
		(inst = inner_miiskira_log_alloc()) &&
		(miiskira$log$error = inner_miiskira_log_create_bypass(inst, name_error, 0)) &&
		(miiskira$log$warning = inner_miiskira_log_create_bypass(inst, name_warning, 1)) &&
		(miiskira$log$info = inner_miiskira_log_create_bypass(inst, name_info, 2)) &&
		(miiskira$log$verbose = inner_miiskira_log_create_bypass(inst, name_verbose, 3))
	) return NULL;
	miiskira_log_finally();
	return "miiskira.log.initial";
}
