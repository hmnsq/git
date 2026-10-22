#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "hashmap.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "list-objects.h"
#അന്തര്ഭാവ "commit-slab.h"
#അന്തര്ഭാവ "wildmatch.h"
#അന്തര്ഭാവ "prio-queue.h"
#അന്തര്ഭാവ "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, ഘടന commit_name *);

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ debug;	/* Display lots of verbose info */
സ്ഥിതം പൂർണ്ണസംഖ്യ all;	/* Any valid ref can be used */
സ്ഥിതം പൂർണ്ണസംഖ്യ tags;	/* Allow lightweight tags */
സ്ഥിതം പൂർണ്ണസംഖ്യ longformat;
സ്ഥിതം പൂർണ്ണസംഖ്യ first_parent;
സ്ഥിതം പൂർണ്ണസംഖ്യ abbrev = -1; /* unspecified */
സ്ഥിതം പൂർണ്ണസംഖ്യ max_candidates = DEFAULT_CANDIDATES;
സ്ഥിതം ഘടന hashmap names;
സ്ഥിതം പൂർണ്ണസംഖ്യ have_util;
സ്ഥിതം ഘടന string_list patterns = STRING_LIST_INIT_NODUP;
സ്ഥിതം ഘടന string_list exclude_patterns = STRING_LIST_INIT_NODUP;
സ്ഥിതം പൂർണ്ണസംഖ്യ always;
സ്ഥിതം സ്ഥിരം അക്ഷരം *suffix, *dirty, *broken;
സ്ഥിതം ഘടന commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
സ്ഥിതം സ്ഥിരം അക്ഷരം *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", ശൂന്യ
};

ഘടന commit_name {
	ഘടന hashmap_entry entry;
	ഘടന object_id peeled;
	ഘടന tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	ഘടന object_id oid;
	അക്ഷരം *path;
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

സ്ഥിതം പൂർണ്ണസംഖ്യ commit_name_neq(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
			   സ്ഥിരം ഘടന hashmap_entry *eptr,
			   സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
			   സ്ഥിരം ശൂന്യം *peeled)
{
	സ്ഥിരം ഘടന commit_name *cn1, *cn2;

	cn1 = container_of(eptr, സ്ഥിരം ഘടന commit_name, entry);
	cn2 = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന commit_name, entry);

	തിരികെ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

സ്ഥിതം വരിയിൽ ഘടന commit_name *find_commit_name(സ്ഥിരം ഘടന object_id *peeled)
{
	തിരികെ hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						ഘടന commit_name, entry);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ replace_name(ഘടന commit_name *e,
			       പൂർണ്ണസംഖ്യ prio,
			       സ്ഥിരം ഘടന object_id *oid,
			       ഘടന tag **tag)
{
	എങ്കിൽ (!e || e->prio < prio)
		തിരികെ 1;

	എങ്കിൽ (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ഘടന tag *t;

		എങ്കിൽ (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			എങ്കിൽ (!t || parse_tag(the_repository, t))
				തിരികെ 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		എങ്കിൽ (!t || parse_tag(the_repository, t))
			തിരികെ 0;
		*tag = t;

		എങ്കിൽ (e->tag->date < t->date)
			തിരികെ 1;
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം add_to_known_names(സ്ഥിരം അക്ഷരം *path,
			       സ്ഥിരം ഘടന object_id *peeled,
			       പൂർണ്ണസംഖ്യ prio,
			       സ്ഥിരം ഘടന object_id *oid)
{
	ഘടന commit_name *e = find_commit_name(peeled);
	ഘടന tag *tag = ശൂന്യ;
	എങ്കിൽ (replace_name(e, prio, oid, &tag)) {
		എങ്കിൽ (!e) {
			e = xmalloc(വലുപ്പം(ഘടന commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = ശൂന്യ;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		സ്വതന്ത്രം(e->path);
		e->path = xstrdup(path);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_name(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	പൂർണ്ണസംഖ്യ is_tag = 0;
	ഘടന object_id peeled;
	പൂർണ്ണസംഖ്യ is_annotated, prio;
	സ്ഥിരം അക്ഷരം *path_to_match = ശൂന്യ;

	എങ്കിൽ (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} അല്ലെങ്കിൽ എങ്കിൽ (all) {
		എങ്കിൽ ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			തിരികെ 0;
		}
	} അല്ലെങ്കിൽ {
		/* Reject anything outside refs/tags/ unless --all */
		തിരികെ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	എങ്കിൽ (exclude_patterns.nr) {
		ഘടന string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			എങ്കിൽ (!wildmatch(item->string, path_to_match, 0))
				തിരികെ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	എങ്കിൽ (patterns.nr) {
		പൂർണ്ണസംഖ്യ found = 0;
		ഘടന string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			എങ്കിൽ (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				നിർത്തുക;
			}
		}

		എങ്കിൽ (!found)
			തിരികെ 0;
	}

	/* Is it annotated? */
	എങ്കിൽ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} അല്ലെങ്കിൽ {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	എങ്കിൽ (is_annotated)
		prio = 2;
	അല്ലെങ്കിൽ എങ്കിൽ (is_tag)
		prio = 1;
	അല്ലെങ്കിൽ
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	തിരികെ 0;
}

ഘടന possible_tag {
	ഘടന commit_name *name;
	പൂർണ്ണസംഖ്യ depth;
	പൂർണ്ണസംഖ്യ found_order;
	unsigned flag_within;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_pt(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	ഘടന possible_tag *a = (ഘടന possible_tag *)a_;
	ഘടന possible_tag *b = (ഘടന possible_tag *)b_;
	എങ്കിൽ (a->depth != b->depth)
		തിരികെ a->depth - b->depth;
	എങ്കിൽ (a->found_order != b->found_order)
		തിരികെ a->found_order - b->found_order;
	തിരികെ 0;
}

സ്ഥിതം unsigned long finish_depth_computation(ഘടന prio_queue *queue,
					      ഘടന possible_tag *best)
{
	unsigned long seen_commits = 0;
	ഘടന oidset unflagged = OIDSET_INIT;
	ഘടന commit *c;

	prio_queue_for_each(queue, c) {
		എങ്കിൽ (!(c->object.flags & best->flag_within))
			oidset_തിരുകുക(&unflagged, &c->object.oid);
	}

	ആകുമ്പോൾ ((c = prio_queue_get(queue))) {
		ഘടന commit_list *parents = c->parents;
		seen_commits++;
		എങ്കിൽ (c->object.flags & best->flag_within) {
			എങ്കിൽ (!oidset_size(&unflagged))
				നിർത്തുക;
		} അല്ലെങ്കിൽ {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		ആകുമ്പോൾ (parents) {
			unsigned seen, flag_before, flag_after;
			ഘടന commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			എങ്കിൽ (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			എങ്കിൽ (!seen && !flag_after)
				oidset_തിരുകുക(&unflagged, &p->object.oid);
			എങ്കിൽ (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	തിരികെ seen_commits;
}

സ്ഥിതം ശൂന്യം append_name(ഘടന commit_name *n, ഘടന strbuf *dst)
{
	എങ്കിൽ (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		എങ്കിൽ (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	എങ്കിൽ (n->tag && !n->name_checked) {
		എങ്കിൽ (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	എങ്കിൽ (n->tag) {
		എങ്കിൽ (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} അല്ലെങ്കിൽ {
		strbuf_addstr(dst, n->path);
	}
}

സ്ഥിതം ശൂന്യം append_suffix(പൂർണ്ണസംഖ്യ depth, സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

സ്ഥിതം ശൂന്യം describe_commit(ഘടന commit *cmit, ഘടന strbuf *dst)
{
	ഘടന commit *c, *gave_up_on = ശൂന്യ;
	ഘടന prio_queue queue = { compare_commits_by_commit_date };
	ഘടന commit_name *n;
	ഘടന possible_tag all_matches[MAX_TAGS];
	unsigned പൂർണ്ണസംഖ്യ match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned പൂർണ്ണസംഖ്യ unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	എങ്കിൽ (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		എങ്കിൽ (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		എങ്കിൽ (suffix)
			strbuf_addstr(dst, suffix);
		തിരികെ;
	}

	എങ്കിൽ (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	എങ്കിൽ (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	എങ്കിൽ (!have_util) {
		ഘടന hashmap_iter iter;
		ഘടന commit *c;
		ഘടന commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			എങ്കിൽ (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	ആകുമ്പോൾ ((c = prio_queue_get(&queue))) {
		ഘടന commit_list *parents = c->parents;
		ഘടന commit_name **slot;

		seen_commits++;

		എങ്കിൽ (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			നിർത്തുക;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : ശൂന്യ;
		എങ്കിൽ (n) {
			എങ്കിൽ (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} അല്ലെങ്കിൽ എങ്കിൽ (match_cnt < max_candidates) {
				ഘടന possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				എങ്കിൽ (n->prio == 2)
					annotated_cnt++;
			}
		}
		വേണ്ടി (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ഘടന possible_tag *t = &all_matches[cur_match];
			എങ്കിൽ (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		എങ്കിൽ (annotated_cnt && !prio_queue_size(&queue)) {
			പൂർണ്ണസംഖ്യ best_depth = INT_MAX;
			unsigned best_within = 0;
			വേണ്ടി (cur_match = 0; cur_match < match_cnt; cur_match++) {
				ഘടന possible_tag *t = &all_matches[cur_match];
				എങ്കിൽ (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} അല്ലെങ്കിൽ എങ്കിൽ (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			എങ്കിൽ ((c->object.flags & best_within) == best_within) {
				എങ്കിൽ (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				നിർത്തുക;
			}
		}
		ആകുമ്പോൾ (parents) {
			ഘടന commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			എങ്കിൽ (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			എങ്കിൽ (first_parent)
				നിർത്തുക;
		}
	}

	എങ്കിൽ (!match_cnt) {
		ഘടന object_id *cmit_oid = &cmit->object.oid;
		എങ്കിൽ (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			എങ്കിൽ (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			തിരികെ;
		}
		എങ്കിൽ (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		അല്ലെങ്കിൽ
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	എങ്കിൽ (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	എങ്കിൽ (debug) {
		സ്ഥിതം പൂർണ്ണസംഖ്യ label_width = -1;
		എങ്കിൽ (label_width < 0) {
			പൂർണ്ണസംഖ്യ i, w;
			വേണ്ടി (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				എങ്കിൽ (label_width < w)
					label_width = w;
			}
		}
		വേണ്ടി (cur_match = 0; cur_match < match_cnt; cur_match++) {
			ഘടന possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		എങ്കിൽ (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	എങ്കിൽ (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	എങ്കിൽ (suffix)
		strbuf_addstr(dst, suffix);
}

ഘടന process_commit_data {
	ഘടന commit *current_commit;
	സ്ഥിരം ഘടന object_id *looking_for;
	ഘടന strbuf *dst;
	ഘടന rev_info *revs;
};

സ്ഥിതം ശൂന്യം process_commit(ഘടന commit *commit, ശൂന്യം *data)
{
	ഘടന process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

സ്ഥിതം ശൂന്യം process_object(ഘടന object *obj, സ്ഥിരം അക്ഷരം *path, ശൂന്യം *data)
{
	ഘടന process_commit_data *pcd = data;

	എങ്കിൽ (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		എങ്കിൽ (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ശൂന്യ;
	}
}

സ്ഥിതം ശൂന്യം describe_blob(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *dst)
{
	ഘടന rev_info revs;
	ഘടന strvec args = STRVEC_INIT;
	ഘടന object_id head_oid;
	ഘടന process_commit_data pcd = { ശൂന്യ, oid, dst, &revs};

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     ശൂന്യ);

	repo_init_revisions(the_repository, &revs, ശൂന്യ);
	setup_revisions_from_strvec(&args, &revs, ശൂന്യ);
	എങ്കിൽ (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	എങ്കിൽ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	എങ്കിൽ (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

സ്ഥിതം ശൂന്യം describe(സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ last_one)
{
	ഘടന object_id oid;
	ഘടന commit *cmit;
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	എങ്കിൽ (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	എങ്കിൽ (cmit)
		describe_commit(cmit, &sb);
	അല്ലെങ്കിൽ എങ്കിൽ (odb_read_object_info(the_repository->objects,
				      &oid, ശൂന്യ) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	അല്ലെങ്കിൽ
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	എങ്കിൽ (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_exact_match(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				    പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *val = opt->മൂല്യം;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_describe(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED )
{
	ഘടന refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	പൂർണ്ണസംഖ്യ contains = 0;
	ഘടന option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ശൂന്യ,
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
			.മൂല്യം = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.മൂല്യം = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	എങ്കിൽ (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	എങ്കിൽ (max_candidates < 0)
		max_candidates = 0;
	അല്ലെങ്കിൽ എങ്കിൽ (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	എങ്കിൽ (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	എങ്കിൽ (contains) {
		ഘടന string_list_item *item;
		ഘടന strvec args;
		സ്ഥിരം അക്ഷരം **argv_copy;
		പൂർണ്ണസംഖ്യ ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ശൂന്യ);
		എങ്കിൽ (always)
			strvec_തള്ളുക(&args, "--always");
		എങ്കിൽ (!all)
			strvec_തള്ളുക(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		എങ്കിൽ (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		എങ്കിൽ (argc)
			strvec_pushv(&args, argv);
		അല്ലെങ്കിൽ
			strvec_തള്ളുക(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		വേണ്ടി (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = ശൂന്യ;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		സ്വതന്ത്രം(argv_copy);
		തിരികെ ret;
	}

	എങ്കിൽ (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, ശൂന്യ, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ശൂന്യ, &for_each_ref_opts);
	എങ്കിൽ (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	എങ്കിൽ (argc == 0) {
		എങ്കിൽ (broken) {
			ഘടന child_process cp = CHILD_PROCESS_INIT;

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

			എങ്കിൽ (!dirty)
				dirty = "-dirty";

			തിരഞ്ഞെടുപ്പ് (run_command(&cp)) {
			അവസ്ഥ 0:
				suffix = ശൂന്യ;
				നിർത്തുക;
			അവസ്ഥ 1:
				suffix = dirty;
				നിർത്തുക;
			സ്ഥിരസ്ഥിതി:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (dirty) {
			ഘടന lock_file index_lock = LOCK_INIT;
			ഘടന rev_info revs;
			പൂർണ്ണസംഖ്യ fd;

			setup_work_വൃക്ഷം(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      ശൂന്യ, ശൂന്യ, ശൂന്യ);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			എങ്കിൽ (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			എങ്കിൽ (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, ശൂന്യ) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			എങ്കിൽ (!diff_result_code(&revs))
				suffix = ശൂന്യ;
			അല്ലെങ്കിൽ
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} അല്ലെങ്കിൽ എങ്കിൽ (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} അല്ലെങ്കിൽ എങ്കിൽ (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} അല്ലെങ്കിൽ {
		ആകുമ്പോൾ (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	തിരികെ 0;
}
