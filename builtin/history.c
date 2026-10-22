#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "cache-tree.h"
#ضمن "commit.h"
#ضمن "commit-reach.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "merge-ort.h"
#ضمن "oidmap.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "read-cache.h"
#ضمن "refs.h"
#ضمن "ref-filter.h"
#ضمن "replay.h"
#ضمن "reset.h"
#ضمن "revision.h"
#ضمن "sequencer.h"
#ضمن "string-list.h"
#ضمن "strvec.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "unpack-trees.h"
#ضمن "wt-status.h"

#تعریف_کړه GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#تعریف_کړه GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#تعریف_کړه GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#تعریف_کړه GIT_HISTORY_وېشل_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#تعریف_کړه GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ساكن فراغ change_معلومات_free(فراغ *util, ثابت حرف *str ناکارول_شوی)
{
	بنية wt_حالت_change_معلومات *d = util;
	حرر(d->rename_سرچینه);
	حرر(d);
}

ساكن صحيح fill_commit_پیغام(بنية repository *repo,
			       ثابت بنية object_id *old_ونه,
			       ثابت بنية object_id *new_ونه,
			       ثابت حرف *default_پیغام,
			       ثابت حرف *action,
			       بنية strbuf *بهر)
{
	ثابت حرف *لار = git_لار_commit_editmsg();
	ثابت حرف *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	بنية wt_حالت s;

	wt_حالت_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_چټک;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_مختاړی = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(لار, "w");
	إذا (!s.fp)
		أرجع error_errno(_("could not open '%s'"), لار);

	strbuf_addstr(بهر, default_پیغام);
	strbuf_addch(بهر, '\n');
	strbuf_commented_addf(بهر, comment_کرښه_str, hint, action, comment_کرښه_str);
	إذا (fwrite(بهر->buf, 1, بهر->len, s.fp) != بهر->len)
		die_errno(_("could not write to '%s'"), لار);

	wt_حالت_collect_changes_trees(&s, old_ونه, new_ونه);
	wt_حالت_print(&s);
	wt_حالت_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_معلومات_free);
	إذا (fclose(s.fp))
		die_errno(_("could not write to '%s'"), لار);

	strbuf_reset(بهر);
	إذا (launch_editor(لار, بهر, عدم)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		أرجع -1;
	}
	strbuf_stripspace(بهر, comment_کرښه_str);

	cleanup_پیغام(بهر, COMMIT_پیغام_CLEANUP_ALL, 0);

	إذا (!بهر->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		أرجع -1;
	}

	أرجع 0;
}

تعداد commit_ونه_flags {
	COMMIT_ونه_EDIT_پیغام = (1 << 0),
};

ساكن صحيح commit_ونه_ext(بنية repository *repo,
			   ثابت حرف *action,
			   بنية commit *commit_with_پیغام,
			   ثابت حرف *پیغام_template,
			   ثابت بنية commit_list *parents,
			   ثابت بنية object_id *old_ونه,
			   ثابت بنية object_id *new_ونه,
			   بنية commit **بهر,
			   تعداد commit_ونه_flags flags)
{
	ثابت حرف *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		عدم,
	};
	ثابت حرف *original_پیغام, *original_body, *ptr;
	بنية commit_extra_سرلیک *original_extra_سرلیکونه = عدم;
	بنية strbuf commit_پیغام_2 = STRBUF_INIT;
	بنية object_id rewritten_commit_oid;
	حرف *original_author = عدم;
	اندازه_t len;
	صحيح ret;

	/* We retain authorship of the original commit. */
	original_پیغام = repo_logmsg_reencode(repo, commit_with_پیغام, عدم, عدم);
	ptr = پیدا_کړه_commit_سرلیک(original_پیغام, "author", &len);
	إذا (ptr)
		original_author = xmemdupz(ptr, len);
	پیدا_کړه_commit_subject(original_پیغام, &original_body);

	إذا (!پیغام_template)
		پیغام_template = original_body;

	إذا (flags & COMMIT_ونه_EDIT_پیغام) {
		ret = fill_commit_پیغام(repo, old_ونه, new_ونه,
					  پیغام_template, action, &commit_پیغام_2);
		إذا (ret < 0)
			اذهب بهر;
	} وإلا {
		strbuf_addstr(&commit_پیغام_2, پیغام_template);
	}

	original_extra_سرلیکونه = read_commit_extra_سرلیکونه(commit_with_پیغام,
							   exclude_gpgsig);

	ret = commit_ونه_extended(commit_پیغام_2.buf, commit_پیغام_2.len, new_ونه,
				   parents, &rewritten_commit_oid, original_author,
				   عدم, عدم, original_extra_سرلیکونه);
	إذا (ret < 0)
		اذهب بهر;

	*بهر = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

بهر:
	free_commit_extra_سرلیکونه(original_extra_سرلیکونه);
	strbuf_release(&commit_پیغام_2);
	حرر(original_author);
	repo_unuse_commit_بفر(repo, commit_with_پیغام, original_پیغام);
	أرجع ret;
}

ساكن صحيح لومړی_پلار_ونه_oid(بنية repository *repo,
				 بنية commit *commit,
				 بنية object_id *بهر)
{
	بنية commit *پلار = commit->parents ? commit->parents->توکی : عدم;

	إذا (!پلار) {
		oidcpy(بهر, repo->hash_algo->تشه_ونه);
		أرجع 0;
	}

	إذا (repo_parse_commit(repo, پلار))
		أرجع error(_("unable to parse parent commit %s"),
			     oid_منزل_hex(&پلار->object.oid));

	oidcpy(بهر, &repo_get_commit_ونه(repo, پلار)->object.oid);
	أرجع 0;
}

ساكن صحيح commit_ونه_with_edited_پیغام(بنية repository *repo,
					   ثابت حرف *action,
					   بنية commit *original,
					   بنية commit **بهر)
{
	بنية object_id پلار_ونه_oid;
	ثابت بنية object_id *ونه_oid;

	ونه_oid = &repo_get_commit_ونه(repo, original)->object.oid;

	إذا (لومړی_پلار_ونه_oid(repo, original, &پلار_ونه_oid) < 0)
		أرجع -1;

	أرجع commit_ونه_ext(repo, action, original, عدم, original->parents,
			       &پلار_ونه_oid, ونه_oid, بهر, COMMIT_ونه_EDIT_پیغام);
}

تعداد ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_سر,
};

ساكن صحيح parse_ref_action(ثابت بنية option *opt, ثابت حرف *ارزښت, صحيح unset)
{
	تعداد ref_action *action = opt->ارزښت;

	BUG_ON_OPT_NEG_NOARG(unset, ارزښت);
	إذا (!strcmp(ارزښت, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} وإلا إذا (!strcmp(ارزښت, "head")) {
		*action = REF_ACTION_سر;
	} وإلا {
		أرجع error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	أرجع 0;
}

ساكن صحيح revwalk_لري_merges(بنية repository *repo,
				   ثابت بنية strvec *revwalk_دلیلونه)
{
	بنية strvec دلیلونه = STRVEC_INIT;
	بنية rev_info revs;
	صحيح ret;

	strvec_pushv(&دلیلونه, revwalk_دلیلونه->v);
	strvec_ټېل(&دلیلونه, "--min-parents=2");

	repo_init_revisions(repo, &revs, عدم);

	تیاری_revisions_from_strvec(&دلیلونه, &revs, عدم);
	إذا (دلیلونه.nr != 1)
		BUG("revisions were set up with invalid argument");

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب بهر;
	}

	إذا (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		اذهب بهر;
	}

	reset_revision_walk();
	ret = 0;

بهر:
	release_revisions(&revs);
	strvec_clear(&دلیلونه);
	أرجع ret;
}

ساكن صحيح تیاری_revwalk(بنية repository *repo,
			 تعداد ref_action action,
			 بنية commit *original,
			 بنية rev_info *revs)
{
	بنية strvec دلیلونه = STRVEC_INIT;
	صحيح ret;

	repo_init_revisions(repo, revs, عدم);
	strvec_ټېل(&دلیلونه, "ignored");
	strvec_ټېل(&دلیلونه, "--reverse");
	strvec_ټېل(&دلیلونه, "--topo-order");
	strvec_ټېل(&دلیلونه, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&دلیلونه, "--ancestry-path=%s",
		     oid_منزل_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&دلیلونه, "^%s", oid_منزل_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	إذا (action == REF_ACTION_سر) {
		بنية commit_list *from_list = عدم;
		بنية commit *سر;

		سر = lookup_commit_reference_by_name("HEAD");
		إذا (!سر) {
			ret = error(_("cannot look up HEAD"));
			اذهب بهر;
		}

		commit_list_ورننباسه(original, &from_list);
		ret = repo_is_descendant_د(repo, سر, from_list);
		commit_list_free(from_list);

		إذا (ret < 0) {
			ret = error(_("cannot determine descendance"));
			اذهب بهر;
		} وإلا إذا (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			اذهب بهر;
		}

		strvec_ټېل(&دلیلونه, "HEAD");
	} وإلا {
		strvec_ټېل(&دلیلونه, "--branches");
		strvec_ټېل(&دلیلونه, "HEAD");
	}

	ret = revwalk_لري_merges(repo, &دلیلونه);
	إذا (ret < 0)
		اذهب بهر;

	تیاری_revisions_from_strvec(&دلیلونه, revs, عدم);
	إذا (دلیلونه.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

بهر:
	strvec_clear(&دلیلونه);
	أرجع ret;
}

ساكن صحيح handle_ref_update(بنية ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت بنية object_id *new_oid,
			     ثابت بنية object_id *old_oid,
			     ثابت حرف *reflog_پیغام,
			     بنية strbuf *err)
{
	إذا (!transaction) {
		چاپ_کړه("update %s %s %s\n",
		       refname, oid_منزل_hex(new_oid), oid_منزل_hex(old_oid));
		أرجع 0;
	}

	أرجع ref_transaction_update(transaction, refname, new_oid, old_oid,
				      عدم, عدم, 0, reflog_پیغام, err);
}

ساكن صحيح compute_pending_ref_updates(بنية rev_info *revs,
				       تعداد ref_action action,
				       بنية commit *original,
				       بنية commit *rewritten,
				       تعداد replay_تشه_commit_action تشه,
				       بنية replay_پایله *پایله)
{
	ثابت بنية name_decoration *decoration;
	بنية replay_revisions_options opts = {
		.تشه = تشه,
	};
	حرف hex[GIT_MAX_HEXSZ + 1];
	bool detached_سر;
	صحيح سر_flags = 0;
	صحيح ret;

	refs_read_ref_ډک(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, عدم, &سر_flags);
	detached_سر = !(سر_flags & REF_ISSYMREF);

	opts.onto = oid_منزل_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, پایله);
	إذا (ret)
		أرجع ret;

	إذا (action != REF_ACTION_BRANCHES && action != REF_ACTION_سر)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	لكل (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->بل)
	{
		إذا (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_سر)
			استمر;

		إذا (action == REF_ACTION_سر &&
		    decoration->type != DECORATION_REF_سر)
			استمر;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		إذا (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_سر &&
		    !detached_سر)
			استمر;

		replay_پایله_کتار_update(پایله, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	أرجع 0;
}

ساكن صحيح apply_pending_ref_updates(بنية repository *repo,
				     ثابت بنية replay_پایله *پایله,
				     ثابت حرف *reflog_پیغام,
				     صحيح dry_پرلپسې)
{
	بنية ref_transaction *transaction = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret;

	إذا (!dry_پرلپسې) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			اذهب بهر;
		}
	}

	لكل (اندازه_t i = 0; i < پایله->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					پایله->updates[i].refname,
					&پایله->updates[i].new_oid,
					&پایله->updates[i].old_oid,
					reflog_پیغام, &err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    پایله->updates[i].refname, err.buf);
			اذهب بهر;
		}
	}

	إذا (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		اذهب بهر;
	}

	ret = 0;

بهر:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	أرجع ret;
}

ساكن صحيح handle_reference_updates(بنية rev_info *revs,
				    تعداد ref_action action,
				    بنية commit *original,
				    بنية commit *rewritten,
				    ثابت حرف *reflog_پیغام,
				    صحيح dry_پرلپسې,
				    تعداد replay_تشه_commit_action تشه)
{
	بنية replay_پایله پایله = { 0 };
	صحيح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  تشه, &پایله);
	إذا (ret)
		اذهب بهر;

	ret = apply_pending_ref_updates(revs->repo, &پایله, reflog_پیغام, dry_پرلپسې);

بهر:
	replay_پایله_release(&پایله);
	أرجع ret;
}

ساكن صحيح commit_became_تشه(بنية repository *repo,
			       بنية commit *original,
			       بنية ونه *پایله)
{
	بنية object_id پلار_ونه_oid;

	إذا (لومړی_پلار_ونه_oid(repo, original, &پلار_ونه_oid) < 0)
		أرجع -1;

	أرجع oideq(&پایله->object.oid, &پلار_ونه_oid);
}

ساكن صحيح parse_opt_تشه(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد replay_تشه_commit_action *ارزښت = opt->ارزښت;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "drop"))
		*ارزښت = REPLAY_تشه_COMMIT_DROP;
	وإلا إذا (!strcmp(arg, "keep"))
		*ارزښت = REPLAY_تشه_COMMIT_KEEP;
	وإلا إذا (!strcmp(arg, "abort"))
		*ارزښت = REPLAY_تشه_COMMIT_ABORT;
	وإلا
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	أرجع 0;
}

ساكن صحيح cmd_history_fixup(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *مختاړی,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		عدم,
	};
	تعداد replay_تشه_commit_action تشه = REPLAY_تشه_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	تعداد commit_ونه_flags flags = 0;
	صحيح dry_پرلپسې = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پرلپسې,
			 N_("perform a dry-run without updating any refs")),
		OPT_بټ(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_ونه_EDIT_پیغام),
		OPT_CALLBACK_F(0, "empty", &تشه, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_تشه),
		OPT_پای_ټکی(),
	};
	بنية یوځای_پایله یوځای_پایله = { 0 };
	بنية یوځای_options یوځای_opts = { 0 };
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *سر_commit, *original, *rewritten;
	بنية ونه *سر_ونه, *original_ونه, *شاخص_ونه;
	بنية rev_info revs = { 0 };
	bool skip_commit = false;
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب بهر;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	إذا (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		اذهب بهر;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بهر;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	سر_commit = lookup_commit_reference_by_name("HEAD");
	إذا (!سر_commit) {
		ret = error(_("cannot look up HEAD"));
		اذهب بهر;
	}

	سر_ونه = repo_get_commit_ونه(repo, سر_commit);
	إذا (!سر_ونه) {
		ret = error(_("cannot get tree for HEAD"));
		اذهب بهر;
	}

	إذا (repo_read_شاخص(repo) < 0) {
		ret = error(_("unable to read index"));
		اذهب بهر;
	}

	إذا (!repo_شاخص_has_changes(repo, سر_ونه, عدم)) {
		ret = error(_("nothing to fixup: no staged changes"));
		اذهب بهر;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	شاخص_ونه = write_دننه_core_شاخص_as_ونه(repo, repo->شاخص);
	إذا (!شاخص_ونه) {
		ret = error(_("unable to write index as a tree"));
		اذهب بهر;
	}

	original_ونه = repo_get_commit_ونه(repo, original);
	إذا (!original_ونه) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		اذهب بهر;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_یوځای_options(&یوځای_opts, repo);
	یوځای_opts.ancestor = "HEAD";
	یوځای_opts.branch1 = argv[0];
	یوځای_opts.branch2 = "staged";
	یوځای_incore_nonrecursive(&یوځای_opts, سر_ونه,
				  original_ونه, شاخص_ونه, &یوځای_پایله);

	إذا (یوځای_پایله.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		اذهب بهر;
	}

	إذا (!یوځای_پایله.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		اذهب بهر;
	}

	ret = commit_became_تشه(repo, original, یوځای_پایله.ونه);
	إذا (ret < 0)
		اذهب بهر;
	إذا (ret > 0) {
		بدل (تشه) {
		حالة REPLAY_تشه_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->توکی : عدم;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			إذا (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				اذهب بهر;
			}

			skip_commit = true;
			اكسر;
		حالة REPLAY_تشه_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			اكسر;
		حالة REPLAY_تشه_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			اذهب بهر;
		}
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب بهر;

	إذا (!skip_commit) {
		ret = commit_ونه_ext(repo, "fixup", original, عدم, original->parents,
				      &original_ونه->object.oid, &یوځای_پایله.ونه->object.oid,
				      &rewritten, flags);
		إذا (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			اذهب بهر;
		}
	}

	strbuf_addf(&reflog_پیغام, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_پرلپسې, تشه);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بهر;
	}

	ret = 0;

بهر:
	یوځای_finalize(&یوځای_opts, &یوځای_پایله);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح cmd_history_reword(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *مختاړی,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پرلپسې = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پرلپسې,
			 N_("perform a dry-run without updating any refs")),
		OPT_پای_ټکی(),
	};
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب بهر;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بهر;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب بهر;

	ret = commit_ونه_with_edited_پیغام(repo, "reworded", original, &rewritten);
	إذا (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		اذهب بهر;
	}

	strbuf_addf(&reflog_پیغام, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_پرلپسې, REPLAY_تشه_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بهر;
	}

	ret = 0;

بهر:
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح write_ondisk_شاخص(بنية repository *repo,
			      بنية object_id *oid,
			      ثابت حرف *لار)
{
	بنية unpack_trees_options opts = { 0 };
	بنية قلف_file قلف = قلف_INIT;
	بنية ونه_desc ونه_desc;
	بنية شاخص_state شاخص;
	بنية ونه *ونه;
	صحيح ret;

	شاخص_state_init(&شاخص, repo);

	opts.سر_idx = -1;
	opts.src_شاخص = &شاخص;
	opts.dst_شاخص = &شاخص;

	ونه = repo_parse_ونه_غیرمستقیم(repo, oid);
	إذا (!ونه) {
		ret = error(_("unable to parse tree %s"), oid_منزل_hex(oid));
		اذهب بهر;
	}
	init_ونه_desc(&ونه_desc, &ونه->object.oid, ونه->بفر, ونه->اندازه);

	إذا (unpack_trees(1, &ونه_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		اذهب بهر;
	}

	اولي_cache_ونه(repo, &شاخص, ونه);

	إذا (repo_hold_قلف_file_for_update(repo, &قلف, لار, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		اذهب بهر;
	}

	إذا (write_locked_شاخص(&شاخص, &قلف, COMMIT_قلف)) {
		ret = error(_("unable to write new index file"));
		اذهب بهر;
	}

	ret = 0;

بهر:
	rollback_قلف_file(&قلف);
	release_شاخص(&شاخص);
	أرجع ret;
}

ساكن صحيح وېشل_commit(بنية repository *repo,
			بنية commit *original,
			بنية pathspec *pathspec,
			بنية commit **بهر)
{
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية strbuf شاخص_file = STRBUF_INIT;
	بنية شاخص_state شاخص = شاخص_STATE_INIT(repo);
	ثابت بنية object_id *original_commit_ونه_oid;
	ثابت بنية object_id *old_ونه_oid, *new_ونه_oid;
	بنية object_id پلار_ونه_oid;
	حرف original_commit_oid[GIT_MAX_HEXSZ + 1];
	بنية commit *لومړی_commit, *دوهم_commit;
	بنية commit_list *parents = عدم;
	بنية ونه *وېشل_ونه;
	صحيح ret;

	إذا (لومړی_پلار_ونه_oid(repo, original, &پلار_ونه_oid) < 0) {
		ret = -1;
		اذهب بهر;
	}
	original_commit_ونه_oid = get_commit_ونه_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_لار_replace(repo, &شاخص_file, "%s", "history-split.index");

	ret = write_ondisk_شاخص(repo, &پلار_ونه_oid, شاخص_file.buf);
	إذا (ret < 0)
		اذهب بهر;

	ret = read_شاخص_from(&شاخص, شاخص_file.buf, repo->gitdir);
	إذا (ret < 0) {
		ret = error(_("failed reading temporary index"));
		اذهب بهر;
	}

	oid_منزل_hex_r(original_commit_oid, &original->object.oid);
	ret = پرلپسې_جمع_کړه_p_شاخص(repo, &شاخص, شاخص_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, جمع_کړه_P_DISALLOW_EDIT);
	إذا (ret < 0)
		اذهب بهر;

	وېشل_ونه = write_دننه_core_شاخص_as_ونه(repo, &شاخص);
	إذا (!وېشل_ونه) {
		ret = error(_("failed split tree"));
		اذهب بهر;
	}

	unlink(شاخص_file.buf);
	strbuf_release(&شاخص_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	إذا (oideq(&وېشل_ونه->object.oid, &پلار_ونه_oid)) {
		ret = error(_("split commit is empty"));
		اذهب بهر;
	} وإلا إذا (oideq(&وېشل_ونه->object.oid, original_commit_ونه_oid)) {
		ret = error(_("split commit tree matches original commit"));
		اذهب بهر;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_ونه_ext(repo, "split-out", original, عدم, original->parents, &پلار_ونه_oid,
			      &وېشل_ونه->object.oid, &لومړی_commit, COMMIT_ونه_EDIT_پیغام);
	إذا (ret < 0) {
		ret = error(_("failed writing first commit"));
		اذهب بهر;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(لومړی_commit, &parents);

	old_ونه_oid = &repo_get_commit_ونه(repo, لومړی_commit)->object.oid;
	new_ونه_oid = &repo_get_commit_ونه(repo, original)->object.oid;

	ret = commit_ونه_ext(repo, "split-out", original, عدم, parents, old_ونه_oid,
			      new_ونه_oid, &دوهم_commit, COMMIT_ونه_EDIT_پیغام);
	إذا (ret < 0) {
		ret = error(_("failed writing second commit"));
		اذهب بهر;
	}

	*بهر = دوهم_commit;
	ret = 0;

بهر:
	إذا (شاخص_file.len)
		unlink(شاخص_file.buf);
	strbuf_release(&شاخص_file);
	commit_list_free(parents);
	release_شاخص(&شاخص);
	أرجع ret;
}

ساكن صحيح cmd_history_وېشل(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *مختاړی,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_وېشل_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پرلپسې = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پرلپسې,
			 N_("perform a dry-run without updating any refs")),
		OPT_پای_ټکی(),
	};
	بنية commit *original, *rewritten = عدم;
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية pathspec pathspec = { 0 };
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	إذا (argc < 1) {
		ret = error(_("command expects a committish"));
		اذهب بهر;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ډک |
		       PATHSPEC_SYMLINK_LEADING_لار |
		       PATHSPEC_مختاړی_ORIGIN,
		       مختاړی, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بهر;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret < 0)
		اذهب بهر;

	إذا (original->parents && original->parents->بل) {
		ret = error(_("cannot split up merge commit"));
		اذهب بهر;
	}

	ret = وېشل_commit(repo, original, &pathspec, &rewritten);
	إذا (ret < 0)
		اذهب بهر;

	strbuf_addf(&reflog_پیغام, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_پرلپسې, REPLAY_تشه_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بهر;
	}

	ret = 0;

بهر:
	strbuf_release(&reflog_پیغام);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	أرجع ret;
}

#تعریف_کړه SQUASH_کتل_شوی (1u << 11)
#تعریف_کړه SQUASH_TIP (1u << 12)
#تعریف_کړه SQUASH_AMEND_موخه (1u << 13)

ساكن bool is_autosquash_subject(ثابت حرف *s)
{
	أرجع starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ساكن bool skip_one_autosquash_مختاړی(ثابت حرف *s, ثابت حرف **بهر)
{
	إذا (skip_مختاړی(s, "amend!", بهر) || skip_مختاړی(s, "fixup!", بهر) ||
	    skip_مختاړی(s, "squash!", بهر)) {
		طالما (**بهر == ' ')
			(*بهر)++;
		أرجع true;
	}
	أرجع false;
}

ساكن فراغ truncate_پیغام_منزل_subject(بنية strbuf *پیغام)
{
	ثابت حرف *eos = مزي_پلټنه(پیغام->buf, "\n\n");

	إذا (eos)
		strbuf_setlen(پیغام, eos - پیغام->buf + 1);
}

بنية subject_معلومات {
	بنية strintmap subjects;
	بنية strbuf subject;
	بنية strbuf squash_پیغام;
	ثابت حرف *پیغام_2;
	bool edit_پیغام;
};

#تعریف_کړه SUBJECT_معلومات_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_پیغام = STRBUF_INIT,	\
}

ساكن فراغ subject_معلومات_clear(بنية subject_معلومات *معلومات)
{
	strintmap_clear(&معلومات->subjects);
	strbuf_release(&معلومات->subject);
	strbuf_release(&معلومات->squash_پیغام);
}

ساكن صحيح squash_amend_پیغام(بنية repository *repo,
				بنية commit *commit,
				بنية subject_معلومات *معلومات,
				بینښه flags)
{
	ثابت حرف *body = معلومات->پیغام_2 + معلومات->subject.len;

	طالما (که_تشه(*body))
		body++;

	إذا (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_پیدا_کړه_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			معلومات->subject.buf);
		أرجع 0;
	}

	إذا (معلومات->edit_پیغام) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_موخه) {
		إذا (starts_with(معلومات->squash_پیغام.buf, "squash!"))
			أرجع error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_پیدا_کړه_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     معلومات->subject.buf);
		إذا (starts_with(معلومات->squash_پیغام.buf, "fixup!"))
			strbuf_splice(&معلومات->squash_پیغام, 0, 5, "amend!", 5);
		إذا (starts_with(معلومات->squash_پیغام.buf, "amend!")) {
			truncate_پیغام_منزل_subject(&معلومات->squash_پیغام);
			strbuf_addch(&معلومات->squash_پیغام, '\n');
		} وإلا {
			strbuf_reset(&معلومات->squash_پیغام);
		}
		strbuf_addstr(&معلومات->squash_پیغام, body);
		strbuf_complete_کرښه(&معلومات->squash_پیغام);
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_پیدا_کړه_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     معلومات->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_squash_پیغام(بنية repository *repo,
				بنية commit *commit,
				بنية subject_معلومات *معلومات,
				بینښه flags)
{
	ثابت حرف *body = معلومات->پیغام_2 + معلومات->subject.len;

	طالما (که_تشه(*body))
		body++;

	إذا (معلومات->edit_پیغام) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_موخه) {
		إذا (starts_with(معلومات->squash_پیغام.buf, "fixup!")) {
			truncate_پیغام_منزل_subject(&معلومات->squash_پیغام);
			strbuf_splice(&معلومات->squash_پیغام, 0, 5, "squash", 6);
		}
		إذا (starts_with(معلومات->squash_پیغام.buf, "squash!")) {
			strbuf_addch(&معلومات->squash_پیغام, '\n');
			strbuf_addstr(&معلومات->squash_پیغام, body);
			strbuf_complete_کرښه(&معلومات->squash_پیغام);
		} وإلا {
			أرجع error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_پیدا_کړه_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     معلومات->subject.buf);
		}
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_پیدا_کړه_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      معلومات->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_check_can_autosquash(بنية repository *repo,
				       بنية commit *commit,
				       بنية subject_معلومات *معلومات,
				       بینښه flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_موخه;
	إذا (starts_with(معلومات->subject.buf, "amend!"))
		أرجع squash_amend_پیغام(repo, commit, معلومات, flags);
	وإلا إذا (starts_with(معلومات->subject.buf, "squash!"))
		أرجع squash_squash_پیغام(repo, commit, معلومات, flags);

	أرجع 0;
}

ساكن صحيح squash_check_autosquash_subject(بنية repository *repo,
					   بنية commit *commit,
					   بنية subject_معلومات *معلومات)
{
	ثابت حرف* s = معلومات->subject.buf;
	بنية commit *موخه;
	بنية hashmap_iter iter;
	بنية strmap_ثبت *ثبت;
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
	طالما (skip_one_autosquash_مختاړی(s, &s)) {
		بینښه flags = strintmap_get(&معلومات->subjects, s);
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit, معلومات, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	موخه = lookup_commit_reference_by_name(s);
	إذا (موخه && starts_with(oid_منزل_hex(&موخه->object.oid), s)) {
		بینښه flags =
			موخه->object.flags & (SQUASH_کتل_شوی | SQUASH_AMEND_موخه);
		إذا (!flags) {
			ثابت حرف *subject_پیل;
			ثابت حرف *بفر = repo_logmsg_reencode(repo, موخه,
								  عدم, عدم);
			اندازه_t subject_len = پیدا_کړه_commit_subject(بفر,
								 &subject_پیل);
			حرف *subject = xmemdupz(subject_پیل, subject_len);

			flags = strintmap_get(&معلومات->subjects, subject);
			حرر(subject);
			repo_unuse_commit_بفر(repo, موخه, بفر);
		}
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit,
							   معلومات, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_ثبت(&معلومات->subjects, &iter, ثبت) {
		s = معلومات->subject.buf;
		طالما(skip_one_autosquash_مختاړی(s, &s)) {
			إذا (starts_with(ثبت->کلی, s)) {
				بینښه ارزښت = (intptr_t)ثبت->ارزښت;

				أرجع squash_check_can_autosquash(repo, commit,
								   معلومات, ارزښت);
			}
		}
	}
	أرجع error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_پیدا_کړه_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       معلومات->subject.buf);
}

ساكن صحيح squash_check_subject(بنية repository *repo,
				بنية commit *commit,
				بنية subject_معلومات *معلومات)
{
	صحيح ret = 0;
	ثابت حرف *buf = repo_logmsg_reencode(repo, commit, عدم, عدم);
	اندازه_t subject_len = پیدا_کړه_commit_subject(buf, &معلومات->پیغام_2);

	strbuf_reset(&معلومات->subject);
	strbuf_جمع_کړه(&معلومات->subject, معلومات->پیغام_2, subject_len);

	إذا (!strintmap_get_اندازه(&معلومات->subjects)) {
		ثابت حرف *s;

		strbuf_addstr(&معلومات->squash_پیغام, معلومات->پیغام_2);
		strbuf_complete_کرښه(&معلومات->squash_پیغام);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = معلومات->subject.buf;
		skip_one_autosquash_مختاړی(s, &s);
		strintmap_سېټ(&معلومات->subjects, s, SQUASH_AMEND_موخه | SQUASH_کتل_شوی);
		commit->object.flags |= SQUASH_AMEND_موخه;
	} وإلا إذا (is_autosquash_subject(معلومات->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, معلومات);
	} وإلا {
		strintmap_سېټ(&معلومات->subjects, معلومات->subject.buf, SQUASH_کتل_شوی);
	}
	repo_unuse_commit_بفر(repo, commit, buf);
	أرجع ret;
}

ساكن صحيح build_squash_پیغام(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *بهر);

ساكن صحيح تیاری_squash_revisions(بنية repository *repo,
				  صحيح argc, ثابت حرف **argv,
				  بنية rev_info *revs)
{
	repo_init_revisions(repo, revs, عدم);
	revs->سرچپه = 1;
	revs->topo_ترتیب = 1;
	revs->ترتیبول_ترتیب = REV_ترتیبول_دننه_ګراف_ترتیب;
	revs->simplify_history = 0;
	revs->ancestry_لار = 1;
	revs->limited = 1;
	revs->ancestry_لار_implicit_bottoms = 1;

	argc = تیاری_revisions(argc, argv, revs, عدم);
	إذا (argc > 1)
		أرجع error(_("unrecognized argument: %s"), argv[1]);

	إذا (revs->سرچپه != 1 || revs->topo_ترتیب != 1 ||
	    revs->ترتیبول_ترتیب != REV_ترتیبول_دننه_ګراف_ترتیب ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_لار != 1 || revs->limited != 1 ||
	    revs->ancestry_لار_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->سرچپه = 1;
		revs->topo_ترتیب = 1;
		revs->ترتیبول_ترتیب = REV_ترتیبول_دننه_ګراف_ترتیب;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_لار = 1;
		revs->limited = 1;
		revs->ancestry_لار_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	لكل (اندازه_t i = 0; i < revs->cmdline.nr; i++)
		إذا (revs->cmdline.rev[i].flags & BOTTOM)
			أرجع 0;

	أرجع error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ساكن صحيح resolve_squash_range(بنية repository *repo,
				bool update_branches,
				bool edit_پیغام,
				صحيح argc, ثابت حرف **argv,
				بنية commit **oldest_بهر,
				بنية commit **tip_بهر,
				حرف **پیغام_بهر)
{
	بنية rev_info revs;
	بنية subject_معلومات subject_معلومات = SUBJECT_معلومات_INIT;
	بنية commit *commit, *oldest = عدم, *tip = عدم;
	بنية strbuf todo_buf = STRBUF_INIT;
	صحيح ret, tip_شمېر = 0;
	bool walk_started = false;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_array refs = { 0 };

	subject_معلومات.edit_پیغام = edit_پیغام;
	ret = تیاری_squash_revisions(repo, argc, argv, &revs);
	إذا (ret < 0)
		اذهب بهر;

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب بهر;
	}
	walk_started = true;
	طالما ((commit = get_revision(&revs))) {
		بنية commit_list *p;

		إذا (edit_پیغام)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_منزل_hex(&commit->object.oid));

		إذا (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			اذهب بهر;
		}
		لكل (p = commit->parents; oldest && p; p = p->بل) {
			بنية commit_list *q;
			بنية object *o;
			bool کتل_شوی;

			إذا (repo_parse_commit(repo, p->توکی)) {
				ret = error(_("cannot parse commit"));
				اذهب بهر;
			}
			o = &p->توکی->object;
			کتل_شوی = o->flags & SQUASH_کتل_شوی;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			لكل (q = oldest->parents; !کتل_شوی && q; q = q->بل) {
				إذا (p->توکی == q->توکی) {
					کتل_شوی = true;
					commit_list_ورننباسه(commit, &filter.with_commit);
				}
			}
			إذا (!کتل_شوی) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_پیدا_کړه_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_پیدا_کړه_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				اذهب بهر;
			}
			إذا (o->flags & SQUASH_TIP) {
				tip_شمېر--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		إذا (!oldest) {
			commit_list_ورننباسه(commit, &filter.with_commit);
			oldest = commit;
		}
		إذا (squash_check_subject(repo, commit, &subject_معلومات)) {
			ret = -1;
			اذهب بهر;
		}
		tip = commit;
		tip->object.flags |= SQUASH_کتل_شوی | SQUASH_TIP;
		tip_شمېر++;
	}
	clear_object_flags(repo, SQUASH_کتل_شوی | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	إذا (!tip_شمېر) {
		ret = error(_("the revision range is empty"));
		اذهب بهر;
	} وإلا إذا (tip_شمېر != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		اذهب بهر;
	} وإلا إذا (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		اذهب بهر;
	} وإلا إذا (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_ورننباسه(tip, &filter.no_commit);
	filter.ډول = FILTER_REFS_BRANCHES;
	إذا (update_branches &&
	    filter_refs(&refs, &filter, filter.ډول)) {
		ret = error(_("could not filter refs"));
		اذهب بهر;
	}
	إذا (refs.nr) {
		بنية ref_format format = REF_FORMAT_INIT;
		بنية ref_ترتیبونه *ترتیبونه;
		بنية string_list ترتیبونه_options = STRING_LIST_INIT_DUP;
		بنية strbuf branches = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		إذا (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&ترتیبونه_options, "refname");
		ترتیبونه = ref_ترتیبونه_options(&ترتیبونه_options);
		ref_array_ترتیبول(ترتیبونه, &refs);
		لكل (صحيح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			إذا (format_ref_array_توکی(refs.توکي[i], &format,
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
		ref_ترتیبونه_release(ترتیبونه);
		string_list_clear(&ترتیبونه_options, 0);
		اذهب بهر;
	}
	إذا (edit_پیغام) {
		strbuf_reset(&subject_معلومات.squash_پیغام);
		ret = build_squash_پیغام(repo, &todo_buf,
					   &subject_معلومات.squash_پیغام);
		إذا (ret < 0)
			اذهب بهر;
	}

	*oldest_بهر = oldest;
	*tip_بهر = tip;
	*پیغام_بهر = strbuf_detach(&subject_معلومات.squash_پیغام, عدم);
	ret = 0;

بهر:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_کتل_شوی | SQUASH_TIP |
			   SQUASH_AMEND_موخه);
	إذا (walk_started)
		reset_revision_walk();
	subject_معلومات_clear(&subject_معلومات);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	أرجع ret;
}

ساكن bool amend_replaces_موخه(بنية todo_list *todo, صحيح موخه)
{
	لكل (صحيح i = موخه + 1; i < todo->nr &&
				 todo->توکي[i].command != TODO_PICK; i++) {
		إذا (todo->توکي[i].command == TODO_SQUASH)
			أرجع false;
		إذا (todo->توکي[i].flags & TODO_REPLACE_FIXUP_پیغام)
			أرجع true;
	}
	أرجع false;
}

ساكن صحيح build_squash_پیغام(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *بهر)
{
	بنية todo_list todo = TODO_LIST_INIT;
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح nr_commits, ret;

	إذا (todo_list_parse_insn_بفر(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		اذهب بهر;
	}

	nr_commits = todo.nr;
	لكل (صحيح i = 0; i < nr_commits; i++) {
		بنية todo_توکی *توکی = &todo.توکي[i];
		ثابت حرف *پیغام_2, *body;
		اندازه_t commented_len;
		bool skip, squashing;

		squashing = توکی->command == TODO_SQUASH ||
			    (توکی->flags & TODO_REPLACE_FIXUP_پیغام);
		إذا (توکی->command == TODO_PICK)
			skip = amend_replaces_موخه(&todo, i);
		وإلا
			skip = !squashing;

		پیغام_2 = repo_logmsg_reencode(repo, توکی->commit, عدم, عدم);
		پیدا_کړه_commit_subject(پیغام_2, &body);

		إذا (skip)
			commented_len = مزي_اوږدوالی(body);
		وإلا إذا (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		وإلا
			commented_len = 0;

		إذا (!i)
			جمع_کړه_squash_combination_سرلیک(بهر, nr_commits);
		strbuf_addch(بهر, '\n');
		جمع_کړه_squash_پیغام_سرلیک(بهر, i + 1, skip);
		strbuf_addstr(بهر, "\n\n");
		strbuf_جمع_کړه_commented_کرښې(بهر, body, commented_len, comment_کرښه_str);
		strbuf_addstr(بهر, body + commented_len);
		strbuf_complete_کرښه(بهر);

		repo_unuse_commit_بفر(repo, توکی->commit, پیغام_2);
	}

	ret = 0;

بهر:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	أرجع ret;
}

ساكن صحيح cmd_history_squash(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *مختاړی,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پرلپسې = 0;
	صحيح edit = 1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پرلپسې,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_پای_ټکی(),
	};
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *oldest, *tip, *rewritten;
	ثابت بنية object_id *بنسټ_ونه_oid, *tip_ونه_oid;
	حرف *پیغام_template = عدم;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	إذا (argc < 2) {
		ret = error(_("command expects a revision range"));
		اذهب بهر;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_پیغام, "squash: updating ");
	strbuf_join_argv(&reflog_پیغام, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &پیغام_template);
	إذا (ret < 0)
		اذهب بهر;

	ret = تیاری_revwalk(repo, action, tip, &revs);
	إذا (ret < 0)
		اذهب بهر;

	بنسټ_ونه_oid = &repo_get_commit_ونه(repo,
					oldest->parents->توکی)->object.oid;
	tip_ونه_oid = &repo_get_commit_ونه(repo, tip)->object.oid;

	ret = commit_ونه_ext(repo, "squash", oldest, پیغام_template,
			      oldest->parents, بنسټ_ونه_oid, tip_ونه_oid,
			      &rewritten,
			      edit ? COMMIT_ونه_EDIT_پیغام : 0);
	إذا (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		اذهب بهر;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_پیغام.buf, dry_پرلپسې,
				       REPLAY_تشه_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بهر;
	}

	ret = 0;

بهر:
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	حرر(پیغام_template);
	أرجع ret;
}

ساكن صحيح update_worktree(بنية repository *repo,
			   ثابت بنية commit *old_سر,
			   ثابت بنية commit *new_سر,
			   bool dry_پرلپسې)
{
	بنية reset_working_ونه_options opts = {
		.oid_from = &old_سر->object.oid,
		.oid = &new_سر->object.oid,
	};
	إذا (dry_پرلپسې)
		opts.flags |= RESET_WORKING_ونه_DRY_پرلپسې;
	أرجع reset_working_ونه(repo, &opts);
}

ساكن صحيح پیدا_کړه_سر_ونه_change(بنية repository *repo,
				 ثابت بنية replay_پایله *پایله,
				 بنية commit **old_سر,
				 بنية commit **new_سر,
				 bool *بدل_شوی)
{
	ثابت بنية replay_ref_update *سر_update = عدم;
	بنية commit *old_سر_commit, *new_سر_commit;
	بنية ونه *old_سر_ونه, *new_سر_ونه;
	ثابت حرف *سر_موخه;
	صحيح سر_flags;

	*بدل_شوی = false;

	سر_موخه = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      عدم, &سر_flags);
	إذا (!سر_موخه)
		أرجع error(_("cannot look up HEAD"));

	لكل (اندازه_t i = 0; i < پایله->updates_nr; i++) {
		إذا (!strcmp(پایله->updates[i].refname, سر_موخه)) {
			سر_update = &پایله->updates[i];
			اكسر;
		}
	}

	إذا (!سر_update)
		أرجع 0;

	old_سر_commit = lookup_commit_reference(repo, &سر_update->old_oid);
	new_سر_commit = lookup_commit_reference(repo, &سر_update->new_oid);
	إذا (!old_سر_commit || !new_سر_commit)
		أرجع error(_("cannot resolve HEAD commit"));

	old_سر_ونه = repo_get_commit_ونه(repo, old_سر_commit);
	new_سر_ونه = repo_get_commit_ونه(repo, new_سر_commit);
	إذا (!old_سر_ونه || !new_سر_ونه)
		أرجع error(_("cannot resolve tree for HEAD"));

	إذا (oideq(&old_سر_ونه->object.oid, &new_سر_ونه->object.oid))
		أرجع 0;

	*old_سر = old_سر_commit;
	*new_سر = new_سر_commit;
	*بدل_شوی = true;

	أرجع 0;
}

ساكن صحيح cmd_history_drop(صحيح argc,
			    ثابت حرف **argv,
			    ثابت حرف *مختاړی,
			    بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		عدم,
	};
	تعداد replay_تشه_commit_action تشه = REPLAY_تشه_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پرلپسې = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پرلپسې,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &تشه, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_تشه),
		OPT_پای_ټکی(),
	};
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	بنية replay_پایله پایله = { 0 };
	بنية commit *old_سر, *new_سر;
	bool سر_حرکتونه = false;
	صحيح ret;

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب بهر;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بهر;
	}

	إذا (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		اذهب بهر;
	} وإلا إذا (original->parents->بل) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		اذهب بهر;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب بهر;

	rewritten = original->parents->توکی;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  تشه, &پایله);
	إذا (ret) {
		ret = error(_("failed replaying descendants"));
		اذهب بهر;
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
	إذا (!is_bare_repository(repo)) {
		ret = پیدا_کړه_سر_ونه_change(repo, &پایله, &old_سر,
					    &new_سر, &سر_حرکتونه);
		إذا (ret < 0)
			اذهب بهر;

		إذا (سر_حرکتونه && update_worktree(repo, old_سر, new_سر, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			اذهب بهر;
		}
	}

	strbuf_addf(&reflog_پیغام, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &پایله, reflog_پیغام.buf, dry_پرلپسې);
	إذا (ret < 0) {
		ret = error(_("failed to update references"));
		اذهب بهر;
	}

	إذا (!dry_پرلپسې && سر_حرکتونه && update_worktree(repo, old_سر, new_سر, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_منزل_hex(&new_سر->object.oid));
		اذهب بهر;
	}

	ret = 0;

بهر:
	replay_پایله_release(&پایله);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	أرجع ret;
}

صحيح cmd_history(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *مختاړی,
		بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_وېشل_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_وېشل),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_پای_ټکی(),
	};

	argc = parse_options(argc, argv, مختاړی, options, usage, 0);
	أرجع fn(argc, argv, مختاړی, repo);
}
