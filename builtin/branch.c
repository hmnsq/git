/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "color.h"
#include "editor.h"
#include "environment.h"
#include "refs.h"
#include "commit.h"
#include "gettext.h"
#include "object-name.h"
#include "remote.h"
#include "parse-options.h"
#include "branch.h"
#include "path.h"
#include "string-list.h"
#include "strmap.h"
#include "column.h"
#include "utf8.h"
#include "ref-filter.h"
#include "worktree.h"
#include "help.h"
#include "advice.h"
#include "commit-reach.h"

static const var_na * const builtin_branch_usage[] = {
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
	NULL
};

static const var_na *head;
static sa_nracanaa object_id head_oid;
static ចំនួនគត់ recurse_submodules = 0;
static ចំនួនគត់ submodule_propagate_branches = 0;

static ga_nanaa git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
static var_na branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
ga_nanaa color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

static const var_na *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

static sa_nracanaa string_list output = STRING_LIST_INIT_DUP;
static unsigned ចំនួនគត់ colopts;

define_list_config_array(color_branch_slots);

static ចំនួនគត់ git_branch_config(const var_na *var, const var_na *value,
			     const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *slot_name;

	ប្រសិនបើ (!strcmp(var, "branch.sort")) {
		ប្រសិនបើ (!value)
			ត្រឡប់ config_error_nonbool(var);
		string_list_append(cb, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (starts_with(var, "column."))
		ត្រឡប់ git_column_config(var, value, "branch", &colopts);
	ប្រសិនបើ (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, value);
		ត្រឡប់ 0;
	}
	ប្រសិនបើ (skip_prefix(var, "color.branch.", &slot_name)) {
		ចំនួនគត់ slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		ប្រសិនបើ (slot < 0)
			ត្រឡប់ 0;
		ប្រសិនបើ (!value)
			ត្រឡប់ config_error_nonbool(var);
		ត្រឡប់ color_parse(value, branch_colors[slot]);
	}
	ប្រសិនបើ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, value);
		ត្រឡប់ 0;
	}
	ប្រសិនបើ (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, value);
		ត្រឡប់ 0;
	}

	ប្រសិនបើ (git_color_config(var, value, cb) < 0)
		ត្រឡប់ -1;

	ត្រឡប់ git_default_config(var, value, ctx, cb);
}

static const var_na *branch_get_color(ga_nanaa color_branch ix)
{
	ប្រសិនបើ (want_color(branch_use_color))
		ត្រឡប់ branch_colors[ix];
	ត្រឡប់ "";
}

static ចំនួនគត់ branch_merged(ចំនួនគត់ kind, const var_na *name,
			 sa_nracanaa commit *rev, sa_nracanaa commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	sa_nracanaa commit *reference_rev = NULL;
	const var_na *reference_name = NULL;
	void *reference_name_to_free = NULL;
	ចំនួនគត់ merged;

	ប្រសិនបើ (kind == FILTER_REFS_BRANCHES) {
		sa_nracanaa branch *branch = branch_get(name);
		const var_na *upstream = branch_get_upstream(branch, NULL);
		sa_nracanaa object_id oid;

		ប្រសិនបើ (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, NULL)) != NULL)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ប្រសិនបើ (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	ប្រសិនបើ (merged < 0)
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
	ប្រសិនបើ (head_rev && head_rev != reference_rev) {
		ចំនួនគត់ expect = repo_in_merge_bases(the_repository, rev, head_rev);
		ប្រសិនបើ (expect < 0)
			exit(128);
		ប្រសិនបើ (expect == merged)
			; /* okay */
		បើមិនដូច្នេះ ប្រសិនបើ (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		បើមិនដូច្នេះ
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	free(reference_name_to_free);
	ត្រឡប់ merged;
}

ga_nanaa delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

static ចំនួនគត់ check_branch_commit(const var_na *branchname, const var_na *refname,
			       const sa_nracanaa object_id *oid, sa_nracanaa commit *head_rev,
			       ចំនួនគត់ kinds, unsigned ចំនួនគត់ flags)
{
	sa_nracanaa commit *rev = lookup_commit_reference(the_repository, oid);
	ប្រសិនបើ (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		ត្រឡប់ -1;
	}
	ប្រសិនបើ (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		ប្រសិនបើ (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		ត្រឡប់ -1;
	}
	ត្រឡប់ 0;
}

static void delete_branch_config(const var_na *branchname)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ប្រសិនបើ (repo_config_rename_section(the_repository, buf.buf, NULL) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

static ចំនួនគត់ delete_branches(ចំនួនគត់ argc, const var_na **argv, ចំនួនគត់ kinds,
			   unsigned ចំនួនគត់ flags)
{
	sa_nracanaa commit *head_rev = NULL;
	sa_nracanaa object_id oid;
	var_na *name = NULL;
	const var_na *fmt;
	ចំនួនគត់ i;
	ចំនួនគត់ ret = 0;
	ចំនួនគត់ remote_branch = 0;
	sa_nracanaa strbuf bname = STRBUF_INIT;
	ga_nanaa interpret_branch_kind allowed_interpret;
	sa_nracanaa string_list refs_to_delete = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list_item *item;
	ចំនួនគត់ branch_name_pos;
	const var_na *fmt_remotes = "refs/remotes/%s";

	switch (kinds) {
	pak_sa FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		bha_nga;
	pak_sa FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		bha_nga;
	default:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ប្រសិនបើ (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	សម្រាប់ (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		var_na *target = NULL;
		ចំនួនគត់ ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		free(name);
		name = mkpathdup(fmt, bname.buf);

		ប្រសិនបើ (kinds == FILTER_REFS_BRANCHES) {
			const var_na *path;
			ប្រសិនបើ ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				continue;
			}
			ប្រសិនបើ ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				continue;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		ប្រសិនបើ (!target) {
			ប្រសិនបើ (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} បើមិនដូច្នេះ {
				var_na *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				var_na *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ប្រសិនបើ (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				បើមិនដូច្នេះ
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			continue;
		}

		ប្រសិនបើ (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			ប្រសិនបើ (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			goto next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		free(target);
	}

	ប្រសិនបើ (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), NULL, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		var_na *describe_ref = item->util;
		var_na *name = item->string;
		ប្រសិនបើ (flags & DELETE_BRANCH_DRY_RUN) {
			ប្រសិនបើ (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} បើមិនដូច្នេះ ប្រសិនបើ (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			var_na *refname = name + branch_name_pos;
			ប្រសិនបើ (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		free(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	free(name);
	strbuf_release(&bname);

	ត្រឡប់ ret;
}

static ចំនួនគត់ calc_maxwidth(sa_nracanaa ref_array *refs, ចំនួនគត់ remote_bonus)
{
	ចំនួនគត់ i, max = 0;
	សម្រាប់ (i = 0; i < refs->nr; i++) {
		sa_nracanaa ref_array_item *it = refs->items[i];
		const var_na *desc = it->refname;
		ចំនួនគត់ w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		ប្រសិនបើ (it->kind == FILTER_REFS_DETACHED_HEAD) {
			var_na *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			free(head_desc);
		} បើមិនដូច្នេះ
			w = utf8_strwidth(desc);

		ប្រសិនបើ (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ប្រសិនបើ (w > max)
			max = w;
	}
	ត្រឡប់ max;
}

static const var_na *quote_literal_for_format(const var_na *s)
{
	static sa_nracanaa strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	ខណៈពេល (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	ត្រឡប់ buf.buf;
}

static var_na *build_format(sa_nracanaa ref_filter *filter, ចំនួនគត់ maxwidth, const var_na *remote_prefix)
{
	sa_nracanaa strbuf fmt = STRBUF_INIT;
	sa_nracanaa strbuf local = STRBUF_INIT;
	sa_nracanaa strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ប្រសិនបើ (filter->verbose) {
		sa_nracanaa strbuf obname = STRBUF_INIT;

		ប្រសិនបើ (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		បើមិនដូច្នេះ ប្រសិនបើ (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		បើមិនដូច្នេះ
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ប្រសិនបើ (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		បើមិនដូច្នេះ
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} បើមិនដូច្នេះ {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	ត្រឡប់ strbuf_detach(&fmt, NULL);
}

static void print_ref_list(sa_nracanaa ref_filter *filter, sa_nracanaa ref_sorting *sorting,
			   sa_nracanaa ref_format *format, sa_nracanaa string_list *output)
{
	ចំនួនគត់ i;
	sa_nracanaa ref_array array;
	ចំនួនគត់ maxwidth = 0;
	const var_na *remote_prefix = "";
	var_na *to_free = NULL;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ប្រសិនបើ (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, parimaa_na(array));

	filter_refs(&array, filter, filter->kind);

	ប្រសិនបើ (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	ប្រសិនបើ (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	ប្រសិនបើ (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	ប្រសិនបើ (column_active(colopts)) {
		sa_nracanaa strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		assert(!filter->verbose && "--column and --verbose are incompatible");

		សម្រាប់ (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			ប្រសិនបើ (format_ref_array_item(array.items[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} បើមិនដូច្នេះ {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	free(to_free);
}

static void print_current_branch_name(void)
{
	ចំនួនគត់ flags;
	const var_na *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, NULL, &flags);
	const var_na *shortname;
	ប្រសិនបើ (!refname)
		die(_("could not resolve HEAD"));
	បើមិនដូច្នេះ ប្រសិនបើ (!(flags & REF_ISSYMREF))
		ត្រឡប់;
	បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	បើមិនដូច្នេះ
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

static void reject_rebase_or_bisect_branch(sa_nracanaa worktree **worktrees,
					   const var_na *target)
{
	ចំនួនគត់ i;

	សម្រាប់ (i = 0; worktrees[i]; i++) {
		sa_nracanaa worktree *wt = worktrees[i];

		ប្រសិនបើ (!wt->is_detached)
			continue;

		ប្រសិនបើ (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		ប្រសិនបើ (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
static ចំនួនគត់ replace_each_worktree_head_symref(sa_nracanaa worktree **worktrees,
					     const var_na *oldref, const var_na *newref,
					     const var_na *logmsg)
{
	ចំនួនគត់ ret = 0;
	ចំនួនគត់ i;

	សម្រាប់ (i = 0; worktrees[i]; i++) {
		sa_nracanaa ref_store *refs;

		ប្រសិនបើ (worktrees[i]->is_detached)
			continue;
		ប្រសិនបើ (!worktrees[i]->head_ref)
			continue;
		ប្រសិនបើ (strcmp(oldref, worktrees[i]->head_ref))
			continue;

		refs = get_worktree_ref_store(worktrees[i]);
		ប្រសិនបើ (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	ត្រឡប់ ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

static void copy_or_rename_branch(const var_na *oldname, const var_na *newname, ចំនួនគត់ copy, ចំនួនគត់ force)
{
	sa_nracanaa strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	sa_nracanaa strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	const var_na *interpreted_oldname = NULL;
	const var_na *interpreted_newname = NULL;
	ចំនួនគត់ recovery = 0, oldref_usage = 0;
	sa_nracanaa worktree **worktrees = get_worktrees(the_repository);

	ប្រសិនបើ (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ប្រសិនបើ (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		បើមិនដូច្នេះ {
			ចំនួនគត់ code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	សម្រាប់ (ចំនួនគត់ i = 0; worktrees[i]; i++) {
		sa_nracanaa worktree *wt = worktrees[i];

		ប្រសិនបើ (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			ប្រសិនបើ (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			bha_nga;
		}
	}

	ប្រសិនបើ ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ប្រសិនបើ (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		បើមិនដូច្នេះ
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ប្រសិនបើ (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	បើមិនដូច្នេះ
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ប្រសិនបើ (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ប្រសិនបើ (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	បើមិនដូច្នេះ
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ប្រសិនបើ (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ប្រសិនបើ (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ប្រសិនបើ (recovery) {
		ប្រសិនបើ (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		បើមិនដូច្នេះ
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ប្រសិនបើ (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ប្រសិនបើ (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ប្រសិនបើ (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

static ចំនួនគត់ parse_opt_forked(const sa_nracanaa option *opt, const var_na *arg, ចំនួនគត់ unset)
{
	sa_nracanaa ref_filter *filter = opt->value;

	BUG_ON_OPT_NEG(unset);
	ប្រសិនបើ (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	ត្រឡប់ 0;
}

sa_nracanaa stacked_branch_data {
	sa_nracanaa strset *deletable_branch_names;
	sa_nracanaa strset *protected_branch_names;
};

static ចំនួនគត់ collect_stacked_branch_base(const sa_nracanaa reference *ref,
				       void *cb_data)
{
	sa_nracanaa stacked_branch_data *data = cb_data;
	const var_na *branch_name;
	sa_nracanaa branch *branch;
	const var_na *upstream_refname;
	const var_na *upstream_branch_name;

	ប្រសិនបើ (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ប្រសិនបើ (strset_contains(data->deletable_branch_names, branch_name))
		ត្រឡប់ 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, NULL);
	ប្រសិនបើ (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		ត្រឡប់ 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	ត្រឡប់ 0;
}

static void protect_stacked_branch_bases(sa_nracanaa ref_store *refs,
					 sa_nracanaa strset *deletable_branch_names,
					 sa_nracanaa strset *protected_branch_names)
{
	sa_nracanaa stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	sa_nracanaa refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->key);
}

static void clear_deleted_upstreams(sa_nracanaa strset *protected_branch_names,
				    sa_nracanaa strset *deletable_branch_names)
{
	sa_nracanaa strbuf key = STRBUF_INIT;
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		sa_nracanaa branch *branch = branch_get(entry->key);
		const var_na *upstream_refname = branch_get_upstream(branch, NULL);
		const var_na *upstream_branch_name;

		ប្រសិនបើ (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			continue;

		strbuf_addf(&key, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, key.buf, NULL);
		strbuf_reset(&key);
		strbuf_addf(&key, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, key.buf, NULL);
		strbuf_reset(&key);
	}

	strbuf_release(&key);
}

static ចំនួនគត់ branch_pushes_to_upstream(sa_nracanaa branch *branch,
				     const var_na *upstream)
{
	sa_nracanaa remote *remote = remote_get(remote_for_branch(branch, NULL));
	var_na *push_refname = NULL;
	var_na *tracking = NULL;
	ចំនួនគត់ ret = 0;

	ប្រសិនបើ (!remote)
		ត្រឡប់ 0;
	ប្រសិនបើ (remote->push.nr)
		push_refname = apply_refspecs(&remote->push, branch->refname);
	បើមិនដូច្នេះ
		push_refname = xstrdup(branch->refname);
	ប្រសិនបើ (push_refname)
		tracking = apply_refspecs(&remote->fetch, push_refname);
	ប្រសិនបើ (tracking && !strcmp(tracking, upstream))
		ret = 1;

	free(push_refname);
	free(tracking);
	ត្រឡប់ ret;
}

static ចំនួនគត់ delete_merged_branches(const sa_nracanaa strvec *upstreams,
				 const var_na **argv, unsigned ចំនួនគត់ flags)
{
	sa_nracanaa ref_store *refs = get_main_ref_store(the_repository);
	sa_nracanaa ref_filter filter = REF_FILTER_INIT;
	sa_nracanaa ref_array candidates = { 0 };
	sa_nracanaa strset deletable_branch_names = STRSET_INIT;
	sa_nracanaa strset protected_branch_names = STRSET_INIT;
	sa_nracanaa strvec branches_to_delete = STRVEC_INIT;
	sa_nracanaa strbuf key = STRBUF_INIT;
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa strmap_entry *entry;
	ចំនួនគត់ ret = 0;

	សម្រាប់ (size_t i = 0; i < upstreams->nr; i++)
		ប្រសិនបើ (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	សម្រាប់ (ចំនួនគត់ i = 0; i < candidates.nr; i++) {
		const var_na *branch_refname = candidates.items[i]->refname;
		const var_na *branch_name;
		sa_nracanaa branch *branch;
		const var_na *upstream_refname;
		ចំនួនគត់ opt_out;

		ប្រសិនបើ (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ប្រសិនបើ (branch_checked_out(branch_refname))
			continue;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, NULL);
		ប្រសិនបើ (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			continue;
		ប្រសិនបើ (branch_pushes_to_upstream(branch, upstream_refname))
			continue;
		ប្រសិនបើ (check_branch_commit(branch_name, branch_name,
					&candidates.items[i]->objectname, NULL,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			continue;

		strbuf_reset(&key);
		strbuf_addf(&key, "branch.%s.deletemerged", branch_name);
		ប្រសិនបើ (!repo_config_get_bool(the_repository, key.buf, &opt_out) &&
		    !opt_out) {
			ប្រសិនបើ (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			continue;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_push(&branches_to_delete, entry->key);

	ប្រសិនបើ (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	ប្រសិនបើ (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&key);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	ត្រឡប់ ret;
}

static GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

static ចំនួនគត់ edit_branch_description(const var_na *branch_name)
{
	ចំនួនគត់ exists;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ប្រសិនបើ (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_line_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_line_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ប្រសិនបើ (launch_editor(edit_description(), &buf, NULL)) {
		strbuf_release(&buf);
		ត្រឡប់ -1;
	}
	strbuf_stripspace(&buf, comment_line_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ប្រសិនបើ (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : NULL);
	strbuf_release(&name);
	strbuf_release(&buf);

	ត្រឡប់ 0;
}

static void die_if_upstream_looks_like_remote(const var_na *new_upstream, const var_na *branch_name)
{
	sa_nracanaa strbuf remote_ref = STRBUF_INIT;
	ចំនួនគត់ code;

	ប្រសិនបើ (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		ត្រឡប់;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ប្រសិនបើ (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		ត្រឡប់;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

ចំនួនគត់ cmd_branch(ចំនួនគត់ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	/* possible actions */
	ចំនួនគត់ delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	sa_nracanaa strvec delete_merged = STRVEC_INIT;
	ចំនួនគត់ dry_run = 0;
	const var_na *new_upstream = NULL;
	ចំនួនគត់ noncreate_actions = 0;
	/* possible options */
	ចំនួនគត់ reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	ga_nanaa branch_track track;
	sa_nracanaa ref_filter filter = REF_FILTER_INIT;
	static sa_nracanaa ref_sorting *sorting;
	sa_nracanaa string_list sorting_options = STRING_LIST_INIT_DUP;
	sa_nracanaa ref_format format = REF_FORMAT_INIT;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);
	ចំនួនគត់ ret;

	sa_nracanaa option options[] = {
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
		OPT_BIT('D', NULL, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', NULL, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', NULL, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_msg();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	ប្រសិនបើ (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, NULL);
	ប្រសិនបើ (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	ប្រសិនបើ (!strcmp(head, "HEAD"))
		filter.detached = 1;
	បើមិនដូច្នេះ ប្រសិនបើ (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	ប្រសិនបើ (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ប្រសិនបើ (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ប្រសិនបើ (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ប្រសិនបើ (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ប្រសិនបើ (recurse_submodules_explicit) {
		ប្រសិនបើ (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ប្រសិនបើ (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ប្រសិនបើ (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ប្រសិនបើ (filter.verbose) {
		ប្រសិនបើ (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ប្រសិនបើ (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ប្រសិនបើ (list)
		setup_auto_pager("branch", 1);

	ប្រសិនបើ (delete) {
		ប្រសិនបើ (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		goto out;
	} បើមិនដូច្នេះ ប្រសិនបើ (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		goto out;
	} បើមិនដូច្នេះ ប្រសិនបើ (show_current) {
		print_current_branch_name();
		ret = 0;
		goto out;
	} បើមិនដូច្នេះ ប្រសិនបើ (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ប្រសិនបើ ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, NULL);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		goto out;
	} បើមិនដូច្នេះ ប្រសិនបើ (edit_description) {
		const var_na *branch_name;
		sa_nracanaa strbuf branch_ref = STRBUF_INIT;
		sa_nracanaa strbuf buf = STRBUF_INIT;

		ប្រសិនបើ (!argc) {
			ប្រសិនបើ (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} បើមិនដូច្នេះ ប្រសិនបើ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} បើមិនដូច្នេះ {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ប្រសិនបើ (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} បើមិនដូច្នេះ ប្រសិនបើ (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} បើមិនដូច្នេះ {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		goto out;
	} បើមិនដូច្នេះ ប្រសិនបើ (copy || rename) {
		ប្រសិនបើ (!argc)
			die(_("branch name required"));
		បើមិនដូច្នេះ ប្រសិនបើ ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		បើមិនដូច្នេះ ប្រសិនបើ (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		បើមិនដូច្នេះ ប្រសិនបើ (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		បើមិនដូច្នេះ
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} បើមិនដូច្នេះ ប្រសិនបើ (new_upstream) {
		sa_nracanaa branch *branch;
		sa_nracanaa strbuf buf = STRBUF_INIT;

		ប្រសិនបើ (!argc)
			branch = branch_get(NULL);
		បើមិនដូច្នេះ ប្រសិនបើ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} បើមិនដូច្នេះ
			die(_("too many arguments to set new upstream"));

		ប្រសិនបើ (!branch) {
			ប្រសិនបើ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ប្រសិនបើ (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ប្រសិនបើ (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ប្រសិនបើ (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} បើមិនដូច្នេះ ប្រសិនបើ (unset_upstream) {
		sa_nracanaa branch *branch;
		sa_nracanaa strbuf buf = STRBUF_INIT;

		ប្រសិនបើ (!argc)
			branch = branch_get(NULL);
		បើមិនដូច្នេះ ប្រសិនបើ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} បើមិនដូច្នេះ
			die(_("too many arguments to unset upstream"));

		ប្រសិនបើ (!branch) {
			ប្រសិនបើ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ប្រសិនបើ (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, NULL, NULL, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, NULL, NULL, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} បើមិនដូច្នេះ ប្រសិនបើ (!noncreate_actions && argc > 0 && argc <= 2) {
		const var_na *branch_name = argv[0];
		const var_na *start_name = argc == 2 ? argv[1] : head;

		ប្រសិនបើ (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ប្រសិនបើ (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ប្រសិនបើ (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, NULL, force,
						    reflog, quiet, track, 0);
			ret = 0;
			goto out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} បើមិនដូច្នេះ
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	ត្រឡប់ ret;
}
