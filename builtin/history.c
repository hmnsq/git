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

स्थिर शून्य change_data_free(शून्य *util, स्थिरांक अक्षर *str UNUSED)
{
	संरचना wt_status_change_data *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

स्थिर पूर्णांक fill_commit_message(संरचना repository *repo,
			       स्थिरांक संरचना object_id *old_वृक्ष,
			       स्थिरांक संरचना object_id *new_वृक्ष,
			       स्थिरांक अक्षर *default_message,
			       स्थिरांक अक्षर *action,
			       संरचना strbuf *out)
{
	स्थिरांक अक्षर *path = git_path_commit_editmsg();
	स्थिरांक अक्षर *hint =
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

	s.fp = ख_खोलो(path, "w");
	जर (!s.fp)
		परत error_errno(_("could not open '%s'"), path);

	strbuf_addstr(out, default_message);
	strbuf_addch(out, '\n');
	strbuf_commented_addf(out, comment_ओळ_str, hint, action, comment_ओळ_str);
	जर (ख_डालो(out->buf, 1, out->len, s.fp) != out->len)
		die_errno(_("could not write to '%s'"), path);

	wt_status_collect_changes_trees(&s, old_वृक्ष, new_वृक्ष);
	wt_status_print(&s);
	wt_status_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_data_free);
	जर (ख_बंद(s.fp))
		die_errno(_("could not write to '%s'"), path);

	strbuf_reset(out);
	जर (launch_editor(path, out, रिक्त)) {
		ख_लिखो(मानक_त्रुटि, _("Aborting commit as launching the editor failed.\n"));
		परत -1;
	}
	strbuf_stripspace(out, comment_ओळ_str);

	cleanup_message(out, COMMIT_संदेश_CLEANUP_ALL, 0);

	जर (!out->len) {
		ख_लिखो(मानक_त्रुटि, _("Aborting commit due to empty commit message.\n"));
		परत -1;
	}

	परत 0;
}

गणना commit_वृक्ष_flags {
	COMMIT_वृक्ष_EDIT_MESSAGE = (1 << 0),
};

स्थिर पूर्णांक commit_वृक्ष_ext(संरचना repository *repo,
			   स्थिरांक अक्षर *action,
			   संरचना commit *commit_with_message,
			   स्थिरांक अक्षर *message_template,
			   स्थिरांक संरचना commit_list *parents,
			   स्थिरांक संरचना object_id *old_वृक्ष,
			   स्थिरांक संरचना object_id *new_वृक्ष,
			   संरचना commit **out,
			   गणना commit_वृक्ष_flags flags)
{
	स्थिरांक अक्षर *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		रिक्त,
	};
	स्थिरांक अक्षर *original_message, *original_body, *ptr;
	संरचना commit_extra_header *original_extra_headers = रिक्त;
	संरचना strbuf commit_message = STRBUF_INIT;
	संरचना object_id rewritten_commit_oid;
	अक्षर *original_author = रिक्त;
	माप_प्रकार len;
	पूर्णांक ret;

	/* We retain authorship of the original commit. */
	original_message = repo_logmsg_reencode(repo, commit_with_message, रिक्त, रिक्त);
	ptr = find_commit_header(original_message, "author", &len);
	जर (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_message, &original_body);

	जर (!message_template)
		message_template = original_body;

	जर (flags & COMMIT_वृक्ष_EDIT_MESSAGE) {
		ret = fill_commit_message(repo, old_वृक्ष, new_वृक्ष,
					  message_template, action, &commit_message);
		जर (ret < 0)
			जा out;
	} अन्यथा {
		strbuf_addstr(&commit_message, message_template);
	}

	original_extra_headers = read_commit_extra_headers(commit_with_message,
							   exclude_gpgsig);

	ret = commit_वृक्ष_extended(commit_message.buf, commit_message.len, new_वृक्ष,
				   parents, &rewritten_commit_oid, original_author,
				   रिक्त, रिक्त, original_extra_headers);
	जर (ret < 0)
		जा out;

	*out = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

out:
	free_commit_extra_headers(original_extra_headers);
	strbuf_release(&commit_message);
	मुक्त(original_author);
	repo_unuse_commit_buffer(repo, commit_with_message, original_message);
	परत ret;
}

स्थिर पूर्णांक first_parent_वृक्ष_oid(संरचना repository *repo,
				 संरचना commit *commit,
				 संरचना object_id *out)
{
	संरचना commit *parent = commit->parents ? commit->parents->item : रिक्त;

	जर (!parent) {
		oidcpy(out, repo->hash_algo->empty_वृक्ष);
		परत 0;
	}

	जर (repo_parse_commit(repo, parent))
		परत error(_("unable to parse parent commit %s"),
			     oid_to_hex(&parent->object.oid));

	oidcpy(out, &repo_get_commit_वृक्ष(repo, parent)->object.oid);
	परत 0;
}

स्थिर पूर्णांक commit_वृक्ष_with_edited_message(संरचना repository *repo,
					   स्थिरांक अक्षर *action,
					   संरचना commit *original,
					   संरचना commit **out)
{
	संरचना object_id parent_वृक्ष_oid;
	स्थिरांक संरचना object_id *वृक्ष_oid;

	वृक्ष_oid = &repo_get_commit_वृक्ष(repo, original)->object.oid;

	जर (first_parent_वृक्ष_oid(repo, original, &parent_वृक्ष_oid) < 0)
		परत -1;

	परत commit_वृक्ष_ext(repo, action, original, रिक्त, original->parents,
			       &parent_वृक्ष_oid, वृक्ष_oid, out, COMMIT_वृक्ष_EDIT_MESSAGE);
}

गणना ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_HEAD,
};

स्थिर पूर्णांक parse_ref_action(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *मूल्य, पूर्णांक unset)
{
	गणना ref_action *action = opt->मूल्य;

	BUG_ON_OPT_NEG_NOARG(unset, मूल्य);
	जर (!म_भेद(मूल्य, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} अन्यथा जर (!म_भेद(मूल्य, "head")) {
		*action = REF_ACTION_HEAD;
	} अन्यथा {
		परत error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	परत 0;
}

स्थिर पूर्णांक revwalk_contains_merges(संरचना repository *repo,
				   स्थिरांक संरचना strvec *revwalk_args)
{
	संरचना strvec args = STRVEC_INIT;
	संरचना rev_info revs;
	पूर्णांक ret;

	strvec_pushv(&args, revwalk_args->v);
	strvec_ढकला(&args, "--min-parents=2");

	repo_init_revisions(repo, &revs, रिक्त);

	setup_revisions_from_strvec(&args, &revs, रिक्त);
	जर (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	जर (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		जा out;
	}

	जर (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		जा out;
	}

	reset_revision_walk();
	ret = 0;

out:
	release_revisions(&revs);
	strvec_clear(&args);
	परत ret;
}

स्थिर पूर्णांक setup_revwalk(संरचना repository *repo,
			 गणना ref_action action,
			 संरचना commit *original,
			 संरचना rev_info *revs)
{
	संरचना strvec args = STRVEC_INIT;
	पूर्णांक ret;

	repo_init_revisions(repo, revs, रिक्त);
	strvec_ढकला(&args, "ignored");
	strvec_ढकला(&args, "--reverse");
	strvec_ढकला(&args, "--topo-order");
	strvec_ढकला(&args, "--full-history");

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
	जर (action == REF_ACTION_HEAD) {
		संरचना commit_list *from_list = रिक्त;
		संरचना commit *head;

		head = lookup_commit_reference_by_name("HEAD");
		जर (!head) {
			ret = error(_("cannot look up HEAD"));
			जा out;
		}

		commit_list_घाला(original, &from_list);
		ret = repo_is_descendant_of(repo, head, from_list);
		commit_list_free(from_list);

		जर (ret < 0) {
			ret = error(_("cannot determine descendance"));
			जा out;
		} अन्यथा जर (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			जा out;
		}

		strvec_ढकला(&args, "HEAD");
	} अन्यथा {
		strvec_ढकला(&args, "--branches");
		strvec_ढकला(&args, "HEAD");
	}

	ret = revwalk_contains_merges(repo, &args);
	जर (ret < 0)
		जा out;

	setup_revisions_from_strvec(&args, revs, रिक्त);
	जर (args.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

out:
	strvec_clear(&args);
	परत ret;
}

स्थिर पूर्णांक handle_ref_update(संरचना ref_transaction *transaction,
			     स्थिरांक अक्षर *refname,
			     स्थिरांक संरचना object_id *new_oid,
			     स्थिरांक संरचना object_id *old_oid,
			     स्थिरांक अक्षर *reflog_संदेश,
			     संरचना strbuf *err)
{
	जर (!transaction) {
		म_लिखो("update %s %s %s\n",
		       refname, oid_to_hex(new_oid), oid_to_hex(old_oid));
		परत 0;
	}

	परत ref_transaction_update(transaction, refname, new_oid, old_oid,
				      रिक्त, रिक्त, 0, reflog_संदेश, err);
}

स्थिर पूर्णांक compute_pending_ref_updates(संरचना rev_info *revs,
				       गणना ref_action action,
				       संरचना commit *original,
				       संरचना commit *rewritten,
				       गणना replay_empty_commit_action empty,
				       संरचना replay_result *result)
{
	स्थिरांक संरचना name_decoration *decoration;
	संरचना replay_revisions_options opts = {
		.empty = empty,
	};
	अक्षर hex[GIT_MAX_HEXSZ + 1];
	bool detached_head;
	पूर्णांक head_flags = 0;
	पूर्णांक ret;

	refs_read_ref_full(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, रिक्त, &head_flags);
	detached_head = !(head_flags & REF_ISSYMREF);

	opts.onto = oid_to_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, result);
	जर (ret)
		परत ret;

	जर (action != REF_ACTION_BRANCHES && action != REF_ACTION_HEAD)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	साठी (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->next)
	{
		जर (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_HEAD)
			सुरूठेवा;

		जर (action == REF_ACTION_HEAD &&
		    decoration->type != DECORATION_REF_HEAD)
			सुरूठेवा;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		जर (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_HEAD &&
		    !detached_head)
			सुरूठेवा;

		replay_result_queue_update(result, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	परत 0;
}

स्थिर पूर्णांक apply_pending_ref_updates(संरचना repository *repo,
				     स्थिरांक संरचना replay_result *result,
				     स्थिरांक अक्षर *reflog_संदेश,
				     पूर्णांक dry_run)
{
	संरचना ref_transaction *transaction = रिक्त;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णांक ret;

	जर (!dry_run) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		जर (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			जा out;
		}
	}

	साठी (माप_प्रकार i = 0; i < result->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					result->updates[i].refname,
					&result->updates[i].new_oid,
					&result->updates[i].old_oid,
					reflog_संदेश, &err);
		जर (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    result->updates[i].refname, err.buf);
			जा out;
		}
	}

	जर (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		जा out;
	}

	ret = 0;

out:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	परत ret;
}

स्थिर पूर्णांक handle_reference_updates(संरचना rev_info *revs,
				    गणना ref_action action,
				    संरचना commit *original,
				    संरचना commit *rewritten,
				    स्थिरांक अक्षर *reflog_संदेश,
				    पूर्णांक dry_run,
				    गणना replay_empty_commit_action empty)
{
	संरचना replay_result result = { 0 };
	पूर्णांक ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  empty, &result);
	जर (ret)
		जा out;

	ret = apply_pending_ref_updates(revs->repo, &result, reflog_संदेश, dry_run);

out:
	replay_result_release(&result);
	परत ret;
}

स्थिर पूर्णांक commit_became_empty(संरचना repository *repo,
			       संरचना commit *original,
			       संरचना वृक्ष *result)
{
	संरचना object_id parent_वृक्ष_oid;

	जर (first_parent_वृक्ष_oid(repo, original, &parent_वृक्ष_oid) < 0)
		परत -1;

	परत oideq(&result->object.oid, &parent_वृक्ष_oid);
}

स्थिर पूर्णांक parse_opt_empty(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	गणना replay_empty_commit_action *मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	जर (!म_भेद(arg, "drop"))
		*मूल्य = REPLAY_EMPTY_COMMIT_DROP;
	अन्यथा जर (!म_भेद(arg, "keep"))
		*मूल्य = REPLAY_EMPTY_COMMIT_KEEP;
	अन्यथा जर (!म_भेद(arg, "abort"))
		*मूल्य = REPLAY_EMPTY_COMMIT_ABORT;
	अन्यथा
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	परत 0;
}

स्थिर पूर्णांक cmd_history_fixup(पूर्णांक argc,
			     स्थिरांक अक्षर **argv,
			     स्थिरांक अक्षर *prefix,
			     संरचना repository *repo)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		रिक्त,
	};
	गणना replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	गणना ref_action action = REF_ACTION_DEFAULT;
	गणना commit_वृक्ष_flags flags = 0;
	पूर्णांक dry_run = 0;
	संरचना option options[] = {
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
	संरचना merge_result merge_result = { 0 };
	संरचना merge_options merge_opts = { 0 };
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	संरचना commit *head_commit, *original, *rewritten;
	संरचना वृक्ष *head_वृक्ष, *original_वृक्ष, *index_वृक्ष;
	संरचना rev_info revs = { 0 };
	bool skip_commit = false;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जर (argc != 1) {
		ret = error(_("command expects a single revision"));
		जा out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	जर (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		जा out;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	जर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जा out;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	head_commit = lookup_commit_reference_by_name("HEAD");
	जर (!head_commit) {
		ret = error(_("cannot look up HEAD"));
		जा out;
	}

	head_वृक्ष = repo_get_commit_वृक्ष(repo, head_commit);
	जर (!head_वृक्ष) {
		ret = error(_("cannot get tree for HEAD"));
		जा out;
	}

	जर (repo_read_index(repo) < 0) {
		ret = error(_("unable to read index"));
		जा out;
	}

	जर (!repo_index_has_changes(repo, head_वृक्ष, रिक्त)) {
		ret = error(_("nothing to fixup: no staged changes"));
		जा out;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	index_वृक्ष = write_in_core_index_as_वृक्ष(repo, repo->index);
	जर (!index_वृक्ष) {
		ret = error(_("unable to write index as a tree"));
		जा out;
	}

	original_वृक्ष = repo_get_commit_वृक्ष(repo, original);
	जर (!original_वृक्ष) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		जा out;
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
	merge_incore_nonrecursive(&merge_opts, head_वृक्ष,
				  original_वृक्ष, index_वृक्ष, &merge_result);

	जर (merge_result.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		जा out;
	}

	जर (!merge_result.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		जा out;
	}

	ret = commit_became_empty(repo, original, merge_result.वृक्ष);
	जर (ret < 0)
		जा out;
	जर (ret > 0) {
		निवड (empty) {
		स्थिती REPLAY_EMPTY_COMMIT_DROP:
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
			जर (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				जा out;
			}

			skip_commit = true;
			थांबा;
		स्थिती REPLAY_EMPTY_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			थांबा;
		स्थिती REPLAY_EMPTY_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			जा out;
		}
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जर (ret)
		जा out;

	जर (!skip_commit) {
		ret = commit_वृक्ष_ext(repo, "fixup", original, रिक्त, original->parents,
				      &original_वृक्ष->object.oid, &merge_result.वृक्ष->object.oid,
				      &rewritten, flags);
		जर (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			जा out;
		}
	}

	strbuf_addf(&reflog_संदेश, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_संदेश.buf, dry_run, empty);
	जर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जा out;
	}

	ret = 0;

out:
	merge_finalize(&merge_opts, &merge_result);
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	परत ret;
}

स्थिर पूर्णांक cmd_history_reword(पूर्णांक argc,
			      स्थिरांक अक्षर **argv,
			      स्थिरांक अक्षर *prefix,
			      संरचना repository *repo)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	संरचना commit *original, *rewritten;
	संरचना rev_info revs = { 0 };
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जर (argc != 1) {
		ret = error(_("command expects a single revision"));
		जा out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	जर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जा out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जर (ret)
		जा out;

	ret = commit_वृक्ष_with_edited_message(repo, "reworded", original, &rewritten);
	जर (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		जा out;
	}

	strbuf_addf(&reflog_संदेश, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_संदेश.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	जर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जा out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	परत ret;
}

स्थिर पूर्णांक write_ondisk_index(संरचना repository *repo,
			      संरचना object_id *oid,
			      स्थिरांक अक्षर *path)
{
	संरचना unpack_trees_options opts = { 0 };
	संरचना lock_file lock = LOCK_INIT;
	संरचना वृक्ष_desc वृक्ष_desc;
	संरचना index_state index;
	संरचना वृक्ष *वृक्ष;
	पूर्णांक ret;

	index_state_init(&index, repo);

	opts.head_idx = -1;
	opts.src_index = &index;
	opts.dst_index = &index;

	वृक्ष = repo_parse_वृक्ष_indirect(repo, oid);
	जर (!वृक्ष) {
		ret = error(_("unable to parse tree %s"), oid_to_hex(oid));
		जा out;
	}
	init_वृक्ष_desc(&वृक्ष_desc, &वृक्ष->object.oid, वृक्ष->buffer, वृक्ष->size);

	जर (unpack_trees(1, &वृक्ष_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		जा out;
	}

	मूळसंख्या_cache_वृक्ष(repo, &index, वृक्ष);

	जर (repo_hold_lock_file_for_update(repo, &lock, path, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		जा out;
	}

	जर (write_locked_index(&index, &lock, COMMIT_LOCK)) {
		ret = error(_("unable to write new index file"));
		जा out;
	}

	ret = 0;

out:
	rollback_lock_file(&lock);
	release_index(&index);
	परत ret;
}

स्थिर पूर्णांक split_commit(संरचना repository *repo,
			संरचना commit *original,
			संरचना pathspec *pathspec,
			संरचना commit **out)
{
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना strbuf index_file = STRBUF_INIT;
	संरचना index_state index = INDEX_STATE_INIT(repo);
	स्थिरांक संरचना object_id *original_commit_वृक्ष_oid;
	स्थिरांक संरचना object_id *old_वृक्ष_oid, *new_वृक्ष_oid;
	संरचना object_id parent_वृक्ष_oid;
	अक्षर original_commit_oid[GIT_MAX_HEXSZ + 1];
	संरचना commit *first_commit, *second_commit;
	संरचना commit_list *parents = रिक्त;
	संरचना वृक्ष *split_वृक्ष;
	पूर्णांक ret;

	जर (first_parent_वृक्ष_oid(repo, original, &parent_वृक्ष_oid) < 0) {
		ret = -1;
		जा out;
	}
	original_commit_वृक्ष_oid = get_commit_वृक्ष_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_path_replace(repo, &index_file, "%s", "history-split.index");

	ret = write_ondisk_index(repo, &parent_वृक्ष_oid, index_file.buf);
	जर (ret < 0)
		जा out;

	ret = read_index_from(&index, index_file.buf, repo->gitdir);
	जर (ret < 0) {
		ret = error(_("failed reading temporary index"));
		जा out;
	}

	oid_to_hex_r(original_commit_oid, &original->object.oid);
	ret = run_add_p_index(repo, &index, index_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, ADD_P_DISALLOW_EDIT);
	जर (ret < 0)
		जा out;

	split_वृक्ष = write_in_core_index_as_वृक्ष(repo, &index);
	जर (!split_वृक्ष) {
		ret = error(_("failed split tree"));
		जा out;
	}

	unlink(index_file.buf);
	strbuf_release(&index_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	जर (oideq(&split_वृक्ष->object.oid, &parent_वृक्ष_oid)) {
		ret = error(_("split commit is empty"));
		जा out;
	} अन्यथा जर (oideq(&split_वृक्ष->object.oid, original_commit_वृक्ष_oid)) {
		ret = error(_("split commit tree matches original commit"));
		जा out;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_वृक्ष_ext(repo, "split-out", original, रिक्त, original->parents, &parent_वृक्ष_oid,
			      &split_वृक्ष->object.oid, &first_commit, COMMIT_वृक्ष_EDIT_MESSAGE);
	जर (ret < 0) {
		ret = error(_("failed writing first commit"));
		जा out;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(first_commit, &parents);

	old_वृक्ष_oid = &repo_get_commit_वृक्ष(repo, first_commit)->object.oid;
	new_वृक्ष_oid = &repo_get_commit_वृक्ष(repo, original)->object.oid;

	ret = commit_वृक्ष_ext(repo, "split-out", original, रिक्त, parents, old_वृक्ष_oid,
			      new_वृक्ष_oid, &second_commit, COMMIT_वृक्ष_EDIT_MESSAGE);
	जर (ret < 0) {
		ret = error(_("failed writing second commit"));
		जा out;
	}

	*out = second_commit;
	ret = 0;

out:
	जर (index_file.len)
		unlink(index_file.buf);
	strbuf_release(&index_file);
	commit_list_free(parents);
	release_index(&index);
	परत ret;
}

स्थिर पूर्णांक cmd_history_split(पूर्णांक argc,
			     स्थिरांक अक्षर **argv,
			     स्थिरांक अक्षर *prefix,
			     संरचना repository *repo)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
		GIT_HISTORY_SPLIT_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_run,
			 N_("perform a dry-run without updating any refs")),
		OPT_END(),
	};
	संरचना commit *original, *rewritten = रिक्त;
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना rev_info revs = { 0 };
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जर (argc < 1) {
		ret = error(_("command expects a committish"));
		जा out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_FULL |
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_PREFIX_ORIGIN,
		       prefix, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	जर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जा out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जर (ret < 0)
		जा out;

	जर (original->parents && original->parents->next) {
		ret = error(_("cannot split up merge commit"));
		जा out;
	}

	ret = split_commit(repo, original, &pathspec, &rewritten);
	जर (ret < 0)
		जा out;

	strbuf_addf(&reflog_संदेश, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_संदेश.buf, dry_run, REPLAY_EMPTY_COMMIT_ABORT);
	जर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जा out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_संदेश);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	परत ret;
}

#define SQUASH_SEEN (1u << 11)
#define SQUASH_TIP (1u << 12)
#define SQUASH_AMEND_TARGET (1u << 13)

स्थिर bool is_autosquash_subject(स्थिरांक अक्षर *s)
{
	परत starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

स्थिर bool skip_one_autosquash_prefix(स्थिरांक अक्षर *s, स्थिरांक अक्षर **out)
{
	जर (skip_prefix(s, "amend!", out) || skip_prefix(s, "fixup!", out) ||
	    skip_prefix(s, "squash!", out)) {
		पर्यंत (**out == ' ')
			(*out)++;
		परत true;
	}
	परत false;
}

स्थिर शून्य truncate_message_to_subject(संरचना strbuf *संदेश)
{
	स्थिरांक अक्षर *eos = म_माला(संदेश->buf, "\n\n");

	जर (eos)
		strbuf_setlen(संदेश, eos - संदेश->buf + 1);
}

संरचना subject_data {
	संरचना strintmap subjects;
	संरचना strbuf subject;
	संरचना strbuf squash_message;
	स्थिरांक अक्षर *message;
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

स्थिर पूर्णांक squash_amend_message(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_data *data,
				अचिन्हित flags)
{
	स्थिरांक अक्षर *body = data->message + data->subject.len;

	पर्यंत (है_खाली(*body))
		body++;

	जर (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			data->subject.buf);
		परत 0;
	}

	जर (data->edit_message) {
		परत 0;
	} अन्यथा जर (flags & SQUASH_AMEND_TARGET) {
		जर (starts_with(data->squash_message.buf, "squash!"))
			परत error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		जर (starts_with(data->squash_message.buf, "fixup!"))
			strbuf_splice(&data->squash_message, 0, 5, "amend!", 5);
		जर (starts_with(data->squash_message.buf, "amend!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_addch(&data->squash_message, '\n');
		} अन्यथा {
			strbuf_reset(&data->squash_message);
		}
		strbuf_addstr(&data->squash_message, body);
		strbuf_complete_ओळ(&data->squash_message);
	} अन्यथा {
		परत error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     data->subject.buf);
	}
	परत 0;
}

स्थिर पूर्णांक squash_squash_message(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_data *data,
				अचिन्हित flags)
{
	स्थिरांक अक्षर *body = data->message + data->subject.len;

	पर्यंत (है_खाली(*body))
		body++;

	जर (data->edit_message) {
		परत 0;
	} अन्यथा जर (flags & SQUASH_AMEND_TARGET) {
		जर (starts_with(data->squash_message.buf, "fixup!")) {
			truncate_message_to_subject(&data->squash_message);
			strbuf_splice(&data->squash_message, 0, 5, "squash", 6);
		}
		जर (starts_with(data->squash_message.buf, "squash!")) {
			strbuf_addch(&data->squash_message, '\n');
			strbuf_addstr(&data->squash_message, body);
			strbuf_complete_ओळ(&data->squash_message);
		} अन्यथा {
			परत error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     data->subject.buf);
		}
	} अन्यथा {
		परत error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      data->subject.buf);
	}
	परत 0;
}

स्थिर पूर्णांक squash_check_can_autosquash(संरचना repository *repo,
				       संरचना commit *commit,
				       संरचना subject_data *data,
				       अचिन्हित flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	जर (starts_with(data->subject.buf, "amend!"))
		परत squash_amend_message(repo, commit, data, flags);
	अन्यथा जर (starts_with(data->subject.buf, "squash!"))
		परत squash_squash_message(repo, commit, data, flags);

	परत 0;
}

स्थिर पूर्णांक squash_check_autosquash_subject(संरचना repository *repo,
					   संरचना commit *commit,
					   संरचना subject_data *data)
{
	स्थिरांक अक्षर* s = data->subject.buf;
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
	पर्यंत (skip_one_autosquash_prefix(s, &s)) {
		अचिन्हित flags = strintmap_get(&data->subjects, s);
		जर (flags)
			परत squash_check_can_autosquash(repo, commit, data, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	जर (target && starts_with(oid_to_hex(&target->object.oid), s)) {
		अचिन्हित flags =
			target->object.flags & (SQUASH_SEEN | SQUASH_AMEND_TARGET);
		जर (!flags) {
			स्थिरांक अक्षर *subject_start;
			स्थिरांक अक्षर *buffer = repo_logmsg_reencode(repo, target,
								  रिक्त, रिक्त);
			माप_प्रकार subject_len = find_commit_subject(buffer,
								 &subject_start);
			अक्षर *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&data->subjects, subject);
			मुक्त(subject);
			repo_unuse_commit_buffer(repo, target, buffer);
		}
		जर (flags)
			परत squash_check_can_autosquash(repo, commit,
							   data, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_entry(&data->subjects, &iter, entry) {
		s = data->subject.buf;
		पर्यंत(skip_one_autosquash_prefix(s, &s)) {
			जर (starts_with(entry->किल्ली, s)) {
				अचिन्हित मूल्य = (intptr_t)entry->मूल्य;

				परत squash_check_can_autosquash(repo, commit,
								   data, मूल्य);
			}
		}
	}
	परत error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       data->subject.buf);
}

स्थिर पूर्णांक squash_check_subject(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_data *data)
{
	पूर्णांक ret = 0;
	स्थिरांक अक्षर *buf = repo_logmsg_reencode(repo, commit, रिक्त, रिक्त);
	माप_प्रकार subject_len = find_commit_subject(buf, &data->message);

	strbuf_reset(&data->subject);
	strbuf_add(&data->subject, data->message, subject_len);

	जर (!strintmap_get_size(&data->subjects)) {
		स्थिरांक अक्षर *s;

		strbuf_addstr(&data->squash_message, data->message);
		strbuf_complete_ओळ(&data->squash_message);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = data->subject.buf;
		skip_one_autosquash_prefix(s, &s);
		strintmap_set(&data->subjects, s, SQUASH_AMEND_TARGET | SQUASH_SEEN);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} अन्यथा जर (is_autosquash_subject(data->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, data);
	} अन्यथा {
		strintmap_set(&data->subjects, data->subject.buf, SQUASH_SEEN);
	}
	repo_unuse_commit_buffer(repo, commit, buf);
	परत ret;
}

स्थिर पूर्णांक build_squash_message(संरचना repository *repo,
				स्थिरांक संरचना strbuf *todo_buf,
				संरचना strbuf *out);

स्थिर पूर्णांक setup_squash_revisions(संरचना repository *repo,
				  पूर्णांक argc, स्थिरांक अक्षर **argv,
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
	जर (argc > 1)
		परत error(_("unrecognized argument: %s"), argv[1]);

	जर (revs->reverse != 1 || revs->topo_order != 1 ||
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
	साठी (माप_प्रकार i = 0; i < revs->cmdline.nr; i++)
		जर (revs->cmdline.rev[i].flags & BOTTOM)
			परत 0;

	परत error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
स्थिर पूर्णांक resolve_squash_range(संरचना repository *repo,
				bool update_branches,
				bool edit_message,
				पूर्णांक argc, स्थिरांक अक्षर **argv,
				संरचना commit **oldest_out,
				संरचना commit **tip_out,
				अक्षर **message_out)
{
	संरचना rev_info revs;
	संरचना subject_data subject_data = SUBJECT_DATA_INIT;
	संरचना commit *commit, *oldest = रिक्त, *tip = रिक्त;
	संरचना strbuf todo_buf = STRBUF_INIT;
	पूर्णांक ret, tip_मोजणी = 0;
	bool walk_started = false;
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_array refs = { 0 };

	subject_data.edit_message = edit_message;
	ret = setup_squash_revisions(repo, argc, argv, &revs);
	जर (ret < 0)
		जा out;

	जर (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		जा out;
	}
	walk_started = true;
	पर्यंत ((commit = get_revision(&revs))) {
		संरचना commit_list *p;

		जर (edit_message)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_to_hex(&commit->object.oid));

		जर (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			जा out;
		}
		साठी (p = commit->parents; oldest && p; p = p->next) {
			संरचना commit_list *q;
			संरचना object *o;
			bool seen;

			जर (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				जा out;
			}
			o = &p->item->object;
			seen = o->flags & SQUASH_SEEN;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			साठी (q = oldest->parents; !seen && q; q = q->next) {
				जर (p->item == q->item) {
					seen = true;
					commit_list_घाला(commit, &filter.with_commit);
				}
			}
			जर (!seen) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				जा out;
			}
			जर (o->flags & SQUASH_TIP) {
				tip_मोजणी--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		जर (!oldest) {
			commit_list_घाला(commit, &filter.with_commit);
			oldest = commit;
		}
		जर (squash_check_subject(repo, commit, &subject_data)) {
			ret = -1;
			जा out;
		}
		tip = commit;
		tip->object.flags |= SQUASH_SEEN | SQUASH_TIP;
		tip_मोजणी++;
	}
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	जर (!tip_मोजणी) {
		ret = error(_("the revision range is empty"));
		जा out;
	} अन्यथा जर (tip_मोजणी != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		जा out;
	} अन्यथा जर (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		जा out;
	} अन्यथा जर (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_घाला(tip, &filter.no_commit);
	filter.kind = FILTER_REFS_BRANCHES;
	जर (update_branches &&
	    filter_refs(&refs, &filter, filter.kind)) {
		ret = error(_("could not filter refs"));
		जा out;
	}
	जर (refs.nr) {
		संरचना ref_format format = REF_FORMAT_INIT;
		संरचना ref_sorting *sorting;
		संरचना string_list sorting_options = STRING_LIST_INIT_DUP;
		संरचना strbuf branches = STRBUF_INIT;
		संरचना strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		जर (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&sorting_options, "refname");
		sorting = ref_sorting_options(&sorting_options);
		ref_array_sort(sorting, &refs);
		साठी (पूर्णांक i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			जर (format_ref_array_item(refs.वस्तुसंच[i], &format,
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
		जा out;
	}
	जर (edit_message) {
		strbuf_reset(&subject_data.squash_message);
		ret = build_squash_message(repo, &todo_buf,
					   &subject_data.squash_message);
		जर (ret < 0)
			जा out;
	}

	*oldest_out = oldest;
	*tip_out = tip;
	*message_out = strbuf_detach(&subject_data.squash_message, रिक्त);
	ret = 0;

out:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_SEEN | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	जर (walk_started)
		reset_revision_walk();
	subject_data_clear(&subject_data);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	परत ret;
}

स्थिर bool amend_replaces_target(संरचना todo_list *todo, पूर्णांक target)
{
	साठी (पूर्णांक i = target + 1; i < todo->nr &&
				 todo->वस्तुसंच[i].command != TODO_PICK; i++) {
		जर (todo->वस्तुसंच[i].command == TODO_SQUASH)
			परत false;
		जर (todo->वस्तुसंच[i].flags & TODO_REPLACE_FIXUP_संदेश)
			परत true;
	}
	परत false;
}

स्थिर पूर्णांक build_squash_message(संरचना repository *repo,
				स्थिरांक संरचना strbuf *todo_buf,
				संरचना strbuf *out)
{
	संरचना todo_list todo = TODO_LIST_INIT;
	संरचना replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णांक nr_commits, ret;

	जर (todo_list_parse_insn_buffer(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		जा out;
	}

	nr_commits = todo.nr;
	साठी (पूर्णांक i = 0; i < nr_commits; i++) {
		संरचना todo_item *item = &todo.वस्तुसंच[i];
		स्थिरांक अक्षर *message, *body;
		माप_प्रकार commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_संदेश);
		जर (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		अन्यथा
			skip = !squashing;

		message = repo_logmsg_reencode(repo, item->commit, रिक्त, रिक्त);
		find_commit_subject(message, &body);

		जर (skip)
			commented_len = म_माप(body);
		अन्यथा जर (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		अन्यथा
			commented_len = 0;

		जर (!i)
			add_squash_combination_header(out, nr_commits);
		strbuf_addch(out, '\n');
		add_squash_message_header(out, i + 1, skip);
		strbuf_addstr(out, "\n\n");
		strbuf_add_commented_ओळी(out, body, commented_len, comment_ओळ_str);
		strbuf_addstr(out, body + commented_len);
		strbuf_complete_ओळ(out);

		repo_unuse_commit_buffer(repo, item->commit, message);
	}

	ret = 0;

out:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	परत ret;
}

स्थिर पूर्णांक cmd_history_squash(पूर्णांक argc,
			      स्थिरांक अक्षर **argv,
			      स्थिरांक अक्षर *prefix,
			      संरचना repository *repo)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
	पूर्णांक edit = 1;
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
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	संरचना commit *oldest, *tip, *rewritten;
	स्थिरांक संरचना object_id *base_वृक्ष_oid, *tip_वृक्ष_oid;
	अक्षर *message_template = रिक्त;
	संरचना rev_info revs = { 0 };
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	जर (argc < 2) {
		ret = error(_("command expects a revision range"));
		जा out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_संदेश, "squash: updating ");
	strbuf_join_argv(&reflog_संदेश, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &message_template);
	जर (ret < 0)
		जा out;

	ret = setup_revwalk(repo, action, tip, &revs);
	जर (ret < 0)
		जा out;

	base_वृक्ष_oid = &repo_get_commit_वृक्ष(repo,
					oldest->parents->item)->object.oid;
	tip_वृक्ष_oid = &repo_get_commit_वृक्ष(repo, tip)->object.oid;

	ret = commit_वृक्ष_ext(repo, "squash", oldest, message_template,
			      oldest->parents, base_वृक्ष_oid, tip_वृक्ष_oid,
			      &rewritten,
			      edit ? COMMIT_वृक्ष_EDIT_MESSAGE : 0);
	जर (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		जा out;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_संदेश.buf, dry_run,
				       REPLAY_EMPTY_COMMIT_ABORT);
	जर (ret < 0) {
		ret = error(_("failed replaying descendants"));
		जा out;
	}

	ret = 0;

out:
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	मुक्त(message_template);
	परत ret;
}

स्थिर पूर्णांक update_worktree(संरचना repository *repo,
			   स्थिरांक संरचना commit *old_head,
			   स्थिरांक संरचना commit *new_head,
			   bool dry_run)
{
	संरचना reset_working_वृक्ष_options opts = {
		.oid_from = &old_head->object.oid,
		.oid = &new_head->object.oid,
	};
	जर (dry_run)
		opts.flags |= RESET_WORKING_वृक्ष_DRY_RUN;
	परत reset_working_वृक्ष(repo, &opts);
}

स्थिर पूर्णांक find_head_वृक्ष_change(संरचना repository *repo,
				 स्थिरांक संरचना replay_result *result,
				 संरचना commit **old_head,
				 संरचना commit **new_head,
				 bool *changed)
{
	स्थिरांक संरचना replay_ref_update *head_update = रिक्त;
	संरचना commit *old_head_commit, *new_head_commit;
	संरचना वृक्ष *old_head_वृक्ष, *new_head_वृक्ष;
	स्थिरांक अक्षर *head_target;
	पूर्णांक head_flags;

	*changed = false;

	head_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      रिक्त, &head_flags);
	जर (!head_target)
		परत error(_("cannot look up HEAD"));

	साठी (माप_प्रकार i = 0; i < result->updates_nr; i++) {
		जर (!म_भेद(result->updates[i].refname, head_target)) {
			head_update = &result->updates[i];
			थांबा;
		}
	}

	जर (!head_update)
		परत 0;

	old_head_commit = lookup_commit_reference(repo, &head_update->old_oid);
	new_head_commit = lookup_commit_reference(repo, &head_update->new_oid);
	जर (!old_head_commit || !new_head_commit)
		परत error(_("cannot resolve HEAD commit"));

	old_head_वृक्ष = repo_get_commit_वृक्ष(repo, old_head_commit);
	new_head_वृक्ष = repo_get_commit_वृक्ष(repo, new_head_commit);
	जर (!old_head_वृक्ष || !new_head_वृक्ष)
		परत error(_("cannot resolve tree for HEAD"));

	जर (oideq(&old_head_वृक्ष->object.oid, &new_head_वृक्ष->object.oid))
		परत 0;

	*old_head = old_head_commit;
	*new_head = new_head_commit;
	*changed = true;

	परत 0;
}

स्थिर पूर्णांक cmd_history_drop(पूर्णांक argc,
			    स्थिरांक अक्षर **argv,
			    स्थिरांक अक्षर *prefix,
			    संरचना repository *repo)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
		GIT_HISTORY_DROP_USAGE,
		रिक्त,
	};
	गणना replay_empty_commit_action empty = REPLAY_EMPTY_COMMIT_DROP;
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णांक dry_run = 0;
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
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	संरचना commit *original, *rewritten;
	संरचना rev_info revs = { 0 };
	संरचना replay_result result = { 0 };
	संरचना commit *old_head, *new_head;
	bool head_moves = false;
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	जर (argc != 1) {
		ret = error(_("command expects a single revision"));
		जा out;
	}
	repo_config(repo, git_default_config, रिक्त);

	जर (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	जर (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		जा out;
	}

	जर (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		जा out;
	} अन्यथा जर (original->parents->next) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		जा out;
	}

	ret = setup_revwalk(repo, action, original, &revs);
	जर (ret)
		जा out;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  empty, &result);
	जर (ret) {
		ret = error(_("failed replaying descendants"));
		जा out;
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
	जर (!is_bare_repository(repo)) {
		ret = find_head_वृक्ष_change(repo, &result, &old_head,
					    &new_head, &head_moves);
		जर (ret < 0)
			जा out;

		जर (head_moves && update_worktree(repo, old_head, new_head, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			जा out;
		}
	}

	strbuf_addf(&reflog_संदेश, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &result, reflog_संदेश.buf, dry_run);
	जर (ret < 0) {
		ret = error(_("failed to update references"));
		जा out;
	}

	जर (!dry_run && head_moves && update_worktree(repo, old_head, new_head, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_to_hex(&new_head->object.oid));
		जा out;
	}

	ret = 0;

out:
	replay_result_release(&result);
	strbuf_release(&reflog_संदेश);
	release_revisions(&revs);
	परत ret;
}

पूर्णांक cmd_history(पूर्णांक argc,
		स्थिरांक अक्षर **argv,
		स्थिरांक अक्षर *prefix,
		संरचना repository *repo)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
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
	परत fn(argc, argv, prefix, repo);
}
