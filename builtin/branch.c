/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "color.h"
#অন্তর্ভাৱ "editor.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "remote.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "branch.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "strmap.h"
#অন্তর্ভাৱ "column.h"
#অন্তর্ভাৱ "utf8.h"
#অন্তর্ভাৱ "ref-filter.h"
#অন্তর্ভাৱ "worktree.h"
#অন্তর্ভাৱ "help.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "commit-reach.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ builtin_branch_usage[] = {
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
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ *head;
স্থির সংরচনা object_id head_oid;
স্থির পূর্ণাঙ্ক recurse_submodules = 0;
স্থির পূর্ণাঙ্ক submodule_propagate_branches = 0;

স্থির গণনা git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
স্থির ৱর্ণ branch_colors[][COLOR_MAXLEN] = {
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

স্থির ধ্রুৱ ৱর্ণ *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

স্থির সংরচনা string_list output = STRING_LIST_INIT_DUP;
স্থির unsigned পূর্ণাঙ্ক colopts;

define_list_config_array(color_branch_slots);

স্থির পূর্ণাঙ্ক git_branch_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			     ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	ধ্রুৱ ৱর্ণ *slot_name;

	যদি (!strcmp(var, "branch.sort")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		string_list_append(cb, মান);
		ঘূৰাই 0;
	}

	যদি (starts_with(var, "column."))
		ঘূৰাই git_column_config(var, মান, "branch", &colopts);
	যদি (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, মান);
		ঘূৰাই 0;
	}
	যদি (skip_prefix(var, "color.branch.", &slot_name)) {
		পূর্ণাঙ্ক slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		যদি (slot < 0)
			ঘূৰাই 0;
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		ঘূৰাই color_parse(মান, branch_colors[slot]);
	}
	যদি (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, মান);
		ঘূৰাই 0;
	}

	যদি (git_color_config(var, মান, cb) < 0)
		ঘূৰাই -1;

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির ধ্রুৱ ৱর্ণ *branch_get_color(গণনা color_branch ix)
{
	যদি (want_color(branch_use_color))
		ঘূৰাই branch_colors[ix];
	ঘূৰাই "";
}

স্থির পূর্ণাঙ্ক branch_merged(পূর্ণাঙ্ক kind, ধ্রুৱ ৱর্ণ *name,
			 সংরচনা commit *rev, সংরচনা commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	সংরচনা commit *reference_rev = রিক্ত;
	ধ্রুৱ ৱর্ণ *reference_name = রিক্ত;
	শূন্য *reference_name_to_free = রিক্ত;
	পূর্ণাঙ্ক merged;

	যদি (kind == FILTER_REFS_BRANCHES) {
		সংরচনা branch *branch = branch_get(name);
		ধ্রুৱ ৱর্ণ *upstream = branch_get_upstream(branch, রিক্ত);
		সংরচনা object_id oid;

		যদি (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, রিক্ত)) != রিক্ত)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	যদি (!reference_rev)
		reference_rev = head_rev;

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
	যদি (head_rev && head_rev != reference_rev) {
		পূর্ণাঙ্ক expect = repo_in_merge_bases(the_repository, rev, head_rev);
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
	ঘূৰাই merged;
}

গণনা delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

স্থির পূর্ণাঙ্ক check_branch_commit(ধ্রুৱ ৱর্ণ *branchname, ধ্রুৱ ৱর্ণ *refname,
			       ধ্রুৱ সংরচনা object_id *oid, সংরচনা commit *head_rev,
			       পূর্ণাঙ্ক kinds, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা commit *rev = lookup_commit_reference(the_repository, oid);
	যদি (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ঘূৰাই -1;
	}
	যদি (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		যদি (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ঘূৰাই -1;
	}
	ঘূৰাই 0;
}

স্থির শূন্য delete_branch_config(ধ্রুৱ ৱর্ণ *branchname)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	যদি (repo_config_rename_section(the_repository, buf.buf, রিক্ত) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

স্থির পূর্ণাঙ্ক delete_branches(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv, পূর্ণাঙ্ক kinds,
			   unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা commit *head_rev = রিক্ত;
	সংরচনা object_id oid;
	ৱর্ণ *name = রিক্ত;
	ধ্রুৱ ৱর্ণ *fmt;
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক ret = 0;
	পূর্ণাঙ্ক remote_branch = 0;
	সংরচনা strbuf bname = STRBUF_INIT;
	গণনা interpret_branch_kind allowed_interpret;
	সংরচনা string_list refs_to_delete = STRING_LIST_INIT_DUP;
	সংরচনা string_list_item *item;
	পূর্ণাঙ্ক branch_name_pos;
	ধ্রুৱ ৱর্ণ *fmt_remotes = "refs/remotes/%s";

	ৱিকল্প (kinds) {
	পক্ষ FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		ভঙ্গ;
	পক্ষ FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		ভঙ্গ;
	স্ৱভাৱ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	যদি (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	বাবে (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		ৱর্ণ *target = রিক্ত;
		পূর্ণাঙ্ক ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		মুক্ত(name);
		name = mkpathdup(fmt, bname.buf);

		যদি (kinds == FILTER_REFS_BRANCHES) {
			ধ্রুৱ ৱর্ণ *path;
			যদি ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				অনুৱর্ত;
			}
			যদি ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				অনুৱর্ত;
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
				ৱর্ণ *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				ৱর্ণ *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
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
			অনুৱর্ত;
		}

		যদি (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			যদি (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			গচ্ছ next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		মুক্ত(target);
	}

	যদি (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), রিক্ত, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ৱর্ণ *describe_ref = item->util;
		ৱর্ণ *name = item->string;
		যদি (flags & DELETE_BRANCH_DRY_RUN) {
			যদি (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} অন্যথা যদি (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			ৱর্ণ *refname = name + branch_name_pos;
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

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক calc_maxwidth(সংরচনা ref_array *refs, পূর্ণাঙ্ক remote_bonus)
{
	পূর্ণাঙ্ক i, max = 0;
	বাবে (i = 0; i < refs->nr; i++) {
		সংরচনা ref_array_item *it = refs->বস্তুসমূহ[i];
		ধ্রুৱ ৱর্ণ *desc = it->refname;
		পূর্ণাঙ্ক w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		যদি (it->kind == FILTER_REFS_DETACHED_HEAD) {
			ৱর্ণ *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			মুক্ত(head_desc);
		} অন্যথা
			w = utf8_strwidth(desc);

		যদি (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		যদি (w > max)
			max = w;
	}
	ঘূৰাই max;
}

স্থির ধ্রুৱ ৱর্ণ *quote_literal_for_format(ধ্রুৱ ৱর্ণ *s)
{
	স্থির সংরচনা strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	যেতিয়ালৈ (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ঘূৰাই buf.buf;
}

স্থির ৱর্ণ *build_format(সংরচনা ref_filter *filter, পূর্ণাঙ্ক maxwidth, ধ্রুৱ ৱর্ণ *remote_prefix)
{
	সংরচনা strbuf fmt = STRBUF_INIT;
	সংরচনা strbuf local = STRBUF_INIT;
	সংরচনা strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	যদি (filter->verbose) {
		সংরচনা strbuf obname = STRBUF_INIT;

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
	ঘূৰাই strbuf_detach(&fmt, রিক্ত);
}

স্থির শূন্য print_ref_list(সংরচনা ref_filter *filter, সংরচনা ref_sorting *sorting,
			   সংরচনা ref_format *format, সংরচনা string_list *output)
{
	পূর্ণাঙ্ক i;
	সংরচনা ref_array array;
	পূর্ণাঙ্ক maxwidth = 0;
	ধ্রুৱ ৱর্ণ *remote_prefix = "";
	ৱর্ণ *to_free = রিক্ত;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	যদি (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, পরিমাণ(array));

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
		সংরচনা strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		প্রতিজ্ঞা(!filter->verbose && "--column and --verbose are incompatible");

		বাবে (i = 0; i < array.nr; i++) {
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
	পূর্ণাঙ্ক flags;
	ধ্রুৱ ৱর্ণ *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, রিক্ত, &flags);
	ধ্রুৱ ৱর্ণ *shortname;
	যদি (!refname)
		die(_("could not resolve HEAD"));
	অন্যথা যদি (!(flags & REF_ISSYMREF))
		ঘূৰাই;
	অন্যথা যদি (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	অন্যথা
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

স্থির শূন্য reject_rebase_or_bisect_branch(সংরচনা worktree **worktrees,
					   ধ্রুৱ ৱর্ণ *target)
{
	পূর্ণাঙ্ক i;

	বাবে (i = 0; worktrees[i]; i++) {
		সংরচনা worktree *wt = worktrees[i];

		যদি (!wt->is_detached)
			অনুৱর্ত;

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
স্থির পূর্ণাঙ্ক replace_each_worktree_head_symref(সংরচনা worktree **worktrees,
					     ধ্রুৱ ৱর্ণ *oldref, ধ্রুৱ ৱর্ণ *newref,
					     ধ্রুৱ ৱর্ণ *logmsg)
{
	পূর্ণাঙ্ক ret = 0;
	পূর্ণাঙ্ক i;

	বাবে (i = 0; worktrees[i]; i++) {
		সংরচনা ref_store *refs;

		যদি (worktrees[i]->is_detached)
			অনুৱর্ত;
		যদি (!worktrees[i]->head_ref)
			অনুৱর্ত;
		যদি (strcmp(oldref, worktrees[i]->head_ref))
			অনুৱর্ত;

		refs = get_worktree_ref_store(worktrees[i]);
		যদি (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	ঘূৰাই ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

স্থির শূন্য copy_or_rename_branch(ধ্রুৱ ৱর্ণ *oldname, ধ্রুৱ ৱর্ণ *newname, পূর্ণাঙ্ক copy, পূর্ণাঙ্ক force)
{
	সংরচনা strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	সংরচনা strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *interpreted_oldname = রিক্ত;
	ধ্রুৱ ৱর্ণ *interpreted_newname = রিক্ত;
	পূর্ণাঙ্ক recovery = 0, oldref_usage = 0;
	সংরচনা worktree **worktrees = get_worktrees(the_repository);

	যদি (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		যদি (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		অন্যথা {
			পূর্ণাঙ্ক code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	বাবে (পূর্ণাঙ্ক i = 0; worktrees[i]; i++) {
		সংরচনা worktree *wt = worktrees[i];

		যদি (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			যদি (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			ভঙ্গ;
		}
	}

	যদি ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		যদি (oldref_usage & IS_HEAD)
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

	যদি (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
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

স্থির পূর্ণাঙ্ক parse_opt_forked(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা ref_filter *filter = opt->মান;

	BUG_ON_OPT_NEG(unset);
	যদি (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ঘূৰাই 0;
}

সংরচনা stacked_branch_data {
	সংরচনা strset *deletable_branch_names;
	সংরচনা strset *protected_branch_names;
};

স্থির পূর্ণাঙ্ক collect_stacked_branch_base(ধ্রুৱ সংরচনা reference *ref,
				       শূন্য *cb_data)
{
	সংরচনা stacked_branch_data *data = cb_data;
	ধ্রুৱ ৱর্ণ *branch_name;
	সংরচনা branch *branch;
	ধ্রুৱ ৱর্ণ *upstream_refname;
	ধ্রুৱ ৱর্ণ *upstream_branch_name;

	যদি (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	যদি (strset_contains(data->deletable_branch_names, branch_name))
		ঘূৰাই 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, রিক্ত);
	যদি (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		ঘূৰাই 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	ঘূৰাই 0;
}

স্থির শূন্য protect_stacked_branch_bases(সংরচনা ref_store *refs,
					 সংরচনা strset *deletable_branch_names,
					 সংরচনা strset *protected_branch_names)
{
	সংরচনা stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	সংরচনা refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	সংরচনা hashmap_iter iter;
	সংরচনা strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->চাবি);
}

স্থির শূন্য clear_deleted_upstreams(সংরচনা strset *protected_branch_names,
				    সংরচনা strset *deletable_branch_names)
{
	সংরচনা strbuf চাবি = STRBUF_INIT;
	সংরচনা hashmap_iter iter;
	সংরচনা strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		সংরচনা branch *branch = branch_get(entry->চাবি);
		ধ্রুৱ ৱর্ণ *upstream_refname = branch_get_upstream(branch, রিক্ত);
		ধ্রুৱ ৱর্ণ *upstream_branch_name;

		যদি (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			অনুৱর্ত;

		strbuf_addf(&চাবি, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, চাবি.buf, রিক্ত);
		strbuf_reset(&চাবি);
		strbuf_addf(&চাবি, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, চাবি.buf, রিক্ত);
		strbuf_reset(&চাবি);
	}

	strbuf_release(&চাবি);
}

স্থির পূর্ণাঙ্ক branch_pushes_to_upstream(সংরচনা branch *branch,
				     ধ্রুৱ ৱর্ণ *upstream)
{
	সংরচনা remote *remote = remote_get(remote_for_branch(branch, রিক্ত));
	ৱর্ণ *ঠেলো_refname = রিক্ত;
	ৱর্ণ *tracking = রিক্ত;
	পূর্ণাঙ্ক ret = 0;

	যদি (!remote)
		ঘূৰাই 0;
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
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক delete_merged_branches(ধ্রুৱ সংরচনা strvec *upstreams,
				 ধ্রুৱ ৱর্ণ **argv, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা ref_store *refs = get_main_ref_store(the_repository);
	সংরচনা ref_filter filter = REF_FILTER_INIT;
	সংরচনা ref_array candidates = { 0 };
	সংরচনা strset deletable_branch_names = STRSET_INIT;
	সংরচনা strset protected_branch_names = STRSET_INIT;
	সংরচনা strvec branches_to_delete = STRVEC_INIT;
	সংরচনা strbuf চাবি = STRBUF_INIT;
	সংরচনা hashmap_iter iter;
	সংরচনা strmap_entry *entry;
	পূর্ণাঙ্ক ret = 0;

	বাবে (size_t i = 0; i < upstreams->nr; i++)
		যদি (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	বাবে (পূর্ণাঙ্ক i = 0; i < candidates.nr; i++) {
		ধ্রুৱ ৱর্ণ *branch_refname = candidates.বস্তুসমূহ[i]->refname;
		ধ্রুৱ ৱর্ণ *branch_name;
		সংরচনা branch *branch;
		ধ্রুৱ ৱর্ণ *upstream_refname;
		পূর্ণাঙ্ক opt_out;

		যদি (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		যদি (branch_checked_out(branch_refname))
			অনুৱর্ত;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, রিক্ত);
		যদি (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			অনুৱর্ত;
		যদি (branch_pushes_to_upstream(branch, upstream_refname))
			অনুৱর্ত;
		যদি (check_branch_commit(branch_name, branch_name,
					&candidates.বস্তুসমূহ[i]->objectname, রিক্ত,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			অনুৱর্ত;

		strbuf_reset(&চাবি);
		strbuf_addf(&চাবি, "branch.%s.deletemerged", branch_name);
		যদি (!repo_config_get_bool(the_repository, চাবি.buf, &opt_out) &&
		    !opt_out) {
			যদি (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			অনুৱর্ত;
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
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
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
	ঘূৰাই ret;
}

স্থির GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

স্থির পূর্ণাঙ্ক edit_branch_description(ধ্রুৱ ৱর্ণ *branch_name)
{
	পূর্ণাঙ্ক exists;
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	যদি (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_সাৰি_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_সাৰি_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	যদি (launch_editor(edit_description(), &buf, রিক্ত)) {
		strbuf_release(&buf);
		ঘূৰাই -1;
	}
	strbuf_stripspace(&buf, comment_সাৰি_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	যদি (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : রিক্ত);
	strbuf_release(&name);
	strbuf_release(&buf);

	ঘূৰাই 0;
}

স্থির শূন্য die_if_upstream_looks_like_remote(ধ্রুৱ ৱর্ণ *new_upstream, ধ্রুৱ ৱর্ণ *branch_name)
{
	সংরচনা strbuf remote_ref = STRBUF_INIT;
	পূর্ণাঙ্ক code;

	যদি (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ঘূৰাই;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	যদি (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ঘূৰাই;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

পূর্ণাঙ্ক cmd_branch(পূর্ণাঙ্ক argc,
	       ধ্রুৱ ৱর্ণ **argv,
	       ধ্রুৱ ৱর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	/* possible actions */
	পূর্ণাঙ্ক delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	সংরচনা strvec delete_merged = STRVEC_INIT;
	পূর্ণাঙ্ক dry_run = 0;
	ধ্রুৱ ৱর্ণ *new_upstream = রিক্ত;
	পূর্ণাঙ্ক noncreate_actions = 0;
	/* possible options */
	পূর্ণাঙ্ক reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	গণনা branch_track track;
	সংরচনা ref_filter filter = REF_FILTER_INIT;
	স্থির সংরচনা ref_sorting *sorting;
	সংরচনা string_list sorting_options = STRING_LIST_INIT_DUP;
	সংরচনা ref_format format = REF_FORMAT_INIT;
	সংরচনা repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);
	পূর্ণাঙ্ক ret;

	সংরচনা option options[] = {
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
		OPT_BIT('D', রিক্ত, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', রিক্ত, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', রিক্ত, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_বাৰ্তা();

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

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, রিক্ত);
	যদি (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	যদি (!strcmp(head, "HEAD"))
		filter.detached = 1;
	অন্যথা যদি (!skip_prefix(head, "refs/heads/", &head))
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
		গচ্ছ out;
	} অন্যথা যদি (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		গচ্ছ out;
	} অন্যথা যদি (show_current) {
		print_current_branch_name();
		ret = 0;
		গচ্ছ out;
	} অন্যথা যদি (list) {
		/*  git branch --list also shows HEAD when it is detached */
		যদি ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, রিক্ত);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		গচ্ছ out;
	} অন্যথা যদি (edit_description) {
		ধ্রুৱ ৱর্ণ *branch_name;
		সংরচনা strbuf branch_ref = STRBUF_INIT;
		সংরচনা strbuf buf = STRBUF_INIT;

		যদি (!argc) {
			যদি (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
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

		গচ্ছ out;
	} অন্যথা যদি (copy || rename) {
		যদি (!argc)
			die(_("branch name required"));
		অন্যথা যদি ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		অন্যথা যদি (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		অন্যথা যদি (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		অন্যথা
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} অন্যথা যদি (new_upstream) {
		সংরচনা branch *branch;
		সংরচনা strbuf buf = STRBUF_INIT;

		যদি (!argc)
			branch = branch_get(রিক্ত);
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
		সংরচনা branch *branch;
		সংরচনা strbuf buf = STRBUF_INIT;

		যদি (!argc)
			branch = branch_get(রিক্ত);
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
		repo_config_set_multivar(the_repository, buf.buf, রিক্ত, রিক্ত, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, রিক্ত, রিক্ত, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} অন্যথা যদি (!noncreate_actions && argc > 0 && argc <= 2) {
		ধ্রুৱ ৱর্ণ *branch_name = argv[0];
		ধ্রুৱ ৱর্ণ *start_name = argc == 2 ? argv[1] : head;

		যদি (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		যদি (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		যদি (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, রিক্ত, force,
						    reflog, quiet, track, 0);
			ret = 0;
			গচ্ছ out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} অন্যথা
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	ঘূৰাই ret;
}
