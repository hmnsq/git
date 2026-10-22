#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "lockfile.h"
#समावेश "commit.h"
#समावेश "tag.h"
#समावेश "refs.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "read-cache-ll.h"
#समावेश "revision.h"
#समावेश "diff.h"
#समावेश "hashmap.h"
#समावेश "setup.h"
#समावेश "strvec.h"
#समावेश "run-command.h"
#समावेश "odb.h"
#समावेश "list-objects.h"
#समावेश "commit-slab.h"
#समावेश "wildmatch.h"
#समावेश "prio-queue.h"
#समावेश "oidset.h"

#घोषणा MAX_TAGS	(FLAG_BITS - 1)
#घोषणा DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, काष्ठा commit_name *);

अटल स्थिर अक्षर * स्थिर describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	शून्य
};

अटल पूर्णांक debug;	/* Display lots of verbose info */
अटल पूर्णांक all;	/* Any valid ref can be used */
अटल पूर्णांक tags;	/* Allow lightweight tags */
अटल पूर्णांक longformat;
अटल पूर्णांक first_parent;
अटल पूर्णांक abbrev = -1; /* unspecified */
अटल पूर्णांक max_candidates = DEFAULT_CANDIDATES;
अटल काष्ठा hashmap names;
अटल पूर्णांक have_util;
अटल काष्ठा string_list patterns = STRING_LIST_INIT_NODUP;
अटल काष्ठा string_list exclude_patterns = STRING_LIST_INIT_NODUP;
अटल पूर्णांक always;
अटल स्थिर अक्षर *suffix, *dirty, *broken;
अटल काष्ठा commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
अटल स्थिर अक्षर *diff_सूचकांक_args[] = {
	"diff-index", "--quiet", "HEAD", "--", शून्य
};

अटल स्थिर अक्षर *update_सूचकांक_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", शून्य
};

काष्ठा commit_name {
	काष्ठा hashmap_entry entry;
	काष्ठा object_id peeled;
	काष्ठा tag *tag;
	अचिन्हित prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	अचिन्हित name_checked:1;
	अचिन्हित misnamed:1;
	काष्ठा object_id oid;
	अक्षर *path;
};

अटल स्थिर अक्षर *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

अटल पूर्णांक commit_name_neq(स्थिर व्योम *cmp_तथ्य UNUSED,
			   स्थिर काष्ठा hashmap_entry *eptr,
			   स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
			   स्थिर व्योम *peeled)
{
	स्थिर काष्ठा commit_name *cn1, *cn2;

	cn1 = container_of(eptr, स्थिर काष्ठा commit_name, entry);
	cn2 = container_of(entry_or_कुंजी, स्थिर काष्ठा commit_name, entry);

	वापस !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

अटल अंतरभूत काष्ठा commit_name *find_commit_name(स्थिर काष्ठा object_id *peeled)
{
	वापस hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						काष्ठा commit_name, entry);
}

अटल पूर्णांक replace_name(काष्ठा commit_name *e,
			       पूर्णांक prio,
			       स्थिर काष्ठा object_id *oid,
			       काष्ठा tag **tag)
{
	अगर (!e || e->prio < prio)
		वापस 1;

	अगर (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		काष्ठा tag *t;

		अगर (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			अगर (!t || parse_tag(the_repository, t))
				वापस 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		अगर (!t || parse_tag(the_repository, t))
			वापस 0;
		*tag = t;

		अगर (e->tag->date < t->date)
			वापस 1;
	}

	वापस 0;
}

अटल व्योम add_to_known_names(स्थिर अक्षर *path,
			       स्थिर काष्ठा object_id *peeled,
			       पूर्णांक prio,
			       स्थिर काष्ठा object_id *oid)
{
	काष्ठा commit_name *e = find_commit_name(peeled);
	काष्ठा tag *tag = शून्य;
	अगर (replace_name(e, prio, oid, &tag)) {
		अगर (!e) {
			e = xmalloc(माप(काष्ठा commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = शून्य;
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

अटल पूर्णांक get_name(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	पूर्णांक is_tag = 0;
	काष्ठा object_id peeled;
	पूर्णांक is_annotated, prio;
	स्थिर अक्षर *path_to_match = शून्य;

	अगर (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} अन्यथा अगर (all) {
		अगर ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			वापस 0;
		}
	} अन्यथा {
		/* Reject anything outside refs/tags/ unless --all */
		वापस 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	अगर (exclude_patterns.nr) {
		काष्ठा string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			अगर (!wildmatch(item->string, path_to_match, 0))
				वापस 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	अगर (patterns.nr) {
		पूर्णांक found = 0;
		काष्ठा string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			अगर (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				अवरोध;
			}
		}

		अगर (!found)
			वापस 0;
	}

	/* Is it annotated? */
	अगर (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
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
	अगर (is_annotated)
		prio = 2;
	अन्यथा अगर (is_tag)
		prio = 1;
	अन्यथा
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	वापस 0;
}

काष्ठा possible_tag {
	काष्ठा commit_name *name;
	पूर्णांक depth;
	पूर्णांक found_order;
	अचिन्हित flag_within;
};

अटल पूर्णांक compare_pt(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	काष्ठा possible_tag *a = (काष्ठा possible_tag *)a_;
	काष्ठा possible_tag *b = (काष्ठा possible_tag *)b_;
	अगर (a->depth != b->depth)
		वापस a->depth - b->depth;
	अगर (a->found_order != b->found_order)
		वापस a->found_order - b->found_order;
	वापस 0;
}

अटल अचिन्हित दीर्घ finish_depth_computation(काष्ठा prio_queue *queue,
					      काष्ठा possible_tag *best)
{
	अचिन्हित दीर्घ seen_commits = 0;
	काष्ठा oidset unflagged = OIDSET_INIT;
	काष्ठा commit *c;

	prio_queue_for_each(queue, c) {
		अगर (!(c->object.flags & best->flag_within))
			oidset_जोड़ो(&unflagged, &c->object.oid);
	}

	जबतक ((c = prio_queue_get(queue))) {
		काष्ठा commit_list *parents = c->parents;
		seen_commits++;
		अगर (c->object.flags & best->flag_within) {
			अगर (!oidset_आकार(&unflagged))
				अवरोध;
		} अन्यथा {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		जबतक (parents) {
			अचिन्हित seen, flag_before, flag_after;
			काष्ठा commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			अगर (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			अगर (!seen && !flag_after)
				oidset_जोड़ो(&unflagged, &p->object.oid);
			अगर (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->अग्र;
		}
	}
	oidset_clear(&unflagged);
	वापस seen_commits;
}

अटल व्योम append_name(काष्ठा commit_name *n, काष्ठा strbuf *dst)
{
	अगर (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		अगर (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	अगर (n->tag && !n->name_checked) {
		अगर (म_भेद(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	अगर (n->tag) {
		अगर (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} अन्यथा {
		strbuf_addstr(dst, n->path);
	}
}

अटल व्योम append_suffix(पूर्णांक depth, स्थिर काष्ठा object_id *oid, काष्ठा strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

अटल व्योम describe_commit(काष्ठा commit *cmit, काष्ठा strbuf *dst)
{
	काष्ठा commit *c, *gave_up_on = शून्य;
	काष्ठा prio_queue queue = { compare_commits_by_commit_date };
	काष्ठा commit_name *n;
	काष्ठा possible_tag all_matches[MAX_TAGS];
	अचिन्हित पूर्णांक match_cnt = 0, annotated_cnt = 0, cur_match;
	अचिन्हित दीर्घ seen_commits = 0;
	अचिन्हित पूर्णांक unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	अगर (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		अगर (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		अगर (suffix)
			strbuf_addstr(dst, suffix);
		वापस;
	}

	अगर (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	अगर (debug)
		ख_लिखो(मानक_त्रुटि, _("No exact match on refs or tags, searching to describe\n"));

	अगर (!have_util) {
		काष्ठा hashmap_iter iter;
		काष्ठा commit *c;
		काष्ठा commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			अगर (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	जबतक ((c = prio_queue_get(&queue))) {
		काष्ठा commit_list *parents = c->parents;
		काष्ठा commit_name **slot;

		seen_commits++;

		अगर (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_आकार(&names)) {
			gave_up_on = c;
			अवरोध;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : शून्य;
		अगर (n) {
			अगर (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} अन्यथा अगर (match_cnt < max_candidates) {
				काष्ठा possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				अगर (n->prio == 2)
					annotated_cnt++;
			}
		}
		क्रम (cur_match = 0; cur_match < match_cnt; cur_match++) {
			काष्ठा possible_tag *t = &all_matches[cur_match];
			अगर (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		अगर (annotated_cnt && !prio_queue_आकार(&queue)) {
			पूर्णांक best_depth = पूर्णांक_उच्च;
			अचिन्हित best_within = 0;
			क्रम (cur_match = 0; cur_match < match_cnt; cur_match++) {
				काष्ठा possible_tag *t = &all_matches[cur_match];
				अगर (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} अन्यथा अगर (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			अगर ((c->object.flags & best_within) == best_within) {
				अगर (debug)
					ख_लिखो(मानक_त्रुटि, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				अवरोध;
			}
		}
		जबतक (parents) {
			काष्ठा commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			अगर (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->अग्र;

			अगर (first_parent)
				अवरोध;
		}
	}

	अगर (!match_cnt) {
		काष्ठा object_id *cmit_oid = &cmit->object.oid;
		अगर (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			अगर (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			वापस;
		}
		अगर (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		अन्यथा
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	अगर (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	अगर (debug) {
		अटल पूर्णांक label_width = -1;
		अगर (label_width < 0) {
			पूर्णांक i, w;
			क्रम (i = 0; i < ARRAY_आकार(prio_names); i++) {
				w = म_माप(_(prio_names[i]));
				अगर (label_width < w)
					label_width = w;
			}
		}
		क्रम (cur_match = 0; cur_match < match_cnt; cur_match++) {
			काष्ठा possible_tag *t = &all_matches[cur_match];
			ख_लिखो(मानक_त्रुटि, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		ख_लिखो(मानक_त्रुटि, _("traversed %lu commits\n"), seen_commits);
		अगर (gave_up_on) {
			ख_लिखो(मानक_त्रुटि,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	अगर (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	अगर (suffix)
		strbuf_addstr(dst, suffix);
}

काष्ठा process_commit_तथ्य {
	काष्ठा commit *current_commit;
	स्थिर काष्ठा object_id *looking_for;
	काष्ठा strbuf *dst;
	काष्ठा rev_info *revs;
};

अटल व्योम process_commit(काष्ठा commit *commit, व्योम *तथ्य)
{
	काष्ठा process_commit_तथ्य *pcd = तथ्य;
	pcd->current_commit = commit;
}

अटल व्योम process_object(काष्ठा object *obj, स्थिर अक्षर *path, व्योम *तथ्य)
{
	काष्ठा process_commit_तथ्य *pcd = तथ्य;

	अगर (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		अगर (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = शून्य;
	}
}

अटल व्योम describe_blob(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *dst)
{
	काष्ठा rev_info revs;
	काष्ठा strvec args = STRVEC_INIT;
	काष्ठा object_id शीर्ष_oid;
	काष्ठा process_commit_तथ्य pcd = { शून्य, oid, dst, &revs};

	अगर (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&शीर्ष_oid),
		     शून्य);

	repo_init_revisions(the_repository, &revs, शून्य);
	setup_revisions_from_strvec(&args, &revs, शून्य);
	अगर (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	अगर (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	अगर (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

अटल व्योम describe(स्थिर अक्षर *arg, पूर्णांक last_one)
{
	काष्ठा object_id oid;
	काष्ठा commit *cmit;
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (debug)
		ख_लिखो(मानक_त्रुटि, _("describe %s\n"), arg);

	अगर (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	अगर (cmit)
		describe_commit(cmit, &sb);
	अन्यथा अगर (odb_read_object_info(the_repository->objects,
				      &oid, शून्य) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	अन्यथा
		die(_("%s is neither a commit nor blob"), arg);

	माला_दो(sb.buf);

	अगर (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

अटल पूर्णांक option_parse_exact_match(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				    पूर्णांक unset)
{
	पूर्णांक *val = opt->मूल्य;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	वापस 0;
}

पूर्णांक cmd_describe(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED )
{
	काष्ठा refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	पूर्णांक contains = 0;
	काष्ठा option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, शून्य,
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
			.मूल्य = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.मूल्य = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	अगर (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	अगर (max_candidates < 0)
		max_candidates = 0;
	अन्यथा अगर (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_बफर = 0;

	अगर (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	अगर (contains) {
		काष्ठा string_list_item *item;
		काष्ठा strvec args;
		स्थिर अक्षर **argv_copy;
		पूर्णांक ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     शून्य);
		अगर (always)
			strvec_धकेलो(&args, "--always");
		अगर (!all)
			strvec_धकेलो(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		अगर (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		अगर (argc)
			strvec_pushv(&args, argv);
		अन्यथा
			strvec_धकेलो(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		क्रम (माप_प्रकार i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = शून्य;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		मुक्त(argv_copy);
		वापस ret;
	}

	अगर (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, शून्य, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, शून्य, &for_each_ref_opts);
	अगर (!hashmap_get_आकार(&names) && !always)
		die(_("No names found, cannot describe anything."));

	अगर (argc == 0) {
		अगर (broken) {
			काष्ठा child_process cp = CHILD_PROCESS_INIT;

			strvec_pushv(&cp.args, update_सूचकांक_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			run_command(&cp);

			child_process_init(&cp);
			strvec_pushv(&cp.args, diff_सूचकांक_args);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			अगर (!dirty)
				dirty = "-dirty";

			चयन (run_command(&cp)) {
			हाल 0:
				suffix = शून्य;
				अवरोध;
			हाल 1:
				suffix = dirty;
				अवरोध;
			शेष:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} अन्यथा अगर (dirty) {
			काष्ठा lock_file सूचकांक_lock = LOCK_INIT;
			काष्ठा rev_info revs;
			पूर्णांक fd;

			setup_work_वृक्ष(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_सूचकांक = 0;
			repo_read_सूचकांक(the_repository);
			refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET|REFRESH_UNMERGED,
				      शून्य, शून्य, शून्य);
			fd = repo_hold_locked_सूचकांक(the_repository,
						    &सूचकांक_lock, 0);
			अगर (0 <= fd)
				repo_update_सूचकांक_if_able(the_repository, &सूचकांक_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			अगर (setup_revisions(ARRAY_आकार(diff_सूचकांक_args) - 1,
					    diff_सूचकांक_args, &revs, शून्य) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_सूचकांक(&revs, 0);

			अगर (!diff_परिणाम_code(&revs))
				suffix = शून्य;
			अन्यथा
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} अन्यथा अगर (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} अन्यथा अगर (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} अन्यथा {
		जबतक (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	वापस 0;
}
