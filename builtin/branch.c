/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_branch_usage[] = {
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
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *head;
ಸ್ಥಿರ ಸಂರಚನಾ object_id head_oid;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ recurse_submodules = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ submodule_propagate_branches = 0;

ಸ್ಥಿರ ಗಣನಾ git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
ಸ್ಥಿರ ವರ್ಣ branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
ಗಣನಾ color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

ಸ್ಥಿರ ಸಂರಚನಾ string_list output = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ colopts;

define_list_config_array(color_branch_slots);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_branch_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			     ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಧ್ರುವ ವರ್ಣ *slot_name;

	ಯದಿ (!strcmp(var, "branch.sort")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		string_list_append(cb, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (starts_with(var, "column."))
		ಪ್ರತ್ಯಾಗಮ git_column_config(var, ಮೌಲ್ಯ, "branch", &colopts);
	ಯದಿ (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (skip_prefix(var, "color.branch.", &slot_name)) {
		ಪೂರ್ಣಾಙ್ಕ slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		ಯದಿ (slot < 0)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಪ್ರತ್ಯಾಗಮ color_parse(ಮೌಲ್ಯ, branch_colors[slot]);
	}
	ಯದಿ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *branch_get_color(ಗಣನಾ color_branch ix)
{
	ಯದಿ (want_color(branch_use_color))
		ಪ್ರತ್ಯಾಗಮ branch_colors[ix];
	ಪ್ರತ್ಯಾಗಮ "";
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ branch_merged(ಪೂರ್ಣಾಙ್ಕ kind, ಧ್ರುವ ವರ್ಣ *name,
			 ಸಂರಚನಾ commit *rev, ಸಂರಚನಾ commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	ಸಂರಚನಾ commit *reference_rev = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *reference_name = ರಿಕ್ತ;
	ಶೂನ್ಯ *reference_name_to_free = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ merged;

	ಯದಿ (kind == FILTER_REFS_BRANCHES) {
		ಸಂರಚನಾ branch *branch = branch_get(name);
		ಧ್ರುವ ವರ್ಣ *upstream = branch_get_upstream(branch, ರಿಕ್ತ);
		ಸಂರಚನಾ object_id oid;

		ಯದಿ (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ರಿಕ್ತ)) != ರಿಕ್ತ)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ಯದಿ (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	ಯದಿ (merged < 0)
		exit(128);

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
	ಯದಿ (head_rev && head_rev != reference_rev) {
		ಪೂರ್ಣಾಙ್ಕ expect = repo_in_merge_bases(the_repository, rev, head_rev);
		ಯದಿ (expect < 0)
			exit(128);
		ಯದಿ (expect == merged)
			; /* okay */
		ಅನ್ಯಥಾ ಯದಿ (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		ಅನ್ಯಥಾ
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	ಮುಕ್ತ(reference_name_to_free);
	ಪ್ರತ್ಯಾಗಮ merged;
}

ಗಣನಾ delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_branch_commit(ಧ್ರುವ ವರ್ಣ *branchname, ಧ್ರುವ ವರ್ಣ *refname,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ commit *head_rev,
			       ಪೂರ್ಣಾಙ್ಕ kinds, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ commit *rev = lookup_commit_reference(the_repository, oid);
	ಯದಿ (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಯದಿ (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		ಯದಿ (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ delete_branch_config(ಧ್ರುವ ವರ್ಣ *branchname)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ಯದಿ (repo_config_rename_section(the_repository, buf.buf, ರಿಕ್ತ) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_branches(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ kinds,
			   unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ commit *head_rev = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id oid;
	ವರ್ಣ *name = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *fmt;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಪೂರ್ಣಾಙ್ಕ remote_branch = 0;
	ಸಂರಚನಾ strbuf bname = STRBUF_INIT;
	ಗಣನಾ interpret_branch_kind allowed_interpret;
	ಸಂರಚನಾ string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list_item *item;
	ಪೂರ್ಣಾಙ್ಕ branch_name_pos;
	ಧ್ರುವ ವರ್ಣ *fmt_remotes = "refs/remotes/%s";

	ವಿಕಲ್ಪ (kinds) {
	ಪಕ್ಷ FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		ಭಙ್ಗ;
	ಪಕ್ಷ FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ಯದಿ (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	ಪ್ರತಿ (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		ವರ್ಣ *target = ರಿಕ್ತ;
		ಪೂರ್ಣಾಙ್ಕ ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		ಮುಕ್ತ(name);
		name = mkpathdup(fmt, bname.buf);

		ಯದಿ (kinds == FILTER_REFS_BRANCHES) {
			ಧ್ರುವ ವರ್ಣ *path;
			ಯದಿ ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				ಅನುವರ್ತ;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		ಯದಿ (!target) {
			ಯದಿ (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} ಅನ್ಯಥಾ {
				ವರ್ಣ *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				ವರ್ಣ *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ಯದಿ (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				ಅನ್ಯಥಾ
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			ಯದಿ (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			ಗಚ್ಛ next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		ಮುಕ್ತ(target);
	}

	ಯದಿ (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ರಿಕ್ತ, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ವರ್ಣ *describe_ref = item->util;
		ವರ್ಣ *name = item->string;
		ಯದಿ (flags & DELETE_BRANCH_DRY_RUN) {
			ಯದಿ (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} ಅನ್ಯಥಾ ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			ವರ್ಣ *refname = name + branch_name_pos;
			ಯದಿ (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		ಮುಕ್ತ(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	ಮುಕ್ತ(name);
	strbuf_release(&bname);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ calc_maxwidth(ಸಂರಚನಾ ref_array *refs, ಪೂರ್ಣಾಙ್ಕ remote_bonus)
{
	ಪೂರ್ಣಾಙ್ಕ i, max = 0;
	ಪ್ರತಿ (i = 0; i < refs->nr; i++) {
		ಸಂರಚನಾ ref_array_item *it = refs->ವಸ್ತುಗಳು[i];
		ಧ್ರುವ ವರ್ಣ *desc = it->refname;
		ಪೂರ್ಣಾಙ್ಕ w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		ಯದಿ (it->kind == FILTER_REFS_DETACHED_HEAD) {
			ವರ್ಣ *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			ಮುಕ್ತ(head_desc);
		} ಅನ್ಯಥಾ
			w = utf8_strwidth(desc);

		ಯದಿ (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ಯದಿ (w > max)
			max = w;
	}
	ಪ್ರತ್ಯಾಗಮ max;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *quote_literal_for_format(ಧ್ರುವ ವರ್ಣ *s)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	ಯಾವತ್ (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ಪ್ರತ್ಯಾಗಮ buf.buf;
}

ಸ್ಥಿರ ವರ್ಣ *build_format(ಸಂರಚನಾ ref_filter *filter, ಪೂರ್ಣಾಙ್ಕ maxwidth, ಧ್ರುವ ವರ್ಣ *remote_prefix)
{
	ಸಂರಚನಾ strbuf fmt = STRBUF_INIT;
	ಸಂರಚನಾ strbuf local = STRBUF_INIT;
	ಸಂರಚನಾ strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ಯದಿ (filter->verbose) {
		ಸಂರಚನಾ strbuf obname = STRBUF_INIT;

		ಯದಿ (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		ಅನ್ಯಥಾ ಯದಿ (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		ಅನ್ಯಥಾ
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ಯದಿ (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		ಅನ್ಯಥಾ
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} ಅನ್ಯಥಾ {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&fmt, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ref_list(ಸಂರಚನಾ ref_filter *filter, ಸಂರಚನಾ ref_sorting *sorting,
			   ಸಂರಚನಾ ref_format *format, ಸಂರಚನಾ string_list *output)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ ref_array array;
	ಪೂರ್ಣಾಙ್ಕ maxwidth = 0;
	ಧ್ರುವ ವರ್ಣ *remote_prefix = "";
	ವರ್ಣ *to_free = ರಿಕ್ತ;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ಯದಿ (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, ಪರಿಮಾಣ(array));

	filter_refs(&array, filter, filter->kind);

	ಯದಿ (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	ಯದಿ (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	ಯದಿ (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	ಯದಿ (column_active(colopts)) {
		ಸಂರಚನಾ strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		ಪ್ರತಿಜ್ಞಾ(!filter->verbose && "--column and --verbose are incompatible");

		ಪ್ರತಿ (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			ಯದಿ (format_ref_array_item(array.ವಸ್ತುಗಳು[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} ಅನ್ಯಥಾ {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	ಮುಕ್ತ(to_free);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_current_branch_name(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ flags;
	ಧ್ರುವ ವರ್ಣ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ರಿಕ್ತ, &flags);
	ಧ್ರುವ ವರ್ಣ *shortname;
	ಯದಿ (!refname)
		die(_("could not resolve HEAD"));
	ಅನ್ಯಥಾ ಯದಿ (!(flags & REF_ISSYMREF))
		ಪ್ರತ್ಯಾಗಮ;
	ಅನ್ಯಥಾ ಯದಿ (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	ಅನ್ಯಥಾ
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

ಸ್ಥಿರ ಶೂನ್ಯ reject_rebase_or_bisect_branch(ಸಂರಚನಾ worktree **worktrees,
					   ಧ್ರುವ ವರ್ಣ *target)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; worktrees[i]; i++) {
		ಸಂರಚನಾ worktree *wt = worktrees[i];

		ಯದಿ (!wt->is_detached)
			ಅನುವರ್ತ;

		ಯದಿ (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		ಯದಿ (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ replace_each_worktree_head_symref(ಸಂರಚನಾ worktree **worktrees,
					     ಧ್ರುವ ವರ್ಣ *oldref, ಧ್ರುವ ವರ್ಣ *newref,
					     ಧ್ರುವ ವರ್ಣ *logmsg)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; worktrees[i]; i++) {
		ಸಂರಚನಾ ref_store *refs;

		ಯದಿ (worktrees[i]->is_detached)
			ಅನುವರ್ತ;
		ಯದಿ (!worktrees[i]->head_ref)
			ಅನುವರ್ತ;
		ಯದಿ (strcmp(oldref, worktrees[i]->head_ref))
			ಅನುವರ್ತ;

		refs = get_worktree_ref_store(worktrees[i]);
		ಯದಿ (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

ಸ್ಥಿರ ಶೂನ್ಯ copy_or_rename_branch(ಧ್ರುವ ವರ್ಣ *oldname, ಧ್ರುವ ವರ್ಣ *newname, ಪೂರ್ಣಾಙ್ಕ copy, ಪೂರ್ಣಾಙ್ಕ force)
{
	ಸಂರಚನಾ strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	ಸಂರಚನಾ strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *interpreted_oldname = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *interpreted_newname = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ recovery = 0, oldref_usage = 0;
	ಸಂರಚನಾ worktree **worktrees = get_worktrees(the_repository);

	ಯದಿ (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ಯದಿ (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		ಅನ್ಯಥಾ {
			ಪೂರ್ಣಾಙ್ಕ code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; worktrees[i]; i++) {
		ಸಂರಚನಾ worktree *wt = worktrees[i];

		ಯದಿ (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			ಯದಿ (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			ಭಙ್ಗ;
		}
	}

	ಯದಿ ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ಯದಿ (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		ಅನ್ಯಥಾ
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ಯದಿ (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	ಅನ್ಯಥಾ
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ಯದಿ (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ಯದಿ (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	ಅನ್ಯಥಾ
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ಯದಿ (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ಯದಿ (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ಯದಿ (recovery) {
		ಯದಿ (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		ಅನ್ಯಥಾ
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ಯದಿ (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ಯದಿ (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ಯದಿ (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_forked(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ ref_filter *filter = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	ಯದಿ (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ stacked_branch_data {
	ಸಂರಚನಾ strset *deletable_branch_names;
	ಸಂರಚನಾ strset *protected_branch_names;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_stacked_branch_base(ಧ್ರುವ ಸಂರಚನಾ reference *ref,
				       ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ stacked_branch_data *data = cb_data;
	ಧ್ರುವ ವರ್ಣ *branch_name;
	ಸಂರಚನಾ branch *branch;
	ಧ್ರುವ ವರ್ಣ *upstream_refname;
	ಧ್ರುವ ವರ್ಣ *upstream_branch_name;

	ಯದಿ (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ಯದಿ (strset_contains(data->deletable_branch_names, branch_name))
		ಪ್ರತ್ಯಾಗಮ 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ರಿಕ್ತ);
	ಯದಿ (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		ಪ್ರತ್ಯಾಗಮ 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ protect_stacked_branch_bases(ಸಂರಚನಾ ref_store *refs,
					 ಸಂರಚನಾ strset *deletable_branch_names,
					 ಸಂರಚನಾ strset *protected_branch_names)
{
	ಸಂರಚನಾ stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	ಸಂರಚನಾ refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->ಕೀಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_deleted_upstreams(ಸಂರಚನಾ strset *protected_branch_names,
				    ಸಂರಚನಾ strset *deletable_branch_names)
{
	ಸಂರಚನಾ strbuf ಕೀಲಿ = STRBUF_INIT;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		ಸಂರಚನಾ branch *branch = branch_get(entry->ಕೀಲಿ);
		ಧ್ರುವ ವರ್ಣ *upstream_refname = branch_get_upstream(branch, ರಿಕ್ತ);
		ಧ್ರುವ ವರ್ಣ *upstream_branch_name;

		ಯದಿ (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			ಅನುವರ್ತ;

		strbuf_addf(&ಕೀಲಿ, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, ಕೀಲಿ.buf, ರಿಕ್ತ);
		strbuf_reset(&ಕೀಲಿ);
		strbuf_addf(&ಕೀಲಿ, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, ಕೀಲಿ.buf, ರಿಕ್ತ);
		strbuf_reset(&ಕೀಲಿ);
	}

	strbuf_release(&ಕೀಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ branch_pushes_to_upstream(ಸಂರಚನಾ branch *branch,
				     ಧ್ರುವ ವರ್ಣ *upstream)
{
	ಸಂರಚನಾ remote *remote = remote_get(remote_for_branch(branch, ರಿಕ್ತ));
	ವರ್ಣ *ತಳ್ಳು_refname = ರಿಕ್ತ;
	ವರ್ಣ *tracking = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (!remote)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (remote->ತಳ್ಳು.nr)
		ತಳ್ಳು_refname = apply_refspecs(&remote->ತಳ್ಳು, branch->refname);
	ಅನ್ಯಥಾ
		ತಳ್ಳು_refname = xstrdup(branch->refname);
	ಯದಿ (ತಳ್ಳು_refname)
		tracking = apply_refspecs(&remote->fetch, ತಳ್ಳು_refname);
	ಯದಿ (tracking && !strcmp(tracking, upstream))
		ret = 1;

	ಮುಕ್ತ(ತಳ್ಳು_refname);
	ಮುಕ್ತ(tracking);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_merged_branches(ಧ್ರುವ ಸಂರಚನಾ strvec *upstreams,
				 ಧ್ರುವ ವರ್ಣ **argv, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ ref_store *refs = get_main_ref_store(the_repository);
	ಸಂರಚನಾ ref_filter filter = REF_FILTER_INIT;
	ಸಂರಚನಾ ref_array candidates = { 0 };
	ಸಂರಚನಾ strset deletable_branch_names = STRSET_INIT;
	ಸಂರಚನಾ strset protected_branch_names = STRSET_INIT;
	ಸಂರಚನಾ strvec branches_to_delete = STRVEC_INIT;
	ಸಂರಚನಾ strbuf ಕೀಲಿ = STRBUF_INIT;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ strmap_entry *entry;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಪ್ರತಿ (size_t i = 0; i < upstreams->nr; i++)
		ಯದಿ (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < candidates.nr; i++) {
		ಧ್ರುವ ವರ್ಣ *branch_refname = candidates.ವಸ್ತುಗಳು[i]->refname;
		ಧ್ರುವ ವರ್ಣ *branch_name;
		ಸಂರಚನಾ branch *branch;
		ಧ್ರುವ ವರ್ಣ *upstream_refname;
		ಪೂರ್ಣಾಙ್ಕ opt_out;

		ಯದಿ (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ಯದಿ (branch_checked_out(branch_refname))
			ಅನುವರ್ತ;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ರಿಕ್ತ);
		ಯದಿ (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			ಅನುವರ್ತ;
		ಯದಿ (branch_pushes_to_upstream(branch, upstream_refname))
			ಅನುವರ್ತ;
		ಯದಿ (check_branch_commit(branch_name, branch_name,
					&candidates.ವಸ್ತುಗಳು[i]->objectname, ರಿಕ್ತ,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			ಅನುವರ್ತ;

		strbuf_reset(&ಕೀಲಿ);
		strbuf_addf(&ಕೀಲಿ, "branch.%s.deletemerged", branch_name);
		ಯದಿ (!repo_config_get_bool(the_repository, ಕೀಲಿ.buf, &opt_out) &&
		    !opt_out) {
			ಯದಿ (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			ಅನುವರ್ತ;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_ತಳ್ಳು(&branches_to_delete, entry->ಕೀಲಿ);

	ಯದಿ (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	ಯದಿ (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&ಕೀಲಿ);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ edit_branch_description(ಧ್ರುವ ವರ್ಣ *branch_name)
{
	ಪೂರ್ಣಾಙ್ಕ exists;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ಯದಿ (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_ಸಾಲು_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_ಸಾಲು_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ಯದಿ (launch_editor(edit_description(), &buf, ರಿಕ್ತ)) {
		strbuf_release(&buf);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	strbuf_stripspace(&buf, comment_ಸಾಲು_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ಯದಿ (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : ರಿಕ್ತ);
	strbuf_release(&name);
	strbuf_release(&buf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ die_if_upstream_looks_like_remote(ಧ್ರುವ ವರ್ಣ *new_upstream, ಧ್ರುವ ವರ್ಣ *branch_name)
{
	ಸಂರಚನಾ strbuf remote_ref = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ code;

	ಯದಿ (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ಪ್ರತ್ಯಾಗಮ;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ಪ್ರತ್ಯಾಗಮ;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

ಪೂರ್ಣಾಙ್ಕ cmd_branch(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	/* possible actions */
	ಪೂರ್ಣಾಙ್ಕ delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	ಸಂರಚನಾ strvec delete_merged = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0;
	ಧ್ರುವ ವರ್ಣ *new_upstream = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ noncreate_actions = 0;
	/* possible options */
	ಪೂರ್ಣಾಙ್ಕ reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	ಗಣನಾ branch_track track;
	ಸಂರಚನಾ ref_filter filter = REF_FILTER_INIT;
	ಸ್ಥಿರ ಸಂರಚನಾ ref_sorting *sorting;
	ಸಂರಚನಾ string_list sorting_options = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ ref_format format = REF_FORMAT_INIT;
	ಸಂರಚನಾ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಸಂರಚನಾ option options[] = {
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
		OPT_BIT('D', ರಿಕ್ತ, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', ರಿಕ್ತ, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', ರಿಕ್ತ, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_ಸಂದೇಶ();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	ಯದಿ (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, ರಿಕ್ತ);
	ಯದಿ (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	ಯದಿ (!strcmp(head, "HEAD"))
		filter.detached = 1;
	ಅನ್ಯಥಾ ಯದಿ (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	ಯದಿ (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ಯದಿ (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ಯದಿ (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ಯದಿ (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ಯದಿ (recurse_submodules_explicit) {
		ಯದಿ (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ಯದಿ (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ಯದಿ (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ಯದಿ (filter.verbose) {
		ಯದಿ (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ಯದಿ (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ಯದಿ (list)
		setup_auto_pager("branch", 1);

	ಯದಿ (delete) {
		ಯದಿ (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (show_current) {
		print_current_branch_name();
		ret = 0;
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ಯದಿ ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, ರಿಕ್ತ);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (edit_description) {
		ಧ್ರುವ ವರ್ಣ *branch_name;
		ಸಂರಚನಾ strbuf branch_ref = STRBUF_INIT;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

		ಯದಿ (!argc) {
			ಯದಿ (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} ಅನ್ಯಥಾ ಯದಿ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} ಅನ್ಯಥಾ {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} ಅನ್ಯಥಾ ಯದಿ (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} ಅನ್ಯಥಾ {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (copy || rename) {
		ಯದಿ (!argc)
			die(_("branch name required"));
		ಅನ್ಯಥಾ ಯದಿ ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		ಅನ್ಯಥಾ ಯದಿ (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		ಅನ್ಯಥಾ ಯದಿ (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		ಅನ್ಯಥಾ
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} ಅನ್ಯಥಾ ಯದಿ (new_upstream) {
		ಸಂರಚನಾ branch *branch;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

		ಯದಿ (!argc)
			branch = branch_get(ರಿಕ್ತ);
		ಅನ್ಯಥಾ ಯದಿ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ಅನ್ಯಥಾ
			die(_("too many arguments to set new upstream"));

		ಯದಿ (!branch) {
			ಯದಿ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ಯದಿ (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ಯದಿ (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} ಅನ್ಯಥಾ ಯದಿ (unset_upstream) {
		ಸಂರಚನಾ branch *branch;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

		ಯದಿ (!argc)
			branch = branch_get(ರಿಕ್ತ);
		ಅನ್ಯಥಾ ಯದಿ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ಅನ್ಯಥಾ
			die(_("too many arguments to unset upstream"));

		ಯದಿ (!branch) {
			ಯದಿ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ಯದಿ (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ರಿಕ್ತ, ರಿಕ್ತ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ರಿಕ್ತ, ರಿಕ್ತ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} ಅನ್ಯಥಾ ಯದಿ (!noncreate_actions && argc > 0 && argc <= 2) {
		ಧ್ರುವ ವರ್ಣ *branch_name = argv[0];
		ಧ್ರುವ ವರ್ಣ *start_name = argc == 2 ? argv[1] : head;

		ಯದಿ (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ಯದಿ (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ಯದಿ (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, ರಿಕ್ತ, force,
						    reflog, quiet, track, 0);
			ret = 0;
			ಗಚ್ಛ out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} ಅನ್ಯಥಾ
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	ಪ್ರತ್ಯಾಗಮ ret;
}
