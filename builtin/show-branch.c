#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "pretty.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"

#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "commit-slab.h"
#അന്തര്ഭാവ "date.h"
#അന്തര്ഭാവ "wildmatch.h"
#അന്തര്ഭാവ "prio-queue.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം*സ്ഥിരം show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ശൂന്യ
};

സ്ഥിതം എണ്ണൽ git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

സ്ഥിതം ഘടന strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_color_code(പൂർണ്ണസംഖ്യ idx)
{
	എങ്കിൽ (want_color(showbranch_use_color))
		തിരികെ column_colors_ansi[idx % column_colors_ansi_max];
	തിരികെ "";
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_color_reset_code(ശൂന്യം)
{
	എങ്കിൽ (want_color(showbranch_use_color))
		തിരികെ GIT_COLOR_RESET;
	തിരികെ "";
}

സ്ഥിതം ഘടന commit *interesting(ഘടന prio_queue *queue)
{
	ഘടന commit *commit;
	prio_queue_for_each(queue, commit) {
		എങ്കിൽ (!(commit->object.flags & UNINTERESTING))
			തിരികെ commit;
	}
	തിരികെ ശൂന്യ;
}

ഘടന commit_name {
	സ്ഥിരം അക്ഷരം *head_name; /* which head's ancestor? */
	പൂർണ്ണസംഖ്യ generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ഘടന commit_name *);
സ്ഥിതം ഘടന commit_name_slab name_slab;

സ്ഥിതം ഘടന commit_name *commit_to_name(ഘടന commit *commit)
{
	തിരികെ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
സ്ഥിതം ശൂന്യം name_commit(ഘടന commit *commit, സ്ഥിരം അക്ഷരം *head_name, പൂർണ്ണസംഖ്യ nth)
{
	ഘടന commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	എങ്കിൽ (!name) {
		name = xmalloc(വലുപ്പം(*name));
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
സ്ഥിതം ശൂന്യം name_parent(ഘടന commit *commit, ഘടന commit *parent)
{
	ഘടന commit_name *commit_name = commit_to_name(commit);
	ഘടന commit_name *parent_name = commit_to_name(parent);
	എങ്കിൽ (!commit_name)
		തിരികെ;
	എങ്കിൽ (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ name_first_parent_chain(ഘടന commit *c)
{
	പൂർണ്ണസംഖ്യ i = 0;
	ആകുമ്പോൾ (c) {
		ഘടന commit *p;
		എങ്കിൽ (!commit_to_name(c))
			നിർത്തുക;
		എങ്കിൽ (!c->parents)
			നിർത്തുക;
		p = c->parents->item;
		എങ്കിൽ (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		അല്ലെങ്കിൽ
			നിർത്തുക;
		c = p;
	}
	തിരികെ i;
}

സ്ഥിതം ശൂന്യം name_commits(ഘടന commit_list *list,
			 ഘടന commit **rev,
			 അക്ഷരം **ref_name,
			 പൂർണ്ണസംഖ്യ അക്കം_rev)
{
	ഘടന commit_list *cl;
	ഘടന commit *c;
	പൂർണ്ണസംഖ്യ i;

	/* First give names to the given heads */
	വേണ്ടി (cl = list; cl; cl = cl->next) {
		c = cl->item;
		എങ്കിൽ (commit_to_name(c))
			തുടരുക;
		വേണ്ടി (i = 0; i < അക്കം_rev; i++) {
			എങ്കിൽ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				നിർത്തുക;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	ചെയ്യുക {
		i = 0;
		വേണ്ടി (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} ആകുമ്പോൾ (i);

	/* Finally, any unnamed commits */
	ചെയ്യുക {
		i = 0;
		വേണ്ടി (cl = list; cl; cl = cl->next) {
			ഘടന commit_list *parents;
			ഘടന commit_name *n;
			പൂർണ്ണസംഖ്യ nth;
			c = cl->item;
			എങ്കിൽ (!commit_to_name(c))
				തുടരുക;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			ആകുമ്പോൾ (parents) {
				ഘടന commit *p = parents->item;
				ഘടന strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				എങ്കിൽ (commit_to_name(p))
					തുടരുക;
				തിരഞ്ഞെടുപ്പ് (n->generation) {
				അവസ്ഥ 0:
					strbuf_addstr(&newname, n->head_name);
					നിർത്തുക;
				അവസ്ഥ 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					നിർത്തുക;
				സ്ഥിരസ്ഥിതി:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					നിർത്തുക;
				}
				എങ്കിൽ (nth == 1)
					strbuf_addch(&newname, '^');
				അല്ലെങ്കിൽ
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ശൂന്യ), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} ആകുമ്പോൾ (i);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ mark_seen(ഘടന commit *commit, ഘടന commit_list **seen_p)
{
	എങ്കിൽ (!commit->object.flags) {
		commit_list_തിരുകുക(commit, seen_p);
		തിരികെ 1;
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം join_revs(ഘടന prio_queue *queue,
		      ഘടന commit_list **seen_p,
		      പൂർണ്ണസംഖ്യ അക്കം_rev, പൂർണ്ണസംഖ്യ extra)
{
	പൂർണ്ണസംഖ്യ all_mask = ((1u << (REV_SHIFT + അക്കം_rev)) - 1);
	പൂർണ്ണസംഖ്യ all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ഘടന commit *commit;

	ആകുമ്പോൾ ((commit = prio_queue_peek(queue))) {
		ഘടന commit_list *parents;
		പൂർണ്ണസംഖ്യ still_interesting = !!interesting(queue);
		പൂർണ്ണസംഖ്യ flags = commit->object.flags & all_mask;

		എങ്കിൽ (!still_interesting && extra <= 0)
			നിർത്തുക;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		എങ്കിൽ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ആകുമ്പോൾ (parents) {
			ഘടന commit *p = parents->item;
			പൂർണ്ണസംഖ്യ this_flag = p->object.flags;
			parents = parents->next;
			എങ്കിൽ ((this_flag & flags) == flags)
				തുടരുക;
			repo_parse_commit(the_repository, p);
			എങ്കിൽ (mark_seen(p, seen_p) && !still_interesting)
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
	വേണ്ടി (;;) {
		പൂർണ്ണസംഖ്യ changed = 0;
		ഘടന commit_list *s;
		വേണ്ടി (s = *seen_p; s; s = s->next) {
			ഘടന commit *c = s->item;
			ഘടന commit_list *parents;

			എങ്കിൽ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				തുടരുക;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ആകുമ്പോൾ (parents) {
				ഘടന commit *p = parents->item;
				parents = parents->next;
				എങ്കിൽ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		എങ്കിൽ (!changed)
			നിർത്തുക;
	}
}

സ്ഥിതം ശൂന്യം show_one_commit(ഘടന commit *commit, പൂർണ്ണസംഖ്യ no_name)
{
	ഘടന strbuf pretty = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *pretty_str = "(unavailable)";
	ഘടന commit_name *name = commit_to_name(commit);

	എങ്കിൽ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	എങ്കിൽ (!no_name) {
		എങ്കിൽ (name && name->head_name) {
			printf("[%s", name->head_name);
			എങ്കിൽ (name->generation) {
				എങ്കിൽ (name->generation == 1)
					printf("^");
				അല്ലെങ്കിൽ
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		അല്ലെങ്കിൽ
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

സ്ഥിതം അക്ഷരം *ref_name[MAX_REVS + 1];
സ്ഥിതം പൂർണ്ണസംഖ്യ ref_name_cnt;

സ്ഥിതം സ്ഥിരം അക്ഷരം *find_digit_prefix(സ്ഥിരം അക്ഷരം *s, പൂർണ്ണസംഖ്യ *v)
{
	സ്ഥിരം അക്ഷരം *p;
	പൂർണ്ണസംഖ്യ ver;
	അക്ഷരം ch;

	വേണ്ടി (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	തിരികെ p;
}


സ്ഥിതം പൂർണ്ണസംഖ്യ version_cmp(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b)
{
	ആകുമ്പോൾ (1) {
		പൂർണ്ണസംഖ്യ va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		എങ്കിൽ (va != vb)
			തിരികെ va - vb;

		ആകുമ്പോൾ (1) {
			പൂർണ്ണസംഖ്യ ca = *a;
			പൂർണ്ണസംഖ്യ cb = *b;
			എങ്കിൽ ('0' <= ca && ca <= '9')
				ca = 0;
			എങ്കിൽ ('0' <= cb && cb <= '9')
				cb = 0;
			എങ്കിൽ (ca != cb)
				തിരികെ ca - cb;
			എങ്കിൽ (!ca)
				നിർത്തുക;
			a++;
			b++;
		}
		എങ്കിൽ (!*a && !*b)
			തിരികെ 0;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_ref_name(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം*a = a_, * സ്ഥിരം*b = b_;
	തിരികെ version_cmp(*a, *b);
}

സ്ഥിതം ശൂന്യം sort_ref_range(പൂർണ്ണസംഖ്യ bottom, പൂർണ്ണസംഖ്യ മുകൾ)
{
	QSORT(ref_name + bottom, മുകൾ - bottom, compare_ref_name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_ref(സ്ഥിരം അക്ഷരം *refname, സ്ഥിരം ഘടന object_id *oid,
		      പൂർണ്ണസംഖ്യ allow_dups)
{
	ഘടന commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!commit)
		തിരികെ 0;

	എങ്കിൽ (!allow_dups) {
		/* Avoid adding the same thing twice */
		വേണ്ടി (i = 0; i < ref_name_cnt; i++)
			എങ്കിൽ (!strcmp(refname, ref_name[i]))
				തിരികെ 0;
	}
	എങ്കിൽ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		തിരികെ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ശൂന്യ;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_head_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	ഘടന object_id tmp;
	പൂർണ്ണസംഖ്യ ofs = 11;
	എങ്കിൽ (!starts_with(ref->name, "refs/heads/"))
		തിരികെ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	എങ്കിൽ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	തിരികെ append_ref(ref->name + ofs, ref->oid, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_remote_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	ഘടന object_id tmp;
	പൂർണ്ണസംഖ്യ ofs = 13;
	എങ്കിൽ (!starts_with(ref->name, "refs/remotes/"))
		തിരികെ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	എങ്കിൽ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	തിരികെ append_ref(ref->name + ofs, ref->oid, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_tag_ref(സ്ഥിരം അക്ഷരം *refname, സ്ഥിരം ഘടന object_id *oid,
			  പൂർണ്ണസംഖ്യ flag UNUSED, ശൂന്യം *cb_data UNUSED)
{
	എങ്കിൽ (!starts_with(refname, "refs/tags/"))
		തിരികെ 0;
	തിരികെ append_ref(refname + 5, oid, 0);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *match_ref_pattern = ശൂന്യ;
സ്ഥിതം പൂർണ്ണസംഖ്യ match_ref_slash = 0;

സ്ഥിതം പൂർണ്ണസംഖ്യ append_matching_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	സ്ഥിരം അക്ഷരം *tail;
	പൂർണ്ണസംഖ്യ slash = എണ്ണം_slashes(ref->name);
	വേണ്ടി (tail = ref->name; *tail && match_ref_slash < slash; )
		എങ്കിൽ (*tail++ == '/')
			slash--;
	എങ്കിൽ (!*tail)
		തിരികെ 0;
	എങ്കിൽ (wildmatch(match_ref_pattern, tail, 0))
		തിരികെ 0;
	എങ്കിൽ (starts_with(ref->name, "refs/heads/"))
		തിരികെ append_head_ref(ref, cb_data);
	എങ്കിൽ (starts_with(ref->name, "refs/tags/"))
		തിരികെ append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	തിരികെ append_ref(ref->name, ref->oid, 0);
}

സ്ഥിതം ശൂന്യം snarf_refs(പൂർണ്ണസംഖ്യ head, പൂർണ്ണസംഖ്യ remotes)
{
	എങ്കിൽ (head) {
		പൂർണ്ണസംഖ്യ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, ശൂന്യ);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	എങ്കിൽ (remotes) {
		പൂർണ്ണസംഖ്യ orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ശൂന്യ);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rev_is_head(സ്ഥിരം അക്ഷരം *head, സ്ഥിരം അക്ഷരം *name)
{
	എങ്കിൽ (!head)
		തിരികെ 0;
	skip_prefix(head, "refs/heads/", &head);
	എങ്കിൽ (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	തിരികെ !strcmp(head, name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_merge_base(സ്ഥിരം ഘടന commit_list *seen, പൂർണ്ണസംഖ്യ അക്കം_rev)
{
	പൂർണ്ണസംഖ്യ all_mask = ((1u << (REV_SHIFT + അക്കം_rev)) - 1);
	പൂർണ്ണസംഖ്യ all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	പൂർണ്ണസംഖ്യ exit_status = 1;

	വേണ്ടി (സ്ഥിരം ഘടന commit_list *s = seen; s; s = s->next) {
		ഘടന commit *commit = s->item;
		പൂർണ്ണസംഖ്യ flags = commit->object.flags & all_mask;
		എങ്കിൽ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	തിരികെ exit_status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_independent(ഘടന commit **rev,
			    പൂർണ്ണസംഖ്യ അക്കം_rev,
			    unsigned പൂർണ്ണസംഖ്യ *rev_mask)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < അക്കം_rev; i++) {
		ഘടന commit *commit = rev[i];
		unsigned പൂർണ്ണസംഖ്യ flag = rev_mask[i];

		എങ്കിൽ (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം append_one_rev(സ്ഥിരം അക്ഷരം *av)
{
	ഘടന object_id revkey;
	എങ്കിൽ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		തിരികെ;
	}
	എങ്കിൽ (strpbrk(av, "*?[")) {
		/* glob style match */
		പൂർണ്ണസംഖ്യ saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = എണ്ണം_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ശൂന്യ);
		എങ്കിൽ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		തിരികെ;
	}
	die("bad sha1 reference %s", av);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_show_branch_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
				  സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "showbranch.default")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		എങ്കിൽ (!default_args.nr)
			strvec_തള്ളുക(&default_args, "show-branch");
		strvec_തള്ളുക(&default_args, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (git_color_config(var, മൂല്യം, cb) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ omit_in_dense(ഘടന commit *commit, ഘടന commit **rev, പൂർണ്ണസംഖ്യ n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	പൂർണ്ണസംഖ്യ i, flag, എണ്ണം;
	വേണ്ടി (i = 0; i < n; i++)
		എങ്കിൽ (rev[i] == commit)
			തിരികെ 0;
	flag = commit->object.flags;
	വേണ്ടി (i = എണ്ണം = 0; i < n; i++) {
		എങ്കിൽ (flag & (1u << (i + REV_SHIFT)))
			എണ്ണം++;
	}
	എങ്കിൽ (എണ്ണം == 1)
		തിരികെ 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ reflog = 0;

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_reflog_param(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			      പൂർണ്ണസംഖ്യ unset)
{
	അക്ഷരം *ep;
	സ്ഥിരം അക്ഷരം **base = (സ്ഥിരം അക്ഷരം **)opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	എങ്കിൽ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	എങ്കിൽ (*ep == ',')
		*base = ep + 1;
	അല്ലെങ്കിൽ എങ്കിൽ (*ep)
		തിരികെ error("unrecognized reflog param '%s'", arg);
	അല്ലെങ്കിൽ
		*base = ശൂന്യ;
	എങ്കിൽ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_show_branch(പൂർണ്ണസംഖ്യ ac,
		സ്ഥിരം അക്ഷരം **av,
		സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	ഘടന commit *rev[MAX_REVS], *commit;
	അക്ഷരം *reflog_സന്ദേശം[MAX_REVS] = {0};
	ഘടന commit_list *seen = ശൂന്യ;
	ഘടന prio_queue queue = { compare_commits_by_commit_date };
	unsigned പൂർണ്ണസംഖ്യ rev_mask[MAX_REVS];
	പൂർണ്ണസംഖ്യ അക്കം_rev, i, extra = 0;
	പൂർണ്ണസംഖ്യ all_heads = 0, all_remotes = 0;
	പൂർണ്ണസംഖ്യ all_mask, all_revs;
	എണ്ണൽ rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	അക്ഷരം *head;
	ഘടന object_id head_oid;
	പൂർണ്ണസംഖ്യ merge_base = 0;
	പൂർണ്ണസംഖ്യ independent = 0;
	പൂർണ്ണസംഖ്യ no_name = 0;
	പൂർണ്ണസംഖ്യ sha1_name = 0;
	പൂർണ്ണസംഖ്യ shown_merge_point = 0;
	പൂർണ്ണസംഖ്യ with_current_branch = 0;
	പൂർണ്ണസംഖ്യ head_at = -1;
	പൂർണ്ണസംഖ്യ topics = 0;
	പൂർണ്ണസംഖ്യ sparse = 0;
	സ്ഥിരം അക്ഷരം *reflog_base = ശൂന്യ;
	ഘടന option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.മൂല്യം = &extra,
			.precision = വലുപ്പം(extra),
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
	സ്ഥിരം അക്ഷരം **args_copy = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ശൂന്യ);

	/* If nothing is specified, try the default first */
	എങ്കിൽ (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	എങ്കിൽ (all_heads)
		all_remotes = 1;

	എങ്കിൽ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		എങ്കിൽ (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		എങ്കിൽ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	എങ്കിൽ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	എങ്കിൽ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	എങ്കിൽ (reflog) {
		ഘടന object_id oid;
		അക്ഷരം *ref;
		പൂർണ്ണസംഖ്യ base = 0;
		unsigned പൂർണ്ണസംഖ്യ flags = 0;

		എങ്കിൽ (ac == 0) {
			സ്ഥിതം സ്ഥിരം അക്ഷരം *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ശൂന്യ);
			fake_av[1] = ശൂന്യ;
			av = fake_av;
			ac = 1;
			എങ്കിൽ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		എങ്കിൽ (ac != 1)
			die(_("--reflog option needs one branch name"));

		എങ്കിൽ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		എങ്കിൽ (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		എങ്കിൽ (reflog_base) {
			അക്ഷരം *ep;
			base = strtoul(reflog_base, &ep, 10);
			എങ്കിൽ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ശൂന്യ,
					    ശൂന്യ, ശൂന്യ, &base);
			}
		}

		വേണ്ടി (i = 0; i < reflog; i++) {
			അക്ഷരം *logmsg = ശൂന്യ;
			അക്ഷരം *nth_desc;
			സ്ഥിരം അക്ഷരം *സന്ദേശം;
			അക്ഷരം *end;
			timestamp_t timestamp;
			പൂർണ്ണസംഖ്യ tz;

			എങ്കിൽ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, ശൂന്യ)) {
				സ്വതന്ത്രം(logmsg);
				reflog = i;
				നിർത്തുക;
			}

			end = strchr(logmsg, '\n');
			എങ്കിൽ (end)
				*end = '\0';

			സന്ദേശം = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_സന്ദേശം[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						സന്ദേശം);
			സ്വതന്ത്രം(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			സ്വതന്ത്രം(nth_desc);
		}
		സ്വതന്ത്രം(ref);
	}
	അല്ലെങ്കിൽ {
		ആകുമ്പോൾ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		എങ്കിൽ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, ശൂന്യ);

	എങ്കിൽ (with_current_branch && head) {
		പൂർണ്ണസംഖ്യ has_head = 0;
		വേണ്ടി (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			എങ്കിൽ (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		എങ്കിൽ (!has_head) {
			സ്ഥിരം അക്ഷരം *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	എങ്കിൽ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		പോകുക out;
	}

	വേണ്ടി (അക്കം_rev = 0; ref_name[അക്കം_rev]; അക്കം_rev++) {
		ഘടന object_id revkey;
		unsigned പൂർണ്ണസംഖ്യ flag = 1u << (അക്കം_rev + REV_SHIFT);

		എങ്കിൽ (MAX_REVS <= അക്കം_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		എങ്കിൽ (repo_get_oid(the_repository, ref_name[അക്കം_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[അക്കം_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		എങ്കിൽ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[അക്കം_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		എങ്കിൽ (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[അക്കം_rev] = commit;
	}
	വേണ്ടി (i = 0; i < അക്കം_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	എങ്കിൽ (0 <= extra)
		join_revs(&queue, &seen, അക്കം_rev, extra);

	commit_list_sort_by_date(&seen);

	എങ്കിൽ (merge_base) {
		ret = show_merge_base(seen, അക്കം_rev);
		പോകുക out;
	}

	എങ്കിൽ (independent) {
		ret = show_independent(rev, അക്കം_rev, rev_mask);
		പോകുക out;
	}

	/* Show list; --more=-1 means list-only */
	എങ്കിൽ (1 < അക്കം_rev || extra < 0) {
		വേണ്ടി (i = 0; i < അക്കം_rev; i++) {
			പൂർണ്ണസംഖ്യ j;
			പൂർണ്ണസംഖ്യ is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			എങ്കിൽ (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			അല്ലെങ്കിൽ {
				വേണ്ടി (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			എങ്കിൽ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			അല്ലെങ്കിൽ
				puts(reflog_സന്ദേശം[i]);

			എങ്കിൽ (is_head)
				head_at = i;
		}
		എങ്കിൽ (0 <= extra) {
			വേണ്ടി (i = 0; i < അക്കം_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	എങ്കിൽ (extra < 0) {
		ret = 0;
		പോകുക out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	എങ്കിൽ (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, അക്കം_rev);

	all_mask = ((1u << (REV_SHIFT + അക്കം_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	വേണ്ടി (ഘടന commit_list *l = seen; l; l = l->next) {
		ഘടന commit *commit = l->item;
		പൂർണ്ണസംഖ്യ this_flag = commit->object.flags;
		പൂർണ്ണസംഖ്യ is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		എങ്കിൽ (1 < അക്കം_rev) {
			പൂർണ്ണസംഖ്യ is_merge = !!(commit->parents &&
					  commit->parents->next);
			എങ്കിൽ (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				തുടരുക;
			എങ്കിൽ (!sparse && is_merge &&
			    omit_in_dense(commit, rev, അക്കം_rev))
				തുടരുക;
			വേണ്ടി (i = 0; i < അക്കം_rev; i++) {
				പൂർണ്ണസംഖ്യ mark;
				എങ്കിൽ (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				അല്ലെങ്കിൽ എങ്കിൽ (is_merge)
					mark = '-';
				അല്ലെങ്കിൽ എങ്കിൽ (i == head_at)
					mark = '*';
				അല്ലെങ്കിൽ
					mark = '+';
				എങ്കിൽ (mark == ' ')
					putchar(mark);
				അല്ലെങ്കിൽ
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		എങ്കിൽ (shown_merge_point && --extra < 0)
			നിർത്തുക;
	}

	ret = 0;

out:
	വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(reflog_സന്ദേശം); i++)
		സ്വതന്ത്രം(reflog_സന്ദേശം[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	സ്വതന്ത്രം(args_copy);
	സ്വതന്ത്രം(head);
	തിരികെ ret;
}
