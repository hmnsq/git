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

স্থির শূন্য change_data_free(শূন্য *util, ধ্রুব বর্ণ *str UNUSED)
{
	সংরচনা wt_status_change_data *d = util;
	মুক্ত(d->rename_source);
	মুক্ত(d);
}

স্থির পূর্ণাঙ্ক fill_commit_message(সংরচনা repository *repo,
			       ধ্রুব সংরচনা object_id *old_বৃক্ষ,
			       ধ্রুব সংরচনা object_id *new_বৃক্ষ,
			       ধ্রুব বর্ণ *default_message,
			       ধ্রুব বর্ণ *action,
			       সংরচনা strbuf *out)
{
	ধ্রুব বর্ণ *path = git_path_commit_editmsg();
	ধ্রুব বর্ণ *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	সংরচনা wt_status s;

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
		প্রত্যাগম error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_সারি_str, hint, action, comment_সারি_str);
	যদি (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_বৃক্ষ, new_বৃক্ষ);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	যদি (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	যদি (launch_editor(path, out, রিক্ত)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		প্রত্যাগম -1;
	}
	strbuf_stripspace(out, comment_সারি_str);

	cleanup_message(out, COMMIT_বার্তা_CLEANUP_ALL, 0);

	যদি (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		প্রত্যাগম -1;
	}

	প্রত্যাগম 0;
}

গণনা commit_বৃক্ষ_flags {
	COMMIT_বৃক্ষ_EDIT_MESSAGE = (1 << 0),
};

স্থির পূর্ণাঙ্ক commit_বৃক্ষ_ext(সংরচনা repository *repo,
			   ধ্রুব বর্ণ *action,
			   সংরচনা commit *commit_with_message,
			   ধ্রুব বর্ণ *message_template,
			   ধ্রুব সংরচনা commit_list *parents,
			   ধ্রুব সংরচনা object_id *old_বৃক্ষ,
			   ধ্রুব সংরচনা object_id *new_বৃক্ষ,
			   সংরচনা commit **out,
			   গণনা commit_বৃক্ষ_flags flags)
{
	ধ্রুব বর্ণ *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		রিক্ত,
	};
	ধ্রুব বর্ণ *original_message, *original_body, *ptr;
	সংরচনা commit_extra_header *original_extra_headers = রিক্ত;
	সংরচনা strbuf commit_message = STRBUF_INIT;
	সংরচনা object_id rewritten_commit_oid;
	বর্ণ *original_author = রিক্ত;
	size_t len;
	পূর্ণাঙ্ক ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, রিক্ত, রিক্ত);
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
			গচ্ছ out;
	} অন্যথা {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_বৃক্ষ_extended(commit_message.buf, commit_message.len, new_বৃক্ষ,
				   parents, &rewritten_commit_oid, original_author,
				   রিক্ত, রিক্ত, original_extra_headers);
	যদি (ret < 0)
		গচ্ছ out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	মুক্ত(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক first_parent_বৃক্ষ_oid(সংরচনা repository *repo,
				 সংরচনা commit *commit,
				 সংরচনা object_id *out)
{
	সংরচনা commit *parent = commit->parents ? commit->parents->item : রিক্ত;

	যদি (!parent) {
		oidcpy(out, repo->hash_algo->empty_বৃক্ষ);
		প্রত্যাগম 0;
	}

	যদি (repo_parse_commit(repo, parent))
		প্রত্যাগম error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_বৃক্ষ(repo, parent)->object.oid);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক commit_বৃক্ষ_with_edited_message(সংরচনা repository *repo,
					   ধ্রুব বর্ণ *action,
					   সংরচনা commit *original,
					   সংরচনা commit **out)
{
	সংরচনা object_id parent_বৃক্ষ_oid;
	ধ্রুব সংরচনা object_id *বৃক্ষ_oid;

	বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, original)->object.oid;

	যদি (first_parent_বৃক্ষ_oid(repo, original, &parent_বৃক্ষ_oid) < 0)
		প্রত্যাগম -1;

	প্রত্যাগম commit_বৃক্ষ_ext(repo, action, original, রিক্ত, original->parents,
			       &parent_বৃক্ষ_oid, বৃক্ষ_oid, out, COMMIT_বৃক্ষ_EDIT_MESSAGE);
}

গণনা ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

স্থির পূর্ণাঙ্ক parse_ref_action(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *মান, পূর্ণাঙ্ক unset)
{
	গণনা ref_action *action = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, মান);
	যদি (!strcmp(মান, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} অন্যথা যদি (!strcmp(মান, "head")) {
		*action = REF_ACTION_HEAD;
	} অন্যথা {
		প্রত্যাগম error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক revwalk_contains_merges(সংরচনা repository *repo,
				   ধ্রুব সংরচনা strvec *revwalk_args)
{
	সংরচনা strvec args = STRVEC_INIT;
	সংরচনা rev_info revs;
	পূর্ণাঙ্ক ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ঠেলো(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, রিক্ত);

	setup_revisions_from_strvec(&args, &revs, রিক্ত);
	যদি (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	যদি (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		গচ্ছ out;
	}

	যদি (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		গচ্ছ out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক setup_revwalk(সংরচনা repository *repo,
			 গণনা ref_action action,
			 সংরচনা commit *original,
			 সংরচনা rev_info *revs)
{
	সংরচনা strvec args = STRVEC_INIT;
	পূর্ণাঙ্ক ret;

	repo_init_revisions(repo, revs, রিক্ত);
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
	যদি (action == REF_ACTION_HEAD) {
		সংরচনা commit_list *from_list = রিক্ত;
		সংরচনা commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		যদি (!head) {
			ret = error(_("cannot look up HEAD"));
			গচ্ছ out;
		}

		commit_list_ঢোকাও(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		যদি (ret < 0) {
			ret = error(_("cannot determine descendance"));
			গচ্ছ out;
		} অন্যথা যদি (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			গচ্ছ out;
		}

		strvec_ঠেলো(&args, "HEAD");
	} অন্যথা {
		strvec_ঠেলো(&args, "--branches");
		strvec_ঠেলো(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	যদি (ret < 0)
		গচ্ছ out;

	setup_revisions_from_strvec(&args, revs, রিক্ত);
	যদি (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক handle_ref_update(সংরচনা ref_transaction *transaction,
			     ধ্রুব বর্ণ *refname,
			     ধ্রুব সংরচনা object_id *new_oid,
			     ধ্রুব সংরচনা object_id *old_oid,
			     ধ্রুব বর্ণ *reflog_বার্তা,
			     সংরচনা strbuf *err)
{
	যদি (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		প্রত্যাগম 0;
	}

	প্রত্যাগম ref_transaction_update(transaction, refname, new_oid, old_oid,
				      রিক্ত, রিক্ত, 0, reflog_বার্তা, err);
}

স্থির পূর্ণাঙ্ক compute_pending_ref_updates(সংরচনা rev_info *revs,
				       গণনা ref_action action,
				       সংরচনা commit *original,
				       সংরচনা commit *rewritten,
				       গণনা replay_empty_commit_action empty,
				       সংরচনা replay_result *result)
{
	ধ্রুব সংরচনা name_decoration *decoration;
	সংরচনা replay_revisions_options opts = {
		.empty = empty,
	};
	বর্ণ hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	পূর্ণাঙ্ক head_flags = 0;
	পূর্ণাঙ্ক ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, রিক্ত, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	যদি (ret)
		প্রত্যাগম ret;

	যদি (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	প্রতি (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		যদি (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			অনুবর্ত;

		যদি (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			অনুবর্ত;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		যদি (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			অনুবর্ত;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক apply_pending_ref_updates(সংরচনা repository *repo,
				     ধ্রুব সংরচনা replay_result *result,
				     ধ্রুব বর্ণ *reflog_বার্তা,
				     পূর্ণাঙ্ক dry_run)
{
	সংরচনা ref_transaction *transaction = রিক্ত;
	সংরচনা strbuf err = STRBUF_INIT;
	পূর্ণাঙ্ক ret;

	যদি (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		যদি (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			গচ্ছ out;
		}
	}

	প্রতি (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_বার্তা, &err);
		যদি (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			গচ্ছ out;
		}
	}

	যদি (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		গচ্ছ out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক handle_reference_updates(সংরচনা rev_info *revs,
				    গণনা ref_action action,
				    সংরচনা commit *original,
				    সংরচনা commit *rewritten,
				    ধ্রুব বর্ণ *reflog_বার্তা,
				    পূর্ণাঙ্ক dry_run,
				    গণনা replay_empty_commit_action empty)
{
	সংরচনা replay_result result = { 0 };
	পূর্ণাঙ্ক ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	যদি (ret)
		গচ্ছ out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_বার্তা, dry_run);

out:
	replay_result_release(&result);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক commit_became_empty(সংরচনা repository *repo,
			       সংরচনা commit *original,
			       সংরচনা বৃক্ষ *result)
{
	সংরচনা object_id parent_বৃক্ষ_oid;

	যদি (first_parent_বৃক্ষ_oid(repo, original, &parent_বৃক্ষ_oid) < 0)
		প্রত্যাগম -1;

	প্রত্যাগম oideq(&result->object.oid, &parent_বৃক্ষ_oid);
}

স্থির পূর্ণাঙ্ক parse_opt_empty(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
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

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক cmd_history_fixup(পূর্ণাঙ্ক argc,
			     ধ্রুব বর্ণ **argv,
			     ধ্রুব বর্ণ *prefix,
			     সংরচনা repository *repo)
{
	ধ্রুব বর্ণ * ধ্রুব usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		রিক্ত,
	};
	গণনা replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	গণনা ref_action action = REF_ACTION_DEFAULT;
	গণনা commit_বৃক্ষ_flags flags = 0;
	পূর্ণাঙ্ক dry_run = 0;
	সংরচনা option options[] = {
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
	সংরচনা merge_result merge_result = { 0 };
	সংরচনা merge_options merge_opts = { 0 };
	সংরচনা strbuf reflog_বার্তা = STRBUF_INIT;
	সংরচনা commit *head_commit, *original, *rewritten;
	সংরচনা বৃক্ষ *head_বৃক্ষ, *original_বৃক্ষ, *index_বৃক্ষ;
	সংরচনা rev_info revs = { 0 };
	bool skip_commit = false;
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1) {
		ret = error(_("command expects a single revision"));
		গচ্ছ out;
	}
	repo_config(repo, git_default_config, রিক্ত);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	যদি (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		গচ্ছ out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		গচ্ছ out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	যদি (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		গচ্ছ out;
	}

	head_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, head_commit);
	যদি (!head_বৃক্ষ) {
		ret = error(_("cannot get tree for HEAD"));
		গচ্ছ out;
	}

	যদি (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		গচ্ছ out;
	}

	যদি (!repo_index_has_changes(repo, head_বৃক্ষ, রিক্ত)) {
		ret = error(_("nothing to fixup: no staged changes"));
		গচ্ছ out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_বৃক্ষ = write_in_core_index_as_বৃক্ষ(repo, repo->index);
	যদি (!index_বৃক্ষ) {
		ret = error(_("unable to write index as a tree"));
		গচ্ছ out;
	}

	original_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, original);
	যদি (!original_বৃক্ষ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		গচ্ছ out;
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
	merge_incore_nonrecursive(&merge_opts, head_বৃক্ষ,
				  original_বৃক্ষ, index_বৃক্ষ, &merge_result);

	যদি (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		গচ্ছ out;
	}

	যদি (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		গচ্ছ out;
	}

	ret = commit_became_empty(repo, original, merge_result.বৃক্ষ);
	যদি (ret < 0)
		গচ্ছ out;
	যদি (ret > 0) {
		বিকল্প (empty) {
		পক্ষ REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : রিক্ত;

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
				গচ্ছ out;
			}

			skip_commit = true;
			ভঙ্গ;
		পক্ষ REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			ভঙ্গ;
		পক্ষ REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			গচ্ছ out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret)
		গচ্ছ out;

	যদি (!skip_commit) {
		ret = commit_বৃক্ষ_ext(repo, "fixup", original, রিক্ত, original->parents,
				      &original_বৃক্ষ->object.oid, &merge_result.বৃক্ষ->object.oid,
				      &rewritten, flags);
		যদি (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			গচ্ছ out;
		}
	}

	strbuf_addf(&reflog_বার্তা, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_বার্তা.buf, dry_run, empty);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		গচ্ছ out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক cmd_history_reword(পূর্ণাঙ্ক argc,
			      ধ্রুব বর্ণ **argv,
			      ধ্রুব বর্ণ *prefix,
			      সংরচনা repository *repo)
{
	ধ্রুব বর্ণ * ধ্রুব usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		রিক্ত,
	};
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণাঙ্ক dry_run = 0;
	সংরচনা option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	সংরচনা strbuf reflog_বার্তা = STRBUF_INIT;
	সংরচনা commit *original, *rewritten;
	সংরচনা rev_info revs = { 0 };
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1) {
		ret = error(_("command expects a single revision"));
		গচ্ছ out;
	}
	repo_config(repo, git_default_config, রিক্ত);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		গচ্ছ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret)
		গচ্ছ out;

	ret = commit_বৃক্ষ_with_edited_message(repo, "reworded", original, &rewritten);
	যদি (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		গচ্ছ out;
	}

	strbuf_addf(&reflog_বার্তা, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_বার্তা.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		গচ্ছ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক write_ondisk_index(সংরচনা repository *repo,
			      সংরচনা object_id *oid,
			      ধ্রুব বর্ণ *path)
{
	সংরচনা unpack_trees_options opts = { 0 };
	সংরচনা lock_file lock = LOCK_INIT;
	সংরচনা বৃক্ষ_desc বৃক্ষ_desc;
	সংরচনা index_state index;
	সংরচনা বৃক্ষ *বৃক্ষ;
	পূর্ণাঙ্ক ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	বৃক্ষ = repo_parse_বৃক্ষ_indirect(repo, oid);
	যদি (!বৃক্ষ) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		গচ্ছ out;
	}
	init_বৃক্ষ_desc(&বৃক্ষ_desc, &বৃক্ষ->object.oid, বৃক্ষ->buffer, বৃক্ষ->size);

	যদি (unpack_trees(1, &বৃক্ষ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		গচ্ছ out;
	}

	মৌলিক_cache_বৃক্ষ(repo, &index, বৃক্ষ);

	যদি (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		গচ্ছ out;
	}

	যদি (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		গচ্ছ out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক split_commit(সংরচনা repository *repo,
			সংরচনা commit *original,
			সংরচনা pathspec *pathspec,
			সংরচনা commit **out)
{
	সংরচনা interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	সংরচনা strbuf index_file = STRBUF_INIT;
	সংরচনা index_state index = INDEX_STATE_INIT(repo);
	ধ্রুব সংরচনা object_id *original_commit_বৃক্ষ_oid;
	ধ্রুব সংরচনা object_id *old_বৃক্ষ_oid, *new_বৃক্ষ_oid;
	সংরচনা object_id parent_বৃক্ষ_oid;
	বর্ণ original_commit_oid[GIT_MAX_HEXSZ + 1];
	সংরচনা commit *first_commit, *second_commit;
	সংরচনা commit_list *parents = রিক্ত;
	সংরচনা বৃক্ষ *split_বৃক্ষ;
	পূর্ণাঙ্ক ret;

	যদি (first_parent_বৃক্ষ_oid(repo, original, &parent_বৃক্ষ_oid) < 0) {
		ret = -1;
		গচ্ছ out;
	}
	original_commit_বৃক্ষ_oid = get_commit_বৃক্ষ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_বৃক্ষ_oid, index_file.buf);
	যদি (ret < 0)
		গচ্ছ out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	যদি (ret < 0) {
		ret = error(_("failed reading temporary index"));
		গচ্ছ out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	যদি (ret < 0)
		গচ্ছ out;

	split_বৃক্ষ = write_in_core_index_as_বৃক্ষ(repo, &index);
	যদি (!split_বৃক্ষ) {
		ret = error(_("failed split tree"));
		গচ্ছ out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	যদি (oideq(&split_বৃক্ষ->object.oid, &parent_বৃক্ষ_oid)) {
		ret = error(_("split commit is empty"));
		গচ্ছ out;
	} অন্যথা যদি (oideq(&split_বৃক্ষ->object.oid, original_commit_বৃক্ষ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		গচ্ছ out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_বৃক্ষ_ext(repo, "split-out", original, রিক্ত, original->parents, &parent_বৃক্ষ_oid,
			      &split_বৃক্ষ->object.oid, &first_commit, COMMIT_বৃক্ষ_EDIT_MESSAGE);
	যদি (ret < 0) {
		ret = error(_("failed writing first commit"));
		গচ্ছ out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, first_commit)->object.oid;
	new_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, original)->object.oid;

	ret = commit_বৃক্ষ_ext(repo, "split-out", original, রিক্ত, parents, old_বৃক্ষ_oid,
			      new_বৃক্ষ_oid, &second_commit, COMMIT_বৃক্ষ_EDIT_MESSAGE);
	যদি (ret < 0) {
		ret = error(_("failed writing second commit"));
		গচ্ছ out;
	}

	*out = second_commit;
	ret = 0;

out:
	যদি (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক cmd_history_split(পূর্ণাঙ্ক argc,
			     ধ্রুব বর্ণ **argv,
			     ধ্রুব বর্ণ *prefix,
			     সংরচনা repository *repo)
{
	ধ্রুব বর্ণ * ধ্রুব usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		রিক্ত,
	};
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণাঙ্ক dry_run = 0;
	সংরচনা option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	সংরচনা commit *original, *rewritten = রিক্ত;
	সংরচনা strbuf reflog_বার্তা = STRBUF_INIT;
	সংরচনা pathspec pathspec = { 0 };
	সংরচনা rev_info revs = { 0 };
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc < 1) {
		ret = error(_("command expects a committish"));
		গচ্ছ out;
	}
	repo_config(repo, git_default_config, রিক্ত);

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
		গচ্ছ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret < 0)
		গচ্ছ out;

	যদি (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		গচ্ছ out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	যদি (ret < 0)
		গচ্ছ out;

	strbuf_addf(&reflog_বার্তা, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_বার্তা.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		গচ্ছ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_বার্তা);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	প্রত্যাগম ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

স্থির bool is_autosquash_subject(ধ্রুব বর্ণ *s)
{
	প্রত্যাগম starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

স্থির bool skip_one_autosquash_prefix(ধ্রুব বর্ণ *s, ধ্রুব বর্ণ **out)
{
	যদি (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		যাবত্ (**out == ' ')
			(*out)++;
		প্রত্যাগম true;
	}
	প্রত্যাগম false;
}

স্থির শূন্য truncate_message_to_subject(সংরচনা strbuf *বার্তা)
{
	ধ্রুব বর্ণ *eos = strstr(বার্তা->buf, "\n\n");

	যদি (eos)
		strbuf_setlen(বার্তা, eos - বার্তা->buf + 1);
}

সংরচনা subject_data {
	সংরচনা strintmap subjects;
	সংরচনা strbuf subject;
	সংরচনা strbuf squash_message;
	ধ্রুব বর্ণ *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

স্থির শূন্য subject_data_clear(সংরচনা subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

স্থির পূর্ণাঙ্ক squash_amend_message(সংরচনা repository *repo,
				সংরচনা commit *commit,
				সংরচনা subject_data *data,
				unsigned flags)
{
	ধ্রুব বর্ণ *body = data->message + data->subject.len;

	যাবত্ (isspace(*body))
		body++;

	যদি (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		প্রত্যাগম 0;
	}

	যদি (data->edit_message) {
		প্রত্যাগম 0;
	} অন্যথা যদি (flags & SQUASH_AMEND_TARGET) {
		যদি (starts_with(data->squash_message.buf, "squash!"))
			প্রত্যাগম error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		যদি (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		যদি (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} অন্যথা {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_সারি(&data->squash_message);
	} অন্যথা {
		প্রত্যাগম error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক squash_squash_message(সংরচনা repository *repo,
				সংরচনা commit *commit,
				সংরচনা subject_data *data,
				unsigned flags)
{
	ধ্রুব বর্ণ *body = data->message + data->subject.len;

	যাবত্ (isspace(*body))
		body++;

	যদি (data->edit_message) {
		প্রত্যাগম 0;
	} অন্যথা যদি (flags & SQUASH_AMEND_TARGET) {
		যদি (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		যদি (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_সারি(&data->squash_message);
		} অন্যথা {
			প্রত্যাগম error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} অন্যথা {
		প্রত্যাগম error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক squash_check_can_autosquash(সংরচনা repository *repo,
				       সংরচনা commit *commit,
				       সংরচনা subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	যদি (starts_with(data->subject.buf, "amend!"))
		প্রত্যাগম squash_amend_message(repo, commit, data, flags);
	অন্যথা যদি (starts_with(data->subject.buf, "squash!"))
		প্রত্যাগম squash_squash_message(repo, commit, data, flags);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক squash_check_autosquash_subject(সংরচনা repository *repo,
					   সংরচনা commit *commit,
					   সংরচনা subject_data *data)
{
	ধ্রুব বর্ণ* s = data->subject.buf;
	সংরচনা commit *target;
	সংরচনা hashmap_iter iter;
	সংরচনা strmap_entry *entry;
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
	যাবত্ (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		যদি (flags)
			প্রত্যাগম squash_check_can_autosquash(repo, commit, data, flags);
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
			ধ্রুব বর্ণ *subject_start;
			ধ্রুব বর্ণ *buffer = repo_logmsg_reencode(repo, target,
								  রিক্ত, রিক্ত);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			বর্ণ *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			মুক্ত(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		যদি (flags)
			প্রত্যাগম squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		যাবত্(skip_one_autosquash_prefix(s, &s)) {
			যদি (starts_with(entry->চাবি, s)) {
				unsigned মান = (intptr_t)entry->মান;

				প্রত্যাগম squash_check_can_autosquash(repo, commit,
								   data, মান);
			}
		}
	}
	প্রত্যাগম error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

স্থির পূর্ণাঙ্ক squash_check_subject(সংরচনা repository *repo,
				সংরচনা commit *commit,
				সংরচনা subject_data *data)
{
	পূর্ণাঙ্ক ret = 0;
	ধ্রুব বর্ণ *buf = repo_logmsg_reencode(repo, commit, রিক্ত, রিক্ত);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	যদি (!strintmap_get_size(&data->subjects)) {
		ধ্রুব বর্ণ *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_সারি(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} অন্যথা যদি (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} অন্যথা {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক build_squash_message(সংরচনা repository *repo,
				ধ্রুব সংরচনা strbuf *todo_buf,
				সংরচনা strbuf *out);

স্থির পূর্ণাঙ্ক setup_squash_revisions(সংরচনা repository *repo,
				  পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				  সংরচনা rev_info *revs)
{
	repo_init_revisions(repo, revs, রিক্ত);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, রিক্ত);
	যদি (argc > 1)
		প্রত্যাগম error(_("unrecognized argument: %s"), argv[1]);

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
	প্রতি (size_t i = 0; i < revs->cmdline.nr; i++)
		যদি (revs->cmdline.rev[i].flags & BOTTOM)
			প্রত্যাগম 0;

	প্রত্যাগম error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
স্থির পূর্ণাঙ্ক resolve_squash_range(সংরচনা repository *repo,
				bool update_branches,
				bool edit_message,
				পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv,
				সংরচনা commit **oldest_out,
				সংরচনা commit **tip_out,
				বর্ণ **message_out)
{
	সংরচনা rev_info revs;
	সংরচনা subject_data subject_data = SUBJECT_DATA_INIT;
	সংরচনা commit *commit, *oldest = রিক্ত, *tip = রিক্ত;
	সংরচনা strbuf todo_buf = STRBUF_INIT;
	পূর্ণাঙ্ক ret, tip_গুনতি = 0;
	bool walk_started = false;
	সংরচনা ref_filter filter = REF_FILTER_INIT;
	সংরচনা ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	যদি (ret < 0)
		গচ্ছ out;

	যদি (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		গচ্ছ out;
	}
	walk_started = true;
	যাবত্ ((commit = get_revision(&revs))) {
		সংরচনা commit_list *p;

		যদি (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		যদি (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			গচ্ছ out;
		}
		প্রতি (p = commit->parents; oldest && p; p = p->next) {
			সংরচনা commit_list *q;
			সংরচনা object *o;
			bool seen;

			যদি (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				গচ্ছ out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			প্রতি (q = oldest->parents; !seen && q; q = q->next) {
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
				গচ্ছ out;
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
		যদি (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			গচ্ছ out;
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
		গচ্ছ out;
	} অন্যথা যদি (tip_গুনতি != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		গচ্ছ out;
	} অন্যথা যদি (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		গচ্ছ out;
	} অন্যথা যদি (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ঢোকাও(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	যদি (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		গচ্ছ out;
	}
	যদি (refs.nr) {
		সংরচনা ref_format format = REF_FORMAT_INIT;
		সংরচনা ref_sorting *sorting;
		সংরচনা string_list sorting_options = STRING_LIST_INIT_DUP;
		সংরচনা strbuf branches = STRBUF_INIT;
		সংরচনা strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		যদি (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		প্রতি (পূর্ণাঙ্ক i = 0; i < refs.nr; i++) {
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
		গচ্ছ out;
	}
	যদি (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		যদি (ret < 0)
			গচ্ছ out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, রিক্ত);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	যদি (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	প্রত্যাগম ret;
}

স্থির bool amend_replaces_target(সংরচনা todo_list *todo, পূর্ণাঙ্ক target)
{
	প্রতি (পূর্ণাঙ্ক i = target + 1; i < todo->nr &&
				 todo->বস্তুসমূহ[i].command != TODO_PICK; i++) {
		যদি (todo->বস্তুসমূহ[i].command == TODO_SQUASH)
			প্রত্যাগম false;
		যদি (todo->বস্তুসমূহ[i].flags & TODO_REPLACE_FIXUP_বার্তা)
			প্রত্যাগম true;
	}
	প্রত্যাগম false;
}

স্থির পূর্ণাঙ্ক build_squash_message(সংরচনা repository *repo,
				ধ্রুব সংরচনা strbuf *todo_buf,
				সংরচনা strbuf *out)
{
	সংরচনা todo_list todo = TODO_LIST_INIT;
	সংরচনা replay_opts opts = REPLAY_OPTS_INIT;
	পূর্ণাঙ্ক nr_commits, ret;

	যদি (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		গচ্ছ out;
	}

	nr_commits = todo.nr;
	প্রতি (পূর্ণাঙ্ক i = 0; i < nr_commits; i++) {
		সংরচনা todo_item *item = &todo.বস্তুসমূহ[i];
		ধ্রুব বর্ণ *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_বার্তা);
		যদি (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		অন্যথা
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, রিক্ত, রিক্ত);
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

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক cmd_history_squash(পূর্ণাঙ্ক argc,
			      ধ্রুব বর্ণ **argv,
			      ধ্রুব বর্ণ *prefix,
			      সংরচনা repository *repo)
{
	ধ্রুব বর্ণ * ধ্রুব usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		রিক্ত,
	};
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণাঙ্ক dry_run = 0;
	পূর্ণাঙ্ক edit = 1;
	সংরচনা option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	সংরচনা strbuf reflog_বার্তা = STRBUF_INIT;
	সংরচনা commit *oldest, *tip, *rewritten;
	ধ্রুব সংরচনা object_id *base_বৃক্ষ_oid, *tip_বৃক্ষ_oid;
	বর্ণ *message_template = রিক্ত;
	সংরচনা rev_info revs = { 0 };
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	যদি (argc < 2) {
		ret = error(_("command expects a revision range"));
		গচ্ছ out;
	}
	repo_config(repo, git_default_config, রিক্ত);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_বার্তা, "squash: updating ");
	strbuf_join_argv(&reflog_বার্তা, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	যদি (ret < 0)
		গচ্ছ out;

	ret = setup_revwalk(repo, action, tip, &revs);
	যদি (ret < 0)
		গচ্ছ out;

	base_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo,
					oldest->parents->item)->object.oid;
	tip_বৃক্ষ_oid = &repo_get_commit_বৃক্ষ(repo, tip)->object.oid;

	ret = commit_বৃক্ষ_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_বৃক্ষ_oid, tip_বৃক্ষ_oid,
			      &rewritten,
			      edit ? COMMIT_বৃক্ষ_EDIT_MESSAGE : 0);
	যদি (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		গচ্ছ out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_বার্তা.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	যদি (ret < 0) {
		ret = error(_("failed replaying descendants"));
		গচ্ছ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	মুক্ত(message_template);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক update_worktree(সংরচনা repository *repo,
			   ধ্রুব সংরচনা commit *old_head,
			   ধ্রুব সংরচনা commit *new_head,
			   bool dry_run)
{
	সংরচনা reset_working_বৃক্ষ_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	যদি (dry_run)
		opts.flags |= RESET_WORKING_বৃক্ষ_DRY_RUN;
	প্রত্যাগম reset_working_বৃক্ষ(repo, &opts);
}

স্থির পূর্ণাঙ্ক find_head_বৃক্ষ_change(সংরচনা repository *repo,
				 ধ্রুব সংরচনা replay_result *result,
				 সংরচনা commit **old_head,
				 সংরচনা commit **new_head,
				 bool *changed)
{
	ধ্রুব সংরচনা replay_ref_update *head_update = রিক্ত;
	সংরচনা commit *old_head_commit, *new_head_commit;
	সংরচনা বৃক্ষ *old_head_বৃক্ষ, *new_head_বৃক্ষ;
	ধ্রুব বর্ণ *head_target;
	পূর্ণাঙ্ক head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      রিক্ত, &head_flags);
	যদি (!head_target)
		প্রত্যাগম error(_("cannot look up HEAD"));

	প্রতি (size_t i = 0; i < result->updates_nr; i++) {
		যদি (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			ভঙ্গ;
		}
	}

	যদি (!head_update)
		প্রত্যাগম 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	যদি (!old_head_commit || !new_head_commit)
		প্রত্যাগম error(_("cannot resolve HEAD commit"));

	old_head_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, old_head_commit);
	new_head_বৃক্ষ = repo_get_commit_বৃক্ষ(repo, new_head_commit);
	যদি (!old_head_বৃক্ষ || !new_head_বৃক্ষ)
		প্রত্যাগম error(_("cannot resolve tree for HEAD"));

	যদি (oideq(&old_head_বৃক্ষ->object.oid, &new_head_বৃক্ষ->object.oid))
		প্রত্যাগম 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক cmd_history_drop(পূর্ণাঙ্ক argc,
			    ধ্রুব বর্ণ **argv,
			    ধ্রুব বর্ণ *prefix,
			    সংরচনা repository *repo)
{
	ধ্রুব বর্ণ * ধ্রুব usage[] = {
		GIT_HISTORY_DROP_USAGE,
		রিক্ত,
	};
	গণনা replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	গণনা ref_action action = REF_ACTION_DEFAULT;
	পূর্ণাঙ্ক dry_run = 0;
	সংরচনা option options[] = {
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
	সংরচনা strbuf reflog_বার্তা = STRBUF_INIT;
	সংরচনা commit *original, *rewritten;
	সংরচনা rev_info revs = { 0 };
	সংরচনা replay_result result = { 0 };
	সংরচনা commit *old_head, *new_head;
	bool head_moves = false;
	পূর্ণাঙ্ক ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	যদি (argc != 1) {
		ret = error(_("command expects a single revision"));
		গচ্ছ out;
	}
	repo_config(repo, git_default_config, রিক্ত);

	যদি (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	যদি (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		গচ্ছ out;
	}

	যদি (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		গচ্ছ out;
	} অন্যথা যদি (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		গচ্ছ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	যদি (ret)
		গচ্ছ out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	যদি (ret) {
		ret = error(_("failed replaying descendants"));
		গচ্ছ out;
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
		ret = find_head_বৃক্ষ_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		যদি (ret < 0)
			গচ্ছ out;

		যদি (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			গচ্ছ out;
		}
	}

	strbuf_addf(&reflog_বার্তা, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_বার্তা.buf, dry_run);
	যদি (ret < 0) {
		ret = error(_("failed to update references"));
		গচ্ছ out;
	}

	যদি (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		গচ্ছ out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_বার্তা);
	release_revisions(&revs);
	প্রত্যাগম ret;
}

পূর্ণাঙ্ক cmd_history(পূর্ণাঙ্ক argc,
		ধ্রুব বর্ণ **argv,
		ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo)
{
	ধ্রুব বর্ণ * ধ্রুব usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		রিক্ত,
	};
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	প্রত্যাগম fn(argc, argv, prefix, repo);
}
