#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "merge-ort.h"
#அந்தர்பாவ "oidmap.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "ref-filter.h"
#அந்தர்பாவ "replay.h"
#அந்தர்பாவ "reset.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "wt-status.h"

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

நிலையான வெறுமை change_data_free(வெறுமை *util, மாறிலி எழுத்து *str UNUSED)
{
	கட்டமைப்பு wt_status_change_data *d = util;
	விடுவி(d->rename_source);
	விடுவி(d);
}

நிலையான முழு fill_commit_message(கட்டமைப்பு repository *repo,
			       மாறிலி கட்டமைப்பு object_id *old_மரம்,
			       மாறிலி கட்டமைப்பு object_id *new_மரம்,
			       மாறிலி எழுத்து *default_message,
			       மாறிலி எழுத்து *action,
			       கட்டமைப்பு strbuf *out)
{
	மாறிலி எழுத்து *path = git_path_commit_editmsg();
	மாறிலி எழுத்து *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	கட்டமைப்பு wt_status s;

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
	எனில் (!s.fp)
		திரும்பு error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_வரி_str, hint, action, comment_வரி_str);
	எனில் (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_மரம், new_மரம்);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	எனில் (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	எனில் (launch_editor(path, out, வெற்று)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		திரும்பு -1;
	}
	strbuf_stripspace(out, comment_வரி_str);

	cleanup_message(out, COMMIT_செய்தி_CLEANUP_ALL, 0);

	எனில் (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		திரும்பு -1;
	}

	திரும்பு 0;
}

எண்ணிக்கை commit_மரம்_flags {
	COMMIT_மரம்_EDIT_MESSAGE = (1 << 0),
};

நிலையான முழு commit_மரம்_ext(கட்டமைப்பு repository *repo,
			   மாறிலி எழுத்து *action,
			   கட்டமைப்பு commit *commit_with_message,
			   மாறிலி எழுத்து *message_template,
			   மாறிலி கட்டமைப்பு commit_list *parents,
			   மாறிலி கட்டமைப்பு object_id *old_மரம்,
			   மாறிலி கட்டமைப்பு object_id *new_மரம்,
			   கட்டமைப்பு commit **out,
			   எண்ணிக்கை commit_மரம்_flags flags)
{
	மாறிலி எழுத்து *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		வெற்று,
	};
	மாறிலி எழுத்து *original_message, *original_body, *ptr;
	கட்டமைப்பு commit_extra_header *original_extra_headers = வெற்று;
	கட்டமைப்பு strbuf commit_message = STRBUF_INIT;
	கட்டமைப்பு object_id rewritten_commit_oid;
	எழுத்து *original_author = வெற்று;
	size_t len;
	முழு ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, வெற்று, வெற்று);
	ptr = find_commit_header(original_message, "author", &len);
	எனில் (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	எனில் (!message_template)
		message_template = original_body;

	எனில் (flags & COMMIT_மரம்_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_மரம், new_மரம்,
					  message_template, action, &commit_message);
		எனில் (ret < 0)
			செல் out;
	} இல்லையெனில் {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_மரம்_extended(commit_message.buf, commit_message.len, new_மரம்,
				   parents, &rewritten_commit_oid, original_author,
				   வெற்று, வெற்று, original_extra_headers);
	எனில் (ret < 0)
		செல் out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	விடுவி(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	திரும்பு ret;
}

நிலையான முழு first_parent_மரம்_oid(கட்டமைப்பு repository *repo,
				 கட்டமைப்பு commit *commit,
				 கட்டமைப்பு object_id *out)
{
	கட்டமைப்பு commit *parent = commit->parents ? commit->parents->item : வெற்று;

	எனில் (!parent) {
		oidcpy(out, repo->hash_algo->empty_மரம்);
		திரும்பு 0;
	}

	எனில் (repo_parse_commit(repo, parent))
		திரும்பு error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_மரம்(repo, parent)->object.oid);
	திரும்பு 0;
}

நிலையான முழு commit_மரம்_with_edited_message(கட்டமைப்பு repository *repo,
					   மாறிலி எழுத்து *action,
					   கட்டமைப்பு commit *original,
					   கட்டமைப்பு commit **out)
{
	கட்டமைப்பு object_id parent_மரம்_oid;
	மாறிலி கட்டமைப்பு object_id *மரம்_oid;

	மரம்_oid = &repo_get_commit_மரம்(repo, original)->object.oid;

	எனில் (first_parent_மரம்_oid(repo, original, &parent_மரம்_oid) < 0)
		திரும்பு -1;

	திரும்பு commit_மரம்_ext(repo, action, original, வெற்று, original->parents,
			       &parent_மரம்_oid, மரம்_oid, out, COMMIT_மரம்_EDIT_MESSAGE);
}

எண்ணிக்கை ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

நிலையான முழு parse_ref_action(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *மதிப்பு, முழு unset)
{
	எண்ணிக்கை ref_action *action = opt->மதிப்பு;

	BUG_ON_OPT_NEG_NOARG(unset, மதிப்பு);
	எனில் (!strcmp(மதிப்பு, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} இல்லையெனில் எனில் (!strcmp(மதிப்பு, "head")) {
		*action = REF_ACTION_HEAD;
	} இல்லையெனில் {
		திரும்பு error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	திரும்பு 0;
}

நிலையான முழு revwalk_contains_merges(கட்டமைப்பு repository *repo,
				   மாறிலி கட்டமைப்பு strvec *revwalk_args)
{
	கட்டமைப்பு strvec args = STRVEC_INIT;
	கட்டமைப்பு rev_info revs;
	முழு ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_தள்ளு(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, வெற்று);

	setup_revisions_from_strvec(&args, &revs, வெற்று);
	எனில் (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	எனில் (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		செல் out;
	}

	எனில் (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		செல் out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	திரும்பு ret;
}

நிலையான முழு setup_revwalk(கட்டமைப்பு repository *repo,
			 எண்ணிக்கை ref_action action,
			 கட்டமைப்பு commit *original,
			 கட்டமைப்பு rev_info *revs)
{
	கட்டமைப்பு strvec args = STRVEC_INIT;
	முழு ret;

	repo_init_revisions(repo, revs, வெற்று);
	strvec_தள்ளு(&args, "ignored");
	strvec_தள்ளு(&args, "--reverse");
	strvec_தள்ளு(&args, "--topo-order");
	strvec_தள்ளு(&args, "--full-history");

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
	எனில் (action == REF_ACTION_HEAD) {
		கட்டமைப்பு commit_list *from_list = வெற்று;
		கட்டமைப்பு commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		எனில் (!head) {
			ret = error(_("cannot look up HEAD"));
			செல் out;
		}

		commit_list_செருகு(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		எனில் (ret < 0) {
			ret = error(_("cannot determine descendance"));
			செல் out;
		} இல்லையெனில் எனில் (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			செல் out;
		}

		strvec_தள்ளு(&args, "HEAD");
	} இல்லையெனில் {
		strvec_தள்ளு(&args, "--branches");
		strvec_தள்ளு(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	எனில் (ret < 0)
		செல் out;

	setup_revisions_from_strvec(&args, revs, வெற்று);
	எனில் (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	திரும்பு ret;
}

நிலையான முழு handle_ref_update(கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *refname,
			     மாறிலி கட்டமைப்பு object_id *new_oid,
			     மாறிலி கட்டமைப்பு object_id *old_oid,
			     மாறிலி எழுத்து *reflog_செய்தி,
			     கட்டமைப்பு strbuf *err)
{
	எனில் (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		திரும்பு 0;
	}

	திரும்பு ref_transaction_update(transaction, refname, new_oid, old_oid,
				      வெற்று, வெற்று, 0, reflog_செய்தி, err);
}

நிலையான முழு compute_pending_ref_updates(கட்டமைப்பு rev_info *revs,
				       எண்ணிக்கை ref_action action,
				       கட்டமைப்பு commit *original,
				       கட்டமைப்பு commit *rewritten,
				       எண்ணிக்கை replay_empty_commit_action empty,
				       கட்டமைப்பு replay_result *result)
{
	மாறிலி கட்டமைப்பு name_decoration *decoration;
	கட்டமைப்பு replay_revisions_options opts = {
		.empty = empty,
	};
	எழுத்து hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	முழு head_flags = 0;
	முழு ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, வெற்று, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	எனில் (ret)
		திரும்பு ret;

	எனில் (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	க்கு (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		எனில் (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			தொடர்;

		எனில் (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			தொடர்;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		எனில் (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			தொடர்;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	திரும்பு 0;
}

நிலையான முழு apply_pending_ref_updates(கட்டமைப்பு repository *repo,
				     மாறிலி கட்டமைப்பு replay_result *result,
				     மாறிலி எழுத்து *reflog_செய்தி,
				     முழு dry_run)
{
	கட்டமைப்பு ref_transaction *transaction = வெற்று;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	முழு ret;

	எனில் (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		எனில் (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			செல் out;
		}
	}

	க்கு (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_செய்தி, &err);
		எனில் (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			செல் out;
		}
	}

	எனில் (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		செல் out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	திரும்பு ret;
}

நிலையான முழு handle_reference_updates(கட்டமைப்பு rev_info *revs,
				    எண்ணிக்கை ref_action action,
				    கட்டமைப்பு commit *original,
				    கட்டமைப்பு commit *rewritten,
				    மாறிலி எழுத்து *reflog_செய்தி,
				    முழு dry_run,
				    எண்ணிக்கை replay_empty_commit_action empty)
{
	கட்டமைப்பு replay_result result = { 0 };
	முழு ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	எனில் (ret)
		செல் out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_செய்தி, dry_run);

out:
	replay_result_release(&result);
	திரும்பு ret;
}

நிலையான முழு commit_became_empty(கட்டமைப்பு repository *repo,
			       கட்டமைப்பு commit *original,
			       கட்டமைப்பு மரம் *result)
{
	கட்டமைப்பு object_id parent_மரம்_oid;

	எனில் (first_parent_மரம்_oid(repo, original, &parent_மரம்_oid) < 0)
		திரும்பு -1;

	திரும்பு oideq(&result->object.oid, &parent_மரம்_oid);
}

நிலையான முழு parse_opt_empty(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	எண்ணிக்கை replay_empty_commit_action *மதிப்பு = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!strcmp(arg, "drop"))
		*மதிப்பு = REPLAY_EMPTY_COMMIT_DROP;
	இல்லையெனில் எனில் (!strcmp(arg, "keep"))
		*மதிப்பு = REPLAY_EMPTY_COMMIT_KEEP;
	இல்லையெனில் எனில் (!strcmp(arg, "abort"))
		*மதிப்பு = REPLAY_EMPTY_COMMIT_ABORT;
	இல்லையெனில்
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	திரும்பு 0;
}

நிலையான முழு cmd_history_fixup(முழு argc,
			     மாறிலி எழுத்து **argv,
			     மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		வெற்று,
	};
	எண்ணிக்கை replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	எண்ணிக்கை ref_action action = REF_ACTION_DEFAULT;
	எண்ணிக்கை commit_மரம்_flags flags = 0;
	முழு dry_run = 0;
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_மரம்_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	கட்டமைப்பு merge_result merge_result = { 0 };
	கட்டமைப்பு merge_options merge_opts = { 0 };
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு commit *head_commit, *original, *rewritten;
	கட்டமைப்பு மரம் *head_மரம், *original_மரம், *index_மரம்;
	கட்டமைப்பு rev_info revs = { 0 };
	bool skip_commit = false;
	முழு ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	எனில் (argc != 1) {
		ret = error(_("command expects a single revision"));
		செல் out;
	}
	repo_config(repo, git_default_config, வெற்று);

	எனில் (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	எனில் (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		செல் out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	எனில் (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		செல் out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	எனில் (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		செல் out;
	}

	head_மரம் = repo_get_commit_மரம்(repo, head_commit);
	எனில் (!head_மரம்) {
		ret = error(_("cannot get tree for HEAD"));
		செல் out;
	}

	எனில் (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		செல் out;
	}

	எனில் (!repo_index_has_changes(repo, head_மரம், வெற்று)) {
		ret = error(_("nothing to fixup: no staged changes"));
		செல் out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_மரம் = write_in_core_index_as_மரம்(repo, repo->index);
	எனில் (!index_மரம்) {
		ret = error(_("unable to write index as a tree"));
		செல் out;
	}

	original_மரம் = repo_get_commit_மரம்(repo, original);
	எனில் (!original_மரம்) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		செல் out;
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
	merge_incore_nonrecursive(&merge_opts, head_மரம்,
				  original_மரம், index_மரம், &merge_result);

	எனில் (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		செல் out;
	}

	எனில் (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		செல் out;
	}

	ret = commit_became_empty(repo, original, merge_result.மரம்);
	எனில் (ret < 0)
		செல் out;
	எனில் (ret > 0) {
		தேர்வு (empty) {
		நிலை REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : வெற்று;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			எனில் (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				செல் out;
			}

			skip_commit = true;
			நிறுத்து;
		நிலை REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			நிறுத்து;
		நிலை REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			செல் out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	எனில் (ret)
		செல் out;

	எனில் (!skip_commit) {
		ret = commit_மரம்_ext(repo, "fixup", original, வெற்று, original->parents,
				      &original_மரம்->object.oid, &merge_result.மரம்->object.oid,
				      &rewritten, flags);
		எனில் (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			செல் out;
		}
	}

	strbuf_addf(&reflog_செய்தி, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_செய்தி.buf, dry_run, empty);
	எனில் (ret < 0) {
		ret = error(_("failed replaying descendants"));
		செல் out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_செய்தி);
	release_revisions(&revs);
	திரும்பு ret;
}

நிலையான முழு cmd_history_reword(முழு argc,
			      மாறிலி எழுத்து **argv,
			      மாறிலி எழுத்து *prefix,
			      கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		வெற்று,
	};
	எண்ணிக்கை ref_action action = REF_ACTION_DEFAULT;
	முழு dry_run = 0;
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு commit *original, *rewritten;
	கட்டமைப்பு rev_info revs = { 0 };
	முழு ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	எனில் (argc != 1) {
		ret = error(_("command expects a single revision"));
		செல் out;
	}
	repo_config(repo, git_default_config, வெற்று);

	எனில் (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	எனில் (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		செல் out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	எனில் (ret)
		செல் out;

	ret = commit_மரம்_with_edited_message(repo, "reworded", original, &rewritten);
	எனில் (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		செல் out;
	}

	strbuf_addf(&reflog_செய்தி, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_செய்தி.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	எனில் (ret < 0) {
		ret = error(_("failed replaying descendants"));
		செல் out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_செய்தி);
	release_revisions(&revs);
	திரும்பு ret;
}

நிலையான முழு write_ondisk_index(கட்டமைப்பு repository *repo,
			      கட்டமைப்பு object_id *oid,
			      மாறிலி எழுத்து *path)
{
	கட்டமைப்பு unpack_trees_options opts = { 0 };
	கட்டமைப்பு lock_file lock = LOCK_INIT;
	கட்டமைப்பு மரம்_desc மரம்_desc;
	கட்டமைப்பு index_state index;
	கட்டமைப்பு மரம் *மரம்;
	முழு ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	மரம் = repo_parse_மரம்_indirect(repo, oid);
	எனில் (!மரம்) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		செல் out;
	}
	init_மரம்_desc(&மரம்_desc, &மரம்->object.oid, மரம்->buffer, மரம்->size);

	எனில் (unpack_trees(1, &மரம்_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		செல் out;
	}

	பகாஎண்_cache_மரம்(repo, &index, மரம்);

	எனில் (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		செல் out;
	}

	எனில் (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		செல் out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	திரும்பு ret;
}

நிலையான முழு split_commit(கட்டமைப்பு repository *repo,
			கட்டமைப்பு commit *original,
			கட்டமைப்பு pathspec *pathspec,
			கட்டமைப்பு commit **out)
{
	கட்டமைப்பு interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	கட்டமைப்பு strbuf index_file = STRBUF_INIT;
	கட்டமைப்பு index_state index = INDEX_STATE_INIT(repo);
	மாறிலி கட்டமைப்பு object_id *original_commit_மரம்_oid;
	மாறிலி கட்டமைப்பு object_id *old_மரம்_oid, *new_மரம்_oid;
	கட்டமைப்பு object_id parent_மரம்_oid;
	எழுத்து original_commit_oid[GIT_MAX_HEXSZ + 1];
	கட்டமைப்பு commit *first_commit, *second_commit;
	கட்டமைப்பு commit_list *parents = வெற்று;
	கட்டமைப்பு மரம் *split_மரம்;
	முழு ret;

	எனில் (first_parent_மரம்_oid(repo, original, &parent_மரம்_oid) < 0) {
		ret = -1;
		செல் out;
	}
	original_commit_மரம்_oid = get_commit_மரம்_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_மரம்_oid, index_file.buf);
	எனில் (ret < 0)
		செல் out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	எனில் (ret < 0) {
		ret = error(_("failed reading temporary index"));
		செல் out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	எனில் (ret < 0)
		செல் out;

	split_மரம் = write_in_core_index_as_மரம்(repo, &index);
	எனில் (!split_மரம்) {
		ret = error(_("failed split tree"));
		செல் out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	எனில் (oideq(&split_மரம்->object.oid, &parent_மரம்_oid)) {
		ret = error(_("split commit is empty"));
		செல் out;
	} இல்லையெனில் எனில் (oideq(&split_மரம்->object.oid, original_commit_மரம்_oid)) {
		ret = error(_("split commit tree matches original commit"));
		செல் out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_மரம்_ext(repo, "split-out", original, வெற்று, original->parents, &parent_மரம்_oid,
			      &split_மரம்->object.oid, &first_commit, COMMIT_மரம்_EDIT_MESSAGE);
	எனில் (ret < 0) {
		ret = error(_("failed writing first commit"));
		செல் out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_மரம்_oid = &repo_get_commit_மரம்(repo, first_commit)->object.oid;
	new_மரம்_oid = &repo_get_commit_மரம்(repo, original)->object.oid;

	ret = commit_மரம்_ext(repo, "split-out", original, வெற்று, parents, old_மரம்_oid,
			      new_மரம்_oid, &second_commit, COMMIT_மரம்_EDIT_MESSAGE);
	எனில் (ret < 0) {
		ret = error(_("failed writing second commit"));
		செல் out;
	}

	*out = second_commit;
	ret = 0;

out:
	எனில் (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	திரும்பு ret;
}

நிலையான முழு cmd_history_split(முழு argc,
			     மாறிலி எழுத்து **argv,
			     மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		வெற்று,
	};
	எண்ணிக்கை ref_action action = REF_ACTION_DEFAULT;
	முழு dry_run = 0;
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	கட்டமைப்பு commit *original, *rewritten = வெற்று;
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு pathspec pathspec = { 0 };
	கட்டமைப்பு rev_info revs = { 0 };
	முழு ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	எனில் (argc < 1) {
		ret = error(_("command expects a committish"));
		செல் out;
	}
	repo_config(repo, git_default_config, வெற்று);

	எனில் (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	எனில் (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		செல் out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	எனில் (ret < 0)
		செல் out;

	எனில் (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		செல் out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	எனில் (ret < 0)
		செல் out;

	strbuf_addf(&reflog_செய்தி, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_செய்தி.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	எனில் (ret < 0) {
		ret = error(_("failed replaying descendants"));
		செல் out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_செய்தி);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	திரும்பு ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

நிலையான bool is_autosquash_subject(மாறிலி எழுத்து *s)
{
	திரும்பு starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

நிலையான bool skip_one_autosquash_prefix(மாறிலி எழுத்து *s, மாறிலி எழுத்து **out)
{
	எனில் (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		போது (**out == ' ')
			(*out)++;
		திரும்பு true;
	}
	திரும்பு false;
}

நிலையான வெறுமை truncate_message_to_subject(கட்டமைப்பு strbuf *செய்தி)
{
	மாறிலி எழுத்து *eos = strstr(செய்தி->buf, "\n\n");

	எனில் (eos)
		strbuf_setlen(செய்தி, eos - செய்தி->buf + 1);
}

கட்டமைப்பு subject_data {
	கட்டமைப்பு strintmap subjects;
	கட்டமைப்பு strbuf subject;
	கட்டமைப்பு strbuf squash_message;
	மாறிலி எழுத்து *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

நிலையான வெறுமை subject_data_clear(கட்டமைப்பு subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

நிலையான முழு squash_amend_message(கட்டமைப்பு repository *repo,
				கட்டமைப்பு commit *commit,
				கட்டமைப்பு subject_data *data,
				unsigned flags)
{
	மாறிலி எழுத்து *body = data->message + data->subject.len;

	போது (isspace(*body))
		body++;

	எனில் (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		திரும்பு 0;
	}

	எனில் (data->edit_message) {
		திரும்பு 0;
	} இல்லையெனில் எனில் (flags & SQUASH_AMEND_TARGET) {
		எனில் (starts_with(data->squash_message.buf, "squash!"))
			திரும்பு error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		எனில் (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		எனில் (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} இல்லையெனில் {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_வரி(&data->squash_message);
	} இல்லையெனில் {
		திரும்பு error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	திரும்பு 0;
}

நிலையான முழு squash_squash_message(கட்டமைப்பு repository *repo,
				கட்டமைப்பு commit *commit,
				கட்டமைப்பு subject_data *data,
				unsigned flags)
{
	மாறிலி எழுத்து *body = data->message + data->subject.len;

	போது (isspace(*body))
		body++;

	எனில் (data->edit_message) {
		திரும்பு 0;
	} இல்லையெனில் எனில் (flags & SQUASH_AMEND_TARGET) {
		எனில் (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		எனில் (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_வரி(&data->squash_message);
		} இல்லையெனில் {
			திரும்பு error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} இல்லையெனில் {
		திரும்பு error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	திரும்பு 0;
}

நிலையான முழு squash_check_can_autosquash(கட்டமைப்பு repository *repo,
				       கட்டமைப்பு commit *commit,
				       கட்டமைப்பு subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	எனில் (starts_with(data->subject.buf, "amend!"))
		திரும்பு squash_amend_message(repo, commit, data, flags);
	இல்லையெனில் எனில் (starts_with(data->subject.buf, "squash!"))
		திரும்பு squash_squash_message(repo, commit, data, flags);

	திரும்பு 0;
}

நிலையான முழு squash_check_autosquash_subject(கட்டமைப்பு repository *repo,
					   கட்டமைப்பு commit *commit,
					   கட்டமைப்பு subject_data *data)
{
	மாறிலி எழுத்து* s = data->subject.buf;
	கட்டமைப்பு commit *target;
	கட்டமைப்பு hashmap_iter iter;
	கட்டமைப்பு strmap_entry *entry;
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
	போது (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		எனில் (flags)
			திரும்பு squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	எனில் (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		எனில் (!flags) {
			மாறிலி எழுத்து *subject_start;
			மாறிலி எழுத்து *buffer = repo_logmsg_reencode(repo, target,
								  வெற்று, வெற்று);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			எழுத்து *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			விடுவி(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		எனில் (flags)
			திரும்பு squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		போது(skip_one_autosquash_prefix(s, &s)) {
			எனில் (starts_with(entry->சாவி, s)) {
				unsigned மதிப்பு = (intptr_t)entry->மதிப்பு;

				திரும்பு squash_check_can_autosquash(repo, commit,
								   data, மதிப்பு);
			}
		}
	}
	திரும்பு error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

நிலையான முழு squash_check_subject(கட்டமைப்பு repository *repo,
				கட்டமைப்பு commit *commit,
				கட்டமைப்பு subject_data *data)
{
	முழு ret = 0;
	மாறிலி எழுத்து *buf = repo_logmsg_reencode(repo, commit, வெற்று, வெற்று);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	எனில் (!strintmap_get_size(&data->subjects)) {
		மாறிலி எழுத்து *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_வரி(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} இல்லையெனில் எனில் (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} இல்லையெனில் {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	திரும்பு ret;
}

நிலையான முழு build_squash_message(கட்டமைப்பு repository *repo,
				மாறிலி கட்டமைப்பு strbuf *todo_buf,
				கட்டமைப்பு strbuf *out);

நிலையான முழு setup_squash_revisions(கட்டமைப்பு repository *repo,
				  முழு argc, மாறிலி எழுத்து **argv,
				  கட்டமைப்பு rev_info *revs)
{
	repo_init_revisions(repo, revs, வெற்று);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, வெற்று);
	எனில் (argc > 1)
		திரும்பு error(_("unrecognized argument: %s"), argv[1]);

	எனில் (revs->reverse != 1 || revs->topo_order != 1 ||
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
	க்கு (size_t i = 0; i < revs->cmdline.nr; i++)
		எனில் (revs->cmdline.rev[i].flags & BOTTOM)
			திரும்பு 0;

	திரும்பு error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
நிலையான முழு resolve_squash_range(கட்டமைப்பு repository *repo,
				bool update_branches,
				bool edit_message,
				முழு argc, மாறிலி எழுத்து **argv,
				கட்டமைப்பு commit **oldest_out,
				கட்டமைப்பு commit **tip_out,
				எழுத்து **message_out)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு subject_data subject_data = SUBJECT_DATA_INIT;
	கட்டமைப்பு commit *commit, *oldest = வெற்று, *tip = வெற்று;
	கட்டமைப்பு strbuf todo_buf = STRBUF_INIT;
	முழு ret, tip_மொத்தம் = 0;
	bool walk_started = false;
	கட்டமைப்பு ref_filter filter = REF_FILTER_INIT;
	கட்டமைப்பு ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	எனில் (ret < 0)
		செல் out;

	எனில் (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		செல் out;
	}
	walk_started = true;
	போது ((commit = get_revision(&revs))) {
		கட்டமைப்பு commit_list *p;

		எனில் (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		எனில் (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			செல் out;
		}
		க்கு (p = commit->parents; oldest && p; p = p->next) {
			கட்டமைப்பு commit_list *q;
			கட்டமைப்பு object *o;
			bool seen;

			எனில் (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				செல் out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			க்கு (q = oldest->parents; !seen && q; q = q->next) {
				எனில் (p->item == q->item) {
					seen = true;
					commit_list_செருகு(commit, &filter.with_commit);
				}
			}
			எனில் (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				செல் out;
			}
			எனில் (o->flags & SQUASH_TIP) {
				tip_மொத்தம்--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		எனில் (!oldest) {
			commit_list_செருகு(commit, &filter.with_commit);
			oldest = commit;
		}
		எனில் (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			செல் out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_மொத்தம்++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	எனில் (!tip_மொத்தம்) {
		ret = error(_("the revision range is empty"));
		செல் out;
	} இல்லையெனில் எனில் (tip_மொத்தம் != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		செல் out;
	} இல்லையெனில் எனில் (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		செல் out;
	} இல்லையெனில் எனில் (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_செருகு(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	எனில் (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		செல் out;
	}
	எனில் (refs.nr) {
		கட்டமைப்பு ref_format format = REF_FORMAT_INIT;
		கட்டமைப்பு ref_sorting *sorting;
		கட்டமைப்பு string_list sorting_options = STRING_LIST_INIT_DUP;
		கட்டமைப்பு strbuf branches = STRBUF_INIT;
		கட்டமைப்பு strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		எனில் (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		க்கு (முழு i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			எனில் (format_ref_array_item(refs.பொருட்கள்[i], &format,
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
		செல் out;
	}
	எனில் (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		எனில் (ret < 0)
			செல் out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, வெற்று);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	எனில் (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	திரும்பு ret;
}

நிலையான bool amend_replaces_target(கட்டமைப்பு todo_list *todo, முழு target)
{
	க்கு (முழு i = target + 1; i < todo->nr &&
				 todo->பொருட்கள்[i].command != TODO_PICK; i++) {
		எனில் (todo->பொருட்கள்[i].command == TODO_SQUASH)
			திரும்பு false;
		எனில் (todo->பொருட்கள்[i].flags & TODO_REPLACE_FIXUP_செய்தி)
			திரும்பு true;
	}
	திரும்பு false;
}

நிலையான முழு build_squash_message(கட்டமைப்பு repository *repo,
				மாறிலி கட்டமைப்பு strbuf *todo_buf,
				கட்டமைப்பு strbuf *out)
{
	கட்டமைப்பு todo_list todo = TODO_LIST_INIT;
	கட்டமைப்பு replay_opts opts = REPLAY_OPTS_INIT;
	முழு nr_commits, ret;

	எனில் (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		செல் out;
	}

	nr_commits = todo.nr;
	க்கு (முழு i = 0; i < nr_commits; i++) {
		கட்டமைப்பு todo_item *item = &todo.பொருட்கள்[i];
		மாறிலி எழுத்து *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_செய்தி);
		எனில் (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		இல்லையெனில்
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, வெற்று, வெற்று);
		find_commit_subject(message, &body);

		எனில் (skip)
			commented_len = strlen(body);
		இல்லையெனில் எனில் (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		இல்லையெனில்
			commented_len = 0;

		எனில் (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_வரிகள்(out, body, commented_len, comment_வரி_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_வரி(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	திரும்பு ret;
}

நிலையான முழு cmd_history_squash(முழு argc,
			      மாறிலி எழுத்து **argv,
			      மாறிலி எழுத்து *prefix,
			      கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		வெற்று,
	};
	எண்ணிக்கை ref_action action = REF_ACTION_DEFAULT;
	முழு dry_run = 0;
	முழு edit = 1;
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு commit *oldest, *tip, *rewritten;
	மாறிலி கட்டமைப்பு object_id *base_மரம்_oid, *tip_மரம்_oid;
	எழுத்து *message_template = வெற்று;
	கட்டமைப்பு rev_info revs = { 0 };
	முழு ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	எனில் (argc < 2) {
		ret = error(_("command expects a revision range"));
		செல் out;
	}
	repo_config(repo, git_default_config, வெற்று);

	எனில் (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_செய்தி, "squash: updating ");
	strbuf_join_argv(&reflog_செய்தி, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	எனில் (ret < 0)
		செல் out;

	ret = setup_revwalk(repo, action, tip, &revs);
	எனில் (ret < 0)
		செல் out;

	base_மரம்_oid = &repo_get_commit_மரம்(repo,
					oldest->parents->item)->object.oid;
	tip_மரம்_oid = &repo_get_commit_மரம்(repo, tip)->object.oid;

	ret = commit_மரம்_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_மரம்_oid, tip_மரம்_oid,
			      &rewritten,
			      edit ? COMMIT_மரம்_EDIT_MESSAGE : 0);
	எனில் (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		செல் out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_செய்தி.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	எனில் (ret < 0) {
		ret = error(_("failed replaying descendants"));
		செல் out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_செய்தி);
	release_revisions(&revs);
	விடுவி(message_template);
	திரும்பு ret;
}

நிலையான முழு update_worktree(கட்டமைப்பு repository *repo,
			   மாறிலி கட்டமைப்பு commit *old_head,
			   மாறிலி கட்டமைப்பு commit *new_head,
			   bool dry_run)
{
	கட்டமைப்பு reset_working_மரம்_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	எனில் (dry_run)
		opts.flags |= RESET_WORKING_மரம்_DRY_RUN;
	திரும்பு reset_working_மரம்(repo, &opts);
}

நிலையான முழு find_head_மரம்_change(கட்டமைப்பு repository *repo,
				 மாறிலி கட்டமைப்பு replay_result *result,
				 கட்டமைப்பு commit **old_head,
				 கட்டமைப்பு commit **new_head,
				 bool *changed)
{
	மாறிலி கட்டமைப்பு replay_ref_update *head_update = வெற்று;
	கட்டமைப்பு commit *old_head_commit, *new_head_commit;
	கட்டமைப்பு மரம் *old_head_மரம், *new_head_மரம்;
	மாறிலி எழுத்து *head_target;
	முழு head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      வெற்று, &head_flags);
	எனில் (!head_target)
		திரும்பு error(_("cannot look up HEAD"));

	க்கு (size_t i = 0; i < result->updates_nr; i++) {
		எனில் (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			நிறுத்து;
		}
	}

	எனில் (!head_update)
		திரும்பு 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	எனில் (!old_head_commit || !new_head_commit)
		திரும்பு error(_("cannot resolve HEAD commit"));

	old_head_மரம் = repo_get_commit_மரம்(repo, old_head_commit);
	new_head_மரம் = repo_get_commit_மரம்(repo, new_head_commit);
	எனில் (!old_head_மரம் || !new_head_மரம்)
		திரும்பு error(_("cannot resolve tree for HEAD"));

	எனில் (oideq(&old_head_மரம்->object.oid, &new_head_மரம்->object.oid))
		திரும்பு 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	திரும்பு 0;
}

நிலையான முழு cmd_history_drop(முழு argc,
			    மாறிலி எழுத்து **argv,
			    மாறிலி எழுத்து *prefix,
			    கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		GIT_HISTORY_DROP_USAGE,
		வெற்று,
	};
	எண்ணிக்கை replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	எண்ணிக்கை ref_action action = REF_ACTION_DEFAULT;
	முழு dry_run = 0;
	கட்டமைப்பு option options[] = {
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
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு commit *original, *rewritten;
	கட்டமைப்பு rev_info revs = { 0 };
	கட்டமைப்பு replay_result result = { 0 };
	கட்டமைப்பு commit *old_head, *new_head;
	bool head_moves = false;
	முழு ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	எனில் (argc != 1) {
		ret = error(_("command expects a single revision"));
		செல் out;
	}
	repo_config(repo, git_default_config, வெற்று);

	எனில் (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	எனில் (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		செல் out;
	}

	எனில் (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		செல் out;
	} இல்லையெனில் எனில் (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		செல் out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	எனில் (ret)
		செல் out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	எனில் (ret) {
		ret = error(_("failed replaying descendants"));
		செல் out;
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
	எனில் (!is_bare_repository(repo)) {
		ret = find_head_மரம்_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		எனில் (ret < 0)
			செல் out;

		எனில் (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			செல் out;
		}
	}

	strbuf_addf(&reflog_செய்தி, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_செய்தி.buf, dry_run);
	எனில் (ret < 0) {
		ret = error(_("failed to update references"));
		செல் out;
	}

	எனில் (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		செல் out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_செய்தி);
	release_revisions(&revs);
	திரும்பு ret;
}

முழு cmd_history(முழு argc,
		மாறிலி எழுத்து **argv,
		மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		வெற்று,
	};
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	திரும்பு fn(argc, argv, prefix, repo);
}
