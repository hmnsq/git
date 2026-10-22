#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "cache-tree.h"
#समावेश "commit.h"
#समावेश "commit-reach.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "lockfile.h"
#समावेश "merge-ort.h"
#समावेश "oidmap.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "read-cache.h"
#समावेश "refs.h"
#समावेश "ref-filter.h"
#समावेश "replay.h"
#समावेश "reset.h"
#समावेश "revision.h"
#समावेश "sequencer.h"
#समावेश "string-list.h"
#समावेश "strvec.h"
#समावेश "tree.h"
#समावेश "tree-walk.h"
#समावेश "unpack-trees.h"
#समावेश "wt-status.h"

#घोषणा GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#घोषणा GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#घोषणा GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#घोषणा GIT_HISTORY_SPLIT_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#घोषणा GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

अटल व्योम change_तथ्य_free(व्योम *util, स्थिर अक्षर *str UNUSED)
{
	काष्ठा wt_status_change_तथ्य *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

अटल पूर्णांक fill_commit_message(काष्ठा repository *repo,
			       स्थिर काष्ठा object_id *old_वृक्ष,
			       स्थिर काष्ठा object_id *new_वृक्ष,
			       स्थिर अक्षर *default_message,
			       स्थिर अक्षर *action,
			       काष्ठा strbuf *out)
{
	स्थिर अक्षर *path = git_path_commit_editmsg();
	स्थिर अक्षर *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	काष्ठा wt_status s;

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

	s.fp = ख_खोलो(path, "w");
	अगर (!s.fp)
		वापस error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_पंक्ति_str, hint, action, comment_पंक्ति_str);
	अगर (ख_डालो(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_वृक्ष, new_वृक्ष);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_तथ्य_free);
	अगर (ख_बंद(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	अगर (launch_editor(path, out, शून्य)) {
		ख_लिखो(मानक_त्रुटि, _("Aborting commit as launching the editor failed.\n"));
		वापस -1;
	}
	strbuf_stripspace(out, comment_पंक्ति_str);

	cleanup_message(out, COMMIT_संदेश_CLEANUP_ALL, 0);

	अगर (!out->len) {
		ख_लिखो(मानक_त्रुटि, _("Aborting commit due to empty commit message.\n"));
		वापस -1;
	}

	वापस 0;
}

क्रमागत commit_वृक्ष_flags {
	COMMIT_वृक्ष_EDIT_MESSAGE = (1 << 0),
};

अटल पूर्णांक commit_वृक्ष_ext(काष्ठा repository *repo,
			   स्थिर अक्षर *action,
			   काष्ठा commit *commit_with_message,
			   स्थिर अक्षर *message_template,
			   स्थिर काष्ठा commit_list *parents,
			   स्थिर काष्ठा object_id *old_वृक्ष,
			   स्थिर काष्ठा object_id *new_वृक्ष,
			   काष्ठा commit **out,
			   क्रमागत commit_वृक्ष_flags flags)
{
	स्थिर अक्षर *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		शून्य,
	};
	स्थिर अक्षर *original_message, *original_body, *ptr;
	काष्ठा commit_extra_header *original_extra_headers = शून्य;
	काष्ठा strbuf commit_message = STRBUF_INIT;
	काष्ठा object_id rewritten_commit_oid;
	अक्षर *original_author = शून्य;
	माप_प्रकार len;
	पूर्णांक ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, शून्य, शून्य);
	ptr = find_commit_header(original_message, "author", &len);
	अगर (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	अगर (!message_template)
		message_template = original_body;

	अगर (flags & COMMIT_वृक्ष_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_वृक्ष, new_वृक्ष,
					  message_template, action, &commit_message);
		अगर (ret < 0)
			जाओ out;
	} अन्यथा {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_वृक्ष_extended(commit_message.buf, commit_message.len, new_वृक्ष,
				   parents, &rewritten_commit_oid, original_author,
				   शून्य, शून्य, original_extra_headers);
	अगर (ret < 0)
		जाओ out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	मुक्त(original_author);
	repo_unuse_commit_बफर(repo, commit_with_message, original_message);
	वापस ret;
}

अटल पूर्णांक first_parent_वृक्ष_oid(काष्ठा repository *repo,
				 काष्ठा commit *commit,
				 काष्ठा object_id *out)
{
	काष्ठा commit *parent = commit->parents ? commit->parents->item : शून्य;

	अगर (!parent) {
		oidcpy(out, repo->hash_algo->empty_वृक्ष);
		वापस 0;
	}

	अगर (repo_parse_commit(repo, parent))
		वापस error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_वृक्ष(repo, parent)->object.oid);
	वापस 0;
}

अटल पूर्णांक commit_वृक्ष_with_edited_message(काष्ठा repository *repo,
					   स्थिर अक्षर *action,
					   काष्ठा commit *original,
					   काष्ठा commit **out)
{
	काष्ठा object_id parent_वृक्ष_oid;
	स्थिर काष्ठा object_id *वृक्ष_oid;

	वृक्ष_oid = &repo_get_commit_वृक्ष(repo, original)->object.oid;

	अगर (first_parent_वृक्ष_oid(repo, original, &parent_वृक्ष_oid) < 0)
		वापस -1;

	वापस commit_वृक्ष_ext(repo, action, original, शून्य, original->parents,
			       &parent_वृक्ष_oid, वृक्ष_oid, out, COMMIT_वृक्ष_EDIT_MESSAGE);
}

क्रमागत ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_शीर्ष,
};

अटल पूर्णांक parse_ref_action(स्थिर काष्ठा option *opt, स्थिर अक्षर *मूल्य, पूर्णांक unset)
{
	क्रमागत ref_action *action = opt->मूल्य;

	BUG_ON_OPT_NEG_NOARG(unset, मूल्य);
	अगर (!म_भेद(मूल्य, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} अन्यथा अगर (!म_भेद(मूल्य, "head")) {
		*action = REF_ACTION_शीर्ष;
	} अन्यथा {
		वापस error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	वापस 0;
}

अटल पूर्णांक revwalk_contains_merges(काष्ठा repository *repo,
				   स्थिर काष्ठा strvec *revwalk_args)
{
	काष्ठा strvec args = STRVEC_INIT;
	काष्ठा rev_info revs;
	पूर्णांक ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_धकेलो(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, शून्य);

	setup_revisions_from_strvec(&args, &revs, शून्य);
	अगर (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	अगर (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		जाओ out;
	}

	अगर (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		जाओ out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	वापस ret;
}

अटल पूर्णांक setup_revwalk(काष्ठा repository *repo,
			 क्रमागत ref_action action,
			 काष्ठा commit *original,
			 काष्ठा rev_info *revs)
{
	काष्ठा strvec args = STRVEC_INIT;
	पूर्णांक ret;

	repo_init_revisions(repo, revs, शून्य);
	strvec_धकेलो(&args, "ignored");
	strvec_धकेलो(&args, "--reverse");
	strvec_धकेलो(&args, "--topo-order");
	strvec_धकेलो(&args, "--full-history");

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
	अगर (action == REF_ACTION_शीर्ष) {
		काष्ठा commit_list *from_list = शून्य;
		काष्ठा commit *शीर्ष;

		शीर्ष = lookup_commit_reference_by_name("HEAD");
		अगर (!शीर्ष) {
			ret = error(_("cannot look up HEAD"));
			जाओ out;
		}

		commit_list_जोड़ो(original, &from_list);
		ret = repo_is_descendant_of(repo, शीर्ष, from_list);
		commit_list_free(from_list);

		अगर (ret < 0) {
			ret = error(_("cannot determine descendance"));
			जाओ out;
		} अन्यथा अगर (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			जाओ out;
		}

		strvec_धकेलो(&args, "HEAD");
	} अन्यथा {
		strvec_धकेलो(&args, "--branches");
		strvec_धकेलो(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	अगर (ret < 0)
		जाओ out;

	setup_revisions_from_strvec(&args, revs, शून्य);
	अगर (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	वापस ret;
}

अटल पूर्णांक handle_ref_update(काष्ठा ref_transaction *transaction,
			     स्थिर अक्षर *refname,
			     स्थिर काष्ठा object_id *new_oid,
			     स्थिर काष्ठा object_id *old_oid,
			     स्थिर अक्षर *reflog_संदेश,
			     काष्ठा strbuf *err)
{
	अगर (!transaction) {
		म_लिखो("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		वापस 0;
	}

	वापस ref_transaction_update(transaction, refname, new_oid, old_oid,
				      शून्य, शून्य, 0, reflog_संदेश, err);
}

अटल पूर्णांक compute_pending_ref_updates(काष्ठा rev_info *revs,
				       क्रमागत ref_action action,
				       काष्ठा commit *original,
				       काष्ठा commit *rewritten,
				       क्रमागत replay_empty_commit_action empty,
				       काष्ठा replay_परिणाम *परिणाम)
{
	स्थिर काष्ठा name_decoration *decoration;
	काष्ठा replay_revisions_options opts = {
		.empty = empty,
	};
	अक्षर hex[GIT_MAX_HEXSZ + 1];
	bool detached_शीर्ष;
	पूर्णांक शीर्ष_flags = 0;
	पूर्णांक ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, शून्य, &शीर्ष_flags);
	detached_शीर्ष = !(शीर्ष_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, परिणाम);
	अगर (ret)
		वापस ret;

	अगर (action != REF_ACTION_BRANCHES && action != REF_ACTION_शीर्ष)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	क्रम (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->अग्र)
	{
		अगर (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_शीर्ष)
			जारी;

		अगर (action == REF_ACTION_शीर्ष &&
		    decoration->type != DECORATION_REF_शीर्ष)
			जारी;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		अगर (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_शीर्ष &&
		    !detached_शीर्ष)
			जारी;

		replay_परिणाम_queue_update(परिणाम, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	वापस 0;
}

अटल पूर्णांक apply_pending_ref_updates(काष्ठा repository *repo,
				     स्थिर काष्ठा replay_परिणाम *परिणाम,
				     स्थिर अक्षर *reflog_संदेश,
				     पूर्णांक dry_run)
{
	काष्ठा ref_transaction *transaction = शून्य;
	काष्ठा strbuf err = STRBUF_INIT;
	पूर्णांक ret;

	अगर (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		अगर (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			जाओ out;
		}
	}

	क्रम (माप_प्रकार i = 0; i < परिणाम->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					परिणाम->updates[i].refname,
					&परिणाम->updates[i].new_oid,
					&परिणाम->updates[i].old_oid,
					reflog_संदेश, &err);
		अगर (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    परिणाम->updates[i].refname, err.buf);
			जाओ out;
		}
	}

	अगर (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		जाओ out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	वापस ret;
}

अटल पूर्णांक handle_reference_updates(काष्ठा rev_info *revs,
				    क्रमागत ref_action action,
				    काष्ठा commit *original,
				    काष्ठा commit *rewritten,
				    स्थिर अक्षर *reflog_संदेश,
				    पूर्णांक dry_run,
				    क्रमागत replay_empty_commit_action empty)
{
	काष्ठा replay_परिणाम परिणाम = { 0 };
	पूर्णांक ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &परिणाम);
	अगर (ret)
		जाओ out;

	ret = apply_pending_ref_updates(revs->repo, &परिणाम, reflog_संदेश, dry_run);

out:
	replay_परिणाम_release(&परिणाम);
	वापस ret;
}

अटल पूर्णांक commit_became_empty(काष्ठा repository *repo,
			       काष्ठा commit *original,
			       काष्ठा वृक्ष *परिणाम)
{
	काष्ठा object_id parent_वृक्ष_oid;

	अगर (first_parent_वृक्ष_oid(repo, original, &parent_वृक्ष_oid) < 0)
		वापस -1;

	वापस oideq(&परिणाम->object.oid, &parent_वृक्ष_oid);
}

अटल पूर्णांक parse_opt_empty(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	क्रमागत replay_empty_commit_action *मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!म_भेद(arg, "drop"))
		*मूल्य = REPLAY_EMPTY_COMMIT_DROP;
	अन्यथा अगर (!म_भेद(arg, "keep"))
		*मूल्य = REPLAY_EMPTY_COMMIT_KEEP;
	अन्यथा अगर (!म_भेद(arg, "abort"))
		*मूल्य = REPLAY_EMPTY_COMMIT_ABORT;
	अन्यथा
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	वापस 0;
}

अटल पूर्णांक cmd_history_fixup(पूर्णांक argc,
			     स्थिर अक्षर **argv,
			     स्थिर अक्षर *prefix,
			     काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		शून्य,
	};
	क्रमागत replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	क्रमागत ref_action action = REF_ACTION_DEFAULT;
	क्रमागत commit_वृक्ष_flags flags = 0;
	पूर्णांक dry_run = 0;
	काष्ठा option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_वृक्ष_EDIT_MESSAGE),
		OPT_CALLBACK_F(0, "empty", &empty, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_empty),
		OPT_END(),
	};
	काष्ठा merge_परिणाम merge_परिणाम = { 0 };
	काष्ठा merge_options merge_opts = { 0 };
	काष्ठा strbuf reflog_संदेश = STRBUF_INIT;
	काष्ठा commit *शीर्ष_commit, *original, *rewritten;
	काष्ठा वृक्ष *शीर्ष_वृक्ष, *original_वृक्ष, *सूचकांक_वृक्ष;
	काष्ठा rev_info revs = { 0 };
	bool skip_commit = false;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	अगर (argc != 1) {
		ret = error(_("command expects a single revision"));
		जाओ out;
	}
	repo_config(repo, git_default_config, शून्य);

	अगर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	अगर (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		जाओ out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	अगर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जाओ out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	शीर्ष_commit = lookup_commit_reference_by_name("HEAD");
	अगर (!शीर्ष_commit) {
		ret = error(_("cannot look up HEAD"));
		जाओ out;
	}

	शीर्ष_वृक्ष = repo_get_commit_वृक्ष(repo, शीर्ष_commit);
	अगर (!शीर्ष_वृक्ष) {
		ret = error(_("cannot get tree for HEAD"));
		जाओ out;
	}

	अगर (repo_read_सूचकांक(repo) < 0) {
		ret = error(_("unable to read index"));
		जाओ out;
	}

	अगर (!repo_सूचकांक_has_changes(repo, शीर्ष_वृक्ष, शून्य)) {
		ret = error(_("nothing to fixup: no staged changes"));
		जाओ out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	सूचकांक_वृक्ष = write_in_core_सूचकांक_as_वृक्ष(repo, repo->सूचकांक);
	अगर (!सूचकांक_वृक्ष) {
		ret = error(_("unable to write index as a tree"));
		जाओ out;
	}

	original_वृक्ष = repo_get_commit_वृक्ष(repo, original);
	अगर (!original_वृक्ष) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		जाओ out;
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
	merge_incore_nonrecursive(&merge_opts, शीर्ष_वृक्ष,
				  original_वृक्ष, सूचकांक_वृक्ष, &merge_परिणाम);

	अगर (merge_परिणाम.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		जाओ out;
	}

	अगर (!merge_परिणाम.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		जाओ out;
	}

	ret = commit_became_empty(repo, original, merge_परिणाम.वृक्ष);
	अगर (ret < 0)
		जाओ out;
	अगर (ret > 0) {
		चयन (empty) {
		हाल REPLAY_EMPTY_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->item : शून्य;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			अगर (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				जाओ out;
			}

			skip_commit = true;
			अवरोध;
		हाल REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			अवरोध;
		हाल REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			जाओ out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	अगर (ret)
		जाओ out;

	अगर (!skip_commit) {
		ret = commit_वृक्ष_ext(repo, "fixup", original, शून्य, original->parents,
				      &original_वृक्ष->object.oid, &merge_परिणाम.वृक्ष->object.oid,
				      &rewritten, flags);
		अगर (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			जाओ out;
		}
	}

	strbuf_addf(&reflog_संदेश, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_संदेश.buf, dry_run, empty);
	अगर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जाओ out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_परिणाम);
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	वापस ret;
}

अटल पूर्णांक cmd_history_reword(पूर्णांक argc,
			      स्थिर अक्षर **argv,
			      स्थिर अक्षर *prefix,
			      काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		शून्य,
	};
	क्रमागत ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	काष्ठा option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	काष्ठा strbuf reflog_संदेश = STRBUF_INIT;
	काष्ठा commit *original, *rewritten;
	काष्ठा rev_info revs = { 0 };
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	अगर (argc != 1) {
		ret = error(_("command expects a single revision"));
		जाओ out;
	}
	repo_config(repo, git_default_config, शून्य);

	अगर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	अगर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जाओ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	अगर (ret)
		जाओ out;

	ret = commit_वृक्ष_with_edited_message(repo, "reworded", original, &rewritten);
	अगर (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		जाओ out;
	}

	strbuf_addf(&reflog_संदेश, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_संदेश.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	अगर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जाओ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	वापस ret;
}

अटल पूर्णांक write_ondisk_सूचकांक(काष्ठा repository *repo,
			      काष्ठा object_id *oid,
			      स्थिर अक्षर *path)
{
	काष्ठा unpack_trees_options opts = { 0 };
	काष्ठा lock_file lock = LOCK_INIT;
	काष्ठा वृक्ष_desc वृक्ष_desc;
	काष्ठा सूचकांक_state सूचकांक;
	काष्ठा वृक्ष *वृक्ष;
	पूर्णांक ret;

	सूचकांक_state_init(&सूचकांक, repo);

	opts.शीर्ष_idx = -1;
	opts.src_सूचकांक = &सूचकांक;
	opts.dst_सूचकांक = &सूचकांक;

	वृक्ष = repo_parse_वृक्ष_indirect(repo, oid);
	अगर (!वृक्ष) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		जाओ out;
	}
	init_वृक्ष_desc(&वृक्ष_desc, &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);

	अगर (unpack_trees(1, &वृक्ष_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		जाओ out;
	}

	अभाज्य_cache_वृक्ष(repo, &सूचकांक, वृक्ष);

	अगर (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		जाओ out;
	}

	अगर (write_locked_सूचकांक(&सूचकांक, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		जाओ out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_सूचकांक(&सूचकांक);
	वापस ret;
}

अटल पूर्णांक split_commit(काष्ठा repository *repo,
			काष्ठा commit *original,
			काष्ठा pathspec *pathspec,
			काष्ठा commit **out)
{
	काष्ठा interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	काष्ठा strbuf सूचकांक_file = STRBUF_INIT;
	काष्ठा सूचकांक_state सूचकांक = सूचकांक_STATE_INIT(repo);
	स्थिर काष्ठा object_id *original_commit_वृक्ष_oid;
	स्थिर काष्ठा object_id *old_वृक्ष_oid, *new_वृक्ष_oid;
	काष्ठा object_id parent_वृक्ष_oid;
	अक्षर original_commit_oid[GIT_MAX_HEXSZ + 1];
	काष्ठा commit *first_commit, *second_commit;
	काष्ठा commit_list *parents = शून्य;
	काष्ठा वृक्ष *split_वृक्ष;
	पूर्णांक ret;

	अगर (first_parent_वृक्ष_oid(repo, original, &parent_वृक्ष_oid) < 0) {
		ret = -1;
		जाओ out;
	}
	original_commit_वृक्ष_oid = get_commit_वृक्ष_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &सूचकांक_file, "%s", "history-split.index");

	ret = write_ondisk_सूचकांक(repo, &parent_वृक्ष_oid, सूचकांक_file.buf);
	अगर (ret < 0)
		जाओ out;

	ret = read_सूचकांक_from(&सूचकांक, सूचकांक_file.buf, repo->gitdir);
	अगर (ret < 0) {
		ret = error(_("failed reading temporary index"));
		जाओ out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_सूचकांक(repo, &सूचकांक, सूचकांक_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	अगर (ret < 0)
		जाओ out;

	split_वृक्ष = write_in_core_सूचकांक_as_वृक्ष(repo, &सूचकांक);
	अगर (!split_वृक्ष) {
		ret = error(_("failed split tree"));
		जाओ out;
	}

	unlink(सूचकांक_file.buf);
	strbuf_release(&सूचकांक_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	अगर (oideq(&split_वृक्ष->object.oid, &parent_वृक्ष_oid)) {
		ret = error(_("split commit is empty"));
		जाओ out;
	} अन्यथा अगर (oideq(&split_वृक्ष->object.oid, original_commit_वृक्ष_oid)) {
		ret = error(_("split commit tree matches original commit"));
		जाओ out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_वृक्ष_ext(repo, "split-out", original, शून्य, original->parents, &parent_वृक्ष_oid,
			      &split_वृक्ष->object.oid, &first_commit, COMMIT_वृक्ष_EDIT_MESSAGE);
	अगर (ret < 0) {
		ret = error(_("failed writing first commit"));
		जाओ out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_वृक्ष_oid = &repo_get_commit_वृक्ष(repo, first_commit)->object.oid;
	new_वृक्ष_oid = &repo_get_commit_वृक्ष(repo, original)->object.oid;

	ret = commit_वृक्ष_ext(repo, "split-out", original, शून्य, parents, old_वृक्ष_oid,
			      new_वृक्ष_oid, &second_commit, COMMIT_वृक्ष_EDIT_MESSAGE);
	अगर (ret < 0) {
		ret = error(_("failed writing second commit"));
		जाओ out;
	}

	*out = second_commit;
	ret = 0;

out:
	अगर (सूचकांक_file.len)
		unlink(सूचकांक_file.buf);
	strbuf_release(&सूचकांक_file);
	commit_list_free(parents);
	release_सूचकांक(&सूचकांक);
	वापस ret;
}

अटल पूर्णांक cmd_history_split(पूर्णांक argc,
			     स्थिर अक्षर **argv,
			     स्थिर अक्षर *prefix,
			     काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		शून्य,
	};
	क्रमागत ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	काष्ठा option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	काष्ठा commit *original, *rewritten = शून्य;
	काष्ठा strbuf reflog_संदेश = STRBUF_INIT;
	काष्ठा pathspec pathspec = { 0 };
	काष्ठा rev_info revs = { 0 };
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	अगर (argc < 1) {
		ret = error(_("command expects a committish"));
		जाओ out;
	}
	repo_config(repo, git_default_config, शून्य);

	अगर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	अगर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जाओ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	अगर (ret < 0)
		जाओ out;

	अगर (original->parents && original->parents->अग्र) {
		ret = error(_("cannot split up merge commit"));
		जाओ out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	अगर (ret < 0)
		जाओ out;

	strbuf_addf(&reflog_संदेश, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_संदेश.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	अगर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जाओ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_संदेश);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	वापस ret;
}

#घोषणा SQUASH_SEEN (1u << 11)
#घोषणा SQUASH_TIP (1u << 12)
#घोषणा SQUASH_AMEND_TARGET (1u << 13)

अटल bool is_autosquash_subject(स्थिर अक्षर *s)
{
	वापस starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

अटल bool skip_one_autosquash_prefix(स्थिर अक्षर *s, स्थिर अक्षर **out)
{
	अगर (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		जबतक (**out == ' ')
			(*out)++;
		वापस true;
	}
	वापस false;
}

अटल व्योम truncate_message_to_subject(काष्ठा strbuf *संदेश)
{
	स्थिर अक्षर *eos = म_माला(संदेश->buf, "\n\n");

	अगर (eos)
		strbuf_setlen(संदेश, eos - संदेश->buf + 1);
}

काष्ठा subject_तथ्य {
	काष्ठा strintmap subjects;
	काष्ठा strbuf subject;
	काष्ठा strbuf squash_message;
	स्थिर अक्षर *message;
	bool edit_message;
};

#घोषणा SUBJECT_तथ्य_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_message = STRBUF_INIT,	\
}

अटल व्योम subject_तथ्य_clear(काष्ठा subject_तथ्य *तथ्य)
{
	strintmap_clear(&तथ्य->subjects);
	strbuf_release(&तथ्य->subject);
	strbuf_release(&तथ्य->squash_message);
}

अटल पूर्णांक squash_amend_message(काष्ठा repository *repo,
				काष्ठा commit *commit,
				काष्ठा subject_तथ्य *तथ्य,
				अचिन्हित flags)
{
	स्थिर अक्षर *body = तथ्य->message + तथ्य->subject.len;

	जबतक (है_खाली(*body))
		body++;

	अगर (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			तथ्य->subject.buf);
		वापस 0;
	}

	अगर (तथ्य->edit_message) {
		वापस 0;
	} अन्यथा अगर (flags & SQUASH_AMEND_TARGET) {
		अगर (starts_with(तथ्य->squash_message.buf, "squash!"))
			वापस error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     तथ्य->subject.buf);
		अगर (starts_with(तथ्य->squash_message.buf, "fixup!"))
			strbuf_splice(&तथ्य->squash_message, 0, 5, "amend!", 5);
		अगर (starts_with(तथ्य->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&तथ्य->squash_message);
			strbuf_addch(&तथ्य->squash_message, '\n');
		} अन्यथा {
			strbuf_reset(&तथ्य->squash_message);
		}
		strbuf_addstr(&तथ्य->squash_message, body);
		strbuf_complete_पंक्ति(&तथ्य->squash_message);
	} अन्यथा {
		वापस error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     तथ्य->subject.buf);
	}
	वापस 0;
}

अटल पूर्णांक squash_squash_message(काष्ठा repository *repo,
				काष्ठा commit *commit,
				काष्ठा subject_तथ्य *तथ्य,
				अचिन्हित flags)
{
	स्थिर अक्षर *body = तथ्य->message + तथ्य->subject.len;

	जबतक (है_खाली(*body))
		body++;

	अगर (तथ्य->edit_message) {
		वापस 0;
	} अन्यथा अगर (flags & SQUASH_AMEND_TARGET) {
		अगर (starts_with(तथ्य->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&तथ्य->squash_message);
			strbuf_splice(&तथ्य->squash_message, 0, 5, "squash", 6);
		}
		अगर (starts_with(तथ्य->squash_message.buf, "squash!")) {
			strbuf_addch(&तथ्य->squash_message, '\n');
			strbuf_addstr(&तथ्य->squash_message, body);
			strbuf_complete_पंक्ति(&तथ्य->squash_message);
		} अन्यथा {
			वापस error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     तथ्य->subject.buf);
		}
	} अन्यथा {
		वापस error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      तथ्य->subject.buf);
	}
	वापस 0;
}

अटल पूर्णांक squash_check_can_autosquash(काष्ठा repository *repo,
				       काष्ठा commit *commit,
				       काष्ठा subject_तथ्य *तथ्य,
				       अचिन्हित flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	अगर (starts_with(तथ्य->subject.buf, "amend!"))
		वापस squash_amend_message(repo, commit, तथ्य, flags);
	अन्यथा अगर (starts_with(तथ्य->subject.buf, "squash!"))
		वापस squash_squash_message(repo, commit, तथ्य, flags);

	वापस 0;
}

अटल पूर्णांक squash_check_autosquash_subject(काष्ठा repository *repo,
					   काष्ठा commit *commit,
					   काष्ठा subject_तथ्य *तथ्य)
{
	स्थिर अक्षर* s = तथ्य->subject.buf;
	काष्ठा commit *target;
	काष्ठा hashmap_iter iter;
	काष्ठा strmap_entry *entry;
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
	जबतक (skip_one_autosquash_prefix(s, &s)) {
		अचिन्हित flags = strintmap_get(&तथ्य->subjects, s);
		अगर (flags)
			वापस squash_check_can_autosquash(repo, commit, तथ्य, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	अगर (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		अचिन्हित flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		अगर (!flags) {
			स्थिर अक्षर *subject_start;
			स्थिर अक्षर *बफर = repo_logmsg_reencode(repo, target,
								  शून्य, शून्य);
			माप_प्रकार subject_len = find_commit_subject(बफर,
								 &subject_start);
			अक्षर *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&तथ्य->subjects, subject);
			मुक्त(subject);
			repo_unuse_commit_बफर(repo, target, बफर);
		}
		अगर (flags)
			वापस squash_check_can_autosquash(repo, commit,
							   तथ्य, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&तथ्य->subjects, &iter, entry) {
		s = तथ्य->subject.buf;
		जबतक(skip_one_autosquash_prefix(s, &s)) {
			अगर (starts_with(entry->कुंजी, s)) {
				अचिन्हित मूल्य = (intptr_t)entry->मूल्य;

				वापस squash_check_can_autosquash(repo, commit,
								   तथ्य, मूल्य);
			}
		}
	}
	वापस error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       तथ्य->subject.buf);
}

अटल पूर्णांक squash_check_subject(काष्ठा repository *repo,
				काष्ठा commit *commit,
				काष्ठा subject_तथ्य *तथ्य)
{
	पूर्णांक ret = 0;
	स्थिर अक्षर *buf = repo_logmsg_reencode(repo, commit, शून्य, शून्य);
	माप_प्रकार subject_len = find_commit_subject(buf, &तथ्य->message);

	strbuf_reset(&तथ्य->subject);
	strbuf_add(&तथ्य->subject, तथ्य->message, subject_len);

	अगर (!strintmap_get_आकार(&तथ्य->subjects)) {
		स्थिर अक्षर *s;

		strbuf_addstr(&तथ्य->squash_message, तथ्य->message);
		strbuf_complete_पंक्ति(&तथ्य->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = तथ्य->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&तथ्य->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} अन्यथा अगर (is_autosquash_subject(तथ्य->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, तथ्य);
	} अन्यथा {
		strintmap_set(&तथ्य->subjects, तथ्य->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_बफर(repo, commit, buf);
	वापस ret;
}

अटल पूर्णांक build_squash_message(काष्ठा repository *repo,
				स्थिर काष्ठा strbuf *todo_buf,
				काष्ठा strbuf *out);

अटल पूर्णांक setup_squash_revisions(काष्ठा repository *repo,
				  पूर्णांक argc, स्थिर अक्षर **argv,
				  काष्ठा rev_info *revs)
{
	repo_init_revisions(repo, revs, शून्य);
	revs->reverse = 1;
	revs->topo_order = 1;
	revs->sort_order = REV_SORT_IN_GRAPH_ORDER;
	revs->simplify_history = 0;
	revs->ancestry_path = 1;
	revs->limited = 1;
	revs->ancestry_path_implicit_bottoms = 1;

	argc = setup_revisions(argc, argv, revs, शून्य);
	अगर (argc > 1)
		वापस error(_("unrecognized argument: %s"), argv[1]);

	अगर (revs->reverse != 1 || revs->topo_order != 1 ||
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
	क्रम (माप_प्रकार i = 0; i < revs->cmdline.nr; i++)
		अगर (revs->cmdline.rev[i].flags & BOTTOM)
			वापस 0;

	वापस error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
अटल पूर्णांक resolve_squash_range(काष्ठा repository *repo,
				bool update_branches,
				bool edit_message,
				पूर्णांक argc, स्थिर अक्षर **argv,
				काष्ठा commit **oldest_out,
				काष्ठा commit **tip_out,
				अक्षर **message_out)
{
	काष्ठा rev_info revs;
	काष्ठा subject_तथ्य subject_तथ्य = SUBJECT_तथ्य_INIT;
	काष्ठा commit *commit, *oldest = शून्य, *tip = शून्य;
	काष्ठा strbuf todo_buf = STRBUF_INIT;
	पूर्णांक ret, tip_गिनती = 0;
	bool walk_started = false;
	काष्ठा ref_filter filter = REF_FILTER_INIT;
	काष्ठा ref_array refs = { 0 };

	subject_तथ्य.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	अगर (ret < 0)
		जाओ out;

	अगर (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		जाओ out;
	}
	walk_started = true;
	जबतक ((commit = get_revision(&revs))) {
		काष्ठा commit_list *p;

		अगर (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		अगर (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			जाओ out;
		}
		क्रम (p = commit->parents; oldest && p; p = p->अग्र) {
			काष्ठा commit_list *q;
			काष्ठा object *o;
			bool seen;

			अगर (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				जाओ out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			क्रम (q = oldest->parents; !seen && q; q = q->अग्र) {
				अगर (p->item == q->item) {
					seen = true;
					commit_list_जोड़ो(commit, &filter.with_commit);
				}
			}
			अगर (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				जाओ out;
			}
			अगर (o->flags & SQUASH_TIP) {
				tip_गिनती--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		अगर (!oldest) {
			commit_list_जोड़ो(commit, &filter.with_commit);
			oldest = commit;
		}
		अगर (squash_check_subject(repo, commit, &subject_तथ्य)) {
			ret = -1;
			जाओ out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_गिनती++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	अगर (!tip_गिनती) {
		ret = error(_("the revision range is empty"));
		जाओ out;
	} अन्यथा अगर (tip_गिनती != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		जाओ out;
	} अन्यथा अगर (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		जाओ out;
	} अन्यथा अगर (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_जोड़ो(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	अगर (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		जाओ out;
	}
	अगर (refs.nr) {
		काष्ठा ref_format format = REF_FORMAT_INIT;
		काष्ठा ref_sorting *sorting;
		काष्ठा string_list sorting_options = STRING_LIST_INIT_DUP;
		काष्ठा strbuf branches = STRBUF_INIT;
		काष्ठा strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		अगर (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		क्रम (पूर्णांक i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			अगर (format_ref_array_item(refs.वस्तुएँ[i], &format,
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
		जाओ out;
	}
	अगर (edit_message) {
		strbuf_reset(&subject_तथ्य.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_तथ्य.squash_message);
		अगर (ret < 0)
			जाओ out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_तथ्य.squash_message, शून्य);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	अगर (walk_started)
		reset_revision_walk();
	subject_तथ्य_clear(&subject_तथ्य);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	वापस ret;
}

अटल bool amend_replaces_target(काष्ठा todo_list *todo, पूर्णांक target)
{
	क्रम (पूर्णांक i = target + 1; i < todo->nr &&
				 todo->वस्तुएँ[i].command != TODO_PICK; i++) {
		अगर (todo->वस्तुएँ[i].command == TODO_SQUASH)
			वापस false;
		अगर (todo->वस्तुएँ[i].flags & TODO_REPLACE_FIXUP_संदेश)
			वापस true;
	}
	वापस false;
}

अटल पूर्णांक build_squash_message(काष्ठा repository *repo,
				स्थिर काष्ठा strbuf *todo_buf,
				काष्ठा strbuf *out)
{
	काष्ठा todo_list todo = TODO_LIST_INIT;
	काष्ठा replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णांक nr_commits, ret;

	अगर (todo_list_parse_insn_बफर(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		जाओ out;
	}

	nr_commits = todo.nr;
	क्रम (पूर्णांक i = 0; i < nr_commits; i++) {
		काष्ठा todo_item *item = &todo.वस्तुएँ[i];
		स्थिर अक्षर *message, *body;
		माप_प्रकार commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_संदेश);
		अगर (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		अन्यथा
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, शून्य, शून्य);
		find_commit_subject(message, &body);

		अगर (skip)
			commented_len = म_माप(body);
		अन्यथा अगर (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		अन्यथा
			commented_len = 0;

		अगर (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_पंक्तियाँ(out, body, commented_len, comment_पंक्ति_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_पंक्ति(out);

		repo_unuse_commit_बफर(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	वापस ret;
}

अटल पूर्णांक cmd_history_squash(पूर्णांक argc,
			      स्थिर अक्षर **argv,
			      स्थिर अक्षर *prefix,
			      काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		शून्य,
	};
	क्रमागत ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	पूर्णांक edit = 1;
	काष्ठा option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_END(),
	};
	काष्ठा strbuf reflog_संदेश = STRBUF_INIT;
	काष्ठा commit *oldest, *tip, *rewritten;
	स्थिर काष्ठा object_id *base_वृक्ष_oid, *tip_वृक्ष_oid;
	अक्षर *message_template = शून्य;
	काष्ठा rev_info revs = { 0 };
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	अगर (argc < 2) {
		ret = error(_("command expects a revision range"));
		जाओ out;
	}
	repo_config(repo, git_default_config, शून्य);

	अगर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_संदेश, "squash: updating ");
	strbuf_join_argv(&reflog_संदेश, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	अगर (ret < 0)
		जाओ out;

	ret = setup_revwalk(repo, action, tip, &revs);
	अगर (ret < 0)
		जाओ out;

	base_वृक्ष_oid = &repo_get_commit_वृक्ष(repo,
					oldest->parents->item)->object.oid;
	tip_वृक्ष_oid = &repo_get_commit_वृक्ष(repo, tip)->object.oid;

	ret = commit_वृक्ष_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_वृक्ष_oid, tip_वृक्ष_oid,
			      &rewritten,
			      edit ? COMMIT_वृक्ष_EDIT_MESSAGE : 0);
	अगर (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		जाओ out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_संदेश.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	अगर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जाओ out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	मुक्त(message_template);
	वापस ret;
}

अटल पूर्णांक update_worktree(काष्ठा repository *repo,
			   स्थिर काष्ठा commit *old_शीर्ष,
			   स्थिर काष्ठा commit *new_शीर्ष,
			   bool dry_run)
{
	काष्ठा reset_working_वृक्ष_options opts = {
		.oid_from = &old_शीर्ष->object.oid,
		.oid = &new_शीर्ष->object.oid,
	};
	अगर (dry_run)
		opts.flags |= RESET_WORKING_वृक्ष_DRY_RUN;
	वापस reset_working_वृक्ष(repo, &opts);
}

अटल पूर्णांक find_शीर्ष_वृक्ष_change(काष्ठा repository *repo,
				 स्थिर काष्ठा replay_परिणाम *परिणाम,
				 काष्ठा commit **old_शीर्ष,
				 काष्ठा commit **new_शीर्ष,
				 bool *changed)
{
	स्थिर काष्ठा replay_ref_update *शीर्ष_update = शून्य;
	काष्ठा commit *old_शीर्ष_commit, *new_शीर्ष_commit;
	काष्ठा वृक्ष *old_शीर्ष_वृक्ष, *new_शीर्ष_वृक्ष;
	स्थिर अक्षर *शीर्ष_target;
	पूर्णांक शीर्ष_flags;

	*changed = false;

	शीर्ष_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      शून्य, &शीर्ष_flags);
	अगर (!शीर्ष_target)
		वापस error(_("cannot look up HEAD"));

	क्रम (माप_प्रकार i = 0; i < परिणाम->updates_nr; i++) {
		अगर (!म_भेद(परिणाम->updates[i].refname, शीर्ष_target)) {
			शीर्ष_update = &परिणाम->updates[i];
			अवरोध;
		}
	}

	अगर (!शीर्ष_update)
		वापस 0;

	old_शीर्ष_commit = lookup_commit_reference(repo, &शीर्ष_update->old_oid);
	new_शीर्ष_commit = lookup_commit_reference(repo, &शीर्ष_update->new_oid);
	अगर (!old_शीर्ष_commit || !new_शीर्ष_commit)
		वापस error(_("cannot resolve HEAD commit"));

	old_शीर्ष_वृक्ष = repo_get_commit_वृक्ष(repo, old_शीर्ष_commit);
	new_शीर्ष_वृक्ष = repo_get_commit_वृक्ष(repo, new_शीर्ष_commit);
	अगर (!old_शीर्ष_वृक्ष || !new_शीर्ष_वृक्ष)
		वापस error(_("cannot resolve tree for HEAD"));

	अगर (oideq(&old_शीर्ष_वृक्ष->object.oid, &new_शीर्ष_वृक्ष->object.oid))
		वापस 0;

	*old_शीर्ष = old_शीर्ष_commit;
	*new_शीर्ष = new_शीर्ष_commit;
	*changed = true;

	वापस 0;
}

अटल पूर्णांक cmd_history_drop(पूर्णांक argc,
			    स्थिर अक्षर **argv,
			    स्थिर अक्षर *prefix,
			    काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर usage[] = {
		GIT_HISTORY_DROP_USAGE,
		शून्य,
	};
	क्रमागत replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	क्रमागत ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	काष्ठा option options[] = {
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
	काष्ठा strbuf reflog_संदेश = STRBUF_INIT;
	काष्ठा commit *original, *rewritten;
	काष्ठा rev_info revs = { 0 };
	काष्ठा replay_परिणाम परिणाम = { 0 };
	काष्ठा commit *old_शीर्ष, *new_शीर्ष;
	bool शीर्ष_moves = false;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	अगर (argc != 1) {
		ret = error(_("command expects a single revision"));
		जाओ out;
	}
	repo_config(repo, git_default_config, शून्य);

	अगर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	अगर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जाओ out;
	}

	अगर (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		जाओ out;
	} अन्यथा अगर (original->parents->अग्र) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		जाओ out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	अगर (ret)
		जाओ out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &परिणाम);
	अगर (ret) {
		ret = error(_("failed replaying descendants"));
		जाओ out;
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
	अगर (!is_bare_repository(repo)) {
		ret = find_शीर्ष_वृक्ष_change(repo, &परिणाम, &old_शीर्ष,
					    &new_शीर्ष, &शीर्ष_moves);
		अगर (ret < 0)
			जाओ out;

		अगर (शीर्ष_moves && update_worktree(repo, old_शीर्ष, new_शीर्ष, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			जाओ out;
		}
	}

	strbuf_addf(&reflog_संदेश, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &परिणाम, reflog_संदेश.buf, dry_run);
	अगर (ret < 0) {
		ret = error(_("failed to update references"));
		जाओ out;
	}

	अगर (!dry_run && शीर्ष_moves && update_worktree(repo, old_शीर्ष, new_शीर्ष, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_शीर्ष->object.oid));
		जाओ out;
	}

	ret = 0;

out:
	replay_परिणाम_release(&परिणाम);
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	वापस ret;
}

पूर्णांक cmd_history(पूर्णांक argc,
		स्थिर अक्षर **argv,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo)
{
	स्थिर अक्षर * स्थिर usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_SPLIT_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		शून्य,
	};
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_split),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	वापस fn(argc, argv, prefix, repo);
}
