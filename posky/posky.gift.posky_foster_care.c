#include "posky.gift.h"

struct miiskira_posky_adorable_s {
	yaw_s *yaw;
	posky_adorable_s *adorable;
	yaw_signal_s *signal;
	miiskira_posky_treat_f treat;
	refer_t lady;
	uint64_t address;
};

static void inner_miiskira_posky_adorable_free_func(struct miiskira_posky_adorable_s *restrict r)
{
	if (r->yaw)
	{
		yaw_stop(r->yaw);
		yaw_signal_inc(r->signal);
		yaw_signal_wake(r->signal, ~0);
		yaw_wait(r->yaw);
		refer_free(r->yaw);
	}
	if (r->signal) refer_free(r->signal);
	if (r->adorable) refer_free(r->adorable);
	if (r->lady) refer_free(r->lady);
}

static void inner_miiskira_posky_adorable_thread(yaw_s *restrict yaw)
{
	struct miiskira_posky_adorable_s *restrict r;
	miiskira_posky_treat_f treat;
	uint32_t status;
	r = (struct miiskira_posky_adorable_s *) yaw->data;
	treat = r->treat;
	while (yaw->running)
	{
		status = yaw_signal_get(r->signal);
		posky_adorable_clear_gift(r->adorable);
		if (treat) treat(r->adorable, r->lady);
		yaw_signal_wait(r->signal, status);
	}
}

static struct miiskira_posky_adorable_s* inner_miiskira_posky_adorable_alloc(const struct miiskira_candy__posky_foster_care_s *restrict candy)
{
	struct miiskira_posky_adorable_s *restrict r;
	if ((r = (struct miiskira_posky_adorable_s *) refer_alloz(sizeof(struct miiskira_posky_adorable_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_posky_adorable_free_func);
		r->adorable = (posky_adorable_s *) refer_save(candy->adorable);
		r->signal = (yaw_signal_s *) refer_save(candy->signal);
		r->treat = candy->treat;
		r->lady = refer_save(candy->lady);
		r->address = posky_adorable_address(r->adorable);
		if ((r->yaw = yaw_alloc_and_start(inner_miiskira_posky_adorable_thread, NULL, r)))
			return r;
		refer_free(r);
	}
	return NULL;
}

const posky_candy_s* inner_miiskira_posky_gift__posky_foster_care(struct miiskira_posky_lady_s *restrict lady, const struct miiskira_candy__posky_foster_care_s *restrict candy)
{
	struct miiskira_posky_s *restrict pri;
	struct miiskira_posky_adorable_s *restrict value;
	const char *restrict name;
	pri = lady->p;
	if (candy->adorable && candy->signal)
	{
		if ((name = posky_name_adorable(pri->posky, posky_adorable_address(candy->adorable))))
		{
			if ((value = inner_miiskira_posky_adorable_alloc(candy)))
			{
				if (vattr_set(pri->pool, name, value))
					log_info("[posky] posky_foster_care adorable (%s:%016lx)", name, value->address);
				else log_warning("[posky] posky_foster_care join (%s) fail", name);
				refer_free(value);
			}
			else log_warning("[posky] posky_foster_care create (%s) fail", name);
		}
		else log_warning("[posky] posky_foster_care don't allow anonymous by (%s:%016lx)", inner_miiskira_posky_get_name(pri->posky, candy->candy.report), candy->candy.report);
	}
	else log_warning("[posky] posky_foster_care fail by (%s:%016lx)", inner_miiskira_posky_get_name(pri->posky, candy->candy.report), candy->candy.report);
	return &candy->candy;
}
