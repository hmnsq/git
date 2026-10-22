#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "hashmap.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "list-objects.h"
#அந்தர்பாவ "commit-slab.h"
#அந்தர்பாவ "wildmatch.h"
#அந்தர்பாவ "prio-queue.h"
#அந்தர்பாவ "oidset.h"

#define MAX_TAGS	(FLAG_BITS - 1)
#define DEFAULT_CANDIDATES 10

define_commit_slab(commit_names, கட்டமைப்பு commit_name *);

நிலையான மாறிலி எழுத்து * மாறிலி describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	வெற்று
};

நிலையான முழு debug;	/* Display lots of verbose info */
நிலையான முழு all;	/* Any valid ref can be used */
நிலையான முழு tags;	/* Allow lightweight tags */
நிலையான முழு longformat;
நிலையான முழு first_parent;
நிலையான முழு abbrev = -1; /* unspecified */
நிலையான முழு max_candidates = DEFAULT_CANDIDATES;
நிலையான கட்டமைப்பு hashmap names;
நிலையான முழு have_util;
நிலையான கட்டமைப்பு string_list patterns = STRING_LIST_INIT_NODUP;
நிலையான கட்டமைப்பு string_list exclude_patterns = STRING_LIST_INIT_NODUP;
நிலையான முழு always;
நிலையான மாறிலி எழுத்து *suffix, *dirty, *broken;
நிலையான கட்டமைப்பு commit_names commit_names;

/* diff-index command arguments to check if working tree is dirty. */
நிலையான மாறிலி எழுத்து *diff_index_args[] = {
	"diff-index", "--quiet", "HEAD", "--", வெற்று
};

நிலையான மாறிலி எழுத்து *update_index_args[] = {
	"update-index", "--unmerged", "-q", "--refresh", வெற்று
};

கட்டமைப்பு commit_name {
	கட்டமைப்பு hashmap_entry entry;
	கட்டமைப்பு object_id peeled;
	கட்டமைப்பு tag *tag;
	unsigned prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	unsigned name_checked:1;
	unsigned misnamed:1;
	கட்டமைப்பு object_id oid;
	எழுத்து *path;
};

நிலையான மாறிலி எழுத்து *prio_names[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

நிலையான முழு commit_name_neq(மாறிலி வெறுமை *cmp_data UNUSED,
			   மாறிலி கட்டமைப்பு hashmap_entry *eptr,
			   மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
			   மாறிலி வெறுமை *peeled)
{
	மாறிலி கட்டமைப்பு commit_name *cn1, *cn2;

	cn1 = container_of(eptr, மாறிலி கட்டமைப்பு commit_name, entry);
	cn2 = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு commit_name, entry);

	திரும்பு !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

நிலையான உள்வரி கட்டமைப்பு commit_name *find_commit_name(மாறிலி கட்டமைப்பு object_id *peeled)
{
	திரும்பு hashmap_get_entry_from_hash(&names, oidhash(peeled), peeled,
						கட்டமைப்பு commit_name, entry);
}

நிலையான முழு replace_name(கட்டமைப்பு commit_name *e,
			       முழு prio,
			       மாறிலி கட்டமைப்பு object_id *oid,
			       கட்டமைப்பு tag **tag)
{
	எனில் (!e || e->prio < prio)
		திரும்பு 1;

	எனில் (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		கட்டமைப்பு tag *t;

		எனில் (!e->tag) {
			t = lookup_tag(the_repository, &e->oid);
			எனில் (!t || parse_tag(the_repository, t))
				திரும்பு 1;
			e->tag = t;
		}

		t = lookup_tag(the_repository, oid);
		எனில் (!t || parse_tag(the_repository, t))
			திரும்பு 0;
		*tag = t;

		எனில் (e->tag->date < t->date)
			திரும்பு 1;
	}

	திரும்பு 0;
}

நிலையான வெறுமை add_to_known_names(மாறிலி எழுத்து *path,
			       மாறிலி கட்டமைப்பு object_id *peeled,
			       முழு prio,
			       மாறிலி கட்டமைப்பு object_id *oid)
{
	கட்டமைப்பு commit_name *e = find_commit_name(peeled);
	கட்டமைப்பு tag *tag = வெற்று;
	எனில் (replace_name(e, prio, oid, &tag)) {
		எனில் (!e) {
			e = xmalloc(அளவு(கட்டமைப்பு commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_entry_init(&e->entry, oidhash(peeled));
			hashmap_add(&names, &e->entry);
			e->path = வெற்று;
		}
		e->tag = tag;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		விடுவி(e->path);
		e->path = xstrdup(path);
	}
}

நிலையான முழு get_name(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	முழு is_tag = 0;
	கட்டமைப்பு object_id peeled;
	முழு is_annotated, prio;
	மாறிலி எழுத்து *path_to_match = வெற்று;

	எனில் (skip_prefix(ref->name, "refs/tags/", &path_to_match)) {
		is_tag = 1;
	} இல்லையெனில் எனில் (all) {
		எனில் ((exclude_patterns.nr || patterns.nr) &&
		    !skip_prefix(ref->name, "refs/heads/", &path_to_match) &&
		    !skip_prefix(ref->name, "refs/remotes/", &path_to_match)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			திரும்பு 0;
		}
	} இல்லையெனில் {
		/* Reject anything outside refs/tags/ unless --all */
		திரும்பு 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	எனில் (exclude_patterns.nr) {
		கட்டமைப்பு string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			எனில் (!wildmatch(item->string, path_to_match, 0))
				திரும்பு 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	எனில் (patterns.nr) {
		முழு found = 0;
		கட்டமைப்பு string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			எனில் (!wildmatch(item->string, path_to_match, 0)) {
				found = 1;
				நிறுத்து;
			}
		}

		எனில் (!found)
			திரும்பு 0;
	}

	/* Is it annotated? */
	எனில் (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} இல்லையெனில் {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	எனில் (is_annotated)
		prio = 2;
	இல்லையெனில் எனில் (is_tag)
		prio = 1;
	இல்லையெனில்
		prio = 0;

	add_to_known_names(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	திரும்பு 0;
}

கட்டமைப்பு possible_tag {
	கட்டமைப்பு commit_name *name;
	முழு depth;
	முழு found_order;
	unsigned flag_within;
};

நிலையான முழு compare_pt(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	கட்டமைப்பு possible_tag *a = (கட்டமைப்பு possible_tag *)a_;
	கட்டமைப்பு possible_tag *b = (கட்டமைப்பு possible_tag *)b_;
	எனில் (a->depth != b->depth)
		திரும்பு a->depth - b->depth;
	எனில் (a->found_order != b->found_order)
		திரும்பு a->found_order - b->found_order;
	திரும்பு 0;
}

நிலையான unsigned long finish_depth_computation(கட்டமைப்பு prio_queue *queue,
					      கட்டமைப்பு possible_tag *best)
{
	unsigned long seen_commits = 0;
	கட்டமைப்பு oidset unflagged = OIDSET_INIT;
	கட்டமைப்பு commit *c;

	prio_queue_for_each(queue, c) {
		எனில் (!(c->object.flags & best->flag_within))
			oidset_செருகு(&unflagged, &c->object.oid);
	}

	போது ((c = prio_queue_get(queue))) {
		கட்டமைப்பு commit_list *parents = c->parents;
		seen_commits++;
		எனில் (c->object.flags & best->flag_within) {
			எனில் (!oidset_size(&unflagged))
				நிறுத்து;
		} இல்லையெனில் {
			oidset_remove(&unflagged, &c->object.oid);
			best->depth++;
		}
		போது (parents) {
			unsigned seen, flag_before, flag_after;
			கட்டமைப்பு commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			seen = p->object.flags & SEEN;
			எனில் (!seen)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			எனில் (!seen && !flag_after)
				oidset_செருகு(&unflagged, &p->object.oid);
			எனில் (seen && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->next;
		}
	}
	oidset_clear(&unflagged);
	திரும்பு seen_commits;
}

நிலையான வெறுமை append_name(கட்டமைப்பு commit_name *n, கட்டமைப்பு strbuf *dst)
{
	எனில் (n->prio == 2 && !n->tag) {
		n->tag = lookup_tag(the_repository, &n->oid);
		எனில் (!n->tag || parse_tag(the_repository, n->tag))
			die(_("annotated tag %s not available"), n->path);
	}
	எனில் (n->tag && !n->name_checked) {
		எனில் (strcmp(n->tag->tag, all ? n->path + 5 : n->path)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->path, n->tag->tag);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	எனில் (n->tag) {
		எனில் (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->tag->tag);
	} இல்லையெனில் {
		strbuf_addstr(dst, n->path);
	}
}

நிலையான வெறுமை append_suffix(முழு depth, மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", depth,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

நிலையான வெறுமை describe_commit(கட்டமைப்பு commit *cmit, கட்டமைப்பு strbuf *dst)
{
	கட்டமைப்பு commit *c, *gave_up_on = வெற்று;
	கட்டமைப்பு prio_queue queue = { compare_commits_by_commit_date };
	கட்டமைப்பு commit_name *n;
	கட்டமைப்பு possible_tag all_matches[MAX_TAGS];
	unsigned முழு match_cnt = 0, annotated_cnt = 0, cur_match;
	unsigned long seen_commits = 0;
	unsigned முழு unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	எனில் (n && (tags || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		எனில் (n->misnamed || longformat)
			append_suffix(0, n->tag ? get_tagged_oid(n->tag) : &cmit->object.oid, dst);
		எனில் (suffix)
			strbuf_addstr(dst, suffix);
		திரும்பு;
	}

	எனில் (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_to_hex(&cmit->object.oid));
	எனில் (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	எனில் (!have_util) {
		கட்டமைப்பு hashmap_iter iter;
		கட்டமைப்பு commit *c;
		கட்டமைப்பு commit_name *n;

		init_commit_names(&commit_names);
		hashmap_for_each_entry(&names, &iter, n,
					entry /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			எனில் (c)
				*commit_names_at(&commit_names, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = SEEN;
	prio_queue_put(&queue, cmit);
	போது ((c = prio_queue_get(&queue))) {
		கட்டமைப்பு commit_list *parents = c->parents;
		கட்டமைப்பு commit_name **slot;

		seen_commits++;

		எனில் (match_cnt == max_candidates ||
		    match_cnt == hashmap_get_size(&names)) {
			gave_up_on = c;
			நிறுத்து;
		}

		slot = commit_names_peek(&commit_names, c);
		n = slot ? *slot : வெற்று;
		எனில் (n) {
			எனில் (!tags && !all && n->prio < 2) {
				unannotated_cnt++;
			} இல்லையெனில் எனில் (match_cnt < max_candidates) {
				கட்டமைப்பு possible_tag *t = &all_matches[match_cnt++];
				t->name = n;
				t->depth = seen_commits - 1;
				t->flag_within = 1u << match_cnt;
				t->found_order = match_cnt;
				c->object.flags |= t->flag_within;
				எனில் (n->prio == 2)
					annotated_cnt++;
			}
		}
		க்கு (cur_match = 0; cur_match < match_cnt; cur_match++) {
			கட்டமைப்பு possible_tag *t = &all_matches[cur_match];
			எனில் (!(c->object.flags & t->flag_within))
				t->depth++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		எனில் (annotated_cnt && !prio_queue_size(&queue)) {
			முழு best_depth = INT_MAX;
			unsigned best_within = 0;
			க்கு (cur_match = 0; cur_match < match_cnt; cur_match++) {
				கட்டமைப்பு possible_tag *t = &all_matches[cur_match];
				எனில் (t->depth < best_depth) {
					best_depth = t->depth;
					best_within = t->flag_within;
				} இல்லையெனில் எனில் (t->depth == best_depth) {
					best_within |= t->flag_within;
				}
			}
			எனில் ((c->object.flags & best_within) == best_within) {
				எனில் (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_to_hex(&c->object.oid));
				நிறுத்து;
			}
		}
		போது (parents) {
			கட்டமைப்பு commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			எனில் (!(p->object.flags & SEEN))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->next;

			எனில் (first_parent)
				நிறுத்து;
		}
	}

	எனில் (!match_cnt) {
		கட்டமைப்பு object_id *cmit_oid = &cmit->object.oid;
		எனில் (always) {
			strbuf_add_unique_abbrev(dst, cmit_oid, abbrev);
			எனில் (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			திரும்பு;
		}
		எனில் (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_to_hex(cmit_oid));
		இல்லையெனில்
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_to_hex(cmit_oid));
	}

	QSORT(all_matches, match_cnt, compare_pt);

	எனில் (gave_up_on) {
		prio_queue_put(&queue, gave_up_on);
		seen_commits--;
	}
	seen_commits += finish_depth_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	எனில் (debug) {
		நிலையான முழு label_width = -1;
		எனில் (label_width < 0) {
			முழு i, w;
			க்கு (i = 0; i < ARRAY_SIZE(prio_names); i++) {
				w = strlen(_(prio_names[i]));
				எனில் (label_width < w)
					label_width = w;
			}
		}
		க்கு (cur_match = 0; cur_match < match_cnt; cur_match++) {
			கட்டமைப்பு possible_tag *t = &all_matches[cur_match];
			fprintf(stderr, " %-*s %8d %s\n",
				label_width, _(prio_names[t->name->prio]),
				t->depth, t->name->path);
		}
		fprintf(stderr, _("traversed %lu commits\n"), seen_commits);
		எனில் (gave_up_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_to_hex(&gave_up_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	எனில் (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].depth, &cmit->object.oid, dst);
	எனில் (suffix)
		strbuf_addstr(dst, suffix);
}

கட்டமைப்பு process_commit_data {
	கட்டமைப்பு commit *current_commit;
	மாறிலி கட்டமைப்பு object_id *looking_for;
	கட்டமைப்பு strbuf *dst;
	கட்டமைப்பு rev_info *revs;
};

நிலையான வெறுமை process_commit(கட்டமைப்பு commit *commit, வெறுமை *data)
{
	கட்டமைப்பு process_commit_data *pcd = data;
	pcd->current_commit = commit;
}

நிலையான வெறுமை process_object(கட்டமைப்பு object *obj, மாறிலி எழுத்து *path, வெறுமை *data)
{
	கட்டமைப்பு process_commit_data *pcd = data;

	எனில் (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		எனில் (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", path);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = வெற்று;
	}
}

நிலையான வெறுமை describe_blob(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *dst)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு strvec args = STRVEC_INIT;
	கட்டமைப்பு object_id head_oid;
	கட்டமைப்பு process_commit_data pcd = { வெற்று, oid, dst, &revs};

	எனில் (repo_get_oid(the_repository, "HEAD", &head_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_to_hex(oid));

	strvec_pushl(&args, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_to_hex(&head_oid),
		     வெற்று);

	repo_init_revisions(the_repository, &revs, வெற்று);
	setup_revisions_from_strvec(&args, &revs, வெற்று);
	எனில் (args.nr > 1)
		BUG("setup_revisions could not handle all args?");

	எனில் (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&args);

	எனில் (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_to_hex(oid));
}

நிலையான வெறுமை describe(மாறிலி எழுத்து *arg, முழு last_one)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit *cmit;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	எனில் (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	எனில் (cmit)
		describe_commit(cmit, &sb);
	இல்லையெனில் எனில் (odb_read_object_info(the_repository->objects,
				      &oid, வெற்று) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	இல்லையெனில்
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	எனில் (!last_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

நிலையான முழு option_parse_exact_match(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				    முழு unset)
{
	முழு *val = opt->மதிப்பு;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	திரும்பு 0;
}

முழு cmd_describe(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED )
{
	கட்டமைப்பு refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	முழு contains = 0;
	கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "contains",   &contains, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &tags, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, வெற்று,
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
			.மதிப்பு = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.மதிப்பு = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, options, describe_usage, 0);
	எனில் (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	எனில் (max_candidates < 0)
		max_candidates = 0;
	இல்லையெனில் எனில் (max_candidates > MAX_TAGS)
		max_candidates = MAX_TAGS;

	save_commit_buffer = 0;

	எனில் (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	எனில் (contains) {
		கட்டமைப்பு string_list_item *item;
		கட்டமைப்பு strvec args;
		மாறிலி எழுத்து **argv_copy;
		முழு ret;

		strvec_init(&args);
		strvec_pushl(&args, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     வெற்று);
		எனில் (always)
			strvec_தள்ளு(&args, "--always");
		எனில் (!all)
			strvec_தள்ளு(&args, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&args, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&args, "--exclude=refs/tags/%s", item->string);

		எனில் (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&args, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&args, "--exclude=refs/remotes/%s", item->string);
		}

		எனில் (argc)
			strvec_pushv(&args, argv);
		இல்லையெனில்
			strvec_தள்ளு(&args, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, args.nr + 1);
		க்கு (size_t i = 0; i < args.nr; i++)
			argv_copy[i] = args.v[i];
		argv_copy[args.nr] = வெற்று;

		ret = cmd_name_rev(args.nr, argv_copy, prefix, the_repository);

		strvec_clear(&args);
		விடுவி(argv_copy);
		திரும்பு ret;
	}

	எனில் (!all)
		for_each_ref_opts.prefix = "refs/tags/";

	hashmap_init(&names, commit_name_neq, வெற்று, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, வெற்று, &for_each_ref_opts);
	எனில் (!hashmap_get_size(&names) && !always)
		die(_("No names found, cannot describe anything."));

	எனில் (argc == 0) {
		எனில் (broken) {
			கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

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

			எனில் (!dirty)
				dirty = "-dirty";

			தேர்வு (run_command(&cp)) {
			நிலை 0:
				suffix = வெற்று;
				நிறுத்து;
			நிலை 1:
				suffix = dirty;
				நிறுத்து;
			இயல்பு:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} இல்லையெனில் எனில் (dirty) {
			கட்டமைப்பு lock_file index_lock = LOCK_INIT;
			கட்டமைப்பு rev_info revs;
			முழு fd;

			setup_work_மரம்(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			repo_read_index(the_repository);
			refresh_index(the_repository->index, REFRESH_QUIET|REFRESH_UNMERGED,
				      வெற்று, வெற்று, வெற்று);
			fd = repo_hold_locked_index(the_repository,
						    &index_lock, 0);
			எனில் (0 <= fd)
				repo_update_index_if_able(the_repository, &index_lock);

			repo_init_revisions(the_repository, &revs, prefix);

			எனில் (setup_revisions(ARRAY_SIZE(diff_index_args) - 1,
					    diff_index_args, &revs, வெற்று) != 1)
				BUG("malformed internal diff-index command line");
			run_diff_index(&revs, 0);

			எனில் (!diff_result_code(&revs))
				suffix = வெற்று;
			இல்லையெனில்
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} இல்லையெனில் எனில் (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} இல்லையெனில் எனில் (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} இல்லையெனில் {
		போது (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	திரும்பு 0;
}
