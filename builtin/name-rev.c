#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "hash-lookup.h"
#അന്തര്ഭാവ "commit-slab.h"
#അന്തര്ഭാവ "commit-graph.h"
#അന്തര്ഭാവ "wildmatch.h"
#അന്തര്ഭാവ "mem-pool.h"
#അന്തര്ഭാവ "pretty.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "notes.h"
#അന്തര്ഭാവ "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

ഘടന rev_name {
	സ്ഥിരം അക്ഷരം *tip_name;
	timestamp_t taggerdate;
	പൂർണ്ണസംഖ്യ generation;
	പൂർണ്ണസംഖ്യ distance;
	പൂർണ്ണസംഖ്യ from_tag;
};

define_commit_slab(commit_rev_name, ഘടന rev_name);

സ്ഥിതം timestamp_t generation_cutoff = GENERATION_സംഖ്യ_INFINITY;
സ്ഥിതം timestamp_t cutoff = TIME_MAX;
സ്ഥിതം ഘടന commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
സ്ഥിതം ശൂന്യം disable_cutoff(ശൂന്യം)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
സ്ഥിതം ശൂന്യം set_commit_cutoff(ഘടന commit *commit)
{

	എങ്കിൽ (cutoff > commit->date)
		cutoff = commit->date;

	എങ്കിൽ (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		എങ്കിൽ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
സ്ഥിതം ശൂന്യം adjust_cutoff_timestamp_for_slop(ശൂന്യം)
{
	എങ്കിൽ (cutoff) {
		/* check for underflow */
		എങ്കിൽ (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		അല്ലെങ്കിൽ
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ commit_is_before_cutoff(ഘടന commit *commit)
{
	എങ്കിൽ (generation_cutoff < GENERATION_സംഖ്യ_INFINITY)
		തിരികെ generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	തിരികെ commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

സ്ഥിതം പൂർണ്ണസംഖ്യ is_valid_rev_name(സ്ഥിരം ഘടന rev_name *name)
{
	തിരികെ name && name->tip_name;
}

സ്ഥിതം ഘടന rev_name *get_commit_rev_name(സ്ഥിരം ഘടന commit *commit)
{
	ഘടന rev_name *name = commit_rev_name_peek(&rev_names, commit);

	തിരികെ is_valid_rev_name(name) ? name : ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ effective_distance(പൂർണ്ണസംഖ്യ distance, പൂർണ്ണസംഖ്യ generation)
{
	തിരികെ distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_better_name(ഘടന rev_name *name,
			  timestamp_t taggerdate,
			  പൂർണ്ണസംഖ്യ generation,
			  പൂർണ്ണസംഖ്യ distance,
			  പൂർണ്ണസംഖ്യ from_tag)
{
	പൂർണ്ണസംഖ്യ name_distance = effective_distance(name->distance, name->generation);
	പൂർണ്ണസംഖ്യ new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	എങ്കിൽ (from_tag && name->from_tag)
		തിരികെ name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	എങ്കിൽ (name->from_tag != from_tag)
		തിരികെ from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	എങ്കിൽ (name_distance != new_distance)
		തിരികെ name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	എങ്കിൽ (name->taggerdate != taggerdate)
		തിരികെ name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	തിരികെ 0;
}

സ്ഥിതം ഘടന rev_name *create_or_update_name(ഘടന commit *commit,
					      timestamp_t taggerdate,
					      പൂർണ്ണസംഖ്യ generation, പൂർണ്ണസംഖ്യ distance,
					      പൂർണ്ണസംഖ്യ from_tag)
{
	ഘടന rev_name *name = commit_rev_name_at(&rev_names, commit);

	എങ്കിൽ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		തിരികെ ശൂന്യ;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	തിരികെ name;
}

സ്ഥിതം അക്ഷരം *get_parent_name(സ്ഥിരം ഘടന rev_name *name, പൂർണ്ണസംഖ്യ parent_സംഖ്യ,
			     ഘടന mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	എങ്കിൽ (name->generation > 0) {
		തിരികെ mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (പൂർണ്ണസംഖ്യ)len, name->tip_name,
				       name->generation, parent_സംഖ്യ);
	} അല്ലെങ്കിൽ {
		തിരികെ mem_pool_strfmt(string_pool, "%.*s^%d",
				       (പൂർണ്ണസംഖ്യ)len, name->tip_name, parent_സംഖ്യ);
	}
}

സ്ഥിതം ശൂന്യം name_rev(ഘടന commit *start_commit,
		സ്ഥിരം അക്ഷരം *tip_name, timestamp_t taggerdate,
		പൂർണ്ണസംഖ്യ from_tag, പൂർണ്ണസംഖ്യ deref, ഘടന mem_pool *string_pool)
{
	ഘടന commit_അട്ടി അട്ടി = COMMIT_അട്ടി_INIT;
	ഘടന commit *commit;
	ഘടന commit_അട്ടി parents_to_queue = COMMIT_അട്ടി_INIT;
	ഘടന rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	എങ്കിൽ (commit_is_before_cutoff(start_commit))
		തിരികെ;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	എങ്കിൽ (!start_name)
		തിരികെ;
	എങ്കിൽ (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	അല്ലെങ്കിൽ
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_അട്ടി_തള്ളുക(&അട്ടി, start_commit);

	ആകുമ്പോൾ ((commit = commit_അട്ടി_എടുക്കുക(&അട്ടി))) {
		ഘടന rev_name *name = get_commit_rev_name(commit);
		ഘടന commit_list *parents;
		പൂർണ്ണസംഖ്യ parent_സംഖ്യ = 1;

		parents_to_queue.nr = 0;

		വേണ്ടി (parents = commit->parents;
				parents;
				parents = parents->next, parent_സംഖ്യ++) {
			ഘടന commit *parent = parents->item;
			ഘടന rev_name *parent_name;
			പൂർണ്ണസംഖ്യ generation, distance;

			repo_parse_commit(the_repository, parent);
			എങ്കിൽ (commit_is_before_cutoff(parent))
				തുടരുക;

			എങ്കിൽ (parent_സംഖ്യ > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} അല്ലെങ്കിൽ {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			എങ്കിൽ (parent_name) {
				എങ്കിൽ (parent_സംഖ്യ > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_സംഖ്യ,
								string_pool);
				അല്ലെങ്കിൽ
					parent_name->tip_name = name->tip_name;
				commit_അട്ടി_തള്ളുക(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		ആകുമ്പോൾ (parents_to_queue.nr)
			commit_അട്ടി_തള്ളുക(&അട്ടി,
					  commit_അട്ടി_എടുക്കുക(&parents_to_queue));
	}

	commit_അട്ടി_clear(&അട്ടി);
	commit_അട്ടി_clear(&parents_to_queue);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ subpath_matches(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *filter)
{
	സ്ഥിരം അക്ഷരം *subpath = path;

	ആകുമ്പോൾ (subpath) {
		എങ്കിൽ (!wildmatch(filter, subpath, 0))
			തിരികെ subpath - path;
		subpath = strchr(subpath, '/');
		എങ്കിൽ (subpath)
			subpath++;
	}
	തിരികെ -1;
}

ഘടന name_ref_data {
	പൂർണ്ണസംഖ്യ tags_only;
	പൂർണ്ണസംഖ്യ name_only;
	ഘടന string_list ref_filters;
	ഘടന string_list exclude_filters;
};

ഘടന pretty_format {
	ഘടന pretty_print_context ctx;
	ഘടന userformat_want want;
};

എണ്ണൽ command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

എണ്ണൽ stdin_mode {
    TEXT = 1,
    REVS = 2,
};

ഘടന command {
	എണ്ണൽ command_type type;
	സംഘം {
		പൂർണ്ണസംഖ്യ name_only;
		ഘടന pretty_format *pretty_format;
	} u;
};

സ്ഥിതം ശൂന്യം init_name_rev_command(ഘടന command *cmd,
				  പൂർണ്ണസംഖ്യ name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

സ്ഥിതം ശൂന്യം init_format_rev_command(ഘടന command *cmd,
				    ഘടന pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

സ്ഥിതം ഘടന tip_table {
	ഘടന tip_table_entry {
		ഘടന object_id oid;
		സ്ഥിരം അക്ഷരം *refname;
		ഘടന commit *commit;
		timestamp_t taggerdate;
		unsigned പൂർണ്ണസംഖ്യ from_tag:1;
		unsigned പൂർണ്ണസംഖ്യ deref:1;
	} *table;
	പൂർണ്ണസംഖ്യ nr;
	പൂർണ്ണസംഖ്യ alloc;
	പൂർണ്ണസംഖ്യ sorted;
} tip_table;

സ്ഥിതം ശൂന്യം add_to_tip_table(സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *refname,
			     പൂർണ്ണസംഖ്യ shorten_unambiguous, ഘടന commit *commit,
			     timestamp_t taggerdate, പൂർണ്ണസംഖ്യ from_tag, പൂർണ്ണസംഖ്യ deref)
{
	അക്ഷരം *short_refname = ശൂന്യ;

	എങ്കിൽ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	അല്ലെങ്കിൽ
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

സ്ഥിതം പൂർണ്ണസംഖ്യ tipcmp(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	സ്ഥിരം ഘടന tip_table_entry *a = a_, *b = b_;
	തിരികെ oidcmp(&a->oid, &b->oid);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_by_tag_and_age(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	സ്ഥിരം ഘടന tip_table_entry *a = a_, *b = b_;
	പൂർണ്ണസംഖ്യ cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	എങ്കിൽ (cmp)
		തിരികെ cmp;

	/* Older is better. */
	എങ്കിൽ (a->taggerdate < b->taggerdate)
		തിരികെ -1;
	തിരികെ a->taggerdate != b->taggerdate;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ name_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന object *o = parse_object(the_repository, ref->oid);
	ഘടന name_ref_data *data = cb_data;
	പൂർണ്ണസംഖ്യ can_abbreviate_output = data->tags_only && data->name_only;
	പൂർണ്ണസംഖ്യ deref = 0;
	പൂർണ്ണസംഖ്യ from_tag = 0;
	ഘടന commit *commit = ശൂന്യ;
	timestamp_t taggerdate = TIME_MAX;

	എങ്കിൽ (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		തിരികെ 0;

	എങ്കിൽ (data->exclude_filters.nr) {
		ഘടന string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			എങ്കിൽ (subpath_matches(ref->name, item->string) >= 0)
				തിരികെ 0;
		}
	}

	എങ്കിൽ (data->ref_filters.nr) {
		ഘടന string_list_item *item;
		പൂർണ്ണസംഖ്യ matched = 0;

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
			തിരഞ്ഞെടുപ്പ് (subpath_matches(ref->name, item->string)) {
			അവസ്ഥ -1: /* did not match */
				നിർത്തുക;
			അവസ്ഥ 0: /* matched fully */
				matched = 1;
				നിർത്തുക;
			സ്ഥിരസ്ഥിതി: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				നിർത്തുക;
			}
		}

		/* If none of the patterns matched, stop now */
		എങ്കിൽ (!matched)
			തിരികെ 0;
	}

	ആകുമ്പോൾ (o && o->type == OBJ_TAG) {
		ഘടന tag *t = (ഘടന tag *) o;
		എങ്കിൽ (!t->tagged)
			നിർത്തുക; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	എങ്കിൽ (o && o->type == OBJ_COMMIT) {
		commit = (ഘടന commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		എങ്കിൽ (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം name_tips(ഘടന mem_pool *string_pool)
{
	പൂർണ്ണസംഖ്യ i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	വേണ്ടി (i = 0; i < tip_table.nr; i++) {
		ഘടന tip_table_entry *e = &tip_table.table[i];
		എങ്കിൽ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

സ്ഥിതം സ്ഥിരം ഘടന object_id *nth_tip_table_ent(size_t ix, സ്ഥിരം ശൂന്യം *table_)
{
	സ്ഥിരം ഘടന tip_table_entry *table = table_;
	തിരികെ &table[ix].oid;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_exact_ref_match(സ്ഥിരം ഘടന object *o)
{
	പൂർണ്ണസംഖ്യ found;

	എങ്കിൽ (!tip_table.table || !tip_table.nr)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	എങ്കിൽ (0 <= found)
		തിരികെ tip_table.table[found].refname;
	തിരികെ ശൂന്യ;
}

/* may return a constant string or use "buf" as scratch space */
സ്ഥിതം സ്ഥിരം അക്ഷരം *get_rev_name(സ്ഥിരം ഘടന object *o, ഘടന strbuf *buf)
{
	ഘടന rev_name *n;
	സ്ഥിരം ഘടന commit *c;

	എങ്കിൽ (o->type != OBJ_COMMIT)
		തിരികെ get_exact_ref_match(o);
	c = (സ്ഥിരം ഘടന commit *) o;
	n = get_commit_rev_name(c);
	എങ്കിൽ (!n)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (!n->generation) {
		തിരികെ n->tip_name;
	} അല്ലെങ്കിൽ {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		തിരികെ buf->buf;
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_format_rev(സ്ഥിരം ഘടന commit *c,
				  ഘടന pretty_format *format_ctx,
				  ഘടന strbuf *buf)
{
	strbuf_reset(buf);

	എങ്കിൽ (format_ctx->want.notes) {
		ഘടന strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, ശൂന്യ);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	തിരികെ buf->buf;
}

സ്ഥിതം ശൂന്യം show_name(സ്ഥിരം ഘടന object *obj,
		      സ്ഥിരം അക്ഷരം *caller_name,
		      പൂർണ്ണസംഖ്യ always, പൂർണ്ണസംഖ്യ allow_undefined, പൂർണ്ണസംഖ്യ name_only)
{
	സ്ഥിരം അക്ഷരം *name;
	സ്ഥിരം ഘടന object_id *oid = &obj->oid;
	ഘടന strbuf buf = STRBUF_INIT;

	എങ്കിൽ (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	എങ്കിൽ (name)
		printf("%s\n", name);
	അല്ലെങ്കിൽ എങ്കിൽ (allow_undefined)
		printf("undefined\n");
	അല്ലെങ്കിൽ എങ്കിൽ (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	അല്ലെങ്കിൽ
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	ശൂന്യ
};

സ്ഥിതം ശൂന്യം name_rev_വരി(അക്ഷരം *p, ഘടന command *cmd)
{
	ഘടന strbuf buf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ counter = 0;
	അക്ഷരം *p_start;
	സ്ഥിരം unsigned hexsz = the_hash_algo->hexsz;

	വേണ്ടി (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		എങ്കിൽ (!ishex(*p)) {
			counter = 0;
		} അല്ലെങ്കിൽ എങ്കിൽ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			ഘടന object_id oid;
			സ്ഥിരം അക്ഷരം *name = ശൂന്യ;
			അക്ഷരം c = *(p + 1);
			പൂർണ്ണസംഖ്യ p_len = p - p_start + 1;
			ഘടന object *o = ശൂന്യ;
			പൂർണ്ണസംഖ്യ oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			തിരഞ്ഞെടുപ്പ് (cmd->type) {
			അവസ്ഥ NAME_REV:
				എങ്കിൽ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				എങ്കിൽ (o)
					name = get_rev_name(o, &buf);
				എങ്കിൽ (!name)
					തുടരുക;
				എങ്കിൽ (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				അല്ലെങ്കിൽ
					printf("%.*s (%s)", p_len, p_start, name);
				നിർത്തുക;
			അവസ്ഥ FORMAT_REV:
				എങ്കിൽ (!oid_ret)
					o = parse_object(the_repository, &oid);
				എങ്കിൽ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((സ്ഥിരം ഘടന commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				എങ്കിൽ (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				അല്ലെങ്കിൽ
					printf("%.*s", p_len, p_start);
				നിർത്തുക;
			സ്ഥിരസ്ഥിതി:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	എങ്കിൽ (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

പൂർണ്ണസംഖ്യ cmd_name_rev(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന mem_pool string_pool;
	ഘടന object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	പൂർണ്ണസംഖ്യ transform_stdin = 0;
#endif
	പൂർണ്ണസംഖ്യ all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	ഘടന name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	ഘടന command cmd;
	ഘടന option opts[] = {
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
	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

#ifndef WITH_BREAKING_CHANGES
	എങ്കിൽ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	എങ്കിൽ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	എങ്കിൽ (all || annotate_stdin)
		disable_cutoff();

	വേണ്ടി (; argc; argc--, argv++) {
		ഘടന object_id oid;
		ഘടന object *object;
		ഘടന commit *commit;

		എങ്കിൽ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			തുടരുക;
		}

		commit = ശൂന്യ;
		object = parse_object(the_repository, &oid);
		എങ്കിൽ (object) {
			ഘടന object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			എങ്കിൽ (peeled && peeled->type == OBJ_COMMIT)
				commit = (ഘടന commit *)peeled;
		}

		എങ്കിൽ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			തുടരുക;
		}

		എങ്കിൽ (commit)
			set_commit_cutoff(commit);

		എങ്കിൽ (peel_tag) {
			എങ്കിൽ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				തുടരുക;
			}
			object = (ഘടന object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	എങ്കിൽ (annotate_stdin) {
		ഘടന strbuf sb = STRBUF_INIT;

		ആകുമ്പോൾ (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_വരി(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} അല്ലെങ്കിൽ എങ്കിൽ (all) {
		പൂർണ്ണസംഖ്യ i, max;

		max = get_max_object_index(the_repository);
		വേണ്ടി (i = 0; i < max; i++) {
			ഘടന object *obj = get_indexed_object(the_repository, i);
			എങ്കിൽ (!obj || obj->type != OBJ_COMMIT)
				തുടരുക;
			show_name(obj, ശൂന്യ,
				  always, allow_undefined, data.name_only);
		}
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ i;
		വേണ്ടി (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	തിരികെ 0;
}

ഘടന format_nul_data {
	bool nul_നിവേശം;
	bool nul_output;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ format_nul_cb(സ്ഥിരം ഘടന option *option,
			 സ്ഥിരം അക്ഷരം *arg,
			 പൂർണ്ണസംഖ്യ unset)
{
	ഘടന format_nul_data *data = option->മൂല്യം;
	data->nul_നിവേശം = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	തിരികെ 0;
}

സ്ഥിതം എണ്ണൽ stdin_mode parse_stdin_mode(സ്ഥിരം അക്ഷരം *stdin_mode)
{
	എങ്കിൽ (!strcmp(stdin_mode, "text"))
		തിരികെ TEXT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		തിരികെ REVS;
	അല്ലെങ്കിൽ
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_format_rev(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *format = ശൂന്യ;
	എണ്ണൽ stdin_mode stdin_mode;
	സ്ഥിരം അക്ഷരം *stdin_mode_arg = ശൂന്യ;
	ഘടന format_nul_data nul_data = { 0, 0 };
	അക്ഷരം output_terminator;
	strbuf_getline_fn getline_fn;
	ഘടന display_notes_opt format_notes_opt;
	ഘടന rev_info format_rev = REV_INFO_INIT;
	ഘടന pretty_format format_pp = { 0 };
	ഘടന string_list notes = STRING_LIST_INIT_NODUP;
	ഘടന strbuf scratch_buf = STRBUF_INIT;
	ഘടന command cmd;
	ഘടന option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_നിവേശം,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	എങ്കിൽ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	എങ്കിൽ (!format)
		die(_("'%s' is required"), "--format");
	എങ്കിൽ (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_നിവേശം ? strbuf_getline_nul : strbuf_getline_lf;
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
	എങ്കിൽ (format_pp.want.notes) {
		പൂർണ്ണസംഖ്യ ignore_show_notes = 0;
		ഘടന string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	തിരഞ്ഞെടുപ്പ് (stdin_mode) {
	അവസ്ഥ TEXT:
		ആകുമ്പോൾ (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_വരി(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		നിർത്തുക;
	അവസ്ഥ REVS:
		ആകുമ്പോൾ (getline_fn(&scratch_buf, stdin) != EOF) {
			ഘടന object_id oid;
			ഘടന object *object;
			ഘടന object *peeled;

			എങ്കിൽ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				തുടരുക;
			}

			object = parse_object(the_repository, &oid);
			എങ്കിൽ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				തുടരുക;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			എങ്കിൽ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				തുടരുക;
			}

			get_format_rev((ഘടന commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	തിരികെ 0;
}
