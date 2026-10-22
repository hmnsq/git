#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "merge-ort.h"
#અન્તર્ભાવ "oidmap.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "ref-filter.h"
#અન્તર્ભાવ "replay.h"
#અન્તર્ભાવ "reset.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "wt-status.h"

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

સ્થિર શૂન્ય change_data_free(શૂન્ય *util, અચલ અક્ષર *str UNUSED)
{
	સંરચના wt_status_change_data *d = util;
	મુક્ત(d->rename_source);
	મુક્ત(d);
}

સ્થિર પૂર્ણાંક fill_commit_message(સંરચના repository *repo,
			       અચલ સંરચના object_id *old_વૃક્ષ,
			       અચલ સંરચના object_id *new_વૃક્ષ,
			       અચલ અક્ષર *default_message,
			       અચલ અક્ષર *action,
			       સંરચના strbuf *out)
{
	અચલ અક્ષર *path = git_path_commit_editmsg();
	અચલ અક્ષર *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	સંરચના wt_status s;

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
	જો (!s.fp)
		પાછા error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_લીટી_str, hint, action, comment_લીટી_str);
	જો (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_વૃક્ષ, new_વૃક્ષ);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	જો (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	જો (launch_editor(path, out, ખાલી)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		પાછા -1;
	}
	strbuf_stripspace(out, comment_લીટી_str);

	cleanup_message(out, COMMIT_સંદેશ_CLEANUP_ALL, 0);

	જો (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		પાછા -1;
	}

	પાછા 0;
}

ગણના commit_વૃક્ષ_flags {
	COMMIT_વૃક્ષ_EDIT_MESSAGE = (1 << 0),
};

સ્થિર પૂર્ણાંક commit_વૃક્ષ_ext(સંરચના repository *repo,
			   અચલ અક્ષર *action,
			   સંરચના commit *commit_with_message,
			   અચલ અક્ષર *message_template,
			   અચલ સંરચના commit_list *parents,
			   અચલ સંરચના object_id *old_વૃક્ષ,
			   અચલ સંરચના object_id *new_વૃક્ષ,
			   સંરચના commit **out,
			   ગણના commit_વૃક્ષ_flags flags)
{
	અચલ અક્ષર *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ખાલી,
	};
	અચલ અક્ષર *original_message, *original_body, *ptr;
	સંરચના commit_extra_header *original_extra_headers = ખાલી;
	સંરચના strbuf commit_message = STRBUF_INIT;
	સંરચના object_id rewritten_commit_oid;
	અક્ષર *original_author = ખાલી;
	size_t len;
	પૂર્ણાંક ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, ખાલી, ખાલી);
	ptr = find_commit_header(original_message, "author", &len);
	જો (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	જો (!message_template)
		message_template = original_body;

	જો (flags & COMMIT_વૃક્ષ_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_વૃક્ષ, new_વૃક્ષ,
					  message_template, action, &commit_message);
		જો (ret < 0)
			જાઓ out;
	} નહીંતર {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_વૃક્ષ_extended(commit_message.buf, commit_message.len, new_વૃક્ષ,
				   parents, &rewritten_commit_oid, original_author,
				   ખાલી, ખાલી, original_extra_headers);
	જો (ret < 0)
		જાઓ out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	મુક્ત(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	પાછા ret;
}

સ્થિર પૂર્ણાંક first_parent_વૃક્ષ_oid(સંરચના repository *repo,
				 સંરચના commit *commit,
				 સંરચના object_id *out)
{
	સંરચના commit *parent = commit->parents ? commit->parents->item : ખાલી;

	જો (!parent) {
		oidcpy(out, repo->hash_algo->empty_વૃક્ષ);
		પાછા 0;
	}

	જો (repo_parse_commit(repo, parent))
		પાછા error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_વૃક્ષ(repo, parent)->object.oid);
	પાછા 0;
}

સ્થિર પૂર્ણાંક commit_વૃક્ષ_with_edited_message(સંરચના repository *repo,
					   અચલ અક્ષર *action,
					   સંરચના commit *original,
					   સંરચના commit **out)
{
	સંરચના object_id parent_વૃક્ષ_oid;
	અચલ સંરચના object_id *વૃક્ષ_oid;

	વૃક્ષ_oid = &repo_get_commit_વૃક્ષ(repo, original)->object.oid;

	જો (first_parent_વૃક્ષ_oid(repo, original, &parent_વૃક્ષ_oid) < 0)
		પાછા -1;

	પાછા commit_વૃક્ષ_ext(repo, action, original, ખાલી, original->parents,
			       &parent_વૃક્ષ_oid, વૃક્ષ_oid, out, COMMIT_વૃક્ષ_EDIT_MESSAGE);
}

ગણના ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

સ્થિર પૂર્ણાંક parse_ref_action(અચલ સંરચના option *opt, અચલ અક્ષર *કિંમત, પૂર્ણાંક unset)
{
	ગણના ref_action *action = opt->કિંમત;

	BUG_ON_OPT_NEG_NOARG(unset, કિંમત);
	જો (!strcmp(કિંમત, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} નહીંતર જો (!strcmp(કિંમત, "head")) {
		*action = REF_ACTION_HEAD;
	} નહીંતર {
		પાછા error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક revwalk_contains_merges(સંરચના repository *repo,
				   અચલ સંરચના strvec *revwalk_args)
{
	સંરચના strvec args = STRVEC_INIT;
	સંરચના rev_info revs;
	પૂર્ણાંક ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ધકેલો(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, ખાલી);

	setup_revisions_from_strvec(&args, &revs, ખાલી);
	જો (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	જો (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		જાઓ out;
	}

	જો (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		જાઓ out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	પાછા ret;
}

સ્થિર પૂર્ણાંક setup_revwalk(સંરચના repository *repo,
			 ગણના ref_action action,
			 સંરચના commit *original,
			 સંરચના rev_info *revs)
{
	સંરચના strvec args = STRVEC_INIT;
	પૂર્ણાંક ret;

	repo_init_revisions(repo, revs, ખાલી);
	strvec_ધકેલો(&args, "ignored");
	strvec_ધકેલો(&args, "--reverse");
	strvec_ધકેલો(&args, "--topo-order");
	strvec_ધકેલો(&args, "--full-history");

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
	જો (action == REF_ACTION_HEAD) {
		સંરચના commit_list *from_list = ખાલી;
		સંરચના commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		જો (!head) {
			ret = error(_("cannot look up HEAD"));
			જાઓ out;
		}

		commit_list_ઉમેરો(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		જો (ret < 0) {
			ret = error(_("cannot determine descendance"));
			જાઓ out;
		} નહીંતર જો (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			જાઓ out;
		}

		strvec_ધકેલો(&args, "HEAD");
	} નહીંતર {
		strvec_ધકેલો(&args, "--branches");
		strvec_ધકેલો(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	જો (ret < 0)
		જાઓ out;

	setup_revisions_from_strvec(&args, revs, ખાલી);
	જો (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	પાછા ret;
}

સ્થિર પૂર્ણાંક handle_ref_update(સંરચના ref_transaction *transaction,
			     અચલ અક્ષર *refname,
			     અચલ સંરચના object_id *new_oid,
			     અચલ સંરચના object_id *old_oid,
			     અચલ અક્ષર *reflog_સંદેશ,
			     સંરચના strbuf *err)
{
	જો (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		પાછા 0;
	}

	પાછા ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ખાલી, ખાલી, 0, reflog_સંદેશ, err);
}

સ્થિર પૂર્ણાંક compute_pending_ref_updates(સંરચના rev_info *revs,
				       ગણના ref_action action,
				       સંરચના commit *original,
				       સંરચના commit *rewritten,
				       ગણના replay_empty_commit_action empty,
				       સંરચના replay_result *result)
{
	અચલ સંરચના name_decoration *decoration;
	સંરચના replay_revisions_options opts = {
		.empty = empty,
	};
	અક્ષર hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	પૂર્ણાંક head_flags = 0;
	પૂર્ણાંક ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ખાલી, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	જો (ret)
		પાછા ret;

	જો (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	માટે (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		જો (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			ચાલુ;

		જો (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			ચાલુ;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		જો (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			ચાલુ;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક apply_pending_ref_updates(સંરચના repository *repo,
				     અચલ સંરચના replay_result *result,
				     અચલ અક્ષર *reflog_સંદેશ,
				     પૂર્ણાંક dry_run)
{
	સંરચના ref_transaction *transaction = ખાલી;
	સંરચના strbuf err = STRBUF_INIT;
	પૂર્ણાંક ret;

	જો (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		જો (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			જાઓ out;
		}
	}

	માટે (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_સંદેશ, &err);
		જો (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			જાઓ out;
		}
	}

	જો (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		જાઓ out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	પાછા ret;
}

સ્થિર પૂર્ણાંક handle_reference_updates(સંરચના rev_info *revs,
				    ગણના ref_action action,
				    સંરચના commit *original,
				    સંરચના commit *rewritten,
				    અચલ અક્ષર *reflog_સંદેશ,
				    પૂર્ણાંક dry_run,
				    ગણના replay_empty_commit_action empty)
{
	સંરચના replay_result result = { 0 };
	પૂર્ણાંક ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	જો (ret)
		જાઓ out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_સંદેશ, dry_run);

out:
	replay_result_release(&result);
	પાછા ret;
}

સ્થિર પૂર્ણાંક commit_became_empty(સંરચના repository *repo,
			       સંરચના commit *original,
			       સંરચના વૃક્ષ *result)
{
	સંરચના object_id parent_વૃક્ષ_oid;

	જો (first_parent_વૃક્ષ_oid(repo, original, &parent_વૃક્ષ_oid) < 0)
		પાછા -1;

	પાછા oideq(&result->object.oid, &parent_વૃક્ષ_oid);
}

સ્થિર પૂર્ણાંક parse_opt_empty(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	ગણના replay_empty_commit_action *કિંમત = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!strcmp(arg, "drop"))
		*કિંમત = REPLAY_EMPTY_COMMIT_DROP;
	નહીંતર જો (!strcmp(arg, "keep"))
		*કિંમત = REPLAY_EMPTY_COMMIT_KEEP;
	નહીંતર જો (!strcmp(arg, "abort"))
		*કિંમત = REPLAY_EMPTY_COMMIT_ABORT;
	નહીંતર
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_history_fixup(પૂર્ણાંક argc,
			     અચલ અક્ષર **argv,
			     અચલ અક્ષર *prefix,
			     સંરચના repository *repo)
{
	અચલ અક્ષર * અચલ usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ખાલી,
	};
	ગણના replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ગણના ref_action action = REF_ACTION_DEFAULT;
	ગણના commit_વૃક્ષ_flags flags = 0;
	પૂર્ણાંક dry_run = 0;
	સંરચના option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_વૃક્ષ_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	સંરચના merge_result merge_result = { 0 };
	સંરચના merge_options merge_opts = { 0 };
	સંરચના strbuf reflog_સંદેશ = STRBUF_INIT;
	સંરચના commit *head_commit, *original, *rewritten;
	સંરચના વૃક્ષ *head_વૃક્ષ, *original_વૃક્ષ, *index_વૃક્ષ;
	સંરચના rev_info revs = { 0 };
	bool skip_commit = false;
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	જો (argc != 1) {
		ret = error(_("command expects a single revision"));
		જાઓ out;
	}
	repo_config(repo, git_default_config, ખાલી);

	જો (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	જો (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		જાઓ out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	જો (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		જાઓ out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	જો (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		જાઓ out;
	}

	head_વૃક્ષ = repo_get_commit_વૃક્ષ(repo, head_commit);
	જો (!head_વૃક્ષ) {
		ret = error(_("cannot get tree for HEAD"));
		જાઓ out;
	}

	જો (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		જાઓ out;
	}

	જો (!repo_index_has_changes(repo, head_વૃક્ષ, ખાલી)) {
		ret = error(_("nothing to fixup: no staged changes"));
		જાઓ out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_વૃક્ષ = write_in_core_index_as_વૃક્ષ(repo, repo->index);
	જો (!index_વૃક્ષ) {
		ret = error(_("unable to write index as a tree"));
		જાઓ out;
	}

	original_વૃક્ષ = repo_get_commit_વૃક્ષ(repo, original);
	જો (!original_વૃક્ષ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		જાઓ out;
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
	merge_incore_nonrecursive(&merge_opts, head_વૃક્ષ,
				  original_વૃક્ષ, index_વૃક્ષ, &merge_result);

	જો (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		જાઓ out;
	}

	જો (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		જાઓ out;
	}

	ret = commit_became_empty(repo, original, merge_result.વૃક્ષ);
	જો (ret < 0)
		જાઓ out;
	જો (ret > 0) {
		પસંદગી (empty) {
		સ્થિતિ REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : ખાલી;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			જો (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				જાઓ out;
			}

			skip_commit = true;
			તોડો;
		સ્થિતિ REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			તોડો;
		સ્થિતિ REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			જાઓ out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	જો (ret)
		જાઓ out;

	જો (!skip_commit) {
		ret = commit_વૃક્ષ_ext(repo, "fixup", original, ખાલી, original->parents,
				      &original_વૃક્ષ->object.oid, &merge_result.વૃક્ષ->object.oid,
				      &rewritten, flags);
		જો (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			જાઓ out;
		}
	}

	strbuf_addf(&reflog_સંદેશ, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_સંદેશ.buf, dry_run, empty);
	જો (ret < 0) {
		ret = error(_("failed replaying descendants"));
		જાઓ out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_સંદેશ);
	release_revisions(&revs);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_history_reword(પૂર્ણાંક argc,
			      અચલ અક્ષર **argv,
			      અચલ અક્ષર *prefix,
			      સંરચના repository *repo)
{
	અચલ અક્ષર * અચલ usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ખાલી,
	};
	ગણના ref_action action = REF_ACTION_DEFAULT;
	પૂર્ણાંક dry_run = 0;
	સંરચના option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	સંરચના strbuf reflog_સંદેશ = STRBUF_INIT;
	સંરચના commit *original, *rewritten;
	સંરચના rev_info revs = { 0 };
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	જો (argc != 1) {
		ret = error(_("command expects a single revision"));
		જાઓ out;
	}
	repo_config(repo, git_default_config, ખાલી);

	જો (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	જો (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		જાઓ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	જો (ret)
		જાઓ out;

	ret = commit_વૃક્ષ_with_edited_message(repo, "reworded", original, &rewritten);
	જો (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		જાઓ out;
	}

	strbuf_addf(&reflog_સંદેશ, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_સંદેશ.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	જો (ret < 0) {
		ret = error(_("failed replaying descendants"));
		જાઓ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_સંદેશ);
	release_revisions(&revs);
	પાછા ret;
}

સ્થિર પૂર્ણાંક write_ondisk_index(સંરચના repository *repo,
			      સંરચના object_id *oid,
			      અચલ અક્ષર *path)
{
	સંરચના unpack_trees_options opts = { 0 };
	સંરચના lock_file lock = LOCK_INIT;
	સંરચના વૃક્ષ_desc વૃક્ષ_desc;
	સંરચના index_state index;
	સંરચના વૃક્ષ *વૃક્ષ;
	પૂર્ણાંક ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	વૃક્ષ = repo_parse_વૃક્ષ_indirect(repo, oid);
	જો (!વૃક્ષ) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		જાઓ out;
	}
	init_વૃક્ષ_desc(&વૃક્ષ_desc, &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);

	જો (unpack_trees(1, &વૃક્ષ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		જાઓ out;
	}

	અવિભાજ્ય_cache_વૃક્ષ(repo, &index, વૃક્ષ);

	જો (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		જાઓ out;
	}

	જો (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		જાઓ out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	પાછા ret;
}

સ્થિર પૂર્ણાંક split_commit(સંરચના repository *repo,
			સંરચના commit *original,
			સંરચના pathspec *pathspec,
			સંરચના commit **out)
{
	સંરચના interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	સંરચના strbuf index_file = STRBUF_INIT;
	સંરચના index_state index = INDEX_STATE_INIT(repo);
	અચલ સંરચના object_id *original_commit_વૃક્ષ_oid;
	અચલ સંરચના object_id *old_વૃક્ષ_oid, *new_વૃક્ષ_oid;
	સંરચના object_id parent_વૃક્ષ_oid;
	અક્ષર original_commit_oid[GIT_MAX_HEXSZ + 1];
	સંરચના commit *first_commit, *second_commit;
	સંરચના commit_list *parents = ખાલી;
	સંરચના વૃક્ષ *split_વૃક્ષ;
	પૂર્ણાંક ret;

	જો (first_parent_વૃક્ષ_oid(repo, original, &parent_વૃક્ષ_oid) < 0) {
		ret = -1;
		જાઓ out;
	}
	original_commit_વૃક્ષ_oid = get_commit_વૃક્ષ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_વૃક્ષ_oid, index_file.buf);
	જો (ret < 0)
		જાઓ out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	જો (ret < 0) {
		ret = error(_("failed reading temporary index"));
		જાઓ out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	જો (ret < 0)
		જાઓ out;

	split_વૃક્ષ = write_in_core_index_as_વૃક્ષ(repo, &index);
	જો (!split_વૃક્ષ) {
		ret = error(_("failed split tree"));
		જાઓ out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	જો (oideq(&split_વૃક્ષ->object.oid, &parent_વૃક્ષ_oid)) {
		ret = error(_("split commit is empty"));
		જાઓ out;
	} નહીંતર જો (oideq(&split_વૃક્ષ->object.oid, original_commit_વૃક્ષ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		જાઓ out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_વૃક્ષ_ext(repo, "split-out", original, ખાલી, original->parents, &parent_વૃક્ષ_oid,
			      &split_વૃક્ષ->object.oid, &first_commit, COMMIT_વૃક્ષ_EDIT_MESSAGE);
	જો (ret < 0) {
		ret = error(_("failed writing first commit"));
		જાઓ out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_વૃક્ષ_oid = &repo_get_commit_વૃક્ષ(repo, first_commit)->object.oid;
	new_વૃક્ષ_oid = &repo_get_commit_વૃક્ષ(repo, original)->object.oid;

	ret = commit_વૃક્ષ_ext(repo, "split-out", original, ખાલી, parents, old_વૃક્ષ_oid,
			      new_વૃક્ષ_oid, &second_commit, COMMIT_વૃક્ષ_EDIT_MESSAGE);
	જો (ret < 0) {
		ret = error(_("failed writing second commit"));
		જાઓ out;
	}

	*out = second_commit;
	ret = 0;

out:
	જો (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_history_split(પૂર્ણાંક argc,
			     અચલ અક્ષર **argv,
			     અચલ અક્ષર *prefix,
			     સંરચના repository *repo)
{
	અચલ અક્ષર * અચલ usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		ખાલી,
	};
	ગણના ref_action action = REF_ACTION_DEFAULT;
	પૂર્ણાંક dry_run = 0;
	સંરચના option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	સંરચના commit *original, *rewritten = ખાલી;
	સંરચના strbuf reflog_સંદેશ = STRBUF_INIT;
	સંરચના pathspec pathspec = { 0 };
	સંરચના rev_info revs = { 0 };
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	જો (argc < 1) {
		ret = error(_("command expects a committish"));
		જાઓ out;
	}
	repo_config(repo, git_default_config, ખાલી);

	જો (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	જો (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		જાઓ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	જો (ret < 0)
		જાઓ out;

	જો (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		જાઓ out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	જો (ret < 0)
		જાઓ out;

	strbuf_addf(&reflog_સંદેશ, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_સંદેશ.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	જો (ret < 0) {
		ret = error(_("failed replaying descendants"));
		જાઓ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_સંદેશ);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	પાછા ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

સ્થિર bool is_autosquash_subject(અચલ અક્ષર *s)
{
	પાછા starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

સ્થિર bool skip_one_autosquash_prefix(અચલ અક્ષર *s, અચલ અક્ષર **out)
{
	જો (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		જ્યારે (**out == ' ')
			(*out)++;
		પાછા true;
	}
	પાછા false;
}

સ્થિર શૂન્ય truncate_message_to_subject(સંરચના strbuf *સંદેશ)
{
	અચલ અક્ષર *eos = strstr(સંદેશ->buf, "\n\n");

	જો (eos)
		strbuf_setlen(સંદેશ, eos - સંદેશ->buf + 1);
}

સંરચના subject_data {
	સંરચના strintmap subjects;
	સંરચના strbuf subject;
	સંરચના strbuf squash_message;
	અચલ અક્ષર *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

સ્થિર શૂન્ય subject_data_clear(સંરચના subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

સ્થિર પૂર્ણાંક squash_amend_message(સંરચના repository *repo,
				સંરચના commit *commit,
				સંરચના subject_data *data,
				unsigned flags)
{
	અચલ અક્ષર *body = data->message + data->subject.len;

	જ્યારે (isspace(*body))
		body++;

	જો (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		પાછા 0;
	}

	જો (data->edit_message) {
		પાછા 0;
	} નહીંતર જો (flags & SQUASH_AMEND_TARGET) {
		જો (starts_with(data->squash_message.buf, "squash!"))
			પાછા error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		જો (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		જો (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} નહીંતર {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_લીટી(&data->squash_message);
	} નહીંતર {
		પાછા error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક squash_squash_message(સંરચના repository *repo,
				સંરચના commit *commit,
				સંરચના subject_data *data,
				unsigned flags)
{
	અચલ અક્ષર *body = data->message + data->subject.len;

	જ્યારે (isspace(*body))
		body++;

	જો (data->edit_message) {
		પાછા 0;
	} નહીંતર જો (flags & SQUASH_AMEND_TARGET) {
		જો (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		જો (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_લીટી(&data->squash_message);
		} નહીંતર {
			પાછા error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} નહીંતર {
		પાછા error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક squash_check_can_autosquash(સંરચના repository *repo,
				       સંરચના commit *commit,
				       સંરચના subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	જો (starts_with(data->subject.buf, "amend!"))
		પાછા squash_amend_message(repo, commit, data, flags);
	નહીંતર જો (starts_with(data->subject.buf, "squash!"))
		પાછા squash_squash_message(repo, commit, data, flags);

	પાછા 0;
}

સ્થિર પૂર્ણાંક squash_check_autosquash_subject(સંરચના repository *repo,
					   સંરચના commit *commit,
					   સંરચના subject_data *data)
{
	અચલ અક્ષર* s = data->subject.buf;
	સંરચના commit *target;
	સંરચના hashmap_iter iter;
	સંરચના strmap_entry *entry;
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
	જ્યારે (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		જો (flags)
			પાછા squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	જો (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		જો (!flags) {
			અચલ અક્ષર *subject_start;
			અચલ અક્ષર *buffer = repo_logmsg_reencode(repo, target,
								  ખાલી, ખાલી);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			અક્ષર *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			મુક્ત(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		જો (flags)
			પાછા squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		જ્યારે(skip_one_autosquash_prefix(s, &s)) {
			જો (starts_with(entry->ચાવી, s)) {
				unsigned કિંમત = (intptr_t)entry->કિંમત;

				પાછા squash_check_can_autosquash(repo, commit,
								   data, કિંમત);
			}
		}
	}
	પાછા error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

સ્થિર પૂર્ણાંક squash_check_subject(સંરચના repository *repo,
				સંરચના commit *commit,
				સંરચના subject_data *data)
{
	પૂર્ણાંક ret = 0;
	અચલ અક્ષર *buf = repo_logmsg_reencode(repo, commit, ખાલી, ખાલી);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	જો (!strintmap_get_size(&data->subjects)) {
		અચલ અક્ષર *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_લીટી(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} નહીંતર જો (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} નહીંતર {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક build_squash_message(સંરચના repository *repo,
				અચલ સંરચના strbuf *todo_buf,
				સંરચના strbuf *out);

સ્થિર પૂર્ણાંક setup_squash_revisions(સંરચના repository *repo,
				  પૂર્ણાંક argc, અચલ અક્ષર **argv,
				  સંરચના rev_info *revs)
{
	repo_init_revisions(repo, revs, ખાલી);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, ખાલી);
	જો (argc > 1)
		પાછા error(_("unrecognized argument: %s"), argv[1]);

	જો (revs->reverse != 1 || revs->topo_order != 1 ||
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
	માટે (size_t i = 0; i < revs->cmdline.nr; i++)
		જો (revs->cmdline.rev[i].flags & BOTTOM)
			પાછા 0;

	પાછા error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
સ્થિર પૂર્ણાંક resolve_squash_range(સંરચના repository *repo,
				bool update_branches,
				bool edit_message,
				પૂર્ણાંક argc, અચલ અક્ષર **argv,
				સંરચના commit **oldest_out,
				સંરચના commit **tip_out,
				અક્ષર **message_out)
{
	સંરચના rev_info revs;
	સંરચના subject_data subject_data = SUBJECT_DATA_INIT;
	સંરચના commit *commit, *oldest = ખાલી, *tip = ખાલી;
	સંરચના strbuf todo_buf = STRBUF_INIT;
	પૂર્ણાંક ret, tip_ગણતરી = 0;
	bool walk_started = false;
	સંરચના ref_filter filter = REF_FILTER_INIT;
	સંરચના ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	જો (ret < 0)
		જાઓ out;

	જો (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		જાઓ out;
	}
	walk_started = true;
	જ્યારે ((commit = get_revision(&revs))) {
		સંરચના commit_list *p;

		જો (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		જો (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			જાઓ out;
		}
		માટે (p = commit->parents; oldest && p; p = p->next) {
			સંરચના commit_list *q;
			સંરચના object *o;
			bool seen;

			જો (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				જાઓ out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			માટે (q = oldest->parents; !seen && q; q = q->next) {
				જો (p->item == q->item) {
					seen = true;
					commit_list_ઉમેરો(commit, &filter.with_commit);
				}
			}
			જો (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				જાઓ out;
			}
			જો (o->flags & SQUASH_TIP) {
				tip_ગણતરી--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		જો (!oldest) {
			commit_list_ઉમેરો(commit, &filter.with_commit);
			oldest = commit;
		}
		જો (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			જાઓ out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_ગણતરી++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	જો (!tip_ગણતરી) {
		ret = error(_("the revision range is empty"));
		જાઓ out;
	} નહીંતર જો (tip_ગણતરી != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		જાઓ out;
	} નહીંતર જો (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		જાઓ out;
	} નહીંતર જો (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ઉમેરો(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	જો (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		જાઓ out;
	}
	જો (refs.nr) {
		સંરચના ref_format format = REF_FORMAT_INIT;
		સંરચના ref_sorting *sorting;
		સંરચના string_list sorting_options = STRING_LIST_INIT_DUP;
		સંરચના strbuf branches = STRBUF_INIT;
		સંરચના strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		જો (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		માટે (પૂર્ણાંક i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			જો (format_ref_array_item(refs.વસ્તુઓ[i], &format,
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
		જાઓ out;
	}
	જો (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		જો (ret < 0)
			જાઓ out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, ખાલી);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	જો (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	પાછા ret;
}

સ્થિર bool amend_replaces_target(સંરચના todo_list *todo, પૂર્ણાંક target)
{
	માટે (પૂર્ણાંક i = target + 1; i < todo->nr &&
				 todo->વસ્તુઓ[i].command != TODO_PICK; i++) {
		જો (todo->વસ્તુઓ[i].command == TODO_SQUASH)
			પાછા false;
		જો (todo->વસ્તુઓ[i].flags & TODO_REPLACE_FIXUP_સંદેશ)
			પાછા true;
	}
	પાછા false;
}

સ્થિર પૂર્ણાંક build_squash_message(સંરચના repository *repo,
				અચલ સંરચના strbuf *todo_buf,
				સંરચના strbuf *out)
{
	સંરચના todo_list todo = TODO_LIST_INIT;
	સંરચના replay_opts opts = REPLAY_OPTS_INIT;
	પૂર્ણાંક nr_commits, ret;

	જો (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		જાઓ out;
	}

	nr_commits = todo.nr;
	માટે (પૂર્ણાંક i = 0; i < nr_commits; i++) {
		સંરચના todo_item *item = &todo.વસ્તુઓ[i];
		અચલ અક્ષર *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_સંદેશ);
		જો (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		નહીંતર
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, ખાલી, ખાલી);
		find_commit_subject(message, &body);

		જો (skip)
			commented_len = strlen(body);
		નહીંતર જો (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		નહીંતર
			commented_len = 0;

		જો (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_લીટીઓ(out, body, commented_len, comment_લીટી_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_લીટી(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_history_squash(પૂર્ણાંક argc,
			      અચલ અક્ષર **argv,
			      અચલ અક્ષર *prefix,
			      સંરચના repository *repo)
{
	અચલ અક્ષર * અચલ usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ખાલી,
	};
	ગણના ref_action action = REF_ACTION_DEFAULT;
	પૂર્ણાંક dry_run = 0;
	પૂર્ણાંક edit = 1;
	સંરચના option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	સંરચના strbuf reflog_સંદેશ = STRBUF_INIT;
	સંરચના commit *oldest, *tip, *rewritten;
	અચલ સંરચના object_id *base_વૃક્ષ_oid, *tip_વૃક્ષ_oid;
	અક્ષર *message_template = ખાલી;
	સંરચના rev_info revs = { 0 };
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	જો (argc < 2) {
		ret = error(_("command expects a revision range"));
		જાઓ out;
	}
	repo_config(repo, git_default_config, ખાલી);

	જો (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_સંદેશ, "squash: updating ");
	strbuf_join_argv(&reflog_સંદેશ, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	જો (ret < 0)
		જાઓ out;

	ret = setup_revwalk(repo, action, tip, &revs);
	જો (ret < 0)
		જાઓ out;

	base_વૃક્ષ_oid = &repo_get_commit_વૃક્ષ(repo,
					oldest->parents->item)->object.oid;
	tip_વૃક્ષ_oid = &repo_get_commit_વૃક્ષ(repo, tip)->object.oid;

	ret = commit_વૃક્ષ_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_વૃક્ષ_oid, tip_વૃક્ષ_oid,
			      &rewritten,
			      edit ? COMMIT_વૃક્ષ_EDIT_MESSAGE : 0);
	જો (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		જાઓ out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_સંદેશ.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	જો (ret < 0) {
		ret = error(_("failed replaying descendants"));
		જાઓ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_સંદેશ);
	release_revisions(&revs);
	મુક્ત(message_template);
	પાછા ret;
}

સ્થિર પૂર્ણાંક update_worktree(સંરચના repository *repo,
			   અચલ સંરચના commit *old_head,
			   અચલ સંરચના commit *new_head,
			   bool dry_run)
{
	સંરચના reset_working_વૃક્ષ_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	જો (dry_run)
		opts.flags |= RESET_WORKING_વૃક્ષ_DRY_RUN;
	પાછા reset_working_વૃક્ષ(repo, &opts);
}

સ્થિર પૂર્ણાંક find_head_વૃક્ષ_change(સંરચના repository *repo,
				 અચલ સંરચના replay_result *result,
				 સંરચના commit **old_head,
				 સંરચના commit **new_head,
				 bool *changed)
{
	અચલ સંરચના replay_ref_update *head_update = ખાલી;
	સંરચના commit *old_head_commit, *new_head_commit;
	સંરચના વૃક્ષ *old_head_વૃક્ષ, *new_head_વૃક્ષ;
	અચલ અક્ષર *head_target;
	પૂર્ણાંક head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ખાલી, &head_flags);
	જો (!head_target)
		પાછા error(_("cannot look up HEAD"));

	માટે (size_t i = 0; i < result->updates_nr; i++) {
		જો (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			તોડો;
		}
	}

	જો (!head_update)
		પાછા 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	જો (!old_head_commit || !new_head_commit)
		પાછા error(_("cannot resolve HEAD commit"));

	old_head_વૃક્ષ = repo_get_commit_વૃક્ષ(repo, old_head_commit);
	new_head_વૃક્ષ = repo_get_commit_વૃક્ષ(repo, new_head_commit);
	જો (!old_head_વૃક્ષ || !new_head_વૃક્ષ)
		પાછા error(_("cannot resolve tree for HEAD"));

	જો (oideq(&old_head_વૃક્ષ->object.oid, &new_head_વૃક્ષ->object.oid))
		પાછા 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_history_drop(પૂર્ણાંક argc,
			    અચલ અક્ષર **argv,
			    અચલ અક્ષર *prefix,
			    સંરચના repository *repo)
{
	અચલ અક્ષર * અચલ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ખાલી,
	};
	ગણના replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	ગણના ref_action action = REF_ACTION_DEFAULT;
	પૂર્ણાંક dry_run = 0;
	સંરચના option options[] = {
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
	સંરચના strbuf reflog_સંદેશ = STRBUF_INIT;
	સંરચના commit *original, *rewritten;
	સંરચના rev_info revs = { 0 };
	સંરચના replay_result result = { 0 };
	સંરચના commit *old_head, *new_head;
	bool head_moves = false;
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	જો (argc != 1) {
		ret = error(_("command expects a single revision"));
		જાઓ out;
	}
	repo_config(repo, git_default_config, ખાલી);

	જો (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	જો (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		જાઓ out;
	}

	જો (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		જાઓ out;
	} નહીંતર જો (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		જાઓ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	જો (ret)
		જાઓ out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	જો (ret) {
		ret = error(_("failed replaying descendants"));
		જાઓ out;
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
	જો (!is_bare_repository(repo)) {
		ret = find_head_વૃક્ષ_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		જો (ret < 0)
			જાઓ out;

		જો (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			જાઓ out;
		}
	}

	strbuf_addf(&reflog_સંદેશ, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_સંદેશ.buf, dry_run);
	જો (ret < 0) {
		ret = error(_("failed to update references"));
		જાઓ out;
	}

	જો (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		જાઓ out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_સંદેશ);
	release_revisions(&revs);
	પાછા ret;
}

પૂર્ણાંક cmd_history(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo)
{
	અચલ અક્ષર * અચલ usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ખાલી,
	};
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	પાછા fn(argc, argv, prefix, repo);
}
