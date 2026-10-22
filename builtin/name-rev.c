#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "hash-lookup.h"
#அந்தர்பாவ "commit-slab.h"
#அந்தர்பாவ "commit-graph.h"
#அந்தர்பாவ "wildmatch.h"
#அந்தர்பாவ "mem-pool.h"
#அந்தர்பாவ "pretty.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "notes.h"
#அந்தர்பாவ "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

கட்டமைப்பு rev_name {
	மாறிலி எழுத்து *tip_name;
	timestamp_t taggerdate;
	முழு generation;
	முழு distance;
	முழு from_tag;
};

define_commit_slab(commit_rev_name, கட்டமைப்பு rev_name);

நிலையான timestamp_t generation_cutoff = GENERATION_எண்_INFINITY;
நிலையான timestamp_t cutoff = TIME_MAX;
நிலையான கட்டமைப்பு commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
நிலையான வெறுமை disable_cutoff(வெறுமை)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
நிலையான வெறுமை set_commit_cutoff(கட்டமைப்பு commit *commit)
{

	எனில் (cutoff > commit->date)
		cutoff = commit->date;

	எனில் (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		எனில் (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
நிலையான வெறுமை adjust_cutoff_timestamp_for_slop(வெறுமை)
{
	எனில் (cutoff) {
		/* check for underflow */
		எனில் (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		இல்லையெனில்
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
நிலையான முழு commit_is_before_cutoff(கட்டமைப்பு commit *commit)
{
	எனில் (generation_cutoff < GENERATION_எண்_INFINITY)
		திரும்பு generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	திரும்பு commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

நிலையான முழு is_valid_rev_name(மாறிலி கட்டமைப்பு rev_name *name)
{
	திரும்பு name && name->tip_name;
}

நிலையான கட்டமைப்பு rev_name *get_commit_rev_name(மாறிலி கட்டமைப்பு commit *commit)
{
	கட்டமைப்பு rev_name *name = commit_rev_name_peek(&rev_names, commit);

	திரும்பு is_valid_rev_name(name) ? name : வெற்று;
}

நிலையான முழு effective_distance(முழு distance, முழு generation)
{
	திரும்பு distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

நிலையான முழு is_better_name(கட்டமைப்பு rev_name *name,
			  timestamp_t taggerdate,
			  முழு generation,
			  முழு distance,
			  முழு from_tag)
{
	முழு name_distance = effective_distance(name->distance, name->generation);
	முழு new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	எனில் (from_tag && name->from_tag)
		திரும்பு name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	எனில் (name->from_tag != from_tag)
		திரும்பு from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	எனில் (name_distance != new_distance)
		திரும்பு name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	எனில் (name->taggerdate != taggerdate)
		திரும்பு name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	திரும்பு 0;
}

நிலையான கட்டமைப்பு rev_name *create_or_update_name(கட்டமைப்பு commit *commit,
					      timestamp_t taggerdate,
					      முழு generation, முழு distance,
					      முழு from_tag)
{
	கட்டமைப்பு rev_name *name = commit_rev_name_at(&rev_names, commit);

	எனில் (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		திரும்பு வெற்று;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	திரும்பு name;
}

நிலையான எழுத்து *get_parent_name(மாறிலி கட்டமைப்பு rev_name *name, முழு parent_எண்,
			     கட்டமைப்பு mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	எனில் (name->generation > 0) {
		திரும்பு mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (முழு)len, name->tip_name,
				       name->generation, parent_எண்);
	} இல்லையெனில் {
		திரும்பு mem_pool_strfmt(string_pool, "%.*s^%d",
				       (முழு)len, name->tip_name, parent_எண்);
	}
}

நிலையான வெறுமை name_rev(கட்டமைப்பு commit *start_commit,
		மாறிலி எழுத்து *tip_name, timestamp_t taggerdate,
		முழு from_tag, முழு deref, கட்டமைப்பு mem_pool *string_pool)
{
	கட்டமைப்பு commit_அடுக்கு அடுக்கு = COMMIT_அடுக்கு_INIT;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு commit_அடுக்கு parents_to_queue = COMMIT_அடுக்கு_INIT;
	கட்டமைப்பு rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	எனில் (commit_is_before_cutoff(start_commit))
		திரும்பு;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	எனில் (!start_name)
		திரும்பு;
	எனில் (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	இல்லையெனில்
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_அடுக்கு_தள்ளு(&அடுக்கு, start_commit);

	போது ((commit = commit_அடுக்கு_எடு(&அடுக்கு))) {
		கட்டமைப்பு rev_name *name = get_commit_rev_name(commit);
		கட்டமைப்பு commit_list *parents;
		முழு parent_எண் = 1;

		parents_to_queue.nr = 0;

		க்கு (parents = commit->parents;
				parents;
				parents = parents->next, parent_எண்++) {
			கட்டமைப்பு commit *parent = parents->item;
			கட்டமைப்பு rev_name *parent_name;
			முழு generation, distance;

			repo_parse_commit(the_repository, parent);
			எனில் (commit_is_before_cutoff(parent))
				தொடர்;

			எனில் (parent_எண் > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} இல்லையெனில் {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			எனில் (parent_name) {
				எனில் (parent_எண் > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_எண்,
								string_pool);
				இல்லையெனில்
					parent_name->tip_name = name->tip_name;
				commit_அடுக்கு_தள்ளு(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		போது (parents_to_queue.nr)
			commit_அடுக்கு_தள்ளு(&அடுக்கு,
					  commit_அடுக்கு_எடு(&parents_to_queue));
	}

	commit_அடுக்கு_clear(&அடுக்கு);
	commit_அடுக்கு_clear(&parents_to_queue);
}

நிலையான முழு subpath_matches(மாறிலி எழுத்து *path, மாறிலி எழுத்து *filter)
{
	மாறிலி எழுத்து *subpath = path;

	போது (subpath) {
		எனில் (!wildmatch(filter, subpath, 0))
			திரும்பு subpath - path;
		subpath = strchr(subpath, '/');
		எனில் (subpath)
			subpath++;
	}
	திரும்பு -1;
}

கட்டமைப்பு name_ref_data {
	முழு tags_only;
	முழு name_only;
	கட்டமைப்பு string_list ref_filters;
	கட்டமைப்பு string_list exclude_filters;
};

கட்டமைப்பு pretty_format {
	கட்டமைப்பு pretty_print_context ctx;
	கட்டமைப்பு userformat_want want;
};

எண்ணிக்கை command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

எண்ணிக்கை stdin_mode {
    TEXT = 1,
    REVS = 2,
};

கட்டமைப்பு command {
	எண்ணிக்கை command_type type;
	ஒன்றிப்பு {
		முழு name_only;
		கட்டமைப்பு pretty_format *pretty_format;
	} u;
};

நிலையான வெறுமை init_name_rev_command(கட்டமைப்பு command *cmd,
				  முழு name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

நிலையான வெறுமை init_format_rev_command(கட்டமைப்பு command *cmd,
				    கட்டமைப்பு pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

நிலையான கட்டமைப்பு tip_table {
	கட்டமைப்பு tip_table_entry {
		கட்டமைப்பு object_id oid;
		மாறிலி எழுத்து *refname;
		கட்டமைப்பு commit *commit;
		timestamp_t taggerdate;
		unsigned முழு from_tag:1;
		unsigned முழு deref:1;
	} *table;
	முழு nr;
	முழு alloc;
	முழு sorted;
} tip_table;

நிலையான வெறுமை add_to_tip_table(மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *refname,
			     முழு shorten_unambiguous, கட்டமைப்பு commit *commit,
			     timestamp_t taggerdate, முழு from_tag, முழு deref)
{
	எழுத்து *short_refname = வெற்று;

	எனில் (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	இல்லையெனில் எனில் (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	இல்லையெனில்
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

நிலையான முழு tipcmp(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	மாறிலி கட்டமைப்பு tip_table_entry *a = a_, *b = b_;
	திரும்பு oidcmp(&a->oid, &b->oid);
}

நிலையான முழு cmp_by_tag_and_age(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	மாறிலி கட்டமைப்பு tip_table_entry *a = a_, *b = b_;
	முழு cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	எனில் (cmp)
		திரும்பு cmp;

	/* Older is better. */
	எனில் (a->taggerdate < b->taggerdate)
		திரும்பு -1;
	திரும்பு a->taggerdate != b->taggerdate;
}

நிலையான முழு name_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு object *o = parse_object(the_repository, ref->oid);
	கட்டமைப்பு name_ref_data *data = cb_data;
	முழு can_abbreviate_output = data->tags_only && data->name_only;
	முழு deref = 0;
	முழு from_tag = 0;
	கட்டமைப்பு commit *commit = வெற்று;
	timestamp_t taggerdate = TIME_MAX;

	எனில் (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		திரும்பு 0;

	எனில் (data->exclude_filters.nr) {
		கட்டமைப்பு string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			எனில் (subpath_matches(ref->name, item->string) >= 0)
				திரும்பு 0;
		}
	}

	எனில் (data->ref_filters.nr) {
		கட்டமைப்பு string_list_item *item;
		முழு matched = 0;

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
			தேர்வு (subpath_matches(ref->name, item->string)) {
			நிலை -1: /* did not match */
				நிறுத்து;
			நிலை 0: /* matched fully */
				matched = 1;
				நிறுத்து;
			இயல்பு: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				நிறுத்து;
			}
		}

		/* If none of the patterns matched, stop now */
		எனில் (!matched)
			திரும்பு 0;
	}

	போது (o && o->type == OBJ_TAG) {
		கட்டமைப்பு tag *t = (கட்டமைப்பு tag *) o;
		எனில் (!t->tagged)
			நிறுத்து; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	எனில் (o && o->type == OBJ_COMMIT) {
		commit = (கட்டமைப்பு commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		எனில் (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	திரும்பு 0;
}

நிலையான வெறுமை name_tips(கட்டமைப்பு mem_pool *string_pool)
{
	முழு i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	க்கு (i = 0; i < tip_table.nr; i++) {
		கட்டமைப்பு tip_table_entry *e = &tip_table.table[i];
		எனில் (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

நிலையான மாறிலி கட்டமைப்பு object_id *nth_tip_table_ent(size_t ix, மாறிலி வெறுமை *table_)
{
	மாறிலி கட்டமைப்பு tip_table_entry *table = table_;
	திரும்பு &table[ix].oid;
}

நிலையான மாறிலி எழுத்து *get_exact_ref_match(மாறிலி கட்டமைப்பு object *o)
{
	முழு found;

	எனில் (!tip_table.table || !tip_table.nr)
		திரும்பு வெற்று;

	எனில் (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	எனில் (0 <= found)
		திரும்பு tip_table.table[found].refname;
	திரும்பு வெற்று;
}

/* may return a constant string or use "buf" as scratch space */
நிலையான மாறிலி எழுத்து *get_rev_name(மாறிலி கட்டமைப்பு object *o, கட்டமைப்பு strbuf *buf)
{
	கட்டமைப்பு rev_name *n;
	மாறிலி கட்டமைப்பு commit *c;

	எனில் (o->type != OBJ_COMMIT)
		திரும்பு get_exact_ref_match(o);
	c = (மாறிலி கட்டமைப்பு commit *) o;
	n = get_commit_rev_name(c);
	எனில் (!n)
		திரும்பு வெற்று;

	எனில் (!n->generation) {
		திரும்பு n->tip_name;
	} இல்லையெனில் {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		திரும்பு buf->buf;
	}
}

நிலையான மாறிலி எழுத்து *get_format_rev(மாறிலி கட்டமைப்பு commit *c,
				  கட்டமைப்பு pretty_format *format_ctx,
				  கட்டமைப்பு strbuf *buf)
{
	strbuf_reset(buf);

	எனில் (format_ctx->want.notes) {
		கட்டமைப்பு strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, வெற்று);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	திரும்பு buf->buf;
}

நிலையான வெறுமை show_name(மாறிலி கட்டமைப்பு object *obj,
		      மாறிலி எழுத்து *caller_name,
		      முழு always, முழு allow_undefined, முழு name_only)
{
	மாறிலி எழுத்து *name;
	மாறிலி கட்டமைப்பு object_id *oid = &obj->oid;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	எனில் (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	எனில் (name)
		printf("%s\n", name);
	இல்லையெனில் எனில் (allow_undefined)
		printf("undefined\n");
	இல்லையெனில் எனில் (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	இல்லையெனில்
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

நிலையான எழுத்து மாறிலி * மாறிலி name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	வெற்று
};

நிலையான வெறுமை name_rev_வரி(எழுத்து *p, கட்டமைப்பு command *cmd)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	முழு counter = 0;
	எழுத்து *p_start;
	மாறிலி unsigned hexsz = the_hash_algo->hexsz;

	க்கு (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		எனில் (!ishex(*p)) {
			counter = 0;
		} இல்லையெனில் எனில் (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			கட்டமைப்பு object_id oid;
			மாறிலி எழுத்து *name = வெற்று;
			எழுத்து c = *(p + 1);
			முழு p_len = p - p_start + 1;
			கட்டமைப்பு object *o = வெற்று;
			முழு oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			தேர்வு (cmd->type) {
			நிலை NAME_REV:
				எனில் (!oid_ret)
					o = lookup_object(the_repository, &oid);
				எனில் (o)
					name = get_rev_name(o, &buf);
				எனில் (!name)
					தொடர்;
				எனில் (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				இல்லையெனில்
					printf("%.*s (%s)", p_len, p_start, name);
				நிறுத்து;
			நிலை FORMAT_REV:
				எனில் (!oid_ret)
					o = parse_object(the_repository, &oid);
				எனில் (o && o->type == OBJ_COMMIT)
					name = get_format_rev((மாறிலி கட்டமைப்பு commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				எனில் (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				இல்லையெனில்
					printf("%.*s", p_len, p_start);
				நிறுத்து;
			இயல்பு:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	எனில் (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

முழு cmd_name_rev(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு mem_pool string_pool;
	கட்டமைப்பு object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	முழு transform_stdin = 0;
#endif
	முழு all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	கட்டமைப்பு name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	கட்டமைப்பு command cmd;
	கட்டமைப்பு option opts[] = {
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
	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

#ifndef WITH_BREAKING_CHANGES
	எனில் (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	எனில் (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	எனில் (all || annotate_stdin)
		disable_cutoff();

	க்கு (; argc; argc--, argv++) {
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு object *object;
		கட்டமைப்பு commit *commit;

		எனில் (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			தொடர்;
		}

		commit = வெற்று;
		object = parse_object(the_repository, &oid);
		எனில் (object) {
			கட்டமைப்பு object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			எனில் (peeled && peeled->type == OBJ_COMMIT)
				commit = (கட்டமைப்பு commit *)peeled;
		}

		எனில் (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			தொடர்;
		}

		எனில் (commit)
			set_commit_cutoff(commit);

		எனில் (peel_tag) {
			எனில் (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				தொடர்;
			}
			object = (கட்டமைப்பு object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	எனில் (annotate_stdin) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;

		போது (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_வரி(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} இல்லையெனில் எனில் (all) {
		முழு i, max;

		max = get_max_object_index(the_repository);
		க்கு (i = 0; i < max; i++) {
			கட்டமைப்பு object *obj = get_indexed_object(the_repository, i);
			எனில் (!obj || obj->type != OBJ_COMMIT)
				தொடர்;
			show_name(obj, வெற்று,
				  always, allow_undefined, data.name_only);
		}
	} இல்லையெனில் {
		முழு i;
		க்கு (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	திரும்பு 0;
}

கட்டமைப்பு format_nul_data {
	bool nul_உள்ளீடு;
	bool nul_output;
};

நிலையான முழு format_nul_cb(மாறிலி கட்டமைப்பு option *option,
			 மாறிலி எழுத்து *arg,
			 முழு unset)
{
	கட்டமைப்பு format_nul_data *data = option->மதிப்பு;
	data->nul_உள்ளீடு = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	திரும்பு 0;
}

நிலையான எண்ணிக்கை stdin_mode parse_stdin_mode(மாறிலி எழுத்து *stdin_mode)
{
	எனில் (!strcmp(stdin_mode, "text"))
		திரும்பு TEXT;
	இல்லையெனில் எனில் (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		திரும்பு REVS;
	இல்லையெனில்
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

நிலையான எழுத்து மாறிலி *மாறிலி format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	வெற்று
};

முழு cmd_format_rev(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *format = வெற்று;
	எண்ணிக்கை stdin_mode stdin_mode;
	மாறிலி எழுத்து *stdin_mode_arg = வெற்று;
	கட்டமைப்பு format_nul_data nul_data = { 0, 0 };
	எழுத்து output_terminator;
	strbuf_getline_fn getline_fn;
	கட்டமைப்பு display_notes_opt format_notes_opt;
	கட்டமைப்பு rev_info format_rev = REV_INFO_INIT;
	கட்டமைப்பு pretty_format format_pp = { 0 };
	கட்டமைப்பு string_list notes = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு strbuf scratch_buf = STRBUF_INIT;
	கட்டமைப்பு command cmd;
	கட்டமைப்பு option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_உள்ளீடு,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	எனில் (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	எனில் (!format)
		die(_("'%s' is required"), "--format");
	எனில் (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_உள்ளீடு ? strbuf_getline_nul : strbuf_getline_lf;
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
	எனில் (format_pp.want.notes) {
		முழு ignore_show_notes = 0;
		கட்டமைப்பு string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	தேர்வு (stdin_mode) {
	நிலை TEXT:
		போது (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_வரி(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		நிறுத்து;
	நிலை REVS:
		போது (getline_fn(&scratch_buf, stdin) != EOF) {
			கட்டமைப்பு object_id oid;
			கட்டமைப்பு object *object;
			கட்டமைப்பு object *peeled;

			எனில் (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				தொடர்;
			}

			object = parse_object(the_repository, &oid);
			எனில் (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				தொடர்;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			எனில் (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				தொடர்;
			}

			get_format_rev((கட்டமைப்பு commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		நிறுத்து;
	இயல்பு:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	திரும்பு 0;
}
