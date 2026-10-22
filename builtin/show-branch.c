#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "pretty.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "color.h"
#অন্তর্ভাৱ "strvec.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "parse-options.h"

#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "commit-slab.h"
#অন্তর্ভাৱ "date.h"
#অন্তর্ভাৱ "wildmatch.h"
#অন্তর্ভাৱ "prio-queue.h"

স্থির ধ্রুৱ ৱর্ণ*ধ্রুৱ show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    রিক্ত
};

স্থির গণনা git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

স্থির সংরচনা strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

স্থির ধ্রুৱ ৱর্ণ *get_color_code(পূর্ণাঙ্ক idx)
{
	যদি (want_color(showbranch_use_color))
		ঘূৰাই column_colors_ansi[idx % column_colors_ansi_max];
	ঘূৰাই "";
}

স্থির ধ্রুৱ ৱর্ণ *get_color_reset_code(শূন্য)
{
	যদি (want_color(showbranch_use_color))
		ঘূৰাই GIT_COLOR_RESET;
	ঘূৰাই "";
}

স্থির সংরচনা commit *interesting(সংরচনা prio_queue *queue)
{
	সংরচনা commit *commit;
	prio_queue_for_each(queue, commit) {
		যদি (!(commit->object.flags & UNINTERESTING))
			ঘূৰাই commit;
	}
	ঘূৰাই রিক্ত;
}

সংরচনা commit_name {
	ধ্রুৱ ৱর্ণ *head_name; /* which head's ancestor? */
	পূর্ণাঙ্ক generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, সংরচনা commit_name *);
স্থির সংরচনা commit_name_slab name_slab;

স্থির সংরচনা commit_name *commit_to_name(সংরচনা commit *commit)
{
	ঘূৰাই *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
স্থির শূন্য name_commit(সংরচনা commit *commit, ধ্রুৱ ৱর্ণ *head_name, পূর্ণাঙ্ক nth)
{
	সংরচনা commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	যদি (!name) {
		name = xmalloc(পরিমাণ(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->head_name = head_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
স্থির শূন্য name_parent(সংরচনা commit *commit, সংরচনা commit *parent)
{
	সংরচনা commit_name *commit_name = commit_to_name(commit);
	সংরচনা commit_name *parent_name = commit_to_name(parent);
	যদি (!commit_name)
		ঘূৰাই;
	যদি (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

স্থির পূর্ণাঙ্ক name_first_parent_chain(সংরচনা commit *c)
{
	পূর্ণাঙ্ক i = 0;
	যেতিয়ালৈ (c) {
		সংরচনা commit *p;
		যদি (!commit_to_name(c))
			ভঙ্গ;
		যদি (!c->parents)
			ভঙ্গ;
		p = c->parents->item;
		যদি (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		অন্যথা
			ভঙ্গ;
		c = p;
	}
	ঘূৰাই i;
}

স্থির শূন্য name_commits(সংরচনা commit_list *list,
			 সংরচনা commit **rev,
			 ৱর্ণ **ref_name,
			 পূর্ণাঙ্ক অঙ্ক_rev)
{
	সংরচনা commit_list *cl;
	সংরচনা commit *c;
	পূর্ণাঙ্ক i;

	/* First give names to the given heads */
	বাবে (cl = list; cl; cl = cl->next) {
		c = cl->item;
		যদি (commit_to_name(c))
			অনুৱর্ত;
		বাবে (i = 0; i < অঙ্ক_rev; i++) {
			যদি (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				ভঙ্গ;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	কৰা {
		i = 0;
		বাবে (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} যেতিয়ালৈ (i);

	/* Finally, any unnamed commits */
	কৰা {
		i = 0;
		বাবে (cl = list; cl; cl = cl->next) {
			সংরচনা commit_list *parents;
			সংরচনা commit_name *n;
			পূর্ণাঙ্ক nth;
			c = cl->item;
			যদি (!commit_to_name(c))
				অনুৱর্ত;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			যেতিয়ালৈ (parents) {
				সংরচনা commit *p = parents->item;
				সংরচনা strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				যদি (commit_to_name(p))
					অনুৱর্ত;
				ৱিকল্প (n->generation) {
				পক্ষ 0:
					strbuf_addstr(&newname, n->head_name);
					ভঙ্গ;
				পক্ষ 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					ভঙ্গ;
				স্ৱভাৱ:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					ভঙ্গ;
				}
				যদি (nth == 1)
					strbuf_addch(&newname, '^');
				অন্যথা
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, রিক্ত), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} যেতিয়ালৈ (i);
}

স্থির পূর্ণাঙ্ক mark_seen(সংরচনা commit *commit, সংরচনা commit_list **seen_p)
{
	যদি (!commit->object.flags) {
		commit_list_ঢোকাও(commit, seen_p);
		ঘূৰাই 1;
	}
	ঘূৰাই 0;
}

স্থির শূন্য join_revs(সংরচনা prio_queue *queue,
		      সংরচনা commit_list **seen_p,
		      পূর্ণাঙ্ক অঙ্ক_rev, পূর্ণাঙ্ক extra)
{
	পূর্ণাঙ্ক all_mask = ((1u << (REV_SHIFT + অঙ্ক_rev)) - 1);
	পূর্ণাঙ্ক all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	সংরচনা commit *commit;

	যেতিয়ালৈ ((commit = prio_queue_peek(queue))) {
		সংরচনা commit_list *parents;
		পূর্ণাঙ্ক still_interesting = !!interesting(queue);
		পূর্ণাঙ্ক flags = commit->object.flags & all_mask;

		যদি (!still_interesting && extra <= 0)
			ভঙ্গ;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		যদি ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		যেতিয়ালৈ (parents) {
			সংরচনা commit *p = parents->item;
			পূর্ণাঙ্ক this_flag = p->object.flags;
			parents = parents->next;
			যদি ((this_flag & flags) == flags)
				অনুৱর্ত;
			repo_parse_commit(the_repository, p);
			যদি (mark_seen(p, seen_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_queue_put(queue, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	বাবে (;;) {
		পূর্ণাঙ্ক changed = 0;
		সংরচনা commit_list *s;
		বাবে (s = *seen_p; s; s = s->next) {
			সংরচনা commit *c = s->item;
			সংরচনা commit_list *parents;

			যদি (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				অনুৱর্ত;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			যেতিয়ালৈ (parents) {
				সংরচনা commit *p = parents->item;
				parents = parents->next;
				যদি (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		যদি (!changed)
			ভঙ্গ;
	}
}

স্থির শূন্য show_one_commit(সংরচনা commit *commit, পূর্ণাঙ্ক no_name)
{
	সংরচনা strbuf pretty = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *pretty_str = "(unavailable)";
	সংরচনা commit_name *name = commit_to_name(commit);

	যদি (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	যদি (!no_name) {
		যদি (name && name->head_name) {
			printf("[%s", name->head_name);
			যদি (name->generation) {
				যদি (name->generation == 1)
					printf("^");
				অন্যথা
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		অন্যথা
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

স্থির ৱর্ণ *ref_name[MAX_REVS + 1];
স্থির পূর্ণাঙ্ক ref_name_cnt;

স্থির ধ্রুৱ ৱর্ণ *find_digit_prefix(ধ্রুৱ ৱর্ণ *s, পূর্ণাঙ্ক *v)
{
	ধ্রুৱ ৱর্ণ *p;
	পূর্ণাঙ্ক ver;
	ৱর্ণ ch;

	বাবে (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ঘূৰাই p;
}


স্থির পূর্ণাঙ্ক version_cmp(ধ্রুৱ ৱর্ণ *a, ধ্রুৱ ৱর্ণ *b)
{
	যেতিয়ালৈ (1) {
		পূর্ণাঙ্ক va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		যদি (va != vb)
			ঘূৰাই va - vb;

		যেতিয়ালৈ (1) {
			পূর্ণাঙ্ক ca = *a;
			পূর্ণাঙ্ক cb = *b;
			যদি ('0' <= ca && ca <= '9')
				ca = 0;
			যদি ('0' <= cb && cb <= '9')
				cb = 0;
			যদি (ca != cb)
				ঘূৰাই ca - cb;
			যদি (!ca)
				ভঙ্গ;
			a++;
			b++;
		}
		যদি (!*a && !*b)
			ঘূৰাই 0;
	}
}

স্থির পূর্ণাঙ্ক compare_ref_name(ধ্রুৱ শূন্য *a_, ধ্রুৱ শূন্য *b_)
{
	ধ্রুৱ ৱর্ণ * ধ্রুৱ*a = a_, * ধ্রুৱ*b = b_;
	ঘূৰাই version_cmp(*a, *b);
}

স্থির শূন্য sort_ref_range(পূর্ণাঙ্ক bottom, পূর্ণাঙ্ক শীৰ্ষ)
{
	QSORT(ref_name + bottom, শীৰ্ষ - bottom, compare_ref_name);
}

স্থির পূর্ণাঙ্ক append_ref(ধ্রুৱ ৱর্ণ *refname, ধ্রুৱ সংরচনা object_id *oid,
		      পূর্ণাঙ্ক allow_dups)
{
	সংরচনা commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	পূর্ণাঙ্ক i;

	যদি (!commit)
		ঘূৰাই 0;

	যদি (!allow_dups) {
		/* Avoid adding the same thing twice */
		বাবে (i = 0; i < ref_name_cnt; i++)
			যদি (!strcmp(refname, ref_name[i]))
				ঘূৰাই 0;
	}
	যদি (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ঘূৰাই 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = রিক্ত;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক append_head_ref(ধ্রুৱ সংরচনা reference *ref, শূন্য *cb_data UNUSED)
{
	সংরচনা object_id tmp;
	পূর্ণাঙ্ক ofs = 11;
	যদি (!starts_with(ref->name, "refs/heads/"))
		ঘূৰাই 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	যদি (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ঘূৰাই append_ref(ref->name + ofs, ref->oid, 0);
}

স্থির পূর্ণাঙ্ক append_remote_ref(ধ্রুৱ সংরচনা reference *ref, শূন্য *cb_data UNUSED)
{
	সংরচনা object_id tmp;
	পূর্ণাঙ্ক ofs = 13;
	যদি (!starts_with(ref->name, "refs/remotes/"))
		ঘূৰাই 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	যদি (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ঘূৰাই append_ref(ref->name + ofs, ref->oid, 0);
}

স্থির পূর্ণাঙ্ক append_tag_ref(ধ্রুৱ ৱর্ণ *refname, ধ্রুৱ সংরচনা object_id *oid,
			  পূর্ণাঙ্ক flag UNUSED, শূন্য *cb_data UNUSED)
{
	যদি (!starts_with(refname, "refs/tags/"))
		ঘূৰাই 0;
	ঘূৰাই append_ref(refname + 5, oid, 0);
}

স্থির ধ্রুৱ ৱর্ণ *match_ref_pattern = রিক্ত;
স্থির পূর্ণাঙ্ক match_ref_slash = 0;

স্থির পূর্ণাঙ্ক append_matching_ref(ধ্রুৱ সংরচনা reference *ref, শূন্য *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ধ্রুৱ ৱর্ণ *tail;
	পূর্ণাঙ্ক slash = গুনতি_slashes(ref->name);
	বাবে (tail = ref->name; *tail && match_ref_slash < slash; )
		যদি (*tail++ == '/')
			slash--;
	যদি (!*tail)
		ঘূৰাই 0;
	যদি (wildmatch(match_ref_pattern, tail, 0))
		ঘূৰাই 0;
	যদি (starts_with(ref->name, "refs/heads/"))
		ঘূৰাই append_head_ref(ref, cb_data);
	যদি (starts_with(ref->name, "refs/tags/"))
		ঘূৰাই append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	ঘূৰাই append_ref(ref->name, ref->oid, 0);
}

স্থির শূন্য snarf_refs(পূর্ণাঙ্ক head, পূর্ণাঙ্ক remotes)
{
	যদি (head) {
		পূর্ণাঙ্ক orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, রিক্ত);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	যদি (remotes) {
		পূর্ণাঙ্ক orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, রিক্ত);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

স্থির পূর্ণাঙ্ক rev_is_head(ধ্রুৱ ৱর্ণ *head, ধ্রুৱ ৱর্ণ *name)
{
	যদি (!head)
		ঘূৰাই 0;
	skip_prefix(head, "refs/heads/", &head);
	যদি (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	ঘূৰাই !strcmp(head, name);
}

স্থির পূর্ণাঙ্ক show_merge_base(ধ্রুৱ সংরচনা commit_list *seen, পূর্ণাঙ্ক অঙ্ক_rev)
{
	পূর্ণাঙ্ক all_mask = ((1u << (REV_SHIFT + অঙ্ক_rev)) - 1);
	পূর্ণাঙ্ক all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	পূর্ণাঙ্ক exit_status = 1;

	বাবে (ধ্রুৱ সংরচনা commit_list *s = seen; s; s = s->next) {
		সংরচনা commit *commit = s->item;
		পূর্ণাঙ্ক flags = commit->object.flags & all_mask;
		যদি (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ঘূৰাই exit_status;
}

স্থির পূর্ণাঙ্ক show_independent(সংরচনা commit **rev,
			    পূর্ণাঙ্ক অঙ্ক_rev,
			    unsigned পূর্ণাঙ্ক *rev_mask)
{
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < অঙ্ক_rev; i++) {
		সংরচনা commit *commit = rev[i];
		unsigned পূর্ণাঙ্ক flag = rev_mask[i];

		যদি (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ঘূৰাই 0;
}

স্থির শূন্য append_one_rev(ধ্রুৱ ৱর্ণ *av)
{
	সংরচনা object_id revkey;
	যদি (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ঘূৰাই;
	}
	যদি (strpbrk(av, "*?[")) {
		/* glob style match */
		পূর্ণাঙ্ক saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = গুনতি_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, রিক্ত);
		যদি (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		ঘূৰাই;
	}
	die("bad sha1 reference %s", av);
}

স্থির পূর্ণাঙ্ক git_show_branch_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
				  ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "showbranch.default")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		যদি (!default_args.nr)
			strvec_ঠেলো(&default_args, "show-branch");
		strvec_ঠেলো(&default_args, মান);
		ঘূৰাই 0;
	}

	যদি (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, মান);
		ঘূৰাই 0;
	}

	যদি (git_color_config(var, মান, cb) < 0)
		ঘূৰাই -1;

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণাঙ্ক omit_in_dense(সংরচনা commit *commit, সংরচনা commit **rev, পূর্ণাঙ্ক n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	পূর্ণাঙ্ক i, flag, গুনতি;
	বাবে (i = 0; i < n; i++)
		যদি (rev[i] == commit)
			ঘূৰাই 0;
	flag = commit->object.flags;
	বাবে (i = গুনতি = 0; i < n; i++) {
		যদি (flag & (1u << (i + REV_SHIFT)))
			গুনতি++;
	}
	যদি (গুনতি == 1)
		ঘূৰাই 1;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক reflog = 0;

স্থির পূর্ণাঙ্ক parse_reflog_param(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg,
			      পূর্ণাঙ্ক unset)
{
	ৱর্ণ *ep;
	ধ্রুৱ ৱর্ণ **base = (ধ্রুৱ ৱর্ণ **)opt->মান;
	BUG_ON_OPT_NEG(unset);
	যদি (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	যদি (*ep == ',')
		*base = ep + 1;
	অন্যথা যদি (*ep)
		ঘূৰাই error("unrecognized reflog param '%s'", arg);
	অন্যথা
		*base = রিক্ত;
	যদি (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_show_branch(পূর্ণাঙ্ক ac,
		ধ্রুৱ ৱর্ণ **av,
		ধ্রুৱ ৱর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	সংরচনা commit *rev[MAX_REVS], *commit;
	ৱর্ণ *reflog_বাৰ্তা[MAX_REVS] = {0};
	সংরচনা commit_list *seen = রিক্ত;
	সংরচনা prio_queue queue = { compare_commits_by_commit_date };
	unsigned পূর্ণাঙ্ক rev_mask[MAX_REVS];
	পূর্ণাঙ্ক অঙ্ক_rev, i, extra = 0;
	পূর্ণাঙ্ক all_heads = 0, all_remotes = 0;
	পূর্ণাঙ্ক all_mask, all_revs;
	গণনা rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	ৱর্ণ *head;
	সংরচনা object_id head_oid;
	পূর্ণাঙ্ক merge_base = 0;
	পূর্ণাঙ্ক independent = 0;
	পূর্ণাঙ্ক no_name = 0;
	পূর্ণাঙ্ক sha1_name = 0;
	পূর্ণাঙ্ক shown_merge_point = 0;
	পূর্ণাঙ্ক with_current_branch = 0;
	পূর্ণাঙ্ক head_at = -1;
	পূর্ণাঙ্ক topics = 0;
	পূর্ণাঙ্ক sparse = 0;
	ধ্রুৱ ৱর্ণ *reflog_base = রিক্ত;
	সংরচনা option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.মান = &extra,
			.precision = পরিমাণ(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_SET_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &merge_base,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_SET_INT_F(0, "topo-order", &sort_order,
			      N_("show commits in topological order"),
			      REV_SORT_IN_GRAPH_ORDER, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_SET_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_SET_INT_F(0, "date-order", &sort_order,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_SORT_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_base, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_END()
	};
	ধ্রুৱ ৱর্ণ **args_copy = রিক্ত;
	পূর্ণাঙ্ক ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, রিক্ত);

	/* If nothing is specified, try the default first */
	যদি (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	যদি (all_heads)
		all_remotes = 1;

	যদি (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		যদি (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		যদি (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	যদি (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	যদি (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	যদি (reflog) {
		সংরচনা object_id oid;
		ৱর্ণ *ref;
		পূর্ণাঙ্ক base = 0;
		unsigned পূর্ণাঙ্ক flags = 0;

		যদি (ac == 0) {
			স্থির ধ্রুৱ ৱর্ণ *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 রিক্ত);
			fake_av[1] = রিক্ত;
			av = fake_av;
			ac = 1;
			যদি (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		যদি (ac != 1)
			die(_("--reflog option needs one branch name"));

		যদি (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		যদি (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		যদি (reflog_base) {
			ৱর্ণ *ep;
			base = strtoul(reflog_base, &ep, 10);
			যদি (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, রিক্ত,
					    রিক্ত, রিক্ত, &base);
			}
		}

		বাবে (i = 0; i < reflog; i++) {
			ৱর্ণ *logmsg = রিক্ত;
			ৱর্ণ *nth_desc;
			ধ্রুৱ ৱর্ণ *বাৰ্তা;
			ৱর্ণ *end;
			timestamp_t timestamp;
			পূর্ণাঙ্ক tz;

			যদি (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, রিক্ত)) {
				মুক্ত(logmsg);
				reflog = i;
				ভঙ্গ;
			}

			end = strchr(logmsg, '\n');
			যদি (end)
				*end = '\0';

			বাৰ্তা = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_বাৰ্তা[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						বাৰ্তা);
			মুক্ত(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			মুক্ত(nth_desc);
		}
		মুক্ত(ref);
	}
	অন্যথা {
		যেতিয়ালৈ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		যদি (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, রিক্ত);

	যদি (with_current_branch && head) {
		পূর্ণাঙ্ক has_head = 0;
		বাবে (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			যদি (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		যদি (!has_head) {
			ধ্রুৱ ৱর্ণ *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	যদি (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		গচ্ছ out;
	}

	বাবে (অঙ্ক_rev = 0; ref_name[অঙ্ক_rev]; অঙ্ক_rev++) {
		সংরচনা object_id revkey;
		unsigned পূর্ণাঙ্ক flag = 1u << (অঙ্ক_rev + REV_SHIFT);

		যদি (MAX_REVS <= অঙ্ক_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		যদি (repo_get_oid(the_repository, ref_name[অঙ্ক_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[অঙ্ক_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		যদি (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[অঙ্ক_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		যদি (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[অঙ্ক_rev] = commit;
	}
	বাবে (i = 0; i < অঙ্ক_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	যদি (0 <= extra)
		join_revs(&queue, &seen, অঙ্ক_rev, extra);

	commit_list_sort_by_date(&seen);

	যদি (merge_base) {
		ret = show_merge_base(seen, অঙ্ক_rev);
		গচ্ছ out;
	}

	যদি (independent) {
		ret = show_independent(rev, অঙ্ক_rev, rev_mask);
		গচ্ছ out;
	}

	/* Show list; --more=-1 means list-only */
	যদি (1 < অঙ্ক_rev || extra < 0) {
		বাবে (i = 0; i < অঙ্ক_rev; i++) {
			পূর্ণাঙ্ক j;
			পূর্ণাঙ্ক is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			যদি (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			অন্যথা {
				বাবে (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			যদি (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			অন্যথা
				puts(reflog_বাৰ্তা[i]);

			যদি (is_head)
				head_at = i;
		}
		যদি (0 <= extra) {
			বাবে (i = 0; i < অঙ্ক_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	যদি (extra < 0) {
		ret = 0;
		গচ্ছ out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	যদি (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, অঙ্ক_rev);

	all_mask = ((1u << (REV_SHIFT + অঙ্ক_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	বাবে (সংরচনা commit_list *l = seen; l; l = l->next) {
		সংরচনা commit *commit = l->item;
		পূর্ণাঙ্ক this_flag = commit->object.flags;
		পূর্ণাঙ্ক is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		যদি (1 < অঙ্ক_rev) {
			পূর্ণাঙ্ক is_merge = !!(commit->parents &&
					  commit->parents->next);
			যদি (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				অনুৱর্ত;
			যদি (!sparse && is_merge &&
			    omit_in_dense(commit, rev, অঙ্ক_rev))
				অনুৱর্ত;
			বাবে (i = 0; i < অঙ্ক_rev; i++) {
				পূর্ণাঙ্ক mark;
				যদি (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				অন্যথা যদি (is_merge)
					mark = '-';
				অন্যথা যদি (i == head_at)
					mark = '*';
				অন্যথা
					mark = '+';
				যদি (mark == ' ')
					putchar(mark);
				অন্যথা
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		যদি (shown_merge_point && --extra < 0)
			ভঙ্গ;
	}

	ret = 0;

out:
	বাবে (size_t i = 0; i < ARRAY_SIZE(reflog_বাৰ্তা); i++)
		মুক্ত(reflog_বাৰ্তা[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	মুক্ত(args_copy);
	মুক্ত(head);
	ঘূৰাই ret;
}
