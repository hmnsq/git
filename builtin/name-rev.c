#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "config.h"
#include "commit.h"
#include "tag.h"
#include "refs.h"
#include "object-name.h"
#include "pager.h"
#include "parse-options.h"
#include "hash-lookup.h"
#include "commit-slab.h"
#include "commit-graph.h"
#include "wildmatch.h"
#include "mem-pool.h"
#include "pretty.h"
#include "revision.h"
#include "notes.h"
#include "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

sa_nracanaa rev_name {
	const var_na *tip_name;
	timestamp_t taggerdate;
	puur_naa_nka generation;
	puur_naa_nka distance;
	puur_naa_nka from_tag;
};

define_commit_slab(commit_rev_name, sa_nracanaa rev_name);

static timestamp_t generation_cutoff = GENERATION_NUMBER_INFINITY;
static timestamp_t cutoff = TIME_MAX;
static sa_nracanaa commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
static void disable_cutoff(void)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
static void set_commit_cutoff(sa_nracanaa commit *commit)
{

	ꦪꦼꦤ꧀ (cutoff > commit->date)
		cutoff = commit->date;

	ꦪꦼꦤ꧀ (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		ꦪꦼꦤ꧀ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
static void adjust_cutoff_timestamp_for_slop(void)
{
	ꦪꦼꦤ꧀ (cutoff) {
		/* check for underflow */
		ꦪꦼꦤ꧀ (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		ꦭꦶꦪ
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
static puur_naa_nka commit_is_before_cutoff(sa_nracanaa commit *commit)
{
	ꦪꦼꦤ꧀ (generation_cutoff < GENERATION_NUMBER_INFINITY)
		ꦮꦭꦶ generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	ꦮꦭꦶ commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

static puur_naa_nka is_valid_rev_name(const sa_nracanaa rev_name *name)
{
	ꦮꦭꦶ name && name->tip_name;
}

static sa_nracanaa rev_name *get_commit_rev_name(const sa_nracanaa commit *commit)
{
	sa_nracanaa rev_name *name = commit_rev_name_peek(&rev_names, commit);

	ꦮꦭꦶ is_valid_rev_name(name) ? name : NULL;
}

static puur_naa_nka effective_distance(puur_naa_nka distance, puur_naa_nka generation)
{
	ꦮꦭꦶ distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

static puur_naa_nka is_better_name(sa_nracanaa rev_name *name,
			  timestamp_t taggerdate,
			  puur_naa_nka generation,
			  puur_naa_nka distance,
			  puur_naa_nka from_tag)
{
	puur_naa_nka name_distance = effective_distance(name->distance, name->generation);
	puur_naa_nka new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	ꦪꦼꦤ꧀ (from_tag && name->from_tag)
		ꦮꦭꦶ name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	ꦪꦼꦤ꧀ (name->from_tag != from_tag)
		ꦮꦭꦶ from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	ꦪꦼꦤ꧀ (name_distance != new_distance)
		ꦮꦭꦶ name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	ꦪꦼꦤ꧀ (name->taggerdate != taggerdate)
		ꦮꦭꦶ name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	ꦮꦭꦶ 0;
}

static sa_nracanaa rev_name *create_or_update_name(sa_nracanaa commit *commit,
					      timestamp_t taggerdate,
					      puur_naa_nka generation, puur_naa_nka distance,
					      puur_naa_nka from_tag)
{
	sa_nracanaa rev_name *name = commit_rev_name_at(&rev_names, commit);

	ꦪꦼꦤ꧀ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		ꦮꦭꦶ NULL;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	ꦮꦭꦶ name;
}

static var_na *get_parent_name(const sa_nracanaa rev_name *name, puur_naa_nka parent_number,
			     sa_nracanaa mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	ꦪꦼꦤ꧀ (name->generation > 0) {
		ꦮꦭꦶ mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (puur_naa_nka)len, name->tip_name,
				       name->generation, parent_number);
	} ꦭꦶꦪ {
		ꦮꦭꦶ mem_pool_strfmt(string_pool, "%.*s^%d",
				       (puur_naa_nka)len, name->tip_name, parent_number);
	}
}

static void name_rev(sa_nracanaa commit *start_commit,
		const var_na *tip_name, timestamp_t taggerdate,
		puur_naa_nka from_tag, puur_naa_nka deref, sa_nracanaa mem_pool *string_pool)
{
	sa_nracanaa commit_stack stack = COMMIT_STACK_INIT;
	sa_nracanaa commit *commit;
	sa_nracanaa commit_stack parents_to_queue = COMMIT_STACK_INIT;
	sa_nracanaa rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	ꦪꦼꦤ꧀ (commit_is_before_cutoff(start_commit))
		ꦮꦭꦶ;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	ꦪꦼꦤ꧀ (!start_name)
		ꦮꦭꦶ;
	ꦪꦼꦤ꧀ (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	ꦭꦶꦪ
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_stack_push(&stack, start_commit);

	ꦱꦼꦢꦼꦭ ((commit = commit_stack_pop(&stack))) {
		sa_nracanaa rev_name *name = get_commit_rev_name(commit);
		sa_nracanaa commit_list *parents;
		puur_naa_nka parent_number = 1;

		parents_to_queue.nr = 0;

		ꦏꦁꦒꦺꦴ (parents = commit->parents;
				parents;
				parents = parents->next, parent_number++) {
			sa_nracanaa commit *parent = parents->item;
			sa_nracanaa rev_name *parent_name;
			puur_naa_nka generation, distance;

			repo_parse_commit(the_repository, parent);
			ꦪꦼꦤ꧀ (commit_is_before_cutoff(parent))
				continue;

			ꦪꦼꦤ꧀ (parent_number > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} ꦭꦶꦪ {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			ꦪꦼꦤ꧀ (parent_name) {
				ꦪꦼꦤ꧀ (parent_number > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_number,
								string_pool);
				ꦭꦶꦪ
					parent_name->tip_name = name->tip_name;
				commit_stack_push(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		ꦱꦼꦢꦼꦭ (parents_to_queue.nr)
			commit_stack_push(&stack,
					  commit_stack_pop(&parents_to_queue));
	}

	commit_stack_clear(&stack);
	commit_stack_clear(&parents_to_queue);
}

static puur_naa_nka subpath_matches(const var_na *path, const var_na *filter)
{
	const var_na *subpath = path;

	ꦱꦼꦢꦼꦭ (subpath) {
		ꦪꦼꦤ꧀ (!wildmatch(filter, subpath, 0))
			ꦮꦭꦶ subpath - path;
		subpath = strchr(subpath, '/');
		ꦪꦼꦤ꧀ (subpath)
			subpath++;
	}
	ꦮꦭꦶ -1;
}

sa_nracanaa name_ref_data {
	puur_naa_nka tags_only;
	puur_naa_nka name_only;
	sa_nracanaa string_list ref_filters;
	sa_nracanaa string_list exclude_filters;
};

sa_nracanaa pretty_format {
	sa_nracanaa pretty_print_context ctx;
	sa_nracanaa userformat_want want;
};

ga_nanaa command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

ga_nanaa stdin_mode {
    TEXT = 1,
    REVS = 2,
};

sa_nracanaa command {
	ga_nanaa command_type type;
	sa_ngha {
		puur_naa_nka name_only;
		sa_nracanaa pretty_format *pretty_format;
	} u;
};

static void init_name_rev_command(sa_nracanaa command *cmd,
				  puur_naa_nka name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

static void init_format_rev_command(sa_nracanaa command *cmd,
				    sa_nracanaa pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

static sa_nracanaa tip_table {
	sa_nracanaa tip_table_entry {
		sa_nracanaa object_id oid;
		const var_na *refname;
		sa_nracanaa commit *commit;
		timestamp_t taggerdate;
		unsigned puur_naa_nka from_tag:1;
		unsigned puur_naa_nka deref:1;
	} *table;
	puur_naa_nka nr;
	puur_naa_nka alloc;
	puur_naa_nka sorted;
} tip_table;

static void add_to_tip_table(const sa_nracanaa object_id *oid, const var_na *refname,
			     puur_naa_nka shorten_unambiguous, sa_nracanaa commit *commit,
			     timestamp_t taggerdate, puur_naa_nka from_tag, puur_naa_nka deref)
{
	var_na *short_refname = NULL;

	ꦪꦼꦤ꧀ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	ꦭꦶꦪ ꦪꦼꦤ꧀ (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	ꦭꦶꦪ
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

static puur_naa_nka tipcmp(const void *a_, const void *b_)
{
	const sa_nracanaa tip_table_entry *a = a_, *b = b_;
	ꦮꦭꦶ oidcmp(&a->oid, &b->oid);
}

static puur_naa_nka cmp_by_tag_and_age(const void *a_, const void *b_)
{
	const sa_nracanaa tip_table_entry *a = a_, *b = b_;
	puur_naa_nka cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	ꦪꦼꦤ꧀ (cmp)
		ꦮꦭꦶ cmp;

	/* Older is better. */
	ꦪꦼꦤ꧀ (a->taggerdate < b->taggerdate)
		ꦮꦭꦶ -1;
	ꦮꦭꦶ a->taggerdate != b->taggerdate;
}

static puur_naa_nka name_ref(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa object *o = parse_object(the_repository, ref->oid);
	sa_nracanaa name_ref_data *data = cb_data;
	puur_naa_nka can_abbreviate_output = data->tags_only && data->name_only;
	puur_naa_nka deref = 0;
	puur_naa_nka from_tag = 0;
	sa_nracanaa commit *commit = NULL;
	timestamp_t taggerdate = TIME_MAX;

	ꦪꦼꦤ꧀ (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		ꦮꦭꦶ 0;

	ꦪꦼꦤ꧀ (data->exclude_filters.nr) {
		sa_nracanaa string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			ꦪꦼꦤ꧀ (subpath_matches(ref->name, item->string) >= 0)
				ꦮꦭꦶ 0;
		}
	}

	ꦪꦼꦤ꧀ (data->ref_filters.nr) {
		sa_nracanaa string_list_item *item;
		puur_naa_nka matched = 0;

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
			switch (subpath_matches(ref->name, item->string)) {
			pak_sa -1: /* did not match */
				bha_nga;
			pak_sa 0: /* matched fully */
				matched = 1;
				bha_nga;
			default: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				bha_nga;
			}
		}

		/* If none of the patterns matched, stop now */
		ꦪꦼꦤ꧀ (!matched)
			ꦮꦭꦶ 0;
	}

	ꦱꦼꦢꦼꦭ (o && o->type == OBJ_TAG) {
		sa_nracanaa tag *t = (sa_nracanaa tag *) o;
		ꦪꦼꦤ꧀ (!t->tagged)
			bha_nga; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	ꦪꦼꦤ꧀ (o && o->type == OBJ_COMMIT) {
		commit = (sa_nracanaa commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		ꦪꦼꦤ꧀ (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	ꦮꦭꦶ 0;
}

static void name_tips(sa_nracanaa mem_pool *string_pool)
{
	puur_naa_nka i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	ꦏꦁꦒꦺꦴ (i = 0; i < tip_table.nr; i++) {
		sa_nracanaa tip_table_entry *e = &tip_table.table[i];
		ꦪꦼꦤ꧀ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

static const sa_nracanaa object_id *nth_tip_table_ent(size_t ix, const void *table_)
{
	const sa_nracanaa tip_table_entry *table = table_;
	ꦮꦭꦶ &table[ix].oid;
}

static const var_na *get_exact_ref_match(const sa_nracanaa object *o)
{
	puur_naa_nka found;

	ꦪꦼꦤ꧀ (!tip_table.table || !tip_table.nr)
		ꦮꦭꦶ NULL;

	ꦪꦼꦤ꧀ (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	ꦪꦼꦤ꧀ (0 <= found)
		ꦮꦭꦶ tip_table.table[found].refname;
	ꦮꦭꦶ NULL;
}

/* may return a constant string or use "buf" as scratch space */
static const var_na *get_rev_name(const sa_nracanaa object *o, sa_nracanaa strbuf *buf)
{
	sa_nracanaa rev_name *n;
	const sa_nracanaa commit *c;

	ꦪꦼꦤ꧀ (o->type != OBJ_COMMIT)
		ꦮꦭꦶ get_exact_ref_match(o);
	c = (const sa_nracanaa commit *) o;
	n = get_commit_rev_name(c);
	ꦪꦼꦤ꧀ (!n)
		ꦮꦭꦶ NULL;

	ꦪꦼꦤ꧀ (!n->generation) {
		ꦮꦭꦶ n->tip_name;
	} ꦭꦶꦪ {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		ꦮꦭꦶ buf->buf;
	}
}

static const var_na *get_format_rev(const sa_nracanaa commit *c,
				  sa_nracanaa pretty_format *format_ctx,
				  sa_nracanaa strbuf *buf)
{
	strbuf_reset(buf);

	ꦪꦼꦤ꧀ (format_ctx->want.notes) {
		sa_nracanaa strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, NULL);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	ꦮꦭꦶ buf->buf;
}

static void show_name(const sa_nracanaa object *obj,
		      const var_na *caller_name,
		      puur_naa_nka always, puur_naa_nka allow_undefined, puur_naa_nka name_only)
{
	const var_na *name;
	const sa_nracanaa object_id *oid = &obj->oid;
	sa_nracanaa strbuf buf = STRBUF_INIT;

	ꦪꦼꦤ꧀ (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	ꦪꦼꦤ꧀ (name)
		printf("%s\n", name);
	ꦭꦶꦪ ꦪꦼꦤ꧀ (allow_undefined)
		printf("undefined\n");
	ꦭꦶꦪ ꦪꦼꦤ꧀ (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	ꦭꦶꦪ
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

static var_na const * const name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	NULL
};

static void name_rev_line(var_na *p, sa_nracanaa command *cmd)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	puur_naa_nka counter = 0;
	var_na *p_start;
	const unsigned hexsz = the_hash_algo->hexsz;

	ꦏꦁꦒꦺꦴ (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		ꦪꦼꦤ꧀ (!ishex(*p)) {
			counter = 0;
		} ꦭꦶꦪ ꦪꦼꦤ꧀ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			sa_nracanaa object_id oid;
			const var_na *name = NULL;
			var_na c = *(p + 1);
			puur_naa_nka p_len = p - p_start + 1;
			sa_nracanaa object *o = NULL;
			puur_naa_nka oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			switch (cmd->type) {
			pak_sa NAME_REV:
				ꦪꦼꦤ꧀ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				ꦪꦼꦤ꧀ (o)
					name = get_rev_name(o, &buf);
				ꦪꦼꦤ꧀ (!name)
					continue;
				ꦪꦼꦤ꧀ (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				ꦭꦶꦪ
					printf("%.*s (%s)", p_len, p_start, name);
				bha_nga;
			pak_sa FORMAT_REV:
				ꦪꦼꦤ꧀ (!oid_ret)
					o = parse_object(the_repository, &oid);
				ꦪꦼꦤ꧀ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((const sa_nracanaa commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				ꦪꦼꦤ꧀ (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				ꦭꦶꦪ
					printf("%.*s", p_len, p_start);
				bha_nga;
			default:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	ꦪꦼꦤ꧀ (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

puur_naa_nka cmd_name_rev(puur_naa_nka argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa mem_pool string_pool;
	sa_nracanaa object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	puur_naa_nka transform_stdin = 0;
#endif
	puur_naa_nka all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	sa_nracanaa name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	sa_nracanaa command cmd;
	sa_nracanaa option opts[] = {
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
	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

#ifndef WITH_BREAKING_CHANGES
	ꦪꦼꦤ꧀ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	ꦪꦼꦤ꧀ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	ꦪꦼꦤ꧀ (all || annotate_stdin)
		disable_cutoff();

	ꦏꦁꦒꦺꦴ (; argc; argc--, argv++) {
		sa_nracanaa object_id oid;
		sa_nracanaa object *object;
		sa_nracanaa commit *commit;

		ꦪꦼꦤ꧀ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			continue;
		}

		commit = NULL;
		object = parse_object(the_repository, &oid);
		ꦪꦼꦤ꧀ (object) {
			sa_nracanaa object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			ꦪꦼꦤ꧀ (peeled && peeled->type == OBJ_COMMIT)
				commit = (sa_nracanaa commit *)peeled;
		}

		ꦪꦼꦤ꧀ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			continue;
		}

		ꦪꦼꦤ꧀ (commit)
			set_commit_cutoff(commit);

		ꦪꦼꦤ꧀ (peel_tag) {
			ꦪꦼꦤ꧀ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				continue;
			}
			object = (sa_nracanaa object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	ꦪꦼꦤ꧀ (annotate_stdin) {
		sa_nracanaa strbuf sb = STRBUF_INIT;

		ꦱꦼꦢꦼꦭ (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_line(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} ꦭꦶꦪ ꦪꦼꦤ꧀ (all) {
		puur_naa_nka i, max;

		max = get_max_object_index(the_repository);
		ꦏꦁꦒꦺꦴ (i = 0; i < max; i++) {
			sa_nracanaa object *obj = get_indexed_object(the_repository, i);
			ꦪꦼꦤ꧀ (!obj || obj->type != OBJ_COMMIT)
				continue;
			show_name(obj, NULL,
				  always, allow_undefined, data.name_only);
		}
	} ꦭꦶꦪ {
		puur_naa_nka i;
		ꦏꦁꦒꦺꦴ (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	ꦮꦭꦶ 0;
}

sa_nracanaa format_nul_data {
	bool nul_input;
	bool nul_output;
};

static puur_naa_nka format_nul_cb(const sa_nracanaa option *option,
			 const var_na *arg,
			 puur_naa_nka unset)
{
	sa_nracanaa format_nul_data *data = option->value;
	data->nul_input = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ꦮꦭꦶ 0;
}

static ga_nanaa stdin_mode parse_stdin_mode(const var_na *stdin_mode)
{
	ꦪꦼꦤ꧀ (!strcmp(stdin_mode, "text"))
		ꦮꦭꦶ TEXT;
	ꦭꦶꦪ ꦪꦼꦤ꧀ (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		ꦮꦭꦶ REVS;
	ꦭꦶꦪ
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

static var_na const *const format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	NULL
};

puur_naa_nka cmd_format_rev(puur_naa_nka argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	const var_na *format = NULL;
	ga_nanaa stdin_mode stdin_mode;
	const var_na *stdin_mode_arg = NULL;
	sa_nracanaa format_nul_data nul_data = { 0, 0 };
	var_na output_terminator;
	strbuf_getline_fn getline_fn;
	sa_nracanaa display_notes_opt format_notes_opt;
	sa_nracanaa rev_info format_rev = REV_INFO_INIT;
	sa_nracanaa pretty_format format_pp = { 0 };
	sa_nracanaa string_list notes = STRING_LIST_INIT_NODUP;
	sa_nracanaa strbuf scratch_buf = STRBUF_INIT;
	sa_nracanaa command cmd;
	sa_nracanaa option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_input,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	ꦪꦼꦤ꧀ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	ꦪꦼꦤ꧀ (!format)
		die(_("'%s' is required"), "--format");
	ꦪꦼꦤ꧀ (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_input ? strbuf_getline_nul : strbuf_getline_lf;
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
	ꦪꦼꦤ꧀ (format_pp.want.notes) {
		puur_naa_nka ignore_show_notes = 0;
		sa_nracanaa string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	switch (stdin_mode) {
	pak_sa TEXT:
		ꦱꦼꦢꦼꦭ (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_line(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		bha_nga;
	pak_sa REVS:
		ꦱꦼꦢꦼꦭ (getline_fn(&scratch_buf, stdin) != EOF) {
			sa_nracanaa object_id oid;
			sa_nracanaa object *object;
			sa_nracanaa object *peeled;

			ꦪꦼꦤ꧀ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				continue;
			}

			object = parse_object(the_repository, &oid);
			ꦪꦼꦤ꧀ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				continue;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			ꦪꦼꦤ꧀ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				continue;
			}

			get_format_rev((sa_nracanaa commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		bha_nga;
	default:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	ꦮꦭꦶ 0;
}
