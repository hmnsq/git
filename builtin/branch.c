/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "color.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "refs.h"
#ضمن "commit.h"
#ضمن "gettext.h"
#ضمن "object-name.h"
#ضمن "remote.h"
#ضمن "parse-options.h"
#ضمن "branch.h"
#ضمن "path.h"
#ضمن "string-list.h"
#ضمن "strmap.h"
#ضمن "column.h"
#ضمن "utf8.h"
#ضمن "ref-filter.h"
#ضمن "worktree.h"
#ضمن "help.h"
#ضمن "advice.h"
#ضمن "commit-reach.h"

ساكن ثابت حرف * ثابت builtin_branch_usage[] = {
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
	عدم
};

ساكن ثابت حرف *سر;
ساكن بنية object_id سر_oid;
ساكن صحيح recurse_submodules = 0;
ساكن صحيح submodule_propagate_branches = 0;

ساكن تعداد git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
ساكن حرف branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_نورمال,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_نورمال,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
تعداد color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

ساكن ثابت حرف *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

ساكن بنية string_list output = STRING_LIST_INIT_DUP;
ساكن بینښه صحيح colopts;

define_list_config_array(color_branch_slots);

ساكن صحيح git_branch_config(ثابت حرف *var, ثابت حرف *ارزښت,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *خونه_name;

	إذا (!strcmp(var, "branch.sort")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		string_list_append(cb, ارزښت);
		أرجع 0;
	}

	إذا (starts_with(var, "column."))
		أرجع git_ستنه_config(var, ارزښت, "branch", &colopts);
	إذا (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, ارزښت);
		أرجع 0;
	}
	إذا (skip_مختاړی(var, "color.branch.", &خونه_name)) {
		صحيح خونه = LOOKUP_CONFIG(color_branch_slots, خونه_name);
		إذا (خونه < 0)
			أرجع 0;
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		أرجع color_parse(ارزښت, branch_colors[خونه]);
	}
	إذا (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ارزښت);
		أرجع 0;
	}
	إذا (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, ارزښت);
		أرجع 0;
	}

	إذا (git_color_config(var, ارزښت, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

ساكن ثابت حرف *branch_get_color(تعداد color_branch ix)
{
	إذا (want_color(branch_use_color))
		أرجع branch_colors[ix];
	أرجع "";
}

ساكن صحيح branch_merged(صحيح ډول, ثابت حرف *name,
			 بنية commit *rev, بنية commit *سر_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	بنية commit *reference_rev = عدم;
	ثابت حرف *reference_name = عدم;
	فراغ *reference_name_منزل_free = عدم;
	صحيح merged;

	إذا (ډول == FILTER_REFS_BRANCHES) {
		بنية branch *branch = branch_get(name);
		ثابت حرف *upstream = branch_get_upstream(branch, عدم);
		بنية object_id oid;

		إذا (upstream &&
		    (reference_name = reference_name_منزل_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, عدم)) != عدم)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	إذا (!reference_rev)
		reference_rev = سر_rev;

	merged = reference_rev ? repo_دننه_یوځای_bases(the_repository, rev,
						     reference_rev) : 0;
	إذا (merged < 0)
		وتل(128);

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
	إذا (سر_rev && سر_rev != reference_rev) {
		صحيح expect = repo_دننه_یوځای_bases(the_repository, rev, سر_rev);
		إذا (expect < 0)
			وتل(128);
		إذا (expect == merged)
			; /* okay */
		وإلا إذا (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		وإلا
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	حرر(reference_name_منزل_free);
	أرجع merged;
}

تعداد delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_سر_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_پرلپسې = (1 << 4),
};

ساكن صحيح check_branch_commit(ثابت حرف *branchname, ثابت حرف *refname,
			       ثابت بنية object_id *oid, بنية commit *سر_rev,
			       صحيح kinds, بینښه صحيح flags)
{
	بنية commit *rev = lookup_commit_reference(the_repository, oid);
	إذا (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		أرجع -1;
	}
	إذا (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, سر_rev)) {
		إذا (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		أرجع -1;
	}
	أرجع 0;
}

ساكن فراغ delete_branch_config(ثابت حرف *branchname)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	إذا (repo_config_rename_section(the_repository, buf.buf, عدم) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

ساكن صحيح delete_branches(صحيح argc, ثابت حرف **argv, صحيح kinds,
			   بینښه صحيح flags)
{
	بنية commit *سر_rev = عدم;
	بنية object_id oid;
	حرف *name = عدم;
	ثابت حرف *fmt;
	صحيح i;
	صحيح ret = 0;
	صحيح remote_branch = 0;
	بنية strbuf bname = STRBUF_INIT;
	تعداد interpret_branch_ډول allowed_interpret;
	بنية string_list refs_منزل_delete = STRING_LIST_INIT_DUP;
	بنية string_list_توکی *توکی;
	صحيح branch_name_pos;
	ثابت حرف *fmt_remotes = "refs/remotes/%s";

	بدل (kinds) {
	حالة FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		اكسر;
	حالة FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		اكسر;
	افتراضي:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	إذا (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_سر_FALLBACK))
		سر_rev = lookup_commit_reference(the_repository, &سر_oid);

	لكل (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		حرف *موخه = عدم;
		صحيح ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		حرر(name);
		name = mkpathdup(fmt, bname.buf);

		إذا (kinds == FILTER_REFS_BRANCHES) {
			ثابت حرف *لار;
			إذا ((لار = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, لار);
				ret = 1;
				استمر;
			}
			إذا ((لار = branch_checked_بهر(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, لار);
				ret = 1;
				استمر;
			}
		}

		موخه = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		إذا (!موخه) {
			إذا (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} وإلا {
				حرف *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				حرف *virtual_موخه = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				إذا (virtual_موخه)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				وإلا
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_موخه);
			}
			ret = 1;
			استمر;
		}

		إذا (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, سر_rev, kinds,
					flags)) {
			إذا (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			اذهب بل;
		}

		توکی = string_list_append(&refs_منزل_delete, name);
		توکی->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? موخه
				    : repo_پیدا_کړه_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	بل:
		حرر(موخه);
	}

	إذا (!(flags & DELETE_BRANCH_DRY_پرلپسې) &&
	    refs_delete_refs(get_main_ref_store(the_repository), عدم, &refs_منزل_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_توکی(توکی, &refs_منزل_delete) {
		حرف *describe_ref = توکی->util;
		حرف *name = توکی->string;
		إذا (flags & DELETE_BRANCH_DRY_پرلپسې) {
			إذا (!(flags & DELETE_BRANCH_QUIET))
				چاپ_کړه(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} وإلا إذا (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			حرف *refname = name + branch_name_pos;
			إذا (!(flags & DELETE_BRANCH_QUIET))
				چاپ_کړه(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		حرر(describe_ref);
	}
	string_list_clear(&refs_منزل_delete, 0);

	حرر(name);
	strbuf_release(&bname);

	أرجع ret;
}

ساكن صحيح calc_maxwidth(بنية ref_array *refs, صحيح remote_bonus)
{
	صحيح i, max = 0;
	لكل (i = 0; i < refs->nr; i++) {
		بنية ref_array_توکی *it = refs->توکي[i];
		ثابت حرف *desc = it->refname;
		صحيح w;

		skip_مختاړی(it->refname, "refs/heads/", &desc);
		skip_مختاړی(it->refname, "refs/remotes/", &desc);
		إذا (it->ډول == FILTER_REFS_DETACHED_سر) {
			حرف *سر_desc = get_سر_description();
			w = utf8_strwidth(سر_desc);
			حرر(سر_desc);
		} وإلا
			w = utf8_strwidth(desc);

		إذا (it->ډول == FILTER_REFS_REMOTES)
			w += remote_bonus;
		إذا (w > max)
			max = w;
	}
	أرجع max;
}

ساكن ثابت حرف *quote_literal_for_format(ثابت حرف *s)
{
	ساكن بنية strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	طالما (strbuf_expand_ګام(&buf, &s))
		strbuf_addstr(&buf, "%%");
	أرجع buf.buf;
}

ساكن حرف *build_format(بنية ref_filter *filter, صحيح maxwidth, ثابت حرف *remote_مختاړی)
{
	بنية strbuf fmt = STRBUF_INIT;
	بنية strbuf local = STRBUF_INIT;
	بنية strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	إذا (filter->verbose) {
		بنية strbuf obname = STRBUF_INIT;

		إذا (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		وإلا إذا (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		وإلا
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		إذا (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		وإلا
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_مختاړی),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} وإلا {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_مختاړی),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	أرجع strbuf_detach(&fmt, عدم);
}

ساكن فراغ print_ref_list(بنية ref_filter *filter, بنية ref_ترتیبونه *ترتیبونه,
			   بنية ref_format *format, بنية string_list *output)
{
	صحيح i;
	بنية ref_array array;
	صحيح maxwidth = 0;
	ثابت حرف *remote_مختاړی = "";
	حرف *منزل_free = عدم;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	إذا (filter->ډول != FILTER_REFS_REMOTES)
		remote_مختاړی = "remotes/";

	memset(&array, 0, حجم(array));

	filter_refs(&array, filter, filter->ډول);

	إذا (filter->verbose)
		maxwidth = calc_maxwidth(&array, مزي_اوږدوالی(remote_مختاړی));

	إذا (!format->format)
		format->format = منزل_free = build_format(filter, maxwidth, remote_مختاړی);
	format->use_color = branch_use_color;

	إذا (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_ترتیبول(ترتیبونه, &array);

	إذا (ستنه_فعال(colopts)) {
		بنية strbuf بهر = STRBUF_INIT, err = STRBUF_INIT;

		أكد(!filter->verbose && "--column and --verbose are incompatible");

		لكل (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&بهر);
			إذا (format_ref_array_توکی(array.توکي[i], format, &بهر, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, بهر.buf);
		}

		strbuf_release(&err);
		strbuf_release(&بهر);
	} وإلا {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	حرر(منزل_free);
}

ساكن فراغ print_current_branch_name(فراغ)
{
	صحيح flags;
	ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, عدم, &flags);
	ثابت حرف *shortname;
	إذا (!refname)
		die(_("could not resolve HEAD"));
	وإلا إذا (!(flags & REF_ISSYMREF))
		أرجع;
	وإلا إذا (skip_مختاړی(refname, "refs/heads/", &shortname))
		puts(shortname);
	وإلا
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

ساكن فراغ reject_rebase_or_bisect_branch(بنية worktree **worktrees,
					   ثابت حرف *موخه)
{
	صحيح i;

	لكل (i = 0; worktrees[i]; i++) {
		بنية worktree *wt = worktrees[i];

		إذا (!wt->is_detached)
			استمر;

		إذا (is_worktree_being_rebased(wt, موخه))
			die(_("branch %s is being rebased at %s"),
			    موخه, wt->لار);

		إذا (is_worktree_being_bisected(wt, موخه))
			die(_("branch %s is being bisected at %s"),
			    موخه, wt->لار);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
ساكن صحيح replace_each_worktree_سر_symref(بنية worktree **worktrees,
					     ثابت حرف *oldref, ثابت حرف *newref,
					     ثابت حرف *logmsg)
{
	صحيح ret = 0;
	صحيح i;

	لكل (i = 0; worktrees[i]; i++) {
		بنية ref_store *refs;

		إذا (worktrees[i]->is_detached)
			استمر;
		إذا (!worktrees[i]->سر_ref)
			استمر;
		إذا (strcmp(oldref, worktrees[i]->سر_ref))
			استمر;

		refs = get_worktree_ref_store(worktrees[i]);
		إذا (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->لار);
	}

	أرجع ret;
}

#تعریف_کړه IS_سر 1
#تعریف_کړه IS_ORPHAN 2

ساكن فراغ copy_or_rename_branch(ثابت حرف *oldname, ثابت حرف *newname, صحيح copy, صحيح force)
{
	بنية strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	بنية strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ثابت حرف *interpreted_oldname = عدم;
	ثابت حرف *interpreted_newname = عدم;
	صحيح recovery = 0, oldref_usage = 0;
	بنية worktree **worktrees = get_worktrees(the_repository);

	إذا (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		إذا (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		وإلا {
			صحيح کوډ = die_پیغام(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			وتل(کوډ);
		}
	}

	لكل (صحيح i = 0; worktrees[i]; i++) {
		بنية worktree *wt = worktrees[i];

		إذا (wt->سر_ref && !strcmp(oldref.buf, wt->سر_ref)) {
			oldref_usage |= IS_سر;
			إذا (is_null_oid(&wt->سر_oid))
				oldref_usage |= IS_ORPHAN;
			اكسر;
		}
	}

	إذا ((copy || !(oldref_usage & IS_سر)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		إذا (oldref_usage & IS_سر)
			die(_("no commit on branch '%s' yet"), oldname);
		وإلا
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	إذا (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	وإلا
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	إذا (!skip_مختاړی(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_مختاړی(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	إذا (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	وإلا
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	إذا (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	إذا (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	إذا (recovery) {
		إذا (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		وإلا
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	إذا (!copy && (oldref_usage & IS_سر) &&
	    replace_each_worktree_سر_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	إذا (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	إذا (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

ساكن صحيح parse_opt_forked(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية ref_filter *filter = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);
	إذا (ref_filter_forked_جمع_کړه(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	أرجع 0;
}

بنية stacked_branch_معلومات {
	بنية strset *deletable_branch_نومونه;
	بنية strset *protected_branch_نومونه;
};

ساكن صحيح collect_stacked_branch_بنسټ(ثابت بنية reference *ref,
				       فراغ *cb_معلومات)
{
	بنية stacked_branch_معلومات *معلومات = cb_معلومات;
	ثابت حرف *branch_name;
	بنية branch *branch;
	ثابت حرف *upstream_refname;
	ثابت حرف *upstream_branch_name;

	إذا (!skip_مختاړی(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	إذا (strset_لري(معلومات->deletable_branch_نومونه, branch_name))
		أرجع 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, عدم);
	إذا (!upstream_refname ||
	    !skip_مختاړی(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_لري(معلومات->deletable_branch_نومونه,
			    upstream_branch_name))
		أرجع 0;

	strset_جمع_کړه(معلومات->protected_branch_نومونه, upstream_branch_name);
	أرجع 0;
}

ساكن فراغ protect_stacked_branch_bases(بنية ref_store *refs,
					 بنية strset *deletable_branch_نومونه,
					 بنية strset *protected_branch_نومونه)
{
	بنية stacked_branch_معلومات معلومات = {
		.deletable_branch_نومونه = deletable_branch_نومونه,
		.protected_branch_نومونه = protected_branch_نومونه,
	};
	بنية refs_for_each_ref_options opts = {
		.مختاړی = "refs/heads/",
	};
	بنية hashmap_iter iter;
	بنية strmap_ثبت *ثبت;

	refs_for_each_ref_ext(refs, collect_stacked_branch_بنسټ, &معلومات, &opts);

	strset_for_each_ثبت(protected_branch_نومونه, &iter, ثبت)
		strset_remove(deletable_branch_نومونه, ثبت->کلی);
}

ساكن فراغ clear_deleted_upstreams(بنية strset *protected_branch_نومونه,
				    بنية strset *deletable_branch_نومونه)
{
	بنية strbuf کلی = STRBUF_INIT;
	بنية hashmap_iter iter;
	بنية strmap_ثبت *ثبت;

	strset_for_each_ثبت(protected_branch_نومونه, &iter, ثبت) {
		بنية branch *branch = branch_get(ثبت->کلی);
		ثابت حرف *upstream_refname = branch_get_upstream(branch, عدم);
		ثابت حرف *upstream_branch_name;

		إذا (!upstream_refname ||
		    !skip_مختاړی(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_لري(deletable_branch_نومونه,
				    upstream_branch_name))
			استمر;

		strbuf_addf(&کلی, "branch.%s.merge", branch->name);
		repo_config_سېټ_gently(the_repository, کلی.buf, عدم);
		strbuf_reset(&کلی);
		strbuf_addf(&کلی, "branch.%s.remote", branch->name);
		repo_config_سېټ_gently(the_repository, کلی.buf, عدم);
		strbuf_reset(&کلی);
	}

	strbuf_release(&کلی);
}

ساكن صحيح branch_pushes_منزل_upstream(بنية branch *branch,
				     ثابت حرف *upstream)
{
	بنية remote *remote = remote_get(remote_for_branch(branch, عدم));
	حرف *ټېل_refname = عدم;
	حرف *tracking = عدم;
	صحيح ret = 0;

	إذا (!remote)
		أرجع 0;
	إذا (remote->ټېل.nr)
		ټېل_refname = apply_refspecs(&remote->ټېل, branch->refname);
	وإلا
		ټېل_refname = xstrdup(branch->refname);
	إذا (ټېل_refname)
		tracking = apply_refspecs(&remote->fetch, ټېل_refname);
	إذا (tracking && !strcmp(tracking, upstream))
		ret = 1;

	حرر(ټېل_refname);
	حرر(tracking);
	أرجع ret;
}

ساكن صحيح delete_merged_branches(ثابت بنية strvec *upstreams,
				 ثابت حرف **argv, بینښه صحيح flags)
{
	بنية ref_store *refs = get_main_ref_store(the_repository);
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_array candidates = { 0 };
	بنية strset deletable_branch_نومونه = STRSET_INIT;
	بنية strset protected_branch_نومونه = STRSET_INIT;
	بنية strvec branches_منزل_delete = STRVEC_INIT;
	بنية strbuf کلی = STRBUF_INIT;
	بنية hashmap_iter iter;
	بنية strmap_ثبت *ثبت;
	صحيح ret = 0;

	لكل (اندازه_t i = 0; i < upstreams->nr; i++)
		إذا (ref_filter_forked_جمع_کړه(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.ډول = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.ډول);

	لكل (صحيح i = 0; i < candidates.nr; i++) {
		ثابت حرف *branch_refname = candidates.توکي[i]->refname;
		ثابت حرف *branch_name;
		بنية branch *branch;
		ثابت حرف *upstream_refname;
		صحيح opt_بهر;

		إذا (!skip_مختاړی(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		إذا (branch_checked_بهر(branch_refname))
			استمر;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, عدم);
		إذا (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			استمر;
		إذا (branch_pushes_منزل_upstream(branch, upstream_refname))
			استمر;
		إذا (check_branch_commit(branch_name, branch_name,
					&candidates.توکي[i]->objectname, عدم,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			استمر;

		strbuf_reset(&کلی);
		strbuf_addf(&کلی, "branch.%s.deletemerged", branch_name);
		إذا (!repo_config_get_bool(the_repository, کلی.buf, &opt_بهر) &&
		    !opt_بهر) {
			إذا (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			استمر;
		}

		strset_جمع_کړه(&deletable_branch_نومونه, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_نومونه,
				     &protected_branch_نومونه);

	strset_for_each_ثبت(&deletable_branch_نومونه, &iter, ثبت)
		strvec_ټېل(&branches_منزل_delete, ثبت->کلی);

	إذا (branches_منزل_delete.nr)
		ret = delete_branches(branches_منزل_delete.nr, branches_منزل_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_سر_FALLBACK |
				      flags);

	إذا (!ret && !(flags & DELETE_BRANCH_DRY_پرلپسې))
		clear_deleted_upstreams(&protected_branch_نومونه,
					&deletable_branch_نومونه);

	strbuf_release(&کلی);
	strvec_clear(&branches_منزل_delete);
	strset_clear(&protected_branch_نومونه);
	strset_clear(&deletable_branch_نومونه);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	أرجع ret;
}

ساكن GIT_لار_FUNC(edit_description, "EDIT_DESCRIPTION")

ساكن صحيح edit_branch_description(ثابت حرف *branch_name)
{
	صحيح exists;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	إذا (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_کرښه_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_کرښه_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	إذا (launch_editor(edit_description(), &buf, عدم)) {
		strbuf_release(&buf);
		أرجع -1;
	}
	strbuf_stripspace(&buf, comment_کرښه_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	إذا (buf.len || exists)
		repo_config_سېټ(the_repository, name.buf, buf.len ? buf.buf : عدم);
	strbuf_release(&name);
	strbuf_release(&buf);

	أرجع 0;
}

ساكن فراغ die_if_upstream_looks_like_remote(ثابت حرف *new_upstream, ثابت حرف *branch_name)
{
	بنية strbuf remote_ref = STRBUF_INIT;
	صحيح کوډ;

	إذا (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		أرجع;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	إذا (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		أرجع;
	}

	کوډ = die_پیغام(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_سېټ_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	وتل(کوډ);
}

صحيح cmd_branch(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *مختاړی,
	       بنية repository *repo ناکارول_شوی)
{
	/* possible actions */
	صحيح delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	بنية strvec delete_merged = STRVEC_INIT;
	صحيح dry_پرلپسې = 0;
	ثابت حرف *new_upstream = عدم;
	صحيح noncreate_actions = 0;
	/* possible options */
	صحيح reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	تعداد branch_track track;
	بنية ref_filter filter = REF_FILTER_INIT;
	ساكن بنية ref_ترتیبونه *ترتیبونه;
	بنية string_list ترتیبونه_options = STRING_LIST_INIT_DUP;
	بنية ref_format format = REF_FORMAT_INIT;
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);
	صحيح ret;

	بنية option options[] = {
		OPT_ډله(N_("Generic options")),
		OPT__VERBOSE(&filter.verbose,
			N_("show hash and subject, give twice for upstream branch")),
		OPT__QUIET(&quiet, N_("suppress informational messages")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_ډېرپېښ),
		OPT_سېټ_INT_F(0, "set-upstream", &track, N_("do not use"),
			BRANCH_TRACK_OVERRIDE, PARSE_OPT_HIDDEN),
		OPT_STRING('u', "set-upstream-to", &new_upstream, N_("upstream"), N_("change the upstream info")),
		OPT_BOOL(0, "unset-upstream", &unset_upstream, N_("unset the upstream info")),
		OPT__COLOR(&branch_use_color, N_("use colored output")),
		OPT_سېټ_INT_F('r', "remotes",     &filter.ډول, N_("act on remote-tracking branches"),
			      FILTER_REFS_REMOTES,
			      PARSE_OPT_NONEG),
		OPT_لري(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_NO_لري(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT__ABBREV(&filter.abbrev),

		OPT_ډله(N_("Specific git-branch actions:")),
		OPT_سېټ_INT_F('a', "all", &filter.ډول, N_("list both remote-tracking and local branches"),
			      FILTER_REFS_REMOTES | FILTER_REFS_BRANCHES,
			      PARSE_OPT_NONEG),
		OPT_بټ('d', "delete", &delete, N_("delete fully merged branch"), 1),
		OPT_بټ('D', عدم, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_بټ('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_بټ('M', عدم, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_تشه,
			N_("do not output a newline after empty formatted refs")),
		OPT_بټ('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_بټ('C', عدم, &copy, N_("copy a branch, even if target exists"), 2),
		OPT_BOOL('l', "list", &list, N_("list branch names")),
		OPT_BOOL(0, "show-current", &show_current, N_("show current branch name")),
		OPT_BOOL(0, "create-reflog", &reflog, N_("create the branch's reflog")),
		OPT_BOOL(0, "edit-description", &edit_description,
			 N_("edit the description for the branch")),
		OPT_CALLBACK_F(0, "delete-merged", &delete_merged, N_("pattern"),
			N_("delete merged branches whose upstream matches <pattern> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_strvec),
		OPT_BOOL(0, "dry-run", &dry_پرلپسې,
			N_("with --delete-merged, only print which branches would be deleted")),
		OPT__FORCE(&force, N_("force creation, move/rename, deletion"), PARSE_OPT_NOCOMPLETE),
		OPT_MERGED(&filter, N_("print only branches that are merged")),
		OPT_NO_MERGED(&filter, N_("print only branches that are not merged")),
		OPT_CALLBACK_F(0, "forked", &filter, N_("branch"),
			N_("print only branches whose upstream matches <branch> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_forked),
		OPT_ستنه(0, "column", &colopts, N_("list branches in columns")),
		OPT_REF_ترتیبول(&ترتیبونه_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at, N_("object"),
			N_("print only branches of the object"), parse_opt_object_name),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules_explicit, N_("recurse through submodules")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_پای_ټکی(),
	};

	تیاری_ref_filter_porcelain_پیغام();

	filter.ډول = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &ترتیبونه_options);
	إذا (!ترتیبونه_options.nr)
		string_list_append(&ترتیبونه_options, "refname");

	track = cfg->branch_track;

	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &سر_oid, عدم);
	إذا (!سر)
		die(_("failed to resolve HEAD as a valid ref"));
	إذا (!strcmp(سر, "HEAD"))
		filter.detached = 1;
	وإلا إذا (!skip_مختاړی(سر, "refs/heads/", &سر))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, مختاړی, options, builtin_branch_usage,
			     0);

	إذا (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	إذا (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	إذا (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	إذا (dry_پرلپسې && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	إذا (recurse_submodules_explicit) {
		إذا (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		إذا (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	إذا (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	إذا (filter.verbose) {
		إذا (explicitly_enable_ستنه(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	إذا (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	إذا (list)
		تیاری_auto_pager("branch", 1);

	إذا (delete) {
		إذا (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.ډول,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		اذهب بهر;
	} وإلا إذا (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_پرلپسې ? DELETE_BRANCH_DRY_پرلپسې : 0));
		اذهب بهر;
	} وإلا إذا (show_current) {
		print_current_branch_name();
		ret = 0;
		اذهب بهر;
	} وإلا إذا (list) {
		/*  git branch --list also shows HEAD when it is detached */
		إذا ((filter.ډول & FILTER_REFS_BRANCHES) && filter.detached)
			filter.ډول |= FILTER_REFS_DETACHED_سر;
		filter.name_patterns = argv;
		/*
		 * If no sorting parameter is given then we default to sorting
		 * by 'refname'. This would give us an alphabetically sorted
		 * array with the 'HEAD' ref at the beginning followed by
		 * local branches 'refs/heads/...' and finally remote-tracking
		 * branches 'refs/remotes/...'.
		 */
		ترتیبونه = ref_ترتیبونه_options(&ترتیبونه_options);
		ref_ترتیبونه_سېټ_ترتیبول_flags_all(ترتیبونه, REF_ترتیبونه_ICASE, icase);
		ref_ترتیبونه_سېټ_ترتیبول_flags_all(
			ترتیبونه, REF_ترتیبونه_DETACHED_سر_لومړی, 1);
		print_ref_list(&filter, ترتیبونه, &format, &output);
		print_columns(&output, colopts, عدم);
		string_list_clear(&output, 0);
		ref_ترتیبونه_release(ترتیبونه);
		ref_filter_clear(&filter);

		ret = 0;
		اذهب بهر;
	} وإلا إذا (edit_description) {
		ثابت حرف *branch_name;
		بنية strbuf branch_ref = STRBUF_INIT;
		بنية strbuf buf = STRBUF_INIT;

		إذا (!argc) {
			إذا (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = سر;
		} وإلا إذا (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} وإلا {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		إذا (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_بهر(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} وإلا إذا (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} وإلا {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		اذهب بهر;
	} وإلا إذا (copy || rename) {
		إذا (!argc)
			die(_("branch name required"));
		وإلا إذا ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		وإلا إذا (argc == 1)
			copy_or_rename_branch(سر, argv[0], copy, copy + rename > 1);
		وإلا إذا (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		وإلا
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} وإلا إذا (new_upstream) {
		بنية branch *branch;
		بنية strbuf buf = STRBUF_INIT;

		إذا (!argc)
			branch = branch_get(عدم);
		وإلا إذا (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} وإلا
			die(_("too many arguments to set new upstream"));

		إذا (!branch) {
			إذا (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		إذا (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			إذا (!argc || branch_checked_بهر(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			إذا (argc == 1 &&
			    advice_enabled(ADVICE_سېټ_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_تیاری_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} وإلا إذا (unset_upstream) {
		بنية branch *branch;
		بنية strbuf buf = STRBUF_INIT;

		إذا (!argc)
			branch = branch_get(عدم);
		وإلا إذا (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} وإلا
			die(_("too many arguments to unset upstream"));

		إذا (!branch) {
			إذا (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		إذا (!branch_has_یوځای_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_سېټ_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_سېټ_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} وإلا إذا (!noncreate_actions && argc > 0 && argc <= 2) {
		ثابت حرف *branch_name = argv[0];
		ثابت حرف *پیل_name = argc == 2 ? argv[1] : سر;

		إذا (filter.ډول != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		إذا (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		إذا (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    پیل_name, عدم, force,
						    reflog, quiet, track, 0);
			ret = 0;
			اذهب بهر;
		}
		create_branch(the_repository, branch_name, پیل_name, force, 0,
			      reflog, quiet, track, 0);
	} وإلا
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

بهر:
	strvec_clear(&delete_merged);
	string_list_clear(&ترتیبونه_options, 0);
	أرجع ret;
}
