/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "color.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "refs.h"
#شامل "commit.h"
#شامل "gettext.h"
#شامل "object-name.h"
#شامل "remote.h"
#شامل "parse-options.h"
#شامل "branch.h"
#شامل "path.h"
#شامل "string-list.h"
#شامل "strmap.h"
#شامل "column.h"
#شامل "utf8.h"
#شامل "ref-filter.h"
#شامل "worktree.h"
#شامل "help.h"
#شامل "advice.h"
#شامل "commit-reach.h"

ساکن ثابت حرف * ثابت builtin_branch_usage[] = {
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
	تہی
};

ساکن ثابت حرف *سر;
ساکن ساخت object_id سر_oid;
ساکن صحیح recurse_submodules = 0;
ساکن صحیح submodule_propagate_branches = 0;

ساکن شمار git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
ساکن حرف branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_معمول,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_معمول,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
شمار color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

ساکن ثابت حرف *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

ساکن ساخت string_list output = STRING_LIST_INIT_DUP;
ساکن غیرعلامتی صحیح colopts;

define_list_config_array(color_branch_slots);

ساکن صحیح git_branch_config(ثابت حرف *var, ثابت حرف *قدر,
			     ثابت ساخت config_context *ctx, خالی *cb)
{
	ثابت حرف *خانہ_name;

	اگر (!strcmp(var, "branch.sort")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		string_list_append(cb, قدر);
		واپس 0;
	}

	اگر (starts_with(var, "column."))
		واپس git_عمود_config(var, قدر, "branch", &colopts);
	اگر (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, قدر);
		واپس 0;
	}
	اگر (skip_سابقہ(var, "color.branch.", &خانہ_name)) {
		صحیح خانہ = LOOKUP_CONFIG(color_branch_slots, خانہ_name);
		اگر (خانہ < 0)
			واپس 0;
		اگر (!قدر)
			واپس config_error_nonbool(var);
		واپس color_parse(قدر, branch_colors[خانہ]);
	}
	اگر (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (git_color_config(var, قدر, cb) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن ثابت حرف *branch_get_color(شمار color_branch ix)
{
	اگر (want_color(branch_use_color))
		واپس branch_colors[ix];
	واپس "";
}

ساکن صحیح branch_merged(صحیح قسم, ثابت حرف *name,
			 ساخت commit *rev, ساخت commit *سر_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	ساخت commit *reference_rev = تہی;
	ثابت حرف *reference_name = تہی;
	خالی *reference_name_مقصد_free = تہی;
	صحیح merged;

	اگر (قسم == FILTER_REFS_BRANCHES) {
		ساخت branch *branch = branch_get(name);
		ثابت حرف *upstream = branch_get_upstream(branch, تہی);
		ساخت object_id oid;

		اگر (upstream &&
		    (reference_name = reference_name_مقصد_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, تہی)) != تہی)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	اگر (!reference_rev)
		reference_rev = سر_rev;

	merged = reference_rev ? repo_اندر_ملاؤ_bases(the_repository, rev,
						     reference_rev) : 0;
	اگر (merged < 0)
		باہر_نکلو(128);

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
	اگر (سر_rev && سر_rev != reference_rev) {
		صحیح expect = repo_اندر_ملاؤ_bases(the_repository, rev, سر_rev);
		اگر (expect < 0)
			باہر_نکلو(128);
		اگر (expect == merged)
			; /* okay */
		ورنہ اگر (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		ورنہ
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	آزاد(reference_name_مقصد_free);
	واپس merged;
}

شمار delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_سر_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_تسلسل = (1 << 4),
};

ساکن صحیح check_branch_commit(ثابت حرف *branchname, ثابت حرف *refname,
			       ثابت ساخت object_id *oid, ساخت commit *سر_rev,
			       صحیح kinds, غیرعلامتی صحیح flags)
{
	ساخت commit *rev = lookup_commit_reference(the_repository, oid);
	اگر (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		واپس -1;
	}
	اگر (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, سر_rev)) {
		اگر (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		واپس -1;
	}
	واپس 0;
}

ساکن خالی delete_branch_config(ثابت حرف *branchname)
{
	ساخت strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	اگر (repo_config_rename_section(the_repository, buf.buf, تہی) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

ساکن صحیح delete_branches(صحیح argc, ثابت حرف **argv, صحیح kinds,
			   غیرعلامتی صحیح flags)
{
	ساخت commit *سر_rev = تہی;
	ساخت object_id oid;
	حرف *name = تہی;
	ثابت حرف *fmt;
	صحیح i;
	صحیح ret = 0;
	صحیح remote_branch = 0;
	ساخت strbuf bname = STRBUF_INIT;
	شمار interpret_branch_قسم allowed_interpret;
	ساخت string_list refs_مقصد_delete = STRING_LIST_INIT_DUP;
	ساخت string_list_شے *شے;
	صحیح branch_name_pos;
	ثابت حرف *fmt_remotes = "refs/remotes/%s";

	انتخاب (kinds) {
	حالت FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		توڑو;
	حالت FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		توڑو;
	طےشدہ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	اگر (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_سر_FALLBACK))
		سر_rev = lookup_commit_reference(the_repository, &سر_oid);

	برائے (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		حرف *ہدف = تہی;
		صحیح ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		آزاد(name);
		name = mkpathdup(fmt, bname.buf);

		اگر (kinds == FILTER_REFS_BRANCHES) {
			ثابت حرف *راستہ;
			اگر ((راستہ = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, راستہ);
				ret = 1;
				جاری;
			}
			اگر ((راستہ = branch_checked_برآمد(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, راستہ);
				ret = 1;
				جاری;
			}
		}

		ہدف = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		اگر (!ہدف) {
			اگر (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} ورنہ {
				حرف *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				حرف *virtual_ہدف = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				اگر (virtual_ہدف)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				ورنہ
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_ہدف);
			}
			ret = 1;
			جاری;
		}

		اگر (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, سر_rev, kinds,
					flags)) {
			اگر (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			جاؤ اگلا;
		}

		شے = string_list_append(&refs_مقصد_delete, name);
		شے->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? ہدف
				    : repo_ڈھونڈو_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	اگلا:
		آزاد(ہدف);
	}

	اگر (!(flags & DELETE_BRANCH_DRY_تسلسل) &&
	    refs_delete_refs(get_main_ref_store(the_repository), تہی, &refs_مقصد_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_شے(شے, &refs_مقصد_delete) {
		حرف *describe_ref = شے->util;
		حرف *name = شے->string;
		اگر (flags & DELETE_BRANCH_DRY_تسلسل) {
			اگر (!(flags & DELETE_BRANCH_QUIET))
				لکھو(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} ورنہ اگر (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			حرف *refname = name + branch_name_pos;
			اگر (!(flags & DELETE_BRANCH_QUIET))
				لکھو(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		آزاد(describe_ref);
	}
	string_list_clear(&refs_مقصد_delete, 0);

	آزاد(name);
	strbuf_release(&bname);

	واپس ret;
}

ساکن صحیح calc_maxwidth(ساخت ref_array *refs, صحیح remote_bonus)
{
	صحیح i, max = 0;
	برائے (i = 0; i < refs->nr; i++) {
		ساخت ref_array_شے *it = refs->اشیاء[i];
		ثابت حرف *desc = it->refname;
		صحیح w;

		skip_سابقہ(it->refname, "refs/heads/", &desc);
		skip_سابقہ(it->refname, "refs/remotes/", &desc);
		اگر (it->قسم == FILTER_REFS_DETACHED_سر) {
			حرف *سر_desc = get_سر_description();
			w = utf8_strwidth(سر_desc);
			آزاد(سر_desc);
		} ورنہ
			w = utf8_strwidth(desc);

		اگر (it->قسم == FILTER_REFS_REMOTES)
			w += remote_bonus;
		اگر (w > max)
			max = w;
	}
	واپس max;
}

ساکن ثابت حرف *quote_literal_for_format(ثابت حرف *s)
{
	ساکن ساخت strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	جب (strbuf_expand_قدم(&buf, &s))
		strbuf_addstr(&buf, "%%");
	واپس buf.buf;
}

ساکن حرف *build_format(ساخت ref_filter *filter, صحیح maxwidth, ثابت حرف *remote_سابقہ)
{
	ساخت strbuf fmt = STRBUF_INIT;
	ساخت strbuf local = STRBUF_INIT;
	ساخت strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	اگر (filter->verbose) {
		ساخت strbuf obname = STRBUF_INIT;

		اگر (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		ورنہ اگر (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		ورنہ
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		اگر (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		ورنہ
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_سابقہ),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} ورنہ {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_سابقہ),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	واپس strbuf_detach(&fmt, تہی);
}

ساکن خالی print_ref_list(ساخت ref_filter *filter, ساخت ref_ترتیب_کاری *ترتیب_کاری,
			   ساخت ref_format *format, ساخت string_list *output)
{
	صحیح i;
	ساخت ref_array array;
	صحیح maxwidth = 0;
	ثابت حرف *remote_سابقہ = "";
	حرف *مقصد_free = تہی;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	اگر (filter->قسم != FILTER_REFS_REMOTES)
		remote_سابقہ = "remotes/";

	memset(&array, 0, حجم(array));

	filter_refs(&array, filter, filter->قسم);

	اگر (filter->verbose)
		maxwidth = calc_maxwidth(&array, لڑی_لمبائی(remote_سابقہ));

	اگر (!format->format)
		format->format = مقصد_free = build_format(filter, maxwidth, remote_سابقہ);
	format->use_color = branch_use_color;

	اگر (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_ترتیب_دو(ترتیب_کاری, &array);

	اگر (عمود_فعال(colopts)) {
		ساخت strbuf برآمد = STRBUF_INIT, err = STRBUF_INIT;

		أكد(!filter->verbose && "--column and --verbose are incompatible");

		برائے (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&برآمد);
			اگر (format_ref_array_شے(array.اشیاء[i], format, &برآمد, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, برآمد.buf);
		}

		strbuf_release(&err);
		strbuf_release(&برآمد);
	} ورنہ {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	آزاد(مقصد_free);
}

ساکن خالی print_current_branch_name(خالی)
{
	صحیح flags;
	ثابت حرف *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, تہی, &flags);
	ثابت حرف *shortname;
	اگر (!refname)
		die(_("could not resolve HEAD"));
	ورنہ اگر (!(flags & REF_ISSYMREF))
		واپس;
	ورنہ اگر (skip_سابقہ(refname, "refs/heads/", &shortname))
		puts(shortname);
	ورنہ
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

ساکن خالی reject_rebase_or_bisect_branch(ساخت worktree **worktrees,
					   ثابت حرف *ہدف)
{
	صحیح i;

	برائے (i = 0; worktrees[i]; i++) {
		ساخت worktree *wt = worktrees[i];

		اگر (!wt->is_detached)
			جاری;

		اگر (is_worktree_being_rebased(wt, ہدف))
			die(_("branch %s is being rebased at %s"),
			    ہدف, wt->راستہ);

		اگر (is_worktree_being_bisected(wt, ہدف))
			die(_("branch %s is being bisected at %s"),
			    ہدف, wt->راستہ);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
ساکن صحیح replace_each_worktree_سر_symref(ساخت worktree **worktrees,
					     ثابت حرف *oldref, ثابت حرف *newref,
					     ثابت حرف *logmsg)
{
	صحیح ret = 0;
	صحیح i;

	برائے (i = 0; worktrees[i]; i++) {
		ساخت ref_store *refs;

		اگر (worktrees[i]->is_detached)
			جاری;
		اگر (!worktrees[i]->سر_ref)
			جاری;
		اگر (strcmp(oldref, worktrees[i]->سر_ref))
			جاری;

		refs = get_worktree_ref_store(worktrees[i]);
		اگر (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->راستہ);
	}

	واپس ret;
}

#تعریف_کرو IS_سر 1
#تعریف_کرو IS_ORPHAN 2

ساکن خالی copy_or_rename_branch(ثابت حرف *oldname, ثابت حرف *newname, صحیح copy, صحیح force)
{
	ساخت strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	ساخت strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ثابت حرف *interpreted_oldname = تہی;
	ثابت حرف *interpreted_newname = تہی;
	صحیح recovery = 0, oldref_usage = 0;
	ساخت worktree **worktrees = get_worktrees(the_repository);

	اگر (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		اگر (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		ورنہ {
			صحیح رمز = die_پیغام(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			باہر_نکلو(رمز);
		}
	}

	برائے (صحیح i = 0; worktrees[i]; i++) {
		ساخت worktree *wt = worktrees[i];

		اگر (wt->سر_ref && !strcmp(oldref.buf, wt->سر_ref)) {
			oldref_usage |= IS_سر;
			اگر (is_null_oid(&wt->سر_oid))
				oldref_usage |= IS_ORPHAN;
			توڑو;
		}
	}

	اگر ((copy || !(oldref_usage & IS_سر)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		اگر (oldref_usage & IS_سر)
			die(_("no commit on branch '%s' yet"), oldname);
		ورنہ
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	اگر (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	ورنہ
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	اگر (!skip_سابقہ(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_سابقہ(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	اگر (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	ورنہ
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	اگر (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	اگر (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	اگر (recovery) {
		اگر (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		ورنہ
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	اگر (!copy && (oldref_usage & IS_سر) &&
	    replace_each_worktree_سر_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	اگر (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	اگر (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

ساکن صحیح parse_opt_forked(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت ref_filter *filter = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	اگر (ref_filter_forked_جوڑو(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	واپس 0;
}

ساخت stacked_branch_ڈیٹا {
	ساخت strset *deletable_branch_نام;
	ساخت strset *protected_branch_نام;
};

ساکن صحیح collect_stacked_branch_بنیاد(ثابت ساخت reference *ref,
				       خالی *cb_ڈیٹا)
{
	ساخت stacked_branch_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ثابت حرف *branch_name;
	ساخت branch *branch;
	ثابت حرف *upstream_refname;
	ثابت حرف *upstream_branch_name;

	اگر (!skip_سابقہ(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	اگر (strset_شامل_ہے(ڈیٹا->deletable_branch_نام, branch_name))
		واپس 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, تہی);
	اگر (!upstream_refname ||
	    !skip_سابقہ(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_شامل_ہے(ڈیٹا->deletable_branch_نام,
			    upstream_branch_name))
		واپس 0;

	strset_جوڑو(ڈیٹا->protected_branch_نام, upstream_branch_name);
	واپس 0;
}

ساکن خالی protect_stacked_branch_bases(ساخت ref_store *refs,
					 ساخت strset *deletable_branch_نام,
					 ساخت strset *protected_branch_نام)
{
	ساخت stacked_branch_ڈیٹا ڈیٹا = {
		.deletable_branch_نام = deletable_branch_نام,
		.protected_branch_نام = protected_branch_نام,
	};
	ساخت refs_for_each_ref_options opts = {
		.سابقہ = "refs/heads/",
	};
	ساخت hashmap_iter iter;
	ساخت strmap_اندراج *اندراج_2;

	refs_for_each_ref_ext(refs, collect_stacked_branch_بنیاد, &ڈیٹا, &opts);

	strset_for_each_اندراج(protected_branch_نام, &iter, اندراج_2)
		strset_remove(deletable_branch_نام, اندراج_2->کلید);
}

ساکن خالی clear_deleted_upstreams(ساخت strset *protected_branch_نام,
				    ساخت strset *deletable_branch_نام)
{
	ساخت strbuf کلید = STRBUF_INIT;
	ساخت hashmap_iter iter;
	ساخت strmap_اندراج *اندراج_2;

	strset_for_each_اندراج(protected_branch_نام, &iter, اندراج_2) {
		ساخت branch *branch = branch_get(اندراج_2->کلید);
		ثابت حرف *upstream_refname = branch_get_upstream(branch, تہی);
		ثابت حرف *upstream_branch_name;

		اگر (!upstream_refname ||
		    !skip_سابقہ(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_شامل_ہے(deletable_branch_نام,
				    upstream_branch_name))
			جاری;

		strbuf_addf(&کلید, "branch.%s.merge", branch->name);
		repo_config_سیٹ_gently(the_repository, کلید.buf, تہی);
		strbuf_reset(&کلید);
		strbuf_addf(&کلید, "branch.%s.remote", branch->name);
		repo_config_سیٹ_gently(the_repository, کلید.buf, تہی);
		strbuf_reset(&کلید);
	}

	strbuf_release(&کلید);
}

ساکن صحیح branch_pushes_مقصد_upstream(ساخت branch *branch,
				     ثابت حرف *upstream)
{
	ساخت remote *remote = remote_get(remote_for_branch(branch, تہی));
	حرف *ڈالو_refname = تہی;
	حرف *tracking = تہی;
	صحیح ret = 0;

	اگر (!remote)
		واپس 0;
	اگر (remote->ڈالو.nr)
		ڈالو_refname = apply_refspecs(&remote->ڈالو, branch->refname);
	ورنہ
		ڈالو_refname = xstrdup(branch->refname);
	اگر (ڈالو_refname)
		tracking = apply_refspecs(&remote->fetch, ڈالو_refname);
	اگر (tracking && !strcmp(tracking, upstream))
		ret = 1;

	آزاد(ڈالو_refname);
	آزاد(tracking);
	واپس ret;
}

ساکن صحیح delete_merged_branches(ثابت ساخت strvec *upstreams,
				 ثابت حرف **argv, غیرعلامتی صحیح flags)
{
	ساخت ref_store *refs = get_main_ref_store(the_repository);
	ساخت ref_filter filter = REF_FILTER_INIT;
	ساخت ref_array candidates = { 0 };
	ساخت strset deletable_branch_نام = STRSET_INIT;
	ساخت strset protected_branch_نام = STRSET_INIT;
	ساخت strvec branches_مقصد_delete = STRVEC_INIT;
	ساخت strbuf کلید = STRBUF_INIT;
	ساخت hashmap_iter iter;
	ساخت strmap_اندراج *اندراج_2;
	صحیح ret = 0;

	برائے (سائز_t i = 0; i < upstreams->nr; i++)
		اگر (ref_filter_forked_جوڑو(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.قسم = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.قسم);

	برائے (صحیح i = 0; i < candidates.nr; i++) {
		ثابت حرف *branch_refname = candidates.اشیاء[i]->refname;
		ثابت حرف *branch_name;
		ساخت branch *branch;
		ثابت حرف *upstream_refname;
		صحیح opt_برآمد;

		اگر (!skip_سابقہ(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		اگر (branch_checked_برآمد(branch_refname))
			جاری;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, تہی);
		اگر (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			جاری;
		اگر (branch_pushes_مقصد_upstream(branch, upstream_refname))
			جاری;
		اگر (check_branch_commit(branch_name, branch_name,
					&candidates.اشیاء[i]->objectname, تہی,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			جاری;

		strbuf_reset(&کلید);
		strbuf_addf(&کلید, "branch.%s.deletemerged", branch_name);
		اگر (!repo_config_get_bool(the_repository, کلید.buf, &opt_برآمد) &&
		    !opt_برآمد) {
			اگر (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			جاری;
		}

		strset_جوڑو(&deletable_branch_نام, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_نام,
				     &protected_branch_نام);

	strset_for_each_اندراج(&deletable_branch_نام, &iter, اندراج_2)
		strvec_ڈالو(&branches_مقصد_delete, اندراج_2->کلید);

	اگر (branches_مقصد_delete.nr)
		ret = delete_branches(branches_مقصد_delete.nr, branches_مقصد_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_سر_FALLBACK |
				      flags);

	اگر (!ret && !(flags & DELETE_BRANCH_DRY_تسلسل))
		clear_deleted_upstreams(&protected_branch_نام,
					&deletable_branch_نام);

	strbuf_release(&کلید);
	strvec_clear(&branches_مقصد_delete);
	strset_clear(&protected_branch_نام);
	strset_clear(&deletable_branch_نام);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	واپس ret;
}

ساکن GIT_راستہ_FUNC(edit_description, "EDIT_DESCRIPTION")

ساکن صحیح edit_branch_description(ثابت حرف *branch_name)
{
	صحیح exists;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	اگر (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_سطر_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_سطر_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	اگر (launch_editor(edit_description(), &buf, تہی)) {
		strbuf_release(&buf);
		واپس -1;
	}
	strbuf_stripspace(&buf, comment_سطر_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	اگر (buf.len || exists)
		repo_config_سیٹ(the_repository, name.buf, buf.len ? buf.buf : تہی);
	strbuf_release(&name);
	strbuf_release(&buf);

	واپس 0;
}

ساکن خالی die_if_upstream_looks_like_remote(ثابت حرف *new_upstream, ثابت حرف *branch_name)
{
	ساخت strbuf remote_ref = STRBUF_INIT;
	صحیح رمز;

	اگر (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		واپس;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	اگر (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		واپس;
	}

	رمز = die_پیغام(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_سیٹ_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	باہر_نکلو(رمز);
}

صحیح cmd_branch(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	/* possible actions */
	صحیح delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	ساخت strvec delete_merged = STRVEC_INIT;
	صحیح dry_تسلسل = 0;
	ثابت حرف *new_upstream = تہی;
	صحیح noncreate_actions = 0;
	/* possible options */
	صحیح reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	شمار branch_track track;
	ساخت ref_filter filter = REF_FILTER_INIT;
	ساکن ساخت ref_ترتیب_کاری *ترتیب_کاری;
	ساخت string_list ترتیب_کاری_options = STRING_LIST_INIT_DUP;
	ساخت ref_format format = REF_FORMAT_INIT;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);
	صحیح ret;

	ساخت option options[] = {
		OPT_گروہ(N_("Generic options")),
		OPT__VERBOSE(&filter.verbose,
			N_("show hash and subject, give twice for upstream branch")),
		OPT__QUIET(&quiet, N_("suppress informational messages")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_منوال),
		OPT_سیٹ_INT_F(0, "set-upstream", &track, N_("do not use"),
			BRANCH_TRACK_OVERRIDE, PARSE_OPT_HIDDEN),
		OPT_STRING('u', "set-upstream-to", &new_upstream, N_("upstream"), N_("change the upstream info")),
		OPT_BOOL(0, "unset-upstream", &unset_upstream, N_("unset the upstream info")),
		OPT__COLOR(&branch_use_color, N_("use colored output")),
		OPT_سیٹ_INT_F('r', "remotes",     &filter.قسم, N_("act on remote-tracking branches"),
			      FILTER_REFS_REMOTES,
			      PARSE_OPT_NONEG),
		OPT_شامل_ہے(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_NO_شامل_ہے(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT__ABBREV(&filter.abbrev),

		OPT_گروہ(N_("Specific git-branch actions:")),
		OPT_سیٹ_INT_F('a', "all", &filter.قسم, N_("list both remote-tracking and local branches"),
			      FILTER_REFS_REMOTES | FILTER_REFS_BRANCHES,
			      PARSE_OPT_NONEG),
		OPT_ثنائی_ہندسہ('d', "delete", &delete, N_("delete fully merged branch"), 1),
		OPT_ثنائی_ہندسہ('D', تہی, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_ثنائی_ہندسہ('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_ثنائی_ہندسہ('M', تہی, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالی_خانے,
			N_("do not output a newline after empty formatted refs")),
		OPT_ثنائی_ہندسہ('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_ثنائی_ہندسہ('C', تہی, &copy, N_("copy a branch, even if target exists"), 2),
		OPT_BOOL('l', "list", &list, N_("list branch names")),
		OPT_BOOL(0, "show-current", &show_current, N_("show current branch name")),
		OPT_BOOL(0, "create-reflog", &reflog, N_("create the branch's reflog")),
		OPT_BOOL(0, "edit-description", &edit_description,
			 N_("edit the description for the branch")),
		OPT_CALLBACK_F(0, "delete-merged", &delete_merged, N_("pattern"),
			N_("delete merged branches whose upstream matches <pattern> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_strvec),
		OPT_BOOL(0, "dry-run", &dry_تسلسل,
			N_("with --delete-merged, only print which branches would be deleted")),
		OPT__FORCE(&force, N_("force creation, move/rename, deletion"), PARSE_OPT_NOCOMPLETE),
		OPT_MERGED(&filter, N_("print only branches that are merged")),
		OPT_NO_MERGED(&filter, N_("print only branches that are not merged")),
		OPT_CALLBACK_F(0, "forked", &filter, N_("branch"),
			N_("print only branches whose upstream matches <branch> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_forked),
		OPT_عمود(0, "column", &colopts, N_("list branches in columns")),
		OPT_REF_ترتیب_دو(&ترتیب_کاری_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at, N_("object"),
			N_("print only branches of the object"), parse_opt_object_name),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules_explicit, N_("recurse through submodules")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_انتہا(),
	};

	تیاری_ref_filter_porcelain_پیغام();

	filter.قسم = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &ترتیب_کاری_options);
	اگر (!ترتیب_کاری_options.nr)
		string_list_append(&ترتیب_کاری_options, "refname");

	track = cfg->branch_track;

	سر = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &سر_oid, تہی);
	اگر (!سر)
		die(_("failed to resolve HEAD as a valid ref"));
	اگر (!strcmp(سر, "HEAD"))
		filter.detached = 1;
	ورنہ اگر (!skip_سابقہ(سر, "refs/heads/", &سر))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, سابقہ, options, builtin_branch_usage,
			     0);

	اگر (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	اگر (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	اگر (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	اگر (dry_تسلسل && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	اگر (recurse_submodules_explicit) {
		اگر (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		اگر (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	اگر (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	اگر (filter.verbose) {
		اگر (explicitly_enable_عمود(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	اگر (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	اگر (list)
		تیاری_auto_pager("branch", 1);

	اگر (delete) {
		اگر (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.قسم,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		جاؤ برآمد;
	} ورنہ اگر (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_تسلسل ? DELETE_BRANCH_DRY_تسلسل : 0));
		جاؤ برآمد;
	} ورنہ اگر (show_current) {
		print_current_branch_name();
		ret = 0;
		جاؤ برآمد;
	} ورنہ اگر (list) {
		/*  git branch --list also shows HEAD when it is detached */
		اگر ((filter.قسم & FILTER_REFS_BRANCHES) && filter.detached)
			filter.قسم |= FILTER_REFS_DETACHED_سر;
		filter.name_patterns = argv;
		/*
		 * If no sorting parameter is given then we default to sorting
		 * by 'refname'. This would give us an alphabetically sorted
		 * array with the 'HEAD' ref at the beginning followed by
		 * local branches 'refs/heads/...' and finally remote-tracking
		 * branches 'refs/remotes/...'.
		 */
		ترتیب_کاری = ref_ترتیب_کاری_options(&ترتیب_کاری_options);
		ref_ترتیب_کاری_سیٹ_ترتیب_دو_flags_all(ترتیب_کاری, REF_ترتیب_کاری_ICASE, icase);
		ref_ترتیب_کاری_سیٹ_ترتیب_دو_flags_all(
			ترتیب_کاری, REF_ترتیب_کاری_DETACHED_سر_پہلا, 1);
		print_ref_list(&filter, ترتیب_کاری, &format, &output);
		print_columns(&output, colopts, تہی);
		string_list_clear(&output, 0);
		ref_ترتیب_کاری_release(ترتیب_کاری);
		ref_filter_clear(&filter);

		ret = 0;
		جاؤ برآمد;
	} ورنہ اگر (edit_description) {
		ثابت حرف *branch_name;
		ساخت strbuf branch_ref = STRBUF_INIT;
		ساخت strbuf buf = STRBUF_INIT;

		اگر (!argc) {
			اگر (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = سر;
		} ورنہ اگر (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} ورنہ {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		اگر (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_برآمد(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} ورنہ اگر (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} ورنہ {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		جاؤ برآمد;
	} ورنہ اگر (copy || rename) {
		اگر (!argc)
			die(_("branch name required"));
		ورنہ اگر ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		ورنہ اگر (argc == 1)
			copy_or_rename_branch(سر, argv[0], copy, copy + rename > 1);
		ورنہ اگر (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		ورنہ
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} ورنہ اگر (new_upstream) {
		ساخت branch *branch;
		ساخت strbuf buf = STRBUF_INIT;

		اگر (!argc)
			branch = branch_get(تہی);
		ورنہ اگر (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ورنہ
			die(_("too many arguments to set new upstream"));

		اگر (!branch) {
			اگر (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		اگر (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			اگر (!argc || branch_checked_برآمد(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			اگر (argc == 1 &&
			    advice_enabled(ADVICE_سیٹ_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_تیاری_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} ورنہ اگر (unset_upstream) {
		ساخت branch *branch;
		ساخت strbuf buf = STRBUF_INIT;

		اگر (!argc)
			branch = branch_get(تہی);
		ورنہ اگر (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ورنہ
			die(_("too many arguments to unset upstream"));

		اگر (!branch) {
			اگر (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		اگر (!branch_has_ملاؤ_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_سیٹ_multivar(the_repository, buf.buf, تہی, تہی, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_سیٹ_multivar(the_repository, buf.buf, تہی, تہی, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} ورنہ اگر (!noncreate_actions && argc > 0 && argc <= 2) {
		ثابت حرف *branch_name = argv[0];
		ثابت حرف *آغاز_name = argc == 2 ? argv[1] : سر;

		اگر (filter.قسم != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		اگر (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		اگر (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    آغاز_name, تہی, force,
						    reflog, quiet, track, 0);
			ret = 0;
			جاؤ برآمد;
		}
		create_branch(the_repository, branch_name, آغاز_name, force, 0,
			      reflog, quiet, track, 0);
	} ورنہ
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

برآمد:
	strvec_clear(&delete_merged);
	string_list_clear(&ترتیب_کاری_options, 0);
	واپس ret;
}
