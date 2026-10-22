#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "refs.h"
#ضمن "object-name.h"
#ضمن "pager.h"
#ضمن "parse-options.h"
#ضمن "hash-lookup.h"
#ضمن "commit-slab.h"
#ضمن "commit-graph.h"
#ضمن "wildmatch.h"
#ضمن "mem-pool.h"
#ضمن "pretty.h"
#ضمن "revision.h"
#ضمن "notes.h"
#ضمن "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#تعریف_کرو CUTOFF_DATE_SLOP 86400

بنية rev_name {
	ثابت حرف *tip_name;
	timestamp_t taggerdate;
	صحيح generation;
	صحيح فاصلہ;
	صحيح from_ٹیگ;
};

define_commit_slab(commit_rev_name, بنية rev_name);

ساكن timestamp_t generation_cutoff = GENERATION_نمبر_INFINITY;
ساكن timestamp_t cutoff = ویلا_MAX;
ساكن بنية commit_rev_name rev_ناں;

/* Disable the cutoff checks entirely */
ساكن فراغ disable_cutoff(فراغ)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ساكن فراغ سیٹ_commit_cutoff(بنية commit *commit)
{

	إذا (cutoff > commit->date)
		cutoff = commit->date;

	إذا (generation_cutoff) {
		timestamp_t generation = commit_گراف_generation(commit);

		إذا (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
ساكن فراغ adjust_cutoff_timestamp_for_slop(فراغ)
{
	إذا (cutoff) {
		/* check for underflow */
		إذا (cutoff > ویلا_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		وإلا
			cutoff = ویلا_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ساكن صحيح commit_is_before_cutoff(بنية commit *commit)
{
	إذا (generation_cutoff < GENERATION_نمبر_INFINITY)
		أرجع generation_cutoff &&
			commit_گراف_generation(commit) < generation_cutoff;

	أرجع commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#تعریف_کرو ملاؤ_TRAVERSAL_بھار 65535

ساكن صحيح is_valid_rev_name(ثابت بنية rev_name *name)
{
	أرجع name && name->tip_name;
}

ساكن بنية rev_name *get_commit_rev_name(ثابت بنية commit *commit)
{
	بنية rev_name *name = commit_rev_name_peek(&rev_ناں, commit);

	أرجع is_valid_rev_name(name) ? name : عدم;
}

ساكن صحيح effective_فاصلہ(صحيح فاصلہ, صحيح generation)
{
	أرجع فاصلہ + (generation > 0 ? ملاؤ_TRAVERSAL_بھار : 0);
}

ساكن صحيح is_better_name(بنية rev_name *name,
			  timestamp_t taggerdate,
			  صحيح generation,
			  صحيح فاصلہ,
			  صحيح from_ٹیگ)
{
	صحيح name_فاصلہ = effective_فاصلہ(name->فاصلہ, name->generation);
	صحيح new_فاصلہ = effective_فاصلہ(فاصلہ, generation);

	/* If both are tags, we prefer the nearer one. */
	إذا (from_ٹیگ && name->from_ٹیگ)
		أرجع name_فاصلہ > new_فاصلہ;

	/* Favor a tag over a non-tag. */
	إذا (name->from_ٹیگ != from_ٹیگ)
		أرجع from_ٹیگ;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	إذا (name_فاصلہ != new_فاصلہ)
		أرجع name_فاصلہ > new_فاصلہ;

	/* ... or tiebreak to favor older date */
	إذا (name->taggerdate != taggerdate)
		أرجع name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	أرجع 0;
}

ساكن بنية rev_name *create_or_update_name(بنية commit *commit,
					      timestamp_t taggerdate,
					      صحيح generation, صحيح فاصلہ,
					      صحيح from_ٹیگ)
{
	بنية rev_name *name = commit_rev_name_at(&rev_ناں, commit);

	إذا (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, فاصلہ, from_ٹیگ))
		أرجع عدم;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->فاصلہ = فاصلہ;
	name->from_ٹیگ = from_ٹیگ;

	أرجع name;
}

ساكن حرف *get_ماپا_name(ثابت بنية rev_name *name, صحيح ماپا_نمبر,
			     بنية mem_pool *string_pool)
{
	ناپ_t len;

	strip_suffix(name->tip_name, "^0", &len);
	إذا (name->generation > 0) {
		أرجع mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (صحيح)len, name->tip_name,
				       name->generation, ماپا_نمبر);
	} وإلا {
		أرجع mem_pool_strfmt(string_pool, "%.*s^%d",
				       (صحيح)len, name->tip_name, ماپا_نمبر);
	}
}

ساكن فراغ name_rev(بنية commit *شروع_commit,
		ثابت حرف *tip_name, timestamp_t taggerdate,
		صحيح from_ٹیگ, صحيح deref, بنية mem_pool *string_pool)
{
	بنية commit_ڈھیر ڈھیر = COMMIT_ڈھیر_INIT;
	بنية commit *commit;
	بنية commit_ڈھیر parents_منزل_قطار = COMMIT_ڈھیر_INIT;
	بنية rev_name *شروع_name;

	repo_parse_commit(the_repository, شروع_commit);
	إذا (commit_is_before_cutoff(شروع_commit))
		أرجع;

	شروع_name = create_or_update_name(شروع_commit, taggerdate, 0, 0,
					   from_ٹیگ);
	إذا (!شروع_name)
		أرجع;
	إذا (deref)
		شروع_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	وإلا
		شروع_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ڈھیر_ڈالو(&ڈھیر, شروع_commit);

	طالما ((commit = commit_ڈھیر_نکالو(&ڈھیر))) {
		بنية rev_name *name = get_commit_rev_name(commit);
		بنية commit_list *parents;
		صحيح ماپا_نمبر = 1;

		parents_منزل_قطار.nr = 0;

		لكل (parents = commit->parents;
				parents;
				parents = parents->اگلا, ماپا_نمبر++) {
			بنية commit *ماپا = parents->شے;
			بنية rev_name *ماپا_name;
			صحيح generation, فاصلہ;

			repo_parse_commit(the_repository, ماپا);
			إذا (commit_is_before_cutoff(ماپا))
				استمر;

			إذا (ماپا_نمبر > 1) {
				generation = 0;
				فاصلہ = name->فاصلہ + ملاؤ_TRAVERSAL_بھار;
			} وإلا {
				generation = name->generation + 1;
				فاصلہ = name->فاصلہ + 1;
			}

			ماپا_name = create_or_update_name(ماپا, taggerdate,
							    generation,
							    فاصلہ, from_ٹیگ);
			إذا (ماپا_name) {
				إذا (ماپا_نمبر > 1)
					ماپا_name->tip_name =
						get_ماپا_name(name,
								ماپا_نمبر,
								string_pool);
				وإلا
					ماپا_name->tip_name = name->tip_name;
				commit_ڈھیر_ڈالو(&parents_منزل_قطار, ماپا);
			}
		}

		/* The first parent must come out first from the stack */
		طالما (parents_منزل_قطار.nr)
			commit_ڈھیر_ڈالو(&ڈھیر,
					  commit_ڈھیر_نکالو(&parents_منزل_قطار));
	}

	commit_ڈھیر_clear(&ڈھیر);
	commit_ڈھیر_clear(&parents_منزل_قطار);
}

ساكن صحيح subpath_matches(ثابت حرف *رستہ, ثابت حرف *filter)
{
	ثابت حرف *subpath = رستہ;

	طالما (subpath) {
		إذا (!wildmatch(filter, subpath, 0))
			أرجع subpath - رستہ;
		subpath = strchr(subpath, '/');
		إذا (subpath)
			subpath++;
	}
	أرجع -1;
}

بنية name_ref_انکڑے {
	صحيح ٹیگ_سوچی_only;
	صحيح name_only;
	بنية string_list ref_filters;
	بنية string_list exclude_filters;
};

بنية pretty_format {
	بنية pretty_print_context ctx;
	بنية userformat_want want;
};

تعداد command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

تعداد stdin_منوال {
    متن_2 = 1,
    REVS = 2,
};

بنية command {
	تعداد command_type type;
	اتحاد {
		صحيح name_only;
		بنية pretty_format *pretty_format;
	} u;
};

ساكن فراغ init_name_rev_command(بنية command *cmd,
				  صحيح name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

ساكن فراغ init_format_rev_command(بنية command *cmd,
				    بنية pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

ساكن بنية tip_جدول {
	بنية tip_جدول_اندراج {
		بنية object_id oid;
		ثابت حرف *refname;
		بنية commit *commit;
		timestamp_t taggerdate;
		بنانشانی صحيح from_ٹیگ:1;
		بنانشانی صحيح deref:1;
	} *جدول;
	صحيح nr;
	صحيح alloc;
	صحيح sorted;
} tip_جدول;

ساكن فراغ جوڑو_منزل_tip_جدول(ثابت بنية object_id *oid, ثابت حرف *refname,
			     صحيح shorten_unambiguous, بنية commit *commit,
			     timestamp_t taggerdate, صحيح from_ٹیگ, صحيح deref)
{
	حرف *short_refname = عدم;

	إذا (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	وإلا إذا (skip_اگیتر(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	وإلا
		skip_اگیتر(refname, "refs/", &refname);

	ALLOC_GROW(tip_جدول.جدول, tip_جدول.nr + 1, tip_جدول.alloc);
	oidcpy(&tip_جدول.جدول[tip_جدول.nr].oid, oid);
	tip_جدول.جدول[tip_جدول.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_جدول.جدول[tip_جدول.nr].commit = commit;
	tip_جدول.جدول[tip_جدول.nr].taggerdate = taggerdate;
	tip_جدول.جدول[tip_جدول.nr].from_ٹیگ = from_ٹیگ;
	tip_جدول.جدول[tip_جدول.nr].deref = deref;
	tip_جدول.nr++;
	tip_جدول.sorted = 0;
}

ساكن صحيح tipcmp(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية tip_جدول_اندراج *a = a_, *b = b_;
	أرجع oidcmp(&a->oid, &b->oid);
}

ساكن صحيح cmp_by_ٹیگ_and_age(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية tip_جدول_اندراج *a = a_, *b = b_;
	صحيح cmp;

	/* Prefer tags. */
	cmp = b->from_ٹیگ - a->from_ٹیگ;
	إذا (cmp)
		أرجع cmp;

	/* Older is better. */
	إذا (a->taggerdate < b->taggerdate)
		أرجع -1;
	أرجع a->taggerdate != b->taggerdate;
}

ساكن صحيح name_ref(ثابت بنية reference *ref, فراغ *cb_انکڑے)
{
	بنية object *o = parse_object(the_repository, ref->oid);
	بنية name_ref_انکڑے *انکڑے = cb_انکڑے;
	صحيح can_abbreviate_output = انکڑے->ٹیگ_سوچی_only && انکڑے->name_only;
	صحيح deref = 0;
	صحيح from_ٹیگ = 0;
	بنية commit *commit = عدم;
	timestamp_t taggerdate = ویلا_MAX;

	إذا (انکڑے->ٹیگ_سوچی_only && !starts_with(ref->name, "refs/tags/"))
		أرجع 0;

	إذا (انکڑے->exclude_filters.nr) {
		بنية string_list_شے *شے;

		for_each_string_list_شے(شے, &انکڑے->exclude_filters) {
			إذا (subpath_matches(ref->name, شے->string) >= 0)
				أرجع 0;
		}
	}

	إذا (انکڑے->ref_filters.nr) {
		بنية string_list_شے *شے;
		صحيح matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_شے(شے, &انکڑے->ref_filters) {
			/*
			 * Check all patterns even after finding a match, so
			 * that we can see if a match with a subpath exists.
			 * When a user asked for 'refs/tags/v*' and 'v1.*',
			 * both of which match, the user is showing her
			 * willingness to accept a shortened output by having
			 * the 'v1.*' in the acceptable refnames, so we
			 * shouldn't stop when seeing 'refs/tags/v1.4' matches
			 * 'refs/tags/v*'.  We should show it as 'v1.4'.
			 */
			بدل (subpath_matches(ref->name, شے->string)) {
			حالة -1: /* did not match */
				اكسر;
			حالة 0: /* matched fully */
				matched = 1;
				اكسر;
			افتراضي: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				اكسر;
			}
		}

		/* If none of the patterns matched, stop now */
		إذا (!matched)
			أرجع 0;
	}

	طالما (o && o->type == OBJ_ٹیگ) {
		بنية ٹیگ *t = (بنية ٹیگ *) o;
		إذا (!t->tagged)
			اكسر; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	إذا (o && o->type == OBJ_COMMIT) {
		commit = (بنية commit *)o;
		from_ٹیگ = starts_with(ref->name, "refs/tags/");
		إذا (taggerdate == ویلا_MAX)
			taggerdate = commit->date;
	}

	جوڑو_منزل_tip_جدول(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_ٹیگ, deref);
	أرجع 0;
}

ساكن فراغ name_tips(بنية mem_pool *string_pool)
{
	صحيح i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_جدول.جدول, tip_جدول.nr, cmp_by_ٹیگ_and_age);
	لكل (i = 0; i < tip_جدول.nr; i++) {
		بنية tip_جدول_اندراج *e = &tip_جدول.جدول[i];
		إذا (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_ٹیگ, e->deref, string_pool);
		}
	}
}

ساكن ثابت بنية object_id *nth_tip_جدول_ent(ناپ_t ix, ثابت فراغ *جدول_2)
{
	ثابت بنية tip_جدول_اندراج *جدول = جدول_2;
	أرجع &جدول[ix].oid;
}

ساكن ثابت حرف *get_exact_ref_میل(ثابت بنية object *o)
{
	صحيح لبھیا_2;

	إذا (!tip_جدول.جدول || !tip_جدول.nr)
		أرجع عدم;

	إذا (!tip_جدول.sorted) {
		QSORT(tip_جدول.جدول, tip_جدول.nr, tipcmp);
		tip_جدول.sorted = 1;
	}

	لبھیا_2 = oid_pos(&o->oid, tip_جدول.جدول, tip_جدول.nr,
			nth_tip_جدول_ent);
	إذا (0 <= لبھیا_2)
		أرجع tip_جدول.جدول[لبھیا_2].refname;
	أرجع عدم;
}

/* may return a constant string or use "buf" as scratch space */
ساكن ثابت حرف *get_rev_name(ثابت بنية object *o, بنية strbuf *buf)
{
	بنية rev_name *n;
	ثابت بنية commit *c;

	إذا (o->type != OBJ_COMMIT)
		أرجع get_exact_ref_میل(o);
	c = (ثابت بنية commit *) o;
	n = get_commit_rev_name(c);
	إذا (!n)
		أرجع عدم;

	إذا (!n->generation) {
		أرجع n->tip_name;
	} وإلا {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		أرجع buf->buf;
	}
}

ساكن ثابت حرف *get_format_rev(ثابت بنية commit *c,
				  بنية pretty_format *format_ctx,
				  بنية strbuf *buf)
{
	strbuf_reset(buf);

	إذا (format_ctx->want.notes) {
		بنية strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_سنیہا = strbuf_detach(&notebuf, عدم);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_سنیہا);

	أرجع buf->buf;
}

ساكن فراغ show_name(ثابت بنية object *obj,
		      ثابت حرف *caller_name,
		      صحيح always, صحيح allow_undefined, صحيح name_only)
{
	ثابت حرف *name;
	ثابت بنية object_id *oid = &obj->oid;
	بنية strbuf buf = STRBUF_INIT;

	إذا (!name_only)
		لکھو("%s ", caller_name ? caller_name : oid_منزل_hex(oid));
	name = get_rev_name(obj, &buf);
	إذا (name)
		لکھو("%s\n", name);
	وإلا إذا (allow_undefined)
		لکھو("undefined\n");
	وإلا إذا (always)
		لکھو("%s\n",
		       repo_لبھو_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	وإلا
		die("cannot describe '%s'", oid_منزل_hex(oid));
	strbuf_release(&buf);
}

ساكن حرف ثابت * ثابت name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	عدم
};

ساكن فراغ name_rev_سطر(حرف *p, بنية command *cmd)
{
	بنية strbuf buf = STRBUF_INIT;
	صحيح counter = 0;
	حرف *p_شروع;
	ثابت بنانشانی hexsz = the_hash_algo->hexsz;

	لكل (p_شروع = p; *p; p++) {
#تعریف_کرو ishex(x) (ہندسہ_اے((x)) || ((x) >= 'a' && (x) <= 'f'))
		إذا (!ishex(*p)) {
			counter = 0;
		} وإلا إذا (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			بنية object_id oid;
			ثابت حرف *name = عدم;
			حرف c = *(p + 1);
			صحيح p_len = p - p_شروع + 1;
			بنية object *o = عدم;
			صحيح oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			بدل (cmd->type) {
			حالة NAME_REV:
				إذا (!oid_ret)
					o = lookup_object(the_repository, &oid);
				إذا (o)
					name = get_rev_name(o, &buf);
				إذا (!name)
					استمر;
				إذا (cmd->u.name_only)
					لکھو("%.*s%s", p_len - hexsz, p_شروع, name);
				وإلا
					لکھو("%.*s (%s)", p_len, p_شروع, name);
				اكسر;
			حالة FORMAT_REV:
				إذا (!oid_ret)
					o = parse_object(the_repository, &oid);
				إذا (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ثابت بنية commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				إذا (name)
					لکھو("%.*s%s", p_len - hexsz, p_شروع, name);
				وإلا
					لکھو("%.*s", p_len, p_شروع);
				اكسر;
			افتراضي:
				BUG("uncovered case: %d", cmd->type);
			}

			p_شروع = p + 1;
		}
	}

	/* flush */
	إذا (p_شروع != p)
		fwrite(p_شروع, p - p_شروع, 1, stdout);

	strbuf_release(&buf);
}

صحيح cmd_name_rev(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اگیتر,
		 بنية repository *repo بن_ورتیا)
{
	بنية mem_pool string_pool;
	بنية object_array revs = OBJECT_ARRAY_INIT;

#جے_بنا_تعریف WITH_BREAKING_CHANGES
	صحيح transform_stdin = 0;
#جے_انت
	صحيح all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_ٹیگ = 0;
	بنية name_ref_انکڑے انکڑے = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	بنية command cmd;
	بنية option opts[] = {
		OPT_BOOL(0, "name-only", &انکڑے.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &انکڑے.ٹیگ_سوچی_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &انکڑے.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &انکڑے.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_گروہ(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#جے_بنا_تعریف WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#جے_انت /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_ٹیگ,
				N_("dereference tags in the input (internal use)")),
		OPT_سرا(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_ناں);
	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اگیتر, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, انکڑے.name_only);

#جے_بنا_تعریف WITH_BREAKING_CHANGES
	إذا (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#جے_انت

	إذا (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	إذا (all || annotate_stdin)
		disable_cutoff();

	لكل (; argc; argc--, argv++) {
		بنية object_id oid;
		بنية object *object;
		بنية commit *commit;

		إذا (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			استمر;
		}

		commit = عدم;
		object = parse_object(the_repository, &oid);
		إذا (object) {
			بنية object *peeled = deref_ٹیگ(the_repository,
							  object, *argv, 0);
			إذا (peeled && peeled->type == OBJ_COMMIT)
				commit = (بنية commit *)peeled;
		}

		إذا (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			استمر;
		}

		إذا (commit)
			سیٹ_commit_cutoff(commit);

		إذا (peel_ٹیگ) {
			إذا (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				استمر;
			}
			object = (بنية object *)commit;
		}
		جوڑو_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &انکڑے);
	name_tips(&string_pool);

	إذا (annotate_stdin) {
		بنية strbuf sb = STRBUF_INIT;

		طالما (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_سطر(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} وإلا إذا (all) {
		صحيح i, max;

		max = get_max_object_اشاریہ(the_repository);
		لكل (i = 0; i < max; i++) {
			بنية object *obj = get_اشاریہ_دار_object(the_repository, i);
			إذا (!obj || obj->type != OBJ_COMMIT)
				استمر;
			show_name(obj, عدم,
				  always, allow_undefined, انکڑے.name_only);
		}
	} وإلا {
		صحيح i;
		لكل (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].شے, revs.objects[i].name,
				  always, allow_undefined, انکڑے.name_only);
	}

	string_list_clear(&انکڑے.ref_filters, 0);
	string_list_clear(&انکڑے.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	أرجع 0;
}

بنية format_nul_انکڑے {
	bool nul_داخلہ;
	bool nul_output;
};

ساكن صحيح format_nul_cb(ثابت بنية option *option,
			 ثابت حرف *arg,
			 صحيح unset)
{
	بنية format_nul_انکڑے *انکڑے = option->قدر;
	انکڑے->nul_داخلہ = 1;
	انکڑے->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع 0;
}

ساكن تعداد stdin_منوال parse_stdin_منوال(ثابت حرف *stdin_منوال)
{
	إذا (!strcmp(stdin_منوال, "text"))
		أرجع متن_2;
	وإلا إذا (!strcmp(stdin_منوال, "revs") ||
		 !strcmp(stdin_منوال, "rev"))
		أرجع REVS;
	وإلا
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

ساكن حرف ثابت *ثابت format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	عدم
};

صحيح cmd_format_rev(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	ثابت حرف *format = عدم;
	تعداد stdin_منوال stdin_منوال;
	ثابت حرف *stdin_منوال_arg = عدم;
	بنية format_nul_انکڑے nul_انکڑے = { 0, 0 };
	حرف output_terminator;
	strbuf_getline_fn getline_fn;
	بنية display_notes_opt format_notes_opt;
	بنية rev_info format_rev = REV_INFO_INIT;
	بنية pretty_format format_pp = { 0 };
	بنية string_list notes = STRING_LIST_INIT_NODUP;
	بنية strbuf scratch_buf = STRBUF_INIT;
	بنية command cmd;
	بنية option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_منوال_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_انکڑے, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_انکڑے.nul_داخلہ,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_انکڑے.nul_output,
			 N_("Use NUL for output termination")),
		OPT_سرا(),
	};

	argc = parse_options(argc, argv, اگیتر, opts, format_rev_usage, 0);

	إذا (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	إذا (!format)
		die(_("'%s' is required"), "--format");
	إذا (!stdin_منوال_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_انکڑے.nul_داخلہ ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_انکڑے.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_منوال = parse_stdin_منوال(stdin_منوال_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_منوال_explicit = format_rev.date_منوال_explicit;
	format_pp.ctx.date_منوال = format_rev.date_منوال;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_لبھو_requirements(format,
				     &format_pp.want);
	إذا (format_pp.want.notes) {
		صحيح ignore_show_notes = 0;
		بنية string_list_شے *n;

		for_each_string_list_شے(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		لدو_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	بدل (stdin_منوال) {
	حالة متن_2:
		طالما (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_سطر(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			لکھو("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		اكسر;
	حالة REVS:
		طالما (getline_fn(&scratch_buf, stdin) != EOF) {
			بنية object_id oid;
			بنية object *object;
			بنية object *peeled;

			إذا (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				استمر;
			}

			object = parse_object(the_repository, &oid);
			إذا (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				استمر;
			}

			peeled = deref_ٹیگ(the_repository, object, scratch_buf.buf, 0);
			إذا (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				استمر;
			}

			get_format_rev((بنية commit *)peeled,
				       &format_pp, &scratch_buf);
			لکھو("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		اكسر;
	افتراضي:
		BUG("uncovered case: %d", stdin_منوال);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	أرجع 0;
}
