#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "merge-ort.h"
#ଅନ୍ତର୍ଭାଵ "oidmap.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "ref-filter.h"
#ଅନ୍ତର୍ଭାଵ "replay.h"
#ଅନ୍ତର୍ଭାଵ "reset.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "wt-status.h"

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

ସ୍ଥିର ଶୂନ୍ୟ change_data_free(ଶୂନ୍ୟ *util, ଅଚଳ ଅକ୍ଷର *str UNUSED)
{
	ସଂରଚନା wt_status_change_data *d = util;
	ମୁକ୍ତ(d->rename_source);
	ମୁକ୍ତ(d);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fill_commit_message(ସଂରଚନା repository *repo,
			       ଅଚଳ ସଂରଚନା object_id *old_ବୃକ୍ଷ,
			       ଅଚଳ ସଂରଚନା object_id *new_ବୃକ୍ଷ,
			       ଅଚଳ ଅକ୍ଷର *default_message,
			       ଅଚଳ ଅକ୍ଷର *action,
			       ସଂରଚନା strbuf *out)
{
	ଅଚଳ ଅକ୍ଷର *path = git_path_commit_editmsg();
	ଅଚଳ ଅକ୍ଷର *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ସଂରଚନା wt_status s;

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
	ଯଦି (!s.fp)
		ଫେରନ୍ତୁ error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_ଧାଡ଼ି_str, hint, action, comment_ଧାଡ଼ି_str);
	ଯଦି (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_ବୃକ୍ଷ, new_ବୃକ୍ଷ);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	ଯଦି (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	ଯଦି (launch_editor(path, out, ଖାଲି)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		ଫେରନ୍ତୁ -1;
	}
	strbuf_stripspace(out, comment_ଧାଡ଼ି_str);

	cleanup_message(out, COMMIT_ସନ୍ଦେଶ_CLEANUP_ALL, 0);

	ଯଦି (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ଫେରନ୍ତୁ -1;
	}

	ଫେରନ୍ତୁ 0;
}

ଗଣନା commit_ବୃକ୍ଷ_flags {
	COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE = (1 << 0),
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_ବୃକ୍ଷ_ext(ସଂରଚନା repository *repo,
			   ଅଚଳ ଅକ୍ଷର *action,
			   ସଂରଚନା commit *commit_with_message,
			   ଅଚଳ ଅକ୍ଷର *message_template,
			   ଅଚଳ ସଂରଚନା commit_list *parents,
			   ଅଚଳ ସଂରଚନା object_id *old_ବୃକ୍ଷ,
			   ଅଚଳ ସଂରଚନା object_id *new_ବୃକ୍ଷ,
			   ସଂରଚନା commit **out,
			   ଗଣନା commit_ବୃକ୍ଷ_flags flags)
{
	ଅଚଳ ଅକ୍ଷର *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ଖାଲି,
	};
	ଅଚଳ ଅକ୍ଷର *original_message, *original_body, *ptr;
	ସଂରଚନା commit_extra_header *original_extra_headers = ଖାଲି;
	ସଂରଚନା strbuf commit_message = STRBUF_INIT;
	ସଂରଚନା object_id rewritten_commit_oid;
	ଅକ୍ଷର *original_author = ଖାଲି;
	size_t len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, ଖାଲି, ଖାଲି);
	ptr = find_commit_header(original_message, "author", &len);
	ଯଦି (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	ଯଦି (!message_template)
		message_template = original_body;

	ଯଦି (flags & COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_ବୃକ୍ଷ, new_ବୃକ୍ଷ,
					  message_template, action, &commit_message);
		ଯଦି (ret < 0)
			ଯାଅ out;
	} ଅନ୍ୟଥା {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_ବୃକ୍ଷ_extended(commit_message.buf, commit_message.len, new_ବୃକ୍ଷ,
				   parents, &rewritten_commit_oid, original_author,
				   ଖାଲି, ଖାଲି, original_extra_headers);
	ଯଦି (ret < 0)
		ଯାଅ out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	ମୁକ୍ତ(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_parent_ବୃକ୍ଷ_oid(ସଂରଚନା repository *repo,
				 ସଂରଚନା commit *commit,
				 ସଂରଚନା object_id *out)
{
	ସଂରଚନା commit *parent = commit->parents ? commit->parents->item : ଖାଲି;

	ଯଦି (!parent) {
		oidcpy(out, repo->hash_algo->empty_ବୃକ୍ଷ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (repo_parse_commit(repo, parent))
		ଫେରନ୍ତୁ error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_ବୃକ୍ଷ(repo, parent)->object.oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_ବୃକ୍ଷ_with_edited_message(ସଂରଚନା repository *repo,
					   ଅଚଳ ଅକ୍ଷର *action,
					   ସଂରଚନା commit *original,
					   ସଂରଚନା commit **out)
{
	ସଂରଚନା object_id parent_ବୃକ୍ଷ_oid;
	ଅଚଳ ସଂରଚନା object_id *ବୃକ୍ଷ_oid;

	ବୃକ୍ଷ_oid = &repo_get_commit_ବୃକ୍ଷ(repo, original)->object.oid;

	ଯଦି (first_parent_ବୃକ୍ଷ_oid(repo, original, &parent_ବୃକ୍ଷ_oid) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ commit_ବୃକ୍ଷ_ext(repo, action, original, ଖାଲି, original->parents,
			       &parent_ବୃକ୍ଷ_oid, ବୃକ୍ଷ_oid, out, COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE);
}

ଗଣନା ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_ref_action(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା ref_action *action = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG_NOARG(unset, ମୂଲ୍ୟ);
	ଯଦି (!strcmp(ମୂଲ୍ୟ, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(ମୂଲ୍ୟ, "head")) {
		*action = REF_ACTION_HEAD;
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା revwalk_contains_merges(ସଂରଚନା repository *repo,
				   ଅଚଳ ସଂରଚନା strvec *revwalk_args)
{
	ସଂରଚନା strvec args = STRVEC_INIT;
	ସଂରଚନା rev_info revs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ଠେଲ(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, ଖାଲି);

	setup_revisions_from_strvec(&args, &revs, ଖାଲି);
	ଯଦି (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ଯଦି (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ଯାଅ out;
	}

	ଯଦି (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		ଯାଅ out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା setup_revwalk(ସଂରଚନା repository *repo,
			 ଗଣନା ref_action action,
			 ସଂରଚନା commit *original,
			 ସଂରଚନା rev_info *revs)
{
	ସଂରଚନା strvec args = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_init_revisions(repo, revs, ଖାଲି);
	strvec_ଠେଲ(&args, "ignored");
	strvec_ଠେଲ(&args, "--reverse");
	strvec_ଠେଲ(&args, "--topo-order");
	strvec_ଠେଲ(&args, "--full-history");

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
	ଯଦି (action == REF_ACTION_HEAD) {
		ସଂରଚନା commit_list *from_list = ଖାଲି;
		ସଂରଚନା commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		ଯଦି (!head) {
			ret = error(_("cannot look up HEAD"));
			ଯାଅ out;
		}

		commit_list_ଭର୍ତ୍ତି(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		ଯଦି (ret < 0) {
			ret = error(_("cannot determine descendance"));
			ଯାଅ out;
		} ଅନ୍ୟଥା ଯଦି (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			ଯାଅ out;
		}

		strvec_ଠେଲ(&args, "HEAD");
	} ଅନ୍ୟଥା {
		strvec_ଠେଲ(&args, "--branches");
		strvec_ଠେଲ(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	ଯଦି (ret < 0)
		ଯାଅ out;

	setup_revisions_from_strvec(&args, revs, ଖାଲି);
	ଯଦି (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_ref_update(ସଂରଚନା ref_transaction *transaction,
			     ଅଚଳ ଅକ୍ଷର *refname,
			     ଅଚଳ ସଂରଚନା object_id *new_oid,
			     ଅଚଳ ସଂରଚନା object_id *old_oid,
			     ଅଚଳ ଅକ୍ଷର *reflog_ସନ୍ଦେଶ,
			     ସଂରଚନା strbuf *err)
{
	ଯଦି (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ଖାଲି, ଖାଲି, 0, reflog_ସନ୍ଦେଶ, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compute_pending_ref_updates(ସଂରଚନା rev_info *revs,
				       ଗଣନା ref_action action,
				       ସଂରଚନା commit *original,
				       ସଂରଚନା commit *rewritten,
				       ଗଣନା replay_empty_commit_action empty,
				       ସଂରଚନା replay_result *result)
{
	ଅଚଳ ସଂରଚନା name_decoration *decoration;
	ସଂରଚନା replay_revisions_options opts = {
		.empty = empty,
	};
	ଅକ୍ଷର hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା head_flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ଖାଲି, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ଯଦି (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	ପାଇଁ (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		ଯଦି (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			ଜାରି;

		ଯଦି (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			ଜାରି;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		ଯଦି (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			ଜାରି;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା apply_pending_ref_updates(ସଂରଚନା repository *repo,
				     ଅଚଳ ସଂରଚନା replay_result *result,
				     ଅଚଳ ଅକ୍ଷର *reflog_ସନ୍ଦେଶ,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run)
{
	ସଂରଚନା ref_transaction *transaction = ଖାଲି;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		ଯଦି (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			ଯାଅ out;
		}
	}

	ପାଇଁ (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_ସନ୍ଦେଶ, &err);
		ଯଦି (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			ଯାଅ out;
		}
	}

	ଯଦି (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		ଯାଅ out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle_reference_updates(ସଂରଚନା rev_info *revs,
				    ଗଣନା ref_action action,
				    ସଂରଚନା commit *original,
				    ସଂରଚନା commit *rewritten,
				    ଅଚଳ ଅକ୍ଷର *reflog_ସନ୍ଦେଶ,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run,
				    ଗଣନା replay_empty_commit_action empty)
{
	ସଂରଚନା replay_result result = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	ଯଦି (ret)
		ଯାଅ out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_ସନ୍ଦେଶ, dry_run);

out:
	replay_result_release(&result);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା commit_became_empty(ସଂରଚନା repository *repo,
			       ସଂରଚନା commit *original,
			       ସଂରଚନା ବୃକ୍ଷ *result)
{
	ସଂରଚନା object_id parent_ବୃକ୍ଷ_oid;

	ଯଦି (first_parent_ବୃକ୍ଷ_oid(repo, original, &parent_ବୃକ୍ଷ_oid) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ oideq(&result->object.oid, &parent_ବୃକ୍ଷ_oid);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_empty(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଗଣନା replay_empty_commit_action *ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!strcmp(arg, "drop"))
		*ମୂଲ୍ୟ = REPLAY_EMPTY_COMMIT_DROP;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "keep"))
		*ମୂଲ୍ୟ = REPLAY_EMPTY_COMMIT_KEEP;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "abort"))
		*ମୂଲ୍ୟ = REPLAY_EMPTY_COMMIT_ABORT;
	ଅନ୍ୟଥା
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_history_fixup(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			     ଅଚଳ ଅକ୍ଷର **argv,
			     ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ଖାଲି,
	};
	ଗଣନା replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ଗଣନା ref_action action = REF_ACTION_DEFAULT;
	ଗଣନା commit_ବୃକ୍ଷ_flags flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0;
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	ସଂରଚନା merge_result merge_result = { 0 };
	ସଂରଚନା merge_options merge_opts = { 0 };
	ସଂରଚନା strbuf reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା commit *head_commit, *original, *rewritten;
	ସଂରଚନା ବୃକ୍ଷ *head_ବୃକ୍ଷ, *original_ବୃକ୍ଷ, *index_ବୃକ୍ଷ;
	ସଂରଚନା rev_info revs = { 0 };
	bool skip_commit = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଯଦି (argc != 1) {
		ret = error(_("command expects a single revision"));
		ଯାଅ out;
	}
	repo_config(repo, git_default_config, ଖାଲି);

	ଯଦି (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	ଯଦି (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		ଯାଅ out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	ଯଦି (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ଯାଅ out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	ଯଦି (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		ଯାଅ out;
	}

	head_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(repo, head_commit);
	ଯଦି (!head_ବୃକ୍ଷ) {
		ret = error(_("cannot get tree for HEAD"));
		ଯାଅ out;
	}

	ଯଦି (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		ଯାଅ out;
	}

	ଯଦି (!repo_index_has_changes(repo, head_ବୃକ୍ଷ, ଖାଲି)) {
		ret = error(_("nothing to fixup: no staged changes"));
		ଯାଅ out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_ବୃକ୍ଷ = write_in_core_index_as_ବୃକ୍ଷ(repo, repo->index);
	ଯଦି (!index_ବୃକ୍ଷ) {
		ret = error(_("unable to write index as a tree"));
		ଯାଅ out;
	}

	original_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(repo, original);
	ଯଦି (!original_ବୃକ୍ଷ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		ଯାଅ out;
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
	merge_incore_nonrecursive(&merge_opts, head_ବୃକ୍ଷ,
				  original_ବୃକ୍ଷ, index_ବୃକ୍ଷ, &merge_result);

	ଯଦି (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		ଯାଅ out;
	}

	ଯଦି (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		ଯାଅ out;
	}

	ret = commit_became_empty(repo, original, merge_result.ବୃକ୍ଷ);
	ଯଦି (ret < 0)
		ଯାଅ out;
	ଯଦି (ret > 0) {
		ବାଛ (empty) {
		ସ୍ଥିତି REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : ଖାଲି;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			ଯଦି (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				ଯାଅ out;
			}

			skip_commit = true;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			ଭାଙ୍ଗ;
		ସ୍ଥିତି REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			ଯାଅ out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ଯଦି (ret)
		ଯାଅ out;

	ଯଦି (!skip_commit) {
		ret = commit_ବୃକ୍ଷ_ext(repo, "fixup", original, ଖାଲି, original->parents,
				      &original_ବୃକ୍ଷ->object.oid, &merge_result.ବୃକ୍ଷ->object.oid,
				      &rewritten, flags);
		ଯଦି (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			ଯାଅ out;
		}
	}

	strbuf_addf(&reflog_ସନ୍ଦେଶ, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ସନ୍ଦେଶ.buf, dry_run, empty);
	ଯଦି (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ଯାଅ out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	release_revisions(&revs);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_history_reword(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			      ଅଚଳ ଅକ୍ଷର **argv,
			      ଅଚଳ ଅକ୍ଷର *prefix,
			      ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ଖାଲି,
	};
	ଗଣନା ref_action action = REF_ACTION_DEFAULT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0;
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ସଂରଚନା strbuf reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା commit *original, *rewritten;
	ସଂରଚନା rev_info revs = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଯଦି (argc != 1) {
		ret = error(_("command expects a single revision"));
		ଯାଅ out;
	}
	repo_config(repo, git_default_config, ଖାଲି);

	ଯଦି (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ଯଦି (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ଯାଅ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ଯଦି (ret)
		ଯାଅ out;

	ret = commit_ବୃକ୍ଷ_with_edited_message(repo, "reworded", original, &rewritten);
	ଯଦି (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		ଯାଅ out;
	}

	strbuf_addf(&reflog_ସନ୍ଦେଶ, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ସନ୍ଦେଶ.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	ଯଦି (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ଯାଅ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	release_revisions(&revs);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_ondisk_index(ସଂରଚନା repository *repo,
			      ସଂରଚନା object_id *oid,
			      ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା unpack_trees_options opts = { 0 };
	ସଂରଚନା lock_file lock = LOCK_INIT;
	ସଂରଚନା ବୃକ୍ଷ_desc ବୃକ୍ଷ_desc;
	ସଂରଚନା index_state index;
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(repo, oid);
	ଯଦି (!ବୃକ୍ଷ) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		ଯାଅ out;
	}
	init_ବୃକ୍ଷ_desc(&ବୃକ୍ଷ_desc, &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);

	ଯଦି (unpack_trees(1, &ବୃକ୍ଷ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		ଯାଅ out;
	}

	ମୌଳିକ_cache_ବୃକ୍ଷ(repo, &index, ବୃକ୍ଷ);

	ଯଦି (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		ଯାଅ out;
	}

	ଯଦି (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		ଯାଅ out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_commit(ସଂରଚନା repository *repo,
			ସଂରଚନା commit *original,
			ସଂରଚନା pathspec *pathspec,
			ସଂରଚନା commit **out)
{
	ସଂରଚନା interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ସଂରଚନା strbuf index_file = STRBUF_INIT;
	ସଂରଚନା index_state index = INDEX_STATE_INIT(repo);
	ଅଚଳ ସଂରଚନା object_id *original_commit_ବୃକ୍ଷ_oid;
	ଅଚଳ ସଂରଚନା object_id *old_ବୃକ୍ଷ_oid, *new_ବୃକ୍ଷ_oid;
	ସଂରଚନା object_id parent_ବୃକ୍ଷ_oid;
	ଅକ୍ଷର original_commit_oid[GIT_MAX_HEXSZ + 1];
	ସଂରଚନା commit *first_commit, *second_commit;
	ସଂରଚନା commit_list *parents = ଖାଲି;
	ସଂରଚନା ବୃକ୍ଷ *split_ବୃକ୍ଷ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (first_parent_ବୃକ୍ଷ_oid(repo, original, &parent_ବୃକ୍ଷ_oid) < 0) {
		ret = -1;
		ଯାଅ out;
	}
	original_commit_ବୃକ୍ଷ_oid = get_commit_ବୃକ୍ଷ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_ବୃକ୍ଷ_oid, index_file.buf);
	ଯଦି (ret < 0)
		ଯାଅ out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	ଯଦି (ret < 0) {
		ret = error(_("failed reading temporary index"));
		ଯାଅ out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	ଯଦି (ret < 0)
		ଯାଅ out;

	split_ବୃକ୍ଷ = write_in_core_index_as_ବୃକ୍ଷ(repo, &index);
	ଯଦି (!split_ବୃକ୍ଷ) {
		ret = error(_("failed split tree"));
		ଯାଅ out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	ଯଦି (oideq(&split_ବୃକ୍ଷ->object.oid, &parent_ବୃକ୍ଷ_oid)) {
		ret = error(_("split commit is empty"));
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (oideq(&split_ବୃକ୍ଷ->object.oid, original_commit_ବୃକ୍ଷ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		ଯାଅ out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_ବୃକ୍ଷ_ext(repo, "split-out", original, ଖାଲି, original->parents, &parent_ବୃକ୍ଷ_oid,
			      &split_ବୃକ୍ଷ->object.oid, &first_commit, COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE);
	ଯଦି (ret < 0) {
		ret = error(_("failed writing first commit"));
		ଯାଅ out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_ବୃକ୍ଷ_oid = &repo_get_commit_ବୃକ୍ଷ(repo, first_commit)->object.oid;
	new_ବୃକ୍ଷ_oid = &repo_get_commit_ବୃକ୍ଷ(repo, original)->object.oid;

	ret = commit_ବୃକ୍ଷ_ext(repo, "split-out", original, ଖାଲି, parents, old_ବୃକ୍ଷ_oid,
			      new_ବୃକ୍ଷ_oid, &second_commit, COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE);
	ଯଦି (ret < 0) {
		ret = error(_("failed writing second commit"));
		ଯାଅ out;
	}

	*out = second_commit;
	ret = 0;

out:
	ଯଦି (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_history_split(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			     ଅଚଳ ଅକ୍ଷର **argv,
			     ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		ଖାଲି,
	};
	ଗଣନା ref_action action = REF_ACTION_DEFAULT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0;
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ସଂରଚନା commit *original, *rewritten = ଖାଲି;
	ସଂରଚନା strbuf reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା pathspec pathspec = { 0 };
	ସଂରଚନା rev_info revs = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଯଦି (argc < 1) {
		ret = error(_("command expects a committish"));
		ଯାଅ out;
	}
	repo_config(repo, git_default_config, ଖାଲି);

	ଯଦି (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	ଯଦି (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ଯାଅ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ଯଦି (ret < 0)
		ଯାଅ out;

	ଯଦି (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		ଯାଅ out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	ଯଦି (ret < 0)
		ଯାଅ out;

	strbuf_addf(&reflog_ସନ୍ଦେଶ, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_ସନ୍ଦେଶ.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	ଯଦି (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ଯାଅ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	ଫେରନ୍ତୁ ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

ସ୍ଥିର bool is_autosquash_subject(ଅଚଳ ଅକ୍ଷର *s)
{
	ଫେରନ୍ତୁ starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ସ୍ଥିର bool skip_one_autosquash_prefix(ଅଚଳ ଅକ୍ଷର *s, ଅଚଳ ଅକ୍ଷର **out)
{
	ଯଦି (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		ଯେତେବେଳେ (**out == ' ')
			(*out)++;
		ଫେରନ୍ତୁ true;
	}
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ truncate_message_to_subject(ସଂରଚନା strbuf *ସନ୍ଦେଶ)
{
	ଅଚଳ ଅକ୍ଷର *eos = strstr(ସନ୍ଦେଶ->buf, "\n\n");

	ଯଦି (eos)
		strbuf_setlen(ସନ୍ଦେଶ, eos - ସନ୍ଦେଶ->buf + 1);
}

ସଂରଚନା subject_data {
	ସଂରଚନା strintmap subjects;
	ସଂରଚନା strbuf subject;
	ସଂରଚନା strbuf squash_message;
	ଅଚଳ ଅକ୍ଷର *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

ସ୍ଥିର ଶୂନ୍ୟ subject_data_clear(ସଂରଚନା subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା squash_amend_message(ସଂରଚନା repository *repo,
				ସଂରଚନା commit *commit,
				ସଂରଚନା subject_data *data,
				unsigned flags)
{
	ଅଚଳ ଅକ୍ଷର *body = data->message + data->subject.len;

	ଯେତେବେଳେ (isspace(*body))
		body++;

	ଯଦି (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (data->edit_message) {
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (flags & SQUASH_AMEND_TARGET) {
		ଯଦି (starts_with(data->squash_message.buf, "squash!"))
			ଫେରନ୍ତୁ error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		ଯଦି (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		ଯଦି (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} ଅନ୍ୟଥା {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_ଧାଡ଼ି(&data->squash_message);
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା squash_squash_message(ସଂରଚନା repository *repo,
				ସଂରଚନା commit *commit,
				ସଂରଚନା subject_data *data,
				unsigned flags)
{
	ଅଚଳ ଅକ୍ଷର *body = data->message + data->subject.len;

	ଯେତେବେଳେ (isspace(*body))
		body++;

	ଯଦି (data->edit_message) {
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (flags & SQUASH_AMEND_TARGET) {
		ଯଦି (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		ଯଦି (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_ଧାଡ଼ି(&data->squash_message);
		} ଅନ୍ୟଥା {
			ଫେରନ୍ତୁ error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା squash_check_can_autosquash(ସଂରଚନା repository *repo,
				       ସଂରଚନା commit *commit,
				       ସଂରଚନା subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	ଯଦି (starts_with(data->subject.buf, "amend!"))
		ଫେରନ୍ତୁ squash_amend_message(repo, commit, data, flags);
	ଅନ୍ୟଥା ଯଦି (starts_with(data->subject.buf, "squash!"))
		ଫେରନ୍ତୁ squash_squash_message(repo, commit, data, flags);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା squash_check_autosquash_subject(ସଂରଚନା repository *repo,
					   ସଂରଚନା commit *commit,
					   ସଂରଚନା subject_data *data)
{
	ଅଚଳ ଅକ୍ଷର* s = data->subject.buf;
	ସଂରଚନା commit *target;
	ସଂରଚନା hashmap_iter iter;
	ସଂରଚନା strmap_entry *entry;
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
	ଯେତେବେଳେ (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		ଯଦି (flags)
			ଫେରନ୍ତୁ squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	ଯଦି (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		ଯଦି (!flags) {
			ଅଚଳ ଅକ୍ଷର *subject_start;
			ଅଚଳ ଅକ୍ଷର *buffer = repo_logmsg_reencode(repo, target,
								  ଖାଲି, ଖାଲି);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			ଅକ୍ଷର *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			ମୁକ୍ତ(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		ଯଦି (flags)
			ଫେରନ୍ତୁ squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		ଯେତେବେଳେ(skip_one_autosquash_prefix(s, &s)) {
			ଯଦି (starts_with(entry->ଚାବି, s)) {
				unsigned ମୂଲ୍ୟ = (intptr_t)entry->ମୂଲ୍ୟ;

				ଫେରନ୍ତୁ squash_check_can_autosquash(repo, commit,
								   data, ମୂଲ୍ୟ);
			}
		}
	}
	ଫେରନ୍ତୁ error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା squash_check_subject(ସଂରଚନା repository *repo,
				ସଂରଚନା commit *commit,
				ସଂରଚନା subject_data *data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅଚଳ ଅକ୍ଷର *buf = repo_logmsg_reencode(repo, commit, ଖାଲି, ଖାଲି);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	ଯଦି (!strintmap_get_size(&data->subjects)) {
		ଅଚଳ ଅକ୍ଷର *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_ଧାଡ଼ି(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} ଅନ୍ୟଥା ଯଦି (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} ଅନ୍ୟଥା {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା build_squash_message(ସଂରଚନା repository *repo,
				ଅଚଳ ସଂରଚନା strbuf *todo_buf,
				ସଂରଚନା strbuf *out);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା setup_squash_revisions(ସଂରଚନା repository *repo,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				  ସଂରଚନା rev_info *revs)
{
	repo_init_revisions(repo, revs, ଖାଲି);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, ଖାଲି);
	ଯଦି (argc > 1)
		ଫେରନ୍ତୁ error(_("unrecognized argument: %s"), argv[1]);

	ଯଦି (revs->reverse != 1 || revs->topo_order != 1 ||
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
	ପାଇଁ (size_t i = 0; i < revs->cmdline.nr; i++)
		ଯଦି (revs->cmdline.rev[i].flags & BOTTOM)
			ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା resolve_squash_range(ସଂରଚନା repository *repo,
				bool update_branches,
				bool edit_message,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
				ସଂରଚନା commit **oldest_out,
				ସଂରଚନା commit **tip_out,
				ଅକ୍ଷର **message_out)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା subject_data subject_data = SUBJECT_DATA_INIT;
	ସଂରଚନା commit *commit, *oldest = ଖାଲି, *tip = ଖାଲି;
	ସଂରଚନା strbuf todo_buf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, tip_ଗଣନା = 0;
	bool walk_started = false;
	ସଂରଚନା ref_filter filter = REF_FILTER_INIT;
	ସଂରଚନା ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	ଯଦି (ret < 0)
		ଯାଅ out;

	ଯଦି (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		ଯାଅ out;
	}
	walk_started = true;
	ଯେତେବେଳେ ((commit = get_revision(&revs))) {
		ସଂରଚନା commit_list *p;

		ଯଦି (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		ଯଦି (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			ଯାଅ out;
		}
		ପାଇଁ (p = commit->parents; oldest && p; p = p->next) {
			ସଂରଚନା commit_list *q;
			ସଂରଚନା object *o;
			bool seen;

			ଯଦି (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				ଯାଅ out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			ପାଇଁ (q = oldest->parents; !seen && q; q = q->next) {
				ଯଦି (p->item == q->item) {
					seen = true;
					commit_list_ଭର୍ତ୍ତି(commit, &filter.with_commit);
				}
			}
			ଯଦି (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				ଯାଅ out;
			}
			ଯଦି (o->flags & SQUASH_TIP) {
				tip_ଗଣନା--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		ଯଦି (!oldest) {
			commit_list_ଭର୍ତ୍ତି(commit, &filter.with_commit);
			oldest = commit;
		}
		ଯଦି (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			ଯାଅ out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_ଗଣନା++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	ଯଦି (!tip_ଗଣନା) {
		ret = error(_("the revision range is empty"));
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (tip_ଗଣନା != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ଭର୍ତ୍ତି(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	ଯଦି (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		ଯାଅ out;
	}
	ଯଦି (refs.nr) {
		ସଂରଚନା ref_format format = REF_FORMAT_INIT;
		ସଂରଚନା ref_sorting *sorting;
		ସଂରଚନା string_list sorting_options = STRING_LIST_INIT_DUP;
		ସଂରଚନା strbuf branches = STRBUF_INIT;
		ସଂରଚନା strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		ଯଦି (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			ଯଦି (format_ref_array_item(refs.ବସ୍ତୁଗୁଡ଼ିକ[i], &format,
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
		ଯାଅ out;
	}
	ଯଦି (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		ଯଦି (ret < 0)
			ଯାଅ out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, ଖାଲି);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	ଯଦି (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର bool amend_replaces_target(ସଂରଚନା todo_list *todo, ପୂର୍ଣ୍ଣସଂଖ୍ୟା target)
{
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = target + 1; i < todo->nr &&
				 todo->ବସ୍ତୁଗୁଡ଼ିକ[i].command != TODO_PICK; i++) {
		ଯଦି (todo->ବସ୍ତୁଗୁଡ଼ିକ[i].command == TODO_SQUASH)
			ଫେରନ୍ତୁ false;
		ଯଦି (todo->ବସ୍ତୁଗୁଡ଼ିକ[i].flags & TODO_REPLACE_FIXUP_ସନ୍ଦେଶ)
			ଫେରନ୍ତୁ true;
	}
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା build_squash_message(ସଂରଚନା repository *repo,
				ଅଚଳ ସଂରଚନା strbuf *todo_buf,
				ସଂରଚନା strbuf *out)
{
	ସଂରଚନା todo_list todo = TODO_LIST_INIT;
	ସଂରଚନା replay_opts opts = REPLAY_OPTS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_commits, ret;

	ଯଦି (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		ଯାଅ out;
	}

	nr_commits = todo.nr;
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nr_commits; i++) {
		ସଂରଚନା todo_item *item = &todo.ବସ୍ତୁଗୁଡ଼ିକ[i];
		ଅଚଳ ଅକ୍ଷର *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_ସନ୍ଦେଶ);
		ଯଦି (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		ଅନ୍ୟଥା
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, ଖାଲି, ଖାଲି);
		find_commit_subject(message, &body);

		ଯଦି (skip)
			commented_len = strlen(body);
		ଅନ୍ୟଥା ଯଦି (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		ଅନ୍ୟଥା
			commented_len = 0;

		ଯଦି (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_ଧାଡ଼ିଗୁଡ଼ିକ(out, body, commented_len, comment_ଧାଡ଼ି_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_ଧାଡ଼ି(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_history_squash(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			      ଅଚଳ ଅକ୍ଷର **argv,
			      ଅଚଳ ଅକ୍ଷର *prefix,
			      ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ଖାଲି,
	};
	ଗଣନା ref_action action = REF_ACTION_DEFAULT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit = 1;
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	ସଂରଚନା strbuf reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା commit *oldest, *tip, *rewritten;
	ଅଚଳ ସଂରଚନା object_id *base_ବୃକ୍ଷ_oid, *tip_ବୃକ୍ଷ_oid;
	ଅକ୍ଷର *message_template = ଖାଲି;
	ସଂରଚନା rev_info revs = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	ଯଦି (argc < 2) {
		ret = error(_("command expects a revision range"));
		ଯାଅ out;
	}
	repo_config(repo, git_default_config, ଖାଲି);

	ଯଦି (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_ସନ୍ଦେଶ, "squash: updating ");
	strbuf_join_argv(&reflog_ସନ୍ଦେଶ, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	ଯଦି (ret < 0)
		ଯାଅ out;

	ret = setup_revwalk(repo, action, tip, &revs);
	ଯଦି (ret < 0)
		ଯାଅ out;

	base_ବୃକ୍ଷ_oid = &repo_get_commit_ବୃକ୍ଷ(repo,
					oldest->parents->item)->object.oid;
	tip_ବୃକ୍ଷ_oid = &repo_get_commit_ବୃକ୍ଷ(repo, tip)->object.oid;

	ret = commit_ବୃକ୍ଷ_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_ବୃକ୍ଷ_oid, tip_ବୃକ୍ଷ_oid,
			      &rewritten,
			      edit ? COMMIT_ବୃକ୍ଷ_EDIT_MESSAGE : 0);
	ଯଦି (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		ଯାଅ out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_ସନ୍ଦେଶ.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	ଯଦି (ret < 0) {
		ret = error(_("failed replaying descendants"));
		ଯାଅ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	release_revisions(&revs);
	ମୁକ୍ତ(message_template);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_worktree(ସଂରଚନା repository *repo,
			   ଅଚଳ ସଂରଚନା commit *old_head,
			   ଅଚଳ ସଂରଚନା commit *new_head,
			   bool dry_run)
{
	ସଂରଚନା reset_working_ବୃକ୍ଷ_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	ଯଦି (dry_run)
		opts.flags |= RESET_WORKING_ବୃକ୍ଷ_DRY_RUN;
	ଫେରନ୍ତୁ reset_working_ବୃକ୍ଷ(repo, &opts);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_head_ବୃକ୍ଷ_change(ସଂରଚନା repository *repo,
				 ଅଚଳ ସଂରଚନା replay_result *result,
				 ସଂରଚନା commit **old_head,
				 ସଂରଚନା commit **new_head,
				 bool *changed)
{
	ଅଚଳ ସଂରଚନା replay_ref_update *head_update = ଖାଲି;
	ସଂରଚନା commit *old_head_commit, *new_head_commit;
	ସଂରଚନା ବୃକ୍ଷ *old_head_ବୃକ୍ଷ, *new_head_ବୃକ୍ଷ;
	ଅଚଳ ଅକ୍ଷର *head_target;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ଖାଲି, &head_flags);
	ଯଦି (!head_target)
		ଫେରନ୍ତୁ error(_("cannot look up HEAD"));

	ପାଇଁ (size_t i = 0; i < result->updates_nr; i++) {
		ଯଦି (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି (!head_update)
		ଫେରନ୍ତୁ 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	ଯଦି (!old_head_commit || !new_head_commit)
		ଫେରନ୍ତୁ error(_("cannot resolve HEAD commit"));

	old_head_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(repo, old_head_commit);
	new_head_ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(repo, new_head_commit);
	ଯଦି (!old_head_ବୃକ୍ଷ || !new_head_ବୃକ୍ଷ)
		ଫେରନ୍ତୁ error(_("cannot resolve tree for HEAD"));

	ଯଦି (oideq(&old_head_ବୃକ୍ଷ->object.oid, &new_head_ବୃକ୍ଷ->object.oid))
		ଫେରନ୍ତୁ 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_history_drop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			    ଅଚଳ ଅକ୍ଷର **argv,
			    ଅଚଳ ଅକ୍ଷର *prefix,
			    ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ଖାଲି,
	};
	ଗଣନା replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ଗଣନା ref_action action = REF_ACTION_DEFAULT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0;
	ସଂରଚନା option options[] = {
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
	ସଂରଚନା strbuf reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା commit *original, *rewritten;
	ସଂରଚନା rev_info revs = { 0 };
	ସଂରଚନା replay_result result = { 0 };
	ସଂରଚନା commit *old_head, *new_head;
	bool head_moves = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଯଦି (argc != 1) {
		ret = error(_("command expects a single revision"));
		ଯାଅ out;
	}
	repo_config(repo, git_default_config, ଖାଲି);

	ଯଦି (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ଯଦି (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		ଯାଅ out;
	}

	ଯଦି (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		ଯାଅ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ଯଦି (ret)
		ଯାଅ out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	ଯଦି (ret) {
		ret = error(_("failed replaying descendants"));
		ଯାଅ out;
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
	ଯଦି (!is_bare_repository(repo)) {
		ret = find_head_ବୃକ୍ଷ_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		ଯଦି (ret < 0)
			ଯାଅ out;

		ଯଦି (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			ଯାଅ out;
		}
	}

	strbuf_addf(&reflog_ସନ୍ଦେଶ, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_ସନ୍ଦେଶ.buf, dry_run);
	ଯଦି (ret < 0) {
		ret = error(_("failed to update references"));
		ଯାଅ out;
	}

	ଯଦି (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		ଯାଅ out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	release_revisions(&revs);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_history(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		ଅଚଳ ଅକ୍ଷର **argv,
		ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ଖାଲି,
	};
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
