#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "advice.h"
#include "cache-tree.h"
#include "commit.h"
#include "commit-reach.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "lockfile.h"
#include "merge-ort.h"
#include "oidmap.h"
#include "parse-options.h"
#include "path.h"
#include "read-cache.h"
#include "refs.h"
#include "ref-filter.h"
#include "replay.h"
#include "reset.h"
#include "revision.h"
#include "sequencer.h"
#include "string-list.h"
#include "strvec.h"
#include "tree.h"
#include "tree-walk.h"
#include "unpack-trees.h"
#include "wt-status.h"

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

static void change_data_free(void *util, const char *str UNUSED)
{
	struct wt_status_change_data *d = util;
	free(d->rename_source);
	free(d);
}

static ціле fill_commit_message(struct repository *repo,
			       const struct object_id *old_tree,
			       const struct object_id *new_tree,
			       const char *default_message,
			       const char *action,
			       struct strbuf *out)
{
	const char *path = git_path_commit_editmsg();
	const char *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	struct wt_status s;

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
	якщо (!s.fp)
		повернути error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_line_str, hint, action, comment_line_str);
	якщо (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_tree, new_tree);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	якщо (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	якщо (launch_editor(path, out, NULL)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		повернути -1;
	}
	strbuf_stripspace(out, comment_line_str);

	cleanup_message(out, COMMIT_MSG_CLEANUP_ALL, 0);

	якщо (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		повернути -1;
	}

	повернути 0;
}

enum commit_tree_flags {
	COMMIT_TREE_EDIT_MESSAGE = (1 << 0),
};

static ціле commit_tree_ext(struct repository *repo,
			   const char *action,
			   struct commit *commit_with_message,
			   const char *message_template,
			   const struct commit_list *parents,
			   const struct object_id *old_tree,
			   const struct object_id *new_tree,
			   struct commit **out,
			   enum commit_tree_flags flags)
{
	const char *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		NULL,
	};
	const char *original_message, *original_body, *ptr;
	struct commit_extra_header *original_extra_headers = NULL;
	struct strbuf commit_message = STRBUF_INIT;
	struct object_id rewritten_commit_oid;
	char *original_author = NULL;
	size_t len;
	ціле ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, NULL, NULL);
	ptr = find_commit_header(original_message, "author", &len);
	якщо (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	якщо (!message_template)
		message_template = original_body;

	якщо (flags & COMMIT_TREE_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_tree, new_tree,
					  message_template, action, &commit_message);
		якщо (ret < 0)
			goto out;
	} інакше {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_tree_extended(commit_message.buf, commit_message.len, new_tree,
				   parents, &rewritten_commit_oid, original_author,
				   NULL, NULL, original_extra_headers);
	якщо (ret < 0)
		goto out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	free(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	повернути ret;
}

static ціле first_parent_tree_oid(struct repository *repo,
				 struct commit *commit,
				 struct object_id *out)
{
	struct commit *parent = commit->parents ? commit->parents->item : NULL;

	якщо (!parent) {
		oidcpy(out, repo->hash_algo->empty_tree);
		повернути 0;
	}

	якщо (repo_parse_commit(repo, parent))
		повернути error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_tree(repo, parent)->object.oid);
	повернути 0;
}

static ціле commit_tree_with_edited_message(struct repository *repo,
					   const char *action,
					   struct commit *original,
					   struct commit **out)
{
	struct object_id parent_tree_oid;
	const struct object_id *tree_oid;

	tree_oid = &repo_get_commit_tree(repo, original)->object.oid;

	якщо (first_parent_tree_oid(repo, original, &parent_tree_oid) < 0)
		повернути -1;

	повернути commit_tree_ext(repo, action, original, NULL, original->parents,
			       &parent_tree_oid, tree_oid, out, COMMIT_TREE_EDIT_MESSAGE);
}

enum ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

static ціле parse_ref_action(const struct option *opt, const char *value, ціле unset)
{
	enum ref_action *action = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, value);
	якщо (!strcmp(value, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} інакше якщо (!strcmp(value, "head")) {
		*action = REF_ACTION_HEAD;
	} інакше {
		повернути error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	повернути 0;
}

static ціле revwalk_contains_merges(struct repository *repo,
				   const struct strvec *revwalk_args)
{
	struct strvec args = STRVEC_INIT;
	struct rev_info revs;
	ціле ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_push(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, NULL);

	setup_revisions_from_strvec(&args, &revs, NULL);
	якщо (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	якщо (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		goto out;
	}

	якщо (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		goto out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	повернути ret;
}

static ціле setup_revwalk(struct repository *repo,
			 enum ref_action action,
			 struct commit *original,
			 struct rev_info *revs)
{
	struct strvec args = STRVEC_INIT;
	ціле ret;

	repo_init_revisions(repo, revs, NULL);
	strvec_push(&args, "ignored");
	strvec_push(&args, "--reverse");
	strvec_push(&args, "--topo-order");
	strvec_push(&args, "--full-history");

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
	якщо (action == REF_ACTION_HEAD) {
		struct commit_list *from_list = NULL;
		struct commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		якщо (!head) {
			ret = error(_("cannot look up HEAD"));
			goto out;
		}

		commit_list_insert(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		якщо (ret < 0) {
			ret = error(_("cannot determine descendance"));
			goto out;
		} інакше якщо (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			goto out;
		}

		strvec_push(&args, "HEAD");
	} інакше {
		strvec_push(&args, "--branches");
		strvec_push(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	якщо (ret < 0)
		goto out;

	setup_revisions_from_strvec(&args, revs, NULL);
	якщо (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	повернути ret;
}

static ціле handle_ref_update(struct ref_transaction *transaction,
			     const char *refname,
			     const struct object_id *new_oid,
			     const struct object_id *old_oid,
			     const char *reflog_msg,
			     struct strbuf *err)
{
	якщо (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		повернути 0;
	}

	повернути ref_transaction_update(transaction, refname, new_oid, old_oid,
				      NULL, NULL, 0, reflog_msg, err);
}

static ціле compute_pending_ref_updates(struct rev_info *revs,
				       enum ref_action action,
				       struct commit *original,
				       struct commit *rewritten,
				       enum replay_empty_commit_action empty,
				       struct replay_result *result)
{
	const struct name_decoration *decoration;
	struct replay_revisions_options opts = {
		.empty = empty,
	};
	char hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	ціле head_flags = 0;
	ціле ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, NULL, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	якщо (ret)
		повернути ret;

	якщо (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	для (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		якщо (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			continue;

		якщо (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			continue;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		якщо (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			continue;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	повернути 0;
}

static ціле apply_pending_ref_updates(struct repository *repo,
				     const struct replay_result *result,
				     const char *reflog_msg,
				     ціле dry_run)
{
	struct ref_transaction *transaction = NULL;
	struct strbuf err = STRBUF_INIT;
	ціле ret;

	якщо (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		якщо (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			goto out;
		}
	}

	для (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_msg, &err);
		якщо (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			goto out;
		}
	}

	якщо (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		goto out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	повернути ret;
}

static ціле handle_reference_updates(struct rev_info *revs,
				    enum ref_action action,
				    struct commit *original,
				    struct commit *rewritten,
				    const char *reflog_msg,
				    ціле dry_run,
				    enum replay_empty_commit_action empty)
{
	struct replay_result result = { 0 };
	ціле ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	якщо (ret)
		goto out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_msg, dry_run);

out:
	replay_result_release(&result);
	повернути ret;
}

static ціле commit_became_empty(struct repository *repo,
			       struct commit *original,
			       struct tree *result)
{
	struct object_id parent_tree_oid;

	якщо (first_parent_tree_oid(repo, original, &parent_tree_oid) < 0)
		повернути -1;

	повернути oideq(&result->object.oid, &parent_tree_oid);
}

static ціле parse_opt_empty(const struct option *opt, const char *arg, ціле unset)
{
	enum replay_empty_commit_action *value = opt->value;

	BUG_ON_OPT_NEG(unset);

	якщо (!strcmp(arg, "drop"))
		*value = REPLAY_EMPTY_COMMIT_DROP;
	інакше якщо (!strcmp(arg, "keep"))
		*value = REPLAY_EMPTY_COMMIT_KEEP;
	інакше якщо (!strcmp(arg, "abort"))
		*value = REPLAY_EMPTY_COMMIT_ABORT;
	інакше
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	повернути 0;
}

static ціле cmd_history_fixup(ціле argc,
			     const char **argv,
			     const char *prefix,
			     struct repository *repo)
{
	const char * const usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		NULL,
	};
	enum replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	enum ref_action action = REF_ACTION_DEFAULT;
	enum commit_tree_flags flags = 0;
	ціле dry_run = 0;
	struct option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_TREE_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	struct merge_result merge_result = { 0 };
	struct merge_options merge_opts = { 0 };
	struct strbuf reflog_msg = STRBUF_INIT;
	struct commit *head_commit, *original, *rewritten;
	struct tree *head_tree, *original_tree, *index_tree;
	struct rev_info revs = { 0 };
	bool skip_commit = false;
	ціле ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	якщо (argc != 1) {
		ret = error(_("command expects a single revision"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	якщо (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	якщо (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		goto out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	якщо (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	якщо (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		goto out;
	}

	head_tree = repo_get_commit_tree(repo, head_commit);
	якщо (!head_tree) {
		ret = error(_("cannot get tree for HEAD"));
		goto out;
	}

	якщо (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		goto out;
	}

	якщо (!repo_index_has_changes(repo, head_tree, NULL)) {
		ret = error(_("nothing to fixup: no staged changes"));
		goto out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_tree = write_in_core_index_as_tree(repo, repo->index);
	якщо (!index_tree) {
		ret = error(_("unable to write index as a tree"));
		goto out;
	}

	original_tree = repo_get_commit_tree(repo, original);
	якщо (!original_tree) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		goto out;
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
	merge_incore_nonrecursive(&merge_opts, head_tree,
				  original_tree, index_tree, &merge_result);

	якщо (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		goto out;
	}

	якщо (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		goto out;
	}

	ret = commit_became_empty(repo, original, merge_result.tree);
	якщо (ret < 0)
		goto out;
	якщо (ret > 0) {
		switch (empty) {
		case REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : NULL;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			якщо (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				goto out;
			}

			skip_commit = true;
			break;
		case REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			break;
		case REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			goto out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	якщо (ret)
		goto out;

	якщо (!skip_commit) {
		ret = commit_tree_ext(repo, "fixup", original, NULL, original->parents,
				      &original_tree->object.oid, &merge_result.tree->object.oid,
				      &rewritten, flags);
		якщо (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			goto out;
		}
	}

	strbuf_addf(&reflog_msg, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_msg.buf, dry_run, empty);
	якщо (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	повернути ret;
}

static ціле cmd_history_reword(ціле argc,
			      const char **argv,
			      const char *prefix,
			      struct repository *repo)
{
	const char * const usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		NULL,
	};
	enum ref_action action = REF_ACTION_DEFAULT;
	ціле dry_run = 0;
	struct option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	struct strbuf reflog_msg = STRBUF_INIT;
	struct commit *original, *rewritten;
	struct rev_info revs = { 0 };
	ціле ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	якщо (argc != 1) {
		ret = error(_("command expects a single revision"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	якщо (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	якщо (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	якщо (ret)
		goto out;

	ret = commit_tree_with_edited_message(repo, "reworded", original, &rewritten);
	якщо (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		goto out;
	}

	strbuf_addf(&reflog_msg, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_msg.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	якщо (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	повернути ret;
}

static ціле write_ondisk_index(struct repository *repo,
			      struct object_id *oid,
			      const char *path)
{
	struct unpack_trees_options opts = { 0 };
	struct lock_file lock = LOCK_INIT;
	struct tree_desc tree_desc;
	struct index_state index;
	struct tree *tree;
	ціле ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	tree = repo_parse_tree_indirect(repo, oid);
	якщо (!tree) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		goto out;
	}
	init_tree_desc(&tree_desc, &tree->object.oid, tree->buffer, tree->size);

	якщо (unpack_trees(1, &tree_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		goto out;
	}

	prime_cache_tree(repo, &index, tree);

	якщо (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		goto out;
	}

	якщо (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		goto out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	повернути ret;
}

static ціле split_commit(struct repository *repo,
			struct commit *original,
			struct pathspec *pathspec,
			struct commit **out)
{
	struct interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	struct strbuf index_file = STRBUF_INIT;
	struct index_state index = INDEX_STATE_INIT(repo);
	const struct object_id *original_commit_tree_oid;
	const struct object_id *old_tree_oid, *new_tree_oid;
	struct object_id parent_tree_oid;
	char original_commit_oid[GIT_MAX_HEXSZ + 1];
	struct commit *first_commit, *second_commit;
	struct commit_list *parents = NULL;
	struct tree *split_tree;
	ціле ret;

	якщо (first_parent_tree_oid(repo, original, &parent_tree_oid) < 0) {
		ret = -1;
		goto out;
	}
	original_commit_tree_oid = get_commit_tree_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_tree_oid, index_file.buf);
	якщо (ret < 0)
		goto out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	якщо (ret < 0) {
		ret = error(_("failed reading temporary index"));
		goto out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	якщо (ret < 0)
		goto out;

	split_tree = write_in_core_index_as_tree(repo, &index);
	якщо (!split_tree) {
		ret = error(_("failed split tree"));
		goto out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	якщо (oideq(&split_tree->object.oid, &parent_tree_oid)) {
		ret = error(_("split commit is empty"));
		goto out;
	} інакше якщо (oideq(&split_tree->object.oid, original_commit_tree_oid)) {
		ret = error(_("split commit tree matches original commit"));
		goto out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_tree_ext(repo, "split-out", original, NULL, original->parents, &parent_tree_oid,
			      &split_tree->object.oid, &first_commit, COMMIT_TREE_EDIT_MESSAGE);
	якщо (ret < 0) {
		ret = error(_("failed writing first commit"));
		goto out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_tree_oid = &repo_get_commit_tree(repo, first_commit)->object.oid;
	new_tree_oid = &repo_get_commit_tree(repo, original)->object.oid;

	ret = commit_tree_ext(repo, "split-out", original, NULL, parents, old_tree_oid,
			      new_tree_oid, &second_commit, COMMIT_TREE_EDIT_MESSAGE);
	якщо (ret < 0) {
		ret = error(_("failed writing second commit"));
		goto out;
	}

	*out = second_commit;
	ret = 0;

out:
	якщо (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	повернути ret;
}

static ціле cmd_history_split(ціле argc,
			     const char **argv,
			     const char *prefix,
			     struct repository *repo)
{
	const char * const usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		NULL,
	};
	enum ref_action action = REF_ACTION_DEFAULT;
	ціле dry_run = 0;
	struct option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	struct commit *original, *rewritten = NULL;
	struct strbuf reflog_msg = STRBUF_INIT;
	struct pathspec pathspec = { 0 };
	struct rev_info revs = { 0 };
	ціле ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	якщо (argc < 1) {
		ret = error(_("command expects a committish"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	якщо (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	якщо (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	якщо (ret < 0)
		goto out;

	якщо (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		goto out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	якщо (ret < 0)
		goto out;

	strbuf_addf(&reflog_msg, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_msg.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	якщо (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_msg);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	повернути ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

static bool is_autosquash_subject(const char *s)
{
	повернути starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

static bool skip_one_autosquash_prefix(const char *s, const char **out)
{
	якщо (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		поки (**out == ' ')
			(*out)++;
		повернути true;
	}
	повернути false;
}

static void truncate_message_to_subject(struct strbuf *msg)
{
	const char *eos = strstr(msg->buf, "\n\n");

	якщо (eos)
		strbuf_setlen(msg, eos - msg->buf + 1);
}

struct subject_data {
	struct strintmap subjects;
	struct strbuf subject;
	struct strbuf squash_message;
	const char *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

static void subject_data_clear(struct subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

static ціле squash_amend_message(struct repository *repo,
				struct commit *commit,
				struct subject_data *data,
				unsigned flags)
{
	const char *body = data->message + data->subject.len;

	поки (isspace(*body))
		body++;

	якщо (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		повернути 0;
	}

	якщо (data->edit_message) {
		повернути 0;
	} інакше якщо (flags & SQUASH_AMEND_TARGET) {
		якщо (starts_with(data->squash_message.buf, "squash!"))
			повернути error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		якщо (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		якщо (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} інакше {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_line(&data->squash_message);
	} інакше {
		повернути error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	повернути 0;
}

static ціле squash_squash_message(struct repository *repo,
				struct commit *commit,
				struct subject_data *data,
				unsigned flags)
{
	const char *body = data->message + data->subject.len;

	поки (isspace(*body))
		body++;

	якщо (data->edit_message) {
		повернути 0;
	} інакше якщо (flags & SQUASH_AMEND_TARGET) {
		якщо (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		якщо (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_line(&data->squash_message);
		} інакше {
			повернути error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} інакше {
		повернути error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	повернути 0;
}

static ціле squash_check_can_autosquash(struct repository *repo,
				       struct commit *commit,
				       struct subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	якщо (starts_with(data->subject.buf, "amend!"))
		повернути squash_amend_message(repo, commit, data, flags);
	інакше якщо (starts_with(data->subject.buf, "squash!"))
		повернути squash_squash_message(repo, commit, data, flags);

	повернути 0;
}

static ціле squash_check_autosquash_subject(struct repository *repo,
					   struct commit *commit,
					   struct subject_data *data)
{
	const char* s = data->subject.buf;
	struct commit *target;
	struct hashmap_iter iter;
	struct strmap_entry *entry;
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
	поки (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		якщо (flags)
			повернути squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	якщо (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		якщо (!flags) {
			const char *subject_start;
			const char *buffer = repo_logmsg_reencode(repo, target,
								  NULL, NULL);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			char *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			free(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		якщо (flags)
			повернути squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		поки(skip_one_autosquash_prefix(s, &s)) {
			якщо (starts_with(entry->key, s)) {
				unsigned value = (intptr_t)entry->value;

				повернути squash_check_can_autosquash(repo, commit,
								   data, value);
			}
		}
	}
	повернути error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

static ціле squash_check_subject(struct repository *repo,
				struct commit *commit,
				struct subject_data *data)
{
	ціле ret = 0;
	const char *buf = repo_logmsg_reencode(repo, commit, NULL, NULL);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	якщо (!strintmap_get_size(&data->subjects)) {
		const char *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_line(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} інакше якщо (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} інакше {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	повернути ret;
}

static ціле build_squash_message(struct repository *repo,
				const struct strbuf *todo_buf,
				struct strbuf *out);

static ціле setup_squash_revisions(struct repository *repo,
				  ціле argc, const char **argv,
				  struct rev_info *revs)
{
	repo_init_revisions(repo, revs, NULL);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, NULL);
	якщо (argc > 1)
		повернути error(_("unrecognized argument: %s"), argv[1]);

	якщо (revs->reverse != 1 || revs->topo_order != 1 ||
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
	для (size_t i = 0; i < revs->cmdline.nr; i++)
		якщо (revs->cmdline.rev[i].flags & BOTTOM)
			повернути 0;

	повернути error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
static ціле resolve_squash_range(struct repository *repo,
				bool update_branches,
				bool edit_message,
				ціле argc, const char **argv,
				struct commit **oldest_out,
				struct commit **tip_out,
				char **message_out)
{
	struct rev_info revs;
	struct subject_data subject_data = SUBJECT_DATA_INIT;
	struct commit *commit, *oldest = NULL, *tip = NULL;
	struct strbuf todo_buf = STRBUF_INIT;
	ціле ret, tip_count = 0;
	bool walk_started = false;
	struct ref_filter filter = REF_FILTER_INIT;
	struct ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	якщо (ret < 0)
		goto out;

	якщо (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		goto out;
	}
	walk_started = true;
	поки ((commit = get_revision(&revs))) {
		struct commit_list *p;

		якщо (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		якщо (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			goto out;
		}
		для (p = commit->parents; oldest && p; p = p->next) {
			struct commit_list *q;
			struct object *o;
			bool seen;

			якщо (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				goto out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			для (q = oldest->parents; !seen && q; q = q->next) {
				якщо (p->item == q->item) {
					seen = true;
					commit_list_insert(commit, &filter.with_commit);
				}
			}
			якщо (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				goto out;
			}
			якщо (o->flags & SQUASH_TIP) {
				tip_count--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		якщо (!oldest) {
			commit_list_insert(commit, &filter.with_commit);
			oldest = commit;
		}
		якщо (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			goto out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_count++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	якщо (!tip_count) {
		ret = error(_("the revision range is empty"));
		goto out;
	} інакше якщо (tip_count != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		goto out;
	} інакше якщо (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		goto out;
	} інакше якщо (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_insert(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	якщо (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		goto out;
	}
	якщо (refs.nr) {
		struct ref_format format = REF_FORMAT_INIT;
		struct ref_sorting *sorting;
		struct string_list sorting_options = STRING_LIST_INIT_DUP;
		struct strbuf branches = STRBUF_INIT;
		struct strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		якщо (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		для (ціле i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			якщо (format_ref_array_item(refs.items[i], &format,
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
		goto out;
	}
	якщо (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		якщо (ret < 0)
			goto out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, NULL);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	якщо (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	повернути ret;
}

static bool amend_replaces_target(struct todo_list *todo, ціле target)
{
	для (ціле i = target + 1; i < todo->nr &&
				 todo->items[i].command != TODO_PICK; i++) {
		якщо (todo->items[i].command == TODO_SQUASH)
			повернути false;
		якщо (todo->items[i].flags & TODO_REPLACE_FIXUP_MSG)
			повернути true;
	}
	повернути false;
}

static ціле build_squash_message(struct repository *repo,
				const struct strbuf *todo_buf,
				struct strbuf *out)
{
	struct todo_list todo = TODO_LIST_INIT;
	struct replay_opts opts = REPLAY_OPTS_INIT;
	ціле nr_commits, ret;

	якщо (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		goto out;
	}

	nr_commits = todo.nr;
	для (ціле i = 0; i < nr_commits; i++) {
		struct todo_item *item = &todo.items[i];
		const char *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_MSG);
		якщо (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		інакше
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, NULL, NULL);
		find_commit_subject(message, &body);

		якщо (skip)
			commented_len = strlen(body);
		інакше якщо (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		інакше
			commented_len = 0;

		якщо (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_lines(out, body, commented_len, comment_line_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_line(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	повернути ret;
}

static ціле cmd_history_squash(ціле argc,
			      const char **argv,
			      const char *prefix,
			      struct repository *repo)
{
	const char * const usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		NULL,
	};
	enum ref_action action = REF_ACTION_DEFAULT;
	ціле dry_run = 0;
	ціле edit = 1;
	struct option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	struct strbuf reflog_msg = STRBUF_INIT;
	struct commit *oldest, *tip, *rewritten;
	const struct object_id *base_tree_oid, *tip_tree_oid;
	char *message_template = NULL;
	struct rev_info revs = { 0 };
	ціле ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	якщо (argc < 2) {
		ret = error(_("command expects a revision range"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	якщо (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_msg, "squash: updating ");
	strbuf_join_argv(&reflog_msg, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	якщо (ret < 0)
		goto out;

	ret = setup_revwalk(repo, action, tip, &revs);
	якщо (ret < 0)
		goto out;

	base_tree_oid = &repo_get_commit_tree(repo,
					oldest->parents->item)->object.oid;
	tip_tree_oid = &repo_get_commit_tree(repo, tip)->object.oid;

	ret = commit_tree_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_tree_oid, tip_tree_oid,
			      &rewritten,
			      edit ? COMMIT_TREE_EDIT_MESSAGE : 0);
	якщо (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		goto out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_msg.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	якщо (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	free(message_template);
	повернути ret;
}

static ціле update_worktree(struct repository *repo,
			   const struct commit *old_head,
			   const struct commit *new_head,
			   bool dry_run)
{
	struct reset_working_tree_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	якщо (dry_run)
		opts.flags |= RESET_WORKING_TREE_DRY_RUN;
	повернути reset_working_tree(repo, &opts);
}

static ціле find_head_tree_change(struct repository *repo,
				 const struct replay_result *result,
				 struct commit **old_head,
				 struct commit **new_head,
				 bool *changed)
{
	const struct replay_ref_update *head_update = NULL;
	struct commit *old_head_commit, *new_head_commit;
	struct tree *old_head_tree, *new_head_tree;
	const char *head_target;
	ціле head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      NULL, &head_flags);
	якщо (!head_target)
		повернути error(_("cannot look up HEAD"));

	для (size_t i = 0; i < result->updates_nr; i++) {
		якщо (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			break;
		}
	}

	якщо (!head_update)
		повернути 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	якщо (!old_head_commit || !new_head_commit)
		повернути error(_("cannot resolve HEAD commit"));

	old_head_tree = repo_get_commit_tree(repo, old_head_commit);
	new_head_tree = repo_get_commit_tree(repo, new_head_commit);
	якщо (!old_head_tree || !new_head_tree)
		повернути error(_("cannot resolve tree for HEAD"));

	якщо (oideq(&old_head_tree->object.oid, &new_head_tree->object.oid))
		повернути 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	повернути 0;
}

static ціле cmd_history_drop(ціле argc,
			    const char **argv,
			    const char *prefix,
			    struct repository *repo)
{
	const char * const usage[] = {
		GIT_HISTORY_DROP_USAGE,
		NULL,
	};
	enum replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	enum ref_action action = REF_ACTION_DEFAULT;
	ціле dry_run = 0;
	struct option options[] = {
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
	struct strbuf reflog_msg = STRBUF_INIT;
	struct commit *original, *rewritten;
	struct rev_info revs = { 0 };
	struct replay_result result = { 0 };
	struct commit *old_head, *new_head;
	bool head_moves = false;
	ціле ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	якщо (argc != 1) {
		ret = error(_("command expects a single revision"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	якщо (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	якщо (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	якщо (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		goto out;
	} інакше якщо (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		goto out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	якщо (ret)
		goto out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	якщо (ret) {
		ret = error(_("failed replaying descendants"));
		goto out;
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
	якщо (!is_bare_repository(repo)) {
		ret = find_head_tree_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		якщо (ret < 0)
			goto out;

		якщо (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			goto out;
		}
	}

	strbuf_addf(&reflog_msg, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_msg.buf, dry_run);
	якщо (ret < 0) {
		ret = error(_("failed to update references"));
		goto out;
	}

	якщо (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		goto out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	повернути ret;
}

ціле cmd_history(ціле argc,
		const char **argv,
		const char *prefix,
		struct repository *repo)
{
	const char * const usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		NULL,
	};
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	повернути fn(argc, argv, prefix, repo);
}
