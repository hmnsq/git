/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "strmap.h"
#அந்தர்பாவ "column.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "ref-filter.h"
#அந்தர்பாவ "worktree.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "commit-reach.h"

நிலையான மாறிலி எழுத்து * மாறிலி builtin_branch_usage[] = {
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
	வெற்று
};

நிலையான மாறிலி எழுத்து *head;
நிலையான கட்டமைப்பு object_id head_oid;
நிலையான முழு recurse_submodules = 0;
நிலையான முழு submodule_propagate_branches = 0;

நிலையான எண்ணிக்கை git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
நிலையான எழுத்து branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_NORMAL,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_NORMAL,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
எண்ணிக்கை color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

நிலையான மாறிலி எழுத்து *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

நிலையான கட்டமைப்பு string_list output = STRING_LIST_INIT_DUP;
நிலையான unsigned முழு colopts;

define_list_config_array(color_branch_slots);

நிலையான முழு git_branch_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			     மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	மாறிலி எழுத்து *slot_name;

	எனில் (!strcmp(var, "branch.sort")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		string_list_append(cb, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (starts_with(var, "column."))
		திரும்பு git_column_config(var, மதிப்பு, "branch", &colopts);
	எனில் (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (skip_prefix(var, "color.branch.", &slot_name)) {
		முழு slot = LOOKUP_CONFIG(color_branch_slots, slot_name);
		எனில் (slot < 0)
			திரும்பு 0;
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		திரும்பு color_parse(மதிப்பு, branch_colors[slot]);
	}
	எனில் (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (git_color_config(var, மதிப்பு, cb) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான மாறிலி எழுத்து *branch_get_color(எண்ணிக்கை color_branch ix)
{
	எனில் (want_color(branch_use_color))
		திரும்பு branch_colors[ix];
	திரும்பு "";
}

நிலையான முழு branch_merged(முழு kind, மாறிலி எழுத்து *name,
			 கட்டமைப்பு commit *rev, கட்டமைப்பு commit *head_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	கட்டமைப்பு commit *reference_rev = வெற்று;
	மாறிலி எழுத்து *reference_name = வெற்று;
	வெறுமை *reference_name_to_free = வெற்று;
	முழு merged;

	எனில் (kind == FILTER_REFS_BRANCHES) {
		கட்டமைப்பு branch *branch = branch_get(name);
		மாறிலி எழுத்து *upstream = branch_get_upstream(branch, வெற்று);
		கட்டமைப்பு object_id oid;

		எனில் (upstream &&
		    (reference_name = reference_name_to_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, வெற்று)) != வெற்று)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	எனில் (!reference_rev)
		reference_rev = head_rev;

	merged = reference_rev ? repo_in_merge_bases(the_repository, rev,
						     reference_rev) : 0;
	எனில் (merged < 0)
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
	எனில் (head_rev && head_rev != reference_rev) {
		முழு expect = repo_in_merge_bases(the_repository, rev, head_rev);
		எனில் (expect < 0)
			exit(128);
		எனில் (expect == merged)
			; /* okay */
		இல்லையெனில் எனில் (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		இல்லையெனில்
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	விடுவி(reference_name_to_free);
	திரும்பு merged;
}

எண்ணிக்கை delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_HEAD_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_RUN = (1 << 4),
};

நிலையான முழு check_branch_commit(மாறிலி எழுத்து *branchname, மாறிலி எழுத்து *refname,
			       மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு commit *head_rev,
			       முழு kinds, unsigned முழு flags)
{
	கட்டமைப்பு commit *rev = lookup_commit_reference(the_repository, oid);
	எனில் (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		திரும்பு -1;
	}
	எனில் (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, head_rev)) {
		எனில் (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		திரும்பு -1;
	}
	திரும்பு 0;
}

நிலையான வெறுமை delete_branch_config(மாறிலி எழுத்து *branchname)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	எனில் (repo_config_rename_section(the_repository, buf.buf, வெற்று) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

நிலையான முழு delete_branches(முழு argc, மாறிலி எழுத்து **argv, முழு kinds,
			   unsigned முழு flags)
{
	கட்டமைப்பு commit *head_rev = வெற்று;
	கட்டமைப்பு object_id oid;
	எழுத்து *name = வெற்று;
	மாறிலி எழுத்து *fmt;
	முழு i;
	முழு ret = 0;
	முழு remote_branch = 0;
	கட்டமைப்பு strbuf bname = STRBUF_INIT;
	எண்ணிக்கை interpret_branch_kind allowed_interpret;
	கட்டமைப்பு string_list refs_to_delete = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list_item *item;
	முழு branch_name_pos;
	மாறிலி எழுத்து *fmt_remotes = "refs/remotes/%s";

	தேர்வு (kinds) {
	நிலை FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		நிறுத்து;
	நிலை FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		நிறுத்து;
	இயல்பு:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	எனில் (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_HEAD_FALLBACK))
		head_rev = lookup_commit_reference(the_repository, &head_oid);

	க்கு (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		எழுத்து *target = வெற்று;
		முழு ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		விடுவி(name);
		name = mkpathdup(fmt, bname.buf);

		எனில் (kinds == FILTER_REFS_BRANCHES) {
			மாறிலி எழுத்து *path;
			எனில் ((path = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, path);
				ret = 1;
				தொடர்;
			}
			எனில் ((path = branch_checked_out(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, path);
				ret = 1;
				தொடர்;
			}
		}

		target = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		எனில் (!target) {
			எனில் (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} இல்லையெனில் {
				எழுத்து *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				எழுத்து *virtual_target = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				எனில் (virtual_target)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				இல்லையெனில்
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_target);
			}
			ret = 1;
			தொடர்;
		}

		எனில் (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, head_rev, kinds,
					flags)) {
			எனில் (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			செல் next;
		}

		item = string_list_append(&refs_to_delete, name);
		item->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? target
				    : repo_find_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	next:
		விடுவி(target);
	}

	எனில் (!(flags & DELETE_BRANCH_DRY_RUN) &&
	    refs_delete_refs(get_main_ref_store(the_repository), வெற்று, &refs_to_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		எழுத்து *describe_ref = item->util;
		எழுத்து *name = item->string;
		எனில் (flags & DELETE_BRANCH_DRY_RUN) {
			எனில் (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} இல்லையெனில் எனில் (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			எழுத்து *refname = name + branch_name_pos;
			எனில் (!(flags & DELETE_BRANCH_QUIET))
				printf(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		விடுவி(describe_ref);
	}
	string_list_clear(&refs_to_delete, 0);

	விடுவி(name);
	strbuf_release(&bname);

	திரும்பு ret;
}

நிலையான முழு calc_maxwidth(கட்டமைப்பு ref_array *refs, முழு remote_bonus)
{
	முழு i, max = 0;
	க்கு (i = 0; i < refs->nr; i++) {
		கட்டமைப்பு ref_array_item *it = refs->பொருட்கள்[i];
		மாறிலி எழுத்து *desc = it->refname;
		முழு w;

		skip_prefix(it->refname, "refs/heads/", &desc);
		skip_prefix(it->refname, "refs/remotes/", &desc);
		எனில் (it->kind == FILTER_REFS_DETACHED_HEAD) {
			எழுத்து *head_desc = get_head_description();
			w = utf8_strwidth(head_desc);
			விடுவி(head_desc);
		} இல்லையெனில்
			w = utf8_strwidth(desc);

		எனில் (it->kind == FILTER_REFS_REMOTES)
			w += remote_bonus;
		எனில் (w > max)
			max = w;
	}
	திரும்பு max;
}

நிலையான மாறிலி எழுத்து *quote_literal_for_format(மாறிலி எழுத்து *s)
{
	நிலையான கட்டமைப்பு strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	போது (strbuf_expand_step(&buf, &s))
		strbuf_addstr(&buf, "%%");
	திரும்பு buf.buf;
}

நிலையான எழுத்து *build_format(கட்டமைப்பு ref_filter *filter, முழு maxwidth, மாறிலி எழுத்து *remote_prefix)
{
	கட்டமைப்பு strbuf fmt = STRBUF_INIT;
	கட்டமைப்பு strbuf local = STRBUF_INIT;
	கட்டமைப்பு strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	எனில் (filter->verbose) {
		கட்டமைப்பு strbuf obname = STRBUF_INIT;

		எனில் (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		இல்லையெனில் எனில் (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		இல்லையெனில்
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		எனில் (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		இல்லையெனில்
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} இல்லையெனில் {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_prefix),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	திரும்பு strbuf_detach(&fmt, வெற்று);
}

நிலையான வெறுமை print_ref_list(கட்டமைப்பு ref_filter *filter, கட்டமைப்பு ref_sorting *sorting,
			   கட்டமைப்பு ref_format *format, கட்டமைப்பு string_list *output)
{
	முழு i;
	கட்டமைப்பு ref_array array;
	முழு maxwidth = 0;
	மாறிலி எழுத்து *remote_prefix = "";
	எழுத்து *to_free = வெற்று;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	எனில் (filter->kind != FILTER_REFS_REMOTES)
		remote_prefix = "remotes/";

	memset(&array, 0, அளவு(array));

	filter_refs(&array, filter, filter->kind);

	எனில் (filter->verbose)
		maxwidth = calc_maxwidth(&array, strlen(remote_prefix));

	எனில் (!format->format)
		format->format = to_free = build_format(filter, maxwidth, remote_prefix);
	format->use_color = branch_use_color;

	எனில் (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_sort(sorting, &array);

	எனில் (column_active(colopts)) {
		கட்டமைப்பு strbuf out = STRBUF_INIT, err = STRBUF_INIT;

		ப்ரதிஜ்ஞா(!filter->verbose && "--column and --verbose are incompatible");

		க்கு (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&out);
			எனில் (format_ref_array_item(array.பொருட்கள்[i], format, &out, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, out.buf);
		}

		strbuf_release(&err);
		strbuf_release(&out);
	} இல்லையெனில் {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	விடுவி(to_free);
}

நிலையான வெறுமை print_current_branch_name(வெறுமை)
{
	முழு flags;
	மாறிலி எழுத்து *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, வெற்று, &flags);
	மாறிலி எழுத்து *shortname;
	எனில் (!refname)
		die(_("could not resolve HEAD"));
	இல்லையெனில் எனில் (!(flags & REF_ISSYMREF))
		திரும்பு;
	இல்லையெனில் எனில் (skip_prefix(refname, "refs/heads/", &shortname))
		puts(shortname);
	இல்லையெனில்
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

நிலையான வெறுமை reject_rebase_or_bisect_branch(கட்டமைப்பு worktree **worktrees,
					   மாறிலி எழுத்து *target)
{
	முழு i;

	க்கு (i = 0; worktrees[i]; i++) {
		கட்டமைப்பு worktree *wt = worktrees[i];

		எனில் (!wt->is_detached)
			தொடர்;

		எனில் (is_worktree_being_rebased(wt, target))
			die(_("branch %s is being rebased at %s"),
			    target, wt->path);

		எனில் (is_worktree_being_bisected(wt, target))
			die(_("branch %s is being bisected at %s"),
			    target, wt->path);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
நிலையான முழு replace_each_worktree_head_symref(கட்டமைப்பு worktree **worktrees,
					     மாறிலி எழுத்து *oldref, மாறிலி எழுத்து *newref,
					     மாறிலி எழுத்து *logmsg)
{
	முழு ret = 0;
	முழு i;

	க்கு (i = 0; worktrees[i]; i++) {
		கட்டமைப்பு ref_store *refs;

		எனில் (worktrees[i]->is_detached)
			தொடர்;
		எனில் (!worktrees[i]->head_ref)
			தொடர்;
		எனில் (strcmp(oldref, worktrees[i]->head_ref))
			தொடர்;

		refs = get_worktree_ref_store(worktrees[i]);
		எனில் (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->path);
	}

	திரும்பு ret;
}

#define IS_HEAD 1
#define IS_ORPHAN 2

நிலையான வெறுமை copy_or_rename_branch(மாறிலி எழுத்து *oldname, மாறிலி எழுத்து *newname, முழு copy, முழு force)
{
	கட்டமைப்பு strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	கட்டமைப்பு strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	மாறிலி எழுத்து *interpreted_oldname = வெற்று;
	மாறிலி எழுத்து *interpreted_newname = வெற்று;
	முழு recovery = 0, oldref_usage = 0;
	கட்டமைப்பு worktree **worktrees = get_worktrees(the_repository);

	எனில் (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		எனில் (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		இல்லையெனில் {
			முழு code = die_message(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			exit(code);
		}
	}

	க்கு (முழு i = 0; worktrees[i]; i++) {
		கட்டமைப்பு worktree *wt = worktrees[i];

		எனில் (wt->head_ref && !strcmp(oldref.buf, wt->head_ref)) {
			oldref_usage |= IS_HEAD;
			எனில் (is_null_oid(&wt->head_oid))
				oldref_usage |= IS_ORPHAN;
			நிறுத்து;
		}
	}

	எனில் ((copy || !(oldref_usage & IS_HEAD)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		எனில் (oldref_usage & IS_HEAD)
			die(_("no commit on branch '%s' yet"), oldname);
		இல்லையெனில்
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	எனில் (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	இல்லையெனில்
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	எனில் (!skip_prefix(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_prefix(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	எனில் (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	இல்லையெனில்
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	எனில் (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	எனில் (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	எனில் (recovery) {
		எனில் (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		இல்லையெனில்
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	எனில் (!copy && (oldref_usage & IS_HEAD) &&
	    replace_each_worktree_head_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	எனில் (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	எனில் (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

நிலையான முழு parse_opt_forked(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு ref_filter *filter = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	எனில் (ref_filter_forked_add(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	திரும்பு 0;
}

கட்டமைப்பு stacked_branch_data {
	கட்டமைப்பு strset *deletable_branch_names;
	கட்டமைப்பு strset *protected_branch_names;
};

நிலையான முழு collect_stacked_branch_base(மாறிலி கட்டமைப்பு reference *ref,
				       வெறுமை *cb_data)
{
	கட்டமைப்பு stacked_branch_data *data = cb_data;
	மாறிலி எழுத்து *branch_name;
	கட்டமைப்பு branch *branch;
	மாறிலி எழுத்து *upstream_refname;
	மாறிலி எழுத்து *upstream_branch_name;

	எனில் (!skip_prefix(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	எனில் (strset_contains(data->deletable_branch_names, branch_name))
		திரும்பு 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, வெற்று);
	எனில் (!upstream_refname ||
	    !skip_prefix(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_contains(data->deletable_branch_names,
			    upstream_branch_name))
		திரும்பு 0;

	strset_add(data->protected_branch_names, upstream_branch_name);
	திரும்பு 0;
}

நிலையான வெறுமை protect_stacked_branch_bases(கட்டமைப்பு ref_store *refs,
					 கட்டமைப்பு strset *deletable_branch_names,
					 கட்டமைப்பு strset *protected_branch_names)
{
	கட்டமைப்பு stacked_branch_data data = {
		.deletable_branch_names = deletable_branch_names,
		.protected_branch_names = protected_branch_names,
	};
	கட்டமைப்பு refs_for_each_ref_options opts = {
		.prefix = "refs/heads/",
	};
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு strmap_entry *entry;

	refs_for_each_ref_ext(refs, collect_stacked_branch_base, &data, &opts);

	strset_for_each_entry(protected_branch_names, &iter, entry)
		strset_remove(deletable_branch_names, entry->சாவி);
}

நிலையான வெறுமை clear_deleted_upstreams(கட்டமைப்பு strset *protected_branch_names,
				    கட்டமைப்பு strset *deletable_branch_names)
{
	கட்டமைப்பு strbuf சாவி = STRBUF_INIT;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு strmap_entry *entry;

	strset_for_each_entry(protected_branch_names, &iter, entry) {
		கட்டமைப்பு branch *branch = branch_get(entry->சாவி);
		மாறிலி எழுத்து *upstream_refname = branch_get_upstream(branch, வெற்று);
		மாறிலி எழுத்து *upstream_branch_name;

		எனில் (!upstream_refname ||
		    !skip_prefix(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_contains(deletable_branch_names,
				    upstream_branch_name))
			தொடர்;

		strbuf_addf(&சாவி, "branch.%s.merge", branch->name);
		repo_config_set_gently(the_repository, சாவி.buf, வெற்று);
		strbuf_reset(&சாவி);
		strbuf_addf(&சாவி, "branch.%s.remote", branch->name);
		repo_config_set_gently(the_repository, சாவி.buf, வெற்று);
		strbuf_reset(&சாவி);
	}

	strbuf_release(&சாவி);
}

நிலையான முழு branch_pushes_to_upstream(கட்டமைப்பு branch *branch,
				     மாறிலி எழுத்து *upstream)
{
	கட்டமைப்பு remote *remote = remote_get(remote_for_branch(branch, வெற்று));
	எழுத்து *தள்ளு_refname = வெற்று;
	எழுத்து *tracking = வெற்று;
	முழு ret = 0;

	எனில் (!remote)
		திரும்பு 0;
	எனில் (remote->தள்ளு.nr)
		தள்ளு_refname = apply_refspecs(&remote->தள்ளு, branch->refname);
	இல்லையெனில்
		தள்ளு_refname = xstrdup(branch->refname);
	எனில் (தள்ளு_refname)
		tracking = apply_refspecs(&remote->fetch, தள்ளு_refname);
	எனில் (tracking && !strcmp(tracking, upstream))
		ret = 1;

	விடுவி(தள்ளு_refname);
	விடுவி(tracking);
	திரும்பு ret;
}

நிலையான முழு delete_merged_branches(மாறிலி கட்டமைப்பு strvec *upstreams,
				 மாறிலி எழுத்து **argv, unsigned முழு flags)
{
	கட்டமைப்பு ref_store *refs = get_main_ref_store(the_repository);
	கட்டமைப்பு ref_filter filter = REF_FILTER_INIT;
	கட்டமைப்பு ref_array candidates = { 0 };
	கட்டமைப்பு strset deletable_branch_names = STRSET_INIT;
	கட்டமைப்பு strset protected_branch_names = STRSET_INIT;
	கட்டமைப்பு strvec branches_to_delete = STRVEC_INIT;
	கட்டமைப்பு strbuf சாவி = STRBUF_INIT;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு strmap_entry *entry;
	முழு ret = 0;

	க்கு (size_t i = 0; i < upstreams->nr; i++)
		எனில் (ref_filter_forked_add(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.kind = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.kind);

	க்கு (முழு i = 0; i < candidates.nr; i++) {
		மாறிலி எழுத்து *branch_refname = candidates.பொருட்கள்[i]->refname;
		மாறிலி எழுத்து *branch_name;
		கட்டமைப்பு branch *branch;
		மாறிலி எழுத்து *upstream_refname;
		முழு opt_out;

		எனில் (!skip_prefix(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		எனில் (branch_checked_out(branch_refname))
			தொடர்;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, வெற்று);
		எனில் (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			தொடர்;
		எனில் (branch_pushes_to_upstream(branch, upstream_refname))
			தொடர்;
		எனில் (check_branch_commit(branch_name, branch_name,
					&candidates.பொருட்கள்[i]->objectname, வெற்று,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			தொடர்;

		strbuf_reset(&சாவி);
		strbuf_addf(&சாவி, "branch.%s.deletemerged", branch_name);
		எனில் (!repo_config_get_bool(the_repository, சாவி.buf, &opt_out) &&
		    !opt_out) {
			எனில் (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			தொடர்;
		}

		strset_add(&deletable_branch_names, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_names,
				     &protected_branch_names);

	strset_for_each_entry(&deletable_branch_names, &iter, entry)
		strvec_தள்ளு(&branches_to_delete, entry->சாவி);

	எனில் (branches_to_delete.nr)
		ret = delete_branches(branches_to_delete.nr, branches_to_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_HEAD_FALLBACK |
				      flags);

	எனில் (!ret && !(flags & DELETE_BRANCH_DRY_RUN))
		clear_deleted_upstreams(&protected_branch_names,
					&deletable_branch_names);

	strbuf_release(&சாவி);
	strvec_clear(&branches_to_delete);
	strset_clear(&protected_branch_names);
	strset_clear(&deletable_branch_names);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	திரும்பு ret;
}

நிலையான GIT_PATH_FUNC(edit_description, "EDIT_DESCRIPTION")

நிலையான முழு edit_branch_description(மாறிலி எழுத்து *branch_name)
{
	முழு exists;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	எனில் (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_வரி_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_வரி_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	எனில் (launch_editor(edit_description(), &buf, வெற்று)) {
		strbuf_release(&buf);
		திரும்பு -1;
	}
	strbuf_stripspace(&buf, comment_வரி_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	எனில் (buf.len || exists)
		repo_config_set(the_repository, name.buf, buf.len ? buf.buf : வெற்று);
	strbuf_release(&name);
	strbuf_release(&buf);

	திரும்பு 0;
}

நிலையான வெறுமை die_if_upstream_looks_like_remote(மாறிலி எழுத்து *new_upstream, மாறிலி எழுத்து *branch_name)
{
	கட்டமைப்பு strbuf remote_ref = STRBUF_INIT;
	முழு code;

	எனில் (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		திரும்பு;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	எனில் (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		திரும்பு;
	}

	code = die_message(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_SET_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	exit(code);
}

முழு cmd_branch(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	/* possible actions */
	முழு delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	கட்டமைப்பு strvec delete_merged = STRVEC_INIT;
	முழு dry_run = 0;
	மாறிலி எழுத்து *new_upstream = வெற்று;
	முழு noncreate_actions = 0;
	/* possible options */
	முழு reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	எண்ணிக்கை branch_track track;
	கட்டமைப்பு ref_filter filter = REF_FILTER_INIT;
	நிலையான கட்டமைப்பு ref_sorting *sorting;
	கட்டமைப்பு string_list sorting_options = STRING_LIST_INIT_DUP;
	கட்டமைப்பு ref_format format = REF_FORMAT_INIT;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);
	முழு ret;

	கட்டமைப்பு option options[] = {
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
		OPT_BIT('D', வெற்று, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_BIT('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_BIT('M', வெற்று, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_BIT('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_BIT('C', வெற்று, &copy, N_("copy a branch, even if target exists"), 2),
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

	setup_ref_filter_porcelain_செய்தி();

	filter.kind = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &sorting_options);
	எனில் (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	track = cfg->branch_track;

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &head_oid, வெற்று);
	எனில் (!head)
		die(_("failed to resolve HEAD as a valid ref"));
	எனில் (!strcmp(head, "HEAD"))
		filter.detached = 1;
	இல்லையெனில் எனில் (!skip_prefix(head, "refs/heads/", &head))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, prefix, options, builtin_branch_usage,
			     0);

	எனில் (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	எனில் (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	எனில் (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	எனில் (dry_run && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	எனில் (recurse_submodules_explicit) {
		எனில் (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		எனில் (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	எனில் (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	எனில் (filter.verbose) {
		எனில் (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	எனில் (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	எனில் (list)
		setup_auto_pager("branch", 1);

	எனில் (delete) {
		எனில் (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.kind,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		செல் out;
	} இல்லையெனில் எனில் (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_run ? DELETE_BRANCH_DRY_RUN : 0));
		செல் out;
	} இல்லையெனில் எனில் (show_current) {
		print_current_branch_name();
		ret = 0;
		செல் out;
	} இல்லையெனில் எனில் (list) {
		/*  git branch --list also shows HEAD when it is detached */
		எனில் ((filter.kind & FILTER_REFS_BRANCHES) && filter.detached)
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
		print_columns(&output, colopts, வெற்று);
		string_list_clear(&output, 0);
		ref_sorting_release(sorting);
		ref_filter_clear(&filter);

		ret = 0;
		செல் out;
	} இல்லையெனில் எனில் (edit_description) {
		மாறிலி எழுத்து *branch_name;
		கட்டமைப்பு strbuf branch_ref = STRBUF_INIT;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		எனில் (!argc) {
			எனில் (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = head;
		} இல்லையெனில் எனில் (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} இல்லையெனில் {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		எனில் (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_out(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} இல்லையெனில் எனில் (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} இல்லையெனில் {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		செல் out;
	} இல்லையெனில் எனில் (copy || rename) {
		எனில் (!argc)
			die(_("branch name required"));
		இல்லையெனில் எனில் ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		இல்லையெனில் எனில் (argc == 1)
			copy_or_rename_branch(head, argv[0], copy, copy + rename > 1);
		இல்லையெனில் எனில் (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		இல்லையெனில்
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} இல்லையெனில் எனில் (new_upstream) {
		கட்டமைப்பு branch *branch;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		எனில் (!argc)
			branch = branch_get(வெற்று);
		இல்லையெனில் எனில் (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} இல்லையெனில்
			die(_("too many arguments to set new upstream"));

		எனில் (!branch) {
			எனில் (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		எனில் (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			எனில் (!argc || branch_checked_out(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			எனில் (argc == 1 &&
			    advice_enabled(ADVICE_SET_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_setup_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} இல்லையெனில் எனில் (unset_upstream) {
		கட்டமைப்பு branch *branch;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		எனில் (!argc)
			branch = branch_get(வெற்று);
		இல்லையெனில் எனில் (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} இல்லையெனில்
			die(_("too many arguments to unset upstream"));

		எனில் (!branch) {
			எனில் (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		எனில் (!branch_has_merge_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, வெற்று, வெற்று, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_set_multivar(the_repository, buf.buf, வெற்று, வெற்று, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} இல்லையெனில் எனில் (!noncreate_actions && argc > 0 && argc <= 2) {
		மாறிலி எழுத்து *branch_name = argv[0];
		மாறிலி எழுத்து *start_name = argc == 2 ? argv[1] : head;

		எனில் (filter.kind != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		எனில் (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		எனில் (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    start_name, வெற்று, force,
						    reflog, quiet, track, 0);
			ret = 0;
			செல் out;
		}
		create_branch(the_repository, branch_name, start_name, force, 0,
			      reflog, quiet, track, 0);
	} இல்லையெனில்
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

out:
	strvec_clear(&delete_merged);
	string_list_clear(&sorting_options, 0);
	திரும்பு ret;
}
