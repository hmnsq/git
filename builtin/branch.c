/*
 * Builtin "git branch"
 *
 * Copyright (c) 2006 Kristian Høgsberg <krh@redhat.com>
 * Based on git-branch.sh by Junio C Hamano.
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "strmap.h"
#అన్తర్భావ "column.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "ref-filter.h"
#అన్తర్భావ "worktree.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "commit-reach.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_branch_usage[] = {
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
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *తల;
స్థిర నిర్మాణం object_id తల_oid;
స్థిర పూర్ణాంకం recurse_submodules = 0;
స్థిర పూర్ణాంకం submodule_propagate_branches = 0;

స్థిర గణన git_colorbool branch_use_color = GIT_COLOR_UNKNOWN;
స్థిర అక్షరం branch_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_సాధారణ,       /* PLAIN */
	GIT_COLOR_RED,          /* REMOTE */
	GIT_COLOR_సాధారణ,       /* LOCAL */
	GIT_COLOR_GREEN,        /* CURRENT */
	GIT_COLOR_BLUE,         /* UPSTREAM */
	GIT_COLOR_CYAN,         /* WORKTREE */
};
గణన color_branch {
	BRANCH_COLOR_RESET = 0,
	BRANCH_COLOR_PLAIN = 1,
	BRANCH_COLOR_REMOTE = 2,
	BRANCH_COLOR_LOCAL = 3,
	BRANCH_COLOR_CURRENT = 4,
	BRANCH_COLOR_UPSTREAM = 5,
	BRANCH_COLOR_WORKTREE = 6
};

స్థిర స్థిరరాశి అక్షరం *color_branch_slots[] = {
	[BRANCH_COLOR_RESET]	= "reset",
	[BRANCH_COLOR_PLAIN]	= "plain",
	[BRANCH_COLOR_REMOTE]	= "remote",
	[BRANCH_COLOR_LOCAL]	= "local",
	[BRANCH_COLOR_CURRENT]	= "current",
	[BRANCH_COLOR_UPSTREAM] = "upstream",
	[BRANCH_COLOR_WORKTREE] = "worktree",
};

స్థిర నిర్మాణం string_list output = STRING_LIST_INIT_DUP;
స్థిర గుర్తులేని పూర్ణాంకం colopts;

define_list_config_array(color_branch_slots);

స్థిర పూర్ణాంకం git_branch_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			     స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	స్థిరరాశి అక్షరం *స్థానం_name;

	ఒకవేళ (!strcmp(var, "branch.sort")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		string_list_append(cb, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (starts_with(var, "column."))
		తిరిగి git_స్తంభం_config(var, విలువ, "branch", &colopts);
	ఒకవేళ (!strcmp(var, "color.branch")) {
		branch_use_color = git_config_colorbool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (skip_ఉపసర్గ(var, "color.branch.", &స్థానం_name)) {
		పూర్ణాంకం స్థానం = LOOKUP_CONFIG(color_branch_slots, స్థానం_name);
		ఒకవేళ (స్థానం < 0)
			తిరిగి 0;
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		తిరిగి color_parse(విలువ, branch_colors[స్థానం]);
	}
	ఒకవేళ (!strcmp(var, "submodule.recurse")) {
		recurse_submodules = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcasecmp(var, "submodule.propagateBranches")) {
		submodule_propagate_branches = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (git_color_config(var, విలువ, cb) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర స్థిరరాశి అక్షరం *branch_get_color(గణన color_branch ix)
{
	ఒకవేళ (want_color(branch_use_color))
		తిరిగి branch_colors[ix];
	తిరిగి "";
}

స్థిర పూర్ణాంకం branch_merged(పూర్ణాంకం రకం, స్థిరరాశి అక్షరం *name,
			 నిర్మాణం commit *rev, నిర్మాణం commit *తల_rev)
{
	/*
	 * This checks whether the merge bases of branch and HEAD (or
	 * the other branch this branch builds upon) contains the
	 * branch, which means that the branch has already been merged
	 * safely to HEAD (or the other branch).
	 */
	నిర్మాణం commit *reference_rev = ఖాళీ;
	స్థిరరాశి అక్షరం *reference_name = ఖాళీ;
	శూన్య *reference_name_గమ్యం_free = ఖాళీ;
	పూర్ణాంకం merged;

	ఒకవేళ (రకం == FILTER_REFS_BRANCHES) {
		నిర్మాణం branch *branch = branch_get(name);
		స్థిరరాశి అక్షరం *upstream = branch_get_upstream(branch, ఖాళీ);
		నిర్మాణం object_id oid;

		ఒకవేళ (upstream &&
		    (reference_name = reference_name_గమ్యం_free =
		     refs_resolve_refdup(get_main_ref_store(the_repository), upstream, RESOLVE_REF_READING,
					 &oid, ఖాళీ)) != ఖాళీ)
			reference_rev = lookup_commit_reference(the_repository,
								&oid);
	}
	ఒకవేళ (!reference_rev)
		reference_rev = తల_rev;

	merged = reference_rev ? repo_లోపల_విలీనం_bases(the_repository, rev,
						     reference_rev) : 0;
	ఒకవేళ (merged < 0)
		నిష్క్రమించు(128);

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
	ఒకవేళ (తల_rev && తల_rev != reference_rev) {
		పూర్ణాంకం expect = repo_లోపల_విలీనం_bases(the_repository, rev, తల_rev);
		ఒకవేళ (expect < 0)
			నిష్క్రమించు(128);
		ఒకవేళ (expect == merged)
			; /* okay */
		లేకపోతే ఒకవేళ (merged)
			warning(_("deleting branch '%s' that has been merged to\n"
				"         '%s', but not yet merged to HEAD"),
				name, reference_name);
		లేకపోతే
			warning(_("not deleting branch '%s' that is not yet merged to\n"
				"         '%s', even though it is merged to HEAD"),
				name, reference_name);
	}
	విడుదల(reference_name_గమ్యం_free);
	తిరిగి merged;
}

గణన delete_branch_flags {
	DELETE_BRANCH_FORCE = (1 << 0),
	DELETE_BRANCH_QUIET = (1 << 1),
	DELETE_BRANCH_SKIP_UNMERGED = (1 << 2),
	DELETE_BRANCH_NO_తల_FALLBACK = (1 << 3),
	DELETE_BRANCH_DRY_నిరంతరం = (1 << 4),
};

స్థిర పూర్ణాంకం check_branch_commit(స్థిరరాశి అక్షరం *branchname, స్థిరరాశి అక్షరం *refname,
			       స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం commit *తల_rev,
			       పూర్ణాంకం kinds, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం commit *rev = lookup_commit_reference(the_repository, oid);
	ఒకవేళ (!(flags & DELETE_BRANCH_FORCE) && !rev) {
		error(_("couldn't look up commit object for '%s'"), refname);
		తిరిగి -1;
	}
	ఒకవేళ (!(flags & DELETE_BRANCH_FORCE) &&
	    !branch_merged(kinds, branchname, rev, తల_rev)) {
		ఒకవేళ (!(flags & DELETE_BRANCH_SKIP_UNMERGED)) {
			error(_("the branch '%s' is not fully merged"),
			      branchname);
			advise_if_enabled(ADVICE_FORCE_DELETE_BRANCH,
					  _("If you are sure you want to delete it, "
					  "run 'git branch -D %s'"), branchname);
		}
		తిరిగి -1;
	}
	తిరిగి 0;
}

స్థిర శూన్య delete_branch_config(స్థిరరాశి అక్షరం *branchname)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "branch.%s", branchname);
	ఒకవేళ (repo_config_rename_section(the_repository, buf.buf, ఖాళీ) < 0)
		warning(_("update of config-file failed"));
	strbuf_release(&buf);
}

స్థిర పూర్ణాంకం delete_branches(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, పూర్ణాంకం kinds,
			   గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం commit *తల_rev = ఖాళీ;
	నిర్మాణం object_id oid;
	అక్షరం *name = ఖాళీ;
	స్థిరరాశి అక్షరం *fmt;
	పూర్ణాంకం i;
	పూర్ణాంకం ret = 0;
	పూర్ణాంకం remote_branch = 0;
	నిర్మాణం strbuf bname = STRBUF_INIT;
	గణన interpret_branch_రకం allowed_interpret;
	నిర్మాణం string_list refs_గమ్యం_delete = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list_అంశం *అంశం;
	పూర్ణాంకం branch_name_pos;
	స్థిరరాశి అక్షరం *fmt_remotes = "refs/remotes/%s";

	ఎంపిక (kinds) {
	స్థితి FILTER_REFS_REMOTES:
		fmt = fmt_remotes;
		/* For subsequent UI messages */
		remote_branch = 1;
		allowed_interpret = INTERPRET_BRANCH_REMOTE;

		flags |= DELETE_BRANCH_FORCE;
		ఆపు;
	స్థితి FILTER_REFS_BRANCHES:
		fmt = "refs/heads/%s";
		allowed_interpret = INTERPRET_BRANCH_LOCAL;
		ఆపు;
	అప్రమేయ:
		die(_("cannot use -a with -d"));
	}
	branch_name_pos = strcspn(fmt, "%");

	ఒకవేళ (!(flags & DELETE_BRANCH_FORCE) &&
	    !(flags & DELETE_BRANCH_NO_తల_FALLBACK))
		తల_rev = lookup_commit_reference(the_repository, &తల_oid);

	కొరకు (i = 0; i < argc; i++, strbuf_reset(&bname)) {
		అక్షరం *లక్ష్యం = ఖాళీ;
		పూర్ణాంకం ref_flags = 0;

		copy_branchname(the_repository, &bname,
				argv[i], allowed_interpret);
		విడుదల(name);
		name = mkpathdup(fmt, bname.buf);

		ఒకవేళ (kinds == FILTER_REFS_BRANCHES) {
			స్థిరరాశి అక్షరం *మార్గం;
			ఒకవేళ ((మార్గం = branch_bisecting(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s' for bisect"),
					      bname.buf, మార్గం);
				ret = 1;
				కొనసాగు;
			}
			ఒకవేళ ((మార్గం = branch_checked_బహిర్గతం(name))) {
				error(_("cannot delete branch '%s' "
					"used by worktree at '%s'"),
				      bname.buf, మార్గం);
				ret = 1;
				కొనసాగు;
			}
		}

		లక్ష్యం = refs_resolve_refdup(get_main_ref_store(the_repository),
					     name,
					     RESOLVE_REF_READING
					     | RESOLVE_REF_NO_RECURSE
					     | RESOLVE_REF_ALLOW_BAD_NAME,
					     &oid, &ref_flags);
		ఒకవేళ (!లక్ష్యం) {
			ఒకవేళ (remote_branch) {
				error(_("remote-tracking branch '%s' not found"), bname.buf);
			} లేకపోతే {
				అక్షరం *virtual_name = mkpathdup(fmt_remotes, bname.buf);
				అక్షరం *virtual_లక్ష్యం = refs_resolve_refdup(get_main_ref_store(the_repository),
									   virtual_name,
									   RESOLVE_REF_READING
									   | RESOLVE_REF_NO_RECURSE
									   | RESOLVE_REF_ALLOW_BAD_NAME,
									   &oid,
									   &ref_flags);
				FREE_AND_NULL(virtual_name);

				ఒకవేళ (virtual_లక్ష్యం)
					error(_("branch '%s' not found.\n"
						"Did you forget --remote?"),
						bname.buf);
				లేకపోతే
					error(_("branch '%s' not found"), bname.buf);
				FREE_AND_NULL(virtual_లక్ష్యం);
			}
			ret = 1;
			కొనసాగు;
		}

		ఒకవేళ (!(ref_flags & (REF_ISSYMREF|REF_ISBROKEN)) &&
		    check_branch_commit(bname.buf, name, &oid, తల_rev, kinds,
					flags)) {
			ఒకవేళ (!(flags & DELETE_BRANCH_SKIP_UNMERGED))
				ret = 1;
			వెళ్ళు తదుపరి;
		}

		అంశం = string_list_append(&refs_గమ్యం_delete, name);
		అంశం->util = xstrdup((ref_flags & REF_ISBROKEN) ? "broken"
				    : (ref_flags & REF_ISSYMREF) ? లక్ష్యం
				    : repo_కనుగొను_unique_abbrev(the_repository, &oid, DEFAULT_ABBREV));

	తదుపరి:
		విడుదల(లక్ష్యం);
	}

	ఒకవేళ (!(flags & DELETE_BRANCH_DRY_నిరంతరం) &&
	    refs_delete_refs(get_main_ref_store(the_repository), ఖాళీ, &refs_గమ్యం_delete, REF_NO_DEREF))
		ret = 1;

	for_each_string_list_అంశం(అంశం, &refs_గమ్యం_delete) {
		అక్షరం *describe_ref = అంశం->util;
		అక్షరం *name = అంశం->string;
		ఒకవేళ (flags & DELETE_BRANCH_DRY_నిరంతరం) {
			ఒకవేళ (!(flags & DELETE_BRANCH_QUIET))
				రాయి(remote_branch
					? _("Would delete remote-tracking branch %s (was %s).\n")
					: _("Would delete branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);
		} లేకపోతే ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), name)) {
			అక్షరం *refname = name + branch_name_pos;
			ఒకవేళ (!(flags & DELETE_BRANCH_QUIET))
				రాయి(remote_branch
					? _("Deleted remote-tracking branch %s (was %s).\n")
					: _("Deleted branch %s (was %s).\n"),
					name + branch_name_pos, describe_ref);

			delete_branch_config(refname);
		}
		విడుదల(describe_ref);
	}
	string_list_clear(&refs_గమ్యం_delete, 0);

	విడుదల(name);
	strbuf_release(&bname);

	తిరిగి ret;
}

స్థిర పూర్ణాంకం calc_maxwidth(నిర్మాణం ref_array *refs, పూర్ణాంకం remote_bonus)
{
	పూర్ణాంకం i, max = 0;
	కొరకు (i = 0; i < refs->nr; i++) {
		నిర్మాణం ref_array_అంశం *it = refs->వస్తువులు[i];
		స్థిరరాశి అక్షరం *desc = it->refname;
		పూర్ణాంకం w;

		skip_ఉపసర్గ(it->refname, "refs/heads/", &desc);
		skip_ఉపసర్గ(it->refname, "refs/remotes/", &desc);
		ఒకవేళ (it->రకం == FILTER_REFS_DETACHED_తల) {
			అక్షరం *తల_desc = get_తల_description();
			w = utf8_strwidth(తల_desc);
			విడుదల(తల_desc);
		} లేకపోతే
			w = utf8_strwidth(desc);

		ఒకవేళ (it->రకం == FILTER_REFS_REMOTES)
			w += remote_bonus;
		ఒకవేళ (w > max)
			max = w;
	}
	తిరిగి max;
}

స్థిర స్థిరరాశి అక్షరం *quote_literal_for_format(స్థిరరాశి అక్షరం *s)
{
	స్థిర నిర్మాణం strbuf buf = STRBUF_INIT;

	strbuf_reset(&buf);
	అయితే (strbuf_expand_అడుగు(&buf, &s))
		strbuf_addstr(&buf, "%%");
	తిరిగి buf.buf;
}

స్థిర అక్షరం *build_format(నిర్మాణం ref_filter *filter, పూర్ణాంకం maxwidth, స్థిరరాశి అక్షరం *remote_ఉపసర్గ)
{
	నిర్మాణం strbuf fmt = STRBUF_INIT;
	నిర్మాణం strbuf local = STRBUF_INIT;
	నిర్మాణం strbuf remote = STRBUF_INIT;

	strbuf_addf(&local, "%%(if)%%(HEAD)%%(then)* %s%%(else)%%(if)%%(worktreepath)%%(then)+ %s%%(else)  %s%%(end)%%(end)",
			branch_get_color(BRANCH_COLOR_CURRENT),
			branch_get_color(BRANCH_COLOR_WORKTREE),
			branch_get_color(BRANCH_COLOR_LOCAL));
	strbuf_addf(&remote, "  %s",
		    branch_get_color(BRANCH_COLOR_REMOTE));

	ఒకవేళ (filter->verbose) {
		నిర్మాణం strbuf obname = STRBUF_INIT;

		ఒకవేళ (filter->abbrev < 0)
			strbuf_addf(&obname, "%%(objectname:short)");
		లేకపోతే ఒకవేళ (!filter->abbrev)
			strbuf_addf(&obname, "%%(objectname)");
		లేకపోతే
			strbuf_addf(&obname, "%%(objectname:short=%d)", filter->abbrev);

		strbuf_addf(&local, "%%(align:%d,left)%%(refname:lstrip=2)%%(end)", maxwidth);
		strbuf_addstr(&local, branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&local, " %s ", obname.buf);

		ఒకవేళ (filter->verbose > 1)
		{
			strbuf_addf(&local, "%%(if:notequals=*)%%(HEAD)%%(then)%%(if)%%(worktreepath)%%(then)(%s%%(worktreepath)%s) %%(end)%%(end)",
				    branch_get_color(BRANCH_COLOR_WORKTREE), branch_get_color(BRANCH_COLOR_RESET));
			strbuf_addf(&local, "%%(if)%%(upstream)%%(then)[%s%%(upstream:short)%s%%(if)%%(upstream:track)"
				    "%%(then): %%(upstream:track,nobracket)%%(end)] %%(end)%%(contents:subject)",
				    branch_get_color(BRANCH_COLOR_UPSTREAM), branch_get_color(BRANCH_COLOR_RESET));
		}
		లేకపోతే
			strbuf_addf(&local, "%%(if)%%(upstream:track)%%(then)%%(upstream:track) %%(end)%%(contents:subject)");

		strbuf_addf(&remote, "%%(align:%d,left)%s%%(refname:lstrip=2)%%(end)%s"
			    "%%(if)%%(symref)%%(then) -> %%(symref:short)"
			    "%%(else) %s %%(contents:subject)%%(end)",
			    maxwidth, quote_literal_for_format(remote_ఉపసర్గ),
			    branch_get_color(BRANCH_COLOR_RESET), obname.buf);
		strbuf_release(&obname);
	} లేకపోతే {
		strbuf_addf(&local, "%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    branch_get_color(BRANCH_COLOR_RESET));
		strbuf_addf(&remote, "%s%%(refname:lstrip=2)%s%%(if)%%(symref)%%(then) -> %%(symref:short)%%(end)",
			    quote_literal_for_format(remote_ఉపసర్గ),
			    branch_get_color(BRANCH_COLOR_RESET));
	}

	strbuf_addf(&fmt, "%%(if:notequals=refs/remotes)%%(refname:rstrip=-2)%%(then)%s%%(else)%s%%(end)", local.buf, remote.buf);

	strbuf_release(&local);
	strbuf_release(&remote);
	తిరిగి strbuf_detach(&fmt, ఖాళీ);
}

స్థిర శూన్య print_ref_list(నిర్మాణం ref_filter *filter, నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ,
			   నిర్మాణం ref_format *format, నిర్మాణం string_list *output)
{
	పూర్ణాంకం i;
	నిర్మాణం ref_array array;
	పూర్ణాంకం maxwidth = 0;
	స్థిరరాశి అక్షరం *remote_ఉపసర్గ = "";
	అక్షరం *గమ్యం_free = ఖాళీ;

	/*
	 * If we are listing more than just remote branches,
	 * then remote branches will have a "remotes/" prefix.
	 * We need to account for this in the width.
	 */
	ఒకవేళ (filter->రకం != FILTER_REFS_REMOTES)
		remote_ఉపసర్గ = "remotes/";

	memset(&array, 0, పరిమాణం(array));

	filter_refs(&array, filter, filter->రకం);

	ఒకవేళ (filter->verbose)
		maxwidth = calc_maxwidth(&array, అక్షరశ్రేణి_పొడవు(remote_ఉపసర్గ));

	ఒకవేళ (!format->format)
		format->format = గమ్యం_free = build_format(filter, maxwidth, remote_ఉపసర్గ);
	format->use_color = branch_use_color;

	ఒకవేళ (verify_ref_format(format))
		die(_("unable to parse format string"));

	filter_ahead_behind(the_repository, &array);
	ref_array_క్రమీకరించు(క్రమీకరణ, &array);

	ఒకవేళ (స్తంభం_చురుకైన(colopts)) {
		నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT, err = STRBUF_INIT;

		ప్రతిజ్ఞా(!filter->verbose && "--column and --verbose are incompatible");

		కొరకు (i = 0; i < array.nr; i++) {
			strbuf_reset(&err);
			strbuf_reset(&బహిర్గతం);
			ఒకవేళ (format_ref_array_అంశం(array.వస్తువులు[i], format, &బహిర్గతం, &err))
				die("%s", err.buf);

			/* format to a string_list to let print_columns() do its job */
			string_list_append(output, బహిర్గతం.buf);
		}

		strbuf_release(&err);
		strbuf_release(&బహిర్గతం);
	} లేకపోతే {
		print_formatted_ref_array(&array, format);
	}

	ref_array_clear(&array);
	విడుదల(గమ్యం_free);
}

స్థిర శూన్య print_current_branch_name(శూన్య)
{
	పూర్ణాంకం flags;
	స్థిరరాశి అక్షరం *refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD", 0, ఖాళీ, &flags);
	స్థిరరాశి అక్షరం *shortname;
	ఒకవేళ (!refname)
		die(_("could not resolve HEAD"));
	లేకపోతే ఒకవేళ (!(flags & REF_ISSYMREF))
		తిరిగి;
	లేకపోతే ఒకవేళ (skip_ఉపసర్గ(refname, "refs/heads/", &shortname))
		puts(shortname);
	లేకపోతే
		die(_("HEAD (%s) points outside of refs/heads/"), refname);
}

స్థిర శూన్య reject_rebase_or_bisect_branch(నిర్మాణం worktree **worktrees,
					   స్థిరరాశి అక్షరం *లక్ష్యం)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; worktrees[i]; i++) {
		నిర్మాణం worktree *wt = worktrees[i];

		ఒకవేళ (!wt->is_detached)
			కొనసాగు;

		ఒకవేళ (is_worktree_being_rebased(wt, లక్ష్యం))
			die(_("branch %s is being rebased at %s"),
			    లక్ష్యం, wt->మార్గం);

		ఒకవేళ (is_worktree_being_bisected(wt, లక్ష్యం))
			die(_("branch %s is being bisected at %s"),
			    లక్ష్యం, wt->మార్గం);
	}
}

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
స్థిర పూర్ణాంకం replace_each_worktree_తల_symref(నిర్మాణం worktree **worktrees,
					     స్థిరరాశి అక్షరం *oldref, స్థిరరాశి అక్షరం *newref,
					     స్థిరరాశి అక్షరం *logmsg)
{
	పూర్ణాంకం ret = 0;
	పూర్ణాంకం i;

	కొరకు (i = 0; worktrees[i]; i++) {
		నిర్మాణం ref_store *refs;

		ఒకవేళ (worktrees[i]->is_detached)
			కొనసాగు;
		ఒకవేళ (!worktrees[i]->తల_ref)
			కొనసాగు;
		ఒకవేళ (strcmp(oldref, worktrees[i]->తల_ref))
			కొనసాగు;

		refs = get_worktree_ref_store(worktrees[i]);
		ఒకవేళ (refs_update_symref(refs, "HEAD", newref, logmsg))
			ret = error(_("HEAD of working tree %s is not updated"),
				    worktrees[i]->మార్గం);
	}

	తిరిగి ret;
}

#నిర్వచించు IS_తల 1
#నిర్వచించు IS_ORPHAN 2

స్థిర శూన్య copy_or_rename_branch(స్థిరరాశి అక్షరం *oldname, స్థిరరాశి అక్షరం *newname, పూర్ణాంకం copy, పూర్ణాంకం force)
{
	నిర్మాణం strbuf oldref = STRBUF_INIT, newref = STRBUF_INIT, logmsg = STRBUF_INIT;
	నిర్మాణం strbuf oldsection = STRBUF_INIT, newsection = STRBUF_INIT;
	స్థిరరాశి అక్షరం *interpreted_oldname = ఖాళీ;
	స్థిరరాశి అక్షరం *interpreted_newname = ఖాళీ;
	పూర్ణాంకం recovery = 0, oldref_usage = 0;
	నిర్మాణం worktree **worktrees = get_worktrees(the_repository);

	ఒకవేళ (check_branch_ref(the_repository, &oldref, oldname)) {
		/*
		 * Bad name --- this could be an attempt to rename a
		 * ref that we used to allow to be created by accident.
		 */
		ఒకవేళ (refs_ref_exists(get_main_ref_store(the_repository), oldref.buf))
			recovery = 1;
		లేకపోతే {
			పూర్ణాంకం సంకేతం_2 = die_సందేశం(_("invalid branch name: '%s'"), oldname);
			advise_if_enabled(ADVICE_REF_SYNTAX,
					  _("See 'git help check-ref-format'"));
			నిష్క్రమించు(సంకేతం_2);
		}
	}

	కొరకు (పూర్ణాంకం i = 0; worktrees[i]; i++) {
		నిర్మాణం worktree *wt = worktrees[i];

		ఒకవేళ (wt->తల_ref && !strcmp(oldref.buf, wt->తల_ref)) {
			oldref_usage |= IS_తల;
			ఒకవేళ (is_null_oid(&wt->తల_oid))
				oldref_usage |= IS_ORPHAN;
			ఆపు;
		}
	}

	ఒకవేళ ((copy || !(oldref_usage & IS_తల)) && !refs_ref_exists(get_main_ref_store(the_repository), oldref.buf)) {
		ఒకవేళ (oldref_usage & IS_తల)
			die(_("no commit on branch '%s' yet"), oldname);
		లేకపోతే
			die(_("no branch named '%s'"), oldname);
	}

	/*
	 * A command like "git branch -M currentbranch currentbranch" cannot
	 * cause the worktree to become inconsistent with HEAD, so allow it.
	 */
	ఒకవేళ (!strcmp(oldname, newname))
		validate_branchname(newname, &newref);
	లేకపోతే
		validate_new_branchname(newname, &newref, force);

	reject_rebase_or_bisect_branch(worktrees, oldref.buf);

	ఒకవేళ (!skip_ఉపసర్గ(oldref.buf, "refs/heads/", &interpreted_oldname) ||
	    !skip_ఉపసర్గ(newref.buf, "refs/heads/", &interpreted_newname)) {
		BUG("expected prefix missing for refs");
	}

	ఒకవేళ (copy)
		strbuf_addf(&logmsg, "Branch: copied %s to %s",
			    oldref.buf, newref.buf);
	లేకపోతే
		strbuf_addf(&logmsg, "Branch: renamed %s to %s",
			    oldref.buf, newref.buf);

	ఒకవేళ (!copy && !(oldref_usage & IS_ORPHAN) &&
	    refs_rename_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch rename failed"));
	ఒకవేళ (copy && refs_copy_existing_ref(get_main_ref_store(the_repository), oldref.buf, newref.buf, logmsg.buf))
		die(_("branch copy failed"));

	ఒకవేళ (recovery) {
		ఒకవేళ (copy)
			warning(_("created a copy of a misnamed branch '%s'"),
				interpreted_oldname);
		లేకపోతే
			warning(_("renamed a misnamed branch '%s' away"),
				interpreted_oldname);
	}

	ఒకవేళ (!copy && (oldref_usage & IS_తల) &&
	    replace_each_worktree_తల_symref(worktrees, oldref.buf, newref.buf,
					      logmsg.buf))
		die(_("branch renamed to %s, but HEAD is not updated"), newname);

	strbuf_release(&logmsg);

	strbuf_addf(&oldsection, "branch.%s", interpreted_oldname);
	strbuf_addf(&newsection, "branch.%s", interpreted_newname);
	ఒకవేళ (!copy && repo_config_rename_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is renamed, but update of config-file failed"));
	ఒకవేళ (copy && strcmp(interpreted_oldname, interpreted_newname) &&
	    repo_config_copy_section(the_repository, oldsection.buf, newsection.buf) < 0)
		die(_("branch is copied, but update of config-file failed"));
	strbuf_release(&oldref);
	strbuf_release(&newref);
	strbuf_release(&oldsection);
	strbuf_release(&newsection);
	free_worktrees(worktrees);
}

స్థిర పూర్ణాంకం parse_opt_forked(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం ref_filter *filter = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	ఒకవేళ (ref_filter_forked_కూడు(filter, arg) < 0)
		die(_("'%s' is not a valid branch or pattern"), arg);
	తిరిగి 0;
}

నిర్మాణం stacked_branch_దత్తాంశం {
	నిర్మాణం strset *deletable_branch_పేర్లు;
	నిర్మాణం strset *protected_branch_పేర్లు;
};

స్థిర పూర్ణాంకం collect_stacked_branch_పునాది(స్థిరరాశి నిర్మాణం reference *ref,
				       శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం stacked_branch_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	స్థిరరాశి అక్షరం *branch_name;
	నిర్మాణం branch *branch;
	స్థిరరాశి అక్షరం *upstream_refname;
	స్థిరరాశి అక్షరం *upstream_branch_name;

	ఒకవేళ (!skip_ఉపసర్గ(ref->name, "refs/heads/", &branch_name))
		BUG("expected local branch ref, got '%s'", ref->name);
	ఒకవేళ (strset_కలిగియుంది(దత్తాంశం->deletable_branch_పేర్లు, branch_name))
		తిరిగి 0;

	branch = branch_get(branch_name);
	upstream_refname = branch_get_upstream(branch, ఖాళీ);
	ఒకవేళ (!upstream_refname ||
	    !skip_ఉపసర్గ(upstream_refname, "refs/heads/",
			 &upstream_branch_name) ||
	    !strset_కలిగియుంది(దత్తాంశం->deletable_branch_పేర్లు,
			    upstream_branch_name))
		తిరిగి 0;

	strset_కూడు(దత్తాంశం->protected_branch_పేర్లు, upstream_branch_name);
	తిరిగి 0;
}

స్థిర శూన్య protect_stacked_branch_bases(నిర్మాణం ref_store *refs,
					 నిర్మాణం strset *deletable_branch_పేర్లు,
					 నిర్మాణం strset *protected_branch_పేర్లు)
{
	నిర్మాణం stacked_branch_దత్తాంశం దత్తాంశం = {
		.deletable_branch_పేర్లు = deletable_branch_పేర్లు,
		.protected_branch_పేర్లు = protected_branch_పేర్లు,
	};
	నిర్మాణం refs_for_each_ref_options opts = {
		.ఉపసర్గ = "refs/heads/",
	};
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం strmap_నమోదు *నమోదు_2;

	refs_for_each_ref_ext(refs, collect_stacked_branch_పునాది, &దత్తాంశం, &opts);

	strset_for_each_నమోదు(protected_branch_పేర్లు, &iter, నమోదు_2)
		strset_remove(deletable_branch_పేర్లు, నమోదు_2->కీలకం);
}

స్థిర శూన్య clear_deleted_upstreams(నిర్మాణం strset *protected_branch_పేర్లు,
				    నిర్మాణం strset *deletable_branch_పేర్లు)
{
	నిర్మాణం strbuf కీలకం = STRBUF_INIT;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం strmap_నమోదు *నమోదు_2;

	strset_for_each_నమోదు(protected_branch_పేర్లు, &iter, నమోదు_2) {
		నిర్మాణం branch *branch = branch_get(నమోదు_2->కీలకం);
		స్థిరరాశి అక్షరం *upstream_refname = branch_get_upstream(branch, ఖాళీ);
		స్థిరరాశి అక్షరం *upstream_branch_name;

		ఒకవేళ (!upstream_refname ||
		    !skip_ఉపసర్గ(upstream_refname, "refs/heads/",
				 &upstream_branch_name) ||
		    !strset_కలిగియుంది(deletable_branch_పేర్లు,
				    upstream_branch_name))
			కొనసాగు;

		strbuf_addf(&కీలకం, "branch.%s.merge", branch->name);
		repo_config_సమితి_gently(the_repository, కీలకం.buf, ఖాళీ);
		strbuf_reset(&కీలకం);
		strbuf_addf(&కీలకం, "branch.%s.remote", branch->name);
		repo_config_సమితి_gently(the_repository, కీలకం.buf, ఖాళీ);
		strbuf_reset(&కీలకం);
	}

	strbuf_release(&కీలకం);
}

స్థిర పూర్ణాంకం branch_pushes_గమ్యం_upstream(నిర్మాణం branch *branch,
				     స్థిరరాశి అక్షరం *upstream)
{
	నిర్మాణం remote *remote = remote_get(remote_for_branch(branch, ఖాళీ));
	అక్షరం *నెట్టు_refname = ఖాళీ;
	అక్షరం *tracking = ఖాళీ;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (!remote)
		తిరిగి 0;
	ఒకవేళ (remote->నెట్టు.nr)
		నెట్టు_refname = apply_refspecs(&remote->నెట్టు, branch->refname);
	లేకపోతే
		నెట్టు_refname = xstrdup(branch->refname);
	ఒకవేళ (నెట్టు_refname)
		tracking = apply_refspecs(&remote->fetch, నెట్టు_refname);
	ఒకవేళ (tracking && !strcmp(tracking, upstream))
		ret = 1;

	విడుదల(నెట్టు_refname);
	విడుదల(tracking);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం delete_merged_branches(స్థిరరాశి నిర్మాణం strvec *upstreams,
				 స్థిరరాశి అక్షరం **argv, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం ref_store *refs = get_main_ref_store(the_repository);
	నిర్మాణం ref_filter filter = REF_FILTER_INIT;
	నిర్మాణం ref_array candidates = { 0 };
	నిర్మాణం strset deletable_branch_పేర్లు = STRSET_INIT;
	నిర్మాణం strset protected_branch_పేర్లు = STRSET_INIT;
	నిర్మాణం strvec branches_గమ్యం_delete = STRVEC_INIT;
	నిర్మాణం strbuf కీలకం = STRBUF_INIT;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం strmap_నమోదు *నమోదు_2;
	పూర్ణాంకం ret = 0;

	కొరకు (కొలత_t i = 0; i < upstreams->nr; i++)
		ఒకవేళ (ref_filter_forked_కూడు(&filter, upstreams->v[i]) < 0)
			die(_("'%s' is not a valid branch or pattern"),
			    upstreams->v[i]);

	filter.రకం = FILTER_REFS_BRANCHES;
	filter.name_patterns = argv;
	filter_refs(&candidates, &filter, filter.రకం);

	కొరకు (పూర్ణాంకం i = 0; i < candidates.nr; i++) {
		స్థిరరాశి అక్షరం *branch_refname = candidates.వస్తువులు[i]->refname;
		స్థిరరాశి అక్షరం *branch_name;
		నిర్మాణం branch *branch;
		స్థిరరాశి అక్షరం *upstream_refname;
		పూర్ణాంకం opt_బహిర్గతం;

		ఒకవేళ (!skip_ఉపసర్గ(branch_refname, "refs/heads/", &branch_name))
			BUG("filter returned non-branch ref '%s'", branch_refname);
		ఒకవేళ (branch_checked_బహిర్గతం(branch_refname))
			కొనసాగు;

		branch = branch_get(branch_name);
		upstream_refname = branch_get_upstream(branch, ఖాళీ);
		ఒకవేళ (!upstream_refname || !refs_ref_exists(refs, upstream_refname))
			కొనసాగు;
		ఒకవేళ (branch_pushes_గమ్యం_upstream(branch, upstream_refname))
			కొనసాగు;
		ఒకవేళ (check_branch_commit(branch_name, branch_name,
					&candidates.వస్తువులు[i]->objectname, ఖాళీ,
					FILTER_REFS_BRANCHES, DELETE_BRANCH_SKIP_UNMERGED))
			కొనసాగు;

		strbuf_reset(&కీలకం);
		strbuf_addf(&కీలకం, "branch.%s.deletemerged", branch_name);
		ఒకవేళ (!repo_config_get_bool(the_repository, కీలకం.buf, &opt_బహిర్గతం) &&
		    !opt_బహిర్గతం) {
			ఒకవేళ (!(flags & DELETE_BRANCH_QUIET))
				fprintf(stderr,
					_("Skipping '%s' (branch.%s.deleteMerged is false)\n"),
					branch_name, branch_name);
			కొనసాగు;
		}

		strset_కూడు(&deletable_branch_పేర్లు, branch_name);
	}

	protect_stacked_branch_bases(refs, &deletable_branch_పేర్లు,
				     &protected_branch_పేర్లు);

	strset_for_each_నమోదు(&deletable_branch_పేర్లు, &iter, నమోదు_2)
		strvec_నెట్టు(&branches_గమ్యం_delete, నమోదు_2->కీలకం);

	ఒకవేళ (branches_గమ్యం_delete.nr)
		ret = delete_branches(branches_గమ్యం_delete.nr, branches_గమ్యం_delete.v,
				      FILTER_REFS_BRANCHES,
				      DELETE_BRANCH_SKIP_UNMERGED |
				      DELETE_BRANCH_NO_తల_FALLBACK |
				      flags);

	ఒకవేళ (!ret && !(flags & DELETE_BRANCH_DRY_నిరంతరం))
		clear_deleted_upstreams(&protected_branch_పేర్లు,
					&deletable_branch_పేర్లు);

	strbuf_release(&కీలకం);
	strvec_clear(&branches_గమ్యం_delete);
	strset_clear(&protected_branch_పేర్లు);
	strset_clear(&deletable_branch_పేర్లు);
	ref_array_clear(&candidates);
	ref_filter_clear(&filter);
	తిరిగి ret;
}

స్థిర GIT_మార్గం_FUNC(edit_description, "EDIT_DESCRIPTION")

స్థిర పూర్ణాంకం edit_branch_description(స్థిరరాశి అక్షరం *branch_name)
{
	పూర్ణాంకం exists;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf name = STRBUF_INIT;

	exists = !read_branch_desc(&buf, branch_name);
	ఒకవేళ (!buf.len || buf.buf[buf.len-1] != '\n')
		strbuf_addch(&buf, '\n');
	strbuf_commented_addf(&buf, comment_పంక్తి_str,
		    _("Please edit the description for the branch\n"
		      "  %s\n"
		      "Lines starting with '%s' will be stripped.\n"),
		    branch_name, comment_పంక్తి_str);
	write_file_buf(edit_description(), buf.buf, buf.len);
	strbuf_reset(&buf);
	ఒకవేళ (launch_editor(edit_description(), &buf, ఖాళీ)) {
		strbuf_release(&buf);
		తిరిగి -1;
	}
	strbuf_stripspace(&buf, comment_పంక్తి_str);

	strbuf_addf(&name, "branch.%s.description", branch_name);
	ఒకవేళ (buf.len || exists)
		repo_config_సమితి(the_repository, name.buf, buf.len ? buf.buf : ఖాళీ);
	strbuf_release(&name);
	strbuf_release(&buf);

	తిరిగి 0;
}

స్థిర శూన్య die_if_upstream_looks_like_remote(స్థిరరాశి అక్షరం *new_upstream, స్థిరరాశి అక్షరం *branch_name)
{
	నిర్మాణం strbuf remote_ref = STRBUF_INIT;
	పూర్ణాంకం సంకేతం_2;

	ఒకవేళ (strchr(new_upstream, '/') ||
	    !remote_is_configured(remote_get(new_upstream), 0))
		తిరిగి;

	strbuf_addf(&remote_ref, "refs/remotes/%s/%s", new_upstream, branch_name);
	ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), remote_ref.buf)) {
		strbuf_release(&remote_ref);
		తిరిగి;
	}

	సంకేతం_2 = die_సందేశం(_("--set-upstream-to takes a single <remote>/<branch> argument"));
	advise_if_enabled(ADVICE_సమితి_UPSTREAM_FAILURE,
			  _("Did you mean to use: git branch --set-upstream-to=%s/%s?"),
			  new_upstream, branch_name);
	strbuf_release(&remote_ref);
	నిష్క్రమించు(సంకేతం_2);
}

పూర్ణాంకం cmd_branch(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	/* possible actions */
	పూర్ణాంకం delete = 0, rename = 0, copy = 0, list = 0,
	    unset_upstream = 0, show_current = 0, edit_description = 0;
	నిర్మాణం strvec delete_merged = STRVEC_INIT;
	పూర్ణాంకం dry_నిరంతరం = 0;
	స్థిరరాశి అక్షరం *new_upstream = ఖాళీ;
	పూర్ణాంకం noncreate_actions = 0;
	/* possible options */
	పూర్ణాంకం reflog = 0, quiet = 0, icase = 0, force = 0,
	    recurse_submodules_explicit = 0;
	గణన branch_track track;
	నిర్మాణం ref_filter filter = REF_FILTER_INIT;
	స్థిర నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ;
	నిర్మాణం string_list క్రమీకరణ_options = STRING_LIST_INIT_DUP;
	నిర్మాణం ref_format format = REF_FORMAT_INIT;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	పూర్ణాంకం ret;

	నిర్మాణం option options[] = {
		OPT_సమూహం(N_("Generic options")),
		OPT__VERBOSE(&filter.verbose,
			N_("show hash and subject, give twice for upstream branch")),
		OPT__QUIET(&quiet, N_("suppress informational messages")),
		OPT_CALLBACK_F('t', "track",  &track, "(direct|inherit)",
			N_("set branch tracking configuration"),
			PARSE_OPT_OPTARG,
			parse_opt_tracking_బహుళకం),
		OPT_సమితి_INT_F(0, "set-upstream", &track, N_("do not use"),
			BRANCH_TRACK_OVERRIDE, PARSE_OPT_HIDDEN),
		OPT_STRING('u', "set-upstream-to", &new_upstream, N_("upstream"), N_("change the upstream info")),
		OPT_BOOL(0, "unset-upstream", &unset_upstream, N_("unset the upstream info")),
		OPT__COLOR(&branch_use_color, N_("use colored output")),
		OPT_సమితి_INT_F('r', "remotes",     &filter.రకం, N_("act on remote-tracking branches"),
			      FILTER_REFS_REMOTES,
			      PARSE_OPT_NONEG),
		OPT_కలిగియుంది(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_NO_కలిగియుంది(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only branches that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only branches that don't contain the commit")),
		OPT__ABBREV(&filter.abbrev),

		OPT_సమూహం(N_("Specific git-branch actions:")),
		OPT_సమితి_INT_F('a', "all", &filter.రకం, N_("list both remote-tracking and local branches"),
			      FILTER_REFS_REMOTES | FILTER_REFS_BRANCHES,
			      PARSE_OPT_NONEG),
		OPT_బిట్టు('d', "delete", &delete, N_("delete fully merged branch"), 1),
		OPT_బిట్టు('D', ఖాళీ, &delete, N_("delete branch (even if not merged)"), 2),
		OPT_బిట్టు('m', "move", &rename, N_("move/rename a branch and its reflog"), 1),
		OPT_బిట్టు('M', ఖాళీ, &rename, N_("move/rename a branch, even if target exists"), 2),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ఖాళీలు,
			N_("do not output a newline after empty formatted refs")),
		OPT_బిట్టు('c', "copy", &copy, N_("copy a branch and its reflog"), 1),
		OPT_బిట్టు('C', ఖాళీ, &copy, N_("copy a branch, even if target exists"), 2),
		OPT_BOOL('l', "list", &list, N_("list branch names")),
		OPT_BOOL(0, "show-current", &show_current, N_("show current branch name")),
		OPT_BOOL(0, "create-reflog", &reflog, N_("create the branch's reflog")),
		OPT_BOOL(0, "edit-description", &edit_description,
			 N_("edit the description for the branch")),
		OPT_CALLBACK_F(0, "delete-merged", &delete_merged, N_("pattern"),
			N_("delete merged branches whose upstream matches <pattern> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_strvec),
		OPT_BOOL(0, "dry-run", &dry_నిరంతరం,
			N_("with --delete-merged, only print which branches would be deleted")),
		OPT__FORCE(&force, N_("force creation, move/rename, deletion"), PARSE_OPT_NOCOMPLETE),
		OPT_MERGED(&filter, N_("print only branches that are merged")),
		OPT_NO_MERGED(&filter, N_("print only branches that are not merged")),
		OPT_CALLBACK_F(0, "forked", &filter, N_("branch"),
			N_("print only branches whose upstream matches <branch> (repeatable)"),
			PARSE_OPT_NONEG, parse_opt_forked),
		OPT_స్తంభం(0, "column", &colopts, N_("list branches in columns")),
		OPT_REF_క్రమీకరించు(&క్రమీకరణ_options),
		OPT_CALLBACK(0, "points-at", &filter.points_at, N_("object"),
			N_("print only branches of the object"), parse_opt_object_name),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules_explicit, N_("recurse through submodules")),
		OPT_STRING(  0 , "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_చివర(),
	};

	అమరిక_ref_filter_porcelain_సందేశం();

	filter.రకం = FILTER_REFS_BRANCHES;
	filter.abbrev = -1;

	show_usage_with_options_if_asked(argc, argv,
					 builtin_branch_usage, options);

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_branch_config, &క్రమీకరణ_options);
	ఒకవేళ (!క్రమీకరణ_options.nr)
		string_list_append(&క్రమీకరణ_options, "refname");

	track = cfg->branch_track;

	తల = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   0, &తల_oid, ఖాళీ);
	ఒకవేళ (!తల)
		die(_("failed to resolve HEAD as a valid ref"));
	ఒకవేళ (!strcmp(తల, "HEAD"))
		filter.detached = 1;
	లేకపోతే ఒకవేళ (!skip_ఉపసర్గ(తల, "refs/heads/", &తల))
		die(_("HEAD not found below refs/heads!"));

	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_branch_usage,
			     0);

	ఒకవేళ (!delete && !rename && !copy && !edit_description && !new_upstream &&
	    !show_current && !unset_upstream && !delete_merged.nr &&
	    argc == 0)
		list = 1;

	ఒకవేళ (filter.with_commit || filter.no_commit ||
	    filter.reachable_from || filter.unreachable_from ||
	    filter.points_at.nr || filter.forked.nr)
		list = 1;

	noncreate_actions = !!delete + !!rename + !!copy + !!new_upstream +
			    !!show_current + !!list + !!edit_description +
			    !!unset_upstream + !!delete_merged.nr;
	ఒకవేళ (noncreate_actions > 1)
		usage_with_options(builtin_branch_usage, options);

	ఒకవేళ (dry_నిరంతరం && !delete_merged.nr)
		die(_("--dry-run requires --delete-merged"));

	ఒకవేళ (recurse_submodules_explicit) {
		ఒకవేళ (!submodule_propagate_branches)
			die(_("branch with --recurse-submodules can only be used if submodule.propagateBranches is enabled"));
		ఒకవేళ (noncreate_actions)
			die(_("--recurse-submodules can only be used to create branches"));
	}

	recurse_submodules =
		(recurse_submodules || recurse_submodules_explicit) &&
		submodule_propagate_branches;

	ఒకవేళ (filter.abbrev == -1)
		filter.abbrev = DEFAULT_ABBREV;
	filter.ignore_case = icase;

	finalize_colopts(&colopts, -1);
	ఒకవేళ (filter.verbose) {
		ఒకవేళ (explicitly_enable_స్తంభం(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "--verbose");
		colopts = 0;
	}

	ఒకవేళ (force) {
		delete *= 2;
		rename *= 2;
		copy *= 2;
	}

	ఒకవేళ (list)
		అమరిక_auto_pager("branch", 1);

	ఒకవేళ (delete) {
		ఒకవేళ (!argc)
			die(_("branch name required"));
		ret = delete_branches(argc, argv, filter.రకం,
				      (delete > 1 ? DELETE_BRANCH_FORCE : 0) |
				      (quiet ? DELETE_BRANCH_QUIET : 0));
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (delete_merged.nr) {
		ret = delete_merged_branches(&delete_merged, argv,
					     (quiet ? DELETE_BRANCH_QUIET : 0) |
					     (dry_నిరంతరం ? DELETE_BRANCH_DRY_నిరంతరం : 0));
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (show_current) {
		print_current_branch_name();
		ret = 0;
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (list) {
		/*  git branch --list also shows HEAD when it is detached */
		ఒకవేళ ((filter.రకం & FILTER_REFS_BRANCHES) && filter.detached)
			filter.రకం |= FILTER_REFS_DETACHED_తల;
		filter.name_patterns = argv;
		/*
		 * If no sorting parameter is given then we default to sorting
		 * by 'refname'. This would give us an alphabetically sorted
		 * array with the 'HEAD' ref at the beginning followed by
		 * local branches 'refs/heads/...' and finally remote-tracking
		 * branches 'refs/remotes/...'.
		 */
		క్రమీకరణ = ref_క్రమీకరణ_options(&క్రమీకరణ_options);
		ref_క్రమీకరణ_సమితి_క్రమీకరించు_flags_all(క్రమీకరణ, REF_క్రమీకరణ_ICASE, icase);
		ref_క్రమీకరణ_సమితి_క్రమీకరించు_flags_all(
			క్రమీకరణ, REF_క్రమీకరణ_DETACHED_తల_మొదటి, 1);
		print_ref_list(&filter, క్రమీకరణ, &format, &output);
		print_columns(&output, colopts, ఖాళీ);
		string_list_clear(&output, 0);
		ref_క్రమీకరణ_release(క్రమీకరణ);
		ref_filter_clear(&filter);

		ret = 0;
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (edit_description) {
		స్థిరరాశి అక్షరం *branch_name;
		నిర్మాణం strbuf branch_ref = STRBUF_INIT;
		నిర్మాణం strbuf buf = STRBUF_INIT;

		ఒకవేళ (!argc) {
			ఒకవేళ (filter.detached)
				die(_("cannot give description to detached HEAD"));
			branch_name = తల;
		} లేకపోతే ఒకవేళ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch_name = buf.buf;
		} లేకపోతే {
			die(_("cannot edit description of more than one branch"));
		}

		strbuf_addf(&branch_ref, "refs/heads/%s", branch_name);
		ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), branch_ref.buf)) {
			error((!argc || branch_checked_బహిర్గతం(branch_ref.buf))
			      ? _("no commit on branch '%s' yet")
			      : _("no branch named '%s'"),
			      branch_name);
			ret = 1;
		} లేకపోతే ఒకవేళ (!edit_branch_description(branch_name)) {
			ret = 0; /* happy */
		} లేకపోతే {
			ret = 1;
		}

		strbuf_release(&branch_ref);
		strbuf_release(&buf);

		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (copy || rename) {
		ఒకవేళ (!argc)
			die(_("branch name required"));
		లేకపోతే ఒకవేళ ((argc == 1) && filter.detached)
			die(copy? _("cannot copy the current branch while not on any")
				: _("cannot rename the current branch while not on any"));
		లేకపోతే ఒకవేళ (argc == 1)
			copy_or_rename_branch(తల, argv[0], copy, copy + rename > 1);
		లేకపోతే ఒకవేళ (argc == 2)
			copy_or_rename_branch(argv[0], argv[1], copy, copy + rename > 1);
		లేకపోతే
			die(copy? _("too many branches for a copy operation")
				: _("too many arguments for a rename operation"));
	} లేకపోతే ఒకవేళ (new_upstream) {
		నిర్మాణం branch *branch;
		నిర్మాణం strbuf buf = STRBUF_INIT;

		ఒకవేళ (!argc)
			branch = branch_get(ఖాళీ);
		లేకపోతే ఒకవేళ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} లేకపోతే
			die(_("too many arguments to set new upstream"));

		ఒకవేళ (!branch) {
			ఒకవేళ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not set upstream of HEAD to %s when "
				      "it does not point to any branch"),
				    new_upstream);
			die(_("no such branch '%s'"), argv[0]);
		}

		ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), branch->refname)) {
			ఒకవేళ (!argc || branch_checked_బహిర్గతం(branch->refname))
				die(_("no commit on branch '%s' yet"), branch->name);
			/*
			 * Check the advice up front to avoid the ref
			 * lookups when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ఒకవేళ (argc == 1 &&
			    advice_enabled(ADVICE_సమితి_UPSTREAM_FAILURE))
				die_if_upstream_looks_like_remote(new_upstream, argv[0]);
			die(_("branch '%s' does not exist"), branch->name);
		}

		dwim_and_అమరిక_tracking(the_repository, branch->name,
					new_upstream, BRANCH_TRACK_OVERRIDE,
					quiet);
		strbuf_release(&buf);
	} లేకపోతే ఒకవేళ (unset_upstream) {
		నిర్మాణం branch *branch;
		నిర్మాణం strbuf buf = STRBUF_INIT;

		ఒకవేళ (!argc)
			branch = branch_get(ఖాళీ);
		లేకపోతే ఒకవేళ (argc == 1) {
			copy_branchname(the_repository, &buf, argv[0],
					INTERPRET_BRANCH_LOCAL);
			branch = branch_get(buf.buf);
		} లేకపోతే
			die(_("too many arguments to unset upstream"));

		ఒకవేళ (!branch) {
			ఒకవేళ (!argc || !strcmp(argv[0], "HEAD"))
				die(_("could not unset upstream of HEAD when "
				      "it does not point to any branch"));
			die(_("no such branch '%s'"), argv[0]);
		}

		ఒకవేళ (!branch_has_విలీనం_config(branch))
			die(_("branch '%s' has no upstream information"), branch->name);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.remote", branch->name);
		repo_config_సమితి_multivar(the_repository, buf.buf, ఖాళీ, ఖాళీ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_reset(&buf);
		strbuf_addf(&buf, "branch.%s.merge", branch->name);
		repo_config_సమితి_multivar(the_repository, buf.buf, ఖాళీ, ఖాళీ, CONFIG_FLAGS_MULTI_REPLACE);
		strbuf_release(&buf);
	} లేకపోతే ఒకవేళ (!noncreate_actions && argc > 0 && argc <= 2) {
		స్థిరరాశి అక్షరం *branch_name = argv[0];
		స్థిరరాశి అక్షరం *ప్రారంభం_name = argc == 2 ? argv[1] : తల;

		ఒకవేళ (filter.రకం != FILTER_REFS_BRANCHES)
			die(_("the -a, and -r, options to 'git branch' do not take a branch name.\n"
				  "Did you mean to use: -a|-r --list <pattern>?"));

		ఒకవేళ (track == BRANCH_TRACK_OVERRIDE)
			die(_("the '--set-upstream' option is no longer supported. Please use '--track' or '--set-upstream-to' instead"));

		ఒకవేళ (recurse_submodules) {
			create_branches_recursively(the_repository, branch_name,
						    ప్రారంభం_name, ఖాళీ, force,
						    reflog, quiet, track, 0);
			ret = 0;
			వెళ్ళు బహిర్గతం;
		}
		create_branch(the_repository, branch_name, ప్రారంభం_name, force, 0,
			      reflog, quiet, track, 0);
	} లేకపోతే
		usage_with_options(builtin_branch_usage, options);

	ret = 0;

బహిర్గతం:
	strvec_clear(&delete_merged);
	string_list_clear(&క్రమీకరణ_options, 0);
	తిరిగి ret;
}
