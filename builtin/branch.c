/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "color.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "refs.h"
#समावेश "commit.h"
#समावेश "gettext.h"
#समावेश "object-name.h"
#समावेश "remote.h"
#समावेश "parse-options.h"
#समावेश "branch.h"
#समावेश "path.h"
#समावेश "string-list.h"
#समावेश "strmap.h"
#समावेश "column.h"
#समावेश "utf8.h"
#समावेश "ref-filter.h"
#समावेश "worktree.h"
#समावेश "help.h"
#समावेश "advice.h"
#समावेश "commit-reach.h"

अटल स्थिर अक्षर * स्थिर builtin_branch_usage[] = {
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
	शून्य
};

अटल स्थिर अक्षर *शीर्ष;
अटल काष्ठा object_id शीर्ष_oid;
अटल पूर्णांक recurse_submodules = 0;
अटल पूर्णांक submodule_propagate_branches = 0;

अटल क्रमागत git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
अटल अक्षर branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
क्रमागत color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

अटल स्थिर अक्षर *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

अटल काष्ठा string_list output = STRING_LIST_INIT_DUP;
अटल अचिन्हित पूर्णांक colopts;

define_list_config_array(color_branch_slots);

अटल पूर्णांक git_branch_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			     स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	स्थिर अक्षर *slot_name;

	अगर (!म_भेद(var, "branch.sort")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		string_list_append(cb, मूल्य);
		वापस 0;
	}

	अगर (starts_with(var, "column."))
		वापस git_column_config(var, मूल्य, "branch", &colopts);
	अगर (!म_भेद(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, मूल्य);
		वापस 0;
	}
	अगर (skip_prefix(var, "color.branch.", &slot_name)) {
		पूर्णांक slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		अगर (slot < 0)
			वापस 0;
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		वापस color_parse(मूल्य, branch_colors[slot]);
	}
	अगर (!म_भेद(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (git_color_config(var, मूल्य, cb) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल स्थिर अक्षर *branch_get_color(क्रमागत color_branch ix)
{
	अगर (want_color(branch_use_color))
		वापस branch_colors[ix];
	वापस "";
}

अटल पूर्णांक branch_merged(पूर्णांक kind, स्थिर अक्षर *name,
			 काष्ठा commit *rev, काष्ठा commit *शीर्ष_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	काष्ठा commit *reference_rev = शून्य;
	स्थिर अक्षर *reference_name = शून्य;
	व्योम *reference_name_to_free = शून्य;
	पूर्णांक merged;

	अगर (kind == FILTER_REFS_BRANCHES) {
		काष्ठा branch *branch = branch_get(name);
		स्थिर अक्षर *upstream = branch_get_upstream(branch, शून्य);
		काष्ठा object_id oid;

		अगर (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, शून्य)) != शून्य)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	अगर (!reference_rev)
		reference_rev = शीर्ष_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	अगर (merged < 0)
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
	अगर (शीर्ष_rev && शीर्ष_rev != reference_rev) {
		पूर्णांक expect = repo_in_merge_bases(the_repository, rev, शीर्ष_rev);
		अगर (expect < 0)
			निकास(128);
		अगर (expect == merged)
			; /* okay */
		अन्यथा अगर (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		अन्यथा
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	मुक्त(reference_name_to_free);
	वापस merged;
}

क्रमागत delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_शीर्ष_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

अटल पूर्णांक check_branch_commit(स्थिर अक्षर *branchname, स्थिर अक्षर *refname,
			       स्थिर काष्ठा object_id *oid, काष्ठा commit *शीर्ष_rev,
			       पूर्णांक kinds, अचिन्हित पूर्णांक flags)
{
	काष्ठा commit *rev = lookup_commit_reference(the_repository, oid);
	अगर (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		वापस -1;
	}
	अगर (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, शीर्ष_rev)) {
		अगर (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		वापस -1;
	}
	वापस 0;
}

अटल व्योम delete_branch_config(स्थिर अक्षर *branchname)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	अगर (repo_config_rename_section(the_repository, buf.buf, शून्य) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

अटल पूर्णांक delete_branches(पूर्णांक argc, स्थिर अक्षर **argv, पूर्णांक kinds,
			   अचिन्हित पूर्णांक flags)
{
	काष्ठा commit *शीर्ष_rev = शून्य;
	काष्ठा object_id oid;
	अक्षर *name = शून्य;
	स्थिर अक्षर *fmt;
	पूर्णांक i;
	पूर्णांक ret = 0;
	पूर्णांक remote_branch = 0;
	काष्ठा strbuf bname = STRBUF_INIT;
	क्रमागत interpret_branch_kind allowed_interpret;
	काष्ठा string_list refs_to_delete = STRING_LIST_INIT_DUP;
	काष्ठा string_list_item *item;
	पूर्णांक branch_name_pos;
	स्थिर अक्षर *fmt_remotes = "refs/remotes/%s";

	चयन (kinds) {
	हाल FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		अवरोध;
	हाल FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		अवरोध;
	शेष:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = म_खोज(fmt, "%");

	अगर (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_शीर्ष_FALLBACK))
		शीर्ष_rev = lookup_commit_reference(the_repository, &शीर्ष_oid);

	क्रम (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		अक्षर *target = शून्य;
		पूर्णांक ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		मुक्त(name);
		name = mkpathdup(fmt, bname.buf);

		अगर (kinds == FILTER_REFS_BRANCHES) {
			स्थिर अक्षर *path;
			अगर ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				जारी;
			}
			अगर ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				जारी;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		अगर (!target) {
			अगर (remote_branch) {
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

				अगर (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				अन्यथा
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			जारी;
		}

		अगर (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, शीर्ष_rev, kinds,
					flags)) {
			अगर (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			जाओ अग्र;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	अग्र:
		मुक्त(target);
	}

	अगर (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), शून्य, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		अक्षर *describe_ref = item->util;
		अक्षर *name = item->string;
		अगर (flags & DELETE_BRANCH_DRY_RUN) {
			अगर (!(flags & DELETE_BRANCH_QUIET))
				म_लिखो(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} अन्यथा अगर (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			अक्षर *refname = name + branch_name_pos;
			अगर (!(flags & DELETE_BRANCH_QUIET))
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

	वापस ret;
}

अटल पूर्णांक calc_maxwidth(काष्ठा ref_array *refs, पूर्णांक remote_bonus)
{
	पूर्णांक i, max = 0;
	क्रम (i = 0; i < refs->nr; i++) {
		काष्ठा ref_array_item *it = refs->वस्तुएँ[i];
		स्थिर अक्षर *desc = it->refname;
		पूर्णांक w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		अगर (it->kind == FILTER_REFS_DETACHED_शीर्ष) {
			अक्षर *शीर्ष_desc = get_शीर्ष_description();
			w = utf8_strwidth(शीर्ष_desc);
			मुक्त(शीर्ष_desc);
		} अन्यथा
			w = utf8_strwidth(desc);

		अगर (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		अगर (w > max)
			max = w;
	}
	वापस max;
}

अटल स्थिर अक्षर *quote_literal_for_format(स्थिर अक्षर *s)
{
	अटल काष्ठा strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	जबतक (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	वापस buf.buf;
}

अटल अक्षर *build_format(काष्ठा ref_filter *filter, पूर्णांक maxwidth, स्थिर अक्षर *remote_prefix)
{
	काष्ठा strbuf fmt = STRBUF_INIT;
	काष्ठा strbuf local = STRBUF_INIT;
	काष्ठा strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	अगर (filter->verbose) {
		काष्ठा strbuf obname = STRBUF_INIT;

		अगर (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		अन्यथा अगर (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		अन्यथा
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		अगर (filter->verbose > 1)
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
	वापस strbuf_detach(&fmt, शून्य);
}

अटल व्योम print_ref_list(काष्ठा ref_filter *filter, काष्ठा ref_sorting *sorting,
			   काष्ठा ref_format *format, काष्ठा string_list *output)
{
	पूर्णांक i;
	काष्ठा ref_array array;
	पूर्णांक maxwidth = 0;
	स्थिर अक्षर *remote_prefix = "";
	अक्षर *to_free = शून्य;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	अगर (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	स_रखो(&array, 0, माप(array));

	filter_refs(&array, filter, filter->kind);

	अगर (filter->verbose)
		maxwidth = calc_maxwidth(&array, म_माप(remote_prefix));

	अगर (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	अगर (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	अगर (column_active(colopts)) {
		काष्ठा strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		निश्चित(!filter->verbose && "--column and --verbose are incompatible");

		क्रम (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			अगर (format_ref_array_item(array.वस्तुएँ[i], format, &out, &err))
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

अटल व्योम print_current_branch_name(व्योम)
{
	पूर्णांक flags;
	स्थिर अक्षर *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, शून्य, &flags);
	स्थिर अक्षर *shortname;
	अगर (!refname)
		die(_("could not resolve HEAD"));
	अन्यथा अगर (!(flags & REF_ISSYMREF))
		वापस;
	अन्यथा अगर (skip_prefix(refname, "refs/heads/", &shortname))
		माला_दो(shortname);
	अन्यथा
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

अटल व्योम reject_rebase_or_bisect_branch(काष्ठा worktree **worktrees,
					   स्थिर अक्षर *target)
{
	पूर्णांक i;

	क्रम (i = 0; worktrees[i]; i++) {
		काष्ठा worktree *wt = worktrees[i];

		अगर (!wt->is_detached)
			जारी;

		अगर (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		अगर (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
अटल पूर्णांक replace_each_worktree_शीर्ष_symref(काष्ठा worktree **worktrees,
					     स्थिर अक्षर *oldref, स्थिर अक्षर *newref,
					     स्थिर अक्षर *logmsg)
{
	पूर्णांक ret = 0;
	पूर्णांक i;

	क्रम (i = 0; worktrees[i]; i++) {
		काष्ठा ref_store *refs;

		अगर (worktrees[i]->is_detached)
			जारी;
		अगर (!worktrees[i]->शीर्ष_ref)
			जारी;
		अगर (म_भेद(oldref, worktrees[i]->शीर्ष_ref))
			जारी;

		refs = get_worktree_ref_store(worktrees[i]);
		अगर (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	वापस ret;
}

#घोषणा IS_शीर्ष 1
#घोषणा IS_ORPHAN 2

अटल व्योम copy_or_rename_branch(स्थिर अक्षर *oldname, स्थिर अक्षर *newname, पूर्णांक copy, पूर्णांक force)
{
	काष्ठा strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	काष्ठा strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	स्थिर अक्षर *interpreted_oldname = शून्य;
	स्थिर अक्षर *interpreted_newname = शून्य;
	पूर्णांक recovery = 0, oldref_usage = 0;
	काष्ठा worktree **worktrees = get_worktrees(the_repository);

	अगर (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		अगर (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		अन्यथा {
			पूर्णांक code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			निकास(code);
		}
	}

	क्रम (पूर्णांक i = 0; worktrees[i]; i++) {
		काष्ठा worktree *wt = worktrees[i];

		अगर (wt->शीर्ष_ref && !म_भेद(oldref.buf, wt->शीर्ष_ref)) {
			oldref_usage |= IS_शीर्ष;
			अगर (is_null_oid(&wt->शीर्ष_oid))
				oldref_usage |= IS_ORPHAN;
			अवरोध;
		}
	}

	अगर ((copy || !(oldref_usage & IS_शीर्ष)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		अगर (oldref_usage & IS_शीर्ष)
			die(_("no commit on branch '%s' yet"), oldname);
		अन्यथा
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	अगर (!म_भेद(oldname, newname))
		validate_branchname(newname, &newref);
	अन्यथा
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	अगर (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	अगर (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	अन्यथा
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	अगर (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	अगर (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	अगर (recovery) {
		अगर (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		अन्यथा
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	अगर (!copy && (oldref_usage & IS_शीर्ष) &&
	    replace_each_worktree_शीर्ष_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	अगर (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	अगर (copy && म_भेद(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

अटल पूर्णांक parse_opt_forked(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा ref_filter *filter = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	अगर (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	वापस 0;
}

काष्ठा stacked_branch_तथ्य {
	काष्ठा strset *deletable_branch_names;
	काष्ठा strset *protected_branch_names;
};

अटल पूर्णांक collect_stacked_branch_base(स्थिर काष्ठा reference *ref,
				       व्योम *cb_तथ्य)
{
	काष्ठा stacked_branch_तथ्य *तथ्य = cb_तथ्य;
	स्थिर अक्षर *branch_name;
	काष्ठा branch *branch;
	स्थिर अक्षर *upstream_refname;
	स्थिर अक्षर *upstream_branch_name;

	अगर (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	अगर (strset_contains(तथ्य->deletable_branch_names, branch_name))
		वापस 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, शून्य);
	अगर (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(तथ्य->deletable_branch_names,
			    upstream_branch_name))
		वापस 0;

	strset_add(तथ्य->protected_branch_names, upstream_branch_name);
	वापस 0;
}

अटल व्योम protect_stacked_branch_bases(काष्ठा ref_store *refs,
					 काष्ठा strset *deletable_branch_names,
					 काष्ठा strset *protected_branch_names)
{
	काष्ठा stacked_branch_तथ्य तथ्य = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	काष्ठा refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	काष्ठा hashmap_iter iter;
	काष्ठा strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &तथ्य, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->कुंजी);
}

अटल व्योम clear_deleted_upstreams(काष्ठा strset *protected_branch_names,
				    काष्ठा strset *deletable_branch_names)
{
	काष्ठा strbuf कुंजी = STRBUF_INIT;
	काष्ठा hashmap_iter iter;
	काष्ठा strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		काष्ठा branch *branch = branch_get(entry->कुंजी);
		स्थिर अक्षर *upstream_refname = branch_get_upstream(branch, शून्य);
		स्थिर अक्षर *upstream_branch_name;

		अगर (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			जारी;

		strbuf_addf(&कुंजी, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, कुंजी.buf, शून्य);
		strbuf_reset(&कुंजी);
		strbuf_addf(&कुंजी, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, कुंजी.buf, शून्य);
		strbuf_reset(&कुंजी);
	}

	strbuf_release(&कुंजी);
}

अटल पूर्णांक branch_pushes_to_upstream(काष्ठा branch *branch,
				     स्थिर अक्षर *upstream)
{
	काष्ठा remote *remote = remote_get(remote_for_branch(branch, शून्य));
	अक्षर *धकेलो_refname = शून्य;
	अक्षर *tracking = शून्य;
	पूर्णांक ret = 0;

	अगर (!remote)
		वापस 0;
	अगर (remote->धकेलो.nr)
		धकेलो_refname = apply_refspecs(&remote->धकेलो, branch->refname);
	अन्यथा
		धकेलो_refname = xstrdup(branch->refname);
	अगर (धकेलो_refname)
		tracking = apply_refspecs(&remote->fetch, धकेलो_refname);
	अगर (tracking && !म_भेद(tracking, upstream))
		ret = 1;

	मुक्त(धकेलो_refname);
	मुक्त(tracking);
	वापस ret;
}

अटल पूर्णांक delete_merged_branches(स्थिर काष्ठा strvec *upstreams,
				 स्थिर अक्षर **argv, अचिन्हित पूर्णांक flags)
{
	काष्ठा ref_store *refs = get_main_ref_store(the_repository);
	काष्ठा ref_filter filter = REF_FILTER_INIT;
	काष्ठा ref_array candidates = { 0 };
	काष्ठा strset deletable_branch_names = STRSET_INIT;
	काष्ठा strset protected_branch_names = STRSET_INIT;
	काष्ठा strvec branches_to_delete = STRVEC_INIT;
	काष्ठा strbuf कुंजी = STRBUF_INIT;
	काष्ठा hashmap_iter iter;
	काष्ठा strmap_entry *entry;
	पूर्णांक ret = 0;

	क्रम (माप_प्रकार i = 0; i < upstreams->nr; i++)
		अगर (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	क्रम (पूर्णांक i = 0; i < candidates.nr; i++) {
		स्थिर अक्षर *branch_refname = candidates.वस्तुएँ[i]->refname;
		स्थिर अक्षर *branch_name;
		काष्ठा branch *branch;
		स्थिर अक्षर *upstream_refname;
		पूर्णांक opt_out;

		अगर (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		अगर (branch_checked_out(branch_refname))
			जारी;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, शून्य);
		अगर (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			जारी;
		अगर (branch_pushes_to_upstream(branch, upstream_refname))
			जारी;
		अगर (check_branch_commit(branch_name, branch_name,
					&candidates.वस्तुएँ[i]->objectname, शून्य,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			जारी;

		strbuf_reset(&कुंजी);
		strbuf_addf(&कुंजी, "branch.%s.deletemerged", branch_name);
		अगर (!repo_config_get_bool(the_repository, कुंजी.buf, &opt_out) &&
		    !opt_out) {
			अगर (!(flags & DELETE_BRANCH_QUIET))
				ख_लिखो(मानक_त्रुटि,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			जारी;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_धकेलो(&branches_to_delete, entry->कुंजी);

	अगर (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_शीर्ष_FALLBACK |
				      flags);

	अगर (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&कुंजी);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	वापस ret;
}

अटल GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

अटल पूर्णांक edit_branch_description(स्थिर अक्षर *branch_name)
{
	पूर्णांक exists;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	अगर (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_पंक्ति_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_पंक्ति_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	अगर (launch_editor(edit_description(), &buf, शून्य)) {
		strbuf_release(&buf);
		वापस -1;
	}
	strbuf_stripspace(&buf, comment_पंक्ति_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	अगर (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : शून्य);
	strbuf_release(&name);
	strbuf_release(&buf);

	वापस 0;
}

अटल व्योम die_if_upstream_looks_like_remote(स्थिर अक्षर *new_upstream, स्थिर अक्षर *branch_name)
{
	काष्ठा strbuf remote_ref = STRBUF_INIT;
	पूर्णांक code;

	अगर (म_अक्षर(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		वापस;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	अगर (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		वापस;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	निकास(code);
}

पूर्णांक cmd_branch(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	/* possible actions */
	पूर्णांक delete = 0, नाम = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	काष्ठा strvec delete_merged = STRVEC_INIT;
	पूर्णांक dry_run = 0;
	स्थिर अक्षर *new_upstream = शून्य;
	पूर्णांक noncreate_actions = 0;
	/* possible options */
	पूर्णांक reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	क्रमागत branch_track track;
	काष्ठा ref_filter filter = REF_FILTER_INIT;
	अटल काष्ठा ref_sorting *sorting;
	काष्ठा string_list sorting_options = STRING_LIST_INIT_DUP;
	काष्ठा ref_format format = REF_FORMAT_INIT;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);
	पूर्णांक ret;

	काष्ठा option options[] = {
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
		OPT_BIT('D', शून्य, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &नाम, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', शून्य, &नाम, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', शून्य, &copy, N_("copy a branch, even if target exists"), 2),
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
	अगर (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	शीर्ष = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &शीर्ष_oid, शून्य);
	अगर (!शीर्ष)
		die(_("failed to resolve HEAD as a valid ref"));
	अगर (!म_भेद(शीर्ष, "HEAD"))
		filter.detached = 1;
	अन्यथा अगर (!skip_prefix(शीर्ष, "refs/heads/", &शीर्ष))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	अगर (!delete && !नाम && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	अगर (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!नाम + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	अगर (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	अगर (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	अगर (recurse_submodules_explicit) {
		अगर (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		अगर (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	अगर (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	अगर (filter.verbose) {
		अगर (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	अगर (force) {
		delete *= 2;
		नाम *= 2;
		copy *= 2;
	}

	अगर (list)
		setup_auto_pager("branch", 1);

	अगर (delete) {
		अगर (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		जाओ out;
	} अन्यथा अगर (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		जाओ out;
	} अन्यथा अगर (show_current) {
		print_current_branch_name();
		ret = 0;
		जाओ out;
	} अन्यथा अगर (list) {
		/*  git branch --list also shows HEAD when it is detached */
		अगर ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
			filter.kind |= FILTER_REFS_DETACHED_शीर्ष;
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
			sorting, REF_SORTING_DETACHED_शीर्ष_FIRST, 1);
		print_ref_list(&filter, sorting, &format, &output);
		print_columns(&output, colopts, शून्य);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		जाओ out;
	} अन्यथा अगर (edit_description) {
		स्थिर अक्षर *branch_name;
		काष्ठा strbuf branch_ref = STRBUF_INIT;
		काष्ठा strbuf buf = STRBUF_INIT;

		अगर (!argc) {
			अगर (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = शीर्ष;
		} अन्यथा अगर (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} अन्यथा {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		अगर (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} अन्यथा अगर (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} अन्यथा {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		जाओ out;
	} अन्यथा अगर (copy || नाम) {
		अगर (!argc)
			die(_("branch name required"));
		अन्यथा अगर ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		अन्यथा अगर (argc == 1)
			copy_or_rename_branch(शीर्ष, argv[0], copy, copy + नाम > 1);
		अन्यथा अगर (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + नाम > 1);
		अन्यथा
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} अन्यथा अगर (new_upstream) {
		काष्ठा branch *branch;
		काष्ठा strbuf buf = STRBUF_INIT;

		अगर (!argc)
			branch = branch_get(शून्य);
		अन्यथा अगर (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} अन्यथा
			die(_("too many arguments to set new upstream"));

		अगर (!branch) {
			अगर (!argc || !म_भेद(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		अगर (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			अगर (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			अगर (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} अन्यथा अगर (unset_upstream) {
		काष्ठा branch *branch;
		काष्ठा strbuf buf = STRBUF_INIT;

		अगर (!argc)
			branch = branch_get(शून्य);
		अन्यथा अगर (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} अन्यथा
			die(_("too many arguments to unset upstream"));

		अगर (!branch) {
			अगर (!argc || !म_भेद(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		अगर (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, शून्य, शून्य, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, शून्य, शून्य, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} अन्यथा अगर (!noncreate_actions && argc > 0 && argc <= 2) {
		स्थिर अक्षर *branch_name = argv[0];
		स्थिर अक्षर *start_name = argc == 2 ? argv[1] : शीर्ष;

		अगर (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		अगर (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		अगर (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, शून्य, force,
						    reflog, quiet, track, 0);
			ret = 0;
			जाओ out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} अन्यथा
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	वापस ret;
}
