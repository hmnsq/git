#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "read-cache-ll.h"
#شامل "revision.h"
#شامل "diff.h"
#شامل "hashmap.h"
#شامل "setup.h"
#شامل "strvec.h"
#شامل "run-command.h"
#شامل "odb.h"
#شامل "list-objects.h"
#شامل "commit-slab.h"
#شامل "wildmatch.h"
#شامل "prio-queue.h"
#شامل "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, ساختار commit_name *);

ایستا ثابت نویسه * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	پوچ
};

ایستا صحیح debug;	/* Display lots of verbose info */
ایستا صحیح all;	/* Any valid ref can be used */
ایستا صحیح tags;	/* Allow lightweight tags */
ایستا صحیح longformat;
ایستا صحیح first_parent;
ایستا صحیح abbrev = -1; /* unspecified */
ایستا صحیح max_candidates = DEFAULT_CANDIDATES;
ایستا ساختار hashmap names;
ایستا صحیح have_util;
ایستا ساختار string_list patterns = STRING_LIST_INIT_NODUP;
ایستا ساختار string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ایستا صحیح always;
ایستا ثابت نویسه *suffix, *dirty, *broken;
ایستا ساختار commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
ایستا ثابت نویسه *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", پوچ
};

ایستا ثابت نویسه *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", پوچ
};

ساختار commit_name {
	ساختار hashmap_entry entry;
	ساختار object_id peeled;
	ساختار tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	ساختار object_id oid;
	نویسه *path;
};

ایستا ثابت نویسه *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ایستا صحیح commit_name_neq(ثابت تهی *cmp_data UNUSED,
			   ثابت ساختار hashmap_entry *eptr,
			   ثابت ساختار hashmap_entry *entry_or_کلید,
			   ثابت تهی *peeled)
{
	ثابت ساختار commit_name *cn1, *cn2;

	cn1 = container_of(eptr, ثابت ساختار commit_name, entry);
	cn2 = container_of(entry_or_کلید, ثابت ساختار commit_name, entry);

	بازگشت !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ایستا درونخطی ساختار commit_name *find_commit_name(ثابت ساختار object_id *peeled)
{
	بازگشت hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						ساختار commit_name, entry);
}

ایستا صحیح replace_name(ساختار commit_name *e,
			       صحیح prio,
			       ثابت ساختار object_id *oid,
			       ساختار tag **tag)
{
	اگر (!e || e->prio < prio)
		بازگشت 1;

	اگر (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ساختار tag *t;

		اگر (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			اگر (!t || parse_tag(the_repository, t))
				بازگشت 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		اگر (!t || parse_tag(the_repository, t))
			بازگشت 0;
		*tag = t;

		اگر (e->tag->date < t->date)
			بازگشت 1;
	}

	بازگشت 0;
}

ایستا تهی add_to_known_names(ثابت نویسه *path,
			       ثابت ساختار object_id *peeled,
			       صحیح prio,
			       ثابت ساختار object_id *oid)
{
	ساختار commit_name *e = find_commit_name(peeled);
	ساختار tag *tag = پوچ;
	اگر (replace_name(e, prio, oid, &tag)) {
		اگر (!e) {
			e = xmalloc(اندازه(ساختار commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = پوچ;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		آزاد(e->path);
		e->path = xstrdup(path);
	}
}

ایستا صحیح get_name(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	صحیح is_tag = 0;
	ساختار object_id peeled;
	صحیح is_annotated, prio;
	ثابت نویسه *path_to_match = پوچ;

	اگر (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} وگرنه اگر (all) {
		اگر ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			بازگشت 0;
		}
	} وگرنه {
		/* Reject anything outside refs/tags/ unless --all */
		بازگشت 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	اگر (exclude_patterns.nr) {
		ساختار string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			اگر (!wildmatch(item->string, path_to_match, 0))
				بازگشت 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	اگر (patterns.nr) {
		صحیح found = 0;
		ساختار string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			اگر (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				بشکن;
			}
		}

		اگر (!found)
			بازگشت 0;
	}

	/* Is it annotated? */
	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} وگرنه {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	اگر (is_annotated)
		prio = 2;
	وگرنه اگر (is_tag)
		prio = 1;
	وگرنه
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	بازگشت 0;
}

ساختار possible_tag {
	ساختار commit_name *name;
	صحیح depth;
	صحیح found_order;
	unsigned flag_within;
};

ایستا صحیح compare_pt(ثابت تهی *a_, ثابت تهی *b_)
{
	ساختار possible_tag *a = (ساختار possible_tag *)a_;
	ساختار possible_tag *b = (ساختار possible_tag *)b_;
	اگر (a->depth != b->depth)
		بازگشت a->depth - b->depth;
	اگر (a->found_order != b->found_order)
		بازگشت a->found_order - b->found_order;
	بازگشت 0;
}

ایستا unsigned long finish_depth_computation(ساختار prio_queue *queue,
					      ساختار possible_tag *best)
{
	unsigned long seen_commits = 0;
	ساختار oidset unflagged = OIDSET_INIT;
	ساختار commit *c;

	prio_queue_for_each(queue, c) {
		اگر (!(c->object.flags & best->flag_within))
			oidset_درج(&unflagged, &c->object.oid);
	}

	وقتی ((c = prio_queue_get(queue))) {
		ساختار commit_list *parents = c->parents;
		seen_commits++;
		اگر (c->object.flags & best->flag_within) {
			اگر (!oidset_size(&unflagged))
				بشکن;
		} وگرنه {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		وقتی (parents) {
			unsigned seen, flag_before, flag_after;
			ساختار commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			اگر (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			اگر (!seen && !flag_after)
				oidset_درج(&unflagged, &p->object.oid);
			اگر (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	بازگشت seen_commits;
}

ایستا تهی append_name(ساختار commit_name *n, ساختار strbuf *dst)
{
	اگر (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		اگر (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	اگر (n->tag && !n->name_checked) {
		اگر (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	اگر (n->tag) {
		اگر (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} وگرنه {
		strbuf_addstr(dst, n->path);
	}
}

ایستا تهی append_suffix(صحیح depth, ثابت ساختار object_id *oid, ساختار strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

ایستا تهی describe_commit(ساختار commit *cmit, ساختار strbuf *dst)
{
	ساختار commit *c, *gave_up_on = پوچ;
	ساختار prio_queue queue = { compare_commits_by_commit_date };
	ساختار commit_name *n;
	ساختار possible_tag all_matches[MAX_TAGS];
	unsigned صحیح match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned صحیح unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	اگر (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		اگر (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		اگر (suffix)
			strbuf_addstr(dst, suffix);
		بازگشت;
	}

	اگر (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	اگر (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	اگر (!have_util) {
		ساختار hashmap_iter iter;
		ساختار commit *c;
		ساختار commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			اگر (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	وقتی ((c = prio_queue_get(&queue))) {
		ساختار commit_list *parents = c->parents;
		ساختار commit_name **slot;

		seen_commits++;

		اگر (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			بشکن;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : پوچ;
		اگر (n) {
			اگر (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} وگرنه اگر (match_cnt < max_candidates) {
				ساختار possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				اگر (n->prio == 2)
					annotated_cnt++;
			}
		}
		برای (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ساختار possible_tag *t = &all_matches[cur_match];
			اگر (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		اگر (annotated_cnt && !prio_queue_size(&queue)) {
			صحیح best_depth = INT_MAX;
			unsigned best_within = 0;
			برای (cur_match = 0; cur_match < match_cnt; cur_match++) {
				ساختار possible_tag *t = &all_matches[cur_match];
				اگر (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} وگرنه اگر (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			اگر ((c->object.flags & best_within) == best_within) {
				اگر (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				بشکن;
			}
		}
		وقتی (parents) {
			ساختار commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			اگر (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			اگر (first_parent)
				بشکن;
		}
	}

	اگر (!match_cnt) {
		ساختار object_id *cmit_oid = &cmit->object.oid;
		اگر (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			اگر (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			بازگشت;
		}
		اگر (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		وگرنه
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	اگر (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	اگر (debug) {
		ایستا صحیح label_width = -1;
		اگر (label_width < 0) {
			صحیح i, w;
			برای (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				اگر (label_width < w)
					label_width = w;
			}
		}
		برای (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ساختار possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		اگر (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	اگر (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	اگر (suffix)
		strbuf_addstr(dst, suffix);
}

ساختار process_commit_data {
	ساختار commit *current_commit;
	ثابت ساختار object_id *looking_for;
	ساختار strbuf *dst;
	ساختار rev_info *revs;
};

ایستا تهی process_commit(ساختار commit *commit, تهی *data)
{
	ساختار process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

ایستا تهی process_object(ساختار object *obj, ثابت نویسه *path, تهی *data)
{
	ساختار process_commit_data *pcd = data;

	اگر (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		اگر (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = پوچ;
	}
}

ایستا تهی describe_blob(ثابت ساختار object_id *oid, ساختار strbuf *dst)
{
	ساختار rev_info revs;
	ساختار strvec args = STRVEC_INIT;
	ساختار object_id head_oid;
	ساختار process_commit_data pcd = { پوچ, oid, dst, &revs};

	اگر (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     پوچ);

	repo_init_revisions(the_repository, &revs, پوچ);
	setup_revisions_from_strvec(&args, &revs, پوچ);
	اگر (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	اگر (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	اگر (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

ایستا تهی describe(ثابت نویسه *arg, صحیح last_one)
{
	ساختار object_id oid;
	ساختار commit *cmit;
	ساختار strbuf sb = STRBUF_INIT;

	اگر (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	اگر (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	اگر (cmit)
		describe_commit(cmit, &sb);
	وگرنه اگر (odb_read_object_info(the_repository->objects,
				      &oid, پوچ) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	وگرنه
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	اگر (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ایستا صحیح option_parse_exact_match(ثابت ساختار option *opt, ثابت نویسه *arg,
				    صحیح unset)
{
	صحیح *val = opt->مقدار;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	بازگشت 0;
}

صحیح cmd_describe(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED )
{
	ساختار refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحیح contains = 0;
	ساختار option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, پوچ,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_match),
		OPT_INTEGER(0, "candidates", &max_candidates,
			    N_("consider <n> most recent tags (default: 10)")),
		OPT_STRING_LIST(0, "match", &patterns, N_("pattern"),
			   N_("only consider tags matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &exclude_patterns, N_("pattern"),
			   N_("do not consider tags matching <pattern>")),
		OPT_BOOL(0, "always",        &always,
			N_("show abbreviated commit object as fallback")),
		{
			.type = OPTION_STRING,
			.long_name = "dirty",
			.مقدار = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.مقدار = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	اگر (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	اگر (max_candidates < 0)
		max_candidates = 0;
	وگرنه اگر (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	اگر (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	اگر (contains) {
		ساختار string_list_item *item;
		ساختار strvec args;
		ثابت نویسه **argv_copy;
		صحیح ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     پوچ);
		اگر (always)
			strvec_افزودن(&args, "--always");
		اگر (!all)
			strvec_افزودن(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		اگر (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		اگر (argc)
			strvec_pushv(&args, argv);
		وگرنه
			strvec_افزودن(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		برای (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = پوچ;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		آزاد(argv_copy);
		بازگشت ret;
	}

	اگر (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, پوچ, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, پوچ, &for_each_ref_opts);
	اگر (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	اگر (argc == 0) {
		اگر (broken) {
			ساختار child_process cp = CHILD_PROCESS_INIT;

			strvec_pushv(&cp.args, update_index_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			run_command(&cp);

			child_process_init(&cp);
			strvec_pushv(&cp.args, diff_index_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			اگر (!dirty)
				dirty = "-dirty";

			انتخاب (run_command(&cp)) {
			حالت 0:
				suffix = پوچ;
				بشکن;
			حالت 1:
				suffix = dirty;
				بشکن;
			پیشفرض:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} وگرنه اگر (dirty) {
			ساختار lock_file index_lock = LOCK_INIT;
			ساختار rev_info revs;
			صحیح fd;

			setup_work_درخت(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      پوچ, پوچ, پوچ);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			اگر (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			اگر (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, پوچ) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			اگر (!diff_result_code(&revs))
				suffix = پوچ;
			وگرنه
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} وگرنه اگر (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} وگرنه اگر (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} وگرنه {
		وقتی (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	بازگشت 0;
}
