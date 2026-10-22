#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "cache-tree.h"
#ضمن "commit.h"
#ضمن "commit-reach.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "merge-ort.h"
#ضمن "oidmap.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "read-cache.h"
#ضمن "refs.h"
#ضمن "ref-filter.h"
#ضمن "replay.h"
#ضمن "reset.h"
#ضمن "revision.h"
#ضمن "sequencer.h"
#ضمن "string-list.h"
#ضمن "strvec.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "unpack-trees.h"
#ضمن "wt-status.h"

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

ساكن فراغ change_data_free(فراغ *util, ثابت حرف *str UNUSED)
{
	بنية wt_status_change_data *d = util;
	حرر(d->rename_source);
	حرر(d);
}

ساكن صحيح fill_commit_message(بنية repository *repo,
			       ثابت بنية object_id *old_شجرة,
			       ثابت بنية object_id *new_شجرة,
			       ثابت حرف *default_message,
			       ثابت حرف *action,
			       بنية strbuf *out)
{
	ثابت حرف *path = git_path_commit_editmsg();
	ثابت حرف *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	بنية wt_status s;

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
	إذا (!s.fp)
		إرجاع error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_سطر_str, hint, action, comment_سطر_str);
	إذا (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_شجرة, new_شجرة);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	إذا (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	إذا (launch_editor(path, out, عدم)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		إرجاع -1;
	}
	strbuf_stripspace(out, comment_سطر_str);

	cleanup_message(out, COMMIT_رسالة_CLEANUP_ALL, 0);

	إذا (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		إرجاع -1;
	}

	إرجاع 0;
}

تعداد commit_شجرة_flags {
	COMMIT_شجرة_EDIT_MESSAGE = (1 << 0),
};

ساكن صحيح commit_شجرة_ext(بنية repository *repo,
			   ثابت حرف *action,
			   بنية commit *commit_with_message,
			   ثابت حرف *message_template,
			   ثابت بنية commit_list *parents,
			   ثابت بنية object_id *old_شجرة,
			   ثابت بنية object_id *new_شجرة,
			   بنية commit **out,
			   تعداد commit_شجرة_flags flags)
{
	ثابت حرف *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		عدم,
	};
	ثابت حرف *original_message, *original_body, *ptr;
	بنية commit_extra_header *original_extra_headers = عدم;
	بنية strbuf commit_message = STRBUF_INIT;
	بنية object_id rewritten_commit_oid;
	حرف *original_author = عدم;
	size_t len;
	صحيح ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, عدم, عدم);
	ptr = find_commit_header(original_message, "author", &len);
	إذا (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	إذا (!message_template)
		message_template = original_body;

	إذا (flags & COMMIT_شجرة_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_شجرة, new_شجرة,
					  message_template, action, &commit_message);
		إذا (ret < 0)
			اذهب out;
	} وإلا {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_شجرة_extended(commit_message.buf, commit_message.len, new_شجرة,
				   parents, &rewritten_commit_oid, original_author,
				   عدم, عدم, original_extra_headers);
	إذا (ret < 0)
		اذهب out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	حرر(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	إرجاع ret;
}

ساكن صحيح first_parent_شجرة_oid(بنية repository *repo,
				 بنية commit *commit,
				 بنية object_id *out)
{
	بنية commit *parent = commit->parents ? commit->parents->item : عدم;

	إذا (!parent) {
		oidcpy(out, repo->hash_algo->empty_شجرة);
		إرجاع 0;
	}

	إذا (repo_parse_commit(repo, parent))
		إرجاع error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_شجرة(repo, parent)->object.oid);
	إرجاع 0;
}

ساكن صحيح commit_شجرة_with_edited_message(بنية repository *repo,
					   ثابت حرف *action,
					   بنية commit *original,
					   بنية commit **out)
{
	بنية object_id parent_شجرة_oid;
	ثابت بنية object_id *شجرة_oid;

	شجرة_oid = &repo_get_commit_شجرة(repo, original)->object.oid;

	إذا (first_parent_شجرة_oid(repo, original, &parent_شجرة_oid) < 0)
		إرجاع -1;

	إرجاع commit_شجرة_ext(repo, action, original, عدم, original->parents,
			       &parent_شجرة_oid, شجرة_oid, out, COMMIT_شجرة_EDIT_MESSAGE);
}

تعداد ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

ساكن صحيح parse_ref_action(ثابت بنية option *opt, ثابت حرف *قيمة, صحيح unset)
{
	تعداد ref_action *action = opt->قيمة;

	BUG_ON_OPT_NEG_NOARG(unset, قيمة);
	إذا (!strcmp(قيمة, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} وإلا إذا (!strcmp(قيمة, "head")) {
		*action = REF_ACTION_HEAD;
	} وإلا {
		إرجاع error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	إرجاع 0;
}

ساكن صحيح revwalk_contains_merges(بنية repository *repo,
				   ثابت بنية strvec *revwalk_args)
{
	بنية strvec args = STRVEC_INIT;
	بنية rev_info revs;
	صحيح ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ادفع(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, عدم);

	setup_revisions_from_strvec(&args, &revs, عدم);
	إذا (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب out;
	}

	إذا (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		اذهب out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	إرجاع ret;
}

ساكن صحيح setup_revwalk(بنية repository *repo,
			 تعداد ref_action action,
			 بنية commit *original,
			 بنية rev_info *revs)
{
	بنية strvec args = STRVEC_INIT;
	صحيح ret;

	repo_init_revisions(repo, revs, عدم);
	strvec_ادفع(&args, "ignored");
	strvec_ادفع(&args, "--reverse");
	strvec_ادفع(&args, "--topo-order");
	strvec_ادفع(&args, "--full-history");

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
	إذا (action == REF_ACTION_HEAD) {
		بنية commit_list *from_list = عدم;
		بنية commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		إذا (!head) {
			ret = error(_("cannot look up HEAD"));
			اذهب out;
		}

		commit_list_أدرج(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		إذا (ret < 0) {
			ret = error(_("cannot determine descendance"));
			اذهب out;
		} وإلا إذا (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			اذهب out;
		}

		strvec_ادفع(&args, "HEAD");
	} وإلا {
		strvec_ادفع(&args, "--branches");
		strvec_ادفع(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	إذا (ret < 0)
		اذهب out;

	setup_revisions_from_strvec(&args, revs, عدم);
	إذا (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	إرجاع ret;
}

ساكن صحيح handle_ref_update(بنية ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت بنية object_id *new_oid,
			     ثابت بنية object_id *old_oid,
			     ثابت حرف *reflog_رسالة,
			     بنية strbuf *err)
{
	إذا (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		إرجاع 0;
	}

	إرجاع ref_transaction_update(transaction, refname, new_oid, old_oid,
				      عدم, عدم, 0, reflog_رسالة, err);
}

ساكن صحيح compute_pending_ref_updates(بنية rev_info *revs,
				       تعداد ref_action action,
				       بنية commit *original,
				       بنية commit *rewritten,
				       تعداد replay_empty_commit_action empty,
				       بنية replay_result *result)
{
	ثابت بنية name_decoration *decoration;
	بنية replay_revisions_options opts = {
		.empty = empty,
	};
	حرف hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	صحيح head_flags = 0;
	صحيح ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, عدم, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	إذا (ret)
		إرجاع ret;

	إذا (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	لكل (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		إذا (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			استمر;

		إذا (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			استمر;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		إذا (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			استمر;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	إرجاع 0;
}

ساكن صحيح apply_pending_ref_updates(بنية repository *repo,
				     ثابت بنية replay_result *result,
				     ثابت حرف *reflog_رسالة,
				     صحيح dry_run)
{
	بنية ref_transaction *transaction = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret;

	إذا (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			اذهب out;
		}
	}

	لكل (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_رسالة, &err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			اذهب out;
		}
	}

	إذا (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		اذهب out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	إرجاع ret;
}

ساكن صحيح handle_reference_updates(بنية rev_info *revs,
				    تعداد ref_action action,
				    بنية commit *original,
				    بنية commit *rewritten,
				    ثابت حرف *reflog_رسالة,
				    صحيح dry_run,
				    تعداد replay_empty_commit_action empty)
{
	بنية replay_result result = { 0 };
	صحيح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	إذا (ret)
		اذهب out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_رسالة, dry_run);

out:
	replay_result_release(&result);
	إرجاع ret;
}

ساكن صحيح commit_became_empty(بنية repository *repo,
			       بنية commit *original,
			       بنية شجرة *result)
{
	بنية object_id parent_شجرة_oid;

	إذا (first_parent_شجرة_oid(repo, original, &parent_شجرة_oid) < 0)
		إرجاع -1;

	إرجاع oideq(&result->object.oid, &parent_شجرة_oid);
}

ساكن صحيح parse_opt_empty(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد replay_empty_commit_action *قيمة = opt->قيمة;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "drop"))
		*قيمة = REPLAY_EMPTY_COMMIT_DROP;
	وإلا إذا (!strcmp(arg, "keep"))
		*قيمة = REPLAY_EMPTY_COMMIT_KEEP;
	وإلا إذا (!strcmp(arg, "abort"))
		*قيمة = REPLAY_EMPTY_COMMIT_ABORT;
	وإلا
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	إرجاع 0;
}

ساكن صحيح cmd_history_fixup(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *prefix,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		عدم,
	};
	تعداد replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	تعداد commit_شجرة_flags flags = 0;
	صحيح dry_run = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_شجرة_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	بنية merge_result merge_result = { 0 };
	بنية merge_options merge_opts = { 0 };
	بنية strbuf reflog_رسالة = STRBUF_INIT;
	بنية commit *head_commit, *original, *rewritten;
	بنية شجرة *head_شجرة, *original_شجرة, *index_شجرة;
	بنية rev_info revs = { 0 };
	bool skip_commit = false;
	صحيح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب out;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	إذا (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		اذهب out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	إذا (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		اذهب out;
	}

	head_شجرة = repo_get_commit_شجرة(repo, head_commit);
	إذا (!head_شجرة) {
		ret = error(_("cannot get tree for HEAD"));
		اذهب out;
	}

	إذا (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		اذهب out;
	}

	إذا (!repo_index_has_changes(repo, head_شجرة, عدم)) {
		ret = error(_("nothing to fixup: no staged changes"));
		اذهب out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_شجرة = write_in_core_index_as_شجرة(repo, repo->index);
	إذا (!index_شجرة) {
		ret = error(_("unable to write index as a tree"));
		اذهب out;
	}

	original_شجرة = repo_get_commit_شجرة(repo, original);
	إذا (!original_شجرة) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		اذهب out;
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
	merge_incore_nonrecursive(&merge_opts, head_شجرة,
				  original_شجرة, index_شجرة, &merge_result);

	إذا (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		اذهب out;
	}

	إذا (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		اذهب out;
	}

	ret = commit_became_empty(repo, original, merge_result.شجرة);
	إذا (ret < 0)
		اذهب out;
	إذا (ret > 0) {
		بدل (empty) {
		حالة REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : عدم;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			إذا (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				اذهب out;
			}

			skip_commit = true;
			اكسر;
		حالة REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			اكسر;
		حالة REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			اذهب out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب out;

	إذا (!skip_commit) {
		ret = commit_شجرة_ext(repo, "fixup", original, عدم, original->parents,
				      &original_شجرة->object.oid, &merge_result.شجرة->object.oid,
				      &rewritten, flags);
		إذا (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			اذهب out;
		}
	}

	strbuf_addf(&reflog_رسالة, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_رسالة.buf, dry_run, empty);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_رسالة);
	release_revisions(&revs);
	إرجاع ret;
}

ساكن صحيح cmd_history_reword(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *prefix,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_run = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	بنية strbuf reflog_رسالة = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب out;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب out;

	ret = commit_شجرة_with_edited_message(repo, "reworded", original, &rewritten);
	إذا (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		اذهب out;
	}

	strbuf_addf(&reflog_رسالة, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_رسالة.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_رسالة);
	release_revisions(&revs);
	إرجاع ret;
}

ساكن صحيح write_ondisk_index(بنية repository *repo,
			      بنية object_id *oid,
			      ثابت حرف *path)
{
	بنية unpack_trees_options opts = { 0 };
	بنية lock_file lock = LOCK_INIT;
	بنية شجرة_desc شجرة_desc;
	بنية index_state index;
	بنية شجرة *شجرة;
	صحيح ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	شجرة = repo_parse_شجرة_indirect(repo, oid);
	إذا (!شجرة) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		اذهب out;
	}
	init_شجرة_desc(&شجرة_desc, &شجرة->object.oid, شجرة->buffer, شجرة->size);

	إذا (unpack_trees(1, &شجرة_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		اذهب out;
	}

	أولي_cache_شجرة(repo, &index, شجرة);

	إذا (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		اذهب out;
	}

	إذا (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		اذهب out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	إرجاع ret;
}

ساكن صحيح split_commit(بنية repository *repo,
			بنية commit *original,
			بنية pathspec *pathspec,
			بنية commit **out)
{
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية strbuf index_file = STRBUF_INIT;
	بنية index_state index = INDEX_STATE_INIT(repo);
	ثابت بنية object_id *original_commit_شجرة_oid;
	ثابت بنية object_id *old_شجرة_oid, *new_شجرة_oid;
	بنية object_id parent_شجرة_oid;
	حرف original_commit_oid[GIT_MAX_HEXSZ + 1];
	بنية commit *first_commit, *second_commit;
	بنية commit_list *parents = عدم;
	بنية شجرة *split_شجرة;
	صحيح ret;

	إذا (first_parent_شجرة_oid(repo, original, &parent_شجرة_oid) < 0) {
		ret = -1;
		اذهب out;
	}
	original_commit_شجرة_oid = get_commit_شجرة_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_شجرة_oid, index_file.buf);
	إذا (ret < 0)
		اذهب out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	إذا (ret < 0) {
		ret = error(_("failed reading temporary index"));
		اذهب out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	إذا (ret < 0)
		اذهب out;

	split_شجرة = write_in_core_index_as_شجرة(repo, &index);
	إذا (!split_شجرة) {
		ret = error(_("failed split tree"));
		اذهب out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	إذا (oideq(&split_شجرة->object.oid, &parent_شجرة_oid)) {
		ret = error(_("split commit is empty"));
		اذهب out;
	} وإلا إذا (oideq(&split_شجرة->object.oid, original_commit_شجرة_oid)) {
		ret = error(_("split commit tree matches original commit"));
		اذهب out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_شجرة_ext(repo, "split-out", original, عدم, original->parents, &parent_شجرة_oid,
			      &split_شجرة->object.oid, &first_commit, COMMIT_شجرة_EDIT_MESSAGE);
	إذا (ret < 0) {
		ret = error(_("failed writing first commit"));
		اذهب out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_شجرة_oid = &repo_get_commit_شجرة(repo, first_commit)->object.oid;
	new_شجرة_oid = &repo_get_commit_شجرة(repo, original)->object.oid;

	ret = commit_شجرة_ext(repo, "split-out", original, عدم, parents, old_شجرة_oid,
			      new_شجرة_oid, &second_commit, COMMIT_شجرة_EDIT_MESSAGE);
	إذا (ret < 0) {
		ret = error(_("failed writing second commit"));
		اذهب out;
	}

	*out = second_commit;
	ret = 0;

out:
	إذا (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	إرجاع ret;
}

ساكن صحيح cmd_history_split(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *prefix,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_run = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	بنية commit *original, *rewritten = عدم;
	بنية strbuf reflog_رسالة = STRBUF_INIT;
	بنية pathspec pathspec = { 0 };
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	إذا (argc < 1) {
		ret = error(_("command expects a committish"));
		اذهب out;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	إذا (ret < 0)
		اذهب out;

	إذا (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		اذهب out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	إذا (ret < 0)
		اذهب out;

	strbuf_addf(&reflog_رسالة, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_رسالة.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_رسالة);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	إرجاع ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

ساكن bool is_autosquash_subject(ثابت حرف *s)
{
	إرجاع starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ساكن bool skip_one_autosquash_prefix(ثابت حرف *s, ثابت حرف **out)
{
	إذا (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		بينما (**out == ' ')
			(*out)++;
		إرجاع true;
	}
	إرجاع false;
}

ساكن فراغ truncate_message_to_subject(بنية strbuf *رسالة)
{
	ثابت حرف *eos = strstr(رسالة->buf, "\n\n");

	إذا (eos)
		strbuf_setlen(رسالة, eos - رسالة->buf + 1);
}

بنية subject_data {
	بنية strintmap subjects;
	بنية strbuf subject;
	بنية strbuf squash_message;
	ثابت حرف *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

ساكن فراغ subject_data_clear(بنية subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

ساكن صحيح squash_amend_message(بنية repository *repo,
				بنية commit *commit,
				بنية subject_data *data,
				unsigned flags)
{
	ثابت حرف *body = data->message + data->subject.len;

	بينما (isspace(*body))
		body++;

	إذا (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		إرجاع 0;
	}

	إذا (data->edit_message) {
		إرجاع 0;
	} وإلا إذا (flags & SQUASH_AMEND_TARGET) {
		إذا (starts_with(data->squash_message.buf, "squash!"))
			إرجاع error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		إذا (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		إذا (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} وإلا {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_سطر(&data->squash_message);
	} وإلا {
		إرجاع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	إرجاع 0;
}

ساكن صحيح squash_squash_message(بنية repository *repo,
				بنية commit *commit,
				بنية subject_data *data,
				unsigned flags)
{
	ثابت حرف *body = data->message + data->subject.len;

	بينما (isspace(*body))
		body++;

	إذا (data->edit_message) {
		إرجاع 0;
	} وإلا إذا (flags & SQUASH_AMEND_TARGET) {
		إذا (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		إذا (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_سطر(&data->squash_message);
		} وإلا {
			إرجاع error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} وإلا {
		إرجاع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	إرجاع 0;
}

ساكن صحيح squash_check_can_autosquash(بنية repository *repo,
				       بنية commit *commit,
				       بنية subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	إذا (starts_with(data->subject.buf, "amend!"))
		إرجاع squash_amend_message(repo, commit, data, flags);
	وإلا إذا (starts_with(data->subject.buf, "squash!"))
		إرجاع squash_squash_message(repo, commit, data, flags);

	إرجاع 0;
}

ساكن صحيح squash_check_autosquash_subject(بنية repository *repo,
					   بنية commit *commit,
					   بنية subject_data *data)
{
	ثابت حرف* s = data->subject.buf;
	بنية commit *target;
	بنية hashmap_iter iter;
	بنية strmap_entry *entry;
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
	بينما (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		إذا (flags)
			إرجاع squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	إذا (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		إذا (!flags) {
			ثابت حرف *subject_start;
			ثابت حرف *buffer = repo_logmsg_reencode(repo, target,
								  عدم, عدم);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			حرف *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			حرر(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		إذا (flags)
			إرجاع squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		بينما(skip_one_autosquash_prefix(s, &s)) {
			إذا (starts_with(entry->مفتاح, s)) {
				unsigned قيمة = (intptr_t)entry->قيمة;

				إرجاع squash_check_can_autosquash(repo, commit,
								   data, قيمة);
			}
		}
	}
	إرجاع error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

ساكن صحيح squash_check_subject(بنية repository *repo,
				بنية commit *commit,
				بنية subject_data *data)
{
	صحيح ret = 0;
	ثابت حرف *buf = repo_logmsg_reencode(repo, commit, عدم, عدم);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	إذا (!strintmap_get_size(&data->subjects)) {
		ثابت حرف *s;

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
	} وإلا إذا (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} وإلا {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	إرجاع ret;
}

ساكن صحيح build_squash_message(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *out);

ساكن صحيح setup_squash_revisions(بنية repository *repo,
				  صحيح argc, ثابت حرف **argv,
				  بنية rev_info *revs)
{
	repo_init_revisions(repo, revs, عدم);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, عدم);
	إذا (argc > 1)
		إرجاع error(_("unrecognized argument: %s"), argv[1]);

	إذا (revs->reverse != 1 || revs->topo_order != 1 ||
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
	لكل (size_t i = 0; i < revs->cmdline.nr; i++)
		إذا (revs->cmdline.rev[i].flags & BOTTOM)
			إرجاع 0;

	إرجاع error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ساكن صحيح resolve_squash_range(بنية repository *repo,
				bool update_branches,
				bool edit_message,
				صحيح argc, ثابت حرف **argv,
				بنية commit **oldest_out,
				بنية commit **tip_out,
				حرف **message_out)
{
	بنية rev_info revs;
	بنية subject_data subject_data = SUBJECT_DATA_INIT;
	بنية commit *commit, *oldest = عدم, *tip = عدم;
	بنية strbuf todo_buf = STRBUF_INIT;
	صحيح ret, tip_تعداد = 0;
	bool walk_started = false;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	إذا (ret < 0)
		اذهب out;

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب out;
	}
	walk_started = true;
	بينما ((commit = get_revision(&revs))) {
		بنية commit_list *p;

		إذا (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		إذا (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			اذهب out;
		}
		لكل (p = commit->parents; oldest && p; p = p->next) {
			بنية commit_list *q;
			بنية object *o;
			bool seen;

			إذا (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				اذهب out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			لكل (q = oldest->parents; !seen && q; q = q->next) {
				إذا (p->item == q->item) {
					seen = true;
					commit_list_أدرج(commit, &filter.with_commit);
				}
			}
			إذا (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				اذهب out;
			}
			إذا (o->flags & SQUASH_TIP) {
				tip_تعداد--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		إذا (!oldest) {
			commit_list_أدرج(commit, &filter.with_commit);
			oldest = commit;
		}
		إذا (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			اذهب out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_تعداد++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	إذا (!tip_تعداد) {
		ret = error(_("the revision range is empty"));
		اذهب out;
	} وإلا إذا (tip_تعداد != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		اذهب out;
	} وإلا إذا (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		اذهب out;
	} وإلا إذا (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_أدرج(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	إذا (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		اذهب out;
	}
	إذا (refs.nr) {
		بنية ref_format format = REF_FORMAT_INIT;
		بنية ref_sorting *sorting;
		بنية string_list sorting_options = STRING_LIST_INIT_DUP;
		بنية strbuf branches = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		إذا (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		لكل (صحيح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			إذا (format_ref_array_item(refs.عناصر[i], &format,
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
		اذهب out;
	}
	إذا (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		إذا (ret < 0)
			اذهب out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, عدم);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	إذا (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	إرجاع ret;
}

ساكن bool amend_replaces_target(بنية todo_list *todo, صحيح target)
{
	لكل (صحيح i = target + 1; i < todo->nr &&
				 todo->عناصر[i].command != TODO_PICK; i++) {
		إذا (todo->عناصر[i].command == TODO_SQUASH)
			إرجاع false;
		إذا (todo->عناصر[i].flags & TODO_REPLACE_FIXUP_رسالة)
			إرجاع true;
	}
	إرجاع false;
}

ساكن صحيح build_squash_message(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *out)
{
	بنية todo_list todo = TODO_LIST_INIT;
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح nr_commits, ret;

	إذا (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		اذهب out;
	}

	nr_commits = todo.nr;
	لكل (صحيح i = 0; i < nr_commits; i++) {
		بنية todo_item *item = &todo.عناصر[i];
		ثابت حرف *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_رسالة);
		إذا (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		وإلا
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, عدم, عدم);
		find_commit_subject(message, &body);

		إذا (skip)
			commented_len = strlen(body);
		وإلا إذا (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		وإلا
			commented_len = 0;

		إذا (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_أسطر(out, body, commented_len, comment_سطر_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_سطر(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	إرجاع ret;
}

ساكن صحيح cmd_history_squash(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *prefix,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_run = 0;
	صحيح edit = 1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	بنية strbuf reflog_رسالة = STRBUF_INIT;
	بنية commit *oldest, *tip, *rewritten;
	ثابت بنية object_id *base_شجرة_oid, *tip_شجرة_oid;
	حرف *message_template = عدم;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	إذا (argc < 2) {
		ret = error(_("command expects a revision range"));
		اذهب out;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_رسالة, "squash: updating ");
	strbuf_join_argv(&reflog_رسالة, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	إذا (ret < 0)
		اذهب out;

	ret = setup_revwalk(repo, action, tip, &revs);
	إذا (ret < 0)
		اذهب out;

	base_شجرة_oid = &repo_get_commit_شجرة(repo,
					oldest->parents->item)->object.oid;
	tip_شجرة_oid = &repo_get_commit_شجرة(repo, tip)->object.oid;

	ret = commit_شجرة_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_شجرة_oid, tip_شجرة_oid,
			      &rewritten,
			      edit ? COMMIT_شجرة_EDIT_MESSAGE : 0);
	إذا (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		اذهب out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_رسالة.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_رسالة);
	release_revisions(&revs);
	حرر(message_template);
	إرجاع ret;
}

ساكن صحيح update_worktree(بنية repository *repo,
			   ثابت بنية commit *old_head,
			   ثابت بنية commit *new_head,
			   bool dry_run)
{
	بنية reset_working_شجرة_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	إذا (dry_run)
		opts.flags |= RESET_WORKING_شجرة_DRY_RUN;
	إرجاع reset_working_شجرة(repo, &opts);
}

ساكن صحيح find_head_شجرة_change(بنية repository *repo,
				 ثابت بنية replay_result *result,
				 بنية commit **old_head,
				 بنية commit **new_head,
				 bool *changed)
{
	ثابت بنية replay_ref_update *head_update = عدم;
	بنية commit *old_head_commit, *new_head_commit;
	بنية شجرة *old_head_شجرة, *new_head_شجرة;
	ثابت حرف *head_target;
	صحيح head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      عدم, &head_flags);
	إذا (!head_target)
		إرجاع error(_("cannot look up HEAD"));

	لكل (size_t i = 0; i < result->updates_nr; i++) {
		إذا (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			اكسر;
		}
	}

	إذا (!head_update)
		إرجاع 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	إذا (!old_head_commit || !new_head_commit)
		إرجاع error(_("cannot resolve HEAD commit"));

	old_head_شجرة = repo_get_commit_شجرة(repo, old_head_commit);
	new_head_شجرة = repo_get_commit_شجرة(repo, new_head_commit);
	إذا (!old_head_شجرة || !new_head_شجرة)
		إرجاع error(_("cannot resolve tree for HEAD"));

	إذا (oideq(&old_head_شجرة->object.oid, &new_head_شجرة->object.oid))
		إرجاع 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	إرجاع 0;
}

ساكن صحيح cmd_history_drop(صحيح argc,
			    ثابت حرف **argv,
			    ثابت حرف *prefix,
			    بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		عدم,
	};
	تعداد replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_run = 0;
	بنية option options[] = {
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
	بنية strbuf reflog_رسالة = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	بنية replay_result result = { 0 };
	بنية commit *old_head, *new_head;
	bool head_moves = false;
	صحيح ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب out;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب out;
	}

	إذا (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		اذهب out;
	} وإلا إذا (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		اذهب out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	إذا (ret) {
		ret = error(_("failed replaying descendants"));
		اذهب out;
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
	إذا (!is_bare_repository(repo)) {
		ret = find_head_شجرة_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		إذا (ret < 0)
			اذهب out;

		إذا (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			اذهب out;
		}
	}

	strbuf_addf(&reflog_رسالة, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_رسالة.buf, dry_run);
	إذا (ret < 0) {
		ret = error(_("failed to update references"));
		اذهب out;
	}

	إذا (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		اذهب out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_رسالة);
	release_revisions(&revs);
	إرجاع ret;
}

صحيح cmd_history(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *prefix,
		بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	إرجاع fn(argc, argv, prefix, repo);
}
