#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "merge-ort.h"
#ಅನ್ತರ್ಭಾವ "oidmap.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "ref-filter.h"
#ಅನ್ತರ್ಭಾವ "replay.h"
#ಅನ್ತರ್ಭಾವ "reset.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "wt-status.h"

#ವ್ಯಾಖ್ಯಾನಿಸು GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#ವ್ಯಾಖ್ಯಾನಿಸು GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#ವ್ಯಾಖ್ಯಾನಿಸು GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#ವ್ಯಾಖ್ಯಾನಿಸು GIT_HISTORY_ವಿಭಜಿಸು_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ಸ್ಥಿರ ಶೂನ್ಯ change_ದತ್ತಾಂಶ_free(ಶೂನ್ಯ *util, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str ಬಳಸದ)
{
	ರಚನೆ wt_ಸ್ಥಿತಿ_change_ದತ್ತಾಂಶ *d = util;
	ಬಿಡುಗಡೆ(d->rename_ಮೂಲ);
	ಬಿಡುಗಡೆ(d);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fill_commit_ಸಂದೇಶ(ರಚನೆ repository *repo,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_ವೃಕ್ಷ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_ವೃಕ್ಷ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *default_ಸಂದೇಶ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action,
			       ರಚನೆ strbuf *ಹೊರಹೋಗು)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = git_ಮಾರ್ಗ_commit_editmsg();
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ರಚನೆ wt_ಸ್ಥಿತಿ s;

	wt_ಸ್ಥಿತಿ_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_ಶೀಘ್ರ;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_ಪೂರ್ವಪ್ರತ್ಯಯ = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(ಮಾರ್ಗ, "w");
	ಆದರೆ (!s.fp)
		ಹಿಂತಿರುಗಿ error_errno(_("could not open '%s'"), ಮಾರ್ಗ);

	strbuf_addstr(ಹೊರಹೋಗು, default_ಸಂದೇಶ);
	strbuf_addch(ಹೊರಹೋಗು, '\n');
	strbuf_commented_addf(ಹೊರಹೋಗು, comment_ಸಾಲು_str, hint, action, comment_ಸಾಲು_str);
	ಆದರೆ (fwrite(ಹೊರಹೋಗು->buf, 1, ಹೊರಹೋಗು->len, s.fp) != ಹೊರಹೋಗು->len)
		die_errno(_("could not write to '%s'"), ಮಾರ್ಗ);

	wt_ಸ್ಥಿತಿ_collect_changes_trees(&s, old_ವೃಕ್ಷ, new_ವೃಕ್ಷ);
	wt_ಸ್ಥಿತಿ_print(&s);
	wt_ಸ್ಥಿತಿ_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_ದತ್ತಾಂಶ_free);
	ಆದರೆ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), ಮಾರ್ಗ);

	strbuf_reset(ಹೊರಹೋಗು);
	ಆದರೆ (launch_editor(ಮಾರ್ಗ, ಹೊರಹೋಗು, ಖಾಲಿ)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		ಹಿಂತಿರುಗಿ -1;
	}
	strbuf_stripspace(ಹೊರಹೋಗು, comment_ಸಾಲು_str);

	cleanup_ಸಂದೇಶ(ಹೊರಹೋಗು, COMMIT_ಸಂದೇಶ_CLEANUP_ALL, 0);

	ಆದರೆ (!ಹೊರಹೋಗು->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಎಣಿಕೆ commit_ವೃಕ್ಷ_flags {
	COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ = (1 << 0),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_ವೃಕ್ಷ_ext(ರಚನೆ repository *repo,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action,
			   ರಚನೆ commit *commit_with_ಸಂದೇಶ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_template,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ commit_list *parents,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_ವೃಕ್ಷ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_ವೃಕ್ಷ,
			   ರಚನೆ commit **ಹೊರಹೋಗು,
			   ಎಣಿಕೆ commit_ವೃಕ್ಷ_flags flags)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ಖಾಲಿ,
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *original_ಸಂದೇಶ, *original_body, *ptr;
	ರಚನೆ commit_extra_ಶೀರ್ಷಿಕೆ *original_extra_ಶೀರ್ಷಿಕೆಗಳು = ಖಾಲಿ;
	ರಚನೆ strbuf commit_ಸಂದೇಶ_2 = STRBUF_INIT;
	ರಚನೆ object_id rewritten_commit_oid;
	ಅಕ್ಷರ *original_author = ಖಾಲಿ;
	ಗಾತ್ರ_t len;
	ಪೂರ್ಣಾಂಕ ret;

	/* We retain authorship of the original commit. */
	original_ಸಂದೇಶ = repo_logmsg_reencode(repo, commit_with_ಸಂದೇಶ, ಖಾಲಿ, ಖಾಲಿ);
	ptr = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(original_ಸಂದೇಶ, "author", &len);
	ಆದರೆ (ptr)
		original_author = xmemdupz(ptr, len);
	ಕಂಡುಹಿಡಿ_commit_subject(original_ಸಂದೇಶ, &original_body);

	ಆದರೆ (!ಸಂದೇಶ_template)
		ಸಂದೇಶ_template = original_body;

	ಆದರೆ (flags & COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ) {
		ret = fill_commit_ಸಂದೇಶ(repo, old_ವೃಕ್ಷ, new_ವೃಕ್ಷ,
					  ಸಂದೇಶ_template, action, &commit_ಸಂದೇಶ_2);
		ಆದರೆ (ret < 0)
			ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(&commit_ಸಂದೇಶ_2, ಸಂದೇಶ_template);
	}

	original_extra_ಶೀರ್ಷಿಕೆಗಳು = read_commit_extra_ಶೀರ್ಷಿಕೆಗಳು(commit_with_ಸಂದೇಶ,
							   exclude_gpgsig);

	ret = commit_ವೃಕ್ಷ_extended(commit_ಸಂದೇಶ_2.buf, commit_ಸಂದೇಶ_2.len, new_ವೃಕ್ಷ,
				   parents, &rewritten_commit_oid, original_author,
				   ಖಾಲಿ, ಖಾಲಿ, original_extra_ಶೀರ್ಷಿಕೆಗಳು);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	*ಹೊರಹೋಗು = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

ಹೊರಹೋಗು:
	free_commit_extra_ಶೀರ್ಷಿಕೆಗಳು(original_extra_ಶೀರ್ಷಿಕೆಗಳು);
	strbuf_release(&commit_ಸಂದೇಶ_2);
	ಬಿಡುಗಡೆ(original_author);
	repo_unuse_commit_ಬಫರ್(repo, commit_with_ಸಂದೇಶ, original_ಸಂದೇಶ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಪೋಷಕ_ವೃಕ್ಷ_oid(ರಚನೆ repository *repo,
				 ರಚನೆ commit *commit,
				 ರಚನೆ object_id *ಹೊರಹೋಗು)
{
	ರಚನೆ commit *ಪೋಷಕ = commit->parents ? commit->parents->ಅಂಶ : ಖಾಲಿ;

	ಆದರೆ (!ಪೋಷಕ) {
		oidcpy(ಹೊರಹೋಗು, repo->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (repo_parse_commit(repo, ಪೋಷಕ))
		ಹಿಂತಿರುಗಿ error(_("unable to parse parent commit %s"),
			     oid_ಗಮ್ಯ_hex(&ಪೋಷಕ->object.oid));

	oidcpy(ಹೊರಹೋಗು, &repo_get_commit_ವೃಕ್ಷ(repo, ಪೋಷಕ)->object.oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_ವೃಕ್ಷ_with_edited_ಸಂದೇಶ(ರಚನೆ repository *repo,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action,
					   ರಚನೆ commit *original,
					   ರಚನೆ commit **ಹೊರಹೋಗು)
{
	ರಚನೆ object_id ಪೋಷಕ_ವೃಕ್ಷ_oid;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ವೃಕ್ಷ_oid;

	ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, original)->object.oid;

	ಆದರೆ (ಮೊದಲ_ಪೋಷಕ_ವೃಕ್ಷ_oid(repo, original, &ಪೋಷಕ_ವೃಕ್ಷ_oid) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ commit_ವೃಕ್ಷ_ext(repo, action, original, ಖಾಲಿ, original->parents,
			       &ಪೋಷಕ_ವೃಕ್ಷ_oid, ವೃಕ್ಷ_oid, ಹೊರಹೋಗು, COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ);
}

ಎಣಿಕೆ ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_ತಲೆ,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ref_action(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ ref_action *action = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, ಮೌಲ್ಯ);
	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "head")) {
		*action = REF_ACTION_ತಲೆ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ revwalk_ಒಳಗೊಂಡಿದೆ_merges(ರಚನೆ repository *repo,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ strvec *revwalk_ವಾದಗಳು)
{
	ರಚನೆ strvec ವಾದಗಳು = STRVEC_INIT;
	ರಚನೆ rev_info revs;
	ಪೂರ್ಣಾಂಕ ret;

	strvec_pushv(&ವಾದಗಳು, revwalk_ವಾದಗಳು->v);
	strvec_ತಳ್ಳು(&ವಾದಗಳು, "--min-parents=2");

	repo_init_revisions(repo, &revs, ಖಾಲಿ);

	ಸಿದ್ಧತೆ_revisions_from_strvec(&ವಾದಗಳು, &revs, ಖಾಲಿ);
	ಆದರೆ (ವಾದಗಳು.nr != 1)
		BUG("revisions were set up with invalid argument");

	ಆದರೆ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	reset_revision_walk();
	ret = 0;

ಹೊರಹೋಗು:
	release_revisions(&revs);
	strvec_clear(&ವಾದಗಳು);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಿದ್ಧತೆ_revwalk(ರಚನೆ repository *repo,
			 ಎಣಿಕೆ ref_action action,
			 ರಚನೆ commit *original,
			 ರಚನೆ rev_info *revs)
{
	ರಚನೆ strvec ವಾದಗಳು = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	repo_init_revisions(repo, revs, ಖಾಲಿ);
	strvec_ತಳ್ಳು(&ವಾದಗಳು, "ignored");
	strvec_ತಳ್ಳು(&ವಾದಗಳು, "--reverse");
	strvec_ತಳ್ಳು(&ವಾದಗಳು, "--topo-order");
	strvec_ತಳ್ಳು(&ವಾದಗಳು, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&ವಾದಗಳು, "--ancestry-path=%s",
		     oid_ಗಮ್ಯ_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&ವಾದಗಳು, "^%s", oid_ಗಮ್ಯ_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	ಆದರೆ (action == REF_ACTION_ತಲೆ) {
		ರಚನೆ commit_list *from_list = ಖಾಲಿ;
		ರಚನೆ commit *ತಲೆ;

		ತಲೆ = lookup_commit_reference_by_name("HEAD");
		ಆದರೆ (!ತಲೆ) {
			ret = error(_("cannot look up HEAD"));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		commit_list_ಸೇರಿಸು(original, &from_list);
		ret = repo_is_descendant_ದ(repo, ತಲೆ, from_list);
		commit_list_free(from_list);

		ಆದರೆ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			ಹೋಗು ಹೊರಹೋಗು;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			ಹೋಗು ಹೊರಹೋಗು;
		}

		strvec_ತಳ್ಳು(&ವಾದಗಳು, "HEAD");
	} ಇಲ್ಲದಿದ್ದರೆ {
		strvec_ತಳ್ಳು(&ವಾದಗಳು, "--branches");
		strvec_ತಳ್ಳು(&ವಾದಗಳು, "HEAD");
	}

	ret = revwalk_ಒಳಗೊಂಡಿದೆ_merges(repo, &ವಾದಗಳು);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ಸಿದ್ಧತೆ_revisions_from_strvec(&ವಾದಗಳು, revs, ಖಾಲಿ);
	ಆದರೆ (ವಾದಗಳು.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

ಹೊರಹೋಗು:
	strvec_clear(&ವಾದಗಳು);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_ref_update(ರಚನೆ ref_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reflog_ಸಂದೇಶ,
			     ರಚನೆ strbuf *err)
{
	ಆದರೆ (!transaction) {
		ಬರೆ("update %s %s %s\n",
		       refname, oid_ಗಮ್ಯ_hex(new_oid), oid_ಗಮ್ಯ_hex(old_oid));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ಖಾಲಿ, ಖಾಲಿ, 0, reflog_ಸಂದೇಶ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compute_pending_ref_updates(ರಚನೆ rev_info *revs,
				       ಎಣಿಕೆ ref_action action,
				       ರಚನೆ commit *original,
				       ರಚನೆ commit *rewritten,
				       ಎಣಿಕೆ replay_ಖಾಲಿಗಳು_commit_action ಖಾಲಿಗಳು,
				       ರಚನೆ replay_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ name_decoration *decoration;
	ರಚನೆ replay_revisions_options opts = {
		.ಖಾಲಿಗಳು = ಖಾಲಿಗಳು,
	};
	ಅಕ್ಷರ hex[GIT_MAX_HEXSZ + 1];
	bool detached_ತಲೆ;
	ಪೂರ್ಣಾಂಕ ತಲೆ_flags = 0;
	ಪೂರ್ಣಾಂಕ ret;

	refs_read_ref_ತುಂಬಿದ(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ಖಾಲಿ, &ತಲೆ_flags);
	detached_ತಲೆ = !(ತಲೆ_flags & REF_ISSYMREF);

	opts.onto = oid_ಗಮ್ಯ_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, ಫಲಿತಾಂಶ);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (action != REF_ACTION_BRANCHES && action != REF_ACTION_ತಲೆ)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	ಗಾಗಿ (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->ಮುಂದಿನ)
	{
		ಆದರೆ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_ತಲೆ)
			ಮುಂದುವರಿ;

		ಆದರೆ (action == REF_ACTION_ತಲೆ &&
		    decoration->type != DECORATION_REF_ತಲೆ)
			ಮುಂದುವರಿ;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		ಆದರೆ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_ತಲೆ &&
		    !detached_ತಲೆ)
			ಮುಂದುವರಿ;

		replay_ಫಲಿತಾಂಶ_ಸರತಿ_update(ಫಲಿತಾಂಶ, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ apply_pending_ref_updates(ರಚನೆ repository *repo,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ replay_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reflog_ಸಂದೇಶ,
				     ಪೂರ್ಣಾಂಕ dry_ಸತತ)
{
	ರಚನೆ ref_transaction *transaction = ಖಾಲಿ;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!dry_ಸತತ) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		ಆದರೆ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಫಲಿತಾಂಶ->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					ಫಲಿತಾಂಶ->updates[i].refname,
					&ಫಲಿತಾಂಶ->updates[i].new_oid,
					&ಫಲಿತಾಂಶ->updates[i].old_oid,
					reflog_ಸಂದೇಶ, &err);
		ಆದರೆ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    ಫಲಿತಾಂಶ->updates[i].refname, err.buf);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ಆದರೆ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ handle_reference_updates(ರಚನೆ rev_info *revs,
				    ಎಣಿಕೆ ref_action action,
				    ರಚನೆ commit *original,
				    ರಚನೆ commit *rewritten,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reflog_ಸಂದೇಶ,
				    ಪೂರ್ಣಾಂಕ dry_ಸತತ,
				    ಎಣಿಕೆ replay_ಖಾಲಿಗಳು_commit_action ಖಾಲಿಗಳು)
{
	ರಚನೆ replay_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  ಖಾಲಿಗಳು, &ಫಲಿತಾಂಶ);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ret = apply_pending_ref_updates(revs->repo, &ಫಲಿತಾಂಶ, reflog_ಸಂದೇಶ, dry_ಸತತ);

ಹೊರಹೋಗು:
	replay_ಫಲಿತಾಂಶ_release(&ಫಲಿತಾಂಶ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_became_ಖಾಲಿಗಳು(ರಚನೆ repository *repo,
			       ರಚನೆ commit *original,
			       ರಚನೆ ವೃಕ್ಷ *ಫಲಿತಾಂಶ)
{
	ರಚನೆ object_id ಪೋಷಕ_ವೃಕ್ಷ_oid;

	ಆದರೆ (ಮೊದಲ_ಪೋಷಕ_ವೃಕ್ಷ_oid(repo, original, &ಪೋಷಕ_ವೃಕ್ಷ_oid) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ oideq(&ಫಲಿತಾಂಶ->object.oid, &ಪೋಷಕ_ವೃಕ್ಷ_oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಎಣಿಕೆ replay_ಖಾಲಿಗಳು_commit_action *ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!strcmp(arg, "drop"))
		*ಮೌಲ್ಯ = REPLAY_ಖಾಲಿಗಳು_COMMIT_DROP;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "keep"))
		*ಮೌಲ್ಯ = REPLAY_ಖಾಲಿಗಳು_COMMIT_KEEP;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "abort"))
		*ಮೌಲ್ಯ = REPLAY_ಖಾಲಿಗಳು_COMMIT_ABORT;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_history_fixup(ಪೂರ್ಣಾಂಕ argc,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ಖಾಲಿ,
	};
	ಎಣಿಕೆ replay_ಖಾಲಿಗಳು_commit_action ಖಾಲಿಗಳು = REPLAY_ಖಾಲಿಗಳು_COMMIT_DROP;
	ಎಣಿಕೆ ref_action action = REF_ACTION_DEFAULT;
	ಎಣಿಕೆ commit_ವೃಕ್ಷ_flags flags = 0;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ಸತತ,
			 N_("perform a dry-run without updating any refs")),
		OPT_ಬಿಟ್(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ),
		OPT_CALLBACK_F(0, "empty", &ಖಾಲಿಗಳು, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ಖಾಲಿಗಳು),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ ವಿಲೀನ_ಫಲಿತಾಂಶ ವಿಲೀನ_ಫಲಿತಾಂಶ = { 0 };
	ರಚನೆ ವಿಲೀನ_options ವಿಲೀನ_opts = { 0 };
	ರಚನೆ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ commit *ತಲೆ_commit, *original, *rewritten;
	ರಚನೆ ವೃಕ್ಷ *ತಲೆ_ವೃಕ್ಷ, *original_ವೃಕ್ಷ, *ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ;
	ರಚನೆ rev_info revs = { 0 };
	bool skip_commit = false;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	ಆದರೆ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	ಆದರೆ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	ಆದರೆ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	ತಲೆ_commit = lookup_commit_reference_by_name("HEAD");
	ಆದರೆ (!ತಲೆ_commit) {
		ret = error(_("cannot look up HEAD"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ತಲೆ_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, ತಲೆ_commit);
	ಆದರೆ (!ತಲೆ_ವೃಕ್ಷ) {
		ret = error(_("cannot get tree for HEAD"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0) {
		ret = error(_("unable to read index"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!repo_ಸೂಚ್ಯಂಕ_has_changes(repo, ತಲೆ_ವೃಕ್ಷ, ಖಾಲಿ)) {
		ret = error(_("nothing to fixup: no staged changes"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ = write_ಒಳ_core_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(repo, repo->ಸೂಚ್ಯಂಕ);
	ಆದರೆ (!ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ) {
		ret = error(_("unable to write index as a tree"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	original_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, original);
	ಆದರೆ (!original_ವೃಕ್ಷ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_ವಿಲೀನ_options(&ವಿಲೀನ_opts, repo);
	ವಿಲೀನ_opts.ancestor = "HEAD";
	ವಿಲೀನ_opts.branch1 = argv[0];
	ವಿಲೀನ_opts.branch2 = "staged";
	ವಿಲೀನ_incore_nonrecursive(&ವಿಲೀನ_opts, ತಲೆ_ವೃಕ್ಷ,
				  original_ವೃಕ್ಷ, ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ, &ವಿಲೀನ_ಫಲಿತಾಂಶ);

	ಆದರೆ (ವಿಲೀನ_ಫಲಿತಾಂಶ.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!ವಿಲೀನ_ಫಲಿತಾಂಶ.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = commit_became_ಖಾಲಿಗಳು(repo, original, ವಿಲೀನ_ಫಲಿತಾಂಶ.ವೃಕ್ಷ);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;
	ಆದರೆ (ret > 0) {
		ಆಯ್ಕೆ (ಖಾಲಿಗಳು) {
		ಸ್ಥಿತಿ REPLAY_ಖಾಲಿಗಳು_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->ಅಂಶ : ಖಾಲಿ;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			ಆದರೆ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				ಹೋಗು ಹೊರಹೋಗು;
			}

			skip_commit = true;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ REPLAY_ಖಾಲಿಗಳು_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ REPLAY_ಖಾಲಿಗಳು_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ret = ಸಿದ್ಧತೆ_revwalk(repo, action, original, &revs);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!skip_commit) {
		ret = commit_ವೃಕ್ಷ_ext(repo, "fixup", original, ಖಾಲಿ, original->parents,
				      &original_ವೃಕ್ಷ->object.oid, &ವಿಲೀನ_ಫಲಿತಾಂಶ.ವೃಕ್ಷ->object.oid,
				      &rewritten, flags);
		ಆದರೆ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	strbuf_addf(&reflog_ಸಂದೇಶ, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_ಸತತ, ಖಾಲಿಗಳು);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	ವಿಲೀನ_finalize(&ವಿಲೀನ_opts, &ವಿಲೀನ_ಫಲಿತಾಂಶ);
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_history_reword(ಪೂರ್ಣಾಂಕ argc,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			      ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ಖಾಲಿ,
	};
	ಎಣಿಕೆ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ಸತತ,
			 N_("perform a dry-run without updating any refs")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ commit *original, *rewritten;
	ರಚನೆ rev_info revs = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	ಆದರೆ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ಆದರೆ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = ಸಿದ್ಧತೆ_revwalk(repo, action, original, &revs);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ret = commit_ವೃಕ್ಷ_with_edited_ಸಂದೇಶ(repo, "reworded", original, &rewritten);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	strbuf_addf(&reflog_ಸಂದೇಶ, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_ಸತತ, REPLAY_ಖಾಲಿಗಳು_COMMIT_ABORT);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_ondisk_ಸೂಚ್ಯಂಕ(ರಚನೆ repository *repo,
			      ರಚನೆ object_id *oid,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ unpack_trees_options opts = { 0 };
	ರಚನೆ ಬೀಗ_file ಬೀಗ = ಬೀಗ_INIT;
	ರಚನೆ ವೃಕ್ಷ_desc ವೃಕ್ಷ_desc;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state ಸೂಚ್ಯಂಕ;
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಪೂರ್ಣಾಂಕ ret;

	ಸೂಚ್ಯಂಕ_state_init(&ಸೂಚ್ಯಂಕ, repo);

	opts.ತಲೆ_idx = -1;
	opts.src_ಸೂಚ್ಯಂಕ = &ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = &ಸೂಚ್ಯಂಕ;

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(repo, oid);
	ಆದರೆ (!ವೃಕ್ಷ) {
		ret = error(_("unable to parse tree %s"), oid_ಗಮ್ಯ_hex(oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ_desc, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);

	ಆದರೆ (unpack_trees(1, &ವೃಕ್ಷ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಅವಿಭಾಜ್ಯ_cache_ವೃಕ್ಷ(repo, &ಸೂಚ್ಯಂಕ, ವೃಕ್ಷ);

	ಆದರೆ (repo_hold_ಬೀಗ_file_for_update(repo, &ಬೀಗ, ಮಾರ್ಗ, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(&ಸೂಚ್ಯಂಕ, &ಬೀಗ, COMMIT_ಬೀಗ)) {
		ret = error(_("unable to write new index file"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	rollback_ಬೀಗ_file(&ಬೀಗ);
	release_ಸೂಚ್ಯಂಕ(&ಸೂಚ್ಯಂಕ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_commit(ರಚನೆ repository *repo,
			ರಚನೆ commit *original,
			ರಚನೆ pathspec *pathspec,
			ರಚನೆ commit **ಹೊರಹೋಗು)
{
	ರಚನೆ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ರಚನೆ strbuf ಸೂಚ್ಯಂಕ_file = STRBUF_INIT;
	ರಚನೆ ಸೂಚ್ಯಂಕ_state ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ_STATE_INIT(repo);
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *original_commit_ವೃಕ್ಷ_oid;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_ವೃಕ್ಷ_oid, *new_ವೃಕ್ಷ_oid;
	ರಚನೆ object_id ಪೋಷಕ_ವೃಕ್ಷ_oid;
	ಅಕ್ಷರ original_commit_oid[GIT_MAX_HEXSZ + 1];
	ರಚನೆ commit *ಮೊದಲ_commit, *ಎರಡನೇ_commit;
	ರಚನೆ commit_list *parents = ಖಾಲಿ;
	ರಚನೆ ವೃಕ್ಷ *ವಿಭಜಿಸು_ವೃಕ್ಷ;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (ಮೊದಲ_ಪೋಷಕ_ವೃಕ್ಷ_oid(repo, original, &ಪೋಷಕ_ವೃಕ್ಷ_oid) < 0) {
		ret = -1;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	original_commit_ವೃಕ್ಷ_oid = get_commit_ವೃಕ್ಷ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_ಮಾರ್ಗ_replace(repo, &ಸೂಚ್ಯಂಕ_file, "%s", "history-split.index");

	ret = write_ondisk_ಸೂಚ್ಯಂಕ(repo, &ಪೋಷಕ_ವೃಕ್ಷ_oid, ಸೂಚ್ಯಂಕ_file.buf);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ret = read_ಸೂಚ್ಯಂಕ_from(&ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_file.buf, repo->gitdir);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	oid_ಗಮ್ಯ_hex_r(original_commit_oid, &original->object.oid);
	ret = ಸತತ_ಕೂಡಿಸು_p_ಸೂಚ್ಯಂಕ(repo, &ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ಕೂಡಿಸು_P_DISALLOW_EDIT);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ವಿಭಜಿಸು_ವೃಕ್ಷ = write_ಒಳ_core_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(repo, &ಸೂಚ್ಯಂಕ);
	ಆದರೆ (!ವಿಭಜಿಸು_ವೃಕ್ಷ) {
		ret = error(_("failed split tree"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	unlink(ಸೂಚ್ಯಂಕ_file.buf);
	strbuf_release(&ಸೂಚ್ಯಂಕ_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	ಆದರೆ (oideq(&ವಿಭಜಿಸು_ವೃಕ್ಷ->object.oid, &ಪೋಷಕ_ವೃಕ್ಷ_oid)) {
		ret = error(_("split commit is empty"));
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oideq(&ವಿಭಜಿಸು_ವೃಕ್ಷ->object.oid, original_commit_ವೃಕ್ಷ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_ವೃಕ್ಷ_ext(repo, "split-out", original, ಖಾಲಿ, original->parents, &ಪೋಷಕ_ವೃಕ್ಷ_oid,
			      &ವಿಭಜಿಸು_ವೃಕ್ಷ->object.oid, &ಮೊದಲ_commit, COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed writing first commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(ಮೊದಲ_commit, &parents);

	old_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, ಮೊದಲ_commit)->object.oid;
	new_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, original)->object.oid;

	ret = commit_ವೃಕ್ಷ_ext(repo, "split-out", original, ಖಾಲಿ, parents, old_ವೃಕ್ಷ_oid,
			      new_ವೃಕ್ಷ_oid, &ಎರಡನೇ_commit, COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed writing second commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	*ಹೊರಹೋಗು = ಎರಡನೇ_commit;
	ret = 0;

ಹೊರಹೋಗು:
	ಆದರೆ (ಸೂಚ್ಯಂಕ_file.len)
		unlink(ಸೂಚ್ಯಂಕ_file.buf);
	strbuf_release(&ಸೂಚ್ಯಂಕ_file);
	commit_list_free(parents);
	release_ಸೂಚ್ಯಂಕ(&ಸೂಚ್ಯಂಕ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_history_ವಿಭಜಿಸು(ಪೂರ್ಣಾಂಕ argc,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		GIT_HISTORY_ವಿಭಜಿಸು_USAGE,
		ಖಾಲಿ,
	};
	ಎಣಿಕೆ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ಸತತ,
			 N_("perform a dry-run without updating any refs")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ commit *original, *rewritten = ಖಾಲಿ;
	ರಚನೆ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ pathspec pathspec = { 0 };
	ರಚನೆ rev_info revs = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	ಆದರೆ (argc < 1) {
		ret = error(_("command expects a committish"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ತುಂಬಿದ |
		       PATHSPEC_SYMLINK_LEADING_ಮಾರ್ಗ |
		       PATHSPEC_ಪೂರ್ವಪ್ರತ್ಯಯ_ORIGIN,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	ಆದರೆ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = ಸಿದ್ಧತೆ_revwalk(repo, action, original, &revs);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (original->parents && original->parents->ಮುಂದಿನ) {
		ret = error(_("cannot split up merge commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = ವಿಭಜಿಸು_commit(repo, original, &pathspec, &rewritten);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	strbuf_addf(&reflog_ಸಂದೇಶ, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_ಸತತ, REPLAY_ಖಾಲಿಗಳು_COMMIT_ABORT);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	strbuf_release(&reflog_ಸಂದೇಶ);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	ಹಿಂತಿರುಗಿ ret;
}

#ವ್ಯಾಖ್ಯಾನಿಸು SQUASH_ನೋಡಿದ (1u << 11)
#ವ್ಯಾಖ್ಯಾನಿಸು SQUASH_TIP (1u << 12)
#ವ್ಯಾಖ್ಯಾನಿಸು SQUASH_AMEND_ಗುರಿ (1u << 13)

ಸ್ಥಿರ bool is_autosquash_subject(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಹಿಂತಿರುಗಿ starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ಸ್ಥಿರ bool skip_one_autosquash_ಪೂರ್ವಪ್ರತ್ಯಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಹೊರಹೋಗು)
{
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(s, "amend!", ಹೊರಹೋಗು) || skip_ಪೂರ್ವಪ್ರತ್ಯಯ(s, "fixup!", ಹೊರಹೋಗು) ||
	    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(s, "squash!", ಹೊರಹೋಗು)) {
		ಆಗ (**ಹೊರಹೋಗು == ' ')
			(*ಹೊರಹೋಗು)++;
		ಹಿಂತಿರುಗಿ true;
	}
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ truncate_ಸಂದೇಶ_ಗಮ್ಯ_subject(ರಚನೆ strbuf *ಸಂದೇಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *eos = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಸಂದೇಶ->buf, "\n\n");

	ಆದರೆ (eos)
		strbuf_setlen(ಸಂದೇಶ, eos - ಸಂದೇಶ->buf + 1);
}

ರಚನೆ subject_ದತ್ತಾಂಶ {
	ರಚನೆ strintmap subjects;
	ರಚನೆ strbuf subject;
	ರಚನೆ strbuf squash_ಸಂದೇಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2;
	bool edit_ಸಂದೇಶ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು SUBJECT_ದತ್ತಾಂಶ_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_ಸಂದೇಶ = STRBUF_INIT,	\
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_ದತ್ತಾಂಶ_clear(ರಚನೆ subject_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	strintmap_clear(&ದತ್ತಾಂಶ->subjects);
	strbuf_release(&ದತ್ತಾಂಶ->subject);
	strbuf_release(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ squash_amend_ಸಂದೇಶ(ರಚನೆ repository *repo,
				ರಚನೆ commit *commit,
				ರಚನೆ subject_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				ಚಿಹ್ನೆರಹಿತ flags)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *body = ದತ್ತಾಂಶ->ಸಂದೇಶ_2 + ದತ್ತಾಂಶ->subject.len;

	ಆಗ (ಖಾಲಿಯೇ(*body))
		body++;

	ಆದರೆ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			ದತ್ತಾಂಶ->subject.buf);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (ದತ್ತಾಂಶ->edit_ಸಂದೇಶ) {
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & SQUASH_AMEND_ಗುರಿ) {
		ಆದರೆ (starts_with(ದತ್ತಾಂಶ->squash_ಸಂದೇಶ.buf, "squash!"))
			ಹಿಂತಿರುಗಿ error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     ದತ್ತಾಂಶ->subject.buf);
		ಆದರೆ (starts_with(ದತ್ತಾಂಶ->squash_ಸಂದೇಶ.buf, "fixup!"))
			strbuf_splice(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, 0, 5, "amend!", 5);
		ಆದರೆ (starts_with(ದತ್ತಾಂಶ->squash_ಸಂದೇಶ.buf, "amend!")) {
			truncate_ಸಂದೇಶ_ಗಮ್ಯ_subject(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
			strbuf_addch(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, '\n');
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_reset(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
		}
		strbuf_addstr(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, body);
		strbuf_complete_ಸಾಲು(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     ದತ್ತಾಂಶ->subject.buf);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ squash_squash_ಸಂದೇಶ(ರಚನೆ repository *repo,
				ರಚನೆ commit *commit,
				ರಚನೆ subject_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				ಚಿಹ್ನೆರಹಿತ flags)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *body = ದತ್ತಾಂಶ->ಸಂದೇಶ_2 + ದತ್ತಾಂಶ->subject.len;

	ಆಗ (ಖಾಲಿಯೇ(*body))
		body++;

	ಆದರೆ (ದತ್ತಾಂಶ->edit_ಸಂದೇಶ) {
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & SQUASH_AMEND_ಗುರಿ) {
		ಆದರೆ (starts_with(ದತ್ತಾಂಶ->squash_ಸಂದೇಶ.buf, "fixup!")) {
			truncate_ಸಂದೇಶ_ಗಮ್ಯ_subject(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
			strbuf_splice(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, 0, 5, "squash", 6);
		}
		ಆದರೆ (starts_with(ದತ್ತಾಂಶ->squash_ಸಂದೇಶ.buf, "squash!")) {
			strbuf_addch(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, '\n');
			strbuf_addstr(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, body);
			strbuf_complete_ಸಾಲು(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಹಿಂತಿರುಗಿ error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     ದತ್ತಾಂಶ->subject.buf);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      ದತ್ತಾಂಶ->subject.buf);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ squash_check_can_autosquash(ರಚನೆ repository *repo,
				       ರಚನೆ commit *commit,
				       ರಚನೆ subject_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
				       ಚಿಹ್ನೆರಹಿತ flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_ಗುರಿ;
	ಆದರೆ (starts_with(ದತ್ತಾಂಶ->subject.buf, "amend!"))
		ಹಿಂತಿರುಗಿ squash_amend_ಸಂದೇಶ(repo, commit, ದತ್ತಾಂಶ, flags);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(ದತ್ತಾಂಶ->subject.buf, "squash!"))
		ಹಿಂತಿರುಗಿ squash_squash_ಸಂದೇಶ(repo, commit, ದತ್ತಾಂಶ, flags);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ squash_check_autosquash_subject(ರಚನೆ repository *repo,
					   ರಚನೆ commit *commit,
					   ರಚನೆ subject_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* s = ದತ್ತಾಂಶ->subject.buf;
	ರಚನೆ commit *ಗುರಿ;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ strmap_ನಮೂದು *ನಮೂದು;
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
	ಆಗ (skip_one_autosquash_ಪೂರ್ವಪ್ರತ್ಯಯ(s, &s)) {
		ಚಿಹ್ನೆರಹಿತ flags = strintmap_get(&ದತ್ತಾಂಶ->subjects, s);
		ಆದರೆ (flags)
			ಹಿಂತಿರುಗಿ squash_check_can_autosquash(repo, commit, ದತ್ತಾಂಶ, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	ಗುರಿ = lookup_commit_reference_by_name(s);
	ಆದರೆ (ಗುರಿ && starts_with(oid_ಗಮ್ಯ_hex(&ಗುರಿ->object.oid), s)) {
		ಚಿಹ್ನೆರಹಿತ flags =
			ಗುರಿ->object.flags & (SQUASH_ನೋಡಿದ | SQUASH_AMEND_ಗುರಿ);
		ಆದರೆ (!flags) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subject_ಆರಂಭ;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್ = repo_logmsg_reencode(repo, ಗುರಿ,
								  ಖಾಲಿ, ಖಾಲಿ);
			ಗಾತ್ರ_t subject_len = ಕಂಡುಹಿಡಿ_commit_subject(ಬಫರ್,
								 &subject_ಆರಂಭ);
			ಅಕ್ಷರ *subject = xmemdupz(subject_ಆರಂಭ, subject_len);

			flags = strintmap_get(&ದತ್ತಾಂಶ->subjects, subject);
			ಬಿಡುಗಡೆ(subject);
			repo_unuse_commit_ಬಫರ್(repo, ಗುರಿ, ಬಫರ್);
		}
		ಆದರೆ (flags)
			ಹಿಂತಿರುಗಿ squash_check_can_autosquash(repo, commit,
							   ದತ್ತಾಂಶ, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_ನಮೂದು(&ದತ್ತಾಂಶ->subjects, &iter, ನಮೂದು) {
		s = ದತ್ತಾಂಶ->subject.buf;
		ಆಗ(skip_one_autosquash_ಪೂರ್ವಪ್ರತ್ಯಯ(s, &s)) {
			ಆದರೆ (starts_with(ನಮೂದು->ಕೀಲಿ, s)) {
				ಚಿಹ್ನೆರಹಿತ ಮೌಲ್ಯ = (intptr_t)ನಮೂದು->ಮೌಲ್ಯ;

				ಹಿಂತಿರುಗಿ squash_check_can_autosquash(repo, commit,
								   ದತ್ತಾಂಶ, ಮೌಲ್ಯ);
			}
		}
	}
	ಹಿಂತಿರುಗಿ error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       ದತ್ತಾಂಶ->subject.buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ squash_check_subject(ರಚನೆ repository *repo,
				ರಚನೆ commit *commit,
				ರಚನೆ subject_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = repo_logmsg_reencode(repo, commit, ಖಾಲಿ, ಖಾಲಿ);
	ಗಾತ್ರ_t subject_len = ಕಂಡುಹಿಡಿ_commit_subject(buf, &ದತ್ತಾಂಶ->ಸಂದೇಶ_2);

	strbuf_reset(&ದತ್ತಾಂಶ->subject);
	strbuf_ಕೂಡಿಸು(&ದತ್ತಾಂಶ->subject, ದತ್ತಾಂಶ->ಸಂದೇಶ_2, subject_len);

	ಆದರೆ (!strintmap_get_ಗಾತ್ರ(&ದತ್ತಾಂಶ->subjects)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s;

		strbuf_addstr(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ, ದತ್ತಾಂಶ->ಸಂದೇಶ_2);
		strbuf_complete_ಸಾಲು(&ದತ್ತಾಂಶ->squash_ಸಂದೇಶ);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = ದತ್ತಾಂಶ->subject.buf;
		skip_one_autosquash_ಪೂರ್ವಪ್ರತ್ಯಯ(s, &s);
		strintmap_ಗಣ(&ದತ್ತಾಂಶ->subjects, s, SQUASH_AMEND_ಗುರಿ | SQUASH_ನೋಡಿದ);
		commit->object.flags |= SQUASH_AMEND_ಗುರಿ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_autosquash_subject(ದತ್ತಾಂಶ->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, ದತ್ತಾಂಶ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strintmap_ಗಣ(&ದತ್ತಾಂಶ->subjects, ದತ್ತಾಂಶ->subject.buf, SQUASH_ನೋಡಿದ);
	}
	repo_unuse_commit_ಬಫರ್(repo, commit, buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ build_squash_ಸಂದೇಶ(ರಚನೆ repository *repo,
				ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *todo_buf,
				ರಚನೆ strbuf *ಹೊರಹೋಗು);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಿದ್ಧತೆ_squash_revisions(ರಚನೆ repository *repo,
				  ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				  ರಚನೆ rev_info *revs)
{
	repo_init_revisions(repo, revs, ಖಾಲಿ);
	revs->ಹಿಮ್ಮುಖ = 1;
	revs->topo_ಕ್ರಮ = 1;
	revs->ವಿಂಗಡಿಸು_ಕ್ರಮ = REV_ವಿಂಗಡಿಸು_ಒಳ_ನಕ್ಷೆ_ಕ್ರಮ;
	revs->simplify_history = 0;
	revs->ancestry_ಮಾರ್ಗ = 1;
	revs->limited = 1;
	revs->ancestry_ಮಾರ್ಗ_implicit_bottoms = 1;

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, revs, ಖಾಲಿ);
	ಆದರೆ (argc > 1)
		ಹಿಂತಿರುಗಿ error(_("unrecognized argument: %s"), argv[1]);

	ಆದರೆ (revs->ಹಿಮ್ಮುಖ != 1 || revs->topo_ಕ್ರಮ != 1 ||
	    revs->ವಿಂಗಡಿಸು_ಕ್ರಮ != REV_ವಿಂಗಡಿಸು_ಒಳ_ನಕ್ಷೆ_ಕ್ರಮ ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_ಮಾರ್ಗ != 1 || revs->limited != 1 ||
	    revs->ancestry_ಮಾರ್ಗ_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->ಹಿಮ್ಮುಖ = 1;
		revs->topo_ಕ್ರಮ = 1;
		revs->ವಿಂಗಡಿಸು_ಕ್ರಮ = REV_ವಿಂಗಡಿಸು_ಒಳ_ನಕ್ಷೆ_ಕ್ರಮ;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_ಮಾರ್ಗ = 1;
		revs->limited = 1;
		revs->ancestry_ಮಾರ್ಗ_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < revs->cmdline.nr; i++)
		ಆದರೆ (revs->cmdline.rev[i].flags & BOTTOM)
			ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ resolve_squash_range(ರಚನೆ repository *repo,
				bool update_branches,
				bool edit_ಸಂದೇಶ,
				ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				ರಚನೆ commit **oldest_ಹೊರಹೋಗು,
				ರಚನೆ commit **tip_ಹೊರಹೋಗು,
				ಅಕ್ಷರ **ಸಂದೇಶ_ಹೊರಹೋಗು)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ subject_ದತ್ತಾಂಶ subject_ದತ್ತಾಂಶ = SUBJECT_ದತ್ತಾಂಶ_INIT;
	ರಚನೆ commit *commit, *oldest = ಖಾಲಿ, *tip = ಖಾಲಿ;
	ರಚನೆ strbuf todo_buf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret, tip_ಎಣಿಕೆ = 0;
	bool walk_started = false;
	ರಚನೆ ref_filter filter = REF_FILTER_INIT;
	ರಚನೆ ref_array refs = { 0 };

	subject_ದತ್ತಾಂಶ.edit_ಸಂದೇಶ = edit_ಸಂದೇಶ;
	ret = ಸಿದ್ಧತೆ_squash_revisions(repo, argc, argv, &revs);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	walk_started = true;
	ಆಗ ((commit = get_revision(&revs))) {
		ರಚನೆ commit_list *p;

		ಆದರೆ (edit_ಸಂದೇಶ)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_ಗಮ್ಯ_hex(&commit->object.oid));

		ಆದರೆ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ಗಾಗಿ (p = commit->parents; oldest && p; p = p->ಮುಂದಿನ) {
			ರಚನೆ commit_list *q;
			ರಚನೆ object *o;
			bool ನೋಡಿದ;

			ಆದರೆ (repo_parse_commit(repo, p->ಅಂಶ)) {
				ret = error(_("cannot parse commit"));
				ಹೋಗು ಹೊರಹೋಗು;
			}
			o = &p->ಅಂಶ->object;
			ನೋಡಿದ = o->flags & SQUASH_ನೋಡಿದ;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			ಗಾಗಿ (q = oldest->parents; !ನೋಡಿದ && q; q = q->ಮುಂದಿನ) {
				ಆದರೆ (p->ಅಂಶ == q->ಅಂಶ) {
					ನೋಡಿದ = true;
					commit_list_ಸೇರಿಸು(commit, &filter.with_commit);
				}
			}
			ಆದರೆ (!ನೋಡಿದ) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				ಹೋಗು ಹೊರಹೋಗು;
			}
			ಆದರೆ (o->flags & SQUASH_TIP) {
				tip_ಎಣಿಕೆ--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		ಆದರೆ (!oldest) {
			commit_list_ಸೇರಿಸು(commit, &filter.with_commit);
			oldest = commit;
		}
		ಆದರೆ (squash_check_subject(repo, commit, &subject_ದತ್ತಾಂಶ)) {
			ret = -1;
			ಹೋಗು ಹೊರಹೋಗು;
		}
		tip = commit;
		tip->object.flags |= SQUASH_ನೋಡಿದ | SQUASH_TIP;
		tip_ಎಣಿಕೆ++;
	}
	clear_object_flags(repo, SQUASH_ನೋಡಿದ | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	ಆದರೆ (!tip_ಎಣಿಕೆ) {
		ret = error(_("the revision range is empty"));
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tip_ಎಣಿಕೆ != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ಸೇರಿಸು(tip, &filter.no_commit);
	filter.ಬಗೆ = FILTER_REFS_BRANCHES;
	ಆದರೆ (update_branches &&
	    filter_refs(&refs, &filter, filter.ಬಗೆ)) {
		ret = error(_("could not filter refs"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	ಆದರೆ (refs.nr) {
		ರಚನೆ ref_format format = REF_FORMAT_INIT;
		ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ;
		ರಚನೆ string_list ವಿಂಗಡಣೆ_options = STRING_LIST_INIT_DUP;
		ರಚನೆ strbuf branches = STRBUF_INIT;
		ರಚನೆ strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		ಆದರೆ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&ವಿಂಗಡಣೆ_options, "refname");
		ವಿಂಗಡಣೆ = ref_ವಿಂಗಡಣೆ_options(&ವಿಂಗಡಣೆ_options);
		ref_array_ವಿಂಗಡಿಸು(ವಿಂಗಡಣೆ, &refs);
		ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			ಆದರೆ (format_ref_array_ಅಂಶ(refs.ವಸ್ತುಗಳು[i], &format,
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
		ref_ವಿಂಗಡಣೆ_release(ವಿಂಗಡಣೆ);
		string_list_clear(&ವಿಂಗಡಣೆ_options, 0);
		ಹೋಗು ಹೊರಹೋಗು;
	}
	ಆದರೆ (edit_ಸಂದೇಶ) {
		strbuf_reset(&subject_ದತ್ತಾಂಶ.squash_ಸಂದೇಶ);
		ret = build_squash_ಸಂದೇಶ(repo, &todo_buf,
					   &subject_ದತ್ತಾಂಶ.squash_ಸಂದೇಶ);
		ಆದರೆ (ret < 0)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	*oldest_ಹೊರಹೋಗು = oldest;
	*tip_ಹೊರಹೋಗು = tip;
	*ಸಂದೇಶ_ಹೊರಹೋಗು = strbuf_detach(&subject_ದತ್ತಾಂಶ.squash_ಸಂದೇಶ, ಖಾಲಿ);
	ret = 0;

ಹೊರಹೋಗು:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_ನೋಡಿದ | SQUASH_TIP |
			   SQUASH_AMEND_ಗುರಿ);
	ಆದರೆ (walk_started)
		reset_revision_walk();
	subject_ದತ್ತಾಂಶ_clear(&subject_ದತ್ತಾಂಶ);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ bool amend_replaces_ಗುರಿ(ರಚನೆ todo_list *todo, ಪೂರ್ಣಾಂಕ ಗುರಿ)
{
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಗುರಿ + 1; i < todo->nr &&
				 todo->ವಸ್ತುಗಳು[i].command != TODO_PICK; i++) {
		ಆದರೆ (todo->ವಸ್ತುಗಳು[i].command == TODO_SQUASH)
			ಹಿಂತಿರುಗಿ false;
		ಆದರೆ (todo->ವಸ್ತುಗಳು[i].flags & TODO_REPLACE_FIXUP_ಸಂದೇಶ)
			ಹಿಂತಿರುಗಿ true;
	}
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ build_squash_ಸಂದೇಶ(ರಚನೆ repository *repo,
				ಸ್ಥಿರಾಂಕ ರಚನೆ strbuf *todo_buf,
				ರಚನೆ strbuf *ಹೊರಹೋಗು)
{
	ರಚನೆ todo_list todo = TODO_LIST_INIT;
	ರಚನೆ replay_opts opts = REPLAY_OPTS_INIT;
	ಪೂರ್ಣಾಂಕ nr_commits, ret;

	ಆದರೆ (todo_list_parse_insn_ಬಫರ್(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	nr_commits = todo.nr;
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nr_commits; i++) {
		ರಚನೆ todo_ಅಂಶ *ಅಂಶ = &todo.ವಸ್ತುಗಳು[i];
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2, *body;
		ಗಾತ್ರ_t commented_len;
		bool skip, squashing;

		squashing = ಅಂಶ->command == TODO_SQUASH ||
			    (ಅಂಶ->flags & TODO_REPLACE_FIXUP_ಸಂದೇಶ);
		ಆದರೆ (ಅಂಶ->command == TODO_PICK)
			skip = amend_replaces_ಗುರಿ(&todo, i);
		ಇಲ್ಲದಿದ್ದರೆ
			skip = !squashing;

		ಸಂದೇಶ_2 = repo_logmsg_reencode(repo, ಅಂಶ->commit, ಖಾಲಿ, ಖಾಲಿ);
		ಕಂಡುಹಿಡಿ_commit_subject(ಸಂದೇಶ_2, &body);

		ಆದರೆ (skip)
			commented_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(body);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		ಇಲ್ಲದಿದ್ದರೆ
			commented_len = 0;

		ಆದರೆ (!i)
			ಕೂಡಿಸು_squash_combination_ಶೀರ್ಷಿಕೆ(ಹೊರಹೋಗು, nr_commits);
		strbuf_addch(ಹೊರಹೋಗು, '\n');
		ಕೂಡಿಸು_squash_ಸಂದೇಶ_ಶೀರ್ಷಿಕೆ(ಹೊರಹೋಗು, i + 1, skip);
		strbuf_addstr(ಹೊರಹೋಗು, "\n\n");
		strbuf_ಕೂಡಿಸು_commented_ಸಾಲುಗಳು(ಹೊರಹೋಗು, body, commented_len, comment_ಸಾಲು_str);
		strbuf_addstr(ಹೊರಹೋಗು, body + commented_len);
		strbuf_complete_ಸಾಲು(ಹೊರಹೋಗು);

		repo_unuse_commit_ಬಫರ್(repo, ಅಂಶ->commit, ಸಂದೇಶ_2);
	}

	ret = 0;

ಹೊರಹೋಗು:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_history_squash(ಪೂರ್ಣಾಂಕ argc,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			      ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ಖಾಲಿ,
	};
	ಎಣಿಕೆ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0;
	ಪೂರ್ಣಾಂಕ edit = 1;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ಸತತ,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ commit *oldest, *tip, *rewritten;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಳ_ವೃಕ್ಷ_oid, *tip_ವೃಕ್ಷ_oid;
	ಅಕ್ಷರ *ಸಂದೇಶ_template = ಖಾಲಿ;
	ರಚನೆ rev_info revs = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	ಆದರೆ (argc < 2) {
		ret = error(_("command expects a revision range"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_ಸಂದೇಶ, "squash: updating ");
	strbuf_join_argv(&reflog_ಸಂದೇಶ, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &ಸಂದೇಶ_template);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ret = ಸಿದ್ಧತೆ_revwalk(repo, action, tip, &revs);
	ಆದರೆ (ret < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	ತಳ_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo,
					oldest->parents->ಅಂಶ)->object.oid;
	tip_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, tip)->object.oid;

	ret = commit_ವೃಕ್ಷ_ext(repo, "squash", oldest, ಸಂದೇಶ_template,
			      oldest->parents, ತಳ_ವೃಕ್ಷ_oid, tip_ವೃಕ್ಷ_oid,
			      &rewritten,
			      edit ? COMMIT_ವೃಕ್ಷ_EDIT_ಸಂದೇಶ : 0);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_ಸತತ,
				       REPLAY_ಖಾಲಿಗಳು_COMMIT_ABORT);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಬಿಡುಗಡೆ(ಸಂದೇಶ_template);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_worktree(ರಚನೆ repository *repo,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ commit *old_ತಲೆ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ commit *new_ತಲೆ,
			   bool dry_ಸತತ)
{
	ರಚನೆ reset_working_ವೃಕ್ಷ_options opts = {
		.oid_from = &old_ತಲೆ->object.oid,
		.oid = &new_ತಲೆ->object.oid,
	};
	ಆದರೆ (dry_ಸತತ)
		opts.flags |= RESET_WORKING_ವೃಕ್ಷ_DRY_ಸತತ;
	ಹಿಂತಿರುಗಿ reset_working_ವೃಕ್ಷ(repo, &opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ತಲೆ_ವೃಕ್ಷ_change(ರಚನೆ repository *repo,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ replay_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ,
				 ರಚನೆ commit **old_ತಲೆ,
				 ರಚನೆ commit **new_ತಲೆ,
				 bool *ಬದಲಾಯಿತು)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ replay_ref_update *ತಲೆ_update = ಖಾಲಿ;
	ರಚನೆ commit *old_ತಲೆ_commit, *new_ತಲೆ_commit;
	ರಚನೆ ವೃಕ್ಷ *old_ತಲೆ_ವೃಕ್ಷ, *new_ತಲೆ_ವೃಕ್ಷ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_ಗುರಿ;
	ಪೂರ್ಣಾಂಕ ತಲೆ_flags;

	*ಬದಲಾಯಿತು = false;

	ತಲೆ_ಗುರಿ = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ಖಾಲಿ, &ತಲೆ_flags);
	ಆದರೆ (!ತಲೆ_ಗುರಿ)
		ಹಿಂತಿರುಗಿ error(_("cannot look up HEAD"));

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಫಲಿತಾಂಶ->updates_nr; i++) {
		ಆದರೆ (!strcmp(ಫಲಿತಾಂಶ->updates[i].refname, ತಲೆ_ಗುರಿ)) {
			ತಲೆ_update = &ಫಲಿತಾಂಶ->updates[i];
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (!ತಲೆ_update)
		ಹಿಂತಿರುಗಿ 0;

	old_ತಲೆ_commit = lookup_commit_reference(repo, &ತಲೆ_update->old_oid);
	new_ತಲೆ_commit = lookup_commit_reference(repo, &ತಲೆ_update->new_oid);
	ಆದರೆ (!old_ತಲೆ_commit || !new_ತಲೆ_commit)
		ಹಿಂತಿರುಗಿ error(_("cannot resolve HEAD commit"));

	old_ತಲೆ_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, old_ತಲೆ_commit);
	new_ತಲೆ_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, new_ತಲೆ_commit);
	ಆದರೆ (!old_ತಲೆ_ವೃಕ್ಷ || !new_ತಲೆ_ವೃಕ್ಷ)
		ಹಿಂತಿರುಗಿ error(_("cannot resolve tree for HEAD"));

	ಆದರೆ (oideq(&old_ತಲೆ_ವೃಕ್ಷ->object.oid, &new_ತಲೆ_ವೃಕ್ಷ->object.oid))
		ಹಿಂತಿರುಗಿ 0;

	*old_ತಲೆ = old_ತಲೆ_commit;
	*new_ತಲೆ = new_ತಲೆ_commit;
	*ಬದಲಾಯಿತು = true;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_history_drop(ಪೂರ್ಣಾಂಕ argc,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			    ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ಖಾಲಿ,
	};
	ಎಣಿಕೆ replay_ಖಾಲಿಗಳು_commit_action ಖಾಲಿಗಳು = REPLAY_ಖಾಲಿಗಳು_COMMIT_DROP;
	ಎಣಿಕೆ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ಸತತ,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &ಖಾಲಿಗಳು, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ಖಾಲಿಗಳು),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ commit *original, *rewritten;
	ರಚನೆ rev_info revs = { 0 };
	ರಚನೆ replay_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ = { 0 };
	ರಚನೆ commit *old_ತಲೆ, *new_ತಲೆ;
	bool ತಲೆ_ಚಲನೆಗಳು = false;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	ಆದರೆ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ಹೋಗು ಹೊರಹೋಗು;
	}
	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ಆದರೆ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (original->parents->ಮುಂದಿನ) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = ಸಿದ್ಧತೆ_revwalk(repo, action, original, &revs);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	rewritten = original->parents->ಅಂಶ;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  ಖಾಲಿಗಳು, &ಫಲಿತಾಂಶ);
	ಆದರೆ (ret) {
		ret = error(_("failed replaying descendants"));
		ಹೋಗು ಹೊರಹೋಗು;
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
	ಆದರೆ (!is_bare_repository(repo)) {
		ret = ಕಂಡುಹಿಡಿ_ತಲೆ_ವೃಕ್ಷ_change(repo, &ಫಲಿತಾಂಶ, &old_ತಲೆ,
					    &new_ತಲೆ, &ತಲೆ_ಚಲನೆಗಳು);
		ಆದರೆ (ret < 0)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (ತಲೆ_ಚಲನೆಗಳು && update_worktree(repo, old_ತಲೆ, new_ತಲೆ, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	strbuf_addf(&reflog_ಸಂದೇಶ, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &ಫಲಿತಾಂಶ, reflog_ಸಂದೇಶ.buf, dry_ಸತತ);
	ಆದರೆ (ret < 0) {
		ret = error(_("failed to update references"));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!dry_ಸತತ && ತಲೆ_ಚಲನೆಗಳು && update_worktree(repo, old_ತಲೆ, new_ತಲೆ, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_ಗಮ್ಯ_hex(&new_ತಲೆ->object.oid));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = 0;

ಹೊರಹೋಗು:
	replay_ಫಲಿತಾಂಶ_release(&ಫಲಿತಾಂಶ);
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_history(ಪೂರ್ಣಾಂಕ argc,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_ವಿಭಜಿಸು_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ಖಾಲಿ,
	};
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_ವಿಭಜಿಸು),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);
	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
