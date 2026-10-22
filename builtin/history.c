#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کن GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#تعریف_کن GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#تعریف_کن GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#تعریف_کن GIT_HISTORY_تقسیم_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#تعریف_کن GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ساكن فراغ change_داده_free(فراغ *util, ثابت حرف *str استفادهنشده)
{
	بنية wt_وضعیت_change_داده *d = util;
	حرر(d->rename_منبع);
	حرر(d);
}

ساكن صحيح fill_commit_پیام(بنية repository *repo,
			       ثابت بنية object_id *old_درخت,
			       ثابت بنية object_id *new_درخت,
			       ثابت حرف *default_پیام,
			       ثابت حرف *action,
			       بنية strbuf *بیرون)
{
	ثابت حرف *مسیر = git_مسیر_commit_editmsg();
	ثابت حرف *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	بنية wt_وضعیت s;

	wt_وضعیت_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_سریع;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_پیشوند = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(مسیر, "w");
	إذا (!s.fp)
		أرجع error_errno(_("could not open '%s'"), مسیر);

	strbuf_addstr(بیرون, default_پیام);
	strbuf_addch(بیرون, '\n');
	strbuf_commented_addf(بیرون, comment_سطر_str, hint, action, comment_سطر_str);
	إذا (fwrite(بیرون->buf, 1, بیرون->len, s.fp) != بیرون->len)
		die_errno(_("could not write to '%s'"), مسیر);

	wt_وضعیت_collect_changes_trees(&s, old_درخت, new_درخت);
	wt_وضعیت_print(&s);
	wt_وضعیت_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_داده_free);
	إذا (fclose(s.fp))
		die_errno(_("could not write to '%s'"), مسیر);

	strbuf_reset(بیرون);
	إذا (launch_editor(مسیر, بیرون, عدم)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		أرجع -1;
	}
	strbuf_stripspace(بیرون, comment_سطر_str);

	cleanup_پیام(بیرون, COMMIT_پیام_CLEANUP_ALL, 0);

	إذا (!بیرون->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		أرجع -1;
	}

	أرجع 0;
}

تعداد commit_درخت_flags {
	COMMIT_درخت_EDIT_پیام = (1 << 0),
};

ساكن صحيح commit_درخت_ext(بنية repository *repo,
			   ثابت حرف *action,
			   بنية commit *commit_with_پیام,
			   ثابت حرف *پیام_template,
			   ثابت بنية commit_list *parents,
			   ثابت بنية object_id *old_درخت,
			   ثابت بنية object_id *new_درخت,
			   بنية commit **بیرون,
			   تعداد commit_درخت_flags flags)
{
	ثابت حرف *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		عدم,
	};
	ثابت حرف *original_پیام, *original_body, *ptr;
	بنية commit_extra_سرآیند *original_extra_سرآیندها = عدم;
	بنية strbuf commit_پیام_2 = STRBUF_INIT;
	بنية object_id rewritten_commit_oid;
	حرف *original_author = عدم;
	اندازه_t len;
	صحيح ret;

	/* We retain authorship of the original commit. */
	original_پیام = repo_logmsg_reencode(repo, commit_with_پیام, عدم, عدم);
	ptr = پیدا_کن_commit_سرآیند(original_پیام, "author", &len);
	إذا (ptr)
		original_author = xmemdupz(ptr, len);
	پیدا_کن_commit_subject(original_پیام, &original_body);

	إذا (!پیام_template)
		پیام_template = original_body;

	إذا (flags & COMMIT_درخت_EDIT_پیام) {
		ret = fill_commit_پیام(repo, old_درخت, new_درخت,
					  پیام_template, action, &commit_پیام_2);
		إذا (ret < 0)
			اذهب بیرون;
	} وإلا {
		strbuf_addstr(&commit_پیام_2, پیام_template);
	}

	original_extra_سرآیندها = read_commit_extra_سرآیندها(commit_with_پیام,
							   exclude_gpgsig);

	ret = commit_درخت_extended(commit_پیام_2.buf, commit_پیام_2.len, new_درخت,
				   parents, &rewritten_commit_oid, original_author,
				   عدم, عدم, original_extra_سرآیندها);
	إذا (ret < 0)
		اذهب بیرون;

	*بیرون = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

بیرون:
	free_commit_extra_سرآیندها(original_extra_سرآیندها);
	strbuf_release(&commit_پیام_2);
	حرر(original_author);
	repo_unuse_commit_میانگیر(repo, commit_with_پیام, original_پیام);
	أرجع ret;
}

ساكن صحيح اولی_والد_درخت_oid(بنية repository *repo,
				 بنية commit *commit,
				 بنية object_id *بیرون)
{
	بنية commit *والد = commit->parents ? commit->parents->قلم : عدم;

	إذا (!والد) {
		oidcpy(بیرون, repo->hash_algo->خالی_درخت);
		أرجع 0;
	}

	إذا (repo_parse_commit(repo, والد))
		أرجع error(_("unable to parse parent commit %s"),
			     oid_مقصد_hex(&والد->object.oid));

	oidcpy(بیرون, &repo_get_commit_درخت(repo, والد)->object.oid);
	أرجع 0;
}

ساكن صحيح commit_درخت_with_edited_پیام(بنية repository *repo,
					   ثابت حرف *action,
					   بنية commit *original,
					   بنية commit **بیرون)
{
	بنية object_id والد_درخت_oid;
	ثابت بنية object_id *درخت_oid;

	درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	إذا (اولی_والد_درخت_oid(repo, original, &والد_درخت_oid) < 0)
		أرجع -1;

	أرجع commit_درخت_ext(repo, action, original, عدم, original->parents,
			       &والد_درخت_oid, درخت_oid, بیرون, COMMIT_درخت_EDIT_پیام);
}

تعداد ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_سر,
};

ساكن صحيح parse_ref_action(ثابت بنية option *opt, ثابت حرف *مقدار, صحيح unset)
{
	تعداد ref_action *action = opt->مقدار;

	BUG_ON_OPT_NEG_NOARG(unset, مقدار);
	إذا (!strcmp(مقدار, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} وإلا إذا (!strcmp(مقدار, "head")) {
		*action = REF_ACTION_سر;
	} وإلا {
		أرجع error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	أرجع 0;
}

ساكن صحيح revwalk_شامل_است_merges(بنية repository *repo,
				   ثابت بنية strvec *revwalk_آرگومانها)
{
	بنية strvec آرگومانها = STRVEC_INIT;
	بنية rev_info revs;
	صحيح ret;

	strvec_pushv(&آرگومانها, revwalk_آرگومانها->v);
	strvec_افزودن(&آرگومانها, "--min-parents=2");

	repo_init_revisions(repo, &revs, عدم);

	راهاندازی_revisions_from_strvec(&آرگومانها, &revs, عدم);
	إذا (آرگومانها.nr != 1)
		BUG("revisions were set up with invalid argument");

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب بیرون;
	}

	إذا (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		اذهب بیرون;
	}

	reset_revision_walk();
	ret = 0;

بیرون:
	release_revisions(&revs);
	strvec_clear(&آرگومانها);
	أرجع ret;
}

ساكن صحيح راهاندازی_revwalk(بنية repository *repo,
			 تعداد ref_action action,
			 بنية commit *original,
			 بنية rev_info *revs)
{
	بنية strvec آرگومانها = STRVEC_INIT;
	صحيح ret;

	repo_init_revisions(repo, revs, عدم);
	strvec_افزودن(&آرگومانها, "ignored");
	strvec_افزودن(&آرگومانها, "--reverse");
	strvec_افزودن(&آرگومانها, "--topo-order");
	strvec_افزودن(&آرگومانها, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&آرگومانها, "--ancestry-path=%s",
		     oid_مقصد_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&آرگومانها, "^%s", oid_مقصد_hex(&original->object.oid));

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
			اذهب بیرون;
		}

		commit_list_درج(original, &from_list);
		ret = repo_is_descendant_از(repo, سر, from_list);
		commit_list_free(from_list);

		إذا (ret < 0) {
			ret = error(_("cannot determine descendance"));
			اذهب بیرون;
		} وإلا إذا (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			اذهب بیرون;
		}

		strvec_افزودن(&آرگومانها, "HEAD");
	} وإلا {
		strvec_افزودن(&آرگومانها, "--branches");
		strvec_افزودن(&آرگومانها, "HEAD");
	}

	ret = revwalk_شامل_است_merges(repo, &آرگومانها);
	إذا (ret < 0)
		اذهب بیرون;

	راهاندازی_revisions_from_strvec(&آرگومانها, revs, عدم);
	إذا (آرگومانها.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

بیرون:
	strvec_clear(&آرگومانها);
	أرجع ret;
}

ساكن صحيح handle_ref_update(بنية ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت بنية object_id *new_oid,
			     ثابت بنية object_id *old_oid,
			     ثابت حرف *reflog_پیام,
			     بنية strbuf *err)
{
	إذا (!transaction) {
		چاپ_کن("update %s %s %s\n",
		       refname, oid_مقصد_hex(new_oid), oid_مقصد_hex(old_oid));
		أرجع 0;
	}

	أرجع ref_transaction_update(transaction, refname, new_oid, old_oid,
				      عدم, عدم, 0, reflog_پیام, err);
}

ساكن صحيح compute_pending_ref_updates(بنية rev_info *revs,
				       تعداد ref_action action,
				       بنية commit *original,
				       بنية commit *rewritten,
				       تعداد replay_خالی_commit_action خالی,
				       بنية replay_نتیجه *نتیجه)
{
	ثابت بنية name_decoration *decoration;
	بنية replay_revisions_options opts = {
		.خالی = خالی,
	};
	حرف hex[GIT_MAX_HEXSZ + 1];
	bool detached_سر;
	صحيح سر_flags = 0;
	صحيح ret;

	refs_read_ref_پر(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, عدم, &سر_flags);
	detached_سر = !(سر_flags & REF_ISSYMREF);

	opts.onto = oid_مقصد_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, نتیجه);
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
	     decoration = decoration->بعدی)
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

		replay_نتیجه_صف_update(نتیجه, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	أرجع 0;
}

ساكن صحيح apply_pending_ref_updates(بنية repository *repo,
				     ثابت بنية replay_نتیجه *نتیجه,
				     ثابت حرف *reflog_پیام,
				     صحيح dry_پیاپی)
{
	بنية ref_transaction *transaction = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret;

	إذا (!dry_پیاپی) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			اذهب بیرون;
		}
	}

	لكل (اندازه_t i = 0; i < نتیجه->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					نتیجه->updates[i].refname,
					&نتیجه->updates[i].new_oid,
					&نتیجه->updates[i].old_oid,
					reflog_پیام, &err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتیجه->updates[i].refname, err.buf);
			اذهب بیرون;
		}
	}

	إذا (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	أرجع ret;
}

ساكن صحيح handle_reference_updates(بنية rev_info *revs,
				    تعداد ref_action action,
				    بنية commit *original,
				    بنية commit *rewritten,
				    ثابت حرف *reflog_پیام,
				    صحيح dry_پیاپی,
				    تعداد replay_خالی_commit_action خالی)
{
	بنية replay_نتیجه نتیجه = { 0 };
	صحيح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  خالی, &نتیجه);
	إذا (ret)
		اذهب بیرون;

	ret = apply_pending_ref_updates(revs->repo, &نتیجه, reflog_پیام, dry_پیاپی);

بیرون:
	replay_نتیجه_release(&نتیجه);
	أرجع ret;
}

ساكن صحيح commit_became_خالی(بنية repository *repo,
			       بنية commit *original,
			       بنية درخت *نتیجه)
{
	بنية object_id والد_درخت_oid;

	إذا (اولی_والد_درخت_oid(repo, original, &والد_درخت_oid) < 0)
		أرجع -1;

	أرجع oideq(&نتیجه->object.oid, &والد_درخت_oid);
}

ساكن صحيح parse_opt_خالی(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد replay_خالی_commit_action *مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "drop"))
		*مقدار = REPLAY_خالی_COMMIT_DROP;
	وإلا إذا (!strcmp(arg, "keep"))
		*مقدار = REPLAY_خالی_COMMIT_KEEP;
	وإلا إذا (!strcmp(arg, "abort"))
		*مقدار = REPLAY_خالی_COMMIT_ABORT;
	وإلا
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	أرجع 0;
}

ساكن صحيح cmd_history_fixup(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *پیشوند,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		عدم,
	};
	تعداد replay_خالی_commit_action خالی = REPLAY_خالی_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	تعداد commit_درخت_flags flags = 0;
	صحيح dry_پیاپی = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پیاپی,
			 N_("perform a dry-run without updating any refs")),
		OPT_بیت(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_درخت_EDIT_پیام),
		OPT_CALLBACK_F(0, "empty", &خالی, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی),
		OPT_انتها(),
	};
	بنية ادغام_نتیجه ادغام_نتیجه = { 0 };
	بنية ادغام_options ادغام_opts = { 0 };
	بنية strbuf reflog_پیام = STRBUF_INIT;
	بنية commit *سر_commit, *original, *rewritten;
	بنية درخت *سر_درخت, *original_درخت, *شاخص_درخت;
	بنية rev_info revs = { 0 };
	bool skip_commit = false;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب بیرون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	إذا (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		اذهب بیرون;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بیرون;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	سر_commit = lookup_commit_reference_by_name("HEAD");
	إذا (!سر_commit) {
		ret = error(_("cannot look up HEAD"));
		اذهب بیرون;
	}

	سر_درخت = repo_get_commit_درخت(repo, سر_commit);
	إذا (!سر_درخت) {
		ret = error(_("cannot get tree for HEAD"));
		اذهب بیرون;
	}

	إذا (repo_read_شاخص(repo) < 0) {
		ret = error(_("unable to read index"));
		اذهب بیرون;
	}

	إذا (!repo_شاخص_has_changes(repo, سر_درخت, عدم)) {
		ret = error(_("nothing to fixup: no staged changes"));
		اذهب بیرون;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	شاخص_درخت = write_درون_core_شاخص_as_درخت(repo, repo->شاخص);
	إذا (!شاخص_درخت) {
		ret = error(_("unable to write index as a tree"));
		اذهب بیرون;
	}

	original_درخت = repo_get_commit_درخت(repo, original);
	إذا (!original_درخت) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		اذهب بیرون;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_ادغام_options(&ادغام_opts, repo);
	ادغام_opts.ancestor = "HEAD";
	ادغام_opts.branch1 = argv[0];
	ادغام_opts.branch2 = "staged";
	ادغام_incore_nonrecursive(&ادغام_opts, سر_درخت,
				  original_درخت, شاخص_درخت, &ادغام_نتیجه);

	إذا (ادغام_نتیجه.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		اذهب بیرون;
	}

	إذا (!ادغام_نتیجه.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		اذهب بیرون;
	}

	ret = commit_became_خالی(repo, original, ادغام_نتیجه.درخت);
	إذا (ret < 0)
		اذهب بیرون;
	إذا (ret > 0) {
		بدل (خالی) {
		حالة REPLAY_خالی_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->قلم : عدم;

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
				اذهب بیرون;
			}

			skip_commit = true;
			اكسر;
		حالة REPLAY_خالی_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			اكسر;
		حالة REPLAY_خالی_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			اذهب بیرون;
		}
	}

	ret = راهاندازی_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب بیرون;

	إذا (!skip_commit) {
		ret = commit_درخت_ext(repo, "fixup", original, عدم, original->parents,
				      &original_درخت->object.oid, &ادغام_نتیجه.درخت->object.oid,
				      &rewritten, flags);
		إذا (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			اذهب بیرون;
		}
	}

	strbuf_addf(&reflog_پیام, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیام.buf, dry_پیاپی, خالی);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	ادغام_finalize(&ادغام_opts, &ادغام_نتیجه);
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح cmd_history_reword(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *پیشوند,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پیاپی = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پیاپی,
			 N_("perform a dry-run without updating any refs")),
		OPT_انتها(),
	};
	بنية strbuf reflog_پیام = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب بیرون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بیرون;
	}

	ret = راهاندازی_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب بیرون;

	ret = commit_درخت_with_edited_پیام(repo, "reworded", original, &rewritten);
	إذا (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		اذهب بیرون;
	}

	strbuf_addf(&reflog_پیام, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیام.buf, dry_پیاپی, REPLAY_خالی_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح write_ondisk_شاخص(بنية repository *repo,
			      بنية object_id *oid,
			      ثابت حرف *مسیر)
{
	بنية unpack_trees_options opts = { 0 };
	بنية قفل_file قفل = قفل_INIT;
	بنية درخت_desc درخت_desc;
	بنية شاخص_state شاخص;
	بنية درخت *درخت;
	صحيح ret;

	شاخص_state_init(&شاخص, repo);

	opts.سر_idx = -1;
	opts.src_شاخص = &شاخص;
	opts.dst_شاخص = &شاخص;

	درخت = repo_parse_درخت_غیرمستقیم(repo, oid);
	إذا (!درخت) {
		ret = error(_("unable to parse tree %s"), oid_مقصد_hex(oid));
		اذهب بیرون;
	}
	init_درخت_desc(&درخت_desc, &درخت->object.oid, درخت->میانگیر, درخت->اندازه);

	إذا (unpack_trees(1, &درخت_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		اذهب بیرون;
	}

	اول_cache_درخت(repo, &شاخص, درخت);

	إذا (repo_hold_قفل_file_for_update(repo, &قفل, مسیر, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		اذهب بیرون;
	}

	إذا (write_locked_شاخص(&شاخص, &قفل, COMMIT_قفل)) {
		ret = error(_("unable to write new index file"));
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	rollback_قفل_file(&قفل);
	release_شاخص(&شاخص);
	أرجع ret;
}

ساكن صحيح تقسیم_commit(بنية repository *repo,
			بنية commit *original,
			بنية pathspec *pathspec,
			بنية commit **بیرون)
{
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية strbuf شاخص_file = STRBUF_INIT;
	بنية شاخص_state شاخص = شاخص_STATE_INIT(repo);
	ثابت بنية object_id *original_commit_درخت_oid;
	ثابت بنية object_id *old_درخت_oid, *new_درخت_oid;
	بنية object_id والد_درخت_oid;
	حرف original_commit_oid[GIT_MAX_HEXSZ + 1];
	بنية commit *اولی_commit, *دومی_commit;
	بنية commit_list *parents = عدم;
	بنية درخت *تقسیم_درخت;
	صحيح ret;

	إذا (اولی_والد_درخت_oid(repo, original, &والد_درخت_oid) < 0) {
		ret = -1;
		اذهب بیرون;
	}
	original_commit_درخت_oid = get_commit_درخت_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_مسیر_replace(repo, &شاخص_file, "%s", "history-split.index");

	ret = write_ondisk_شاخص(repo, &والد_درخت_oid, شاخص_file.buf);
	إذا (ret < 0)
		اذهب بیرون;

	ret = read_شاخص_from(&شاخص, شاخص_file.buf, repo->gitdir);
	إذا (ret < 0) {
		ret = error(_("failed reading temporary index"));
		اذهب بیرون;
	}

	oid_مقصد_hex_r(original_commit_oid, &original->object.oid);
	ret = پیاپی_جمع_کن_p_شاخص(repo, &شاخص, شاخص_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, جمع_کن_P_DISALLOW_EDIT);
	إذا (ret < 0)
		اذهب بیرون;

	تقسیم_درخت = write_درون_core_شاخص_as_درخت(repo, &شاخص);
	إذا (!تقسیم_درخت) {
		ret = error(_("failed split tree"));
		اذهب بیرون;
	}

	unlink(شاخص_file.buf);
	strbuf_release(&شاخص_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	إذا (oideq(&تقسیم_درخت->object.oid, &والد_درخت_oid)) {
		ret = error(_("split commit is empty"));
		اذهب بیرون;
	} وإلا إذا (oideq(&تقسیم_درخت->object.oid, original_commit_درخت_oid)) {
		ret = error(_("split commit tree matches original commit"));
		اذهب بیرون;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_درخت_ext(repo, "split-out", original, عدم, original->parents, &والد_درخت_oid,
			      &تقسیم_درخت->object.oid, &اولی_commit, COMMIT_درخت_EDIT_پیام);
	إذا (ret < 0) {
		ret = error(_("failed writing first commit"));
		اذهب بیرون;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(اولی_commit, &parents);

	old_درخت_oid = &repo_get_commit_درخت(repo, اولی_commit)->object.oid;
	new_درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	ret = commit_درخت_ext(repo, "split-out", original, عدم, parents, old_درخت_oid,
			      new_درخت_oid, &دومی_commit, COMMIT_درخت_EDIT_پیام);
	إذا (ret < 0) {
		ret = error(_("failed writing second commit"));
		اذهب بیرون;
	}

	*بیرون = دومی_commit;
	ret = 0;

بیرون:
	إذا (شاخص_file.len)
		unlink(شاخص_file.buf);
	strbuf_release(&شاخص_file);
	commit_list_free(parents);
	release_شاخص(&شاخص);
	أرجع ret;
}

ساكن صحيح cmd_history_تقسیم(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *پیشوند,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_تقسیم_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پیاپی = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پیاپی,
			 N_("perform a dry-run without updating any refs")),
		OPT_انتها(),
	};
	بنية commit *original, *rewritten = عدم;
	بنية strbuf reflog_پیام = STRBUF_INIT;
	بنية pathspec pathspec = { 0 };
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	إذا (argc < 1) {
		ret = error(_("command expects a committish"));
		اذهب بیرون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_پر |
		       PATHSPEC_SYMLINK_LEADING_مسیر |
		       PATHSPEC_پیشوند_ORIGIN,
		       پیشوند, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بیرون;
	}

	ret = راهاندازی_revwalk(repo, action, original, &revs);
	إذا (ret < 0)
		اذهب بیرون;

	إذا (original->parents && original->parents->بعدی) {
		ret = error(_("cannot split up merge commit"));
		اذهب بیرون;
	}

	ret = تقسیم_commit(repo, original, &pathspec, &rewritten);
	إذا (ret < 0)
		اذهب بیرون;

	strbuf_addf(&reflog_پیام, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیام.buf, dry_پیاپی, REPLAY_خالی_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	strbuf_release(&reflog_پیام);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	أرجع ret;
}

#تعریف_کن SQUASH_دیده (1u << 11)
#تعریف_کن SQUASH_TIP (1u << 12)
#تعریف_کن SQUASH_AMEND_هدف (1u << 13)

ساكن bool is_autosquash_subject(ثابت حرف *s)
{
	أرجع starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ساكن bool skip_one_autosquash_پیشوند(ثابت حرف *s, ثابت حرف **بیرون)
{
	إذا (skip_پیشوند(s, "amend!", بیرون) || skip_پیشوند(s, "fixup!", بیرون) ||
	    skip_پیشوند(s, "squash!", بیرون)) {
		طالما (**بیرون == ' ')
			(*بیرون)++;
		أرجع true;
	}
	أرجع false;
}

ساكن فراغ truncate_پیام_مقصد_subject(بنية strbuf *پیام)
{
	ثابت حرف *eos = جستجوی_رشته(پیام->buf, "\n\n");

	إذا (eos)
		strbuf_setlen(پیام, eos - پیام->buf + 1);
}

بنية subject_داده {
	بنية strintmap subjects;
	بنية strbuf subject;
	بنية strbuf squash_پیام;
	ثابت حرف *پیام_2;
	bool edit_پیام;
};

#تعریف_کن SUBJECT_داده_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_پیام = STRBUF_INIT,	\
}

ساكن فراغ subject_داده_clear(بنية subject_داده *داده)
{
	strintmap_clear(&داده->subjects);
	strbuf_release(&داده->subject);
	strbuf_release(&داده->squash_پیام);
}

ساكن صحيح squash_amend_پیام(بنية repository *repo,
				بنية commit *commit,
				بنية subject_داده *داده,
				بیعلامت flags)
{
	ثابت حرف *body = داده->پیام_2 + داده->subject.len;

	طالما (آیا_فاصله(*body))
		body++;

	إذا (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_پیدا_کن_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			داده->subject.buf);
		أرجع 0;
	}

	إذا (داده->edit_پیام) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_هدف) {
		إذا (starts_with(داده->squash_پیام.buf, "squash!"))
			أرجع error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_پیدا_کن_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     داده->subject.buf);
		إذا (starts_with(داده->squash_پیام.buf, "fixup!"))
			strbuf_splice(&داده->squash_پیام, 0, 5, "amend!", 5);
		إذا (starts_with(داده->squash_پیام.buf, "amend!")) {
			truncate_پیام_مقصد_subject(&داده->squash_پیام);
			strbuf_addch(&داده->squash_پیام, '\n');
		} وإلا {
			strbuf_reset(&داده->squash_پیام);
		}
		strbuf_addstr(&داده->squash_پیام, body);
		strbuf_complete_سطر(&داده->squash_پیام);
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_پیدا_کن_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     داده->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_squash_پیام(بنية repository *repo,
				بنية commit *commit,
				بنية subject_داده *داده,
				بیعلامت flags)
{
	ثابت حرف *body = داده->پیام_2 + داده->subject.len;

	طالما (آیا_فاصله(*body))
		body++;

	إذا (داده->edit_پیام) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_هدف) {
		إذا (starts_with(داده->squash_پیام.buf, "fixup!")) {
			truncate_پیام_مقصد_subject(&داده->squash_پیام);
			strbuf_splice(&داده->squash_پیام, 0, 5, "squash", 6);
		}
		إذا (starts_with(داده->squash_پیام.buf, "squash!")) {
			strbuf_addch(&داده->squash_پیام, '\n');
			strbuf_addstr(&داده->squash_پیام, body);
			strbuf_complete_سطر(&داده->squash_پیام);
		} وإلا {
			أرجع error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_پیدا_کن_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     داده->subject.buf);
		}
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_پیدا_کن_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      داده->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_check_can_autosquash(بنية repository *repo,
				       بنية commit *commit,
				       بنية subject_داده *داده,
				       بیعلامت flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_هدف;
	إذا (starts_with(داده->subject.buf, "amend!"))
		أرجع squash_amend_پیام(repo, commit, داده, flags);
	وإلا إذا (starts_with(داده->subject.buf, "squash!"))
		أرجع squash_squash_پیام(repo, commit, داده, flags);

	أرجع 0;
}

ساكن صحيح squash_check_autosquash_subject(بنية repository *repo,
					   بنية commit *commit,
					   بنية subject_داده *داده)
{
	ثابت حرف* s = داده->subject.buf;
	بنية commit *هدف;
	بنية hashmap_iter iter;
	بنية strmap_مدخل *مدخل;
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
	طالما (skip_one_autosquash_پیشوند(s, &s)) {
		بیعلامت flags = strintmap_get(&داده->subjects, s);
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit, داده, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	هدف = lookup_commit_reference_by_name(s);
	إذا (هدف && starts_with(oid_مقصد_hex(&هدف->object.oid), s)) {
		بیعلامت flags =
			هدف->object.flags & (SQUASH_دیده | SQUASH_AMEND_هدف);
		إذا (!flags) {
			ثابت حرف *subject_شروع;
			ثابت حرف *میانگیر = repo_logmsg_reencode(repo, هدف,
								  عدم, عدم);
			اندازه_t subject_len = پیدا_کن_commit_subject(میانگیر,
								 &subject_شروع);
			حرف *subject = xmemdupz(subject_شروع, subject_len);

			flags = strintmap_get(&داده->subjects, subject);
			حرر(subject);
			repo_unuse_commit_میانگیر(repo, هدف, میانگیر);
		}
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit,
							   داده, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_مدخل(&داده->subjects, &iter, مدخل) {
		s = داده->subject.buf;
		طالما(skip_one_autosquash_پیشوند(s, &s)) {
			إذا (starts_with(مدخل->کلید, s)) {
				بیعلامت مقدار = (intptr_t)مدخل->مقدار;

				أرجع squash_check_can_autosquash(repo, commit,
								   داده, مقدار);
			}
		}
	}
	أرجع error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_پیدا_کن_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       داده->subject.buf);
}

ساكن صحيح squash_check_subject(بنية repository *repo,
				بنية commit *commit,
				بنية subject_داده *داده)
{
	صحيح ret = 0;
	ثابت حرف *buf = repo_logmsg_reencode(repo, commit, عدم, عدم);
	اندازه_t subject_len = پیدا_کن_commit_subject(buf, &داده->پیام_2);

	strbuf_reset(&داده->subject);
	strbuf_جمع_کن(&داده->subject, داده->پیام_2, subject_len);

	إذا (!strintmap_get_اندازه(&داده->subjects)) {
		ثابت حرف *s;

		strbuf_addstr(&داده->squash_پیام, داده->پیام_2);
		strbuf_complete_سطر(&داده->squash_پیام);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = داده->subject.buf;
		skip_one_autosquash_پیشوند(s, &s);
		strintmap_مجموعه(&داده->subjects, s, SQUASH_AMEND_هدف | SQUASH_دیده);
		commit->object.flags |= SQUASH_AMEND_هدف;
	} وإلا إذا (is_autosquash_subject(داده->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, داده);
	} وإلا {
		strintmap_مجموعه(&داده->subjects, داده->subject.buf, SQUASH_دیده);
	}
	repo_unuse_commit_میانگیر(repo, commit, buf);
	أرجع ret;
}

ساكن صحيح build_squash_پیام(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *بیرون);

ساكن صحيح راهاندازی_squash_revisions(بنية repository *repo,
				  صحيح argc, ثابت حرف **argv,
				  بنية rev_info *revs)
{
	repo_init_revisions(repo, revs, عدم);
	revs->معکوس = 1;
	revs->topo_ترتیب = 1;
	revs->مرتب_سازی_ترتیب = REV_مرتب_سازی_درون_گراف_ترتیب;
	revs->simplify_history = 0;
	revs->ancestry_مسیر = 1;
	revs->limited = 1;
	revs->ancestry_مسیر_implicit_bottoms = 1;

	argc = راهاندازی_revisions(argc, argv, revs, عدم);
	إذا (argc > 1)
		أرجع error(_("unrecognized argument: %s"), argv[1]);

	إذا (revs->معکوس != 1 || revs->topo_ترتیب != 1 ||
	    revs->مرتب_سازی_ترتیب != REV_مرتب_سازی_درون_گراف_ترتیب ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_مسیر != 1 || revs->limited != 1 ||
	    revs->ancestry_مسیر_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->معکوس = 1;
		revs->topo_ترتیب = 1;
		revs->مرتب_سازی_ترتیب = REV_مرتب_سازی_درون_گراف_ترتیب;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_مسیر = 1;
		revs->limited = 1;
		revs->ancestry_مسیر_implicit_bottoms = 1;
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
				bool edit_پیام,
				صحيح argc, ثابت حرف **argv,
				بنية commit **oldest_بیرون,
				بنية commit **tip_بیرون,
				حرف **پیام_بیرون)
{
	بنية rev_info revs;
	بنية subject_داده subject_داده = SUBJECT_داده_INIT;
	بنية commit *commit, *oldest = عدم, *tip = عدم;
	بنية strbuf todo_buf = STRBUF_INIT;
	صحيح ret, tip_شمار = 0;
	bool walk_started = false;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_array refs = { 0 };

	subject_داده.edit_پیام = edit_پیام;
	ret = راهاندازی_squash_revisions(repo, argc, argv, &revs);
	إذا (ret < 0)
		اذهب بیرون;

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب بیرون;
	}
	walk_started = true;
	طالما ((commit = get_revision(&revs))) {
		بنية commit_list *p;

		إذا (edit_پیام)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_مقصد_hex(&commit->object.oid));

		إذا (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			اذهب بیرون;
		}
		لكل (p = commit->parents; oldest && p; p = p->بعدی) {
			بنية commit_list *q;
			بنية object *o;
			bool دیده;

			إذا (repo_parse_commit(repo, p->قلم)) {
				ret = error(_("cannot parse commit"));
				اذهب بیرون;
			}
			o = &p->قلم->object;
			دیده = o->flags & SQUASH_دیده;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			لكل (q = oldest->parents; !دیده && q; q = q->بعدی) {
				إذا (p->قلم == q->قلم) {
					دیده = true;
					commit_list_درج(commit, &filter.with_commit);
				}
			}
			إذا (!دیده) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_پیدا_کن_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_پیدا_کن_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				اذهب بیرون;
			}
			إذا (o->flags & SQUASH_TIP) {
				tip_شمار--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		إذا (!oldest) {
			commit_list_درج(commit, &filter.with_commit);
			oldest = commit;
		}
		إذا (squash_check_subject(repo, commit, &subject_داده)) {
			ret = -1;
			اذهب بیرون;
		}
		tip = commit;
		tip->object.flags |= SQUASH_دیده | SQUASH_TIP;
		tip_شمار++;
	}
	clear_object_flags(repo, SQUASH_دیده | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	إذا (!tip_شمار) {
		ret = error(_("the revision range is empty"));
		اذهب بیرون;
	} وإلا إذا (tip_شمار != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		اذهب بیرون;
	} وإلا إذا (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		اذهب بیرون;
	} وإلا إذا (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_درج(tip, &filter.no_commit);
	filter.نوع = FILTER_REFS_BRANCHES;
	إذا (update_branches &&
	    filter_refs(&refs, &filter, filter.نوع)) {
		ret = error(_("could not filter refs"));
		اذهب بیرون;
	}
	إذا (refs.nr) {
		بنية ref_format format = REF_FORMAT_INIT;
		بنية ref_ترتیبدهی *ترتیبدهی;
		بنية string_list ترتیبدهی_options = STRING_LIST_INIT_DUP;
		بنية strbuf branches = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		إذا (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&ترتیبدهی_options, "refname");
		ترتیبدهی = ref_ترتیبدهی_options(&ترتیبدهی_options);
		ref_array_مرتب_سازی(ترتیبدهی, &refs);
		لكل (صحيح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			إذا (format_ref_array_قلم(refs.اقلام[i], &format,
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
		ref_ترتیبدهی_release(ترتیبدهی);
		string_list_clear(&ترتیبدهی_options, 0);
		اذهب بیرون;
	}
	إذا (edit_پیام) {
		strbuf_reset(&subject_داده.squash_پیام);
		ret = build_squash_پیام(repo, &todo_buf,
					   &subject_داده.squash_پیام);
		إذا (ret < 0)
			اذهب بیرون;
	}

	*oldest_بیرون = oldest;
	*tip_بیرون = tip;
	*پیام_بیرون = strbuf_detach(&subject_داده.squash_پیام, عدم);
	ret = 0;

بیرون:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_دیده | SQUASH_TIP |
			   SQUASH_AMEND_هدف);
	إذا (walk_started)
		reset_revision_walk();
	subject_داده_clear(&subject_داده);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	أرجع ret;
}

ساكن bool amend_replaces_هدف(بنية todo_list *todo, صحيح هدف)
{
	لكل (صحيح i = هدف + 1; i < todo->nr &&
				 todo->اقلام[i].command != TODO_PICK; i++) {
		إذا (todo->اقلام[i].command == TODO_SQUASH)
			أرجع false;
		إذا (todo->اقلام[i].flags & TODO_REPLACE_FIXUP_پیام)
			أرجع true;
	}
	أرجع false;
}

ساكن صحيح build_squash_پیام(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *بیرون)
{
	بنية todo_list todo = TODO_LIST_INIT;
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح nr_commits, ret;

	إذا (todo_list_parse_insn_میانگیر(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		اذهب بیرون;
	}

	nr_commits = todo.nr;
	لكل (صحيح i = 0; i < nr_commits; i++) {
		بنية todo_قلم *قلم = &todo.اقلام[i];
		ثابت حرف *پیام_2, *body;
		اندازه_t commented_len;
		bool skip, squashing;

		squashing = قلم->command == TODO_SQUASH ||
			    (قلم->flags & TODO_REPLACE_FIXUP_پیام);
		إذا (قلم->command == TODO_PICK)
			skip = amend_replaces_هدف(&todo, i);
		وإلا
			skip = !squashing;

		پیام_2 = repo_logmsg_reencode(repo, قلم->commit, عدم, عدم);
		پیدا_کن_commit_subject(پیام_2, &body);

		إذا (skip)
			commented_len = طول_رشته(body);
		وإلا إذا (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		وإلا
			commented_len = 0;

		إذا (!i)
			جمع_کن_squash_combination_سرآیند(بیرون, nr_commits);
		strbuf_addch(بیرون, '\n');
		جمع_کن_squash_پیام_سرآیند(بیرون, i + 1, skip);
		strbuf_addstr(بیرون, "\n\n");
		strbuf_جمع_کن_commented_سطرها(بیرون, body, commented_len, comment_سطر_str);
		strbuf_addstr(بیرون, body + commented_len);
		strbuf_complete_سطر(بیرون);

		repo_unuse_commit_میانگیر(repo, قلم->commit, پیام_2);
	}

	ret = 0;

بیرون:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	أرجع ret;
}

ساكن صحيح cmd_history_squash(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *پیشوند,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پیاپی = 0;
	صحيح edit = 1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پیاپی,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_انتها(),
	};
	بنية strbuf reflog_پیام = STRBUF_INIT;
	بنية commit *oldest, *tip, *rewritten;
	ثابت بنية object_id *پایه_درخت_oid, *tip_درخت_oid;
	حرف *پیام_template = عدم;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	إذا (argc < 2) {
		ret = error(_("command expects a revision range"));
		اذهب بیرون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_پیام, "squash: updating ");
	strbuf_join_argv(&reflog_پیام, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &پیام_template);
	إذا (ret < 0)
		اذهب بیرون;

	ret = راهاندازی_revwalk(repo, action, tip, &revs);
	إذا (ret < 0)
		اذهب بیرون;

	پایه_درخت_oid = &repo_get_commit_درخت(repo,
					oldest->parents->قلم)->object.oid;
	tip_درخت_oid = &repo_get_commit_درخت(repo, tip)->object.oid;

	ret = commit_درخت_ext(repo, "squash", oldest, پیام_template,
			      oldest->parents, پایه_درخت_oid, tip_درخت_oid,
			      &rewritten,
			      edit ? COMMIT_درخت_EDIT_پیام : 0);
	إذا (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		اذهب بیرون;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_پیام.buf, dry_پیاپی,
				       REPLAY_خالی_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	حرر(پیام_template);
	أرجع ret;
}

ساكن صحيح update_worktree(بنية repository *repo,
			   ثابت بنية commit *old_سر,
			   ثابت بنية commit *new_سر,
			   bool dry_پیاپی)
{
	بنية reset_working_درخت_options opts = {
		.oid_from = &old_سر->object.oid,
		.oid = &new_سر->object.oid,
	};
	إذا (dry_پیاپی)
		opts.flags |= RESET_WORKING_درخت_DRY_پیاپی;
	أرجع reset_working_درخت(repo, &opts);
}

ساكن صحيح پیدا_کن_سر_درخت_change(بنية repository *repo,
				 ثابت بنية replay_نتیجه *نتیجه,
				 بنية commit **old_سر,
				 بنية commit **new_سر,
				 bool *تغییریافته)
{
	ثابت بنية replay_ref_update *سر_update = عدم;
	بنية commit *old_سر_commit, *new_سر_commit;
	بنية درخت *old_سر_درخت, *new_سر_درخت;
	ثابت حرف *سر_هدف;
	صحيح سر_flags;

	*تغییریافته = false;

	سر_هدف = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      عدم, &سر_flags);
	إذا (!سر_هدف)
		أرجع error(_("cannot look up HEAD"));

	لكل (اندازه_t i = 0; i < نتیجه->updates_nr; i++) {
		إذا (!strcmp(نتیجه->updates[i].refname, سر_هدف)) {
			سر_update = &نتیجه->updates[i];
			اكسر;
		}
	}

	إذا (!سر_update)
		أرجع 0;

	old_سر_commit = lookup_commit_reference(repo, &سر_update->old_oid);
	new_سر_commit = lookup_commit_reference(repo, &سر_update->new_oid);
	إذا (!old_سر_commit || !new_سر_commit)
		أرجع error(_("cannot resolve HEAD commit"));

	old_سر_درخت = repo_get_commit_درخت(repo, old_سر_commit);
	new_سر_درخت = repo_get_commit_درخت(repo, new_سر_commit);
	إذا (!old_سر_درخت || !new_سر_درخت)
		أرجع error(_("cannot resolve tree for HEAD"));

	إذا (oideq(&old_سر_درخت->object.oid, &new_سر_درخت->object.oid))
		أرجع 0;

	*old_سر = old_سر_commit;
	*new_سر = new_سر_commit;
	*تغییریافته = true;

	أرجع 0;
}

ساكن صحيح cmd_history_drop(صحيح argc,
			    ثابت حرف **argv,
			    ثابت حرف *پیشوند,
			    بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		عدم,
	};
	تعداد replay_خالی_commit_action خالی = REPLAY_خالی_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_پیاپی = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_پیاپی,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &خالی, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی),
		OPT_انتها(),
	};
	بنية strbuf reflog_پیام = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	بنية replay_نتیجه نتیجه = { 0 };
	بنية commit *old_سر, *new_سر;
	bool سر_حرکتها = false;
	صحيح ret;

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب بیرون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب بیرون;
	}

	إذا (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		اذهب بیرون;
	} وإلا إذا (original->parents->بعدی) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		اذهب بیرون;
	}

	ret = راهاندازی_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب بیرون;

	rewritten = original->parents->قلم;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  خالی, &نتیجه);
	إذا (ret) {
		ret = error(_("failed replaying descendants"));
		اذهب بیرون;
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
		ret = پیدا_کن_سر_درخت_change(repo, &نتیجه, &old_سر,
					    &new_سر, &سر_حرکتها);
		إذا (ret < 0)
			اذهب بیرون;

		إذا (سر_حرکتها && update_worktree(repo, old_سر, new_سر, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			اذهب بیرون;
		}
	}

	strbuf_addf(&reflog_پیام, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &نتیجه, reflog_پیام.buf, dry_پیاپی);
	إذا (ret < 0) {
		ret = error(_("failed to update references"));
		اذهب بیرون;
	}

	إذا (!dry_پیاپی && سر_حرکتها && update_worktree(repo, old_سر, new_سر, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_مقصد_hex(&new_سر->object.oid));
		اذهب بیرون;
	}

	ret = 0;

بیرون:
	replay_نتیجه_release(&نتیجه);
	strbuf_release(&reflog_پیام);
	release_revisions(&revs);
	أرجع ret;
}

صحيح cmd_history(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *پیشوند,
		بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_تقسیم_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_تقسیم),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_انتها(),
	};

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	أرجع fn(argc, argv, پیشوند, repo);
}
