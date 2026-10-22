#define USE_THE_REPOSITORY_VARIABLE

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

#define GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#define GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#define GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#define GIT_HISTORY_SPLIT_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#define GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ಸ್ಥಿರ ಶೂನ್ಯ change_data_free(ಶೂನ್ಯ *util, ಧ್ರುವ ವರ್ಣ *str UNUSED)
{
	ಸಂರಚನಾ wt_status_change_data *d = util;
	ಮುಕ್ತ(d->rename_source);
	ಮುಕ್ತ(d);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fill_commit_message(ಸಂರಚನಾ repository *repo,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *old_ವೃಕ್ಷ,
			       ಧ್ರುವ ಸಂರಚನಾ object_id *new_ವೃಕ್ಷ,
			       ಧ್ರುವ ವರ್ಣ *default_message,
			       ಧ್ರುವ ವರ್ಣ *action,
			       ಸಂರಚನಾ strbuf *out)
{
	ಧ್ರುವ ವರ್ಣ *path = git_path_commit_editmsg();
	ಧ್ರುವ ವರ್ಣ *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ಸಂರಚನಾ wt_status s;

	wt_status_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_QUICK;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_prefix = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(path, "w");
	ಯದಿ (!s.fp)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_ಸಾಲು_str, hint, action, comment_ಸಾಲು_str);
	ಯದಿ (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_ವೃಕ್ಷ, new_ವೃಕ್ಷ);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	ಯದಿ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	ಯದಿ (launch_editor(path, out, ರಿಕ್ತ)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	strbuf_stripspace(out, comment_ಸಾಲು_str);

	cleanup_message(out, COMMIT_ಸಂದೇಶ_CLEANUP_ALL, 0);

	ಯದಿ (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಗಣನಾ commit_ವೃಕ್ಷ_flags {
	COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE = (1 << 0),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_ವೃಕ್ಷ_ext(ಸಂರಚನಾ repository *repo,
			   ಧ್ರುವ ವರ್ಣ *action,
			   ಸಂರಚನಾ commit *commit_with_message,
			   ಧ್ರುವ ವರ್ಣ *message_template,
			   ಧ್ರುವ ಸಂರಚನಾ commit_list *parents,
			   ಧ್ರುವ ಸಂರಚನಾ object_id *old_ವೃಕ್ಷ,
			   ಧ್ರುವ ಸಂರಚನಾ object_id *new_ವೃಕ್ಷ,
			   ಸಂರಚನಾ commit **out,
			   ಗಣನಾ commit_ವೃಕ್ಷ_flags flags)
{
	ಧ್ರುವ ವರ್ಣ *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ರಿಕ್ತ,
	};
	ಧ್ರುವ ವರ್ಣ *original_message, *original_body, *ptr;
	ಸಂರಚನಾ commit_extra_header *original_extra_headers = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf commit_message = STRBUF_INIT;
	ಸಂರಚನಾ object_id rewritten_commit_oid;
	ವರ್ಣ *original_author = ರಿಕ್ತ;
	size_t len;
	ಪೂರ್ಣಾಙ್ಕ ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, ರಿಕ್ತ, ರಿಕ್ತ);
	ptr = find_commit_header(original_message, "author", &len);
	ಯದಿ (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	ಯದಿ (!message_template)
		message_template = original_body;

	ಯದಿ (flags & COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_ವೃಕ್ಷ, new_ವೃಕ್ಷ,
					  message_template, action, &commit_message);
		ಯದಿ (ret < 0)
			ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_ವೃಕ್ಷ_extended(commit_message.buf, commit_message.len, new_ವೃಕ್ಷ,
				   parents, &rewritten_commit_oid, original_author,
				   ರಿಕ್ತ, ರಿಕ್ತ, original_extra_headers);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	ಮುಕ್ತ(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ first_parent_ವೃಕ್ಷ_oid(ಸಂರಚನಾ repository *repo,
				 ಸಂರಚನಾ commit *commit,
				 ಸಂರಚನಾ object_id *out)
{
	ಸಂರಚನಾ commit *parent = commit->parents ? commit->parents->item : ರಿಕ್ತ;

	ಯದಿ (!parent) {
		oidcpy(out, repo->hash_algo->empty_ವೃಕ್ಷ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (repo_parse_commit(repo, parent))
		ಪ್ರತ್ಯಾಗಮ error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_ವೃಕ್ಷ(repo, parent)->object.oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_ವೃಕ್ಷ_with_edited_message(ಸಂರಚನಾ repository *repo,
					   ಧ್ರುವ ವರ್ಣ *action,
					   ಸಂರಚನಾ commit *original,
					   ಸಂರಚನಾ commit **out)
{
	ಸಂರಚನಾ object_id parent_ವೃಕ್ಷ_oid;
	ಧ್ರುವ ಸಂರಚನಾ object_id *ವೃಕ್ಷ_oid;

	ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, original)->object.oid;

	ಯದಿ (first_parent_ವೃಕ್ಷ_oid(repo, original, &parent_ವೃಕ್ಷ_oid) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ commit_ವೃಕ್ಷ_ext(repo, action, original, ರಿಕ್ತ, original->parents,
			       &parent_ವೃಕ್ಷ_oid, ವೃಕ್ಷ_oid, out, COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE);
}

ಗಣನಾ ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_ref_action(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ ref_action *action = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, ಮೌಲ್ಯ);
	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಮೌಲ್ಯ, "head")) {
		*action = REF_ACTION_HEAD;
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ revwalk_contains_merges(ಸಂರಚನಾ repository *repo,
				   ಧ್ರುವ ಸಂರಚನಾ strvec *revwalk_args)
{
	ಸಂರಚನಾ strvec args = STRVEC_INIT;
	ಸಂರಚನಾ rev_info revs;
	ಪೂರ್ಣಾಙ್ಕ ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ತಳ್ಳು(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, ರಿಕ್ತ);

	setup_revisions_from_strvec(&args, &revs, ರಿಕ್ತ);
	ಯದಿ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ಯದಿ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		ಗಚ್ಛ out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ setup_revwalk(ಸಂರಚನಾ repository *repo,
			 ಗಣನಾ ref_action action,
			 ಸಂರಚನಾ commit *original,
			 ಸಂರಚನಾ rev_info *revs)
{
	ಸಂರಚನಾ strvec args = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_init_revisions(repo, revs, ರಿಕ್ತ);
	strvec_ತಳ್ಳು(&args, "ignored");
	strvec_ತಳ್ಳು(&args, "--reverse");
	strvec_ತಳ್ಳು(&args, "--topo-order");
	strvec_ತಳ್ಳು(&args, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&args, "--ancestry-path=%s",
		     oid_to_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&args, "^%s", oid_to_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	ಯದಿ (action == REF_ACTION_HEAD) {
		ಸಂರಚನಾ commit_list *from_list = ರಿಕ್ತ;
		ಸಂರಚನಾ commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		ಯದಿ (!head) {
			ret = error(_("cannot look up HEAD"));
			ಗಚ್ಛ out;
		}

		commit_list_ಸೇರಿಸು(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		ಯದಿ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			ಗಚ್ಛ out;
		} ಅನ್ಯಥಾ ಯದಿ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			ಗಚ್ಛ out;
		}

		strvec_ತಳ್ಳು(&args, "HEAD");
	} ಅನ್ಯಥಾ {
		strvec_ತಳ್ಳು(&args, "--branches");
		strvec_ತಳ್ಳು(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	setup_revisions_from_strvec(&args, revs, ರಿಕ್ತ);
	ಯದಿ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_ref_update(ಸಂರಚನಾ ref_transaction *transaction,
			     ಧ್ರುವ ವರ್ಣ *refname,
			     ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid,
			     ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
			     ಧ್ರುವ ವರ್ಣ *reflog_ಸಂದೇಶ,
			     ಸಂರಚನಾ strbuf *err)
{
	ಯದಿ (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ರಿಕ್ತ, ರಿಕ್ತ, 0, reflog_ಸಂದೇಶ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compute_pending_ref_updates(ಸಂರಚನಾ rev_info *revs,
				       ಗಣನಾ ref_action action,
				       ಸಂರಚನಾ commit *original,
				       ಸಂರಚನಾ commit *rewritten,
				       ಗಣನಾ replay_empty_commit_action empty,
				       ಸಂರಚನಾ replay_result *result)
{
	ಧ್ರುವ ಸಂರಚನಾ name_decoration *decoration;
	ಸಂರಚನಾ replay_revisions_options opts = {
		.empty = empty,
	};
	ವರ್ಣ hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	ಪೂರ್ಣಾಙ್ಕ head_flags = 0;
	ಪೂರ್ಣಾಙ್ಕ ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ರಿಕ್ತ, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ಯದಿ (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	ಪ್ರತಿ (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		ಯದಿ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			ಅನುವರ್ತ;

		ಯದಿ (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			ಅನುವರ್ತ;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		ಯದಿ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			ಅನುವರ್ತ;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ apply_pending_ref_updates(ಸಂರಚನಾ repository *repo,
				     ಧ್ರುವ ಸಂರಚನಾ replay_result *result,
				     ಧ್ರುವ ವರ್ಣ *reflog_ಸಂದೇಶ,
				     ಪೂರ್ಣಾಙ್ಕ dry_run)
{
	ಸಂರಚನಾ ref_transaction *transaction = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		ಯದಿ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			ಗಚ್ಛ out;
		}
	}

	ಪ್ರತಿ (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_ಸಂದೇಶ, &err);
		ಯದಿ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			ಗಚ್ಛ out;
		}
	}

	ಯದಿ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ handle_reference_updates(ಸಂರಚನಾ rev_info *revs,
				    ಗಣನಾ ref_action action,
				    ಸಂರಚನಾ commit *original,
				    ಸಂರಚನಾ commit *rewritten,
				    ಧ್ರುವ ವರ್ಣ *reflog_ಸಂದೇಶ,
				    ಪೂರ್ಣಾಙ್ಕ dry_run,
				    ಗಣನಾ replay_empty_commit_action empty)
{
	ಸಂರಚನಾ replay_result result = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_ಸಂದೇಶ, dry_run);

out:
	replay_result_release(&result);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ commit_became_empty(ಸಂರಚನಾ repository *repo,
			       ಸಂರಚನಾ commit *original,
			       ಸಂರಚನಾ ವೃಕ್ಷ *result)
{
	ಸಂರಚನಾ object_id parent_ವೃಕ್ಷ_oid;

	ಯದಿ (first_parent_ವೃಕ್ಷ_oid(repo, original, &parent_ವೃಕ್ಷ_oid) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ oideq(&result->object.oid, &parent_ವೃಕ್ಷ_oid);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_empty(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಗಣನಾ replay_empty_commit_action *ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!strcmp(arg, "drop"))
		*ಮೌಲ್ಯ = REPLAY_EMPTY_COMMIT_DROP;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "keep"))
		*ಮೌಲ್ಯ = REPLAY_EMPTY_COMMIT_KEEP;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "abort"))
		*ಮೌಲ್ಯ = REPLAY_EMPTY_COMMIT_ABORT;
	ಅನ್ಯಥಾ
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_history_fixup(ಪೂರ್ಣಾಙ್ಕ argc,
			     ಧ್ರುವ ವರ್ಣ **argv,
			     ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ರಿಕ್ತ,
	};
	ಗಣನಾ replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ಗಣನಾ ref_action action = REF_ACTION_DEFAULT;
	ಗಣನಾ commit_ವೃಕ್ಷ_flags flags = 0;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	ಸಂರಚನಾ merge_result merge_result = { 0 };
	ಸಂರಚನಾ merge_options merge_opts = { 0 };
	ಸಂರಚನಾ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ commit *head_commit, *original, *rewritten;
	ಸಂರಚನಾ ವೃಕ್ಷ *head_ವೃಕ್ಷ, *original_ವೃಕ್ಷ, *index_ವೃಕ್ಷ;
	ಸಂರಚನಾ rev_info revs = { 0 };
	bool skip_commit = false;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ಯದಿ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ಗಚ್ಛ out;
	}
	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ಯದಿ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	ಯದಿ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		ಗಚ್ಛ out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	ಯದಿ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಗಚ್ಛ out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	ಯದಿ (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		ಗಚ್ಛ out;
	}

	head_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, head_commit);
	ಯದಿ (!head_ವೃಕ್ಷ) {
		ret = error(_("cannot get tree for HEAD"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (!repo_index_has_changes(repo, head_ವೃಕ್ಷ, ರಿಕ್ತ)) {
		ret = error(_("nothing to fixup: no staged changes"));
		ಗಚ್ಛ out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_ವೃಕ್ಷ = write_in_core_index_as_ವೃಕ್ಷ(repo, repo->index);
	ಯದಿ (!index_ವೃಕ್ಷ) {
		ret = error(_("unable to write index as a tree"));
		ಗಚ್ಛ out;
	}

	original_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, original);
	ಯದಿ (!original_ವೃಕ್ಷ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		ಗಚ್ಛ out;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_merge_options(&merge_opts, repo);
	merge_opts.ancestor = "HEAD";
	merge_opts.branch1 = argv[0];
	merge_opts.branch2 = "staged";
	merge_incore_nonrecursive(&merge_opts, head_ವೃಕ್ಷ,
				  original_ವೃಕ್ಷ, index_ವೃಕ್ಷ, &merge_result);

	ಯದಿ (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		ಗಚ್ಛ out;
	}

	ret = commit_became_empty(repo, original, merge_result.ವೃಕ್ಷ);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;
	ಯದಿ (ret > 0) {
		ವಿಕಲ್ಪ (empty) {
		ಪಕ್ಷ REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : ರಿಕ್ತ;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			ಯದಿ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				ಗಚ್ಛ out;
			}

			skip_commit = true;
			ಭಙ್ಗ;
		ಪಕ್ಷ REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			ಭಙ್ಗ;
		ಪಕ್ಷ REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			ಗಚ್ಛ out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ಯದಿ (!skip_commit) {
		ret = commit_ವೃಕ್ಷ_ext(repo, "fixup", original, ರಿಕ್ತ, original->parents,
				      &original_ವೃಕ್ಷ->object.oid, &merge_result.ವೃಕ್ಷ->object.oid,
				      &rewritten, flags);
		ಯದಿ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			ಗಚ್ಛ out;
		}
	}

	strbuf_addf(&reflog_ಸಂದೇಶ, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_run, empty);
	ಯದಿ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_history_reword(ಪೂರ್ಣಾಙ್ಕ argc,
			      ಧ್ರುವ ವರ್ಣ **argv,
			      ಧ್ರುವ ವರ್ಣ *prefix,
			      ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ರಿಕ್ತ,
	};
	ಗಣನಾ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ಸಂರಚನಾ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ commit *original, *rewritten;
	ಸಂರಚನಾ rev_info revs = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ಯದಿ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ಗಚ್ಛ out;
	}
	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ಯದಿ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ಯದಿ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಗಚ್ಛ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ret = commit_ವೃಕ್ಷ_with_edited_message(repo, "reworded", original, &rewritten);
	ಯದಿ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		ಗಚ್ಛ out;
	}

	strbuf_addf(&reflog_ಸಂದೇಶ, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	ಯದಿ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_ondisk_index(ಸಂರಚನಾ repository *repo,
			      ಸಂರಚನಾ object_id *oid,
			      ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ unpack_trees_options opts = { 0 };
	ಸಂರಚನಾ lock_file lock = LOCK_INIT;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc ವೃಕ್ಷ_desc;
	ಸಂರಚನಾ index_state index;
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಪೂರ್ಣಾಙ್ಕ ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(repo, oid);
	ಯದಿ (!ವೃಕ್ಷ) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		ಗಚ್ಛ out;
	}
	init_ವೃಕ್ಷ_desc(&ವೃಕ್ಷ_desc, &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);

	ಯದಿ (unpack_trees(1, &ವೃಕ್ಷ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		ಗಚ್ಛ out;
	}

	ಅವಿಭಾಜ್ಯ_cache_ವೃಕ್ಷ(repo, &index, ವೃಕ್ಷ);

	ಯದಿ (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_commit(ಸಂರಚನಾ repository *repo,
			ಸಂರಚನಾ commit *original,
			ಸಂರಚನಾ pathspec *pathspec,
			ಸಂರಚನಾ commit **out)
{
	ಸಂರಚನಾ interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ಸಂರಚನಾ strbuf index_file = STRBUF_INIT;
	ಸಂರಚನಾ index_state index = INDEX_STATE_INIT(repo);
	ಧ್ರುವ ಸಂರಚನಾ object_id *original_commit_ವೃಕ್ಷ_oid;
	ಧ್ರುವ ಸಂರಚನಾ object_id *old_ವೃಕ್ಷ_oid, *new_ವೃಕ್ಷ_oid;
	ಸಂರಚನಾ object_id parent_ವೃಕ್ಷ_oid;
	ವರ್ಣ original_commit_oid[GIT_MAX_HEXSZ + 1];
	ಸಂರಚನಾ commit *first_commit, *second_commit;
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
	ಸಂರಚನಾ ವೃಕ್ಷ *split_ವೃಕ್ಷ;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (first_parent_ವೃಕ್ಷ_oid(repo, original, &parent_ವೃಕ್ಷ_oid) < 0) {
		ret = -1;
		ಗಚ್ಛ out;
	}
	original_commit_ವೃಕ್ಷ_oid = get_commit_ವೃಕ್ಷ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_ವೃಕ್ಷ_oid, index_file.buf);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	ಯದಿ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		ಗಚ್ಛ out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	split_ವೃಕ್ಷ = write_in_core_index_as_ವೃಕ್ಷ(repo, &index);
	ಯದಿ (!split_ವೃಕ್ಷ) {
		ret = error(_("failed split tree"));
		ಗಚ್ಛ out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	ಯದಿ (oideq(&split_ವೃಕ್ಷ->object.oid, &parent_ವೃಕ್ಷ_oid)) {
		ret = error(_("split commit is empty"));
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (oideq(&split_ವೃಕ್ಷ->object.oid, original_commit_ವೃಕ್ಷ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		ಗಚ್ಛ out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_ವೃಕ್ಷ_ext(repo, "split-out", original, ರಿಕ್ತ, original->parents, &parent_ವೃಕ್ಷ_oid,
			      &split_ವೃಕ್ಷ->object.oid, &first_commit, COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE);
	ಯದಿ (ret < 0) {
		ret = error(_("failed writing first commit"));
		ಗಚ್ಛ out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, first_commit)->object.oid;
	new_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, original)->object.oid;

	ret = commit_ವೃಕ್ಷ_ext(repo, "split-out", original, ರಿಕ್ತ, parents, old_ವೃಕ್ಷ_oid,
			      new_ವೃಕ್ಷ_oid, &second_commit, COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE);
	ಯದಿ (ret < 0) {
		ret = error(_("failed writing second commit"));
		ಗಚ್ಛ out;
	}

	*out = second_commit;
	ret = 0;

out:
	ಯದಿ (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_history_split(ಪೂರ್ಣಾಙ್ಕ argc,
			     ಧ್ರುವ ವರ್ಣ **argv,
			     ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		ರಿಕ್ತ,
	};
	ಗಣನಾ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ಸಂರಚನಾ commit *original, *rewritten = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ pathspec pathspec = { 0 };
	ಸಂರಚನಾ rev_info revs = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ಯದಿ (argc < 1) {
		ret = error(_("command expects a committish"));
		ಗಚ್ಛ out;
	}
	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ಯದಿ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	ಯದಿ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಗಚ್ಛ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	ಯದಿ (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		ಗಚ್ಛ out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	strbuf_addf(&reflog_ಸಂದೇಶ, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	ಯದಿ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_ಸಂದೇಶ);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	ಪ್ರತ್ಯಾಗಮ ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

ಸ್ಥಿರ bool is_autosquash_subject(ಧ್ರುವ ವರ್ಣ *s)
{
	ಪ್ರತ್ಯಾಗಮ starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ಸ್ಥಿರ bool skip_one_autosquash_prefix(ಧ್ರುವ ವರ್ಣ *s, ಧ್ರುವ ವರ್ಣ **out)
{
	ಯದಿ (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		ಯಾವತ್ (**out == ' ')
			(*out)++;
		ಪ್ರತ್ಯಾಗಮ true;
	}
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ truncate_message_to_subject(ಸಂರಚನಾ strbuf *ಸಂದೇಶ)
{
	ಧ್ರುವ ವರ್ಣ *eos = strstr(ಸಂದೇಶ->buf, "\n\n");

	ಯದಿ (eos)
		strbuf_setlen(ಸಂದೇಶ, eos - ಸಂದೇಶ->buf + 1);
}

ಸಂರಚನಾ subject_data {
	ಸಂರಚನಾ strintmap subjects;
	ಸಂರಚನಾ strbuf subject;
	ಸಂರಚನಾ strbuf squash_message;
	ಧ್ರುವ ವರ್ಣ *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_data_clear(ಸಂರಚನಾ subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ squash_amend_message(ಸಂರಚನಾ repository *repo,
				ಸಂರಚನಾ commit *commit,
				ಸಂರಚನಾ subject_data *data,
				unsigned flags)
{
	ಧ್ರುವ ವರ್ಣ *body = data->message + data->subject.len;

	ಯಾವತ್ (isspace(*body))
		body++;

	ಯದಿ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (data->edit_message) {
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (flags & SQUASH_AMEND_TARGET) {
		ಯದಿ (starts_with(data->squash_message.buf, "squash!"))
			ಪ್ರತ್ಯಾಗಮ error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		ಯದಿ (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		ಯದಿ (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} ಅನ್ಯಥಾ {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_ಸಾಲು(&data->squash_message);
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ squash_squash_message(ಸಂರಚನಾ repository *repo,
				ಸಂರಚನಾ commit *commit,
				ಸಂರಚನಾ subject_data *data,
				unsigned flags)
{
	ಧ್ರುವ ವರ್ಣ *body = data->message + data->subject.len;

	ಯಾವತ್ (isspace(*body))
		body++;

	ಯದಿ (data->edit_message) {
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (flags & SQUASH_AMEND_TARGET) {
		ಯದಿ (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		ಯದಿ (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_ಸಾಲು(&data->squash_message);
		} ಅನ್ಯಥಾ {
			ಪ್ರತ್ಯಾಗಮ error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ squash_check_can_autosquash(ಸಂರಚನಾ repository *repo,
				       ಸಂರಚನಾ commit *commit,
				       ಸಂರಚನಾ subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	ಯದಿ (starts_with(data->subject.buf, "amend!"))
		ಪ್ರತ್ಯಾಗಮ squash_amend_message(repo, commit, data, flags);
	ಅನ್ಯಥಾ ಯದಿ (starts_with(data->subject.buf, "squash!"))
		ಪ್ರತ್ಯಾಗಮ squash_squash_message(repo, commit, data, flags);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ squash_check_autosquash_subject(ಸಂರಚನಾ repository *repo,
					   ಸಂರಚನಾ commit *commit,
					   ಸಂರಚನಾ subject_data *data)
{
	ಧ್ರುವ ವರ್ಣ* s = data->subject.buf;
	ಸಂರಚನಾ commit *target;
	ಸಂರಚನಾ hashmap_iter iter;
	ಸಂರಚನಾ strmap_entry *entry;
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
	ಯಾವತ್ (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		ಯದಿ (flags)
			ಪ್ರತ್ಯಾಗಮ squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	ಯದಿ (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		ಯದಿ (!flags) {
			ಧ್ರುವ ವರ್ಣ *subject_start;
			ಧ್ರುವ ವರ್ಣ *buffer = repo_logmsg_reencode(repo, target,
								  ರಿಕ್ತ, ರಿಕ್ತ);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			ವರ್ಣ *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			ಮುಕ್ತ(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		ಯದಿ (flags)
			ಪ್ರತ್ಯಾಗಮ squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		ಯಾವತ್(skip_one_autosquash_prefix(s, &s)) {
			ಯದಿ (starts_with(entry->ಕೀಲಿ, s)) {
				unsigned ಮೌಲ್ಯ = (intptr_t)entry->ಮೌಲ್ಯ;

				ಪ್ರತ್ಯಾಗಮ squash_check_can_autosquash(repo, commit,
								   data, ಮೌಲ್ಯ);
			}
		}
	}
	ಪ್ರತ್ಯಾಗಮ error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ squash_check_subject(ಸಂರಚನಾ repository *repo,
				ಸಂರಚನಾ commit *commit,
				ಸಂರಚನಾ subject_data *data)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಧ್ರುವ ವರ್ಣ *buf = repo_logmsg_reencode(repo, commit, ರಿಕ್ತ, ರಿಕ್ತ);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	ಯದಿ (!strintmap_get_size(&data->subjects)) {
		ಧ್ರುವ ವರ್ಣ *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_ಸಾಲು(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} ಅನ್ಯಥಾ ಯದಿ (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} ಅನ್ಯಥಾ {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ build_squash_message(ಸಂರಚನಾ repository *repo,
				ಧ್ರುವ ಸಂರಚನಾ strbuf *todo_buf,
				ಸಂರಚನಾ strbuf *out);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ setup_squash_revisions(ಸಂರಚನಾ repository *repo,
				  ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				  ಸಂರಚನಾ rev_info *revs)
{
	repo_init_revisions(repo, revs, ರಿಕ್ತ);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, ರಿಕ್ತ);
	ಯದಿ (argc > 1)
		ಪ್ರತ್ಯಾಗಮ error(_("unrecognized argument: %s"), argv[1]);

	ಯದಿ (revs->reverse != 1 || revs->topo_order != 1 ||
	    revs->sort_order != REV_SORT_IN_GRAPH_ORDER ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_path != 1 || revs->limited != 1 ||
	    revs->ancestry_path_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->reverse = 1;
		revs->topo_order = 1;
		revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_path = 1;
		revs->limited = 1;
		revs->ancestry_path_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	ಪ್ರತಿ (size_t i = 0; i < revs->cmdline.nr; i++)
		ಯದಿ (revs->cmdline.rev[i].flags & BOTTOM)
			ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ resolve_squash_range(ಸಂರಚನಾ repository *repo,
				bool update_branches,
				bool edit_message,
				ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
				ಸಂರಚನಾ commit **oldest_out,
				ಸಂರಚನಾ commit **tip_out,
				ವರ್ಣ **message_out)
{
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ subject_data subject_data = SUBJECT_DATA_INIT;
	ಸಂರಚನಾ commit *commit, *oldest = ರಿಕ್ತ, *tip = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf todo_buf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret, tip_ಎಣಿಕೆ = 0;
	bool walk_started = false;
	ಸಂರಚನಾ ref_filter filter = REF_FILTER_INIT;
	ಸಂರಚನಾ ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	ಯದಿ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ಗಚ್ಛ out;
	}
	walk_started = true;
	ಯಾವತ್ ((commit = get_revision(&revs))) {
		ಸಂರಚನಾ commit_list *p;

		ಯದಿ (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		ಯದಿ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			ಗಚ್ಛ out;
		}
		ಪ್ರತಿ (p = commit->parents; oldest && p; p = p->next) {
			ಸಂರಚನಾ commit_list *q;
			ಸಂರಚನಾ object *o;
			bool seen;

			ಯದಿ (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				ಗಚ್ಛ out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			ಪ್ರತಿ (q = oldest->parents; !seen && q; q = q->next) {
				ಯದಿ (p->item == q->item) {
					seen = true;
					commit_list_ಸೇರಿಸು(commit, &filter.with_commit);
				}
			}
			ಯದಿ (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				ಗಚ್ಛ out;
			}
			ಯದಿ (o->flags & SQUASH_TIP) {
				tip_ಎಣಿಕೆ--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		ಯದಿ (!oldest) {
			commit_list_ಸೇರಿಸು(commit, &filter.with_commit);
			oldest = commit;
		}
		ಯದಿ (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			ಗಚ್ಛ out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_ಎಣಿಕೆ++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	ಯದಿ (!tip_ಎಣಿಕೆ) {
		ret = error(_("the revision range is empty"));
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (tip_ಎಣಿಕೆ != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ಸೇರಿಸು(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	ಯದಿ (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		ಗಚ್ಛ out;
	}
	ಯದಿ (refs.nr) {
		ಸಂರಚನಾ ref_format format = REF_FORMAT_INIT;
		ಸಂರಚನಾ ref_sorting *sorting;
		ಸಂರಚನಾ string_list sorting_options = STRING_LIST_INIT_DUP;
		ಸಂರಚನಾ strbuf branches = STRBUF_INIT;
		ಸಂರಚನಾ strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		ಯದಿ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			ಯದಿ (format_ref_array_item(refs.ವಸ್ತುಗಳು[i], &format,
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
		ref_sorting_release(sorting);
		string_list_clear(&sorting_options, 0);
		ಗಚ್ಛ out;
	}
	ಯದಿ (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		ಯದಿ (ret < 0)
			ಗಚ್ಛ out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, ರಿಕ್ತ);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	ಯದಿ (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ bool amend_replaces_target(ಸಂರಚನಾ todo_list *todo, ಪೂರ್ಣಾಙ್ಕ target)
{
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = target + 1; i < todo->nr &&
				 todo->ವಸ್ತುಗಳು[i].command != TODO_PICK; i++) {
		ಯದಿ (todo->ವಸ್ತುಗಳು[i].command == TODO_SQUASH)
			ಪ್ರತ್ಯಾಗಮ false;
		ಯದಿ (todo->ವಸ್ತುಗಳು[i].flags & TODO_REPLACE_FIXUP_ಸಂದೇಶ)
			ಪ್ರತ್ಯಾಗಮ true;
	}
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ build_squash_message(ಸಂರಚನಾ repository *repo,
				ಧ್ರುವ ಸಂರಚನಾ strbuf *todo_buf,
				ಸಂರಚನಾ strbuf *out)
{
	ಸಂರಚನಾ todo_list todo = TODO_LIST_INIT;
	ಸಂರಚನಾ replay_opts opts = REPLAY_OPTS_INIT;
	ಪೂರ್ಣಾಙ್ಕ nr_commits, ret;

	ಯದಿ (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		ಗಚ್ಛ out;
	}

	nr_commits = todo.nr;
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nr_commits; i++) {
		ಸಂರಚನಾ todo_item *item = &todo.ವಸ್ತುಗಳು[i];
		ಧ್ರುವ ವರ್ಣ *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_ಸಂದೇಶ);
		ಯದಿ (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		ಅನ್ಯಥಾ
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, ರಿಕ್ತ, ರಿಕ್ತ);
		find_commit_subject(message, &body);

		ಯದಿ (skip)
			commented_len = strlen(body);
		ಅನ್ಯಥಾ ಯದಿ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		ಅನ್ಯಥಾ
			commented_len = 0;

		ಯದಿ (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_ಸಾಲುಗಳು(out, body, commented_len, comment_ಸಾಲು_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_ಸಾಲು(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_history_squash(ಪೂರ್ಣಾಙ್ಕ argc,
			      ಧ್ರುವ ವರ್ಣ **argv,
			      ಧ್ರುವ ವರ್ಣ *prefix,
			      ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ರಿಕ್ತ,
	};
	ಗಣನಾ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0;
	ಪೂರ್ಣಾಙ್ಕ edit = 1;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	ಸಂರಚನಾ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ commit *oldest, *tip, *rewritten;
	ಧ್ರುವ ಸಂರಚನಾ object_id *base_ವೃಕ್ಷ_oid, *tip_ವೃಕ್ಷ_oid;
	ವರ್ಣ *message_template = ರಿಕ್ತ;
	ಸಂರಚನಾ rev_info revs = { 0 };
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	ಯದಿ (argc < 2) {
		ret = error(_("command expects a revision range"));
		ಗಚ್ಛ out;
	}
	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ಯದಿ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_ಸಂದೇಶ, "squash: updating ");
	strbuf_join_argv(&reflog_ಸಂದೇಶ, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	ret = setup_revwalk(repo, action, tip, &revs);
	ಯದಿ (ret < 0)
		ಗಚ್ಛ out;

	base_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo,
					oldest->parents->item)->object.oid;
	tip_ವೃಕ್ಷ_oid = &repo_get_commit_ವೃಕ್ಷ(repo, tip)->object.oid;

	ret = commit_ವೃಕ್ಷ_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_ವೃಕ್ಷ_oid, tip_ವೃಕ್ಷ_oid,
			      &rewritten,
			      edit ? COMMIT_ವೃಕ್ಷ_EDIT_MESSAGE : 0);
	ಯದಿ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		ಗಚ್ಛ out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_ಸಂದೇಶ.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	ಯದಿ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಮುಕ್ತ(message_template);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_worktree(ಸಂರಚನಾ repository *repo,
			   ಧ್ರುವ ಸಂರಚನಾ commit *old_head,
			   ಧ್ರುವ ಸಂರಚನಾ commit *new_head,
			   bool dry_run)
{
	ಸಂರಚನಾ reset_working_ವೃಕ್ಷ_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	ಯದಿ (dry_run)
		opts.flags |= RESET_WORKING_ವೃಕ್ಷ_DRY_RUN;
	ಪ್ರತ್ಯಾಗಮ reset_working_ವೃಕ್ಷ(repo, &opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ find_head_ವೃಕ್ಷ_change(ಸಂರಚನಾ repository *repo,
				 ಧ್ರುವ ಸಂರಚನಾ replay_result *result,
				 ಸಂರಚನಾ commit **old_head,
				 ಸಂರಚನಾ commit **new_head,
				 bool *changed)
{
	ಧ್ರುವ ಸಂರಚನಾ replay_ref_update *head_update = ರಿಕ್ತ;
	ಸಂರಚನಾ commit *old_head_commit, *new_head_commit;
	ಸಂರಚನಾ ವೃಕ್ಷ *old_head_ವೃಕ್ಷ, *new_head_ವೃಕ್ಷ;
	ಧ್ರುವ ವರ್ಣ *head_target;
	ಪೂರ್ಣಾಙ್ಕ head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ರಿಕ್ತ, &head_flags);
	ಯದಿ (!head_target)
		ಪ್ರತ್ಯಾಗಮ error(_("cannot look up HEAD"));

	ಪ್ರತಿ (size_t i = 0; i < result->updates_nr; i++) {
		ಯದಿ (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			ಭಙ್ಗ;
		}
	}

	ಯದಿ (!head_update)
		ಪ್ರತ್ಯಾಗಮ 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	ಯದಿ (!old_head_commit || !new_head_commit)
		ಪ್ರತ್ಯಾಗಮ error(_("cannot resolve HEAD commit"));

	old_head_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, old_head_commit);
	new_head_ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(repo, new_head_commit);
	ಯದಿ (!old_head_ವೃಕ್ಷ || !new_head_ವೃಕ್ಷ)
		ಪ್ರತ್ಯಾಗಮ error(_("cannot resolve tree for HEAD"));

	ಯದಿ (oideq(&old_head_ವೃಕ್ಷ->object.oid, &new_head_ವೃಕ್ಷ->object.oid))
		ಪ್ರತ್ಯಾಗಮ 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_history_drop(ಪೂರ್ಣಾಙ್ಕ argc,
			    ಧ್ರುವ ವರ್ಣ **argv,
			    ಧ್ರುವ ವರ್ಣ *prefix,
			    ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ರಿಕ್ತ,
	};
	ಗಣನಾ replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ಗಣನಾ ref_action action = REF_ACTION_DEFAULT;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	ಸಂರಚನಾ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ commit *original, *rewritten;
	ಸಂರಚನಾ rev_info revs = { 0 };
	ಸಂರಚನಾ replay_result result = { 0 };
	ಸಂರಚನಾ commit *old_head, *new_head;
	bool head_moves = false;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ಯದಿ (argc != 1) {
		ret = error(_("command expects a single revision"));
		ಗಚ್ಛ out;
	}
	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ಯದಿ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ಯದಿ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ಗಚ್ಛ out;
	}

	ಯದಿ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		ಗಚ್ಛ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	ಯದಿ (ret) {
		ret = error(_("failed replaying descendants"));
		ಗಚ್ಛ out;
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
	ಯದಿ (!is_bare_repository(repo)) {
		ret = find_head_ವೃಕ್ಷ_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		ಯದಿ (ret < 0)
			ಗಚ್ಛ out;

		ಯದಿ (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			ಗಚ್ಛ out;
		}
	}

	strbuf_addf(&reflog_ಸಂದೇಶ, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_ಸಂದೇಶ.buf, dry_run);
	ಯದಿ (ret < 0) {
		ret = error(_("failed to update references"));
		ಗಚ್ಛ out;
	}

	ಯದಿ (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		ಗಚ್ಛ out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_ಸಂದೇಶ);
	release_revisions(&revs);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_history(ಪೂರ್ಣಾಙ್ಕ argc,
		ಧ್ರುವ ವರ್ಣ **argv,
		ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ರಿಕ್ತ,
	};
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
