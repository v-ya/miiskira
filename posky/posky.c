#include "posky.pri.h"
#include "posky.gift.h"

// get name

const char* inner_miiskira_posky_get_name(posky_s *restrict posky, uint64_t address)
{
	const char *restrict name;
	if (address & 0xffffffff00000000ul)
		// adorable
		name = posky_name_adorable(posky, address);
	else
		// group
		name = posky_name_group(posky, address);
	return name?name:"?";
}

// feeding

void miiskira_posky_feeding(posky_candy_s *restrict candy)
{
	posky_feeding(p->posky, candy);
}

// proxy adorable

static void inner_miiskira_candy__posky_proxy_free_func(struct miiskira_candy__posky_proxy_s *restrict r)
{
	if (r->gift) refer_free(r->gift);
}

void miiskira_posky_feeding_proxy_adorable(posky_candy_s *restrict candy, const char *restrict name)
{
	struct miiskira_candy__posky_proxy_s *restrict r;
	if ((r = (struct miiskira_candy__posky_proxy_s *) refer_alloz(sizeof(struct miiskira_candy__posky_proxy_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_candy__posky_proxy_free_func);
		r->candy.target = p->address;
		r->candy.type = miiskira$type$posky_proxy_adorable;
		r->candy.stamp = miiskira$posky$stamp;
		r->name = name;
		r->gift = (posky_candy_s *) refer_save(candy);
		posky_feeding(p->posky, &r->candy);
		refer_free(r);
	}
	else log_warning("[feeding] posky_proxy_adorable fail");
}

// clear lonely

void miiskira_posky_feeding_clear_lonely(void)
{
	posky_candy_s *restrict r;
	if ((r = (posky_candy_s *) refer_alloz(sizeof(posky_candy_s))))
	{
		r->target = p->address;
		r->type = miiskira$type$posky_clear_lonely;
		r->stamp = miiskira$posky$stamp;
		posky_feeding(p->posky, r);
		refer_free(r);
	}
	else log_warning("[feeding] posky_clear_lonely fail");
}

// trick

static void inner_miiskira_candy__posky_trick_free_func(struct miiskira_candy__posky_trick_s *restrict r)
{
	if (r->lady) refer_free(r->lady);
}

void miiskira_posky_feeding_trick(miiskira_posky_trick_f trick, refer_t lady, uint64_t report)
{
	struct miiskira_candy__posky_trick_s *restrict r;
	if ((r = (struct miiskira_candy__posky_trick_s *) refer_alloz(sizeof(struct miiskira_candy__posky_trick_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_candy__posky_trick_free_func);
		r->candy.target = p->address;
		r->candy.report = report;
		r->candy.type = miiskira$type$posky_trick;
		r->candy.stamp = miiskira$posky$stamp;
		r->trick = trick;
		r->lady = refer_save(lady);
		posky_feeding(p->posky, &r->candy);
		refer_free(r);
	}
	else log_warning("[feeding] posky_trick fail");
}

// foster care

static void inner_miiskira_candy__posky_foster_care_free_func(struct miiskira_candy__posky_foster_care_s *restrict r)
{
	if (r->adorable) refer_free(r->adorable);
	if (r->signal) refer_free(r->signal);
	if (r->lady) refer_free(r->lady);
}

void miiskira_posky_feeding_foster_care(posky_adorable_s *adorable, yaw_signal_s *signal, miiskira_posky_treat_f treat, refer_t lady, uint64_t report)
{
	struct miiskira_candy__posky_foster_care_s *restrict r;
	if ((r = (struct miiskira_candy__posky_foster_care_s *) refer_alloz(sizeof(struct miiskira_candy__posky_foster_care_s))))
	{
		refer_set_free(r, (refer_free_f) inner_miiskira_candy__posky_foster_care_free_func);
		r->candy.target = p->address;
		r->candy.report = report;
		r->candy.type = miiskira$type$posky_foster_care;
		r->candy.stamp = miiskira$posky$stamp;
		r->adorable = (posky_adorable_s *) refer_save(adorable);
		r->signal = (yaw_signal_s *) refer_save(signal);
		r->treat = treat;
		r->lady = refer_save(lady);
		posky_feeding(p->posky, &r->candy);
		refer_free(r);
	}
	else log_warning("[feeding] posky_foster_care fail");
}
