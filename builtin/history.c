#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "merge-ort.h"
#അന്തര്ഭാവ "oidmap.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "ref-filter.h"
#അന്തര്ഭാവ "replay.h"
#അന്തര്ഭാവ "reset.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "wt-status.h"

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

സ്ഥിതം ശൂന്യം change_data_free(ശൂന്യം *util, സ്ഥിരം അക്ഷരം *str UNUSED)
{
	ഘടന wt_status_change_data *d = util;
	സ്വതന്ത്രം(d->rename_source);
	സ്വതന്ത്രം(d);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ fill_commit_message(ഘടന repository *repo,
			       സ്ഥിരം ഘടന object_id *old_വൃക്ഷം,
			       സ്ഥിരം ഘടന object_id *new_വൃക്ഷം,
			       സ്ഥിരം അക്ഷരം *default_message,
			       സ്ഥിരം അക്ഷരം *action,
			       ഘടന strbuf *out)
{
	സ്ഥിരം അക്ഷരം *path = git_path_commit_editmsg();
	സ്ഥിരം അക്ഷരം *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ഘടന wt_status s;

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
	എങ്കിൽ (!s.fp)
		തിരികെ error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_വരി_str, hint, action, comment_വരി_str);
	എങ്കിൽ (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_വൃക്ഷം, new_വൃക്ഷം);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	എങ്കിൽ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	എങ്കിൽ (launch_editor(path, out, ശൂന്യ)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		തിരികെ -1;
	}
	strbuf_stripspace(out, comment_വരി_str);

	cleanup_message(out, COMMIT_സന്ദേശം_CLEANUP_ALL, 0);

	എങ്കിൽ (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		തിരികെ -1;
	}

	തിരികെ 0;
}

എണ്ണൽ commit_വൃക്ഷം_flags {
	COMMIT_വൃക്ഷം_EDIT_MESSAGE = (1 << 0),
};

സ്ഥിതം പൂർണ്ണസംഖ്യ commit_വൃക്ഷം_ext(ഘടന repository *repo,
			   സ്ഥിരം അക്ഷരം *action,
			   ഘടന commit *commit_with_message,
			   സ്ഥിരം അക്ഷരം *message_template,
			   സ്ഥിരം ഘടന commit_list *parents,
			   സ്ഥിരം ഘടന object_id *old_വൃക്ഷം,
			   സ്ഥിരം ഘടന object_id *new_വൃക്ഷം,
			   ഘടന commit **out,
			   എണ്ണൽ commit_വൃക്ഷം_flags flags)
{
	സ്ഥിരം അക്ഷരം *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		ശൂന്യ,
	};
	സ്ഥിരം അക്ഷരം *original_message, *original_body, *ptr;
	ഘടന commit_extra_header *original_extra_headers = ശൂന്യ;
	ഘടന strbuf commit_message = STRBUF_INIT;
	ഘടന object_id rewritten_commit_oid;
	അക്ഷരം *original_author = ശൂന്യ;
	size_t len;
	പൂർണ്ണസംഖ്യ ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, ശൂന്യ, ശൂന്യ);
	ptr = find_commit_header(original_message, "author", &len);
	എങ്കിൽ (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	എങ്കിൽ (!message_template)
		message_template = original_body;

	എങ്കിൽ (flags & COMMIT_വൃക്ഷം_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_വൃക്ഷം, new_വൃക്ഷം,
					  message_template, action, &commit_message);
		എങ്കിൽ (ret < 0)
			പോകുക out;
	} അല്ലെങ്കിൽ {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_വൃക്ഷം_extended(commit_message.buf, commit_message.len, new_വൃക്ഷം,
				   parents, &rewritten_commit_oid, original_author,
				   ശൂന്യ, ശൂന്യ, original_extra_headers);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	സ്വതന്ത്രം(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ first_parent_വൃക്ഷം_oid(ഘടന repository *repo,
				 ഘടന commit *commit,
				 ഘടന object_id *out)
{
	ഘടന commit *parent = commit->parents ? commit->parents->item : ശൂന്യ;

	എങ്കിൽ (!parent) {
		oidcpy(out, repo->hash_algo->empty_വൃക്ഷം);
		തിരികെ 0;
	}

	എങ്കിൽ (repo_parse_commit(repo, parent))
		തിരികെ error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_വൃക്ഷം(repo, parent)->object.oid);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ commit_വൃക്ഷം_with_edited_message(ഘടന repository *repo,
					   സ്ഥിരം അക്ഷരം *action,
					   ഘടന commit *original,
					   ഘടന commit **out)
{
	ഘടന object_id parent_വൃക്ഷം_oid;
	സ്ഥിരം ഘടന object_id *വൃക്ഷം_oid;

	വൃക്ഷം_oid = &repo_get_commit_വൃക്ഷം(repo, original)->object.oid;

	എങ്കിൽ (first_parent_വൃക്ഷം_oid(repo, original, &parent_വൃക്ഷം_oid) < 0)
		തിരികെ -1;

	തിരികെ commit_വൃക്ഷം_ext(repo, action, original, ശൂന്യ, original->parents,
			       &parent_വൃക്ഷം_oid, വൃക്ഷം_oid, out, COMMIT_വൃക്ഷം_EDIT_MESSAGE);
}

എണ്ണൽ ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_ref_action(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *മൂല്യം, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ ref_action *action = opt->മൂല്യം;

	BUG_ON_OPT_NEG_NOARG(unset, മൂല്യം);
	എങ്കിൽ (!strcmp(മൂല്യം, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(മൂല്യം, "head")) {
		*action = REF_ACTION_HEAD;
	} അല്ലെങ്കിൽ {
		തിരികെ error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ revwalk_contains_merges(ഘടന repository *repo,
				   സ്ഥിരം ഘടന strvec *revwalk_args)
{
	ഘടന strvec args = STRVEC_INIT;
	ഘടന rev_info revs;
	പൂർണ്ണസംഖ്യ ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_തള്ളുക(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, ശൂന്യ);

	setup_revisions_from_strvec(&args, &revs, ശൂന്യ);
	എങ്കിൽ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	എങ്കിൽ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		പോകുക out;
	}

	എങ്കിൽ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		പോകുക out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ setup_revwalk(ഘടന repository *repo,
			 എണ്ണൽ ref_action action,
			 ഘടന commit *original,
			 ഘടന rev_info *revs)
{
	ഘടന strvec args = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ ret;

	repo_init_revisions(repo, revs, ശൂന്യ);
	strvec_തള്ളുക(&args, "ignored");
	strvec_തള്ളുക(&args, "--reverse");
	strvec_തള്ളുക(&args, "--topo-order");
	strvec_തള്ളുക(&args, "--full-history");

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
	എങ്കിൽ (action == REF_ACTION_HEAD) {
		ഘടന commit_list *from_list = ശൂന്യ;
		ഘടന commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		എങ്കിൽ (!head) {
			ret = error(_("cannot look up HEAD"));
			പോകുക out;
		}

		commit_list_തിരുകുക(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		എങ്കിൽ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			പോകുക out;
		} അല്ലെങ്കിൽ എങ്കിൽ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			പോകുക out;
		}

		strvec_തള്ളുക(&args, "HEAD");
	} അല്ലെങ്കിൽ {
		strvec_തള്ളുക(&args, "--branches");
		strvec_തള്ളുക(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	setup_revisions_from_strvec(&args, revs, ശൂന്യ);
	എങ്കിൽ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_ref_update(ഘടന ref_transaction *transaction,
			     സ്ഥിരം അക്ഷരം *refname,
			     സ്ഥിരം ഘടന object_id *new_oid,
			     സ്ഥിരം ഘടന object_id *old_oid,
			     സ്ഥിരം അക്ഷരം *reflog_സന്ദേശം,
			     ഘടന strbuf *err)
{
	എങ്കിൽ (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		തിരികെ 0;
	}

	തിരികെ ref_transaction_update(transaction, refname, new_oid, old_oid,
				      ശൂന്യ, ശൂന്യ, 0, reflog_സന്ദേശം, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compute_pending_ref_updates(ഘടന rev_info *revs,
				       എണ്ണൽ ref_action action,
				       ഘടന commit *original,
				       ഘടന commit *rewritten,
				       എണ്ണൽ replay_empty_commit_action empty,
				       ഘടന replay_result *result)
{
	സ്ഥിരം ഘടന name_decoration *decoration;
	ഘടന replay_revisions_options opts = {
		.empty = empty,
	};
	അക്ഷരം hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	പൂർണ്ണസംഖ്യ head_flags = 0;
	പൂർണ്ണസംഖ്യ ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, ശൂന്യ, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	എങ്കിൽ (ret)
		തിരികെ ret;

	എങ്കിൽ (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	വേണ്ടി (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		എങ്കിൽ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			തുടരുക;

		എങ്കിൽ (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			തുടരുക;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		എങ്കിൽ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			തുടരുക;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ apply_pending_ref_updates(ഘടന repository *repo,
				     സ്ഥിരം ഘടന replay_result *result,
				     സ്ഥിരം അക്ഷരം *reflog_സന്ദേശം,
				     പൂർണ്ണസംഖ്യ dry_run)
{
	ഘടന ref_transaction *transaction = ശൂന്യ;
	ഘടന strbuf err = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		എങ്കിൽ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			പോകുക out;
		}
	}

	വേണ്ടി (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_സന്ദേശം, &err);
		എങ്കിൽ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			പോകുക out;
		}
	}

	എങ്കിൽ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		പോകുക out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ handle_reference_updates(ഘടന rev_info *revs,
				    എണ്ണൽ ref_action action,
				    ഘടന commit *original,
				    ഘടന commit *rewritten,
				    സ്ഥിരം അക്ഷരം *reflog_സന്ദേശം,
				    പൂർണ്ണസംഖ്യ dry_run,
				    എണ്ണൽ replay_empty_commit_action empty)
{
	ഘടന replay_result result = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	എങ്കിൽ (ret)
		പോകുക out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_സന്ദേശം, dry_run);

out:
	replay_result_release(&result);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ commit_became_empty(ഘടന repository *repo,
			       ഘടന commit *original,
			       ഘടന വൃക്ഷം *result)
{
	ഘടന object_id parent_വൃക്ഷം_oid;

	എങ്കിൽ (first_parent_വൃക്ഷം_oid(repo, original, &parent_വൃക്ഷം_oid) < 0)
		തിരികെ -1;

	തിരികെ oideq(&result->object.oid, &parent_വൃക്ഷം_oid);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_empty(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ replay_empty_commit_action *മൂല്യം = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!strcmp(arg, "drop"))
		*മൂല്യം = REPLAY_EMPTY_COMMIT_DROP;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "keep"))
		*മൂല്യം = REPLAY_EMPTY_COMMIT_KEEP;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "abort"))
		*മൂല്യം = REPLAY_EMPTY_COMMIT_ABORT;
	അല്ലെങ്കിൽ
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_history_fixup(പൂർണ്ണസംഖ്യ argc,
			     സ്ഥിരം അക്ഷരം **argv,
			     സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		ശൂന്യ,
	};
	എണ്ണൽ replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	എണ്ണൽ ref_action action = REF_ACTION_DEFAULT;
	എണ്ണൽ commit_വൃക്ഷം_flags flags = 0;
	പൂർണ്ണസംഖ്യ dry_run = 0;
	ഘടന option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_വൃക്ഷം_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	ഘടന merge_result merge_result = { 0 };
	ഘടന merge_options merge_opts = { 0 };
	ഘടന strbuf reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന commit *head_commit, *original, *rewritten;
	ഘടന വൃക്ഷം *head_വൃക്ഷം, *original_വൃക്ഷം, *index_വൃക്ഷം;
	ഘടന rev_info revs = { 0 };
	bool skip_commit = false;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	എങ്കിൽ (argc != 1) {
		ret = error(_("command expects a single revision"));
		പോകുക out;
	}
	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	എങ്കിൽ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		പോകുക out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	എങ്കിൽ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		പോകുക out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	എങ്കിൽ (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		പോകുക out;
	}

	head_വൃക്ഷം = repo_get_commit_വൃക്ഷം(repo, head_commit);
	എങ്കിൽ (!head_വൃക്ഷം) {
		ret = error(_("cannot get tree for HEAD"));
		പോകുക out;
	}

	എങ്കിൽ (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		പോകുക out;
	}

	എങ്കിൽ (!repo_index_has_changes(repo, head_വൃക്ഷം, ശൂന്യ)) {
		ret = error(_("nothing to fixup: no staged changes"));
		പോകുക out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_വൃക്ഷം = write_in_core_index_as_വൃക്ഷം(repo, repo->index);
	എങ്കിൽ (!index_വൃക്ഷം) {
		ret = error(_("unable to write index as a tree"));
		പോകുക out;
	}

	original_വൃക്ഷം = repo_get_commit_വൃക്ഷം(repo, original);
	എങ്കിൽ (!original_വൃക്ഷം) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		പോകുക out;
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
	merge_incore_nonrecursive(&merge_opts, head_വൃക്ഷം,
				  original_വൃക്ഷം, index_വൃക്ഷം, &merge_result);

	എങ്കിൽ (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		പോകുക out;
	}

	എങ്കിൽ (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		പോകുക out;
	}

	ret = commit_became_empty(repo, original, merge_result.വൃക്ഷം);
	എങ്കിൽ (ret < 0)
		പോകുക out;
	എങ്കിൽ (ret > 0) {
		തിരഞ്ഞെടുപ്പ് (empty) {
		അവസ്ഥ REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : ശൂന്യ;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			എങ്കിൽ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				പോകുക out;
			}

			skip_commit = true;
			നിർത്തുക;
		അവസ്ഥ REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			നിർത്തുക;
		അവസ്ഥ REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			പോകുക out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	എങ്കിൽ (ret)
		പോകുക out;

	എങ്കിൽ (!skip_commit) {
		ret = commit_വൃക്ഷം_ext(repo, "fixup", original, ശൂന്യ, original->parents,
				      &original_വൃക്ഷം->object.oid, &merge_result.വൃക്ഷം->object.oid,
				      &rewritten, flags);
		എങ്കിൽ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			പോകുക out;
		}
	}

	strbuf_addf(&reflog_സന്ദേശം, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_സന്ദേശം.buf, dry_run, empty);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		പോകുക out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_സന്ദേശം);
	release_revisions(&revs);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_history_reword(പൂർണ്ണസംഖ്യ argc,
			      സ്ഥിരം അക്ഷരം **argv,
			      സ്ഥിരം അക്ഷരം *prefix,
			      ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		ശൂന്യ,
	};
	എണ്ണൽ ref_action action = REF_ACTION_DEFAULT;
	പൂർണ്ണസംഖ്യ dry_run = 0;
	ഘടന option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ഘടന strbuf reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന commit *original, *rewritten;
	ഘടന rev_info revs = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	എങ്കിൽ (argc != 1) {
		ret = error(_("command expects a single revision"));
		പോകുക out;
	}
	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	എങ്കിൽ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		പോകുക out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	എങ്കിൽ (ret)
		പോകുക out;

	ret = commit_വൃക്ഷം_with_edited_message(repo, "reworded", original, &rewritten);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		പോകുക out;
	}

	strbuf_addf(&reflog_സന്ദേശം, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_സന്ദേശം.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		പോകുക out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_സന്ദേശം);
	release_revisions(&revs);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_ondisk_index(ഘടന repository *repo,
			      ഘടന object_id *oid,
			      സ്ഥിരം അക്ഷരം *path)
{
	ഘടന unpack_trees_options opts = { 0 };
	ഘടന lock_file lock = LOCK_INIT;
	ഘടന വൃക്ഷം_desc വൃക്ഷം_desc;
	ഘടന index_state index;
	ഘടന വൃക്ഷം *വൃക്ഷം;
	പൂർണ്ണസംഖ്യ ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(repo, oid);
	എങ്കിൽ (!വൃക്ഷം) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		പോകുക out;
	}
	init_വൃക്ഷം_desc(&വൃക്ഷം_desc, &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);

	എങ്കിൽ (unpack_trees(1, &വൃക്ഷം_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		പോകുക out;
	}

	അഭാജ്യം_cache_വൃക്ഷം(repo, &index, വൃക്ഷം);

	എങ്കിൽ (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		പോകുക out;
	}

	എങ്കിൽ (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		പോകുക out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ split_commit(ഘടന repository *repo,
			ഘടന commit *original,
			ഘടന pathspec *pathspec,
			ഘടന commit **out)
{
	ഘടന interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ഘടന strbuf index_file = STRBUF_INIT;
	ഘടന index_state index = INDEX_STATE_INIT(repo);
	സ്ഥിരം ഘടന object_id *original_commit_വൃക്ഷം_oid;
	സ്ഥിരം ഘടന object_id *old_വൃക്ഷം_oid, *new_വൃക്ഷം_oid;
	ഘടന object_id parent_വൃക്ഷം_oid;
	അക്ഷരം original_commit_oid[GIT_MAX_HEXSZ + 1];
	ഘടന commit *first_commit, *second_commit;
	ഘടന commit_list *parents = ശൂന്യ;
	ഘടന വൃക്ഷം *split_വൃക്ഷം;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (first_parent_വൃക്ഷം_oid(repo, original, &parent_വൃക്ഷം_oid) < 0) {
		ret = -1;
		പോകുക out;
	}
	original_commit_വൃക്ഷം_oid = get_commit_വൃക്ഷം_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_വൃക്ഷം_oid, index_file.buf);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		പോകുക out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	split_വൃക്ഷം = write_in_core_index_as_വൃക്ഷം(repo, &index);
	എങ്കിൽ (!split_വൃക്ഷം) {
		ret = error(_("failed split tree"));
		പോകുക out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	എങ്കിൽ (oideq(&split_വൃക്ഷം->object.oid, &parent_വൃക്ഷം_oid)) {
		ret = error(_("split commit is empty"));
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (oideq(&split_വൃക്ഷം->object.oid, original_commit_വൃക്ഷം_oid)) {
		ret = error(_("split commit tree matches original commit"));
		പോകുക out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_വൃക്ഷം_ext(repo, "split-out", original, ശൂന്യ, original->parents, &parent_വൃക്ഷം_oid,
			      &split_വൃക്ഷം->object.oid, &first_commit, COMMIT_വൃക്ഷം_EDIT_MESSAGE);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed writing first commit"));
		പോകുക out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_വൃക്ഷം_oid = &repo_get_commit_വൃക്ഷം(repo, first_commit)->object.oid;
	new_വൃക്ഷം_oid = &repo_get_commit_വൃക്ഷം(repo, original)->object.oid;

	ret = commit_വൃക്ഷം_ext(repo, "split-out", original, ശൂന്യ, parents, old_വൃക്ഷം_oid,
			      new_വൃക്ഷം_oid, &second_commit, COMMIT_വൃക്ഷം_EDIT_MESSAGE);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed writing second commit"));
		പോകുക out;
	}

	*out = second_commit;
	ret = 0;

out:
	എങ്കിൽ (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_history_split(പൂർണ്ണസംഖ്യ argc,
			     സ്ഥിരം അക്ഷരം **argv,
			     സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		ശൂന്യ,
	};
	എണ്ണൽ ref_action action = REF_ACTION_DEFAULT;
	പൂർണ്ണസംഖ്യ dry_run = 0;
	ഘടന option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	ഘടന commit *original, *rewritten = ശൂന്യ;
	ഘടന strbuf reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന pathspec pathspec = { 0 };
	ഘടന rev_info revs = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	എങ്കിൽ (argc < 1) {
		ret = error(_("command expects a committish"));
		പോകുക out;
	}
	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	എങ്കിൽ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		പോകുക out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	എങ്കിൽ (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		പോകുക out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	strbuf_addf(&reflog_സന്ദേശം, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_സന്ദേശം.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		പോകുക out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_സന്ദേശം);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	തിരികെ ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

സ്ഥിതം bool is_autosquash_subject(സ്ഥിരം അക്ഷരം *s)
{
	തിരികെ starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

സ്ഥിതം bool skip_one_autosquash_prefix(സ്ഥിരം അക്ഷരം *s, സ്ഥിരം അക്ഷരം **out)
{
	എങ്കിൽ (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		ആകുമ്പോൾ (**out == ' ')
			(*out)++;
		തിരികെ true;
	}
	തിരികെ false;
}

സ്ഥിതം ശൂന്യം truncate_message_to_subject(ഘടന strbuf *സന്ദേശം)
{
	സ്ഥിരം അക്ഷരം *eos = strstr(സന്ദേശം->buf, "\n\n");

	എങ്കിൽ (eos)
		strbuf_setlen(സന്ദേശം, eos - സന്ദേശം->buf + 1);
}

ഘടന subject_data {
	ഘടന strintmap subjects;
	ഘടന strbuf subject;
	ഘടന strbuf squash_message;
	സ്ഥിരം അക്ഷരം *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

സ്ഥിതം ശൂന്യം subject_data_clear(ഘടന subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ squash_amend_message(ഘടന repository *repo,
				ഘടന commit *commit,
				ഘടന subject_data *data,
				unsigned flags)
{
	സ്ഥിരം അക്ഷരം *body = data->message + data->subject.len;

	ആകുമ്പോൾ (isspace(*body))
		body++;

	എങ്കിൽ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		തിരികെ 0;
	}

	എങ്കിൽ (data->edit_message) {
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (flags & SQUASH_AMEND_TARGET) {
		എങ്കിൽ (starts_with(data->squash_message.buf, "squash!"))
			തിരികെ error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		എങ്കിൽ (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		എങ്കിൽ (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} അല്ലെങ്കിൽ {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_വരി(&data->squash_message);
	} അല്ലെങ്കിൽ {
		തിരികെ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ squash_squash_message(ഘടന repository *repo,
				ഘടന commit *commit,
				ഘടന subject_data *data,
				unsigned flags)
{
	സ്ഥിരം അക്ഷരം *body = data->message + data->subject.len;

	ആകുമ്പോൾ (isspace(*body))
		body++;

	എങ്കിൽ (data->edit_message) {
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (flags & SQUASH_AMEND_TARGET) {
		എങ്കിൽ (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		എങ്കിൽ (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_വരി(&data->squash_message);
		} അല്ലെങ്കിൽ {
			തിരികെ error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} അല്ലെങ്കിൽ {
		തിരികെ error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ squash_check_can_autosquash(ഘടന repository *repo,
				       ഘടന commit *commit,
				       ഘടന subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	എങ്കിൽ (starts_with(data->subject.buf, "amend!"))
		തിരികെ squash_amend_message(repo, commit, data, flags);
	അല്ലെങ്കിൽ എങ്കിൽ (starts_with(data->subject.buf, "squash!"))
		തിരികെ squash_squash_message(repo, commit, data, flags);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ squash_check_autosquash_subject(ഘടന repository *repo,
					   ഘടന commit *commit,
					   ഘടന subject_data *data)
{
	സ്ഥിരം അക്ഷരം* s = data->subject.buf;
	ഘടന commit *target;
	ഘടന hashmap_iter iter;
	ഘടന strmap_entry *entry;
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
	ആകുമ്പോൾ (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		എങ്കിൽ (flags)
			തിരികെ squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	എങ്കിൽ (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		എങ്കിൽ (!flags) {
			സ്ഥിരം അക്ഷരം *subject_start;
			സ്ഥിരം അക്ഷരം *buffer = repo_logmsg_reencode(repo, target,
								  ശൂന്യ, ശൂന്യ);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			അക്ഷരം *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			സ്വതന്ത്രം(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		എങ്കിൽ (flags)
			തിരികെ squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		ആകുമ്പോൾ(skip_one_autosquash_prefix(s, &s)) {
			എങ്കിൽ (starts_with(entry->താക്കോൽ, s)) {
				unsigned മൂല്യം = (intptr_t)entry->മൂല്യം;

				തിരികെ squash_check_can_autosquash(repo, commit,
								   data, മൂല്യം);
			}
		}
	}
	തിരികെ error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ squash_check_subject(ഘടന repository *repo,
				ഘടന commit *commit,
				ഘടന subject_data *data)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	സ്ഥിരം അക്ഷരം *buf = repo_logmsg_reencode(repo, commit, ശൂന്യ, ശൂന്യ);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	എങ്കിൽ (!strintmap_get_size(&data->subjects)) {
		സ്ഥിരം അക്ഷരം *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_വരി(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} അല്ലെങ്കിൽ എങ്കിൽ (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} അല്ലെങ്കിൽ {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ build_squash_message(ഘടന repository *repo,
				സ്ഥിരം ഘടന strbuf *todo_buf,
				ഘടന strbuf *out);

സ്ഥിതം പൂർണ്ണസംഖ്യ setup_squash_revisions(ഘടന repository *repo,
				  പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				  ഘടന rev_info *revs)
{
	repo_init_revisions(repo, revs, ശൂന്യ);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, ശൂന്യ);
	എങ്കിൽ (argc > 1)
		തിരികെ error(_("unrecognized argument: %s"), argv[1]);

	എങ്കിൽ (revs->reverse != 1 || revs->topo_order != 1 ||
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
	വേണ്ടി (size_t i = 0; i < revs->cmdline.nr; i++)
		എങ്കിൽ (revs->cmdline.rev[i].flags & BOTTOM)
			തിരികെ 0;

	തിരികെ error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ resolve_squash_range(ഘടന repository *repo,
				bool update_branches,
				bool edit_message,
				പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
				ഘടന commit **oldest_out,
				ഘടന commit **tip_out,
				അക്ഷരം **message_out)
{
	ഘടന rev_info revs;
	ഘടന subject_data subject_data = SUBJECT_DATA_INIT;
	ഘടന commit *commit, *oldest = ശൂന്യ, *tip = ശൂന്യ;
	ഘടന strbuf todo_buf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret, tip_എണ്ണം = 0;
	bool walk_started = false;
	ഘടന ref_filter filter = REF_FILTER_INIT;
	ഘടന ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	എങ്കിൽ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		പോകുക out;
	}
	walk_started = true;
	ആകുമ്പോൾ ((commit = get_revision(&revs))) {
		ഘടന commit_list *p;

		എങ്കിൽ (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		എങ്കിൽ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			പോകുക out;
		}
		വേണ്ടി (p = commit->parents; oldest && p; p = p->next) {
			ഘടന commit_list *q;
			ഘടന object *o;
			bool seen;

			എങ്കിൽ (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				പോകുക out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			വേണ്ടി (q = oldest->parents; !seen && q; q = q->next) {
				എങ്കിൽ (p->item == q->item) {
					seen = true;
					commit_list_തിരുകുക(commit, &filter.with_commit);
				}
			}
			എങ്കിൽ (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				പോകുക out;
			}
			എങ്കിൽ (o->flags & SQUASH_TIP) {
				tip_എണ്ണം--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		എങ്കിൽ (!oldest) {
			commit_list_തിരുകുക(commit, &filter.with_commit);
			oldest = commit;
		}
		എങ്കിൽ (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			പോകുക out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_എണ്ണം++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	എങ്കിൽ (!tip_എണ്ണം) {
		ret = error(_("the revision range is empty"));
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (tip_എണ്ണം != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_തിരുകുക(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	എങ്കിൽ (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		പോകുക out;
	}
	എങ്കിൽ (refs.nr) {
		ഘടന ref_format format = REF_FORMAT_INIT;
		ഘടന ref_sorting *sorting;
		ഘടന string_list sorting_options = STRING_LIST_INIT_DUP;
		ഘടന strbuf branches = STRBUF_INIT;
		ഘടന strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		എങ്കിൽ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			എങ്കിൽ (format_ref_array_item(refs.ഇനങ്ങൾ[i], &format,
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
		പോകുക out;
	}
	എങ്കിൽ (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		എങ്കിൽ (ret < 0)
			പോകുക out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, ശൂന്യ);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	എങ്കിൽ (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	തിരികെ ret;
}

സ്ഥിതം bool amend_replaces_target(ഘടന todo_list *todo, പൂർണ്ണസംഖ്യ target)
{
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = target + 1; i < todo->nr &&
				 todo->ഇനങ്ങൾ[i].command != TODO_PICK; i++) {
		എങ്കിൽ (todo->ഇനങ്ങൾ[i].command == TODO_SQUASH)
			തിരികെ false;
		എങ്കിൽ (todo->ഇനങ്ങൾ[i].flags & TODO_REPLACE_FIXUP_സന്ദേശം)
			തിരികെ true;
	}
	തിരികെ false;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ build_squash_message(ഘടന repository *repo,
				സ്ഥിരം ഘടന strbuf *todo_buf,
				ഘടന strbuf *out)
{
	ഘടന todo_list todo = TODO_LIST_INIT;
	ഘടന replay_opts opts = REPLAY_OPTS_INIT;
	പൂർണ്ണസംഖ്യ nr_commits, ret;

	എങ്കിൽ (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		പോകുക out;
	}

	nr_commits = todo.nr;
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nr_commits; i++) {
		ഘടന todo_item *item = &todo.ഇനങ്ങൾ[i];
		സ്ഥിരം അക്ഷരം *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_സന്ദേശം);
		എങ്കിൽ (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		അല്ലെങ്കിൽ
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, ശൂന്യ, ശൂന്യ);
		find_commit_subject(message, &body);

		എങ്കിൽ (skip)
			commented_len = strlen(body);
		അല്ലെങ്കിൽ എങ്കിൽ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		അല്ലെങ്കിൽ
			commented_len = 0;

		എങ്കിൽ (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_വരികൾ(out, body, commented_len, comment_വരി_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_വരി(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_history_squash(പൂർണ്ണസംഖ്യ argc,
			      സ്ഥിരം അക്ഷരം **argv,
			      സ്ഥിരം അക്ഷരം *prefix,
			      ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		ശൂന്യ,
	};
	എണ്ണൽ ref_action action = REF_ACTION_DEFAULT;
	പൂർണ്ണസംഖ്യ dry_run = 0;
	പൂർണ്ണസംഖ്യ edit = 1;
	ഘടന option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	ഘടന strbuf reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന commit *oldest, *tip, *rewritten;
	സ്ഥിരം ഘടന object_id *base_വൃക്ഷം_oid, *tip_വൃക്ഷം_oid;
	അക്ഷരം *message_template = ശൂന്യ;
	ഘടന rev_info revs = { 0 };
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	എങ്കിൽ (argc < 2) {
		ret = error(_("command expects a revision range"));
		പോകുക out;
	}
	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_സന്ദേശം, "squash: updating ");
	strbuf_join_argv(&reflog_സന്ദേശം, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	ret = setup_revwalk(repo, action, tip, &revs);
	എങ്കിൽ (ret < 0)
		പോകുക out;

	base_വൃക്ഷം_oid = &repo_get_commit_വൃക്ഷം(repo,
					oldest->parents->item)->object.oid;
	tip_വൃക്ഷം_oid = &repo_get_commit_വൃക്ഷം(repo, tip)->object.oid;

	ret = commit_വൃക്ഷം_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_വൃക്ഷം_oid, tip_വൃക്ഷം_oid,
			      &rewritten,
			      edit ? COMMIT_വൃക്ഷം_EDIT_MESSAGE : 0);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		പോകുക out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_സന്ദേശം.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		പോകുക out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_സന്ദേശം);
	release_revisions(&revs);
	സ്വതന്ത്രം(message_template);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_worktree(ഘടന repository *repo,
			   സ്ഥിരം ഘടന commit *old_head,
			   സ്ഥിരം ഘടന commit *new_head,
			   bool dry_run)
{
	ഘടന reset_working_വൃക്ഷം_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	എങ്കിൽ (dry_run)
		opts.flags |= RESET_WORKING_വൃക്ഷം_DRY_RUN;
	തിരികെ reset_working_വൃക്ഷം(repo, &opts);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_head_വൃക്ഷം_change(ഘടന repository *repo,
				 സ്ഥിരം ഘടന replay_result *result,
				 ഘടന commit **old_head,
				 ഘടന commit **new_head,
				 bool *changed)
{
	സ്ഥിരം ഘടന replay_ref_update *head_update = ശൂന്യ;
	ഘടന commit *old_head_commit, *new_head_commit;
	ഘടന വൃക്ഷം *old_head_വൃക്ഷം, *new_head_വൃക്ഷം;
	സ്ഥിരം അക്ഷരം *head_target;
	പൂർണ്ണസംഖ്യ head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      ശൂന്യ, &head_flags);
	എങ്കിൽ (!head_target)
		തിരികെ error(_("cannot look up HEAD"));

	വേണ്ടി (size_t i = 0; i < result->updates_nr; i++) {
		എങ്കിൽ (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			നിർത്തുക;
		}
	}

	എങ്കിൽ (!head_update)
		തിരികെ 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	എങ്കിൽ (!old_head_commit || !new_head_commit)
		തിരികെ error(_("cannot resolve HEAD commit"));

	old_head_വൃക്ഷം = repo_get_commit_വൃക്ഷം(repo, old_head_commit);
	new_head_വൃക്ഷം = repo_get_commit_വൃക്ഷം(repo, new_head_commit);
	എങ്കിൽ (!old_head_വൃക്ഷം || !new_head_വൃക്ഷം)
		തിരികെ error(_("cannot resolve tree for HEAD"));

	എങ്കിൽ (oideq(&old_head_വൃക്ഷം->object.oid, &new_head_വൃക്ഷം->object.oid))
		തിരികെ 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_history_drop(പൂർണ്ണസംഖ്യ argc,
			    സ്ഥിരം അക്ഷരം **argv,
			    സ്ഥിരം അക്ഷരം *prefix,
			    ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		GIT_HISTORY_DROP_USAGE,
		ശൂന്യ,
	};
	എണ്ണൽ replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	എണ്ണൽ ref_action action = REF_ACTION_DEFAULT;
	പൂർണ്ണസംഖ്യ dry_run = 0;
	ഘടന option options[] = {
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
	ഘടന strbuf reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന commit *original, *rewritten;
	ഘടന rev_info revs = { 0 };
	ഘടന replay_result result = { 0 };
	ഘടന commit *old_head, *new_head;
	bool head_moves = false;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	എങ്കിൽ (argc != 1) {
		ret = error(_("command expects a single revision"));
		പോകുക out;
	}
	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	എങ്കിൽ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		പോകുക out;
	}

	എങ്കിൽ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		പോകുക out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	എങ്കിൽ (ret)
		പോകുക out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	എങ്കിൽ (ret) {
		ret = error(_("failed replaying descendants"));
		പോകുക out;
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
	എങ്കിൽ (!is_bare_repository(repo)) {
		ret = find_head_വൃക്ഷം_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		എങ്കിൽ (ret < 0)
			പോകുക out;

		എങ്കിൽ (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			പോകുക out;
		}
	}

	strbuf_addf(&reflog_സന്ദേശം, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_സന്ദേശം.buf, dry_run);
	എങ്കിൽ (ret < 0) {
		ret = error(_("failed to update references"));
		പോകുക out;
	}

	എങ്കിൽ (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		പോകുക out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_സന്ദേശം);
	release_revisions(&revs);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_history(പൂർണ്ണസംഖ്യ argc,
		സ്ഥിരം അക്ഷരം **argv,
		സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		ശൂന്യ,
	};
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	തിരികെ fn(argc, argv, prefix, repo);
}
