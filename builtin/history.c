#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "merge-ort.h"
#అన్తర్భావ "oidmap.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "ref-filter.h"
#అన్తర్భావ "replay.h"
#అన్తర్భావ "reset.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "wt-status.h"

#నిర్వచించు GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#నిర్వచించు GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#నిర్వచించు GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#నిర్వచించు GIT_HISTORY_విభజించు_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#నిర్వచించు GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

స్థిర శూన్య change_దత్తాంశం_free(శూన్య *util, స్థిరరాశి అక్షరం *str వాడని)
{
	నిర్మాణం wt_స్థితి_change_దత్తాంశం *d = util;
	విడుదల(d->rename_ఆధారం);
	విడుదల(d);
}

స్థిర పూర్ణాంకం fill_commit_సందేశం(నిర్మాణం repository *repo,
			       స్థిరరాశి నిర్మాణం object_id *old_వృక్షం,
			       స్థిరరాశి నిర్మాణం object_id *new_వృక్షం,
			       స్థిరరాశి అక్షరం *default_సందేశం,
			       స్థిరరాశి అక్షరం *action,
			       నిర్మాణం strbuf *బహిర్గతం)
{
	స్థిరరాశి అక్షరం *మార్గం = git_మార్గం_commit_editmsg();
	స్థిరరాశి అక్షరం *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	నిర్మాణం wt_స్థితి s;

	wt_స్థితి_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_శీఘ్ర;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_ఉపసర్గ = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(మార్గం, "w");
	ఒకవేళ (!s.fp)
		తిరిగి error_errno(_("could not open '%s'"), మార్గం);

	strbuf_addstr(బహిర్గతం, default_సందేశం);
	strbuf_addch(బహిర్గతం, '\n');
	strbuf_commented_addf(బహిర్గతం, comment_పంక్తి_str, hint, action, comment_పంక్తి_str);
	ఒకవేళ (fwrite(బహిర్గతం->buf, 1, బహిర్గతం->len, s.fp) != బహిర్గతం->len)
		die_errno(_("could not write to '%s'"), మార్గం);

	wt_స్థితి_collect_changes_trees(&s, old_వృక్షం, new_వృక్షం);
	wt_స్థితి_print(&s);
	wt_స్థితి_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_దత్తాంశం_free);
	ఒకవేళ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), మార్గం);

	strbuf_reset(బహిర్గతం);
	ఒకవేళ (launch_editor(మార్గం, బహిర్గతం, ఖాళీ)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		తిరిగి -1;
	}
	strbuf_stripspace(బహిర్గతం, comment_పంక్తి_str);

	cleanup_సందేశం(బహిర్గతం, COMMIT_సందేశం_CLEANUP_ALL, 0);

	ఒకవేళ (!బహిర్గతం->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		తిరిగి -1;
	}

	తిరిగి 0;
}

గణన commit_వృక్షం_flags {
	COMMIT_వృక్షం_EDIT_సందేశం = (1 << 0),
};

స్థిర పూర్ణాంకం commit_వృక్షం_ext(నిర్మాణం repository *repo,
			   స్థిరరాశి అక్షరం *action,
			   నిర్మాణం commit *commit_with_సందేశం,
			   స్థిరరాశి అక్షరం *సందేశం_template,
			   స్థిరరాశి నిర్మాణం commit_list *parents,
			   స్థిరరాశి నిర్మాణం object_id *old_వృక్షం,
			   స్థిరరాశి నిర్మాణం object_id *new_వృక్షం,
			   నిర్మాణం commit **బహిర్గతం,
			   గణన commit_వృక్షం_flags flags)
{
	స్థిరరాశి అక్షరం *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ఖాళీ,
	};
	స్థిరరాశి అక్షరం *original_సందేశం, *original_body, *ptr;
	నిర్మాణం commit_extra_శీర్షిక *original_extra_శీర్షికలు = ఖాళీ;
	నిర్మాణం strbuf commit_సందేశం_2 = STRBUF_INIT;
	నిర్మాణం object_id rewritten_commit_oid;
	అక్షరం *original_author = ఖాళీ;
	కొలత_t len;
	పూర్ణాంకం ret;

	/* We retain authorship of the original commit. */
	original_సందేశం = repo_logmsg_reencode(repo, commit_with_సందేశం, ఖాళీ, ఖాళీ);
	ptr = కనుగొను_commit_శీర్షిక(original_సందేశం, "author", &len);
	ఒకవేళ (ptr)
		original_author = xmemdupz(ptr, len);
	కనుగొను_commit_subject(original_సందేశం, &original_body);

	ఒకవేళ (!సందేశం_template)
		సందేశం_template = original_body;

	ఒకవేళ (flags & COMMIT_వృక్షం_EDIT_సందేశం) {
		ret = fill_commit_సందేశం(repo, old_వృక్షం, new_వృక్షం,
					  సందేశం_template, action, &commit_సందేశం_2);
		ఒకవేళ (ret < 0)
			వెళ్ళు బహిర్గతం;
	} లేకపోతే {
		strbuf_addstr(&commit_సందేశం_2, సందేశం_template);
	}

	original_extra_శీర్షికలు = read_commit_extra_శీర్షికలు(commit_with_సందేశం,
							   exclude_gpgsig);

	ret = commit_వృక్షం_extended(commit_సందేశం_2.buf, commit_సందేశం_2.len, new_వృక్షం,
				   parents, &rewritten_commit_oid, original_author,
				   ఖాళీ, ఖాళీ, original_extra_శీర్షికలు);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	*బహిర్గతం = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

బహిర్గతం:
	free_commit_extra_శీర్షికలు(original_extra_శీర్షికలు);
	strbuf_release(&commit_సందేశం_2);
	విడుదల(original_author);
	repo_unuse_commit_బఫరు(repo, commit_with_సందేశం, original_సందేశం);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం మొదటి_మాతృ_వృక్షం_oid(నిర్మాణం repository *repo,
				 నిర్మాణం commit *commit,
				 నిర్మాణం object_id *బహిర్గతం)
{
	నిర్మాణం commit *మాతృ = commit->parents ? commit->parents->అంశం : ఖాళీ;

	ఒకవేళ (!మాతృ) {
		oidcpy(బహిర్గతం, repo->hash_algo->ఖాళీలు_వృక్షం);
		తిరిగి 0;
	}

	ఒకవేళ (repo_parse_commit(repo, మాతృ))
		తిరిగి error(_("unable to parse parent commit %s"),
			     oid_గమ్యం_hex(&మాతృ->object.oid));

	oidcpy(బహిర్గతం, &repo_get_commit_వృక్షం(repo, మాతృ)->object.oid);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం commit_వృక్షం_with_edited_సందేశం(నిర్మాణం repository *repo,
					   స్థిరరాశి అక్షరం *action,
					   నిర్మాణం commit *original,
					   నిర్మాణం commit **బహిర్గతం)
{
	నిర్మాణం object_id మాతృ_వృక్షం_oid;
	స్థిరరాశి నిర్మాణం object_id *వృక్షం_oid;

	వృక్షం_oid = &repo_get_commit_వృక్షం(repo, original)->object.oid;

	ఒకవేళ (మొదటి_మాతృ_వృక్షం_oid(repo, original, &మాతృ_వృక్షం_oid) < 0)
		తిరిగి -1;

	తిరిగి commit_వృక్షం_ext(repo, action, original, ఖాళీ, original->parents,
			       &మాతృ_వృక్షం_oid, వృక్షం_oid, బహిర్గతం, COMMIT_వృక్షం_EDIT_సందేశం);
}

గణన ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_తల,
};

స్థిర పూర్ణాంకం parse_ref_action(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *విలువ, పూర్ణాంకం unset)
{
	గణన ref_action *action = opt->విలువ;

	BUG_ON_OPT_NEG_NOARG(unset, విలువ);
	ఒకవేళ (!strcmp(విలువ, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} లేకపోతే ఒకవేళ (!strcmp(విలువ, "head")) {
		*action = REF_ACTION_తల;
	} లేకపోతే {
		తిరిగి error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం revwalk_కలిగియుంది_merges(నిర్మాణం repository *repo,
				   స్థిరరాశి నిర్మాణం strvec *revwalk_వాదనలు)
{
	నిర్మాణం strvec వాదనలు = STRVEC_INIT;
	నిర్మాణం rev_info revs;
	పూర్ణాంకం ret;

	strvec_pushv(&వాదనలు, revwalk_వాదనలు->v);
	strvec_నెట్టు(&వాదనలు, "--min-parents=2");

	repo_init_revisions(repo, &revs, ఖాళీ);

	అమరిక_revisions_from_strvec(&వాదనలు, &revs, ఖాళీ);
	ఒకవేళ (వాదనలు.nr != 1)
		BUG("revisions were set up with invalid argument");

	ఒకవేళ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		వెళ్ళు బహిర్గతం;
	}

	reset_revision_walk();
	ret = 0;

బహిర్గతం:
	release_revisions(&revs);
	strvec_clear(&వాదనలు);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం అమరిక_revwalk(నిర్మాణం repository *repo,
			 గణన ref_action action,
			 నిర్మాణం commit *original,
			 నిర్మాణం rev_info *revs)
{
	నిర్మాణం strvec వాదనలు = STRVEC_INIT;
	పూర్ణాంకం ret;

	repo_init_revisions(repo, revs, ఖాళీ);
	strvec_నెట్టు(&వాదనలు, "ignored");
	strvec_నెట్టు(&వాదనలు, "--reverse");
	strvec_నెట్టు(&వాదనలు, "--topo-order");
	strvec_నెట్టు(&వాదనలు, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&వాదనలు, "--ancestry-path=%s",
		     oid_గమ్యం_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&వాదనలు, "^%s", oid_గమ్యం_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	ఒకవేళ (action == REF_ACTION_తల) {
		నిర్మాణం commit_list *from_list = ఖాళీ;
		నిర్మాణం commit *తల;

		తల = lookup_commit_reference_by_name("HEAD");
		ఒకవేళ (!తల) {
			ret = error(_("cannot look up HEAD"));
			వెళ్ళు బహిర్గతం;
		}

		commit_list_చొప్పించు(original, &from_list);
		ret = repo_is_descendant_యొక్క(repo, తల, from_list);
		commit_list_free(from_list);

		ఒకవేళ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			వెళ్ళు బహిర్గతం;
		} లేకపోతే ఒకవేళ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			వెళ్ళు బహిర్గతం;
		}

		strvec_నెట్టు(&వాదనలు, "HEAD");
	} లేకపోతే {
		strvec_నెట్టు(&వాదనలు, "--branches");
		strvec_నెట్టు(&వాదనలు, "HEAD");
	}

	ret = revwalk_కలిగియుంది_merges(repo, &వాదనలు);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	అమరిక_revisions_from_strvec(&వాదనలు, revs, ఖాళీ);
	ఒకవేళ (వాదనలు.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

బహిర్గతం:
	strvec_clear(&వాదనలు);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం handle_ref_update(నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *refname,
			     స్థిరరాశి నిర్మాణం object_id *new_oid,
			     స్థిరరాశి నిర్మాణం object_id *old_oid,
			     స్థిరరాశి అక్షరం *reflog_సందేశం,
			     నిర్మాణం strbuf *err)
{
	ఒకవేళ (!transaction) {
		రాయి("update %s %s %s\n",
		       refname, oid_గమ్యం_hex(new_oid), oid_గమ్యం_hex(old_oid));
		తిరిగి 0;
	}

	తిరిగి ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ఖాళీ, ఖాళీ, 0, reflog_సందేశం, err);
}

స్థిర పూర్ణాంకం compute_pending_ref_updates(నిర్మాణం rev_info *revs,
				       గణన ref_action action,
				       నిర్మాణం commit *original,
				       నిర్మాణం commit *rewritten,
				       గణన replay_ఖాళీలు_commit_action ఖాళీలు,
				       నిర్మాణం replay_ఫలితం *ఫలితం)
{
	స్థిరరాశి నిర్మాణం name_decoration *decoration;
	నిర్మాణం replay_revisions_options opts = {
		.ఖాళీలు = ఖాళీలు,
	};
	అక్షరం hex[GIT_MAX_HEXSZ + 1];
	bool detached_తల;
	పూర్ణాంకం తల_flags = 0;
	పూర్ణాంకం ret;

	refs_read_ref_నిండిన(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ఖాళీ, &తల_flags);
	detached_తల = !(తల_flags & REF_ISSYMREF);

	opts.onto = oid_గమ్యం_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, ఫలితం);
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (action != REF_ACTION_BRANCHES && action != REF_ACTION_తల)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	కొరకు (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->తదుపరి)
	{
		ఒకవేళ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_తల)
			కొనసాగు;

		ఒకవేళ (action == REF_ACTION_తల &&
		    decoration->type != DECORATION_REF_తల)
			కొనసాగు;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		ఒకవేళ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_తల &&
		    !detached_తల)
			కొనసాగు;

		replay_ఫలితం_వరుస_update(ఫలితం, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం apply_pending_ref_updates(నిర్మాణం repository *repo,
				     స్థిరరాశి నిర్మాణం replay_ఫలితం *ఫలితం,
				     స్థిరరాశి అక్షరం *reflog_సందేశం,
				     పూర్ణాంకం dry_నిరంతరం)
{
	నిర్మాణం ref_transaction *transaction = ఖాళీ;
	నిర్మాణం strbuf err = STRBUF_INIT;
	పూర్ణాంకం ret;

	ఒకవేళ (!dry_నిరంతరం) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		ఒకవేళ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			వెళ్ళు బహిర్గతం;
		}
	}

	కొరకు (కొలత_t i = 0; i < ఫలితం->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					ఫలితం->updates[i].refname,
					&ఫలితం->updates[i].new_oid,
					&ఫలితం->updates[i].old_oid,
					reflog_సందేశం, &err);
		ఒకవేళ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    ఫలితం->updates[i].refname, err.buf);
			వెళ్ళు బహిర్గతం;
		}
	}

	ఒకవేళ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం handle_reference_updates(నిర్మాణం rev_info *revs,
				    గణన ref_action action,
				    నిర్మాణం commit *original,
				    నిర్మాణం commit *rewritten,
				    స్థిరరాశి అక్షరం *reflog_సందేశం,
				    పూర్ణాంకం dry_నిరంతరం,
				    గణన replay_ఖాళీలు_commit_action ఖాళీలు)
{
	నిర్మాణం replay_ఫలితం ఫలితం = { 0 };
	పూర్ణాంకం ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  ఖాళీలు, &ఫలితం);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ret = apply_pending_ref_updates(revs->repo, &ఫలితం, reflog_సందేశం, dry_నిరంతరం);

బహిర్గతం:
	replay_ఫలితం_release(&ఫలితం);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం commit_became_ఖాళీలు(నిర్మాణం repository *repo,
			       నిర్మాణం commit *original,
			       నిర్మాణం వృక్షం *ఫలితం)
{
	నిర్మాణం object_id మాతృ_వృక్షం_oid;

	ఒకవేళ (మొదటి_మాతృ_వృక్షం_oid(repo, original, &మాతృ_వృక్షం_oid) < 0)
		తిరిగి -1;

	తిరిగి oideq(&ఫలితం->object.oid, &మాతృ_వృక్షం_oid);
}

స్థిర పూర్ణాంకం parse_opt_ఖాళీలు(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	గణన replay_ఖాళీలు_commit_action *విలువ = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!strcmp(arg, "drop"))
		*విలువ = REPLAY_ఖాళీలు_COMMIT_DROP;
	లేకపోతే ఒకవేళ (!strcmp(arg, "keep"))
		*విలువ = REPLAY_ఖాళీలు_COMMIT_KEEP;
	లేకపోతే ఒకవేళ (!strcmp(arg, "abort"))
		*విలువ = REPLAY_ఖాళీలు_COMMIT_ABORT;
	లేకపోతే
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_history_fixup(పూర్ణాంకం argc,
			     స్థిరరాశి అక్షరం **argv,
			     స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ఖాళీ,
	};
	గణన replay_ఖాళీలు_commit_action ఖాళీలు = REPLAY_ఖాళీలు_COMMIT_DROP;
	గణన ref_action action = REF_ACTION_DEFAULT;
	గణన commit_వృక్షం_flags flags = 0;
	పూర్ణాంకం dry_నిరంతరం = 0;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_నిరంతరం,
			 N_("perform a dry-run without updating any refs")),
		OPT_బిట్టు(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_వృక్షం_EDIT_సందేశం),
		OPT_CALLBACK_F(0, "empty", &ఖాళీలు, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ఖాళీలు),
		OPT_చివర(),
	};
	నిర్మాణం విలీనం_ఫలితం విలీనం_ఫలితం = { 0 };
	నిర్మాణం విలీనం_options విలీనం_opts = { 0 };
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం commit *తల_commit, *original, *rewritten;
	నిర్మాణం వృక్షం *తల_వృక్షం, *original_వృక్షం, *సూచీ_వృక్షం;
	నిర్మాణం rev_info revs = { 0 };
	bool skip_commit = false;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	ఒకవేళ (argc != 1) {
		ret = error(_("command expects a single revision"));
		వెళ్ళు బహిర్గతం;
	}
	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	ఒకవేళ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		వెళ్ళు బహిర్గతం;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	ఒకవేళ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	తల_commit = lookup_commit_reference_by_name("HEAD");
	ఒకవేళ (!తల_commit) {
		ret = error(_("cannot look up HEAD"));
		వెళ్ళు బహిర్గతం;
	}

	తల_వృక్షం = repo_get_commit_వృక్షం(repo, తల_commit);
	ఒకవేళ (!తల_వృక్షం) {
		ret = error(_("cannot get tree for HEAD"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (repo_read_సూచీ(repo) < 0) {
		ret = error(_("unable to read index"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!repo_సూచీ_has_changes(repo, తల_వృక్షం, ఖాళీ)) {
		ret = error(_("nothing to fixup: no staged changes"));
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	సూచీ_వృక్షం = write_లోపల_core_సూచీ_as_వృక్షం(repo, repo->సూచీ);
	ఒకవేళ (!సూచీ_వృక్షం) {
		ret = error(_("unable to write index as a tree"));
		వెళ్ళు బహిర్గతం;
	}

	original_వృక్షం = repo_get_commit_వృక్షం(repo, original);
	ఒకవేళ (!original_వృక్షం) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_విలీనం_options(&విలీనం_opts, repo);
	విలీనం_opts.ancestor = "HEAD";
	విలీనం_opts.branch1 = argv[0];
	విలీనం_opts.branch2 = "staged";
	విలీనం_incore_nonrecursive(&విలీనం_opts, తల_వృక్షం,
				  original_వృక్షం, సూచీ_వృక్షం, &విలీనం_ఫలితం);

	ఒకవేళ (విలీనం_ఫలితం.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!విలీనం_ఫలితం.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		వెళ్ళు బహిర్గతం;
	}

	ret = commit_became_ఖాళీలు(repo, original, విలీనం_ఫలితం.వృక్షం);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;
	ఒకవేళ (ret > 0) {
		ఎంపిక (ఖాళీలు) {
		స్థితి REPLAY_ఖాళీలు_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->అంశం : ఖాళీ;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			ఒకవేళ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				వెళ్ళు బహిర్గతం;
			}

			skip_commit = true;
			ఆపు;
		స్థితి REPLAY_ఖాళీలు_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			ఆపు;
		స్థితి REPLAY_ఖాళీలు_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			వెళ్ళు బహిర్గతం;
		}
	}

	ret = అమరిక_revwalk(repo, action, original, &revs);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!skip_commit) {
		ret = commit_వృక్షం_ext(repo, "fixup", original, ఖాళీ, original->parents,
				      &original_వృక్షం->object.oid, &విలీనం_ఫలితం.వృక్షం->object.oid,
				      &rewritten, flags);
		ఒకవేళ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			వెళ్ళు బహిర్గతం;
		}
	}

	strbuf_addf(&reflog_సందేశం, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_సందేశం.buf, dry_నిరంతరం, ఖాళీలు);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	విలీనం_finalize(&విలీనం_opts, &విలీనం_ఫలితం);
	strbuf_release(&reflog_సందేశం);
	release_revisions(&revs);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_history_reword(పూర్ణాంకం argc,
			      స్థిరరాశి అక్షరం **argv,
			      స్థిరరాశి అక్షరం *ఉపసర్గ,
			      నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ఖాళీ,
	};
	గణన ref_action action = REF_ACTION_DEFAULT;
	పూర్ణాంకం dry_నిరంతరం = 0;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_నిరంతరం,
			 N_("perform a dry-run without updating any refs")),
		OPT_చివర(),
	};
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం commit *original, *rewritten;
	నిర్మాణం rev_info revs = { 0 };
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	ఒకవేళ (argc != 1) {
		ret = error(_("command expects a single revision"));
		వెళ్ళు బహిర్గతం;
	}
	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ఒకవేళ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		వెళ్ళు బహిర్గతం;
	}

	ret = అమరిక_revwalk(repo, action, original, &revs);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ret = commit_వృక్షం_with_edited_సందేశం(repo, "reworded", original, &rewritten);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		వెళ్ళు బహిర్గతం;
	}

	strbuf_addf(&reflog_సందేశం, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_సందేశం.buf, dry_నిరంతరం, REPLAY_ఖాళీలు_COMMIT_ABORT);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	strbuf_release(&reflog_సందేశం);
	release_revisions(&revs);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం write_ondisk_సూచీ(నిర్మాణం repository *repo,
			      నిర్మాణం object_id *oid,
			      స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం unpack_trees_options opts = { 0 };
	నిర్మాణం తాళం_file తాళం = తాళం_INIT;
	నిర్మాణం వృక్షం_desc వృక్షం_desc;
	నిర్మాణం సూచీ_state సూచీ;
	నిర్మాణం వృక్షం *వృక్షం;
	పూర్ణాంకం ret;

	సూచీ_state_init(&సూచీ, repo);

	opts.తల_idx = -1;
	opts.src_సూచీ = &సూచీ;
	opts.dst_సూచీ = &సూచీ;

	వృక్షం = repo_parse_వృక్షం_పరోక్ష(repo, oid);
	ఒకవేళ (!వృక్షం) {
		ret = error(_("unable to parse tree %s"), oid_గమ్యం_hex(oid));
		వెళ్ళు బహిర్గతం;
	}
	init_వృక్షం_desc(&వృక్షం_desc, &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);

	ఒకవేళ (unpack_trees(1, &వృక్షం_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		వెళ్ళు బహిర్గతం;
	}

	ప్రధానసంఖ్య_cache_వృక్షం(repo, &సూచీ, వృక్షం);

	ఒకవేళ (repo_hold_తాళం_file_for_update(repo, &తాళం, మార్గం, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (write_locked_సూచీ(&సూచీ, &తాళం, COMMIT_తాళం)) {
		ret = error(_("unable to write new index file"));
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	rollback_తాళం_file(&తాళం);
	release_సూచీ(&సూచీ);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం విభజించు_commit(నిర్మాణం repository *repo,
			నిర్మాణం commit *original,
			నిర్మాణం pathspec *pathspec,
			నిర్మాణం commit **బహిర్గతం)
{
	నిర్మాణం interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	నిర్మాణం strbuf సూచీ_file = STRBUF_INIT;
	నిర్మాణం సూచీ_state సూచీ = సూచీ_STATE_INIT(repo);
	స్థిరరాశి నిర్మాణం object_id *original_commit_వృక్షం_oid;
	స్థిరరాశి నిర్మాణం object_id *old_వృక్షం_oid, *new_వృక్షం_oid;
	నిర్మాణం object_id మాతృ_వృక్షం_oid;
	అక్షరం original_commit_oid[GIT_MAX_HEXSZ + 1];
	నిర్మాణం commit *మొదటి_commit, *రెండవ_commit;
	నిర్మాణం commit_list *parents = ఖాళీ;
	నిర్మాణం వృక్షం *విభజించు_వృక్షం;
	పూర్ణాంకం ret;

	ఒకవేళ (మొదటి_మాతృ_వృక్షం_oid(repo, original, &మాతృ_వృక్షం_oid) < 0) {
		ret = -1;
		వెళ్ళు బహిర్గతం;
	}
	original_commit_వృక్షం_oid = get_commit_వృక్షం_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_మార్గం_replace(repo, &సూచీ_file, "%s", "history-split.index");

	ret = write_ondisk_సూచీ(repo, &మాతృ_వృక్షం_oid, సూచీ_file.buf);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	ret = read_సూచీ_from(&సూచీ, సూచీ_file.buf, repo->gitdir);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		వెళ్ళు బహిర్గతం;
	}

	oid_గమ్యం_hex_r(original_commit_oid, &original->object.oid);
	ret = నిరంతరం_కూడు_p_సూచీ(repo, &సూచీ, సూచీ_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, కూడు_P_DISALLOW_EDIT);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	విభజించు_వృక్షం = write_లోపల_core_సూచీ_as_వృక్షం(repo, &సూచీ);
	ఒకవేళ (!విభజించు_వృక్షం) {
		ret = error(_("failed split tree"));
		వెళ్ళు బహిర్గతం;
	}

	unlink(సూచీ_file.buf);
	strbuf_release(&సూచీ_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	ఒకవేళ (oideq(&విభజించు_వృక్షం->object.oid, &మాతృ_వృక్షం_oid)) {
		ret = error(_("split commit is empty"));
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (oideq(&విభజించు_వృక్షం->object.oid, original_commit_వృక్షం_oid)) {
		ret = error(_("split commit tree matches original commit"));
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_వృక్షం_ext(repo, "split-out", original, ఖాళీ, original->parents, &మాతృ_వృక్షం_oid,
			      &విభజించు_వృక్షం->object.oid, &మొదటి_commit, COMMIT_వృక్షం_EDIT_సందేశం);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed writing first commit"));
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(మొదటి_commit, &parents);

	old_వృక్షం_oid = &repo_get_commit_వృక్షం(repo, మొదటి_commit)->object.oid;
	new_వృక్షం_oid = &repo_get_commit_వృక్షం(repo, original)->object.oid;

	ret = commit_వృక్షం_ext(repo, "split-out", original, ఖాళీ, parents, old_వృక్షం_oid,
			      new_వృక్షం_oid, &రెండవ_commit, COMMIT_వృక్షం_EDIT_సందేశం);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed writing second commit"));
		వెళ్ళు బహిర్గతం;
	}

	*బహిర్గతం = రెండవ_commit;
	ret = 0;

బహిర్గతం:
	ఒకవేళ (సూచీ_file.len)
		unlink(సూచీ_file.buf);
	strbuf_release(&సూచీ_file);
	commit_list_free(parents);
	release_సూచీ(&సూచీ);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_history_విభజించు(పూర్ణాంకం argc,
			     స్థిరరాశి అక్షరం **argv,
			     స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		GIT_HISTORY_విభజించు_USAGE,
		ఖాళీ,
	};
	గణన ref_action action = REF_ACTION_DEFAULT;
	పూర్ణాంకం dry_నిరంతరం = 0;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_నిరంతరం,
			 N_("perform a dry-run without updating any refs")),
		OPT_చివర(),
	};
	నిర్మాణం commit *original, *rewritten = ఖాళీ;
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం pathspec pathspec = { 0 };
	నిర్మాణం rev_info revs = { 0 };
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	ఒకవేళ (argc < 1) {
		ret = error(_("command expects a committish"));
		వెళ్ళు బహిర్గతం;
	}
	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_నిండిన |
		       PATHSPEC_SYMLINK_LEADING_మార్గం |
		       PATHSPEC_ఉపసర్గ_ORIGIN,
		       ఉపసర్గ, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	ఒకవేళ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		వెళ్ళు బహిర్గతం;
	}

	ret = అమరిక_revwalk(repo, action, original, &revs);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (original->parents && original->parents->తదుపరి) {
		ret = error(_("cannot split up merge commit"));
		వెళ్ళు బహిర్గతం;
	}

	ret = విభజించు_commit(repo, original, &pathspec, &rewritten);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	strbuf_addf(&reflog_సందేశం, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_సందేశం.buf, dry_నిరంతరం, REPLAY_ఖాళీలు_COMMIT_ABORT);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	strbuf_release(&reflog_సందేశం);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	తిరిగి ret;
}

#నిర్వచించు SQUASH_చూసిన (1u << 11)
#నిర్వచించు SQUASH_TIP (1u << 12)
#నిర్వచించు SQUASH_AMEND_లక్ష్యం (1u << 13)

స్థిర bool is_autosquash_subject(స్థిరరాశి అక్షరం *s)
{
	తిరిగి starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

స్థిర bool skip_one_autosquash_ఉపసర్గ(స్థిరరాశి అక్షరం *s, స్థిరరాశి అక్షరం **బహిర్గతం)
{
	ఒకవేళ (skip_ఉపసర్గ(s, "amend!", బహిర్గతం) || skip_ఉపసర్గ(s, "fixup!", బహిర్గతం) ||
	    skip_ఉపసర్గ(s, "squash!", బహిర్గతం)) {
		అయితే (**బహిర్గతం == ' ')
			(*బహిర్గతం)++;
		తిరిగి true;
	}
	తిరిగి false;
}

స్థిర శూన్య truncate_సందేశం_గమ్యం_subject(నిర్మాణం strbuf *సందేశం)
{
	స్థిరరాశి అక్షరం *eos = అక్షరశ్రేణి_శోధన(సందేశం->buf, "\n\n");

	ఒకవేళ (eos)
		strbuf_setlen(సందేశం, eos - సందేశం->buf + 1);
}

నిర్మాణం subject_దత్తాంశం {
	నిర్మాణం strintmap subjects;
	నిర్మాణం strbuf subject;
	నిర్మాణం strbuf squash_సందేశం;
	స్థిరరాశి అక్షరం *సందేశం_2;
	bool edit_సందేశం;
};

#నిర్వచించు SUBJECT_దత్తాంశం_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_సందేశం = STRBUF_INIT,	\
}

స్థిర శూన్య subject_దత్తాంశం_clear(నిర్మాణం subject_దత్తాంశం *దత్తాంశం)
{
	strintmap_clear(&దత్తాంశం->subjects);
	strbuf_release(&దత్తాంశం->subject);
	strbuf_release(&దత్తాంశం->squash_సందేశం);
}

స్థిర పూర్ణాంకం squash_amend_సందేశం(నిర్మాణం repository *repo,
				నిర్మాణం commit *commit,
				నిర్మాణం subject_దత్తాంశం *దత్తాంశం,
				గుర్తులేని flags)
{
	స్థిరరాశి అక్షరం *body = దత్తాంశం->సందేశం_2 + దత్తాంశం->subject.len;

	అయితే (ఖాళీనా(*body))
		body++;

	ఒకవేళ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_కనుగొను_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			దత్తాంశం->subject.buf);
		తిరిగి 0;
	}

	ఒకవేళ (దత్తాంశం->edit_సందేశం) {
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (flags & SQUASH_AMEND_లక్ష్యం) {
		ఒకవేళ (starts_with(దత్తాంశం->squash_సందేశం.buf, "squash!"))
			తిరిగి error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_కనుగొను_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     దత్తాంశం->subject.buf);
		ఒకవేళ (starts_with(దత్తాంశం->squash_సందేశం.buf, "fixup!"))
			strbuf_splice(&దత్తాంశం->squash_సందేశం, 0, 5, "amend!", 5);
		ఒకవేళ (starts_with(దత్తాంశం->squash_సందేశం.buf, "amend!")) {
			truncate_సందేశం_గమ్యం_subject(&దత్తాంశం->squash_సందేశం);
			strbuf_addch(&దత్తాంశం->squash_సందేశం, '\n');
		} లేకపోతే {
			strbuf_reset(&దత్తాంశం->squash_సందేశం);
		}
		strbuf_addstr(&దత్తాంశం->squash_సందేశం, body);
		strbuf_complete_పంక్తి(&దత్తాంశం->squash_సందేశం);
	} లేకపోతే {
		తిరిగి error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_కనుగొను_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     దత్తాంశం->subject.buf);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం squash_squash_సందేశం(నిర్మాణం repository *repo,
				నిర్మాణం commit *commit,
				నిర్మాణం subject_దత్తాంశం *దత్తాంశం,
				గుర్తులేని flags)
{
	స్థిరరాశి అక్షరం *body = దత్తాంశం->సందేశం_2 + దత్తాంశం->subject.len;

	అయితే (ఖాళీనా(*body))
		body++;

	ఒకవేళ (దత్తాంశం->edit_సందేశం) {
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (flags & SQUASH_AMEND_లక్ష్యం) {
		ఒకవేళ (starts_with(దత్తాంశం->squash_సందేశం.buf, "fixup!")) {
			truncate_సందేశం_గమ్యం_subject(&దత్తాంశం->squash_సందేశం);
			strbuf_splice(&దత్తాంశం->squash_సందేశం, 0, 5, "squash", 6);
		}
		ఒకవేళ (starts_with(దత్తాంశం->squash_సందేశం.buf, "squash!")) {
			strbuf_addch(&దత్తాంశం->squash_సందేశం, '\n');
			strbuf_addstr(&దత్తాంశం->squash_సందేశం, body);
			strbuf_complete_పంక్తి(&దత్తాంశం->squash_సందేశం);
		} లేకపోతే {
			తిరిగి error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_కనుగొను_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     దత్తాంశం->subject.buf);
		}
	} లేకపోతే {
		తిరిగి error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_కనుగొను_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      దత్తాంశం->subject.buf);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం squash_check_can_autosquash(నిర్మాణం repository *repo,
				       నిర్మాణం commit *commit,
				       నిర్మాణం subject_దత్తాంశం *దత్తాంశం,
				       గుర్తులేని flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_లక్ష్యం;
	ఒకవేళ (starts_with(దత్తాంశం->subject.buf, "amend!"))
		తిరిగి squash_amend_సందేశం(repo, commit, దత్తాంశం, flags);
	లేకపోతే ఒకవేళ (starts_with(దత్తాంశం->subject.buf, "squash!"))
		తిరిగి squash_squash_సందేశం(repo, commit, దత్తాంశం, flags);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం squash_check_autosquash_subject(నిర్మాణం repository *repo,
					   నిర్మాణం commit *commit,
					   నిర్మాణం subject_దత్తాంశం *దత్తాంశం)
{
	స్థిరరాశి అక్షరం* s = దత్తాంశం->subject.buf;
	నిర్మాణం commit *లక్ష్యం;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం strmap_నమోదు *నమోదు_2;
	/* Try skipping autosquash prefixes one at a time to allow
	 * squashing
	 *     a commit
	 *     fixup! fixup! a commit
	 *
	 * where we may have started with
	 *     a commit
	 *     fixup! a commit
	 *     fixup! fixup! a commit
	 *
	 * and squashed the first fixup separately from the second
	 */
	అయితే (skip_one_autosquash_ఉపసర్గ(s, &s)) {
		గుర్తులేని flags = strintmap_get(&దత్తాంశం->subjects, s);
		ఒకవేళ (flags)
			తిరిగి squash_check_can_autosquash(repo, commit, దత్తాంశం, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	లక్ష్యం = lookup_commit_reference_by_name(s);
	ఒకవేళ (లక్ష్యం && starts_with(oid_గమ్యం_hex(&లక్ష్యం->object.oid), s)) {
		గుర్తులేని flags =
			లక్ష్యం->object.flags & (SQUASH_చూసిన | SQUASH_AMEND_లక్ష్యం);
		ఒకవేళ (!flags) {
			స్థిరరాశి అక్షరం *subject_ప్రారంభం;
			స్థిరరాశి అక్షరం *బఫరు = repo_logmsg_reencode(repo, లక్ష్యం,
								  ఖాళీ, ఖాళీ);
			కొలత_t subject_len = కనుగొను_commit_subject(బఫరు,
								 &subject_ప్రారంభం);
			అక్షరం *subject = xmemdupz(subject_ప్రారంభం, subject_len);

			flags = strintmap_get(&దత్తాంశం->subjects, subject);
			విడుదల(subject);
			repo_unuse_commit_బఫరు(repo, లక్ష్యం, బఫరు);
		}
		ఒకవేళ (flags)
			తిరిగి squash_check_can_autosquash(repo, commit,
							   దత్తాంశం, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_నమోదు(&దత్తాంశం->subjects, &iter, నమోదు_2) {
		s = దత్తాంశం->subject.buf;
		అయితే(skip_one_autosquash_ఉపసర్గ(s, &s)) {
			ఒకవేళ (starts_with(నమోదు_2->కీలకం, s)) {
				గుర్తులేని విలువ = (intptr_t)నమోదు_2->విలువ;

				తిరిగి squash_check_can_autosquash(repo, commit,
								   దత్తాంశం, విలువ);
			}
		}
	}
	తిరిగి error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_కనుగొను_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       దత్తాంశం->subject.buf);
}

స్థిర పూర్ణాంకం squash_check_subject(నిర్మాణం repository *repo,
				నిర్మాణం commit *commit,
				నిర్మాణం subject_దత్తాంశం *దత్తాంశం)
{
	పూర్ణాంకం ret = 0;
	స్థిరరాశి అక్షరం *buf = repo_logmsg_reencode(repo, commit, ఖాళీ, ఖాళీ);
	కొలత_t subject_len = కనుగొను_commit_subject(buf, &దత్తాంశం->సందేశం_2);

	strbuf_reset(&దత్తాంశం->subject);
	strbuf_కూడు(&దత్తాంశం->subject, దత్తాంశం->సందేశం_2, subject_len);

	ఒకవేళ (!strintmap_get_కొలత(&దత్తాంశం->subjects)) {
		స్థిరరాశి అక్షరం *s;

		strbuf_addstr(&దత్తాంశం->squash_సందేశం, దత్తాంశం->సందేశం_2);
		strbuf_complete_పంక్తి(&దత్తాంశం->squash_సందేశం);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = దత్తాంశం->subject.buf;
		skip_one_autosquash_ఉపసర్గ(s, &s);
		strintmap_సమితి(&దత్తాంశం->subjects, s, SQUASH_AMEND_లక్ష్యం | SQUASH_చూసిన);
		commit->object.flags |= SQUASH_AMEND_లక్ష్యం;
	} లేకపోతే ఒకవేళ (is_autosquash_subject(దత్తాంశం->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, దత్తాంశం);
	} లేకపోతే {
		strintmap_సమితి(&దత్తాంశం->subjects, దత్తాంశం->subject.buf, SQUASH_చూసిన);
	}
	repo_unuse_commit_బఫరు(repo, commit, buf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం build_squash_సందేశం(నిర్మాణం repository *repo,
				స్థిరరాశి నిర్మాణం strbuf *todo_buf,
				నిర్మాణం strbuf *బహిర్గతం);

స్థిర పూర్ణాంకం అమరిక_squash_revisions(నిర్మాణం repository *repo,
				  పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				  నిర్మాణం rev_info *revs)
{
	repo_init_revisions(repo, revs, ఖాళీ);
	revs->తిరగేయి = 1;
	revs->topo_క్రమం = 1;
	revs->క్రమీకరించు_క్రమం = REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం;
	revs->simplify_history = 0;
	revs->ancestry_మార్గం = 1;
	revs->limited = 1;
	revs->ancestry_మార్గం_implicit_bottoms = 1;

	argc = అమరిక_revisions(argc, argv, revs, ఖాళీ);
	ఒకవేళ (argc > 1)
		తిరిగి error(_("unrecognized argument: %s"), argv[1]);

	ఒకవేళ (revs->తిరగేయి != 1 || revs->topo_క్రమం != 1 ||
	    revs->క్రమీకరించు_క్రమం != REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_మార్గం != 1 || revs->limited != 1 ||
	    revs->ancestry_మార్గం_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->తిరగేయి = 1;
		revs->topo_క్రమం = 1;
		revs->క్రమీకరించు_క్రమం = REV_క్రమీకరించు_లోపల_లేఖాచిత్రం_క్రమం;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_మార్గం = 1;
		revs->limited = 1;
		revs->ancestry_మార్గం_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	కొరకు (కొలత_t i = 0; i < revs->cmdline.nr; i++)
		ఒకవేళ (revs->cmdline.rev[i].flags & BOTTOM)
			తిరిగి 0;

	తిరిగి error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
స్థిర పూర్ణాంకం resolve_squash_range(నిర్మాణం repository *repo,
				bool update_branches,
				bool edit_సందేశం,
				పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				నిర్మాణం commit **oldest_బహిర్గతం,
				నిర్మాణం commit **tip_బహిర్గతం,
				అక్షరం **సందేశం_బహిర్గతం)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం subject_దత్తాంశం subject_దత్తాంశం = SUBJECT_దత్తాంశం_INIT;
	నిర్మాణం commit *commit, *oldest = ఖాళీ, *tip = ఖాళీ;
	నిర్మాణం strbuf todo_buf = STRBUF_INIT;
	పూర్ణాంకం ret, tip_లెక్క = 0;
	bool walk_started = false;
	నిర్మాణం ref_filter filter = REF_FILTER_INIT;
	నిర్మాణం ref_array refs = { 0 };

	subject_దత్తాంశం.edit_సందేశం = edit_సందేశం;
	ret = అమరిక_squash_revisions(repo, argc, argv, &revs);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		వెళ్ళు బహిర్గతం;
	}
	walk_started = true;
	అయితే ((commit = get_revision(&revs))) {
		నిర్మాణం commit_list *p;

		ఒకవేళ (edit_సందేశం)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_గమ్యం_hex(&commit->object.oid));

		ఒకవేళ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			వెళ్ళు బహిర్గతం;
		}
		కొరకు (p = commit->parents; oldest && p; p = p->తదుపరి) {
			నిర్మాణం commit_list *q;
			నిర్మాణం object *o;
			bool చూసిన;

			ఒకవేళ (repo_parse_commit(repo, p->అంశం)) {
				ret = error(_("cannot parse commit"));
				వెళ్ళు బహిర్గతం;
			}
			o = &p->అంశం->object;
			చూసిన = o->flags & SQUASH_చూసిన;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			కొరకు (q = oldest->parents; !చూసిన && q; q = q->తదుపరి) {
				ఒకవేళ (p->అంశం == q->అంశం) {
					చూసిన = true;
					commit_list_చొప్పించు(commit, &filter.with_commit);
				}
			}
			ఒకవేళ (!చూసిన) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_కనుగొను_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_కనుగొను_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				వెళ్ళు బహిర్గతం;
			}
			ఒకవేళ (o->flags & SQUASH_TIP) {
				tip_లెక్క--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		ఒకవేళ (!oldest) {
			commit_list_చొప్పించు(commit, &filter.with_commit);
			oldest = commit;
		}
		ఒకవేళ (squash_check_subject(repo, commit, &subject_దత్తాంశం)) {
			ret = -1;
			వెళ్ళు బహిర్గతం;
		}
		tip = commit;
		tip->object.flags |= SQUASH_చూసిన | SQUASH_TIP;
		tip_లెక్క++;
	}
	clear_object_flags(repo, SQUASH_చూసిన | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	ఒకవేళ (!tip_లెక్క) {
		ret = error(_("the revision range is empty"));
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (tip_లెక్క != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_చొప్పించు(tip, &filter.no_commit);
	filter.రకం = FILTER_REFS_BRANCHES;
	ఒకవేళ (update_branches &&
	    filter_refs(&refs, &filter, filter.రకం)) {
		ret = error(_("could not filter refs"));
		వెళ్ళు బహిర్గతం;
	}
	ఒకవేళ (refs.nr) {
		నిర్మాణం ref_format format = REF_FORMAT_INIT;
		నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ;
		నిర్మాణం string_list క్రమీకరణ_options = STRING_LIST_INIT_DUP;
		నిర్మాణం strbuf branches = STRBUF_INIT;
		నిర్మాణం strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		ఒకవేళ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&క్రమీకరణ_options, "refname");
		క్రమీకరణ = ref_క్రమీకరణ_options(&క్రమీకరణ_options);
		ref_array_క్రమీకరించు(క్రమీకరణ, &refs);
		కొరకు (పూర్ణాంకం i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			ఒకవేళ (format_ref_array_అంశం(refs.వస్తువులు[i], &format,
						  &branches, &err))
				BUG("could not format branch name: %s", err.buf);
		}
		/*
		 * TODO: also check HEADS from other worktrees.
		 */
		ret = error(_("the following branches cannot be rewritten as "
			      "descendants of the squashed commit:%s"), branches.buf);
		advise_if_enabled(ADVICE_HISTORY_UPDATE_REFS,
				  _("Use --update-refs=head to rewrite only "
				    "the current branch and leave such branches "
				    "untouched."));
		strbuf_release(&err);
		strbuf_release(&branches);
		ref_క్రమీకరణ_release(క్రమీకరణ);
		string_list_clear(&క్రమీకరణ_options, 0);
		వెళ్ళు బహిర్గతం;
	}
	ఒకవేళ (edit_సందేశం) {
		strbuf_reset(&subject_దత్తాంశం.squash_సందేశం);
		ret = build_squash_సందేశం(repo, &todo_buf,
					   &subject_దత్తాంశం.squash_సందేశం);
		ఒకవేళ (ret < 0)
			వెళ్ళు బహిర్గతం;
	}

	*oldest_బహిర్గతం = oldest;
	*tip_బహిర్గతం = tip;
	*సందేశం_బహిర్గతం = strbuf_detach(&subject_దత్తాంశం.squash_సందేశం, ఖాళీ);
	ret = 0;

బహిర్గతం:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_చూసిన | SQUASH_TIP |
			   SQUASH_AMEND_లక్ష్యం);
	ఒకవేళ (walk_started)
		reset_revision_walk();
	subject_దత్తాంశం_clear(&subject_దత్తాంశం);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	తిరిగి ret;
}

స్థిర bool amend_replaces_లక్ష్యం(నిర్మాణం todo_list *todo, పూర్ణాంకం లక్ష్యం)
{
	కొరకు (పూర్ణాంకం i = లక్ష్యం + 1; i < todo->nr &&
				 todo->వస్తువులు[i].command != TODO_PICK; i++) {
		ఒకవేళ (todo->వస్తువులు[i].command == TODO_SQUASH)
			తిరిగి false;
		ఒకవేళ (todo->వస్తువులు[i].flags & TODO_REPLACE_FIXUP_సందేశం)
			తిరిగి true;
	}
	తిరిగి false;
}

స్థిర పూర్ణాంకం build_squash_సందేశం(నిర్మాణం repository *repo,
				స్థిరరాశి నిర్మాణం strbuf *todo_buf,
				నిర్మాణం strbuf *బహిర్గతం)
{
	నిర్మాణం todo_list todo = TODO_LIST_INIT;
	నిర్మాణం replay_opts opts = REPLAY_OPTS_INIT;
	పూర్ణాంకం nr_commits, ret;

	ఒకవేళ (todo_list_parse_insn_బఫరు(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		వెళ్ళు బహిర్గతం;
	}

	nr_commits = todo.nr;
	కొరకు (పూర్ణాంకం i = 0; i < nr_commits; i++) {
		నిర్మాణం todo_అంశం *అంశం = &todo.వస్తువులు[i];
		స్థిరరాశి అక్షరం *సందేశం_2, *body;
		కొలత_t commented_len;
		bool skip, squashing;

		squashing = అంశం->command == TODO_SQUASH ||
			    (అంశం->flags & TODO_REPLACE_FIXUP_సందేశం);
		ఒకవేళ (అంశం->command == TODO_PICK)
			skip = amend_replaces_లక్ష్యం(&todo, i);
		లేకపోతే
			skip = !squashing;

		సందేశం_2 = repo_logmsg_reencode(repo, అంశం->commit, ఖాళీ, ఖాళీ);
		కనుగొను_commit_subject(సందేశం_2, &body);

		ఒకవేళ (skip)
			commented_len = అక్షరశ్రేణి_పొడవు(body);
		లేకపోతే ఒకవేళ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		లేకపోతే
			commented_len = 0;

		ఒకవేళ (!i)
			కూడు_squash_combination_శీర్షిక(బహిర్గతం, nr_commits);
		strbuf_addch(బహిర్గతం, '\n');
		కూడు_squash_సందేశం_శీర్షిక(బహిర్గతం, i + 1, skip);
		strbuf_addstr(బహిర్గతం, "\n\n");
		strbuf_కూడు_commented_పంక్తులు(బహిర్గతం, body, commented_len, comment_పంక్తి_str);
		strbuf_addstr(బహిర్గతం, body + commented_len);
		strbuf_complete_పంక్తి(బహిర్గతం);

		repo_unuse_commit_బఫరు(repo, అంశం->commit, సందేశం_2);
	}

	ret = 0;

బహిర్గతం:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_history_squash(పూర్ణాంకం argc,
			      స్థిరరాశి అక్షరం **argv,
			      స్థిరరాశి అక్షరం *ఉపసర్గ,
			      నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ఖాళీ,
	};
	గణన ref_action action = REF_ACTION_DEFAULT;
	పూర్ణాంకం dry_నిరంతరం = 0;
	పూర్ణాంకం edit = 1;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_నిరంతరం,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_చివర(),
	};
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం commit *oldest, *tip, *rewritten;
	స్థిరరాశి నిర్మాణం object_id *పునాది_వృక్షం_oid, *tip_వృక్షం_oid;
	అక్షరం *సందేశం_template = ఖాళీ;
	నిర్మాణం rev_info revs = { 0 };
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	ఒకవేళ (argc < 2) {
		ret = error(_("command expects a revision range"));
		వెళ్ళు బహిర్గతం;
	}
	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_సందేశం, "squash: updating ");
	strbuf_join_argv(&reflog_సందేశం, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &సందేశం_template);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	ret = అమరిక_revwalk(repo, action, tip, &revs);
	ఒకవేళ (ret < 0)
		వెళ్ళు బహిర్గతం;

	పునాది_వృక్షం_oid = &repo_get_commit_వృక్షం(repo,
					oldest->parents->అంశం)->object.oid;
	tip_వృక్షం_oid = &repo_get_commit_వృక్షం(repo, tip)->object.oid;

	ret = commit_వృక్షం_ext(repo, "squash", oldest, సందేశం_template,
			      oldest->parents, పునాది_వృక్షం_oid, tip_వృక్షం_oid,
			      &rewritten,
			      edit ? COMMIT_వృక్షం_EDIT_సందేశం : 0);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		వెళ్ళు బహిర్గతం;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_సందేశం.buf, dry_నిరంతరం,
				       REPLAY_ఖాళీలు_COMMIT_ABORT);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	strbuf_release(&reflog_సందేశం);
	release_revisions(&revs);
	విడుదల(సందేశం_template);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం update_worktree(నిర్మాణం repository *repo,
			   స్థిరరాశి నిర్మాణం commit *old_తల,
			   స్థిరరాశి నిర్మాణం commit *new_తల,
			   bool dry_నిరంతరం)
{
	నిర్మాణం reset_working_వృక్షం_options opts = {
		.oid_from = &old_తల->object.oid,
		.oid = &new_తల->object.oid,
	};
	ఒకవేళ (dry_నిరంతరం)
		opts.flags |= RESET_WORKING_వృక్షం_DRY_నిరంతరం;
	తిరిగి reset_working_వృక్షం(repo, &opts);
}

స్థిర పూర్ణాంకం కనుగొను_తల_వృక్షం_change(నిర్మాణం repository *repo,
				 స్థిరరాశి నిర్మాణం replay_ఫలితం *ఫలితం,
				 నిర్మాణం commit **old_తల,
				 నిర్మాణం commit **new_తల,
				 bool *మారింది)
{
	స్థిరరాశి నిర్మాణం replay_ref_update *తల_update = ఖాళీ;
	నిర్మాణం commit *old_తల_commit, *new_తల_commit;
	నిర్మాణం వృక్షం *old_తల_వృక్షం, *new_తల_వృక్షం;
	స్థిరరాశి అక్షరం *తల_లక్ష్యం;
	పూర్ణాంకం తల_flags;

	*మారింది = false;

	తల_లక్ష్యం = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ఖాళీ, &తల_flags);
	ఒకవేళ (!తల_లక్ష్యం)
		తిరిగి error(_("cannot look up HEAD"));

	కొరకు (కొలత_t i = 0; i < ఫలితం->updates_nr; i++) {
		ఒకవేళ (!strcmp(ఫలితం->updates[i].refname, తల_లక్ష్యం)) {
			తల_update = &ఫలితం->updates[i];
			ఆపు;
		}
	}

	ఒకవేళ (!తల_update)
		తిరిగి 0;

	old_తల_commit = lookup_commit_reference(repo, &తల_update->old_oid);
	new_తల_commit = lookup_commit_reference(repo, &తల_update->new_oid);
	ఒకవేళ (!old_తల_commit || !new_తల_commit)
		తిరిగి error(_("cannot resolve HEAD commit"));

	old_తల_వృక్షం = repo_get_commit_వృక్షం(repo, old_తల_commit);
	new_తల_వృక్షం = repo_get_commit_వృక్షం(repo, new_తల_commit);
	ఒకవేళ (!old_తల_వృక్షం || !new_తల_వృక్షం)
		తిరిగి error(_("cannot resolve tree for HEAD"));

	ఒకవేళ (oideq(&old_తల_వృక్షం->object.oid, &new_తల_వృక్షం->object.oid))
		తిరిగి 0;

	*old_తల = old_తల_commit;
	*new_తల = new_తల_commit;
	*మారింది = true;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_history_drop(పూర్ణాంకం argc,
			    స్థిరరాశి అక్షరం **argv,
			    స్థిరరాశి అక్షరం *ఉపసర్గ,
			    నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ఖాళీ,
	};
	గణన replay_ఖాళీలు_commit_action ఖాళీలు = REPLAY_ఖాళీలు_COMMIT_DROP;
	గణన ref_action action = REF_ACTION_DEFAULT;
	పూర్ణాంకం dry_నిరంతరం = 0;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_నిరంతరం,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &ఖాళీలు, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ఖాళీలు),
		OPT_చివర(),
	};
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం commit *original, *rewritten;
	నిర్మాణం rev_info revs = { 0 };
	నిర్మాణం replay_ఫలితం ఫలితం = { 0 };
	నిర్మాణం commit *old_తల, *new_తల;
	bool తల_కదలికలు = false;
	పూర్ణాంకం ret;

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	ఒకవేళ (argc != 1) {
		ret = error(_("command expects a single revision"));
		వెళ్ళు బహిర్గతం;
	}
	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ఒకవేళ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (original->parents->తదుపరి) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		వెళ్ళు బహిర్గతం;
	}

	ret = అమరిక_revwalk(repo, action, original, &revs);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	rewritten = original->parents->అంశం;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  ఖాళీలు, &ఫలితం);
	ఒకవేళ (ret) {
		ret = error(_("failed replaying descendants"));
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * If HEAD will move as a result of the rewrite then we'll have to
	 * merge in the changes into the worktree and index. This merge can of
	 * course conflict, which will cause the whole operation to abort.
	 *
	 * If we had already updated the refs at that point then we'd have an
	 * inconsistent repository state. So we first perform a dry-run merge
	 * here before updating refs.
	 */
	ఒకవేళ (!is_bare_repository(repo)) {
		ret = కనుగొను_తల_వృక్షం_change(repo, &ఫలితం, &old_తల,
					    &new_తల, &తల_కదలికలు);
		ఒకవేళ (ret < 0)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (తల_కదలికలు && update_worktree(repo, old_తల, new_తల, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			వెళ్ళు బహిర్గతం;
		}
	}

	strbuf_addf(&reflog_సందేశం, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &ఫలితం, reflog_సందేశం.buf, dry_నిరంతరం);
	ఒకవేళ (ret < 0) {
		ret = error(_("failed to update references"));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!dry_నిరంతరం && తల_కదలికలు && update_worktree(repo, old_తల, new_తల, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_గమ్యం_hex(&new_తల->object.oid));
		వెళ్ళు బహిర్గతం;
	}

	ret = 0;

బహిర్గతం:
	replay_ఫలితం_release(&ఫలితం);
	strbuf_release(&reflog_సందేశం);
	release_revisions(&revs);
	తిరిగి ret;
}

పూర్ణాంకం cmd_history(పూర్ణాంకం argc,
		స్థిరరాశి అక్షరం **argv,
		స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_విభజించు_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ఖాళీ,
	};
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_విభజించు),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);
	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
