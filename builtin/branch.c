/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "strmap.h"
#അന്തര്ഭാവ "column.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "ref-filter.h"
#അന്തര്ഭാവ "worktree.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "commit-reach.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_branch_usage[] = {
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
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *head;
സ്ഥിതം ഘടന object_id head_oid;
സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ submodule_propagate_branches = 0;

സ്ഥിതം എണ്ണൽ git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
സ്ഥിതം അക്ഷരം branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
എണ്ണൽ color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

സ്ഥിതം ഘടന string_list output = STRING_LIST_INIT_DUP;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ colopts;

define_list_config_array(color_branch_slots);

സ്ഥിതം പൂർണ്ണസംഖ്യ git_branch_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			     സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	സ്ഥിരം അക്ഷരം *slot_name;

	എങ്കിൽ (!strcmp(var, "branch.sort")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		string_list_append(cb, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (starts_with(var, "column."))
		തിരികെ git_column_config(var, മൂല്യം, "branch", &colopts);
	എങ്കിൽ (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (skip_prefix(var, "color.branch.", &slot_name)) {
		പൂർണ്ണസംഖ്യ slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		എങ്കിൽ (slot < 0)
			തിരികെ 0;
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		തിരികെ color_parse(മൂല്യം, branch_colors[slot]);
	}
	എങ്കിൽ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (git_color_config(var, മൂല്യം, cb) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *branch_get_color(എണ്ണൽ color_branch ix)
{
	എങ്കിൽ (want_color(branch_use_color))
		തിരികെ branch_colors[ix];
	തിരികെ "";
}

സ്ഥിതം പൂർണ്ണസംഖ്യ branch_merged(പൂർണ്ണസംഖ്യ kind, സ്ഥിരം അക്ഷരം *name,
			 ഘടന commit *rev, ഘടന commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	ഘടന commit *reference_rev = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *reference_name = ശൂന്യ;
	ശൂന്യം *reference_name_to_free = ശൂന്യ;
	പൂർണ്ണസംഖ്യ merged;

	എങ്കിൽ (kind == FILTER_REFS_BRANCHES) {
		ഘടന branch *branch = branch_get(name);
		സ്ഥിരം അക്ഷരം *upstream = branch_get_upstream(branch, ശൂന്യ);
		ഘടന object_id oid;

		എങ്കിൽ (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ശൂന്യ)) != ശൂന്യ)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	എങ്കിൽ (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	എങ്കിൽ (merged < 0)
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
	എങ്കിൽ (head_rev && head_rev != reference_rev) {
		പൂർണ്ണസംഖ്യ expect = repo_in_merge_bases(the_repository, rev, head_rev);
		എങ്കിൽ (expect < 0)
			exit(128);
		എങ്കിൽ (expect == merged)
			; /* okay */
		അല്ലെങ്കിൽ എങ്കിൽ (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		അല്ലെങ്കിൽ
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	സ്വതന്ത്രം(reference_name_to_free);
	തിരികെ merged;
}

എണ്ണൽ delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

സ്ഥിതം പൂർണ്ണസംഖ്യ check_branch_commit(സ്ഥിരം അക്ഷരം *branchname, സ്ഥിരം അക്ഷരം *refname,
			       സ്ഥിരം ഘടന object_id *oid, ഘടന commit *head_rev,
			       പൂർണ്ണസംഖ്യ kinds, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന commit *rev = lookup_commit_reference(the_repository, oid);
	എങ്കിൽ (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		തിരികെ -1;
	}
	എങ്കിൽ (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		എങ്കിൽ (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		തിരികെ -1;
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം delete_branch_config(സ്ഥിരം അക്ഷരം *branchname)
{
	ഘടന strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	എങ്കിൽ (repo_config_rename_section(the_repository, buf.buf, ശൂന്യ) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_branches(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, പൂർണ്ണസംഖ്യ kinds,
			   unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന commit *head_rev = ശൂന്യ;
	ഘടന object_id oid;
	അക്ഷരം *name = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *fmt;
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ ret = 0;
	പൂർണ്ണസംഖ്യ remote_branch = 0;
	ഘടന strbuf bname = STRBUF_INIT;
	എണ്ണൽ interpret_branch_kind allowed_interpret;
	ഘടന string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ഘടന string_list_item *item;
	പൂർണ്ണസംഖ്യ branch_name_pos;
	സ്ഥിരം അക്ഷരം *fmt_remotes = "refs/remotes/%s";

	തിരഞ്ഞെടുപ്പ് (kinds) {
	അവസ്ഥ FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		നിർത്തുക;
	അവസ്ഥ FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	എങ്കിൽ (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	വേണ്ടി (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		അക്ഷരം *target = ശൂന്യ;
		പൂർണ്ണസംഖ്യ ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		സ്വതന്ത്രം(name);
		name = mkpathdup(fmt, bname.buf);

		എങ്കിൽ (kinds == FILTER_REFS_BRANCHES) {
			സ്ഥിരം അക്ഷരം *path;
			എങ്കിൽ ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				തുടരുക;
			}
			എങ്കിൽ ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				തുടരുക;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		എങ്കിൽ (!target) {
			എങ്കിൽ (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} അല്ലെങ്കിൽ {
				അക്ഷരം *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				അക്ഷരം *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				എങ്കിൽ (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				അല്ലെങ്കിൽ
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			തുടരുക;
		}

		എങ്കിൽ (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			എങ്കിൽ (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			പോകുക next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		സ്വതന്ത്രം(target);
	}

	എങ്കിൽ (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ശൂന്യ, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		അക്ഷരം *describe_ref = item->util;
		അക്ഷരം *name = item->string;
		എങ്കിൽ (flags & DELETE_BRANCH_DRY_RUN) {
			എങ്കിൽ (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} അല്ലെങ്കിൽ എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			അക്ഷരം *refname = name + branch_name_pos;
			എങ്കിൽ (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		സ്വതന്ത്രം(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	സ്വതന്ത്രം(name);
	strbuf_release(&bname);

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ calc_maxwidth(ഘടന ref_array *refs, പൂർണ്ണസംഖ്യ remote_bonus)
{
	പൂർണ്ണസംഖ്യ i, max = 0;
	വേണ്ടി (i = 0; i < refs->nr; i++) {
		ഘടന ref_array_item *it = refs->ഇനങ്ങൾ[i];
		സ്ഥിരം അക്ഷരം *desc = it->refname;
		പൂർണ്ണസംഖ്യ w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		എങ്കിൽ (it->kind == FILTER_REFS_DETACHED_HEAD) {
			അക്ഷരം *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			സ്വതന്ത്രം(head_desc);
		} അല്ലെങ്കിൽ
			w = utf8_strwidth(desc);

		എങ്കിൽ (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		എങ്കിൽ (w > max)
			max = w;
	}
	തിരികെ max;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *quote_literal_for_format(സ്ഥിരം അക്ഷരം *s)
{
	സ്ഥിതം ഘടന strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	ആകുമ്പോൾ (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	തിരികെ buf.buf;
}

സ്ഥിതം അക്ഷരം *build_format(ഘടന ref_filter *filter, പൂർണ്ണസംഖ്യ maxwidth, സ്ഥിരം അക്ഷരം *remote_prefix)
{
	ഘടന strbuf fmt = STRBUF_INIT;
	ഘടന strbuf local = STRBUF_INIT;
	ഘടന strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	എങ്കിൽ (filter->verbose) {
		ഘടന strbuf obname = STRBUF_INIT;

		എങ്കിൽ (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		അല്ലെങ്കിൽ എങ്കിൽ (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		അല്ലെങ്കിൽ
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		എങ്കിൽ (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		അല്ലെങ്കിൽ
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} അല്ലെങ്കിൽ {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	തിരികെ strbuf_detach(&fmt, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം print_ref_list(ഘടന ref_filter *filter, ഘടന ref_sorting *sorting,
			   ഘടന ref_format *format, ഘടന string_list *output)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന ref_array array;
	പൂർണ്ണസംഖ്യ maxwidth = 0;
	സ്ഥിരം അക്ഷരം *remote_prefix = "";
	അക്ഷരം *to_free = ശൂന്യ;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	എങ്കിൽ (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, വലുപ്പം(array));

	filter_refs(&array, filter, filter->kind);

	എങ്കിൽ (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	എങ്കിൽ (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	എങ്കിൽ (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	എങ്കിൽ (column_active(colopts)) {
		ഘടന strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		പ്രതിജ്ഞാ(!filter->verbose && "--column and --verbose are incompatible");

		വേണ്ടി (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			എങ്കിൽ (format_ref_array_item(array.ഇനങ്ങൾ[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} അല്ലെങ്കിൽ {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	സ്വതന്ത്രം(to_free);
}

സ്ഥിതം ശൂന്യം print_current_branch_name(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ flags;
	സ്ഥിരം അക്ഷരം *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ശൂന്യ, &flags);
	സ്ഥിരം അക്ഷരം *shortname;
	എങ്കിൽ (!refname)
		die(_("could not resolve HEAD"));
	അല്ലെങ്കിൽ എങ്കിൽ (!(flags & REF_ISSYMREF))
		തിരികെ;
	അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	അല്ലെങ്കിൽ
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

സ്ഥിതം ശൂന്യം reject_rebase_or_bisect_branch(ഘടന worktree **worktrees,
					   സ്ഥിരം അക്ഷരം *target)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; worktrees[i]; i++) {
		ഘടന worktree *wt = worktrees[i];

		എങ്കിൽ (!wt->is_detached)
			തുടരുക;

		എങ്കിൽ (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		എങ്കിൽ (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ replace_each_worktree_head_symref(ഘടന worktree **worktrees,
					     സ്ഥിരം അക്ഷരം *oldref, സ്ഥിരം അക്ഷരം *newref,
					     സ്ഥിരം അക്ഷരം *logmsg)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; worktrees[i]; i++) {
		ഘടന ref_store *refs;

		എങ്കിൽ (worktrees[i]->is_detached)
			തുടരുക;
		എങ്കിൽ (!worktrees[i]->head_ref)
			തുടരുക;
		എങ്കിൽ (strcmp(oldref, worktrees[i]->head_ref))
			തുടരുക;

		refs = get_worktree_ref_store(worktrees[i]);
		എങ്കിൽ (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	തിരികെ ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

സ്ഥിതം ശൂന്യം copy_or_rename_branch(സ്ഥിരം അക്ഷരം *oldname, സ്ഥിരം അക്ഷരം *newname, പൂർണ്ണസംഖ്യ copy, പൂർണ്ണസംഖ്യ force)
{
	ഘടന strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	ഘടന strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *interpreted_oldname = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *interpreted_newname = ശൂന്യ;
	പൂർണ്ണസംഖ്യ recovery = 0, oldref_usage = 0;
	ഘടന worktree **worktrees = get_worktrees(the_repository);

	എങ്കിൽ (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		എങ്കിൽ (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		അല്ലെങ്കിൽ {
			പൂർണ്ണസംഖ്യ code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; worktrees[i]; i++) {
		ഘടന worktree *wt = worktrees[i];

		എങ്കിൽ (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			എങ്കിൽ (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			നിർത്തുക;
		}
	}

	എങ്കിൽ ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		എങ്കിൽ (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		അല്ലെങ്കിൽ
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	എങ്കിൽ (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	അല്ലെങ്കിൽ
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	എങ്കിൽ (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	എങ്കിൽ (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	അല്ലെങ്കിൽ
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	എങ്കിൽ (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	എങ്കിൽ (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	എങ്കിൽ (recovery) {
		എങ്കിൽ (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		അല്ലെങ്കിൽ
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	എങ്കിൽ (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	എങ്കിൽ (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	എങ്കിൽ (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_forked(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന ref_filter *filter = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);
	എങ്കിൽ (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	തിരികെ 0;
}

ഘടന stacked_branch_data {
	ഘടന strset *deletable_branch_names;
	ഘടന strset *protected_branch_names;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_stacked_branch_base(സ്ഥിരം ഘടന reference *ref,
				       ശൂന്യം *cb_data)
{
	ഘടന stacked_branch_data *data = cb_data;
	സ്ഥിരം അക്ഷരം *branch_name;
	ഘടന branch *branch;
	സ്ഥിരം അക്ഷരം *upstream_refname;
	സ്ഥിരം അക്ഷരം *upstream_branch_name;

	എങ്കിൽ (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	എങ്കിൽ (strset_contains(data->deletable_branch_names, branch_name))
		തിരികെ 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ശൂന്യ);
	എങ്കിൽ (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		തിരികെ 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം protect_stacked_branch_bases(ഘടന ref_store *refs,
					 ഘടന strset *deletable_branch_names,
					 ഘടന strset *protected_branch_names)
{
	ഘടന stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	ഘടന refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	ഘടന hashmap_iter iter;
	ഘടന strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->താക്കോൽ);
}

സ്ഥിതം ശൂന്യം clear_deleted_upstreams(ഘടന strset *protected_branch_names,
				    ഘടന strset *deletable_branch_names)
{
	ഘടന strbuf താക്കോൽ = STRBUF_INIT;
	ഘടന hashmap_iter iter;
	ഘടന strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		ഘടന branch *branch = branch_get(entry->താക്കോൽ);
		സ്ഥിരം അക്ഷരം *upstream_refname = branch_get_upstream(branch, ശൂന്യ);
		സ്ഥിരം അക്ഷരം *upstream_branch_name;

		എങ്കിൽ (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			തുടരുക;

		strbuf_addf(&താക്കോൽ, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, താക്കോൽ.buf, ശൂന്യ);
		strbuf_reset(&താക്കോൽ);
		strbuf_addf(&താക്കോൽ, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, താക്കോൽ.buf, ശൂന്യ);
		strbuf_reset(&താക്കോൽ);
	}

	strbuf_release(&താക്കോൽ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ branch_pushes_to_upstream(ഘടന branch *branch,
				     സ്ഥിരം അക്ഷരം *upstream)
{
	ഘടന remote *remote = remote_get(remote_for_branch(branch, ശൂന്യ));
	അക്ഷരം *തള്ളുക_refname = ശൂന്യ;
	അക്ഷരം *tracking = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (!remote)
		തിരികെ 0;
	എങ്കിൽ (remote->തള്ളുക.nr)
		തള്ളുക_refname = apply_refspecs(&remote->തള്ളുക, branch->refname);
	അല്ലെങ്കിൽ
		തള്ളുക_refname = xstrdup(branch->refname);
	എങ്കിൽ (തള്ളുക_refname)
		tracking = apply_refspecs(&remote->fetch, തള്ളുക_refname);
	എങ്കിൽ (tracking && !strcmp(tracking, upstream))
		ret = 1;

	സ്വതന്ത്രം(തള്ളുക_refname);
	സ്വതന്ത്രം(tracking);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_merged_branches(സ്ഥിരം ഘടന strvec *upstreams,
				 സ്ഥിരം അക്ഷരം **argv, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന ref_store *refs = get_main_ref_store(the_repository);
	ഘടന ref_filter filter = REF_FILTER_INIT;
	ഘടന ref_array candidates = { 0 };
	ഘടന strset deletable_branch_names = STRSET_INIT;
	ഘടന strset protected_branch_names = STRSET_INIT;
	ഘടന strvec branches_to_delete = STRVEC_INIT;
	ഘടന strbuf താക്കോൽ = STRBUF_INIT;
	ഘടന hashmap_iter iter;
	ഘടന strmap_entry *entry;
	പൂർണ്ണസംഖ്യ ret = 0;

	വേണ്ടി (size_t i = 0; i < upstreams->nr; i++)
		എങ്കിൽ (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < candidates.nr; i++) {
		സ്ഥിരം അക്ഷരം *branch_refname = candidates.ഇനങ്ങൾ[i]->refname;
		സ്ഥിരം അക്ഷരം *branch_name;
		ഘടന branch *branch;
		സ്ഥിരം അക്ഷരം *upstream_refname;
		പൂർണ്ണസംഖ്യ opt_out;

		എങ്കിൽ (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		എങ്കിൽ (branch_checked_out(branch_refname))
			തുടരുക;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ശൂന്യ);
		എങ്കിൽ (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			തുടരുക;
		എങ്കിൽ (branch_pushes_to_upstream(branch, upstream_refname))
			തുടരുക;
		എങ്കിൽ (check_branch_commit(branch_name, branch_name,
					&candidates.ഇനങ്ങൾ[i]->objectname, ശൂന്യ,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			തുടരുക;

		strbuf_reset(&താക്കോൽ);
		strbuf_addf(&താക്കോൽ, "branch.%s.deletemerged", branch_name);
		എങ്കിൽ (!repo_config_get_bool(the_repository, താക്കോൽ.buf, &opt_out) &&
		    !opt_out) {
			എങ്കിൽ (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			തുടരുക;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_തള്ളുക(&branches_to_delete, entry->താക്കോൽ);

	എങ്കിൽ (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	എങ്കിൽ (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&താക്കോൽ);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	തിരികെ ret;
}

സ്ഥിതം GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

സ്ഥിതം പൂർണ്ണസംഖ്യ edit_branch_description(സ്ഥിരം അക്ഷരം *branch_name)
{
	പൂർണ്ണസംഖ്യ exists;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	എങ്കിൽ (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_വരി_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_വരി_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	എങ്കിൽ (launch_editor(edit_description(), &buf, ശൂന്യ)) {
		strbuf_release(&buf);
		തിരികെ -1;
	}
	strbuf_stripspace(&buf, comment_വരി_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	എങ്കിൽ (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : ശൂന്യ);
	strbuf_release(&name);
	strbuf_release(&buf);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം die_if_upstream_looks_like_remote(സ്ഥിരം അക്ഷരം *new_upstream, സ്ഥിരം അക്ഷരം *branch_name)
{
	ഘടന strbuf remote_ref = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ code;

	എങ്കിൽ (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		തിരികെ;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		തിരികെ;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

പൂർണ്ണസംഖ്യ cmd_branch(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	/* possible actions */
	പൂർണ്ണസംഖ്യ delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	ഘടന strvec delete_merged = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ dry_run = 0;
	സ്ഥിരം അക്ഷരം *new_upstream = ശൂന്യ;
	പൂർണ്ണസംഖ്യ noncreate_actions = 0;
	/* possible options */
	പൂർണ്ണസംഖ്യ reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	എണ്ണൽ branch_track track;
	ഘടന ref_filter filter = REF_FILTER_INIT;
	സ്ഥിതം ഘടന ref_sorting *sorting;
	ഘടന string_list sorting_options = STRING_LIST_INIT_DUP;
	ഘടന ref_format format = REF_FORMAT_INIT;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);
	പൂർണ്ണസംഖ്യ ret;

	ഘടന option options[] = {
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
		OPT_BIT('D', ശൂന്യ, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', ശൂന്യ, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', ശൂന്യ, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_സന്ദേശം();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	എങ്കിൽ (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, ശൂന്യ);
	എങ്കിൽ (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	എങ്കിൽ (!strcmp(head, "HEAD"))
		filter.detached = 1;
	അല്ലെങ്കിൽ എങ്കിൽ (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	എങ്കിൽ (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	എങ്കിൽ (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	എങ്കിൽ (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	എങ്കിൽ (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	എങ്കിൽ (recurse_submodules_explicit) {
		എങ്കിൽ (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		എങ്കിൽ (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	എങ്കിൽ (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	എങ്കിൽ (filter.verbose) {
		എങ്കിൽ (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	എങ്കിൽ (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	എങ്കിൽ (list)
		setup_auto_pager("branch", 1);

	എങ്കിൽ (delete) {
		എങ്കിൽ (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (show_current) {
		print_current_branch_name();
		ret = 0;
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (list) {
		/*  git branch --list also shows HEAD when it is detached */
		എങ്കിൽ ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, ശൂന്യ);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (edit_description) {
		സ്ഥിരം അക്ഷരം *branch_name;
		ഘടന strbuf branch_ref = STRBUF_INIT;
		ഘടന strbuf buf = STRBUF_INIT;

		എങ്കിൽ (!argc) {
			എങ്കിൽ (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} അല്ലെങ്കിൽ എങ്കിൽ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} അല്ലെങ്കിൽ {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} അല്ലെങ്കിൽ {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (copy || rename) {
		എങ്കിൽ (!argc)
			die(_("branch name required"));
		അല്ലെങ്കിൽ എങ്കിൽ ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		അല്ലെങ്കിൽ എങ്കിൽ (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		അല്ലെങ്കിൽ എങ്കിൽ (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		അല്ലെങ്കിൽ
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} അല്ലെങ്കിൽ എങ്കിൽ (new_upstream) {
		ഘടന branch *branch;
		ഘടന strbuf buf = STRBUF_INIT;

		എങ്കിൽ (!argc)
			branch = branch_get(ശൂന്യ);
		അല്ലെങ്കിൽ എങ്കിൽ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} അല്ലെങ്കിൽ
			die(_("too many arguments to set new upstream"));

		എങ്കിൽ (!branch) {
			എങ്കിൽ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			എങ്കിൽ (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			എങ്കിൽ (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} അല്ലെങ്കിൽ എങ്കിൽ (unset_upstream) {
		ഘടന branch *branch;
		ഘടന strbuf buf = STRBUF_INIT;

		എങ്കിൽ (!argc)
			branch = branch_get(ശൂന്യ);
		അല്ലെങ്കിൽ എങ്കിൽ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} അല്ലെങ്കിൽ
			die(_("too many arguments to unset upstream"));

		എങ്കിൽ (!branch) {
			എങ്കിൽ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		എങ്കിൽ (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ശൂന്യ, ശൂന്യ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ശൂന്യ, ശൂന്യ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} അല്ലെങ്കിൽ എങ്കിൽ (!noncreate_actions && argc > 0 && argc <= 2) {
		സ്ഥിരം അക്ഷരം *branch_name = argv[0];
		സ്ഥിരം അക്ഷരം *start_name = argc == 2 ? argv[1] : head;

		എങ്കിൽ (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		എങ്കിൽ (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		എങ്കിൽ (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, ശൂന്യ, force,
						    reflog, quiet, track, 0);
			ret = 0;
			പോകുക out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} അല്ലെങ്കിൽ
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	തിരികെ ret;
}
