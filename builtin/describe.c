#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "refs.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "read-cache-ll.h"
#ضمن "revision.h"
#ضمن "diff.h"
#ضمن "hashmap.h"
#ضمن "setup.h"
#ضمن "strvec.h"
#ضمن "run-command.h"
#ضمن "odb.h"
#ضمن "list-objects.h"
#ضمن "commit-slab.h"
#ضمن "wildmatch.h"
#ضمن "prio-queue.h"
#ضمن "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, بنية commit_name *);

ساكن ثابت حرف * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	عدم
};

ساكن صحيح debug;	/* Display lots of verbose info */
ساكن صحيح all;	/* Any valid ref can be used */
ساكن صحيح tags;	/* Allow lightweight tags */
ساكن صحيح longformat;
ساكن صحيح first_parent;
ساكن صحيح abbrev = -1; /* unspecified */
ساكن صحيح max_candidates = DEFAULT_CANDIDATES;
ساكن بنية hashmap names;
ساكن صحيح have_util;
ساكن بنية string_list patterns = STRING_LIST_INIT_NODUP;
ساكن بنية string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ساكن صحيح always;
ساكن ثابت حرف *suffix, *dirty, *broken;
ساكن بنية commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
ساكن ثابت حرف *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", عدم
};

ساكن ثابت حرف *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", عدم
};

بنية commit_name {
	بنية hashmap_entry entry;
	بنية object_id peeled;
	بنية tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	بنية object_id oid;
	حرف *path;
};

ساكن ثابت حرف *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ساكن صحيح commit_name_neq(ثابت فراغ *cmp_data UNUSED,
			   ثابت بنية hashmap_entry *eptr,
			   ثابت بنية hashmap_entry *entry_or_key,
			   ثابت فراغ *peeled)
{
	ثابت بنية commit_name *cn1, *cn2;

	cn1 = container_of(eptr, ثابت بنية commit_name, entry);
	cn2 = container_of(entry_or_key, ثابت بنية commit_name, entry);

	أرجع !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ساكن مضمن بنية commit_name *find_commit_name(ثابت بنية object_id *peeled)
{
	أرجع hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						بنية commit_name, entry);
}

ساكن صحيح replace_name(بنية commit_name *e,
			       صحيح prio,
			       ثابت بنية object_id *oid,
			       بنية tag **tag)
{
	𐤀𐤌 (!e || e->prio < prio)
		أرجع 1;

	𐤀𐤌 (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		بنية tag *t;

		𐤀𐤌 (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			𐤀𐤌 (!t || parse_tag(the_repository, t))
				أرجع 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		𐤀𐤌 (!t || parse_tag(the_repository, t))
			أرجع 0;
		*tag = t;

		𐤀𐤌 (e->tag->date < t->date)
			أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ add_to_known_names(ثابت حرف *path,
			       ثابت بنية object_id *peeled,
			       صحيح prio,
			       ثابت بنية object_id *oid)
{
	بنية commit_name *e = find_commit_name(peeled);
	بنية tag *tag = عدم;
	𐤀𐤌 (replace_name(e, prio, oid, &tag)) {
		𐤀𐤌 (!e) {
			e = xmalloc(حجم(بنية commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = عدم;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		حرر(e->path);
		e->path = xstrdup(path);
	}
}

ساكن صحيح get_name(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	صحيح is_tag = 0;
	بنية object_id peeled;
	صحيح is_annotated, prio;
	ثابت حرف *path_to_match = عدم;

	𐤀𐤌 (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} وإلا 𐤀𐤌 (all) {
		𐤀𐤌 ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			أرجع 0;
		}
	} وإلا {
		/* Reject anything outside refs/tags/ unless --all */
		أرجع 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	𐤀𐤌 (exclude_patterns.nr) {
		بنية string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			𐤀𐤌 (!wildmatch(item->string, path_to_match, 0))
				أرجع 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	𐤀𐤌 (patterns.nr) {
		صحيح found = 0;
		بنية string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			𐤀𐤌 (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				اكسر;
			}
		}

		𐤀𐤌 (!found)
			أرجع 0;
	}

	/* Is it annotated? */
	𐤀𐤌 (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} وإلا {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	𐤀𐤌 (is_annotated)
		prio = 2;
	وإلا 𐤀𐤌 (is_tag)
		prio = 1;
	وإلا
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	أرجع 0;
}

بنية possible_tag {
	بنية commit_name *name;
	صحيح depth;
	صحيح found_order;
	unsigned flag_within;
};

ساكن صحيح compare_pt(ثابت فراغ *a_, ثابت فراغ *b_)
{
	بنية possible_tag *a = (بنية possible_tag *)a_;
	بنية possible_tag *b = (بنية possible_tag *)b_;
	𐤀𐤌 (a->depth != b->depth)
		أرجع a->depth - b->depth;
	𐤀𐤌 (a->found_order != b->found_order)
		أرجع a->found_order - b->found_order;
	أرجع 0;
}

ساكن unsigned long finish_depth_computation(بنية prio_queue *queue,
					      بنية possible_tag *best)
{
	unsigned long seen_commits = 0;
	بنية oidset unflagged = OIDSET_INIT;
	بنية commit *c;

	prio_queue_for_each(queue, c) {
		𐤀𐤌 (!(c->object.flags & best->flag_within))
			oidset_insert(&unflagged, &c->object.oid);
	}

	طالما ((c = prio_queue_get(queue))) {
		بنية commit_list *parents = c->parents;
		seen_commits++;
		𐤀𐤌 (c->object.flags & best->flag_within) {
			𐤀𐤌 (!oidset_size(&unflagged))
				اكسر;
		} وإلا {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		طالما (parents) {
			unsigned seen, flag_before, flag_after;
			بنية commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			𐤀𐤌 (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			𐤀𐤌 (!seen && !flag_after)
				oidset_insert(&unflagged, &p->object.oid);
			𐤀𐤌 (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	أرجع seen_commits;
}

ساكن فراغ append_name(بنية commit_name *n, بنية strbuf *dst)
{
	𐤀𐤌 (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		𐤀𐤌 (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	𐤀𐤌 (n->tag && !n->name_checked) {
		𐤀𐤌 (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	𐤀𐤌 (n->tag) {
		𐤀𐤌 (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} وإلا {
		strbuf_addstr(dst, n->path);
	}
}

ساكن فراغ append_suffix(صحيح depth, ثابت بنية object_id *oid, بنية strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

ساكن فراغ describe_commit(بنية commit *cmit, بنية strbuf *dst)
{
	بنية commit *c, *gave_up_on = عدم;
	بنية prio_queue queue = { compare_commits_by_commit_date };
	بنية commit_name *n;
	بنية possible_tag all_matches[MAX_TAGS];
	unsigned صحيح match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned صحيح unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	𐤀𐤌 (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		𐤀𐤌 (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		𐤀𐤌 (suffix)
			strbuf_addstr(dst, suffix);
		أرجع;
	}

	𐤀𐤌 (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	𐤀𐤌 (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	𐤀𐤌 (!have_util) {
		بنية hashmap_iter iter;
		بنية commit *c;
		بنية commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			𐤀𐤌 (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	طالما ((c = prio_queue_get(&queue))) {
		بنية commit_list *parents = c->parents;
		بنية commit_name **slot;

		seen_commits++;

		𐤀𐤌 (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			اكسر;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : عدم;
		𐤀𐤌 (n) {
			𐤀𐤌 (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} وإلا 𐤀𐤌 (match_cnt < max_candidates) {
				بنية possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				𐤀𐤌 (n->prio == 2)
					annotated_cnt++;
			}
		}
		لكل (cur_match = 0; cur_match < match_cnt; cur_match++) {
			بنية possible_tag *t = &all_matches[cur_match];
			𐤀𐤌 (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		𐤀𐤌 (annotated_cnt && !prio_queue_size(&queue)) {
			صحيح best_depth = INT_MAX;
			unsigned best_within = 0;
			لكل (cur_match = 0; cur_match < match_cnt; cur_match++) {
				بنية possible_tag *t = &all_matches[cur_match];
				𐤀𐤌 (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} وإلا 𐤀𐤌 (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			𐤀𐤌 ((c->object.flags & best_within) == best_within) {
				𐤀𐤌 (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				اكسر;
			}
		}
		طالما (parents) {
			بنية commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			𐤀𐤌 (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			𐤀𐤌 (first_parent)
				اكسر;
		}
	}

	𐤀𐤌 (!match_cnt) {
		بنية object_id *cmit_oid = &cmit->object.oid;
		𐤀𐤌 (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			𐤀𐤌 (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			أرجع;
		}
		𐤀𐤌 (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		وإلا
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	𐤀𐤌 (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	𐤀𐤌 (debug) {
		ساكن صحيح label_width = -1;
		𐤀𐤌 (label_width < 0) {
			صحيح i, w;
			لكل (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				𐤀𐤌 (label_width < w)
					label_width = w;
			}
		}
		لكل (cur_match = 0; cur_match < match_cnt; cur_match++) {
			بنية possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		𐤀𐤌 (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	𐤀𐤌 (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	𐤀𐤌 (suffix)
		strbuf_addstr(dst, suffix);
}

بنية process_commit_data {
	بنية commit *current_commit;
	ثابت بنية object_id *looking_for;
	بنية strbuf *dst;
	بنية rev_info *revs;
};

ساكن فراغ process_commit(بنية commit *commit, فراغ *data)
{
	بنية process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

ساكن فراغ process_object(بنية object *obj, ثابت حرف *path, فراغ *data)
{
	بنية process_commit_data *pcd = data;

	𐤀𐤌 (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		𐤀𐤌 (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = عدم;
	}
}

ساكن فراغ describe_blob(ثابت بنية object_id *oid, بنية strbuf *dst)
{
	بنية rev_info revs;
	بنية strvec args = STRVEC_INIT;
	بنية object_id head_oid;
	بنية process_commit_data pcd = { عدم, oid, dst, &revs};

	𐤀𐤌 (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     عدم);

	repo_init_revisions(the_repository, &revs, عدم);
	setup_revisions_from_strvec(&args, &revs, عدم);
	𐤀𐤌 (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	𐤀𐤌 (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	𐤀𐤌 (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

ساكن فراغ describe(ثابت حرف *arg, صحيح last_one)
{
	بنية object_id oid;
	بنية commit *cmit;
	بنية strbuf sb = STRBUF_INIT;

	𐤀𐤌 (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	𐤀𐤌 (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	𐤀𐤌 (cmit)
		describe_commit(cmit, &sb);
	وإلا 𐤀𐤌 (odb_read_object_info(the_repository->objects,
				      &oid, عدم) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	وإلا
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	𐤀𐤌 (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ساكن صحيح option_parse_exact_match(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	صحيح *val = opt->value;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	أرجع 0;
}

صحيح cmd_describe(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo UNUSED )
{
	بنية refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحيح contains = 0;
	بنية option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, عدم,
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
			.value = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.value = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	𐤀𐤌 (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	𐤀𐤌 (max_candidates < 0)
		max_candidates = 0;
	وإلا 𐤀𐤌 (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	𐤀𐤌 (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	𐤀𐤌 (contains) {
		بنية string_list_item *item;
		بنية strvec args;
		ثابت حرف **argv_copy;
		صحيح ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     عدم);
		𐤀𐤌 (always)
			strvec_push(&args, "--always");
		𐤀𐤌 (!all)
			strvec_push(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		𐤀𐤌 (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		𐤀𐤌 (argc)
			strvec_pushv(&args, argv);
		وإلا
			strvec_push(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		لكل (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = عدم;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		حرر(argv_copy);
		أرجع ret;
	}

	𐤀𐤌 (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, عدم, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, عدم, &for_each_ref_opts);
	𐤀𐤌 (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	𐤀𐤌 (argc == 0) {
		𐤀𐤌 (broken) {
			بنية child_process cp = CHILD_PROCESS_INIT;

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

			𐤀𐤌 (!dirty)
				dirty = "-dirty";

			بدل (run_command(&cp)) {
			حالة 0:
				suffix = عدم;
				اكسر;
			حالة 1:
				suffix = dirty;
				اكسر;
			افتراضي:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} وإلا 𐤀𐤌 (dirty) {
			بنية lock_file index_lock = LOCK_INIT;
			بنية rev_info revs;
			صحيح fd;

			setup_work_tree(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      عدم, عدم, عدم);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			𐤀𐤌 (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			𐤀𐤌 (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, عدم) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			𐤀𐤌 (!diff_result_code(&revs))
				suffix = عدم;
			وإلا
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} وإلا 𐤀𐤌 (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} وإلا 𐤀𐤌 (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} وإلا {
		طالما (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	أرجع 0;
}
