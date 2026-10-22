#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ort.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ref-filter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replay.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wt-status.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GIT_HISTORY_ਵੰਡੋ_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ਸਥਿਰ ਖਾਲੀ change_ਅੰਕੜੇ_free(ਖਾਲੀ *util, ਅਚਲ ਅੱਖਰ *str ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ wt_ਸਥਿਤੀ_change_ਅੰਕੜੇ *d = util;
	ਮੁਕਤ(d->rename_ਸਰੋਤ);
	ਮੁਕਤ(d);
}

ਸਥਿਰ ਪੂਰਨ fill_commit_ਸੁਨੇਹਾ(ਬਣਤਰ repository *repo,
			       ਅਚਲ ਬਣਤਰ object_id *old_ਰੁੱਖ,
			       ਅਚਲ ਬਣਤਰ object_id *new_ਰੁੱਖ,
			       ਅਚਲ ਅੱਖਰ *default_ਸੁਨੇਹਾ,
			       ਅਚਲ ਅੱਖਰ *action,
			       ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = git_ਰਸਤਾ_commit_editmsg();
	ਅਚਲ ਅੱਖਰ *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ਬਣਤਰ wt_ਸਥਿਤੀ s;

	wt_ਸਥਿਤੀ_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_ਤੇਜ਼;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_ਅਗੇਤਰ = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(ਰਸਤਾ, "w");
	ਜੇ (!s.fp)
		ਵਾਪਸ error_errno(_("could not open '%s'"), ਰਸਤਾ);

	strbuf_addstr(ਬਾਹਰਲਾ, default_ਸੁਨੇਹਾ);
	strbuf_addch(ਬਾਹਰਲਾ, '\n');
	strbuf_commented_addf(ਬਾਹਰਲਾ, comment_ਸਤਰ_str, hint, action, comment_ਸਤਰ_str);
	ਜੇ (fwrite(ਬਾਹਰਲਾ->buf, 1, ਬਾਹਰਲਾ->len, s.fp) != ਬਾਹਰਲਾ->len)
		die_errno(_("could not write to '%s'"), ਰਸਤਾ);

	wt_ਸਥਿਤੀ_collect_changes_trees(&s, old_ਰੁੱਖ, new_ਰੁੱਖ);
	wt_ਸਥਿਤੀ_print(&s);
	wt_ਸਥਿਤੀ_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_ਅੰਕੜੇ_free);
	ਜੇ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), ਰਸਤਾ);

	strbuf_reset(ਬਾਹਰਲਾ);
	ਜੇ (launch_editor(ਰਸਤਾ, ਬਾਹਰਲਾ, ਨਿਰਾ)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		ਵਾਪਸ -1;
	}
	strbuf_stripspace(ਬਾਹਰਲਾ, comment_ਸਤਰ_str);

	cleanup_ਸੁਨੇਹਾ(ਬਾਹਰਲਾ, COMMIT_ਸੁਨੇਹਾ_CLEANUP_ALL, 0);

	ਜੇ (!ਬਾਹਰਲਾ->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ਵਾਪਸ -1;
	}

	ਵਾਪਸ 0;
}

ਗਿਣਤੀ commit_ਰੁੱਖ_flags {
	COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ = (1 << 0),
};

ਸਥਿਰ ਪੂਰਨ commit_ਰੁੱਖ_ext(ਬਣਤਰ repository *repo,
			   ਅਚਲ ਅੱਖਰ *action,
			   ਬਣਤਰ commit *commit_with_ਸੁਨੇਹਾ,
			   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_template,
			   ਅਚਲ ਬਣਤਰ commit_list *parents,
			   ਅਚਲ ਬਣਤਰ object_id *old_ਰੁੱਖ,
			   ਅਚਲ ਬਣਤਰ object_id *new_ਰੁੱਖ,
			   ਬਣਤਰ commit **ਬਾਹਰਲਾ,
			   ਗਿਣਤੀ commit_ਰੁੱਖ_flags flags)
{
	ਅਚਲ ਅੱਖਰ *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ਨਿਰਾ,
	};
	ਅਚਲ ਅੱਖਰ *original_ਸੁਨੇਹਾ, *original_body, *ptr;
	ਬਣਤਰ commit_extra_ਸਿਰਲੇਖ *original_extra_ਸਿਰਲੇਖਾਂ = ਨਿਰਾ;
	ਬਣਤਰ strbuf commit_ਸੁਨੇਹਾ_2 = STRBUF_INIT;
	ਬਣਤਰ object_id rewritten_commit_oid;
	ਅੱਖਰ *original_author = ਨਿਰਾ;
	ਮਾਪ_t len;
	ਪੂਰਨ ret;

	/* We retain authorship of the original commit. */
	original_ਸੁਨੇਹਾ = repo_logmsg_reencode(repo, commit_with_ਸੁਨੇਹਾ, ਨਿਰਾ, ਨਿਰਾ);
	ptr = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(original_ਸੁਨੇਹਾ, "author", &len);
	ਜੇ (ptr)
		original_author = xmemdupz(ptr, len);
	ਲੱਭੋ_commit_subject(original_ਸੁਨੇਹਾ, &original_body);

	ਜੇ (!ਸੁਨੇਹਾ_template)
		ਸੁਨੇਹਾ_template = original_body;

	ਜੇ (flags & COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ) {
		ret = fill_commit_ਸੁਨੇਹਾ(repo, old_ਰੁੱਖ, new_ਰੁੱਖ,
					  ਸੁਨੇਹਾ_template, action, &commit_ਸੁਨੇਹਾ_2);
		ਜੇ (ret < 0)
			ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(&commit_ਸੁਨੇਹਾ_2, ਸੁਨੇਹਾ_template);
	}

	original_extra_ਸਿਰਲੇਖਾਂ = read_commit_extra_ਸਿਰਲੇਖਾਂ(commit_with_ਸੁਨੇਹਾ,
							   exclude_gpgsig);

	ret = commit_ਰੁੱਖ_extended(commit_ਸੁਨੇਹਾ_2.buf, commit_ਸੁਨੇਹਾ_2.len, new_ਰੁੱਖ,
				   parents, &rewritten_commit_oid, original_author,
				   ਨਿਰਾ, ਨਿਰਾ, original_extra_ਸਿਰਲੇਖਾਂ);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	*ਬਾਹਰਲਾ = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

ਬਾਹਰਲਾ:
	free_commit_extra_ਸਿਰਲੇਖਾਂ(original_extra_ਸਿਰਲੇਖਾਂ);
	strbuf_release(&commit_ਸੁਨੇਹਾ_2);
	ਮੁਕਤ(original_author);
	repo_unuse_commit_ਬਫਰ(repo, commit_with_ਸੁਨੇਹਾ, original_ਸੁਨੇਹਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਪਹਿਲਾ_ਮਾਪਾ_ਰੁੱਖ_oid(ਬਣਤਰ repository *repo,
				 ਬਣਤਰ commit *commit,
				 ਬਣਤਰ object_id *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ commit *ਮਾਪਾ = commit->parents ? commit->parents->ਵਸਤੂ : ਨਿਰਾ;

	ਜੇ (!ਮਾਪਾ) {
		oidcpy(ਬਾਹਰਲਾ, repo->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);
		ਵਾਪਸ 0;
	}

	ਜੇ (repo_parse_commit(repo, ਮਾਪਾ))
		ਵਾਪਸ error(_("unable to parse parent commit %s"),
			     oid_ਮੰਜ਼ਿਲ_hex(&ਮਾਪਾ->object.oid));

	oidcpy(ਬਾਹਰਲਾ, &repo_get_commit_ਰੁੱਖ(repo, ਮਾਪਾ)->object.oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ commit_ਰੁੱਖ_with_edited_ਸੁਨੇਹਾ(ਬਣਤਰ repository *repo,
					   ਅਚਲ ਅੱਖਰ *action,
					   ਬਣਤਰ commit *original,
					   ਬਣਤਰ commit **ਬਾਹਰਲਾ)
{
	ਬਣਤਰ object_id ਮਾਪਾ_ਰੁੱਖ_oid;
	ਅਚਲ ਬਣਤਰ object_id *ਰੁੱਖ_oid;

	ਰੁੱਖ_oid = &repo_get_commit_ਰੁੱਖ(repo, original)->object.oid;

	ਜੇ (ਪਹਿਲਾ_ਮਾਪਾ_ਰੁੱਖ_oid(repo, original, &ਮਾਪਾ_ਰੁੱਖ_oid) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ commit_ਰੁੱਖ_ext(repo, action, original, ਨਿਰਾ, original->parents,
			       &ਮਾਪਾ_ਰੁੱਖ_oid, ਰੁੱਖ_oid, ਬਾਹਰਲਾ, COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ);
}

ਗਿਣਤੀ ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_ਸਿਰ,
};

ਸਥਿਰ ਪੂਰਨ parse_ref_action(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ ref_action *action = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG_NOARG(unset, ਮੁੱਲ);
	ਜੇ (!strcmp(ਮੁੱਲ, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਮੁੱਲ, "head")) {
		*action = REF_ACTION_ਸਿਰ;
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ revwalk_ਸ਼ਾਮਲ_ਹੈ_merges(ਬਣਤਰ repository *repo,
				   ਅਚਲ ਬਣਤਰ strvec *revwalk_ਦਲੀਲਾਂ)
{
	ਬਣਤਰ strvec ਦਲੀਲਾਂ = STRVEC_INIT;
	ਬਣਤਰ rev_info revs;
	ਪੂਰਨ ret;

	strvec_pushv(&ਦਲੀਲਾਂ, revwalk_ਦਲੀਲਾਂ->v);
	strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--min-parents=2");

	repo_init_revisions(repo, &revs, ਨਿਰਾ);

	ਤਿਆਰੀ_revisions_from_strvec(&ਦਲੀਲਾਂ, &revs, ਨਿਰਾ);
	ਜੇ (ਦਲੀਲਾਂ.nr != 1)
		BUG("revisions were set up with invalid argument");

	ਜੇ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	reset_revision_walk();
	ret = 0;

ਬਾਹਰਲਾ:
	release_revisions(&revs);
	strvec_clear(&ਦਲੀਲਾਂ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਤਿਆਰੀ_revwalk(ਬਣਤਰ repository *repo,
			 ਗਿਣਤੀ ref_action action,
			 ਬਣਤਰ commit *original,
			 ਬਣਤਰ rev_info *revs)
{
	ਬਣਤਰ strvec ਦਲੀਲਾਂ = STRVEC_INIT;
	ਪੂਰਨ ret;

	repo_init_revisions(repo, revs, ਨਿਰਾ);
	strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "ignored");
	strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--reverse");
	strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--topo-order");
	strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&ਦਲੀਲਾਂ, "--ancestry-path=%s",
		     oid_ਮੰਜ਼ਿਲ_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&ਦਲੀਲਾਂ, "^%s", oid_ਮੰਜ਼ਿਲ_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	ਜੇ (action == REF_ACTION_ਸਿਰ) {
		ਬਣਤਰ commit_list *from_list = ਨਿਰਾ;
		ਬਣਤਰ commit *ਸਿਰ;

		ਸਿਰ = lookup_commit_reference_by_name("HEAD");
		ਜੇ (!ਸਿਰ) {
			ret = error(_("cannot look up HEAD"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		commit_list_ਪਾਓ(original, &from_list);
		ret = repo_is_descendant_ਦਾ(repo, ਸਿਰ, from_list);
		commit_list_free(from_list);

		ਜੇ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			ਜਾਓ ਬਾਹਰਲਾ;
		} ਨਹੀਂਤਾਂ ਜੇ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "HEAD");
	} ਨਹੀਂਤਾਂ {
		strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--branches");
		strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "HEAD");
	}

	ret = revwalk_ਸ਼ਾਮਲ_ਹੈ_merges(repo, &ਦਲੀਲਾਂ);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਤਿਆਰੀ_revisions_from_strvec(&ਦਲੀਲਾਂ, revs, ਨਿਰਾ);
	ਜੇ (ਦਲੀਲਾਂ.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

ਬਾਹਰਲਾ:
	strvec_clear(&ਦਲੀਲਾਂ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ handle_ref_update(ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *refname,
			     ਅਚਲ ਬਣਤਰ object_id *new_oid,
			     ਅਚਲ ਬਣਤਰ object_id *old_oid,
			     ਅਚਲ ਅੱਖਰ *reflog_ਸੁਨੇਹਾ,
			     ਬਣਤਰ strbuf *err)
{
	ਜੇ (!transaction) {
		ਲਿਖੋ("update %s %s %s\n",
		       refname, oid_ਮੰਜ਼ਿਲ_hex(new_oid), oid_ਮੰਜ਼ਿਲ_hex(old_oid));
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ਨਿਰਾ, ਨਿਰਾ, 0, reflog_ਸੁਨੇਹਾ, err);
}

ਸਥਿਰ ਪੂਰਨ compute_pending_ref_updates(ਬਣਤਰ rev_info *revs,
				       ਗਿਣਤੀ ref_action action,
				       ਬਣਤਰ commit *original,
				       ਬਣਤਰ commit *rewritten,
				       ਗਿਣਤੀ replay_ਖਾਲੀ_ਥਾਵਾਂ_commit_action ਖਾਲੀ_ਥਾਵਾਂ,
				       ਬਣਤਰ replay_ਨਤੀਜਾ *ਨਤੀਜਾ)
{
	ਅਚਲ ਬਣਤਰ name_decoration *decoration;
	ਬਣਤਰ replay_revisions_options opts = {
		.ਖਾਲੀ_ਥਾਵਾਂ = ਖਾਲੀ_ਥਾਵਾਂ,
	};
	ਅੱਖਰ hex[GIT_MAX_HEXSZ + 1];
	bool detached_ਸਿਰ;
	ਪੂਰਨ ਸਿਰ_flags = 0;
	ਪੂਰਨ ret;

	refs_read_ref_ਭਰਿਆ(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ਨਿਰਾ, &ਸਿਰ_flags);
	detached_ਸਿਰ = !(ਸਿਰ_flags & REF_ISSYMREF);

	opts.onto = oid_ਮੰਜ਼ਿਲ_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, ਨਤੀਜਾ);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਜੇ (action != REF_ACTION_BRANCHES && action != REF_ACTION_ਸਿਰ)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	ਲਈ (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->ਅਗਲਾ)
	{
		ਜੇ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_ਸਿਰ)
			ਜਾਰੀ;

		ਜੇ (action == REF_ACTION_ਸਿਰ &&
		    decoration->type != DECORATION_REF_ਸਿਰ)
			ਜਾਰੀ;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		ਜੇ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_ਸਿਰ &&
		    !detached_ਸਿਰ)
			ਜਾਰੀ;

		replay_ਨਤੀਜਾ_ਕਤਾਰ_update(ਨਤੀਜਾ, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ apply_pending_ref_updates(ਬਣਤਰ repository *repo,
				     ਅਚਲ ਬਣਤਰ replay_ਨਤੀਜਾ *ਨਤੀਜਾ,
				     ਅਚਲ ਅੱਖਰ *reflog_ਸੁਨੇਹਾ,
				     ਪੂਰਨ dry_ਲੜੀਵਾਰ)
{
	ਬਣਤਰ ref_transaction *transaction = ਨਿਰਾ;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਪੂਰਨ ret;

	ਜੇ (!dry_ਲੜੀਵਾਰ) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		ਜੇ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ਲਈ (ਮਾਪ_t i = 0; i < ਨਤੀਜਾ->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					ਨਤੀਜਾ->updates[i].refname,
					&ਨਤੀਜਾ->updates[i].new_oid,
					&ਨਤੀਜਾ->updates[i].old_oid,
					reflog_ਸੁਨੇਹਾ, &err);
		ਜੇ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    ਨਤੀਜਾ->updates[i].refname, err.buf);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ਜੇ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ handle_reference_updates(ਬਣਤਰ rev_info *revs,
				    ਗਿਣਤੀ ref_action action,
				    ਬਣਤਰ commit *original,
				    ਬਣਤਰ commit *rewritten,
				    ਅਚਲ ਅੱਖਰ *reflog_ਸੁਨੇਹਾ,
				    ਪੂਰਨ dry_ਲੜੀਵਾਰ,
				    ਗਿਣਤੀ replay_ਖਾਲੀ_ਥਾਵਾਂ_commit_action ਖਾਲੀ_ਥਾਵਾਂ)
{
	ਬਣਤਰ replay_ਨਤੀਜਾ ਨਤੀਜਾ = { 0 };
	ਪੂਰਨ ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  ਖਾਲੀ_ਥਾਵਾਂ, &ਨਤੀਜਾ);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ret = apply_pending_ref_updates(revs->repo, &ਨਤੀਜਾ, reflog_ਸੁਨੇਹਾ, dry_ਲੜੀਵਾਰ);

ਬਾਹਰਲਾ:
	replay_ਨਤੀਜਾ_release(&ਨਤੀਜਾ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ commit_became_ਖਾਲੀ_ਥਾਵਾਂ(ਬਣਤਰ repository *repo,
			       ਬਣਤਰ commit *original,
			       ਬਣਤਰ ਰੁੱਖ *ਨਤੀਜਾ)
{
	ਬਣਤਰ object_id ਮਾਪਾ_ਰੁੱਖ_oid;

	ਜੇ (ਪਹਿਲਾ_ਮਾਪਾ_ਰੁੱਖ_oid(repo, original, &ਮਾਪਾ_ਰੁੱਖ_oid) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ oideq(&ਨਤੀਜਾ->object.oid, &ਮਾਪਾ_ਰੁੱਖ_oid);
}

ਸਥਿਰ ਪੂਰਨ parse_opt_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਗਿਣਤੀ replay_ਖਾਲੀ_ਥਾਵਾਂ_commit_action *ਮੁੱਲ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!strcmp(arg, "drop"))
		*ਮੁੱਲ = REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_DROP;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "keep"))
		*ਮੁੱਲ = REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_KEEP;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "abort"))
		*ਮੁੱਲ = REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_ABORT;
	ਨਹੀਂਤਾਂ
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_history_fixup(ਪੂਰਨ argc,
			     ਅਚਲ ਅੱਖਰ **argv,
			     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ਨਿਰਾ,
	};
	ਗਿਣਤੀ replay_ਖਾਲੀ_ਥਾਵਾਂ_commit_action ਖਾਲੀ_ਥਾਵਾਂ = REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_DROP;
	ਗਿਣਤੀ ref_action action = REF_ACTION_DEFAULT;
	ਗਿਣਤੀ commit_ਰੁੱਖ_flags flags = 0;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ਲੜੀਵਾਰ,
			 N_("perform a dry-run without updating any refs")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ),
		OPT_CALLBACK_F(0, "empty", &ਖਾਲੀ_ਥਾਵਾਂ, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ਖਾਲੀ_ਥਾਵਾਂ),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ ਮਿਲਾਓ_ਨਤੀਜਾ ਮਿਲਾਓ_ਨਤੀਜਾ = { 0 };
	ਬਣਤਰ ਮਿਲਾਓ_options ਮਿਲਾਓ_opts = { 0 };
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ commit *ਸਿਰ_commit, *original, *rewritten;
	ਬਣਤਰ ਰੁੱਖ *ਸਿਰ_ਰੁੱਖ, *original_ਰੁੱਖ, *ਸੂਚਕਾਂਕ_ਰੁੱਖ;
	ਬਣਤਰ rev_info revs = { 0 };
	bool skip_commit = false;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਜੇ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	ਜੇ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	ਜੇ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	ਸਿਰ_commit = lookup_commit_reference_by_name("HEAD");
	ਜੇ (!ਸਿਰ_commit) {
		ret = error(_("cannot look up HEAD"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਸਿਰ_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(repo, ਸਿਰ_commit);
	ਜੇ (!ਸਿਰ_ਰੁੱਖ) {
		ret = error(_("cannot get tree for HEAD"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0) {
		ret = error(_("unable to read index"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!repo_ਸੂਚਕਾਂਕ_has_changes(repo, ਸਿਰ_ਰੁੱਖ, ਨਿਰਾ)) {
		ret = error(_("nothing to fixup: no staged changes"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	ਸੂਚਕਾਂਕ_ਰੁੱਖ = write_ਵਿੱਚ_core_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(repo, repo->ਸੂਚਕਾਂਕ);
	ਜੇ (!ਸੂਚਕਾਂਕ_ਰੁੱਖ) {
		ret = error(_("unable to write index as a tree"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	original_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(repo, original);
	ਜੇ (!original_ਰੁੱਖ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_ਮਿਲਾਓ_options(&ਮਿਲਾਓ_opts, repo);
	ਮਿਲਾਓ_opts.ancestor = "HEAD";
	ਮਿਲਾਓ_opts.branch1 = argv[0];
	ਮਿਲਾਓ_opts.branch2 = "staged";
	ਮਿਲਾਓ_incore_nonrecursive(&ਮਿਲਾਓ_opts, ਸਿਰ_ਰੁੱਖ,
				  original_ਰੁੱਖ, ਸੂਚਕਾਂਕ_ਰੁੱਖ, &ਮਿਲਾਓ_ਨਤੀਜਾ);

	ਜੇ (ਮਿਲਾਓ_ਨਤੀਜਾ.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!ਮਿਲਾਓ_ਨਤੀਜਾ.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = commit_became_ਖਾਲੀ_ਥਾਵਾਂ(repo, original, ਮਿਲਾਓ_ਨਤੀਜਾ.ਰੁੱਖ);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;
	ਜੇ (ret > 0) {
		ਚੋਣ (ਖਾਲੀ_ਥਾਵਾਂ) {
		ਹਾਲਤ REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->ਵਸਤੂ : ਨਿਰਾ;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			ਜੇ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				ਜਾਓ ਬਾਹਰਲਾ;
			}

			skip_commit = true;
			ਰੋਕੋ;
		ਹਾਲਤ REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			ਰੋਕੋ;
		ਹਾਲਤ REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ret = ਤਿਆਰੀ_revwalk(repo, action, original, &revs);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!skip_commit) {
		ret = commit_ਰੁੱਖ_ext(repo, "fixup", original, ਨਿਰਾ, original->parents,
				      &original_ਰੁੱਖ->object.oid, &ਮਿਲਾਓ_ਨਤੀਜਾ.ਰੁੱਖ->object.oid,
				      &rewritten, flags);
		ਜੇ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	strbuf_addf(&reflog_ਸੁਨੇਹਾ, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ਸੁਨੇਹਾ.buf, dry_ਲੜੀਵਾਰ, ਖਾਲੀ_ਥਾਵਾਂ);
	ਜੇ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	ਮਿਲਾਓ_finalize(&ਮਿਲਾਓ_opts, &ਮਿਲਾਓ_ਨਤੀਜਾ);
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	release_revisions(&revs);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_history_reword(ਪੂਰਨ argc,
			      ਅਚਲ ਅੱਖਰ **argv,
			      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			      ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ਨਿਰਾ,
	};
	ਗਿਣਤੀ ref_action action = REF_ACTION_DEFAULT;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ਲੜੀਵਾਰ,
			 N_("perform a dry-run without updating any refs")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ commit *original, *rewritten;
	ਬਣਤਰ rev_info revs = { 0 };
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਜੇ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ਜੇ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = ਤਿਆਰੀ_revwalk(repo, action, original, &revs);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ret = commit_ਰੁੱਖ_with_edited_ਸੁਨੇਹਾ(repo, "reworded", original, &rewritten);
	ਜੇ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	strbuf_addf(&reflog_ਸੁਨੇਹਾ, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ਸੁਨੇਹਾ.buf, dry_ਲੜੀਵਾਰ, REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_ABORT);
	ਜੇ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	release_revisions(&revs);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ write_ondisk_ਸੂਚਕਾਂਕ(ਬਣਤਰ repository *repo,
			      ਬਣਤਰ object_id *oid,
			      ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ unpack_trees_options opts = { 0 };
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ = ਤਾਲਾ_INIT;
	ਬਣਤਰ ਰੁੱਖ_desc ਰੁੱਖ_desc;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state ਸੂਚਕਾਂਕ;
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਪੂਰਨ ret;

	ਸੂਚਕਾਂਕ_state_init(&ਸੂਚਕਾਂਕ, repo);

	opts.ਸਿਰ_idx = -1;
	opts.src_ਸੂਚਕਾਂਕ = &ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = &ਸੂਚਕਾਂਕ;

	ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(repo, oid);
	ਜੇ (!ਰੁੱਖ) {
		ret = error(_("unable to parse tree %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	init_ਰੁੱਖ_desc(&ਰੁੱਖ_desc, &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);

	ਜੇ (unpack_trees(1, &ਰੁੱਖ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਅਭਾਜ_cache_ਰੁੱਖ(repo, &ਸੂਚਕਾਂਕ, ਰੁੱਖ);

	ਜੇ (repo_hold_ਤਾਲਾ_file_for_update(repo, &ਤਾਲਾ, ਰਸਤਾ, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(&ਸੂਚਕਾਂਕ, &ਤਾਲਾ, COMMIT_ਤਾਲਾ)) {
		ret = error(_("unable to write new index file"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	rollback_ਤਾਲਾ_file(&ਤਾਲਾ);
	release_ਸੂਚਕਾਂਕ(&ਸੂਚਕਾਂਕ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_commit(ਬਣਤਰ repository *repo,
			ਬਣਤਰ commit *original,
			ਬਣਤਰ pathspec *pathspec,
			ਬਣਤਰ commit **ਬਾਹਰਲਾ)
{
	ਬਣਤਰ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ਬਣਤਰ strbuf ਸੂਚਕਾਂਕ_file = STRBUF_INIT;
	ਬਣਤਰ ਸੂਚਕਾਂਕ_state ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_STATE_INIT(repo);
	ਅਚਲ ਬਣਤਰ object_id *original_commit_ਰੁੱਖ_oid;
	ਅਚਲ ਬਣਤਰ object_id *old_ਰੁੱਖ_oid, *new_ਰੁੱਖ_oid;
	ਬਣਤਰ object_id ਮਾਪਾ_ਰੁੱਖ_oid;
	ਅੱਖਰ original_commit_oid[GIT_MAX_HEXSZ + 1];
	ਬਣਤਰ commit *ਪਹਿਲਾ_commit, *ਦੂਜਾ_commit;
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
	ਬਣਤਰ ਰੁੱਖ *ਵੰਡੋ_ਰੁੱਖ;
	ਪੂਰਨ ret;

	ਜੇ (ਪਹਿਲਾ_ਮਾਪਾ_ਰੁੱਖ_oid(repo, original, &ਮਾਪਾ_ਰੁੱਖ_oid) < 0) {
		ret = -1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	original_commit_ਰੁੱਖ_oid = get_commit_ਰੁੱਖ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_ਰਸਤਾ_replace(repo, &ਸੂਚਕਾਂਕ_file, "%s", "history-split.index");

	ret = write_ondisk_ਸੂਚਕਾਂਕ(repo, &ਮਾਪਾ_ਰੁੱਖ_oid, ਸੂਚਕਾਂਕ_file.buf);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ret = read_ਸੂਚਕਾਂਕ_from(&ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_file.buf, repo->gitdir);
	ਜੇ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	oid_ਮੰਜ਼ਿਲ_hex_r(original_commit_oid, &original->object.oid);
	ret = ਲੜੀਵਾਰ_ਜੋੜੋ_p_ਸੂਚਕਾਂਕ(repo, &ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ਜੋੜੋ_P_DISALLOW_EDIT);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਵੰਡੋ_ਰੁੱਖ = write_ਵਿੱਚ_core_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(repo, &ਸੂਚਕਾਂਕ);
	ਜੇ (!ਵੰਡੋ_ਰੁੱਖ) {
		ret = error(_("failed split tree"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	unlink(ਸੂਚਕਾਂਕ_file.buf);
	strbuf_release(&ਸੂਚਕਾਂਕ_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	ਜੇ (oideq(&ਵੰਡੋ_ਰੁੱਖ->object.oid, &ਮਾਪਾ_ਰੁੱਖ_oid)) {
		ret = error(_("split commit is empty"));
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (oideq(&ਵੰਡੋ_ਰੁੱਖ->object.oid, original_commit_ਰੁੱਖ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_ਰੁੱਖ_ext(repo, "split-out", original, ਨਿਰਾ, original->parents, &ਮਾਪਾ_ਰੁੱਖ_oid,
			      &ਵੰਡੋ_ਰੁੱਖ->object.oid, &ਪਹਿਲਾ_commit, COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ);
	ਜੇ (ret < 0) {
		ret = error(_("failed writing first commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(ਪਹਿਲਾ_commit, &parents);

	old_ਰੁੱਖ_oid = &repo_get_commit_ਰੁੱਖ(repo, ਪਹਿਲਾ_commit)->object.oid;
	new_ਰੁੱਖ_oid = &repo_get_commit_ਰੁੱਖ(repo, original)->object.oid;

	ret = commit_ਰੁੱਖ_ext(repo, "split-out", original, ਨਿਰਾ, parents, old_ਰੁੱਖ_oid,
			      new_ਰੁੱਖ_oid, &ਦੂਜਾ_commit, COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ);
	ਜੇ (ret < 0) {
		ret = error(_("failed writing second commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	*ਬਾਹਰਲਾ = ਦੂਜਾ_commit;
	ret = 0;

ਬਾਹਰਲਾ:
	ਜੇ (ਸੂਚਕਾਂਕ_file.len)
		unlink(ਸੂਚਕਾਂਕ_file.buf);
	strbuf_release(&ਸੂਚਕਾਂਕ_file);
	commit_list_free(parents);
	release_ਸੂਚਕਾਂਕ(&ਸੂਚਕਾਂਕ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_history_ਵੰਡੋ(ਪੂਰਨ argc,
			     ਅਚਲ ਅੱਖਰ **argv,
			     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		GIT_HISTORY_ਵੰਡੋ_USAGE,
		ਨਿਰਾ,
	};
	ਗਿਣਤੀ ref_action action = REF_ACTION_DEFAULT;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ਲੜੀਵਾਰ,
			 N_("perform a dry-run without updating any refs")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ commit *original, *rewritten = ਨਿਰਾ;
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ pathspec pathspec = { 0 };
	ਬਣਤਰ rev_info revs = { 0 };
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਜੇ (argc < 1) {
		ret = error(_("command expects a committish"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ਭਰਿਆ |
		       PATHSPEC_SYMLINK_LEADING_ਰਸਤਾ |
		       PATHSPEC_ਅਗੇਤਰ_ORIGIN,
		       ਅਗੇਤਰ, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	ਜੇ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = ਤਿਆਰੀ_revwalk(repo, action, original, &revs);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (original->parents && original->parents->ਅਗਲਾ) {
		ret = error(_("cannot split up merge commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = ਵੰਡੋ_commit(repo, original, &pathspec, &rewritten);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	strbuf_addf(&reflog_ਸੁਨੇਹਾ, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ਸੁਨੇਹਾ.buf, dry_ਲੜੀਵਾਰ, REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_ABORT);
	ਜੇ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	ਵਾਪਸ ret;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SQUASH_ਵੇਖਿਆ (1u << 11)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SQUASH_TIP (1u << 12)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SQUASH_AMEND_ਨਿਸ਼ਾਨਾ (1u << 13)

ਸਥਿਰ bool is_autosquash_subject(ਅਚਲ ਅੱਖਰ *s)
{
	ਵਾਪਸ starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ਸਥਿਰ bool skip_one_autosquash_ਅਗੇਤਰ(ਅਚਲ ਅੱਖਰ *s, ਅਚਲ ਅੱਖਰ **ਬਾਹਰਲਾ)
{
	ਜੇ (skip_ਅਗੇਤਰ(s, "amend!", ਬਾਹਰਲਾ) || skip_ਅਗੇਤਰ(s, "fixup!", ਬਾਹਰਲਾ) ||
	    skip_ਅਗੇਤਰ(s, "squash!", ਬਾਹਰਲਾ)) {
		ਜਦੋਂ (**ਬਾਹਰਲਾ == ' ')
			(*ਬਾਹਰਲਾ)++;
		ਵਾਪਸ true;
	}
	ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ truncate_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_subject(ਬਣਤਰ strbuf *ਸੁਨੇਹਾ)
{
	ਅਚਲ ਅੱਖਰ *eos = ਲੜੀ_ਖੋਜ(ਸੁਨੇਹਾ->buf, "\n\n");

	ਜੇ (eos)
		strbuf_setlen(ਸੁਨੇਹਾ, eos - ਸੁਨੇਹਾ->buf + 1);
}

ਬਣਤਰ subject_ਅੰਕੜੇ {
	ਬਣਤਰ strintmap subjects;
	ਬਣਤਰ strbuf subject;
	ਬਣਤਰ strbuf squash_ਸੁਨੇਹਾ;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2;
	bool edit_ਸੁਨੇਹਾ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SUBJECT_ਅੰਕੜੇ_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_ਸੁਨੇਹਾ = STRBUF_INIT,	\
}

ਸਥਿਰ ਖਾਲੀ subject_ਅੰਕੜੇ_clear(ਬਣਤਰ subject_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	strintmap_clear(&ਅੰਕੜੇ->subjects);
	strbuf_release(&ਅੰਕੜੇ->subject);
	strbuf_release(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
}

ਸਥਿਰ ਪੂਰਨ squash_amend_ਸੁਨੇਹਾ(ਬਣਤਰ repository *repo,
				ਬਣਤਰ commit *commit,
				ਬਣਤਰ subject_ਅੰਕੜੇ *ਅੰਕੜੇ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਅਚਲ ਅੱਖਰ *body = ਅੰਕੜੇ->ਸੁਨੇਹਾ_2 + ਅੰਕੜੇ->subject.len;

	ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*body))
		body++;

	ਜੇ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_ਲੱਭੋ_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			ਅੰਕੜੇ->subject.buf);
		ਵਾਪਸ 0;
	}

	ਜੇ (ਅੰਕੜੇ->edit_ਸੁਨੇਹਾ) {
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (flags & SQUASH_AMEND_ਨਿਸ਼ਾਨਾ) {
		ਜੇ (starts_with(ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ.buf, "squash!"))
			ਵਾਪਸ error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_ਲੱਭੋ_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     ਅੰਕੜੇ->subject.buf);
		ਜੇ (starts_with(ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ.buf, "fixup!"))
			strbuf_splice(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, 0, 5, "amend!", 5);
		ਜੇ (starts_with(ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ.buf, "amend!")) {
			truncate_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_subject(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
			strbuf_addch(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, '\n');
		} ਨਹੀਂਤਾਂ {
			strbuf_reset(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
		}
		strbuf_addstr(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, body);
		strbuf_complete_ਸਤਰ(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_ਲੱਭੋ_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     ਅੰਕੜੇ->subject.buf);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ squash_squash_ਸੁਨੇਹਾ(ਬਣਤਰ repository *repo,
				ਬਣਤਰ commit *commit,
				ਬਣਤਰ subject_ਅੰਕੜੇ *ਅੰਕੜੇ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਅਚਲ ਅੱਖਰ *body = ਅੰਕੜੇ->ਸੁਨੇਹਾ_2 + ਅੰਕੜੇ->subject.len;

	ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*body))
		body++;

	ਜੇ (ਅੰਕੜੇ->edit_ਸੁਨੇਹਾ) {
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (flags & SQUASH_AMEND_ਨਿਸ਼ਾਨਾ) {
		ਜੇ (starts_with(ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ.buf, "fixup!")) {
			truncate_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_subject(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
			strbuf_splice(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, 0, 5, "squash", 6);
		}
		ਜੇ (starts_with(ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ.buf, "squash!")) {
			strbuf_addch(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, '\n');
			strbuf_addstr(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, body);
			strbuf_complete_ਸਤਰ(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
		} ਨਹੀਂਤਾਂ {
			ਵਾਪਸ error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_ਲੱਭੋ_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     ਅੰਕੜੇ->subject.buf);
		}
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_ਲੱਭੋ_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      ਅੰਕੜੇ->subject.buf);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ squash_check_can_autosquash(ਬਣਤਰ repository *repo,
				       ਬਣਤਰ commit *commit,
				       ਬਣਤਰ subject_ਅੰਕੜੇ *ਅੰਕੜੇ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_ਨਿਸ਼ਾਨਾ;
	ਜੇ (starts_with(ਅੰਕੜੇ->subject.buf, "amend!"))
		ਵਾਪਸ squash_amend_ਸੁਨੇਹਾ(repo, commit, ਅੰਕੜੇ, flags);
	ਨਹੀਂਤਾਂ ਜੇ (starts_with(ਅੰਕੜੇ->subject.buf, "squash!"))
		ਵਾਪਸ squash_squash_ਸੁਨੇਹਾ(repo, commit, ਅੰਕੜੇ, flags);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ squash_check_autosquash_subject(ਬਣਤਰ repository *repo,
					   ਬਣਤਰ commit *commit,
					   ਬਣਤਰ subject_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਅਚਲ ਅੱਖਰ* s = ਅੰਕੜੇ->subject.buf;
	ਬਣਤਰ commit *ਨਿਸ਼ਾਨਾ;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ strmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
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
	ਜਦੋਂ (skip_one_autosquash_ਅਗੇਤਰ(s, &s)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ flags = strintmap_get(&ਅੰਕੜੇ->subjects, s);
		ਜੇ (flags)
			ਵਾਪਸ squash_check_can_autosquash(repo, commit, ਅੰਕੜੇ, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	ਨਿਸ਼ਾਨਾ = lookup_commit_reference_by_name(s);
	ਜੇ (ਨਿਸ਼ਾਨਾ && starts_with(oid_ਮੰਜ਼ਿਲ_hex(&ਨਿਸ਼ਾਨਾ->object.oid), s)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ flags =
			ਨਿਸ਼ਾਨਾ->object.flags & (SQUASH_ਵੇਖਿਆ | SQUASH_AMEND_ਨਿਸ਼ਾਨਾ);
		ਜੇ (!flags) {
			ਅਚਲ ਅੱਖਰ *subject_ਸ਼ੁਰੂ;
			ਅਚਲ ਅੱਖਰ *ਬਫਰ = repo_logmsg_reencode(repo, ਨਿਸ਼ਾਨਾ,
								  ਨਿਰਾ, ਨਿਰਾ);
			ਮਾਪ_t subject_len = ਲੱਭੋ_commit_subject(ਬਫਰ,
								 &subject_ਸ਼ੁਰੂ);
			ਅੱਖਰ *subject = xmemdupz(subject_ਸ਼ੁਰੂ, subject_len);

			flags = strintmap_get(&ਅੰਕੜੇ->subjects, subject);
			ਮੁਕਤ(subject);
			repo_unuse_commit_ਬਫਰ(repo, ਨਿਸ਼ਾਨਾ, ਬਫਰ);
		}
		ਜੇ (flags)
			ਵਾਪਸ squash_check_can_autosquash(repo, commit,
							   ਅੰਕੜੇ, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_ਇੰਦਰਾਜ(&ਅੰਕੜੇ->subjects, &iter, ਇੰਦਰਾਜ) {
		s = ਅੰਕੜੇ->subject.buf;
		ਜਦੋਂ(skip_one_autosquash_ਅਗੇਤਰ(s, &s)) {
			ਜੇ (starts_with(ਇੰਦਰਾਜ->ਕੁੰਜੀ, s)) {
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ = (intptr_t)ਇੰਦਰਾਜ->ਮੁੱਲ;

				ਵਾਪਸ squash_check_can_autosquash(repo, commit,
								   ਅੰਕੜੇ, ਮੁੱਲ);
			}
		}
	}
	ਵਾਪਸ error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_ਲੱਭੋ_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       ਅੰਕੜੇ->subject.buf);
}

ਸਥਿਰ ਪੂਰਨ squash_check_subject(ਬਣਤਰ repository *repo,
				ਬਣਤਰ commit *commit,
				ਬਣਤਰ subject_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਪੂਰਨ ret = 0;
	ਅਚਲ ਅੱਖਰ *buf = repo_logmsg_reencode(repo, commit, ਨਿਰਾ, ਨਿਰਾ);
	ਮਾਪ_t subject_len = ਲੱਭੋ_commit_subject(buf, &ਅੰਕੜੇ->ਸੁਨੇਹਾ_2);

	strbuf_reset(&ਅੰਕੜੇ->subject);
	strbuf_ਜੋੜੋ(&ਅੰਕੜੇ->subject, ਅੰਕੜੇ->ਸੁਨੇਹਾ_2, subject_len);

	ਜੇ (!strintmap_get_ਮਾਪ(&ਅੰਕੜੇ->subjects)) {
		ਅਚਲ ਅੱਖਰ *s;

		strbuf_addstr(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ, ਅੰਕੜੇ->ਸੁਨੇਹਾ_2);
		strbuf_complete_ਸਤਰ(&ਅੰਕੜੇ->squash_ਸੁਨੇਹਾ);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = ਅੰਕੜੇ->subject.buf;
		skip_one_autosquash_ਅਗੇਤਰ(s, &s);
		strintmap_ਸੈੱਟ(&ਅੰਕੜੇ->subjects, s, SQUASH_AMEND_ਨਿਸ਼ਾਨਾ | SQUASH_ਵੇਖਿਆ);
		commit->object.flags |= SQUASH_AMEND_ਨਿਸ਼ਾਨਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (is_autosquash_subject(ਅੰਕੜੇ->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, ਅੰਕੜੇ);
	} ਨਹੀਂਤਾਂ {
		strintmap_ਸੈੱਟ(&ਅੰਕੜੇ->subjects, ਅੰਕੜੇ->subject.buf, SQUASH_ਵੇਖਿਆ);
	}
	repo_unuse_commit_ਬਫਰ(repo, commit, buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ build_squash_ਸੁਨੇਹਾ(ਬਣਤਰ repository *repo,
				ਅਚਲ ਬਣਤਰ strbuf *todo_buf,
				ਬਣਤਰ strbuf *ਬਾਹਰਲਾ);

ਸਥਿਰ ਪੂਰਨ ਤਿਆਰੀ_squash_revisions(ਬਣਤਰ repository *repo,
				  ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				  ਬਣਤਰ rev_info *revs)
{
	repo_init_revisions(repo, revs, ਨਿਰਾ);
	revs->ਉਲਟ = 1;
	revs->topo_ਕ੍ਰਮ = 1;
	revs->ਛਾਂਟੋ_ਕ੍ਰਮ = REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ;
	revs->simplify_history = 0;
	revs->ancestry_ਰਸਤਾ = 1;
	revs->limited = 1;
	revs->ancestry_ਰਸਤਾ_implicit_bottoms = 1;

	argc = ਤਿਆਰੀ_revisions(argc, argv, revs, ਨਿਰਾ);
	ਜੇ (argc > 1)
		ਵਾਪਸ error(_("unrecognized argument: %s"), argv[1]);

	ਜੇ (revs->ਉਲਟ != 1 || revs->topo_ਕ੍ਰਮ != 1 ||
	    revs->ਛਾਂਟੋ_ਕ੍ਰਮ != REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_ਰਸਤਾ != 1 || revs->limited != 1 ||
	    revs->ancestry_ਰਸਤਾ_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->ਉਲਟ = 1;
		revs->topo_ਕ੍ਰਮ = 1;
		revs->ਛਾਂਟੋ_ਕ੍ਰਮ = REV_ਛਾਂਟੋ_ਵਿੱਚ_ਗ੍ਰਾਫ_ਕ੍ਰਮ;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_ਰਸਤਾ = 1;
		revs->limited = 1;
		revs->ancestry_ਰਸਤਾ_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	ਲਈ (ਮਾਪ_t i = 0; i < revs->cmdline.nr; i++)
		ਜੇ (revs->cmdline.rev[i].flags & BOTTOM)
			ਵਾਪਸ 0;

	ਵਾਪਸ error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ਸਥਿਰ ਪੂਰਨ resolve_squash_range(ਬਣਤਰ repository *repo,
				bool update_branches,
				bool edit_ਸੁਨੇਹਾ,
				ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				ਬਣਤਰ commit **oldest_ਬਾਹਰਲਾ,
				ਬਣਤਰ commit **tip_ਬਾਹਰਲਾ,
				ਅੱਖਰ **ਸੁਨੇਹਾ_ਬਾਹਰਲਾ)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ subject_ਅੰਕੜੇ subject_ਅੰਕੜੇ = SUBJECT_ਅੰਕੜੇ_INIT;
	ਬਣਤਰ commit *commit, *oldest = ਨਿਰਾ, *tip = ਨਿਰਾ;
	ਬਣਤਰ strbuf todo_buf = STRBUF_INIT;
	ਪੂਰਨ ret, tip_ਗਿਣਤੀ = 0;
	bool walk_started = false;
	ਬਣਤਰ ref_filter filter = REF_FILTER_INIT;
	ਬਣਤਰ ref_array refs = { 0 };

	subject_ਅੰਕੜੇ.edit_ਸੁਨੇਹਾ = edit_ਸੁਨੇਹਾ;
	ret = ਤਿਆਰੀ_squash_revisions(repo, argc, argv, &revs);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	walk_started = true;
	ਜਦੋਂ ((commit = get_revision(&revs))) {
		ਬਣਤਰ commit_list *p;

		ਜੇ (edit_ਸੁਨੇਹਾ)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));

		ਜੇ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ਲਈ (p = commit->parents; oldest && p; p = p->ਅਗਲਾ) {
			ਬਣਤਰ commit_list *q;
			ਬਣਤਰ object *o;
			bool ਵੇਖਿਆ;

			ਜੇ (repo_parse_commit(repo, p->ਵਸਤੂ)) {
				ret = error(_("cannot parse commit"));
				ਜਾਓ ਬਾਹਰਲਾ;
			}
			o = &p->ਵਸਤੂ->object;
			ਵੇਖਿਆ = o->flags & SQUASH_ਵੇਖਿਆ;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			ਲਈ (q = oldest->parents; !ਵੇਖਿਆ && q; q = q->ਅਗਲਾ) {
				ਜੇ (p->ਵਸਤੂ == q->ਵਸਤੂ) {
					ਵੇਖਿਆ = true;
					commit_list_ਪਾਓ(commit, &filter.with_commit);
				}
			}
			ਜੇ (!ਵੇਖਿਆ) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_ਲੱਭੋ_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_ਲੱਭੋ_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				ਜਾਓ ਬਾਹਰਲਾ;
			}
			ਜੇ (o->flags & SQUASH_TIP) {
				tip_ਗਿਣਤੀ--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		ਜੇ (!oldest) {
			commit_list_ਪਾਓ(commit, &filter.with_commit);
			oldest = commit;
		}
		ਜੇ (squash_check_subject(repo, commit, &subject_ਅੰਕੜੇ)) {
			ret = -1;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		tip = commit;
		tip->object.flags |= SQUASH_ਵੇਖਿਆ | SQUASH_TIP;
		tip_ਗਿਣਤੀ++;
	}
	clear_object_flags(repo, SQUASH_ਵੇਖਿਆ | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	ਜੇ (!tip_ਗਿਣਤੀ) {
		ret = error(_("the revision range is empty"));
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (tip_ਗਿਣਤੀ != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ਪਾਓ(tip, &filter.no_commit);
	filter.ਕਿਸਮ = FILTER_REFS_BRANCHES;
	ਜੇ (update_branches &&
	    filter_refs(&refs, &filter, filter.ਕਿਸਮ)) {
		ret = error(_("could not filter refs"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	ਜੇ (refs.nr) {
		ਬਣਤਰ ref_format format = REF_FORMAT_INIT;
		ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ;
		ਬਣਤਰ string_list ਛਾਂਟੀ_options = STRING_LIST_INIT_DUP;
		ਬਣਤਰ strbuf branches = STRBUF_INIT;
		ਬਣਤਰ strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		ਜੇ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&ਛਾਂਟੀ_options, "refname");
		ਛਾਂਟੀ = ref_ਛਾਂਟੀ_options(&ਛਾਂਟੀ_options);
		ref_array_ਛਾਂਟੋ(ਛਾਂਟੀ, &refs);
		ਲਈ (ਪੂਰਨ i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			ਜੇ (format_ref_array_ਵਸਤੂ(refs.ਚੀਜ਼ਾਂ[i], &format,
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
		ref_ਛਾਂਟੀ_release(ਛਾਂਟੀ);
		string_list_clear(&ਛਾਂਟੀ_options, 0);
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	ਜੇ (edit_ਸੁਨੇਹਾ) {
		strbuf_reset(&subject_ਅੰਕੜੇ.squash_ਸੁਨੇਹਾ);
		ret = build_squash_ਸੁਨੇਹਾ(repo, &todo_buf,
					   &subject_ਅੰਕੜੇ.squash_ਸੁਨੇਹਾ);
		ਜੇ (ret < 0)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	*oldest_ਬਾਹਰਲਾ = oldest;
	*tip_ਬਾਹਰਲਾ = tip;
	*ਸੁਨੇਹਾ_ਬਾਹਰਲਾ = strbuf_detach(&subject_ਅੰਕੜੇ.squash_ਸੁਨੇਹਾ, ਨਿਰਾ);
	ret = 0;

ਬਾਹਰਲਾ:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_ਵੇਖਿਆ | SQUASH_TIP |
			   SQUASH_AMEND_ਨਿਸ਼ਾਨਾ);
	ਜੇ (walk_started)
		reset_revision_walk();
	subject_ਅੰਕੜੇ_clear(&subject_ਅੰਕੜੇ);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	ਵਾਪਸ ret;
}

ਸਥਿਰ bool amend_replaces_ਨਿਸ਼ਾਨਾ(ਬਣਤਰ todo_list *todo, ਪੂਰਨ ਨਿਸ਼ਾਨਾ)
{
	ਲਈ (ਪੂਰਨ i = ਨਿਸ਼ਾਨਾ + 1; i < todo->nr &&
				 todo->ਚੀਜ਼ਾਂ[i].command != TODO_PICK; i++) {
		ਜੇ (todo->ਚੀਜ਼ਾਂ[i].command == TODO_SQUASH)
			ਵਾਪਸ false;
		ਜੇ (todo->ਚੀਜ਼ਾਂ[i].flags & TODO_REPLACE_FIXUP_ਸੁਨੇਹਾ)
			ਵਾਪਸ true;
	}
	ਵਾਪਸ false;
}

ਸਥਿਰ ਪੂਰਨ build_squash_ਸੁਨੇਹਾ(ਬਣਤਰ repository *repo,
				ਅਚਲ ਬਣਤਰ strbuf *todo_buf,
				ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ todo_list todo = TODO_LIST_INIT;
	ਬਣਤਰ replay_opts opts = REPLAY_OPTS_INIT;
	ਪੂਰਨ nr_commits, ret;

	ਜੇ (todo_list_parse_insn_ਬਫਰ(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	nr_commits = todo.nr;
	ਲਈ (ਪੂਰਨ i = 0; i < nr_commits; i++) {
		ਬਣਤਰ todo_ਵਸਤੂ *ਵਸਤੂ = &todo.ਚੀਜ਼ਾਂ[i];
		ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2, *body;
		ਮਾਪ_t commented_len;
		bool skip, squashing;

		squashing = ਵਸਤੂ->command == TODO_SQUASH ||
			    (ਵਸਤੂ->flags & TODO_REPLACE_FIXUP_ਸੁਨੇਹਾ);
		ਜੇ (ਵਸਤੂ->command == TODO_PICK)
			skip = amend_replaces_ਨਿਸ਼ਾਨਾ(&todo, i);
		ਨਹੀਂਤਾਂ
			skip = !squashing;

		ਸੁਨੇਹਾ_2 = repo_logmsg_reencode(repo, ਵਸਤੂ->commit, ਨਿਰਾ, ਨਿਰਾ);
		ਲੱਭੋ_commit_subject(ਸੁਨੇਹਾ_2, &body);

		ਜੇ (skip)
			commented_len = ਲੜੀ_ਲੰਬਾਈ(body);
		ਨਹੀਂਤਾਂ ਜੇ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		ਨਹੀਂਤਾਂ
			commented_len = 0;

		ਜੇ (!i)
			ਜੋੜੋ_squash_combination_ਸਿਰਲੇਖ(ਬਾਹਰਲਾ, nr_commits);
		strbuf_addch(ਬਾਹਰਲਾ, '\n');
		ਜੋੜੋ_squash_ਸੁਨੇਹਾ_ਸਿਰਲੇਖ(ਬਾਹਰਲਾ, i + 1, skip);
		strbuf_addstr(ਬਾਹਰਲਾ, "\n\n");
		strbuf_ਜੋੜੋ_commented_ਸਤਰਾਂ(ਬਾਹਰਲਾ, body, commented_len, comment_ਸਤਰ_str);
		strbuf_addstr(ਬਾਹਰਲਾ, body + commented_len);
		strbuf_complete_ਸਤਰ(ਬਾਹਰਲਾ);

		repo_unuse_commit_ਬਫਰ(repo, ਵਸਤੂ->commit, ਸੁਨੇਹਾ_2);
	}

	ret = 0;

ਬਾਹਰਲਾ:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_history_squash(ਪੂਰਨ argc,
			      ਅਚਲ ਅੱਖਰ **argv,
			      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			      ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ਨਿਰਾ,
	};
	ਗਿਣਤੀ ref_action action = REF_ACTION_DEFAULT;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0;
	ਪੂਰਨ edit = 1;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ਲੜੀਵਾਰ,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ commit *oldest, *tip, *rewritten;
	ਅਚਲ ਬਣਤਰ object_id *ਅਧਾਰ_ਰੁੱਖ_oid, *tip_ਰੁੱਖ_oid;
	ਅੱਖਰ *ਸੁਨੇਹਾ_template = ਨਿਰਾ;
	ਬਣਤਰ rev_info revs = { 0 };
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	ਜੇ (argc < 2) {
		ret = error(_("command expects a revision range"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_ਸੁਨੇਹਾ, "squash: updating ");
	strbuf_join_argv(&reflog_ਸੁਨੇਹਾ, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &ਸੁਨੇਹਾ_template);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ret = ਤਿਆਰੀ_revwalk(repo, action, tip, &revs);
	ਜੇ (ret < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਅਧਾਰ_ਰੁੱਖ_oid = &repo_get_commit_ਰੁੱਖ(repo,
					oldest->parents->ਵਸਤੂ)->object.oid;
	tip_ਰੁੱਖ_oid = &repo_get_commit_ਰੁੱਖ(repo, tip)->object.oid;

	ret = commit_ਰੁੱਖ_ext(repo, "squash", oldest, ਸੁਨੇਹਾ_template,
			      oldest->parents, ਅਧਾਰ_ਰੁੱਖ_oid, tip_ਰੁੱਖ_oid,
			      &rewritten,
			      edit ? COMMIT_ਰੁੱਖ_EDIT_ਸੁਨੇਹਾ : 0);
	ਜੇ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_ਸੁਨੇਹਾ.buf, dry_ਲੜੀਵਾਰ,
				       REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_ABORT);
	ਜੇ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	release_revisions(&revs);
	ਮੁਕਤ(ਸੁਨੇਹਾ_template);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ update_worktree(ਬਣਤਰ repository *repo,
			   ਅਚਲ ਬਣਤਰ commit *old_ਸਿਰ,
			   ਅਚਲ ਬਣਤਰ commit *new_ਸਿਰ,
			   bool dry_ਲੜੀਵਾਰ)
{
	ਬਣਤਰ reset_working_ਰੁੱਖ_options opts = {
		.oid_from = &old_ਸਿਰ->object.oid,
		.oid = &new_ਸਿਰ->object.oid,
	};
	ਜੇ (dry_ਲੜੀਵਾਰ)
		opts.flags |= RESET_WORKING_ਰੁੱਖ_DRY_ਲੜੀਵਾਰ;
	ਵਾਪਸ reset_working_ਰੁੱਖ(repo, &opts);
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_ਸਿਰ_ਰੁੱਖ_change(ਬਣਤਰ repository *repo,
				 ਅਚਲ ਬਣਤਰ replay_ਨਤੀਜਾ *ਨਤੀਜਾ,
				 ਬਣਤਰ commit **old_ਸਿਰ,
				 ਬਣਤਰ commit **new_ਸਿਰ,
				 bool *ਬਦਲਿਆ)
{
	ਅਚਲ ਬਣਤਰ replay_ref_update *ਸਿਰ_update = ਨਿਰਾ;
	ਬਣਤਰ commit *old_ਸਿਰ_commit, *new_ਸਿਰ_commit;
	ਬਣਤਰ ਰੁੱਖ *old_ਸਿਰ_ਰੁੱਖ, *new_ਸਿਰ_ਰੁੱਖ;
	ਅਚਲ ਅੱਖਰ *ਸਿਰ_ਨਿਸ਼ਾਨਾ;
	ਪੂਰਨ ਸਿਰ_flags;

	*ਬਦਲਿਆ = false;

	ਸਿਰ_ਨਿਸ਼ਾਨਾ = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ਨਿਰਾ, &ਸਿਰ_flags);
	ਜੇ (!ਸਿਰ_ਨਿਸ਼ਾਨਾ)
		ਵਾਪਸ error(_("cannot look up HEAD"));

	ਲਈ (ਮਾਪ_t i = 0; i < ਨਤੀਜਾ->updates_nr; i++) {
		ਜੇ (!strcmp(ਨਤੀਜਾ->updates[i].refname, ਸਿਰ_ਨਿਸ਼ਾਨਾ)) {
			ਸਿਰ_update = &ਨਤੀਜਾ->updates[i];
			ਰੋਕੋ;
		}
	}

	ਜੇ (!ਸਿਰ_update)
		ਵਾਪਸ 0;

	old_ਸਿਰ_commit = lookup_commit_reference(repo, &ਸਿਰ_update->old_oid);
	new_ਸਿਰ_commit = lookup_commit_reference(repo, &ਸਿਰ_update->new_oid);
	ਜੇ (!old_ਸਿਰ_commit || !new_ਸਿਰ_commit)
		ਵਾਪਸ error(_("cannot resolve HEAD commit"));

	old_ਸਿਰ_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(repo, old_ਸਿਰ_commit);
	new_ਸਿਰ_ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(repo, new_ਸਿਰ_commit);
	ਜੇ (!old_ਸਿਰ_ਰੁੱਖ || !new_ਸਿਰ_ਰੁੱਖ)
		ਵਾਪਸ error(_("cannot resolve tree for HEAD"));

	ਜੇ (oideq(&old_ਸਿਰ_ਰੁੱਖ->object.oid, &new_ਸਿਰ_ਰੁੱਖ->object.oid))
		ਵਾਪਸ 0;

	*old_ਸਿਰ = old_ਸਿਰ_commit;
	*new_ਸਿਰ = new_ਸਿਰ_commit;
	*ਬਦਲਿਆ = true;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_history_drop(ਪੂਰਨ argc,
			    ਅਚਲ ਅੱਖਰ **argv,
			    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			    ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ਨਿਰਾ,
	};
	ਗਿਣਤੀ replay_ਖਾਲੀ_ਥਾਵਾਂ_commit_action ਖਾਲੀ_ਥਾਵਾਂ = REPLAY_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT_DROP;
	ਗਿਣਤੀ ref_action action = REF_ACTION_DEFAULT;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_ਲੜੀਵਾਰ,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &ਖਾਲੀ_ਥਾਵਾਂ, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_ਖਾਲੀ_ਥਾਵਾਂ),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ commit *original, *rewritten;
	ਬਣਤਰ rev_info revs = { 0 };
	ਬਣਤਰ replay_ਨਤੀਜਾ ਨਤੀਜਾ = { 0 };
	ਬਣਤਰ commit *old_ਸਿਰ, *new_ਸਿਰ;
	bool ਸਿਰ_ਚਾਲਾਂ = false;
	ਪੂਰਨ ret;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਜੇ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ਜੇ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (original->parents->ਅਗਲਾ) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = ਤਿਆਰੀ_revwalk(repo, action, original, &revs);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	rewritten = original->parents->ਵਸਤੂ;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  ਖਾਲੀ_ਥਾਵਾਂ, &ਨਤੀਜਾ);
	ਜੇ (ret) {
		ret = error(_("failed replaying descendants"));
		ਜਾਓ ਬਾਹਰਲਾ;
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
	ਜੇ (!is_bare_repository(repo)) {
		ret = ਲੱਭੋ_ਸਿਰ_ਰੁੱਖ_change(repo, &ਨਤੀਜਾ, &old_ਸਿਰ,
					    &new_ਸਿਰ, &ਸਿਰ_ਚਾਲਾਂ);
		ਜੇ (ret < 0)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (ਸਿਰ_ਚਾਲਾਂ && update_worktree(repo, old_ਸਿਰ, new_ਸਿਰ, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	strbuf_addf(&reflog_ਸੁਨੇਹਾ, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &ਨਤੀਜਾ, reflog_ਸੁਨੇਹਾ.buf, dry_ਲੜੀਵਾਰ);
	ਜੇ (ret < 0) {
		ret = error(_("failed to update references"));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!dry_ਲੜੀਵਾਰ && ਸਿਰ_ਚਾਲਾਂ && update_worktree(repo, old_ਸਿਰ, new_ਸਿਰ, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_ਮੰਜ਼ਿਲ_hex(&new_ਸਿਰ->object.oid));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = 0;

ਬਾਹਰਲਾ:
	replay_ਨਤੀਜਾ_release(&ਨਤੀਜਾ);
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	release_revisions(&revs);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_history(ਪੂਰਨ argc,
		ਅਚਲ ਅੱਖਰ **argv,
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_ਵੰਡੋ_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ਨਿਰਾ,
	};
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_ਵੰਡੋ),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
