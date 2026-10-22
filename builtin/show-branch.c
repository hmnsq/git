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

स्थिर ध्रुव वर्ण*ध्रुव show_branch_usage[] = {
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

स्थिर ध्रुव वर्ण *get_color_code(पूर्णाङ्क idx)
{
	जइ (want_color(showbranch_use_color))
		प्रत्यागम column_colors_ansi[idx % column_colors_ansi_max];
	प्रत्यागम "";
}

स्थिर ध्रुव वर्ण *get_color_reset_code(शून्य)
{
	जइ (want_color(showbranch_use_color))
		प्रत्यागम GIT_COLOR_RESET;
	प्रत्यागम "";
}

स्थिर संरचना commit *interesting(संरचना prio_queue *queue)
{
	संरचना commit *commit;
	prio_queue_for_each(queue, commit) {
		जइ (!(commit->object.flags & UNINTERESTING))
			प्रत्यागम commit;
	}
	प्रत्यागम रिक्त;
}

संरचना commit_name {
	ध्रुव वर्ण *head_name; /* which head's ancestor? */
	पूर्णाङ्क generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, संरचना commit_name *);
स्थिर संरचना commit_name_slab name_slab;

स्थिर संरचना commit_name *commit_to_name(संरचना commit *commit)
{
	प्रत्यागम *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
स्थिर शून्य name_commit(संरचना commit *commit, ध्रुव वर्ण *head_name, पूर्णाङ्क nth)
{
	संरचना commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	जइ (!name) {
		name = xmalloc(परिमाण(*name));
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
	जइ (!commit_name)
		प्रत्यागम;
	जइ (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

स्थिर पूर्णाङ्क name_first_parent_chain(संरचना commit *c)
{
	पूर्णाङ्क i = 0;
	जाव (c) {
		संरचना commit *p;
		जइ (!commit_to_name(c))
			भङ्ग;
		जइ (!c->parents)
			भङ्ग;
		p = c->parents->item;
		जइ (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		अण्णहा
			भङ्ग;
		c = p;
	}
	प्रत्यागम i;
}

स्थिर शून्य name_commits(संरचना commit_list *list,
			 संरचना commit **rev,
			 वर्ण **ref_name,
			 पूर्णाङ्क अङ्कः_rev)
{
	संरचना commit_list *cl;
	संरचना commit *c;
	पूर्णाङ्क i;

	/* First give names to the given heads */
	प्रति (cl = list; cl; cl = cl->next) {
		c = cl->item;
		जइ (commit_to_name(c))
			अनुवर्त;
		प्रति (i = 0; i < अङ्कः_rev; i++) {
			जइ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				भङ्ग;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	करेहि {
		i = 0;
		प्रति (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} जाव (i);

	/* Finally, any unnamed commits */
	करेहि {
		i = 0;
		प्रति (cl = list; cl; cl = cl->next) {
			संरचना commit_list *parents;
			संरचना commit_name *n;
			पूर्णाङ्क nth;
			c = cl->item;
			जइ (!commit_to_name(c))
				अनुवर्त;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			जाव (parents) {
				संरचना commit *p = parents->item;
				संरचना strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				जइ (commit_to_name(p))
					अनुवर्त;
				विकल्प (n->generation) {
				पक्ष 0:
					strbuf_addstr(&newname, n->head_name);
					भङ्ग;
				पक्ष 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					भङ्ग;
				स्वभाव:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					भङ्ग;
				}
				जइ (nth == 1)
					strbuf_addch(&newname, '^');
				अण्णहा
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, रिक्त), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} जाव (i);
}

स्थिर पूर्णाङ्क mark_seen(संरचना commit *commit, संरचना commit_list **seen_p)
{
	जइ (!commit->object.flags) {
		commit_list_निवेशय(commit, seen_p);
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
}

स्थिर शून्य join_revs(संरचना prio_queue *queue,
		      संरचना commit_list **seen_p,
		      पूर्णाङ्क अङ्कः_rev, पूर्णाङ्क extra)
{
	पूर्णाङ्क all_mask = ((1u << (REV_SHIFT + अङ्कः_rev)) - 1);
	पूर्णाङ्क all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	संरचना commit *commit;

	जाव ((commit = prio_queue_peek(queue))) {
		संरचना commit_list *parents;
		पूर्णाङ्क still_interesting = !!interesting(queue);
		पूर्णाङ्क flags = commit->object.flags & all_mask;

		जइ (!still_interesting && extra <= 0)
			भङ्ग;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		जइ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		जाव (parents) {
			संरचना commit *p = parents->item;
			पूर्णाङ्क this_flag = p->object.flags;
			parents = parents->next;
			जइ ((this_flag & flags) == flags)
				अनुवर्त;
			repo_parse_commit(the_repository, p);
			जइ (mark_seen(p, seen_p) && !still_interesting)
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
	प्रति (;;) {
		पूर्णाङ्क changed = 0;
		संरचना commit_list *s;
		प्रति (s = *seen_p; s; s = s->next) {
			संरचना commit *c = s->item;
			संरचना commit_list *parents;

			जइ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				अनुवर्त;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			जाव (parents) {
				संरचना commit *p = parents->item;
				parents = parents->next;
				जइ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		जइ (!changed)
			भङ्ग;
	}
}

स्थिर शून्य show_one_commit(संरचना commit *commit, पूर्णाङ्क no_name)
{
	संरचना strbuf pretty = STRBUF_INIT;
	ध्रुव वर्ण *pretty_str = "(unavailable)";
	संरचना commit_name *name = commit_to_name(commit);

	जइ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	जइ (!no_name) {
		जइ (name && name->head_name) {
			printf("[%s", name->head_name);
			जइ (name->generation) {
				जइ (name->generation == 1)
					printf("^");
				अण्णहा
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		अण्णहा
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

स्थिर वर्ण *ref_name[MAX_REVS + 1];
स्थिर पूर्णाङ्क ref_name_cnt;

स्थिर ध्रुव वर्ण *find_digit_prefix(ध्रुव वर्ण *s, पूर्णाङ्क *v)
{
	ध्रुव वर्ण *p;
	पूर्णाङ्क ver;
	वर्ण ch;

	प्रति (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	प्रत्यागम p;
}


स्थिर पूर्णाङ्क version_cmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
	जाव (1) {
		पूर्णाङ्क va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		जइ (va != vb)
			प्रत्यागम va - vb;

		जाव (1) {
			पूर्णाङ्क ca = *a;
			पूर्णाङ्क cb = *b;
			जइ ('0' <= ca && ca <= '9')
				ca = 0;
			जइ ('0' <= cb && cb <= '9')
				cb = 0;
			जइ (ca != cb)
				प्रत्यागम ca - cb;
			जइ (!ca)
				भङ्ग;
			a++;
			b++;
		}
		जइ (!*a && !*b)
			प्रत्यागम 0;
	}
}

स्थिर पूर्णाङ्क compare_ref_name(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव वर्ण * ध्रुव*a = a_, * ध्रुव*b = b_;
	प्रत्यागम version_cmp(*a, *b);
}

स्थिर शून्य sort_ref_range(पूर्णाङ्क bottom, पूर्णाङ्क शिखरम्)
{
	QSORT(ref_name + bottom, शिखरम् - bottom, compare_ref_name);
}

स्थिर पूर्णाङ्क append_ref(ध्रुव वर्ण *refname, ध्रुव संरचना object_id *oid,
		      पूर्णाङ्क allow_dups)
{
	संरचना commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	पूर्णाङ्क i;

	जइ (!commit)
		प्रत्यागम 0;

	जइ (!allow_dups) {
		/* Avoid adding the same thing twice */
		प्रति (i = 0; i < ref_name_cnt; i++)
			जइ (!strcmp(refname, ref_name[i]))
				प्रत्यागम 0;
	}
	जइ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		प्रत्यागम 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = रिक्त;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क append_head_ref(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	संरचना object_id tmp;
	पूर्णाङ्क ofs = 11;
	जइ (!starts_with(ref->name, "refs/heads/"))
		प्रत्यागम 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	जइ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	प्रत्यागम append_ref(ref->name + ofs, ref->oid, 0);
}

स्थिर पूर्णाङ्क append_remote_ref(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	संरचना object_id tmp;
	पूर्णाङ्क ofs = 13;
	जइ (!starts_with(ref->name, "refs/remotes/"))
		प्रत्यागम 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	जइ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	प्रत्यागम append_ref(ref->name + ofs, ref->oid, 0);
}

स्थिर पूर्णाङ्क append_tag_ref(ध्रुव वर्ण *refname, ध्रुव संरचना object_id *oid,
			  पूर्णाङ्क flag UNUSED, शून्य *cb_data UNUSED)
{
	जइ (!starts_with(refname, "refs/tags/"))
		प्रत्यागम 0;
	प्रत्यागम append_ref(refname + 5, oid, 0);
}

स्थिर ध्रुव वर्ण *match_ref_pattern = रिक्त;
स्थिर पूर्णाङ्क match_ref_slash = 0;

स्थिर पूर्णाङ्क append_matching_ref(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ध्रुव वर्ण *tail;
	पूर्णाङ्क slash = गणनम्_slashes(ref->name);
	प्रति (tail = ref->name; *tail && match_ref_slash < slash; )
		जइ (*tail++ == '/')
			slash--;
	जइ (!*tail)
		प्रत्यागम 0;
	जइ (wildmatch(match_ref_pattern, tail, 0))
		प्रत्यागम 0;
	जइ (starts_with(ref->name, "refs/heads/"))
		प्रत्यागम append_head_ref(ref, cb_data);
	जइ (starts_with(ref->name, "refs/tags/"))
		प्रत्यागम append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	प्रत्यागम append_ref(ref->name, ref->oid, 0);
}

स्थिर शून्य snarf_refs(पूर्णाङ्क head, पूर्णाङ्क remotes)
{
	जइ (head) {
		पूर्णाङ्क orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, रिक्त);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	जइ (remotes) {
		पूर्णाङ्क orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, रिक्त);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

स्थिर पूर्णाङ्क rev_is_head(ध्रुव वर्ण *head, ध्रुव वर्ण *name)
{
	जइ (!head)
		प्रत्यागम 0;
	skip_prefix(head, "refs/heads/", &head);
	जइ (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	प्रत्यागम !strcmp(head, name);
}

स्थिर पूर्णाङ्क show_merge_base(ध्रुव संरचना commit_list *seen, पूर्णाङ्क अङ्कः_rev)
{
	पूर्णाङ्क all_mask = ((1u << (REV_SHIFT + अङ्कः_rev)) - 1);
	पूर्णाङ्क all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	पूर्णाङ्क exit_status = 1;

	प्रति (ध्रुव संरचना commit_list *s = seen; s; s = s->next) {
		संरचना commit *commit = s->item;
		पूर्णाङ्क flags = commit->object.flags & all_mask;
		जइ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	प्रत्यागम exit_status;
}

स्थिर पूर्णाङ्क show_independent(संरचना commit **rev,
			    पूर्णाङ्क अङ्कः_rev,
			    unsigned पूर्णाङ्क *rev_mask)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < अङ्कः_rev; i++) {
		संरचना commit *commit = rev[i];
		unsigned पूर्णाङ्क flag = rev_mask[i];

		जइ (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	प्रत्यागम 0;
}

स्थिर शून्य append_one_rev(ध्रुव वर्ण *av)
{
	संरचना object_id revkey;
	जइ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		प्रत्यागम;
	}
	जइ (strpbrk(av, "*?[")) {
		/* glob style match */
		पूर्णाङ्क saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = गणनम्_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, रिक्त);
		जइ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		प्रत्यागम;
	}
	die("bad sha1 reference %s", av);
}

स्थिर पूर्णाङ्क git_show_branch_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	जइ (!strcmp(var, "showbranch.default")) {
		जइ (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		जइ (!default_args.nr)
			strvec_प्रक्षिप(&default_args, "show-branch");
		strvec_प्रक्षिप(&default_args, मानम्);
		प्रत्यागम 0;
	}

	जइ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, मानम्);
		प्रत्यागम 0;
	}

	जइ (git_color_config(var, मानम्, cb) < 0)
		प्रत्यागम -1;

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क omit_in_dense(संरचना commit *commit, संरचना commit **rev, पूर्णाङ्क n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	पूर्णाङ्क i, flag, गणनम्;
	प्रति (i = 0; i < n; i++)
		जइ (rev[i] == commit)
			प्रत्यागम 0;
	flag = commit->object.flags;
	प्रति (i = गणनम् = 0; i < n; i++) {
		जइ (flag & (1u << (i + REV_SHIFT)))
			गणनम्++;
	}
	जइ (गणनम् == 1)
		प्रत्यागम 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क reflog = 0;

स्थिर पूर्णाङ्क parse_reflog_param(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			      पूर्णाङ्क unset)
{
	वर्ण *ep;
	ध्रुव वर्ण **base = (ध्रुव वर्ण **)opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	जइ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	जइ (*ep == ',')
		*base = ep + 1;
	अण्णहा जइ (*ep)
		प्रत्यागम error("unrecognized reflog param '%s'", arg);
	अण्णहा
		*base = रिक्त;
	जइ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_show_branch(पूर्णाङ्क ac,
		ध्रुव वर्ण **av,
		ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना commit *rev[MAX_REVS], *commit;
	वर्ण *reflog_सन्देशः[MAX_REVS] = {0};
	संरचना commit_list *seen = रिक्त;
	संरचना prio_queue queue = { compare_commits_by_commit_date };
	unsigned पूर्णाङ्क rev_mask[MAX_REVS];
	पूर्णाङ्क अङ्कः_rev, i, extra = 0;
	पूर्णाङ्क all_heads = 0, all_remotes = 0;
	पूर्णाङ्क all_mask, all_revs;
	गणना rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	वर्ण *head;
	संरचना object_id head_oid;
	पूर्णाङ्क merge_base = 0;
	पूर्णाङ्क independent = 0;
	पूर्णाङ्क no_name = 0;
	पूर्णाङ्क sha1_name = 0;
	पूर्णाङ्क shown_merge_point = 0;
	पूर्णाङ्क with_current_branch = 0;
	पूर्णाङ्क head_at = -1;
	पूर्णाङ्क topics = 0;
	पूर्णाङ्क sparse = 0;
	ध्रुव वर्ण *reflog_base = रिक्त;
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
			.मानम् = &extra,
			.precision = परिमाण(extra),
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
	ध्रुव वर्ण **args_copy = रिक्त;
	पूर्णाङ्क ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, रिक्त);

	/* If nothing is specified, try the default first */
	जइ (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	जइ (all_heads)
		all_remotes = 1;

	जइ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		जइ (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		जइ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	जइ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	जइ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	जइ (reflog) {
		संरचना object_id oid;
		वर्ण *ref;
		पूर्णाङ्क base = 0;
		unsigned पूर्णाङ्क flags = 0;

		जइ (ac == 0) {
			स्थिर ध्रुव वर्ण *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 रिक्त);
			fake_av[1] = रिक्त;
			av = fake_av;
			ac = 1;
			जइ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		जइ (ac != 1)
			die(_("--reflog option needs one branch name"));

		जइ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		जइ (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		जइ (reflog_base) {
			वर्ण *ep;
			base = strtoul(reflog_base, &ep, 10);
			जइ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, रिक्त,
					    रिक्त, रिक्त, &base);
			}
		}

		प्रति (i = 0; i < reflog; i++) {
			वर्ण *logmsg = रिक्त;
			वर्ण *nth_desc;
			ध्रुव वर्ण *सन्देशः;
			वर्ण *end;
			timestamp_t timestamp;
			पूर्णाङ्क tz;

			जइ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, रिक्त)) {
				मुक्त(logmsg);
				reflog = i;
				भङ्ग;
			}

			end = strchr(logmsg, '\n');
			जइ (end)
				*end = '\0';

			सन्देशः = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_सन्देशः[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						सन्देशः);
			मुक्त(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			मुक्त(nth_desc);
		}
		मुक्त(ref);
	}
	अण्णहा {
		जाव (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		जइ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, रिक्त);

	जइ (with_current_branch && head) {
		पूर्णाङ्क has_head = 0;
		प्रति (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			जइ (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		जइ (!has_head) {
			ध्रुव वर्ण *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	जइ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		गच्छ out;
	}

	प्रति (अङ्कः_rev = 0; ref_name[अङ्कः_rev]; अङ्कः_rev++) {
		संरचना object_id revkey;
		unsigned पूर्णाङ्क flag = 1u << (अङ्कः_rev + REV_SHIFT);

		जइ (MAX_REVS <= अङ्कः_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		जइ (repo_get_oid(the_repository, ref_name[अङ्कः_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[अङ्कः_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		जइ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[अङ्कः_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		जइ (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[अङ्कः_rev] = commit;
	}
	प्रति (i = 0; i < अङ्कः_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	जइ (0 <= extra)
		join_revs(&queue, &seen, अङ्कः_rev, extra);

	commit_list_sort_by_date(&seen);

	जइ (merge_base) {
		ret = show_merge_base(seen, अङ्कः_rev);
		गच्छ out;
	}

	जइ (independent) {
		ret = show_independent(rev, अङ्कः_rev, rev_mask);
		गच्छ out;
	}

	/* Show list; --more=-1 means list-only */
	जइ (1 < अङ्कः_rev || extra < 0) {
		प्रति (i = 0; i < अङ्कः_rev; i++) {
			पूर्णाङ्क j;
			पूर्णाङ्क is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			जइ (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			अण्णहा {
				प्रति (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			जइ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			अण्णहा
				puts(reflog_सन्देशः[i]);

			जइ (is_head)
				head_at = i;
		}
		जइ (0 <= extra) {
			प्रति (i = 0; i < अङ्कः_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	जइ (extra < 0) {
		ret = 0;
		गच्छ out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	जइ (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, अङ्कः_rev);

	all_mask = ((1u << (REV_SHIFT + अङ्कः_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	प्रति (संरचना commit_list *l = seen; l; l = l->next) {
		संरचना commit *commit = l->item;
		पूर्णाङ्क this_flag = commit->object.flags;
		पूर्णाङ्क is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		जइ (1 < अङ्कः_rev) {
			पूर्णाङ्क is_merge = !!(commit->parents &&
					  commit->parents->next);
			जइ (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				अनुवर्त;
			जइ (!sparse && is_merge &&
			    omit_in_dense(commit, rev, अङ्कः_rev))
				अनुवर्त;
			प्रति (i = 0; i < अङ्कः_rev; i++) {
				पूर्णाङ्क mark;
				जइ (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				अण्णहा जइ (is_merge)
					mark = '-';
				अण्णहा जइ (i == head_at)
					mark = '*';
				अण्णहा
					mark = '+';
				जइ (mark == ' ')
					putchar(mark);
				अण्णहा
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		जइ (shown_merge_point && --extra < 0)
			भङ्ग;
	}

	ret = 0;

out:
	प्रति (size_t i = 0; i < ARRAY_SIZE(reflog_सन्देशः); i++)
		मुक्त(reflog_सन्देशः[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	मुक्त(args_copy);
	मुक्त(head);
	प्रत्यागम ret;
}
