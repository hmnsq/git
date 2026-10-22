#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "cache-tree.h"
#شامل "commit.h"
#شامل "commit-reach.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "merge-ort.h"
#شامل "oidmap.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "read-cache.h"
#شامل "refs.h"
#شامل "ref-filter.h"
#شامل "replay.h"
#شامل "reset.h"
#شامل "revision.h"
#شامل "sequencer.h"
#شامل "string-list.h"
#شامل "strvec.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "unpack-trees.h"
#شامل "wt-status.h"

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

ایستا تهی change_data_free(تهی *util, ثابت نویسه *str UNUSED)
{
	ساختار wt_status_change_data *d = util;
	آزاد(d->rename_source);
	آزاد(d);
}

ایستا صحیح fill_commit_message(ساختار repository *repo,
			       ثابت ساختار object_id *old_درخت,
			       ثابت ساختار object_id *new_درخت,
			       ثابت نویسه *default_message,
			       ثابت نویسه *action,
			       ساختار strbuf *out)
{
	ثابت نویسه *path = git_path_commit_editmsg();
	ثابت نویسه *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ساختار wt_status s;

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
	اگر (!s.fp)
		بازگشت error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_سطر_str, hint, action, comment_سطر_str);
	اگر (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_درخت, new_درخت);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	اگر (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	اگر (launch_editor(path, out, پوچ)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		بازگشت -1;
	}
	strbuf_stripspace(out, comment_سطر_str);

	cleanup_message(out, COMMIT_پیام_CLEANUP_ALL, 0);

	اگر (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		بازگشت -1;
	}

	بازگشت 0;
}

شمارش commit_درخت_flags {
	COMMIT_درخت_EDIT_MESSAGE = (1 << 0),
};

ایستا صحیح commit_درخت_ext(ساختار repository *repo,
			   ثابت نویسه *action,
			   ساختار commit *commit_with_message,
			   ثابت نویسه *message_template,
			   ثابت ساختار commit_list *parents,
			   ثابت ساختار object_id *old_درخت,
			   ثابت ساختار object_id *new_درخت,
			   ساختار commit **out,
			   شمارش commit_درخت_flags flags)
{
	ثابت نویسه *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		پوچ,
	};
	ثابت نویسه *original_message, *original_body, *ptr;
	ساختار commit_extra_header *original_extra_headers = پوچ;
	ساختار strbuf commit_message = STRBUF_INIT;
	ساختار object_id rewritten_commit_oid;
	نویسه *original_author = پوچ;
	size_t len;
	صحیح ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, پوچ, پوچ);
	ptr = find_commit_header(original_message, "author", &len);
	اگر (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	اگر (!message_template)
		message_template = original_body;

	اگر (flags & COMMIT_درخت_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_درخت, new_درخت,
					  message_template, action, &commit_message);
		اگر (ret < 0)
			برو out;
	} وگرنه {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_درخت_extended(commit_message.buf, commit_message.len, new_درخت,
				   parents, &rewritten_commit_oid, original_author,
				   پوچ, پوچ, original_extra_headers);
	اگر (ret < 0)
		برو out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	آزاد(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	بازگشت ret;
}

ایستا صحیح first_parent_درخت_oid(ساختار repository *repo,
				 ساختار commit *commit,
				 ساختار object_id *out)
{
	ساختار commit *parent = commit->parents ? commit->parents->item : پوچ;

	اگر (!parent) {
		oidcpy(out, repo->hash_algo->empty_درخت);
		بازگشت 0;
	}

	اگر (repo_parse_commit(repo, parent))
		بازگشت error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_درخت(repo, parent)->object.oid);
	بازگشت 0;
}

ایستا صحیح commit_درخت_with_edited_message(ساختار repository *repo,
					   ثابت نویسه *action,
					   ساختار commit *original,
					   ساختار commit **out)
{
	ساختار object_id parent_درخت_oid;
	ثابت ساختار object_id *درخت_oid;

	درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	اگر (first_parent_درخت_oid(repo, original, &parent_درخت_oid) < 0)
		بازگشت -1;

	بازگشت commit_درخت_ext(repo, action, original, پوچ, original->parents,
			       &parent_درخت_oid, درخت_oid, out, COMMIT_درخت_EDIT_MESSAGE);
}

شمارش ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

ایستا صحیح parse_ref_action(ثابت ساختار option *opt, ثابت نویسه *مقدار, صحیح unset)
{
	شمارش ref_action *action = opt->مقدار;

	BUG_ON_OPT_NEG_NOARG(unset, مقدار);
	اگر (!strcmp(مقدار, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} وگرنه اگر (!strcmp(مقدار, "head")) {
		*action = REF_ACTION_HEAD;
	} وگرنه {
		بازگشت error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	بازگشت 0;
}

ایستا صحیح revwalk_contains_merges(ساختار repository *repo,
				   ثابت ساختار strvec *revwalk_args)
{
	ساختار strvec args = STRVEC_INIT;
	ساختار rev_info revs;
	صحیح ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_افزودن(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, پوچ);

	setup_revisions_from_strvec(&args, &revs, پوچ);
	اگر (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	اگر (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		برو out;
	}

	اگر (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		برو out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	بازگشت ret;
}

ایستا صحیح setup_revwalk(ساختار repository *repo,
			 شمارش ref_action action,
			 ساختار commit *original,
			 ساختار rev_info *revs)
{
	ساختار strvec args = STRVEC_INIT;
	صحیح ret;

	repo_init_revisions(repo, revs, پوچ);
	strvec_افزودن(&args, "ignored");
	strvec_افزودن(&args, "--reverse");
	strvec_افزودن(&args, "--topo-order");
	strvec_افزودن(&args, "--full-history");

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
	اگر (action == REF_ACTION_HEAD) {
		ساختار commit_list *from_list = پوچ;
		ساختار commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		اگر (!head) {
			ret = error(_("cannot look up HEAD"));
			برو out;
		}

		commit_list_درج(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		اگر (ret < 0) {
			ret = error(_("cannot determine descendance"));
			برو out;
		} وگرنه اگر (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			برو out;
		}

		strvec_افزودن(&args, "HEAD");
	} وگرنه {
		strvec_افزودن(&args, "--branches");
		strvec_افزودن(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	اگر (ret < 0)
		برو out;

	setup_revisions_from_strvec(&args, revs, پوچ);
	اگر (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	بازگشت ret;
}

ایستا صحیح handle_ref_update(ساختار ref_transaction *transaction,
			     ثابت نویسه *refname,
			     ثابت ساختار object_id *new_oid,
			     ثابت ساختار object_id *old_oid,
			     ثابت نویسه *reflog_پیام,
			     ساختار strbuf *err)
{
	اگر (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		بازگشت 0;
	}

	بازگشت ref_transaction_update(transaction, refname, new_oid, old_oid,
				      پوچ, پوچ, 0, reflog_پیام, err);
}

ایستا صحیح compute_pending_ref_updates(ساختار rev_info *revs,
				       شمارش ref_action action,
				       ساختار commit *original,
				       ساختار commit *rewritten,
				       شمارش replay_empty_commit_action empty,
				       ساختار replay_result *result)
{
	ثابت ساختار name_decoration *decoration;
	ساختار replay_revisions_options opts = {
		.empty = empty,
	};
	نویسه hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	صحیح head_flags = 0;
	صحیح ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, پوچ, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	اگر (ret)
		بازگشت ret;

	اگر (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	برای (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		اگر (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			ادامه;

		اگر (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			ادامه;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		اگر (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			ادامه;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	بازگشت 0;
}

ایستا صحیح apply_pending_ref_updates(ساختار repository *repo,
				     ثابت ساختار replay_result *result,
				     ثابت نویسه *reflog_پیام,
				     صحیح dry_run)
{
	ساختار ref_transaction *transaction = پوچ;
	ساختار strbuf err = STRBUF_INIT;
	صحیح ret;

	اگر (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		اگر (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			برو out;
		}
	}

	برای (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_پیام, &err);
		اگر (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			برو out;
		}
	}

	اگر (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		برو out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	بازگشت ret;
}

ایستا صحیح handle_reference_updates(ساختار rev_info *revs,
				    شمارش ref_action action,
				    ساختار commit *original,
				    ساختار commit *rewritten,
				    ثابت نویسه *reflog_پیام,
				    صحیح dry_run,
				    شمارش replay_empty_commit_action empty)
{
	ساختار replay_result result = { 0 };
	صحیح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	اگر (ret)
		برو out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_پیام, dry_run);

out:
	replay_result_release(&result);
	بازگشت ret;
}

ایستا صحیح commit_became_empty(ساختار repository *repo,
			       ساختار commit *original,
			       ساختار درخت *result)
{
	ساختار object_id parent_درخت_oid;

	اگر (first_parent_درخت_oid(repo, original, &parent_درخت_oid) < 0)
		بازگشت -1;

	بازگشت oideq(&result->object.oid, &parent_درخت_oid);
}

ایستا صحیح parse_opt_empty(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	شمارش replay_empty_commit_action *مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "drop"))
		*مقدار = REPLAY_EMPTY_COMMIT_DROP;
	وگرنه اگر (!strcmp(arg, "keep"))
		*مقدار = REPLAY_EMPTY_COMMIT_KEEP;
	وگرنه اگر (!strcmp(arg, "abort"))
		*مقدار = REPLAY_EMPTY_COMMIT_ABORT;
	وگرنه
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	بازگشت 0;
}

ایستا صحیح cmd_history_fixup(صحیح argc,
			     ثابت نویسه **argv,
			     ثابت نویسه *prefix,
			     ساختار repository *repo)
{
	ثابت نویسه * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		پوچ,
	};
	شمارش replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	شمارش ref_action action = REF_ACTION_DEFAULT;
	شمارش commit_درخت_flags flags = 0;
	صحیح dry_run = 0;
	ساختار option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_درخت_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	ساختار merge_result merge_result = { 0 };
	ساختار merge_options merge_opts = { 0 };
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	ساختار commit *head_commit, *original, *rewritten;
	ساختار درخت *head_درخت, *original_درخت, *index_درخت;
	ساختار rev_info revs = { 0 };
	bool skip_commit = false;
	صحیح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	اگر (argc != 1) {
		ret = error(_("command expects a single revision"));
		برو out;
	}
	repo_config(repo, git_default_config, پوچ);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	اگر (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		برو out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		برو out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	اگر (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		برو out;
	}

	head_درخت = repo_get_commit_درخت(repo, head_commit);
	اگر (!head_درخت) {
		ret = error(_("cannot get tree for HEAD"));
		برو out;
	}

	اگر (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		برو out;
	}

	اگر (!repo_index_has_changes(repo, head_درخت, پوچ)) {
		ret = error(_("nothing to fixup: no staged changes"));
		برو out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_درخت = write_in_core_index_as_درخت(repo, repo->index);
	اگر (!index_درخت) {
		ret = error(_("unable to write index as a tree"));
		برو out;
	}

	original_درخت = repo_get_commit_درخت(repo, original);
	اگر (!original_درخت) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		برو out;
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
	merge_incore_nonrecursive(&merge_opts, head_درخت,
				  original_درخت, index_درخت, &merge_result);

	اگر (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		برو out;
	}

	اگر (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		برو out;
	}

	ret = commit_became_empty(repo, original, merge_result.درخت);
	اگر (ret < 0)
		برو out;
	اگر (ret > 0) {
		انتخاب (empty) {
		حالت REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : پوچ;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			اگر (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				برو out;
			}

			skip_commit = true;
			بشکن;
		حالت REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			بشکن;
		حالت REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			برو out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	اگر (ret)
		برو out;

	اگر (!skip_commit) {
		ret = commit_درخت_ext(repo, "fixup", original, پوچ, original->parents,
				      &original_درخت->object.oid, &merge_result.درخت->object.oid,
				      &rewritten, flags);
		اگر (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			برو out;
		}
	}

	strbuf_addf(&reflog_پیام, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیام.buf, dry_run, empty);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		برو out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	بازگشت ret;
}

ایستا صحیح cmd_history_reword(صحیح argc,
			      ثابت نویسه **argv,
			      ثابت نویسه *prefix,
			      ساختار repository *repo)
{
	ثابت نویسه * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		پوچ,
	};
	شمارش ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_run = 0;
	ساختار option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	ساختار commit *original, *rewritten;
	ساختار rev_info revs = { 0 };
	صحیح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	اگر (argc != 1) {
		ret = error(_("command expects a single revision"));
		برو out;
	}
	repo_config(repo, git_default_config, پوچ);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		برو out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	اگر (ret)
		برو out;

	ret = commit_درخت_with_edited_message(repo, "reworded", original, &rewritten);
	اگر (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		برو out;
	}

	strbuf_addf(&reflog_پیام, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیام.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		برو out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	بازگشت ret;
}

ایستا صحیح write_ondisk_index(ساختار repository *repo,
			      ساختار object_id *oid,
			      ثابت نویسه *path)
{
	ساختار unpack_trees_options opts = { 0 };
	ساختار lock_file lock = LOCK_INIT;
	ساختار درخت_desc درخت_desc;
	ساختار index_state index;
	ساختار درخت *درخت;
	صحیح ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	درخت = repo_parse_درخت_indirect(repo, oid);
	اگر (!درخت) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		برو out;
	}
	init_درخت_desc(&درخت_desc, &درخت->object.oid, درخت->buffer, درخت->size);

	اگر (unpack_trees(1, &درخت_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		برو out;
	}

	اول_cache_درخت(repo, &index, درخت);

	اگر (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		برو out;
	}

	اگر (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		برو out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	بازگشت ret;
}

ایستا صحیح split_commit(ساختار repository *repo,
			ساختار commit *original,
			ساختار pathspec *pathspec,
			ساختار commit **out)
{
	ساختار interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ساختار strbuf index_file = STRBUF_INIT;
	ساختار index_state index = INDEX_STATE_INIT(repo);
	ثابت ساختار object_id *original_commit_درخت_oid;
	ثابت ساختار object_id *old_درخت_oid, *new_درخت_oid;
	ساختار object_id parent_درخت_oid;
	نویسه original_commit_oid[GIT_MAX_HEXSZ + 1];
	ساختار commit *first_commit, *second_commit;
	ساختار commit_list *parents = پوچ;
	ساختار درخت *split_درخت;
	صحیح ret;

	اگر (first_parent_درخت_oid(repo, original, &parent_درخت_oid) < 0) {
		ret = -1;
		برو out;
	}
	original_commit_درخت_oid = get_commit_درخت_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_درخت_oid, index_file.buf);
	اگر (ret < 0)
		برو out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	اگر (ret < 0) {
		ret = error(_("failed reading temporary index"));
		برو out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	اگر (ret < 0)
		برو out;

	split_درخت = write_in_core_index_as_درخت(repo, &index);
	اگر (!split_درخت) {
		ret = error(_("failed split tree"));
		برو out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	اگر (oideq(&split_درخت->object.oid, &parent_درخت_oid)) {
		ret = error(_("split commit is empty"));
		برو out;
	} وگرنه اگر (oideq(&split_درخت->object.oid, original_commit_درخت_oid)) {
		ret = error(_("split commit tree matches original commit"));
		برو out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_درخت_ext(repo, "split-out", original, پوچ, original->parents, &parent_درخت_oid,
			      &split_درخت->object.oid, &first_commit, COMMIT_درخت_EDIT_MESSAGE);
	اگر (ret < 0) {
		ret = error(_("failed writing first commit"));
		برو out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_درخت_oid = &repo_get_commit_درخت(repo, first_commit)->object.oid;
	new_درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	ret = commit_درخت_ext(repo, "split-out", original, پوچ, parents, old_درخت_oid,
			      new_درخت_oid, &second_commit, COMMIT_درخت_EDIT_MESSAGE);
	اگر (ret < 0) {
		ret = error(_("failed writing second commit"));
		برو out;
	}

	*out = second_commit;
	ret = 0;

out:
	اگر (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	بازگشت ret;
}

ایستا صحیح cmd_history_split(صحیح argc,
			     ثابت نویسه **argv,
			     ثابت نویسه *prefix,
			     ساختار repository *repo)
{
	ثابت نویسه * ثابت usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		پوچ,
	};
	شمارش ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_run = 0;
	ساختار option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ساختار commit *original, *rewritten = پوچ;
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	ساختار pathspec pathspec = { 0 };
	ساختار rev_info revs = { 0 };
	صحیح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	اگر (argc < 1) {
		ret = error(_("command expects a committish"));
		برو out;
	}
	repo_config(repo, git_default_config, پوچ);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		برو out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	اگر (ret < 0)
		برو out;

	اگر (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		برو out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	اگر (ret < 0)
		برو out;

	strbuf_addf(&reflog_پیام, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیام.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		برو out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_پیام);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	بازگشت ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

ایستا bool is_autosquash_subject(ثابت نویسه *s)
{
	بازگشت starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ایستا bool skip_one_autosquash_prefix(ثابت نویسه *s, ثابت نویسه **out)
{
	اگر (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		وقتی (**out == ' ')
			(*out)++;
		بازگشت true;
	}
	بازگشت false;
}

ایستا تهی truncate_message_to_subject(ساختار strbuf *پیام)
{
	ثابت نویسه *eos = strstr(پیام->buf, "\n\n");

	اگر (eos)
		strbuf_setlen(پیام, eos - پیام->buf + 1);
}

ساختار subject_data {
	ساختار strintmap subjects;
	ساختار strbuf subject;
	ساختار strbuf squash_message;
	ثابت نویسه *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

ایستا تهی subject_data_clear(ساختار subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

ایستا صحیح squash_amend_message(ساختار repository *repo,
				ساختار commit *commit,
				ساختار subject_data *data,
				unsigned flags)
{
	ثابت نویسه *body = data->message + data->subject.len;

	وقتی (isspace(*body))
		body++;

	اگر (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		بازگشت 0;
	}

	اگر (data->edit_message) {
		بازگشت 0;
	} وگرنه اگر (flags & SQUASH_AMEND_TARGET) {
		اگر (starts_with(data->squash_message.buf, "squash!"))
			بازگشت error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		اگر (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		اگر (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} وگرنه {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_سطر(&data->squash_message);
	} وگرنه {
		بازگشت error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	بازگشت 0;
}

ایستا صحیح squash_squash_message(ساختار repository *repo,
				ساختار commit *commit,
				ساختار subject_data *data,
				unsigned flags)
{
	ثابت نویسه *body = data->message + data->subject.len;

	وقتی (isspace(*body))
		body++;

	اگر (data->edit_message) {
		بازگشت 0;
	} وگرنه اگر (flags & SQUASH_AMEND_TARGET) {
		اگر (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		اگر (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_سطر(&data->squash_message);
		} وگرنه {
			بازگشت error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} وگرنه {
		بازگشت error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	بازگشت 0;
}

ایستا صحیح squash_check_can_autosquash(ساختار repository *repo,
				       ساختار commit *commit,
				       ساختار subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	اگر (starts_with(data->subject.buf, "amend!"))
		بازگشت squash_amend_message(repo, commit, data, flags);
	وگرنه اگر (starts_with(data->subject.buf, "squash!"))
		بازگشت squash_squash_message(repo, commit, data, flags);

	بازگشت 0;
}

ایستا صحیح squash_check_autosquash_subject(ساختار repository *repo,
					   ساختار commit *commit,
					   ساختار subject_data *data)
{
	ثابت نویسه* s = data->subject.buf;
	ساختار commit *target;
	ساختار hashmap_iter iter;
	ساختار strmap_entry *entry;
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
	وقتی (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		اگر (flags)
			بازگشت squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	اگر (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		اگر (!flags) {
			ثابت نویسه *subject_start;
			ثابت نویسه *buffer = repo_logmsg_reencode(repo, target,
								  پوچ, پوچ);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			نویسه *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			آزاد(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		اگر (flags)
			بازگشت squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		وقتی(skip_one_autosquash_prefix(s, &s)) {
			اگر (starts_with(entry->کلید, s)) {
				unsigned مقدار = (intptr_t)entry->مقدار;

				بازگشت squash_check_can_autosquash(repo, commit,
								   data, مقدار);
			}
		}
	}
	بازگشت error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

ایستا صحیح squash_check_subject(ساختار repository *repo,
				ساختار commit *commit,
				ساختار subject_data *data)
{
	صحیح ret = 0;
	ثابت نویسه *buf = repo_logmsg_reencode(repo, commit, پوچ, پوچ);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	اگر (!strintmap_get_size(&data->subjects)) {
		ثابت نویسه *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_سطر(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} وگرنه اگر (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} وگرنه {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	بازگشت ret;
}

ایستا صحیح build_squash_message(ساختار repository *repo,
				ثابت ساختار strbuf *todo_buf,
				ساختار strbuf *out);

ایستا صحیح setup_squash_revisions(ساختار repository *repo,
				  صحیح argc, ثابت نویسه **argv,
				  ساختار rev_info *revs)
{
	repo_init_revisions(repo, revs, پوچ);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, پوچ);
	اگر (argc > 1)
		بازگشت error(_("unrecognized argument: %s"), argv[1]);

	اگر (revs->reverse != 1 || revs->topo_order != 1 ||
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
	برای (size_t i = 0; i < revs->cmdline.nr; i++)
		اگر (revs->cmdline.rev[i].flags & BOTTOM)
			بازگشت 0;

	بازگشت error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ایستا صحیح resolve_squash_range(ساختار repository *repo,
				bool update_branches,
				bool edit_message,
				صحیح argc, ثابت نویسه **argv,
				ساختار commit **oldest_out,
				ساختار commit **tip_out,
				نویسه **message_out)
{
	ساختار rev_info revs;
	ساختار subject_data subject_data = SUBJECT_DATA_INIT;
	ساختار commit *commit, *oldest = پوچ, *tip = پوچ;
	ساختار strbuf todo_buf = STRBUF_INIT;
	صحیح ret, tip_شمار = 0;
	bool walk_started = false;
	ساختار ref_filter filter = REF_FILTER_INIT;
	ساختار ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	اگر (ret < 0)
		برو out;

	اگر (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		برو out;
	}
	walk_started = true;
	وقتی ((commit = get_revision(&revs))) {
		ساختار commit_list *p;

		اگر (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		اگر (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			برو out;
		}
		برای (p = commit->parents; oldest && p; p = p->next) {
			ساختار commit_list *q;
			ساختار object *o;
			bool seen;

			اگر (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				برو out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			برای (q = oldest->parents; !seen && q; q = q->next) {
				اگر (p->item == q->item) {
					seen = true;
					commit_list_درج(commit, &filter.with_commit);
				}
			}
			اگر (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				برو out;
			}
			اگر (o->flags & SQUASH_TIP) {
				tip_شمار--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		اگر (!oldest) {
			commit_list_درج(commit, &filter.with_commit);
			oldest = commit;
		}
		اگر (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			برو out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_شمار++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	اگر (!tip_شمار) {
		ret = error(_("the revision range is empty"));
		برو out;
	} وگرنه اگر (tip_شمار != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		برو out;
	} وگرنه اگر (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		برو out;
	} وگرنه اگر (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_درج(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	اگر (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		برو out;
	}
	اگر (refs.nr) {
		ساختار ref_format format = REF_FORMAT_INIT;
		ساختار ref_sorting *sorting;
		ساختار string_list sorting_options = STRING_LIST_INIT_DUP;
		ساختار strbuf branches = STRBUF_INIT;
		ساختار strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		اگر (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		برای (صحیح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			اگر (format_ref_array_item(refs.اقلام[i], &format,
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
		برو out;
	}
	اگر (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		اگر (ret < 0)
			برو out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, پوچ);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	اگر (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	بازگشت ret;
}

ایستا bool amend_replaces_target(ساختار todo_list *todo, صحیح target)
{
	برای (صحیح i = target + 1; i < todo->nr &&
				 todo->اقلام[i].command != TODO_PICK; i++) {
		اگر (todo->اقلام[i].command == TODO_SQUASH)
			بازگشت false;
		اگر (todo->اقلام[i].flags & TODO_REPLACE_FIXUP_پیام)
			بازگشت true;
	}
	بازگشت false;
}

ایستا صحیح build_squash_message(ساختار repository *repo,
				ثابت ساختار strbuf *todo_buf,
				ساختار strbuf *out)
{
	ساختار todo_list todo = TODO_LIST_INIT;
	ساختار replay_opts opts = REPLAY_OPTS_INIT;
	صحیح nr_commits, ret;

	اگر (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		برو out;
	}

	nr_commits = todo.nr;
	برای (صحیح i = 0; i < nr_commits; i++) {
		ساختار todo_item *item = &todo.اقلام[i];
		ثابت نویسه *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_پیام);
		اگر (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		وگرنه
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, پوچ, پوچ);
		find_commit_subject(message, &body);

		اگر (skip)
			commented_len = strlen(body);
		وگرنه اگر (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		وگرنه
			commented_len = 0;

		اگر (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_سطرها(out, body, commented_len, comment_سطر_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_سطر(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	بازگشت ret;
}

ایستا صحیح cmd_history_squash(صحیح argc,
			      ثابت نویسه **argv,
			      ثابت نویسه *prefix,
			      ساختار repository *repo)
{
	ثابت نویسه * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		پوچ,
	};
	شمارش ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_run = 0;
	صحیح edit = 1;
	ساختار option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	ساختار commit *oldest, *tip, *rewritten;
	ثابت ساختار object_id *base_درخت_oid, *tip_درخت_oid;
	نویسه *message_template = پوچ;
	ساختار rev_info revs = { 0 };
	صحیح ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	اگر (argc < 2) {
		ret = error(_("command expects a revision range"));
		برو out;
	}
	repo_config(repo, git_default_config, پوچ);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_پیام, "squash: updating ");
	strbuf_join_argv(&reflog_پیام, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	اگر (ret < 0)
		برو out;

	ret = setup_revwalk(repo, action, tip, &revs);
	اگر (ret < 0)
		برو out;

	base_درخت_oid = &repo_get_commit_درخت(repo,
					oldest->parents->item)->object.oid;
	tip_درخت_oid = &repo_get_commit_درخت(repo, tip)->object.oid;

	ret = commit_درخت_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_درخت_oid, tip_درخت_oid,
			      &rewritten,
			      edit ? COMMIT_درخت_EDIT_MESSAGE : 0);
	اگر (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		برو out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_پیام.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		برو out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	آزاد(message_template);
	بازگشت ret;
}

ایستا صحیح update_worktree(ساختار repository *repo,
			   ثابت ساختار commit *old_head,
			   ثابت ساختار commit *new_head,
			   bool dry_run)
{
	ساختار reset_working_درخت_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	اگر (dry_run)
		opts.flags |= RESET_WORKING_درخت_DRY_RUN;
	بازگشت reset_working_درخت(repo, &opts);
}

ایستا صحیح find_head_درخت_change(ساختار repository *repo,
				 ثابت ساختار replay_result *result,
				 ساختار commit **old_head,
				 ساختار commit **new_head,
				 bool *changed)
{
	ثابت ساختار replay_ref_update *head_update = پوچ;
	ساختار commit *old_head_commit, *new_head_commit;
	ساختار درخت *old_head_درخت, *new_head_درخت;
	ثابت نویسه *head_target;
	صحیح head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      پوچ, &head_flags);
	اگر (!head_target)
		بازگشت error(_("cannot look up HEAD"));

	برای (size_t i = 0; i < result->updates_nr; i++) {
		اگر (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			بشکن;
		}
	}

	اگر (!head_update)
		بازگشت 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	اگر (!old_head_commit || !new_head_commit)
		بازگشت error(_("cannot resolve HEAD commit"));

	old_head_درخت = repo_get_commit_درخت(repo, old_head_commit);
	new_head_درخت = repo_get_commit_درخت(repo, new_head_commit);
	اگر (!old_head_درخت || !new_head_درخت)
		بازگشت error(_("cannot resolve tree for HEAD"));

	اگر (oideq(&old_head_درخت->object.oid, &new_head_درخت->object.oid))
		بازگشت 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	بازگشت 0;
}

ایستا صحیح cmd_history_drop(صحیح argc,
			    ثابت نویسه **argv,
			    ثابت نویسه *prefix,
			    ساختار repository *repo)
{
	ثابت نویسه * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		پوچ,
	};
	شمارش replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	شمارش ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_run = 0;
	ساختار option options[] = {
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
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	ساختار commit *original, *rewritten;
	ساختار rev_info revs = { 0 };
	ساختار replay_result result = { 0 };
	ساختار commit *old_head, *new_head;
	bool head_moves = false;
	صحیح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	اگر (argc != 1) {
		ret = error(_("command expects a single revision"));
		برو out;
	}
	repo_config(repo, git_default_config, پوچ);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		برو out;
	}

	اگر (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		برو out;
	} وگرنه اگر (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		برو out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	اگر (ret)
		برو out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	اگر (ret) {
		ret = error(_("failed replaying descendants"));
		برو out;
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
	اگر (!is_bare_repository(repo)) {
		ret = find_head_درخت_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		اگر (ret < 0)
			برو out;

		اگر (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			برو out;
		}
	}

	strbuf_addf(&reflog_پیام, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_پیام.buf, dry_run);
	اگر (ret < 0) {
		ret = error(_("failed to update references"));
		برو out;
	}

	اگر (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		برو out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	بازگشت ret;
}

صحیح cmd_history(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه *prefix,
		ساختار repository *repo)
{
	ثابت نویسه * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		پوچ,
	};
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	بازگشت fn(argc, argv, prefix, repo);
}
