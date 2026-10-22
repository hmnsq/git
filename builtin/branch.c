/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "strmap.h"
#અન્તર્ભાવ "column.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "ref-filter.h"
#અન્તર્ભાવ "worktree.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "commit-reach.h"

સ્થિર અચલ અક્ષર * અચલ builtin_branch_usage[] = {
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
	ખાલી
};

સ્થિર અચલ અક્ષર *head;
સ્થિર સંરચના object_id head_oid;
સ્થિર પૂર્ણાંક recurse_submodules = 0;
સ્થિર પૂર્ણાંક submodule_propagate_branches = 0;

સ્થિર ગણના git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
સ્થિર અક્ષર branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
ગણના color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

સ્થિર અચલ અક્ષર *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

સ્થિર સંરચના string_list output = STRING_LIST_INIT_DUP;
સ્થિર unsigned પૂર્ણાંક colopts;

define_list_config_array(color_branch_slots);

સ્થિર પૂર્ણાંક git_branch_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			     અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	અચલ અક્ષર *slot_name;

	જો (!strcmp(var, "branch.sort")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		string_list_append(cb, કિંમત);
		પાછા 0;
	}

	જો (starts_with(var, "column."))
		પાછા git_column_config(var, કિંમત, "branch", &colopts);
	જો (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, કિંમત);
		પાછા 0;
	}
	જો (skip_prefix(var, "color.branch.", &slot_name)) {
		પૂર્ણાંક slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		જો (slot < 0)
			પાછા 0;
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		પાછા color_parse(કિંમત, branch_colors[slot]);
	}
	જો (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (git_color_config(var, કિંમત, cb) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર અચલ અક્ષર *branch_get_color(ગણના color_branch ix)
{
	જો (want_color(branch_use_color))
		પાછા branch_colors[ix];
	પાછા "";
}

સ્થિર પૂર્ણાંક branch_merged(પૂર્ણાંક kind, અચલ અક્ષર *name,
			 સંરચના commit *rev, સંરચના commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	સંરચના commit *reference_rev = ખાલી;
	અચલ અક્ષર *reference_name = ખાલી;
	શૂન્ય *reference_name_to_free = ખાલી;
	પૂર્ણાંક merged;

	જો (kind == FILTER_REFS_BRANCHES) {
		સંરચના branch *branch = branch_get(name);
		અચલ અક્ષર *upstream = branch_get_upstream(branch, ખાલી);
		સંરચના object_id oid;

		જો (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ખાલી)) != ખાલી)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	જો (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	જો (merged < 0)
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
	જો (head_rev && head_rev != reference_rev) {
		પૂર્ણાંક expect = repo_in_merge_bases(the_repository, rev, head_rev);
		જો (expect < 0)
			exit(128);
		જો (expect == merged)
			; /* okay */
		નહીંતર જો (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		નહીંતર
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	મુક્ત(reference_name_to_free);
	પાછા merged;
}

ગણના delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

સ્થિર પૂર્ણાંક check_branch_commit(અચલ અક્ષર *branchname, અચલ અક્ષર *refname,
			       અચલ સંરચના object_id *oid, સંરચના commit *head_rev,
			       પૂર્ણાંક kinds, unsigned પૂર્ણાંક flags)
{
	સંરચના commit *rev = lookup_commit_reference(the_repository, oid);
	જો (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		પાછા -1;
	}
	જો (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		જો (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		પાછા -1;
	}
	પાછા 0;
}

સ્થિર શૂન્ય delete_branch_config(અચલ અક્ષર *branchname)
{
	સંરચના strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	જો (repo_config_rename_section(the_repository, buf.buf, ખાલી) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

સ્થિર પૂર્ણાંક delete_branches(પૂર્ણાંક argc, અચલ અક્ષર **argv, પૂર્ણાંક kinds,
			   unsigned પૂર્ણાંક flags)
{
	સંરચના commit *head_rev = ખાલી;
	સંરચના object_id oid;
	અક્ષર *name = ખાલી;
	અચલ અક્ષર *fmt;
	પૂર્ણાંક i;
	પૂર્ણાંક ret = 0;
	પૂર્ણાંક remote_branch = 0;
	સંરચના strbuf bname = STRBUF_INIT;
	ગણના interpret_branch_kind allowed_interpret;
	સંરચના string_list refs_to_delete = STRING_LIST_INIT_DUP;
	સંરચના string_list_item *item;
	પૂર્ણાંક branch_name_pos;
	અચલ અક્ષર *fmt_remotes = "refs/remotes/%s";

	પસંદગી (kinds) {
	સ્થિતિ FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		તોડો;
	સ્થિતિ FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		તોડો;
	મૂળભૂત:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	જો (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	માટે (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		અક્ષર *target = ખાલી;
		પૂર્ણાંક ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		મુક્ત(name);
		name = mkpathdup(fmt, bname.buf);

		જો (kinds == FILTER_REFS_BRANCHES) {
			અચલ અક્ષર *path;
			જો ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				ચાલુ;
			}
			જો ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				ચાલુ;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		જો (!target) {
			જો (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} નહીંતર {
				અક્ષર *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				અક્ષર *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				જો (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				નહીંતર
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			ચાલુ;
		}

		જો (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			જો (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			જાઓ next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		મુક્ત(target);
	}

	જો (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ખાલી, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		અક્ષર *describe_ref = item->util;
		અક્ષર *name = item->string;
		જો (flags & DELETE_BRANCH_DRY_RUN) {
			જો (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} નહીંતર જો (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			અક્ષર *refname = name + branch_name_pos;
			જો (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		મુક્ત(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	મુક્ત(name);
	strbuf_release(&bname);

	પાછા ret;
}

સ્થિર પૂર્ણાંક calc_maxwidth(સંરચના ref_array *refs, પૂર્ણાંક remote_bonus)
{
	પૂર્ણાંક i, max = 0;
	માટે (i = 0; i < refs->nr; i++) {
		સંરચના ref_array_item *it = refs->વસ્તુઓ[i];
		અચલ અક્ષર *desc = it->refname;
		પૂર્ણાંક w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		જો (it->kind == FILTER_REFS_DETACHED_HEAD) {
			અક્ષર *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			મુક્ત(head_desc);
		} નહીંતર
			w = utf8_strwidth(desc);

		જો (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		જો (w > max)
			max = w;
	}
	પાછા max;
}

સ્થિર અચલ અક્ષર *quote_literal_for_format(અચલ અક્ષર *s)
{
	સ્થિર સંરચના strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	જ્યારે (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	પાછા buf.buf;
}

સ્થિર અક્ષર *build_format(સંરચના ref_filter *filter, પૂર્ણાંક maxwidth, અચલ અક્ષર *remote_prefix)
{
	સંરચના strbuf fmt = STRBUF_INIT;
	સંરચના strbuf local = STRBUF_INIT;
	સંરચના strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	જો (filter->verbose) {
		સંરચના strbuf obname = STRBUF_INIT;

		જો (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		નહીંતર જો (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		નહીંતર
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		જો (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		નહીંતર
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} નહીંતર {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	પાછા strbuf_detach(&fmt, ખાલી);
}

સ્થિર શૂન્ય print_ref_list(સંરચના ref_filter *filter, સંરચના ref_sorting *sorting,
			   સંરચના ref_format *format, સંરચના string_list *output)
{
	પૂર્ણાંક i;
	સંરચના ref_array array;
	પૂર્ણાંક maxwidth = 0;
	અચલ અક્ષર *remote_prefix = "";
	અક્ષર *to_free = ખાલી;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	જો (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, કદ(array));

	filter_refs(&array, filter, filter->kind);

	જો (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	જો (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	જો (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	જો (column_active(colopts)) {
		સંરચના strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		પ્રતિજ્ઞા(!filter->verbose && "--column and --verbose are incompatible");

		માટે (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			જો (format_ref_array_item(array.વસ્તુઓ[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} નહીંતર {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	મુક્ત(to_free);
}

સ્થિર શૂન્ય print_current_branch_name(શૂન્ય)
{
	પૂર્ણાંક flags;
	અચલ અક્ષર *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ખાલી, &flags);
	અચલ અક્ષર *shortname;
	જો (!refname)
		die(_("could not resolve HEAD"));
	નહીંતર જો (!(flags & REF_ISSYMREF))
		પાછા;
	નહીંતર જો (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	નહીંતર
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

સ્થિર શૂન્ય reject_rebase_or_bisect_branch(સંરચના worktree **worktrees,
					   અચલ અક્ષર *target)
{
	પૂર્ણાંક i;

	માટે (i = 0; worktrees[i]; i++) {
		સંરચના worktree *wt = worktrees[i];

		જો (!wt->is_detached)
			ચાલુ;

		જો (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		જો (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
સ્થિર પૂર્ણાંક replace_each_worktree_head_symref(સંરચના worktree **worktrees,
					     અચલ અક્ષર *oldref, અચલ અક્ષર *newref,
					     અચલ અક્ષર *logmsg)
{
	પૂર્ણાંક ret = 0;
	પૂર્ણાંક i;

	માટે (i = 0; worktrees[i]; i++) {
		સંરચના ref_store *refs;

		જો (worktrees[i]->is_detached)
			ચાલુ;
		જો (!worktrees[i]->head_ref)
			ચાલુ;
		જો (strcmp(oldref, worktrees[i]->head_ref))
			ચાલુ;

		refs = get_worktree_ref_store(worktrees[i]);
		જો (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	પાછા ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

સ્થિર શૂન્ય copy_or_rename_branch(અચલ અક્ષર *oldname, અચલ અક્ષર *newname, પૂર્ણાંક copy, પૂર્ણાંક force)
{
	સંરચના strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	સંરચના strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	અચલ અક્ષર *interpreted_oldname = ખાલી;
	અચલ અક્ષર *interpreted_newname = ખાલી;
	પૂર્ણાંક recovery = 0, oldref_usage = 0;
	સંરચના worktree **worktrees = get_worktrees(the_repository);

	જો (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		જો (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		નહીંતર {
			પૂર્ણાંક code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	માટે (પૂર્ણાંક i = 0; worktrees[i]; i++) {
		સંરચના worktree *wt = worktrees[i];

		જો (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			જો (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			તોડો;
		}
	}

	જો ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		જો (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		નહીંતર
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	જો (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	નહીંતર
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	જો (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	જો (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	નહીંતર
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	જો (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	જો (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	જો (recovery) {
		જો (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		નહીંતર
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	જો (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	જો (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	જો (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

સ્થિર પૂર્ણાંક parse_opt_forked(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના ref_filter *filter = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);
	જો (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	પાછા 0;
}

સંરચના stacked_branch_data {
	સંરચના strset *deletable_branch_names;
	સંરચના strset *protected_branch_names;
};

સ્થિર પૂર્ણાંક collect_stacked_branch_base(અચલ સંરચના reference *ref,
				       શૂન્ય *cb_data)
{
	સંરચના stacked_branch_data *data = cb_data;
	અચલ અક્ષર *branch_name;
	સંરચના branch *branch;
	અચલ અક્ષર *upstream_refname;
	અચલ અક્ષર *upstream_branch_name;

	જો (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	જો (strset_contains(data->deletable_branch_names, branch_name))
		પાછા 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ખાલી);
	જો (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		પાછા 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	પાછા 0;
}

સ્થિર શૂન્ય protect_stacked_branch_bases(સંરચના ref_store *refs,
					 સંરચના strset *deletable_branch_names,
					 સંરચના strset *protected_branch_names)
{
	સંરચના stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	સંરચના refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	સંરચના hashmap_iter iter;
	સંરચના strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->ચાવી);
}

સ્થિર શૂન્ય clear_deleted_upstreams(સંરચના strset *protected_branch_names,
				    સંરચના strset *deletable_branch_names)
{
	સંરચના strbuf ચાવી = STRBUF_INIT;
	સંરચના hashmap_iter iter;
	સંરચના strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		સંરચના branch *branch = branch_get(entry->ચાવી);
		અચલ અક્ષર *upstream_refname = branch_get_upstream(branch, ખાલી);
		અચલ અક્ષર *upstream_branch_name;

		જો (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			ચાલુ;

		strbuf_addf(&ચાવી, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, ચાવી.buf, ખાલી);
		strbuf_reset(&ચાવી);
		strbuf_addf(&ચાવી, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, ચાવી.buf, ખાલી);
		strbuf_reset(&ચાવી);
	}

	strbuf_release(&ચાવી);
}

સ્થિર પૂર્ણાંક branch_pushes_to_upstream(સંરચના branch *branch,
				     અચલ અક્ષર *upstream)
{
	સંરચના remote *remote = remote_get(remote_for_branch(branch, ખાલી));
	અક્ષર *ધકેલો_refname = ખાલી;
	અક્ષર *tracking = ખાલી;
	પૂર્ણાંક ret = 0;

	જો (!remote)
		પાછા 0;
	જો (remote->ધકેલો.nr)
		ધકેલો_refname = apply_refspecs(&remote->ધકેલો, branch->refname);
	નહીંતર
		ધકેલો_refname = xstrdup(branch->refname);
	જો (ધકેલો_refname)
		tracking = apply_refspecs(&remote->fetch, ધકેલો_refname);
	જો (tracking && !strcmp(tracking, upstream))
		ret = 1;

	મુક્ત(ધકેલો_refname);
	મુક્ત(tracking);
	પાછા ret;
}

સ્થિર પૂર્ણાંક delete_merged_branches(અચલ સંરચના strvec *upstreams,
				 અચલ અક્ષર **argv, unsigned પૂર્ણાંક flags)
{
	સંરચના ref_store *refs = get_main_ref_store(the_repository);
	સંરચના ref_filter filter = REF_FILTER_INIT;
	સંરચના ref_array candidates = { 0 };
	સંરચના strset deletable_branch_names = STRSET_INIT;
	સંરચના strset protected_branch_names = STRSET_INIT;
	સંરચના strvec branches_to_delete = STRVEC_INIT;
	સંરચના strbuf ચાવી = STRBUF_INIT;
	સંરચના hashmap_iter iter;
	સંરચના strmap_entry *entry;
	પૂર્ણાંક ret = 0;

	માટે (size_t i = 0; i < upstreams->nr; i++)
		જો (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	માટે (પૂર્ણાંક i = 0; i < candidates.nr; i++) {
		અચલ અક્ષર *branch_refname = candidates.વસ્તુઓ[i]->refname;
		અચલ અક્ષર *branch_name;
		સંરચના branch *branch;
		અચલ અક્ષર *upstream_refname;
		પૂર્ણાંક opt_out;

		જો (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		જો (branch_checked_out(branch_refname))
			ચાલુ;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ખાલી);
		જો (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			ચાલુ;
		જો (branch_pushes_to_upstream(branch, upstream_refname))
			ચાલુ;
		જો (check_branch_commit(branch_name, branch_name,
					&candidates.વસ્તુઓ[i]->objectname, ખાલી,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			ચાલુ;

		strbuf_reset(&ચાવી);
		strbuf_addf(&ચાવી, "branch.%s.deletemerged", branch_name);
		જો (!repo_config_get_bool(the_repository, ચાવી.buf, &opt_out) &&
		    !opt_out) {
			જો (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			ચાલુ;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_ધકેલો(&branches_to_delete, entry->ચાવી);

	જો (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	જો (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&ચાવી);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	પાછા ret;
}

સ્થિર GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

સ્થિર પૂર્ણાંક edit_branch_description(અચલ અક્ષર *branch_name)
{
	પૂર્ણાંક exists;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	જો (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_લીટી_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_લીટી_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	જો (launch_editor(edit_description(), &buf, ખાલી)) {
		strbuf_release(&buf);
		પાછા -1;
	}
	strbuf_stripspace(&buf, comment_લીટી_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	જો (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : ખાલી);
	strbuf_release(&name);
	strbuf_release(&buf);

	પાછા 0;
}

સ્થિર શૂન્ય die_if_upstream_looks_like_remote(અચલ અક્ષર *new_upstream, અચલ અક્ષર *branch_name)
{
	સંરચના strbuf remote_ref = STRBUF_INIT;
	પૂર્ણાંક code;

	જો (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		પાછા;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	જો (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		પાછા;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

પૂર્ણાંક cmd_branch(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	/* possible actions */
	પૂર્ણાંક delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	સંરચના strvec delete_merged = STRVEC_INIT;
	પૂર્ણાંક dry_run = 0;
	અચલ અક્ષર *new_upstream = ખાલી;
	પૂર્ણાંક noncreate_actions = 0;
	/* possible options */
	પૂર્ણાંક reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	ગણના branch_track track;
	સંરચના ref_filter filter = REF_FILTER_INIT;
	સ્થિર સંરચના ref_sorting *sorting;
	સંરચના string_list sorting_options = STRING_LIST_INIT_DUP;
	સંરચના ref_format format = REF_FORMAT_INIT;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);
	પૂર્ણાંક ret;

	સંરચના option options[] = {
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
		OPT_BIT('D', ખાલી, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', ખાલી, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', ખાલી, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_સંદેશ();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	જો (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, ખાલી);
	જો (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	જો (!strcmp(head, "HEAD"))
		filter.detached = 1;
	નહીંતર જો (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	જો (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	જો (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	જો (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	જો (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	જો (recurse_submodules_explicit) {
		જો (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		જો (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	જો (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	જો (filter.verbose) {
		જો (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	જો (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	જો (list)
		setup_auto_pager("branch", 1);

	જો (delete) {
		જો (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		જાઓ out;
	} નહીંતર જો (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		જાઓ out;
	} નહીંતર જો (show_current) {
		print_current_branch_name();
		ret = 0;
		જાઓ out;
	} નહીંતર જો (list) {
		/*  git branch --list also shows HEAD when it is detached */
		જો ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, ખાલી);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		જાઓ out;
	} નહીંતર જો (edit_description) {
		અચલ અક્ષર *branch_name;
		સંરચના strbuf branch_ref = STRBUF_INIT;
		સંરચના strbuf buf = STRBUF_INIT;

		જો (!argc) {
			જો (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} નહીંતર જો (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} નહીંતર {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		જો (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} નહીંતર જો (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} નહીંતર {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		જાઓ out;
	} નહીંતર જો (copy || rename) {
		જો (!argc)
			die(_("branch name required"));
		નહીંતર જો ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		નહીંતર જો (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		નહીંતર જો (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		નહીંતર
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} નહીંતર જો (new_upstream) {
		સંરચના branch *branch;
		સંરચના strbuf buf = STRBUF_INIT;

		જો (!argc)
			branch = branch_get(ખાલી);
		નહીંતર જો (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} નહીંતર
			die(_("too many arguments to set new upstream"));

		જો (!branch) {
			જો (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		જો (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			જો (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			જો (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} નહીંતર જો (unset_upstream) {
		સંરચના branch *branch;
		સંરચના strbuf buf = STRBUF_INIT;

		જો (!argc)
			branch = branch_get(ખાલી);
		નહીંતર જો (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} નહીંતર
			die(_("too many arguments to unset upstream"));

		જો (!branch) {
			જો (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		જો (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ખાલી, ખાલી, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, ખાલી, ખાલી, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} નહીંતર જો (!noncreate_actions && argc > 0 && argc <= 2) {
		અચલ અક્ષર *branch_name = argv[0];
		અચલ અક્ષર *start_name = argc == 2 ? argv[1] : head;

		જો (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		જો (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		જો (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, ખાલી, force,
						    reflog, quiet, track, 0);
			ret = 0;
			જાઓ out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} નહીંતર
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	પાછા ret;
}
