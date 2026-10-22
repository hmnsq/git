#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "pager.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "hash-lookup.h"
#অন্তর্ভাব "commit-slab.h"
#অন্তর্ভাব "commit-graph.h"
#অন্তর্ভাব "wildmatch.h"
#অন্তর্ভাব "mem-pool.h"
#অন্তর্ভাব "pretty.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "notes.h"
#অন্তর্ভাব "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

কাঠামো rev_name {
	ধ্রুবক অক্ষর *tip_name;
	timestamp_t taggerdate;
	পূর্ণসংখ্যা generation;
	পূর্ণসংখ্যা distance;
	পূর্ণসংখ্যা from_tag;
};

define_commit_slab(commit_rev_name, কাঠামো rev_name);

স্থির timestamp_t generation_cutoff = GENERATION_সংখ্যা_INFINITY;
স্থির timestamp_t cutoff = TIME_MAX;
স্থির কাঠামো commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
স্থির শূন্য disable_cutoff(শূন্য)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
স্থির শূন্য set_commit_cutoff(কাঠামো commit *commit)
{

	যদি (cutoff > commit->date)
		cutoff = commit->date;

	যদি (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		যদি (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
স্থির শূন্য adjust_cutoff_timestamp_for_slop(শূন্য)
{
	যদি (cutoff) {
		/* check for underflow */
		যদি (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		অন্যথা
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
স্থির পূর্ণসংখ্যা commit_is_before_cutoff(কাঠামো commit *commit)
{
	যদি (generation_cutoff < GENERATION_সংখ্যা_INFINITY)
		ফেরত generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	ফেরত commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

স্থির পূর্ণসংখ্যা is_valid_rev_name(ধ্রুবক কাঠামো rev_name *name)
{
	ফেরত name && name->tip_name;
}

স্থির কাঠামো rev_name *get_commit_rev_name(ধ্রুবক কাঠামো commit *commit)
{
	কাঠামো rev_name *name = commit_rev_name_peek(&rev_names, commit);

	ফেরত is_valid_rev_name(name) ? name : ফাঁকা;
}

স্থির পূর্ণসংখ্যা effective_distance(পূর্ণসংখ্যা distance, পূর্ণসংখ্যা generation)
{
	ফেরত distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

স্থির পূর্ণসংখ্যা is_better_name(কাঠামো rev_name *name,
			  timestamp_t taggerdate,
			  পূর্ণসংখ্যা generation,
			  পূর্ণসংখ্যা distance,
			  পূর্ণসংখ্যা from_tag)
{
	পূর্ণসংখ্যা name_distance = effective_distance(name->distance, name->generation);
	পূর্ণসংখ্যা new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	যদি (from_tag && name->from_tag)
		ফেরত name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	যদি (name->from_tag != from_tag)
		ফেরত from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	যদি (name_distance != new_distance)
		ফেরত name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	যদি (name->taggerdate != taggerdate)
		ফেরত name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	ফেরত 0;
}

স্থির কাঠামো rev_name *create_or_update_name(কাঠামো commit *commit,
					      timestamp_t taggerdate,
					      পূর্ণসংখ্যা generation, পূর্ণসংখ্যা distance,
					      পূর্ণসংখ্যা from_tag)
{
	কাঠামো rev_name *name = commit_rev_name_at(&rev_names, commit);

	যদি (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		ফেরত ফাঁকা;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	ফেরত name;
}

স্থির অক্ষর *get_parent_name(ধ্রুবক কাঠামো rev_name *name, পূর্ণসংখ্যা parent_সংখ্যা,
			     কাঠামো mem_pool *string_pool)
{
	আকার_t len;

	strip_suffix(name->tip_name, "^0", &len);
	যদি (name->generation > 0) {
		ফেরত mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (পূর্ণসংখ্যা)len, name->tip_name,
				       name->generation, parent_সংখ্যা);
	} অন্যথা {
		ফেরত mem_pool_strfmt(string_pool, "%.*s^%d",
				       (পূর্ণসংখ্যা)len, name->tip_name, parent_সংখ্যা);
	}
}

স্থির শূন্য name_rev(কাঠামো commit *start_commit,
		ধ্রুবক অক্ষর *tip_name, timestamp_t taggerdate,
		পূর্ণসংখ্যা from_tag, পূর্ণসংখ্যা deref, কাঠামো mem_pool *string_pool)
{
	কাঠামো commit_স্তূপ স্তূপ = COMMIT_স্তূপ_INIT;
	কাঠামো commit *commit;
	কাঠামো commit_স্তূপ parents_to_queue = COMMIT_স্তূপ_INIT;
	কাঠামো rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	যদি (commit_is_before_cutoff(start_commit))
		ফেরত;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	যদি (!start_name)
		ফেরত;
	যদি (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	অন্যথা
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_স্তূপ_ঠেলো(&স্তূপ, start_commit);

	যখন ((commit = commit_স্তূপ_বের_করো(&স্তূপ))) {
		কাঠামো rev_name *name = get_commit_rev_name(commit);
		কাঠামো commit_list *parents;
		পূর্ণসংখ্যা parent_সংখ্যা = 1;

		parents_to_queue.nr = 0;

		জন্য (parents = commit->parents;
				parents;
				parents = parents->পরবর্তী, parent_সংখ্যা++) {
			কাঠামো commit *parent = parents->item;
			কাঠামো rev_name *parent_name;
			পূর্ণসংখ্যা generation, distance;

			repo_parse_commit(the_repository, parent);
			যদি (commit_is_before_cutoff(parent))
				চালু;

			যদি (parent_সংখ্যা > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} অন্যথা {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			যদি (parent_name) {
				যদি (parent_সংখ্যা > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_সংখ্যা,
								string_pool);
				অন্যথা
					parent_name->tip_name = name->tip_name;
				commit_স্তূপ_ঠেলো(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		যখন (parents_to_queue.nr)
			commit_স্তূপ_ঠেলো(&স্তূপ,
					  commit_স্তূপ_বের_করো(&parents_to_queue));
	}

	commit_স্তূপ_clear(&স্তূপ);
	commit_স্তূপ_clear(&parents_to_queue);
}

স্থির পূর্ণসংখ্যা subpath_matches(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *filter)
{
	ধ্রুবক অক্ষর *subpath = path;

	যখন (subpath) {
		যদি (!wildmatch(filter, subpath, 0))
			ফেরত subpath - path;
		subpath = strchr(subpath, '/');
		যদি (subpath)
			subpath++;
	}
	ফেরত -1;
}

কাঠামো name_ref_তথ্য {
	পূর্ণসংখ্যা tags_only;
	পূর্ণসংখ্যা name_only;
	কাঠামো string_list ref_filters;
	কাঠামো string_list exclude_filters;
};

কাঠামো pretty_format {
	কাঠামো pretty_print_context ctx;
	কাঠামো userformat_want want;
};

গণনা command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

গণনা stdin_mode {
    TEXT = 1,
    REVS = 2,
};

কাঠামো command {
	গণনা command_type type;
	সংঘ {
		পূর্ণসংখ্যা name_only;
		কাঠামো pretty_format *pretty_format;
	} u;
};

স্থির শূন্য init_name_rev_command(কাঠামো command *cmd,
				  পূর্ণসংখ্যা name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

স্থির শূন্য init_format_rev_command(কাঠামো command *cmd,
				    কাঠামো pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

স্থির কাঠামো tip_table {
	কাঠামো tip_table_entry {
		কাঠামো object_id oid;
		ধ্রুবক অক্ষর *refname;
		কাঠামো commit *commit;
		timestamp_t taggerdate;
		unsigned পূর্ণসংখ্যা from_tag:1;
		unsigned পূর্ণসংখ্যা deref:1;
	} *table;
	পূর্ণসংখ্যা nr;
	পূর্ণসংখ্যা alloc;
	পূর্ণসংখ্যা sorted;
} tip_table;

স্থির শূন্য add_to_tip_table(ধ্রুবক কাঠামো object_id *oid, ধ্রুবক অক্ষর *refname,
			     পূর্ণসংখ্যা shorten_unambiguous, কাঠামো commit *commit,
			     timestamp_t taggerdate, পূর্ণসংখ্যা from_tag, পূর্ণসংখ্যা deref)
{
	অক্ষর *short_refname = ফাঁকা;

	যদি (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	অন্যথা যদি (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	অন্যথা
		skip_prefix(refname, "refs/", &refname);

	ALLOC_GROW(tip_table.table, tip_table.nr + 1, tip_table.alloc);
	oidcpy(&tip_table.table[tip_table.nr].oid, oid);
	tip_table.table[tip_table.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_table.table[tip_table.nr].commit = commit;
	tip_table.table[tip_table.nr].taggerdate = taggerdate;
	tip_table.table[tip_table.nr].from_tag = from_tag;
	tip_table.table[tip_table.nr].deref = deref;
	tip_table.nr++;
	tip_table.sorted = 0;
}

স্থির পূর্ণসংখ্যা tipcmp(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	ধ্রুবক কাঠামো tip_table_entry *a = a_, *b = b_;
	ফেরত oidcmp(&a->oid, &b->oid);
}

স্থির পূর্ণসংখ্যা cmp_by_tag_and_age(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	ধ্রুবক কাঠামো tip_table_entry *a = a_, *b = b_;
	পূর্ণসংখ্যা cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	যদি (cmp)
		ফেরত cmp;

	/* Older is better. */
	যদি (a->taggerdate < b->taggerdate)
		ফেরত -1;
	ফেরত a->taggerdate != b->taggerdate;
}

স্থির পূর্ণসংখ্যা name_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	কাঠামো object *o = parse_object(the_repository, ref->oid);
	কাঠামো name_ref_তথ্য *তথ্য = cb_তথ্য;
	পূর্ণসংখ্যা can_abbreviate_output = তথ্য->tags_only && তথ্য->name_only;
	পূর্ণসংখ্যা deref = 0;
	পূর্ণসংখ্যা from_tag = 0;
	কাঠামো commit *commit = ফাঁকা;
	timestamp_t taggerdate = TIME_MAX;

	যদি (তথ্য->tags_only && !starts_with(ref->name, "refs/tags/"))
		ফেরত 0;

	যদি (তথ্য->exclude_filters.nr) {
		কাঠামো string_list_item *item;

		for_each_string_list_item(item, &তথ্য->exclude_filters) {
			যদি (subpath_matches(ref->name, item->string) >= 0)
				ফেরত 0;
		}
	}

	যদি (তথ্য->ref_filters.nr) {
		কাঠামো string_list_item *item;
		পূর্ণসংখ্যা matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_item(item, &তথ্য->ref_filters) {
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
			নির্বাচন (subpath_matches(ref->name, item->string)) {
			ক্ষেত্র -1: /* did not match */
				বিরতি;
			ক্ষেত্র 0: /* matched fully */
				matched = 1;
				বিরতি;
			পূর্বনির্ধারিত: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				বিরতি;
			}
		}

		/* If none of the patterns matched, stop now */
		যদি (!matched)
			ফেরত 0;
	}

	যখন (o && o->type == OBJ_TAG) {
		কাঠামো tag *t = (কাঠামো tag *) o;
		যদি (!t->tagged)
			বিরতি; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	যদি (o && o->type == OBJ_COMMIT) {
		commit = (কাঠামো commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		যদি (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	ফেরত 0;
}

স্থির শূন্য name_tips(কাঠামো mem_pool *string_pool)
{
	পূর্ণসংখ্যা i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	জন্য (i = 0; i < tip_table.nr; i++) {
		কাঠামো tip_table_entry *e = &tip_table.table[i];
		যদি (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

স্থির ধ্রুবক কাঠামো object_id *nth_tip_table_ent(আকার_t ix, ধ্রুবক শূন্য *table_)
{
	ধ্রুবক কাঠামো tip_table_entry *table = table_;
	ফেরত &table[ix].oid;
}

স্থির ধ্রুবক অক্ষর *get_exact_ref_match(ধ্রুবক কাঠামো object *o)
{
	পূর্ণসংখ্যা found;

	যদি (!tip_table.table || !tip_table.nr)
		ফেরত ফাঁকা;

	যদি (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	যদি (0 <= found)
		ফেরত tip_table.table[found].refname;
	ফেরত ফাঁকা;
}

/* may return a constant string or use "buf" as scratch space */
স্থির ধ্রুবক অক্ষর *get_rev_name(ধ্রুবক কাঠামো object *o, কাঠামো strbuf *buf)
{
	কাঠামো rev_name *n;
	ধ্রুবক কাঠামো commit *c;

	যদি (o->type != OBJ_COMMIT)
		ফেরত get_exact_ref_match(o);
	c = (ধ্রুবক কাঠামো commit *) o;
	n = get_commit_rev_name(c);
	যদি (!n)
		ফেরত ফাঁকা;

	যদি (!n->generation) {
		ফেরত n->tip_name;
	} অন্যথা {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		ফেরত buf->buf;
	}
}

স্থির ধ্রুবক অক্ষর *get_format_rev(ধ্রুবক কাঠামো commit *c,
				  কাঠামো pretty_format *format_ctx,
				  কাঠামো strbuf *buf)
{
	strbuf_reset(buf);

	যদি (format_ctx->want.notes) {
		কাঠামো strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, ফাঁকা);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	ফেরত buf->buf;
}

স্থির শূন্য show_name(ধ্রুবক কাঠামো object *obj,
		      ধ্রুবক অক্ষর *caller_name,
		      পূর্ণসংখ্যা always, পূর্ণসংখ্যা allow_undefined, পূর্ণসংখ্যা name_only)
{
	ধ্রুবক অক্ষর *name;
	ধ্রুবক কাঠামো object_id *oid = &obj->oid;
	কাঠামো strbuf buf = STRBUF_INIT;

	যদি (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	যদি (name)
		printf("%s\n", name);
	অন্যথা যদি (allow_undefined)
		printf("undefined\n");
	অন্যথা যদি (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	অন্যথা
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

স্থির অক্ষর ধ্রুবক * ধ্রুবক name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	ফাঁকা
};

স্থির শূন্য name_rev_সারি(অক্ষর *p, কাঠামো command *cmd)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	পূর্ণসংখ্যা গণক = 0;
	অক্ষর *p_start;
	ধ্রুবক unsigned hexsz = the_hash_algo->hexsz;

	জন্য (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		যদি (!ishex(*p)) {
			গণক = 0;
		} অন্যথা যদি (++গণক == hexsz &&
			   !ishex(*(p + 1))) {
			কাঠামো object_id oid;
			ধ্রুবক অক্ষর *name = ফাঁকা;
			অক্ষর c = *(p + 1);
			পূর্ণসংখ্যা p_len = p - p_start + 1;
			কাঠামো object *o = ফাঁকা;
			পূর্ণসংখ্যা oid_ret = 1;

			গণক = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			নির্বাচন (cmd->type) {
			ক্ষেত্র NAME_REV:
				যদি (!oid_ret)
					o = lookup_object(the_repository, &oid);
				যদি (o)
					name = get_rev_name(o, &buf);
				যদি (!name)
					চালু;
				যদি (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				অন্যথা
					printf("%.*s (%s)", p_len, p_start, name);
				বিরতি;
			ক্ষেত্র FORMAT_REV:
				যদি (!oid_ret)
					o = parse_object(the_repository, &oid);
				যদি (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ধ্রুবক কাঠামো commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				যদি (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				অন্যথা
					printf("%.*s", p_len, p_start);
				বিরতি;
			পূর্বনির্ধারিত:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	যদি (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

পূর্ণসংখ্যা cmd_name_rev(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo UNUSED)
{
	কাঠামো mem_pool string_pool;
	কাঠামো object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	পূর্ণসংখ্যা transform_stdin = 0;
#endif
	পূর্ণসংখ্যা all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	কাঠামো name_ref_তথ্য তথ্য = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	কাঠামো command cmd;
	কাঠামো option opts[] = {
		OPT_BOOL(0, "name-only", &তথ্য.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &তথ্য.tags_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &তথ্য.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &তথ্য.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_GROUP(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#ifndef WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#endif /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_tag,
				N_("dereference tags in the input (internal use)")),
		OPT_END(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_names);
	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, তথ্য.name_only);

#ifndef WITH_BREAKING_CHANGES
	যদি (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	যদি (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	যদি (all || annotate_stdin)
		disable_cutoff();

	জন্য (; argc; argc--, argv++) {
		কাঠামো object_id oid;
		কাঠামো object *object;
		কাঠামো commit *commit;

		যদি (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			চালু;
		}

		commit = ফাঁকা;
		object = parse_object(the_repository, &oid);
		যদি (object) {
			কাঠামো object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			যদি (peeled && peeled->type == OBJ_COMMIT)
				commit = (কাঠামো commit *)peeled;
		}

		যদি (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			চালু;
		}

		যদি (commit)
			set_commit_cutoff(commit);

		যদি (peel_tag) {
			যদি (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				চালু;
			}
			object = (কাঠামো object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &তথ্য);
	name_tips(&string_pool);

	যদি (annotate_stdin) {
		কাঠামো strbuf sb = STRBUF_INIT;

		যখন (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_সারি(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} অন্যথা যদি (all) {
		পূর্ণসংখ্যা i, max;

		max = get_max_object_সূচক(the_repository);
		জন্য (i = 0; i < max; i++) {
			কাঠামো object *obj = get_indexed_object(the_repository, i);
			যদি (!obj || obj->type != OBJ_COMMIT)
				চালু;
			show_name(obj, ফাঁকা,
				  always, allow_undefined, তথ্য.name_only);
		}
	} অন্যথা {
		পূর্ণসংখ্যা i;
		জন্য (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, তথ্য.name_only);
	}

	string_list_clear(&তথ্য.ref_filters, 0);
	string_list_clear(&তথ্য.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	ফেরত 0;
}

কাঠামো format_nul_তথ্য {
	bool nul_প্রবেশ;
	bool nul_output;
};

স্থির পূর্ণসংখ্যা format_nul_cb(ধ্রুবক কাঠামো option *option,
			 ধ্রুবক অক্ষর *arg,
			 পূর্ণসংখ্যা unset)
{
	কাঠামো format_nul_তথ্য *তথ্য = option->মান;
	তথ্য->nul_প্রবেশ = 1;
	তথ্য->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ফেরত 0;
}

স্থির গণনা stdin_mode parse_stdin_mode(ধ্রুবক অক্ষর *stdin_mode)
{
	যদি (!strcmp(stdin_mode, "text"))
		ফেরত TEXT;
	অন্যথা যদি (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		ফেরত REVS;
	অন্যথা
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

স্থির অক্ষর ধ্রুবক *ধ্রুবক format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	ফাঁকা
};

পূর্ণসংখ্যা cmd_format_rev(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর *format = ফাঁকা;
	গণনা stdin_mode stdin_mode;
	ধ্রুবক অক্ষর *stdin_mode_arg = ফাঁকা;
	কাঠামো format_nul_তথ্য nul_তথ্য = { 0, 0 };
	অক্ষর output_terminator;
	strbuf_getline_fn getline_fn;
	কাঠামো display_notes_opt format_notes_opt;
	কাঠামো rev_info format_rev = REV_INFO_INIT;
	কাঠামো pretty_format format_pp = { 0 };
	কাঠামো string_list notes = STRING_LIST_INIT_NODUP;
	কাঠামো strbuf scratch_buf = STRBUF_INIT;
	কাঠামো command cmd;
	কাঠামো option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_তথ্য, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_তথ্য.nul_প্রবেশ,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_তথ্য.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	যদি (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	যদি (!format)
		die(_("'%s' is required"), "--format");
	যদি (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_তথ্য.nul_প্রবেশ ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_তথ্য.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_mode = parse_stdin_mode(stdin_mode_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_mode_explicit = format_rev.date_mode_explicit;
	format_pp.ctx.date_mode = format_rev.date_mode;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_find_requirements(format,
				     &format_pp.want);
	যদি (format_pp.want.notes) {
		পূর্ণসংখ্যা ignore_show_notes = 0;
		কাঠামো string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	নির্বাচন (stdin_mode) {
	ক্ষেত্র TEXT:
		যখন (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_সারি(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		বিরতি;
	ক্ষেত্র REVS:
		যখন (getline_fn(&scratch_buf, stdin) != EOF) {
			কাঠামো object_id oid;
			কাঠামো object *object;
			কাঠামো object *peeled;

			যদি (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				চালু;
			}

			object = parse_object(the_repository, &oid);
			যদি (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				চালু;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			যদি (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				চালু;
			}

			get_format_rev((কাঠামো commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	ফেরত 0;
}
