/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "strmap.h"
#ଅନ୍ତର୍ଭାଵ "column.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "ref-filter.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"
#ଅନ୍ତର୍ଭାଵ "help.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_branch_usage[] = {
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
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *head;
ସ୍ଥିର ସଂରଚନା object_id head_oid;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା submodule_propagate_branches = 0;

ସ୍ଥିର ଗଣନା git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
ସ୍ଥିର ଅକ୍ଷର branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
ଗଣନା color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

ସ୍ଥିର ସଂରଚନା string_list output = STRING_LIST_INIT_DUP;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା colopts;

define_list_config_array(color_branch_slots);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_branch_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			     ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଅଚଳ ଅକ୍ଷର *slot_name;

	ଯଦି (!strcmp(var, "branch.sort")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		string_list_append(cb, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (starts_with(var, "column."))
		ଫେରନ୍ତୁ git_column_config(var, ମୂଲ୍ୟ, "branch", &colopts);
	ଯଦି (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (skip_prefix(var, "color.branch.", &slot_name)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		ଯଦି (slot < 0)
			ଫେରନ୍ତୁ 0;
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଫେରନ୍ତୁ color_parse(ମୂଲ୍ୟ, branch_colors[slot]);
	}
	ଯଦି (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (git_color_config(var, ମୂଲ୍ୟ, cb) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *branch_get_color(ଗଣନା color_branch ix)
{
	ଯଦି (want_color(branch_use_color))
		ଫେରନ୍ତୁ branch_colors[ix];
	ଫେରନ୍ତୁ "";
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା branch_merged(ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind, ଅଚଳ ଅକ୍ଷର *name,
			 ସଂରଚନା commit *rev, ସଂରଚନା commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	ସଂରଚନା commit *reference_rev = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *reference_name = ଖାଲି;
	ଶୂନ୍ୟ *reference_name_to_free = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା merged;

	ଯଦି (kind == FILTER_REFS_BRANCHES) {
		ସଂରଚନା branch *branch = branch_get(name);
		ଅଚଳ ଅକ୍ଷର *upstream = branch_get_upstream(branch, ଖାଲି);
		ସଂରଚନା object_id oid;

		ଯଦି (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ଖାଲି)) != ଖାଲି)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ଯଦି (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	ଯଦି (merged < 0)
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
	ଯଦି (head_rev && head_rev != reference_rev) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା expect = repo_in_merge_bases(the_repository, rev, head_rev);
		ଯଦି (expect < 0)
			exit(128);
		ଯଦି (expect == merged)
			; /* okay */
		ଅନ୍ୟଥା ଯଦି (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		ଅନ୍ୟଥା
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	ମୁକ୍ତ(reference_name_to_free);
	ଫେରନ୍ତୁ merged;
}

ଗଣନା delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_branch_commit(ଅଚଳ ଅକ୍ଷର *branchname, ଅଚଳ ଅକ୍ଷର *refname,
			       ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା commit *head_rev,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା kinds, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା commit *rev = lookup_commit_reference(the_repository, oid);
	ଯଦି (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ଫେରନ୍ତୁ -1;
	}
	ଯଦି (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		ଯଦି (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ଫେରନ୍ତୁ -1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ delete_branch_config(ଅଚଳ ଅକ୍ଷର *branchname)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ଯଦି (repo_config_rename_section(the_repository, buf.buf, ଖାଲି) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_branches(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା kinds,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା commit *head_rev = ଖାଲି;
	ସଂରଚନା object_id oid;
	ଅକ୍ଷର *name = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *fmt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା remote_branch = 0;
	ସଂରଚନା strbuf bname = STRBUF_INIT;
	ଗଣନା interpret_branch_kind allowed_interpret;
	ସଂରଚନା string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list_item *item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା branch_name_pos;
	ଅଚଳ ଅକ୍ଷର *fmt_remotes = "refs/remotes/%s";

	ବାଛ (kinds) {
	ସ୍ଥିତି FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ଯଦି (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	ପାଇଁ (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		ଅକ୍ଷର *target = ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		ମୁକ୍ତ(name);
		name = mkpathdup(fmt, bname.buf);

		ଯଦି (kinds == FILTER_REFS_BRANCHES) {
			ଅଚଳ ଅକ୍ଷର *path;
			ଯଦି ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				ଜାରି;
			}
			ଯଦି ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				ଜାରି;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		ଯଦି (!target) {
			ଯଦି (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} ଅନ୍ୟଥା {
				ଅକ୍ଷର *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				ଅକ୍ଷର *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ଯଦି (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				ଅନ୍ୟଥା
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			ଜାରି;
		}

		ଯଦି (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			ଯଦି (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			ଯାଅ next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		ମୁକ୍ତ(target);
	}

	ଯଦି (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ଖାଲି, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ଅକ୍ଷର *describe_ref = item->util;
		ଅକ୍ଷର *name = item->string;
		ଯଦି (flags & DELETE_BRANCH_DRY_RUN) {
			ଯଦି (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} ଅନ୍ୟଥା ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			ଅକ୍ଷର *refname = name + branch_name_pos;
			ଯଦି (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		ମୁକ୍ତ(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	ମୁକ୍ତ(name);
	strbuf_release(&bname);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା calc_maxwidth(ସଂରଚନା ref_array *refs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା remote_bonus)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, max = 0;
	ପାଇଁ (i = 0; i < refs->nr; i++) {
		ସଂରଚନା ref_array_item *it = refs->ବସ୍ତୁଗୁଡ଼ିକ[i];
		ଅଚଳ ଅକ୍ଷର *desc = it->refname;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		ଯଦି (it->kind == FILTER_REFS_DETACHED_HEAD) {
			ଅକ୍ଷର *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			ମୁକ୍ତ(head_desc);
		} ଅନ୍ୟଥା
			w = utf8_strwidth(desc);

		ଯଦି (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ଯଦି (w > max)
			max = w;
	}
	ଫେରନ୍ତୁ max;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *quote_literal_for_format(ଅଚଳ ଅକ୍ଷର *s)
{
	ସ୍ଥିର ସଂରଚନା strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	ଯେତେବେଳେ (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ଫେରନ୍ତୁ buf.buf;
}

ସ୍ଥିର ଅକ୍ଷର *build_format(ସଂରଚନା ref_filter *filter, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxwidth, ଅଚଳ ଅକ୍ଷର *remote_prefix)
{
	ସଂରଚନା strbuf fmt = STRBUF_INIT;
	ସଂରଚନା strbuf local = STRBUF_INIT;
	ସଂରଚନା strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ଯଦି (filter->verbose) {
		ସଂରଚନା strbuf obname = STRBUF_INIT;

		ଯଦି (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		ଅନ୍ୟଥା ଯଦି (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		ଅନ୍ୟଥା
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ଯଦି (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		ଅନ୍ୟଥା
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} ଅନ୍ୟଥା {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	ଫେରନ୍ତୁ strbuf_detach(&fmt, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ print_ref_list(ସଂରଚନା ref_filter *filter, ସଂରଚନା ref_sorting *sorting,
			   ସଂରଚନା ref_format *format, ସଂରଚନା string_list *output)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା ref_array array;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxwidth = 0;
	ଅଚଳ ଅକ୍ଷର *remote_prefix = "";
	ଅକ୍ଷର *to_free = ଖାଲି;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ଯଦି (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, ଆକାର(array));

	filter_refs(&array, filter, filter->kind);

	ଯଦି (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	ଯଦି (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	ଯଦି (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	ଯଦି (column_active(colopts)) {
		ସଂରଚନା strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		ପ୍ରତିଜ୍ଞା(!filter->verbose && "--column and --verbose are incompatible");

		ପାଇଁ (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			ଯଦି (format_ref_array_item(array.ବସ୍ତୁଗୁଡ଼ିକ[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} ଅନ୍ୟଥା {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	ମୁକ୍ତ(to_free);
}

ସ୍ଥିର ଶୂନ୍ୟ print_current_branch_name(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	ଅଚଳ ଅକ୍ଷର *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ଖାଲି, &flags);
	ଅଚଳ ଅକ୍ଷର *shortname;
	ଯଦି (!refname)
		die(_("could not resolve HEAD"));
	ଅନ୍ୟଥା ଯଦି (!(flags & REF_ISSYMREF))
		ଫେରନ୍ତୁ;
	ଅନ୍ୟଥା ଯଦି (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	ଅନ୍ୟଥା
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

ସ୍ଥିର ଶୂନ୍ୟ reject_rebase_or_bisect_branch(ସଂରଚନା worktree **worktrees,
					   ଅଚଳ ଅକ୍ଷର *target)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; worktrees[i]; i++) {
		ସଂରଚନା worktree *wt = worktrees[i];

		ଯଦି (!wt->is_detached)
			ଜାରି;

		ଯଦି (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		ଯଦି (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା replace_each_worktree_head_symref(ସଂରଚନା worktree **worktrees,
					     ଅଚଳ ଅକ୍ଷର *oldref, ଅଚଳ ଅକ୍ଷର *newref,
					     ଅଚଳ ଅକ୍ଷର *logmsg)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; worktrees[i]; i++) {
		ସଂରଚନା ref_store *refs;

		ଯଦି (worktrees[i]->is_detached)
			ଜାରି;
		ଯଦି (!worktrees[i]->head_ref)
			ଜାରି;
		ଯଦି (strcmp(oldref, worktrees[i]->head_ref))
			ଜାରି;

		refs = get_worktree_ref_store(worktrees[i]);
		ଯଦି (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	ଫେରନ୍ତୁ ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

ସ୍ଥିର ଶୂନ୍ୟ copy_or_rename_branch(ଅଚଳ ଅକ୍ଷର *oldname, ଅଚଳ ଅକ୍ଷର *newname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା force)
{
	ସଂରଚନା strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	ସଂରଚନା strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *interpreted_oldname = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *interpreted_newname = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା recovery = 0, oldref_usage = 0;
	ସଂରଚନା worktree **worktrees = get_worktrees(the_repository);

	ଯଦି (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ଯଦି (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		ଅନ୍ୟଥା {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; worktrees[i]; i++) {
		ସଂରଚନା worktree *wt = worktrees[i];

		ଯଦି (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			ଯଦି (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ଯଦି (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		ଅନ୍ୟଥା
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ଯଦି (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	ଅନ୍ୟଥା
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ଯଦି (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ଯଦି (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	ଅନ୍ୟଥା
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ଯଦି (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ଯଦି (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ଯଦି (recovery) {
		ଯଦି (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		ଅନ୍ୟଥା
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ଯଦି (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ଯଦି (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ଯଦି (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_forked(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା ref_filter *filter = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);
	ଯଦି (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା stacked_branch_data {
	ସଂରଚନା strset *deletable_branch_names;
	ସଂରଚନା strset *protected_branch_names;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_stacked_branch_base(ଅଚଳ ସଂରଚନା reference *ref,
				       ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା stacked_branch_data *data = cb_data;
	ଅଚଳ ଅକ୍ଷର *branch_name;
	ସଂରଚନା branch *branch;
	ଅଚଳ ଅକ୍ଷର *upstream_refname;
	ଅଚଳ ଅକ୍ଷର *upstream_branch_name;

	ଯଦି (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ଯଦି (strset_contains(data->deletable_branch_names, branch_name))
		ଫେରନ୍ତୁ 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ଖାଲି);
	ଯଦି (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		ଫେରନ୍ତୁ 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ protect_stacked_branch_bases(ସଂରଚନା ref_store *refs,
					 ସଂରଚନା strset *deletable_branch_names,
					 ସଂରଚନା strset *protected_branch_names)
{
	ସଂରଚନା stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	ସଂରଚନା refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->ଚାବି);
}

ସ୍ଥିର ଶୂନ୍ୟ clear_deleted_upstreams(ସଂରଚନା strset *protected_branch_names,
				    ସଂରଚନା strset *deletable_branch_names)
{
	ସଂରଚନା strbuf ଚାବି = STRBUF_INIT;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		ସଂରଚନା branch *branch = branch_get(entry->ଚାବି);
		ଅଚଳ ଅକ୍ଷର *upstream_refname = branch_get_upstream(branch, ଖାଲି);
		ଅଚଳ ଅକ୍ଷର *upstream_branch_name;

		ଯଦି (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			ଜାରି;

		strbuf_addf(&ଚାବି, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, ଚାବି.buf, ଖାଲି);
		strbuf_reset(&ଚାବି);
		strbuf_addf(&ଚାବି, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, ଚାବି.buf, ଖାଲି);
		strbuf_reset(&ଚାବି);
	}

	strbuf_release(&ଚାବି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା branch_pushes_to_upstream(ସଂରଚନା branch *branch,
				     ଅଚଳ ଅକ୍ଷର *upstream)
{
	ସଂରଚନା remote *remote = remote_get(remote_for_branch(branch, ଖାଲି));
	ଅକ୍ଷର *ଠେଲ_refname = ଖାଲି;
	ଅକ୍ଷର *tracking = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (!remote)
		ଫେରନ୍ତୁ 0;
	ଯଦି (remote->ଠେଲ.nr)
		ଠେଲ_refname = apply_refspecs(&remote->ଠେଲ, branch->refname);
	ଅନ୍ୟଥା
		ଠେଲ_refname = xstrdup(branch->refname);
	ଯଦି (ଠେଲ_refname)
		tracking = apply_refspecs(&remote->fetch, ଠେଲ_refname);
	ଯଦି (tracking && !strcmp(tracking, upstream))
		ret = 1;

	ମୁକ୍ତ(ଠେଲ_refname);
	ମୁକ୍ତ(tracking);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_merged_branches(ଅଚଳ ସଂରଚନା strvec *upstreams,
				 ଅଚଳ ଅକ୍ଷର **argv, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା ref_store *refs = get_main_ref_store(the_repository);
	ସଂରଚନା ref_filter filter = REF_FILTER_INIT;
	ସଂରଚନା ref_array candidates = { 0 };
	ସଂରଚନା strset deletable_branch_names = STRSET_INIT;
	ସଂରଚନା strset protected_branch_names = STRSET_INIT;
	ସଂରଚନା strvec branches_to_delete = STRVEC_INIT;
	ସଂରଚନା strbuf ଚାବି = STRBUF_INIT;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା strmap_entry *entry;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ପାଇଁ (size_t i = 0; i < upstreams->nr; i++)
		ଯଦି (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < candidates.nr; i++) {
		ଅଚଳ ଅକ୍ଷର *branch_refname = candidates.ବସ୍ତୁଗୁଡ଼ିକ[i]->refname;
		ଅଚଳ ଅକ୍ଷର *branch_name;
		ସଂରଚନା branch *branch;
		ଅଚଳ ଅକ୍ଷର *upstream_refname;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_out;

		ଯଦି (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ଯଦି (branch_checked_out(branch_refname))
			ଜାରି;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ଖାଲି);
		ଯଦି (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			ଜାରି;
		ଯଦି (branch_pushes_to_upstream(branch, upstream_refname))
			ଜାରି;
		ଯଦି (check_branch_commit(branch_name, branch_name,
					&candidates.ବସ୍ତୁଗୁଡ଼ିକ[i]->objectname, ଖାଲି,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			ଜାରି;

		strbuf_reset(&ଚାବି);
		strbuf_addf(&ଚାବି, "branch.%s.deletemerged", branch_name);
		ଯଦି (!repo_config_get_bool(the_repository, ଚାବି.buf, &opt_out) &&
		    !opt_out) {
			ଯଦି (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			ଜାରି;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_ଠେଲ(&branches_to_delete, entry->ଚାବି);

	ଯଦି (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	ଯଦି (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&ଚାବି);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit_branch_description(ଅଚଳ ଅକ୍ଷର *branch_name)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exists;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ଯଦି (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_ଧାଡ଼ି_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_ଧାଡ଼ି_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ଯଦି (launch_editor(edit_description(), &buf, ଖାଲି)) {
		strbuf_release(&buf);
		ଫେରନ୍ତୁ -1;
	}
	strbuf_stripspace(&buf, comment_ଧାଡ଼ି_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ଯଦି (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : ଖାଲି);
	strbuf_release(&name);
	strbuf_release(&buf);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ die_if_upstream_looks_like_remote(ଅଚଳ ଅକ୍ଷର *new_upstream, ଅଚଳ ଅକ୍ଷର *branch_name)
{
	ସଂରଚନା strbuf remote_ref = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

	ଯଦି (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ଫେରନ୍ତୁ;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ଫେରନ୍ତୁ;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_branch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	/* possible actions */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	ସଂରଚନା strvec delete_merged = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0;
	ଅଚଳ ଅକ୍ଷର *new_upstream = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା noncreate_actions = 0;
	/* possible options */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	ଗଣନା branch_track track;
	ସଂରଚନା ref_filter filter = REF_FILTER_INIT;
	ସ୍ଥିର ସଂରଚନା ref_sorting *sorting;
	ସଂରଚନା string_list sorting_options = STRING_LIST_INIT_DUP;
	ସଂରଚନା ref_format format = REF_FORMAT_INIT;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସଂରଚନା option options[] = {
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
		OPT_BIT('D', ଖାଲି, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', ଖାଲି, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', ଖାଲି, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_ସନ୍ଦେଶ();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	ଯଦି (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, ଖାଲି);
	ଯଦି (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	ଯଦି (!strcmp(head, "HEAD"))
		filter.detached = 1;
	ଅନ୍ୟଥା ଯଦି (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	ଯଦି (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ଯଦି (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ଯଦି (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ଯଦି (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ଯଦି (recurse_submodules_explicit) {
		ଯଦି (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ଯଦି (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ଯଦି (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ଯଦି (filter.verbose) {
		ଯଦି (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ଯଦି (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ଯଦି (list)
		setup_auto_pager("branch", 1);

	ଯଦି (delete) {
		ଯଦି (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (show_current) {
		print_current_branch_name();
		ret = 0;
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ଯଦି ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, ଖାଲି);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (edit_description) {
		ଅଚଳ ଅକ୍ଷର *branch_name;
		ସଂରଚନା strbuf branch_ref = STRBUF_INIT;
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		ଯଦି (!argc) {
			ଯଦି (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} ଅନ୍ୟଥା ଯଦି (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} ଅନ୍ୟଥା {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} ଅନ୍ୟଥା ଯଦି (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} ଅନ୍ୟଥା {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (copy || rename) {
		ଯଦି (!argc)
			die(_("branch name required"));
		ଅନ୍ୟଥା ଯଦି ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		ଅନ୍ୟଥା ଯଦି (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		ଅନ୍ୟଥା ଯଦି (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		ଅନ୍ୟଥା
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} ଅନ୍ୟଥା ଯଦି (new_upstream) {
		ସଂରଚନା branch *branch;
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		ଯଦି (!argc)
			branch = branch_get(ଖାଲି);
		ଅନ୍ୟଥା ଯଦି (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ଅନ୍ୟଥା
			die(_("too many arguments to set new upstream"));

		ଯଦି (!branch) {
			ଯଦି (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ଯଦି (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ଯଦି (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} ଅନ୍ୟଥା ଯଦି (unset_upstream) {
		ସଂରଚନା branch *branch;
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		ଯଦି (!argc)
			branch = branch_get(ଖାଲି);
		ଅନ୍ୟଥା ଯଦି (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} ଅନ୍ୟଥା
			die(_("too many arguments to unset upstream"));

		ଯଦି (!branch) {
			ଯଦି (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ଯଦି (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ଖାଲି, ଖାଲି, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ଖାଲି, ଖାଲି, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} ଅନ୍ୟଥା ଯଦି (!noncreate_actions && argc > 0 && argc <= 2) {
		ଅଚଳ ଅକ୍ଷର *branch_name = argv[0];
		ଅଚଳ ଅକ୍ଷର *start_name = argc == 2 ? argv[1] : head;

		ଯଦି (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ଯଦି (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ଯଦି (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, ଖାଲି, force,
						    reflog, quiet, track, 0);
			ret = 0;
			ଯାଅ out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} ଅନ୍ୟଥା
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	ଫେରନ୍ତୁ ret;
}
