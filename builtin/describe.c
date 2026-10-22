#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "hashmap.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "list-objects.h"
#અન્તર્ભાવ "commit-slab.h"
#અન્તર્ભાવ "wildmatch.h"
#અન્તર્ભાવ "prio-queue.h"
#અન્તર્ભાવ "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, સંરચના commit_name *);

સ્થિર અચલ અક્ષર * અચલ describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ખાલી
};

સ્થિર પૂર્ણાંક debug;	/* Display lots of verbose info */
સ્થિર પૂર્ણાંક all;	/* Any valid ref can be used */
સ્થિર પૂર્ણાંક tags;	/* Allow lightweight tags */
સ્થિર પૂર્ણાંક longformat;
સ્થિર પૂર્ણાંક first_parent;
સ્થિર પૂર્ણાંક abbrev = -1; /* unspecified */
સ્થિર પૂર્ણાંક max_candidates = DEFAULT_CANDIDATES;
સ્થિર સંરચના hashmap names;
સ્થિર પૂર્ણાંક have_util;
સ્થિર સંરચના string_list patterns = STRING_LIST_INIT_NODUP;
સ્થિર સંરચના string_list exclude_patterns = STRING_LIST_INIT_NODUP;
સ્થિર પૂર્ણાંક always;
સ્થિર અચલ અક્ષર *suffix, *dirty, *broken;
સ્થિર સંરચના commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
સ્થિર અચલ અક્ષર *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", ખાલી
};

સ્થિર અચલ અક્ષર *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", ખાલી
};

સંરચના commit_name {
	સંરચના hashmap_entry entry;
	સંરચના object_id peeled;
	સંરચના tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	સંરચના object_id oid;
	અક્ષર *path;
};

સ્થિર અચલ અક્ષર *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

સ્થિર પૂર્ણાંક commit_name_neq(અચલ શૂન્ય *cmp_data UNUSED,
			   અચલ સંરચના hashmap_entry *eptr,
			   અચલ સંરચના hashmap_entry *entry_or_ચાવી,
			   અચલ શૂન્ય *peeled)
{
	અચલ સંરચના commit_name *cn1, *cn2;

	cn1 = container_of(eptr, અચલ સંરચના commit_name, entry);
	cn2 = container_of(entry_or_ચાવી, અચલ સંરચના commit_name, entry);

	પાછા !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

સ્થિર અંતઃરેખ સંરચના commit_name *find_commit_name(અચલ સંરચના object_id *peeled)
{
	પાછા hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						સંરચના commit_name, entry);
}

સ્થિર પૂર્ણાંક replace_name(સંરચના commit_name *e,
			       પૂર્ણાંક prio,
			       અચલ સંરચના object_id *oid,
			       સંરચના tag **tag)
{
	જો (!e || e->prio < prio)
		પાછા 1;

	જો (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		સંરચના tag *t;

		જો (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			જો (!t || parse_tag(the_repository, t))
				પાછા 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		જો (!t || parse_tag(the_repository, t))
			પાછા 0;
		*tag = t;

		જો (e->tag->date < t->date)
			પાછા 1;
	}

	પાછા 0;
}

સ્થિર શૂન્ય add_to_known_names(અચલ અક્ષર *path,
			       અચલ સંરચના object_id *peeled,
			       પૂર્ણાંક prio,
			       અચલ સંરચના object_id *oid)
{
	સંરચના commit_name *e = find_commit_name(peeled);
	સંરચના tag *tag = ખાલી;
	જો (replace_name(e, prio, oid, &tag)) {
		જો (!e) {
			e = xmalloc(કદ(સંરચના commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = ખાલી;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		મુક્ત(e->path);
		e->path = xstrdup(path);
	}
}

સ્થિર પૂર્ણાંક get_name(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	પૂર્ણાંક is_tag = 0;
	સંરચના object_id peeled;
	પૂર્ણાંક is_annotated, prio;
	અચલ અક્ષર *path_to_match = ખાલી;

	જો (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} નહીંતર જો (all) {
		જો ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			પાછા 0;
		}
	} નહીંતર {
		/* Reject anything outside refs/tags/ unless --all */
		પાછા 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	જો (exclude_patterns.nr) {
		સંરચના string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			જો (!wildmatch(item->string, path_to_match, 0))
				પાછા 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	જો (patterns.nr) {
		પૂર્ણાંક found = 0;
		સંરચના string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			જો (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				તોડો;
			}
		}

		જો (!found)
			પાછા 0;
	}

	/* Is it annotated? */
	જો (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} નહીંતર {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	જો (is_annotated)
		prio = 2;
	નહીંતર જો (is_tag)
		prio = 1;
	નહીંતર
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	પાછા 0;
}

સંરચના possible_tag {
	સંરચના commit_name *name;
	પૂર્ણાંક depth;
	પૂર્ણાંક found_order;
	unsigned flag_within;
};

સ્થિર પૂર્ણાંક compare_pt(અચલ શૂન્ય *a_, અચલ શૂન્ય *b_)
{
	સંરચના possible_tag *a = (સંરચના possible_tag *)a_;
	સંરચના possible_tag *b = (સંરચના possible_tag *)b_;
	જો (a->depth != b->depth)
		પાછા a->depth - b->depth;
	જો (a->found_order != b->found_order)
		પાછા a->found_order - b->found_order;
	પાછા 0;
}

સ્થિર unsigned long finish_depth_computation(સંરચના prio_queue *queue,
					      સંરચના possible_tag *best)
{
	unsigned long seen_commits = 0;
	સંરચના oidset unflagged = OIDSET_INIT;
	સંરચના commit *c;

	prio_queue_for_each(queue, c) {
		જો (!(c->object.flags & best->flag_within))
			oidset_ઉમેરો(&unflagged, &c->object.oid);
	}

	જ્યારે ((c = prio_queue_get(queue))) {
		સંરચના commit_list *parents = c->parents;
		seen_commits++;
		જો (c->object.flags & best->flag_within) {
			જો (!oidset_size(&unflagged))
				તોડો;
		} નહીંતર {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		જ્યારે (parents) {
			unsigned seen, flag_before, flag_after;
			સંરચના commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			જો (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			જો (!seen && !flag_after)
				oidset_ઉમેરો(&unflagged, &p->object.oid);
			જો (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	પાછા seen_commits;
}

સ્થિર શૂન્ય append_name(સંરચના commit_name *n, સંરચના strbuf *dst)
{
	જો (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		જો (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	જો (n->tag && !n->name_checked) {
		જો (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	જો (n->tag) {
		જો (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} નહીંતર {
		strbuf_addstr(dst, n->path);
	}
}

સ્થિર શૂન્ય append_suffix(પૂર્ણાંક depth, અચલ સંરચના object_id *oid, સંરચના strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

સ્થિર શૂન્ય describe_commit(સંરચના commit *cmit, સંરચના strbuf *dst)
{
	સંરચના commit *c, *gave_up_on = ખાલી;
	સંરચના prio_queue queue = { compare_commits_by_commit_date };
	સંરચના commit_name *n;
	સંરચના possible_tag all_matches[MAX_TAGS];
	unsigned પૂર્ણાંક match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned પૂર્ણાંક unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	જો (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		જો (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		જો (suffix)
			strbuf_addstr(dst, suffix);
		પાછા;
	}

	જો (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	જો (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	જો (!have_util) {
		સંરચના hashmap_iter iter;
		સંરચના commit *c;
		સંરચના commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			જો (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	જ્યારે ((c = prio_queue_get(&queue))) {
		સંરચના commit_list *parents = c->parents;
		સંરચના commit_name **slot;

		seen_commits++;

		જો (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			તોડો;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : ખાલી;
		જો (n) {
			જો (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} નહીંતર જો (match_cnt < max_candidates) {
				સંરચના possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				જો (n->prio == 2)
					annotated_cnt++;
			}
		}
		માટે (cur_match = 0; cur_match < match_cnt; cur_match++) {
			સંરચના possible_tag *t = &all_matches[cur_match];
			જો (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		જો (annotated_cnt && !prio_queue_size(&queue)) {
			પૂર્ણાંક best_depth = INT_MAX;
			unsigned best_within = 0;
			માટે (cur_match = 0; cur_match < match_cnt; cur_match++) {
				સંરચના possible_tag *t = &all_matches[cur_match];
				જો (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} નહીંતર જો (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			જો ((c->object.flags & best_within) == best_within) {
				જો (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				તોડો;
			}
		}
		જ્યારે (parents) {
			સંરચના commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			જો (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			જો (first_parent)
				તોડો;
		}
	}

	જો (!match_cnt) {
		સંરચના object_id *cmit_oid = &cmit->object.oid;
		જો (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			જો (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			પાછા;
		}
		જો (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		નહીંતર
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	જો (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	જો (debug) {
		સ્થિર પૂર્ણાંક label_width = -1;
		જો (label_width < 0) {
			પૂર્ણાંક i, w;
			માટે (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				જો (label_width < w)
					label_width = w;
			}
		}
		માટે (cur_match = 0; cur_match < match_cnt; cur_match++) {
			સંરચના possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		જો (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	જો (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	જો (suffix)
		strbuf_addstr(dst, suffix);
}

સંરચના process_commit_data {
	સંરચના commit *current_commit;
	અચલ સંરચના object_id *looking_for;
	સંરચના strbuf *dst;
	સંરચના rev_info *revs;
};

સ્થિર શૂન્ય process_commit(સંરચના commit *commit, શૂન્ય *data)
{
	સંરચના process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

સ્થિર શૂન્ય process_object(સંરચના object *obj, અચલ અક્ષર *path, શૂન્ય *data)
{
	સંરચના process_commit_data *pcd = data;

	જો (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		જો (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ખાલી;
	}
}

સ્થિર શૂન્ય describe_blob(અચલ સંરચના object_id *oid, સંરચના strbuf *dst)
{
	સંરચના rev_info revs;
	સંરચના strvec args = STRVEC_INIT;
	સંરચના object_id head_oid;
	સંરચના process_commit_data pcd = { ખાલી, oid, dst, &revs};

	જો (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     ખાલી);

	repo_init_revisions(the_repository, &revs, ખાલી);
	setup_revisions_from_strvec(&args, &revs, ખાલી);
	જો (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	જો (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	જો (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

સ્થિર શૂન્ય describe(અચલ અક્ષર *arg, પૂર્ણાંક last_one)
{
	સંરચના object_id oid;
	સંરચના commit *cmit;
	સંરચના strbuf sb = STRBUF_INIT;

	જો (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	જો (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	જો (cmit)
		describe_commit(cmit, &sb);
	નહીંતર જો (odb_read_object_info(the_repository->objects,
				      &oid, ખાલી) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	નહીંતર
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	જો (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

સ્થિર પૂર્ણાંક option_parse_exact_match(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				    પૂર્ણાંક unset)
{
	પૂર્ણાંક *val = opt->કિંમત;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	પાછા 0;
}

પૂર્ણાંક cmd_describe(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED )
{
	સંરચના refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	પૂર્ણાંક contains = 0;
	સંરચના option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ખાલી,
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
			.કિંમત = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.કિંમત = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	જો (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	જો (max_candidates < 0)
		max_candidates = 0;
	નહીંતર જો (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	જો (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	જો (contains) {
		સંરચના string_list_item *item;
		સંરચના strvec args;
		અચલ અક્ષર **argv_copy;
		પૂર્ણાંક ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ખાલી);
		જો (always)
			strvec_ધકેલો(&args, "--always");
		જો (!all)
			strvec_ધકેલો(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		જો (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		જો (argc)
			strvec_pushv(&args, argv);
		નહીંતર
			strvec_ધકેલો(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		માટે (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = ખાલી;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		મુક્ત(argv_copy);
		પાછા ret;
	}

	જો (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, ખાલી, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ખાલી, &for_each_ref_opts);
	જો (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	જો (argc == 0) {
		જો (broken) {
			સંરચના child_process cp = CHILD_PROCESS_INIT;

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

			જો (!dirty)
				dirty = "-dirty";

			પસંદગી (run_command(&cp)) {
			સ્થિતિ 0:
				suffix = ખાલી;
				તોડો;
			સ્થિતિ 1:
				suffix = dirty;
				તોડો;
			મૂળભૂત:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} નહીંતર જો (dirty) {
			સંરચના lock_file index_lock = LOCK_INIT;
			સંરચના rev_info revs;
			પૂર્ણાંક fd;

			setup_work_વૃક્ષ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      ખાલી, ખાલી, ખાલી);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			જો (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			જો (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, ખાલી) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			જો (!diff_result_code(&revs))
				suffix = ખાલી;
			નહીંતર
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} નહીંતર જો (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} નહીંતર જો (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} નહીંતર {
		જ્યારે (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	પાછા 0;
}
