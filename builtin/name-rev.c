#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "pager.h"
#شامل "parse-options.h"
#شامل "hash-lookup.h"
#شامل "commit-slab.h"
#شامل "commit-graph.h"
#شامل "wildmatch.h"
#شامل "mem-pool.h"
#شامل "pretty.h"
#شامل "revision.h"
#شامل "notes.h"
#شامل "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#define CUTOFF_DATE_SLOP 86400

ساختار rev_name {
	ثابت نویسه *tip_name;
	timestamp_t taggerdate;
	صحیح generation;
	صحیح distance;
	صحیح from_tag;
};

define_commit_slab(commit_rev_name, ساختار rev_name);

ایستا timestamp_t generation_cutoff = GENERATION_عدد_INFINITY;
ایستا timestamp_t cutoff = TIME_MAX;
ایستا ساختار commit_rev_name rev_names;

/* Disable the cutoff checks entirely */
ایستا تهی disable_cutoff(تهی)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
ایستا تهی set_commit_cutoff(ساختار commit *commit)
{

	اگر (cutoff > commit->date)
		cutoff = commit->date;

	اگر (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		اگر (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
ایستا تهی adjust_cutoff_timestamp_for_slop(تهی)
{
	اگر (cutoff) {
		/* check for underflow */
		اگر (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		وگرنه
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
ایستا صحیح commit_is_before_cutoff(ساختار commit *commit)
{
	اگر (generation_cutoff < GENERATION_عدد_INFINITY)
		بازگشت generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	بازگشت commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#define MERGE_TRAVERSAL_WEIGHT 65535

ایستا صحیح is_valid_rev_name(ثابت ساختار rev_name *name)
{
	بازگشت name && name->tip_name;
}

ایستا ساختار rev_name *get_commit_rev_name(ثابت ساختار commit *commit)
{
	ساختار rev_name *name = commit_rev_name_peek(&rev_names, commit);

	بازگشت is_valid_rev_name(name) ? name : پوچ;
}

ایستا صحیح effective_distance(صحیح distance, صحیح generation)
{
	بازگشت distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

ایستا صحیح is_better_name(ساختار rev_name *name,
			  timestamp_t taggerdate,
			  صحیح generation,
			  صحیح distance,
			  صحیح from_tag)
{
	صحیح name_distance = effective_distance(name->distance, name->generation);
	صحیح new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	اگر (from_tag && name->from_tag)
		بازگشت name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	اگر (name->from_tag != from_tag)
		بازگشت from_tag;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	اگر (name_distance != new_distance)
		بازگشت name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	اگر (name->taggerdate != taggerdate)
		بازگشت name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	بازگشت 0;
}

ایستا ساختار rev_name *create_or_update_name(ساختار commit *commit,
					      timestamp_t taggerdate,
					      صحیح generation, صحیح distance,
					      صحیح from_tag)
{
	ساختار rev_name *name = commit_rev_name_at(&rev_names, commit);

	اگر (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_tag))
		بازگشت پوچ;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_tag = from_tag;

	بازگشت name;
}

ایستا نویسه *get_parent_name(ثابت ساختار rev_name *name, صحیح parent_عدد,
			     ساختار mem_pool *string_pool)
{
	size_t len;

	strip_suffix(name->tip_name, "^0", &len);
	اگر (name->generation > 0) {
		بازگشت mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (صحیح)len, name->tip_name,
				       name->generation, parent_عدد);
	} وگرنه {
		بازگشت mem_pool_strfmt(string_pool, "%.*s^%d",
				       (صحیح)len, name->tip_name, parent_عدد);
	}
}

ایستا تهی name_rev(ساختار commit *start_commit,
		ثابت نویسه *tip_name, timestamp_t taggerdate,
		صحیح from_tag, صحیح deref, ساختار mem_pool *string_pool)
{
	ساختار commit_پشته پشته = COMMIT_پشته_INIT;
	ساختار commit *commit;
	ساختار commit_پشته parents_to_queue = COMMIT_پشته_INIT;
	ساختار rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	اگر (commit_is_before_cutoff(start_commit))
		بازگشت;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_tag);
	اگر (!start_name)
		بازگشت;
	اگر (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	وگرنه
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_پشته_افزودن(&پشته, start_commit);

	وقتی ((commit = commit_پشته_برداشت(&پشته))) {
		ساختار rev_name *name = get_commit_rev_name(commit);
		ساختار commit_list *parents;
		صحیح parent_عدد = 1;

		parents_to_queue.nr = 0;

		برای (parents = commit->parents;
				parents;
				parents = parents->next, parent_عدد++) {
			ساختار commit *parent = parents->item;
			ساختار rev_name *parent_name;
			صحیح generation, distance;

			repo_parse_commit(the_repository, parent);
			اگر (commit_is_before_cutoff(parent))
				ادامه;

			اگر (parent_عدد > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} وگرنه {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_tag);
			اگر (parent_name) {
				اگر (parent_عدد > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_عدد,
								string_pool);
				وگرنه
					parent_name->tip_name = name->tip_name;
				commit_پشته_افزودن(&parents_to_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		وقتی (parents_to_queue.nr)
			commit_پشته_افزودن(&پشته,
					  commit_پشته_برداشت(&parents_to_queue));
	}

	commit_پشته_clear(&پشته);
	commit_پشته_clear(&parents_to_queue);
}

ایستا صحیح subpath_matches(ثابت نویسه *path, ثابت نویسه *filter)
{
	ثابت نویسه *subpath = path;

	وقتی (subpath) {
		اگر (!wildmatch(filter, subpath, 0))
			بازگشت subpath - path;
		subpath = strchr(subpath, '/');
		اگر (subpath)
			subpath++;
	}
	بازگشت -1;
}

ساختار name_ref_data {
	صحیح tags_only;
	صحیح name_only;
	ساختار string_list ref_filters;
	ساختار string_list exclude_filters;
};

ساختار pretty_format {
	ساختار pretty_print_context ctx;
	ساختار userformat_want want;
};

شمارش command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

شمارش stdin_mode {
    TEXT = 1,
    REVS = 2,
};

ساختار command {
	شمارش command_type type;
	اجتماع {
		صحیح name_only;
		ساختار pretty_format *pretty_format;
	} u;
};

ایستا تهی init_name_rev_command(ساختار command *cmd,
				  صحیح name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

ایستا تهی init_format_rev_command(ساختار command *cmd,
				    ساختار pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

ایستا ساختار tip_table {
	ساختار tip_table_entry {
		ساختار object_id oid;
		ثابت نویسه *refname;
		ساختار commit *commit;
		timestamp_t taggerdate;
		unsigned صحیح from_tag:1;
		unsigned صحیح deref:1;
	} *table;
	صحیح nr;
	صحیح alloc;
	صحیح sorted;
} tip_table;

ایستا تهی add_to_tip_table(ثابت ساختار object_id *oid, ثابت نویسه *refname,
			     صحیح shorten_unambiguous, ساختار commit *commit,
			     timestamp_t taggerdate, صحیح from_tag, صحیح deref)
{
	نویسه *short_refname = پوچ;

	اگر (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	وگرنه اگر (skip_prefix(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	وگرنه
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

ایستا صحیح tipcmp(ثابت تهی *a_, ثابت تهی *b_)
{
	ثابت ساختار tip_table_entry *a = a_, *b = b_;
	بازگشت oidcmp(&a->oid, &b->oid);
}

ایستا صحیح cmp_by_tag_and_age(ثابت تهی *a_, ثابت تهی *b_)
{
	ثابت ساختار tip_table_entry *a = a_, *b = b_;
	صحیح cmp;

	/* Prefer tags. */
	cmp = b->from_tag - a->from_tag;
	اگر (cmp)
		بازگشت cmp;

	/* Older is better. */
	اگر (a->taggerdate < b->taggerdate)
		بازگشت -1;
	بازگشت a->taggerdate != b->taggerdate;
}

ایستا صحیح name_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار object *o = parse_object(the_repository, ref->oid);
	ساختار name_ref_data *data = cb_data;
	صحیح can_abbreviate_output = data->tags_only && data->name_only;
	صحیح deref = 0;
	صحیح from_tag = 0;
	ساختار commit *commit = پوچ;
	timestamp_t taggerdate = TIME_MAX;

	اگر (data->tags_only && !starts_with(ref->name, "refs/tags/"))
		بازگشت 0;

	اگر (data->exclude_filters.nr) {
		ساختار string_list_item *item;

		for_each_string_list_item(item, &data->exclude_filters) {
			اگر (subpath_matches(ref->name, item->string) >= 0)
				بازگشت 0;
		}
	}

	اگر (data->ref_filters.nr) {
		ساختار string_list_item *item;
		صحیح matched = 0;

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
			انتخاب (subpath_matches(ref->name, item->string)) {
			حالت -1: /* did not match */
				بشکن;
			حالت 0: /* matched fully */
				matched = 1;
				بشکن;
			پیشفرض: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				بشکن;
			}
		}

		/* If none of the patterns matched, stop now */
		اگر (!matched)
			بازگشت 0;
	}

	وقتی (o && o->type == OBJ_TAG) {
		ساختار tag *t = (ساختار tag *) o;
		اگر (!t->tagged)
			بشکن; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	اگر (o && o->type == OBJ_COMMIT) {
		commit = (ساختار commit *)o;
		from_tag = starts_with(ref->name, "refs/tags/");
		اگر (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	add_to_tip_table(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_tag, deref);
	بازگشت 0;
}

ایستا تهی name_tips(ساختار mem_pool *string_pool)
{
	صحیح i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_table.table, tip_table.nr, cmp_by_tag_and_age);
	برای (i = 0; i < tip_table.nr; i++) {
		ساختار tip_table_entry *e = &tip_table.table[i];
		اگر (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_tag, e->deref, string_pool);
		}
	}
}

ایستا ثابت ساختار object_id *nth_tip_table_ent(size_t ix, ثابت تهی *table_)
{
	ثابت ساختار tip_table_entry *table = table_;
	بازگشت &table[ix].oid;
}

ایستا ثابت نویسه *get_exact_ref_match(ثابت ساختار object *o)
{
	صحیح found;

	اگر (!tip_table.table || !tip_table.nr)
		بازگشت پوچ;

	اگر (!tip_table.sorted) {
		QSORT(tip_table.table, tip_table.nr, tipcmp);
		tip_table.sorted = 1;
	}

	found = oid_pos(&o->oid, tip_table.table, tip_table.nr,
			nth_tip_table_ent);
	اگر (0 <= found)
		بازگشت tip_table.table[found].refname;
	بازگشت پوچ;
}

/* may return a constant string or use "buf" as scratch space */
ایستا ثابت نویسه *get_rev_name(ثابت ساختار object *o, ساختار strbuf *buf)
{
	ساختار rev_name *n;
	ثابت ساختار commit *c;

	اگر (o->type != OBJ_COMMIT)
		بازگشت get_exact_ref_match(o);
	c = (ثابت ساختار commit *) o;
	n = get_commit_rev_name(c);
	اگر (!n)
		بازگشت پوچ;

	اگر (!n->generation) {
		بازگشت n->tip_name;
	} وگرنه {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		بازگشت buf->buf;
	}
}

ایستا ثابت نویسه *get_format_rev(ثابت ساختار commit *c,
				  ساختار pretty_format *format_ctx,
				  ساختار strbuf *buf)
{
	strbuf_reset(buf);

	اگر (format_ctx->want.notes) {
		ساختار strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_message = strbuf_detach(&notebuf, پوچ);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_message);

	بازگشت buf->buf;
}

ایستا تهی show_name(ثابت ساختار object *obj,
		      ثابت نویسه *caller_name,
		      صحیح always, صحیح allow_undefined, صحیح name_only)
{
	ثابت نویسه *name;
	ثابت ساختار object_id *oid = &obj->oid;
	ساختار strbuf buf = STRBUF_INIT;

	اگر (!name_only)
		printf("%s ", caller_name ? caller_name : oid_to_hex(oid));
	name = get_rev_name(obj, &buf);
	اگر (name)
		printf("%s\n", name);
	وگرنه اگر (allow_undefined)
		printf("undefined\n");
	وگرنه اگر (always)
		printf("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	وگرنه
		die("cannot describe '%s'", oid_to_hex(oid));
	strbuf_release(&buf);
}

ایستا نویسه ثابت * ثابت name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	پوچ
};

ایستا تهی name_rev_سطر(نویسه *p, ساختار command *cmd)
{
	ساختار strbuf buf = STRBUF_INIT;
	صحیح counter = 0;
	نویسه *p_start;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	برای (p_start = p; *p; p++) {
#define ishex(x) (isdigit((x)) || ((x) >= 'a' && (x) <= 'f'))
		اگر (!ishex(*p)) {
			counter = 0;
		} وگرنه اگر (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			ساختار object_id oid;
			ثابت نویسه *name = پوچ;
			نویسه c = *(p + 1);
			صحیح p_len = p - p_start + 1;
			ساختار object *o = پوچ;
			صحیح oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			انتخاب (cmd->type) {
			حالت NAME_REV:
				اگر (!oid_ret)
					o = lookup_object(the_repository, &oid);
				اگر (o)
					name = get_rev_name(o, &buf);
				اگر (!name)
					ادامه;
				اگر (cmd->u.name_only)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				وگرنه
					printf("%.*s (%s)", p_len, p_start, name);
				بشکن;
			حالت FORMAT_REV:
				اگر (!oid_ret)
					o = parse_object(the_repository, &oid);
				اگر (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ثابت ساختار commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				اگر (name)
					printf("%.*s%s", p_len - hexsz, p_start, name);
				وگرنه
					printf("%.*s", p_len, p_start);
				بشکن;
			پیشفرض:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	اگر (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

صحیح cmd_name_rev(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED)
{
	ساختار mem_pool string_pool;
	ساختار object_array revs = OBJECT_ARRAY_INIT;

#ifndef WITH_BREAKING_CHANGES
	صحیح transform_stdin = 0;
#endif
	صحیح all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_tag = 0;
	ساختار name_ref_data data = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	ساختار command cmd;
	ساختار option opts[] = {
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
	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, data.name_only);

#ifndef WITH_BREAKING_CHANGES
	اگر (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#endif

	اگر (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	اگر (all || annotate_stdin)
		disable_cutoff();

	برای (; argc; argc--, argv++) {
		ساختار object_id oid;
		ساختار object *object;
		ساختار commit *commit;

		اگر (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			ادامه;
		}

		commit = پوچ;
		object = parse_object(the_repository, &oid);
		اگر (object) {
			ساختار object *peeled = deref_tag(the_repository,
							  object, *argv, 0);
			اگر (peeled && peeled->type == OBJ_COMMIT)
				commit = (ساختار commit *)peeled;
		}

		اگر (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			ادامه;
		}

		اگر (commit)
			set_commit_cutoff(commit);

		اگر (peel_tag) {
			اگر (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				ادامه;
			}
			object = (ساختار object *)commit;
		}
		add_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &data);
	name_tips(&string_pool);

	اگر (annotate_stdin) {
		ساختار strbuf sb = STRBUF_INIT;

		وقتی (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_سطر(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} وگرنه اگر (all) {
		صحیح i, max;

		max = get_max_object_index(the_repository);
		برای (i = 0; i < max; i++) {
			ساختار object *obj = get_indexed_object(the_repository, i);
			اگر (!obj || obj->type != OBJ_COMMIT)
				ادامه;
			show_name(obj, پوچ,
				  always, allow_undefined, data.name_only);
		}
	} وگرنه {
		صحیح i;
		برای (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, data.name_only);
	}

	string_list_clear(&data.ref_filters, 0);
	string_list_clear(&data.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	بازگشت 0;
}

ساختار format_nul_data {
	bool nul_درون_داد;
	bool nul_output;
};

ایستا صحیح format_nul_cb(ثابت ساختار option *option,
			 ثابت نویسه *arg,
			 صحیح unset)
{
	ساختار format_nul_data *data = option->مقدار;
	data->nul_درون_داد = 1;
	data->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	بازگشت 0;
}

ایستا شمارش stdin_mode parse_stdin_mode(ثابت نویسه *stdin_mode)
{
	اگر (!strcmp(stdin_mode, "text"))
		بازگشت TEXT;
	وگرنه اگر (!strcmp(stdin_mode, "revs") ||
		 !strcmp(stdin_mode, "rev"))
		بازگشت REVS;
	وگرنه
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

ایستا نویسه ثابت *ثابت format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	پوچ
};

صحیح cmd_format_rev(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ثابت نویسه *format = پوچ;
	شمارش stdin_mode stdin_mode;
	ثابت نویسه *stdin_mode_arg = پوچ;
	ساختار format_nul_data nul_data = { 0, 0 };
	نویسه output_terminator;
	strbuf_getline_fn getline_fn;
	ساختار display_notes_opt format_notes_opt;
	ساختار rev_info format_rev = REV_INFO_INIT;
	ساختار pretty_format format_pp = { 0 };
	ساختار string_list notes = STRING_LIST_INIT_NODUP;
	ساختار strbuf scratch_buf = STRBUF_INIT;
	ساختار command cmd;
	ساختار option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_mode_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_data, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_data.nul_درون_داد,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_data.nul_output,
			 N_("Use NUL for output termination")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, opts, format_rev_usage, 0);

	اگر (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	اگر (!format)
		die(_("'%s' is required"), "--format");
	اگر (!stdin_mode_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_data.nul_درون_داد ? strbuf_getline_nul : strbuf_getline_lf;
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
	اگر (format_pp.want.notes) {
		صحیح ignore_show_notes = 0;
		ساختار string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		load_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	انتخاب (stdin_mode) {
	حالت TEXT:
		وقتی (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_سطر(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			printf("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		بشکن;
	حالت REVS:
		وقتی (getline_fn(&scratch_buf, stdin) != EOF) {
			ساختار object_id oid;
			ساختار object *object;
			ساختار object *peeled;

			اگر (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				ادامه;
			}

			object = parse_object(the_repository, &oid);
			اگر (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				ادامه;
			}

			peeled = deref_tag(the_repository, object, scratch_buf.buf, 0);
			اگر (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				ادامه;
			}

			get_format_rev((ساختار commit *)peeled,
				       &format_pp, &scratch_buf);
			printf("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		بشکن;
	پیشفرض:
		BUG("uncovered case: %d", stdin_mode);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	بازگشت 0;
}
