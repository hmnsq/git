#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "hashmap.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "list-objects.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "prio-queue.h"
#अन्तर्भाव "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, संरचना commit_name *);

स्थिर अचर वर्ण * अचर describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	रिक्त
};

स्थिर पूर्णाङ्क debug;	/* Display lots of verbose info */
स्थिर पूर्णाङ्क all;	/* Any valid ref can be used */
स्थिर पूर्णाङ्क tags;	/* Allow lightweight tags */
स्थिर पूर्णाङ्क longformat;
स्थिर पूर्णाङ्क first_parent;
स्थिर पूर्णाङ्क abbrev = -1; /* unspecified */
स्थिर पूर्णाङ्क max_candidates = DEFAULT_CANDIDATES;
स्थिर संरचना hashmap names;
स्थिर पूर्णाङ्क have_util;
स्थिर संरचना string_list patterns = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list exclude_patterns = STRING_LIST_INIT_NODUP;
स्थिर पूर्णाङ्क always;
स्थिर अचर वर्ण *suffix, *dirty, *broken;
स्थिर संरचना commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
स्थिर अचर वर्ण *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", रिक्त
};

स्थिर अचर वर्ण *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", रिक्त
};

संरचना commit_name {
	संरचना hashmap_entry entry;
	संरचना object_id peeled;
	संरचना tag *tag;
	अचिन्हित prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	अचिन्हित name_checked:1;
	अचिन्हित misnamed:1;
	संरचना object_id oid;
	वर्ण *path;
};

स्थिर अचर वर्ण *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

स्थिर पूर्णाङ्क commit_name_neq(अचर शून्य *cmp_data UNUSED,
			   अचर संरचना hashmap_entry *eptr,
			   अचर संरचना hashmap_entry *entry_or_साँचो,
			   अचर शून्य *peeled)
{
	अचर संरचना commit_name *cn1, *cn2;

	cn1 = container_of(eptr, अचर संरचना commit_name, entry);
	cn2 = container_of(entry_or_साँचो, अचर संरचना commit_name, entry);

	फर्काऊ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

स्थिर अन्तःस्थ संरचना commit_name *find_commit_name(अचर संरचना object_id *peeled)
{
	फर्काऊ hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						संरचना commit_name, entry);
}

स्थिर पूर्णाङ्क replace_name(संरचना commit_name *e,
			       पूर्णाङ्क prio,
			       अचर संरचना object_id *oid,
			       संरचना tag **tag)
{
	यदि (!e || e->prio < prio)
		फर्काऊ 1;

	यदि (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		संरचना tag *t;

		यदि (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			यदि (!t || parse_tag(the_repository, t))
				फर्काऊ 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		यदि (!t || parse_tag(the_repository, t))
			फर्काऊ 0;
		*tag = t;

		यदि (e->tag->date < t->date)
			फर्काऊ 1;
	}

	फर्काऊ 0;
}

स्थिर शून्य add_to_known_names(अचर वर्ण *path,
			       अचर संरचना object_id *peeled,
			       पूर्णाङ्क prio,
			       अचर संरचना object_id *oid)
{
	संरचना commit_name *e = find_commit_name(peeled);
	संरचना tag *tag = रिक्त;
	यदि (replace_name(e, prio, oid, &tag)) {
		यदि (!e) {
			e = xmalloc(परिमाण(संरचना commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = रिक्त;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		मुक्त(e->path);
		e->path = xstrdup(path);
	}
}

स्थिर पूर्णाङ्क get_name(अचर संरचना reference *ref, शून्य *cb_data UNUSED)
{
	पूर्णाङ्क is_tag = 0;
	संरचना object_id peeled;
	पूर्णाङ्क is_annotated, prio;
	अचर वर्ण *path_to_match = रिक्त;

	यदि (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} अन्यथा यदि (all) {
		यदि ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			फर्काऊ 0;
		}
	} अन्यथा {
		/* Reject anything outside refs/tags/ unless --all */
		फर्काऊ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	यदि (exclude_patterns.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			यदि (!wildmatch(item->string, path_to_match, 0))
				फर्काऊ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	यदि (patterns.nr) {
		पूर्णाङ्क found = 0;
		संरचना string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			यदि (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				रोक;
			}
		}

		यदि (!found)
			फर्काऊ 0;
	}

	/* Is it annotated? */
	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} अन्यथा {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	यदि (is_annotated)
		prio = 2;
	अन्यथा यदि (is_tag)
		prio = 1;
	अन्यथा
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	फर्काऊ 0;
}

संरचना possible_tag {
	संरचना commit_name *name;
	पूर्णाङ्क depth;
	पूर्णाङ्क found_order;
	अचिन्हित flag_within;
};

स्थिर पूर्णाङ्क compare_pt(अचर शून्य *a_, अचर शून्य *b_)
{
	संरचना possible_tag *a = (संरचना possible_tag *)a_;
	संरचना possible_tag *b = (संरचना possible_tag *)b_;
	यदि (a->depth != b->depth)
		फर्काऊ a->depth - b->depth;
	यदि (a->found_order != b->found_order)
		फर्काऊ a->found_order - b->found_order;
	फर्काऊ 0;
}

स्थिर अचिन्हित दीर्घ finish_depth_computation(संरचना prio_queue *queue,
					      संरचना possible_tag *best)
{
	अचिन्हित दीर्घ seen_commits = 0;
	संरचना oidset unflagged = OIDSET_INIT;
	संरचना commit *c;

	prio_queue_for_each(queue, c) {
		यदि (!(c->object.flags & best->flag_within))
			oidset_घुसाउ(&unflagged, &c->object.oid);
	}

	जबसम्म ((c = prio_queue_get(queue))) {
		संरचना commit_list *parents = c->parents;
		seen_commits++;
		यदि (c->object.flags & best->flag_within) {
			यदि (!oidset_size(&unflagged))
				रोक;
		} अन्यथा {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		जबसम्म (parents) {
			अचिन्हित seen, flag_before, flag_after;
			संरचना commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			यदि (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			यदि (!seen && !flag_after)
				oidset_घुसाउ(&unflagged, &p->object.oid);
			यदि (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	फर्काऊ seen_commits;
}

स्थिर शून्य append_name(संरचना commit_name *n, संरचना strbuf *dst)
{
	यदि (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		यदि (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	यदि (n->tag && !n->name_checked) {
		यदि (म_भेद(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	यदि (n->tag) {
		यदि (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} अन्यथा {
		strbuf_addstr(dst, n->path);
	}
}

स्थिर शून्य append_suffix(पूर्णाङ्क depth, अचर संरचना object_id *oid, संरचना strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

स्थिर शून्य describe_commit(संरचना commit *cmit, संरचना strbuf *dst)
{
	संरचना commit *c, *gave_up_on = रिक्त;
	संरचना prio_queue queue = { compare_commits_by_commit_date };
	संरचना commit_name *n;
	संरचना possible_tag all_matches[MAX_TAGS];
	अचिन्हित पूर्णाङ्क match_cnt = 0, annotated_cnt = 0, cur_match;
	अचिन्हित दीर्घ seen_commits = 0;
	अचिन्हित पूर्णाङ्क unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	यदि (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		यदि (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		यदि (suffix)
			strbuf_addstr(dst, suffix);
		फर्काऊ;
	}

	यदि (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	यदि (debug)
		ख_लिखो(मानक_त्रुटि, _("No exact match on refs or tags, searching to describe\n"));

	यदि (!have_util) {
		संरचना hashmap_iter iter;
		संरचना commit *c;
		संरचना commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			यदि (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	जबसम्म ((c = prio_queue_get(&queue))) {
		संरचना commit_list *parents = c->parents;
		संरचना commit_name **slot;

		seen_commits++;

		यदि (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			रोक;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : रिक्त;
		यदि (n) {
			यदि (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} अन्यथा यदि (match_cnt < max_candidates) {
				संरचना possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				यदि (n->prio == 2)
					annotated_cnt++;
			}
		}
		लागि (cur_match = 0; cur_match < match_cnt; cur_match++) {
			संरचना possible_tag *t = &all_matches[cur_match];
			यदि (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		यदि (annotated_cnt && !prio_queue_size(&queue)) {
			पूर्णाङ्क best_depth = पूर्णांक_उच्च;
			अचिन्हित best_within = 0;
			लागि (cur_match = 0; cur_match < match_cnt; cur_match++) {
				संरचना possible_tag *t = &all_matches[cur_match];
				यदि (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} अन्यथा यदि (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			यदि ((c->object.flags & best_within) == best_within) {
				यदि (debug)
					ख_लिखो(मानक_त्रुटि, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				रोक;
			}
		}
		जबसम्म (parents) {
			संरचना commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			यदि (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			यदि (first_parent)
				रोक;
		}
	}

	यदि (!match_cnt) {
		संरचना object_id *cmit_oid = &cmit->object.oid;
		यदि (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			यदि (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			फर्काऊ;
		}
		यदि (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		अन्यथा
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	यदि (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	यदि (debug) {
		स्थिर पूर्णाङ्क label_width = -1;
		यदि (label_width < 0) {
			पूर्णाङ्क i, w;
			लागि (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = म_माप(_(prio_names[i]));
				यदि (label_width < w)
					label_width = w;
			}
		}
		लागि (cur_match = 0; cur_match < match_cnt; cur_match++) {
			संरचना possible_tag *t = &all_matches[cur_match];
			ख_लिखो(मानक_त्रुटि, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		ख_लिखो(मानक_त्रुटि, _("traversed %lu commits\n"), seen_commits);
		यदि (gave_up_on) {
			ख_लिखो(मानक_त्रुटि,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	यदि (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	यदि (suffix)
		strbuf_addstr(dst, suffix);
}

संरचना process_commit_data {
	संरचना commit *current_commit;
	अचर संरचना object_id *looking_for;
	संरचना strbuf *dst;
	संरचना rev_info *revs;
};

स्थिर शून्य process_commit(संरचना commit *commit, शून्य *data)
{
	संरचना process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

स्थिर शून्य process_object(संरचना object *obj, अचर वर्ण *path, शून्य *data)
{
	संरचना process_commit_data *pcd = data;

	यदि (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		यदि (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = रिक्त;
	}
}

स्थिर शून्य describe_blob(अचर संरचना object_id *oid, संरचना strbuf *dst)
{
	संरचना rev_info revs;
	संरचना strvec args = STRVEC_INIT;
	संरचना object_id head_oid;
	संरचना process_commit_data pcd = { रिक्त, oid, dst, &revs};

	यदि (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     रिक्त);

	repo_init_revisions(the_repository, &revs, रिक्त);
	setup_revisions_from_strvec(&args, &revs, रिक्त);
	यदि (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	यदि (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	यदि (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

स्थिर शून्य describe(अचर वर्ण *arg, पूर्णाङ्क last_one)
{
	संरचना object_id oid;
	संरचना commit *cmit;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (debug)
		ख_लिखो(मानक_त्रुटि, _("describe %s\n"), arg);

	यदि (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	यदि (cmit)
		describe_commit(cmit, &sb);
	अन्यथा यदि (odb_read_object_info(the_repository->objects,
				      &oid, रिक्त) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	अन्यथा
		die(_("%s is neither a commit nor blob"), arg);

	माला_दो(sb.buf);

	यदि (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

स्थिर पूर्णाङ्क option_parse_exact_match(अचर संरचना option *opt, अचर वर्ण *arg,
				    पूर्णाङ्क unset)
{
	पूर्णाङ्क *val = opt->मान;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	फर्काऊ 0;
}

पूर्णाङ्क cmd_describe(पूर्णाङ्क argc,
		 अचर वर्ण **argv,
		 अचर वर्ण *prefix,
		 संरचना repository *repo UNUSED )
{
	संरचना refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	पूर्णाङ्क contains = 0;
	संरचना option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, रिक्त,
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
			.मान = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.मान = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	यदि (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	यदि (max_candidates < 0)
		max_candidates = 0;
	अन्यथा यदि (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	यदि (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	यदि (contains) {
		संरचना string_list_item *item;
		संरचना strvec args;
		अचर वर्ण **argv_copy;
		पूर्णाङ्क ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     रिक्त);
		यदि (always)
			strvec_थप(&args, "--always");
		यदि (!all)
			strvec_थप(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		यदि (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		यदि (argc)
			strvec_pushv(&args, argv);
		अन्यथा
			strvec_थप(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		लागि (माप_प्रकार i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = रिक्त;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		मुक्त(argv_copy);
		फर्काऊ ret;
	}

	यदि (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, रिक्त, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, रिक्त, &for_each_ref_opts);
	यदि (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	यदि (argc == 0) {
		यदि (broken) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

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

			यदि (!dirty)
				dirty = "-dirty";

			विकल्प (run_command(&cp)) {
			पक्ष 0:
				suffix = रिक्त;
				रोक;
			पक्ष 1:
				suffix = dirty;
				रोक;
			स्वभाव:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} अन्यथा यदि (dirty) {
			संरचना lock_file index_lock = LOCK_INIT;
			संरचना rev_info revs;
			पूर्णाङ्क fd;

			setup_work_रूख(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      रिक्त, रिक्त, रिक्त);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			यदि (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			यदि (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, रिक्त) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			यदि (!diff_result_code(&revs))
				suffix = रिक्त;
			अन्यथा
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} अन्यथा यदि (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} अन्यथा यदि (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} अन्यथा {
		जबसम्म (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	फर्काऊ 0;
}
