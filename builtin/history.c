#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

#परिभाषय GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#परिभाषय GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#परिभाषय GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#परिभाषय GIT_HISTORY_विभज_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#परिभाषय GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

स्थिर शून्य change_दत्त_free(शून्य *util, ध्रुव वर्ण *str अप्रयुक्तम्)
{
	संरचना wt_स्थितिः_change_दत्त *d = util;
	मुक्त(d->rename_source);
	मुक्त(d);
}

स्थिर पूर्णाङ्क fill_commit_सन्देशः(संरचना repository *repo,
			       ध्रुव संरचना object_id *old_वृक्षः,
			       ध्रुव संरचना object_id *new_वृक्षः,
			       ध्रुव वर्ण *default_सन्देशः,
			       ध्रुव वर्ण *action,
			       संरचना strbuf *बहिर्गमनम्)
{
	ध्रुव वर्ण *मार्गः = git_मार्गः_commit_editmsg();
	ध्रुव वर्ण *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	संरचना wt_स्थितिः s;

	wt_स्थितिः_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_शीघ्र;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_उपसर्गः = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(मार्गः, "w");
	यदि (!s.fp)
		प्रत्यागम error_errno(_("could not open '%s'"), मार्गः);

	strbuf_addstr(बहिर्गमनम्, default_सन्देशः);
	strbuf_addch(बहिर्गमनम्, '\n');
	strbuf_commented_addf(बहिर्गमनम्, comment_पङ्क्तिः_str, hint, action, comment_पङ्क्तिः_str);
	यदि (fwrite(बहिर्गमनम्->buf, 1, बहिर्गमनम्->len, s.fp) != बहिर्गमनम्->len)
		die_errno(_("could not write to '%s'"), मार्गः);

	wt_स्थितिः_collect_changes_trees(&s, old_वृक्षः, new_वृक्षः);
	wt_स्थितिः_print(&s);
	wt_स्थितिः_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_दत्त_free);
	यदि (fclose(s.fp))
		die_errno(_("could not write to '%s'"), मार्गः);

	strbuf_reset(बहिर्गमनम्);
	यदि (launch_editor(मार्गः, बहिर्गमनम्, रिक्त)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		प्रत्यागम -1;
	}
	strbuf_stripspace(बहिर्गमनम्, comment_पङ्क्तिः_str);

	cleanup_सन्देशः(बहिर्गमनम्, COMMIT_सन्देशः_CLEANUP_ALL, 0);

	यदि (!बहिर्गमनम्->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		प्रत्यागम -1;
	}

	प्रत्यागम 0;
}

गणना commit_वृक्षः_flags {
	COMMIT_वृक्षः_EDIT_सन्देशः = (1 << 0),
};

स्थिर पूर्णाङ्क commit_वृक्षः_ext(संरचना repository *repo,
			   ध्रुव वर्ण *action,
			   संरचना commit *commit_with_सन्देशः,
			   ध्रुव वर्ण *सन्देशः_template,
			   ध्रुव संरचना commit_list *parents,
			   ध्रुव संरचना object_id *old_वृक्षः,
			   ध्रुव संरचना object_id *new_वृक्षः,
			   संरचना commit **बहिर्गमनम्,
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
	ध्रुव वर्ण *original_सन्देशः, *original_body, *ptr;
	संरचना commit_extra_शीर्षकम् *original_extra_शीर्षकाणि = रिक्त;
	संरचना strbuf commit_सन्देशः_2 = STRBUF_INIT;
	संरचना object_id rewritten_commit_oid;
	वर्ण *original_author = रिक्त;
	आकार_t len;
	पूर्णाङ्क ret;

	/* We retain authorship of the original commit. */
	original_सन्देशः = repo_logmsg_reencode(repo, commit_with_सन्देशः, रिक्त, रिक्त);
	ptr = find_commit_शीर्षकम्(original_सन्देशः, "author", &len);
	यदि (ptr)
		original_author = xmemdupz(ptr, len);
	find_commit_subject(original_सन्देशः, &original_body);

	यदि (!सन्देशः_template)
		सन्देशः_template = original_body;

	यदि (flags & COMMIT_वृक्षः_EDIT_सन्देशः) {
		ret = fill_commit_सन्देशः(repo, old_वृक्षः, new_वृक्षः,
					  सन्देशः_template, action, &commit_सन्देशः_2);
		यदि (ret < 0)
			गच्छ बहिर्गमनम्;
	} अन्यथा {
		strbuf_addstr(&commit_सन्देशः_2, सन्देशः_template);
	}

	original_extra_शीर्षकाणि = read_commit_extra_शीर्षकाणि(commit_with_सन्देशः,
							   exclude_gpgsig);

	ret = commit_वृक्षः_extended(commit_सन्देशः_2.buf, commit_सन्देशः_2.len, new_वृक्षः,
				   parents, &rewritten_commit_oid, original_author,
				   रिक्त, रिक्त, original_extra_शीर्षकाणि);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	*बहिर्गमनम् = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

बहिर्गमनम्:
	free_commit_extra_शीर्षकाणि(original_extra_शीर्षकाणि);
	strbuf_release(&commit_सन्देशः_2);
	मुक्त(original_author);
	repo_unuse_commit_बफर(repo, commit_with_सन्देशः, original_सन्देशः);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क first_parent_वृक्षः_oid(संरचना repository *repo,
				 संरचना commit *commit,
				 संरचना object_id *बहिर्गमनम्)
{
	संरचना commit *parent = commit->parents ? commit->parents->item : रिक्त;

	यदि (!parent) {
		oidcpy(बहिर्गमनम्, repo->hash_algo->रिक्तानि_वृक्षः);
		प्रत्यागम 0;
	}

	यदि (repo_parse_commit(repo, parent))
		प्रत्यागम error(_("unable to parse parent commit %s"),
			     oid_गन्तव्यम्_hex(&parent->object.oid));

	oidcpy(बहिर्गमनम्, &repo_get_commit_वृक्षः(repo, parent)->object.oid);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क commit_वृक्षः_with_edited_सन्देशः(संरचना repository *repo,
					   ध्रुव वर्ण *action,
					   संरचना commit *original,
					   संरचना commit **बहिर्गमनम्)
{
	संरचना object_id parent_वृक्षः_oid;
	ध्रुव संरचना object_id *वृक्षः_oid;

	वृक्षः_oid = &repo_get_commit_वृक्षः(repo, original)->object.oid;

	यदि (first_parent_वृक्षः_oid(repo, original, &parent_वृक्षः_oid) < 0)
		प्रत्यागम -1;

	प्रत्यागम commit_वृक्षः_ext(repo, action, original, रिक्त, original->parents,
			       &parent_वृक्षः_oid, वृक्षः_oid, बहिर्गमनम्, COMMIT_वृक्षः_EDIT_सन्देशः);
}

गणना ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_शीर्ष,
};

स्थिर पूर्णाङ्क parse_ref_action(ध्रुव संरचना option *opt, ध्रुव वर्ण *मानम्, पूर्णाङ्क unset)
{
	गणना ref_action *action = opt->मानम्;

	BUG_ON_OPT_NEG_NOARG(unset, मानम्);
	यदि (!वाक्यं_तुलय(मानम्, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} अन्यथा यदि (!वाक्यं_तुलय(मानम्, "head")) {
		*action = REF_ACTION_शीर्ष;
	} अन्यथा {
		प्रत्यागम error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क revwalk_अन्तर्भवति_merges(संरचना repository *repo,
				   ध्रुव संरचना strvec *revwalk_तर्काः)
{
	संरचना strvec तर्काः = STRVEC_INIT;
	संरचना rev_info revs;
	पूर्णाङ्क ret;

	strvec_pushv(&तर्काः, revwalk_तर्काः->v);
	strvec_प्रक्षिप(&तर्काः, "--min-parents=2");

	repo_init_revisions(repo, &revs, रिक्त);

	सज्जता_revisions_from_strvec(&तर्काः, &revs, रिक्त);
	यदि (तर्काः.nr != 1)
		BUG("revisions were set up with invalid argument");

	यदि (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		गच्छ बहिर्गमनम्;
	}

	यदि (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		गच्छ बहिर्गमनम्;
	}

	reset_revision_walk();
	ret = 0;

बहिर्गमनम्:
	release_revisions(&revs);
	strvec_clear(&तर्काः);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क सज्जता_revwalk(संरचना repository *repo,
			 गणना ref_action action,
			 संरचना commit *original,
			 संरचना rev_info *revs)
{
	संरचना strvec तर्काः = STRVEC_INIT;
	पूर्णाङ्क ret;

	repo_init_revisions(repo, revs, रिक्त);
	strvec_प्रक्षिप(&तर्काः, "ignored");
	strvec_प्रक्षिप(&तर्काः, "--reverse");
	strvec_प्रक्षिप(&तर्काः, "--topo-order");
	strvec_प्रक्षिप(&तर्काः, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&तर्काः, "--ancestry-path=%s",
		     oid_गन्तव्यम्_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&तर्काः, "^%s", oid_गन्तव्यम्_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	यदि (action == REF_ACTION_शीर्ष) {
		संरचना commit_list *from_list = रिक्त;
		संरचना commit *शीर्ष;

		शीर्ष = lookup_commit_reference_by_name("HEAD");
		यदि (!शीर्ष) {
			ret = error(_("cannot look up HEAD"));
			गच्छ बहिर्गमनम्;
		}

		commit_list_निवेशय(original, &from_list);
		ret = repo_is_descendant_सम्बन्धि(repo, शीर्ष, from_list);
		commit_list_free(from_list);

		यदि (ret < 0) {
			ret = error(_("cannot determine descendance"));
			गच्छ बहिर्गमनम्;
		} अन्यथा यदि (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			गच्छ बहिर्गमनम्;
		}

		strvec_प्रक्षिप(&तर्काः, "HEAD");
	} अन्यथा {
		strvec_प्रक्षिप(&तर्काः, "--branches");
		strvec_प्रक्षिप(&तर्काः, "HEAD");
	}

	ret = revwalk_अन्तर्भवति_merges(repo, &तर्काः);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	सज्जता_revisions_from_strvec(&तर्काः, revs, रिक्त);
	यदि (तर्काः.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

बहिर्गमनम्:
	strvec_clear(&तर्काः);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क handle_ref_update(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *refname,
			     ध्रुव संरचना object_id *new_oid,
			     ध्रुव संरचना object_id *old_oid,
			     ध्रुव वर्ण *reflog_सन्देशः,
			     संरचना strbuf *err)
{
	यदि (!transaction) {
		प्रारूप_लिख("update %s %s %s\n",
		       refname, oid_गन्तव्यम्_hex(new_oid), oid_गन्तव्यम्_hex(old_oid));
		प्रत्यागम 0;
	}

	प्रत्यागम ref_transaction_update(transaction, refname, new_oid, old_oid,
				      रिक्त, रिक्त, 0, reflog_सन्देशः, err);
}

स्थिर पूर्णाङ्क compute_pending_ref_updates(संरचना rev_info *revs,
				       गणना ref_action action,
				       संरचना commit *original,
				       संरचना commit *rewritten,
				       गणना replay_रिक्तानि_commit_action रिक्तानि,
				       संरचना replay_परिणाम *परिणाम)
{
	ध्रुव संरचना name_decoration *decoration;
	संरचना replay_revisions_options opts = {
		.रिक्तानि = रिक्तानि,
	};
	वर्ण hex[GIT_MAX_HEXSZ + 1];
	bool detached_शीर्ष;
	पूर्णाङ्क शीर्ष_flags = 0;
	पूर्णाङ्क ret;

	refs_read_ref_पूर्णम्(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, रिक्त, &शीर्ष_flags);
	detached_शीर्ष = !(शीर्ष_flags & REF_ISSYMREF);

	opts.onto = oid_गन्तव्यम्_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, परिणाम);
	यदि (ret)
		प्रत्यागम ret;

	यदि (action != REF_ACTION_BRANCHES && action != REF_ACTION_शीर्ष)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	प्रति (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->अग्र)
	{
		यदि (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_शीर्ष)
			अनुवर्त;

		यदि (action == REF_ACTION_शीर्ष &&
		    decoration->type != DECORATION_REF_शीर्ष)
			अनुवर्त;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		यदि (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_शीर्ष &&
		    !detached_शीर्ष)
			अनुवर्त;

		replay_परिणाम_queue_update(परिणाम, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क apply_pending_ref_updates(संरचना repository *repo,
				     ध्रुव संरचना replay_परिणाम *परिणाम,
				     ध्रुव वर्ण *reflog_सन्देशः,
				     पूर्णाङ्क dry_सततम्)
{
	संरचना ref_transaction *transaction = रिक्त;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णाङ्क ret;

	यदि (!dry_सततम्) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		यदि (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			गच्छ बहिर्गमनम्;
		}
	}

	प्रति (आकार_t i = 0; i < परिणाम->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					परिणाम->updates[i].refname,
					&परिणाम->updates[i].new_oid,
					&परिणाम->updates[i].old_oid,
					reflog_सन्देशः, &err);
		यदि (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    परिणाम->updates[i].refname, err.buf);
			गच्छ बहिर्गमनम्;
		}
	}

	यदि (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क handle_reference_updates(संरचना rev_info *revs,
				    गणना ref_action action,
				    संरचना commit *original,
				    संरचना commit *rewritten,
				    ध्रुव वर्ण *reflog_सन्देशः,
				    पूर्णाङ्क dry_सततम्,
				    गणना replay_रिक्तानि_commit_action रिक्तानि)
{
	संरचना replay_परिणाम परिणाम = { 0 };
	पूर्णाङ्क ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  रिक्तानि, &परिणाम);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	ret = apply_pending_ref_updates(revs->repo, &परिणाम, reflog_सन्देशः, dry_सततम्);

बहिर्गमनम्:
	replay_परिणाम_release(&परिणाम);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क commit_became_रिक्तानि(संरचना repository *repo,
			       संरचना commit *original,
			       संरचना वृक्षः *परिणाम)
{
	संरचना object_id parent_वृक्षः_oid;

	यदि (first_parent_वृक्षः_oid(repo, original, &parent_वृक्षः_oid) < 0)
		प्रत्यागम -1;

	प्रत्यागम oideq(&परिणाम->object.oid, &parent_वृक्षः_oid);
}

स्थिर पूर्णाङ्क parse_opt_रिक्तानि(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना replay_रिक्तानि_commit_action *मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!वाक्यं_तुलय(arg, "drop"))
		*मानम् = REPLAY_रिक्तानि_COMMIT_DROP;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "keep"))
		*मानम् = REPLAY_रिक्तानि_COMMIT_KEEP;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "abort"))
		*मानम् = REPLAY_रिक्तानि_COMMIT_ABORT;
	अन्यथा
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_history_fixup(पूर्णाङ्क argc,
			     ध्रुव वर्ण **argv,
			     ध्रुव वर्ण *उपसर्गः,
			     संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		रिक्त,
	};
	गणना replay_रिक्तानि_commit_action रिक्तानि = REPLAY_रिक्तानि_COMMIT_DROP;
	गणना ref_action action = REF_ACTION_DEFAULT;
	गणना commit_वृक्षः_flags flags = 0;
	पूर्णाङ्क dry_सततम् = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_सततम्,
			 N_("perform a dry-run without updating any refs")),
		OPT_BIT(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_वृक्षः_EDIT_सन्देशः),
		OPT_CALLBACK_F(0, "empty", &रिक्तानि, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_रिक्तानि),
		OPT_अन्तः(),
	};
	संरचना merge_परिणाम merge_परिणाम = { 0 };
	संरचना merge_options merge_opts = { 0 };
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *शीर्ष_commit, *original, *rewritten;
	संरचना वृक्षः *शीर्ष_वृक्षः, *original_वृक्षः, *सूचकांक_वृक्षः;
	संरचना rev_info revs = { 0 };
	bool skip_commit = false;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	यदि (argc != 1) {
		ret = error(_("command expects a single revision"));
		गच्छ बहिर्गमनम्;
	}
	repo_config(repo, git_default_config, रिक्त);

	यदि (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	यदि (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		गच्छ बहिर्गमनम्;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	यदि (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ बहिर्गमनम्;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	शीर्ष_commit = lookup_commit_reference_by_name("HEAD");
	यदि (!शीर्ष_commit) {
		ret = error(_("cannot look up HEAD"));
		गच्छ बहिर्गमनम्;
	}

	शीर्ष_वृक्षः = repo_get_commit_वृक्षः(repo, शीर्ष_commit);
	यदि (!शीर्ष_वृक्षः) {
		ret = error(_("cannot get tree for HEAD"));
		गच्छ बहिर्गमनम्;
	}

	यदि (repo_read_सूचकांक(repo) < 0) {
		ret = error(_("unable to read index"));
		गच्छ बहिर्गमनम्;
	}

	यदि (!repo_सूचकांक_has_changes(repo, शीर्ष_वृक्षः, रिक्त)) {
		ret = error(_("nothing to fixup: no staged changes"));
		गच्छ बहिर्गमनम्;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	सूचकांक_वृक्षः = write_अन्तर्_core_सूचकांक_as_वृक्षः(repo, repo->सूचकांक);
	यदि (!सूचकांक_वृक्षः) {
		ret = error(_("unable to write index as a tree"));
		गच्छ बहिर्गमनम्;
	}

	original_वृक्षः = repo_get_commit_वृक्षः(repo, original);
	यदि (!original_वृक्षः) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		गच्छ बहिर्गमनम्;
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
	merge_incore_nonrecursive(&merge_opts, शीर्ष_वृक्षः,
				  original_वृक्षः, सूचकांक_वृक्षः, &merge_परिणाम);

	यदि (merge_परिणाम.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		गच्छ बहिर्गमनम्;
	}

	यदि (!merge_परिणाम.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		गच्छ बहिर्गमनम्;
	}

	ret = commit_became_रिक्तानि(repo, original, merge_परिणाम.वृक्षः);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;
	यदि (ret > 0) {
		विकल्प (रिक्तानि) {
		पक्ष REPLAY_रिक्तानि_COMMIT_DROP:
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
			यदि (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				गच्छ बहिर्गमनम्;
			}

			skip_commit = true;
			भङ्ग;
		पक्ष REPLAY_रिक्तानि_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			भङ्ग;
		पक्ष REPLAY_रिक्तानि_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			गच्छ बहिर्गमनम्;
		}
	}

	ret = सज्जता_revwalk(repo, action, original, &revs);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	यदि (!skip_commit) {
		ret = commit_वृक्षः_ext(repo, "fixup", original, रिक्त, original->parents,
				      &original_वृक्षः->object.oid, &merge_परिणाम.वृक्षः->object.oid,
				      &rewritten, flags);
		यदि (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			गच्छ बहिर्गमनम्;
		}
	}

	strbuf_addf(&reflog_सन्देशः, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_सन्देशः.buf, dry_सततम्, रिक्तानि);
	यदि (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	merge_finalize(&merge_opts, &merge_परिणाम);
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_history_reword(पूर्णाङ्क argc,
			      ध्रुव वर्ण **argv,
			      ध्रुव वर्ण *उपसर्गः,
			      संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_सततम् = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_सततम्,
			 N_("perform a dry-run without updating any refs")),
		OPT_अन्तः(),
	};
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *original, *rewritten;
	संरचना rev_info revs = { 0 };
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	यदि (argc != 1) {
		ret = error(_("command expects a single revision"));
		गच्छ बहिर्गमनम्;
	}
	repo_config(repo, git_default_config, रिक्त);

	यदि (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	यदि (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ बहिर्गमनम्;
	}

	ret = सज्जता_revwalk(repo, action, original, &revs);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	ret = commit_वृक्षः_with_edited_सन्देशः(repo, "reworded", original, &rewritten);
	यदि (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		गच्छ बहिर्गमनम्;
	}

	strbuf_addf(&reflog_सन्देशः, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_सन्देशः.buf, dry_सततम्, REPLAY_रिक्तानि_COMMIT_ABORT);
	यदि (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क write_ondisk_सूचकांक(संरचना repository *repo,
			      संरचना object_id *oid,
			      ध्रुव वर्ण *मार्गः)
{
	संरचना unpack_trees_options opts = { 0 };
	संरचना तालकम्_file तालकम् = तालकम्_INIT;
	संरचना वृक्षः_desc वृक्षः_desc;
	संरचना सूचकांक_state सूचकांक;
	संरचना वृक्षः *वृक्षः;
	पूर्णाङ्क ret;

	सूचकांक_state_init(&सूचकांक, repo);

	opts.शीर्ष_idx = -1;
	opts.src_सूचकांक = &सूचकांक;
	opts.dst_सूचकांक = &सूचकांक;

	वृक्षः = repo_parse_वृक्षः_परोक्षम्(repo, oid);
	यदि (!वृक्षः) {
		ret = error(_("unable to parse tree %s"), oid_गन्तव्यम्_hex(oid));
		गच्छ बहिर्गमनम्;
	}
	init_वृक्षः_desc(&वृक्षः_desc, &वृक्षः->object.oid, वृक्षः->बफर, वृक्षः->आकार);

	यदि (unpack_trees(1, &वृक्षः_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		गच्छ बहिर्गमनम्;
	}

	अविभाज्यः_cache_वृक्षः(repo, &सूचकांक, वृक्षः);

	यदि (repo_hold_तालकम्_file_for_update(repo, &तालकम्, मार्गः, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		गच्छ बहिर्गमनम्;
	}

	यदि (write_locked_सूचकांक(&सूचकांक, &तालकम्, COMMIT_तालकम्)) {
		ret = error(_("unable to write new index file"));
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	rollback_तालकम्_file(&तालकम्);
	release_सूचकांक(&सूचकांक);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क विभज_commit(संरचना repository *repo,
			संरचना commit *original,
			संरचना pathspec *pathspec,
			संरचना commit **बहिर्गमनम्)
{
	संरचना interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	संरचना strbuf सूचकांक_file = STRBUF_INIT;
	संरचना सूचकांक_state सूचकांक = सूचकांक_STATE_INIT(repo);
	ध्रुव संरचना object_id *original_commit_वृक्षः_oid;
	ध्रुव संरचना object_id *old_वृक्षः_oid, *new_वृक्षः_oid;
	संरचना object_id parent_वृक्षः_oid;
	वर्ण original_commit_oid[GIT_MAX_HEXSZ + 1];
	संरचना commit *first_commit, *second_commit;
	संरचना commit_list *parents = रिक्त;
	संरचना वृक्षः *विभज_वृक्षः;
	पूर्णाङ्क ret;

	यदि (first_parent_वृक्षः_oid(repo, original, &parent_वृक्षः_oid) < 0) {
		ret = -1;
		गच्छ बहिर्गमनम्;
	}
	original_commit_वृक्षः_oid = get_commit_वृक्षः_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_मार्गः_replace(repo, &सूचकांक_file, "%s", "history-split.index");

	ret = write_ondisk_सूचकांक(repo, &parent_वृक्षः_oid, सूचकांक_file.buf);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	ret = read_सूचकांक_from(&सूचकांक, सूचकांक_file.buf, repo->gitdir);
	यदि (ret < 0) {
		ret = error(_("failed reading temporary index"));
		गच्छ बहिर्गमनम्;
	}

	oid_गन्तव्यम्_hex_r(original_commit_oid, &original->object.oid);
	ret = सततम्_सङ्कलय_p_सूचकांक(repo, &सूचकांक, सूचकांक_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, सङ्कलय_P_DISALLOW_EDIT);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	विभज_वृक्षः = write_अन्तर्_core_सूचकांक_as_वृक्षः(repo, &सूचकांक);
	यदि (!विभज_वृक्षः) {
		ret = error(_("failed split tree"));
		गच्छ बहिर्गमनम्;
	}

	unlink(सूचकांक_file.buf);
	strbuf_release(&सूचकांक_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	यदि (oideq(&विभज_वृक्षः->object.oid, &parent_वृक्षः_oid)) {
		ret = error(_("split commit is empty"));
		गच्छ बहिर्गमनम्;
	} अन्यथा यदि (oideq(&विभज_वृक्षः->object.oid, original_commit_वृक्षः_oid)) {
		ret = error(_("split commit tree matches original commit"));
		गच्छ बहिर्गमनम्;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_वृक्षः_ext(repo, "split-out", original, रिक्त, original->parents, &parent_वृक्षः_oid,
			      &विभज_वृक्षः->object.oid, &first_commit, COMMIT_वृक्षः_EDIT_सन्देशः);
	यदि (ret < 0) {
		ret = error(_("failed writing first commit"));
		गच्छ बहिर्गमनम्;
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
			      new_वृक्षः_oid, &second_commit, COMMIT_वृक्षः_EDIT_सन्देशः);
	यदि (ret < 0) {
		ret = error(_("failed writing second commit"));
		गच्छ बहिर्गमनम्;
	}

	*बहिर्गमनम् = second_commit;
	ret = 0;

बहिर्गमनम्:
	यदि (सूचकांक_file.len)
		unlink(सूचकांक_file.buf);
	strbuf_release(&सूचकांक_file);
	commit_list_free(parents);
	release_सूचकांक(&सूचकांक);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_history_विभज(पूर्णाङ्क argc,
			     ध्रुव वर्ण **argv,
			     ध्रुव वर्ण *उपसर्गः,
			     संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_विभज_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_सततम् = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_सततम्,
			 N_("perform a dry-run without updating any refs")),
		OPT_अन्तः(),
	};
	संरचना commit *original, *rewritten = रिक्त;
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना pathspec pathspec = { 0 };
	संरचना rev_info revs = { 0 };
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	यदि (argc < 1) {
		ret = error(_("command expects a committish"));
		गच्छ बहिर्गमनम्;
	}
	repo_config(repo, git_default_config, रिक्त);

	यदि (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_पूर्णम् |
		       PATHSPEC_SYMLINK_LEADING_मार्गः |
		       PATHSPEC_उपसर्गः_ORIGIN,
		       उपसर्गः, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	यदि (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ बहिर्गमनम्;
	}

	ret = सज्जता_revwalk(repo, action, original, &revs);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	यदि (original->parents && original->parents->अग्र) {
		ret = error(_("cannot split up merge commit"));
		गच्छ बहिर्गमनम्;
	}

	ret = विभज_commit(repo, original, &pathspec, &rewritten);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	strbuf_addf(&reflog_सन्देशः, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_सन्देशः.buf, dry_सततम्, REPLAY_रिक्तानि_COMMIT_ABORT);
	यदि (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	strbuf_release(&reflog_सन्देशः);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	प्रत्यागम ret;
}

#परिभाषय SQUASH_आलोकितम् (1u << 11)
#परिभाषय SQUASH_TIP (1u << 12)
#परिभाषय SQUASH_AMEND_TARGET (1u << 13)

स्थिर bool is_autosquash_subject(ध्रुव वर्ण *s)
{
	प्रत्यागम starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

स्थिर bool skip_one_autosquash_उपसर्गः(ध्रुव वर्ण *s, ध्रुव वर्ण **बहिर्गमनम्)
{
	यदि (skip_उपसर्गः(s, "amend!", बहिर्गमनम्) || skip_उपसर्गः(s, "fixup!", बहिर्गमनम्) ||
	    skip_उपसर्गः(s, "squash!", बहिर्गमनम्)) {
		यावत् (**बहिर्गमनम् == ' ')
			(*बहिर्गमनम्)++;
		प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर शून्य truncate_सन्देशः_गन्तव्यम्_subject(संरचना strbuf *सन्देशः)
{
	ध्रुव वर्ण *eos = वाक्ये_अन्विष(सन्देशः->buf, "\n\n");

	यदि (eos)
		strbuf_setlen(सन्देशः, eos - सन्देशः->buf + 1);
}

संरचना subject_दत्त {
	संरचना strintmap subjects;
	संरचना strbuf subject;
	संरचना strbuf squash_सन्देशः;
	ध्रुव वर्ण *सन्देशः_2;
	bool edit_सन्देशः;
};

#परिभाषय SUBJECT_दत्त_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_सन्देशः = STRBUF_INIT,	\
}

स्थिर शून्य subject_दत्त_clear(संरचना subject_दत्त *दत्त)
{
	strintmap_clear(&दत्त->subjects);
	strbuf_release(&दत्त->subject);
	strbuf_release(&दत्त->squash_सन्देशः);
}

स्थिर पूर्णाङ्क squash_amend_सन्देशः(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_दत्त *दत्त,
				अचिह्नित flags)
{
	ध्रुव वर्ण *body = दत्त->सन्देशः_2 + दत्त->subject.len;

	यावत् (रिक्तम्_वा(*body))
		body++;

	यदि (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_find_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			दत्त->subject.buf);
		प्रत्यागम 0;
	}

	यदि (दत्त->edit_सन्देशः) {
		प्रत्यागम 0;
	} अन्यथा यदि (flags & SQUASH_AMEND_TARGET) {
		यदि (starts_with(दत्त->squash_सन्देशः.buf, "squash!"))
			प्रत्यागम error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     दत्त->subject.buf);
		यदि (starts_with(दत्त->squash_सन्देशः.buf, "fixup!"))
			strbuf_splice(&दत्त->squash_सन्देशः, 0, 5, "amend!", 5);
		यदि (starts_with(दत्त->squash_सन्देशः.buf, "amend!")) {
			truncate_सन्देशः_गन्तव्यम्_subject(&दत्त->squash_सन्देशः);
			strbuf_addch(&दत्त->squash_सन्देशः, '\n');
		} अन्यथा {
			strbuf_reset(&दत्त->squash_सन्देशः);
		}
		strbuf_addstr(&दत्त->squash_सन्देशः, body);
		strbuf_complete_पङ्क्तिः(&दत्त->squash_सन्देशः);
	} अन्यथा {
		प्रत्यागम error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_find_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     दत्त->subject.buf);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क squash_squash_सन्देशः(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_दत्त *दत्त,
				अचिह्नित flags)
{
	ध्रुव वर्ण *body = दत्त->सन्देशः_2 + दत्त->subject.len;

	यावत् (रिक्तम्_वा(*body))
		body++;

	यदि (दत्त->edit_सन्देशः) {
		प्रत्यागम 0;
	} अन्यथा यदि (flags & SQUASH_AMEND_TARGET) {
		यदि (starts_with(दत्त->squash_सन्देशः.buf, "fixup!")) {
			truncate_सन्देशः_गन्तव्यम्_subject(&दत्त->squash_सन्देशः);
			strbuf_splice(&दत्त->squash_सन्देशः, 0, 5, "squash", 6);
		}
		यदि (starts_with(दत्त->squash_सन्देशः.buf, "squash!")) {
			strbuf_addch(&दत्त->squash_सन्देशः, '\n');
			strbuf_addstr(&दत्त->squash_सन्देशः, body);
			strbuf_complete_पङ्क्तिः(&दत्त->squash_सन्देशः);
		} अन्यथा {
			प्रत्यागम error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_find_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     दत्त->subject.buf);
		}
	} अन्यथा {
		प्रत्यागम error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_find_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      दत्त->subject.buf);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क squash_check_can_autosquash(संरचना repository *repo,
				       संरचना commit *commit,
				       संरचना subject_दत्त *दत्त,
				       अचिह्नित flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_TARGET;
	यदि (starts_with(दत्त->subject.buf, "amend!"))
		प्रत्यागम squash_amend_सन्देशः(repo, commit, दत्त, flags);
	अन्यथा यदि (starts_with(दत्त->subject.buf, "squash!"))
		प्रत्यागम squash_squash_सन्देशः(repo, commit, दत्त, flags);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क squash_check_autosquash_subject(संरचना repository *repo,
					   संरचना commit *commit,
					   संरचना subject_दत्त *दत्त)
{
	ध्रुव वर्ण* s = दत्त->subject.buf;
	संरचना commit *target;
	संरचना hashmap_iter iter;
	संरचना strmap_प्रविष्टिः *प्रविष्टिः;
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
	यावत् (skip_one_autosquash_उपसर्गः(s, &s)) {
		अचिह्नित flags = strintmap_get(&दत्त->subjects, s);
		यदि (flags)
			प्रत्यागम squash_check_can_autosquash(repo, commit, दत्त, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	target = lookup_commit_reference_by_name(s);
	यदि (target && starts_with(oid_गन्तव्यम्_hex(&target->object.oid), s)) {
		अचिह्नित flags =
			target->object.flags & (SQUASH_आलोकितम् | SQUASH_AMEND_TARGET);
		यदि (!flags) {
			ध्रुव वर्ण *subject_start;
			ध्रुव वर्ण *बफर = repo_logmsg_reencode(repo, target,
								  रिक्त, रिक्त);
			आकार_t subject_len = find_commit_subject(बफर,
								 &subject_start);
			वर्ण *subject = xmemdupz(subject_start, subject_len);

			flags = strintmap_get(&दत्त->subjects, subject);
			मुक्त(subject);
			repo_unuse_commit_बफर(repo, target, बफर);
		}
		यदि (flags)
			प्रत्यागम squash_check_can_autosquash(repo, commit,
							   दत्त, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_प्रविष्टिः(&दत्त->subjects, &iter, प्रविष्टिः) {
		s = दत्त->subject.buf;
		यावत्(skip_one_autosquash_उपसर्गः(s, &s)) {
			यदि (starts_with(प्रविष्टिः->कुञ्चिका, s)) {
				अचिह्नित मानम् = (intptr_t)प्रविष्टिः->मानम्;

				प्रत्यागम squash_check_can_autosquash(repo, commit,
								   दत्त, मानम्);
			}
		}
	}
	प्रत्यागम error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_find_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       दत्त->subject.buf);
}

स्थिर पूर्णाङ्क squash_check_subject(संरचना repository *repo,
				संरचना commit *commit,
				संरचना subject_दत्त *दत्त)
{
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *buf = repo_logmsg_reencode(repo, commit, रिक्त, रिक्त);
	आकार_t subject_len = find_commit_subject(buf, &दत्त->सन्देशः_2);

	strbuf_reset(&दत्त->subject);
	strbuf_सङ्कलय(&दत्त->subject, दत्त->सन्देशः_2, subject_len);

	यदि (!strintmap_get_आकार(&दत्त->subjects)) {
		ध्रुव वर्ण *s;

		strbuf_addstr(&दत्त->squash_सन्देशः, दत्त->सन्देशः_2);
		strbuf_complete_पङ्क्तिः(&दत्त->squash_सन्देशः);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = दत्त->subject.buf;
		skip_one_autosquash_उपसर्गः(s, &s);
		strintmap_समुच्चयः(&दत्त->subjects, s, SQUASH_AMEND_TARGET | SQUASH_आलोकितम्);
		commit->object.flags |= SQUASH_AMEND_TARGET;
	} अन्यथा यदि (is_autosquash_subject(दत्त->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, दत्त);
	} अन्यथा {
		strintmap_समुच्चयः(&दत्त->subjects, दत्त->subject.buf, SQUASH_आलोकितम्);
	}
	repo_unuse_commit_बफर(repo, commit, buf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क build_squash_सन्देशः(संरचना repository *repo,
				ध्रुव संरचना strbuf *todo_buf,
				संरचना strbuf *बहिर्गमनम्);

स्थिर पूर्णाङ्क सज्जता_squash_revisions(संरचना repository *repo,
				  पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				  संरचना rev_info *revs)
{
	repo_init_revisions(repo, revs, रिक्त);
	revs->reverse = 1;
	revs->topo_क्रमः = 1;
	revs->क्रमय_क्रमः = REV_क्रमय_अन्तर्_GRAPH_क्रमः;
	revs->simplify_history = 0;
	revs->ancestry_मार्गः = 1;
	revs->limited = 1;
	revs->ancestry_मार्गः_implicit_bottoms = 1;

	argc = सज्जता_revisions(argc, argv, revs, रिक्त);
	यदि (argc > 1)
		प्रत्यागम error(_("unrecognized argument: %s"), argv[1]);

	यदि (revs->reverse != 1 || revs->topo_क्रमः != 1 ||
	    revs->क्रमय_क्रमः != REV_क्रमय_अन्तर्_GRAPH_क्रमः ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_मार्गः != 1 || revs->limited != 1 ||
	    revs->ancestry_मार्गः_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->reverse = 1;
		revs->topo_क्रमः = 1;
		revs->क्रमय_क्रमः = REV_क्रमय_अन्तर्_GRAPH_क्रमः;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_मार्गः = 1;
		revs->limited = 1;
		revs->ancestry_मार्गः_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	प्रति (आकार_t i = 0; i < revs->cmdline.nr; i++)
		यदि (revs->cmdline.rev[i].flags & BOTTOM)
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
				bool edit_सन्देशः,
				पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				संरचना commit **oldest_बहिर्गमनम्,
				संरचना commit **tip_बहिर्गमनम्,
				वर्ण **सन्देशः_बहिर्गमनम्)
{
	संरचना rev_info revs;
	संरचना subject_दत्त subject_दत्त = SUBJECT_दत्त_INIT;
	संरचना commit *commit, *oldest = रिक्त, *tip = रिक्त;
	संरचना strbuf todo_buf = STRBUF_INIT;
	पूर्णाङ्क ret, tip_गणनम् = 0;
	bool walk_started = false;
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_array refs = { 0 };

	subject_दत्त.edit_सन्देशः = edit_सन्देशः;
	ret = सज्जता_squash_revisions(repo, argc, argv, &revs);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	यदि (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		गच्छ बहिर्गमनम्;
	}
	walk_started = true;
	यावत् ((commit = get_revision(&revs))) {
		संरचना commit_list *p;

		यदि (edit_सन्देशः)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_गन्तव्यम्_hex(&commit->object.oid));

		यदि (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			गच्छ बहिर्गमनम्;
		}
		प्रति (p = commit->parents; oldest && p; p = p->अग्र) {
			संरचना commit_list *q;
			संरचना object *o;
			bool आलोकितम्;

			यदि (repo_parse_commit(repo, p->item)) {
				ret = error(_("cannot parse commit"));
				गच्छ बहिर्गमनम्;
			}
			o = &p->item->object;
			आलोकितम् = o->flags & SQUASH_आलोकितम्;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			प्रति (q = oldest->parents; !आलोकितम् && q; q = q->अग्र) {
				यदि (p->item == q->item) {
					आलोकितम् = true;
					commit_list_निवेशय(commit, &filter.with_commit);
				}
			}
			यदि (!आलोकितम्) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_find_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_find_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				गच्छ बहिर्गमनम्;
			}
			यदि (o->flags & SQUASH_TIP) {
				tip_गणनम्--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		यदि (!oldest) {
			commit_list_निवेशय(commit, &filter.with_commit);
			oldest = commit;
		}
		यदि (squash_check_subject(repo, commit, &subject_दत्त)) {
			ret = -1;
			गच्छ बहिर्गमनम्;
		}
		tip = commit;
		tip->object.flags |= SQUASH_आलोकितम् | SQUASH_TIP;
		tip_गणनम्++;
	}
	clear_object_flags(repo, SQUASH_आलोकितम् | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	यदि (!tip_गणनम्) {
		ret = error(_("the revision range is empty"));
		गच्छ बहिर्गमनम्;
	} अन्यथा यदि (tip_गणनम् != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		गच्छ बहिर्गमनम्;
	} अन्यथा यदि (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		गच्छ बहिर्गमनम्;
	} अन्यथा यदि (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_निवेशय(tip, &filter.no_commit);
	filter.विधा = FILTER_REFS_BRANCHES;
	यदि (update_branches &&
	    filter_refs(&refs, &filter, filter.विधा)) {
		ret = error(_("could not filter refs"));
		गच्छ बहिर्गमनम्;
	}
	यदि (refs.nr) {
		संरचना ref_format format = REF_FORMAT_INIT;
		संरचना ref_क्रमणम् *क्रमणम्;
		संरचना string_list क्रमणम्_options = STRING_LIST_INIT_DUP;
		संरचना strbuf branches = STRBUF_INIT;
		संरचना strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		यदि (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&क्रमणम्_options, "refname");
		क्रमणम् = ref_क्रमणम्_options(&क्रमणम्_options);
		ref_array_क्रमय(क्रमणम्, &refs);
		प्रति (पूर्णाङ्क i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			यदि (format_ref_array_item(refs.वस्तूनि[i], &format,
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
		ref_क्रमणम्_release(क्रमणम्);
		string_list_clear(&क्रमणम्_options, 0);
		गच्छ बहिर्गमनम्;
	}
	यदि (edit_सन्देशः) {
		strbuf_reset(&subject_दत्त.squash_सन्देशः);
		ret = build_squash_सन्देशः(repo, &todo_buf,
					   &subject_दत्त.squash_सन्देशः);
		यदि (ret < 0)
			गच्छ बहिर्गमनम्;
	}

	*oldest_बहिर्गमनम् = oldest;
	*tip_बहिर्गमनम् = tip;
	*सन्देशः_बहिर्गमनम् = strbuf_detach(&subject_दत्त.squash_सन्देशः, रिक्त);
	ret = 0;

बहिर्गमनम्:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_आलोकितम् | SQUASH_TIP |
			   SQUASH_AMEND_TARGET);
	यदि (walk_started)
		reset_revision_walk();
	subject_दत्त_clear(&subject_दत्त);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	प्रत्यागम ret;
}

स्थिर bool amend_replaces_target(संरचना todo_list *todo, पूर्णाङ्क target)
{
	प्रति (पूर्णाङ्क i = target + 1; i < todo->nr &&
				 todo->वस्तूनि[i].command != TODO_PICK; i++) {
		यदि (todo->वस्तूनि[i].command == TODO_SQUASH)
			प्रत्यागम false;
		यदि (todo->वस्तूनि[i].flags & TODO_REPLACE_FIXUP_सन्देशः)
			प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर पूर्णाङ्क build_squash_सन्देशः(संरचना repository *repo,
				ध्रुव संरचना strbuf *todo_buf,
				संरचना strbuf *बहिर्गमनम्)
{
	संरचना todo_list todo = TODO_LIST_INIT;
	संरचना replay_opts opts = REPLAY_OPTS_INIT;
	पूर्णाङ्क nr_commits, ret;

	यदि (todo_list_parse_insn_बफर(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		गच्छ बहिर्गमनम्;
	}

	nr_commits = todo.nr;
	प्रति (पूर्णाङ्क i = 0; i < nr_commits; i++) {
		संरचना todo_item *item = &todo.वस्तूनि[i];
		ध्रुव वर्ण *सन्देशः_2, *body;
		आकार_t commented_len;
		bool skip, squashing;

		squashing = item->command == TODO_SQUASH ||
			    (item->flags & TODO_REPLACE_FIXUP_सन्देशः);
		यदि (item->command == TODO_PICK)
			skip = amend_replaces_target(&todo, i);
		अन्यथा
			skip = !squashing;

		सन्देशः_2 = repo_logmsg_reencode(repo, item->commit, रिक्त, रिक्त);
		find_commit_subject(सन्देशः_2, &body);

		यदि (skip)
			commented_len = वाक्यदैर्घ्यम्(body);
		अन्यथा यदि (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		अन्यथा
			commented_len = 0;

		यदि (!i)
			सङ्कलय_squash_combination_शीर्षकम्(बहिर्गमनम्, nr_commits);
		strbuf_addch(बहिर्गमनम्, '\n');
		सङ्कलय_squash_सन्देशः_शीर्षकम्(बहिर्गमनम्, i + 1, skip);
		strbuf_addstr(बहिर्गमनम्, "\n\n");
		strbuf_सङ्कलय_commented_पङ्क्तयः(बहिर्गमनम्, body, commented_len, comment_पङ्क्तिः_str);
		strbuf_addstr(बहिर्गमनम्, body + commented_len);
		strbuf_complete_पङ्क्तिः(बहिर्गमनम्);

		repo_unuse_commit_बफर(repo, item->commit, सन्देशः_2);
	}

	ret = 0;

बहिर्गमनम्:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_history_squash(पूर्णाङ्क argc,
			      ध्रुव वर्ण **argv,
			      ध्रुव वर्ण *उपसर्गः,
			      संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		रिक्त,
	};
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_सततम् = 0;
	पूर्णाङ्क edit = 1;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_सततम्,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_अन्तः(),
	};
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *oldest, *tip, *rewritten;
	ध्रुव संरचना object_id *आधारः_वृक्षः_oid, *tip_वृक्षः_oid;
	वर्ण *सन्देशः_template = रिक्त;
	संरचना rev_info revs = { 0 };
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	यदि (argc < 2) {
		ret = error(_("command expects a revision range"));
		गच्छ बहिर्गमनम्;
	}
	repo_config(repo, git_default_config, रिक्त);

	यदि (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_सन्देशः, "squash: updating ");
	strbuf_join_argv(&reflog_सन्देशः, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &सन्देशः_template);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	ret = सज्जता_revwalk(repo, action, tip, &revs);
	यदि (ret < 0)
		गच्छ बहिर्गमनम्;

	आधारः_वृक्षः_oid = &repo_get_commit_वृक्षः(repo,
					oldest->parents->item)->object.oid;
	tip_वृक्षः_oid = &repo_get_commit_वृक्षः(repo, tip)->object.oid;

	ret = commit_वृक्षः_ext(repo, "squash", oldest, सन्देशः_template,
			      oldest->parents, आधारः_वृक्षः_oid, tip_वृक्षः_oid,
			      &rewritten,
			      edit ? COMMIT_वृक्षः_EDIT_सन्देशः : 0);
	यदि (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		गच्छ बहिर्गमनम्;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_सन्देशः.buf, dry_सततम्,
				       REPLAY_रिक्तानि_COMMIT_ABORT);
	यदि (ret < 0) {
		ret = error(_("failed replaying descendants"));
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	मुक्त(सन्देशः_template);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क update_worktree(संरचना repository *repo,
			   ध्रुव संरचना commit *old_शीर्ष,
			   ध्रुव संरचना commit *new_शीर्ष,
			   bool dry_सततम्)
{
	संरचना reset_working_वृक्षः_options opts = {
		.oid_from = &old_शीर्ष->object.oid,
		.oid = &new_शीर्ष->object.oid,
	};
	यदि (dry_सततम्)
		opts.flags |= RESET_WORKING_वृक्षः_DRY_सततम्;
	प्रत्यागम reset_working_वृक्षः(repo, &opts);
}

स्थिर पूर्णाङ्क find_शीर्ष_वृक्षः_change(संरचना repository *repo,
				 ध्रुव संरचना replay_परिणाम *परिणाम,
				 संरचना commit **old_शीर्ष,
				 संरचना commit **new_शीर्ष,
				 bool *परिवर्तितम्)
{
	ध्रुव संरचना replay_ref_update *शीर्ष_update = रिक्त;
	संरचना commit *old_शीर्ष_commit, *new_शीर्ष_commit;
	संरचना वृक्षः *old_शीर्ष_वृक्षः, *new_शीर्ष_वृक्षः;
	ध्रुव वर्ण *शीर्ष_target;
	पूर्णाङ्क शीर्ष_flags;

	*परिवर्तितम् = false;

	शीर्ष_target = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      रिक्त, &शीर्ष_flags);
	यदि (!शीर्ष_target)
		प्रत्यागम error(_("cannot look up HEAD"));

	प्रति (आकार_t i = 0; i < परिणाम->updates_nr; i++) {
		यदि (!वाक्यं_तुलय(परिणाम->updates[i].refname, शीर्ष_target)) {
			शीर्ष_update = &परिणाम->updates[i];
			भङ्ग;
		}
	}

	यदि (!शीर्ष_update)
		प्रत्यागम 0;

	old_शीर्ष_commit = lookup_commit_reference(repo, &शीर्ष_update->old_oid);
	new_शीर्ष_commit = lookup_commit_reference(repo, &शीर्ष_update->new_oid);
	यदि (!old_शीर्ष_commit || !new_शीर्ष_commit)
		प्रत्यागम error(_("cannot resolve HEAD commit"));

	old_शीर्ष_वृक्षः = repo_get_commit_वृक्षः(repo, old_शीर्ष_commit);
	new_शीर्ष_वृक्षः = repo_get_commit_वृक्षः(repo, new_शीर्ष_commit);
	यदि (!old_शीर्ष_वृक्षः || !new_शीर्ष_वृक्षः)
		प्रत्यागम error(_("cannot resolve tree for HEAD"));

	यदि (oideq(&old_शीर्ष_वृक्षः->object.oid, &new_शीर्ष_वृक्षः->object.oid))
		प्रत्यागम 0;

	*old_शीर्ष = old_शीर्ष_commit;
	*new_शीर्ष = new_शीर्ष_commit;
	*परिवर्तितम् = true;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_history_drop(पूर्णाङ्क argc,
			    ध्रुव वर्ण **argv,
			    ध्रुव वर्ण *उपसर्गः,
			    संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_DROP_USAGE,
		रिक्त,
	};
	गणना replay_रिक्तानि_commit_action रिक्तानि = REPLAY_रिक्तानि_COMMIT_DROP;
	गणना ref_action action = REF_ACTION_DEFAULT;
	पूर्णाङ्क dry_सततम् = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_सततम्,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &रिक्तानि, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_रिक्तानि),
		OPT_अन्तः(),
	};
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना commit *original, *rewritten;
	संरचना rev_info revs = { 0 };
	संरचना replay_परिणाम परिणाम = { 0 };
	संरचना commit *old_शीर्ष, *new_शीर्ष;
	bool शीर्ष_moves = false;
	पूर्णाङ्क ret;

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	यदि (argc != 1) {
		ret = error(_("command expects a single revision"));
		गच्छ बहिर्गमनम्;
	}
	repo_config(repo, git_default_config, रिक्त);

	यदि (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	यदि (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		गच्छ बहिर्गमनम्;
	}

	यदि (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		गच्छ बहिर्गमनम्;
	} अन्यथा यदि (original->parents->अग्र) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		गच्छ बहिर्गमनम्;
	}

	ret = सज्जता_revwalk(repo, action, original, &revs);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	rewritten = original->parents->item;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  रिक्तानि, &परिणाम);
	यदि (ret) {
		ret = error(_("failed replaying descendants"));
		गच्छ बहिर्गमनम्;
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
	यदि (!is_bare_repository(repo)) {
		ret = find_शीर्ष_वृक्षः_change(repo, &परिणाम, &old_शीर्ष,
					    &new_शीर्ष, &शीर्ष_moves);
		यदि (ret < 0)
			गच्छ बहिर्गमनम्;

		यदि (शीर्ष_moves && update_worktree(repo, old_शीर्ष, new_शीर्ष, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			गच्छ बहिर्गमनम्;
		}
	}

	strbuf_addf(&reflog_सन्देशः, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &परिणाम, reflog_सन्देशः.buf, dry_सततम्);
	यदि (ret < 0) {
		ret = error(_("failed to update references"));
		गच्छ बहिर्गमनम्;
	}

	यदि (!dry_सततम् && शीर्ष_moves && update_worktree(repo, old_शीर्ष, new_शीर्ष, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_गन्तव्यम्_hex(&new_शीर्ष->object.oid));
		गच्छ बहिर्गमनम्;
	}

	ret = 0;

बहिर्गमनम्:
	replay_परिणाम_release(&परिणाम);
	strbuf_release(&reflog_सन्देशः);
	release_revisions(&revs);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_history(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_विभज_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		रिक्त,
	};
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_विभज),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	प्रत्यागम fn(argc, argv, उपसर्गः, repo);
}
