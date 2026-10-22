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

সংরচনা rev_name {
	ধ্রুব বর্ণ *tip_name;
	timestamp_t taggerdate;
	পূর্ণাঙ্ক generation;
	পূর্ণাঙ্ক distance;
	পূর্ণাঙ্ক from_tag;
};

define_commit_slab(commit_rev_name, সংরচনা rev_name);

স্থির timestamp_t generation_cutoff = GENERATION_সংখ্যা_INFINITY;
স্থির timestamp_t cutoff = TIME_MAX;
স্থির সংরচনা commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
স্থির শূন্য disable_cutoff(শূন্য)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
স্থির শূন্য set_commit_cutoff(সংরচনা commit *commit)
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
স্থির পূর্ণাঙ্ক commit_is_before_cutoff(সংরচনা commit *commit)
{
	যদি (generation_cutoff < GENERATION_সংখ্যা_INFINITY)
		প্রত্যাগম generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	প্রত্যাগম commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

স্থির পূর্ণাঙ্ক is_valid_rev_name(ধ্রুব সংরচনা rev_name *name)
{
	প্রত্যাগম name && name->tip_name;
}

স্থির সংরচনা rev_name *get_commit_rev_name(ধ্রুব সংরচনা commit *commit)
{
	সংরচনা rev_name *name = commit_rev_name_peek(&rev_names, commit);

	প্রত্যাগম is_valid_rev_name(name) ? name : রিক্ত;
}

স্থির পূর্ণাঙ্ক effective_distance(পূর্ণাঙ্ক distance, পূর্ণাঙ্ক generation)
{
	প্রত্যাগম distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

স্থির পূর্ণাঙ্ক is_better_name(সংরচনা rev_name *name,
			  timestamp_t taggerdate,
			  পূর্ণাঙ্ক generation,
			  পূর্ণাঙ্ক distance,
			  পূর্ণাঙ্ক from_tag)
{
	পূর্ণাঙ্ক name_distance = effective_distance(name->distance, name->generation);
	পূর্ণাঙ্ক new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	যদি (from_tag && name->from_tag)
		প্রত্যাগম name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	যদি (name->from_tag != from_tag)
		প্রত্যাগম from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	যদি (name_distance != new_distance)
		প্রত্যাগম name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	যদি (name->taggerdate != taggerdate)
		প্রত্যাগম name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	প্রত্যাগম 0;
}

স্থির সংরচনা rev_name *create_or_update_name(সংরচনা commit *commit,
					      timestamp_t taggerdate,
					      পূর্ণাঙ্ক generation, পূর্ণাঙ্ক distance,
					      পূর্ণাঙ্ক from_tag)
{
	সংরচনা rev_name *name = commit_rev_name_at(&rev_names, commit);

	যদি (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		প্রত্যাগম রিক্ত;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	প্রত্যাগম name;
}

স্থির বর্ণ *get_parent_name(ধ্রুব সংরচনা rev_name *name, পূর্ণাঙ্ক parent_সংখ্যা,
			     সংরচনা mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	যদি (name->generation > 0) {
		প্রত্যাগম mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (পূর্ণাঙ্ক)len, name->tip_name,
				       name->generation, parent_সংখ্যা);
	} অন্যথা {
		প্রত্যাগম mem_pool_strfmt(string_pool, "%.*s^%d",
				       (পূর্ণাঙ্ক)len, name->tip_name, parent_সংখ্যা);
	}
}

স্থির শূন্য name_rev(সংরচনা commit *start_commit,
		ধ্রুব বর্ণ *tip_name, timestamp_t taggerdate,
		পূর্ণাঙ্ক from_tag, পূর্ণাঙ্ক deref, সংরচনা mem_pool *string_pool)
{
	সংরচনা commit_স্তূপ স্তূপ = COMMIT_স্তূপ_INIT;
	সংরচনা commit *commit;
	সংরচনা commit_স্তূপ parents_to_queue = COMMIT_স্তূপ_INIT;
	সংরচনা rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	যদি (commit_is_before_cutoff(start_commit))
		প্রত্যাগম;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	যদি (!start_name)
		প্রত্যাগম;
	যদি (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	অন্যথা
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_স্তূপ_ঠেলো(&স্তূপ, start_commit);

	যাবত্ ((commit = commit_স্তূপ_বের_করো(&স্তূপ))) {
		সংরচনা rev_name *name = get_commit_rev_name(commit);
		সংরচনা commit_list *parents;
		পূর্ণাঙ্ক parent_সংখ্যা = 1;

		parents_to_queue.nr = 0;

		প্রতি (parents = commit->parents;
				parents;
				parents = parents->next, parent_সংখ্যা++) {
			সংরচনা commit *parent = parents->item;
			সংরচনা rev_name *parent_name;
			পূর্ণাঙ্ক generation, distance;

			repo_parse_commit(the_repository, parent);
			যদি (commit_is_before_cutoff(parent))
				অনুবর্ত;

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
		যাবত্ (parents_to_queue.nr)
			commit_স্তূপ_ঠেলো(&স্তূপ,
					  commit_স্তূপ_বের_করো(&parents_to_queue));
	}

	commit_স্তূপ_clear(&স্তূপ);
	commit_স্তূপ_clear(&parents_to_queue);
}

স্থির পূর্ণাঙ্ক subpath_matches(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *filter)
{
	ধ্রুব বর্ণ *subpath = path;

	যাবত্ (subpath) {
		যদি (!wildmatch(filter, subpath, 0))
			প্রত্যাগম subpath - path;
		subpath = strchr(subpath, '/');
		যদি (subpath)
			subpath++;
	}
	প্রত্যাগম -1;
}

সংরচনা name_ref_data {
	পূর্ণাঙ্ক tags_only;
	পূর্ণাঙ্ক name_only;
	সংরচনা string_list ref_filters;
	সংরচনা string_list exclude_filters;
};

সংরচনা pretty_format {
	সংরচনা pretty_print_context ctx;
	সংরচনা userformat_want want;
};

গণনা command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

গণনা stdin_mode {
    TEXT = 1,
    REVS = 2,
};

সংরচনা command {
	গণনা command_type type;
	সঙ্ঘ {
		পূর্ণাঙ্ক name_only;
		সংরচনা pretty_format *pretty_format;
	} u;
};

স্থির শূন্য init_name_rev_command(সংরচনা command *cmd,
				  পূর্ণাঙ্ক name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

স্থির শূন্য init_format_rev_command(সংরচনা command *cmd,
				    সংরচনা pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

স্থির সংরচনা tip_table {
	সংরচনা tip_table_entry {
		সংরচনা object_id oid;
		ধ্রুব বর্ণ *refname;
		সংরচনা commit *commit;
		timestamp_t taggerdate;
		unsigned পূর্ণাঙ্ক from_tag:1;
		unsigned পূর্ণাঙ্ক deref:1;
	} *table;
	পূর্ণাঙ্ক nr;
	পূর্ণাঙ্ক alloc;
	পূর্ণাঙ্ক sorted;
} tip_table;

স্থির শূন্য add_to_tip_table(ধ্রুব সংরচনা object_id *oid, ধ্রুব বর্ণ *refname,
			     পূর্ণাঙ্ক shorten_unambiguous, সংরচনা commit *commit,
			     timestamp_t taggerdate, পূর্ণাঙ্ক from_tag, পূর্ণাঙ্ক deref)
{
	বর্ণ *short_refname = রিক্ত;

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

স্থির পূর্ণাঙ্ক tipcmp(ধ্রুব শূন্য *a_, ধ্রুব শূন্য *b_)
{
	ধ্রুব সংরচনা tip_table_entry *a = a_, *b = b_;
	প্রত্যাগম oidcmp(&a->oid, &b->oid);
}

স্থির পূর্ণাঙ্ক cmp_by_tag_and_age(ধ্রুব শূন্য *a_, ধ্রুব শূন্য *b_)
{
	ধ্রুব সংরচনা tip_table_entry *a = a_, *b = b_;
	পূর্ণাঙ্ক cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	যদি (cmp)
		প্রত্যাগম cmp;

	/* Older is better. */
	যদি (a->taggerdate < b->taggerdate)
		প্রত্যাগম -1;
	প্রত্যাগম a->taggerdate != b->taggerdate;
}

স্থির পূর্ণাঙ্ক name_ref(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা object *o = parse_object(the_repository, ref->oid);
	সংরচনা name_ref_data *data = cb_data;
	পূর্ণাঙ্ক can_abbreviate_output = data->tags_only && data->name_only;
	পূর্ণাঙ্ক deref = 0;
	পূর্ণাঙ্ক from_tag = 0;
	সংরচনা commit *commit = রিক্ত;
	timestamp_t taggerdate = TIME_MAX;

	যদি (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		প্রত্যাগম 0;

	যদি (data->exclude_filters.nr) {
		সংরচনা string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			যদি (subpath_matches(ref->name, item->string) >= 0)
				প্রত্যাগম 0;
		}
	}

	যদি (data->ref_filters.nr) {
		সংরচনা string_list_item *item;
		পূর্ণাঙ্ক matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_item(item, &data->ref_filters) {
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
			বিকল্প (subpath_matches(ref->name, item->string)) {
			পক্ষ -1: /* did not match */
				ভঙ্গ;
			পক্ষ 0: /* matched fully */
				matched = 1;
				ভঙ্গ;
			স্বভাব: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				ভঙ্গ;
			}
		}

		/* If none of the patterns matched, stop now */
		যদি (!matched)
			প্রত্যাগম 0;
	}

	যাবত্ (o && o->type == OBJ_TAG) {
		সংরচনা tag *t = (সংরচনা tag *) o;
		যদি (!t->tagged)
			ভঙ্গ; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	যদি (o && o->type == OBJ_COMMIT) {
		commit = (সংরচনা commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		যদি (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	প্রত্যাগম 0;
}

স্থির শূন্য name_tips(সংরচনা mem_pool *string_pool)
{
	পূর্ণাঙ্ক i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	প্রতি (i = 0; i < tip_table.nr; i++) {
		সংরচনা tip_table_entry *e = &tip_table.table[i];
		যদি (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

স্থির ধ্রুব সংরচনা object_id *nth_tip_table_ent(size_t ix, ধ্রুব শূন্য *table_)
{
	ধ্রুব সংরচনা tip_table_entry *table = table_;
	প্রত্যাগম &table[ix].oid;
}

স্থির ধ্রুব বর্ণ *get_exact_ref_match(ধ্রুব সংরচনা object *o)
{
	পূর্ণাঙ্ক found;

	যদি (!tip_table.table || !tip_table.nr)
		প্রত্যাগম রিক্ত;

	যদি (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	যদি (0 <= found)
		প্রত্যাগম tip_table.table[found].refname;
	প্রত্যাগম রিক্ত;
}

/* may return a constant string or use "buf" as scratch space */
স্থির ধ্রুব বর্ণ *get_rev_name(ধ্রুব সংরচনা object *o, সংরচনা strbuf *buf)
{
	সংরচনা rev_name *n;
	ধ্রুব সংরচনা commit *c;

	যদি (o->type != OBJ_COMMIT)
		প্রত্যাগম get_exact_ref_match(o);
	c = (ধ্রুব সংরচনা commit *) o;
	n = get_commit_rev_name(c);
	যদি (!n)
		প্রত্যাগম রিক্ত;

	যদি (!n->generation) {
		প্রত্যাগম n->tip_name;
	} অন্যথা {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		প্রত্যাগম buf->buf;
	}
}

স্থির ধ্রুব বর্ণ *get_format_rev(ধ্রুব সংরচনা commit *c,
				  সংরচনা pretty_format *format_ctx,
				  সংরচনা strbuf *buf)
{
	strbuf_reset(buf);

	যদি (format_ctx->want.notes) {
		সংরচনা strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, রিক্ত);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	প্রত্যাগম buf->buf;
}

স্থির শূন্য show_name(ধ্রুব সংরচনা object *obj,
		      ধ্রুব বর্ণ *caller_name,
		      পূর্ণাঙ্ক always, পূর্ণাঙ্ক allow_undefined, পূর্ণাঙ্ক name_only)
{
	ধ্রুব বর্ণ *name;
	ধ্রুব সংরচনা object_id *oid = &obj->oid;
	সংরচনা strbuf buf = STRBUF_INIT;

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

স্থির বর্ণ ধ্রুব * ধ্রুব name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	রিক্ত
};

স্থির শূন্য name_rev_সারি(বর্ণ *p, সংরচনা command *cmd)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	পূর্ণাঙ্ক counter = 0;
	বর্ণ *p_start;
	ধ্রুব unsigned hexsz = the_hash_algo->hexsz;

	প্রতি (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		যদি (!ishex(*p)) {
			counter = 0;
		} অন্যথা যদি (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			সংরচনা object_id oid;
			ধ্রুব বর্ণ *name = রিক্ত;
			বর্ণ c = *(p + 1);
			পূর্ণাঙ্ক p_len = p - p_start + 1;
			সংরচনা object *o = রিক্ত;
			পূর্ণাঙ্ক oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			বিকল্প (cmd->type) {
			পক্ষ NAME_REV:
				যদি (!oid_ret)
					o = lookup_object(the_repository, &oid);
				যদি (o)
					name = get_rev_name(o, &buf);
				যদি (!name)
					অনুবর্ত;
				যদি (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				অন্যথা
					printf("%.*s (%s)", p_len, p_start, name);
				ভঙ্গ;
			পক্ষ FORMAT_REV:
				যদি (!oid_ret)
					o = parse_object(the_repository, &oid);
				যদি (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ধ্রুব সংরচনা commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				যদি (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				অন্যথা
					printf("%.*s", p_len, p_start);
				ভঙ্গ;
			স্বভাব:
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

পূর্ণাঙ্ক cmd_name_rev(পূর্ণাঙ্ক argc,
		 ধ্রুব বর্ণ **argv,
		 ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	সংরচনা mem_pool string_pool;
	সংরচনা object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	পূর্ণাঙ্ক transform_stdin = 0;
#endif
	পূর্ণাঙ্ক all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	সংরচনা name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	সংরচনা command cmd;
	সংরচনা option opts[] = {
		OPT_BOOL(0, "name-only", &data.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &data.tags_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &data.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &data.exclude_filters, N_("pattern"),
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
	repo_config(the_repository, git_default_config, রিক্ত);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

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

	প্রতি (; argc; argc--, argv++) {
		সংরচনা object_id oid;
		সংরচনা object *object;
		সংরচনা commit *commit;

		যদি (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			অনুবর্ত;
		}

		commit = রিক্ত;
		object = parse_object(the_repository, &oid);
		যদি (object) {
			সংরচনা object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			যদি (peeled && peeled->type == OBJ_COMMIT)
				commit = (সংরচনা commit *)peeled;
		}

		যদি (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			অনুবর্ত;
		}

		যদি (commit)
			set_commit_cutoff(commit);

		যদি (peel_tag) {
			যদি (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				অনুবর্ত;
			}
			object = (সংরচনা object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	যদি (annotate_stdin) {
		সংরচনা strbuf sb = STRBUF_INIT;

		যাবত্ (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_সারি(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} অন্যথা যদি (all) {
		পূর্ণাঙ্ক i, max;

		max = get_max_object_index(the_repository);
		প্রতি (i = 0; i < max; i++) {
			সংরচনা object *obj = get_indexed_object(the_repository, i);
			যদি (!obj || obj->type != OBJ_COMMIT)
				অনুবর্ত;
			show_name(obj, রিক্ত,
				  always, allow_undefined, data.name_only);
		}
	} অন্যথা {
		পূর্ণাঙ্ক i;
		প্রতি (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	প্রত্যাগম 0;
}

সংরচনা format_nul_data {
	bool nul_প্রবেশ;
	bool nul_output;
};

স্থির পূর্ণাঙ্ক format_nul_cb(ধ্রুব সংরচনা option *option,
			 ধ্রুব বর্ণ *arg,
			 পূর্ণাঙ্ক unset)
{
	সংরচনা format_nul_data *data = option->মান;
	data->nul_প্রবেশ = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	প্রত্যাগম 0;
}

স্থির গণনা stdin_mode parse_stdin_mode(ধ্রুব বর্ণ *stdin_mode)
{
	যদি (!strcmp(stdin_mode, "text"))
		প্রত্যাগম TEXT;
	অন্যথা যদি (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		প্রত্যাগম REVS;
	অন্যথা
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

স্থির বর্ণ ধ্রুব *ধ্রুব format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	রিক্ত
};

পূর্ণাঙ্ক cmd_format_rev(পূর্ণাঙ্ক argc,
		   ধ্রুব বর্ণ **argv,
		   ধ্রুব বর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	ধ্রুব বর্ণ *format = রিক্ত;
	গণনা stdin_mode stdin_mode;
	ধ্রুব বর্ণ *stdin_mode_arg = রিক্ত;
	সংরচনা format_nul_data nul_data = { 0, 0 };
	বর্ণ output_terminator;
	strbuf_getline_fn getline_fn;
	সংরচনা display_notes_opt format_notes_opt;
	সংরচনা rev_info format_rev = REV_INFO_INIT;
	সংরচনা pretty_format format_pp = { 0 };
	সংরচনা string_list notes = STRING_LIST_INIT_NODUP;
	সংরচনা strbuf scratch_buf = STRBUF_INIT;
	সংরচনা command cmd;
	সংরচনা option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_প্রবেশ,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
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

	getline_fn = nul_data.nul_প্রবেশ ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_data.nul_output ? '\0' : '\n';

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
		পূর্ণাঙ্ক ignore_show_notes = 0;
		সংরচনা string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	বিকল্প (stdin_mode) {
	পক্ষ TEXT:
		যাবত্ (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_সারি(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		ভঙ্গ;
	পক্ষ REVS:
		যাবত্ (getline_fn(&scratch_buf, stdin) != EOF) {
			সংরচনা object_id oid;
			সংরচনা object *object;
			সংরচনা object *peeled;

			যদি (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				অনুবর্ত;
			}

			object = parse_object(the_repository, &oid);
			যদি (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				অনুবর্ত;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			যদি (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				অনুবর্ত;
			}

			get_format_rev((সংরচনা commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		ভঙ্গ;
	স্বভাব:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	প্রত্যাগম 0;
}
