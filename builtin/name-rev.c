#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash-lookup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-slab.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-graph.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mem-pool.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pretty.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "notes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CUTOFF_DATE_SLOP 86400

ਬਣਤਰ rev_name {
	ਅਚਲ ਅੱਖਰ *tip_name;
	timestamp_t taggerdate;
	ਪੂਰਨ generation;
	ਪੂਰਨ ਦੂਰੀ;
	ਪੂਰਨ from_ਟੈਗ;
};

define_commit_slab(commit_rev_name, ਬਣਤਰ rev_name);

ਸਥਿਰ timestamp_t generation_cutoff = GENERATION_ਸੰਖਿਆ_INFINITY;
ਸਥਿਰ timestamp_t cutoff = ਸਮਾਂ_MAX;
ਸਥਿਰ ਬਣਤਰ commit_rev_name rev_ਨਾਂ;

/* Disable the cutoff checks entirely */
ਸਥਿਰ ਖਾਲੀ disable_cutoff(ਖਾਲੀ)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_commit_cutoff(ਬਣਤਰ commit *commit)
{

	ਜੇ (cutoff > commit->date)
		cutoff = commit->date;

	ਜੇ (generation_cutoff) {
		timestamp_t generation = commit_ਗ੍ਰਾਫ_generation(commit);

		ਜੇ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
ਸਥਿਰ ਖਾਲੀ adjust_cutoff_timestamp_for_slop(ਖਾਲੀ)
{
	ਜੇ (cutoff) {
		/* check for underflow */
		ਜੇ (cutoff > ਸਮਾਂ_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		ਨਹੀਂਤਾਂ
			cutoff = ਸਮਾਂ_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ਸਥਿਰ ਪੂਰਨ commit_is_before_cutoff(ਬਣਤਰ commit *commit)
{
	ਜੇ (generation_cutoff < GENERATION_ਸੰਖਿਆ_INFINITY)
		ਵਾਪਸ generation_cutoff &&
			commit_ਗ੍ਰਾਫ_generation(commit) < generation_cutoff;

	ਵਾਪਸ commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਿਲਾਓ_TRAVERSAL_ਭਾਰ 65535

ਸਥਿਰ ਪੂਰਨ is_valid_rev_name(ਅਚਲ ਬਣਤਰ rev_name *name)
{
	ਵਾਪਸ name && name->tip_name;
}

ਸਥਿਰ ਬਣਤਰ rev_name *get_commit_rev_name(ਅਚਲ ਬਣਤਰ commit *commit)
{
	ਬਣਤਰ rev_name *name = commit_rev_name_peek(&rev_ਨਾਂ, commit);

	ਵਾਪਸ is_valid_rev_name(name) ? name : ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ effective_ਦੂਰੀ(ਪੂਰਨ ਦੂਰੀ, ਪੂਰਨ generation)
{
	ਵਾਪਸ ਦੂਰੀ + (generation > 0 ? ਮਿਲਾਓ_TRAVERSAL_ਭਾਰ : 0);
}

ਸਥਿਰ ਪੂਰਨ is_better_name(ਬਣਤਰ rev_name *name,
			  timestamp_t taggerdate,
			  ਪੂਰਨ generation,
			  ਪੂਰਨ ਦੂਰੀ,
			  ਪੂਰਨ from_ਟੈਗ)
{
	ਪੂਰਨ name_ਦੂਰੀ = effective_ਦੂਰੀ(name->ਦੂਰੀ, name->generation);
	ਪੂਰਨ new_ਦੂਰੀ = effective_ਦੂਰੀ(ਦੂਰੀ, generation);

	/* If both are tags, we prefer the nearer one. */
	ਜੇ (from_ਟੈਗ && name->from_ਟੈਗ)
		ਵਾਪਸ name_ਦੂਰੀ > new_ਦੂਰੀ;

	/* Favor a tag over a non-tag. */
	ਜੇ (name->from_ਟੈਗ != from_ਟੈਗ)
		ਵਾਪਸ from_ਟੈਗ;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	ਜੇ (name_ਦੂਰੀ != new_ਦੂਰੀ)
		ਵਾਪਸ name_ਦੂਰੀ > new_ਦੂਰੀ;

	/* ... or tiebreak to favor older date */
	ਜੇ (name->taggerdate != taggerdate)
		ਵਾਪਸ name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ rev_name *create_or_update_name(ਬਣਤਰ commit *commit,
					      timestamp_t taggerdate,
					      ਪੂਰਨ generation, ਪੂਰਨ ਦੂਰੀ,
					      ਪੂਰਨ from_ਟੈਗ)
{
	ਬਣਤਰ rev_name *name = commit_rev_name_at(&rev_ਨਾਂ, commit);

	ਜੇ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, ਦੂਰੀ, from_ਟੈਗ))
		ਵਾਪਸ ਨਿਰਾ;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->ਦੂਰੀ = ਦੂਰੀ;
	name->from_ਟੈਗ = from_ਟੈਗ;

	ਵਾਪਸ name;
}

ਸਥਿਰ ਅੱਖਰ *get_ਮਾਪਾ_name(ਅਚਲ ਬਣਤਰ rev_name *name, ਪੂਰਨ ਮਾਪਾ_ਸੰਖਿਆ,
			     ਬਣਤਰ mem_pool *string_pool)
{
	ਮਾਪ_t len;

	strip_suffix(name->tip_name, "^0", &len);
	ਜੇ (name->generation > 0) {
		ਵਾਪਸ mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (ਪੂਰਨ)len, name->tip_name,
				       name->generation, ਮਾਪਾ_ਸੰਖਿਆ);
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ mem_pool_strfmt(string_pool, "%.*s^%d",
				       (ਪੂਰਨ)len, name->tip_name, ਮਾਪਾ_ਸੰਖਿਆ);
	}
}

ਸਥਿਰ ਖਾਲੀ name_rev(ਬਣਤਰ commit *ਸ਼ੁਰੂ_commit,
		ਅਚਲ ਅੱਖਰ *tip_name, timestamp_t taggerdate,
		ਪੂਰਨ from_ਟੈਗ, ਪੂਰਨ deref, ਬਣਤਰ mem_pool *string_pool)
{
	ਬਣਤਰ commit_ਢੇਰ ਢੇਰ = COMMIT_ਢੇਰ_INIT;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ commit_ਢੇਰ parents_ਮੰਜ਼ਿਲ_ਕਤਾਰ = COMMIT_ਢੇਰ_INIT;
	ਬਣਤਰ rev_name *ਸ਼ੁਰੂ_name;

	repo_parse_commit(the_repository, ਸ਼ੁਰੂ_commit);
	ਜੇ (commit_is_before_cutoff(ਸ਼ੁਰੂ_commit))
		ਵਾਪਸ;

	ਸ਼ੁਰੂ_name = create_or_update_name(ਸ਼ੁਰੂ_commit, taggerdate, 0, 0,
					   from_ਟੈਗ);
	ਜੇ (!ਸ਼ੁਰੂ_name)
		ਵਾਪਸ;
	ਜੇ (deref)
		ਸ਼ੁਰੂ_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	ਨਹੀਂਤਾਂ
		ਸ਼ੁਰੂ_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ਢੇਰ_ਧੱਕੋ(&ਢੇਰ, ਸ਼ੁਰੂ_commit);

	ਜਦੋਂ ((commit = commit_ਢੇਰ_ਕੱਢੋ(&ਢੇਰ))) {
		ਬਣਤਰ rev_name *name = get_commit_rev_name(commit);
		ਬਣਤਰ commit_list *parents;
		ਪੂਰਨ ਮਾਪਾ_ਸੰਖਿਆ = 1;

		parents_ਮੰਜ਼ਿਲ_ਕਤਾਰ.nr = 0;

		ਲਈ (parents = commit->parents;
				parents;
				parents = parents->ਅਗਲਾ, ਮਾਪਾ_ਸੰਖਿਆ++) {
			ਬਣਤਰ commit *ਮਾਪਾ = parents->ਵਸਤੂ;
			ਬਣਤਰ rev_name *ਮਾਪਾ_name;
			ਪੂਰਨ generation, ਦੂਰੀ;

			repo_parse_commit(the_repository, ਮਾਪਾ);
			ਜੇ (commit_is_before_cutoff(ਮਾਪਾ))
				ਜਾਰੀ;

			ਜੇ (ਮਾਪਾ_ਸੰਖਿਆ > 1) {
				generation = 0;
				ਦੂਰੀ = name->ਦੂਰੀ + ਮਿਲਾਓ_TRAVERSAL_ਭਾਰ;
			} ਨਹੀਂਤਾਂ {
				generation = name->generation + 1;
				ਦੂਰੀ = name->ਦੂਰੀ + 1;
			}

			ਮਾਪਾ_name = create_or_update_name(ਮਾਪਾ, taggerdate,
							    generation,
							    ਦੂਰੀ, from_ਟੈਗ);
			ਜੇ (ਮਾਪਾ_name) {
				ਜੇ (ਮਾਪਾ_ਸੰਖਿਆ > 1)
					ਮਾਪਾ_name->tip_name =
						get_ਮਾਪਾ_name(name,
								ਮਾਪਾ_ਸੰਖਿਆ,
								string_pool);
				ਨਹੀਂਤਾਂ
					ਮਾਪਾ_name->tip_name = name->tip_name;
				commit_ਢੇਰ_ਧੱਕੋ(&parents_ਮੰਜ਼ਿਲ_ਕਤਾਰ, ਮਾਪਾ);
			}
		}

		/* The first parent must come out first from the stack */
		ਜਦੋਂ (parents_ਮੰਜ਼ਿਲ_ਕਤਾਰ.nr)
			commit_ਢੇਰ_ਧੱਕੋ(&ਢੇਰ,
					  commit_ਢੇਰ_ਕੱਢੋ(&parents_ਮੰਜ਼ਿਲ_ਕਤਾਰ));
	}

	commit_ਢੇਰ_clear(&ਢੇਰ);
	commit_ਢੇਰ_clear(&parents_ਮੰਜ਼ਿਲ_ਕਤਾਰ);
}

ਸਥਿਰ ਪੂਰਨ subpath_matches(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *filter)
{
	ਅਚਲ ਅੱਖਰ *subpath = ਰਸਤਾ;

	ਜਦੋਂ (subpath) {
		ਜੇ (!wildmatch(filter, subpath, 0))
			ਵਾਪਸ subpath - ਰਸਤਾ;
		subpath = strchr(subpath, '/');
		ਜੇ (subpath)
			subpath++;
	}
	ਵਾਪਸ -1;
}

ਬਣਤਰ name_ref_ਅੰਕੜੇ {
	ਪੂਰਨ ਟੈਗ_ਸੂਚੀ_only;
	ਪੂਰਨ name_only;
	ਬਣਤਰ string_list ref_filters;
	ਬਣਤਰ string_list exclude_filters;
};

ਬਣਤਰ pretty_format {
	ਬਣਤਰ pretty_print_context ctx;
	ਬਣਤਰ userformat_want want;
};

ਗਿਣਤੀ command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

ਗਿਣਤੀ stdin_ਬਹੁਲਕ {
    ਪਾਠ_2 = 1,
    REVS = 2,
};

ਬਣਤਰ command {
	ਗਿਣਤੀ command_type type;
	ਸੰਘ {
		ਪੂਰਨ name_only;
		ਬਣਤਰ pretty_format *pretty_format;
	} u;
};

ਸਥਿਰ ਖਾਲੀ init_name_rev_command(ਬਣਤਰ command *cmd,
				  ਪੂਰਨ name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

ਸਥਿਰ ਖਾਲੀ init_format_rev_command(ਬਣਤਰ command *cmd,
				    ਬਣਤਰ pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

ਸਥਿਰ ਬਣਤਰ tip_ਸਾਰਣੀ {
	ਬਣਤਰ tip_ਸਾਰਣੀ_ਇੰਦਰਾਜ {
		ਬਣਤਰ object_id oid;
		ਅਚਲ ਅੱਖਰ *refname;
		ਬਣਤਰ commit *commit;
		timestamp_t taggerdate;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ from_ਟੈਗ:1;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ deref:1;
	} *ਸਾਰਣੀ;
	ਪੂਰਨ nr;
	ਪੂਰਨ alloc;
	ਪੂਰਨ sorted;
} tip_ਸਾਰਣੀ;

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ_tip_ਸਾਰਣੀ(ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *refname,
			     ਪੂਰਨ shorten_unambiguous, ਬਣਤਰ commit *commit,
			     timestamp_t taggerdate, ਪੂਰਨ from_ਟੈਗ, ਪੂਰਨ deref)
{
	ਅੱਖਰ *short_refname = ਨਿਰਾ;

	ਜੇ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	ਨਹੀਂਤਾਂ
		skip_ਅਗੇਤਰ(refname, "refs/", &refname);

	ALLOC_GROW(tip_ਸਾਰਣੀ.ਸਾਰਣੀ, tip_ਸਾਰਣੀ.nr + 1, tip_ਸਾਰਣੀ.alloc);
	oidcpy(&tip_ਸਾਰਣੀ.ਸਾਰਣੀ[tip_ਸਾਰਣੀ.nr].oid, oid);
	tip_ਸਾਰਣੀ.ਸਾਰਣੀ[tip_ਸਾਰਣੀ.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_ਸਾਰਣੀ.ਸਾਰਣੀ[tip_ਸਾਰਣੀ.nr].commit = commit;
	tip_ਸਾਰਣੀ.ਸਾਰਣੀ[tip_ਸਾਰਣੀ.nr].taggerdate = taggerdate;
	tip_ਸਾਰਣੀ.ਸਾਰਣੀ[tip_ਸਾਰਣੀ.nr].from_ਟੈਗ = from_ਟੈਗ;
	tip_ਸਾਰਣੀ.ਸਾਰਣੀ[tip_ਸਾਰਣੀ.nr].deref = deref;
	tip_ਸਾਰਣੀ.nr++;
	tip_ਸਾਰਣੀ.sorted = 0;
}

ਸਥਿਰ ਪੂਰਨ tipcmp(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਅਚਲ ਬਣਤਰ tip_ਸਾਰਣੀ_ਇੰਦਰਾਜ *a = a_, *b = b_;
	ਵਾਪਸ oidcmp(&a->oid, &b->oid);
}

ਸਥਿਰ ਪੂਰਨ cmp_by_ਟੈਗ_and_age(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਅਚਲ ਬਣਤਰ tip_ਸਾਰਣੀ_ਇੰਦਰਾਜ *a = a_, *b = b_;
	ਪੂਰਨ cmp;

	/* Prefer tags. */
	cmp = b->from_ਟੈਗ - a->from_ਟੈਗ;
	ਜੇ (cmp)
		ਵਾਪਸ cmp;

	/* Older is better. */
	ਜੇ (a->taggerdate < b->taggerdate)
		ਵਾਪਸ -1;
	ਵਾਪਸ a->taggerdate != b->taggerdate;
}

ਸਥਿਰ ਪੂਰਨ name_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ object *o = parse_object(the_repository, ref->oid);
	ਬਣਤਰ name_ref_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਪੂਰਨ can_abbreviate_output = ਅੰਕੜੇ->ਟੈਗ_ਸੂਚੀ_only && ਅੰਕੜੇ->name_only;
	ਪੂਰਨ deref = 0;
	ਪੂਰਨ from_ਟੈਗ = 0;
	ਬਣਤਰ commit *commit = ਨਿਰਾ;
	timestamp_t taggerdate = ਸਮਾਂ_MAX;

	ਜੇ (ਅੰਕੜੇ->ਟੈਗ_ਸੂਚੀ_only && !starts_with(ref->name, "refs/tags/"))
		ਵਾਪਸ 0;

	ਜੇ (ਅੰਕੜੇ->exclude_filters.nr) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਅੰਕੜੇ->exclude_filters) {
			ਜੇ (subpath_matches(ref->name, ਵਸਤੂ->string) >= 0)
				ਵਾਪਸ 0;
		}
	}

	ਜੇ (ਅੰਕੜੇ->ref_filters.nr) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		ਪੂਰਨ matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਅੰਕੜੇ->ref_filters) {
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
			ਚੋਣ (subpath_matches(ref->name, ਵਸਤੂ->string)) {
			ਹਾਲਤ -1: /* did not match */
				ਰੋਕੋ;
			ਹਾਲਤ 0: /* matched fully */
				matched = 1;
				ਰੋਕੋ;
			ਮੂਲ: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				ਰੋਕੋ;
			}
		}

		/* If none of the patterns matched, stop now */
		ਜੇ (!matched)
			ਵਾਪਸ 0;
	}

	ਜਦੋਂ (o && o->type == OBJ_ਟੈਗ) {
		ਬਣਤਰ ਟੈਗ *t = (ਬਣਤਰ ਟੈਗ *) o;
		ਜੇ (!t->tagged)
			ਰੋਕੋ; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	ਜੇ (o && o->type == OBJ_COMMIT) {
		commit = (ਬਣਤਰ commit *)o;
		from_ਟੈਗ = starts_with(ref->name, "refs/tags/");
		ਜੇ (taggerdate == ਸਮਾਂ_MAX)
			taggerdate = commit->date;
	}

	ਜੋੜੋ_ਮੰਜ਼ਿਲ_tip_ਸਾਰਣੀ(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_ਟੈਗ, deref);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ name_tips(ਬਣਤਰ mem_pool *string_pool)
{
	ਪੂਰਨ i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_ਸਾਰਣੀ.ਸਾਰਣੀ, tip_ਸਾਰਣੀ.nr, cmp_by_ਟੈਗ_and_age);
	ਲਈ (i = 0; i < tip_ਸਾਰਣੀ.nr; i++) {
		ਬਣਤਰ tip_ਸਾਰਣੀ_ਇੰਦਰਾਜ *e = &tip_ਸਾਰਣੀ.ਸਾਰਣੀ[i];
		ਜੇ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_ਟੈਗ, e->deref, string_pool);
		}
	}
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ object_id *nth_tip_ਸਾਰਣੀ_ent(ਮਾਪ_t ix, ਅਚਲ ਖਾਲੀ *ਸਾਰਣੀ_2)
{
	ਅਚਲ ਬਣਤਰ tip_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਸਾਰਣੀ = ਸਾਰਣੀ_2;
	ਵਾਪਸ &ਸਾਰਣੀ[ix].oid;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_exact_ref_ਮੇਲ(ਅਚਲ ਬਣਤਰ object *o)
{
	ਪੂਰਨ ਮਿਲਿਆ_2;

	ਜੇ (!tip_ਸਾਰਣੀ.ਸਾਰਣੀ || !tip_ਸਾਰਣੀ.nr)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (!tip_ਸਾਰਣੀ.sorted) {
		QSORT(tip_ਸਾਰਣੀ.ਸਾਰਣੀ, tip_ਸਾਰਣੀ.nr, tipcmp);
		tip_ਸਾਰਣੀ.sorted = 1;
	}

	ਮਿਲਿਆ_2 = oid_pos(&o->oid, tip_ਸਾਰਣੀ.ਸਾਰਣੀ, tip_ਸਾਰਣੀ.nr,
			nth_tip_ਸਾਰਣੀ_ent);
	ਜੇ (0 <= ਮਿਲਿਆ_2)
		ਵਾਪਸ tip_ਸਾਰਣੀ.ਸਾਰਣੀ[ਮਿਲਿਆ_2].refname;
	ਵਾਪਸ ਨਿਰਾ;
}

/* may return a constant string or use "buf" as scratch space */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_rev_name(ਅਚਲ ਬਣਤਰ object *o, ਬਣਤਰ strbuf *buf)
{
	ਬਣਤਰ rev_name *n;
	ਅਚਲ ਬਣਤਰ commit *c;

	ਜੇ (o->type != OBJ_COMMIT)
		ਵਾਪਸ get_exact_ref_ਮੇਲ(o);
	c = (ਅਚਲ ਬਣਤਰ commit *) o;
	n = get_commit_rev_name(c);
	ਜੇ (!n)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (!n->generation) {
		ਵਾਪਸ n->tip_name;
	} ਨਹੀਂਤਾਂ {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		ਵਾਪਸ buf->buf;
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_format_rev(ਅਚਲ ਬਣਤਰ commit *c,
				  ਬਣਤਰ pretty_format *format_ctx,
				  ਬਣਤਰ strbuf *buf)
{
	strbuf_reset(buf);

	ਜੇ (format_ctx->want.notes) {
		ਬਣਤਰ strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_ਸੁਨੇਹਾ = strbuf_detach(&notebuf, ਨਿਰਾ);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_ਸੁਨੇਹਾ);

	ਵਾਪਸ buf->buf;
}

ਸਥਿਰ ਖਾਲੀ show_name(ਅਚਲ ਬਣਤਰ object *obj,
		      ਅਚਲ ਅੱਖਰ *caller_name,
		      ਪੂਰਨ always, ਪੂਰਨ allow_undefined, ਪੂਰਨ name_only)
{
	ਅਚਲ ਅੱਖਰ *name;
	ਅਚਲ ਬਣਤਰ object_id *oid = &obj->oid;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	ਜੇ (!name_only)
		ਲਿਖੋ("%s ", caller_name ? caller_name : oid_ਮੰਜ਼ਿਲ_hex(oid));
	name = get_rev_name(obj, &buf);
	ਜੇ (name)
		ਲਿਖੋ("%s\n", name);
	ਨਹੀਂਤਾਂ ਜੇ (allow_undefined)
		ਲਿਖੋ("undefined\n");
	ਨਹੀਂਤਾਂ ਜੇ (always)
		ਲਿਖੋ("%s\n",
		       repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	ਨਹੀਂਤਾਂ
		die("cannot describe '%s'", oid_ਮੰਜ਼ਿਲ_hex(oid));
	strbuf_release(&buf);
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	ਨਿਰਾ
};

ਸਥਿਰ ਖਾਲੀ name_rev_ਸਤਰ(ਅੱਖਰ *p, ਬਣਤਰ command *cmd)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਪੂਰਨ counter = 0;
	ਅੱਖਰ *p_ਸ਼ੁਰੂ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hexsz = the_hash_algo->hexsz;

	ਲਈ (p_ਸ਼ੁਰੂ = p; *p; p++) {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ishex(x) (ਅੰਕ_ਹੈ((x)) || ((x) >= 'a' && (x) <= 'f'))
		ਜੇ (!ishex(*p)) {
			counter = 0;
		} ਨਹੀਂਤਾਂ ਜੇ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			ਬਣਤਰ object_id oid;
			ਅਚਲ ਅੱਖਰ *name = ਨਿਰਾ;
			ਅੱਖਰ c = *(p + 1);
			ਪੂਰਨ p_len = p - p_ਸ਼ੁਰੂ + 1;
			ਬਣਤਰ object *o = ਨਿਰਾ;
			ਪੂਰਨ oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			ਚੋਣ (cmd->type) {
			ਹਾਲਤ NAME_REV:
				ਜੇ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				ਜੇ (o)
					name = get_rev_name(o, &buf);
				ਜੇ (!name)
					ਜਾਰੀ;
				ਜੇ (cmd->u.name_only)
					ਲਿਖੋ("%.*s%s", p_len - hexsz, p_ਸ਼ੁਰੂ, name);
				ਨਹੀਂਤਾਂ
					ਲਿਖੋ("%.*s (%s)", p_len, p_ਸ਼ੁਰੂ, name);
				ਰੋਕੋ;
			ਹਾਲਤ FORMAT_REV:
				ਜੇ (!oid_ret)
					o = parse_object(the_repository, &oid);
				ਜੇ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ਅਚਲ ਬਣਤਰ commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				ਜੇ (name)
					ਲਿਖੋ("%.*s%s", p_len - hexsz, p_ਸ਼ੁਰੂ, name);
				ਨਹੀਂਤਾਂ
					ਲਿਖੋ("%.*s", p_len, p_ਸ਼ੁਰੂ);
				ਰੋਕੋ;
			ਮੂਲ:
				BUG("uncovered case: %d", cmd->type);
			}

			p_ਸ਼ੁਰੂ = p + 1;
		}
	}

	/* flush */
	ਜੇ (p_ਸ਼ੁਰੂ != p)
		fwrite(p_ਸ਼ੁਰੂ, p - p_ਸ਼ੁਰੂ, 1, stdout);

	strbuf_release(&buf);
}

ਪੂਰਨ cmd_name_rev(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ mem_pool string_pool;
	ਬਣਤਰ object_array revs = OBJECT_ARRAY_INIT;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	ਪੂਰਨ transform_stdin = 0;
#ਜੇ_ਅੰਤ
	ਪੂਰਨ all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_ਟੈਗ = 0;
	ਬਣਤਰ name_ref_ਅੰਕੜੇ ਅੰਕੜੇ = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	ਬਣਤਰ command cmd;
	ਬਣਤਰ option opts[] = {
		OPT_BOOL(0, "name-only", &ਅੰਕੜੇ.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &ਅੰਕੜੇ.ਟੈਗ_ਸੂਚੀ_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &ਅੰਕੜੇ.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &ਅੰਕੜੇ.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_ਸਮੂਹ(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#ਜੇ_ਅੰਤ /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_ਟੈਗ,
				N_("dereference tags in the input (internal use)")),
		OPT_ਸਿਰਾ(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_ਨਾਂ);
	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, ਅੰਕੜੇ.name_only);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	ਜੇ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#ਜੇ_ਅੰਤ

	ਜੇ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	ਜੇ (all || annotate_stdin)
		disable_cutoff();

	ਲਈ (; argc; argc--, argv++) {
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ object *object;
		ਬਣਤਰ commit *commit;

		ਜੇ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			ਜਾਰੀ;
		}

		commit = ਨਿਰਾ;
		object = parse_object(the_repository, &oid);
		ਜੇ (object) {
			ਬਣਤਰ object *peeled = deref_ਟੈਗ(the_repository,
							  object, *argv, 0);
			ਜੇ (peeled && peeled->type == OBJ_COMMIT)
				commit = (ਬਣਤਰ commit *)peeled;
		}

		ਜੇ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			ਜਾਰੀ;
		}

		ਜੇ (commit)
			ਸੈੱਟ_commit_cutoff(commit);

		ਜੇ (peel_ਟੈਗ) {
			ਜੇ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				ਜਾਰੀ;
			}
			object = (ਬਣਤਰ object *)commit;
		}
		ਜੋੜੋ_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &ਅੰਕੜੇ);
	name_tips(&string_pool);

	ਜੇ (annotate_stdin) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;

		ਜਦੋਂ (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_ਸਤਰ(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} ਨਹੀਂਤਾਂ ਜੇ (all) {
		ਪੂਰਨ i, max;

		max = get_max_object_ਸੂਚਕਾਂਕ(the_repository);
		ਲਈ (i = 0; i < max; i++) {
			ਬਣਤਰ object *obj = get_ਸੂਚਕਾਂਕਿਤ_object(the_repository, i);
			ਜੇ (!obj || obj->type != OBJ_COMMIT)
				ਜਾਰੀ;
			show_name(obj, ਨਿਰਾ,
				  always, allow_undefined, ਅੰਕੜੇ.name_only);
		}
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ i;
		ਲਈ (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].ਵਸਤੂ, revs.objects[i].name,
				  always, allow_undefined, ਅੰਕੜੇ.name_only);
	}

	string_list_clear(&ਅੰਕੜੇ.ref_filters, 0);
	string_list_clear(&ਅੰਕੜੇ.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	ਵਾਪਸ 0;
}

ਬਣਤਰ format_nul_ਅੰਕੜੇ {
	bool nul_ਨਿਵੇਸ਼;
	bool nul_output;
};

ਸਥਿਰ ਪੂਰਨ format_nul_cb(ਅਚਲ ਬਣਤਰ option *option,
			 ਅਚਲ ਅੱਖਰ *arg,
			 ਪੂਰਨ unset)
{
	ਬਣਤਰ format_nul_ਅੰਕੜੇ *ਅੰਕੜੇ = option->ਮੁੱਲ;
	ਅੰਕੜੇ->nul_ਨਿਵੇਸ਼ = 1;
	ਅੰਕੜੇ->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਗਿਣਤੀ stdin_ਬਹੁਲਕ parse_stdin_ਬਹੁਲਕ(ਅਚਲ ਅੱਖਰ *stdin_ਬਹੁਲਕ)
{
	ਜੇ (!strcmp(stdin_ਬਹੁਲਕ, "text"))
		ਵਾਪਸ ਪਾਠ_2;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(stdin_ਬਹੁਲਕ, "revs") ||
		 !strcmp(stdin_ਬਹੁਲਕ, "rev"))
		ਵਾਪਸ REVS;
	ਨਹੀਂਤਾਂ
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_format_rev(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *format = ਨਿਰਾ;
	ਗਿਣਤੀ stdin_ਬਹੁਲਕ stdin_ਬਹੁਲਕ;
	ਅਚਲ ਅੱਖਰ *stdin_ਬਹੁਲਕ_arg = ਨਿਰਾ;
	ਬਣਤਰ format_nul_ਅੰਕੜੇ nul_ਅੰਕੜੇ = { 0, 0 };
	ਅੱਖਰ output_terminator;
	strbuf_getline_fn getline_fn;
	ਬਣਤਰ display_notes_opt format_notes_opt;
	ਬਣਤਰ rev_info format_rev = REV_INFO_INIT;
	ਬਣਤਰ pretty_format format_pp = { 0 };
	ਬਣਤਰ string_list notes = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ strbuf scratch_buf = STRBUF_INIT;
	ਬਣਤਰ command cmd;
	ਬਣਤਰ option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_ਬਹੁਲਕ_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_ਅੰਕੜੇ, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_ਅੰਕੜੇ.nul_ਨਿਵੇਸ਼,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_ਅੰਕੜੇ.nul_output,
			 N_("Use NUL for output termination")),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, format_rev_usage, 0);

	ਜੇ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	ਜੇ (!format)
		die(_("'%s' is required"), "--format");
	ਜੇ (!stdin_ਬਹੁਲਕ_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_ਅੰਕੜੇ.nul_ਨਿਵੇਸ਼ ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_ਅੰਕੜੇ.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_ਬਹੁਲਕ = parse_stdin_ਬਹੁਲਕ(stdin_ਬਹੁਲਕ_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_ਬਹੁਲਕ_explicit = format_rev.date_ਬਹੁਲਕ_explicit;
	format_pp.ctx.date_ਬਹੁਲਕ = format_rev.date_ਬਹੁਲਕ;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_ਲੱਭੋ_requirements(format,
				     &format_pp.want);
	ਜੇ (format_pp.want.notes) {
		ਪੂਰਨ ignore_show_notes = 0;
		ਬਣਤਰ string_list_ਵਸਤੂ *n;

		for_each_string_list_ਵਸਤੂ(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		ਲੱਦੋ_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	ਚੋਣ (stdin_ਬਹੁਲਕ) {
	ਹਾਲਤ ਪਾਠ_2:
		ਜਦੋਂ (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_ਸਤਰ(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			ਲਿਖੋ("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		ਰੋਕੋ;
	ਹਾਲਤ REVS:
		ਜਦੋਂ (getline_fn(&scratch_buf, stdin) != EOF) {
			ਬਣਤਰ object_id oid;
			ਬਣਤਰ object *object;
			ਬਣਤਰ object *peeled;

			ਜੇ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				ਜਾਰੀ;
			}

			object = parse_object(the_repository, &oid);
			ਜੇ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				ਜਾਰੀ;
			}

			peeled = deref_ਟੈਗ(the_repository, object, scratch_buf.buf, 0);
			ਜੇ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				ਜਾਰੀ;
			}

			get_format_rev((ਬਣਤਰ commit *)peeled,
				       &format_pp, &scratch_buf);
			ਲਿਖੋ("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("uncovered case: %d", stdin_ਬਹੁਲਕ);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	ਵਾਪਸ 0;
}
