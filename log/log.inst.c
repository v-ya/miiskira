#include "log.pri.h"

static void inner_miiskira_log_free_func(struct miiskira_log_s *restrict r)
{
	if (r->metux) refer_free(r->metux);
	exbuffer_uini(&r->attr);
	exbuffer_uini(&r->msg);
}

struct miiskira_log_s* inner_miiskira_log_alloc(void)
{
	struct miiskira_log_s *restrict r;
	if ((r = (struct miiskira_log_s *) refer_alloz(sizeof(struct miiskira_log_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_log_free_func);
		if (exbuffer_init(&r->msg, 0) &&
			exbuffer_init(&r->attr, 0) &&
			(r->metux = yaw_lock_alloc_mutex()))
		{
			r->stamp_start = yaw_timestamp_msec();
			return r;
		}
		refer_free(r);
	}
	return NULL;
}

static struct miiskira_log_s* inner_miiskira_log_append(struct miiskira_log_bypass_s *restrict r, char *restrict msg, uintptr_t length)
{
	struct miiskira_log_s *restrict inst;
	struct miiskira_log_attr_t attr;
	inst = r->inst;
	attr.name = r->name;
	attr.msg_pos = inst->msg.used;
	attr.stamp_by_start = yaw_timestamp_msec() - inst->stamp_start;
	if (length && msg[length - 1] == '\n')
		msg[--length] = 0;
	if (exbuffer_append(&inst->msg, msg, length + 1))
	{
		if (exbuffer_append(&inst->attr, &attr, sizeof(attr)))
			return inst;
		inst->msg.used = attr.msg_pos;
	}
	return NULL;
}

static void inner_miiskira_log_bypass_free_func(struct miiskira_log_bypass_s *restrict r)
{
	if (r->inst) refer_free(r->inst);
}

static int inner_miiskira_log_bypass_report_func(char *restrict msg, uintptr_t length, struct miiskira_log_bypass_s *restrict bp)
{
	inner_miiskira_log_append(bp, msg, length);
	return 1;
}

struct mlog_s* inner_miiskira_log_create_bypass(struct miiskira_log_s *restrict r, const char *restrict name)
{
	struct miiskira_log_bypass_s *restrict bp;
	mlog_s *restrict ml;
	ml = NULL;
	if ((bp = (struct miiskira_log_bypass_s *) refer_alloz(sizeof(struct miiskira_log_bypass_s))))
	{
		refer_set_free(bp, (refer_free_f) inner_miiskira_log_bypass_free_func);
		bp->name = name;
		bp->inst = (struct miiskira_log_s *) refer_save(r);
		if ((ml = mlog_alloc(0)))
		{
			mlog_set_locker(ml, r->metux);
			mlog_set_report(ml, (mlog_report_f) inner_miiskira_log_bypass_report_func, bp);
		}
		refer_free(bp);
	}
	return ml;
}

void inner_miiskira_log_clear(struct miiskira_log_s *restrict r)
{
	yaw_lock_lock(r->metux);
	r->attr.used = 0;
	r->msg.used = 0;
	yaw_lock_unlock(r->metux);
}

#include <inttypes.h>

void inner_miiskira_log_dump(struct miiskira_log_s *restrict r, mlog_s *restrict ml)
{
	if (ml && ml->locker != r->metux)
	{
		const char *restrict msg;
		const struct miiskira_log_attr_t *restrict attr;
		uintptr_t i, n;
		yaw_lock_lock(r->metux);
		msg = (const char *) r->msg.data;
		attr = (const struct miiskira_log_attr_t *) r->attr.data;
		n = r->attr.used / sizeof(struct miiskira_log_attr_t);
		for (i = 0; i < n; ++i)
			mlog_printf(ml, "[%8.3f] %s %s\n", attr[i].stamp_by_start / 1000.0, attr[i].name, msg + attr[i].msg_pos);
		yaw_lock_unlock(r->metux);
	}
}
