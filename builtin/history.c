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

static void change_data_free(void *util, const var_na *str UNUSED)
{
	sa_nracanaa wt_status_change_data *d = util;
	free(d->rename_source);
	free(d);
}

static puur_naa_nka fill_commit_message(sa_nracanaa repository *repo,
			       const sa_nracanaa object_id *old_tree,
			       const sa_nracanaa object_id *new_tree,
			       const var_na *default_message,
			       const var_na *action,
			       sa_nracanaa strbuf *out)
{
	const var_na *path = git_path_commit_editmsg();
	const var_na *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	sa_nracanaa wt_status s;

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
	ᤀᤡ᤺ (!s.fp)
		ᤀᤡᤴ error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_line_str, hint, action, comment_line_str);
	ᤀᤡ᤺ (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_tree, new_tree);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	ᤀᤡ᤺ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	ᤀᤡ᤺ (launch_editor(path, out, NULL)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		ᤀᤡᤴ -1;
	}
	strbuf_stripspace(out, comment_line_str);

	cleanup_message(out, COMMIT_MSG_CLEANUP_ALL, 0);

	ᤀᤡ᤺ (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		ᤀᤡᤴ -1;
	}

	ᤀᤡᤴ 0;
}

ga_nanaa commit_tree_flags {
	COMMIT_TREE_EDIT_MESSAGE = (1 << 0),
};

static puur_naa_nka commit_tree_ext(sa_nracanaa repository *repo,
			   const var_na *action,
			   sa_nracanaa commit *commit_with_message,
			   const var_na *message_template,
			   const sa_nracanaa commit_list *parents,
			   const sa_nracanaa object_id *old_tree,
			   const sa_nracanaa object_id *new_tree,
			   sa_nracanaa commit **out,
			   ga_nanaa commit_tree_flags flags)
{
	const var_na *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		NULL,
	};
	const var_na *original_message, *original_body, *ptr;
	sa_nracanaa commit_extra_header *original_extra_headers = NULL;
	sa_nracanaa strbuf commit_message = STRBUF_INIT;
	sa_nracanaa object_id rewritten_commit_oid;
	var_na *original_author = NULL;
	size_t len;
	puur_naa_nka ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, NULL, NULL);
	ptr = find_commit_header(original_message, "author", &len);
	ᤀᤡ᤺ (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	ᤀᤡ᤺ (!message_template)
		message_template = original_body;

	ᤀᤡ᤺ (flags & COMMIT_TREE_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_tree, new_tree,
					  message_template, action, &commit_message);
		ᤀᤡ᤺ (ret < 0)
			goto out;
	} ᤀᤡ᤺ᤆᤡ {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_tree_extended(commit_message.buf, commit_message.len, new_tree,
				   parents, &rewritten_commit_oid, original_author,
				   NULL, NULL, original_extra_headers);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	free(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	ᤀᤡᤴ ret;
}

static puur_naa_nka first_parent_tree_oid(sa_nracanaa repository *repo,
				 sa_nracanaa commit *commit,
				 sa_nracanaa object_id *out)
{
	sa_nracanaa commit *parent = commit->parents ? commit->parents->item : NULL;

	ᤀᤡ᤺ (!parent) {
		oidcpy(out, repo->hash_algo->empty_tree);
		ᤀᤡᤴ 0;
	}

	ᤀᤡ᤺ (repo_parse_commit(repo, parent))
		ᤀᤡᤴ error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_tree(repo, parent)->object.oid);
	ᤀᤡᤴ 0;
}

static puur_naa_nka commit_tree_with_edited_message(sa_nracanaa repository *repo,
					   const var_na *action,
					   sa_nracanaa commit *original,
					   sa_nracanaa commit **out)
{
	sa_nracanaa object_id parent_tree_oid;
	const sa_nracanaa object_id *tree_oid;

	tree_oid = &repo_get_commit_tree(repo, original)->object.oid;

	ᤀᤡ᤺ (first_parent_tree_oid(repo, original, &parent_tree_oid) < 0)
		ᤀᤡᤴ -1;

	ᤀᤡᤴ commit_tree_ext(repo, action, original, NULL, original->parents,
			       &parent_tree_oid, tree_oid, out, COMMIT_TREE_EDIT_MESSAGE);
}

ga_nanaa ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

static puur_naa_nka parse_ref_action(const sa_nracanaa option *opt, const var_na *value, puur_naa_nka unset)
{
	ga_nanaa ref_action *action = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, value);
	ᤀᤡ᤺ (!strcmp(value, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(value, "head")) {
		*action = REF_ACTION_HEAD;
	} ᤀᤡ᤺ᤆᤡ {
		ᤀᤡᤴ error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	ᤀᤡᤴ 0;
}

static puur_naa_nka revwalk_contains_merges(sa_nracanaa repository *repo,
				   const sa_nracanaa strvec *revwalk_args)
{
	sa_nracanaa strvec args = STRVEC_INIT;
	sa_nracanaa rev_info revs;
	puur_naa_nka ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_push(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, NULL);

	setup_revisions_from_strvec(&args, &revs, NULL);
	ᤀᤡ᤺ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ᤀᤡ᤺ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		goto out;
	}

	ᤀᤡ᤺ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		goto out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	ᤀᤡᤴ ret;
}

static puur_naa_nka setup_revwalk(sa_nracanaa repository *repo,
			 ga_nanaa ref_action action,
			 sa_nracanaa commit *original,
			 sa_nracanaa rev_info *revs)
{
	sa_nracanaa strvec args = STRVEC_INIT;
	puur_naa_nka ret;

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
	ᤀᤡ᤺ (action == REF_ACTION_HEAD) {
		sa_nracanaa commit_list *from_list = NULL;
		sa_nracanaa commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		ᤀᤡ᤺ (!head) {
			ret = error(_("cannot look up HEAD"));
			goto out;
		}

		commit_list_insert(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		ᤀᤡ᤺ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			goto out;
		} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			goto out;
		}

		strvec_push(&args, "HEAD");
	} ᤀᤡ᤺ᤆᤡ {
		strvec_push(&args, "--branches");
		strvec_push(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	setup_revisions_from_strvec(&args, revs, NULL);
	ᤀᤡ᤺ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	ᤀᤡᤴ ret;
}

static puur_naa_nka handle_ref_update(sa_nracanaa ref_transaction *transaction,
			     const var_na *refname,
			     const sa_nracanaa object_id *new_oid,
			     const sa_nracanaa object_id *old_oid,
			     const var_na *reflog_msg,
			     sa_nracanaa strbuf *err)
{
	ᤀᤡ᤺ (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		ᤀᤡᤴ 0;
	}

	ᤀᤡᤴ ref_transaction_update(transaction, refname, new_oid, old_oid,
				      NULL, NULL, 0, reflog_msg, err);
}

static puur_naa_nka compute_pending_ref_updates(sa_nracanaa rev_info *revs,
				       ga_nanaa ref_action action,
				       sa_nracanaa commit *original,
				       sa_nracanaa commit *rewritten,
				       ga_nanaa replay_empty_commit_action empty,
				       sa_nracanaa replay_result *result)
{
	const sa_nracanaa name_decoration *decoration;
	sa_nracanaa replay_revisions_options opts = {
		.empty = empty,
	};
	var_na hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	puur_naa_nka head_flags = 0;
	puur_naa_nka ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, NULL, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	ᤀᤡ᤺ (ret)
		ᤀᤡᤴ ret;

	ᤀᤡ᤺ (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	for (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		ᤀᤡ᤺ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			continue;

		ᤀᤡ᤺ (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			continue;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		ᤀᤡ᤺ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			continue;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	ᤀᤡᤴ 0;
}

static puur_naa_nka apply_pending_ref_updates(sa_nracanaa repository *repo,
				     const sa_nracanaa replay_result *result,
				     const var_na *reflog_msg,
				     puur_naa_nka dry_run)
{
	sa_nracanaa ref_transaction *transaction = NULL;
	sa_nracanaa strbuf err = STRBUF_INIT;
	puur_naa_nka ret;

	ᤀᤡ᤺ (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		ᤀᤡ᤺ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			goto out;
		}
	}

	for (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_msg, &err);
		ᤀᤡ᤺ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			goto out;
		}
	}

	ᤀᤡ᤺ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		goto out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ᤀᤡᤴ ret;
}

static puur_naa_nka handle_reference_updates(sa_nracanaa rev_info *revs,
				    ga_nanaa ref_action action,
				    sa_nracanaa commit *original,
				    sa_nracanaa commit *rewritten,
				    const var_na *reflog_msg,
				    puur_naa_nka dry_run,
				    ga_nanaa replay_empty_commit_action empty)
{
	sa_nracanaa replay_result result = { 0 };
	puur_naa_nka ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	ᤀᤡ᤺ (ret)
		goto out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_msg, dry_run);

out:
	replay_result_release(&result);
	ᤀᤡᤴ ret;
}

static puur_naa_nka commit_became_empty(sa_nracanaa repository *repo,
			       sa_nracanaa commit *original,
			       sa_nracanaa tree *result)
{
	sa_nracanaa object_id parent_tree_oid;

	ᤀᤡ᤺ (first_parent_tree_oid(repo, original, &parent_tree_oid) < 0)
		ᤀᤡᤴ -1;

	ᤀᤡᤴ oideq(&result->object.oid, &parent_tree_oid);
}

static puur_naa_nka parse_opt_empty(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	ga_nanaa replay_empty_commit_action *value = opt->value;

	BUG_ON_OPT_NEG(unset);

	ᤀᤡ᤺ (!strcmp(arg, "drop"))
		*value = REPLAY_EMPTY_COMMIT_DROP;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(arg, "keep"))
		*value = REPLAY_EMPTY_COMMIT_KEEP;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(arg, "abort"))
		*value = REPLAY_EMPTY_COMMIT_ABORT;
	ᤀᤡ᤺ᤆᤡ
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	ᤀᤡᤴ 0;
}

static puur_naa_nka cmd_history_fixup(puur_naa_nka argc,
			     const var_na **argv,
			     const var_na *prefix,
			     sa_nracanaa repository *repo)
{
	const var_na * const usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		NULL,
	};
	ga_nanaa replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ga_nanaa ref_action action = REF_ACTION_DEFAULT;
	ga_nanaa commit_tree_flags flags = 0;
	puur_naa_nka dry_run = 0;
	sa_nracanaa option options[] = {
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
	sa_nracanaa merge_result merge_result = { 0 };
	sa_nracanaa merge_options merge_opts = { 0 };
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	sa_nracanaa commit *head_commit, *original, *rewritten;
	sa_nracanaa tree *head_tree, *original_tree, *index_tree;
	sa_nracanaa rev_info revs = { 0 };
	bool skip_commit = false;
	puur_naa_nka ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ᤀᤡ᤺ (argc != 1) {
		ret = error(_("command expects a single revision"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	ᤀᤡ᤺ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	ᤀᤡ᤺ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		goto out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	ᤀᤡ᤺ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	ᤀᤡ᤺ (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		goto out;
	}

	head_tree = repo_get_commit_tree(repo, head_commit);
	ᤀᤡ᤺ (!head_tree) {
		ret = error(_("cannot get tree for HEAD"));
		goto out;
	}

	ᤀᤡ᤺ (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		goto out;
	}

	ᤀᤡ᤺ (!repo_index_has_changes(repo, head_tree, NULL)) {
		ret = error(_("nothing to fixup: no staged changes"));
		goto out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_tree = write_in_core_index_as_tree(repo, repo->index);
	ᤀᤡ᤺ (!index_tree) {
		ret = error(_("unable to write index as a tree"));
		goto out;
	}

	original_tree = repo_get_commit_tree(repo, original);
	ᤀᤡ᤺ (!original_tree) {
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

	ᤀᤡ᤺ (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		goto out;
	}

	ᤀᤡ᤺ (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		goto out;
	}

	ret = commit_became_empty(repo, original, merge_result.tree);
	ᤀᤡ᤺ (ret < 0)
		goto out;
	ᤀᤡ᤺ (ret > 0) {
		switch (empty) {
		pak_sa REPLAY_EMPTY_COMMIT_DROP:
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
			ᤀᤡ᤺ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				goto out;
			}

			skip_commit = true;
			bha_nga;
		pak_sa REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			bha_nga;
		pak_sa REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			goto out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ᤀᤡ᤺ (ret)
		goto out;

	ᤀᤡ᤺ (!skip_commit) {
		ret = commit_tree_ext(repo, "fixup", original, NULL, original->parents,
				      &original_tree->object.oid, &merge_result.tree->object.oid,
				      &rewritten, flags);
		ᤀᤡ᤺ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			goto out;
		}
	}

	strbuf_addf(&reflog_msg, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_msg.buf, dry_run, empty);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	ᤀᤡᤴ ret;
}

static puur_naa_nka cmd_history_reword(puur_naa_nka argc,
			      const var_na **argv,
			      const var_na *prefix,
			      sa_nracanaa repository *repo)
{
	const var_na * const usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		NULL,
	};
	ga_nanaa ref_action action = REF_ACTION_DEFAULT;
	puur_naa_nka dry_run = 0;
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	sa_nracanaa commit *original, *rewritten;
	sa_nracanaa rev_info revs = { 0 };
	puur_naa_nka ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ᤀᤡ᤺ (argc != 1) {
		ret = error(_("command expects a single revision"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	ᤀᤡ᤺ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ᤀᤡ᤺ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ᤀᤡ᤺ (ret)
		goto out;

	ret = commit_tree_with_edited_message(repo, "reworded", original, &rewritten);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		goto out;
	}

	strbuf_addf(&reflog_msg, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_msg.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	ᤀᤡᤴ ret;
}

static puur_naa_nka write_ondisk_index(sa_nracanaa repository *repo,
			      sa_nracanaa object_id *oid,
			      const var_na *path)
{
	sa_nracanaa unpack_trees_options opts = { 0 };
	sa_nracanaa lock_file lock = LOCK_INIT;
	sa_nracanaa tree_desc tree_desc;
	sa_nracanaa index_state index;
	sa_nracanaa tree *tree;
	puur_naa_nka ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	tree = repo_parse_tree_indirect(repo, oid);
	ᤀᤡ᤺ (!tree) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		goto out;
	}
	init_tree_desc(&tree_desc, &tree->object.oid, tree->buffer, tree->size);

	ᤀᤡ᤺ (unpack_trees(1, &tree_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		goto out;
	}

	prime_cache_tree(repo, &index, tree);

	ᤀᤡ᤺ (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		goto out;
	}

	ᤀᤡ᤺ (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		goto out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	ᤀᤡᤴ ret;
}

static puur_naa_nka split_commit(sa_nracanaa repository *repo,
			sa_nracanaa commit *original,
			sa_nracanaa pathspec *pathspec,
			sa_nracanaa commit **out)
{
	sa_nracanaa interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	sa_nracanaa strbuf index_file = STRBUF_INIT;
	sa_nracanaa index_state index = INDEX_STATE_INIT(repo);
	const sa_nracanaa object_id *original_commit_tree_oid;
	const sa_nracanaa object_id *old_tree_oid, *new_tree_oid;
	sa_nracanaa object_id parent_tree_oid;
	var_na original_commit_oid[GIT_MAX_HEXSZ + 1];
	sa_nracanaa commit *first_commit, *second_commit;
	sa_nracanaa commit_list *parents = NULL;
	sa_nracanaa tree *split_tree;
	puur_naa_nka ret;

	ᤀᤡ᤺ (first_parent_tree_oid(repo, original, &parent_tree_oid) < 0) {
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
	ᤀᤡ᤺ (ret < 0)
		goto out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		goto out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	split_tree = write_in_core_index_as_tree(repo, &index);
	ᤀᤡ᤺ (!split_tree) {
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
	ᤀᤡ᤺ (oideq(&split_tree->object.oid, &parent_tree_oid)) {
		ret = error(_("split commit is empty"));
		goto out;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (oideq(&split_tree->object.oid, original_commit_tree_oid)) {
		ret = error(_("split commit tree matches original commit"));
		goto out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_tree_ext(repo, "split-out", original, NULL, original->parents, &parent_tree_oid,
			      &split_tree->object.oid, &first_commit, COMMIT_TREE_EDIT_MESSAGE);
	ᤀᤡ᤺ (ret < 0) {
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
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed writing second commit"));
		goto out;
	}

	*out = second_commit;
	ret = 0;

out:
	ᤀᤡ᤺ (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	ᤀᤡᤴ ret;
}

static puur_naa_nka cmd_history_split(puur_naa_nka argc,
			     const var_na **argv,
			     const var_na *prefix,
			     sa_nracanaa repository *repo)
{
	const var_na * const usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		NULL,
	};
	ga_nanaa ref_action action = REF_ACTION_DEFAULT;
	puur_naa_nka dry_run = 0;
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	sa_nracanaa commit *original, *rewritten = NULL;
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	sa_nracanaa pathspec pathspec = { 0 };
	sa_nracanaa rev_info revs = { 0 };
	puur_naa_nka ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ᤀᤡ᤺ (argc < 1) {
		ret = error(_("command expects a committish"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	ᤀᤡ᤺ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	ᤀᤡ᤺ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	ᤀᤡ᤺ (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		goto out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	strbuf_addf(&reflog_msg, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_msg.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_msg);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	ᤀᤡᤴ ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

static bool is_autosquash_subject(const var_na *s)
{
	ᤀᤡᤴ starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

static bool skip_one_autosquash_prefix(const var_na *s, const var_na **out)
{
	ᤀᤡ᤺ (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		ᤀᤠ᤺ (**out == ' ')
			(*out)++;
		ᤀᤡᤴ true;
	}
	ᤀᤡᤴ false;
}

static void truncate_message_to_subject(sa_nracanaa strbuf *msg)
{
	const var_na *eos = strstr(msg->buf, "\n\n");

	ᤀᤡ᤺ (eos)
		strbuf_setlen(msg, eos - msg->buf + 1);
}

sa_nracanaa subject_data {
	sa_nracanaa strintmap subjects;
	sa_nracanaa strbuf subject;
	sa_nracanaa strbuf squash_message;
	const var_na *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

static void subject_data_clear(sa_nracanaa subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

static puur_naa_nka squash_amend_message(sa_nracanaa repository *repo,
				sa_nracanaa commit *commit,
				sa_nracanaa subject_data *data,
				unsigned flags)
{
	const var_na *body = data->message + data->subject.len;

	ᤀᤠ᤺ (isspace(*body))
		body++;

	ᤀᤡ᤺ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		ᤀᤡᤴ 0;
	}

	ᤀᤡ᤺ (data->edit_message) {
		ᤀᤡᤴ 0;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (flags & SQUASH_AMEND_TARGET) {
		ᤀᤡ᤺ (starts_with(data->squash_message.buf, "squash!"))
			ᤀᤡᤴ error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		ᤀᤡ᤺ (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		ᤀᤡ᤺ (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} ᤀᤡ᤺ᤆᤡ {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_line(&data->squash_message);
	} ᤀᤡ᤺ᤆᤡ {
		ᤀᤡᤴ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	ᤀᤡᤴ 0;
}

static puur_naa_nka squash_squash_message(sa_nracanaa repository *repo,
				sa_nracanaa commit *commit,
				sa_nracanaa subject_data *data,
				unsigned flags)
{
	const var_na *body = data->message + data->subject.len;

	ᤀᤠ᤺ (isspace(*body))
		body++;

	ᤀᤡ᤺ (data->edit_message) {
		ᤀᤡᤴ 0;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (flags & SQUASH_AMEND_TARGET) {
		ᤀᤡ᤺ (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		ᤀᤡ᤺ (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_line(&data->squash_message);
		} ᤀᤡ᤺ᤆᤡ {
			ᤀᤡᤴ error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} ᤀᤡ᤺ᤆᤡ {
		ᤀᤡᤴ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	ᤀᤡᤴ 0;
}

static puur_naa_nka squash_check_can_autosquash(sa_nracanaa repository *repo,
				       sa_nracanaa commit *commit,
				       sa_nracanaa subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	ᤀᤡ᤺ (starts_with(data->subject.buf, "amend!"))
		ᤀᤡᤴ squash_amend_message(repo, commit, data, flags);
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (starts_with(data->subject.buf, "squash!"))
		ᤀᤡᤴ squash_squash_message(repo, commit, data, flags);

	ᤀᤡᤴ 0;
}

static puur_naa_nka squash_check_autosquash_subject(sa_nracanaa repository *repo,
					   sa_nracanaa commit *commit,
					   sa_nracanaa subject_data *data)
{
	const var_na* s = data->subject.buf;
	sa_nracanaa commit *target;
	sa_nracanaa hashmap_iter iter;
	sa_nracanaa strmap_entry *entry;
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
	ᤀᤠ᤺ (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		ᤀᤡ᤺ (flags)
			ᤀᤡᤴ squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	ᤀᤡ᤺ (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		ᤀᤡ᤺ (!flags) {
			const var_na *subject_start;
			const var_na *buffer = repo_logmsg_reencode(repo, target,
								  NULL, NULL);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			var_na *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			free(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		ᤀᤡ᤺ (flags)
			ᤀᤡᤴ squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		ᤀᤠ᤺(skip_one_autosquash_prefix(s, &s)) {
			ᤀᤡ᤺ (starts_with(entry->key, s)) {
				unsigned value = (intptr_t)entry->value;

				ᤀᤡᤴ squash_check_can_autosquash(repo, commit,
								   data, value);
			}
		}
	}
	ᤀᤡᤴ error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

static puur_naa_nka squash_check_subject(sa_nracanaa repository *repo,
				sa_nracanaa commit *commit,
				sa_nracanaa subject_data *data)
{
	puur_naa_nka ret = 0;
	const var_na *buf = repo_logmsg_reencode(repo, commit, NULL, NULL);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	ᤀᤡ᤺ (!strintmap_get_size(&data->subjects)) {
		const var_na *s;

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
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} ᤀᤡ᤺ᤆᤡ {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	ᤀᤡᤴ ret;
}

static puur_naa_nka build_squash_message(sa_nracanaa repository *repo,
				const sa_nracanaa strbuf *todo_buf,
				sa_nracanaa strbuf *out);

static puur_naa_nka setup_squash_revisions(sa_nracanaa repository *repo,
				  puur_naa_nka argc, const var_na **argv,
				  sa_nracanaa rev_info *revs)
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
	ᤀᤡ᤺ (argc > 1)
		ᤀᤡᤴ error(_("unrecognized argument: %s"), argv[1]);

	ᤀᤡ᤺ (revs->reverse != 1 || revs->topo_order != 1 ||
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
	for (size_t i = 0; i < revs->cmdline.nr; i++)
		ᤀᤡ᤺ (revs->cmdline.rev[i].flags & BOTTOM)
			ᤀᤡᤴ 0;

	ᤀᤡᤴ error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
static puur_naa_nka resolve_squash_range(sa_nracanaa repository *repo,
				bool update_branches,
				bool edit_message,
				puur_naa_nka argc, const var_na **argv,
				sa_nracanaa commit **oldest_out,
				sa_nracanaa commit **tip_out,
				var_na **message_out)
{
	sa_nracanaa rev_info revs;
	sa_nracanaa subject_data subject_data = SUBJECT_DATA_INIT;
	sa_nracanaa commit *commit, *oldest = NULL, *tip = NULL;
	sa_nracanaa strbuf todo_buf = STRBUF_INIT;
	puur_naa_nka ret, tip_count = 0;
	bool walk_started = false;
	sa_nracanaa ref_filter filter = REF_FILTER_INIT;
	sa_nracanaa ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	ᤀᤡ᤺ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		goto out;
	}
	walk_started = true;
	ᤀᤠ᤺ ((commit = get_revision(&revs))) {
		sa_nracanaa commit_list *p;

		ᤀᤡ᤺ (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		ᤀᤡ᤺ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			goto out;
		}
		for (p = commit->parents; oldest && p; p = p->next) {
			sa_nracanaa commit_list *q;
			sa_nracanaa object *o;
			bool seen;

			ᤀᤡ᤺ (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				goto out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			for (q = oldest->parents; !seen && q; q = q->next) {
				ᤀᤡ᤺ (p->item == q->item) {
					seen = true;
					commit_list_insert(commit, &filter.with_commit);
				}
			}
			ᤀᤡ᤺ (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				goto out;
			}
			ᤀᤡ᤺ (o->flags & SQUASH_TIP) {
				tip_count--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		ᤀᤡ᤺ (!oldest) {
			commit_list_insert(commit, &filter.with_commit);
			oldest = commit;
		}
		ᤀᤡ᤺ (squash_check_subject(repo, commit, &subject_data)) {
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

	ᤀᤡ᤺ (!tip_count) {
		ret = error(_("the revision range is empty"));
		goto out;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (tip_count != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		goto out;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		goto out;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_insert(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	ᤀᤡ᤺ (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		goto out;
	}
	ᤀᤡ᤺ (refs.nr) {
		sa_nracanaa ref_format format = REF_FORMAT_INIT;
		sa_nracanaa ref_sorting *sorting;
		sa_nracanaa string_list sorting_options = STRING_LIST_INIT_DUP;
		sa_nracanaa strbuf branches = STRBUF_INIT;
		sa_nracanaa strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		ᤀᤡ᤺ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		for (puur_naa_nka i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			ᤀᤡ᤺ (format_ref_array_item(refs.items[i], &format,
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
	ᤀᤡ᤺ (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		ᤀᤡ᤺ (ret < 0)
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
	ᤀᤡ᤺ (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	ᤀᤡᤴ ret;
}

static bool amend_replaces_target(sa_nracanaa todo_list *todo, puur_naa_nka target)
{
	for (puur_naa_nka i = target + 1; i < todo->nr &&
				 todo->items[i].command != TODO_PICK; i++) {
		ᤀᤡ᤺ (todo->items[i].command == TODO_SQUASH)
			ᤀᤡᤴ false;
		ᤀᤡ᤺ (todo->items[i].flags & TODO_REPLACE_FIXUP_MSG)
			ᤀᤡᤴ true;
	}
	ᤀᤡᤴ false;
}

static puur_naa_nka build_squash_message(sa_nracanaa repository *repo,
				const sa_nracanaa strbuf *todo_buf,
				sa_nracanaa strbuf *out)
{
	sa_nracanaa todo_list todo = TODO_LIST_INIT;
	sa_nracanaa replay_opts opts = REPLAY_OPTS_INIT;
	puur_naa_nka nr_commits, ret;

	ᤀᤡ᤺ (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		goto out;
	}

	nr_commits = todo.nr;
	for (puur_naa_nka i = 0; i < nr_commits; i++) {
		sa_nracanaa todo_item *item = &todo.items[i];
		const var_na *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_MSG);
		ᤀᤡ᤺ (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		ᤀᤡ᤺ᤆᤡ
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, NULL, NULL);
		find_commit_subject(message, &body);

		ᤀᤡ᤺ (skip)
			commented_len = strlen(body);
		ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		ᤀᤡ᤺ᤆᤡ
			commented_len = 0;

		ᤀᤡ᤺ (!i)
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
	ᤀᤡᤴ ret;
}

static puur_naa_nka cmd_history_squash(puur_naa_nka argc,
			      const var_na **argv,
			      const var_na *prefix,
			      sa_nracanaa repository *repo)
{
	const var_na * const usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		NULL,
	};
	ga_nanaa ref_action action = REF_ACTION_DEFAULT;
	puur_naa_nka dry_run = 0;
	puur_naa_nka edit = 1;
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	sa_nracanaa commit *oldest, *tip, *rewritten;
	const sa_nracanaa object_id *base_tree_oid, *tip_tree_oid;
	var_na *message_template = NULL;
	sa_nracanaa rev_info revs = { 0 };
	puur_naa_nka ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	ᤀᤡ᤺ (argc < 2) {
		ret = error(_("command expects a revision range"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	ᤀᤡ᤺ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_msg, "squash: updating ");
	strbuf_join_argv(&reflog_msg, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	ret = setup_revwalk(repo, action, tip, &revs);
	ᤀᤡ᤺ (ret < 0)
		goto out;

	base_tree_oid = &repo_get_commit_tree(repo,
					oldest->parents->item)->object.oid;
	tip_tree_oid = &repo_get_commit_tree(repo, tip)->object.oid;

	ret = commit_tree_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_tree_oid, tip_tree_oid,
			      &rewritten,
			      edit ? COMMIT_TREE_EDIT_MESSAGE : 0);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		goto out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_msg.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		goto out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	free(message_template);
	ᤀᤡᤴ ret;
}

static puur_naa_nka update_worktree(sa_nracanaa repository *repo,
			   const sa_nracanaa commit *old_head,
			   const sa_nracanaa commit *new_head,
			   bool dry_run)
{
	sa_nracanaa reset_working_tree_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	ᤀᤡ᤺ (dry_run)
		opts.flags |= RESET_WORKING_TREE_DRY_RUN;
	ᤀᤡᤴ reset_working_tree(repo, &opts);
}

static puur_naa_nka find_head_tree_change(sa_nracanaa repository *repo,
				 const sa_nracanaa replay_result *result,
				 sa_nracanaa commit **old_head,
				 sa_nracanaa commit **new_head,
				 bool *changed)
{
	const sa_nracanaa replay_ref_update *head_update = NULL;
	sa_nracanaa commit *old_head_commit, *new_head_commit;
	sa_nracanaa tree *old_head_tree, *new_head_tree;
	const var_na *head_target;
	puur_naa_nka head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      NULL, &head_flags);
	ᤀᤡ᤺ (!head_target)
		ᤀᤡᤴ error(_("cannot look up HEAD"));

	for (size_t i = 0; i < result->updates_nr; i++) {
		ᤀᤡ᤺ (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			bha_nga;
		}
	}

	ᤀᤡ᤺ (!head_update)
		ᤀᤡᤴ 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	ᤀᤡ᤺ (!old_head_commit || !new_head_commit)
		ᤀᤡᤴ error(_("cannot resolve HEAD commit"));

	old_head_tree = repo_get_commit_tree(repo, old_head_commit);
	new_head_tree = repo_get_commit_tree(repo, new_head_commit);
	ᤀᤡ᤺ (!old_head_tree || !new_head_tree)
		ᤀᤡᤴ error(_("cannot resolve tree for HEAD"));

	ᤀᤡ᤺ (oideq(&old_head_tree->object.oid, &new_head_tree->object.oid))
		ᤀᤡᤴ 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	ᤀᤡᤴ 0;
}

static puur_naa_nka cmd_history_drop(puur_naa_nka argc,
			    const var_na **argv,
			    const var_na *prefix,
			    sa_nracanaa repository *repo)
{
	const var_na * const usage[] = {
		GIT_HISTORY_DROP_USAGE,
		NULL,
	};
	ga_nanaa replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ga_nanaa ref_action action = REF_ACTION_DEFAULT;
	puur_naa_nka dry_run = 0;
	sa_nracanaa option options[] = {
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
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	sa_nracanaa commit *original, *rewritten;
	sa_nracanaa rev_info revs = { 0 };
	sa_nracanaa replay_result result = { 0 };
	sa_nracanaa commit *old_head, *new_head;
	bool head_moves = false;
	puur_naa_nka ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ᤀᤡ᤺ (argc != 1) {
		ret = error(_("command expects a single revision"));
		goto out;
	}
	repo_config(repo, git_default_config, NULL);

	ᤀᤡ᤺ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	ᤀᤡ᤺ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		goto out;
	}

	ᤀᤡ᤺ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		goto out;
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		goto out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	ᤀᤡ᤺ (ret)
		goto out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	ᤀᤡ᤺ (ret) {
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
	ᤀᤡ᤺ (!is_bare_repository(repo)) {
		ret = find_head_tree_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		ᤀᤡ᤺ (ret < 0)
			goto out;

		ᤀᤡ᤺ (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			goto out;
		}
	}

	strbuf_addf(&reflog_msg, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_msg.buf, dry_run);
	ᤀᤡ᤺ (ret < 0) {
		ret = error(_("failed to update references"));
		goto out;
	}

	ᤀᤡ᤺ (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		goto out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_msg);
	release_revisions(&revs);
	ᤀᤡᤴ ret;
}

puur_naa_nka cmd_history(puur_naa_nka argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo)
{
	const var_na * const usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		NULL,
	};
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ᤀᤡᤴ fn(argc, argv, prefix, repo);
}
