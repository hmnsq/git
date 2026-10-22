/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "strmap.h"
#অন্তর্ভাব "column.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "ref-filter.h"
#অন্তর্ভাব "worktree.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "commit-reach.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_branch_usage[] = {
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
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *শীর্ষ;
স্থির কাঠামো object_id শীর্ষ_oid;
স্থির পূর্ণসংখ্যা recurse_submodules = 0;
স্থির পূর্ণসংখ্যা submodule_propagate_branches = 0;

স্থির গণনা git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
স্থির অক্ষর branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
গণনা color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

স্থির ধ্রুবক অক্ষর *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

স্থির কাঠামো string_list output = STRING_LIST_INIT_DUP;
স্থির unsigned পূর্ণসংখ্যা colopts;

define_list_config_array(color_branch_slots);

স্থির পূর্ণসংখ্যা git_branch_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			     ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	ধ্রুবক অক্ষর *slot_name;

	যদি (!strcmp(var, "branch.sort")) {
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		string_list_append(cb, মান);
		ফেরত 0;
	}

	যদি (starts_with(var, "column."))
		ফেরত git_column_config(var, মান, "branch", &colopts);
	যদি (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, মান);
		ফেরত 0;
	}
	যদি (skip_prefix(var, "color.branch.", &slot_name)) {
		পূর্ণসংখ্যা slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		যদি (slot < 0)
			ফেরত 0;
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		ফেরত color_parse(মান, branch_colors[slot]);
	}
	যদি (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, মান);
		ফেরত 0;
	}
	যদি (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (git_color_config(var, মান, cb) < 0)
		ফেরত -1;

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির ধ্রুবক অক্ষর *branch_get_color(গণনা color_branch ix)
{
	যদি (want_color(branch_use_color))
		ফেরত branch_colors[ix];
	ফেরত "";
}

স্থির পূর্ণসংখ্যা branch_merged(পূর্ণসংখ্যা kind, ধ্রুবক অক্ষর *name,
			 কাঠামো commit *rev, কাঠামো commit *শীর্ষ_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	কাঠামো commit *reference_rev = ফাঁকা;
	ধ্রুবক অক্ষর *reference_name = ফাঁকা;
	শূন্য *reference_name_to_free = ফাঁকা;
	পূর্ণসংখ্যা merged;

	যদি (kind == FILTER_REFS_BRANCHES) {
		কাঠামো branch *branch = branch_get(name);
		ধ্রুবক অক্ষর *upstream = branch_get_upstream(branch, ফাঁকা);
		কাঠামো object_id oid;

		যদি (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ফাঁকা)) != ফাঁকা)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	যদি (!reference_rev)
		reference_rev = শীর্ষ_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	যদি (merged < 0)
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
	যদি (শীর্ষ_rev && শীর্ষ_rev != reference_rev) {
		পূর্ণসংখ্যা expect = repo_in_merge_bases(the_repository, rev, শীর্ষ_rev);
		যদি (expect < 0)
			exit(128);
		যদি (expect == merged)
			; /* okay */
		অন্যথা যদি (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		অন্যথা
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	মুক্ত(reference_name_to_free);
	ফেরত merged;
}

গণনা delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_শীর্ষ_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

স্থির পূর্ণসংখ্যা check_branch_commit(ধ্রুবক অক্ষর *branchname, ধ্রুবক অক্ষর *refname,
			       ধ্রুবক কাঠামো object_id *oid, কাঠামো commit *শীর্ষ_rev,
			       পূর্ণসংখ্যা kinds, unsigned পূর্ণসংখ্যা flags)
{
	কাঠামো commit *rev = lookup_commit_reference(the_repository, oid);
	যদি (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ফেরত -1;
	}
	যদি (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, শীর্ষ_rev)) {
		যদি (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ফেরত -1;
	}
	ফেরত 0;
}

স্থির শূন্য delete_branch_config(ধ্রুবক অক্ষর *branchname)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	যদি (repo_config_rename_section(the_repository, buf.buf, ফাঁকা) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

স্থির পূর্ণসংখ্যা delete_branches(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, পূর্ণসংখ্যা kinds,
			   unsigned পূর্ণসংখ্যা flags)
{
	কাঠামো commit *শীর্ষ_rev = ফাঁকা;
	কাঠামো object_id oid;
	অক্ষর *name = ফাঁকা;
	ধ্রুবক অক্ষর *fmt;
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা ret = 0;
	পূর্ণসংখ্যা remote_branch = 0;
	কাঠামো strbuf bname = STRBUF_INIT;
	গণনা interpret_branch_kind allowed_interpret;
	কাঠামো string_list refs_to_delete = STRING_LIST_INIT_DUP;
	কাঠামো string_list_item *item;
	পূর্ণসংখ্যা branch_name_pos;
	ধ্রুবক অক্ষর *fmt_remotes = "refs/remotes/%s";

	নির্বাচন (kinds) {
	ক্ষেত্র FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		বিরতি;
	ক্ষেত্র FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		বিরতি;
	পূর্বনির্ধারিত:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	যদি (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_শীর্ষ_FALLBACK))
		শীর্ষ_rev = lookup_commit_reference(the_repository, &শীর্ষ_oid);

	জন্য (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		অক্ষর *target = ফাঁকা;
		পূর্ণসংখ্যা ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		মুক্ত(name);
		name = mkpathdup(fmt, bname.buf);

		যদি (kinds == FILTER_REFS_BRANCHES) {
			ধ্রুবক অক্ষর *path;
			যদি ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				চালু;
			}
			যদি ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				চালু;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		যদি (!target) {
			যদি (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} অন্যথা {
				অক্ষর *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				অক্ষর *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				যদি (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				অন্যথা
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			চালু;
		}

		যদি (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, শীর্ষ_rev, kinds,
					flags)) {
			যদি (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			যাও পরবর্তী;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	পরবর্তী:
		মুক্ত(target);
	}

	যদি (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ফাঁকা, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		অক্ষর *describe_ref = item->util;
		অক্ষর *name = item->string;
		যদি (flags & DELETE_BRANCH_DRY_RUN) {
			যদি (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} অন্যথা যদি (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			অক্ষর *refname = name + branch_name_pos;
			যদি (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		মুক্ত(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	মুক্ত(name);
	strbuf_release(&bname);

	ফেরত ret;
}

স্থির পূর্ণসংখ্যা calc_maxwidth(কাঠামো ref_array *refs, পূর্ণসংখ্যা remote_bonus)
{
	পূর্ণসংখ্যা i, max = 0;
	জন্য (i = 0; i < refs->nr; i++) {
		কাঠামো ref_array_item *it = refs->বস্তুসমূহ[i];
		ধ্রুবক অক্ষর *desc = it->refname;
		পূর্ণসংখ্যা w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		যদি (it->kind == FILTER_REFS_DETACHED_শীর্ষ) {
			অক্ষর *শীর্ষ_desc = get_শীর্ষ_description();
			w = utf8_strwidth(শীর্ষ_desc);
			মুক্ত(শীর্ষ_desc);
		} অন্যথা
			w = utf8_strwidth(desc);

		যদি (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		যদি (w > max)
			max = w;
	}
	ফেরত max;
}

স্থির ধ্রুবক অক্ষর *quote_literal_for_format(ধ্রুবক অক্ষর *s)
{
	স্থির কাঠামো strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	যখন (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ফেরত buf.buf;
}

স্থির অক্ষর *build_format(কাঠামো ref_filter *filter, পূর্ণসংখ্যা maxwidth, ধ্রুবক অক্ষর *remote_prefix)
{
	কাঠামো strbuf fmt = STRBUF_INIT;
	কাঠামো strbuf local = STRBUF_INIT;
	কাঠামো strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	যদি (filter->verbose) {
		কাঠামো strbuf obname = STRBUF_INIT;

		যদি (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		অন্যথা যদি (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		অন্যথা
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		যদি (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		অন্যথা
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} অন্যথা {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	ফেরত strbuf_detach(&fmt, ফাঁকা);
}

স্থির শূন্য print_ref_list(কাঠামো ref_filter *filter, কাঠামো ref_sorting *sorting,
			   কাঠামো ref_format *format, কাঠামো string_list *output)
{
	পূর্ণসংখ্যা i;
	কাঠামো ref_array array;
	পূর্ণসংখ্যা maxwidth = 0;
	ধ্রুবক অক্ষর *remote_prefix = "";
	অক্ষর *to_free = ফাঁকা;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	যদি (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, আকার(array));

	filter_refs(&array, filter, filter->kind);

	যদি (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	যদি (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	যদি (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	যদি (column_active(colopts)) {
		কাঠামো strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		প্রতিজ্ঞা(!filter->verbose && "--column and --verbose are incompatible");

		জন্য (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			যদি (format_ref_array_item(array.বস্তুসমূহ[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} অন্যথা {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	মুক্ত(to_free);
}

স্থির শূন্য print_current_branch_name(শূন্য)
{
	পূর্ণসংখ্যা flags;
	ধ্রুবক অক্ষর *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ফাঁকা, &flags);
	ধ্রুবক অক্ষর *shortname;
	যদি (!refname)
		die(_("could not resolve HEAD"));
	অন্যথা যদি (!(flags & REF_ISSYMREF))
		ফেরত;
	অন্যথা যদি (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	অন্যথা
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

স্থির শূন্য reject_rebase_or_bisect_branch(কাঠামো worktree **worktrees,
					   ধ্রুবক অক্ষর *target)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; worktrees[i]; i++) {
		কাঠামো worktree *wt = worktrees[i];

		যদি (!wt->is_detached)
			চালু;

		যদি (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		যদি (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
স্থির পূর্ণসংখ্যা replace_each_worktree_শীর্ষ_symref(কাঠামো worktree **worktrees,
					     ধ্রুবক অক্ষর *oldref, ধ্রুবক অক্ষর *newref,
					     ধ্রুবক অক্ষর *logmsg)
{
	পূর্ণসংখ্যা ret = 0;
	পূর্ণসংখ্যা i;

	জন্য (i = 0; worktrees[i]; i++) {
		কাঠামো ref_store *refs;

		যদি (worktrees[i]->is_detached)
			চালু;
		যদি (!worktrees[i]->শীর্ষ_ref)
			চালু;
		যদি (strcmp(oldref, worktrees[i]->শীর্ষ_ref))
			চালু;

		refs = get_worktree_ref_store(worktrees[i]);
		যদি (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	ফেরত ret;
}

#define IS_শীর্ষ 1
#define IS_ORPHAN 2

স্থির শূন্য copy_or_rename_branch(ধ্রুবক অক্ষর *oldname, ধ্রুবক অক্ষর *newname, পূর্ণসংখ্যা copy, পূর্ণসংখ্যা force)
{
	কাঠামো strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	কাঠামো strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ধ্রুবক অক্ষর *interpreted_oldname = ফাঁকা;
	ধ্রুবক অক্ষর *interpreted_newname = ফাঁকা;
	পূর্ণসংখ্যা recovery = 0, oldref_usage = 0;
	কাঠামো worktree **worktrees = get_worktrees(the_repository);

	যদি (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		যদি (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		অন্যথা {
			পূর্ণসংখ্যা code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	জন্য (পূর্ণসংখ্যা i = 0; worktrees[i]; i++) {
		কাঠামো worktree *wt = worktrees[i];

		যদি (wt->শীর্ষ_ref && !strcmp(oldref.buf, wt->শীর্ষ_ref)) {
			oldref_usage |= IS_শীর্ষ;
			যদি (is_null_oid(&wt->শীর্ষ_oid))
				oldref_usage |= IS_ORPHAN;
			বিরতি;
		}
	}

	যদি ((copy || !(oldref_usage & IS_শীর্ষ)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		যদি (oldref_usage & IS_শীর্ষ)
			die(_("no commit on branch '%s' yet"), oldname);
		অন্যথা
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	যদি (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	অন্যথা
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	যদি (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	যদি (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	অন্যথা
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	যদি (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	যদি (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	যদি (recovery) {
		যদি (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		অন্যথা
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	যদি (!copy && (oldref_usage & IS_শীর্ষ) &&
	    replace_each_worktree_শীর্ষ_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	যদি (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	যদি (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

স্থির পূর্ণসংখ্যা parse_opt_forked(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো ref_filter *filter = opt->মান;

	BUG_ON_OPT_NEG(unset);
	যদি (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ফেরত 0;
}

কাঠামো stacked_branch_তথ্য {
	কাঠামো strset *deletable_branch_names;
	কাঠামো strset *protected_branch_names;
};

স্থির পূর্ণসংখ্যা collect_stacked_branch_base(ধ্রুবক কাঠামো reference *ref,
				       শূন্য *cb_তথ্য)
{
	কাঠামো stacked_branch_তথ্য *তথ্য = cb_তথ্য;
	ধ্রুবক অক্ষর *branch_name;
	কাঠামো branch *branch;
	ধ্রুবক অক্ষর *upstream_refname;
	ধ্রুবক অক্ষর *upstream_branch_name;

	যদি (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	যদি (strset_contains(তথ্য->deletable_branch_names, branch_name))
		ফেরত 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ফাঁকা);
	যদি (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(তথ্য->deletable_branch_names,
			    upstream_branch_name))
		ফেরত 0;

	strset_add(তথ্য->protected_branch_names, upstream_branch_name);
	ফেরত 0;
}

স্থির শূন্য protect_stacked_branch_bases(কাঠামো ref_store *refs,
					 কাঠামো strset *deletable_branch_names,
					 কাঠামো strset *protected_branch_names)
{
	কাঠামো stacked_branch_তথ্য তথ্য = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	কাঠামো refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	কাঠামো hashmap_iter iter;
	কাঠামো strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &তথ্য, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->চাবি);
}

স্থির শূন্য clear_deleted_upstreams(কাঠামো strset *protected_branch_names,
				    কাঠামো strset *deletable_branch_names)
{
	কাঠামো strbuf চাবি = STRBUF_INIT;
	কাঠামো hashmap_iter iter;
	কাঠামো strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		কাঠামো branch *branch = branch_get(entry->চাবি);
		ধ্রুবক অক্ষর *upstream_refname = branch_get_upstream(branch, ফাঁকা);
		ধ্রুবক অক্ষর *upstream_branch_name;

		যদি (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			চালু;

		strbuf_addf(&চাবি, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, চাবি.buf, ফাঁকা);
		strbuf_reset(&চাবি);
		strbuf_addf(&চাবি, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, চাবি.buf, ফাঁকা);
		strbuf_reset(&চাবি);
	}

	strbuf_release(&চাবি);
}

স্থির পূর্ণসংখ্যা branch_pushes_to_upstream(কাঠামো branch *branch,
				     ধ্রুবক অক্ষর *upstream)
{
	কাঠামো remote *remote = remote_get(remote_for_branch(branch, ফাঁকা));
	অক্ষর *ঠেলো_refname = ফাঁকা;
	অক্ষর *tracking = ফাঁকা;
	পূর্ণসংখ্যা ret = 0;

	যদি (!remote)
		ফেরত 0;
	যদি (remote->ঠেলো.nr)
		ঠেলো_refname = apply_refspecs(&remote->ঠেলো, branch->refname);
	অন্যথা
		ঠেলো_refname = xstrdup(branch->refname);
	যদি (ঠেলো_refname)
		tracking = apply_refspecs(&remote->fetch, ঠেলো_refname);
	যদি (tracking && !strcmp(tracking, upstream))
		ret = 1;

	মুক্ত(ঠেলো_refname);
	মুক্ত(tracking);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা delete_merged_branches(ধ্রুবক কাঠামো strvec *upstreams,
				 ধ্রুবক অক্ষর **argv, unsigned পূর্ণসংখ্যা flags)
{
	কাঠামো ref_store *refs = get_main_ref_store(the_repository);
	কাঠামো ref_filter filter = REF_FILTER_INIT;
	কাঠামো ref_array candidates = { 0 };
	কাঠামো strset deletable_branch_names = STRSET_INIT;
	কাঠামো strset protected_branch_names = STRSET_INIT;
	কাঠামো strvec branches_to_delete = STRVEC_INIT;
	কাঠামো strbuf চাবি = STRBUF_INIT;
	কাঠামো hashmap_iter iter;
	কাঠামো strmap_entry *entry;
	পূর্ণসংখ্যা ret = 0;

	জন্য (আকার_t i = 0; i < upstreams->nr; i++)
		যদি (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	জন্য (পূর্ণসংখ্যা i = 0; i < candidates.nr; i++) {
		ধ্রুবক অক্ষর *branch_refname = candidates.বস্তুসমূহ[i]->refname;
		ধ্রুবক অক্ষর *branch_name;
		কাঠামো branch *branch;
		ধ্রুবক অক্ষর *upstream_refname;
		পূর্ণসংখ্যা opt_out;

		যদি (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		যদি (branch_checked_out(branch_refname))
			চালু;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ফাঁকা);
		যদি (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			চালু;
		যদি (branch_pushes_to_upstream(branch, upstream_refname))
			চালু;
		যদি (check_branch_commit(branch_name, branch_name,
					&candidates.বস্তুসমূহ[i]->objectname, ফাঁকা,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			চালু;

		strbuf_reset(&চাবি);
		strbuf_addf(&চাবি, "branch.%s.deletemerged", branch_name);
		যদি (!repo_config_get_bool(the_repository, চাবি.buf, &opt_out) &&
		    !opt_out) {
			যদি (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			চালু;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_ঠেলো(&branches_to_delete, entry->চাবি);

	যদি (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_শীর্ষ_FALLBACK |
				      flags);

	যদি (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&চাবি);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	ফেরত ret;
}

স্থির GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

স্থির পূর্ণসংখ্যা edit_branch_description(ধ্রুবক অক্ষর *branch_name)
{
	পূর্ণসংখ্যা exists;
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	যদি (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_সারি_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_সারি_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	যদি (launch_editor(edit_description(), &buf, ফাঁকা)) {
		strbuf_release(&buf);
		ফেরত -1;
	}
	strbuf_stripspace(&buf, comment_সারি_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	যদি (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : ফাঁকা);
	strbuf_release(&name);
	strbuf_release(&buf);

	ফেরত 0;
}

স্থির শূন্য die_if_upstream_looks_like_remote(ধ্রুবক অক্ষর *new_upstream, ধ্রুবক অক্ষর *branch_name)
{
	কাঠামো strbuf remote_ref = STRBUF_INIT;
	পূর্ণসংখ্যা code;

	যদি (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ফেরত;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	যদি (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ফেরত;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

পূর্ণসংখ্যা cmd_branch(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo UNUSED)
{
	/* possible actions */
	পূর্ণসংখ্যা delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	কাঠামো strvec delete_merged = STRVEC_INIT;
	পূর্ণসংখ্যা dry_run = 0;
	ধ্রুবক অক্ষর *new_upstream = ফাঁকা;
	পূর্ণসংখ্যা noncreate_actions = 0;
	/* possible options */
	পূর্ণসংখ্যা reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	গণনা branch_track track;
	কাঠামো ref_filter filter = REF_FILTER_INIT;
	স্থির কাঠামো ref_sorting *sorting;
	কাঠামো string_list sorting_options = STRING_LIST_INIT_DUP;
	কাঠামো ref_format format = REF_FORMAT_INIT;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণসংখ্যা ret;

	কাঠামো option options[] = {
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
		OPT_BIT('D', ফাঁকা, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', ফাঁকা, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', ফাঁকা, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_বার্তা();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	যদি (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	শীর্ষ = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &শীর্ষ_oid, ফাঁকা);
	যদি (!শীর্ষ)
		die(_("failed to resolve HEAD as a valid ref"));
	যদি (!strcmp(শীর্ষ, "HEAD"))
		filter.detached = 1;
	অন্যথা যদি (!skip_prefix(শীর্ষ, "refs/heads/", &শীর্ষ))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	যদি (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	যদি (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	যদি (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	যদি (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	যদি (recurse_submodules_explicit) {
		যদি (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		যদি (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	যদি (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	যদি (filter.verbose) {
		যদি (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	যদি (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	যদি (list)
		setup_auto_pager("branch", 1);

	যদি (delete) {
		যদি (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		যাও out;
	} অন্যথা যদি (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		যাও out;
	} অন্যথা যদি (show_current) {
		print_current_branch_name();
		ret = 0;
		যাও out;
	} অন্যথা যদি (list) {
		/*  git branch --list also shows HEAD when it is detached */
		যদি ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
			filter.kind |= FILTER_REFS_DETACHED_শীর্ষ;
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
			sorting, REF_SORTING_DETACHED_শীর্ষ_FIRST, 1);
		print_ref_list(&filter, sorting, &format, &output);
		print_columns(&output, colopts, ফাঁকা);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		যাও out;
	} অন্যথা যদি (edit_description) {
		ধ্রুবক অক্ষর *branch_name;
		কাঠামো strbuf branch_ref = STRBUF_INIT;
		কাঠামো strbuf buf = STRBUF_INIT;

		যদি (!argc) {
			যদি (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = শীর্ষ;
		} অন্যথা যদি (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} অন্যথা {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		যদি (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} অন্যথা যদি (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} অন্যথা {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		যাও out;
	} অন্যথা যদি (copy || rename) {
		যদি (!argc)
			die(_("branch name required"));
		অন্যথা যদি ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		অন্যথা যদি (argc == 1)
			copy_or_rename_branch(শীর্ষ, argv[0], copy, copy + rename > 1);
		অন্যথা যদি (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		অন্যথা
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} অন্যথা যদি (new_upstream) {
		কাঠামো branch *branch;
		কাঠামো strbuf buf = STRBUF_INIT;

		যদি (!argc)
			branch = branch_get(ফাঁকা);
		অন্যথা যদি (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} অন্যথা
			die(_("too many arguments to set new upstream"));

		যদি (!branch) {
			যদি (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		যদি (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			যদি (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			যদি (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} অন্যথা যদি (unset_upstream) {
		কাঠামো branch *branch;
		কাঠামো strbuf buf = STRBUF_INIT;

		যদি (!argc)
			branch = branch_get(ফাঁকা);
		অন্যথা যদি (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} অন্যথা
			die(_("too many arguments to unset upstream"));

		যদি (!branch) {
			যদি (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		যদি (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ফাঁকা, ফাঁকা, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ফাঁকা, ফাঁকা, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} অন্যথা যদি (!noncreate_actions && argc > 0 && argc <= 2) {
		ধ্রুবক অক্ষর *branch_name = argv[0];
		ধ্রুবক অক্ষর *start_name = argc == 2 ? argv[1] : শীর্ষ;

		যদি (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		যদি (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		যদি (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, ফাঁকা, force,
						    reflog, quiet, track, 0);
			ret = 0;
			যাও out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} অন্যথা
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	ফেরত ret;
}
