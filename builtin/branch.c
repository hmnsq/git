/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "strmap.h"
#अन्तर्भाव "column.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "ref-filter.h"
#अन्तर्भाव "worktree.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "commit-reach.h"

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_branch_usage[] = {
	N_("git branch [<options>] [-r | -a] [--merged] [--no-merged] [(--forked <branch>)...]"),
	N_("git branch [<options>] [-f] [--recurse-submodules] <branch-name> [<start-point>]"),
	N_("git branch [<options>] [-l] [<pattern>...]"),
	N_("git branch [<options>] [-r] (-d | -D) <branch-name>..."),
	N_("git branch [<options>] (-m | -M) [<old-branch>] <new-branch>"),
	N_("git branch [<options>] (-c | -C) [<old-branch>] <new-branch>"),
	N_("git branch [<options>] [-r | -a] [--points-at]"),
	N_("git branch [<options>] [-r | -a] [--format]"),
	N_("git branch [<options>] (--delete-merged <pattern>)... "
	   "[<branch-pattern>...]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर *head;
स्थिर संरचना object_id head_oid;
स्थिर पूर्णांक recurse_submodules = 0;
स्थिर पूर्णांक submodule_propagate_branches = 0;

स्थिर गणना git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
स्थिर अक्षर branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
गणना color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

स्थिर स्थिरांक अक्षर *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

स्थिर संरचना string_list output = STRING_LIST_INIT_DUP;
स्थिर अचिन्हित पूर्णांक colopts;

define_list_config_array(color_branch_slots);

स्थिर पूर्णांक git_branch_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			     स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	स्थिरांक अक्षर *slot_name;

	जर (!म_भेद(var, "branch.sort")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		string_list_append(cb, मूल्य);
		परत 0;
	}

	जर (starts_with(var, "column."))
		परत git_column_config(var, मूल्य, "branch", &colopts);
	जर (!म_भेद(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, मूल्य);
		परत 0;
	}
	जर (skip_prefix(var, "color.branch.", &slot_name)) {
		पूर्णांक slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		जर (slot < 0)
			परत 0;
		जर (!मूल्य)
			परत config_error_nonbool(var);
		परत color_parse(मूल्य, branch_colors[slot]);
	}
	जर (!म_भेद(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, मूल्य);
		परत 0;
	}
	जर (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (git_color_config(var, मूल्य, cb) < 0)
		परत -1;

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर स्थिरांक अक्षर *branch_get_color(गणना color_branch ix)
{
	जर (want_color(branch_use_color))
		परत branch_colors[ix];
	परत "";
}

स्थिर पूर्णांक branch_merged(पूर्णांक kind, स्थिरांक अक्षर *name,
			 संरचना commit *rev, संरचना commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	संरचना commit *reference_rev = रिक्त;
	स्थिरांक अक्षर *reference_name = रिक्त;
	शून्य *reference_name_to_free = रिक्त;
	पूर्णांक merged;

	जर (kind == FILTER_REFS_BRANCHES) {
		संरचना branch *branch = branch_get(name);
		स्थिरांक अक्षर *upstream = branch_get_upstream(branch, रिक्त);
		संरचना object_id oid;

		जर (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, रिक्त)) != रिक्त)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	जर (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	जर (merged < 0)
		निकास(128);

	/*
	 * After the safety valve is fully redefined to "check with
	 * upstream, if any, otherwise with HEAD", we should just
	 * return the result of the repo_in_merge_bases() above without
	 * any of the following code, but during the transition period,
	 * a gentle reminder is in order.  Callers that opt out of the
	 * HEAD fallback by passing head_rev=NULL are not interested in
	 * the reminder either: they have already established that the
	 * branch has an upstream, so HEAD is irrelevant to the decision.
	 */
	जर (head_rev && head_rev != reference_rev) {
		पूर्णांक expect = repo_in_merge_bases(the_repository, rev, head_rev);
		जर (expect < 0)
			निकास(128);
		जर (expect == merged)
			; /* okay */
		अन्यथा जर (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		अन्यथा
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	मुक्त(reference_name_to_free);
	परत merged;
}

गणना delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

स्थिर पूर्णांक check_branch_commit(स्थिरांक अक्षर *branchname, स्थिरांक अक्षर *refname,
			       स्थिरांक संरचना object_id *oid, संरचना commit *head_rev,
			       पूर्णांक kinds, अचिन्हित पूर्णांक flags)
{
	संरचना commit *rev = lookup_commit_reference(the_repository, oid);
	जर (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		परत -1;
	}
	जर (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		जर (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		परत -1;
	}
	परत 0;
}

स्थिर शून्य delete_branch_config(स्थिरांक अक्षर *branchname)
{
	संरचना strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	जर (repo_config_rename_section(the_repository, buf.buf, रिक्त) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

स्थिर पूर्णांक delete_branches(पूर्णांक argc, स्थिरांक अक्षर **argv, पूर्णांक kinds,
			   अचिन्हित पूर्णांक flags)
{
	संरचना commit *head_rev = रिक्त;
	संरचना object_id oid;
	अक्षर *name = रिक्त;
	स्थिरांक अक्षर *fmt;
	पूर्णांक i;
	पूर्णांक ret = 0;
	पूर्णांक remote_branch = 0;
	संरचना strbuf bname = STRBUF_INIT;
	गणना interpret_branch_kind allowed_interpret;
	संरचना string_list refs_to_delete = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;
	पूर्णांक branch_name_pos;
	स्थिरांक अक्षर *fmt_remotes = "refs/remotes/%s";

	निवड (kinds) {
	स्थिती FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		थांबा;
	स्थिती FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		थांबा;
	पूर्वनिर्धारित:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = म_खोज(fmt, "%");

	जर (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	साठी (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		अक्षर *target = रिक्त;
		पूर्णांक ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		मुक्त(name);
		name = mkpathdup(fmt, bname.buf);

		जर (kinds == FILTER_REFS_BRANCHES) {
			स्थिरांक अक्षर *path;
			जर ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				सुरूठेवा;
			}
			जर ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				सुरूठेवा;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		जर (!target) {
			जर (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} अन्यथा {
				अक्षर *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				अक्षर *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				जर (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				अन्यथा
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			सुरूठेवा;
		}

		जर (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			जर (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			जा next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		मुक्त(target);
	}

	जर (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), रिक्त, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		अक्षर *describe_ref = item->util;
		अक्षर *name = item->string;
		जर (flags & DELETE_BRANCH_DRY_RUN) {
			जर (!(flags & DELETE_BRANCH_QUIET))
				म_लिखो(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} अन्यथा जर (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			अक्षर *refname = name + branch_name_pos;
			जर (!(flags & DELETE_BRANCH_QUIET))
				म_लिखो(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		मुक्त(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	मुक्त(name);
	strbuf_release(&bname);

	परत ret;
}

स्थिर पूर्णांक calc_maxwidth(संरचना ref_array *refs, पूर्णांक remote_bonus)
{
	पूर्णांक i, max = 0;
	साठी (i = 0; i < refs->nr; i++) {
		संरचना ref_array_item *it = refs->वस्तुसंच[i];
		स्थिरांक अक्षर *desc = it->refname;
		पूर्णांक w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		जर (it->kind == FILTER_REFS_DETACHED_HEAD) {
			अक्षर *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			मुक्त(head_desc);
		} अन्यथा
			w = utf8_strwidth(desc);

		जर (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		जर (w > max)
			max = w;
	}
	परत max;
}

स्थिर स्थिरांक अक्षर *quote_literal_for_format(स्थिरांक अक्षर *s)
{
	स्थिर संरचना strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	पर्यंत (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	परत buf.buf;
}

स्थिर अक्षर *build_format(संरचना ref_filter *filter, पूर्णांक maxwidth, स्थिरांक अक्षर *remote_prefix)
{
	संरचना strbuf fmt = STRBUF_INIT;
	संरचना strbuf local = STRBUF_INIT;
	संरचना strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	जर (filter->verbose) {
		संरचना strbuf obname = STRBUF_INIT;

		जर (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		अन्यथा जर (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		अन्यथा
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		जर (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		अन्यथा
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} अन्यथा {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	परत strbuf_detach(&fmt, रिक्त);
}

स्थिर शून्य print_ref_list(संरचना ref_filter *filter, संरचना ref_sorting *sorting,
			   संरचना ref_format *format, संरचना string_list *output)
{
	पूर्णांक i;
	संरचना ref_array array;
	पूर्णांक maxwidth = 0;
	स्थिरांक अक्षर *remote_prefix = "";
	अक्षर *to_free = रिक्त;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	जर (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	स_रखो(&array, 0, आकार(array));

	filter_refs(&array, filter, filter->kind);

	जर (filter->verbose)
		maxwidth = calc_maxwidth(&array, म_माप(remote_prefix));

	जर (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	जर (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	जर (column_active(colopts)) {
		संरचना strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		प्रतिज्ञा(!filter->verbose && "--column and --verbose are incompatible");

		साठी (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			जर (format_ref_array_item(array.वस्तुसंच[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} अन्यथा {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	मुक्त(to_free);
}

स्थिर शून्य print_current_branch_name(शून्य)
{
	पूर्णांक flags;
	स्थिरांक अक्षर *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, रिक्त, &flags);
	स्थिरांक अक्षर *shortname;
	जर (!refname)
		die(_("could not resolve HEAD"));
	अन्यथा जर (!(flags & REF_ISSYMREF))
		परत;
	अन्यथा जर (skip_prefix(refname, "refs/heads/", &shortname))
		माला_दो(shortname);
	अन्यथा
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

स्थिर शून्य reject_rebase_or_bisect_branch(संरचना worktree **worktrees,
					   स्थिरांक अक्षर *target)
{
	पूर्णांक i;

	साठी (i = 0; worktrees[i]; i++) {
		संरचना worktree *wt = worktrees[i];

		जर (!wt->is_detached)
			सुरूठेवा;

		जर (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		जर (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
स्थिर पूर्णांक replace_each_worktree_head_symref(संरचना worktree **worktrees,
					     स्थिरांक अक्षर *oldref, स्थिरांक अक्षर *newref,
					     स्थिरांक अक्षर *logmsg)
{
	पूर्णांक ret = 0;
	पूर्णांक i;

	साठी (i = 0; worktrees[i]; i++) {
		संरचना ref_store *refs;

		जर (worktrees[i]->is_detached)
			सुरूठेवा;
		जर (!worktrees[i]->head_ref)
			सुरूठेवा;
		जर (म_भेद(oldref, worktrees[i]->head_ref))
			सुरूठेवा;

		refs = get_worktree_ref_store(worktrees[i]);
		जर (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	परत ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

स्थिर शून्य copy_or_rename_branch(स्थिरांक अक्षर *oldname, स्थिरांक अक्षर *newname, पूर्णांक copy, पूर्णांक force)
{
	संरचना strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	संरचना strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	स्थिरांक अक्षर *interpreted_oldname = रिक्त;
	स्थिरांक अक्षर *interpreted_newname = रिक्त;
	पूर्णांक recovery = 0, oldref_usage = 0;
	संरचना worktree **worktrees = get_worktrees(the_repository);

	जर (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		जर (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		अन्यथा {
			पूर्णांक code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			निकास(code);
		}
	}

	साठी (पूर्णांक i = 0; worktrees[i]; i++) {
		संरचना worktree *wt = worktrees[i];

		जर (wt->head_ref && !म_भेद(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			जर (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			थांबा;
		}
	}

	जर ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		जर (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		अन्यथा
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	जर (!म_भेद(oldname, newname))
		validate_branchname(newname, &newref);
	अन्यथा
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	जर (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	जर (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	अन्यथा
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	जर (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	जर (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	जर (recovery) {
		जर (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		अन्यथा
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	जर (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	जर (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	जर (copy && म_भेद(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

स्थिर पूर्णांक parse_opt_forked(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना ref_filter *filter = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	जर (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	परत 0;
}

संरचना stacked_branch_data {
	संरचना strset *deletable_branch_names;
	संरचना strset *protected_branch_names;
};

स्थिर पूर्णांक collect_stacked_branch_base(स्थिरांक संरचना reference *ref,
				       शून्य *cb_data)
{
	संरचना stacked_branch_data *data = cb_data;
	स्थिरांक अक्षर *branch_name;
	संरचना branch *branch;
	स्थिरांक अक्षर *upstream_refname;
	स्थिरांक अक्षर *upstream_branch_name;

	जर (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	जर (strset_contains(data->deletable_branch_names, branch_name))
		परत 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, रिक्त);
	जर (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		परत 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	परत 0;
}

स्थिर शून्य protect_stacked_branch_bases(संरचना ref_store *refs,
					 संरचना strset *deletable_branch_names,
					 संरचना strset *protected_branch_names)
{
	संरचना stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	संरचना refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	संरचना hashmap_iter iter;
	संरचना strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->किल्ली);
}

स्थिर शून्य clear_deleted_upstreams(संरचना strset *protected_branch_names,
				    संरचना strset *deletable_branch_names)
{
	संरचना strbuf किल्ली = STRBUF_INIT;
	संरचना hashmap_iter iter;
	संरचना strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		संरचना branch *branch = branch_get(entry->किल्ली);
		स्थिरांक अक्षर *upstream_refname = branch_get_upstream(branch, रिक्त);
		स्थिरांक अक्षर *upstream_branch_name;

		जर (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			सुरूठेवा;

		strbuf_addf(&किल्ली, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, किल्ली.buf, रिक्त);
		strbuf_reset(&किल्ली);
		strbuf_addf(&किल्ली, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, किल्ली.buf, रिक्त);
		strbuf_reset(&किल्ली);
	}

	strbuf_release(&किल्ली);
}

स्थिर पूर्णांक branch_pushes_to_upstream(संरचना branch *branch,
				     स्थिरांक अक्षर *upstream)
{
	संरचना remote *remote = remote_get(remote_for_branch(branch, रिक्त));
	अक्षर *ढकला_refname = रिक्त;
	अक्षर *tracking = रिक्त;
	पूर्णांक ret = 0;

	जर (!remote)
		परत 0;
	जर (remote->ढकला.nr)
		ढकला_refname = apply_refspecs(&remote->ढकला, branch->refname);
	अन्यथा
		ढकला_refname = xstrdup(branch->refname);
	जर (ढकला_refname)
		tracking = apply_refspecs(&remote->fetch, ढकला_refname);
	जर (tracking && !म_भेद(tracking, upstream))
		ret = 1;

	मुक्त(ढकला_refname);
	मुक्त(tracking);
	परत ret;
}

स्थिर पूर्णांक delete_merged_branches(स्थिरांक संरचना strvec *upstreams,
				 स्थिरांक अक्षर **argv, अचिन्हित पूर्णांक flags)
{
	संरचना ref_store *refs = get_main_ref_store(the_repository);
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_array candidates = { 0 };
	संरचना strset deletable_branch_names = STRSET_INIT;
	संरचना strset protected_branch_names = STRSET_INIT;
	संरचना strvec branches_to_delete = STRVEC_INIT;
	संरचना strbuf किल्ली = STRBUF_INIT;
	संरचना hashmap_iter iter;
	संरचना strmap_entry *entry;
	पूर्णांक ret = 0;

	साठी (माप_प्रकार i = 0; i < upstreams->nr; i++)
		जर (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	साठी (पूर्णांक i = 0; i < candidates.nr; i++) {
		स्थिरांक अक्षर *branch_refname = candidates.वस्तुसंच[i]->refname;
		स्थिरांक अक्षर *branch_name;
		संरचना branch *branch;
		स्थिरांक अक्षर *upstream_refname;
		पूर्णांक opt_out;

		जर (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		जर (branch_checked_out(branch_refname))
			सुरूठेवा;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, रिक्त);
		जर (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			सुरूठेवा;
		जर (branch_pushes_to_upstream(branch, upstream_refname))
			सुरूठेवा;
		जर (check_branch_commit(branch_name, branch_name,
					&candidates.वस्तुसंच[i]->objectname, रिक्त,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			सुरूठेवा;

		strbuf_reset(&किल्ली);
		strbuf_addf(&किल्ली, "branch.%s.deletemerged", branch_name);
		जर (!repo_config_get_bool(the_repository, किल्ली.buf, &opt_out) &&
		    !opt_out) {
			जर (!(flags & DELETE_BRANCH_QUIET))
				ख_लिखो(मानक_त्रुटि,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			सुरूठेवा;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_ढकला(&branches_to_delete, entry->किल्ली);

	जर (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	जर (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&किल्ली);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	परत ret;
}

स्थिर GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

स्थिर पूर्णांक edit_branch_description(स्थिरांक अक्षर *branch_name)
{
	पूर्णांक exists;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	जर (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_ओळ_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_ओळ_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	जर (launch_editor(edit_description(), &buf, रिक्त)) {
		strbuf_release(&buf);
		परत -1;
	}
	strbuf_stripspace(&buf, comment_ओळ_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	जर (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : रिक्त);
	strbuf_release(&name);
	strbuf_release(&buf);

	परत 0;
}

स्थिर शून्य die_if_upstream_looks_like_remote(स्थिरांक अक्षर *new_upstream, स्थिरांक अक्षर *branch_name)
{
	संरचना strbuf remote_ref = STRBUF_INIT;
	पूर्णांक code;

	जर (म_अक्षर(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		परत;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	जर (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		परत;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	निकास(code);
}

पूर्णांक cmd_branch(पूर्णांक argc,
	       स्थिरांक अक्षर **argv,
	       स्थिरांक अक्षर *prefix,
	       संरचना repository *repo UNUSED)
{
	/* possible actions */
	पूर्णांक delete = 0, नाम = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	संरचना strvec delete_merged = STRVEC_INIT;
	पूर्णांक dry_run = 0;
	स्थिरांक अक्षर *new_upstream = रिक्त;
	पूर्णांक noncreate_actions = 0;
	/* possible options */
	पूर्णांक reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	गणना branch_track track;
	संरचना ref_filter filter = REF_FILTER_INIT;
	स्थिर संरचना ref_sorting *sorting;
	संरचना string_list sorting_options = STRING_LIST_INIT_DUP;
	संरचना ref_format format = REF_FORMAT_INIT;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);
	पूर्णांक ret;

	संरचना option options[] = {
		OPT_GROUP(N_("Generic options")),
		OPT__VERBOSE(&filter.verbose,
			N_("show hash and subject, give twice for upstream branch")),
		OPT__QUIET(&quiet, N_("suppress informational messages")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_mode),
		OPT_SET_INT_F(0, "set-upstream", &track, N_("do not use"),
			BRANCH_TRACK_OVERRIDE, PARSE_OPT_HIDDEN),
		OPT_STRING('u', "set-upstream-to", &new_upstream, N_("upstream"), N_("change the upstream info")),
		OPT_BOOL(0, "unset-upstream", &unset_upstream, N_("unset the upstream info")),
		OPT__COLOR(&branch_use_color, N_("use colored output")),
		OPT_SET_INT_F('r', "remotes",     &filter.kind, N_("act on remote-tracking branches"),
			      FILTER_REFS_REMOTES,
			      PARSE_OPT_NONEG),
		OPT_CONTAINS(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_NO_CONTAINS(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT__ABBREV(&filter.abbrev),

		OPT_GROUP(N_("Specific git-branch actions:")),
		OPT_SET_INT_F('a', "all", &filter.kind, N_("list both remote-tracking and local branches"),
			      FILTER_REFS_REMOTES | FILTER_REFS_BRANCHES,
			      PARSE_OPT_NONEG),
		OPT_BIT('d', "delete", &delete, N_("delete fully merged branch"), 1),
		OPT_BIT('D', रिक्त, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &नाम, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', रिक्त, &नाम, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', रिक्त, &copy, N_("copy a branch, even if target exists"), 2),
		OPT_BOOL('l', "list", &list, N_("list branch names")),
		OPT_BOOL(0, "show-current", &show_current, N_("show current branch name")),
		OPT_BOOL(0, "create-reflog", &reflog, N_("create the branch's reflog")),
		OPT_BOOL(0, "edit-description", &edit_description,
			 N_("edit the description for the branch")),
		OPT_CALLBACK_F(0, "delete-merged", &delete_merged, N_("pattern"),
			N_("delete merged branches whose upstream matches <pattern> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_strvec),
		OPT_BOOL(0, "dry-run", &dry_run,
			N_("with --delete-merged, only print which branches would be deleted")),
		OPT__FORCE(&force, N_("force creation, move/rename, deletion"), PARSE_OPT_NOCOMPLETE),
		OPT_MERGED(&filter, N_("print only branches that are merged")),
		OPT_NO_MERGED(&filter, N_("print only branches that are not merged")),
		OPT_CALLBACK_F(0, "forked", &filter, N_("branch"),
			N_("print only branches whose upstream matches <branch> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_forked),
		OPT_COLUMN(0, "column", &colopts, N_("list branches in columns")),
		OPT_REF_SORT(&sorting_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at, N_("object"),
			N_("print only branches of the object"), parse_opt_object_name),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules_explicit, N_("recurse through submodules")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_END(),
	};

	setup_ref_filter_porcelain_संदेश();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	जर (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, रिक्त);
	जर (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	जर (!म_भेद(head, "HEAD"))
		filter.detached = 1;
	अन्यथा जर (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	जर (!delete && !नाम && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	जर (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!नाम + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	जर (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	जर (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	जर (recurse_submodules_explicit) {
		जर (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		जर (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	जर (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	जर (filter.verbose) {
		जर (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	जर (force) {
		delete *= 2;
		नाम *= 2;
		copy *= 2;
	}

	जर (list)
		setup_auto_pager("branch", 1);

	जर (delete) {
		जर (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		जा out;
	} अन्यथा जर (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		जा out;
	} अन्यथा जर (show_current) {
		print_current_branch_name();
		ret = 0;
		जा out;
	} अन्यथा जर (list) {
		/*  git branch --list also shows HEAD when it is detached */
		जर ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
			filter.kind |= FILTER_REFS_DETACHED_HEAD;
		filter.name_patterns = argv;
		/*
		 * If no sorting parameter is given then we default to sorting
		 * by 'refname'. This would give us an alphabetically sorted
		 * array with the 'HEAD' ref at the beginning followed by
		 * local branches 'refs/heads/...' and finally remote-tracking
		 * branches 'refs/remotes/...'.
		 */
		sorting = ref_sorting_options(&sorting_options);
		ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
		ref_sorting_set_sort_flags_all(
			sorting, REF_SORTING_DETACHED_HEAD_FIRST, 1);
		print_ref_list(&filter, sorting, &format, &output);
		print_columns(&output, colopts, रिक्त);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		जा out;
	} अन्यथा जर (edit_description) {
		स्थिरांक अक्षर *branch_name;
		संरचना strbuf branch_ref = STRBUF_INIT;
		संरचना strbuf buf = STRBUF_INIT;

		जर (!argc) {
			जर (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} अन्यथा जर (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} अन्यथा {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		जर (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} अन्यथा जर (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} अन्यथा {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		जा out;
	} अन्यथा जर (copy || नाम) {
		जर (!argc)
			die(_("branch name required"));
		अन्यथा जर ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		अन्यथा जर (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + नाम > 1);
		अन्यथा जर (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + नाम > 1);
		अन्यथा
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} अन्यथा जर (new_upstream) {
		संरचना branch *branch;
		संरचना strbuf buf = STRBUF_INIT;

		जर (!argc)
			branch = branch_get(रिक्त);
		अन्यथा जर (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} अन्यथा
			die(_("too many arguments to set new upstream"));

		जर (!branch) {
			जर (!argc || !म_भेद(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		जर (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			जर (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			जर (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} अन्यथा जर (unset_upstream) {
		संरचना branch *branch;
		संरचना strbuf buf = STRBUF_INIT;

		जर (!argc)
			branch = branch_get(रिक्त);
		अन्यथा जर (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} अन्यथा
			die(_("too many arguments to unset upstream"));

		जर (!branch) {
			जर (!argc || !म_भेद(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		जर (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, रिक्त, रिक्त, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, रिक्त, रिक्त, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} अन्यथा जर (!noncreate_actions && argc > 0 && argc <= 2) {
		स्थिरांक अक्षर *branch_name = argv[0];
		स्थिरांक अक्षर *start_name = argc == 2 ? argv[1] : head;

		जर (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		जर (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		जर (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, रिक्त, force,
						    reflog, quiet, track, 0);
			ret = 0;
			जा out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} अन्यथा
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	परत ret;
}
