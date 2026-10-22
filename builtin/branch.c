/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "column.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ref-filter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_branch_usage[] = {
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
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸਿਰ;
ਸਥਿਰ ਬਣਤਰ object_id ਸਿਰ_oid;
ਸਥਿਰ ਪੂਰਨ recurse_submodules = 0;
ਸਥਿਰ ਪੂਰਨ submodule_propagate_branches = 0;

ਸਥਿਰ ਗਿਣਤੀ git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
ਸਥਿਰ ਅੱਖਰ branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_ਸਾਧਾਰਨ,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_ਸਾਧਾਰਨ,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
ਗਿਣਤੀ color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

ਸਥਿਰ ਬਣਤਰ string_list output = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ colopts;

define_list_config_array(color_branch_slots);

ਸਥਿਰ ਪੂਰਨ git_branch_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			     ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਅਚਲ ਅੱਖਰ *ਥਾਂ_name;

	ਜੇ (!strcmp(var, "branch.sort")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		string_list_append(cb, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (starts_with(var, "column."))
		ਵਾਪਸ git_ਕਾਲਮ_config(var, ਮੁੱਲ, "branch", &colopts);
	ਜੇ (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (skip_ਅਗੇਤਰ(var, "color.branch.", &ਥਾਂ_name)) {
		ਪੂਰਨ ਥਾਂ = LOOKUP_CONFIG(color_branch_slots, ਥਾਂ_name);
		ਜੇ (ਥਾਂ < 0)
			ਵਾਪਸ 0;
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਵਾਪਸ color_parse(ਮੁੱਲ, branch_colors[ਥਾਂ]);
	}
	ਜੇ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (git_color_config(var, ਮੁੱਲ, cb) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *branch_get_color(ਗਿਣਤੀ color_branch ix)
{
	ਜੇ (want_color(branch_use_color))
		ਵਾਪਸ branch_colors[ix];
	ਵਾਪਸ "";
}

ਸਥਿਰ ਪੂਰਨ branch_merged(ਪੂਰਨ ਕਿਸਮ, ਅਚਲ ਅੱਖਰ *name,
			 ਬਣਤਰ commit *rev, ਬਣਤਰ commit *ਸਿਰ_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	ਬਣਤਰ commit *reference_rev = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *reference_name = ਨਿਰਾ;
	ਖਾਲੀ *reference_name_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਪੂਰਨ merged;

	ਜੇ (ਕਿਸਮ == FILTER_REFS_BRANCHES) {
		ਬਣਤਰ branch *branch = branch_get(name);
		ਅਚਲ ਅੱਖਰ *upstream = branch_get_upstream(branch, ਨਿਰਾ);
		ਬਣਤਰ object_id oid;

		ਜੇ (upstream &&
		    (reference_name = reference_name_ਮੰਜ਼ਿਲ_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ਨਿਰਾ)) != ਨਿਰਾ)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ਜੇ (!reference_rev)
		reference_rev = ਸਿਰ_rev;

	merged = reference_rev ? repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, rev,
						     reference_rev) : 0;
	ਜੇ (merged < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);

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
	ਜੇ (ਸਿਰ_rev && ਸਿਰ_rev != reference_rev) {
		ਪੂਰਨ expect = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, rev, ਸਿਰ_rev);
		ਜੇ (expect < 0)
			ਬਾਹਰ_ਨਿਕਲੋ(128);
		ਜੇ (expect == merged)
			; /* okay */
		ਨਹੀਂਤਾਂ ਜੇ (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		ਨਹੀਂਤਾਂ
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	ਮੁਕਤ(reference_name_ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ merged;
}

ਗਿਣਤੀ delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_ਸਿਰ_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_ਲੜੀਵਾਰ = (1 << 4),
};

ਸਥਿਰ ਪੂਰਨ check_branch_commit(ਅਚਲ ਅੱਖਰ *branchname, ਅਚਲ ਅੱਖਰ *refname,
			       ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ commit *ਸਿਰ_rev,
			       ਪੂਰਨ kinds, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ commit *rev = lookup_commit_reference(the_repository, oid);
	ਜੇ (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ਵਾਪਸ -1;
	}
	ਜੇ (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, ਸਿਰ_rev)) {
		ਜੇ (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ਵਾਪਸ -1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ delete_branch_config(ਅਚਲ ਅੱਖਰ *branchname)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ਜੇ (repo_config_rename_section(the_repository, buf.buf, ਨਿਰਾ) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

ਸਥਿਰ ਪੂਰਨ delete_branches(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਪੂਰਨ kinds,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ commit *ਸਿਰ_rev = ਨਿਰਾ;
	ਬਣਤਰ object_id oid;
	ਅੱਖਰ *name = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *fmt;
	ਪੂਰਨ i;
	ਪੂਰਨ ret = 0;
	ਪੂਰਨ remote_branch = 0;
	ਬਣਤਰ strbuf bname = STRBUF_INIT;
	ਗਿਣਤੀ interpret_branch_ਕਿਸਮ allowed_interpret;
	ਬਣਤਰ string_list refs_ਮੰਜ਼ਿਲ_delete = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਪੂਰਨ branch_name_pos;
	ਅਚਲ ਅੱਖਰ *fmt_remotes = "refs/remotes/%s";

	ਚੋਣ (kinds) {
	ਹਾਲਤ FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		ਰੋਕੋ;
	ਹਾਲਤ FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		ਰੋਕੋ;
	ਮੂਲ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ਜੇ (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_ਸਿਰ_FALLBACK))
		ਸਿਰ_rev = lookup_commit_reference(the_repository, &ਸਿਰ_oid);

	ਲਈ (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		ਅੱਖਰ *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
		ਪੂਰਨ ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		ਮੁਕਤ(name);
		name = mkpathdup(fmt, bname.buf);

		ਜੇ (kinds == FILTER_REFS_BRANCHES) {
			ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
			ਜੇ ((ਰਸਤਾ = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, ਰਸਤਾ);
				ret = 1;
				ਜਾਰੀ;
			}
			ਜੇ ((ਰਸਤਾ = branch_checked_ਬਾਹਰਲਾ(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, ਰਸਤਾ);
				ret = 1;
				ਜਾਰੀ;
			}
		}

		ਨਿਸ਼ਾਨਾ = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		ਜੇ (!ਨਿਸ਼ਾਨਾ) {
			ਜੇ (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} ਨਹੀਂਤਾਂ {
				ਅੱਖਰ *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				ਅੱਖਰ *virtual_ਨਿਸ਼ਾਨਾ = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ਜੇ (virtual_ਨਿਸ਼ਾਨਾ)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				ਨਹੀਂਤਾਂ
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_ਨਿਸ਼ਾਨਾ);
			}
			ret = 1;
			ਜਾਰੀ;
		}

		ਜੇ (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, ਸਿਰ_rev, kinds,
					flags)) {
			ਜੇ (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			ਜਾਓ ਅਗਲਾ;
		}

		ਵਸਤੂ = string_list_append(&refs_ਮੰਜ਼ਿਲ_delete, name);
		ਵਸਤੂ->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? ਨਿਸ਼ਾਨਾ
				    : repo_ਲੱਭੋ_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	ਅਗਲਾ:
		ਮੁਕਤ(ਨਿਸ਼ਾਨਾ);
	}

	ਜੇ (!(flags & DELETE_BRANCH_DRY_ਲੜੀਵਾਰ) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ਨਿਰਾ, &refs_ਮੰਜ਼ਿਲ_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &refs_ਮੰਜ਼ਿਲ_delete) {
		ਅੱਖਰ *describe_ref = ਵਸਤੂ->util;
		ਅੱਖਰ *name = ਵਸਤੂ->string;
		ਜੇ (flags & DELETE_BRANCH_DRY_ਲੜੀਵਾਰ) {
			ਜੇ (!(flags & DELETE_BRANCH_QUIET))
				ਲਿਖੋ(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} ਨਹੀਂਤਾਂ ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			ਅੱਖਰ *refname = name + branch_name_pos;
			ਜੇ (!(flags & DELETE_BRANCH_QUIET))
				ਲਿਖੋ(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		ਮੁਕਤ(describe_ref);
	}
	string_list_clear(&refs_ਮੰਜ਼ਿਲ_delete, 0);

	ਮੁਕਤ(name);
	strbuf_release(&bname);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ calc_maxwidth(ਬਣਤਰ ref_array *refs, ਪੂਰਨ remote_bonus)
{
	ਪੂਰਨ i, max = 0;
	ਲਈ (i = 0; i < refs->nr; i++) {
		ਬਣਤਰ ref_array_ਵਸਤੂ *it = refs->ਚੀਜ਼ਾਂ[i];
		ਅਚਲ ਅੱਖਰ *desc = it->refname;
		ਪੂਰਨ w;

		skip_ਅਗੇਤਰ(it->refname, "refs/heads/", &desc);
		skip_ਅਗੇਤਰ(it->refname, "refs/remotes/", &desc);
		ਜੇ (it->ਕਿਸਮ == FILTER_REFS_DETACHED_ਸਿਰ) {
			ਅੱਖਰ *ਸਿਰ_desc = get_ਸਿਰ_description();
			w = utf8_strwidth(ਸਿਰ_desc);
			ਮੁਕਤ(ਸਿਰ_desc);
		} ਨਹੀਂਤਾਂ
			w = utf8_strwidth(desc);

		ਜੇ (it->ਕਿਸਮ == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ਜੇ (w > max)
			max = w;
	}
	ਵਾਪਸ max;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *quote_literal_for_format(ਅਚਲ ਅੱਖਰ *s)
{
	ਸਥਿਰ ਬਣਤਰ strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	ਜਦੋਂ (strbuf_expand_ਕਦਮ(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ਵਾਪਸ buf.buf;
}

ਸਥਿਰ ਅੱਖਰ *build_format(ਬਣਤਰ ref_filter *filter, ਪੂਰਨ maxwidth, ਅਚਲ ਅੱਖਰ *remote_ਅਗੇਤਰ)
{
	ਬਣਤਰ strbuf fmt = STRBUF_INIT;
	ਬਣਤਰ strbuf local = STRBUF_INIT;
	ਬਣਤਰ strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ਜੇ (filter->verbose) {
		ਬਣਤਰ strbuf obname = STRBUF_INIT;

		ਜੇ (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		ਨਹੀਂਤਾਂ ਜੇ (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		ਨਹੀਂਤਾਂ
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ਜੇ (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		ਨਹੀਂਤਾਂ
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_ਅਗੇਤਰ),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} ਨਹੀਂਤਾਂ {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_ਅਗੇਤਰ),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	ਵਾਪਸ strbuf_detach(&fmt, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ print_ref_list(ਬਣਤਰ ref_filter *filter, ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ,
			   ਬਣਤਰ ref_format *format, ਬਣਤਰ string_list *output)
{
	ਪੂਰਨ i;
	ਬਣਤਰ ref_array array;
	ਪੂਰਨ maxwidth = 0;
	ਅਚਲ ਅੱਖਰ *remote_ਅਗੇਤਰ = "";
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ਜੇ (filter->ਕਿਸਮ != FILTER_REFS_REMOTES)
		remote_ਅਗੇਤਰ = "remotes/";

	memset(&array, 0, ਆਕਾਰ(array));

	filter_refs(&array, filter, filter->ਕਿਸਮ);

	ਜੇ (filter->verbose)
		maxwidth = calc_maxwidth(&array, ਲੜੀ_ਲੰਬਾਈ(remote_ਅਗੇਤਰ));

	ਜੇ (!format->format)
		format->format = ਮੰਜ਼ਿਲ_free = build_format(filter, maxwidth, remote_ਅਗੇਤਰ);
	format->use_color = branch_use_color;

	ਜੇ (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_ਛਾਂਟੋ(ਛਾਂਟੀ, &array);

	ਜੇ (ਕਾਲਮ_ਸਰਗਰਮ(colopts)) {
		ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT, err = STRBUF_INIT;

		ਪ੍ਰਤਿਜ੍ਞਾ(!filter->verbose && "--column and --verbose are incompatible");

		ਲਈ (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&ਬਾਹਰਲਾ);
			ਜੇ (format_ref_array_ਵਸਤੂ(array.ਚੀਜ਼ਾਂ[i], format, &ਬਾਹਰਲਾ, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, ਬਾਹਰਲਾ.buf);
		}

		strbuf_release(&err);
		strbuf_release(&ਬਾਹਰਲਾ);
	} ਨਹੀਂਤਾਂ {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
}

ਸਥਿਰ ਖਾਲੀ print_current_branch_name(ਖਾਲੀ)
{
	ਪੂਰਨ flags;
	ਅਚਲ ਅੱਖਰ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ਨਿਰਾ, &flags);
	ਅਚਲ ਅੱਖਰ *shortname;
	ਜੇ (!refname)
		die(_("could not resolve HEAD"));
	ਨਹੀਂਤਾਂ ਜੇ (!(flags & REF_ISSYMREF))
		ਵਾਪਸ;
	ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(refname, "refs/heads/", &shortname))
		puts(shortname);
	ਨਹੀਂਤਾਂ
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

ਸਥਿਰ ਖਾਲੀ reject_rebase_or_bisect_branch(ਬਣਤਰ worktree **worktrees,
					   ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; worktrees[i]; i++) {
		ਬਣਤਰ worktree *wt = worktrees[i];

		ਜੇ (!wt->is_detached)
			ਜਾਰੀ;

		ਜੇ (is_worktree_being_rebased(wt, ਨਿਸ਼ਾਨਾ))
			die(_("branch %s is being rebased at %s"),
			    ਨਿਸ਼ਾਨਾ, wt->ਰਸਤਾ);

		ਜੇ (is_worktree_being_bisected(wt, ਨਿਸ਼ਾਨਾ))
			die(_("branch %s is being bisected at %s"),
			    ਨਿਸ਼ਾਨਾ, wt->ਰਸਤਾ);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
ਸਥਿਰ ਪੂਰਨ replace_each_worktree_ਸਿਰ_symref(ਬਣਤਰ worktree **worktrees,
					     ਅਚਲ ਅੱਖਰ *oldref, ਅਚਲ ਅੱਖਰ *newref,
					     ਅਚਲ ਅੱਖਰ *logmsg)
{
	ਪੂਰਨ ret = 0;
	ਪੂਰਨ i;

	ਲਈ (i = 0; worktrees[i]; i++) {
		ਬਣਤਰ ref_store *refs;

		ਜੇ (worktrees[i]->is_detached)
			ਜਾਰੀ;
		ਜੇ (!worktrees[i]->ਸਿਰ_ref)
			ਜਾਰੀ;
		ਜੇ (strcmp(oldref, worktrees[i]->ਸਿਰ_ref))
			ਜਾਰੀ;

		refs = get_worktree_ref_store(worktrees[i]);
		ਜੇ (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->ਰਸਤਾ);
	}

	ਵਾਪਸ ret;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_ਸਿਰ 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_ORPHAN 2

ਸਥਿਰ ਖਾਲੀ copy_or_rename_branch(ਅਚਲ ਅੱਖਰ *oldname, ਅਚਲ ਅੱਖਰ *newname, ਪੂਰਨ copy, ਪੂਰਨ force)
{
	ਬਣਤਰ strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	ਬਣਤਰ strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *interpreted_oldname = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *interpreted_newname = ਨਿਰਾ;
	ਪੂਰਨ recovery = 0, oldref_usage = 0;
	ਬਣਤਰ worktree **worktrees = get_worktrees(the_repository);

	ਜੇ (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ਜੇ (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		ਨਹੀਂਤਾਂ {
			ਪੂਰਨ ਕੋਡ = die_ਸੁਨੇਹਾ(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			ਬਾਹਰ_ਨਿਕਲੋ(ਕੋਡ);
		}
	}

	ਲਈ (ਪੂਰਨ i = 0; worktrees[i]; i++) {
		ਬਣਤਰ worktree *wt = worktrees[i];

		ਜੇ (wt->ਸਿਰ_ref && !strcmp(oldref.buf, wt->ਸਿਰ_ref)) {
			oldref_usage |= IS_ਸਿਰ;
			ਜੇ (is_null_oid(&wt->ਸਿਰ_oid))
				oldref_usage |= IS_ORPHAN;
			ਰੋਕੋ;
		}
	}

	ਜੇ ((copy || !(oldref_usage & IS_ਸਿਰ)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ਜੇ (oldref_usage & IS_ਸਿਰ)
			die(_("no commit on branch '%s' yet"), oldname);
		ਨਹੀਂਤਾਂ
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ਜੇ (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	ਨਹੀਂਤਾਂ
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ਜੇ (!skip_ਅਗੇਤਰ(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_ਅਗੇਤਰ(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ਜੇ (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	ਨਹੀਂਤਾਂ
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ਜੇ (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ਜੇ (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ਜੇ (recovery) {
		ਜੇ (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		ਨਹੀਂਤਾਂ
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ਜੇ (!copy && (oldref_usage & IS_ਸਿਰ) &&
	    replace_each_worktree_ਸਿਰ_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ਜੇ (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ਜੇ (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

ਸਥਿਰ ਪੂਰਨ parse_opt_forked(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ ref_filter *filter = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	ਜੇ (ref_filter_forked_ਜੋੜੋ(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ਵਾਪਸ 0;
}

ਬਣਤਰ stacked_branch_ਅੰਕੜੇ {
	ਬਣਤਰ strset *deletable_branch_ਨਾਂ;
	ਬਣਤਰ strset *protected_branch_ਨਾਂ;
};

ਸਥਿਰ ਪੂਰਨ collect_stacked_branch_ਅਧਾਰ(ਅਚਲ ਬਣਤਰ reference *ref,
				       ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ stacked_branch_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਅਚਲ ਅੱਖਰ *branch_name;
	ਬਣਤਰ branch *branch;
	ਅਚਲ ਅੱਖਰ *upstream_refname;
	ਅਚਲ ਅੱਖਰ *upstream_branch_name;

	ਜੇ (!skip_ਅਗੇਤਰ(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ਜੇ (strset_ਸ਼ਾਮਲ_ਹੈ(ਅੰਕੜੇ->deletable_branch_ਨਾਂ, branch_name))
		ਵਾਪਸ 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ਨਿਰਾ);
	ਜੇ (!upstream_refname ||
	    !skip_ਅਗੇਤਰ(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_ਸ਼ਾਮਲ_ਹੈ(ਅੰਕੜੇ->deletable_branch_ਨਾਂ,
			    upstream_branch_name))
		ਵਾਪਸ 0;

	strset_ਜੋੜੋ(ਅੰਕੜੇ->protected_branch_ਨਾਂ, upstream_branch_name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ protect_stacked_branch_bases(ਬਣਤਰ ref_store *refs,
					 ਬਣਤਰ strset *deletable_branch_ਨਾਂ,
					 ਬਣਤਰ strset *protected_branch_ਨਾਂ)
{
	ਬਣਤਰ stacked_branch_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.deletable_branch_ਨਾਂ = deletable_branch_ਨਾਂ,
		.protected_branch_ਨਾਂ = protected_branch_ਨਾਂ,
	};
	ਬਣਤਰ refs_for_each_ref_options opts = {
		.ਅਗੇਤਰ = "refs/heads/",
	};
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ strmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	refs_for_each_ref_ext(refs, collect_stacked_branch_ਅਧਾਰ, &ਅੰਕੜੇ, &opts);

	strset_for_each_ਇੰਦਰਾਜ(protected_branch_ਨਾਂ, &iter, ਇੰਦਰਾਜ)
		strset_remove(deletable_branch_ਨਾਂ, ਇੰਦਰਾਜ->ਕੁੰਜੀ);
}

ਸਥਿਰ ਖਾਲੀ clear_deleted_upstreams(ਬਣਤਰ strset *protected_branch_ਨਾਂ,
				    ਬਣਤਰ strset *deletable_branch_ਨਾਂ)
{
	ਬਣਤਰ strbuf ਕੁੰਜੀ = STRBUF_INIT;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ strmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

	strset_for_each_ਇੰਦਰਾਜ(protected_branch_ਨਾਂ, &iter, ਇੰਦਰਾਜ) {
		ਬਣਤਰ branch *branch = branch_get(ਇੰਦਰਾਜ->ਕੁੰਜੀ);
		ਅਚਲ ਅੱਖਰ *upstream_refname = branch_get_upstream(branch, ਨਿਰਾ);
		ਅਚਲ ਅੱਖਰ *upstream_branch_name;

		ਜੇ (!upstream_refname ||
		    !skip_ਅਗੇਤਰ(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_ਸ਼ਾਮਲ_ਹੈ(deletable_branch_ਨਾਂ,
				    upstream_branch_name))
			ਜਾਰੀ;

		strbuf_addf(&ਕੁੰਜੀ, "branch.%s.merge", branch->name);
		repo_config_ਸੈੱਟ_gently(the_repository, ਕੁੰਜੀ.buf, ਨਿਰਾ);
		strbuf_reset(&ਕੁੰਜੀ);
		strbuf_addf(&ਕੁੰਜੀ, "branch.%s.remote", branch->name);
		repo_config_ਸੈੱਟ_gently(the_repository, ਕੁੰਜੀ.buf, ਨਿਰਾ);
		strbuf_reset(&ਕੁੰਜੀ);
	}

	strbuf_release(&ਕੁੰਜੀ);
}

ਸਥਿਰ ਪੂਰਨ branch_pushes_ਮੰਜ਼ਿਲ_upstream(ਬਣਤਰ branch *branch,
				     ਅਚਲ ਅੱਖਰ *upstream)
{
	ਬਣਤਰ remote *remote = remote_get(remote_for_branch(branch, ਨਿਰਾ));
	ਅੱਖਰ *ਧੱਕੋ_refname = ਨਿਰਾ;
	ਅੱਖਰ *tracking = ਨਿਰਾ;
	ਪੂਰਨ ret = 0;

	ਜੇ (!remote)
		ਵਾਪਸ 0;
	ਜੇ (remote->ਧੱਕੋ.nr)
		ਧੱਕੋ_refname = apply_refspecs(&remote->ਧੱਕੋ, branch->refname);
	ਨਹੀਂਤਾਂ
		ਧੱਕੋ_refname = xstrdup(branch->refname);
	ਜੇ (ਧੱਕੋ_refname)
		tracking = apply_refspecs(&remote->fetch, ਧੱਕੋ_refname);
	ਜੇ (tracking && !strcmp(tracking, upstream))
		ret = 1;

	ਮੁਕਤ(ਧੱਕੋ_refname);
	ਮੁਕਤ(tracking);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ delete_merged_branches(ਅਚਲ ਬਣਤਰ strvec *upstreams,
				 ਅਚਲ ਅੱਖਰ **argv, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ ref_store *refs = get_main_ref_store(the_repository);
	ਬਣਤਰ ref_filter filter = REF_FILTER_INIT;
	ਬਣਤਰ ref_array candidates = { 0 };
	ਬਣਤਰ strset deletable_branch_ਨਾਂ = STRSET_INIT;
	ਬਣਤਰ strset protected_branch_ਨਾਂ = STRSET_INIT;
	ਬਣਤਰ strvec branches_ਮੰਜ਼ਿਲ_delete = STRVEC_INIT;
	ਬਣਤਰ strbuf ਕੁੰਜੀ = STRBUF_INIT;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ strmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਪੂਰਨ ret = 0;

	ਲਈ (ਮਾਪ_t i = 0; i < upstreams->nr; i++)
		ਜੇ (ref_filter_forked_ਜੋੜੋ(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.ਕਿਸਮ = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.ਕਿਸਮ);

	ਲਈ (ਪੂਰਨ i = 0; i < candidates.nr; i++) {
		ਅਚਲ ਅੱਖਰ *branch_refname = candidates.ਚੀਜ਼ਾਂ[i]->refname;
		ਅਚਲ ਅੱਖਰ *branch_name;
		ਬਣਤਰ branch *branch;
		ਅਚਲ ਅੱਖਰ *upstream_refname;
		ਪੂਰਨ opt_ਬਾਹਰਲਾ;

		ਜੇ (!skip_ਅਗੇਤਰ(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ਜੇ (branch_checked_ਬਾਹਰਲਾ(branch_refname))
			ਜਾਰੀ;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ਨਿਰਾ);
		ਜੇ (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			ਜਾਰੀ;
		ਜੇ (branch_pushes_ਮੰਜ਼ਿਲ_upstream(branch, upstream_refname))
			ਜਾਰੀ;
		ਜੇ (check_branch_commit(branch_name, branch_name,
					&candidates.ਚੀਜ਼ਾਂ[i]->objectname, ਨਿਰਾ,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			ਜਾਰੀ;

		strbuf_reset(&ਕੁੰਜੀ);
		strbuf_addf(&ਕੁੰਜੀ, "branch.%s.deletemerged", branch_name);
		ਜੇ (!repo_config_get_bool(the_repository, ਕੁੰਜੀ.buf, &opt_ਬਾਹਰਲਾ) &&
		    !opt_ਬਾਹਰਲਾ) {
			ਜੇ (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			ਜਾਰੀ;
		}

		strset_ਜੋੜੋ(&deletable_branch_ਨਾਂ, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_ਨਾਂ,
				     &protected_branch_ਨਾਂ);

	strset_for_each_ਇੰਦਰਾਜ(&deletable_branch_ਨਾਂ, &iter, ਇੰਦਰਾਜ)
		strvec_ਧੱਕੋ(&branches_ਮੰਜ਼ਿਲ_delete, ਇੰਦਰਾਜ->ਕੁੰਜੀ);

	ਜੇ (branches_ਮੰਜ਼ਿਲ_delete.nr)
		ret = delete_branches(branches_ਮੰਜ਼ਿਲ_delete.nr, branches_ਮੰਜ਼ਿਲ_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_ਸਿਰ_FALLBACK |
				      flags);

	ਜੇ (!ret && !(flags & DELETE_BRANCH_DRY_ਲੜੀਵਾਰ))
		clear_deleted_upstreams(&protected_branch_ਨਾਂ,
					&deletable_branch_ਨਾਂ);

	strbuf_release(&ਕੁੰਜੀ);
	strvec_clear(&branches_ਮੰਜ਼ਿਲ_delete);
	strset_clear(&protected_branch_ਨਾਂ);
	strset_clear(&deletable_branch_ਨਾਂ);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	ਵਾਪਸ ret;
}

ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(edit_description, "EDIT_DESCRIPTION")

ਸਥਿਰ ਪੂਰਨ edit_branch_description(ਅਚਲ ਅੱਖਰ *branch_name)
{
	ਪੂਰਨ exists;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ਜੇ (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_ਸਤਰ_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_ਸਤਰ_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ਜੇ (launch_editor(edit_description(), &buf, ਨਿਰਾ)) {
		strbuf_release(&buf);
		ਵਾਪਸ -1;
	}
	strbuf_stripspace(&buf, comment_ਸਤਰ_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ਜੇ (buf.len || exists)
		repo_config_ਸੈੱਟ(the_repository, name.buf, buf.len ? buf.buf : ਨਿਰਾ);
	strbuf_release(&name);
	strbuf_release(&buf);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ die_if_upstream_looks_like_remote(ਅਚਲ ਅੱਖਰ *new_upstream, ਅਚਲ ਅੱਖਰ *branch_name)
{
	ਬਣਤਰ strbuf remote_ref = STRBUF_INIT;
	ਪੂਰਨ ਕੋਡ;

	ਜੇ (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ਵਾਪਸ;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ਵਾਪਸ;
	}

	ਕੋਡ = die_ਸੁਨੇਹਾ(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_ਸੈੱਟ_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	ਬਾਹਰ_ਨਿਕਲੋ(ਕੋਡ);
}

ਪੂਰਨ cmd_branch(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	/* possible actions */
	ਪੂਰਨ delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	ਬਣਤਰ strvec delete_merged = STRVEC_INIT;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0;
	ਅਚਲ ਅੱਖਰ *new_upstream = ਨਿਰਾ;
	ਪੂਰਨ noncreate_actions = 0;
	/* possible options */
	ਪੂਰਨ reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	ਗਿਣਤੀ branch_track track;
	ਬਣਤਰ ref_filter filter = REF_FILTER_INIT;
	ਸਥਿਰ ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ;
	ਬਣਤਰ string_list ਛਾਂਟੀ_options = STRING_LIST_INIT_DUP;
	ਬਣਤਰ ref_format format = REF_FORMAT_INIT;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਪੂਰਨ ret;

	ਬਣਤਰ option options[] = {
		OPT_ਸਮੂਹ(N_("Generic options")),
		OPT__VERBOSE(&filter.verbose,
			N_("show hash and subject, give twice for upstream branch")),
		OPT__QUIET(&quiet, N_("suppress informational messages")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ਬਹੁਲਕ),
		OPT_ਸੈੱਟ_INT_F(0, "set-upstream", &track, N_("do not use"),
			BRANCH_TRACK_OVERRIDE, PARSE_OPT_HIDDEN),
		OPT_STRING('u', "set-upstream-to", &new_upstream, N_("upstream"), N_("change the upstream info")),
		OPT_BOOL(0, "unset-upstream", &unset_upstream, N_("unset the upstream info")),
		OPT__COLOR(&branch_use_color, N_("use colored output")),
		OPT_ਸੈੱਟ_INT_F('r', "remotes",     &filter.ਕਿਸਮ, N_("act on remote-tracking branches"),
			      FILTER_REFS_REMOTES,
			      PARSE_OPT_NONEG),
		OPT_ਸ਼ਾਮਲ_ਹੈ(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_NO_ਸ਼ਾਮਲ_ਹੈ(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT__ABBREV(&filter.abbrev),

		OPT_ਸਮੂਹ(N_("Specific git-branch actions:")),
		OPT_ਸੈੱਟ_INT_F('a', "all", &filter.ਕਿਸਮ, N_("list both remote-tracking and local branches"),
			      FILTER_REFS_REMOTES | FILTER_REFS_BRANCHES,
			      PARSE_OPT_NONEG),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('d', "delete", &delete, N_("delete fully merged branch"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('D', ਨਿਰਾ, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('M', ਨਿਰਾ, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ਖਾਲੀ_ਥਾਵਾਂ,
			N_("do not output a newline after empty formatted refs")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('C', ਨਿਰਾ, &copy, N_("copy a branch, even if target exists"), 2),
		OPT_BOOL('l', "list", &list, N_("list branch names")),
		OPT_BOOL(0, "show-current", &show_current, N_("show current branch name")),
		OPT_BOOL(0, "create-reflog", &reflog, N_("create the branch's reflog")),
		OPT_BOOL(0, "edit-description", &edit_description,
			 N_("edit the description for the branch")),
		OPT_CALLBACK_F(0, "delete-merged", &delete_merged, N_("pattern"),
			N_("delete merged branches whose upstream matches <pattern> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_strvec),
		OPT_BOOL(0, "dry-run", &dry_ਲੜੀਵਾਰ,
			N_("with --delete-merged, only print which branches would be deleted")),
		OPT__FORCE(&force, N_("force creation, move/rename, deletion"), PARSE_OPT_NOCOMPLETE),
		OPT_MERGED(&filter, N_("print only branches that are merged")),
		OPT_NO_MERGED(&filter, N_("print only branches that are not merged")),
		OPT_CALLBACK_F(0, "forked", &filter, N_("branch"),
			N_("print only branches whose upstream matches <branch> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_forked),
		OPT_ਕਾਲਮ(0, "column", &colopts, N_("list branches in columns")),
		OPT_REF_ਛਾਂਟੋ(&ਛਾਂਟੀ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at, N_("object"),
			N_("print only branches of the object"), parse_opt_object_name),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules_explicit, N_("recurse through submodules")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_ਸਿਰਾ(),
	};

	ਤਿਆਰੀ_ref_filter_porcelain_ਸੁਨੇਹਾ();

	filter.ਕਿਸਮ = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &ਛਾਂਟੀ_options);
	ਜੇ (!ਛਾਂਟੀ_options.nr)
		string_list_append(&ਛਾਂਟੀ_options, "refname");

	track = cfg->branch_track;

	ਸਿਰ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &ਸਿਰ_oid, ਨਿਰਾ);
	ਜੇ (!ਸਿਰ)
		die(_("failed to resolve HEAD as a valid ref"));
	ਜੇ (!strcmp(ਸਿਰ, "HEAD"))
		filter.detached = 1;
	ਨਹੀਂਤਾਂ ਜੇ (!skip_ਅਗੇਤਰ(ਸਿਰ, "refs/heads/", &ਸਿਰ))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_branch_usage,
			     0);

	ਜੇ (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ਜੇ (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ਜੇ (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ਜੇ (dry_ਲੜੀਵਾਰ && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ਜੇ (recurse_submodules_explicit) {
		ਜੇ (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ਜੇ (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ਜੇ (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ਜੇ (filter.verbose) {
		ਜੇ (explicitly_enable_ਕਾਲਮ(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ਜੇ (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ਜੇ (list)
		ਤਿਆਰੀ_auto_pager("branch", 1);

	ਜੇ (delete) {
		ਜੇ (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.ਕਿਸਮ,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_ਲੜੀਵਾਰ ? DELETE_BRANCH_DRY_ਲੜੀਵਾਰ : 0));
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (show_current) {
		print_current_branch_name();
		ret = 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ਜੇ ((filter.ਕਿਸਮ & FILTER_REFS_BRANCHES) && filter.detached)
			filter.ਕਿਸਮ |= FILTER_REFS_DETACHED_ਸਿਰ;
		filter.name_patterns = argv;
		/*
		 * If no sorting parameter is given then we default to sorting
		 * by 'refname'. This would give us an alphabetically sorted
		 * array with the 'HEAD' ref at the beginning followed by
		 * local branches 'refs/heads/...' and finally remote-tracking
		 * branches 'refs/remotes/...'.
		 */
		ਛਾਂਟੀ = ref_ਛਾਂਟੀ_options(&ਛਾਂਟੀ_options);
		ref_ਛਾਂਟੀ_ਸੈੱਟ_ਛਾਂਟੋ_flags_all(ਛਾਂਟੀ, REF_ਛਾਂਟੀ_ICASE, icase);
		ref_ਛਾਂਟੀ_ਸੈੱਟ_ਛਾਂਟੋ_flags_all(
			ਛਾਂਟੀ, REF_ਛਾਂਟੀ_DETACHED_ਸਿਰ_ਪਹਿਲਾ, 1);
		print_ref_list(&filter, ਛਾਂਟੀ, &format, &output);
		print_columns(&output, colopts, ਨਿਰਾ);
		string_list_clear(&output, 0);
		ref_ਛਾਂਟੀ_release(ਛਾਂਟੀ);
		ref_filter_clear(&filter);

		ret = 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (edit_description) {
		ਅਚਲ ਅੱਖਰ *branch_name;
		ਬਣਤਰ strbuf branch_ref = STRBUF_INIT;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		ਜੇ (!argc) {
			ਜੇ (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = ਸਿਰ;
		} ਨਹੀਂਤਾਂ ਜੇ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} ਨਹੀਂਤਾਂ {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_ਬਾਹਰਲਾ(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} ਨਹੀਂਤਾਂ {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (copy || rename) {
		ਜੇ (!argc)
			die(_("branch name required"));
		ਨਹੀਂਤਾਂ ਜੇ ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		ਨਹੀਂਤਾਂ ਜੇ (argc == 1)
			copy_or_rename_branch(ਸਿਰ, argv[0], copy, copy + rename > 1);
		ਨਹੀਂਤਾਂ ਜੇ (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		ਨਹੀਂਤਾਂ
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} ਨਹੀਂਤਾਂ ਜੇ (new_upstream) {
		ਬਣਤਰ branch *branch;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		ਜੇ (!argc)
			branch = branch_get(ਨਿਰਾ);
		ਨਹੀਂਤਾਂ ਜੇ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ਨਹੀਂਤਾਂ
			die(_("too many arguments to set new upstream"));

		ਜੇ (!branch) {
			ਜੇ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ਜੇ (!argc || branch_checked_ਬਾਹਰਲਾ(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ਜੇ (argc == 1 &&
			    advice_enabled(ADVICE_ਸੈੱਟ_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_ਤਿਆਰੀ_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} ਨਹੀਂਤਾਂ ਜੇ (unset_upstream) {
		ਬਣਤਰ branch *branch;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		ਜੇ (!argc)
			branch = branch_get(ਨਿਰਾ);
		ਨਹੀਂਤਾਂ ਜੇ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ਨਹੀਂਤਾਂ
			die(_("too many arguments to unset upstream"));

		ਜੇ (!branch) {
			ਜੇ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ਜੇ (!branch_has_ਮਿਲਾਓ_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, ਨਿਰਾ, ਨਿਰਾ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, ਨਿਰਾ, ਨਿਰਾ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} ਨਹੀਂਤਾਂ ਜੇ (!noncreate_actions && argc > 0 && argc <= 2) {
		ਅਚਲ ਅੱਖਰ *branch_name = argv[0];
		ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ_name = argc == 2 ? argv[1] : ਸਿਰ;

		ਜੇ (filter.ਕਿਸਮ != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ਜੇ (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ਜੇ (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    ਸ਼ੁਰੂ_name, ਨਿਰਾ, force,
						    reflog, quiet, track, 0);
			ret = 0;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		create_branch(the_repository, branch_name, ਸ਼ੁਰੂ_name, force, 0,
			      reflog, quiet, track, 0);
	} ਨਹੀਂਤਾਂ
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

ਬਾਹਰਲਾ:
	strvec_clear(&delete_merged);
	string_list_clear(&ਛਾਂਟੀ_options, 0);
	ਵਾਪਸ ret;
}
