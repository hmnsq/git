#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "merge-ort.h"
#अन्तर्भाव "oidmap.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "ref-filter.h"
#अन्तर्भाव "replay.h"
#अन्तर्भाव "reset.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "wt-status.h"

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

स्थिर शून्य change_data_free(शून्य *util, ध्रुव वर्ण *str UNUSED)
{
	संरचना wt_status_change_data *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

स्थिर पूर्णाङ्क fill_commit_message(संरचना repository *repo,
			       ध्रुव संरचना object_id *old_वृक्षः,
			       ध्रुव संरचना object_id *new_वृक्षः,
			       ध्रुव वर्ण *default_message,
			       ध्रुव वर्ण *action,
			       संरचना strbuf *out)
{
	ध्रुव वर्ण *path = git_path_commit_editmsg();
	ध्रुव वर्ण *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	संरचना wt_status s;

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
	जइ (!s.fp)
		प्रत्यागम error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_पङ्क्तिः_str, hint, action, comment_पङ्क्तिः_str);
	जइ (fwrite(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_वृक्षः, new_वृक्षः);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	जइ (fclose(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	जइ (launch_editor(path, out, रिक्त)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		प्रत्यागम -1;
	}
	strbuf_stripspace(out, comment_पङ्क्तिः_str);

	cleanup_message(out, COMMIT_सन्देशः_CLEANUP_ALL, 0);

	जइ (!out->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

गणना commit_वृक्षः_flags {
	COMMIT_वृक्षः_EDIT_MESSAGE = (1 << 0),
};

स्थिर पूर्णाङ्क commit_वृक्षः_ext(संरचना repository *repo,
			   ध्रुव वर्ण *action,
			   संरचना commit *commit_with_message,
			   ध्रुव वर्ण *message_template,
			   ध्रुव संरचना commit_list *parents,
			   ध्रुव संरचना object_id *old_वृक्षः,
			   ध्रुव संरचना object_id *new_वृक्षः,
			   संरचना commit **out,
			   गणना commit_वृक्षः_flags flags)
{
	ध्रुव वर्ण *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		रिक्त,
	};
	ध्रुव वर्ण *original_message, *original_body, *ptr;
	संरचना commit_extra_header *original_extra_headers = रिक्त;
	संरचना strbuf commit_message = STRBUF_INIT;
	संरचना object_id rewritten_commit_oid;
	वर्ण *original_author = रिक्त;
	size_t len;
	पूर्णाङ्क ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, रिक्त, रिक्त);
	ptr = find_commit_header(original_message, "author", &len);
	जइ (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	जइ (!message_template)
		message_template = original_body;

	जइ (flags & COMMIT_वृक्षः_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_वृक्षः, new_वृक्षः,
					  message_template, action, &commit_message);
		जइ (ret < 0)
			गच्छ out;
	} अण्णहा {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_वृक्षः_extended(commit_message.buf, commit_message.len, new_वृक्षः,
				   parents, &rewritten_commit_oid, original_author,
				   रिक्त, रिक्त, original_extra_headers);
	जइ (ret < 0)
		गच्छ out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	मुक्त(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क first_parent_वृक्षः_oid(संरचना repository *repo,
				 संरचना commit *commit,
				 संरचना object_id *out)
{
	संरचना commit *parent = commit->parents ? commit->parents->item : रिक्त;

	जइ (!parent) {
		oidcpy(out, repo->hash_algo->empty_वृक्षः);
		प्रत्यागम 0;
	}

	जइ (repo_parse_commit(repo, parent))
		प्रत्यागम error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_वृक्षः(repo, parent)->object.oid);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क commit_वृक्षः_with_edited_message(संरचना repository *repo,
					   ध्रुव वर्ण *action,
					   संरचना commit *original,
					   संरचना commit **out)
{
	संरचना object_id parent_वृक्षः_oid;
	ध्रुव संरचना object_id *वृक्षः_oid;

	वृक्षः_oid = &repo_get_commit_वृक्षः(repo, original)->object.oid;

	जइ (first_parent_वृक्षः_oid(repo, original, &parent_वृक्षः_oid) < 0)
		प्रत्यागम -1;

	प्रत्यागम commit_वृक्षः_ext(repo, action, original, रिक्त, original->parents,
			       &parent_वृक्षः_oid, वृक्षः_oid, out, COMMIT_वृक्षः_EDIT_MESSAGE);
}

गणना ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

स्थिर पूर्णाङ्क parse_ref_action(ध्रुव संरचना option *opt, ध्रुव वर्ण *मानम्, पूर्णाङ्क unset)
{
	गणना ref_action *action = opt->मानम्;

	BUG_ON_OPT_NEG_NOARG(unset, मानम्);
	जइ (!strcmp(मानम्, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} अण्णहा जइ (!strcmp(मानम्, "head")) {
		*action = REF_ACTION_HEAD;
	} अण्णहा {
		प्रत्यागम error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क revwalk_contains_merges(संरचना repository *repo,
				   ध्रुव संरचना strvec *revwalk_args)
{
	संरचना strvec args = STRVEC_INIT;
	संरचना rev_info revs;
	पूर्णाङ्क ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_प्रक्षिप(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, रिक्त);

	setup_revisions_from_strvec(&args, &revs, रिक्त);
	जइ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	जइ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		गच्छ out;
	}

	जइ (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		गच्छ out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क setup_revwalk(संरचना repository *repo,
			 गणना ref_action action,
			 संरचना commit *original,
			 संरचना rev_info *revs)
{
	संरचना strvec args = STRVEC_INIT;
	पूर्णाङ्क ret;

	repo_init_revisions(repo, revs, रिक्त);
	strvec_प्रक्षिप(&args, "ignored");
	strvec_प्रक्षिप(&args, "--reverse");
	strvec_प्रक्षिप(&args, "--topo-order");
	strvec_प्रक्षिप(&args, "--full-history");

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
	जइ (action == REF_ACTION_HEAD) {
		संरचना commit_list *from_list = रिक्त;
		संरचना commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		जइ (!head) {
			ret = error(_("cannot look up HEAD"));
			गच्छ out;
		}

		commit_list_निवेशय(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		जइ (ret < 0) {
			ret = error(_("cannot determine descendance"));
			गच्छ out;
		} अण्णहा जइ (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			गच्छ out;
		}

		strvec_प्रक्षिप(&args, "HEAD");
	} अण्णहा {
		strvec_प्रक्षिप(&args, "--branches");
		strvec_प्रक्षिप(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	जइ (ret < 0)
		गच्छ out;

	setup_revisions_from_strvec(&args, revs, रिक्त);
	जइ (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क handle_ref_update(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *refname,
			     ध्रुव संरचना object_id *new_oid,
			     ध्रुव संरचना object_id *old_oid,
			     ध्रुव वर्ण *reflog_सन्देशः,
			     संरचना strbuf *err)
{
	जइ (!transaction) {
		printf("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		प्रत्यागम 0;
	}

	प्रत्यागम ref_transaction_update(transaction, refname, new_oid, old_oid,
				      रिक्त, रिक्त, 0, reflog_सन्देशः, err);
}

स्थिर पूर्णाङ्क compute_pending_ref_updates(संरचना rev_info *revs,
				       गणना ref_action action,
				       संरचना commit *original,
				       संरचना commit *rewritten,
				       गणना replay_empty_commit_action empty,
				       संरचना replay_result *result)
{
	ध्रुव संरचना name_decoration *decoration;
	संरचना replay_revisions_options opts = {
		.empty = empty,
	};
	वर्ण hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	पूर्णाङ्क head_flags = 0;
	पूर्णाङ्क ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, रिक्त, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	जइ (ret)
		प्रत्यागम ret;

	जइ (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	प्रति (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		जइ (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			अनुवर्त;

		जइ (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			अनुवर्त;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		जइ (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			अनुवर्त;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क apply_pending_ref_updates(संरचना repository *repo,
				     ध्रुव संरचना replay_result *result,
				     ध्रुव वर्ण *reflog_सन्देशः,
				     पूर्णाङ्क dry_run)
{
	संरचना ref_transaction *transaction = रिक्त;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णाङ्क ret;

	जइ (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		जइ (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			गच्छ out;
		}
	}

	प्रति (size_t i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_सन्देशः, &err);
		जइ (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			गच्छ out;
		}
	}

	जइ (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		गच्छ out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क handle_reference_updates(संरचना rev_info *revs,
				    गणना ref_action action,
				    संरचना commit *original,
				    संरचना commit *rewritten,
				    ध्रुव वर्ण *reflog_सन्देशः,
				    पूर्णाङ्क dry_run,
				    गणना replay_empty_commit_action empty)
{
	संरचना replay_result result = { 0 };
	पूर्णाङ्क ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	जइ (ret)
		गच्छ out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_सन्देशः, dry_run);

out:
	replay_result_release(&result);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क commit_became_empty(संरचना repository *repo,
			       संरचना commit *original,
			       संरचना वृक्षः *result)
{
	संरचना object_id parent_वृक्षः_oid;

	जइ (first_parent_वृक्षः_oid(repo, original, &parent_वृक्षः_oid) < 0)
		प्रत्यागम -1;

	प्रत्यागम oideq(&result->object.oid, &parent_वृक्षः_oid);
}

स्थिर पूर्णाङ्क parse_opt_empty(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना replay_empty_commit_action *मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	जइ (!strcmp(arg, "drop"))
		*मानम् = REPLAY_EMPTY_COMMIT_DROP;
	अण्णहा जइ (!strcmp(arg, "keep"))
		*मानम् = REPLAY_EMPTY_COMMIT_KEEP;
	अण्णहा जइ (!strcmp(arg, "abort"))
		*मानम् = REPLAY_EMPTY_COMMIT_ABORT;
	अण्णहा
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_history_fixup(पूर्णाङ्क argc,
			     ध्रुव वर्ण **argv,
			     ध्रुव वर्ण *prefix,
			     संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		रिक्त,
	};
	गणना replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	गणना ref_action action = REF_ACTION_DEFAULT;
	गणना commit_वृक्षः_flags flags = 0;
	पूर्णाङ्क dry_run = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_वृक्षः_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	संरचना merge_result merge_result = { 0 };
	संरचना merge_options merge_opts = { 0 };
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *head_commit, *original, *rewritten;
	संरचना वृक्षः *head_वृक्षः, *original_वृक्षः, *index_वृक्षः;
	संरचना rev_info revs = { 0 };
	bool skip_commit = false;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जइ (argc != 1) {
		ret = error(_("command expects a single revision"));
		गच्छ out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जइ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	जइ (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		गच्छ out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	जइ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	जइ (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		गच्छ out;
	}

	head_वृक्षः = repo_get_commit_वृक्षः(repo, head_commit);
	जइ (!head_वृक्षः) {
		ret = error(_("cannot get tree for HEAD"));
		गच्छ out;
	}

	जइ (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		गच्छ out;
	}

	जइ (!repo_index_has_changes(repo, head_वृक्षः, रिक्त)) {
		ret = error(_("nothing to fixup: no staged changes"));
		गच्छ out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_वृक्षः = write_in_core_index_as_वृक्षः(repo, repo->index);
	जइ (!index_वृक्षः) {
		ret = error(_("unable to write index as a tree"));
		गच्छ out;
	}

	original_वृक्षः = repo_get_commit_वृक्षः(repo, original);
	जइ (!original_वृक्षः) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		गच्छ out;
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
	merge_incore_nonrecursive(&merge_opts, head_वृक्षः,
				  original_वृक्षः, index_वृक्षः, &merge_result);

	जइ (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		गच्छ out;
	}

	जइ (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		गच्छ out;
	}

	ret = commit_became_empty(repo, original, merge_result.वृक्षः);
	जइ (ret < 0)
		गच्छ out;
	जइ (ret > 0) {
		विकल्प (empty) {
		पक्ष REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : रिक्त;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			जइ (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				गच्छ out;
			}

			skip_commit = true;
			भङ्ग;
		पक्ष REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			भङ्ग;
		पक्ष REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			गच्छ out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जइ (ret)
		गच्छ out;

	जइ (!skip_commit) {
		ret = commit_वृक्षः_ext(repo, "fixup", original, रिक्त, original->parents,
				      &original_वृक्षः->object.oid, &merge_result.वृक्षः->object.oid,
				      &rewritten, flags);
		जइ (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			गच्छ out;
		}
	}

	strbuf_addf(&reflog_सन्देशः, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_सन्देशः.buf, dry_run, empty);
	जइ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_history_reword(पूर्णाङ्क argc,
			      ध्रुव वर्ण **argv,
			      ध्रुव वर्ण *prefix,
			      संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_run = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *original, *rewritten;
	संरचना rev_info revs = { 0 };
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जइ (argc != 1) {
		ret = error(_("command expects a single revision"));
		गच्छ out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जइ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	जइ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जइ (ret)
		गच्छ out;

	ret = commit_वृक्षः_with_edited_message(repo, "reworded", original, &rewritten);
	जइ (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		गच्छ out;
	}

	strbuf_addf(&reflog_सन्देशः, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_सन्देशः.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	जइ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क write_ondisk_index(संरचना repository *repo,
			      संरचना object_id *oid,
			      ध्रुव वर्ण *path)
{
	संरचना unpack_trees_options opts = { 0 };
	संरचना lock_file lock = LOCK_INIT;
	संरचना वृक्षः_desc वृक्षः_desc;
	संरचना index_state index;
	संरचना वृक्षः *वृक्षः;
	पूर्णाङ्क ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	वृक्षः = repo_parse_वृक्षः_indirect(repo, oid);
	जइ (!वृक्षः) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		गच्छ out;
	}
	init_वृक्षः_desc(&वृक्षः_desc, &वृक्षः->object.oid, वृक्षः->buffer, वृक्षः->size);

	जइ (unpack_trees(1, &वृक्षः_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		गच्छ out;
	}

	अविभाज्यः_cache_वृक्षः(repo, &index, वृक्षः);

	जइ (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		गच्छ out;
	}

	जइ (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		गच्छ out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क split_commit(संरचना repository *repo,
			संरचना commit *original,
			संरचना pathspec *pathspec,
			संरचना commit **out)
{
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना strbuf index_file = STRBUF_INIT;
	संरचना index_state index = INDEX_STATE_INIT(repo);
	ध्रुव संरचना object_id *original_commit_वृक्षः_oid;
	ध्रुव संरचना object_id *old_वृक्षः_oid, *new_वृक्षः_oid;
	संरचना object_id parent_वृक्षः_oid;
	वर्ण original_commit_oid[GIT_MAX_HEXSZ + 1];
	संरचना commit *first_commit, *second_commit;
	संरचना commit_list *parents = रिक्त;
	संरचना वृक्षः *split_वृक्षः;
	पूर्णाङ्क ret;

	जइ (first_parent_वृक्षः_oid(repo, original, &parent_वृक्षः_oid) < 0) {
		ret = -1;
		गच्छ out;
	}
	original_commit_वृक्षः_oid = get_commit_वृक्षः_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_वृक्षः_oid, index_file.buf);
	जइ (ret < 0)
		गच्छ out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	जइ (ret < 0) {
		ret = error(_("failed reading temporary index"));
		गच्छ out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	जइ (ret < 0)
		गच्छ out;

	split_वृक्षः = write_in_core_index_as_वृक्षः(repo, &index);
	जइ (!split_वृक्षः) {
		ret = error(_("failed split tree"));
		गच्छ out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	जइ (oideq(&split_वृक्षः->object.oid, &parent_वृक्षः_oid)) {
		ret = error(_("split commit is empty"));
		गच्छ out;
	} अण्णहा जइ (oideq(&split_वृक्षः->object.oid, original_commit_वृक्षः_oid)) {
		ret = error(_("split commit tree matches original commit"));
		गच्छ out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_वृक्षः_ext(repo, "split-out", original, रिक्त, original->parents, &parent_वृक्षः_oid,
			      &split_वृक्षः->object.oid, &first_commit, COMMIT_वृक्षः_EDIT_MESSAGE);
	जइ (ret < 0) {
		ret = error(_("failed writing first commit"));
		गच्छ out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_वृक्षः_oid = &repo_get_commit_वृक्षः(repo, first_commit)->object.oid;
	new_वृक्षः_oid = &repo_get_commit_वृक्षः(repo, original)->object.oid;

	ret = commit_वृक्षः_ext(repo, "split-out", original, रिक्त, parents, old_वृक्षः_oid,
			      new_वृक्षः_oid, &second_commit, COMMIT_वृक्षः_EDIT_MESSAGE);
	जइ (ret < 0) {
		ret = error(_("failed writing second commit"));
		गच्छ out;
	}

	*out = second_commit;
	ret = 0;

out:
	जइ (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_history_split(पूर्णाङ्क argc,
			     ध्रुव वर्ण **argv,
			     ध्रुव वर्ण *prefix,
			     संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_run = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	संरचना commit *original, *rewritten = रिक्त;
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना rev_info revs = { 0 };
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जइ (argc < 1) {
		ret = error(_("command expects a committish"));
		गच्छ out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जइ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	जइ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जइ (ret < 0)
		गच्छ out;

	जइ (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		गच्छ out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	जइ (ret < 0)
		गच्छ out;

	strbuf_addf(&reflog_सन्देशः, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_सन्देशः.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	जइ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_सन्देशः);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	प्रत्यागम ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

स्थिर bool is_autosquash_subject(ध्रुव वर्ण *s)
{
	प्रत्यागम starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

स्थिर bool skip_one_autosquash_prefix(ध्रुव वर्ण *s, ध्रुव वर्ण **out)
{
	जइ (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		जाव (**out == ' ')
			(*out)++;
		प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर शून्य truncate_message_to_subject(संरचना strbuf *सन्देशः)
{
	ध्रुव वर्ण *eos = strstr(सन्देशः->buf, "\n\n");

	जइ (eos)
		strbuf_setlen(सन्देशः, eos - सन्देशः->buf + 1);
}

संरचना subject_data {
	संरचना strintmap subjects;
	संरचना strbuf subject;
	संरचना strbuf squash_message;
	ध्रुव वर्ण *message;
	bool edit_message;
};

#define SUBJECT_DATA_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

स्थिर शून्य subject_data_clear(संरचना subject_data *data)
{
	strintmap_clear(&data->subjects);
	strbuf_release(&data->subject);
	strbuf_release(&data->squash_message);
}

स्थिर पूर्णाङ्क squash_amend_message(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_data *data,
				unsigned flags)
{
	ध्रुव वर्ण *body = data->message + data->subject.len;

	जाव (isspace(*body))
		body++;

	जइ (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		प्रत्यागम 0;
	}

	जइ (data->edit_message) {
		प्रत्यागम 0;
	} अण्णहा जइ (flags & SQUASH_AMEND_TARGET) {
		जइ (starts_with(data->squash_message.buf, "squash!"))
			प्रत्यागम error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		जइ (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		जइ (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} अण्णहा {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_पङ्क्तिः(&data->squash_message);
	} अण्णहा {
		प्रत्यागम error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क squash_squash_message(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_data *data,
				unsigned flags)
{
	ध्रुव वर्ण *body = data->message + data->subject.len;

	जाव (isspace(*body))
		body++;

	जइ (data->edit_message) {
		प्रत्यागम 0;
	} अण्णहा जइ (flags & SQUASH_AMEND_TARGET) {
		जइ (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		जइ (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_पङ्क्तिः(&data->squash_message);
		} अण्णहा {
			प्रत्यागम error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} अण्णहा {
		प्रत्यागम error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क squash_check_can_autosquash(संरचना repository *repo,
				       संरचना commit *commit,
				       संरचना subject_data *data,
				       unsigned flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	जइ (starts_with(data->subject.buf, "amend!"))
		प्रत्यागम squash_amend_message(repo, commit, data, flags);
	अण्णहा जइ (starts_with(data->subject.buf, "squash!"))
		प्रत्यागम squash_squash_message(repo, commit, data, flags);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क squash_check_autosquash_subject(संरचना repository *repo,
					   संरचना commit *commit,
					   संरचना subject_data *data)
{
	ध्रुव वर्ण* s = data->subject.buf;
	संरचना commit *target;
	संरचना hashmap_iter iter;
	संरचना strmap_entry *entry;
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
	जाव (skip_one_autosquash_prefix(s, &s)) {
		unsigned flags = strintmap_get(&data->subjects, s);
		जइ (flags)
			प्रत्यागम squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	जइ (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		unsigned flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		जइ (!flags) {
			ध्रुव वर्ण *subject_start;
			ध्रुव वर्ण *buffer = repo_logmsg_reencode(repo, target,
								  रिक्त, रिक्त);
			size_t subject_len = find_commit_subject(buffer,
								 &subject_start);
			वर्ण *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			मुक्त(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		जइ (flags)
			प्रत्यागम squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		जाव(skip_one_autosquash_prefix(s, &s)) {
			जइ (starts_with(entry->कुञ्चिका, s)) {
				unsigned मानम् = (intptr_t)entry->मानम्;

				प्रत्यागम squash_check_can_autosquash(repo, commit,
								   data, मानम्);
			}
		}
	}
	प्रत्यागम error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

स्थिर पूर्णाङ्क squash_check_subject(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_data *data)
{
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *buf = repo_logmsg_reencode(repo, commit, रिक्त, रिक्त);
	size_t subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	जइ (!strintmap_get_size(&data->subjects)) {
		ध्रुव वर्ण *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_पङ्क्तिः(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} अण्णहा जइ (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} अण्णहा {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क build_squash_message(संरचना repository *repo,
				ध्रुव संरचना strbuf *todo_buf,
				संरचना strbuf *out);

स्थिर पूर्णाङ्क setup_squash_revisions(संरचना repository *repo,
				  पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				  संरचना rev_info *revs)
{
	repo_init_revisions(repo, revs, रिक्त);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, रिक्त);
	जइ (argc > 1)
		प्रत्यागम error(_("unrecognized argument: %s"), argv[1]);

	जइ (revs->reverse != 1 || revs->topo_order != 1 ||
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
	प्रति (size_t i = 0; i < revs->cmdline.nr; i++)
		जइ (revs->cmdline.rev[i].flags & BOTTOM)
			प्रत्यागम 0;

	प्रत्यागम error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
स्थिर पूर्णाङ्क resolve_squash_range(संरचना repository *repo,
				bool update_branches,
				bool edit_message,
				पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				संरचना commit **oldest_out,
				संरचना commit **tip_out,
				वर्ण **message_out)
{
	संरचना rev_info revs;
	संरचना subject_data subject_data = SUBJECT_DATA_INIT;
	संरचना commit *commit, *oldest = रिक्त, *tip = रिक्त;
	संरचना strbuf todo_buf = STRBUF_INIT;
	पूर्णाङ्क ret, tip_गणनम् = 0;
	bool walk_started = false;
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	जइ (ret < 0)
		गच्छ out;

	जइ (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		गच्छ out;
	}
	walk_started = true;
	जाव ((commit = get_revision(&revs))) {
		संरचना commit_list *p;

		जइ (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		जइ (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			गच्छ out;
		}
		प्रति (p = commit->parents; oldest && p; p = p->next) {
			संरचना commit_list *q;
			संरचना object *o;
			bool seen;

			जइ (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				गच्छ out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			प्रति (q = oldest->parents; !seen && q; q = q->next) {
				जइ (p->item == q->item) {
					seen = true;
					commit_list_निवेशय(commit, &filter.with_commit);
				}
			}
			जइ (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				गच्छ out;
			}
			जइ (o->flags & SQUASH_TIP) {
				tip_गणनम्--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		जइ (!oldest) {
			commit_list_निवेशय(commit, &filter.with_commit);
			oldest = commit;
		}
		जइ (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			गच्छ out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_गणनम्++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	जइ (!tip_गणनम्) {
		ret = error(_("the revision range is empty"));
		गच्छ out;
	} अण्णहा जइ (tip_गणनम् != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		गच्छ out;
	} अण्णहा जइ (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		गच्छ out;
	} अण्णहा जइ (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_निवेशय(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	जइ (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		गच्छ out;
	}
	जइ (refs.nr) {
		संरचना ref_format format = REF_FORMAT_INIT;
		संरचना ref_sorting *sorting;
		संरचना string_list sorting_options = STRING_LIST_INIT_DUP;
		संरचना strbuf branches = STRBUF_INIT;
		संरचना strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		जइ (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		प्रति (पूर्णाङ्क i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			जइ (format_ref_array_item(refs.वस्तूनि[i], &format,
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
		गच्छ out;
	}
	जइ (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		जइ (ret < 0)
			गच्छ out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, रिक्त);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	जइ (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	प्रत्यागम ret;
}

स्थिर bool amend_replaces_target(संरचना todo_list *todo, पूर्णाङ्क target)
{
	प्रति (पूर्णाङ्क i = target + 1; i < todo->nr &&
				 todo->वस्तूनि[i].command != TODO_PICK; i++) {
		जइ (todo->वस्तूनि[i].command == TODO_SQUASH)
			प्रत्यागम false;
		जइ (todo->वस्तूनि[i].flags & TODO_REPLACE_FIXUP_सन्देशः)
			प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर पूर्णाङ्क build_squash_message(संरचना repository *repo,
				ध्रुव संरचना strbuf *todo_buf,
				संरचना strbuf *out)
{
	संरचना todo_list todo = TODO_LIST_INIT;
	संरचना replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णाङ्क nr_commits, ret;

	जइ (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		गच्छ out;
	}

	nr_commits = todo.nr;
	प्रति (पूर्णाङ्क i = 0; i < nr_commits; i++) {
		संरचना todo_item *item = &todo.वस्तूनि[i];
		ध्रुव वर्ण *message, *body;
		size_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_सन्देशः);
		जइ (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		अण्णहा
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, रिक्त, रिक्त);
		find_commit_subject(message, &body);

		जइ (skip)
			commented_len = strlen(body);
		अण्णहा जइ (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		अण्णहा
			commented_len = 0;

		जइ (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_पङ्क्तयः(out, body, commented_len, comment_पङ्क्तिः_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_पङ्क्तिः(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_history_squash(पूर्णाङ्क argc,
			      ध्रुव वर्ण **argv,
			      ध्रुव वर्ण *prefix,
			      संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_run = 0;
	पूर्णाङ्क edit = 1;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *oldest, *tip, *rewritten;
	ध्रुव संरचना object_id *base_वृक्षः_oid, *tip_वृक्षः_oid;
	वर्ण *message_template = रिक्त;
	संरचना rev_info revs = { 0 };
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	जइ (argc < 2) {
		ret = error(_("command expects a revision range"));
		गच्छ out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जइ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_सन्देशः, "squash: updating ");
	strbuf_join_argv(&reflog_सन्देशः, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	जइ (ret < 0)
		गच्छ out;

	ret = setup_revwalk(repo, action, tip, &revs);
	जइ (ret < 0)
		गच्छ out;

	base_वृक्षः_oid = &repo_get_commit_वृक्षः(repo,
					oldest->parents->item)->object.oid;
	tip_वृक्षः_oid = &repo_get_commit_वृक्षः(repo, tip)->object.oid;

	ret = commit_वृक्षः_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_वृक्षः_oid, tip_वृक्षः_oid,
			      &rewritten,
			      edit ? COMMIT_वृक्षः_EDIT_MESSAGE : 0);
	जइ (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		गच्छ out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_सन्देशः.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	जइ (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	मुक्त(message_template);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क update_worktree(संरचना repository *repo,
			   ध्रुव संरचना commit *old_head,
			   ध्रुव संरचना commit *new_head,
			   bool dry_run)
{
	संरचना reset_working_वृक्षः_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	जइ (dry_run)
		opts.flags |= RESET_WORKING_वृक्षः_DRY_RUN;
	प्रत्यागम reset_working_वृक्षः(repo, &opts);
}

स्थिर पूर्णाङ्क find_head_वृक्षः_change(संरचना repository *repo,
				 ध्रुव संरचना replay_result *result,
				 संरचना commit **old_head,
				 संरचना commit **new_head,
				 bool *changed)
{
	ध्रुव संरचना replay_ref_update *head_update = रिक्त;
	संरचना commit *old_head_commit, *new_head_commit;
	संरचना वृक्षः *old_head_वृक्षः, *new_head_वृक्षः;
	ध्रुव वर्ण *head_target;
	पूर्णाङ्क head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      रिक्त, &head_flags);
	जइ (!head_target)
		प्रत्यागम error(_("cannot look up HEAD"));

	प्रति (size_t i = 0; i < result->updates_nr; i++) {
		जइ (!strcmp(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			भङ्ग;
		}
	}

	जइ (!head_update)
		प्रत्यागम 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	जइ (!old_head_commit || !new_head_commit)
		प्रत्यागम error(_("cannot resolve HEAD commit"));

	old_head_वृक्षः = repo_get_commit_वृक्षः(repo, old_head_commit);
	new_head_वृक्षः = repo_get_commit_वृक्षः(repo, new_head_commit);
	जइ (!old_head_वृक्षः || !new_head_वृक्षः)
		प्रत्यागम error(_("cannot resolve tree for HEAD"));

	जइ (oideq(&old_head_वृक्षः->object.oid, &new_head_वृक्षः->object.oid))
		प्रत्यागम 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_history_drop(पूर्णाङ्क argc,
			    ध्रुव वर्ण **argv,
			    ध्रुव वर्ण *prefix,
			    संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_DROP_USAGE,
		रिक्त,
	};
	गणना replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_run = 0;
	संरचना option options[] = {
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
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *original, *rewritten;
	संरचना rev_info revs = { 0 };
	संरचना replay_result result = { 0 };
	संरचना commit *old_head, *new_head;
	bool head_moves = false;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जइ (argc != 1) {
		ret = error(_("command expects a single revision"));
		गच्छ out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जइ (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	जइ (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ out;
	}

	जइ (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		गच्छ out;
	} अण्णहा जइ (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		गच्छ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जइ (ret)
		गच्छ out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	जइ (ret) {
		ret = error(_("failed replaying descendants"));
		गच्छ out;
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
	जइ (!is_bare_repository(repo)) {
		ret = find_head_वृक्षः_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		जइ (ret < 0)
			गच्छ out;

		जइ (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			गच्छ out;
		}
	}

	strbuf_addf(&reflog_सन्देशः, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_सन्देशः.buf, dry_run);
	जइ (ret < 0) {
		ret = error(_("failed to update references"));
		गच्छ out;
	}

	जइ (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		गच्छ out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_history(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *prefix,
		संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		रिक्त,
	};
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	प्रत्यागम fn(argc, argv, prefix, repo);
}
