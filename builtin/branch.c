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

static const char * const builtin_branch_usage[] = {
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

static const char *head;
static struct object_id head_oid;
static цео recurse_submodules = 0;
static цео submodule_propagate_branches = 0;

static enum git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
static char branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
enum color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

static const char *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

static struct string_list output = STRING_LIST_INIT_DUP;
static unsigned цео colopts;

define_list_config_array(color_branch_slots);

static цео git_branch_config(const char *var, const char *value,
			     const struct config_context *ctx, void *cb)
{
	const char *slot_name;

	ако (!strcmp(var, "branch.sort")) {
		ако (!value)
			врати config_error_nonbool(var);
		string_list_append(cb, value);
		врати 0;
	}

	ако (starts_with(var, "column."))
		врати git_column_config(var, value, "branch", &colopts);
	ако (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, value);
		врати 0;
	}
	ако (skip_prefix(var, "color.branch.", &slot_name)) {
		цео slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		ако (slot < 0)
			врати 0;
		ако (!value)
			врати config_error_nonbool(var);
		врати color_parse(value, branch_colors[slot]);
	}
	ако (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, value);
		врати 0;
	}
	ако (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, value);
		врати 0;
	}

	ако (git_color_config(var, value, cb) < 0)
		врати -1;

	врати git_default_config(var, value, ctx, cb);
}

static const char *branch_get_color(enum color_branch ix)
{
	ако (want_color(branch_use_color))
		врати branch_colors[ix];
	врати "";
}

static цео branch_merged(цео kind, const char *name,
			 struct commit *rev, struct commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	struct commit *reference_rev = NULL;
	const char *reference_name = NULL;
	void *reference_name_to_free = NULL;
	цео merged;

	ако (kind == FILTER_REFS_BRANCHES) {
		struct branch *branch = branch_get(name);
		const char *upstream = branch_get_upstream(branch, NULL);
		struct object_id oid;

		ако (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, NULL)) != NULL)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ако (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	ако (merged < 0)
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
	ако (head_rev && head_rev != reference_rev) {
		цео expect = repo_in_merge_bases(the_repository, rev, head_rev);
		ако (expect < 0)
			exit(128);
		ако (expect == merged)
			; /* okay */
		инако ако (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		инако
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	free(reference_name_to_free);
	врати merged;
}

enum delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

static цео check_branch_commit(const char *branchname, const char *refname,
			       const struct object_id *oid, struct commit *head_rev,
			       цео kinds, unsigned цео flags)
{
	struct commit *rev = lookup_commit_reference(the_repository, oid);
	ако (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		врати -1;
	}
	ако (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		ако (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		врати -1;
	}
	врати 0;
}

static void delete_branch_config(const char *branchname)
{
	struct strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ако (repo_config_rename_section(the_repository, buf.buf, NULL) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

static цео delete_branches(цео argc, const char **argv, цео kinds,
			   unsigned цео flags)
{
	struct commit *head_rev = NULL;
	struct object_id oid;
	char *name = NULL;
	const char *fmt;
	цео i;
	цео ret = 0;
	цео remote_branch = 0;
	struct strbuf bname = STRBUF_INIT;
	enum interpret_branch_kind allowed_interpret;
	struct string_list refs_to_delete = STRING_LIST_INIT_DUP;
	struct string_list_item *item;
	цео branch_name_pos;
	const char *fmt_remotes = "refs/remotes/%s";

	switch (kinds) {
	case FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		break;
	case FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		break;
	default:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ако (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	за (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		char *target = NULL;
		цео ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		free(name);
		name = mkpathdup(fmt, bname.buf);

		ако (kinds == FILTER_REFS_BRANCHES) {
			const char *path;
			ако ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				continue;
			}
			ако ((path = branch_checked_out(name))) {
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
		ако (!target) {
			ако (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} инако {
				char *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				char *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ако (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				инако
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			continue;
		}

		ако (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			ако (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
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

	ако (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), NULL, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		char *describe_ref = item->util;
		char *name = item->string;
		ако (flags & DELETE_BRANCH_DRY_RUN) {
			ако (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} инако ако (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			char *refname = name + branch_name_pos;
			ако (!(flags & DELETE_BRANCH_QUIET))
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

	врати ret;
}

static цео calc_maxwidth(struct ref_array *refs, цео remote_bonus)
{
	цео i, max = 0;
	за (i = 0; i < refs->nr; i++) {
		struct ref_array_item *it = refs->items[i];
		const char *desc = it->refname;
		цео w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		ако (it->kind == FILTER_REFS_DETACHED_HEAD) {
			char *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			free(head_desc);
		} инако
			w = utf8_strwidth(desc);

		ако (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ако (w > max)
			max = w;
	}
	врати max;
}

static const char *quote_literal_for_format(const char *s)
{
	static struct strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	док (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	врати buf.buf;
}

static char *build_format(struct ref_filter *filter, цео maxwidth, const char *remote_prefix)
{
	struct strbuf fmt = STRBUF_INIT;
	struct strbuf local = STRBUF_INIT;
	struct strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ако (filter->verbose) {
		struct strbuf obname = STRBUF_INIT;

		ако (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		инако ако (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		инако
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ако (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		инако
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} инако {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	врати strbuf_detach(&fmt, NULL);
}

static void print_ref_list(struct ref_filter *filter, struct ref_sorting *sorting,
			   struct ref_format *format, struct string_list *output)
{
	цео i;
	struct ref_array array;
	цео maxwidth = 0;
	const char *remote_prefix = "";
	char *to_free = NULL;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ако (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, sizeof(array));

	filter_refs(&array, filter, filter->kind);

	ако (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	ако (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	ако (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	ако (column_active(colopts)) {
		struct strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		assert(!filter->verbose && "--column and --verbose are incompatible");

		за (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			ако (format_ref_array_item(array.items[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} инако {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	free(to_free);
}

static void print_current_branch_name(void)
{
	цео flags;
	const char *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, NULL, &flags);
	const char *shortname;
	ако (!refname)
		die(_("could not resolve HEAD"));
	инако ако (!(flags & REF_ISSYMREF))
		врати;
	инако ако (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	инако
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

static void reject_rebase_or_bisect_branch(struct worktree **worktrees,
					   const char *target)
{
	цео i;

	за (i = 0; worktrees[i]; i++) {
		struct worktree *wt = worktrees[i];

		ако (!wt->is_detached)
			continue;

		ако (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		ако (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
static цео replace_each_worktree_head_symref(struct worktree **worktrees,
					     const char *oldref, const char *newref,
					     const char *logmsg)
{
	цео ret = 0;
	цео i;

	за (i = 0; worktrees[i]; i++) {
		struct ref_store *refs;

		ако (worktrees[i]->is_detached)
			continue;
		ако (!worktrees[i]->head_ref)
			continue;
		ако (strcmp(oldref, worktrees[i]->head_ref))
			continue;

		refs = get_worktree_ref_store(worktrees[i]);
		ако (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	врати ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

static void copy_or_rename_branch(const char *oldname, const char *newname, цео copy, цео force)
{
	struct strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	struct strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	const char *interpreted_oldname = NULL;
	const char *interpreted_newname = NULL;
	цео recovery = 0, oldref_usage = 0;
	struct worktree **worktrees = get_worktrees(the_repository);

	ако (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ако (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		инако {
			цео code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	за (цео i = 0; worktrees[i]; i++) {
		struct worktree *wt = worktrees[i];

		ако (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			ако (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			break;
		}
	}

	ако ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ако (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		инако
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ако (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	инако
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ако (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ако (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	инако
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ако (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ако (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ако (recovery) {
		ако (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		инако
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ако (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ако (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ако (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

static цео parse_opt_forked(const struct option *opt, const char *arg, цео unset)
{
	struct ref_filter *filter = opt->value;

	BUG_ON_OPT_NEG(unset);
	ако (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	врати 0;
}

struct stacked_branch_data {
	struct strset *deletable_branch_names;
	struct strset *protected_branch_names;
};

static цео collect_stacked_branch_base(const struct reference *ref,
				       void *cb_data)
{
	struct stacked_branch_data *data = cb_data;
	const char *branch_name;
	struct branch *branch;
	const char *upstream_refname;
	const char *upstream_branch_name;

	ако (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ако (strset_contains(data->deletable_branch_names, branch_name))
		врати 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, NULL);
	ако (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		врати 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	врати 0;
}

static void protect_stacked_branch_bases(struct ref_store *refs,
					 struct strset *deletable_branch_names,
					 struct strset *protected_branch_names)
{
	struct stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	struct refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	struct hashmap_iter iter;
	struct strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->key);
}

static void clear_deleted_upstreams(struct strset *protected_branch_names,
				    struct strset *deletable_branch_names)
{
	struct strbuf key = STRBUF_INIT;
	struct hashmap_iter iter;
	struct strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		struct branch *branch = branch_get(entry->key);
		const char *upstream_refname = branch_get_upstream(branch, NULL);
		const char *upstream_branch_name;

		ако (!upstream_refname ||
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

static цео branch_pushes_to_upstream(struct branch *branch,
				     const char *upstream)
{
	struct remote *remote = remote_get(remote_for_branch(branch, NULL));
	char *push_refname = NULL;
	char *tracking = NULL;
	цео ret = 0;

	ако (!remote)
		врати 0;
	ако (remote->push.nr)
		push_refname = apply_refspecs(&remote->push, branch->refname);
	инако
		push_refname = xstrdup(branch->refname);
	ако (push_refname)
		tracking = apply_refspecs(&remote->fetch, push_refname);
	ако (tracking && !strcmp(tracking, upstream))
		ret = 1;

	free(push_refname);
	free(tracking);
	врати ret;
}

static цео delete_merged_branches(const struct strvec *upstreams,
				 const char **argv, unsigned цео flags)
{
	struct ref_store *refs = get_main_ref_store(the_repository);
	struct ref_filter filter = REF_FILTER_INIT;
	struct ref_array candidates = { 0 };
	struct strset deletable_branch_names = STRSET_INIT;
	struct strset protected_branch_names = STRSET_INIT;
	struct strvec branches_to_delete = STRVEC_INIT;
	struct strbuf key = STRBUF_INIT;
	struct hashmap_iter iter;
	struct strmap_entry *entry;
	цео ret = 0;

	за (size_t i = 0; i < upstreams->nr; i++)
		ако (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	за (цео i = 0; i < candidates.nr; i++) {
		const char *branch_refname = candidates.items[i]->refname;
		const char *branch_name;
		struct branch *branch;
		const char *upstream_refname;
		цео opt_out;

		ако (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ако (branch_checked_out(branch_refname))
			continue;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, NULL);
		ако (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			continue;
		ако (branch_pushes_to_upstream(branch, upstream_refname))
			continue;
		ако (check_branch_commit(branch_name, branch_name,
					&candidates.items[i]->objectname, NULL,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			continue;

		strbuf_reset(&key);
		strbuf_addf(&key, "branch.%s.deletemerged", branch_name);
		ако (!repo_config_get_bool(the_repository, key.buf, &opt_out) &&
		    !opt_out) {
			ако (!(flags & DELETE_BRANCH_QUIET))
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

	ако (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	ако (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&key);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	врати ret;
}

static GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

static цео edit_branch_description(const char *branch_name)
{
	цео exists;
	struct strbuf buf = STRBUF_INIT;
	struct strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ако (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_line_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_line_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ако (launch_editor(edit_description(), &buf, NULL)) {
		strbuf_release(&buf);
		врати -1;
	}
	strbuf_stripspace(&buf, comment_line_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ако (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : NULL);
	strbuf_release(&name);
	strbuf_release(&buf);

	врати 0;
}

static void die_if_upstream_looks_like_remote(const char *new_upstream, const char *branch_name)
{
	struct strbuf remote_ref = STRBUF_INIT;
	цео code;

	ако (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		врати;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ако (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		врати;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

цео cmd_branch(цео argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	/* possible actions */
	цео delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	struct strvec delete_merged = STRVEC_INIT;
	цео dry_run = 0;
	const char *new_upstream = NULL;
	цео noncreate_actions = 0;
	/* possible options */
	цео reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	enum branch_track track;
	struct ref_filter filter = REF_FILTER_INIT;
	static struct ref_sorting *sorting;
	struct string_list sorting_options = STRING_LIST_INIT_DUP;
	struct ref_format format = REF_FORMAT_INIT;
	struct repo_config_values *cfg = repo_config_values(the_repository);
	цео ret;

	struct option options[] = {
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
	ако (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, NULL);
	ако (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	ако (!strcmp(head, "HEAD"))
		filter.detached = 1;
	инако ако (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	ако (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ако (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ако (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ако (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ако (recurse_submodules_explicit) {
		ако (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ако (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ако (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ако (filter.verbose) {
		ако (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ако (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ако (list)
		setup_auto_pager("branch", 1);

	ако (delete) {
		ако (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		goto out;
	} инако ако (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		goto out;
	} инако ако (show_current) {
		print_current_branch_name();
		ret = 0;
		goto out;
	} инако ако (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ако ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
	} инако ако (edit_description) {
		const char *branch_name;
		struct strbuf branch_ref = STRBUF_INIT;
		struct strbuf buf = STRBUF_INIT;

		ако (!argc) {
			ако (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} инако ако (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} инако {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ако (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} инако ако (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} инако {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		goto out;
	} инако ако (copy || rename) {
		ако (!argc)
			die(_("branch name required"));
		инако ако ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		инако ако (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		инако ако (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		инако
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} инако ако (new_upstream) {
		struct branch *branch;
		struct strbuf buf = STRBUF_INIT;

		ако (!argc)
			branch = branch_get(NULL);
		инако ако (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} инако
			die(_("too many arguments to set new upstream"));

		ако (!branch) {
			ако (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ако (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ако (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ако (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} инако ако (unset_upstream) {
		struct branch *branch;
		struct strbuf buf = STRBUF_INIT;

		ако (!argc)
			branch = branch_get(NULL);
		инако ако (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} инако
			die(_("too many arguments to unset upstream"));

		ако (!branch) {
			ако (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ако (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, NULL, NULL, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, NULL, NULL, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} инако ако (!noncreate_actions && argc > 0 && argc <= 2) {
		const char *branch_name = argv[0];
		const char *start_name = argc == 2 ? argv[1] : head;

		ако (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ако (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ако (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, NULL, force,
						    reflog, quiet, track, 0);
			ret = 0;
			goto out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} инако
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	врати ret;
}
