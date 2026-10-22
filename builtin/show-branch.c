#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "pretty.h"
#समावेश "refs.h"
#समावेश "color.h"
#समावेश "strvec.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"

#समावेश "dir.h"
#समावेश "commit-slab.h"
#समावेश "date.h"
#समावेश "wildmatch.h"
#समावेश "prio-queue.h"

अटल स्थिर अक्षर*स्थिर show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    शून्य
};

अटल क्रमागत git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

अटल काष्ठा strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#घोषणा UNINTERESTING	01

#घोषणा REV_SHIFT	 2
#घोषणा MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#घोषणा DEFAULT_REFLOG	4

अटल स्थिर अक्षर *get_color_code(पूर्णांक idx)
{
	अगर (want_color(showbranch_use_color))
		वापस column_colors_ansi[idx % column_colors_ansi_max];
	वापस "";
}

अटल स्थिर अक्षर *get_color_reset_code(व्योम)
{
	अगर (want_color(showbranch_use_color))
		वापस GIT_COLOR_RESET;
	वापस "";
}

अटल काष्ठा commit *interesting(काष्ठा prio_queue *queue)
{
	काष्ठा commit *commit;
	prio_queue_for_each(queue, commit) {
		अगर (!(commit->object.flags & UNINTERESTING))
			वापस commit;
	}
	वापस शून्य;
}

काष्ठा commit_name {
	स्थिर अक्षर *शीर्ष_name; /* which head's ancestor? */
	पूर्णांक generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, काष्ठा commit_name *);
अटल काष्ठा commit_name_slab name_slab;

अटल काष्ठा commit_name *commit_to_name(काष्ठा commit *commit)
{
	वापस *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
अटल व्योम name_commit(काष्ठा commit *commit, स्थिर अक्षर *शीर्ष_name, पूर्णांक nth)
{
	काष्ठा commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	अगर (!name) {
		name = xmalloc(माप(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->शीर्ष_name = शीर्ष_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
अटल व्योम name_parent(काष्ठा commit *commit, काष्ठा commit *parent)
{
	काष्ठा commit_name *commit_name = commit_to_name(commit);
	काष्ठा commit_name *parent_name = commit_to_name(parent);
	अगर (!commit_name)
		वापस;
	अगर (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->शीर्ष_name,
			    commit_name->generation + 1);
}

अटल पूर्णांक name_first_parent_chain(काष्ठा commit *c)
{
	पूर्णांक i = 0;
	जबतक (c) {
		काष्ठा commit *p;
		अगर (!commit_to_name(c))
			अवरोध;
		अगर (!c->parents)
			अवरोध;
		p = c->parents->item;
		अगर (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		अन्यथा
			अवरोध;
		c = p;
	}
	वापस i;
}

अटल व्योम name_commits(काष्ठा commit_list *list,
			 काष्ठा commit **rev,
			 अक्षर **ref_name,
			 पूर्णांक अंक_rev)
{
	काष्ठा commit_list *cl;
	काष्ठा commit *c;
	पूर्णांक i;

	/* First give names to the given heads */
	क्रम (cl = list; cl; cl = cl->अग्र) {
		c = cl->item;
		अगर (commit_to_name(c))
			जारी;
		क्रम (i = 0; i < अंक_rev; i++) {
			अगर (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				अवरोध;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	करो {
		i = 0;
		क्रम (cl = list; cl; cl = cl->अग्र) {
			i += name_first_parent_chain(cl->item);
		}
	} जबतक (i);

	/* Finally, any unnamed commits */
	करो {
		i = 0;
		क्रम (cl = list; cl; cl = cl->अग्र) {
			काष्ठा commit_list *parents;
			काष्ठा commit_name *n;
			पूर्णांक nth;
			c = cl->item;
			अगर (!commit_to_name(c))
				जारी;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			जबतक (parents) {
				काष्ठा commit *p = parents->item;
				काष्ठा strbuf newname = STRBUF_INIT;
				parents = parents->अग्र;
				nth++;
				अगर (commit_to_name(p))
					जारी;
				चयन (n->generation) {
				हाल 0:
					strbuf_addstr(&newname, n->शीर्ष_name);
					अवरोध;
				हाल 1:
					strbuf_addf(&newname, "%s^", n->शीर्ष_name);
					अवरोध;
				शेष:
					strbuf_addf(&newname, "%s~%d",
						    n->शीर्ष_name, n->generation);
					अवरोध;
				}
				अगर (nth == 1)
					strbuf_addch(&newname, '^');
				अन्यथा
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, शून्य), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} जबतक (i);
}

अटल पूर्णांक mark_seen(काष्ठा commit *commit, काष्ठा commit_list **seen_p)
{
	अगर (!commit->object.flags) {
		commit_list_जोड़ो(commit, seen_p);
		वापस 1;
	}
	वापस 0;
}

अटल व्योम join_revs(काष्ठा prio_queue *queue,
		      काष्ठा commit_list **seen_p,
		      पूर्णांक अंक_rev, पूर्णांक extra)
{
	पूर्णांक all_mask = ((1u << (REV_SHIFT + अंक_rev)) - 1);
	पूर्णांक all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	काष्ठा commit *commit;

	जबतक ((commit = prio_queue_peek(queue))) {
		काष्ठा commit_list *parents;
		पूर्णांक still_interesting = !!interesting(queue);
		पूर्णांक flags = commit->object.flags & all_mask;

		अगर (!still_interesting && extra <= 0)
			अवरोध;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		अगर ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		जबतक (parents) {
			काष्ठा commit *p = parents->item;
			पूर्णांक this_flag = p->object.flags;
			parents = parents->अग्र;
			अगर ((this_flag & flags) == flags)
				जारी;
			repo_parse_commit(the_repository, p);
			अगर (mark_seen(p, seen_p) && !still_interesting)
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
	क्रम (;;) {
		पूर्णांक changed = 0;
		काष्ठा commit_list *s;
		क्रम (s = *seen_p; s; s = s->अग्र) {
			काष्ठा commit *c = s->item;
			काष्ठा commit_list *parents;

			अगर (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				जारी;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			जबतक (parents) {
				काष्ठा commit *p = parents->item;
				parents = parents->अग्र;
				अगर (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		अगर (!changed)
			अवरोध;
	}
}

अटल व्योम show_one_commit(काष्ठा commit *commit, पूर्णांक no_name)
{
	काष्ठा strbuf pretty = STRBUF_INIT;
	स्थिर अक्षर *pretty_str = "(unavailable)";
	काष्ठा commit_name *name = commit_to_name(commit);

	अगर (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	अगर (!no_name) {
		अगर (name && name->शीर्ष_name) {
			म_लिखो("[%s", name->शीर्ष_name);
			अगर (name->generation) {
				अगर (name->generation == 1)
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

अटल अक्षर *ref_name[MAX_REVS + 1];
अटल पूर्णांक ref_name_cnt;

अटल स्थिर अक्षर *find_digit_prefix(स्थिर अक्षर *s, पूर्णांक *v)
{
	स्थिर अक्षर *p;
	पूर्णांक ver;
	अक्षर ch;

	क्रम (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	वापस p;
}


अटल पूर्णांक version_cmp(स्थिर अक्षर *a, स्थिर अक्षर *b)
{
	जबतक (1) {
		पूर्णांक va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		अगर (va != vb)
			वापस va - vb;

		जबतक (1) {
			पूर्णांक ca = *a;
			पूर्णांक cb = *b;
			अगर ('0' <= ca && ca <= '9')
				ca = 0;
			अगर ('0' <= cb && cb <= '9')
				cb = 0;
			अगर (ca != cb)
				वापस ca - cb;
			अगर (!ca)
				अवरोध;
			a++;
			b++;
		}
		अगर (!*a && !*b)
			वापस 0;
	}
}

अटल पूर्णांक compare_ref_name(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	स्थिर अक्षर * स्थिर*a = a_, * स्थिर*b = b_;
	वापस version_cmp(*a, *b);
}

अटल व्योम sort_ref_range(पूर्णांक bottom, पूर्णांक शीर्ष_2)
{
	QSORT(ref_name + bottom, शीर्ष_2 - bottom, compare_ref_name);
}

अटल पूर्णांक append_ref(स्थिर अक्षर *refname, स्थिर काष्ठा object_id *oid,
		      पूर्णांक allow_dups)
{
	काष्ठा commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	पूर्णांक i;

	अगर (!commit)
		वापस 0;

	अगर (!allow_dups) {
		/* Avoid adding the same thing twice */
		क्रम (i = 0; i < ref_name_cnt; i++)
			अगर (!म_भेद(refname, ref_name[i]))
				वापस 0;
	}
	अगर (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		वापस 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = शून्य;
	वापस 0;
}

अटल पूर्णांक append_शीर्ष_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	काष्ठा object_id tmp;
	पूर्णांक ofs = 11;
	अगर (!starts_with(ref->name, "refs/heads/"))
		वापस 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	अगर (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	वापस append_ref(ref->name + ofs, ref->oid, 0);
}

अटल पूर्णांक append_remote_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	काष्ठा object_id tmp;
	पूर्णांक ofs = 13;
	अगर (!starts_with(ref->name, "refs/remotes/"))
		वापस 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	अगर (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	वापस append_ref(ref->name + ofs, ref->oid, 0);
}

अटल पूर्णांक append_tag_ref(स्थिर अक्षर *refname, स्थिर काष्ठा object_id *oid,
			  पूर्णांक flag UNUSED, व्योम *cb_तथ्य UNUSED)
{
	अगर (!starts_with(refname, "refs/tags/"))
		वापस 0;
	वापस append_ref(refname + 5, oid, 0);
}

अटल स्थिर अक्षर *match_ref_pattern = शून्य;
अटल पूर्णांक match_ref_slash = 0;

अटल पूर्णांक append_matching_ref(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	स्थिर अक्षर *पुच्छ;
	पूर्णांक slash = गिनती_slashes(ref->name);
	क्रम (पुच्छ = ref->name; *पुच्छ && match_ref_slash < slash; )
		अगर (*पुच्छ++ == '/')
			slash--;
	अगर (!*पुच्छ)
		वापस 0;
	अगर (wildmatch(match_ref_pattern, पुच्छ, 0))
		वापस 0;
	अगर (starts_with(ref->name, "refs/heads/"))
		वापस append_शीर्ष_ref(ref, cb_तथ्य);
	अगर (starts_with(ref->name, "refs/tags/"))
		वापस append_tag_ref(ref->name, ref->oid, ref->flags, cb_तथ्य);
	वापस append_ref(ref->name, ref->oid, 0);
}

अटल व्योम snarf_refs(पूर्णांक शीर्ष, पूर्णांक remotes)
{
	अगर (शीर्ष) {
		पूर्णांक orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_शीर्ष_ref, शून्य);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	अगर (remotes) {
		पूर्णांक orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, शून्य);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

अटल पूर्णांक rev_is_शीर्ष(स्थिर अक्षर *शीर्ष, स्थिर अक्षर *name)
{
	अगर (!शीर्ष)
		वापस 0;
	skip_prefix(शीर्ष, "refs/heads/", &शीर्ष);
	अगर (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	वापस !म_भेद(शीर्ष, name);
}

अटल पूर्णांक show_merge_base(स्थिर काष्ठा commit_list *seen, पूर्णांक अंक_rev)
{
	पूर्णांक all_mask = ((1u << (REV_SHIFT + अंक_rev)) - 1);
	पूर्णांक all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	पूर्णांक exit_status = 1;

	क्रम (स्थिर काष्ठा commit_list *s = seen; s; s = s->अग्र) {
		काष्ठा commit *commit = s->item;
		पूर्णांक flags = commit->object.flags & all_mask;
		अगर (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			माला_दो(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	वापस exit_status;
}

अटल पूर्णांक show_independent(काष्ठा commit **rev,
			    पूर्णांक अंक_rev,
			    अचिन्हित पूर्णांक *rev_mask)
{
	पूर्णांक i;

	क्रम (i = 0; i < अंक_rev; i++) {
		काष्ठा commit *commit = rev[i];
		अचिन्हित पूर्णांक flag = rev_mask[i];

		अगर (commit->object.flags == flag)
			माला_दो(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	वापस 0;
}

अटल व्योम append_one_rev(स्थिर अक्षर *av)
{
	काष्ठा object_id revkey;
	अगर (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		वापस;
	}
	अगर (strpbrk(av, "*?[")) {
		/* glob style match */
		पूर्णांक saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = गिनती_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, शून्य);
		अगर (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		वापस;
	}
	die("bad sha1 reference %s", av);
}

अटल पूर्णांक git_show_branch_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
				  स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "showbranch.default")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		अगर (!default_args.nr)
			strvec_धकेलो(&default_args, "show-branch");
		strvec_धकेलो(&default_args, मूल्य);
		वापस 0;
	}

	अगर (!म_भेद(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, मूल्य);
		वापस 0;
	}

	अगर (git_color_config(var, मूल्य, cb) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल पूर्णांक omit_in_dense(काष्ठा commit *commit, काष्ठा commit **rev, पूर्णांक n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	पूर्णांक i, flag, गिनती;
	क्रम (i = 0; i < n; i++)
		अगर (rev[i] == commit)
			वापस 0;
	flag = commit->object.flags;
	क्रम (i = गिनती = 0; i < n; i++) {
		अगर (flag & (1u << (i + REV_SHIFT)))
			गिनती++;
	}
	अगर (गिनती == 1)
		वापस 1;
	वापस 0;
}

अटल पूर्णांक reflog = 0;

अटल पूर्णांक parse_reflog_param(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			      पूर्णांक unset)
{
	अक्षर *ep;
	स्थिर अक्षर **base = (स्थिर अक्षर **)opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	अगर (!arg)
		arg = "";
	reflog = म_से_अदीर्घ(arg, &ep, 10);
	अगर (*ep == ',')
		*base = ep + 1;
	अन्यथा अगर (*ep)
		वापस error("unrecognized reflog param '%s'", arg);
	अन्यथा
		*base = शून्य;
	अगर (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	वापस 0;
}

पूर्णांक cmd_show_branch(पूर्णांक ac,
		स्थिर अक्षर **av,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	काष्ठा commit *rev[MAX_REVS], *commit;
	अक्षर *reflog_संदेश[MAX_REVS] = {0};
	काष्ठा commit_list *seen = शून्य;
	काष्ठा prio_queue queue = { compare_commits_by_commit_date };
	अचिन्हित पूर्णांक rev_mask[MAX_REVS];
	पूर्णांक अंक_rev, i, extra = 0;
	पूर्णांक all_heads = 0, all_remotes = 0;
	पूर्णांक all_mask, all_revs;
	क्रमागत rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	अक्षर *शीर्ष;
	काष्ठा object_id शीर्ष_oid;
	पूर्णांक merge_base = 0;
	पूर्णांक independent = 0;
	पूर्णांक no_name = 0;
	पूर्णांक sha1_name = 0;
	पूर्णांक shown_merge_point = 0;
	पूर्णांक with_current_branch = 0;
	पूर्णांक शीर्ष_at = -1;
	पूर्णांक topics = 0;
	पूर्णांक sparse = 0;
	स्थिर अक्षर *reflog_base = शून्य;
	काष्ठा option builtin_show_branch_options[] = {
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
			.precision = माप(extra),
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
	स्थिर अक्षर **args_copy = शून्य;
	पूर्णांक ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, शून्य);

	/* If nothing is specified, try the default first */
	अगर (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	अगर (all_heads)
		all_remotes = 1;

	अगर (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		अगर (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		अगर (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	अगर (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	अगर (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	अगर (reflog) {
		काष्ठा object_id oid;
		अक्षर *ref;
		पूर्णांक base = 0;
		अचिन्हित पूर्णांक flags = 0;

		अगर (ac == 0) {
			अटल स्थिर अक्षर *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 शून्य);
			fake_av[1] = शून्य;
			av = fake_av;
			ac = 1;
			अगर (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		अगर (ac != 1)
			die(_("--reflog option needs one branch name"));

		अगर (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		अगर (!repo_dwim_ref(the_repository, *av, म_माप(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		अगर (reflog_base) {
			अक्षर *ep;
			base = म_से_अदीर्घ(reflog_base, &ep, 10);
			अगर (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, शून्य,
					    शून्य, शून्य, &base);
			}
		}

		क्रम (i = 0; i < reflog; i++) {
			अक्षर *logmsg = शून्य;
			अक्षर *nth_desc;
			स्थिर अक्षर *संदेश;
			अक्षर *end;
			timestamp_t timestamp;
			पूर्णांक tz;

			अगर (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, शून्य)) {
				मुक्त(logmsg);
				reflog = i;
				अवरोध;
			}

			end = म_अक्षर(logmsg, '\n');
			अगर (end)
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
		जबतक (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		अगर (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	शीर्ष = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &शीर्ष_oid, शून्य);

	अगर (with_current_branch && शीर्ष) {
		पूर्णांक has_शीर्ष = 0;
		क्रम (i = 0; !has_शीर्ष && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			अगर (rev_is_शीर्ष(शीर्ष, ref_name[i]))
				has_शीर्ष++;
		}
		अगर (!has_शीर्ष) {
			स्थिर अक्षर *name = शीर्ष;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	अगर (!ref_name_cnt) {
		ख_लिखो(मानक_त्रुटि, "No revs to be shown.\n");
		ret = 0;
		जाओ out;
	}

	क्रम (अंक_rev = 0; ref_name[अंक_rev]; अंक_rev++) {
		काष्ठा object_id revkey;
		अचिन्हित पूर्णांक flag = 1u << (अंक_rev + REV_SHIFT);

		अगर (MAX_REVS <= अंक_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		अगर (repo_get_oid(the_repository, ref_name[अंक_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[अंक_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		अगर (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[अंक_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		अगर (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[अंक_rev] = commit;
	}
	क्रम (i = 0; i < अंक_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	अगर (0 <= extra)
		join_revs(&queue, &seen, अंक_rev, extra);

	commit_list_sort_by_date(&seen);

	अगर (merge_base) {
		ret = show_merge_base(seen, अंक_rev);
		जाओ out;
	}

	अगर (independent) {
		ret = show_independent(rev, अंक_rev, rev_mask);
		जाओ out;
	}

	/* Show list; --more=-1 means list-only */
	अगर (1 < अंक_rev || extra < 0) {
		क्रम (i = 0; i < अंक_rev; i++) {
			पूर्णांक j;
			पूर्णांक is_शीर्ष = rev_is_शीर्ष(शीर्ष, ref_name[i]) &&
				      oideq(&शीर्ष_oid, &rev[i]->object.oid);
			अगर (extra < 0)
				म_लिखो("%c [%s] ",
				       is_शीर्ष ? '*' : ' ', ref_name[i]);
			अन्यथा {
				क्रम (j = 0; j < i; j++)
					अक्षर_दो(' ');
				म_लिखो("%s%c%s [%s] ",
				       get_color_code(i),
				       is_शीर्ष ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			अगर (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			अन्यथा
				माला_दो(reflog_संदेश[i]);

			अगर (is_शीर्ष)
				शीर्ष_at = i;
		}
		अगर (0 <= extra) {
			क्रम (i = 0; i < अंक_rev; i++)
				अक्षर_दो('-');
			अक्षर_दो('\n');
		}
	}
	अगर (extra < 0) {
		ret = 0;
		जाओ out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	अगर (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, अंक_rev);

	all_mask = ((1u << (REV_SHIFT + अंक_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	क्रम (काष्ठा commit_list *l = seen; l; l = l->अग्र) {
		काष्ठा commit *commit = l->item;
		पूर्णांक this_flag = commit->object.flags;
		पूर्णांक is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		अगर (1 < अंक_rev) {
			पूर्णांक is_merge = !!(commit->parents &&
					  commit->parents->अग्र);
			अगर (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				जारी;
			अगर (!sparse && is_merge &&
			    omit_in_dense(commit, rev, अंक_rev))
				जारी;
			क्रम (i = 0; i < अंक_rev; i++) {
				पूर्णांक mark;
				अगर (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				अन्यथा अगर (is_merge)
					mark = '-';
				अन्यथा अगर (i == शीर्ष_at)
					mark = '*';
				अन्यथा
					mark = '+';
				अगर (mark == ' ')
					अक्षर_दो(mark);
				अन्यथा
					म_लिखो("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			अक्षर_दो(' ');
		}
		show_one_commit(commit, no_name);

		अगर (shown_merge_point && --extra < 0)
			अवरोध;
	}

	ret = 0;

out:
	क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(reflog_संदेश); i++)
		मुक्त(reflog_संदेश[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	मुक्त(args_copy);
	मुक्त(शीर्ष);
	वापस ret;
}
