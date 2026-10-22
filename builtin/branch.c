/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "strmap.h"
#ಅನ್ತರ್ಭಾವ "column.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "ref-filter.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_branch_usage[] = {
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
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ;
ಸ್ಥಿರ ರಚನೆ object_id ತಲೆ_oid;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ submodule_propagate_branches = 0;

ಸ್ಥಿರ ಎಣಿಕೆ git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
ಸ್ಥಿರ ಅಕ್ಷರ branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_ಸಾಮಾನ್ಯ,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_ಸಾಮಾನ್ಯ,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
ಎಣಿಕೆ color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

ಸ್ಥಿರ ರಚನೆ string_list output = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ colopts;

define_list_config_array(color_branch_slots);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_branch_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ_name;

	ಆದರೆ (!strcmp(var, "branch.sort")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		string_list_append(cb, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (starts_with(var, "column."))
		ಹಿಂತಿರುಗಿ git_ಸ್ತಂಭ_config(var, ಮೌಲ್ಯ, "branch", &colopts);
	ಆದರೆ (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(var, "color.branch.", &ಸ್ಥಾನ_name)) {
		ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = LOOKUP_CONFIG(color_branch_slots, ಸ್ಥಾನ_name);
		ಆದರೆ (ಸ್ಥಾನ < 0)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಹಿಂತಿರುಗಿ color_parse(ಮೌಲ್ಯ, branch_colors[ಸ್ಥಾನ]);
	}
	ಆದರೆ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_get_color(ಎಣಿಕೆ color_branch ix)
{
	ಆದರೆ (want_color(branch_use_color))
		ಹಿಂತಿರುಗಿ branch_colors[ix];
	ಹಿಂತಿರುಗಿ "";
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ branch_merged(ಪೂರ್ಣಾಂಕ ಬಗೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			 ರಚನೆ commit *rev, ರಚನೆ commit *ತಲೆ_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	ರಚನೆ commit *reference_rev = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reference_name = ಖಾಲಿ;
	ಶೂನ್ಯ *reference_name_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ merged;

	ಆದರೆ (ಬಗೆ == FILTER_REFS_BRANCHES) {
		ರಚನೆ branch *branch = branch_get(name);
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream = branch_get_upstream(branch, ಖಾಲಿ);
		ರಚನೆ object_id oid;

		ಆದರೆ (upstream &&
		    (reference_name = reference_name_ಗಮ್ಯ_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ಖಾಲಿ)) != ಖಾಲಿ)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ಆದರೆ (!reference_rev)
		reference_rev = ತಲೆ_rev;

	merged = reference_rev ? repo_ಒಳ_ವಿಲೀನ_bases(the_repository, rev,
						     reference_rev) : 0;
	ಆದರೆ (merged < 0)
		ನಿರ್ಗಮಿಸು(128);

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
	ಆದರೆ (ತಲೆ_rev && ತಲೆ_rev != reference_rev) {
		ಪೂರ್ಣಾಂಕ expect = repo_ಒಳ_ವಿಲೀನ_bases(the_repository, rev, ತಲೆ_rev);
		ಆದರೆ (expect < 0)
			ನಿರ್ಗಮಿಸು(128);
		ಆದರೆ (expect == merged)
			; /* okay */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	ಬಿಡುಗಡೆ(reference_name_ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ merged;
}

ಎಣಿಕೆ delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_ತಲೆ_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_ಸತತ = (1 << 4),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_branch_commit(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branchname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ commit *ತಲೆ_rev,
			       ಪೂರ್ಣಾಂಕ kinds, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ commit *rev = lookup_commit_reference(the_repository, oid);
	ಆದರೆ (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ಹಿಂತಿರುಗಿ -1;
	}
	ಆದರೆ (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, ತಲೆ_rev)) {
		ಆದರೆ (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ಹಿಂತಿರುಗಿ -1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ delete_branch_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branchname)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ಆದರೆ (repo_config_rename_section(the_repository, buf.buf, ಖಾಲಿ) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_branches(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ kinds,
			   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ commit *ತಲೆ_rev = ಖಾಲಿ;
	ರಚನೆ object_id oid;
	ಅಕ್ಷರ *name = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt;
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಪೂರ್ಣಾಂಕ remote_branch = 0;
	ರಚನೆ strbuf bname = STRBUF_INIT;
	ಎಣಿಕೆ interpret_branch_ಬಗೆ allowed_interpret;
	ರಚನೆ string_list refs_ಗಮ್ಯ_delete = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಪೂರ್ಣಾಂಕ branch_name_pos;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt_remotes = "refs/remotes/%s";

	ಆಯ್ಕೆ (kinds) {
	ಸ್ಥಿತಿ FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ಆದರೆ (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_ತಲೆ_FALLBACK))
		ತಲೆ_rev = lookup_commit_reference(the_repository, &ತಲೆ_oid);

	ಗಾಗಿ (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		ಅಕ್ಷರ *ಗುರಿ = ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		ಬಿಡುಗಡೆ(name);
		name = mkpathdup(fmt, bname.buf);

		ಆದರೆ (kinds == FILTER_REFS_BRANCHES) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
			ಆದರೆ ((ಮಾರ್ಗ = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, ಮಾರ್ಗ);
				ret = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ ((ಮಾರ್ಗ = branch_checked_ಹೊರಹೋಗು(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, ಮಾರ್ಗ);
				ret = 1;
				ಮುಂದುವರಿ;
			}
		}

		ಗುರಿ = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		ಆದರೆ (!ಗುರಿ) {
			ಆದರೆ (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಅಕ್ಷರ *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				ಅಕ್ಷರ *virtual_ಗುರಿ = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ಆದರೆ (virtual_ಗುರಿ)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				ಇಲ್ಲದಿದ್ದರೆ
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_ಗುರಿ);
			}
			ret = 1;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, ತಲೆ_rev, kinds,
					flags)) {
			ಆದರೆ (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			ಹೋಗು ಮುಂದಿನ;
		}

		ಅಂಶ = string_list_append(&refs_ಗಮ್ಯ_delete, name);
		ಅಂಶ->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? ಗುರಿ
				    : repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	ಮುಂದಿನ:
		ಬಿಡುಗಡೆ(ಗುರಿ);
	}

	ಆದರೆ (!(flags & DELETE_BRANCH_DRY_ಸತತ) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ಖಾಲಿ, &refs_ಗಮ್ಯ_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &refs_ಗಮ್ಯ_delete) {
		ಅಕ್ಷರ *describe_ref = ಅಂಶ->util;
		ಅಕ್ಷರ *name = ಅಂಶ->string;
		ಆದರೆ (flags & DELETE_BRANCH_DRY_ಸತತ) {
			ಆದರೆ (!(flags & DELETE_BRANCH_QUIET))
				ಬರೆ(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			ಅಕ್ಷರ *refname = name + branch_name_pos;
			ಆದರೆ (!(flags & DELETE_BRANCH_QUIET))
				ಬರೆ(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		ಬಿಡುಗಡೆ(describe_ref);
	}
	string_list_clear(&refs_ಗಮ್ಯ_delete, 0);

	ಬಿಡುಗಡೆ(name);
	strbuf_release(&bname);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ calc_maxwidth(ರಚನೆ ref_array *refs, ಪೂರ್ಣಾಂಕ remote_bonus)
{
	ಪೂರ್ಣಾಂಕ i, max = 0;
	ಗಾಗಿ (i = 0; i < refs->nr; i++) {
		ರಚನೆ ref_array_ಅಂಶ *it = refs->ವಸ್ತುಗಳು[i];
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *desc = it->refname;
		ಪೂರ್ಣಾಂಕ w;

		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(it->refname, "refs/heads/", &desc);
		skip_ಪೂರ್ವಪ್ರತ್ಯಯ(it->refname, "refs/remotes/", &desc);
		ಆದರೆ (it->ಬಗೆ == FILTER_REFS_DETACHED_ತಲೆ) {
			ಅಕ್ಷರ *ತಲೆ_desc = get_ತಲೆ_description();
			w = utf8_strwidth(ತಲೆ_desc);
			ಬಿಡುಗಡೆ(ತಲೆ_desc);
		} ಇಲ್ಲದಿದ್ದರೆ
			w = utf8_strwidth(desc);

		ಆದರೆ (it->ಬಗೆ == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ಆದರೆ (w > max)
			max = w;
	}
	ಹಿಂತಿರುಗಿ max;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *quote_literal_for_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಸ್ಥಿರ ರಚನೆ strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	ಆಗ (strbuf_expand_ಹೆಜ್ಜೆ(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ಹಿಂತಿರುಗಿ buf.buf;
}

ಸ್ಥಿರ ಅಕ್ಷರ *build_format(ರಚನೆ ref_filter *filter, ಪೂರ್ಣಾಂಕ maxwidth, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ರಚನೆ strbuf fmt = STRBUF_INIT;
	ರಚನೆ strbuf local = STRBUF_INIT;
	ರಚನೆ strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ಆದರೆ (filter->verbose) {
		ರಚನೆ strbuf obname = STRBUF_INIT;

		ಆದರೆ (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ಆದರೆ (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_ಪೂರ್ವಪ್ರತ್ಯಯ),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_ಪೂರ್ವಪ್ರತ್ಯಯ),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	ಹಿಂತಿರುಗಿ strbuf_detach(&fmt, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ref_list(ರಚನೆ ref_filter *filter, ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ,
			   ರಚನೆ ref_format *format, ರಚನೆ string_list *output)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ ref_array array;
	ಪೂರ್ಣಾಂಕ maxwidth = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_ಪೂರ್ವಪ್ರತ್ಯಯ = "";
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ಆದರೆ (filter->ಬಗೆ != FILTER_REFS_REMOTES)
		remote_ಪೂರ್ವಪ್ರತ್ಯಯ = "remotes/";

	memset(&array, 0, ಗಾತ್ರ(array));

	filter_refs(&array, filter, filter->ಬಗೆ);

	ಆದರೆ (filter->verbose)
		maxwidth = calc_maxwidth(&array, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(remote_ಪೂರ್ವಪ್ರತ್ಯಯ));

	ಆದರೆ (!format->format)
		format->format = ಗಮ್ಯ_free = build_format(filter, maxwidth, remote_ಪೂರ್ವಪ್ರತ್ಯಯ);
	format->use_color = branch_use_color;

	ಆದರೆ (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_ವಿಂಗಡಿಸು(ವಿಂಗಡಣೆ, &array);

	ಆದರೆ (ಸ್ತಂಭ_ಸಕ್ರಿಯ(colopts)) {
		ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT, err = STRBUF_INIT;

		ಪ್ರತಿಜ್ಞಾ(!filter->verbose && "--column and --verbose are incompatible");

		ಗಾಗಿ (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&ಹೊರಹೋಗು);
			ಆದರೆ (format_ref_array_ಅಂಶ(array.ವಸ್ತುಗಳು[i], format, &ಹೊರಹೋಗು, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, ಹೊರಹೋಗು.buf);
		}

		strbuf_release(&err);
		strbuf_release(&ಹೊರಹೋಗು);
	} ಇಲ್ಲದಿದ್ದರೆ {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_current_branch_name(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ flags;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ಖಾಲಿ, &flags);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shortname;
	ಆದರೆ (!refname)
		die(_("could not resolve HEAD"));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(flags & REF_ISSYMREF))
		ಹಿಂತಿರುಗಿ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(refname, "refs/heads/", &shortname))
		puts(shortname);
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

ಸ್ಥಿರ ಶೂನ್ಯ reject_rebase_or_bisect_branch(ರಚನೆ worktree **worktrees,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; worktrees[i]; i++) {
		ರಚನೆ worktree *wt = worktrees[i];

		ಆದರೆ (!wt->is_detached)
			ಮುಂದುವರಿ;

		ಆದರೆ (is_worktree_being_rebased(wt, ಗುರಿ))
			die(_("branch %s is being rebased at %s"),
			    ಗುರಿ, wt->ಮಾರ್ಗ);

		ಆದರೆ (is_worktree_being_bisected(wt, ಗುರಿ))
			die(_("branch %s is being bisected at %s"),
			    ಗುರಿ, wt->ಮಾರ್ಗ);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ replace_each_worktree_ತಲೆ_symref(ರಚನೆ worktree **worktrees,
					     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oldref, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newref,
					     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *logmsg)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; worktrees[i]; i++) {
		ರಚನೆ ref_store *refs;

		ಆದರೆ (worktrees[i]->is_detached)
			ಮುಂದುವರಿ;
		ಆದರೆ (!worktrees[i]->ತಲೆ_ref)
			ಮುಂದುವರಿ;
		ಆದರೆ (strcmp(oldref, worktrees[i]->ತಲೆ_ref))
			ಮುಂದುವರಿ;

		refs = get_worktree_ref_store(worktrees[i]);
		ಆದರೆ (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->ಮಾರ್ಗ);
	}

	ಹಿಂತಿರುಗಿ ret;
}

#ವ್ಯಾಖ್ಯಾನಿಸು IS_ತಲೆ 1
#ವ್ಯಾಖ್ಯಾನಿಸು IS_ORPHAN 2

ಸ್ಥಿರ ಶೂನ್ಯ copy_or_rename_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oldname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newname, ಪೂರ್ಣಾಂಕ copy, ಪೂರ್ಣಾಂಕ force)
{
	ರಚನೆ strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	ರಚನೆ strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *interpreted_oldname = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *interpreted_newname = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ recovery = 0, oldref_usage = 0;
	ರಚನೆ worktree **worktrees = get_worktrees(the_repository);

	ಆದರೆ (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ಆದರೆ (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		ಇಲ್ಲದಿದ್ದರೆ {
			ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			ನಿರ್ಗಮಿಸು(ಸಂಕೇತ_2);
		}
	}

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; worktrees[i]; i++) {
		ರಚನೆ worktree *wt = worktrees[i];

		ಆದರೆ (wt->ತಲೆ_ref && !strcmp(oldref.buf, wt->ತಲೆ_ref)) {
			oldref_usage |= IS_ತಲೆ;
			ಆದರೆ (is_null_oid(&wt->ತಲೆ_oid))
				oldref_usage |= IS_ORPHAN;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ ((copy || !(oldref_usage & IS_ತಲೆ)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ಆದರೆ (oldref_usage & IS_ತಲೆ)
			die(_("no commit on branch '%s' yet"), oldname);
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ಆದರೆ (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	ಇಲ್ಲದಿದ್ದರೆ
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ಆದರೆ (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ಆದರೆ (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ಆದರೆ (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ಆದರೆ (recovery) {
		ಆದರೆ (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ಆದರೆ (!copy && (oldref_usage & IS_ತಲೆ) &&
	    replace_each_worktree_ತಲೆ_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ಆದರೆ (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ಆದರೆ (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_forked(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ ref_filter *filter = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	ಆದರೆ (ref_filter_forked_ಕೂಡಿಸು(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ stacked_branch_ದತ್ತಾಂಶ {
	ರಚನೆ strset *deletable_branch_ಹೆಸರುಗಳು;
	ರಚನೆ strset *protected_branch_ಹೆಸರುಗಳು;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_stacked_branch_ತಳ(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref,
				       ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ stacked_branch_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name;
	ರಚನೆ branch *branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_refname;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_branch_name;

	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ಆದರೆ (strset_ಒಳಗೊಂಡಿದೆ(ದತ್ತಾಂಶ->deletable_branch_ಹೆಸರುಗಳು, branch_name))
		ಹಿಂತಿರುಗಿ 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ಖಾಲಿ);
	ಆದರೆ (!upstream_refname ||
	    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_ಒಳಗೊಂಡಿದೆ(ದತ್ತಾಂಶ->deletable_branch_ಹೆಸರುಗಳು,
			    upstream_branch_name))
		ಹಿಂತಿರುಗಿ 0;

	strset_ಕೂಡಿಸು(ದತ್ತಾಂಶ->protected_branch_ಹೆಸರುಗಳು, upstream_branch_name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ protect_stacked_branch_bases(ರಚನೆ ref_store *refs,
					 ರಚನೆ strset *deletable_branch_ಹೆಸರುಗಳು,
					 ರಚನೆ strset *protected_branch_ಹೆಸರುಗಳು)
{
	ರಚನೆ stacked_branch_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.deletable_branch_ಹೆಸರುಗಳು = deletable_branch_ಹೆಸರುಗಳು,
		.protected_branch_ಹೆಸರುಗಳು = protected_branch_ಹೆಸರುಗಳು,
	};
	ರಚನೆ refs_for_each_ref_options opts = {
		.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/heads/",
	};
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ strmap_ನಮೂದು *ನಮೂದು;

	refs_for_each_ref_ext(refs, collect_stacked_branch_ತಳ, &ದತ್ತಾಂಶ, &opts);

	strset_for_each_ನಮೂದು(protected_branch_ಹೆಸರುಗಳು, &iter, ನಮೂದು)
		strset_remove(deletable_branch_ಹೆಸರುಗಳು, ನಮೂದು->ಕೀಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_deleted_upstreams(ರಚನೆ strset *protected_branch_ಹೆಸರುಗಳು,
				    ರಚನೆ strset *deletable_branch_ಹೆಸರುಗಳು)
{
	ರಚನೆ strbuf ಕೀಲಿ = STRBUF_INIT;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ strmap_ನಮೂದು *ನಮೂದು;

	strset_for_each_ನಮೂದು(protected_branch_ಹೆಸರುಗಳು, &iter, ನಮೂದು) {
		ರಚನೆ branch *branch = branch_get(ನಮೂದು->ಕೀಲಿ);
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_refname = branch_get_upstream(branch, ಖಾಲಿ);
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_branch_name;

		ಆದರೆ (!upstream_refname ||
		    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_ಒಳಗೊಂಡಿದೆ(deletable_branch_ಹೆಸರುಗಳು,
				    upstream_branch_name))
			ಮುಂದುವರಿ;

		strbuf_addf(&ಕೀಲಿ, "branch.%s.merge", branch->name);
		repo_config_ಗಣ_gently(the_repository, ಕೀಲಿ.buf, ಖಾಲಿ);
		strbuf_reset(&ಕೀಲಿ);
		strbuf_addf(&ಕೀಲಿ, "branch.%s.remote", branch->name);
		repo_config_ಗಣ_gently(the_repository, ಕೀಲಿ.buf, ಖಾಲಿ);
		strbuf_reset(&ಕೀಲಿ);
	}

	strbuf_release(&ಕೀಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ branch_pushes_ಗಮ್ಯ_upstream(ರಚನೆ branch *branch,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream)
{
	ರಚನೆ remote *remote = remote_get(remote_for_branch(branch, ಖಾಲಿ));
	ಅಕ್ಷರ *ತಳ್ಳು_refname = ಖಾಲಿ;
	ಅಕ್ಷರ *tracking = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (!remote)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (remote->ತಳ್ಳು.nr)
		ತಳ್ಳು_refname = apply_refspecs(&remote->ತಳ್ಳು, branch->refname);
	ಇಲ್ಲದಿದ್ದರೆ
		ತಳ್ಳು_refname = xstrdup(branch->refname);
	ಆದರೆ (ತಳ್ಳು_refname)
		tracking = apply_refspecs(&remote->fetch, ತಳ್ಳು_refname);
	ಆದರೆ (tracking && !strcmp(tracking, upstream))
		ret = 1;

	ಬಿಡುಗಡೆ(ತಳ್ಳು_refname);
	ಬಿಡುಗಡೆ(tracking);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_merged_branches(ಸ್ಥಿರಾಂಕ ರಚನೆ strvec *upstreams,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ ref_store *refs = get_main_ref_store(the_repository);
	ರಚನೆ ref_filter filter = REF_FILTER_INIT;
	ರಚನೆ ref_array candidates = { 0 };
	ರಚನೆ strset deletable_branch_ಹೆಸರುಗಳು = STRSET_INIT;
	ರಚನೆ strset protected_branch_ಹೆಸರುಗಳು = STRSET_INIT;
	ರಚನೆ strvec branches_ಗಮ್ಯ_delete = STRVEC_INIT;
	ರಚನೆ strbuf ಕೀಲಿ = STRBUF_INIT;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ strmap_ನಮೂದು *ನಮೂದು;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < upstreams->nr; i++)
		ಆದರೆ (ref_filter_forked_ಕೂಡಿಸು(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.ಬಗೆ = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.ಬಗೆ);

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < candidates.nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_refname = candidates.ವಸ್ತುಗಳು[i]->refname;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name;
		ರಚನೆ branch *branch;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream_refname;
		ಪೂರ್ಣಾಂಕ opt_ಹೊರಹೋಗು;

		ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ಆದರೆ (branch_checked_ಹೊರಹೋಗು(branch_refname))
			ಮುಂದುವರಿ;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ಖಾಲಿ);
		ಆದರೆ (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			ಮುಂದುವರಿ;
		ಆದರೆ (branch_pushes_ಗಮ್ಯ_upstream(branch, upstream_refname))
			ಮುಂದುವರಿ;
		ಆದರೆ (check_branch_commit(branch_name, branch_name,
					&candidates.ವಸ್ತುಗಳು[i]->objectname, ಖಾಲಿ,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			ಮುಂದುವರಿ;

		strbuf_reset(&ಕೀಲಿ);
		strbuf_addf(&ಕೀಲಿ, "branch.%s.deletemerged", branch_name);
		ಆದರೆ (!repo_config_get_bool(the_repository, ಕೀಲಿ.buf, &opt_ಹೊರಹೋಗು) &&
		    !opt_ಹೊರಹೋಗು) {
			ಆದರೆ (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			ಮುಂದುವರಿ;
		}

		strset_ಕೂಡಿಸು(&deletable_branch_ಹೆಸರುಗಳು, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_ಹೆಸರುಗಳು,
				     &protected_branch_ಹೆಸರುಗಳು);

	strset_for_each_ನಮೂದು(&deletable_branch_ಹೆಸರುಗಳು, &iter, ನಮೂದು)
		strvec_ತಳ್ಳು(&branches_ಗಮ್ಯ_delete, ನಮೂದು->ಕೀಲಿ);

	ಆದರೆ (branches_ಗಮ್ಯ_delete.nr)
		ret = delete_branches(branches_ಗಮ್ಯ_delete.nr, branches_ಗಮ್ಯ_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_ತಲೆ_FALLBACK |
				      flags);

	ಆದರೆ (!ret && !(flags & DELETE_BRANCH_DRY_ಸತತ))
		clear_deleted_upstreams(&protected_branch_ಹೆಸರುಗಳು,
					&deletable_branch_ಹೆಸರುಗಳು);

	strbuf_release(&ಕೀಲಿ);
	strvec_clear(&branches_ಗಮ್ಯ_delete);
	strset_clear(&protected_branch_ಹೆಸರುಗಳು);
	strset_clear(&deletable_branch_ಹೆಸರುಗಳು);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(edit_description, "EDIT_DESCRIPTION")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ edit_branch_description(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name)
{
	ಪೂರ್ಣಾಂಕ exists;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ಆದರೆ (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_ಸಾಲು_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_ಸಾಲು_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ಆದರೆ (launch_editor(edit_description(), &buf, ಖಾಲಿ)) {
		strbuf_release(&buf);
		ಹಿಂತಿರುಗಿ -1;
	}
	strbuf_stripspace(&buf, comment_ಸಾಲು_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ಆದರೆ (buf.len || exists)
		repo_config_ಗಣ(the_repository, name.buf, buf.len ? buf.buf : ಖಾಲಿ);
	strbuf_release(&name);
	strbuf_release(&buf);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ die_if_upstream_looks_like_remote(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_upstream, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name)
{
	ರಚನೆ strbuf remote_ref = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

	ಆದರೆ (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ಹಿಂತಿರುಗಿ;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ಹಿಂತಿರುಗಿ;
	}

	ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_ಗಣ_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	ನಿರ್ಗಮಿಸು(ಸಂಕೇತ_2);
}

ಪೂರ್ಣಾಂಕ cmd_branch(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	/* possible actions */
	ಪೂರ್ಣಾಂಕ delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	ರಚನೆ strvec delete_merged = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_upstream = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ noncreate_actions = 0;
	/* possible options */
	ಪೂರ್ಣಾಂಕ reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	ಎಣಿಕೆ branch_track track;
	ರಚನೆ ref_filter filter = REF_FILTER_INIT;
	ಸ್ಥಿರ ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ;
	ರಚನೆ string_list ವಿಂಗಡಣೆ_options = STRING_LIST_INIT_DUP;
	ರಚನೆ ref_format format = REF_FORMAT_INIT;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಂಕ ret;

	ರಚನೆ option options[] = {
		OPT_ಗುಂಪು(N_("Generic options")),
		OPT__VERBOSE(&filter.verbose,
			N_("show hash and subject, give twice for upstream branch")),
		OPT__QUIET(&quiet, N_("suppress informational messages")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ಬಹುಲಕ),
		OPT_ಗಣ_INT_F(0, "set-upstream", &track, N_("do not use"),
			BRANCH_TRACK_OVERRIDE, PARSE_OPT_HIDDEN),
		OPT_STRING('u', "set-upstream-to", &new_upstream, N_("upstream"), N_("change the upstream info")),
		OPT_BOOL(0, "unset-upstream", &unset_upstream, N_("unset the upstream info")),
		OPT__COLOR(&branch_use_color, N_("use colored output")),
		OPT_ಗಣ_INT_F('r', "remotes",     &filter.ಬಗೆ, N_("act on remote-tracking branches"),
			      FILTER_REFS_REMOTES,
			      PARSE_OPT_NONEG),
		OPT_ಒಳಗೊಂಡಿದೆ(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_NO_ಒಳಗೊಂಡಿದೆ(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT__ABBREV(&filter.abbrev),

		OPT_ಗುಂಪು(N_("Specific git-branch actions:")),
		OPT_ಗಣ_INT_F('a', "all", &filter.ಬಗೆ, N_("list both remote-tracking and local branches"),
			      FILTER_REFS_REMOTES | FILTER_REFS_BRANCHES,
			      PARSE_OPT_NONEG),
		OPT_ಬಿಟ್('d', "delete", &delete, N_("delete fully merged branch"), 1),
		OPT_ಬಿಟ್('D', ಖಾಲಿ, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_ಬಿಟ್('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_ಬಿಟ್('M', ಖಾಲಿ, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ಖಾಲಿಗಳು,
			N_("do not output a newline after empty formatted refs")),
		OPT_ಬಿಟ್('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_ಬಿಟ್('C', ಖಾಲಿ, &copy, N_("copy a branch, even if target exists"), 2),
		OPT_BOOL('l', "list", &list, N_("list branch names")),
		OPT_BOOL(0, "show-current", &show_current, N_("show current branch name")),
		OPT_BOOL(0, "create-reflog", &reflog, N_("create the branch's reflog")),
		OPT_BOOL(0, "edit-description", &edit_description,
			 N_("edit the description for the branch")),
		OPT_CALLBACK_F(0, "delete-merged", &delete_merged, N_("pattern"),
			N_("delete merged branches whose upstream matches <pattern> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_strvec),
		OPT_BOOL(0, "dry-run", &dry_ಸತತ,
			N_("with --delete-merged, only print which branches would be deleted")),
		OPT__FORCE(&force, N_("force creation, move/rename, deletion"), PARSE_OPT_NOCOMPLETE),
		OPT_MERGED(&filter, N_("print only branches that are merged")),
		OPT_NO_MERGED(&filter, N_("print only branches that are not merged")),
		OPT_CALLBACK_F(0, "forked", &filter, N_("branch"),
			N_("print only branches whose upstream matches <branch> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_forked),
		OPT_ಸ್ತಂಭ(0, "column", &colopts, N_("list branches in columns")),
		OPT_REF_ವಿಂಗಡಿಸು(&ವಿಂಗಡಣೆ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at, N_("object"),
			N_("print only branches of the object"), parse_opt_object_name),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules_explicit, N_("recurse through submodules")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_ಕೊನೆ(),
	};

	ಸಿದ್ಧತೆ_ref_filter_porcelain_ಸಂದೇಶ();

	filter.ಬಗೆ = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &ವಿಂಗಡಣೆ_options);
	ಆದರೆ (!ವಿಂಗಡಣೆ_options.nr)
		string_list_append(&ವಿಂಗಡಣೆ_options, "refname");

	track = cfg->branch_track;

	ತಲೆ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &ತಲೆ_oid, ಖಾಲಿ);
	ಆದರೆ (!ತಲೆ)
		die(_("failed to resolve HEAD as a valid ref"));
	ಆದರೆ (!strcmp(ತಲೆ, "HEAD"))
		filter.detached = 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ತಲೆ, "refs/heads/", &ತಲೆ))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_branch_usage,
			     0);

	ಆದರೆ (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ಆದರೆ (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ಆದರೆ (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ಆದರೆ (dry_ಸತತ && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ಆದರೆ (recurse_submodules_explicit) {
		ಆದರೆ (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ಆದರೆ (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ಆದರೆ (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ಆದರೆ (filter.verbose) {
		ಆದರೆ (explicitly_enable_ಸ್ತಂಭ(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ಆದರೆ (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ಆದರೆ (list)
		ಸಿದ್ಧತೆ_auto_pager("branch", 1);

	ಆದರೆ (delete) {
		ಆದರೆ (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.ಬಗೆ,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_ಸತತ ? DELETE_BRANCH_DRY_ಸತತ : 0));
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (show_current) {
		print_current_branch_name();
		ret = 0;
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ಆದರೆ ((filter.ಬಗೆ & FILTER_REFS_BRANCHES) && filter.detached)
			filter.ಬಗೆ |= FILTER_REFS_DETACHED_ತಲೆ;
		filter.name_patterns = argv;
		/*
		 * If no sorting parameter is given then we default to sorting
		 * by 'refname'. This would give us an alphabetically sorted
		 * array with the 'HEAD' ref at the beginning followed by
		 * local branches 'refs/heads/...' and finally remote-tracking
		 * branches 'refs/remotes/...'.
		 */
		ವಿಂಗಡಣೆ = ref_ವಿಂಗಡಣೆ_options(&ವಿಂಗಡಣೆ_options);
		ref_ವಿಂಗಡಣೆ_ಗಣ_ವಿಂಗಡಿಸು_flags_all(ವಿಂಗಡಣೆ, REF_ವಿಂಗಡಣೆ_ICASE, icase);
		ref_ವಿಂಗಡಣೆ_ಗಣ_ವಿಂಗಡಿಸು_flags_all(
			ವಿಂಗಡಣೆ, REF_ವಿಂಗಡಣೆ_DETACHED_ತಲೆ_ಮೊದಲ, 1);
		print_ref_list(&filter, ವಿಂಗಡಣೆ, &format, &output);
		print_columns(&output, colopts, ಖಾಲಿ);
		string_list_clear(&output, 0);
		ref_ವಿಂಗಡಣೆ_release(ವಿಂಗಡಣೆ);
		ref_filter_clear(&filter);

		ret = 0;
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (edit_description) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name;
		ರಚನೆ strbuf branch_ref = STRBUF_INIT;
		ರಚನೆ strbuf buf = STRBUF_INIT;

		ಆದರೆ (!argc) {
			ಆದರೆ (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = ತಲೆ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} ಇಲ್ಲದಿದ್ದರೆ {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_ಹೊರಹೋಗು(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} ಇಲ್ಲದಿದ್ದರೆ {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (copy || rename) {
		ಆದರೆ (!argc)
			die(_("branch name required"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 1)
			copy_or_rename_branch(ತಲೆ, argv[0], copy, copy + rename > 1);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		ಇಲ್ಲದಿದ್ದರೆ
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (new_upstream) {
		ರಚನೆ branch *branch;
		ರಚನೆ strbuf buf = STRBUF_INIT;

		ಆದರೆ (!argc)
			branch = branch_get(ಖಾಲಿ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ಇಲ್ಲದಿದ್ದರೆ
			die(_("too many arguments to set new upstream"));

		ಆದರೆ (!branch) {
			ಆದರೆ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ಆದರೆ (!argc || branch_checked_ಹೊರಹೋಗು(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ಆದರೆ (argc == 1 &&
			    advice_enabled(ADVICE_ಗಣ_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_ಸಿದ್ಧತೆ_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unset_upstream) {
		ರಚನೆ branch *branch;
		ರಚನೆ strbuf buf = STRBUF_INIT;

		ಆದರೆ (!argc)
			branch = branch_get(ಖಾಲಿ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ಇಲ್ಲದಿದ್ದರೆ
			die(_("too many arguments to unset upstream"));

		ಆದರೆ (!branch) {
			ಆದರೆ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ಆದರೆ (!branch_has_ವಿಲೀನ_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_ಗಣ_multivar(the_repository, buf.buf, ಖಾಲಿ, ಖಾಲಿ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_ಗಣ_multivar(the_repository, buf.buf, ಖಾಲಿ, ಖಾಲಿ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!noncreate_actions && argc > 0 && argc <= 2) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name = argv[0];
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆರಂಭ_name = argc == 2 ? argv[1] : ತಲೆ;

		ಆದರೆ (filter.ಬಗೆ != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ಆದರೆ (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ಆದರೆ (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    ಆರಂಭ_name, ಖಾಲಿ, force,
						    reflog, quiet, track, 0);
			ret = 0;
			ಹೋಗು ಹೊರಹೋಗು;
		}
		create_branch(the_repository, branch_name, ಆರಂಭ_name, force, 0,
			      reflog, quiet, track, 0);
	} ಇಲ್ಲದಿದ್ದರೆ
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

ಹೊರಹೋಗು:
	strvec_clear(&delete_merged);
	string_list_clear(&ವಿಂಗಡಣೆ_options, 0);
	ಹಿಂತಿರುಗಿ ret;
}
