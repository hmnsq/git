#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

स्थिर संरचना strvec default_तर्काः = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#परिभाषय UNINTERESTING	01

#परिभाषय REV_सरणम्	 2
#परिभाषय MAX_REVS	(FLAG_द्वयङ्काः - REV_सरणम्) /* should not exceed bits_per_int - REV_SHIFT */

#परिभाषय DEFAULT_REFLOG	4

स्थिर ध्रुव वर्ण *get_color_संहिता(पूर्णाङ्क idx)
{
	यदि (want_color(showbranch_use_color))
		प्रत्यागम column_colors_ansi[idx % column_colors_ansi_max];
	प्रत्यागम "";
}

स्थिर ध्रुव वर्ण *get_color_reset_संहिता(शून्य)
{
	यदि (want_color(showbranch_use_color))
		प्रत्यागम GIT_COLOR_RESET;
	प्रत्यागम "";
}

स्थिर संरचना commit *interesting(संरचना prio_queue *queue)
{
	संरचना commit *commit;
	prio_queue_for_each(queue, commit) {
		यदि (!(commit->object.flags & UNINTERESTING))
			प्रत्यागम commit;
	}
	प्रत्यागम रिक्त;
}

संरचना commit_name {
	ध्रुव वर्ण *शीर्ष_name; /* which head's ancestor? */
	पूर्णाङ्क generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, संरचना commit_name *);
स्थिर संरचना commit_name_slab name_slab;

स्थिर संरचना commit_name *commit_गन्तव्यम्_name(संरचना commit *commit)
{
	प्रत्यागम *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
स्थिर शून्य name_commit(संरचना commit *commit, ध्रुव वर्ण *शीर्ष_name, पूर्णाङ्क nth)
{
	संरचना commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	यदि (!name) {
		name = xmalloc(परिमाण(*name));
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
स्थिर शून्य name_parent(संरचना commit *commit, संरचना commit *parent)
{
	संरचना commit_name *commit_name = commit_गन्तव्यम्_name(commit);
	संरचना commit_name *parent_name = commit_गन्तव्यम्_name(parent);
	यदि (!commit_name)
		प्रत्यागम;
	यदि (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->शीर्ष_name,
			    commit_name->generation + 1);
}

स्थिर पूर्णाङ्क name_first_parent_chain(संरचना commit *c)
{
	पूर्णाङ्क i = 0;
	यावत् (c) {
		संरचना commit *p;
		यदि (!commit_गन्तव्यम्_name(c))
			भङ्ग;
		यदि (!c->parents)
			भङ्ग;
		p = c->parents->item;
		यदि (!commit_गन्तव्यम्_name(p)) {
			name_parent(c, p);
			i++;
		}
		अन्यथा
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
	प्रति (cl = list; cl; cl = cl->अग्र) {
		c = cl->item;
		यदि (commit_गन्तव्यम्_name(c))
			अनुवर्त;
		प्रति (i = 0; i < अङ्कः_rev; i++) {
			यदि (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				भङ्ग;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	कुरु {
		i = 0;
		प्रति (cl = list; cl; cl = cl->अग्र) {
			i += name_first_parent_chain(cl->item);
		}
	} यावत् (i);

	/* Finally, any unnamed commits */
	कुरु {
		i = 0;
		प्रति (cl = list; cl; cl = cl->अग्र) {
			संरचना commit_list *parents;
			संरचना commit_name *n;
			पूर्णाङ्क nth;
			c = cl->item;
			यदि (!commit_गन्तव्यम्_name(c))
				अनुवर्त;
			n = commit_गन्तव्यम्_name(c);
			parents = c->parents;
			nth = 0;
			यावत् (parents) {
				संरचना commit *p = parents->item;
				संरचना strbuf newname = STRBUF_INIT;
				parents = parents->अग्र;
				nth++;
				यदि (commit_गन्तव्यम्_name(p))
					अनुवर्त;
				विकल्प (n->generation) {
				पक्ष 0:
					strbuf_addstr(&newname, n->शीर्ष_name);
					भङ्ग;
				पक्ष 1:
					strbuf_addf(&newname, "%s^", n->शीर्ष_name);
					भङ्ग;
				स्वभाव:
					strbuf_addf(&newname, "%s~%d",
						    n->शीर्ष_name, n->generation);
					भङ्ग;
				}
				यदि (nth == 1)
					strbuf_addch(&newname, '^');
				अन्यथा
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, रिक्त), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} यावत् (i);
}

स्थिर पूर्णाङ्क mark_आलोकितम्(संरचना commit *commit, संरचना commit_list **आलोकितम्_p)
{
	यदि (!commit->object.flags) {
		commit_list_निवेशय(commit, आलोकितम्_p);
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
}

स्थिर शून्य join_revs(संरचना prio_queue *queue,
		      संरचना commit_list **आलोकितम्_p,
		      पूर्णाङ्क अङ्कः_rev, पूर्णाङ्क extra)
{
	पूर्णाङ्क all_आवरणम् = ((1u << (REV_सरणम् + अङ्कः_rev)) - 1);
	पूर्णाङ्क all_revs = all_आवरणम् & ~((1u << REV_सरणम्) - 1);
	संरचना commit *commit;

	यावत् ((commit = prio_queue_peek(queue))) {
		संरचना commit_list *parents;
		पूर्णाङ्क still_interesting = !!interesting(queue);
		पूर्णाङ्क flags = commit->object.flags & all_आवरणम्;

		यदि (!still_interesting && extra <= 0)
			भङ्ग;

		prio_queue_get(queue);

		mark_आलोकितम्(commit, आलोकितम्_p);
		यदि ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		यावत् (parents) {
			संरचना commit *p = parents->item;
			पूर्णाङ्क this_flag = p->object.flags;
			parents = parents->अग्र;
			यदि ((this_flag & flags) == flags)
				अनुवर्त;
			repo_parse_commit(the_repository, p);
			यदि (mark_आलोकितम्(p, आलोकितम्_p) && !still_interesting)
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
		पूर्णाङ्क परिवर्तितम् = 0;
		संरचना commit_list *s;
		प्रति (s = *आलोकितम्_p; s; s = s->अग्र) {
			संरचना commit *c = s->item;
			संरचना commit_list *parents;

			यदि (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				अनुवर्त;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			यावत् (parents) {
				संरचना commit *p = parents->item;
				parents = parents->अग्र;
				यदि (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					परिवर्तितम् = 1;
				}
			}
		}
		यदि (!परिवर्तितम्)
			भङ्ग;
	}
}

स्थिर शून्य show_one_commit(संरचना commit *commit, पूर्णाङ्क no_name)
{
	संरचना strbuf pretty = STRBUF_INIT;
	ध्रुव वर्ण *pretty_str = "(unavailable)";
	संरचना commit_name *name = commit_गन्तव्यम्_name(commit);

	यदि (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_उपसर्गः(pretty_str, "[PATCH] ", &pretty_str);

	यदि (!no_name) {
		यदि (name && name->शीर्ष_name) {
			प्रारूप_लिख("[%s", name->शीर्ष_name);
			यदि (name->generation) {
				यदि (name->generation == 1)
					प्रारूप_लिख("^");
				अन्यथा
					प्रारूप_लिख("~%d", name->generation);
			}
			प्रारूप_लिख("] ");
		}
		अन्यथा
			प्रारूप_लिख("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	पङ्क्तिं_लिख(pretty_str);
	strbuf_release(&pretty);
}

स्थिर वर्ण *ref_name[MAX_REVS + 1];
स्थिर पूर्णाङ्क ref_name_cnt;

स्थिर ध्रुव वर्ण *find_digit_उपसर्गः(ध्रुव वर्ण *s, पूर्णाङ्क *v)
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


स्थिर पूर्णाङ्क संस्करणम्_cmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
	यावत् (1) {
		पूर्णाङ्क va, vb;

		a = find_digit_उपसर्गः(a, &va);
		b = find_digit_उपसर्गः(b, &vb);
		यदि (va != vb)
			प्रत्यागम va - vb;

		यावत् (1) {
			पूर्णाङ्क ca = *a;
			पूर्णाङ्क cb = *b;
			यदि ('0' <= ca && ca <= '9')
				ca = 0;
			यदि ('0' <= cb && cb <= '9')
				cb = 0;
			यदि (ca != cb)
				प्रत्यागम ca - cb;
			यदि (!ca)
				भङ्ग;
			a++;
			b++;
		}
		यदि (!*a && !*b)
			प्रत्यागम 0;
	}
}

स्थिर पूर्णाङ्क compare_ref_name(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव वर्ण * ध्रुव*a = a_, * ध्रुव*b = b_;
	प्रत्यागम संस्करणम्_cmp(*a, *b);
}

स्थिर शून्य क्रमय_ref_range(पूर्णाङ्क bottom, पूर्णाङ्क शिखरम्)
{
	QSORT(ref_name + bottom, शिखरम् - bottom, compare_ref_name);
}

स्थिर पूर्णाङ्क append_ref(ध्रुव वर्ण *refname, ध्रुव संरचना object_id *oid,
		      पूर्णाङ्क allow_dups)
{
	संरचना commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	पूर्णाङ्क i;

	यदि (!commit)
		प्रत्यागम 0;

	यदि (!allow_dups) {
		/* Avoid adding the same thing twice */
		प्रति (i = 0; i < ref_name_cnt; i++)
			यदि (!वाक्यं_तुलय(refname, ref_name[i]))
				प्रत्यागम 0;
	}
	यदि (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		प्रत्यागम 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = रिक्त;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क append_शीर्ष_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	संरचना object_id tmp;
	पूर्णाङ्क ofs = 11;
	यदि (!starts_with(ref->name, "refs/heads/"))
		प्रत्यागम 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	यदि (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	प्रत्यागम append_ref(ref->name + ofs, ref->oid, 0);
}

स्थिर पूर्णाङ्क append_remote_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	संरचना object_id tmp;
	पूर्णाङ्क ofs = 13;
	यदि (!starts_with(ref->name, "refs/remotes/"))
		प्रत्यागम 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	यदि (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	प्रत्यागम append_ref(ref->name + ofs, ref->oid, 0);
}

स्थिर पूर्णाङ्क append_चिह्नकम्_ref(ध्रुव वर्ण *refname, ध्रुव संरचना object_id *oid,
			  पूर्णाङ्क flag अप्रयुक्तम्, शून्य *cb_दत्त अप्रयुक्तम्)
{
	यदि (!starts_with(refname, "refs/tags/"))
		प्रत्यागम 0;
	प्रत्यागम append_ref(refname + 5, oid, 0);
}

स्थिर ध्रुव वर्ण *मेलनम्_ref_आदर्शः = रिक्त;
स्थिर पूर्णाङ्क मेलनम्_ref_slash = 0;

स्थिर पूर्णाङ्क append_matching_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ध्रुव वर्ण *पुच्छ;
	पूर्णाङ्क slash = गणनम्_slashes(ref->name);
	प्रति (पुच्छ = ref->name; *पुच्छ && मेलनम्_ref_slash < slash; )
		यदि (*पुच्छ++ == '/')
			slash--;
	यदि (!*पुच्छ)
		प्रत्यागम 0;
	यदि (wildmatch(मेलनम्_ref_आदर्शः, पुच्छ, 0))
		प्रत्यागम 0;
	यदि (starts_with(ref->name, "refs/heads/"))
		प्रत्यागम append_शीर्ष_ref(ref, cb_दत्त);
	यदि (starts_with(ref->name, "refs/tags/"))
		प्रत्यागम append_चिह्नकम्_ref(ref->name, ref->oid, ref->flags, cb_दत्त);
	प्रत्यागम append_ref(ref->name, ref->oid, 0);
}

स्थिर शून्य snarf_refs(पूर्णाङ्क शीर्ष, पूर्णाङ्क remotes)
{
	यदि (शीर्ष) {
		पूर्णाङ्क orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_शीर्ष_ref, रिक्त);
		क्रमय_ref_range(orig_cnt, ref_name_cnt);
	}
	यदि (remotes) {
		पूर्णाङ्क orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, रिक्त);
		क्रमय_ref_range(orig_cnt, ref_name_cnt);
	}
}

स्थिर पूर्णाङ्क rev_is_शीर्ष(ध्रुव वर्ण *शीर्ष, ध्रुव वर्ण *name)
{
	यदि (!शीर्ष)
		प्रत्यागम 0;
	skip_उपसर्गः(शीर्ष, "refs/heads/", &शीर्ष);
	यदि (!skip_उपसर्गः(name, "refs/heads/", &name))
		skip_उपसर्गः(name, "heads/", &name);
	प्रत्यागम !वाक्यं_तुलय(शीर्ष, name);
}

स्थिर पूर्णाङ्क show_merge_आधारः(ध्रुव संरचना commit_list *आलोकितम्, पूर्णाङ्क अङ्कः_rev)
{
	पूर्णाङ्क all_आवरणम् = ((1u << (REV_सरणम् + अङ्कः_rev)) - 1);
	पूर्णाङ्क all_revs = all_आवरणम् & ~((1u << REV_सरणम्) - 1);
	पूर्णाङ्क exit_स्थितिः = 1;

	प्रति (ध्रुव संरचना commit_list *s = आलोकितम्; s; s = s->अग्र) {
		संरचना commit *commit = s->item;
		पूर्णाङ्क flags = commit->object.flags & all_आवरणम्;
		यदि (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			पङ्क्तिं_लिख(oid_गन्तव्यम्_hex(&commit->object.oid));
			exit_स्थितिः = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	प्रत्यागम exit_स्थितिः;
}

स्थिर पूर्णाङ्क show_independent(संरचना commit **rev,
			    पूर्णाङ्क अङ्कः_rev,
			    अचिह्नित पूर्णाङ्क *rev_आवरणम्)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < अङ्कः_rev; i++) {
		संरचना commit *commit = rev[i];
		अचिह्नित पूर्णाङ्क flag = rev_आवरणम्[i];

		यदि (commit->object.flags == flag)
			पङ्क्तिं_लिख(oid_गन्तव्यम्_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	प्रत्यागम 0;
}

स्थिर शून्य append_one_rev(ध्रुव वर्ण *av)
{
	संरचना object_id revkey;
	यदि (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		प्रत्यागम;
	}
	यदि (strpbrk(av, "*?[")) {
		/* glob style match */
		पूर्णाङ्क saved_matches = ref_name_cnt;

		मेलनम्_ref_आदर्शः = av;
		मेलनम्_ref_slash = गणनम्_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, रिक्त);
		यदि (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		क्रमय_ref_range(saved_matches, ref_name_cnt);
		प्रत्यागम;
	}
	die("bad sha1 reference %s", av);
}

स्थिर पूर्णाङ्क git_show_branch_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
				  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "showbranch.default")) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		यदि (!default_तर्काः.nr)
			strvec_प्रक्षिप(&default_तर्काः, "show-branch");
		strvec_प्रक्षिप(&default_तर्काः, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (git_color_config(var, मानम्, cb) < 0)
		प्रत्यागम -1;

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क omit_अन्तर्_dense(संरचना commit *commit, संरचना commit **rev, पूर्णाङ्क n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	पूर्णाङ्क i, flag, गणनम्;
	प्रति (i = 0; i < n; i++)
		यदि (rev[i] == commit)
			प्रत्यागम 0;
	flag = commit->object.flags;
	प्रति (i = गणनम् = 0; i < n; i++) {
		यदि (flag & (1u << (i + REV_सरणम्)))
			गणनम्++;
	}
	यदि (गणनम् == 1)
		प्रत्यागम 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क reflog = 0;

स्थिर पूर्णाङ्क parse_reflog_param(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			      पूर्णाङ्क unset)
{
	वर्ण *ep;
	ध्रुव वर्ण **आधारः = (ध्रुव वर्ण **)opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	यदि (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	यदि (*ep == ',')
		*आधारः = ep + 1;
	अन्यथा यदि (*ep)
		प्रत्यागम error("unrecognized reflog param '%s'", arg);
	अन्यथा
		*आधारः = रिक्त;
	यदि (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_show_branch(पूर्णाङ्क ac,
		ध्रुव वर्ण **av,
		ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	संरचना commit *rev[MAX_REVS], *commit;
	वर्ण *reflog_सन्देशः[MAX_REVS] = {0};
	संरचना commit_list *आलोकितम् = रिक्त;
	संरचना prio_queue queue = { compare_commits_by_commit_date };
	अचिह्नित पूर्णाङ्क rev_आवरणम्[MAX_REVS];
	पूर्णाङ्क अङ्कः_rev, i, extra = 0;
	पूर्णाङ्क all_heads = 0, all_remotes = 0;
	पूर्णाङ्क all_आवरणम्, all_revs;
	गणना rev_क्रमय_क्रमः क्रमय_क्रमः = REV_क्रमय_अन्तर्_GRAPH_क्रमः;
	वर्ण *शीर्ष;
	संरचना object_id शीर्ष_oid;
	पूर्णाङ्क merge_आधारः = 0;
	पूर्णाङ्क independent = 0;
	पूर्णाङ्क no_name = 0;
	पूर्णाङ्क sha1_name = 0;
	पूर्णाङ्क shown_merge_point = 0;
	पूर्णाङ्क with_current_branch = 0;
	पूर्णाङ्क शीर्ष_at = -1;
	पूर्णाङ्क topics = 0;
	पूर्णाङ्क sparse = 0;
	ध्रुव वर्ण *reflog_आधारः = रिक्त;
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
		OPT_समुच्चयः_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &merge_आधारः,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_समुच्चयः_INT_F(0, "topo-order", &क्रमय_क्रमः,
			      N_("show commits in topological order"),
			      REV_क्रमय_अन्तर्_GRAPH_क्रमः, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_समुच्चयः_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_समुच्चयः_INT_F(0, "date-order", &क्रमय_क्रमः,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_क्रमय_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_आधारः, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_अन्तः()
	};
	ध्रुव वर्ण **तर्काः_copy = रिक्त;
	पूर्णाङ्क ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, रिक्त);

	/* If nothing is specified, try the default first */
	यदि (ac == 1 && default_तर्काः.nr) {
		DUP_ARRAY(तर्काः_copy, default_तर्काः.v, default_तर्काः.nr);
		ac = default_तर्काः.nr;
		av = तर्काः_copy;
	}

	ac = parse_options(ac, av, उपसर्गः, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	यदि (all_heads)
		all_remotes = 1;

	यदि (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		यदि (independent || merge_आधारः)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		यदि (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	यदि (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	यदि (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	यदि (reflog) {
		संरचना object_id oid;
		वर्ण *ref;
		पूर्णाङ्क आधारः = 0;
		अचिह्नित पूर्णाङ्क flags = 0;

		यदि (ac == 0) {
			स्थिर ध्रुव वर्ण *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 रिक्त);
			fake_av[1] = रिक्त;
			av = fake_av;
			ac = 1;
			यदि (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		यदि (ac != 1)
			die(_("--reflog option needs one branch name"));

		यदि (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		यदि (!repo_dwim_ref(the_repository, *av, वाक्यदैर्घ्यम्(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		यदि (reflog_आधारः) {
			वर्ण *ep;
			आधारः = strtoul(reflog_आधारः, &ep, 10);
			यदि (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_आधारः);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, रिक्त,
					    रिक्त, रिक्त, &आधारः);
			}
		}

		प्रति (i = 0; i < reflog; i++) {
			वर्ण *logmsg = रिक्त;
			वर्ण *nth_desc;
			ध्रुव वर्ण *सन्देशः;
			वर्ण *अन्तः;
			timestamp_t timestamp;
			पूर्णाङ्क tz;

			यदि (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, आधारः + i, &oid, &logmsg,
					&timestamp, &tz, रिक्त)) {
				मुक्त(logmsg);
				reflog = i;
				भङ्ग;
			}

			अन्तः = strchr(logmsg, '\n');
			यदि (अन्तः)
				*अन्तः = '\0';

			सन्देशः = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_सन्देशः[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_बहुलकम्(RELATIVE)),
						सन्देशः);
			मुक्त(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, आधारः+i);
			append_ref(nth_desc, &oid, 1);
			मुक्त(nth_desc);
		}
		मुक्त(ref);
	}
	अन्यथा {
		यावत् (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		यदि (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	शीर्ष = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &शीर्ष_oid, रिक्त);

	यदि (with_current_branch && शीर्ष) {
		पूर्णाङ्क has_शीर्ष = 0;
		प्रति (i = 0; !has_शीर्ष && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			यदि (rev_is_शीर्ष(शीर्ष, ref_name[i]))
				has_शीर्ष++;
		}
		यदि (!has_शीर्ष) {
			ध्रुव वर्ण *name = शीर्ष;
			skip_उपसर्गः(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	यदि (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		गच्छ बहिर्गमनम्;
	}

	प्रति (अङ्कः_rev = 0; ref_name[अङ्कः_rev]; अङ्कः_rev++) {
		संरचना object_id revkey;
		अचिह्नित पूर्णाङ्क flag = 1u << (अङ्कः_rev + REV_सरणम्);

		यदि (MAX_REVS <= अङ्कः_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		यदि (repo_get_oid(the_repository, ref_name[अङ्कः_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[अङ्कः_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		यदि (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[अङ्कः_rev], oid_गन्तव्यम्_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_आलोकितम्(commit, &आलोकितम्);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		यदि (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[अङ्कः_rev] = commit;
	}
	प्रति (i = 0; i < अङ्कः_rev; i++)
		rev_आवरणम्[i] = rev[i]->object.flags;

	यदि (0 <= extra)
		join_revs(&queue, &आलोकितम्, अङ्कः_rev, extra);

	commit_list_क्रमय_by_date(&आलोकितम्);

	यदि (merge_आधारः) {
		ret = show_merge_आधारः(आलोकितम्, अङ्कः_rev);
		गच्छ बहिर्गमनम्;
	}

	यदि (independent) {
		ret = show_independent(rev, अङ्कः_rev, rev_आवरणम्);
		गच्छ बहिर्गमनम्;
	}

	/* Show list; --more=-1 means list-only */
	यदि (1 < अङ्कः_rev || extra < 0) {
		प्रति (i = 0; i < अङ्कः_rev; i++) {
			पूर्णाङ्क j;
			पूर्णाङ्क is_शीर्ष = rev_is_शीर्ष(शीर्ष, ref_name[i]) &&
				      oideq(&शीर्ष_oid, &rev[i]->object.oid);
			यदि (extra < 0)
				प्रारूप_लिख("%c [%s] ",
				       is_शीर्ष ? '*' : ' ', ref_name[i]);
			अन्यथा {
				प्रति (j = 0; j < i; j++)
					अक्षरं_लिख(' ');
				प्रारूप_लिख("%s%c%s [%s] ",
				       get_color_संहिता(i),
				       is_शीर्ष ? '*' : '!',
				       get_color_reset_संहिता(), ref_name[i]);
			}

			यदि (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			अन्यथा
				पङ्क्तिं_लिख(reflog_सन्देशः[i]);

			यदि (is_शीर्ष)
				शीर्ष_at = i;
		}
		यदि (0 <= extra) {
			प्रति (i = 0; i < अङ्कः_rev; i++)
				अक्षरं_लिख('-');
			अक्षरं_लिख('\n');
		}
	}
	यदि (extra < 0) {
		ret = 0;
		गच्छ बहिर्गमनम्;
	}

	/* Sort topologically */
	क्रमय_अन्तर्_topological_क्रमः(&आलोकितम्, क्रमय_क्रमः);

	/* Give names to commits */
	यदि (!sha1_name && !no_name)
		name_commits(आलोकितम्, rev, ref_name, अङ्कः_rev);

	all_आवरणम् = ((1u << (REV_सरणम् + अङ्कः_rev)) - 1);
	all_revs = all_आवरणम् & ~((1u << REV_सरणम्) - 1);

	प्रति (संरचना commit_list *l = आलोकितम्; l; l = l->अग्र) {
		संरचना commit *commit = l->item;
		पूर्णाङ्क this_flag = commit->object.flags;
		पूर्णाङ्क is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		यदि (1 < अङ्कः_rev) {
			पूर्णाङ्क is_merge = !!(commit->parents &&
					  commit->parents->अग्र);
			यदि (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_सरणम्)))
				अनुवर्त;
			यदि (!sparse && is_merge &&
			    omit_अन्तर्_dense(commit, rev, अङ्कः_rev))
				अनुवर्त;
			प्रति (i = 0; i < अङ्कः_rev; i++) {
				पूर्णाङ्क mark;
				यदि (!(this_flag & (1u << (i + REV_सरणम्))))
					mark = ' ';
				अन्यथा यदि (is_merge)
					mark = '-';
				अन्यथा यदि (i == शीर्ष_at)
					mark = '*';
				अन्यथा
					mark = '+';
				यदि (mark == ' ')
					अक्षरं_लिख(mark);
				अन्यथा
					प्रारूप_लिख("%s%c%s",
					       get_color_संहिता(i),
					       mark, get_color_reset_संहिता());
			}
			अक्षरं_लिख(' ');
		}
		show_one_commit(commit, no_name);

		यदि (shown_merge_point && --extra < 0)
			भङ्ग;
	}

	ret = 0;

बहिर्गमनम्:
	प्रति (आकार_t i = 0; i < ARRAY_आकार(reflog_सन्देशः); i++)
		मुक्त(reflog_सन्देशः[i]);
	commit_list_free(आलोकितम्);
	clear_prio_queue(&queue);
	मुक्त(तर्काः_copy);
	मुक्त(शीर्ष);
	प्रत्यागम ret;
}
