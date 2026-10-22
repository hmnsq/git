#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "hash-lookup.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "commit-graph.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "mem-pool.h"
#ಅನ್ತರ್ಭಾವ "pretty.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "notes.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

ಸಂರಚನಾ rev_name {
	ಧ್ರುವ ವರ್ಣ *tip_name;
	timestamp_t taggerdate;
	ಪೂರ್ಣಾಙ್ಕ generation;
	ಪೂರ್ಣಾಙ್ಕ distance;
	ಪೂರ್ಣಾಙ್ಕ from_tag;
};

define_commit_slab(commit_rev_name, ಸಂರಚನಾ rev_name);

ಸ್ಥಿರ timestamp_t generation_cutoff = GENERATION_ಸಂಖ್ಯೆ_INFINITY;
ಸ್ಥಿರ timestamp_t cutoff = TIME_MAX;
ಸ್ಥಿರ ಸಂರಚನಾ commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
ಸ್ಥಿರ ಶೂನ್ಯ disable_cutoff(ಶೂನ್ಯ)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ಸ್ಥಿರ ಶೂನ್ಯ set_commit_cutoff(ಸಂರಚನಾ commit *commit)
{

	ಯದಿ (cutoff > commit->date)
		cutoff = commit->date;

	ಯದಿ (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		ಯದಿ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
ಸ್ಥಿರ ಶೂನ್ಯ adjust_cutoff_timestamp_for_slop(ಶೂನ್ಯ)
{
	ಯದಿ (cutoff) {
		/* check for underflow */
		ಯದಿ (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		ಅನ್ಯಥಾ
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_is_before_cutoff(ಸಂರಚನಾ commit *commit)
{
	ಯದಿ (generation_cutoff < GENERATION_ಸಂಖ್ಯೆ_INFINITY)
		ಪ್ರತ್ಯಾಗಮ generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	ಪ್ರತ್ಯಾಗಮ commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_valid_rev_name(ಧ್ರುವ ಸಂರಚನಾ rev_name *name)
{
	ಪ್ರತ್ಯಾಗಮ name && name->tip_name;
}

ಸ್ಥಿರ ಸಂರಚನಾ rev_name *get_commit_rev_name(ಧ್ರುವ ಸಂರಚನಾ commit *commit)
{
	ಸಂರಚನಾ rev_name *name = commit_rev_name_peek(&rev_names, commit);

	ಪ್ರತ್ಯಾಗಮ is_valid_rev_name(name) ? name : ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ effective_distance(ಪೂರ್ಣಾಙ್ಕ distance, ಪೂರ್ಣಾಙ್ಕ generation)
{
	ಪ್ರತ್ಯಾಗಮ distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_better_name(ಸಂರಚನಾ rev_name *name,
			  timestamp_t taggerdate,
			  ಪೂರ್ಣಾಙ್ಕ generation,
			  ಪೂರ್ಣಾಙ್ಕ distance,
			  ಪೂರ್ಣಾಙ್ಕ from_tag)
{
	ಪೂರ್ಣಾಙ್ಕ name_distance = effective_distance(name->distance, name->generation);
	ಪೂರ್ಣಾಙ್ಕ new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	ಯದಿ (from_tag && name->from_tag)
		ಪ್ರತ್ಯಾಗಮ name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	ಯದಿ (name->from_tag != from_tag)
		ಪ್ರತ್ಯಾಗಮ from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	ಯದಿ (name_distance != new_distance)
		ಪ್ರತ್ಯಾಗಮ name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	ಯದಿ (name->taggerdate != taggerdate)
		ಪ್ರತ್ಯಾಗಮ name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ rev_name *create_or_update_name(ಸಂರಚನಾ commit *commit,
					      timestamp_t taggerdate,
					      ಪೂರ್ಣಾಙ್ಕ generation, ಪೂರ್ಣಾಙ್ಕ distance,
					      ಪೂರ್ಣಾಙ್ಕ from_tag)
{
	ಸಂರಚನಾ rev_name *name = commit_rev_name_at(&rev_names, commit);

	ಯದಿ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	ಪ್ರತ್ಯಾಗಮ name;
}

ಸ್ಥಿರ ವರ್ಣ *get_parent_name(ಧ್ರುವ ಸಂರಚನಾ rev_name *name, ಪೂರ್ಣಾಙ್ಕ parent_ಸಂಖ್ಯೆ,
			     ಸಂರಚನಾ mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	ಯದಿ (name->generation > 0) {
		ಪ್ರತ್ಯಾಗಮ mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (ಪೂರ್ಣಾಙ್ಕ)len, name->tip_name,
				       name->generation, parent_ಸಂಖ್ಯೆ);
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ mem_pool_strfmt(string_pool, "%.*s^%d",
				       (ಪೂರ್ಣಾಙ್ಕ)len, name->tip_name, parent_ಸಂಖ್ಯೆ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ name_rev(ಸಂರಚನಾ commit *start_commit,
		ಧ್ರುವ ವರ್ಣ *tip_name, timestamp_t taggerdate,
		ಪೂರ್ಣಾಙ್ಕ from_tag, ಪೂರ್ಣಾಙ್ಕ deref, ಸಂರಚನಾ mem_pool *string_pool)
{
	ಸಂರಚನಾ commit_ರಾಶಿ ರಾಶಿ = COMMIT_ರಾಶಿ_INIT;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ commit_ರಾಶಿ parents_to_queue = COMMIT_ರಾಶಿ_INIT;
	ಸಂರಚನಾ rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	ಯದಿ (commit_is_before_cutoff(start_commit))
		ಪ್ರತ್ಯಾಗಮ;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	ಯದಿ (!start_name)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	ಅನ್ಯಥಾ
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ರಾಶಿ_ತಳ್ಳು(&ರಾಶಿ, start_commit);

	ಯಾವತ್ ((commit = commit_ರಾಶಿ_ತೆಗೆ(&ರಾಶಿ))) {
		ಸಂರಚನಾ rev_name *name = get_commit_rev_name(commit);
		ಸಂರಚನಾ commit_list *parents;
		ಪೂರ್ಣಾಙ್ಕ parent_ಸಂಖ್ಯೆ = 1;

		parents_to_queue.nr = 0;

		ಪ್ರತಿ (parents = commit->parents;
				parents;
				parents = parents->next, parent_ಸಂಖ್ಯೆ++) {
			ಸಂರಚನಾ commit *parent = parents->item;
			ಸಂರಚನಾ rev_name *parent_name;
			ಪೂರ್ಣಾಙ್ಕ generation, distance;

			repo_parse_commit(the_repository, parent);
			ಯದಿ (commit_is_before_cutoff(parent))
				ಅನುವರ್ತ;

			ಯದಿ (parent_ಸಂಖ್ಯೆ > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} ಅನ್ಯಥಾ {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			ಯದಿ (parent_name) {
				ಯದಿ (parent_ಸಂಖ್ಯೆ > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_ಸಂಖ್ಯೆ,
								string_pool);
				ಅನ್ಯಥಾ
					parent_name->tip_name = name->tip_name;
				commit_ರಾಶಿ_ತಳ್ಳು(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		ಯಾವತ್ (parents_to_queue.nr)
			commit_ರಾಶಿ_ತಳ್ಳು(&ರಾಶಿ,
					  commit_ರಾಶಿ_ತೆಗೆ(&parents_to_queue));
	}

	commit_ರಾಶಿ_clear(&ರಾಶಿ);
	commit_ರಾಶಿ_clear(&parents_to_queue);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ subpath_matches(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *filter)
{
	ಧ್ರುವ ವರ್ಣ *subpath = path;

	ಯಾವತ್ (subpath) {
		ಯದಿ (!wildmatch(filter, subpath, 0))
			ಪ್ರತ್ಯಾಗಮ subpath - path;
		subpath = strchr(subpath, '/');
		ಯದಿ (subpath)
			subpath++;
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸಂರಚನಾ name_ref_data {
	ಪೂರ್ಣಾಙ್ಕ tags_only;
	ಪೂರ್ಣಾಙ್ಕ name_only;
	ಸಂರಚನಾ string_list ref_filters;
	ಸಂರಚನಾ string_list exclude_filters;
};

ಸಂರಚನಾ pretty_format {
	ಸಂರಚನಾ pretty_print_context ctx;
	ಸಂರಚನಾ userformat_want want;
};

ಗಣನಾ command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

ಗಣನಾ stdin_mode {
    TEXT = 1,
    REVS = 2,
};

ಸಂರಚನಾ command {
	ಗಣನಾ command_type type;
	ಸಙ್ಘ {
		ಪೂರ್ಣಾಙ್ಕ name_only;
		ಸಂರಚನಾ pretty_format *pretty_format;
	} u;
};

ಸ್ಥಿರ ಶೂನ್ಯ init_name_rev_command(ಸಂರಚನಾ command *cmd,
				  ಪೂರ್ಣಾಙ್ಕ name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_format_rev_command(ಸಂರಚನಾ command *cmd,
				    ಸಂರಚನಾ pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

ಸ್ಥಿರ ಸಂರಚನಾ tip_table {
	ಸಂರಚನಾ tip_table_entry {
		ಸಂರಚನಾ object_id oid;
		ಧ್ರುವ ವರ್ಣ *refname;
		ಸಂರಚನಾ commit *commit;
		timestamp_t taggerdate;
		unsigned ಪೂರ್ಣಾಙ್ಕ from_tag:1;
		unsigned ಪೂರ್ಣಾಙ್ಕ deref:1;
	} *table;
	ಪೂರ್ಣಾಙ್ಕ nr;
	ಪೂರ್ಣಾಙ್ಕ alloc;
	ಪೂರ್ಣಾಙ್ಕ sorted;
} tip_table;

ಸ್ಥಿರ ಶೂನ್ಯ add_to_tip_table(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *refname,
			     ಪೂರ್ಣಾಙ್ಕ shorten_unambiguous, ಸಂರಚನಾ commit *commit,
			     timestamp_t taggerdate, ಪೂರ್ಣಾಙ್ಕ from_tag, ಪೂರ್ಣಾಙ್ಕ deref)
{
	ವರ್ಣ *short_refname = ರಿಕ್ತ;

	ಯದಿ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	ಅನ್ಯಥಾ ಯದಿ (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	ಅನ್ಯಥಾ
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tipcmp(ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
{
	ಧ್ರುವ ಸಂರಚನಾ tip_table_entry *a = a_, *b = b_;
	ಪ್ರತ್ಯಾಗಮ oidcmp(&a->oid, &b->oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp_by_tag_and_age(ಧ್ರುವ ಶೂನ್ಯ *a_, ಧ್ರುವ ಶೂನ್ಯ *b_)
{
	ಧ್ರುವ ಸಂರಚನಾ tip_table_entry *a = a_, *b = b_;
	ಪೂರ್ಣಾಙ್ಕ cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	ಯದಿ (cmp)
		ಪ್ರತ್ಯಾಗಮ cmp;

	/* Older is better. */
	ಯದಿ (a->taggerdate < b->taggerdate)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಪ್ರತ್ಯಾಗಮ a->taggerdate != b->taggerdate;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ name_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ object *o = parse_object(the_repository, ref->oid);
	ಸಂರಚನಾ name_ref_data *data = cb_data;
	ಪೂರ್ಣಾಙ್ಕ can_abbreviate_output = data->tags_only && data->name_only;
	ಪೂರ್ಣಾಙ್ಕ deref = 0;
	ಪೂರ್ಣಾಙ್ಕ from_tag = 0;
	ಸಂರಚನಾ commit *commit = ರಿಕ್ತ;
	timestamp_t taggerdate = TIME_MAX;

	ಯದಿ (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (data->exclude_filters.nr) {
		ಸಂರಚನಾ string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			ಯದಿ (subpath_matches(ref->name, item->string) >= 0)
				ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಯದಿ (data->ref_filters.nr) {
		ಸಂರಚನಾ string_list_item *item;
		ಪೂರ್ಣಾಙ್ಕ matched = 0;

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
			ವಿಕಲ್ಪ (subpath_matches(ref->name, item->string)) {
			ಪಕ್ಷ -1: /* did not match */
				ಭಙ್ಗ;
			ಪಕ್ಷ 0: /* matched fully */
				matched = 1;
				ಭಙ್ಗ;
			ಸ್ವಭಾವ: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				ಭಙ್ಗ;
			}
		}

		/* If none of the patterns matched, stop now */
		ಯದಿ (!matched)
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯಾವತ್ (o && o->type == OBJ_TAG) {
		ಸಂರಚನಾ tag *t = (ಸಂರಚನಾ tag *) o;
		ಯದಿ (!t->tagged)
			ಭಙ್ಗ; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	ಯದಿ (o && o->type == OBJ_COMMIT) {
		commit = (ಸಂರಚನಾ commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		ಯದಿ (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ name_tips(ಸಂರಚನಾ mem_pool *string_pool)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	ಪ್ರತಿ (i = 0; i < tip_table.nr; i++) {
		ಸಂರಚನಾ tip_table_entry *e = &tip_table.table[i];
		ಯದಿ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ object_id *nth_tip_table_ent(size_t ix, ಧ್ರುವ ಶೂನ್ಯ *table_)
{
	ಧ್ರುವ ಸಂರಚನಾ tip_table_entry *table = table_;
	ಪ್ರತ್ಯಾಗಮ &table[ix].oid;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_exact_ref_match(ಧ್ರುವ ಸಂರಚನಾ object *o)
{
	ಪೂರ್ಣಾಙ್ಕ found;

	ಯದಿ (!tip_table.table || !tip_table.nr)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	ಯದಿ (0 <= found)
		ಪ್ರತ್ಯಾಗಮ tip_table.table[found].refname;
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* may return a constant string or use "buf" as scratch space */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_rev_name(ಧ್ರುವ ಸಂರಚನಾ object *o, ಸಂರಚನಾ strbuf *buf)
{
	ಸಂರಚನಾ rev_name *n;
	ಧ್ರುವ ಸಂರಚನಾ commit *c;

	ಯದಿ (o->type != OBJ_COMMIT)
		ಪ್ರತ್ಯಾಗಮ get_exact_ref_match(o);
	c = (ಧ್ರುವ ಸಂರಚನಾ commit *) o;
	n = get_commit_rev_name(c);
	ಯದಿ (!n)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (!n->generation) {
		ಪ್ರತ್ಯಾಗಮ n->tip_name;
	} ಅನ್ಯಥಾ {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		ಪ್ರತ್ಯಾಗಮ buf->buf;
	}
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_format_rev(ಧ್ರುವ ಸಂರಚನಾ commit *c,
				  ಸಂರಚನಾ pretty_format *format_ctx,
				  ಸಂರಚನಾ strbuf *buf)
{
	strbuf_reset(buf);

	ಯದಿ (format_ctx->want.notes) {
		ಸಂರಚನಾ strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, ರಿಕ್ತ);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	ಪ್ರತ್ಯಾಗಮ buf->buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_name(ಧ್ರುವ ಸಂರಚನಾ object *obj,
		      ಧ್ರುವ ವರ್ಣ *caller_name,
		      ಪೂರ್ಣಾಙ್ಕ always, ಪೂರ್ಣಾಙ್ಕ allow_undefined, ಪೂರ್ಣಾಙ್ಕ name_only)
{
	ಧ್ರುವ ವರ್ಣ *name;
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid = &obj->oid;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	ಯದಿ (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	ಯದಿ (name)
		printf("%s\n", name);
	ಅನ್ಯಥಾ ಯದಿ (allow_undefined)
		printf("undefined\n");
	ಅನ್ಯಥಾ ಯದಿ (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	ಅನ್ಯಥಾ
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಶೂನ್ಯ name_rev_ಸಾಲು(ವರ್ಣ *p, ಸಂರಚನಾ command *cmd)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ counter = 0;
	ವರ್ಣ *p_start;
	ಧ್ರುವ unsigned hexsz = the_hash_algo->hexsz;

	ಪ್ರತಿ (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		ಯದಿ (!ishex(*p)) {
			counter = 0;
		} ಅನ್ಯಥಾ ಯದಿ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			ಸಂರಚನಾ object_id oid;
			ಧ್ರುವ ವರ್ಣ *name = ರಿಕ್ತ;
			ವರ್ಣ c = *(p + 1);
			ಪೂರ್ಣಾಙ್ಕ p_len = p - p_start + 1;
			ಸಂರಚನಾ object *o = ರಿಕ್ತ;
			ಪೂರ್ಣಾಙ್ಕ oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			ವಿಕಲ್ಪ (cmd->type) {
			ಪಕ್ಷ NAME_REV:
				ಯದಿ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				ಯದಿ (o)
					name = get_rev_name(o, &buf);
				ಯದಿ (!name)
					ಅನುವರ್ತ;
				ಯದಿ (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				ಅನ್ಯಥಾ
					printf("%.*s (%s)", p_len, p_start, name);
				ಭಙ್ಗ;
			ಪಕ್ಷ FORMAT_REV:
				ಯದಿ (!oid_ret)
					o = parse_object(the_repository, &oid);
				ಯದಿ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ಧ್ರುವ ಸಂರಚನಾ commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				ಯದಿ (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				ಅನ್ಯಥಾ
					printf("%.*s", p_len, p_start);
				ಭಙ್ಗ;
			ಸ್ವಭಾವ:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	ಯದಿ (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

ಪೂರ್ಣಾಙ್ಕ cmd_name_rev(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ mem_pool string_pool;
	ಸಂರಚನಾ object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	ಪೂರ್ಣಾಙ್ಕ transform_stdin = 0;
#endif
	ಪೂರ್ಣಾಙ್ಕ all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	ಸಂರಚನಾ name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	ಸಂರಚನಾ command cmd;
	ಸಂರಚನಾ option opts[] = {
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
	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

#ifndef WITH_BREAKING_CHANGES
	ಯದಿ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	ಯದಿ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	ಯದಿ (all || annotate_stdin)
		disable_cutoff();

	ಪ್ರತಿ (; argc; argc--, argv++) {
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ object *object;
		ಸಂರಚನಾ commit *commit;

		ಯದಿ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			ಅನುವರ್ತ;
		}

		commit = ರಿಕ್ತ;
		object = parse_object(the_repository, &oid);
		ಯದಿ (object) {
			ಸಂರಚನಾ object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			ಯದಿ (peeled && peeled->type == OBJ_COMMIT)
				commit = (ಸಂರಚನಾ commit *)peeled;
		}

		ಯದಿ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			ಅನುವರ್ತ;
		}

		ಯದಿ (commit)
			set_commit_cutoff(commit);

		ಯದಿ (peel_tag) {
			ಯದಿ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				ಅನುವರ್ತ;
			}
			object = (ಸಂರಚನಾ object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	ಯದಿ (annotate_stdin) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

		ಯಾವತ್ (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_ಸಾಲು(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} ಅನ್ಯಥಾ ಯದಿ (all) {
		ಪೂರ್ಣಾಙ್ಕ i, max;

		max = get_max_object_index(the_repository);
		ಪ್ರತಿ (i = 0; i < max; i++) {
			ಸಂರಚನಾ object *obj = get_indexed_object(the_repository, i);
			ಯದಿ (!obj || obj->type != OBJ_COMMIT)
				ಅನುವರ್ತ;
			show_name(obj, ರಿಕ್ತ,
				  always, allow_undefined, data.name_only);
		}
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಪ್ರತಿ (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ format_nul_data {
	bool nul_ಒಳಹರಿವು;
	bool nul_output;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_nul_cb(ಧ್ರುವ ಸಂರಚನಾ option *option,
			 ಧ್ರುವ ವರ್ಣ *arg,
			 ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ format_nul_data *data = option->ಮೌಲ್ಯ;
	data->nul_ಒಳಹರಿವು = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಗಣನಾ stdin_mode parse_stdin_mode(ಧ್ರುವ ವರ್ಣ *stdin_mode)
{
	ಯದಿ (!strcmp(stdin_mode, "text"))
		ಪ್ರತ್ಯಾಗಮ TEXT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		ಪ್ರತ್ಯಾಗಮ REVS;
	ಅನ್ಯಥಾ
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_format_rev(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *format = ರಿಕ್ತ;
	ಗಣನಾ stdin_mode stdin_mode;
	ಧ್ರುವ ವರ್ಣ *stdin_mode_arg = ರಿಕ್ತ;
	ಸಂರಚನಾ format_nul_data nul_data = { 0, 0 };
	ವರ್ಣ output_terminator;
	strbuf_getline_fn getline_fn;
	ಸಂರಚನಾ display_notes_opt format_notes_opt;
	ಸಂರಚನಾ rev_info format_rev = REV_INFO_INIT;
	ಸಂರಚನಾ pretty_format format_pp = { 0 };
	ಸಂರಚನಾ string_list notes = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ strbuf scratch_buf = STRBUF_INIT;
	ಸಂರಚನಾ command cmd;
	ಸಂರಚನಾ option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_ಒಳಹರಿವು,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	ಯದಿ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	ಯದಿ (!format)
		die(_("'%s' is required"), "--format");
	ಯದಿ (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_ಒಳಹರಿವು ? strbuf_getline_nul : strbuf_getline_lf;
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
	ಯದಿ (format_pp.want.notes) {
		ಪೂರ್ಣಾಙ್ಕ ignore_show_notes = 0;
		ಸಂರಚನಾ string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	ವಿಕಲ್ಪ (stdin_mode) {
	ಪಕ್ಷ TEXT:
		ಯಾವತ್ (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_ಸಾಲು(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		ಭಙ್ಗ;
	ಪಕ್ಷ REVS:
		ಯಾವತ್ (getline_fn(&scratch_buf, stdin) != EOF) {
			ಸಂರಚನಾ object_id oid;
			ಸಂರಚನಾ object *object;
			ಸಂರಚನಾ object *peeled;

			ಯದಿ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				ಅನುವರ್ತ;
			}

			object = parse_object(the_repository, &oid);
			ಯದಿ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				ಅನುವರ್ತ;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			ಯದಿ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				ಅನುವರ್ತ;
			}

			get_format_rev((ಸಂರಚನಾ commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	ಪ್ರತ್ಯಾಗಮ 0;
}
