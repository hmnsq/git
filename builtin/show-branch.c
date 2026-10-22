#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "pretty.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"

#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "commit-slab.h"
#અન્તર્ભાવ "date.h"
#અન્તર્ભાવ "wildmatch.h"
#અન્તર્ભાવ "prio-queue.h"

સ્થિર અચલ અક્ષર*અચલ show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ખાલી
};

સ્થિર ગણના git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

સ્થિર સંરચના strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

સ્થિર અચલ અક્ષર *get_color_code(પૂર્ણાંક idx)
{
	જો (want_color(showbranch_use_color))
		પાછા column_colors_ansi[idx % column_colors_ansi_max];
	પાછા "";
}

સ્થિર અચલ અક્ષર *get_color_reset_code(શૂન્ય)
{
	જો (want_color(showbranch_use_color))
		પાછા GIT_COLOR_RESET;
	પાછા "";
}

સ્થિર સંરચના commit *interesting(સંરચના prio_queue *queue)
{
	સંરચના commit *commit;
	prio_queue_for_each(queue, commit) {
		જો (!(commit->object.flags & UNINTERESTING))
			પાછા commit;
	}
	પાછા ખાલી;
}

સંરચના commit_name {
	અચલ અક્ષર *head_name; /* which head's ancestor? */
	પૂર્ણાંક generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, સંરચના commit_name *);
સ્થિર સંરચના commit_name_slab name_slab;

સ્થિર સંરચના commit_name *commit_to_name(સંરચના commit *commit)
{
	પાછા *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
સ્થિર શૂન્ય name_commit(સંરચના commit *commit, અચલ અક્ષર *head_name, પૂર્ણાંક nth)
{
	સંરચના commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	જો (!name) {
		name = xmalloc(કદ(*name));
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
સ્થિર શૂન્ય name_parent(સંરચના commit *commit, સંરચના commit *parent)
{
	સંરચના commit_name *commit_name = commit_to_name(commit);
	સંરચના commit_name *parent_name = commit_to_name(parent);
	જો (!commit_name)
		પાછા;
	જો (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

સ્થિર પૂર્ણાંક name_first_parent_chain(સંરચના commit *c)
{
	પૂર્ણાંક i = 0;
	જ્યારે (c) {
		સંરચના commit *p;
		જો (!commit_to_name(c))
			તોડો;
		જો (!c->parents)
			તોડો;
		p = c->parents->item;
		જો (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		નહીંતર
			તોડો;
		c = p;
	}
	પાછા i;
}

સ્થિર શૂન્ય name_commits(સંરચના commit_list *list,
			 સંરચના commit **rev,
			 અક્ષર **ref_name,
			 પૂર્ણાંક અંક_rev)
{
	સંરચના commit_list *cl;
	સંરચના commit *c;
	પૂર્ણાંક i;

	/* First give names to the given heads */
	માટે (cl = list; cl; cl = cl->next) {
		c = cl->item;
		જો (commit_to_name(c))
			ચાલુ;
		માટે (i = 0; i < અંક_rev; i++) {
			જો (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				તોડો;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	કરો {
		i = 0;
		માટે (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} જ્યારે (i);

	/* Finally, any unnamed commits */
	કરો {
		i = 0;
		માટે (cl = list; cl; cl = cl->next) {
			સંરચના commit_list *parents;
			સંરચના commit_name *n;
			પૂર્ણાંક nth;
			c = cl->item;
			જો (!commit_to_name(c))
				ચાલુ;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			જ્યારે (parents) {
				સંરચના commit *p = parents->item;
				સંરચના strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				જો (commit_to_name(p))
					ચાલુ;
				પસંદગી (n->generation) {
				સ્થિતિ 0:
					strbuf_addstr(&newname, n->head_name);
					તોડો;
				સ્થિતિ 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					તોડો;
				મૂળભૂત:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					તોડો;
				}
				જો (nth == 1)
					strbuf_addch(&newname, '^');
				નહીંતર
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ખાલી), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} જ્યારે (i);
}

સ્થિર પૂર્ણાંક mark_seen(સંરચના commit *commit, સંરચના commit_list **seen_p)
{
	જો (!commit->object.flags) {
		commit_list_ઉમેરો(commit, seen_p);
		પાછા 1;
	}
	પાછા 0;
}

સ્થિર શૂન્ય join_revs(સંરચના prio_queue *queue,
		      સંરચના commit_list **seen_p,
		      પૂર્ણાંક અંક_rev, પૂર્ણાંક extra)
{
	પૂર્ણાંક all_mask = ((1u << (REV_SHIFT + અંક_rev)) - 1);
	પૂર્ણાંક all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	સંરચના commit *commit;

	જ્યારે ((commit = prio_queue_peek(queue))) {
		સંરચના commit_list *parents;
		પૂર્ણાંક still_interesting = !!interesting(queue);
		પૂર્ણાંક flags = commit->object.flags & all_mask;

		જો (!still_interesting && extra <= 0)
			તોડો;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		જો ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		જ્યારે (parents) {
			સંરચના commit *p = parents->item;
			પૂર્ણાંક this_flag = p->object.flags;
			parents = parents->next;
			જો ((this_flag & flags) == flags)
				ચાલુ;
			repo_parse_commit(the_repository, p);
			જો (mark_seen(p, seen_p) && !still_interesting)
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
	માટે (;;) {
		પૂર્ણાંક changed = 0;
		સંરચના commit_list *s;
		માટે (s = *seen_p; s; s = s->next) {
			સંરચના commit *c = s->item;
			સંરચના commit_list *parents;

			જો (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				ચાલુ;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			જ્યારે (parents) {
				સંરચના commit *p = parents->item;
				parents = parents->next;
				જો (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		જો (!changed)
			તોડો;
	}
}

સ્થિર શૂન્ય show_one_commit(સંરચના commit *commit, પૂર્ણાંક no_name)
{
	સંરચના strbuf pretty = STRBUF_INIT;
	અચલ અક્ષર *pretty_str = "(unavailable)";
	સંરચના commit_name *name = commit_to_name(commit);

	જો (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	જો (!no_name) {
		જો (name && name->head_name) {
			printf("[%s", name->head_name);
			જો (name->generation) {
				જો (name->generation == 1)
					printf("^");
				નહીંતર
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		નહીંતર
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

સ્થિર અક્ષર *ref_name[MAX_REVS + 1];
સ્થિર પૂર્ણાંક ref_name_cnt;

સ્થિર અચલ અક્ષર *find_digit_prefix(અચલ અક્ષર *s, પૂર્ણાંક *v)
{
	અચલ અક્ષર *p;
	પૂર્ણાંક ver;
	અક્ષર ch;

	માટે (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	પાછા p;
}


સ્થિર પૂર્ણાંક version_cmp(અચલ અક્ષર *a, અચલ અક્ષર *b)
{
	જ્યારે (1) {
		પૂર્ણાંક va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		જો (va != vb)
			પાછા va - vb;

		જ્યારે (1) {
			પૂર્ણાંક ca = *a;
			પૂર્ણાંક cb = *b;
			જો ('0' <= ca && ca <= '9')
				ca = 0;
			જો ('0' <= cb && cb <= '9')
				cb = 0;
			જો (ca != cb)
				પાછા ca - cb;
			જો (!ca)
				તોડો;
			a++;
			b++;
		}
		જો (!*a && !*b)
			પાછા 0;
	}
}

સ્થિર પૂર્ણાંક compare_ref_name(અચલ શૂન્ય *a_, અચલ શૂન્ય *b_)
{
	અચલ અક્ષર * અચલ*a = a_, * અચલ*b = b_;
	પાછા version_cmp(*a, *b);
}

સ્થિર શૂન્ય sort_ref_range(પૂર્ણાંક bottom, પૂર્ણાંક ટોચ)
{
	QSORT(ref_name + bottom, ટોચ - bottom, compare_ref_name);
}

સ્થિર પૂર્ણાંક append_ref(અચલ અક્ષર *refname, અચલ સંરચના object_id *oid,
		      પૂર્ણાંક allow_dups)
{
	સંરચના commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	પૂર્ણાંક i;

	જો (!commit)
		પાછા 0;

	જો (!allow_dups) {
		/* Avoid adding the same thing twice */
		માટે (i = 0; i < ref_name_cnt; i++)
			જો (!strcmp(refname, ref_name[i]))
				પાછા 0;
	}
	જો (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		પાછા 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ખાલી;
	પાછા 0;
}

સ્થિર પૂર્ણાંક append_head_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	સંરચના object_id tmp;
	પૂર્ણાંક ofs = 11;
	જો (!starts_with(ref->name, "refs/heads/"))
		પાછા 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	જો (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	પાછા append_ref(ref->name + ofs, ref->oid, 0);
}

સ્થિર પૂર્ણાંક append_remote_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	સંરચના object_id tmp;
	પૂર્ણાંક ofs = 13;
	જો (!starts_with(ref->name, "refs/remotes/"))
		પાછા 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	જો (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	પાછા append_ref(ref->name + ofs, ref->oid, 0);
}

સ્થિર પૂર્ણાંક append_tag_ref(અચલ અક્ષર *refname, અચલ સંરચના object_id *oid,
			  પૂર્ણાંક flag UNUSED, શૂન્ય *cb_data UNUSED)
{
	જો (!starts_with(refname, "refs/tags/"))
		પાછા 0;
	પાછા append_ref(refname + 5, oid, 0);
}

સ્થિર અચલ અક્ષર *match_ref_pattern = ખાલી;
સ્થિર પૂર્ણાંક match_ref_slash = 0;

સ્થિર પૂર્ણાંક append_matching_ref(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	અચલ અક્ષર *tail;
	પૂર્ણાંક slash = ગણતરી_slashes(ref->name);
	માટે (tail = ref->name; *tail && match_ref_slash < slash; )
		જો (*tail++ == '/')
			slash--;
	જો (!*tail)
		પાછા 0;
	જો (wildmatch(match_ref_pattern, tail, 0))
		પાછા 0;
	જો (starts_with(ref->name, "refs/heads/"))
		પાછા append_head_ref(ref, cb_data);
	જો (starts_with(ref->name, "refs/tags/"))
		પાછા append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	પાછા append_ref(ref->name, ref->oid, 0);
}

સ્થિર શૂન્ય snarf_refs(પૂર્ણાંક head, પૂર્ણાંક remotes)
{
	જો (head) {
		પૂર્ણાંક orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, ખાલી);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	જો (remotes) {
		પૂર્ણાંક orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ખાલી);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

સ્થિર પૂર્ણાંક rev_is_head(અચલ અક્ષર *head, અચલ અક્ષર *name)
{
	જો (!head)
		પાછા 0;
	skip_prefix(head, "refs/heads/", &head);
	જો (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	પાછા !strcmp(head, name);
}

સ્થિર પૂર્ણાંક show_merge_base(અચલ સંરચના commit_list *seen, પૂર્ણાંક અંક_rev)
{
	પૂર્ણાંક all_mask = ((1u << (REV_SHIFT + અંક_rev)) - 1);
	પૂર્ણાંક all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	પૂર્ણાંક exit_status = 1;

	માટે (અચલ સંરચના commit_list *s = seen; s; s = s->next) {
		સંરચના commit *commit = s->item;
		પૂર્ણાંક flags = commit->object.flags & all_mask;
		જો (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	પાછા exit_status;
}

સ્થિર પૂર્ણાંક show_independent(સંરચના commit **rev,
			    પૂર્ણાંક અંક_rev,
			    unsigned પૂર્ણાંક *rev_mask)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < અંક_rev; i++) {
		સંરચના commit *commit = rev[i];
		unsigned પૂર્ણાંક flag = rev_mask[i];

		જો (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	પાછા 0;
}

સ્થિર શૂન્ય append_one_rev(અચલ અક્ષર *av)
{
	સંરચના object_id revkey;
	જો (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		પાછા;
	}
	જો (strpbrk(av, "*?[")) {
		/* glob style match */
		પૂર્ણાંક saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = ગણતરી_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ખાલી);
		જો (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		પાછા;
	}
	die("bad sha1 reference %s", av);
}

સ્થિર પૂર્ણાંક git_show_branch_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
				  અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "showbranch.default")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		જો (!default_args.nr)
			strvec_ધકેલો(&default_args, "show-branch");
		strvec_ધકેલો(&default_args, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, કિંમત);
		પાછા 0;
	}

	જો (git_color_config(var, કિંમત, cb) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર પૂર્ણાંક omit_in_dense(સંરચના commit *commit, સંરચના commit **rev, પૂર્ણાંક n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	પૂર્ણાંક i, flag, ગણતરી;
	માટે (i = 0; i < n; i++)
		જો (rev[i] == commit)
			પાછા 0;
	flag = commit->object.flags;
	માટે (i = ગણતરી = 0; i < n; i++) {
		જો (flag & (1u << (i + REV_SHIFT)))
			ગણતરી++;
	}
	જો (ગણતરી == 1)
		પાછા 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક reflog = 0;

સ્થિર પૂર્ણાંક parse_reflog_param(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			      પૂર્ણાંક unset)
{
	અક્ષર *ep;
	અચલ અક્ષર **base = (અચલ અક્ષર **)opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	જો (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	જો (*ep == ',')
		*base = ep + 1;
	નહીંતર જો (*ep)
		પાછા error("unrecognized reflog param '%s'", arg);
	નહીંતર
		*base = ખાલી;
	જો (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	પાછા 0;
}

પૂર્ણાંક cmd_show_branch(પૂર્ણાંક ac,
		અચલ અક્ષર **av,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	સંરચના commit *rev[MAX_REVS], *commit;
	અક્ષર *reflog_સંદેશ[MAX_REVS] = {0};
	સંરચના commit_list *seen = ખાલી;
	સંરચના prio_queue queue = { compare_commits_by_commit_date };
	unsigned પૂર્ણાંક rev_mask[MAX_REVS];
	પૂર્ણાંક અંક_rev, i, extra = 0;
	પૂર્ણાંક all_heads = 0, all_remotes = 0;
	પૂર્ણાંક all_mask, all_revs;
	ગણના rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	અક્ષર *head;
	સંરચના object_id head_oid;
	પૂર્ણાંક merge_base = 0;
	પૂર્ણાંક independent = 0;
	પૂર્ણાંક no_name = 0;
	પૂર્ણાંક sha1_name = 0;
	પૂર્ણાંક shown_merge_point = 0;
	પૂર્ણાંક with_current_branch = 0;
	પૂર્ણાંક head_at = -1;
	પૂર્ણાંક topics = 0;
	પૂર્ણાંક sparse = 0;
	અચલ અક્ષર *reflog_base = ખાલી;
	સંરચના option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.કિંમત = &extra,
			.precision = કદ(extra),
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
	અચલ અક્ષર **args_copy = ખાલી;
	પૂર્ણાંક ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ખાલી);

	/* If nothing is specified, try the default first */
	જો (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	જો (all_heads)
		all_remotes = 1;

	જો (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		જો (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		જો (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	જો (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	જો (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	જો (reflog) {
		સંરચના object_id oid;
		અક્ષર *ref;
		પૂર્ણાંક base = 0;
		unsigned પૂર્ણાંક flags = 0;

		જો (ac == 0) {
			સ્થિર અચલ અક્ષર *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ખાલી);
			fake_av[1] = ખાલી;
			av = fake_av;
			ac = 1;
			જો (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		જો (ac != 1)
			die(_("--reflog option needs one branch name"));

		જો (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		જો (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		જો (reflog_base) {
			અક્ષર *ep;
			base = strtoul(reflog_base, &ep, 10);
			જો (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ખાલી,
					    ખાલી, ખાલી, &base);
			}
		}

		માટે (i = 0; i < reflog; i++) {
			અક્ષર *logmsg = ખાલી;
			અક્ષર *nth_desc;
			અચલ અક્ષર *સંદેશ;
			અક્ષર *end;
			timestamp_t timestamp;
			પૂર્ણાંક tz;

			જો (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, ખાલી)) {
				મુક્ત(logmsg);
				reflog = i;
				તોડો;
			}

			end = strchr(logmsg, '\n');
			જો (end)
				*end = '\0';

			સંદેશ = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_સંદેશ[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						સંદેશ);
			મુક્ત(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			મુક્ત(nth_desc);
		}
		મુક્ત(ref);
	}
	નહીંતર {
		જ્યારે (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		જો (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, ખાલી);

	જો (with_current_branch && head) {
		પૂર્ણાંક has_head = 0;
		માટે (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			જો (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		જો (!has_head) {
			અચલ અક્ષર *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	જો (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		જાઓ out;
	}

	માટે (અંક_rev = 0; ref_name[અંક_rev]; અંક_rev++) {
		સંરચના object_id revkey;
		unsigned પૂર્ણાંક flag = 1u << (અંક_rev + REV_SHIFT);

		જો (MAX_REVS <= અંક_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		જો (repo_get_oid(the_repository, ref_name[અંક_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[અંક_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		જો (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[અંક_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		જો (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[અંક_rev] = commit;
	}
	માટે (i = 0; i < અંક_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	જો (0 <= extra)
		join_revs(&queue, &seen, અંક_rev, extra);

	commit_list_sort_by_date(&seen);

	જો (merge_base) {
		ret = show_merge_base(seen, અંક_rev);
		જાઓ out;
	}

	જો (independent) {
		ret = show_independent(rev, અંક_rev, rev_mask);
		જાઓ out;
	}

	/* Show list; --more=-1 means list-only */
	જો (1 < અંક_rev || extra < 0) {
		માટે (i = 0; i < અંક_rev; i++) {
			પૂર્ણાંક j;
			પૂર્ણાંક is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			જો (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			નહીંતર {
				માટે (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			જો (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			નહીંતર
				puts(reflog_સંદેશ[i]);

			જો (is_head)
				head_at = i;
		}
		જો (0 <= extra) {
			માટે (i = 0; i < અંક_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	જો (extra < 0) {
		ret = 0;
		જાઓ out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	જો (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, અંક_rev);

	all_mask = ((1u << (REV_SHIFT + અંક_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	માટે (સંરચના commit_list *l = seen; l; l = l->next) {
		સંરચના commit *commit = l->item;
		પૂર્ણાંક this_flag = commit->object.flags;
		પૂર્ણાંક is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		જો (1 < અંક_rev) {
			પૂર્ણાંક is_merge = !!(commit->parents &&
					  commit->parents->next);
			જો (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				ચાલુ;
			જો (!sparse && is_merge &&
			    omit_in_dense(commit, rev, અંક_rev))
				ચાલુ;
			માટે (i = 0; i < અંક_rev; i++) {
				પૂર્ણાંક mark;
				જો (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				નહીંતર જો (is_merge)
					mark = '-';
				નહીંતર જો (i == head_at)
					mark = '*';
				નહીંતર
					mark = '+';
				જો (mark == ' ')
					putchar(mark);
				નહીંતર
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		જો (shown_merge_point && --extra < 0)
			તોડો;
	}

	ret = 0;

out:
	માટે (size_t i = 0; i < ARRAY_SIZE(reflog_સંદેશ); i++)
		મુક્ત(reflog_સંદેશ[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	મુક્ત(args_copy);
	મુક્ત(head);
	પાછા ret;
}
