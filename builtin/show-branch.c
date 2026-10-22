#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "pretty.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"

#अन्तर्भाव "dir.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "prio-queue.h"

स्थिर स्थिरांक अक्षर*स्थिरांक show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    रिक्त
};

स्थिर गणना git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

स्थिर संरचना strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

स्थिर स्थिरांक अक्षर *get_color_code(पूर्णांक idx)
{
	जर (want_color(showbranch_use_color))
		परत column_colors_ansi[idx % column_colors_ansi_max];
	परत "";
}

स्थिर स्थिरांक अक्षर *get_color_reset_code(शून्य)
{
	जर (want_color(showbranch_use_color))
		परत GIT_COLOR_RESET;
	परत "";
}

स्थिर संरचना commit *interesting(संरचना prio_queue *queue)
{
	संरचना commit *commit;
	prio_queue_for_each(queue, commit) {
		जर (!(commit->object.flags & UNINTERESTING))
			परत commit;
	}
	परत रिक्त;
}

संरचना commit_name {
	स्थिरांक अक्षर *head_name; /* which head's ancestor? */
	पूर्णांक generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, संरचना commit_name *);
स्थिर संरचना commit_name_slab name_slab;

स्थिर संरचना commit_name *commit_to_name(संरचना commit *commit)
{
	परत *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
स्थिर शून्य name_commit(संरचना commit *commit, स्थिरांक अक्षर *head_name, पूर्णांक nth)
{
	संरचना commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	जर (!name) {
		name = xmalloc(आकार(*name));
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
स्थिर शून्य name_parent(संरचना commit *commit, संरचना commit *parent)
{
	संरचना commit_name *commit_name = commit_to_name(commit);
	संरचना commit_name *parent_name = commit_to_name(parent);
	जर (!commit_name)
		परत;
	जर (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

स्थिर पूर्णांक name_first_parent_chain(संरचना commit *c)
{
	पूर्णांक i = 0;
	पर्यंत (c) {
		संरचना commit *p;
		जर (!commit_to_name(c))
			थांबा;
		जर (!c->parents)
			थांबा;
		p = c->parents->item;
		जर (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		अन्यथा
			थांबा;
		c = p;
	}
	परत i;
}

स्थिर शून्य name_commits(संरचना commit_list *list,
			 संरचना commit **rev,
			 अक्षर **ref_name,
			 पूर्णांक अंक_rev)
{
	संरचना commit_list *cl;
	संरचना commit *c;
	पूर्णांक i;

	/* First give names to the given heads */
	साठी (cl = list; cl; cl = cl->next) {
		c = cl->item;
		जर (commit_to_name(c))
			सुरूठेवा;
		साठी (i = 0; i < अंक_rev; i++) {
			जर (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				थांबा;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	करा {
		i = 0;
		साठी (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} पर्यंत (i);

	/* Finally, any unnamed commits */
	करा {
		i = 0;
		साठी (cl = list; cl; cl = cl->next) {
			संरचना commit_list *parents;
			संरचना commit_name *n;
			पूर्णांक nth;
			c = cl->item;
			जर (!commit_to_name(c))
				सुरूठेवा;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			पर्यंत (parents) {
				संरचना commit *p = parents->item;
				संरचना strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				जर (commit_to_name(p))
					सुरूठेवा;
				निवड (n->generation) {
				स्थिती 0:
					strbuf_addstr(&newname, n->head_name);
					थांबा;
				स्थिती 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					थांबा;
				पूर्वनिर्धारित:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					थांबा;
				}
				जर (nth == 1)
					strbuf_addch(&newname, '^');
				अन्यथा
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, रिक्त), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} पर्यंत (i);
}

स्थिर पूर्णांक mark_seen(संरचना commit *commit, संरचना commit_list **seen_p)
{
	जर (!commit->object.flags) {
		commit_list_घाला(commit, seen_p);
		परत 1;
	}
	परत 0;
}

स्थिर शून्य join_revs(संरचना prio_queue *queue,
		      संरचना commit_list **seen_p,
		      पूर्णांक अंक_rev, पूर्णांक extra)
{
	पूर्णांक all_mask = ((1u << (REV_SHIFT + अंक_rev)) - 1);
	पूर्णांक all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	संरचना commit *commit;

	पर्यंत ((commit = prio_queue_peek(queue))) {
		संरचना commit_list *parents;
		पूर्णांक still_interesting = !!interesting(queue);
		पूर्णांक flags = commit->object.flags & all_mask;

		जर (!still_interesting && extra <= 0)
			थांबा;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		जर ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		पर्यंत (parents) {
			संरचना commit *p = parents->item;
			पूर्णांक this_flag = p->object.flags;
			parents = parents->next;
			जर ((this_flag & flags) == flags)
				सुरूठेवा;
			repo_parse_commit(the_repository, p);
			जर (mark_seen(p, seen_p) && !still_interesting)
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
	साठी (;;) {
		पूर्णांक changed = 0;
		संरचना commit_list *s;
		साठी (s = *seen_p; s; s = s->next) {
			संरचना commit *c = s->item;
			संरचना commit_list *parents;

			जर (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				सुरूठेवा;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			पर्यंत (parents) {
				संरचना commit *p = parents->item;
				parents = parents->next;
				जर (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		जर (!changed)
			थांबा;
	}
}

स्थिर शून्य show_one_commit(संरचना commit *commit, पूर्णांक no_name)
{
	संरचना strbuf pretty = STRBUF_INIT;
	स्थिरांक अक्षर *pretty_str = "(unavailable)";
	संरचना commit_name *name = commit_to_name(commit);

	जर (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	जर (!no_name) {
		जर (name && name->head_name) {
			म_लिखो("[%s", name->head_name);
			जर (name->generation) {
				जर (name->generation == 1)
					म_लिखो("^");
				अन्यथा
					म_लिखो("~%d", name->generation);
			}
			म_लिखो("] ");
		}
		अन्यथा
			म_लिखो("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	माला_दो(pretty_str);
	strbuf_release(&pretty);
}

स्थिर अक्षर *ref_name[MAX_REVS + 1];
स्थिर पूर्णांक ref_name_cnt;

स्थिर स्थिरांक अक्षर *find_digit_prefix(स्थिरांक अक्षर *s, पूर्णांक *v)
{
	स्थिरांक अक्षर *p;
	पूर्णांक ver;
	अक्षर ch;

	साठी (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	परत p;
}


स्थिर पूर्णांक version_cmp(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b)
{
	पर्यंत (1) {
		पूर्णांक va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		जर (va != vb)
			परत va - vb;

		पर्यंत (1) {
			पूर्णांक ca = *a;
			पूर्णांक cb = *b;
			जर ('0' <= ca && ca <= '9')
				ca = 0;
			जर ('0' <= cb && cb <= '9')
				cb = 0;
			जर (ca != cb)
				परत ca - cb;
			जर (!ca)
				थांबा;
			a++;
			b++;
		}
		जर (!*a && !*b)
			परत 0;
	}
}

स्थिर पूर्णांक compare_ref_name(स्थिरांक शून्य *a_, स्थिरांक शून्य *b_)
{
	स्थिरांक अक्षर * स्थिरांक*a = a_, * स्थिरांक*b = b_;
	परत version_cmp(*a, *b);
}

स्थिर शून्य sort_ref_range(पूर्णांक bottom, पूर्णांक शीर्ष)
{
	QSORT(ref_name + bottom, शीर्ष - bottom, compare_ref_name);
}

स्थिर पूर्णांक append_ref(स्थिरांक अक्षर *refname, स्थिरांक संरचना object_id *oid,
		      पूर्णांक allow_dups)
{
	संरचना commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	पूर्णांक i;

	जर (!commit)
		परत 0;

	जर (!allow_dups) {
		/* Avoid adding the same thing twice */
		साठी (i = 0; i < ref_name_cnt; i++)
			जर (!म_भेद(refname, ref_name[i]))
				परत 0;
	}
	जर (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		परत 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = रिक्त;
	परत 0;
}

स्थिर पूर्णांक append_head_ref(स्थिरांक संरचना reference *ref, शून्य *cb_data UNUSED)
{
	संरचना object_id tmp;
	पूर्णांक ofs = 11;
	जर (!starts_with(ref->name, "refs/heads/"))
		परत 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	जर (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	परत append_ref(ref->name + ofs, ref->oid, 0);
}

स्थिर पूर्णांक append_remote_ref(स्थिरांक संरचना reference *ref, शून्य *cb_data UNUSED)
{
	संरचना object_id tmp;
	पूर्णांक ofs = 13;
	जर (!starts_with(ref->name, "refs/remotes/"))
		परत 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	जर (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	परत append_ref(ref->name + ofs, ref->oid, 0);
}

स्थिर पूर्णांक append_tag_ref(स्थिरांक अक्षर *refname, स्थिरांक संरचना object_id *oid,
			  पूर्णांक flag UNUSED, शून्य *cb_data UNUSED)
{
	जर (!starts_with(refname, "refs/tags/"))
		परत 0;
	परत append_ref(refname + 5, oid, 0);
}

स्थिर स्थिरांक अक्षर *match_ref_pattern = रिक्त;
स्थिर पूर्णांक match_ref_slash = 0;

स्थिर पूर्णांक append_matching_ref(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	स्थिरांक अक्षर *tail;
	पूर्णांक slash = मोजणी_slashes(ref->name);
	साठी (tail = ref->name; *tail && match_ref_slash < slash; )
		जर (*tail++ == '/')
			slash--;
	जर (!*tail)
		परत 0;
	जर (wildmatch(match_ref_pattern, tail, 0))
		परत 0;
	जर (starts_with(ref->name, "refs/heads/"))
		परत append_head_ref(ref, cb_data);
	जर (starts_with(ref->name, "refs/tags/"))
		परत append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	परत append_ref(ref->name, ref->oid, 0);
}

स्थिर शून्य snarf_refs(पूर्णांक head, पूर्णांक remotes)
{
	जर (head) {
		पूर्णांक orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, रिक्त);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	जर (remotes) {
		पूर्णांक orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, रिक्त);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

स्थिर पूर्णांक rev_is_head(स्थिरांक अक्षर *head, स्थिरांक अक्षर *name)
{
	जर (!head)
		परत 0;
	skip_prefix(head, "refs/heads/", &head);
	जर (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	परत !म_भेद(head, name);
}

स्थिर पूर्णांक show_merge_base(स्थिरांक संरचना commit_list *seen, पूर्णांक अंक_rev)
{
	पूर्णांक all_mask = ((1u << (REV_SHIFT + अंक_rev)) - 1);
	पूर्णांक all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	पूर्णांक exit_status = 1;

	साठी (स्थिरांक संरचना commit_list *s = seen; s; s = s->next) {
		संरचना commit *commit = s->item;
		पूर्णांक flags = commit->object.flags & all_mask;
		जर (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			माला_दो(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	परत exit_status;
}

स्थिर पूर्णांक show_independent(संरचना commit **rev,
			    पूर्णांक अंक_rev,
			    अचिन्हित पूर्णांक *rev_mask)
{
	पूर्णांक i;

	साठी (i = 0; i < अंक_rev; i++) {
		संरचना commit *commit = rev[i];
		अचिन्हित पूर्णांक flag = rev_mask[i];

		जर (commit->object.flags == flag)
			माला_दो(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	परत 0;
}

स्थिर शून्य append_one_rev(स्थिरांक अक्षर *av)
{
	संरचना object_id revkey;
	जर (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		परत;
	}
	जर (strpbrk(av, "*?[")) {
		/* glob style match */
		पूर्णांक saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = मोजणी_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, रिक्त);
		जर (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		परत;
	}
	die("bad sha1 reference %s", av);
}

स्थिर पूर्णांक git_show_branch_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
				  स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (!म_भेद(var, "showbranch.default")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		जर (!default_args.nr)
			strvec_ढकला(&default_args, "show-branch");
		strvec_ढकला(&default_args, मूल्य);
		परत 0;
	}

	जर (!म_भेद(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, मूल्य);
		परत 0;
	}

	जर (git_color_config(var, मूल्य, cb) < 0)
		परत -1;

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर पूर्णांक omit_in_dense(संरचना commit *commit, संरचना commit **rev, पूर्णांक n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	पूर्णांक i, flag, मोजणी;
	साठी (i = 0; i < n; i++)
		जर (rev[i] == commit)
			परत 0;
	flag = commit->object.flags;
	साठी (i = मोजणी = 0; i < n; i++) {
		जर (flag & (1u << (i + REV_SHIFT)))
			मोजणी++;
	}
	जर (मोजणी == 1)
		परत 1;
	परत 0;
}

स्थिर पूर्णांक reflog = 0;

स्थिर पूर्णांक parse_reflog_param(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
			      पूर्णांक unset)
{
	अक्षर *ep;
	स्थिरांक अक्षर **base = (स्थिरांक अक्षर **)opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	जर (!arg)
		arg = "";
	reflog = म_से_अदीर्घ(arg, &ep, 10);
	जर (*ep == ',')
		*base = ep + 1;
	अन्यथा जर (*ep)
		परत error("unrecognized reflog param '%s'", arg);
	अन्यथा
		*base = रिक्त;
	जर (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	परत 0;
}

पूर्णांक cmd_show_branch(पूर्णांक ac,
		स्थिरांक अक्षर **av,
		स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना commit *rev[MAX_REVS], *commit;
	अक्षर *reflog_संदेश[MAX_REVS] = {0};
	संरचना commit_list *seen = रिक्त;
	संरचना prio_queue queue = { compare_commits_by_commit_date };
	अचिन्हित पूर्णांक rev_mask[MAX_REVS];
	पूर्णांक अंक_rev, i, extra = 0;
	पूर्णांक all_heads = 0, all_remotes = 0;
	पूर्णांक all_mask, all_revs;
	गणना rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	अक्षर *head;
	संरचना object_id head_oid;
	पूर्णांक merge_base = 0;
	पूर्णांक independent = 0;
	पूर्णांक no_name = 0;
	पूर्णांक sha1_name = 0;
	पूर्णांक shown_merge_point = 0;
	पूर्णांक with_current_branch = 0;
	पूर्णांक head_at = -1;
	पूर्णांक topics = 0;
	पूर्णांक sparse = 0;
	स्थिरांक अक्षर *reflog_base = रिक्त;
	संरचना option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.मूल्य = &extra,
			.precision = आकार(extra),
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
	स्थिरांक अक्षर **args_copy = रिक्त;
	पूर्णांक ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, रिक्त);

	/* If nothing is specified, try the default first */
	जर (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	जर (all_heads)
		all_remotes = 1;

	जर (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		जर (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		जर (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	जर (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	जर (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	जर (reflog) {
		संरचना object_id oid;
		अक्षर *ref;
		पूर्णांक base = 0;
		अचिन्हित पूर्णांक flags = 0;

		जर (ac == 0) {
			स्थिर स्थिरांक अक्षर *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 रिक्त);
			fake_av[1] = रिक्त;
			av = fake_av;
			ac = 1;
			जर (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		जर (ac != 1)
			die(_("--reflog option needs one branch name"));

		जर (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		जर (!repo_dwim_ref(the_repository, *av, म_माप(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		जर (reflog_base) {
			अक्षर *ep;
			base = म_से_अदीर्घ(reflog_base, &ep, 10);
			जर (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, रिक्त,
					    रिक्त, रिक्त, &base);
			}
		}

		साठी (i = 0; i < reflog; i++) {
			अक्षर *logmsg = रिक्त;
			अक्षर *nth_desc;
			स्थिरांक अक्षर *संदेश;
			अक्षर *end;
			timestamp_t timestamp;
			पूर्णांक tz;

			जर (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, रिक्त)) {
				मुक्त(logmsg);
				reflog = i;
				थांबा;
			}

			end = म_अक्षर(logmsg, '\n');
			जर (end)
				*end = '\0';

			संदेश = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_संदेश[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						संदेश);
			मुक्त(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			मुक्त(nth_desc);
		}
		मुक्त(ref);
	}
	अन्यथा {
		पर्यंत (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		जर (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, रिक्त);

	जर (with_current_branch && head) {
		पूर्णांक has_head = 0;
		साठी (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			जर (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		जर (!has_head) {
			स्थिरांक अक्षर *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	जर (!ref_name_cnt) {
		ख_लिखो(मानक_त्रुटि, "No revs to be shown.\n");
		ret = 0;
		जा out;
	}

	साठी (अंक_rev = 0; ref_name[अंक_rev]; अंक_rev++) {
		संरचना object_id revkey;
		अचिन्हित पूर्णांक flag = 1u << (अंक_rev + REV_SHIFT);

		जर (MAX_REVS <= अंक_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		जर (repo_get_oid(the_repository, ref_name[अंक_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[अंक_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		जर (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[अंक_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		जर (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[अंक_rev] = commit;
	}
	साठी (i = 0; i < अंक_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	जर (0 <= extra)
		join_revs(&queue, &seen, अंक_rev, extra);

	commit_list_sort_by_date(&seen);

	जर (merge_base) {
		ret = show_merge_base(seen, अंक_rev);
		जा out;
	}

	जर (independent) {
		ret = show_independent(rev, अंक_rev, rev_mask);
		जा out;
	}

	/* Show list; --more=-1 means list-only */
	जर (1 < अंक_rev || extra < 0) {
		साठी (i = 0; i < अंक_rev; i++) {
			पूर्णांक j;
			पूर्णांक is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			जर (extra < 0)
				म_लिखो("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			अन्यथा {
				साठी (j = 0; j < i; j++)
					अक्षर_दो(' ');
				म_लिखो("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			जर (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			अन्यथा
				माला_दो(reflog_संदेश[i]);

			जर (is_head)
				head_at = i;
		}
		जर (0 <= extra) {
			साठी (i = 0; i < अंक_rev; i++)
				अक्षर_दो('-');
			अक्षर_दो('\n');
		}
	}
	जर (extra < 0) {
		ret = 0;
		जा out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	जर (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, अंक_rev);

	all_mask = ((1u << (REV_SHIFT + अंक_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	साठी (संरचना commit_list *l = seen; l; l = l->next) {
		संरचना commit *commit = l->item;
		पूर्णांक this_flag = commit->object.flags;
		पूर्णांक is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		जर (1 < अंक_rev) {
			पूर्णांक is_merge = !!(commit->parents &&
					  commit->parents->next);
			जर (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				सुरूठेवा;
			जर (!sparse && is_merge &&
			    omit_in_dense(commit, rev, अंक_rev))
				सुरूठेवा;
			साठी (i = 0; i < अंक_rev; i++) {
				पूर्णांक mark;
				जर (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				अन्यथा जर (is_merge)
					mark = '-';
				अन्यथा जर (i == head_at)
					mark = '*';
				अन्यथा
					mark = '+';
				जर (mark == ' ')
					अक्षर_दो(mark);
				अन्यथा
					म_लिखो("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			अक्षर_दो(' ');
		}
		show_one_commit(commit, no_name);

		जर (shown_merge_point && --extra < 0)
			थांबा;
	}

	ret = 0;

out:
	साठी (माप_प्रकार i = 0; i < ARRAY_SIZE(reflog_संदेश); i++)
		मुक्त(reflog_संदेश[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	मुक्त(args_copy);
	मुक्त(head);
	परत ret;
}
