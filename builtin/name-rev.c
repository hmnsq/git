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
	ध्रुव वर्ण *tip_name;
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

	जइ (cutoff > commit->date)
		cutoff = commit->date;

	जइ (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		जइ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
स्थिर शून्य adjust_cutoff_timestamp_for_slop(शून्य)
{
	जइ (cutoff) {
		/* check for underflow */
		जइ (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		अण्णहा
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
स्थिर पूर्णाङ्क commit_is_before_cutoff(संरचना commit *commit)
{
	जइ (generation_cutoff < GENERATION_सङ्ख्या_INFINITY)
		प्रत्यागम generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	प्रत्यागम commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

स्थिर पूर्णाङ्क is_valid_rev_name(ध्रुव संरचना rev_name *name)
{
	प्रत्यागम name && name->tip_name;
}

स्थिर संरचना rev_name *get_commit_rev_name(ध्रुव संरचना commit *commit)
{
	संरचना rev_name *name = commit_rev_name_peek(&rev_names, commit);

	प्रत्यागम is_valid_rev_name(name) ? name : रिक्त;
}

स्थिर पूर्णाङ्क effective_distance(पूर्णाङ्क distance, पूर्णाङ्क generation)
{
	प्रत्यागम distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
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
	जइ (from_tag && name->from_tag)
		प्रत्यागम name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	जइ (name->from_tag != from_tag)
		प्रत्यागम from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	जइ (name_distance != new_distance)
		प्रत्यागम name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	जइ (name->taggerdate != taggerdate)
		प्रत्यागम name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	प्रत्यागम 0;
}

स्थिर संरचना rev_name *create_or_update_name(संरचना commit *commit,
					      timestamp_t taggerdate,
					      पूर्णाङ्क generation, पूर्णाङ्क distance,
					      पूर्णाङ्क from_tag)
{
	संरचना rev_name *name = commit_rev_name_at(&rev_names, commit);

	जइ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		प्रत्यागम रिक्त;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	प्रत्यागम name;
}

स्थिर वर्ण *get_parent_name(ध्रुव संरचना rev_name *name, पूर्णाङ्क parent_सङ्ख्या,
			     संरचना mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	जइ (name->generation > 0) {
		प्रत्यागम mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (पूर्णाङ्क)len, name->tip_name,
				       name->generation, parent_सङ्ख्या);
	} अण्णहा {
		प्रत्यागम mem_pool_strfmt(string_pool, "%.*s^%d",
				       (पूर्णाङ्क)len, name->tip_name, parent_सङ्ख्या);
	}
}

स्थिर शून्य name_rev(संरचना commit *start_commit,
		ध्रुव वर्ण *tip_name, timestamp_t taggerdate,
		पूर्णाङ्क from_tag, पूर्णाङ्क deref, संरचना mem_pool *string_pool)
{
	संरचना commit_राशिः राशिः = COMMIT_राशिः_INIT;
	संरचना commit *commit;
	संरचना commit_राशिः parents_to_queue = COMMIT_राशिः_INIT;
	संरचना rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	जइ (commit_is_before_cutoff(start_commit))
		प्रत्यागम;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	जइ (!start_name)
		प्रत्यागम;
	जइ (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	अण्णहा
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_राशिः_प्रक्षिप(&राशिः, start_commit);

	जाव ((commit = commit_राशिः_निष्कासय(&राशिः))) {
		संरचना rev_name *name = get_commit_rev_name(commit);
		संरचना commit_list *parents;
		पूर्णाङ्क parent_सङ्ख्या = 1;

		parents_to_queue.nr = 0;

		प्रति (parents = commit->parents;
				parents;
				parents = parents->next, parent_सङ्ख्या++) {
			संरचना commit *parent = parents->item;
			संरचना rev_name *parent_name;
			पूर्णाङ्क generation, distance;

			repo_parse_commit(the_repository, parent);
			जइ (commit_is_before_cutoff(parent))
				अनुवर्त;

			जइ (parent_सङ्ख्या > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} अण्णहा {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			जइ (parent_name) {
				जइ (parent_सङ्ख्या > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_सङ्ख्या,
								string_pool);
				अण्णहा
					parent_name->tip_name = name->tip_name;
				commit_राशिः_प्रक्षिप(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		जाव (parents_to_queue.nr)
			commit_राशिः_प्रक्षिप(&राशिः,
					  commit_राशिः_निष्कासय(&parents_to_queue));
	}

	commit_राशिः_clear(&राशिः);
	commit_राशिः_clear(&parents_to_queue);
}

स्थिर पूर्णाङ्क subpath_matches(ध्रुव वर्ण *path, ध्रुव वर्ण *filter)
{
	ध्रुव वर्ण *subpath = path;

	जाव (subpath) {
		जइ (!wildmatch(filter, subpath, 0))
			प्रत्यागम subpath - path;
		subpath = strchr(subpath, '/');
		जइ (subpath)
			subpath++;
	}
	प्रत्यागम -1;
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
		ध्रुव वर्ण *refname;
		संरचना commit *commit;
		timestamp_t taggerdate;
		unsigned पूर्णाङ्क from_tag:1;
		unsigned पूर्णाङ्क deref:1;
	} *table;
	पूर्णाङ्क nr;
	पूर्णाङ्क alloc;
	पूर्णाङ्क sorted;
} tip_table;

स्थिर शून्य add_to_tip_table(ध्रुव संरचना object_id *oid, ध्रुव वर्ण *refname,
			     पूर्णाङ्क shorten_unambiguous, संरचना commit *commit,
			     timestamp_t taggerdate, पूर्णाङ्क from_tag, पूर्णाङ्क deref)
{
	वर्ण *short_refname = रिक्त;

	जइ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	अण्णहा जइ (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	अण्णहा
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

स्थिर पूर्णाङ्क tipcmp(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना tip_table_entry *a = a_, *b = b_;
	प्रत्यागम oidcmp(&a->oid, &b->oid);
}

स्थिर पूर्णाङ्क cmp_by_tag_and_age(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना tip_table_entry *a = a_, *b = b_;
	पूर्णाङ्क cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	जइ (cmp)
		प्रत्यागम cmp;

	/* Older is better. */
	जइ (a->taggerdate < b->taggerdate)
		प्रत्यागम -1;
	प्रत्यागम a->taggerdate != b->taggerdate;
}

स्थिर पूर्णाङ्क name_ref(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	संरचना object *o = parse_object(the_repository, ref->oid);
	संरचना name_ref_data *data = cb_data;
	पूर्णाङ्क can_abbreviate_output = data->tags_only && data->name_only;
	पूर्णाङ्क deref = 0;
	पूर्णाङ्क from_tag = 0;
	संरचना commit *commit = रिक्त;
	timestamp_t taggerdate = TIME_MAX;

	जइ (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		प्रत्यागम 0;

	जइ (data->exclude_filters.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			जइ (subpath_matches(ref->name, item->string) >= 0)
				प्रत्यागम 0;
		}
	}

	जइ (data->ref_filters.nr) {
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
				भङ्ग;
			पक्ष 0: /* matched fully */
				matched = 1;
				भङ्ग;
			स्वभाव: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				भङ्ग;
			}
		}

		/* If none of the patterns matched, stop now */
		जइ (!matched)
			प्रत्यागम 0;
	}

	जाव (o && o->type == OBJ_TAG) {
		संरचना tag *t = (संरचना tag *) o;
		जइ (!t->tagged)
			भङ्ग; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	जइ (o && o->type == OBJ_COMMIT) {
		commit = (संरचना commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		जइ (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	प्रत्यागम 0;
}

स्थिर शून्य name_tips(संरचना mem_pool *string_pool)
{
	पूर्णाङ्क i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	प्रति (i = 0; i < tip_table.nr; i++) {
		संरचना tip_table_entry *e = &tip_table.table[i];
		जइ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

स्थिर ध्रुव संरचना object_id *nth_tip_table_ent(size_t ix, ध्रुव शून्य *table_)
{
	ध्रुव संरचना tip_table_entry *table = table_;
	प्रत्यागम &table[ix].oid;
}

स्थिर ध्रुव वर्ण *get_exact_ref_match(ध्रुव संरचना object *o)
{
	पूर्णाङ्क found;

	जइ (!tip_table.table || !tip_table.nr)
		प्रत्यागम रिक्त;

	जइ (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	जइ (0 <= found)
		प्रत्यागम tip_table.table[found].refname;
	प्रत्यागम रिक्त;
}

/* may return a constant string or use "buf" as scratch space */
स्थिर ध्रुव वर्ण *get_rev_name(ध्रुव संरचना object *o, संरचना strbuf *buf)
{
	संरचना rev_name *n;
	ध्रुव संरचना commit *c;

	जइ (o->type != OBJ_COMMIT)
		प्रत्यागम get_exact_ref_match(o);
	c = (ध्रुव संरचना commit *) o;
	n = get_commit_rev_name(c);
	जइ (!n)
		प्रत्यागम रिक्त;

	जइ (!n->generation) {
		प्रत्यागम n->tip_name;
	} अण्णहा {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		प्रत्यागम buf->buf;
	}
}

स्थिर ध्रुव वर्ण *get_format_rev(ध्रुव संरचना commit *c,
				  संरचना pretty_format *format_ctx,
				  संरचना strbuf *buf)
{
	strbuf_reset(buf);

	जइ (format_ctx->want.notes) {
		संरचना strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, रिक्त);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	प्रत्यागम buf->buf;
}

स्थिर शून्य show_name(ध्रुव संरचना object *obj,
		      ध्रुव वर्ण *caller_name,
		      पूर्णाङ्क always, पूर्णाङ्क allow_undefined, पूर्णाङ्क name_only)
{
	ध्रुव वर्ण *name;
	ध्रुव संरचना object_id *oid = &obj->oid;
	संरचना strbuf buf = STRBUF_INIT;

	जइ (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	जइ (name)
		printf("%s\n", name);
	अण्णहा जइ (allow_undefined)
		printf("undefined\n");
	अण्णहा जइ (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	अण्णहा
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

स्थिर वर्ण ध्रुव * ध्रुव name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	रिक्त
};

स्थिर शून्य name_rev_पङ्क्तिः(वर्ण *p, संरचना command *cmd)
{
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क counter = 0;
	वर्ण *p_start;
	ध्रुव unsigned hexsz = the_hash_algo->hexsz;

	प्रति (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		जइ (!ishex(*p)) {
			counter = 0;
		} अण्णहा जइ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			संरचना object_id oid;
			ध्रुव वर्ण *name = रिक्त;
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
				जइ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				जइ (o)
					name = get_rev_name(o, &buf);
				जइ (!name)
					अनुवर्त;
				जइ (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				अण्णहा
					printf("%.*s (%s)", p_len, p_start, name);
				भङ्ग;
			पक्ष FORMAT_REV:
				जइ (!oid_ret)
					o = parse_object(the_repository, &oid);
				जइ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ध्रुव संरचना commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				जइ (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				अण्णहा
					printf("%.*s", p_len, p_start);
				भङ्ग;
			स्वभाव:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	जइ (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

पूर्णाङ्क cmd_name_rev(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
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
	जइ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	जइ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	जइ (all || annotate_stdin)
		disable_cutoff();

	प्रति (; argc; argc--, argv++) {
		संरचना object_id oid;
		संरचना object *object;
		संरचना commit *commit;

		जइ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			अनुवर्त;
		}

		commit = रिक्त;
		object = parse_object(the_repository, &oid);
		जइ (object) {
			संरचना object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			जइ (peeled && peeled->type == OBJ_COMMIT)
				commit = (संरचना commit *)peeled;
		}

		जइ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			अनुवर्त;
		}

		जइ (commit)
			set_commit_cutoff(commit);

		जइ (peel_tag) {
			जइ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				अनुवर्त;
			}
			object = (संरचना object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	जइ (annotate_stdin) {
		संरचना strbuf sb = STRBUF_INIT;

		जाव (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_पङ्क्तिः(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} अण्णहा जइ (all) {
		पूर्णाङ्क i, max;

		max = get_max_object_index(the_repository);
		प्रति (i = 0; i < max; i++) {
			संरचना object *obj = get_indexed_object(the_repository, i);
			जइ (!obj || obj->type != OBJ_COMMIT)
				अनुवर्त;
			show_name(obj, रिक्त,
				  always, allow_undefined, data.name_only);
		}
	} अण्णहा {
		पूर्णाङ्क i;
		प्रति (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	प्रत्यागम 0;
}

संरचना format_nul_data {
	bool nul_निवेशः;
	bool nul_output;
};

स्थिर पूर्णाङ्क format_nul_cb(ध्रुव संरचना option *option,
			 ध्रुव वर्ण *arg,
			 पूर्णाङ्क unset)
{
	संरचना format_nul_data *data = option->मानम्;
	data->nul_निवेशः = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	प्रत्यागम 0;
}

स्थिर गणना stdin_mode parse_stdin_mode(ध्रुव वर्ण *stdin_mode)
{
	जइ (!strcmp(stdin_mode, "text"))
		प्रत्यागम TEXT;
	अण्णहा जइ (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		प्रत्यागम REVS;
	अण्णहा
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

स्थिर वर्ण ध्रुव *ध्रुव format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	रिक्त
};

पूर्णाङ्क cmd_format_rev(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	ध्रुव वर्ण *format = रिक्त;
	गणना stdin_mode stdin_mode;
	ध्रुव वर्ण *stdin_mode_arg = रिक्त;
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
		OPT_BOOL(0, "null-input", &nul_data.nul_निवेशः,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	जइ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	जइ (!format)
		die(_("'%s' is required"), "--format");
	जइ (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_निवेशः ? strbuf_getline_nul : strbuf_getline_lf;
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
	जइ (format_pp.want.notes) {
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
		जाव (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_पङ्क्तिः(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		भङ्ग;
	पक्ष REVS:
		जाव (getline_fn(&scratch_buf, stdin) != EOF) {
			संरचना object_id oid;
			संरचना object *object;
			संरचना object *peeled;

			जइ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				अनुवर्त;
			}

			object = parse_object(the_repository, &oid);
			जइ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				अनुवर्त;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			जइ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				अनुवर्त;
			}

			get_format_rev((संरचना commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		भङ्ग;
	स्वभाव:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	प्रत्यागम 0;
}
