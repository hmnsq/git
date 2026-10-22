#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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
#وصف_ڪريو CUTOFF_DATE_SLOP 86400

بنية rev_name {
	ثابت حرف *tip_name;
	timestamp_t taggerdate;
	صحيح generation;
	صحيح مفاصلو;
	صحيح from_ٽيگ;
};

define_commit_slab(commit_rev_name, بنية rev_name);

ساكن timestamp_t generation_cutoff = GENERATION_عدد_INFINITY;
ساكن timestamp_t cutoff = وقت_MAX;
ساكن بنية commit_rev_name rev_نالا;

/* Disable the cutoff checks entirely */
ساكن فراغ disable_cutoff(فراغ)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ساكن فراغ سيٽ_commit_cutoff(بنية commit *commit)
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
		إذا (cutoff > وقت_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		وإلا
			cutoff = وقت_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ساكن صحيح commit_is_before_cutoff(بنية commit *commit)
{
	إذا (generation_cutoff < GENERATION_عدد_INFINITY)
		أرجع generation_cutoff &&
			commit_گراف_generation(commit) < generation_cutoff;

	أرجع commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#وصف_ڪريو ملايو_TRAVERSAL_وزن 65535

ساكن صحيح is_valid_rev_name(ثابت بنية rev_name *name)
{
	أرجع name && name->tip_name;
}

ساكن بنية rev_name *get_commit_rev_name(ثابت بنية commit *commit)
{
	بنية rev_name *name = commit_rev_name_peek(&rev_نالا, commit);

	أرجع is_valid_rev_name(name) ? name : عدم;
}

ساكن صحيح effective_مفاصلو(صحيح مفاصلو, صحيح generation)
{
	أرجع مفاصلو + (generation > 0 ? ملايو_TRAVERSAL_وزن : 0);
}

ساكن صحيح is_better_name(بنية rev_name *name,
			  timestamp_t taggerdate,
			  صحيح generation,
			  صحيح مفاصلو,
			  صحيح from_ٽيگ)
{
	صحيح name_مفاصلو = effective_مفاصلو(name->مفاصلو, name->generation);
	صحيح new_مفاصلو = effective_مفاصلو(مفاصلو, generation);

	/* If both are tags, we prefer the nearer one. */
	إذا (from_ٽيگ && name->from_ٽيگ)
		أرجع name_مفاصلو > new_مفاصلو;

	/* Favor a tag over a non-tag. */
	إذا (name->from_ٽيگ != from_ٽيگ)
		أرجع from_ٽيگ;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	إذا (name_مفاصلو != new_مفاصلو)
		أرجع name_مفاصلو > new_مفاصلو;

	/* ... or tiebreak to favor older date */
	إذا (name->taggerdate != taggerdate)
		أرجع name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	أرجع 0;
}

ساكن بنية rev_name *create_or_update_name(بنية commit *commit,
					      timestamp_t taggerdate,
					      صحيح generation, صحيح مفاصلو,
					      صحيح from_ٽيگ)
{
	بنية rev_name *name = commit_rev_name_at(&rev_نالا, commit);

	إذا (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, مفاصلو, from_ٽيگ))
		أرجع عدم;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->مفاصلو = مفاصلو;
	name->from_ٽيگ = from_ٽيگ;

	أرجع name;
}

ساكن حرف *get_والد_name(ثابت بنية rev_name *name, صحيح والد_عدد,
			     بنية mem_pool *string_pool)
{
	سائيز_t len;

	strip_suffix(name->tip_name, "^0", &len);
	إذا (name->generation > 0) {
		أرجع mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (صحيح)len, name->tip_name,
				       name->generation, والد_عدد);
	} وإلا {
		أرجع mem_pool_strfmt(string_pool, "%.*s^%d",
				       (صحيح)len, name->tip_name, والد_عدد);
	}
}

ساكن فراغ name_rev(بنية commit *شروعات_commit,
		ثابت حرف *tip_name, timestamp_t taggerdate,
		صحيح from_ٽيگ, صحيح deref, بنية mem_pool *string_pool)
{
	بنية commit_ڍير ڍير = COMMIT_ڍير_INIT;
	بنية commit *commit;
	بنية commit_ڍير parents_منزل_قطار = COMMIT_ڍير_INIT;
	بنية rev_name *شروعات_name;

	repo_parse_commit(the_repository, شروعات_commit);
	إذا (commit_is_before_cutoff(شروعات_commit))
		أرجع;

	شروعات_name = create_or_update_name(شروعات_commit, taggerdate, 0, 0,
					   from_ٽيگ);
	إذا (!شروعات_name)
		أرجع;
	إذا (deref)
		شروعات_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	وإلا
		شروعات_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ڍير_ڌڪو(&ڍير, شروعات_commit);

	طالما ((commit = commit_ڍير_ڪڍو(&ڍير))) {
		بنية rev_name *name = get_commit_rev_name(commit);
		بنية commit_list *parents;
		صحيح والد_عدد = 1;

		parents_منزل_قطار.nr = 0;

		لكل (parents = commit->parents;
				parents;
				parents = parents->ايندڙ, والد_عدد++) {
			بنية commit *والد = parents->شي;
			بنية rev_name *والد_name;
			صحيح generation, مفاصلو;

			repo_parse_commit(the_repository, والد);
			إذا (commit_is_before_cutoff(والد))
				استمر;

			إذا (والد_عدد > 1) {
				generation = 0;
				مفاصلو = name->مفاصلو + ملايو_TRAVERSAL_وزن;
			} وإلا {
				generation = name->generation + 1;
				مفاصلو = name->مفاصلو + 1;
			}

			والد_name = create_or_update_name(والد, taggerdate,
							    generation,
							    مفاصلو, from_ٽيگ);
			إذا (والد_name) {
				إذا (والد_عدد > 1)
					والد_name->tip_name =
						get_والد_name(name,
								والد_عدد,
								string_pool);
				وإلا
					والد_name->tip_name = name->tip_name;
				commit_ڍير_ڌڪو(&parents_منزل_قطار, والد);
			}
		}

		/* The first parent must come out first from the stack */
		طالما (parents_منزل_قطار.nr)
			commit_ڍير_ڌڪو(&ڍير,
					  commit_ڍير_ڪڍو(&parents_منزل_قطار));
	}

	commit_ڍير_clear(&ڍير);
	commit_ڍير_clear(&parents_منزل_قطار);
}

ساكن صحيح subpath_matches(ثابت حرف *رستو, ثابت حرف *filter)
{
	ثابت حرف *subpath = رستو;

	طالما (subpath) {
		إذا (!wildmatch(filter, subpath, 0))
			أرجع subpath - رستو;
		subpath = strchr(subpath, '/');
		إذا (subpath)
			subpath++;
	}
	أرجع -1;
}

بنية name_ref_معلومات {
	صحيح ٽيگ_فهرست_only;
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

تعداد stdin_گهڻائي {
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
	بنية tip_جدول_داخلا {
		بنية object_id oid;
		ثابت حرف *refname;
		بنية commit *commit;
		timestamp_t taggerdate;
		بنانشان صحيح from_ٽيگ:1;
		بنانشان صحيح deref:1;
	} *جدول;
	صحيح nr;
	صحيح alloc;
	صحيح sorted;
} tip_جدول;

ساكن فراغ جوڙيو_منزل_tip_جدول(ثابت بنية object_id *oid, ثابت حرف *refname,
			     صحيح shorten_unambiguous, بنية commit *commit,
			     timestamp_t taggerdate, صحيح from_ٽيگ, صحيح deref)
{
	حرف *short_refname = عدم;

	إذا (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	وإلا إذا (skip_اڳياڙي(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	وإلا
		skip_اڳياڙي(refname, "refs/", &refname);

	ALLOC_GROW(tip_جدول.جدول, tip_جدول.nr + 1, tip_جدول.alloc);
	oidcpy(&tip_جدول.جدول[tip_جدول.nr].oid, oid);
	tip_جدول.جدول[tip_جدول.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_جدول.جدول[tip_جدول.nr].commit = commit;
	tip_جدول.جدول[tip_جدول.nr].taggerdate = taggerdate;
	tip_جدول.جدول[tip_جدول.nr].from_ٽيگ = from_ٽيگ;
	tip_جدول.جدول[tip_جدول.nr].deref = deref;
	tip_جدول.nr++;
	tip_جدول.sorted = 0;
}

ساكن صحيح tipcmp(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية tip_جدول_داخلا *a = a_, *b = b_;
	أرجع oidcmp(&a->oid, &b->oid);
}

ساكن صحيح cmp_by_ٽيگ_and_age(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية tip_جدول_داخلا *a = a_, *b = b_;
	صحيح cmp;

	/* Prefer tags. */
	cmp = b->from_ٽيگ - a->from_ٽيگ;
	إذا (cmp)
		أرجع cmp;

	/* Older is better. */
	إذا (a->taggerdate < b->taggerdate)
		أرجع -1;
	أرجع a->taggerdate != b->taggerdate;
}

ساكن صحيح name_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية object *o = parse_object(the_repository, ref->oid);
	بنية name_ref_معلومات *معلومات = cb_معلومات;
	صحيح can_abbreviate_output = معلومات->ٽيگ_فهرست_only && معلومات->name_only;
	صحيح deref = 0;
	صحيح from_ٽيگ = 0;
	بنية commit *commit = عدم;
	timestamp_t taggerdate = وقت_MAX;

	إذا (معلومات->ٽيگ_فهرست_only && !starts_with(ref->name, "refs/tags/"))
		أرجع 0;

	إذا (معلومات->exclude_filters.nr) {
		بنية string_list_شي *شي;

		for_each_string_list_شي(شي, &معلومات->exclude_filters) {
			إذا (subpath_matches(ref->name, شي->string) >= 0)
				أرجع 0;
		}
	}

	إذا (معلومات->ref_filters.nr) {
		بنية string_list_شي *شي;
		صحيح matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_شي(شي, &معلومات->ref_filters) {
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
			بدل (subpath_matches(ref->name, شي->string)) {
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

	طالما (o && o->type == OBJ_ٽيگ) {
		بنية ٽيگ *t = (بنية ٽيگ *) o;
		إذا (!t->tagged)
			اكسر; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	إذا (o && o->type == OBJ_COMMIT) {
		commit = (بنية commit *)o;
		from_ٽيگ = starts_with(ref->name, "refs/tags/");
		إذا (taggerdate == وقت_MAX)
			taggerdate = commit->date;
	}

	جوڙيو_منزل_tip_جدول(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_ٽيگ, deref);
	أرجع 0;
}

ساكن فراغ name_tips(بنية mem_pool *string_pool)
{
	صحيح i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_جدول.جدول, tip_جدول.nr, cmp_by_ٽيگ_and_age);
	لكل (i = 0; i < tip_جدول.nr; i++) {
		بنية tip_جدول_داخلا *e = &tip_جدول.جدول[i];
		إذا (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_ٽيگ, e->deref, string_pool);
		}
	}
}

ساكن ثابت بنية object_id *nth_tip_جدول_ent(سائيز_t ix, ثابت فراغ *جدول_2)
{
	ثابت بنية tip_جدول_داخلا *جدول = جدول_2;
	أرجع &جدول[ix].oid;
}

ساكن ثابت حرف *get_exact_ref_ميلاپ(ثابت بنية object *o)
{
	صحيح مليو_2;

	إذا (!tip_جدول.جدول || !tip_جدول.nr)
		أرجع عدم;

	إذا (!tip_جدول.sorted) {
		QSORT(tip_جدول.جدول, tip_جدول.nr, tipcmp);
		tip_جدول.sorted = 1;
	}

	مليو_2 = oid_pos(&o->oid, tip_جدول.جدول, tip_جدول.nr,
			nth_tip_جدول_ent);
	إذا (0 <= مليو_2)
		أرجع tip_جدول.جدول[مليو_2].refname;
	أرجع عدم;
}

/* may return a constant string or use "buf" as scratch space */
ساكن ثابت حرف *get_rev_name(ثابت بنية object *o, بنية strbuf *buf)
{
	بنية rev_name *n;
	ثابت بنية commit *c;

	إذا (o->type != OBJ_COMMIT)
		أرجع get_exact_ref_ميلاپ(o);
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
		format_ctx->ctx.notes_پيغام = strbuf_detach(&notebuf, عدم);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_پيغام);

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
		لکو("%s ", caller_name ? caller_name : oid_منزل_hex(oid));
	name = get_rev_name(obj, &buf);
	إذا (name)
		لکو("%s\n", name);
	وإلا إذا (allow_undefined)
		لکو("undefined\n");
	وإلا إذا (always)
		لکو("%s\n",
		       repo_ڳوليو_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
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

ساكن فراغ name_rev_سٽ(حرف *p, بنية command *cmd)
{
	بنية strbuf buf = STRBUF_INIT;
	صحيح counter = 0;
	حرف *p_شروعات;
	ثابت بنانشان hexsz = the_hash_algo->hexsz;

	لكل (p_شروعات = p; *p; p++) {
#وصف_ڪريو ishex(x) (انگ_آهي((x)) || ((x) >= 'a' && (x) <= 'f'))
		إذا (!ishex(*p)) {
			counter = 0;
		} وإلا إذا (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			بنية object_id oid;
			ثابت حرف *name = عدم;
			حرف c = *(p + 1);
			صحيح p_len = p - p_شروعات + 1;
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
					لکو("%.*s%s", p_len - hexsz, p_شروعات, name);
				وإلا
					لکو("%.*s (%s)", p_len, p_شروعات, name);
				اكسر;
			حالة FORMAT_REV:
				إذا (!oid_ret)
					o = parse_object(the_repository, &oid);
				إذا (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ثابت بنية commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				إذا (name)
					لکو("%.*s%s", p_len - hexsz, p_شروعات, name);
				وإلا
					لکو("%.*s", p_len, p_شروعات);
				اكسر;
			افتراضي:
				BUG("uncovered case: %d", cmd->type);
			}

			p_شروعات = p + 1;
		}
	}

	/* flush */
	إذا (p_شروعات != p)
		fwrite(p_شروعات, p - p_شروعات, 1, stdout);

	strbuf_release(&buf);
}

صحيح cmd_name_rev(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية mem_pool string_pool;
	بنية object_array revs = OBJECT_ARRAY_INIT;

#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
	صحيح transform_stdin = 0;
#جيڪڏهن_پورو
	صحيح all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_ٽيگ = 0;
	بنية name_ref_معلومات معلومات = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	بنية command cmd;
	بنية option opts[] = {
		OPT_BOOL(0, "name-only", &معلومات.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &معلومات.ٽيگ_فهرست_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &معلومات.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &معلومات.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_گروهه(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#جيڪڏهن_پورو /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_ٽيگ,
				N_("dereference tags in the input (internal use)")),
		OPT_پڇاڙي(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_نالا);
	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اڳياڙي, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, معلومات.name_only);

#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
	إذا (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#جيڪڏهن_پورو

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
			بنية object *peeled = deref_ٽيگ(the_repository,
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
			سيٽ_commit_cutoff(commit);

		إذا (peel_ٽيگ) {
			إذا (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				استمر;
			}
			object = (بنية object *)commit;
		}
		جوڙيو_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &معلومات);
	name_tips(&string_pool);

	إذا (annotate_stdin) {
		بنية strbuf sb = STRBUF_INIT;

		طالما (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_سٽ(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} وإلا إذا (all) {
		صحيح i, max;

		max = get_max_object_اشاريو(the_repository);
		لكل (i = 0; i < max; i++) {
			بنية object *obj = get_اشاريي_وارو_object(the_repository, i);
			إذا (!obj || obj->type != OBJ_COMMIT)
				استمر;
			show_name(obj, عدم,
				  always, allow_undefined, معلومات.name_only);
		}
	} وإلا {
		صحيح i;
		لكل (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].شي, revs.objects[i].name,
				  always, allow_undefined, معلومات.name_only);
	}

	string_list_clear(&معلومات.ref_filters, 0);
	string_list_clear(&معلومات.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	أرجع 0;
}

بنية format_nul_معلومات {
	bool nul_داخلا;
	bool nul_output;
};

ساكن صحيح format_nul_cb(ثابت بنية option *option,
			 ثابت حرف *arg,
			 صحيح unset)
{
	بنية format_nul_معلومات *معلومات = option->قدر;
	معلومات->nul_داخلا = 1;
	معلومات->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع 0;
}

ساكن تعداد stdin_گهڻائي parse_stdin_گهڻائي(ثابت حرف *stdin_گهڻائي)
{
	إذا (!strcmp(stdin_گهڻائي, "text"))
		أرجع متن_2;
	وإلا إذا (!strcmp(stdin_گهڻائي, "revs") ||
		 !strcmp(stdin_گهڻائي, "rev"))
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
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	ثابت حرف *format = عدم;
	تعداد stdin_گهڻائي stdin_گهڻائي;
	ثابت حرف *stdin_گهڻائي_arg = عدم;
	بنية format_nul_معلومات nul_معلومات = { 0, 0 };
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
		OPT_STRING(0, "stdin-mode", &stdin_گهڻائي_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_معلومات, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_معلومات.nul_داخلا,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_معلومات.nul_output,
			 N_("Use NUL for output termination")),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, اڳياڙي, opts, format_rev_usage, 0);

	إذا (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	إذا (!format)
		die(_("'%s' is required"), "--format");
	إذا (!stdin_گهڻائي_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_معلومات.nul_داخلا ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_معلومات.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_گهڻائي = parse_stdin_گهڻائي(stdin_گهڻائي_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_گهڻائي_explicit = format_rev.date_گهڻائي_explicit;
	format_pp.ctx.date_گهڻائي = format_rev.date_گهڻائي;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_ڳوليو_requirements(format,
				     &format_pp.want);
	إذا (format_pp.want.notes) {
		صحيح ignore_show_notes = 0;
		بنية string_list_شي *n;

		for_each_string_list_شي(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		ڀريو_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	بدل (stdin_گهڻائي) {
	حالة متن_2:
		طالما (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_سٽ(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			لکو("%c", output_terminator);
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

			peeled = deref_ٽيگ(the_repository, object, scratch_buf.buf, 0);
			إذا (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				استمر;
			}

			get_format_rev((بنية commit *)peeled,
				       &format_pp, &scratch_buf);
			لکو("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		اكسر;
	افتراضي:
		BUG("uncovered case: %d", stdin_گهڻائي);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	أرجع 0;
}
