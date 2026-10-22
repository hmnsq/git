#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "merge-ort.h"
#অন্তর্ভাব "oidmap.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "ref-filter.h"
#অন্তর্ভাব "replay.h"
#অন্তর্ভাব "reset.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "tree-walk.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "wt-status.h"

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

স্থির শূন্য change_তথ্য_free(শূন্য *util, ধ্রুবক অক্ষর *str UNUSED)
{
	কাঠামো wt_status_change_তথ্য *d = util;
	মুক্ত(d->rename_source);
	মুক্ত(d);
}

স্থির পূর্ণসংখ্যা fill_commit_message(কাঠামো repository *repo,
			       ধ্রুবক কাঠামো object_id *old_বৃক্ষ,
			       ধ্রুবক কাঠামো object_id *new_বৃক্ষ,
			       ধ্রুবক অক্ষর *default_message,
			       ধ্রুবক অক্ষর *action,
			       কাঠামো strbuf *out)
{
	ধ্রুবক অক্ষর *path = git_path_commit_editmsg();
	ধ্রুবক অক্ষর *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	কাঠামো wt_status s;

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
	যদি (!s.fp)
		ফেরত error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_সারি_str, hint, action, comment_সারি_str);
	যদি (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_বৃক্ষ, new_বৃক্ষ);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_তথ্য_free);
	যদি (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	যদি (launch_editor(path, out, ফাঁকা)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		ফেরত -1;
	}
	strbuf_stripspace(out, comment_সারি_str);

	cleanup_message(out, COMMIT_বার্তা_CLEANUP_ALL, 0);

	যদি (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ফেরত -1;
	}

	ফেরত 0;
}

গণনা commit_বৃক্ষ_flags {
	COMMIT_বৃক্ষ_EDIT_MESSAGE = (1 << 0),
};

স্থির পূর্ণসংখ্যা commit_বৃক্ষ_ext(কাঠামো repository *repo,
			   ধ্রুবক অক্ষর *action,
			   কাঠামো commit *commit_with_message,
			   ধ্রুবক অক্ষর *message_template,
			   ধ্রুবক কাঠামো commit_list *parents,
			   ধ্রুবক কাঠামো object_id *old_বৃক্ষ,
			   ধ্রুবক কাঠামো object_id *new_বৃক্ষ,
			   কাঠামো commit **out,
			   গণনা commit_বৃক্ষ_flags flags)
{
	ধ্রুবক অক্ষর *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ফাঁকা,
	};
	ধ্রুবক অক্ষর *original_message, *original_body, *ptr;
	কাঠামো commit_extra_header *original_extra_headers = ফাঁকা;
	কাঠামো strbuf commit_message = STRBUF_INIT;
	কাঠামো object_id rewritten_commit_oid;
	অক্ষর *original_author = ফাঁকা;
	আকার_t len;
	পূর্ণসংখ্যা ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, ফাঁকা, ফাঁকা);
	ptr = find_commit_header(original_message, "author", &len);
	যদি (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	যদি (!message_template)
		message_template = original_body;

	যদি (flags & COMMIT_বৃক্ষ_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_বৃক্ষ, new_বৃক্ষ,
					  message_template, action, &commit_message);
		যদি (ret < 0)
			যাও out;
	} অন্যথা {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_বৃক্ষ_extended(commit_message.buf, commit_message.len, new_বৃক্ষ,
				   parents, &rewritten_commit_oid, original_author,
				   ফাঁকা, ফাঁকা, original_extra_headers);
	যদি (ret < 0)
		যাও out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	মুক্ত(original_author);
	repo_unuse_commit_বাফার(repo, commit_with_message, original_message);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা first_parent_বৃক্ষ_oid(কাঠামো repository *repo,
				 কাঠামো commit *commit,
				 কাঠামো object_id *out)
{
	কাঠামো commit *parent = commit->parents ? commit->parents->item : ফাঁকা;

	যদি (!parent) {
		oidcpy(out, repo->hash_algo->empty_বৃক্ষ);
		ফেরত 0;
	}

	যদি (repo_parse_commit(repo, parent))
		ফেরত error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_বৃক্ষ(repo, parent)->object.oid);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা commit_বৃক্ষ_with_edited_message(কাঠামো repository *repo,
					   ধ্রুবক অক্ষর *action,
					   কাঠামো commit *original,
					   কাঠামো commit **out)
{
	কাঠামো object_id parent_বৃক্ষ_oid;
	ধ্রুবক কাঠামো object_id *বৃক্ষ_oid;

	বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, original)->object.oid;

	যদি (first_parent_বৃক্ষ_oid(repo, original, &parent_বৃক্ষ_oid) < 0)
		ফেরত -1;

	ফেরত commit_বৃক্ষ_ext(repo, action, original, ফাঁকা, original->parents,
			       &parent_বৃক্ষ_oid, বৃক্ষ_oid, out, COMMIT_বৃক্ষ_EDIT_MESSAGE);
}

গণনা ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_শীর্ষ,
};

স্থির পূর্ণসংখ্যা parse_ref_action(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *মান, পূর্ণসংখ্যা unset)
{
	গণনা ref_action *action = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, মান);
	যদি (!strcmp(মান, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} অন্যথা যদি (!strcmp(মান, "head")) {
		*action = REF_ACTION_শীর্ষ;
	} অন্যথা {
		ফেরত error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা revwalk_contains_merges(কাঠামো repository *repo,
				   ধ্রুবক কাঠামো strvec *revwalk_args)
{
	কাঠামো strvec args = STRVEC_INIT;
	কাঠামো rev_info revs;
	পূর্ণসংখ্যা ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ঠেলো(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, ফাঁকা);

	setup_revisions_from_strvec(&args, &revs, ফাঁকা);
	যদি (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	যদি (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		যাও out;
	}

	যদি (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		যাও out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা setup_revwalk(কাঠামো repository *repo,
			 গণনা ref_action action,
			 কাঠামো commit *original,
			 কাঠামো rev_info *revs)
{
	কাঠামো strvec args = STRVEC_INIT;
	পূর্ণসংখ্যা ret;

	repo_init_revisions(repo, revs, ফাঁকা);
	strvec_ঠেলো(&args, "ignored");
	strvec_ঠেলো(&args, "--reverse");
	strvec_ঠেলো(&args, "--topo-order");
	strvec_ঠেলো(&args, "--full-history");

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
	যদি (action == REF_ACTION_শীর্ষ) {
		কাঠামো commit_list *from_list = ফাঁকা;
		কাঠামো commit *শীর্ষ;

		শীর্ষ = lookup_commit_reference_by_name("HEAD");
		যদি (!শীর্ষ) {
			ret = error(_("cannot look up HEAD"));
			যাও out;
		}

		commit_list_ঢোকাও(original, &from_list);
		ret = repo_is_descendant_of(repo, শীর্ষ, from_list);
		commit_list_free(from_list);

		যদি (ret < 0) {
			ret = error(_("cannot determine descendance"));
			যাও out;
		} অন্যথা যদি (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			যাও out;
		}

		strvec_ঠেলো(&args, "HEAD");
	} অন্যথা {
		strvec_ঠেলো(&args, "--branches");
		strvec_ঠেলো(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	যদি (ret < 0)
		যাও out;

	setup_revisions_from_strvec(&args, revs, ফাঁকা);
	যদি (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা handle_ref_update(কাঠামো ref_transaction *transaction,
			     ধ্রুবক অক্ষর *refname,
			     ধ্রুবক কাঠামো object_id *new_oid,
			     ধ্রুবক কাঠামো object_id *old_oid,
			     ধ্রুবক অক্ষর *reflog_বার্তা,
			     কাঠামো strbuf *err)
{
	যদি (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		ফেরত 0;
	}

	ফেরত ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ফাঁকা, ফাঁকা, 0, reflog_বার্তা, err);
}

স্থির পূর্ণসংখ্যা compute_pending_ref_updates(কাঠামো rev_info *revs,
				       গণনা ref_action action,
				       কাঠামো commit *original,
				       কাঠামো commit *rewritten,
				       গণনা replay_empty_commit_action empty,
				       কাঠামো replay_ফলাফল *ফলাফল)
{
	ধ্রুবক কাঠামো name_decoration *decoration;
	কাঠামো replay_revisions_options opts = {
		.empty = empty,
	};
	অক্ষর hex[GIT_MAX_HEXSZ + 1];
	bool detached_শীর্ষ;
	পূর্ণসংখ্যা শীর্ষ_flags = 0;
	পূর্ণসংখ্যা ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ফাঁকা, &শীর্ষ_flags);
	detached_শীর্ষ = !(শীর্ষ_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, ফলাফল);
	যদি (ret)
		ফেরত ret;

	যদি (action != REF_ACTION_BRANCHES && action != REF_ACTION_শীর্ষ)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	জন্য (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->পরবর্তী)
	{
		যদি (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_শীর্ষ)
			চালু;

		যদি (action == REF_ACTION_শীর্ষ &&
		    decoration->type != DECORATION_REF_শীর্ষ)
			চালু;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		যদি (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_শীর্ষ &&
		    !detached_শীর্ষ)
			চালু;

		replay_ফলাফল_queue_update(ফলাফল, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা apply_pending_ref_updates(কাঠামো repository *repo,
				     ধ্রুবক কাঠামো replay_ফলাফল *ফলাফল,
				     ধ্রুবক অক্ষর *reflog_বার্তা,
				     পূর্ণসংখ্যা dry_run)
{
	কাঠামো ref_transaction *transaction = ফাঁকা;
	কাঠামো strbuf err = STRBUF_INIT;
	পূর্ণসংখ্যা ret;

	যদি (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		যদি (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			যাও out;
		}
	}

	জন্য (আকার_t i = 0; i < ফলাফল->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					ফলাফল->updates[i].refname,
					&ফলাফল->updates[i].new_oid,
					&ফলাফল->updates[i].old_oid,
					reflog_বার্তা, &err);
		যদি (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    ফলাফল->updates[i].refname, err.buf);
			যাও out;
		}
	}

	যদি (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		যাও out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা handle_reference_updates(কাঠামো rev_info *revs,
				    গণনা ref_action action,
				    কাঠামো commit *original,
				    কাঠামো commit *rewritten,
				    ধ্রুবক অক্ষর *reflog_বার্তা,
				    পূর্ণসংখ্যা dry_run,
				    গণনা replay_empty_commit_action empty)
{
	কাঠামো replay_ফলাফল ফলাফল = { 0 };
	পূর্ণসংখ্যা ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &ফলাফল);
	যদি (ret)
		যাও out;

	ret = apply_pending_ref_updates(revs->repo, &ফলাফল, reflog_বার্তা, dry_run);

out:
	replay_ফলাফল_release(&ফলাফল);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা commit_became_empty(কাঠামো repository *repo,
			       কাঠামো commit *original,
			       কাঠামো বৃক্ষ *ফলাফল)
{
	কাঠামো object_id parent_বৃক্ষ_oid;

	যদি (first_parent_বৃক্ষ_oid(repo, original, &parent_বৃক্ষ_oid) < 0)
		ফেরত -1;

	ফেরত oideq(&ফলাফল->object.oid, &parent_বৃক্ষ_oid);
}

স্থির পূর্ণসংখ্যা parse_opt_empty(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	গণনা replay_empty_commit_action *মান = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!strcmp(arg, "drop"))
		*মান = REPLAY_EMPTY_COMMIT_DROP;
	অন্যথা যদি (!strcmp(arg, "keep"))
		*মান = REPLAY_EMPTY_COMMIT_KEEP;
	অন্যথা যদি (!strcmp(arg, "abort"))
		*মান = REPLAY_EMPTY_COMMIT_ABORT;
	অন্যথা
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_history_fixup(পূর্ণসংখ্যা argc,
			     ধ্রুবক অক্ষর **argv,
			     ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ফাঁকা,
	};
	গণনা replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	গণনা ref_action action = REF_ACTION_DEFAULT;
	গণনা commit_বৃক্ষ_flags flags = 0;
	পূর্ণসংখ্যা dry_run = 0;
	কাঠামো option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_বৃক্ষ_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	কাঠামো merge_ফলাফল merge_ফলাফল = { 0 };
	কাঠামো merge_options merge_opts = { 0 };
	কাঠামো strbuf reflog_বার্তা = STRBUF_INIT;
	কাঠামো commit *শীর্ষ_commit, *original, *rewritten;
	কাঠামো বৃক্ষ *শীর্ষ_বৃক্ষ, *original_বৃক্ষ, *সূচক_বৃক্ষ;
	কাঠামো rev_info revs = { 0 };
	bool skip_commit = false;
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1) {
		ret = error(_("command expects a single revision"));
		যাও out;
	}
	repo_config(repo, git_default_config, ফাঁকা);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	যদি (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		যাও out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		যাও out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	শীর্ষ_commit = lookup_commit_reference_by_name("HEAD");
	যদি (!শীর্ষ_commit) {
		ret = error(_("cannot look up HEAD"));
		যাও out;
	}

	শীর্ষ_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, শীর্ষ_commit);
	যদি (!শীর্ষ_বৃক্ষ) {
		ret = error(_("cannot get tree for HEAD"));
		যাও out;
	}

	যদি (repo_read_সূচক(repo) < 0) {
		ret = error(_("unable to read index"));
		যাও out;
	}

	যদি (!repo_সূচক_has_changes(repo, শীর্ষ_বৃক্ষ, ফাঁকা)) {
		ret = error(_("nothing to fixup: no staged changes"));
		যাও out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	সূচক_বৃক্ষ = write_in_core_সূচক_as_বৃক্ষ(repo, repo->সূচক);
	যদি (!সূচক_বৃক্ষ) {
		ret = error(_("unable to write index as a tree"));
		যাও out;
	}

	original_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, original);
	যদি (!original_বৃক্ষ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		যাও out;
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
	merge_incore_nonrecursive(&merge_opts, শীর্ষ_বৃক্ষ,
				  original_বৃক্ষ, সূচক_বৃক্ষ, &merge_ফলাফল);

	যদি (merge_ফলাফল.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		যাও out;
	}

	যদি (!merge_ফলাফল.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		যাও out;
	}

	ret = commit_became_empty(repo, original, merge_ফলাফল.বৃক্ষ);
	যদি (ret < 0)
		যাও out;
	যদি (ret > 0) {
		নির্বাচন (empty) {
		ক্ষেত্র REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : ফাঁকা;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			যদি (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				যাও out;
			}

			skip_commit = true;
			বিরতি;
		ক্ষেত্র REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			বিরতি;
		ক্ষেত্র REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			যাও out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret)
		যাও out;

	যদি (!skip_commit) {
		ret = commit_বৃক্ষ_ext(repo, "fixup", original, ফাঁকা, original->parents,
				      &original_বৃক্ষ->object.oid, &merge_ফলাফল.বৃক্ষ->object.oid,
				      &rewritten, flags);
		যদি (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			যাও out;
		}
	}

	strbuf_addf(&reflog_বার্তা, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_বার্তা.buf, dry_run, empty);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		যাও out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_ফলাফল);
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_history_reword(পূর্ণসংখ্যা argc,
			      ধ্রুবক অক্ষর **argv,
			      ধ্রুবক অক্ষর *prefix,
			      কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ফাঁকা,
	};
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণসংখ্যা dry_run = 0;
	কাঠামো option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	কাঠামো strbuf reflog_বার্তা = STRBUF_INIT;
	কাঠামো commit *original, *rewritten;
	কাঠামো rev_info revs = { 0 };
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1) {
		ret = error(_("command expects a single revision"));
		যাও out;
	}
	repo_config(repo, git_default_config, ফাঁকা);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		যাও out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret)
		যাও out;

	ret = commit_বৃক্ষ_with_edited_message(repo, "reworded", original, &rewritten);
	যদি (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		যাও out;
	}

	strbuf_addf(&reflog_বার্তা, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_বার্তা.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		যাও out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা write_ondisk_সূচক(কাঠামো repository *repo,
			      কাঠামো object_id *oid,
			      ধ্রুবক অক্ষর *path)
{
	কাঠামো unpack_trees_options opts = { 0 };
	কাঠামো lock_file lock = LOCK_INIT;
	কাঠামো বৃক্ষ_desc বৃক্ষ_desc;
	কাঠামো সূচক_state সূচক;
	কাঠামো বৃক্ষ *বৃক্ষ;
	পূর্ণসংখ্যা ret;

	সূচক_state_init(&সূচক, repo);

	opts.শীর্ষ_idx = -1;
	opts.src_সূচক = &সূচক;
	opts.dst_সূচক = &সূচক;

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(repo, oid);
	যদি (!বৃক্ষ) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		যাও out;
	}
	init_বৃক্ষ_desc(&বৃক্ষ_desc, &বৃক্ষ->object.oid, বৃক্ষ->বাফার, বৃক্ষ->আকার_2);

	যদি (unpack_trees(1, &বৃক্ষ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		যাও out;
	}

	মৌলিক_cache_বৃক্ষ(repo, &সূচক, বৃক্ষ);

	যদি (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		যাও out;
	}

	যদি (write_locked_সূচক(&সূচক, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		যাও out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_সূচক(&সূচক);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা split_commit(কাঠামো repository *repo,
			কাঠামো commit *original,
			কাঠামো pathspec *pathspec,
			কাঠামো commit **out)
{
	কাঠামো interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	কাঠামো strbuf সূচক_file = STRBUF_INIT;
	কাঠামো সূচক_state সূচক = সূচক_STATE_INIT(repo);
	ধ্রুবক কাঠামো object_id *original_commit_বৃক্ষ_oid;
	ধ্রুবক কাঠামো object_id *old_বৃক্ষ_oid, *new_বৃক্ষ_oid;
	কাঠামো object_id parent_বৃক্ষ_oid;
	অক্ষর original_commit_oid[GIT_MAX_HEXSZ + 1];
	কাঠামো commit *first_commit, *second_commit;
	কাঠামো commit_list *parents = ফাঁকা;
	কাঠামো বৃক্ষ *split_বৃক্ষ;
	পূর্ণসংখ্যা ret;

	যদি (first_parent_বৃক্ষ_oid(repo, original, &parent_বৃক্ষ_oid) < 0) {
		ret = -1;
		যাও out;
	}
	original_commit_বৃক্ষ_oid = get_commit_বৃক্ষ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &সূচক_file, "%s", "history-split.index");

	ret = write_ondisk_সূচক(repo, &parent_বৃক্ষ_oid, সূচক_file.buf);
	যদি (ret < 0)
		যাও out;

	ret = read_সূচক_from(&সূচক, সূচক_file.buf, repo->gitdir);
	যদি (ret < 0) {
		ret = error(_("failed reading temporary index"));
		যাও out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_সূচক(repo, &সূচক, সূচক_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	যদি (ret < 0)
		যাও out;

	split_বৃক্ষ = write_in_core_সূচক_as_বৃক্ষ(repo, &সূচক);
	যদি (!split_বৃক্ষ) {
		ret = error(_("failed split tree"));
		যাও out;
	}

	unlink(সূচক_file.buf);
	strbuf_release(&সূচক_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	যদি (oideq(&split_বৃক্ষ->object.oid, &parent_বৃক্ষ_oid)) {
		ret = error(_("split commit is empty"));
		যাও out;
	} অন্যথা যদি (oideq(&split_বৃক্ষ->object.oid, original_commit_বৃক্ষ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		যাও out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_বৃক্ষ_ext(repo, "split-out", original, ফাঁকা, original->parents, &parent_বৃক্ষ_oid,
			      &split_বৃক্ষ->object.oid, &first_commit, COMMIT_বৃক্ষ_EDIT_MESSAGE);
	যদি (ret < 0) {
		ret = error(_("failed writing first commit"));
		যাও out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, first_commit)->object.oid;
	new_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, original)->object.oid;

	ret = commit_বৃক্ষ_ext(repo, "split-out", original, ফাঁকা, parents, old_বৃক্ষ_oid,
			      new_বৃক্ষ_oid, &second_commit, COMMIT_বৃক্ষ_EDIT_MESSAGE);
	যদি (ret < 0) {
		ret = error(_("failed writing second commit"));
		যাও out;
	}

	*out = second_commit;
	ret = 0;

out:
	যদি (সূচক_file.len)
		unlink(সূচক_file.buf);
	strbuf_release(&সূচক_file);
	commit_list_free(parents);
	release_সূচক(&সূচক);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_history_split(পূর্ণসংখ্যা argc,
			     ধ্রুবক অক্ষর **argv,
			     ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		ফাঁকা,
	};
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণসংখ্যা dry_run = 0;
	কাঠামো option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	কাঠামো commit *original, *rewritten = ফাঁকা;
	কাঠামো strbuf reflog_বার্তা = STRBUF_INIT;
	কাঠামো pathspec pathspec = { 0 };
	কাঠামো rev_info revs = { 0 };
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc < 1) {
		ret = error(_("command expects a committish"));
		যাও out;
	}
	repo_config(repo, git_default_config, ফাঁকা);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		যাও out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret < 0)
		যাও out;

	যদি (original->parents && original->parents->পরবর্তী) {
		ret = error(_("cannot split up merge commit"));
		যাও out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	যদি (ret < 0)
		যাও out;

	strbuf_addf(&reflog_বার্তা, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_বার্তা.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		যাও out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_বার্তা);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	ফেরত ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

স্থির bool is_autosquash_subject(ধ্রুবক অক্ষর *s)
{
	ফেরত starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

স্থির bool skip_one_autosquash_prefix(ধ্রুবক অক্ষর *s, ধ্রুবক অক্ষর **out)
{
	যদি (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		যখন (**out == ' ')
			(*out)++;
		ফেরত true;
	}
	ফেরত false;
}

স্থির শূন্য truncate_message_to_subject(কাঠামো strbuf *বার্তা)
{
	ধ্রুবক অক্ষর *eos = strstr(বার্তা->buf, "\n\n");

	যদি (eos)
		strbuf_setlen(বার্তা, eos - বার্তা->buf + 1);
}

কাঠামো subject_তথ্য {
	কাঠামো strintmap subjects;
	কাঠামো strbuf subject;
	কাঠামো strbuf squash_message;
	ধ্রুবক অক্ষর *message;
	bool edit_message;
};

#define SUBJECT_তথ্য_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

স্থির শূন্য subject_তথ্য_clear(কাঠামো subject_তথ্য *তথ্য)
{
	strintmap_clear(&তথ্য->subjects);
	strbuf_release(&তথ্য->subject);
	strbuf_release(&তথ্য->squash_message);
}

স্থির পূর্ণসংখ্যা squash_amend_message(কাঠামো repository *repo,
				কাঠামো commit *commit,
				কাঠামো subject_তথ্য *তথ্য,
				unsigned flags)
{
	ধ্রুবক অক্ষর *body = তথ্য->message + তথ্য->subject.len;

	যখন (isspace(*body))
		body++;

	যদি (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			তথ্য->subject.buf);
		ফেরত 0;
	}

	যদি (তথ্য->edit_message) {
		ফেরত 0;
	} অন্যথা যদি (flags & SQUASH_AMEND_TARGET) {
		যদি (starts_with(তথ্য->squash_message.buf, "squash!"))
			ফেরত error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     তথ্য->subject.buf);
		যদি (starts_with(তথ্য->squash_message.buf, "fixup!"))
			strbuf_splice(&তথ্য->squash_message, 0, 5, "amend!", 5);
		যদি (starts_with(তথ্য->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&তথ্য->squash_message);
			strbuf_addch(&তথ্য->squash_message, '\n');
		} অন্যথা {
			strbuf_reset(&তথ্য->squash_message);
		}
		strbuf_addstr(&তথ্য->squash_message, body);
		strbuf_complete_সারি(&তথ্য->squash_message);
	} অন্যথা {
		ফেরত error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     তথ্য->subject.buf);
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা squash_squash_message(কাঠামো repository *repo,
				কাঠামো commit *commit,
				কাঠামো subject_তথ্য *তথ্য,
				unsigned flags)
{
	ধ্রুবক অক্ষর *body = তথ্য->message + তথ্য->subject.len;

	যখন (isspace(*body))
		body++;

	যদি (তথ্য->edit_message) {
		ফেরত 0;
	} অন্যথা যদি (flags & SQUASH_AMEND_TARGET) {
		যদি (starts_with(তথ্য->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&তথ্য->squash_message);
			strbuf_splice(&তথ্য->squash_message, 0, 5, "squash", 6);
		}
		যদি (starts_with(তথ্য->squash_message.buf, "squash!")) {
			strbuf_addch(&তথ্য->squash_message, '\n');
			strbuf_addstr(&তথ্য->squash_message, body);
			strbuf_complete_সারি(&তথ্য->squash_message);
		} অন্যথা {
			ফেরত error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     তথ্য->subject.buf);
		}
	} অন্যথা {
		ফেরত error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      তথ্য->subject.buf);
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা squash_check_can_autosquash(কাঠামো repository *repo,
				       কাঠামো commit *commit,
				       কাঠামো subject_তথ্য *তথ্য,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	যদি (starts_with(তথ্য->subject.buf, "amend!"))
		ফেরত squash_amend_message(repo, commit, তথ্য, flags);
	অন্যথা যদি (starts_with(তথ্য->subject.buf, "squash!"))
		ফেরত squash_squash_message(repo, commit, তথ্য, flags);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা squash_check_autosquash_subject(কাঠামো repository *repo,
					   কাঠামো commit *commit,
					   কাঠামো subject_তথ্য *তথ্য)
{
	ধ্রুবক অক্ষর* s = তথ্য->subject.buf;
	কাঠামো commit *target;
	কাঠামো hashmap_iter iter;
	কাঠামো strmap_entry *entry;
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
	যখন (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&তথ্য->subjects, s);
		যদি (flags)
			ফেরত squash_check_can_autosquash(repo, commit, তথ্য, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	যদি (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		যদি (!flags) {
			ধ্রুবক অক্ষর *subject_start;
			ধ্রুবক অক্ষর *বাফার = repo_logmsg_reencode(repo, target,
								  ফাঁকা, ফাঁকা);
			আকার_t subject_len = find_commit_subject(বাফার,
								 &subject_start);
			অক্ষর *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&তথ্য->subjects, subject);
			মুক্ত(subject);
			repo_unuse_commit_বাফার(repo, target, বাফার);
		}
		যদি (flags)
			ফেরত squash_check_can_autosquash(repo, commit,
							   তথ্য, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&তথ্য->subjects, &iter, entry) {
		s = তথ্য->subject.buf;
		যখন(skip_one_autosquash_prefix(s, &s)) {
			যদি (starts_with(entry->চাবি, s)) {
				unsigned মান = (intptr_t)entry->মান;

				ফেরত squash_check_can_autosquash(repo, commit,
								   তথ্য, মান);
			}
		}
	}
	ফেরত error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       তথ্য->subject.buf);
}

স্থির পূর্ণসংখ্যা squash_check_subject(কাঠামো repository *repo,
				কাঠামো commit *commit,
				কাঠামো subject_তথ্য *তথ্য)
{
	পূর্ণসংখ্যা ret = 0;
	ধ্রুবক অক্ষর *buf = repo_logmsg_reencode(repo, commit, ফাঁকা, ফাঁকা);
	আকার_t subject_len = find_commit_subject(buf, &তথ্য->message);

	strbuf_reset(&তথ্য->subject);
	strbuf_add(&তথ্য->subject, তথ্য->message, subject_len);

	যদি (!strintmap_get_আকার(&তথ্য->subjects)) {
		ধ্রুবক অক্ষর *s;

		strbuf_addstr(&তথ্য->squash_message, তথ্য->message);
		strbuf_complete_সারি(&তথ্য->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = তথ্য->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&তথ্য->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} অন্যথা যদি (is_autosquash_subject(তথ্য->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, তথ্য);
	} অন্যথা {
		strintmap_set(&তথ্য->subjects, তথ্য->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_বাফার(repo, commit, buf);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা build_squash_message(কাঠামো repository *repo,
				ধ্রুবক কাঠামো strbuf *todo_buf,
				কাঠামো strbuf *out);

স্থির পূর্ণসংখ্যা setup_squash_revisions(কাঠামো repository *repo,
				  পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				  কাঠামো rev_info *revs)
{
	repo_init_revisions(repo, revs, ফাঁকা);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, ফাঁকা);
	যদি (argc > 1)
		ফেরত error(_("unrecognized argument: %s"), argv[1]);

	যদি (revs->reverse != 1 || revs->topo_order != 1 ||
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
	জন্য (আকার_t i = 0; i < revs->cmdline.nr; i++)
		যদি (revs->cmdline.rev[i].flags & BOTTOM)
			ফেরত 0;

	ফেরত error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
স্থির পূর্ণসংখ্যা resolve_squash_range(কাঠামো repository *repo,
				bool update_branches,
				bool edit_message,
				পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv,
				কাঠামো commit **oldest_out,
				কাঠামো commit **tip_out,
				অক্ষর **message_out)
{
	কাঠামো rev_info revs;
	কাঠামো subject_তথ্য subject_তথ্য = SUBJECT_তথ্য_INIT;
	কাঠামো commit *commit, *oldest = ফাঁকা, *tip = ফাঁকা;
	কাঠামো strbuf todo_buf = STRBUF_INIT;
	পূর্ণসংখ্যা ret, tip_গুনতি = 0;
	bool walk_started = false;
	কাঠামো ref_filter filter = REF_FILTER_INIT;
	কাঠামো ref_array refs = { 0 };

	subject_তথ্য.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	যদি (ret < 0)
		যাও out;

	যদি (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		যাও out;
	}
	walk_started = true;
	যখন ((commit = get_revision(&revs))) {
		কাঠামো commit_list *p;

		যদি (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		যদি (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			যাও out;
		}
		জন্য (p = commit->parents; oldest && p; p = p->পরবর্তী) {
			কাঠামো commit_list *q;
			কাঠামো object *o;
			bool seen;

			যদি (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				যাও out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			জন্য (q = oldest->parents; !seen && q; q = q->পরবর্তী) {
				যদি (p->item == q->item) {
					seen = true;
					commit_list_ঢোকাও(commit, &filter.with_commit);
				}
			}
			যদি (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				যাও out;
			}
			যদি (o->flags & SQUASH_TIP) {
				tip_গুনতি--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		যদি (!oldest) {
			commit_list_ঢোকাও(commit, &filter.with_commit);
			oldest = commit;
		}
		যদি (squash_check_subject(repo, commit, &subject_তথ্য)) {
			ret = -1;
			যাও out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_গুনতি++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	যদি (!tip_গুনতি) {
		ret = error(_("the revision range is empty"));
		যাও out;
	} অন্যথা যদি (tip_গুনতি != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		যাও out;
	} অন্যথা যদি (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		যাও out;
	} অন্যথা যদি (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ঢোকাও(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	যদি (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		যাও out;
	}
	যদি (refs.nr) {
		কাঠামো ref_format format = REF_FORMAT_INIT;
		কাঠামো ref_sorting *sorting;
		কাঠামো string_list sorting_options = STRING_LIST_INIT_DUP;
		কাঠামো strbuf branches = STRBUF_INIT;
		কাঠামো strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		যদি (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		জন্য (পূর্ণসংখ্যা i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			যদি (format_ref_array_item(refs.বস্তুসমূহ[i], &format,
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
		যাও out;
	}
	যদি (edit_message) {
		strbuf_reset(&subject_তথ্য.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_তথ্য.squash_message);
		যদি (ret < 0)
			যাও out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_তথ্য.squash_message, ফাঁকা);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	যদি (walk_started)
		reset_revision_walk();
	subject_তথ্য_clear(&subject_তথ্য);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	ফেরত ret;
}

স্থির bool amend_replaces_target(কাঠামো todo_list *todo, পূর্ণসংখ্যা target)
{
	জন্য (পূর্ণসংখ্যা i = target + 1; i < todo->nr &&
				 todo->বস্তুসমূহ[i].command != TODO_PICK; i++) {
		যদি (todo->বস্তুসমূহ[i].command == TODO_SQUASH)
			ফেরত false;
		যদি (todo->বস্তুসমূহ[i].flags & TODO_REPLACE_FIXUP_বার্তা)
			ফেরত true;
	}
	ফেরত false;
}

স্থির পূর্ণসংখ্যা build_squash_message(কাঠামো repository *repo,
				ধ্রুবক কাঠামো strbuf *todo_buf,
				কাঠামো strbuf *out)
{
	কাঠামো todo_list todo = TODO_LIST_INIT;
	কাঠামো replay_opts opts = REPLAY_OPTS_INIT;
	পূর্ণসংখ্যা nr_commits, ret;

	যদি (todo_list_parse_insn_বাফার(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		যাও out;
	}

	nr_commits = todo.nr;
	জন্য (পূর্ণসংখ্যা i = 0; i < nr_commits; i++) {
		কাঠামো todo_item *item = &todo.বস্তুসমূহ[i];
		ধ্রুবক অক্ষর *message, *body;
		আকার_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_বার্তা);
		যদি (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		অন্যথা
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, ফাঁকা, ফাঁকা);
		find_commit_subject(message, &body);

		যদি (skip)
			commented_len = strlen(body);
		অন্যথা যদি (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		অন্যথা
			commented_len = 0;

		যদি (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_সারিগুলি(out, body, commented_len, comment_সারি_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_সারি(out);

		repo_unuse_commit_বাফার(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_history_squash(পূর্ণসংখ্যা argc,
			      ধ্রুবক অক্ষর **argv,
			      ধ্রুবক অক্ষর *prefix,
			      কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ফাঁকা,
	};
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণসংখ্যা dry_run = 0;
	পূর্ণসংখ্যা edit = 1;
	কাঠামো option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	কাঠামো strbuf reflog_বার্তা = STRBUF_INIT;
	কাঠামো commit *oldest, *tip, *rewritten;
	ধ্রুবক কাঠামো object_id *base_বৃক্ষ_oid, *tip_বৃক্ষ_oid;
	অক্ষর *message_template = ফাঁকা;
	কাঠামো rev_info revs = { 0 };
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	যদি (argc < 2) {
		ret = error(_("command expects a revision range"));
		যাও out;
	}
	repo_config(repo, git_default_config, ফাঁকা);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_বার্তা, "squash: updating ");
	strbuf_join_argv(&reflog_বার্তা, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	যদি (ret < 0)
		যাও out;

	ret = setup_revwalk(repo, action, tip, &revs);
	যদি (ret < 0)
		যাও out;

	base_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo,
					oldest->parents->item)->object.oid;
	tip_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, tip)->object.oid;

	ret = commit_বৃক্ষ_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_বৃক্ষ_oid, tip_বৃক্ষ_oid,
			      &rewritten,
			      edit ? COMMIT_বৃক্ষ_EDIT_MESSAGE : 0);
	যদি (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		যাও out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_বার্তা.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		যাও out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	মুক্ত(message_template);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা update_worktree(কাঠামো repository *repo,
			   ধ্রুবক কাঠামো commit *old_শীর্ষ,
			   ধ্রুবক কাঠামো commit *new_শীর্ষ,
			   bool dry_run)
{
	কাঠামো reset_working_বৃক্ষ_options opts = {
		.oid_from = &old_শীর্ষ->object.oid,
		.oid = &new_শীর্ষ->object.oid,
	};
	যদি (dry_run)
		opts.flags |= RESET_WORKING_বৃক্ষ_DRY_RUN;
	ফেরত reset_working_বৃক্ষ(repo, &opts);
}

স্থির পূর্ণসংখ্যা find_শীর্ষ_বৃক্ষ_change(কাঠামো repository *repo,
				 ধ্রুবক কাঠামো replay_ফলাফল *ফলাফল,
				 কাঠামো commit **old_শীর্ষ,
				 কাঠামো commit **new_শীর্ষ,
				 bool *changed)
{
	ধ্রুবক কাঠামো replay_ref_update *শীর্ষ_update = ফাঁকা;
	কাঠামো commit *old_শীর্ষ_commit, *new_শীর্ষ_commit;
	কাঠামো বৃক্ষ *old_শীর্ষ_বৃক্ষ, *new_শীর্ষ_বৃক্ষ;
	ধ্রুবক অক্ষর *শীর্ষ_target;
	পূর্ণসংখ্যা শীর্ষ_flags;

	*changed = false;

	শীর্ষ_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ফাঁকা, &শীর্ষ_flags);
	যদি (!শীর্ষ_target)
		ফেরত error(_("cannot look up HEAD"));

	জন্য (আকার_t i = 0; i < ফলাফল->updates_nr; i++) {
		যদি (!strcmp(ফলাফল->updates[i].refname, শীর্ষ_target)) {
			শীর্ষ_update = &ফলাফল->updates[i];
			বিরতি;
		}
	}

	যদি (!শীর্ষ_update)
		ফেরত 0;

	old_শীর্ষ_commit = lookup_commit_reference(repo, &শীর্ষ_update->old_oid);
	new_শীর্ষ_commit = lookup_commit_reference(repo, &শীর্ষ_update->new_oid);
	যদি (!old_শীর্ষ_commit || !new_শীর্ষ_commit)
		ফেরত error(_("cannot resolve HEAD commit"));

	old_শীর্ষ_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, old_শীর্ষ_commit);
	new_শীর্ষ_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, new_শীর্ষ_commit);
	যদি (!old_শীর্ষ_বৃক্ষ || !new_শীর্ষ_বৃক্ষ)
		ফেরত error(_("cannot resolve tree for HEAD"));

	যদি (oideq(&old_শীর্ষ_বৃক্ষ->object.oid, &new_শীর্ষ_বৃক্ষ->object.oid))
		ফেরত 0;

	*old_শীর্ষ = old_শীর্ষ_commit;
	*new_শীর্ষ = new_শীর্ষ_commit;
	*changed = true;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cmd_history_drop(পূর্ণসংখ্যা argc,
			    ধ্রুবক অক্ষর **argv,
			    ধ্রুবক অক্ষর *prefix,
			    কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ফাঁকা,
	};
	গণনা replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণসংখ্যা dry_run = 0;
	কাঠামো option options[] = {
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
	কাঠামো strbuf reflog_বার্তা = STRBUF_INIT;
	কাঠামো commit *original, *rewritten;
	কাঠামো rev_info revs = { 0 };
	কাঠামো replay_ফলাফল ফলাফল = { 0 };
	কাঠামো commit *old_শীর্ষ, *new_শীর্ষ;
	bool শীর্ষ_moves = false;
	পূর্ণসংখ্যা ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1) {
		ret = error(_("command expects a single revision"));
		যাও out;
	}
	repo_config(repo, git_default_config, ফাঁকা);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		যাও out;
	}

	যদি (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		যাও out;
	} অন্যথা যদি (original->parents->পরবর্তী) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		যাও out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret)
		যাও out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &ফলাফল);
	যদি (ret) {
		ret = error(_("failed replaying descendants"));
		যাও out;
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
	যদি (!is_bare_repository(repo)) {
		ret = find_শীর্ষ_বৃক্ষ_change(repo, &ফলাফল, &old_শীর্ষ,
					    &new_শীর্ষ, &শীর্ষ_moves);
		যদি (ret < 0)
			যাও out;

		যদি (শীর্ষ_moves && update_worktree(repo, old_শীর্ষ, new_শীর্ষ, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			যাও out;
		}
	}

	strbuf_addf(&reflog_বার্তা, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &ফলাফল, reflog_বার্তা.buf, dry_run);
	যদি (ret < 0) {
		ret = error(_("failed to update references"));
		যাও out;
	}

	যদি (!dry_run && শীর্ষ_moves && update_worktree(repo, old_শীর্ষ, new_শীর্ষ, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_শীর্ষ->object.oid));
		যাও out;
	}

	ret = 0;

out:
	replay_ফলাফল_release(&ফলাফল);
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	ফেরত ret;
}

পূর্ণসংখ্যা cmd_history(পূর্ণসংখ্যা argc,
		ধ্রুবক অক্ষর **argv,
		ধ্রুবক অক্ষর *prefix,
		কাঠামো repository *repo)
{
	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ফাঁকা,
	};
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ফেরত fn(argc, argv, prefix, repo);
}
