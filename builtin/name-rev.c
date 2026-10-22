#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "hash-lookup.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "mem-pool.h"
#अन्तर्भाव "pretty.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "notes.h"
#अन्तर्भाव "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

संरचना rev_name {
	अचर वर्ण *tip_name;
	timestamp_t taggerdate;
	पूर्णाङ्क generation;
	पूर्णाङ्क distance;
	पूर्णाङ्क from_tag;
};

define_commit_slab(commit_rev_name, संरचना rev_name);

स्थिर timestamp_t generation_cutoff = GENERATION_सङ्ख्या_INFINITY;
स्थिर timestamp_t cutoff = TIME_MAX;
स्थिर संरचना commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
स्थिर शून्य disable_cutoff(शून्य)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
स्थिर शून्य set_commit_cutoff(संरचना commit *commit)
{

	यदि (cutoff > commit->date)
		cutoff = commit->date;

	यदि (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		यदि (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
स्थिर शून्य adjust_cutoff_timestamp_for_slop(शून्य)
{
	यदि (cutoff) {
		/* check for underflow */
		यदि (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		अन्यथा
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
स्थिर पूर्णाङ्क commit_is_before_cutoff(संरचना commit *commit)
{
	यदि (generation_cutoff < GENERATION_सङ्ख्या_INFINITY)
		फर्काऊ generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	फर्काऊ commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

स्थिर पूर्णाङ्क is_valid_rev_name(अचर संरचना rev_name *name)
{
	फर्काऊ name && name->tip_name;
}

स्थिर संरचना rev_name *get_commit_rev_name(अचर संरचना commit *commit)
{
	संरचना rev_name *name = commit_rev_name_peek(&rev_names, commit);

	फर्काऊ is_valid_rev_name(name) ? name : रिक्त;
}

स्थिर पूर्णाङ्क effective_distance(पूर्णाङ्क distance, पूर्णाङ्क generation)
{
	फर्काऊ distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

स्थिर पूर्णाङ्क is_better_name(संरचना rev_name *name,
			  timestamp_t taggerdate,
			  पूर्णाङ्क generation,
			  पूर्णाङ्क distance,
			  पूर्णाङ्क from_tag)
{
	पूर्णाङ्क name_distance = effective_distance(name->distance, name->generation);
	पूर्णाङ्क new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	यदि (from_tag && name->from_tag)
		फर्काऊ name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	यदि (name->from_tag != from_tag)
		फर्काऊ from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	यदि (name_distance != new_distance)
		फर्काऊ name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	यदि (name->taggerdate != taggerdate)
		फर्काऊ name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	फर्काऊ 0;
}

स्थिर संरचना rev_name *create_or_update_name(संरचना commit *commit,
					      timestamp_t taggerdate,
					      पूर्णाङ्क generation, पूर्णाङ्क distance,
					      पूर्णाङ्क from_tag)
{
	संरचना rev_name *name = commit_rev_name_at(&rev_names, commit);

	यदि (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		फर्काऊ रिक्त;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	फर्काऊ name;
}

स्थिर वर्ण *get_parent_name(अचर संरचना rev_name *name, पूर्णाङ्क parent_सङ्ख्या,
			     संरचना mem_pool *string_pool)
{
	माप_प्रकार len;

	strip_suffix(name->tip_name, "^0", &len);
	यदि (name->generation > 0) {
		फर्काऊ mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (पूर्णाङ्क)len, name->tip_name,
				       name->generation, parent_सङ्ख्या);
	} अन्यथा {
		फर्काऊ mem_pool_strfmt(string_pool, "%.*s^%d",
				       (पूर्णाङ्क)len, name->tip_name, parent_सङ्ख्या);
	}
}

स्थिर शून्य name_rev(संरचना commit *start_commit,
		अचर वर्ण *tip_name, timestamp_t taggerdate,
		पूर्णाङ्क from_tag, पूर्णाङ्क deref, संरचना mem_pool *string_pool)
{
	संरचना commit_थाक थाक = COMMIT_थाक_INIT;
	संरचना commit *commit;
	संरचना commit_थाक parents_to_queue = COMMIT_थाक_INIT;
	संरचना rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	यदि (commit_is_before_cutoff(start_commit))
		फर्काऊ;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	यदि (!start_name)
		फर्काऊ;
	यदि (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	अन्यथा
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_थाक_थप(&थाक, start_commit);

	जबसम्म ((commit = commit_थाक_झिक(&थाक))) {
		संरचना rev_name *name = get_commit_rev_name(commit);
		संरचना commit_list *parents;
		पूर्णाङ्क parent_सङ्ख्या = 1;

		parents_to_queue.nr = 0;

		लागि (parents = commit->parents;
				parents;
				parents = parents->next, parent_सङ्ख्या++) {
			संरचना commit *parent = parents->item;
			संरचना rev_name *parent_name;
			पूर्णाङ्क generation, distance;

			repo_parse_commit(the_repository, parent);
			यदि (commit_is_before_cutoff(parent))
				जारी;

			यदि (parent_सङ्ख्या > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} अन्यथा {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			यदि (parent_name) {
				यदि (parent_सङ्ख्या > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_सङ्ख्या,
								string_pool);
				अन्यथा
					parent_name->tip_name = name->tip_name;
				commit_थाक_थप(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		जबसम्म (parents_to_queue.nr)
			commit_थाक_थप(&थाक,
					  commit_थाक_झिक(&parents_to_queue));
	}

	commit_थाक_clear(&थाक);
	commit_थाक_clear(&parents_to_queue);
}

स्थिर पूर्णाङ्क subpath_matches(अचर वर्ण *path, अचर वर्ण *filter)
{
	अचर वर्ण *subpath = path;

	जबसम्म (subpath) {
		यदि (!wildmatch(filter, subpath, 0))
			फर्काऊ subpath - path;
		subpath = म_अक्षर(subpath, '/');
		यदि (subpath)
			subpath++;
	}
	फर्काऊ -1;
}

संरचना name_ref_data {
	पूर्णाङ्क tags_only;
	पूर्णाङ्क name_only;
	संरचना string_list ref_filters;
	संरचना string_list exclude_filters;
};

संरचना pretty_format {
	संरचना pretty_print_context ctx;
	संरचना userformat_want want;
};

गणना command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

गणना stdin_mode {
    TEXT = 1,
    REVS = 2,
};

संरचना command {
	गणना command_type type;
	सङ्घ {
		पूर्णाङ्क name_only;
		संरचना pretty_format *pretty_format;
	} u;
};

स्थिर शून्य init_name_rev_command(संरचना command *cmd,
				  पूर्णाङ्क name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

स्थिर शून्य init_format_rev_command(संरचना command *cmd,
				    संरचना pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

स्थिर संरचना tip_table {
	संरचना tip_table_entry {
		संरचना object_id oid;
		अचर वर्ण *refname;
		संरचना commit *commit;
		timestamp_t taggerdate;
		अचिन्हित पूर्णाङ्क from_tag:1;
		अचिन्हित पूर्णाङ्क deref:1;
	} *table;
	पूर्णाङ्क nr;
	पूर्णाङ्क alloc;
	पूर्णाङ्क sorted;
} tip_table;

स्थिर शून्य add_to_tip_table(अचर संरचना object_id *oid, अचर वर्ण *refname,
			     पूर्णाङ्क shorten_unambiguous, संरचना commit *commit,
			     timestamp_t taggerdate, पूर्णाङ्क from_tag, पूर्णाङ्क deref)
{
	वर्ण *short_refname = रिक्त;

	यदि (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	अन्यथा यदि (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	अन्यथा
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

स्थिर पूर्णाङ्क tipcmp(अचर शून्य *a_, अचर शून्य *b_)
{
	अचर संरचना tip_table_entry *a = a_, *b = b_;
	फर्काऊ oidcmp(&a->oid, &b->oid);
}

स्थिर पूर्णाङ्क cmp_by_tag_and_age(अचर शून्य *a_, अचर शून्य *b_)
{
	अचर संरचना tip_table_entry *a = a_, *b = b_;
	पूर्णाङ्क cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	यदि (cmp)
		फर्काऊ cmp;

	/* Older is better. */
	यदि (a->taggerdate < b->taggerdate)
		फर्काऊ -1;
	फर्काऊ a->taggerdate != b->taggerdate;
}

स्थिर पूर्णाङ्क name_ref(अचर संरचना reference *ref, शून्य *cb_data)
{
	संरचना object *o = parse_object(the_repository, ref->oid);
	संरचना name_ref_data *data = cb_data;
	पूर्णाङ्क can_abbreviate_output = data->tags_only && data->name_only;
	पूर्णाङ्क deref = 0;
	पूर्णाङ्क from_tag = 0;
	संरचना commit *commit = रिक्त;
	timestamp_t taggerdate = TIME_MAX;

	यदि (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		फर्काऊ 0;

	यदि (data->exclude_filters.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			यदि (subpath_matches(ref->name, item->string) >= 0)
				फर्काऊ 0;
		}
	}

	यदि (data->ref_filters.nr) {
		संरचना string_list_item *item;
		पूर्णाङ्क matched = 0;

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
			विकल्प (subpath_matches(ref->name, item->string)) {
			पक्ष -1: /* did not match */
				रोक;
			पक्ष 0: /* matched fully */
				matched = 1;
				रोक;
			स्वभाव: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				रोक;
			}
		}

		/* If none of the patterns matched, stop now */
		यदि (!matched)
			फर्काऊ 0;
	}

	जबसम्म (o && o->type == OBJ_TAG) {
		संरचना tag *t = (संरचना tag *) o;
		यदि (!t->tagged)
			रोक; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	यदि (o && o->type == OBJ_COMMIT) {
		commit = (संरचना commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		यदि (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	फर्काऊ 0;
}

स्थिर शून्य name_tips(संरचना mem_pool *string_pool)
{
	पूर्णाङ्क i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	लागि (i = 0; i < tip_table.nr; i++) {
		संरचना tip_table_entry *e = &tip_table.table[i];
		यदि (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

स्थिर अचर संरचना object_id *nth_tip_table_ent(माप_प्रकार ix, अचर शून्य *table_)
{
	अचर संरचना tip_table_entry *table = table_;
	फर्काऊ &table[ix].oid;
}

स्थिर अचर वर्ण *get_exact_ref_match(अचर संरचना object *o)
{
	पूर्णाङ्क found;

	यदि (!tip_table.table || !tip_table.nr)
		फर्काऊ रिक्त;

	यदि (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	यदि (0 <= found)
		फर्काऊ tip_table.table[found].refname;
	फर्काऊ रिक्त;
}

/* may return a constant string or use "buf" as scratch space */
स्थिर अचर वर्ण *get_rev_name(अचर संरचना object *o, संरचना strbuf *buf)
{
	संरचना rev_name *n;
	अचर संरचना commit *c;

	यदि (o->type != OBJ_COMMIT)
		फर्काऊ get_exact_ref_match(o);
	c = (अचर संरचना commit *) o;
	n = get_commit_rev_name(c);
	यदि (!n)
		फर्काऊ रिक्त;

	यदि (!n->generation) {
		फर्काऊ n->tip_name;
	} अन्यथा {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		फर्काऊ buf->buf;
	}
}

स्थिर अचर वर्ण *get_format_rev(अचर संरचना commit *c,
				  संरचना pretty_format *format_ctx,
				  संरचना strbuf *buf)
{
	strbuf_reset(buf);

	यदि (format_ctx->want.notes) {
		संरचना strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, रिक्त);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	फर्काऊ buf->buf;
}

स्थिर शून्य show_name(अचर संरचना object *obj,
		      अचर वर्ण *caller_name,
		      पूर्णाङ्क always, पूर्णाङ्क allow_undefined, पूर्णाङ्क name_only)
{
	अचर वर्ण *name;
	अचर संरचना object_id *oid = &obj->oid;
	संरचना strbuf buf = STRBUF_INIT;

	यदि (!name_only)
		म_लिखो("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	यदि (name)
		म_लिखो("%s\n", name);
	अन्यथा यदि (allow_undefined)
		म_लिखो("undefined\n");
	अन्यथा यदि (always)
		म_लिखो("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	अन्यथा
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

स्थिर वर्ण अचर * अचर name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	रिक्त
};

स्थिर शून्य name_rev_पङ्क्ति(वर्ण *p, संरचना command *cmd)
{
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क counter = 0;
	वर्ण *p_start;
	अचर अचिन्हित hexsz = the_hash_algo->hexsz;

	लागि (p_start = p; *p; p++) {
#define ishex(x) (है_अंक((x)) || ((x) >= 'a' && (x) <= 'f'))
		यदि (!ishex(*p)) {
			counter = 0;
		} अन्यथा यदि (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			संरचना object_id oid;
			अचर वर्ण *name = रिक्त;
			वर्ण c = *(p + 1);
			पूर्णाङ्क p_len = p - p_start + 1;
			संरचना object *o = रिक्त;
			पूर्णाङ्क oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			विकल्प (cmd->type) {
			पक्ष NAME_REV:
				यदि (!oid_ret)
					o = lookup_object(the_repository, &oid);
				यदि (o)
					name = get_rev_name(o, &buf);
				यदि (!name)
					जारी;
				यदि (cmd->u.name_only)
					म_लिखो("%.*s%s", p_len - hexsz, p_start, name);
				अन्यथा
					म_लिखो("%.*s (%s)", p_len, p_start, name);
				रोक;
			पक्ष FORMAT_REV:
				यदि (!oid_ret)
					o = parse_object(the_repository, &oid);
				यदि (o && o->type == OBJ_COMMIT)
					name = get_format_rev((अचर संरचना commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				यदि (name)
					म_लिखो("%.*s%s", p_len - hexsz, p_start, name);
				अन्यथा
					म_लिखो("%.*s", p_len, p_start);
				रोक;
			स्वभाव:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	यदि (p_start != p)
		ख_डालो(p_start, p - p_start, 1, मानक_निकास);

	strbuf_release(&buf);
}

पूर्णाङ्क cmd_name_rev(पूर्णाङ्क argc,
		 अचर वर्ण **argv,
		 अचर वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना mem_pool string_pool;
	संरचना object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	पूर्णाङ्क transform_stdin = 0;
#endif
	पूर्णाङ्क all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	संरचना name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	संरचना command cmd;
	संरचना option opts[] = {
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
	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

#ifndef WITH_BREAKING_CHANGES
	यदि (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	यदि (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	यदि (all || annotate_stdin)
		disable_cutoff();

	लागि (; argc; argc--, argv++) {
		संरचना object_id oid;
		संरचना object *object;
		संरचना commit *commit;

		यदि (repo_get_oid(the_repository, *argv, &oid)) {
			ख_लिखो(मानक_त्रुटि, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			जारी;
		}

		commit = रिक्त;
		object = parse_object(the_repository, &oid);
		यदि (object) {
			संरचना object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			यदि (peeled && peeled->type == OBJ_COMMIT)
				commit = (संरचना commit *)peeled;
		}

		यदि (!object) {
			ख_लिखो(मानक_त्रुटि, "Could not get object for %s. Skipping.\n",
					*argv);
			जारी;
		}

		यदि (commit)
			set_commit_cutoff(commit);

		यदि (peel_tag) {
			यदि (!commit) {
				ख_लिखो(मानक_त्रुटि, "Could not get commit for %s. Skipping.\n",
					*argv);
				जारी;
			}
			object = (संरचना object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	यदि (annotate_stdin) {
		संरचना strbuf sb = STRBUF_INIT;

		जबसम्म (strbuf_getline(&sb, मानक_निवेश) != खातापूर्ण) {
			strbuf_addch(&sb, '\n');
			name_rev_पङ्क्ति(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} अन्यथा यदि (all) {
		पूर्णाङ्क i, max;

		max = get_max_object_index(the_repository);
		लागि (i = 0; i < max; i++) {
			संरचना object *obj = get_indexed_object(the_repository, i);
			यदि (!obj || obj->type != OBJ_COMMIT)
				जारी;
			show_name(obj, रिक्त,
				  always, allow_undefined, data.name_only);
		}
	} अन्यथा {
		पूर्णाङ्क i;
		लागि (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	फर्काऊ 0;
}

संरचना format_nul_data {
	bool nul_आगत;
	bool nul_output;
};

स्थिर पूर्णाङ्क format_nul_cb(अचर संरचना option *option,
			 अचर वर्ण *arg,
			 पूर्णाङ्क unset)
{
	संरचना format_nul_data *data = option->मान;
	data->nul_आगत = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	फर्काऊ 0;
}

स्थिर गणना stdin_mode parse_stdin_mode(अचर वर्ण *stdin_mode)
{
	यदि (!म_भेद(stdin_mode, "text"))
		फर्काऊ TEXT;
	अन्यथा यदि (!म_भेद(stdin_mode, "revs") ||
		 !म_भेद(stdin_mode, "rev"))
		फर्काऊ REVS;
	अन्यथा
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

स्थिर वर्ण अचर *अचर format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	रिक्त
};

पूर्णाङ्क cmd_format_rev(पूर्णाङ्क argc,
		   अचर वर्ण **argv,
		   अचर वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	अचर वर्ण *format = रिक्त;
	गणना stdin_mode stdin_mode;
	अचर वर्ण *stdin_mode_arg = रिक्त;
	संरचना format_nul_data nul_data = { 0, 0 };
	वर्ण output_terminator;
	strbuf_getline_fn getline_fn;
	संरचना display_notes_opt format_notes_opt;
	संरचना rev_info format_rev = REV_INFO_INIT;
	संरचना pretty_format format_pp = { 0 };
	संरचना string_list notes = STRING_LIST_INIT_NODUP;
	संरचना strbuf scratch_buf = STRBUF_INIT;
	संरचना command cmd;
	संरचना option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_आगत,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	यदि (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	यदि (!format)
		die(_("'%s' is required"), "--format");
	यदि (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_आगत ? strbuf_getline_nul : strbuf_getline_lf;
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
	यदि (format_pp.want.notes) {
		पूर्णाङ्क ignore_show_notes = 0;
		संरचना string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	विकल्प (stdin_mode) {
	पक्ष TEXT:
		जबसम्म (getline_fn(&scratch_buf, मानक_निवेश) != खातापूर्ण) {
			name_rev_पङ्क्ति(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			म_लिखो("%c", output_terminator);
			maybe_flush_or_die(मानक_निकास, "stdout");
		}
		रोक;
	पक्ष REVS:
		जबसम्म (getline_fn(&scratch_buf, मानक_निवेश) != खातापूर्ण) {
			संरचना object_id oid;
			संरचना object *object;
			संरचना object *peeled;

			यदि (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				ख_लिखो(मानक_त्रुटि, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				जारी;
			}

			object = parse_object(the_repository, &oid);
			यदि (!object) {
				ख_लिखो(मानक_त्रुटि, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				जारी;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			यदि (!peeled || peeled->type != OBJ_COMMIT) {
				ख_लिखो(मानक_त्रुटि,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				जारी;
			}

			get_format_rev((संरचना commit *)peeled,
				       &format_pp, &scratch_buf);
			म_लिखो("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(मानक_निकास, "stdout");
			strbuf_release(&scratch_buf);
		}
		रोक;
	स्वभाव:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	फर्काऊ 0;
}
