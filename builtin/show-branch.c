#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "pretty.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"

#అన్తర్భావ "dir.h"
#అన్తర్భావ "commit-slab.h"
#అన్తర్భావ "date.h"
#అన్తర్భావ "wildmatch.h"
#అన్తర్భావ "prio-queue.h"

స్థిర స్థిరరాశి అక్షరం*స్థిరరాశి show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ఖాళీ
};

స్థిర గణన git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

స్థిర నిర్మాణం strvec default_వాదనలు = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#నిర్వచించు UNINTERESTING	01

#నిర్వచించు REV_జరుపు	 2
#నిర్వచించు MAX_REVS	(FLAG_బిట్లు - REV_జరుపు) /* should not exceed bits_per_int - REV_SHIFT */

#నిర్వచించు DEFAULT_REFLOG	4

స్థిర స్థిరరాశి అక్షరం *get_color_సంకేతం(పూర్ణాంకం idx)
{
	ఒకవేళ (want_color(showbranch_use_color))
		తిరిగి స్తంభం_colors_ansi[idx % స్తంభం_colors_ansi_max];
	తిరిగి "";
}

స్థిర స్థిరరాశి అక్షరం *get_color_reset_సంకేతం(శూన్య)
{
	ఒకవేళ (want_color(showbranch_use_color))
		తిరిగి GIT_COLOR_RESET;
	తిరిగి "";
}

స్థిర నిర్మాణం commit *interesting(నిర్మాణం prio_వరుస *వరుస)
{
	నిర్మాణం commit *commit;
	prio_వరుస_for_each(వరుస, commit) {
		ఒకవేళ (!(commit->object.flags & UNINTERESTING))
			తిరిగి commit;
	}
	తిరిగి ఖాళీ;
}

నిర్మాణం commit_name {
	స్థిరరాశి అక్షరం *తల_name; /* which head's ancestor? */
	పూర్ణాంకం generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, నిర్మాణం commit_name *);
స్థిర నిర్మాణం commit_name_slab name_slab;

స్థిర నిర్మాణం commit_name *commit_గమ్యం_name(నిర్మాణం commit *commit)
{
	తిరిగి *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
స్థిర శూన్య name_commit(నిర్మాణం commit *commit, స్థిరరాశి అక్షరం *తల_name, పూర్ణాంకం nth)
{
	నిర్మాణం commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ఒకవేళ (!name) {
		name = xmalloc(పరిమాణం(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->తల_name = తల_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
స్థిర శూన్య name_మాతృ(నిర్మాణం commit *commit, నిర్మాణం commit *మాతృ)
{
	నిర్మాణం commit_name *commit_name = commit_గమ్యం_name(commit);
	నిర్మాణం commit_name *మాతృ_name = commit_గమ్యం_name(మాతృ);
	ఒకవేళ (!commit_name)
		తిరిగి;
	ఒకవేళ (!మాతృ_name ||
	    commit_name->generation + 1 < మాతృ_name->generation)
		name_commit(మాతృ, commit_name->తల_name,
			    commit_name->generation + 1);
}

స్థిర పూర్ణాంకం name_మొదటి_మాతృ_chain(నిర్మాణం commit *c)
{
	పూర్ణాంకం i = 0;
	అయితే (c) {
		నిర్మాణం commit *p;
		ఒకవేళ (!commit_గమ్యం_name(c))
			ఆపు;
		ఒకవేళ (!c->parents)
			ఆపు;
		p = c->parents->అంశం;
		ఒకవేళ (!commit_గమ్యం_name(p)) {
			name_మాతృ(c, p);
			i++;
		}
		లేకపోతే
			ఆపు;
		c = p;
	}
	తిరిగి i;
}

స్థిర శూన్య name_commits(నిర్మాణం commit_list *list,
			 నిర్మాణం commit **rev,
			 అక్షరం **ref_name,
			 పూర్ణాంకం అంకె_rev)
{
	నిర్మాణం commit_list *cl;
	నిర్మాణం commit *c;
	పూర్ణాంకం i;

	/* First give names to the given heads */
	కొరకు (cl = list; cl; cl = cl->తదుపరి) {
		c = cl->అంశం;
		ఒకవేళ (commit_గమ్యం_name(c))
			కొనసాగు;
		కొరకు (i = 0; i < అంకె_rev; i++) {
			ఒకవేళ (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				ఆపు;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	చేయి {
		i = 0;
		కొరకు (cl = list; cl; cl = cl->తదుపరి) {
			i += name_మొదటి_మాతృ_chain(cl->అంశం);
		}
	} అయితే (i);

	/* Finally, any unnamed commits */
	చేయి {
		i = 0;
		కొరకు (cl = list; cl; cl = cl->తదుపరి) {
			నిర్మాణం commit_list *parents;
			నిర్మాణం commit_name *n;
			పూర్ణాంకం nth;
			c = cl->అంశం;
			ఒకవేళ (!commit_గమ్యం_name(c))
				కొనసాగు;
			n = commit_గమ్యం_name(c);
			parents = c->parents;
			nth = 0;
			అయితే (parents) {
				నిర్మాణం commit *p = parents->అంశం;
				నిర్మాణం strbuf newname = STRBUF_INIT;
				parents = parents->తదుపరి;
				nth++;
				ఒకవేళ (commit_గమ్యం_name(p))
					కొనసాగు;
				ఎంపిక (n->generation) {
				స్థితి 0:
					strbuf_addstr(&newname, n->తల_name);
					ఆపు;
				స్థితి 1:
					strbuf_addf(&newname, "%s^", n->తల_name);
					ఆపు;
				అప్రమేయ:
					strbuf_addf(&newname, "%s~%d",
						    n->తల_name, n->generation);
					ఆపు;
				}
				ఒకవేళ (nth == 1)
					strbuf_addch(&newname, '^');
				లేకపోతే
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ఖాళీ), 0);
				i++;
				name_మొదటి_మాతృ_chain(p);
			}
		}
	} అయితే (i);
}

స్థిర పూర్ణాంకం mark_చూసిన(నిర్మాణం commit *commit, నిర్మాణం commit_list **చూసిన_p)
{
	ఒకవేళ (!commit->object.flags) {
		commit_list_చొప్పించు(commit, చూసిన_p);
		తిరిగి 1;
	}
	తిరిగి 0;
}

స్థిర శూన్య join_revs(నిర్మాణం prio_వరుస *వరుస,
		      నిర్మాణం commit_list **చూసిన_p,
		      పూర్ణాంకం అంకె_rev, పూర్ణాంకం extra)
{
	పూర్ణాంకం all_ముసుగు = ((1u << (REV_జరుపు + అంకె_rev)) - 1);
	పూర్ణాంకం all_revs = all_ముసుగు & ~((1u << REV_జరుపు) - 1);
	నిర్మాణం commit *commit;

	అయితే ((commit = prio_వరుస_peek(వరుస))) {
		నిర్మాణం commit_list *parents;
		పూర్ణాంకం still_interesting = !!interesting(వరుస);
		పూర్ణాంకం flags = commit->object.flags & all_ముసుగు;

		ఒకవేళ (!still_interesting && extra <= 0)
			ఆపు;

		prio_వరుస_get(వరుస);

		mark_చూసిన(commit, చూసిన_p);
		ఒకవేళ ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		అయితే (parents) {
			నిర్మాణం commit *p = parents->అంశం;
			పూర్ణాంకం this_flag = p->object.flags;
			parents = parents->తదుపరి;
			ఒకవేళ ((this_flag & flags) == flags)
				కొనసాగు;
			repo_parse_commit(the_repository, p);
			ఒకవేళ (mark_చూసిన(p, చూసిన_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_వరుస_put(వరుస, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	కొరకు (;;) {
		పూర్ణాంకం మారింది = 0;
		నిర్మాణం commit_list *s;
		కొరకు (s = *చూసిన_p; s; s = s->తదుపరి) {
			నిర్మాణం commit *c = s->అంశం;
			నిర్మాణం commit_list *parents;

			ఒకవేళ (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				కొనసాగు;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			అయితే (parents) {
				నిర్మాణం commit *p = parents->అంశం;
				parents = parents->తదుపరి;
				ఒకవేళ (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					మారింది = 1;
				}
			}
		}
		ఒకవేళ (!మారింది)
			ఆపు;
	}
}

స్థిర శూన్య show_one_commit(నిర్మాణం commit *commit, పూర్ణాంకం no_name)
{
	నిర్మాణం strbuf pretty = STRBUF_INIT;
	స్థిరరాశి అక్షరం *pretty_str = "(unavailable)";
	నిర్మాణం commit_name *name = commit_గమ్యం_name(commit);

	ఒకవేళ (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_ఉపసర్గ(pretty_str, "[PATCH] ", &pretty_str);

	ఒకవేళ (!no_name) {
		ఒకవేళ (name && name->తల_name) {
			రాయి("[%s", name->తల_name);
			ఒకవేళ (name->generation) {
				ఒకవేళ (name->generation == 1)
					రాయి("^");
				లేకపోతే
					రాయి("~%d", name->generation);
			}
			రాయి("] ");
		}
		లేకపోతే
			రాయి("[%s] ",
			       repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

స్థిర అక్షరం *ref_name[MAX_REVS + 1];
స్థిర పూర్ణాంకం ref_name_cnt;

స్థిర స్థిరరాశి అక్షరం *కనుగొను_digit_ఉపసర్గ(స్థిరరాశి అక్షరం *s, పూర్ణాంకం *v)
{
	స్థిరరాశి అక్షరం *p;
	పూర్ణాంకం ver;
	అక్షరం ch;

	కొరకు (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	తిరిగి p;
}


స్థిర పూర్ణాంకం సంస్కరణ_cmp(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b)
{
	అయితే (1) {
		పూర్ణాంకం va, vb;

		a = కనుగొను_digit_ఉపసర్గ(a, &va);
		b = కనుగొను_digit_ఉపసర్గ(b, &vb);
		ఒకవేళ (va != vb)
			తిరిగి va - vb;

		అయితే (1) {
			పూర్ణాంకం ca = *a;
			పూర్ణాంకం cb = *b;
			ఒకవేళ ('0' <= ca && ca <= '9')
				ca = 0;
			ఒకవేళ ('0' <= cb && cb <= '9')
				cb = 0;
			ఒకవేళ (ca != cb)
				తిరిగి ca - cb;
			ఒకవేళ (!ca)
				ఆపు;
			a++;
			b++;
		}
		ఒకవేళ (!*a && !*b)
			తిరిగి 0;
	}
}

స్థిర పూర్ణాంకం compare_ref_name(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి*a = a_, * స్థిరరాశి*b = b_;
	తిరిగి సంస్కరణ_cmp(*a, *b);
}

స్థిర శూన్య క్రమీకరించు_ref_range(పూర్ణాంకం bottom, పూర్ణాంకం శిఖరం)
{
	QSORT(ref_name + bottom, శిఖరం - bottom, compare_ref_name);
}

స్థిర పూర్ణాంకం append_ref(స్థిరరాశి అక్షరం *refname, స్థిరరాశి నిర్మాణం object_id *oid,
		      పూర్ణాంకం allow_dups)
{
	నిర్మాణం commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	పూర్ణాంకం i;

	ఒకవేళ (!commit)
		తిరిగి 0;

	ఒకవేళ (!allow_dups) {
		/* Avoid adding the same thing twice */
		కొరకు (i = 0; i < ref_name_cnt; i++)
			ఒకవేళ (!strcmp(refname, ref_name[i]))
				తిరిగి 0;
	}
	ఒకవేళ (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		తిరిగి 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ఖాళీ;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం append_తల_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	నిర్మాణం object_id tmp;
	పూర్ణాంకం ofs = 11;
	ఒకవేళ (!starts_with(ref->name, "refs/heads/"))
		తిరిగి 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ఒకవేళ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	తిరిగి append_ref(ref->name + ofs, ref->oid, 0);
}

స్థిర పూర్ణాంకం append_remote_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	నిర్మాణం object_id tmp;
	పూర్ణాంకం ofs = 13;
	ఒకవేళ (!starts_with(ref->name, "refs/remotes/"))
		తిరిగి 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ఒకవేళ (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	తిరిగి append_ref(ref->name + ofs, ref->oid, 0);
}

స్థిర పూర్ణాంకం append_ట్యాగ్_ref(స్థిరరాశి అక్షరం *refname, స్థిరరాశి నిర్మాణం object_id *oid,
			  పూర్ణాంకం flag వాడని, శూన్య *cb_దత్తాంశం వాడని)
{
	ఒకవేళ (!starts_with(refname, "refs/tags/"))
		తిరిగి 0;
	తిరిగి append_ref(refname + 5, oid, 0);
}

స్థిర స్థిరరాశి అక్షరం *సరిపోలిక_ref_నమూనా = ఖాళీ;
స్థిర పూర్ణాంకం సరిపోలిక_ref_slash = 0;

స్థిర పూర్ణాంకం append_matching_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	స్థిరరాశి అక్షరం *తోక;
	పూర్ణాంకం slash = లెక్క_slashes(ref->name);
	కొరకు (తోక = ref->name; *తోక && సరిపోలిక_ref_slash < slash; )
		ఒకవేళ (*తోక++ == '/')
			slash--;
	ఒకవేళ (!*తోక)
		తిరిగి 0;
	ఒకవేళ (wildmatch(సరిపోలిక_ref_నమూనా, తోక, 0))
		తిరిగి 0;
	ఒకవేళ (starts_with(ref->name, "refs/heads/"))
		తిరిగి append_తల_ref(ref, cb_దత్తాంశం);
	ఒకవేళ (starts_with(ref->name, "refs/tags/"))
		తిరిగి append_ట్యాగ్_ref(ref->name, ref->oid, ref->flags, cb_దత్తాంశం);
	తిరిగి append_ref(ref->name, ref->oid, 0);
}

స్థిర శూన్య snarf_refs(పూర్ణాంకం తల, పూర్ణాంకం remotes)
{
	ఒకవేళ (తల) {
		పూర్ణాంకం orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_తల_ref, ఖాళీ);
		క్రమీకరించు_ref_range(orig_cnt, ref_name_cnt);
	}
	ఒకవేళ (remotes) {
		పూర్ణాంకం orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ఖాళీ);
		క్రమీకరించు_ref_range(orig_cnt, ref_name_cnt);
	}
}

స్థిర పూర్ణాంకం rev_is_తల(స్థిరరాశి అక్షరం *తల, స్థిరరాశి అక్షరం *name)
{
	ఒకవేళ (!తల)
		తిరిగి 0;
	skip_ఉపసర్గ(తల, "refs/heads/", &తల);
	ఒకవేళ (!skip_ఉపసర్గ(name, "refs/heads/", &name))
		skip_ఉపసర్గ(name, "heads/", &name);
	తిరిగి !strcmp(తల, name);
}

స్థిర పూర్ణాంకం show_విలీనం_పునాది(స్థిరరాశి నిర్మాణం commit_list *చూసిన, పూర్ణాంకం అంకె_rev)
{
	పూర్ణాంకం all_ముసుగు = ((1u << (REV_జరుపు + అంకె_rev)) - 1);
	పూర్ణాంకం all_revs = all_ముసుగు & ~((1u << REV_జరుపు) - 1);
	పూర్ణాంకం exit_స్థితి = 1;

	కొరకు (స్థిరరాశి నిర్మాణం commit_list *s = చూసిన; s; s = s->తదుపరి) {
		నిర్మాణం commit *commit = s->అంశం;
		పూర్ణాంకం flags = commit->object.flags & all_ముసుగు;
		ఒకవేళ (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_గమ్యం_hex(&commit->object.oid));
			exit_స్థితి = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	తిరిగి exit_స్థితి;
}

స్థిర పూర్ణాంకం show_independent(నిర్మాణం commit **rev,
			    పూర్ణాంకం అంకె_rev,
			    గుర్తులేని పూర్ణాంకం *rev_ముసుగు)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < అంకె_rev; i++) {
		నిర్మాణం commit *commit = rev[i];
		గుర్తులేని పూర్ణాంకం flag = rev_ముసుగు[i];

		ఒకవేళ (commit->object.flags == flag)
			puts(oid_గమ్యం_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	తిరిగి 0;
}

స్థిర శూన్య append_one_rev(స్థిరరాశి అక్షరం *av)
{
	నిర్మాణం object_id revkey;
	ఒకవేళ (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		తిరిగి;
	}
	ఒకవేళ (strpbrk(av, "*?[")) {
		/* glob style match */
		పూర్ణాంకం saved_matches = ref_name_cnt;

		సరిపోలిక_ref_నమూనా = av;
		సరిపోలిక_ref_slash = లెక్క_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ఖాళీ);
		ఒకవేళ (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		క్రమీకరించు_ref_range(saved_matches, ref_name_cnt);
		తిరిగి;
	}
	die("bad sha1 reference %s", av);
}

స్థిర పూర్ణాంకం git_show_branch_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
				  స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "showbranch.default")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ఒకవేళ (!default_వాదనలు.nr)
			strvec_నెట్టు(&default_వాదనలు, "show-branch");
		strvec_నెట్టు(&default_వాదనలు, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (git_color_config(var, విలువ, cb) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర పూర్ణాంకం omit_లోపల_dense(నిర్మాణం commit *commit, నిర్మాణం commit **rev, పూర్ణాంకం n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	పూర్ణాంకం i, flag, లెక్క;
	కొరకు (i = 0; i < n; i++)
		ఒకవేళ (rev[i] == commit)
			తిరిగి 0;
	flag = commit->object.flags;
	కొరకు (i = లెక్క = 0; i < n; i++) {
		ఒకవేళ (flag & (1u << (i + REV_జరుపు)))
			లెక్క++;
	}
	ఒకవేళ (లెక్క == 1)
		తిరిగి 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం reflog = 0;

స్థిర పూర్ణాంకం parse_reflog_param(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			      పూర్ణాంకం unset)
{
	అక్షరం *ep;
	స్థిరరాశి అక్షరం **పునాది = (స్థిరరాశి అక్షరం **)opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	ఒకవేళ (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ఒకవేళ (*ep == ',')
		*పునాది = ep + 1;
	లేకపోతే ఒకవేళ (*ep)
		తిరిగి error("unrecognized reflog param '%s'", arg);
	లేకపోతే
		*పునాది = ఖాళీ;
	ఒకవేళ (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	తిరిగి 0;
}

పూర్ణాంకం cmd_show_branch(పూర్ణాంకం ac,
		స్థిరరాశి అక్షరం **av,
		స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం commit *rev[MAX_REVS], *commit;
	అక్షరం *reflog_సందేశం[MAX_REVS] = {0};
	నిర్మాణం commit_list *చూసిన = ఖాళీ;
	నిర్మాణం prio_వరుస వరుస = { compare_commits_by_commit_date };
	గుర్తులేని పూర్ణాంకం rev_ముసుగు[MAX_REVS];
	పూర్ణాంకం అంకె_rev, i, extra = 0;
	పూర్ణాంకం all_heads = 0, all_remotes = 0;
	పూర్ణాంకం all_ముసుగు, all_revs;
	గణన rev_క్రమీకరించు_క్రమం క్రమీకరించు_క్రమం = REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం;
	అక్షరం *తల;
	నిర్మాణం object_id తల_oid;
	పూర్ణాంకం విలీనం_పునాది = 0;
	పూర్ణాంకం independent = 0;
	పూర్ణాంకం no_name = 0;
	పూర్ణాంకం sha1_name = 0;
	పూర్ణాంకం shown_విలీనం_బిందువు = 0;
	పూర్ణాంకం with_current_branch = 0;
	పూర్ణాంకం తల_at = -1;
	పూర్ణాంకం topics = 0;
	పూర్ణాంకం sparse = 0;
	స్థిరరాశి అక్షరం *reflog_పునాది = ఖాళీ;
	నిర్మాణం option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.విలువ = &extra,
			.precision = పరిమాణం(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_సమితి_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &విలీనం_పునాది,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_సమితి_INT_F(0, "topo-order", &క్రమీకరించు_క్రమం,
			      N_("show commits in topological order"),
			      REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_సమితి_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_సమితి_INT_F(0, "date-order", &క్రమీకరించు_క్రమం,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_క్రమీకరించు_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_పునాది, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_చివర()
	};
	స్థిరరాశి అక్షరం **వాదనలు_copy = ఖాళీ;
	పూర్ణాంకం ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ఖాళీ);

	/* If nothing is specified, try the default first */
	ఒకవేళ (ac == 1 && default_వాదనలు.nr) {
		DUP_ARRAY(వాదనలు_copy, default_వాదనలు.v, default_వాదనలు.nr);
		ac = default_వాదనలు.nr;
		av = వాదనలు_copy;
	}

	ac = parse_options(ac, av, ఉపసర్గ, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ఒకవేళ (all_heads)
		all_remotes = 1;

	ఒకవేళ (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ఒకవేళ (independent || విలీనం_పునాది)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ఒకవేళ (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ఒకవేళ (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ఒకవేళ (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ఒకవేళ (reflog) {
		నిర్మాణం object_id oid;
		అక్షరం *ref;
		పూర్ణాంకం పునాది = 0;
		గుర్తులేని పూర్ణాంకం flags = 0;

		ఒకవేళ (ac == 0) {
			స్థిర స్థిరరాశి అక్షరం *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ఖాళీ);
			fake_av[1] = ఖాళీ;
			av = fake_av;
			ac = 1;
			ఒకవేళ (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ఒకవేళ (ac != 1)
			die(_("--reflog option needs one branch name"));

		ఒకవేళ (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ఒకవేళ (!repo_dwim_ref(the_repository, *av, అక్షరశ్రేణి_పొడవు(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ఒకవేళ (reflog_పునాది) {
			అక్షరం *ep;
			పునాది = strtoul(reflog_పునాది, &ep, 10);
			ఒకవేళ (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_పునాది);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ఖాళీ,
					    ఖాళీ, ఖాళీ, &పునాది);
			}
		}

		కొరకు (i = 0; i < reflog; i++) {
			అక్షరం *logmsg = ఖాళీ;
			అక్షరం *nth_desc;
			స్థిరరాశి అక్షరం *సందేశం;
			అక్షరం *చివర;
			timestamp_t timestamp;
			పూర్ణాంకం tz;

			ఒకవేళ (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, పునాది + i, &oid, &logmsg,
					&timestamp, &tz, ఖాళీ)) {
				విడుదల(logmsg);
				reflog = i;
				ఆపు;
			}

			చివర = strchr(logmsg, '\n');
			ఒకవేళ (చివర)
				*చివర = '\0';

			సందేశం = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_సందేశం[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_బహుళకం(RELATIVE)),
						సందేశం);
			విడుదల(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, పునాది+i);
			append_ref(nth_desc, &oid, 1);
			విడుదల(nth_desc);
		}
		విడుదల(ref);
	}
	లేకపోతే {
		అయితే (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ఒకవేళ (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	తల = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &తల_oid, ఖాళీ);

	ఒకవేళ (with_current_branch && తల) {
		పూర్ణాంకం has_తల = 0;
		కొరకు (i = 0; !has_తల && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ఒకవేళ (rev_is_తల(తల, ref_name[i]))
				has_తల++;
		}
		ఒకవేళ (!has_తల) {
			స్థిరరాశి అక్షరం *name = తల;
			skip_ఉపసర్గ(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ఒకవేళ (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		వెళ్ళు బహిర్గతం;
	}

	కొరకు (అంకె_rev = 0; ref_name[అంకె_rev]; అంకె_rev++) {
		నిర్మాణం object_id revkey;
		గుర్తులేని పూర్ణాంకం flag = 1u << (అంకె_rev + REV_జరుపు);

		ఒకవేళ (MAX_REVS <= అంకె_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ఒకవేళ (repo_get_oid(the_repository, ref_name[అంకె_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[అంకె_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ఒకవేళ (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[అంకె_rev], oid_గమ్యం_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_చూసిన(commit, &చూసిన);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ఒకవేళ (commit->object.flags == flag)
			prio_వరుస_put(&వరుస, commit);
		rev[అంకె_rev] = commit;
	}
	కొరకు (i = 0; i < అంకె_rev; i++)
		rev_ముసుగు[i] = rev[i]->object.flags;

	ఒకవేళ (0 <= extra)
		join_revs(&వరుస, &చూసిన, అంకె_rev, extra);

	commit_list_క్రమీకరించు_by_date(&చూసిన);

	ఒకవేళ (విలీనం_పునాది) {
		ret = show_విలీనం_పునాది(చూసిన, అంకె_rev);
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (independent) {
		ret = show_independent(rev, అంకె_rev, rev_ముసుగు);
		వెళ్ళు బహిర్గతం;
	}

	/* Show list; --more=-1 means list-only */
	ఒకవేళ (1 < అంకె_rev || extra < 0) {
		కొరకు (i = 0; i < అంకె_rev; i++) {
			పూర్ణాంకం j;
			పూర్ణాంకం is_తల = rev_is_తల(తల, ref_name[i]) &&
				      oideq(&తల_oid, &rev[i]->object.oid);
			ఒకవేళ (extra < 0)
				రాయి("%c [%s] ",
				       is_తల ? '*' : ' ', ref_name[i]);
			లేకపోతే {
				కొరకు (j = 0; j < i; j++)
					అక్షరం_రాయి(' ');
				రాయి("%s%c%s [%s] ",
				       get_color_సంకేతం(i),
				       is_తల ? '*' : '!',
				       get_color_reset_సంకేతం(), ref_name[i]);
			}

			ఒకవేళ (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			లేకపోతే
				puts(reflog_సందేశం[i]);

			ఒకవేళ (is_తల)
				తల_at = i;
		}
		ఒకవేళ (0 <= extra) {
			కొరకు (i = 0; i < అంకె_rev; i++)
				అక్షరం_రాయి('-');
			అక్షరం_రాయి('\n');
		}
	}
	ఒకవేళ (extra < 0) {
		ret = 0;
		వెళ్ళు బహిర్గతం;
	}

	/* Sort topologically */
	క్రమీకరించు_లోపల_topological_క్రమం(&చూసిన, క్రమీకరించు_క్రమం);

	/* Give names to commits */
	ఒకవేళ (!sha1_name && !no_name)
		name_commits(చూసిన, rev, ref_name, అంకె_rev);

	all_ముసుగు = ((1u << (REV_జరుపు + అంకె_rev)) - 1);
	all_revs = all_ముసుగు & ~((1u << REV_జరుపు) - 1);

	కొరకు (నిర్మాణం commit_list *l = చూసిన; l; l = l->తదుపరి) {
		నిర్మాణం commit *commit = l->అంశం;
		పూర్ణాంకం this_flag = commit->object.flags;
		పూర్ణాంకం is_విలీనం_బిందువు = ((this_flag & all_revs) == all_revs);

		shown_విలీనం_బిందువు |= is_విలీనం_బిందువు;

		ఒకవేళ (1 < అంకె_rev) {
			పూర్ణాంకం is_విలీనం = !!(commit->parents &&
					  commit->parents->తదుపరి);
			ఒకవేళ (topics &&
			    !is_విలీనం_బిందువు &&
			    (this_flag & (1u << REV_జరుపు)))
				కొనసాగు;
			ఒకవేళ (!sparse && is_విలీనం &&
			    omit_లోపల_dense(commit, rev, అంకె_rev))
				కొనసాగు;
			కొరకు (i = 0; i < అంకె_rev; i++) {
				పూర్ణాంకం mark;
				ఒకవేళ (!(this_flag & (1u << (i + REV_జరుపు))))
					mark = ' ';
				లేకపోతే ఒకవేళ (is_విలీనం)
					mark = '-';
				లేకపోతే ఒకవేళ (i == తల_at)
					mark = '*';
				లేకపోతే
					mark = '+';
				ఒకవేళ (mark == ' ')
					అక్షరం_రాయి(mark);
				లేకపోతే
					రాయి("%s%c%s",
					       get_color_సంకేతం(i),
					       mark, get_color_reset_సంకేతం());
			}
			అక్షరం_రాయి(' ');
		}
		show_one_commit(commit, no_name);

		ఒకవేళ (shown_విలీనం_బిందువు && --extra < 0)
			ఆపు;
	}

	ret = 0;

బహిర్గతం:
	కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(reflog_సందేశం); i++)
		విడుదల(reflog_సందేశం[i]);
	commit_list_free(చూసిన);
	clear_prio_వరుస(&వరుస);
	విడుదల(వాదనలు_copy);
	విడుదల(తల);
	తిరిగి ret;
}
