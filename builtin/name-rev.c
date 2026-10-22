#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "config.h"
#समावेश "commit.h"
#समावेश "tag.h"
#समावेश "refs.h"
#समावेश "object-name.h"
#समावेश "pager.h"
#समावेश "parse-options.h"
#समावेश "hash-lookup.h"
#समावेश "commit-slab.h"
#समावेश "commit-graph.h"
#समावेश "wildmatch.h"
#समावेश "mem-pool.h"
#समावेश "pretty.h"
#समावेश "revision.h"
#समावेश "notes.h"
#समावेश "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#घोषणा CUTOFF_DATE_SLOP 86400

काष्ठा rev_name {
	स्थिर अक्षर *tip_name;
	timestamp_t taggerdate;
	पूर्णांक generation;
	पूर्णांक distance;
	पूर्णांक from_tag;
};

define_commit_slab(commit_rev_name, काष्ठा rev_name);

अटल timestamp_t generation_cutoff = GENERATION_संख्या_INFINITY;
अटल timestamp_t cutoff = TIME_MAX;
अटल काष्ठा commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
अटल व्योम disable_cutoff(व्योम)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
अटल व्योम set_commit_cutoff(काष्ठा commit *commit)
{

	अगर (cutoff > commit->date)
		cutoff = commit->date;

	अगर (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		अगर (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
अटल व्योम adjust_cutoff_timestamp_for_slop(व्योम)
{
	अगर (cutoff) {
		/* check for underflow */
		अगर (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		अन्यथा
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
अटल पूर्णांक commit_is_before_cutoff(काष्ठा commit *commit)
{
	अगर (generation_cutoff < GENERATION_संख्या_INFINITY)
		वापस generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	वापस commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#घोषणा MERGE_TRAVERSAL_WEIGHT 65535

अटल पूर्णांक is_valid_rev_name(स्थिर काष्ठा rev_name *name)
{
	वापस name && name->tip_name;
}

अटल काष्ठा rev_name *get_commit_rev_name(स्थिर काष्ठा commit *commit)
{
	काष्ठा rev_name *name = commit_rev_name_peek(&rev_names, commit);

	वापस is_valid_rev_name(name) ? name : शून्य;
}

अटल पूर्णांक effective_distance(पूर्णांक distance, पूर्णांक generation)
{
	वापस distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

अटल पूर्णांक is_better_name(काष्ठा rev_name *name,
			  timestamp_t taggerdate,
			  पूर्णांक generation,
			  पूर्णांक distance,
			  पूर्णांक from_tag)
{
	पूर्णांक name_distance = effective_distance(name->distance, name->generation);
	पूर्णांक new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	अगर (from_tag && name->from_tag)
		वापस name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	अगर (name->from_tag != from_tag)
		वापस from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	अगर (name_distance != new_distance)
		वापस name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	अगर (name->taggerdate != taggerdate)
		वापस name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	वापस 0;
}

अटल काष्ठा rev_name *create_or_update_name(काष्ठा commit *commit,
					      timestamp_t taggerdate,
					      पूर्णांक generation, पूर्णांक distance,
					      पूर्णांक from_tag)
{
	काष्ठा rev_name *name = commit_rev_name_at(&rev_names, commit);

	अगर (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		वापस शून्य;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	वापस name;
}

अटल अक्षर *get_parent_name(स्थिर काष्ठा rev_name *name, पूर्णांक parent_संख्या,
			     काष्ठा mem_pool *string_pool)
{
	माप_प्रकार len;

	strip_suffix(name->tip_name, "^0", &len);
	अगर (name->generation > 0) {
		वापस mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (पूर्णांक)len, name->tip_name,
				       name->generation, parent_संख्या);
	} अन्यथा {
		वापस mem_pool_strfmt(string_pool, "%.*s^%d",
				       (पूर्णांक)len, name->tip_name, parent_संख्या);
	}
}

अटल व्योम name_rev(काष्ठा commit *start_commit,
		स्थिर अक्षर *tip_name, timestamp_t taggerdate,
		पूर्णांक from_tag, पूर्णांक deref, काष्ठा mem_pool *string_pool)
{
	काष्ठा commit_ढेर ढेर = COMMIT_ढेर_INIT;
	काष्ठा commit *commit;
	काष्ठा commit_ढेर parents_to_queue = COMMIT_ढेर_INIT;
	काष्ठा rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	अगर (commit_is_before_cutoff(start_commit))
		वापस;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	अगर (!start_name)
		वापस;
	अगर (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	अन्यथा
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_ढेर_धकेलो(&ढेर, start_commit);

	जबतक ((commit = commit_ढेर_निकालो(&ढेर))) {
		काष्ठा rev_name *name = get_commit_rev_name(commit);
		काष्ठा commit_list *parents;
		पूर्णांक parent_संख्या = 1;

		parents_to_queue.nr = 0;

		क्रम (parents = commit->parents;
				parents;
				parents = parents->अग्र, parent_संख्या++) {
			काष्ठा commit *parent = parents->item;
			काष्ठा rev_name *parent_name;
			पूर्णांक generation, distance;

			repo_parse_commit(the_repository, parent);
			अगर (commit_is_before_cutoff(parent))
				जारी;

			अगर (parent_संख्या > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} अन्यथा {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			अगर (parent_name) {
				अगर (parent_संख्या > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_संख्या,
								string_pool);
				अन्यथा
					parent_name->tip_name = name->tip_name;
				commit_ढेर_धकेलो(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		जबतक (parents_to_queue.nr)
			commit_ढेर_धकेलो(&ढेर,
					  commit_ढेर_निकालो(&parents_to_queue));
	}

	commit_ढेर_clear(&ढेर);
	commit_ढेर_clear(&parents_to_queue);
}

अटल पूर्णांक subpath_matches(स्थिर अक्षर *path, स्थिर अक्षर *filter)
{
	स्थिर अक्षर *subpath = path;

	जबतक (subpath) {
		अगर (!wildmatch(filter, subpath, 0))
			वापस subpath - path;
		subpath = म_अक्षर(subpath, '/');
		अगर (subpath)
			subpath++;
	}
	वापस -1;
}

काष्ठा name_ref_तथ्य {
	पूर्णांक tags_only;
	पूर्णांक name_only;
	काष्ठा string_list ref_filters;
	काष्ठा string_list exclude_filters;
};

काष्ठा pretty_format {
	काष्ठा pretty_print_context ctx;
	काष्ठा userformat_want want;
};

क्रमागत command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

क्रमागत stdin_mode {
    TEXT = 1,
    REVS = 2,
};

काष्ठा command {
	क्रमागत command_type type;
	जोड़ {
		पूर्णांक name_only;
		काष्ठा pretty_format *pretty_format;
	} u;
};

अटल व्योम init_name_rev_command(काष्ठा command *cmd,
				  पूर्णांक name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

अटल व्योम init_format_rev_command(काष्ठा command *cmd,
				    काष्ठा pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

अटल काष्ठा tip_table {
	काष्ठा tip_table_entry {
		काष्ठा object_id oid;
		स्थिर अक्षर *refname;
		काष्ठा commit *commit;
		timestamp_t taggerdate;
		अचिन्हित पूर्णांक from_tag:1;
		अचिन्हित पूर्णांक deref:1;
	} *table;
	पूर्णांक nr;
	पूर्णांक alloc;
	पूर्णांक sorted;
} tip_table;

अटल व्योम add_to_tip_table(स्थिर काष्ठा object_id *oid, स्थिर अक्षर *refname,
			     पूर्णांक shorten_unambiguous, काष्ठा commit *commit,
			     timestamp_t taggerdate, पूर्णांक from_tag, पूर्णांक deref)
{
	अक्षर *short_refname = शून्य;

	अगर (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	अन्यथा अगर (skip_prefix(refname, "refs/heads/", &refname))
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

अटल पूर्णांक tipcmp(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	स्थिर काष्ठा tip_table_entry *a = a_, *b = b_;
	वापस oidcmp(&a->oid, &b->oid);
}

अटल पूर्णांक cmp_by_tag_and_age(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	स्थिर काष्ठा tip_table_entry *a = a_, *b = b_;
	पूर्णांक cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	अगर (cmp)
		वापस cmp;

	/* Older is better. */
	अगर (a->taggerdate < b->taggerdate)
		वापस -1;
	वापस a->taggerdate != b->taggerdate;
}

अटल पूर्णांक name_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा object *o = parse_object(the_repository, ref->oid);
	काष्ठा name_ref_तथ्य *तथ्य = cb_तथ्य;
	पूर्णांक can_abbreviate_output = तथ्य->tags_only && तथ्य->name_only;
	पूर्णांक deref = 0;
	पूर्णांक from_tag = 0;
	काष्ठा commit *commit = शून्य;
	timestamp_t taggerdate = TIME_MAX;

	अगर (तथ्य->tags_only && !starts_with(ref->name, "refs/tags/"))
		वापस 0;

	अगर (तथ्य->exclude_filters.nr) {
		काष्ठा string_list_item *item;

		for_each_string_list_item(item, &तथ्य->exclude_filters) {
			अगर (subpath_matches(ref->name, item->string) >= 0)
				वापस 0;
		}
	}

	अगर (तथ्य->ref_filters.nr) {
		काष्ठा string_list_item *item;
		पूर्णांक matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_item(item, &तथ्य->ref_filters) {
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
			चयन (subpath_matches(ref->name, item->string)) {
			हाल -1: /* did not match */
				अवरोध;
			हाल 0: /* matched fully */
				matched = 1;
				अवरोध;
			शेष: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				अवरोध;
			}
		}

		/* If none of the patterns matched, stop now */
		अगर (!matched)
			वापस 0;
	}

	जबतक (o && o->type == OBJ_TAG) {
		काष्ठा tag *t = (काष्ठा tag *) o;
		अगर (!t->tagged)
			अवरोध; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	अगर (o && o->type == OBJ_COMMIT) {
		commit = (काष्ठा commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		अगर (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	वापस 0;
}

अटल व्योम name_tips(काष्ठा mem_pool *string_pool)
{
	पूर्णांक i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	क्रम (i = 0; i < tip_table.nr; i++) {
		काष्ठा tip_table_entry *e = &tip_table.table[i];
		अगर (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

अटल स्थिर काष्ठा object_id *nth_tip_table_ent(माप_प्रकार ix, स्थिर व्योम *table_)
{
	स्थिर काष्ठा tip_table_entry *table = table_;
	वापस &table[ix].oid;
}

अटल स्थिर अक्षर *get_exact_ref_match(स्थिर काष्ठा object *o)
{
	पूर्णांक found;

	अगर (!tip_table.table || !tip_table.nr)
		वापस शून्य;

	अगर (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	अगर (0 <= found)
		वापस tip_table.table[found].refname;
	वापस शून्य;
}

/* may return a constant string or use "buf" as scratch space */
अटल स्थिर अक्षर *get_rev_name(स्थिर काष्ठा object *o, काष्ठा strbuf *buf)
{
	काष्ठा rev_name *n;
	स्थिर काष्ठा commit *c;

	अगर (o->type != OBJ_COMMIT)
		वापस get_exact_ref_match(o);
	c = (स्थिर काष्ठा commit *) o;
	n = get_commit_rev_name(c);
	अगर (!n)
		वापस शून्य;

	अगर (!n->generation) {
		वापस n->tip_name;
	} अन्यथा {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		वापस buf->buf;
	}
}

अटल स्थिर अक्षर *get_format_rev(स्थिर काष्ठा commit *c,
				  काष्ठा pretty_format *format_ctx,
				  काष्ठा strbuf *buf)
{
	strbuf_reset(buf);

	अगर (format_ctx->want.notes) {
		काष्ठा strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, शून्य);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	वापस buf->buf;
}

अटल व्योम show_name(स्थिर काष्ठा object *obj,
		      स्थिर अक्षर *caller_name,
		      पूर्णांक always, पूर्णांक allow_undefined, पूर्णांक name_only)
{
	स्थिर अक्षर *name;
	स्थिर काष्ठा object_id *oid = &obj->oid;
	काष्ठा strbuf buf = STRBUF_INIT;

	अगर (!name_only)
		म_लिखो("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	अगर (name)
		म_लिखो("%s\n", name);
	अन्यथा अगर (allow_undefined)
		म_लिखो("undefined\n");
	अन्यथा अगर (always)
		म_लिखो("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	अन्यथा
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

अटल अक्षर स्थिर * स्थिर name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	शून्य
};

अटल व्योम name_rev_पंक्ति(अक्षर *p, काष्ठा command *cmd)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	पूर्णांक गणक = 0;
	अक्षर *p_start;
	स्थिर अचिन्हित hexsz = the_hash_algo->hexsz;

	क्रम (p_start = p; *p; p++) {
#घोषणा ishex(x) (है_अंक((x)) || ((x) >= 'a' && (x) <= 'f'))
		अगर (!ishex(*p)) {
			गणक = 0;
		} अन्यथा अगर (++गणक == hexsz &&
			   !ishex(*(p + 1))) {
			काष्ठा object_id oid;
			स्थिर अक्षर *name = शून्य;
			अक्षर c = *(p + 1);
			पूर्णांक p_len = p - p_start + 1;
			काष्ठा object *o = शून्य;
			पूर्णांक oid_ret = 1;

			गणक = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			चयन (cmd->type) {
			हाल NAME_REV:
				अगर (!oid_ret)
					o = lookup_object(the_repository, &oid);
				अगर (o)
					name = get_rev_name(o, &buf);
				अगर (!name)
					जारी;
				अगर (cmd->u.name_only)
					म_लिखो("%.*s%s", p_len - hexsz, p_start, name);
				अन्यथा
					म_लिखो("%.*s (%s)", p_len, p_start, name);
				अवरोध;
			हाल FORMAT_REV:
				अगर (!oid_ret)
					o = parse_object(the_repository, &oid);
				अगर (o && o->type == OBJ_COMMIT)
					name = get_format_rev((स्थिर काष्ठा commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				अगर (name)
					म_लिखो("%.*s%s", p_len - hexsz, p_start, name);
				अन्यथा
					म_लिखो("%.*s", p_len, p_start);
				अवरोध;
			शेष:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	अगर (p_start != p)
		ख_डालो(p_start, p - p_start, 1, मानक_निकास);

	strbuf_release(&buf);
}

पूर्णांक cmd_name_rev(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा mem_pool string_pool;
	काष्ठा object_array revs = OBJECT_ARRAY_INIT;

#अगर_अघोषित WITH_BREAKING_CHANGES
	पूर्णांक transform_stdin = 0;
#पूर्ण_अगर
	पूर्णांक all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	काष्ठा name_ref_तथ्य तथ्य = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	काष्ठा command cmd;
	काष्ठा option opts[] = {
		OPT_BOOL(0, "name-only", &तथ्य.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &तथ्य.tags_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &तथ्य.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &तथ्य.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_GROUP(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#अगर_अघोषित WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#पूर्ण_अगर /* WITH_BREAKING_CHANGES */
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
	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, तथ्य.name_only);

#अगर_अघोषित WITH_BREAKING_CHANGES
	अगर (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#पूर्ण_अगर

	अगर (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	अगर (all || annotate_stdin)
		disable_cutoff();

	क्रम (; argc; argc--, argv++) {
		काष्ठा object_id oid;
		काष्ठा object *object;
		काष्ठा commit *commit;

		अगर (repo_get_oid(the_repository, *argv, &oid)) {
			ख_लिखो(मानक_त्रुटि, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			जारी;
		}

		commit = शून्य;
		object = parse_object(the_repository, &oid);
		अगर (object) {
			काष्ठा object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			अगर (peeled && peeled->type == OBJ_COMMIT)
				commit = (काष्ठा commit *)peeled;
		}

		अगर (!object) {
			ख_लिखो(मानक_त्रुटि, "Could not get object for %s. Skipping.\n",
					*argv);
			जारी;
		}

		अगर (commit)
			set_commit_cutoff(commit);

		अगर (peel_tag) {
			अगर (!commit) {
				ख_लिखो(मानक_त्रुटि, "Could not get commit for %s. Skipping.\n",
					*argv);
				जारी;
			}
			object = (काष्ठा object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &तथ्य);
	name_tips(&string_pool);

	अगर (annotate_stdin) {
		काष्ठा strbuf sb = STRBUF_INIT;

		जबतक (strbuf_getline(&sb, मानक_निवेश) != खातापूर्ण) {
			strbuf_addch(&sb, '\n');
			name_rev_पंक्ति(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} अन्यथा अगर (all) {
		पूर्णांक i, max;

		max = get_max_object_सूचकांक(the_repository);
		क्रम (i = 0; i < max; i++) {
			काष्ठा object *obj = get_indexed_object(the_repository, i);
			अगर (!obj || obj->type != OBJ_COMMIT)
				जारी;
			show_name(obj, शून्य,
				  always, allow_undefined, तथ्य.name_only);
		}
	} अन्यथा {
		पूर्णांक i;
		क्रम (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, तथ्य.name_only);
	}

	string_list_clear(&तथ्य.ref_filters, 0);
	string_list_clear(&तथ्य.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	वापस 0;
}

काष्ठा format_nul_तथ्य {
	bool nul_निवेश;
	bool nul_output;
};

अटल पूर्णांक format_nul_cb(स्थिर काष्ठा option *option,
			 स्थिर अक्षर *arg,
			 पूर्णांक unset)
{
	काष्ठा format_nul_तथ्य *तथ्य = option->मूल्य;
	तथ्य->nul_निवेश = 1;
	तथ्य->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	वापस 0;
}

अटल क्रमागत stdin_mode parse_stdin_mode(स्थिर अक्षर *stdin_mode)
{
	अगर (!म_भेद(stdin_mode, "text"))
		वापस TEXT;
	अन्यथा अगर (!म_भेद(stdin_mode, "revs") ||
		 !म_भेद(stdin_mode, "rev"))
		वापस REVS;
	अन्यथा
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

अटल अक्षर स्थिर *स्थिर format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	शून्य
};

पूर्णांक cmd_format_rev(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *format = शून्य;
	क्रमागत stdin_mode stdin_mode;
	स्थिर अक्षर *stdin_mode_arg = शून्य;
	काष्ठा format_nul_तथ्य nul_तथ्य = { 0, 0 };
	अक्षर output_terminator;
	strbuf_getline_fn getline_fn;
	काष्ठा display_notes_opt format_notes_opt;
	काष्ठा rev_info format_rev = REV_INFO_INIT;
	काष्ठा pretty_format format_pp = { 0 };
	काष्ठा string_list notes = STRING_LIST_INIT_NODUP;
	काष्ठा strbuf scratch_buf = STRBUF_INIT;
	काष्ठा command cmd;
	काष्ठा option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_तथ्य, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_तथ्य.nul_निवेश,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_तथ्य.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	अगर (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	अगर (!format)
		die(_("'%s' is required"), "--format");
	अगर (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_तथ्य.nul_निवेश ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_तथ्य.nul_output ? '\0' : '\n';

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
	अगर (format_pp.want.notes) {
		पूर्णांक ignore_show_notes = 0;
		काष्ठा string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	चयन (stdin_mode) {
	हाल TEXT:
		जबतक (getline_fn(&scratch_buf, मानक_निवेश) != खातापूर्ण) {
			name_rev_पंक्ति(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			म_लिखो("%c", output_terminator);
			maybe_flush_or_die(मानक_निकास, "stdout");
		}
		अवरोध;
	हाल REVS:
		जबतक (getline_fn(&scratch_buf, मानक_निवेश) != खातापूर्ण) {
			काष्ठा object_id oid;
			काष्ठा object *object;
			काष्ठा object *peeled;

			अगर (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				ख_लिखो(मानक_त्रुटि, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				जारी;
			}

			object = parse_object(the_repository, &oid);
			अगर (!object) {
				ख_लिखो(मानक_त्रुटि, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				जारी;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			अगर (!peeled || peeled->type != OBJ_COMMIT) {
				ख_लिखो(मानक_त्रुटि,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				जारी;
			}

			get_format_rev((काष्ठा commit *)peeled,
				       &format_pp, &scratch_buf);
			म_लिखो("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(मानक_निकास, "stdout");
			strbuf_release(&scratch_buf);
		}
		अवरोध;
	शेष:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	वापस 0;
}
