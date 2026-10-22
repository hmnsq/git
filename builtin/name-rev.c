#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "pager.h"
#شامل "parse-options.h"
#شامل "hash-lookup.h"
#شامل "commit-slab.h"
#شامل "commit-graph.h"
#شامل "wildmatch.h"
#شامل "mem-pool.h"
#شامل "pretty.h"
#شامل "revision.h"
#شامل "notes.h"
#شامل "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#تعریف_کرو CUTOFF_DATE_SLOP 86400

ساخت rev_name {
	ثابت حرف *tip_name;
	timestamp_t taggerdate;
	صحیح generation;
	صحیح فاصلہ;
	صحیح from_لیبل;
};

define_commit_slab(commit_rev_name, ساخت rev_name);

ساکن timestamp_t generation_cutoff = GENERATION_نمبر_INFINITY;
ساکن timestamp_t cutoff = وقت_MAX;
ساکن ساخت commit_rev_name rev_نام;

/* Disable the cutoff checks entirely */
ساکن خالی disable_cutoff(خالی)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ساکن خالی سیٹ_commit_cutoff(ساخت commit *commit)
{

	اگر (cutoff > commit->date)
		cutoff = commit->date;

	اگر (generation_cutoff) {
		timestamp_t generation = commit_گراف_generation(commit);

		اگر (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
ساکن خالی adjust_cutoff_timestamp_for_slop(خالی)
{
	اگر (cutoff) {
		/* check for underflow */
		اگر (cutoff > وقت_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		ورنہ
			cutoff = وقت_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ساکن صحیح commit_is_before_cutoff(ساخت commit *commit)
{
	اگر (generation_cutoff < GENERATION_نمبر_INFINITY)
		واپس generation_cutoff &&
			commit_گراف_generation(commit) < generation_cutoff;

	واپس commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#تعریف_کرو ملاؤ_TRAVERSAL_وزن 65535

ساکن صحیح is_valid_rev_name(ثابت ساخت rev_name *name)
{
	واپس name && name->tip_name;
}

ساکن ساخت rev_name *get_commit_rev_name(ثابت ساخت commit *commit)
{
	ساخت rev_name *name = commit_rev_name_peek(&rev_نام, commit);

	واپس is_valid_rev_name(name) ? name : تہی;
}

ساکن صحیح effective_فاصلہ(صحیح فاصلہ, صحیح generation)
{
	واپس فاصلہ + (generation > 0 ? ملاؤ_TRAVERSAL_وزن : 0);
}

ساکن صحیح is_better_name(ساخت rev_name *name,
			  timestamp_t taggerdate,
			  صحیح generation,
			  صحیح فاصلہ,
			  صحیح from_لیبل)
{
	صحیح name_فاصلہ = effective_فاصلہ(name->فاصلہ, name->generation);
	صحیح new_فاصلہ = effective_فاصلہ(فاصلہ, generation);

	/* If both are tags, we prefer the nearer one. */
	اگر (from_لیبل && name->from_لیبل)
		واپس name_فاصلہ > new_فاصلہ;

	/* Favor a tag over a non-tag. */
	اگر (name->from_لیبل != from_لیبل)
		واپس from_لیبل;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	اگر (name_فاصلہ != new_فاصلہ)
		واپس name_فاصلہ > new_فاصلہ;

	/* ... or tiebreak to favor older date */
	اگر (name->taggerdate != taggerdate)
		واپس name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	واپس 0;
}

ساکن ساخت rev_name *create_or_update_name(ساخت commit *commit,
					      timestamp_t taggerdate,
					      صحیح generation, صحیح فاصلہ,
					      صحیح from_لیبل)
{
	ساخت rev_name *name = commit_rev_name_at(&rev_نام, commit);

	اگر (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, فاصلہ, from_لیبل))
		واپس تہی;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->فاصلہ = فاصلہ;
	name->from_لیبل = from_لیبل;

	واپس name;
}

ساکن حرف *get_والد_name(ثابت ساخت rev_name *name, صحیح والد_نمبر,
			     ساخت mem_pool *string_pool)
{
	سائز_t len;

	strip_suffix(name->tip_name, "^0", &len);
	اگر (name->generation > 0) {
		واپس mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (صحیح)len, name->tip_name,
				       name->generation, والد_نمبر);
	} ورنہ {
		واپس mem_pool_strfmt(string_pool, "%.*s^%d",
				       (صحیح)len, name->tip_name, والد_نمبر);
	}
}

ساکن خالی name_rev(ساخت commit *آغاز_commit,
		ثابت حرف *tip_name, timestamp_t taggerdate,
		صحیح from_لیبل, صحیح deref, ساخت mem_pool *string_pool)
{
	ساخت commit_ڈھیر ڈھیر = COMMIT_ڈھیر_INIT;
	ساخت commit *commit;
	ساخت commit_ڈھیر parents_مقصد_قطار = COMMIT_ڈھیر_INIT;
	ساخت rev_name *آغاز_name;

	repo_parse_commit(the_repository, آغاز_commit);
	اگر (commit_is_before_cutoff(آغاز_commit))
		واپس;

	آغاز_name = create_or_update_name(آغاز_commit, taggerdate, 0, 0,
					   from_لیبل);
	اگر (!آغاز_name)
		واپس;
	اگر (deref)
		آغاز_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	ورنہ
		آغاز_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ڈھیر_ڈالو(&ڈھیر, آغاز_commit);

	جب ((commit = commit_ڈھیر_نکالو(&ڈھیر))) {
		ساخت rev_name *name = get_commit_rev_name(commit);
		ساخت commit_list *parents;
		صحیح والد_نمبر = 1;

		parents_مقصد_قطار.nr = 0;

		برائے (parents = commit->parents;
				parents;
				parents = parents->اگلا, والد_نمبر++) {
			ساخت commit *والد = parents->شے;
			ساخت rev_name *والد_name;
			صحیح generation, فاصلہ;

			repo_parse_commit(the_repository, والد);
			اگر (commit_is_before_cutoff(والد))
				جاری;

			اگر (والد_نمبر > 1) {
				generation = 0;
				فاصلہ = name->فاصلہ + ملاؤ_TRAVERSAL_وزن;
			} ورنہ {
				generation = name->generation + 1;
				فاصلہ = name->فاصلہ + 1;
			}

			والد_name = create_or_update_name(والد, taggerdate,
							    generation,
							    فاصلہ, from_لیبل);
			اگر (والد_name) {
				اگر (والد_نمبر > 1)
					والد_name->tip_name =
						get_والد_name(name,
								والد_نمبر,
								string_pool);
				ورنہ
					والد_name->tip_name = name->tip_name;
				commit_ڈھیر_ڈالو(&parents_مقصد_قطار, والد);
			}
		}

		/* The first parent must come out first from the stack */
		جب (parents_مقصد_قطار.nr)
			commit_ڈھیر_ڈالو(&ڈھیر,
					  commit_ڈھیر_نکالو(&parents_مقصد_قطار));
	}

	commit_ڈھیر_clear(&ڈھیر);
	commit_ڈھیر_clear(&parents_مقصد_قطار);
}

ساکن صحیح subpath_matches(ثابت حرف *راستہ, ثابت حرف *filter)
{
	ثابت حرف *subpath = راستہ;

	جب (subpath) {
		اگر (!wildmatch(filter, subpath, 0))
			واپس subpath - راستہ;
		subpath = strchr(subpath, '/');
		اگر (subpath)
			subpath++;
	}
	واپس -1;
}

ساخت name_ref_ڈیٹا {
	صحیح لیبل_فہرست_only;
	صحیح name_only;
	ساخت string_list ref_filters;
	ساخت string_list exclude_filters;
};

ساخت pretty_format {
	ساخت pretty_print_context ctx;
	ساخت userformat_want want;
};

شمار command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

شمار stdin_منوال {
    متن_2 = 1,
    REVS = 2,
};

ساخت command {
	شمار command_type type;
	اتحاد {
		صحیح name_only;
		ساخت pretty_format *pretty_format;
	} u;
};

ساکن خالی init_name_rev_command(ساخت command *cmd,
				  صحیح name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

ساکن خالی init_format_rev_command(ساخت command *cmd,
				    ساخت pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

ساکن ساخت tip_جدول {
	ساخت tip_جدول_اندراج {
		ساخت object_id oid;
		ثابت حرف *refname;
		ساخت commit *commit;
		timestamp_t taggerdate;
		غیرعلامتی صحیح from_لیبل:1;
		غیرعلامتی صحیح deref:1;
	} *جدول;
	صحیح nr;
	صحیح alloc;
	صحیح sorted;
} tip_جدول;

ساکن خالی جوڑو_مقصد_tip_جدول(ثابت ساخت object_id *oid, ثابت حرف *refname,
			     صحیح shorten_unambiguous, ساخت commit *commit,
			     timestamp_t taggerdate, صحیح from_لیبل, صحیح deref)
{
	حرف *short_refname = تہی;

	اگر (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	ورنہ اگر (skip_سابقہ(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	ورنہ
		skip_سابقہ(refname, "refs/", &refname);

	ALLOC_GROW(tip_جدول.جدول, tip_جدول.nr + 1, tip_جدول.alloc);
	oidcpy(&tip_جدول.جدول[tip_جدول.nr].oid, oid);
	tip_جدول.جدول[tip_جدول.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_جدول.جدول[tip_جدول.nr].commit = commit;
	tip_جدول.جدول[tip_جدول.nr].taggerdate = taggerdate;
	tip_جدول.جدول[tip_جدول.nr].from_لیبل = from_لیبل;
	tip_جدول.جدول[tip_جدول.nr].deref = deref;
	tip_جدول.nr++;
	tip_جدول.sorted = 0;
}

ساکن صحیح tipcmp(ثابت خالی *a_, ثابت خالی *b_)
{
	ثابت ساخت tip_جدول_اندراج *a = a_, *b = b_;
	واپس oidcmp(&a->oid, &b->oid);
}

ساکن صحیح cmp_by_لیبل_and_age(ثابت خالی *a_, ثابت خالی *b_)
{
	ثابت ساخت tip_جدول_اندراج *a = a_, *b = b_;
	صحیح cmp;

	/* Prefer tags. */
	cmp = b->from_لیبل - a->from_لیبل;
	اگر (cmp)
		واپس cmp;

	/* Older is better. */
	اگر (a->taggerdate < b->taggerdate)
		واپس -1;
	واپس a->taggerdate != b->taggerdate;
}

ساکن صحیح name_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت object *o = parse_object(the_repository, ref->oid);
	ساخت name_ref_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	صحیح can_abbreviate_output = ڈیٹا->لیبل_فہرست_only && ڈیٹا->name_only;
	صحیح deref = 0;
	صحیح from_لیبل = 0;
	ساخت commit *commit = تہی;
	timestamp_t taggerdate = وقت_MAX;

	اگر (ڈیٹا->لیبل_فہرست_only && !starts_with(ref->name, "refs/tags/"))
		واپس 0;

	اگر (ڈیٹا->exclude_filters.nr) {
		ساخت string_list_شے *شے;

		for_each_string_list_شے(شے, &ڈیٹا->exclude_filters) {
			اگر (subpath_matches(ref->name, شے->string) >= 0)
				واپس 0;
		}
	}

	اگر (ڈیٹا->ref_filters.nr) {
		ساخت string_list_شے *شے;
		صحیح matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_شے(شے, &ڈیٹا->ref_filters) {
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
			انتخاب (subpath_matches(ref->name, شے->string)) {
			حالت -1: /* did not match */
				توڑو;
			حالت 0: /* matched fully */
				matched = 1;
				توڑو;
			طےشدہ: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				توڑو;
			}
		}

		/* If none of the patterns matched, stop now */
		اگر (!matched)
			واپس 0;
	}

	جب (o && o->type == OBJ_لیبل) {
		ساخت لیبل *t = (ساخت لیبل *) o;
		اگر (!t->tagged)
			توڑو; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	اگر (o && o->type == OBJ_COMMIT) {
		commit = (ساخت commit *)o;
		from_لیبل = starts_with(ref->name, "refs/tags/");
		اگر (taggerdate == وقت_MAX)
			taggerdate = commit->date;
	}

	جوڑو_مقصد_tip_جدول(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_لیبل, deref);
	واپس 0;
}

ساکن خالی name_tips(ساخت mem_pool *string_pool)
{
	صحیح i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_جدول.جدول, tip_جدول.nr, cmp_by_لیبل_and_age);
	برائے (i = 0; i < tip_جدول.nr; i++) {
		ساخت tip_جدول_اندراج *e = &tip_جدول.جدول[i];
		اگر (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_لیبل, e->deref, string_pool);
		}
	}
}

ساکن ثابت ساخت object_id *nth_tip_جدول_ent(سائز_t ix, ثابت خالی *جدول_2)
{
	ثابت ساخت tip_جدول_اندراج *جدول = جدول_2;
	واپس &جدول[ix].oid;
}

ساکن ثابت حرف *get_exact_ref_مطابقت(ثابت ساخت object *o)
{
	صحیح ملا_2;

	اگر (!tip_جدول.جدول || !tip_جدول.nr)
		واپس تہی;

	اگر (!tip_جدول.sorted) {
		QSORT(tip_جدول.جدول, tip_جدول.nr, tipcmp);
		tip_جدول.sorted = 1;
	}

	ملا_2 = oid_pos(&o->oid, tip_جدول.جدول, tip_جدول.nr,
			nth_tip_جدول_ent);
	اگر (0 <= ملا_2)
		واپس tip_جدول.جدول[ملا_2].refname;
	واپس تہی;
}

/* may return a constant string or use "buf" as scratch space */
ساکن ثابت حرف *get_rev_name(ثابت ساخت object *o, ساخت strbuf *buf)
{
	ساخت rev_name *n;
	ثابت ساخت commit *c;

	اگر (o->type != OBJ_COMMIT)
		واپس get_exact_ref_مطابقت(o);
	c = (ثابت ساخت commit *) o;
	n = get_commit_rev_name(c);
	اگر (!n)
		واپس تہی;

	اگر (!n->generation) {
		واپس n->tip_name;
	} ورنہ {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		واپس buf->buf;
	}
}

ساکن ثابت حرف *get_format_rev(ثابت ساخت commit *c,
				  ساخت pretty_format *format_ctx,
				  ساخت strbuf *buf)
{
	strbuf_reset(buf);

	اگر (format_ctx->want.notes) {
		ساخت strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_پیغام = strbuf_detach(&notebuf, تہی);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_پیغام);

	واپس buf->buf;
}

ساکن خالی show_name(ثابت ساخت object *obj,
		      ثابت حرف *caller_name,
		      صحیح always, صحیح allow_undefined, صحیح name_only)
{
	ثابت حرف *name;
	ثابت ساخت object_id *oid = &obj->oid;
	ساخت strbuf buf = STRBUF_INIT;

	اگر (!name_only)
		لکھو("%s ", caller_name ? caller_name : oid_مقصد_hex(oid));
	name = get_rev_name(obj, &buf);
	اگر (name)
		لکھو("%s\n", name);
	ورنہ اگر (allow_undefined)
		لکھو("undefined\n");
	ورنہ اگر (always)
		لکھو("%s\n",
		       repo_ڈھونڈو_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	ورنہ
		die("cannot describe '%s'", oid_مقصد_hex(oid));
	strbuf_release(&buf);
}

ساکن حرف ثابت * ثابت name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	تہی
};

ساکن خالی name_rev_سطر(حرف *p, ساخت command *cmd)
{
	ساخت strbuf buf = STRBUF_INIT;
	صحیح شمار_2 = 0;
	حرف *p_آغاز;
	ثابت غیرعلامتی hexsz = the_hash_algo->hexsz;

	برائے (p_آغاز = p; *p; p++) {
#تعریف_کرو ishex(x) (ہندسہ_ہے((x)) || ((x) >= 'a' && (x) <= 'f'))
		اگر (!ishex(*p)) {
			شمار_2 = 0;
		} ورنہ اگر (++شمار_2 == hexsz &&
			   !ishex(*(p + 1))) {
			ساخت object_id oid;
			ثابت حرف *name = تہی;
			حرف c = *(p + 1);
			صحیح p_len = p - p_آغاز + 1;
			ساخت object *o = تہی;
			صحیح oid_ret = 1;

			شمار_2 = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			انتخاب (cmd->type) {
			حالت NAME_REV:
				اگر (!oid_ret)
					o = lookup_object(the_repository, &oid);
				اگر (o)
					name = get_rev_name(o, &buf);
				اگر (!name)
					جاری;
				اگر (cmd->u.name_only)
					لکھو("%.*s%s", p_len - hexsz, p_آغاز, name);
				ورنہ
					لکھو("%.*s (%s)", p_len, p_آغاز, name);
				توڑو;
			حالت FORMAT_REV:
				اگر (!oid_ret)
					o = parse_object(the_repository, &oid);
				اگر (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ثابت ساخت commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				اگر (name)
					لکھو("%.*s%s", p_len - hexsz, p_آغاز, name);
				ورنہ
					لکھو("%.*s", p_len, p_آغاز);
				توڑو;
			طےشدہ:
				BUG("uncovered case: %d", cmd->type);
			}

			p_آغاز = p + 1;
		}
	}

	/* flush */
	اگر (p_آغاز != p)
		fwrite(p_آغاز, p - p_آغاز, 1, stdout);

	strbuf_release(&buf);
}

صحیح cmd_name_rev(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت mem_pool string_pool;
	ساخت object_array revs = OBJECT_ARRAY_INIT;

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	صحیح transform_stdin = 0;
#اگر_ختم
	صحیح all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_لیبل = 0;
	ساخت name_ref_ڈیٹا ڈیٹا = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	ساخت command cmd;
	ساخت option opts[] = {
		OPT_BOOL(0, "name-only", &ڈیٹا.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &ڈیٹا.لیبل_فہرست_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &ڈیٹا.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &ڈیٹا.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_گروہ(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#اگر_ختم /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_لیبل,
				N_("dereference tags in the input (internal use)")),
		OPT_انتہا(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_نام);
	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, ڈیٹا.name_only);

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	اگر (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#اگر_ختم

	اگر (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	اگر (all || annotate_stdin)
		disable_cutoff();

	برائے (; argc; argc--, argv++) {
		ساخت object_id oid;
		ساخت object *object;
		ساخت commit *commit;

		اگر (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			جاری;
		}

		commit = تہی;
		object = parse_object(the_repository, &oid);
		اگر (object) {
			ساخت object *peeled = deref_لیبل(the_repository,
							  object, *argv, 0);
			اگر (peeled && peeled->type == OBJ_COMMIT)
				commit = (ساخت commit *)peeled;
		}

		اگر (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			جاری;
		}

		اگر (commit)
			سیٹ_commit_cutoff(commit);

		اگر (peel_لیبل) {
			اگر (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				جاری;
			}
			object = (ساخت object *)commit;
		}
		جوڑو_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &ڈیٹا);
	name_tips(&string_pool);

	اگر (annotate_stdin) {
		ساخت strbuf sb = STRBUF_INIT;

		جب (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_سطر(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} ورنہ اگر (all) {
		صحیح i, max;

		max = get_max_object_اشاریہ(the_repository);
		برائے (i = 0; i < max; i++) {
			ساخت object *obj = get_اشاریہ_دار_object(the_repository, i);
			اگر (!obj || obj->type != OBJ_COMMIT)
				جاری;
			show_name(obj, تہی,
				  always, allow_undefined, ڈیٹا.name_only);
		}
	} ورنہ {
		صحیح i;
		برائے (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].شے, revs.objects[i].name,
				  always, allow_undefined, ڈیٹا.name_only);
	}

	string_list_clear(&ڈیٹا.ref_filters, 0);
	string_list_clear(&ڈیٹا.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	واپس 0;
}

ساخت format_nul_ڈیٹا {
	bool nul_داخلہ;
	bool nul_output;
};

ساکن صحیح format_nul_cb(ثابت ساخت option *option,
			 ثابت حرف *arg,
			 صحیح unset)
{
	ساخت format_nul_ڈیٹا *ڈیٹا = option->قدر;
	ڈیٹا->nul_داخلہ = 1;
	ڈیٹا->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	واپس 0;
}

ساکن شمار stdin_منوال parse_stdin_منوال(ثابت حرف *stdin_منوال)
{
	اگر (!strcmp(stdin_منوال, "text"))
		واپس متن_2;
	ورنہ اگر (!strcmp(stdin_منوال, "revs") ||
		 !strcmp(stdin_منوال, "rev"))
		واپس REVS;
	ورنہ
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

ساکن حرف ثابت *ثابت format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	تہی
};

صحیح cmd_format_rev(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف *format = تہی;
	شمار stdin_منوال stdin_منوال;
	ثابت حرف *stdin_منوال_arg = تہی;
	ساخت format_nul_ڈیٹا nul_ڈیٹا = { 0, 0 };
	حرف output_terminator;
	strbuf_getline_fn getline_fn;
	ساخت display_notes_opt format_notes_opt;
	ساخت rev_info format_rev = REV_INFO_INIT;
	ساخت pretty_format format_pp = { 0 };
	ساخت string_list notes = STRING_LIST_INIT_NODUP;
	ساخت strbuf scratch_buf = STRBUF_INIT;
	ساخت command cmd;
	ساخت option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_منوال_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_ڈیٹا, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_ڈیٹا.nul_داخلہ,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_ڈیٹا.nul_output,
			 N_("Use NUL for output termination")),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, opts, format_rev_usage, 0);

	اگر (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	اگر (!format)
		die(_("'%s' is required"), "--format");
	اگر (!stdin_منوال_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_ڈیٹا.nul_داخلہ ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_ڈیٹا.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_منوال = parse_stdin_منوال(stdin_منوال_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_منوال_explicit = format_rev.date_منوال_explicit;
	format_pp.ctx.date_منوال = format_rev.date_منوال;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_ڈھونڈو_requirements(format,
				     &format_pp.want);
	اگر (format_pp.want.notes) {
		صحیح ignore_show_notes = 0;
		ساخت string_list_شے *n;

		for_each_string_list_شے(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		بھرو_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	انتخاب (stdin_منوال) {
	حالت متن_2:
		جب (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_سطر(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			لکھو("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		توڑو;
	حالت REVS:
		جب (getline_fn(&scratch_buf, stdin) != EOF) {
			ساخت object_id oid;
			ساخت object *object;
			ساخت object *peeled;

			اگر (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				جاری;
			}

			object = parse_object(the_repository, &oid);
			اگر (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				جاری;
			}

			peeled = deref_لیبل(the_repository, object, scratch_buf.buf, 0);
			اگر (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				جاری;
			}

			get_format_rev((ساخت commit *)peeled,
				       &format_pp, &scratch_buf);
			لکھو("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		توڑو;
	طےشدہ:
		BUG("uncovered case: %d", stdin_منوال);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	واپس 0;
}
