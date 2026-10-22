#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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
#ವ್ಯಾಖ್ಯಾನಿಸು CUTOFF_DATE_SLOP 86400

ರಚನೆ rev_name {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tip_name;
	timestamp_t taggerdate;
	ಪೂರ್ಣಾಂಕ generation;
	ಪೂರ್ಣಾಂಕ ದೂರ;
	ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್;
};

define_commit_slab(commit_rev_name, ರಚನೆ rev_name);

ಸ್ಥಿರ timestamp_t generation_cutoff = GENERATION_ಸಂಖ್ಯೆ_INFINITY;
ಸ್ಥಿರ timestamp_t cutoff = ಸಮಯ_MAX;
ಸ್ಥಿರ ರಚನೆ commit_rev_name rev_ಹೆಸರುಗಳು;

/* Disable the cutoff checks entirely */
ಸ್ಥಿರ ಶೂನ್ಯ disable_cutoff(ಶೂನ್ಯ)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_commit_cutoff(ರಚನೆ commit *commit)
{

	ಆದರೆ (cutoff > commit->date)
		cutoff = commit->date;

	ಆದರೆ (generation_cutoff) {
		timestamp_t generation = commit_ನಕ್ಷೆ_generation(commit);

		ಆದರೆ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
ಸ್ಥಿರ ಶೂನ್ಯ adjust_cutoff_timestamp_for_slop(ಶೂನ್ಯ)
{
	ಆದರೆ (cutoff) {
		/* check for underflow */
		ಆದರೆ (cutoff > ಸಮಯ_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		ಇಲ್ಲದಿದ್ದರೆ
			cutoff = ಸಮಯ_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_is_before_cutoff(ರಚನೆ commit *commit)
{
	ಆದರೆ (generation_cutoff < GENERATION_ಸಂಖ್ಯೆ_INFINITY)
		ಹಿಂತಿರುಗಿ generation_cutoff &&
			commit_ನಕ್ಷೆ_generation(commit) < generation_cutoff;

	ಹಿಂತಿರುಗಿ commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಲೀನ_TRAVERSAL_ತೂಕ 65535

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_valid_rev_name(ಸ್ಥಿರಾಂಕ ರಚನೆ rev_name *name)
{
	ಹಿಂತಿರುಗಿ name && name->tip_name;
}

ಸ್ಥಿರ ರಚನೆ rev_name *get_commit_rev_name(ಸ್ಥಿರಾಂಕ ರಚನೆ commit *commit)
{
	ರಚನೆ rev_name *name = commit_rev_name_peek(&rev_ಹೆಸರುಗಳು, commit);

	ಹಿಂತಿರುಗಿ is_valid_rev_name(name) ? name : ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ effective_ದೂರ(ಪೂರ್ಣಾಂಕ ದೂರ, ಪೂರ್ಣಾಂಕ generation)
{
	ಹಿಂತಿರುಗಿ ದೂರ + (generation > 0 ? ವಿಲೀನ_TRAVERSAL_ತೂಕ : 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_better_name(ರಚನೆ rev_name *name,
			  timestamp_t taggerdate,
			  ಪೂರ್ಣಾಂಕ generation,
			  ಪೂರ್ಣಾಂಕ ದೂರ,
			  ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್)
{
	ಪೂರ್ಣಾಂಕ name_ದೂರ = effective_ದೂರ(name->ದೂರ, name->generation);
	ಪೂರ್ಣಾಂಕ new_ದೂರ = effective_ದೂರ(ದೂರ, generation);

	/* If both are tags, we prefer the nearer one. */
	ಆದರೆ (from_ಟ್ಯಾಗ್ && name->from_ಟ್ಯಾಗ್)
		ಹಿಂತಿರುಗಿ name_ದೂರ > new_ದೂರ;

	/* Favor a tag over a non-tag. */
	ಆದರೆ (name->from_ಟ್ಯಾಗ್ != from_ಟ್ಯಾಗ್)
		ಹಿಂತಿರುಗಿ from_ಟ್ಯಾಗ್;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	ಆದರೆ (name_ದೂರ != new_ದೂರ)
		ಹಿಂತಿರುಗಿ name_ದೂರ > new_ದೂರ;

	/* ... or tiebreak to favor older date */
	ಆದರೆ (name->taggerdate != taggerdate)
		ಹಿಂತಿರುಗಿ name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ rev_name *create_or_update_name(ರಚನೆ commit *commit,
					      timestamp_t taggerdate,
					      ಪೂರ್ಣಾಂಕ generation, ಪೂರ್ಣಾಂಕ ದೂರ,
					      ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್)
{
	ರಚನೆ rev_name *name = commit_rev_name_at(&rev_ಹೆಸರುಗಳು, commit);

	ಆದರೆ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, ದೂರ, from_ಟ್ಯಾಗ್))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->ದೂರ = ದೂರ;
	name->from_ಟ್ಯಾಗ್ = from_ಟ್ಯಾಗ್;

	ಹಿಂತಿರುಗಿ name;
}

ಸ್ಥಿರ ಅಕ್ಷರ *get_ಪೋಷಕ_name(ಸ್ಥಿರಾಂಕ ರಚನೆ rev_name *name, ಪೂರ್ಣಾಂಕ ಪೋಷಕ_ಸಂಖ್ಯೆ,
			     ರಚನೆ mem_pool *string_pool)
{
	ಗಾತ್ರ_t len;

	strip_suffix(name->tip_name, "^0", &len);
	ಆದರೆ (name->generation > 0) {
		ಹಿಂತಿರುಗಿ mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (ಪೂರ್ಣಾಂಕ)len, name->tip_name,
				       name->generation, ಪೋಷಕ_ಸಂಖ್ಯೆ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ mem_pool_strfmt(string_pool, "%.*s^%d",
				       (ಪೂರ್ಣಾಂಕ)len, name->tip_name, ಪೋಷಕ_ಸಂಖ್ಯೆ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ name_rev(ರಚನೆ commit *ಆರಂಭ_commit,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tip_name, timestamp_t taggerdate,
		ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್, ಪೂರ್ಣಾಂಕ deref, ರಚನೆ mem_pool *string_pool)
{
	ರಚನೆ commit_ರಾಶಿ ರಾಶಿ = COMMIT_ರಾಶಿ_INIT;
	ರಚನೆ commit *commit;
	ರಚನೆ commit_ರಾಶಿ parents_ಗಮ್ಯ_ಸರತಿ = COMMIT_ರಾಶಿ_INIT;
	ರಚನೆ rev_name *ಆರಂಭ_name;

	repo_parse_commit(the_repository, ಆರಂಭ_commit);
	ಆದರೆ (commit_is_before_cutoff(ಆರಂಭ_commit))
		ಹಿಂತಿರುಗಿ;

	ಆರಂಭ_name = create_or_update_name(ಆರಂಭ_commit, taggerdate, 0, 0,
					   from_ಟ್ಯಾಗ್);
	ಆದರೆ (!ಆರಂಭ_name)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (deref)
		ಆರಂಭ_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	ಇಲ್ಲದಿದ್ದರೆ
		ಆರಂಭ_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ರಾಶಿ_ತಳ್ಳು(&ರಾಶಿ, ಆರಂಭ_commit);

	ಆಗ ((commit = commit_ರಾಶಿ_ತೆಗೆ(&ರಾಶಿ))) {
		ರಚನೆ rev_name *name = get_commit_rev_name(commit);
		ರಚನೆ commit_list *parents;
		ಪೂರ್ಣಾಂಕ ಪೋಷಕ_ಸಂಖ್ಯೆ = 1;

		parents_ಗಮ್ಯ_ಸರತಿ.nr = 0;

		ಗಾಗಿ (parents = commit->parents;
				parents;
				parents = parents->ಮುಂದಿನ, ಪೋಷಕ_ಸಂಖ್ಯೆ++) {
			ರಚನೆ commit *ಪೋಷಕ = parents->ಅಂಶ;
			ರಚನೆ rev_name *ಪೋಷಕ_name;
			ಪೂರ್ಣಾಂಕ generation, ದೂರ;

			repo_parse_commit(the_repository, ಪೋಷಕ);
			ಆದರೆ (commit_is_before_cutoff(ಪೋಷಕ))
				ಮುಂದುವರಿ;

			ಆದರೆ (ಪೋಷಕ_ಸಂಖ್ಯೆ > 1) {
				generation = 0;
				ದೂರ = name->ದೂರ + ವಿಲೀನ_TRAVERSAL_ತೂಕ;
			} ಇಲ್ಲದಿದ್ದರೆ {
				generation = name->generation + 1;
				ದೂರ = name->ದೂರ + 1;
			}

			ಪೋಷಕ_name = create_or_update_name(ಪೋಷಕ, taggerdate,
							    generation,
							    ದೂರ, from_ಟ್ಯಾಗ್);
			ಆದರೆ (ಪೋಷಕ_name) {
				ಆದರೆ (ಪೋಷಕ_ಸಂಖ್ಯೆ > 1)
					ಪೋಷಕ_name->tip_name =
						get_ಪೋಷಕ_name(name,
								ಪೋಷಕ_ಸಂಖ್ಯೆ,
								string_pool);
				ಇಲ್ಲದಿದ್ದರೆ
					ಪೋಷಕ_name->tip_name = name->tip_name;
				commit_ರಾಶಿ_ತಳ್ಳು(&parents_ಗಮ್ಯ_ಸರತಿ, ಪೋಷಕ);
			}
		}

		/* The first parent must come out first from the stack */
		ಆಗ (parents_ಗಮ್ಯ_ಸರತಿ.nr)
			commit_ರಾಶಿ_ತಳ್ಳು(&ರಾಶಿ,
					  commit_ರಾಶಿ_ತೆಗೆ(&parents_ಗಮ್ಯ_ಸರತಿ));
	}

	commit_ರಾಶಿ_clear(&ರಾಶಿ);
	commit_ರಾಶಿ_clear(&parents_ಗಮ್ಯ_ಸರತಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ subpath_matches(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filter)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subpath = ಮಾರ್ಗ;

	ಆಗ (subpath) {
		ಆದರೆ (!wildmatch(filter, subpath, 0))
			ಹಿಂತಿರುಗಿ subpath - ಮಾರ್ಗ;
		subpath = strchr(subpath, '/');
		ಆದರೆ (subpath)
			subpath++;
	}
	ಹಿಂತಿರುಗಿ -1;
}

ರಚನೆ name_ref_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು_only;
	ಪೂರ್ಣಾಂಕ name_only;
	ರಚನೆ string_list ref_filters;
	ರಚನೆ string_list exclude_filters;
};

ರಚನೆ pretty_format {
	ರಚನೆ pretty_print_context ctx;
	ರಚನೆ userformat_want want;
};

ಎಣಿಕೆ command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

ಎಣಿಕೆ stdin_ಬಹುಲಕ {
    ಪಠ್ಯ_2 = 1,
    REVS = 2,
};

ರಚನೆ command {
	ಎಣಿಕೆ command_type type;
	ಒಕ್ಕೂಟ {
		ಪೂರ್ಣಾಂಕ name_only;
		ರಚನೆ pretty_format *pretty_format;
	} u;
};

ಸ್ಥಿರ ಶೂನ್ಯ init_name_rev_command(ರಚನೆ command *cmd,
				  ಪೂರ್ಣಾಂಕ name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_format_rev_command(ರಚನೆ command *cmd,
				    ರಚನೆ pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

ಸ್ಥಿರ ರಚನೆ tip_ಕೋಷ್ಟಕ {
	ರಚನೆ tip_ಕೋಷ್ಟಕ_ನಮೂದು {
		ರಚನೆ object_id oid;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname;
		ರಚನೆ commit *commit;
		timestamp_t taggerdate;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್:1;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ deref:1;
	} *ಕೋಷ್ಟಕ;
	ಪೂರ್ಣಾಂಕ nr;
	ಪೂರ್ಣಾಂಕ alloc;
	ಪೂರ್ಣಾಂಕ sorted;
} tip_ಕೋಷ್ಟಕ;

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಗಮ್ಯ_tip_ಕೋಷ್ಟಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
			     ಪೂರ್ಣಾಂಕ shorten_unambiguous, ರಚನೆ commit *commit,
			     timestamp_t taggerdate, ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್, ಪೂರ್ಣಾಂಕ deref)
{
	ಅಕ್ಷರ *short_refname = ಖಾಲಿ;

	ಆದರೆ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	ಇಲ್ಲದಿದ್ದರೆ
		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(refname, "refs/", &refname);

	ALLOC_GROW(tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ, tip_ಕೋಷ್ಟಕ.nr + 1, tip_ಕೋಷ್ಟಕ.alloc);
	oidcpy(&tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[tip_ಕೋಷ್ಟಕ.nr].oid, oid);
	tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[tip_ಕೋಷ್ಟಕ.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[tip_ಕೋಷ್ಟಕ.nr].commit = commit;
	tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[tip_ಕೋಷ್ಟಕ.nr].taggerdate = taggerdate;
	tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[tip_ಕೋಷ್ಟಕ.nr].from_ಟ್ಯಾಗ್ = from_ಟ್ಯಾಗ್;
	tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[tip_ಕೋಷ್ಟಕ.nr].deref = deref;
	tip_ಕೋಷ್ಟಕ.nr++;
	tip_ಕೋಷ್ಟಕ.sorted = 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tipcmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ tip_ಕೋಷ್ಟಕ_ನಮೂದು *a = a_, *b = b_;
	ಹಿಂತಿರುಗಿ oidcmp(&a->oid, &b->oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmp_by_ಟ್ಯಾಗ್_and_age(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ tip_ಕೋಷ್ಟಕ_ನಮೂದು *a = a_, *b = b_;
	ಪೂರ್ಣಾಂಕ cmp;

	/* Prefer tags. */
	cmp = b->from_ಟ್ಯಾಗ್ - a->from_ಟ್ಯಾಗ್;
	ಆದರೆ (cmp)
		ಹಿಂತಿರುಗಿ cmp;

	/* Older is better. */
	ಆದರೆ (a->taggerdate < b->taggerdate)
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ a->taggerdate != b->taggerdate;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ name_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ object *o = parse_object(the_repository, ref->oid);
	ರಚನೆ name_ref_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ can_abbreviate_output = ದತ್ತಾಂಶ->ಟ್ಯಾಗುಗಳು_only && ದತ್ತಾಂಶ->name_only;
	ಪೂರ್ಣಾಂಕ deref = 0;
	ಪೂರ್ಣಾಂಕ from_ಟ್ಯಾಗ್ = 0;
	ರಚನೆ commit *commit = ಖಾಲಿ;
	timestamp_t taggerdate = ಸಮಯ_MAX;

	ಆದರೆ (ದತ್ತಾಂಶ->ಟ್ಯಾಗುಗಳು_only && !starts_with(ref->name, "refs/tags/"))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ದತ್ತಾಂಶ->exclude_filters.nr) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		for_each_string_list_ಅಂಶ(ಅಂಶ, &ದತ್ತಾಂಶ->exclude_filters) {
			ಆದರೆ (subpath_matches(ref->name, ಅಂಶ->string) >= 0)
				ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಆದರೆ (ದತ್ತಾಂಶ->ref_filters.nr) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		ಪೂರ್ಣಾಂಕ matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_ಅಂಶ(ಅಂಶ, &ದತ್ತಾಂಶ->ref_filters) {
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
			ಆಯ್ಕೆ (subpath_matches(ref->name, ಅಂಶ->string)) {
			ಸ್ಥಿತಿ -1: /* did not match */
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ 0: /* matched fully */
				matched = 1;
				ನಿಲ್ಲಿಸು;
			ಪೂರ್ವನಿಯೋಜಿತ: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				ನಿಲ್ಲಿಸು;
			}
		}

		/* If none of the patterns matched, stop now */
		ಆದರೆ (!matched)
			ಹಿಂತಿರುಗಿ 0;
	}

	ಆಗ (o && o->type == OBJ_ಟ್ಯಾಗ್) {
		ರಚನೆ ಟ್ಯಾಗ್ *t = (ರಚನೆ ಟ್ಯಾಗ್ *) o;
		ಆದರೆ (!t->tagged)
			ನಿಲ್ಲಿಸು; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	ಆದರೆ (o && o->type == OBJ_COMMIT) {
		commit = (ರಚನೆ commit *)o;
		from_ಟ್ಯಾಗ್ = starts_with(ref->name, "refs/tags/");
		ಆದರೆ (taggerdate == ಸಮಯ_MAX)
			taggerdate = commit->date;
	}

	ಕೂಡಿಸು_ಗಮ್ಯ_tip_ಕೋಷ್ಟಕ(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_ಟ್ಯಾಗ್, deref);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ name_tips(ರಚನೆ mem_pool *string_pool)
{
	ಪೂರ್ಣಾಂಕ i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ, tip_ಕೋಷ್ಟಕ.nr, cmp_by_ಟ್ಯಾಗ್_and_age);
	ಗಾಗಿ (i = 0; i < tip_ಕೋಷ್ಟಕ.nr; i++) {
		ರಚನೆ tip_ಕೋಷ್ಟಕ_ನಮೂದು *e = &tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[i];
		ಆದರೆ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_ಟ್ಯಾಗ್, e->deref, string_pool);
		}
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *nth_tip_ಕೋಷ್ಟಕ_ent(ಗಾತ್ರ_t ix, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೋಷ್ಟಕ_2)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ tip_ಕೋಷ್ಟಕ_ನಮೂದು *ಕೋಷ್ಟಕ = ಕೋಷ್ಟಕ_2;
	ಹಿಂತಿರುಗಿ &ಕೋಷ್ಟಕ[ix].oid;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_exact_ref_ಹೊಂದಿಕೆ(ಸ್ಥಿರಾಂಕ ರಚನೆ object *o)
{
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2;

	ಆದರೆ (!tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ || !tip_ಕೋಷ್ಟಕ.nr)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (!tip_ಕೋಷ್ಟಕ.sorted) {
		QSORT(tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ, tip_ಕೋಷ್ಟಕ.nr, tipcmp);
		tip_ಕೋಷ್ಟಕ.sorted = 1;
	}

	ಸಿಕ್ಕಿತು_2 = oid_pos(&o->oid, tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ, tip_ಕೋಷ್ಟಕ.nr,
			nth_tip_ಕೋಷ್ಟಕ_ent);
	ಆದರೆ (0 <= ಸಿಕ್ಕಿತು_2)
		ಹಿಂತಿರುಗಿ tip_ಕೋಷ್ಟಕ.ಕೋಷ್ಟಕ[ಸಿಕ್ಕಿತು_2].refname;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* may return a constant string or use "buf" as scratch space */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_rev_name(ಸ್ಥಿರಾಂಕ ರಚನೆ object *o, ರಚನೆ strbuf *buf)
{
	ರಚನೆ rev_name *n;
	ಸ್ಥಿರಾಂಕ ರಚನೆ commit *c;

	ಆದರೆ (o->type != OBJ_COMMIT)
		ಹಿಂತಿರುಗಿ get_exact_ref_ಹೊಂದಿಕೆ(o);
	c = (ಸ್ಥಿರಾಂಕ ರಚನೆ commit *) o;
	n = get_commit_rev_name(c);
	ಆದರೆ (!n)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (!n->generation) {
		ಹಿಂತಿರುಗಿ n->tip_name;
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		ಹಿಂತಿರುಗಿ buf->buf;
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_format_rev(ಸ್ಥಿರಾಂಕ ರಚನೆ commit *c,
				  ರಚನೆ pretty_format *format_ctx,
				  ರಚನೆ strbuf *buf)
{
	strbuf_reset(buf);

	ಆದರೆ (format_ctx->want.notes) {
		ರಚನೆ strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_ಸಂದೇಶ = strbuf_detach(&notebuf, ಖಾಲಿ);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_ಸಂದೇಶ);

	ಹಿಂತಿರುಗಿ buf->buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_name(ಸ್ಥಿರಾಂಕ ರಚನೆ object *obj,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *caller_name,
		      ಪೂರ್ಣಾಂಕ always, ಪೂರ್ಣಾಂಕ allow_undefined, ಪೂರ್ಣಾಂಕ name_only)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid = &obj->oid;
	ರಚನೆ strbuf buf = STRBUF_INIT;

	ಆದರೆ (!name_only)
		ಬರೆ("%s ", caller_name ? caller_name : oid_ಗಮ್ಯ_hex(oid));
	name = get_rev_name(obj, &buf);
	ಆದರೆ (name)
		ಬರೆ("%s\n", name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (allow_undefined)
		ಬರೆ("undefined\n");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (always)
		ಬರೆ("%s\n",
		       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	ಇಲ್ಲದಿದ್ದರೆ
		die("cannot describe '%s'", oid_ಗಮ್ಯ_hex(oid));
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಶೂನ್ಯ name_rev_ಸಾಲು(ಅಕ್ಷರ *p, ರಚನೆ command *cmd)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ counter = 0;
	ಅಕ್ಷರ *p_ಆರಂಭ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hexsz = the_hash_algo->hexsz;

	ಗಾಗಿ (p_ಆರಂಭ = p; *p; p++) {
#ವ್ಯಾಖ್ಯಾನಿಸು ishex(x) (ಅಂಕಿಯೇ((x)) || ((x) >= 'a' && (x) <= 'f'))
		ಆದರೆ (!ishex(*p)) {
			counter = 0;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			ರಚನೆ object_id oid;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ಖಾಲಿ;
			ಅಕ್ಷರ c = *(p + 1);
			ಪೂರ್ಣಾಂಕ p_len = p - p_ಆರಂಭ + 1;
			ರಚನೆ object *o = ಖಾಲಿ;
			ಪೂರ್ಣಾಂಕ oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			ಆಯ್ಕೆ (cmd->type) {
			ಸ್ಥಿತಿ NAME_REV:
				ಆದರೆ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				ಆದರೆ (o)
					name = get_rev_name(o, &buf);
				ಆದರೆ (!name)
					ಮುಂದುವರಿ;
				ಆದರೆ (cmd->u.name_only)
					ಬರೆ("%.*s%s", p_len - hexsz, p_ಆರಂಭ, name);
				ಇಲ್ಲದಿದ್ದರೆ
					ಬರೆ("%.*s (%s)", p_len, p_ಆರಂಭ, name);
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ FORMAT_REV:
				ಆದರೆ (!oid_ret)
					o = parse_object(the_repository, &oid);
				ಆದರೆ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ಸ್ಥಿರಾಂಕ ರಚನೆ commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				ಆದರೆ (name)
					ಬರೆ("%.*s%s", p_len - hexsz, p_ಆರಂಭ, name);
				ಇಲ್ಲದಿದ್ದರೆ
					ಬರೆ("%.*s", p_len, p_ಆರಂಭ);
				ನಿಲ್ಲಿಸು;
			ಪೂರ್ವನಿಯೋಜಿತ:
				BUG("uncovered case: %d", cmd->type);
			}

			p_ಆರಂಭ = p + 1;
		}
	}

	/* flush */
	ಆದರೆ (p_ಆರಂಭ != p)
		fwrite(p_ಆರಂಭ, p - p_ಆರಂಭ, 1, stdout);

	strbuf_release(&buf);
}

ಪೂರ್ಣಾಂಕ cmd_name_rev(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ mem_pool string_pool;
	ರಚನೆ object_array revs = OBJECT_ARRAY_INIT;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	ಪೂರ್ಣಾಂಕ transform_stdin = 0;
#ಆದರೆ_ಅಂತ್ಯ
	ಪೂರ್ಣಾಂಕ all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_ಟ್ಯಾಗ್ = 0;
	ರಚನೆ name_ref_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	ರಚನೆ command cmd;
	ರಚನೆ option opts[] = {
		OPT_BOOL(0, "name-only", &ದತ್ತಾಂಶ.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &ದತ್ತಾಂಶ.ಟ್ಯಾಗುಗಳು_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &ದತ್ತಾಂಶ.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &ದತ್ತಾಂಶ.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_ಗುಂಪು(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#ಆದರೆ_ಅಂತ್ಯ /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_ಟ್ಯಾಗ್,
				N_("dereference tags in the input (internal use)")),
		OPT_ಕೊನೆ(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_ಹೆಸರುಗಳು);
	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, ದತ್ತಾಂಶ.name_only);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	ಆದರೆ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	ಆದರೆ (all || annotate_stdin)
		disable_cutoff();

	ಗಾಗಿ (; argc; argc--, argv++) {
		ರಚನೆ object_id oid;
		ರಚನೆ object *object;
		ರಚನೆ commit *commit;

		ಆದರೆ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			ಮುಂದುವರಿ;
		}

		commit = ಖಾಲಿ;
		object = parse_object(the_repository, &oid);
		ಆದರೆ (object) {
			ರಚನೆ object *peeled = deref_ಟ್ಯಾಗ್(the_repository,
							  object, *argv, 0);
			ಆದರೆ (peeled && peeled->type == OBJ_COMMIT)
				commit = (ರಚನೆ commit *)peeled;
		}

		ಆದರೆ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (commit)
			ಗಣ_commit_cutoff(commit);

		ಆದರೆ (peel_ಟ್ಯಾಗ್) {
			ಆದರೆ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				ಮುಂದುವರಿ;
			}
			object = (ರಚನೆ object *)commit;
		}
		ಕೂಡಿಸು_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &ದತ್ತಾಂಶ);
	name_tips(&string_pool);

	ಆದರೆ (annotate_stdin) {
		ರಚನೆ strbuf sb = STRBUF_INIT;

		ಆಗ (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_ಸಾಲು(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (all) {
		ಪೂರ್ಣಾಂಕ i, max;

		max = get_max_object_ಸೂಚ್ಯಂಕ(the_repository);
		ಗಾಗಿ (i = 0; i < max; i++) {
			ರಚನೆ object *obj = get_ಸೂಚ್ಯಂಕಿತ_object(the_repository, i);
			ಆದರೆ (!obj || obj->type != OBJ_COMMIT)
				ಮುಂದುವರಿ;
			show_name(obj, ಖಾಲಿ,
				  always, allow_undefined, ದತ್ತಾಂಶ.name_only);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ i;
		ಗಾಗಿ (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].ಅಂಶ, revs.objects[i].name,
				  always, allow_undefined, ದತ್ತಾಂಶ.name_only);
	}

	string_list_clear(&ದತ್ತಾಂಶ.ref_filters, 0);
	string_list_clear(&ದತ್ತಾಂಶ.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ format_nul_ದತ್ತಾಂಶ {
	bool nul_ಒಳಹರಿವು;
	bool nul_output;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_nul_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			 ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ format_nul_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = option->ಮೌಲ್ಯ;
	ದತ್ತಾಂಶ->nul_ಒಳಹರಿವು = 1;
	ದತ್ತಾಂಶ->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಎಣಿಕೆ stdin_ಬಹುಲಕ parse_stdin_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stdin_ಬಹುಲಕ)
{
	ಆದರೆ (!strcmp(stdin_ಬಹುಲಕ, "text"))
		ಹಿಂತಿರುಗಿ ಪಠ್ಯ_2;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(stdin_ಬಹುಲಕ, "revs") ||
		 !strcmp(stdin_ಬಹುಲಕ, "rev"))
		ಹಿಂತಿರುಗಿ REVS;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_format_rev(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format = ಖಾಲಿ;
	ಎಣಿಕೆ stdin_ಬಹುಲಕ stdin_ಬಹುಲಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stdin_ಬಹುಲಕ_arg = ಖಾಲಿ;
	ರಚನೆ format_nul_ದತ್ತಾಂಶ nul_ದತ್ತಾಂಶ = { 0, 0 };
	ಅಕ್ಷರ output_terminator;
	strbuf_getline_fn getline_fn;
	ರಚನೆ display_notes_opt format_notes_opt;
	ರಚನೆ rev_info format_rev = REV_INFO_INIT;
	ರಚನೆ pretty_format format_pp = { 0 };
	ರಚನೆ string_list notes = STRING_LIST_INIT_NODUP;
	ರಚನೆ strbuf scratch_buf = STRBUF_INIT;
	ರಚನೆ command cmd;
	ರಚನೆ option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_ಬಹುಲಕ_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_ದತ್ತಾಂಶ, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_ದತ್ತಾಂಶ.nul_ಒಳಹರಿವು,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_ದತ್ತಾಂಶ.nul_output,
			 N_("Use NUL for output termination")),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, format_rev_usage, 0);

	ಆದರೆ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	ಆದರೆ (!format)
		die(_("'%s' is required"), "--format");
	ಆದರೆ (!stdin_ಬಹುಲಕ_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_ದತ್ತಾಂಶ.nul_ಒಳಹರಿವು ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_ದತ್ತಾಂಶ.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_ಬಹುಲಕ = parse_stdin_ಬಹುಲಕ(stdin_ಬಹುಲಕ_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_ಬಹುಲಕ_explicit = format_rev.date_ಬಹುಲಕ_explicit;
	format_pp.ctx.date_ಬಹುಲಕ = format_rev.date_ಬಹುಲಕ;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_ಕಂಡುಹಿಡಿ_requirements(format,
				     &format_pp.want);
	ಆದರೆ (format_pp.want.notes) {
		ಪೂರ್ಣಾಂಕ ignore_show_notes = 0;
		ರಚನೆ string_list_ಅಂಶ *n;

		for_each_string_list_ಅಂಶ(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		ತುಂಬು_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	ಆಯ್ಕೆ (stdin_ಬಹುಲಕ) {
	ಸ್ಥಿತಿ ಪಠ್ಯ_2:
		ಆಗ (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_ಸಾಲು(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			ಬರೆ("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ REVS:
		ಆಗ (getline_fn(&scratch_buf, stdin) != EOF) {
			ರಚನೆ object_id oid;
			ರಚನೆ object *object;
			ರಚನೆ object *peeled;

			ಆದರೆ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				ಮುಂದುವರಿ;
			}

			object = parse_object(the_repository, &oid);
			ಆದರೆ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				ಮುಂದುವರಿ;
			}

			peeled = deref_ಟ್ಯಾಗ್(the_repository, object, scratch_buf.buf, 0);
			ಆದರೆ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				ಮುಂದುವರಿ;
			}

			get_format_rev((ರಚನೆ commit *)peeled,
				       &format_pp, &scratch_buf);
			ಬರೆ("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("uncovered case: %d", stdin_ಬಹುಲಕ);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	ಹಿಂತಿರುಗಿ 0;
}
