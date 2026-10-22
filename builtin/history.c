#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "cache-tree.h"
#شامل "commit.h"
#شامل "commit-reach.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "merge-ort.h"
#شامل "oidmap.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "read-cache.h"
#شامل "refs.h"
#شامل "ref-filter.h"
#شامل "replay.h"
#شامل "reset.h"
#شامل "revision.h"
#شامل "sequencer.h"
#شامل "string-list.h"
#شامل "strvec.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "unpack-trees.h"
#شامل "wt-status.h"

#تعریف_کرو GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#تعریف_کرو GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#تعریف_کرو GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#تعریف_کرو GIT_HISTORY_بانٹو_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#تعریف_کرو GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ساکن خالی change_ڈیٹا_free(خالی *util, ثابت حرف *str غیر_مستعمل)
{
	ساخت wt_حالت_change_ڈیٹا *d = util;
	آزاد(d->rename_ماخذ);
	آزاد(d);
}

ساکن صحیح fill_commit_پیغام(ساخت repository *repo,
			       ثابت ساخت object_id *old_درخت,
			       ثابت ساخت object_id *new_درخت,
			       ثابت حرف *default_پیغام,
			       ثابت حرف *action,
			       ساخت strbuf *برآمد)
{
	ثابت حرف *راستہ = git_راستہ_commit_editmsg();
	ثابت حرف *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	ساخت wt_حالت s;

	wt_حالت_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_تیز;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_سابقہ = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(راستہ, "w");
	اگر (!s.fp)
		واپس error_errno(_("could not open '%s'"), راستہ);

	strbuf_addstr(برآمد, default_پیغام);
	strbuf_addch(برآمد, '\n');
	strbuf_commented_addf(برآمد, comment_سطر_str, hint, action, comment_سطر_str);
	اگر (fwrite(برآمد->buf, 1, برآمد->len, s.fp) != برآمد->len)
		die_errno(_("could not write to '%s'"), راستہ);

	wt_حالت_collect_changes_trees(&s, old_درخت, new_درخت);
	wt_حالت_print(&s);
	wt_حالت_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_ڈیٹا_free);
	اگر (fclose(s.fp))
		die_errno(_("could not write to '%s'"), راستہ);

	strbuf_reset(برآمد);
	اگر (launch_editor(راستہ, برآمد, تہی)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		واپس -1;
	}
	strbuf_stripspace(برآمد, comment_سطر_str);

	cleanup_پیغام(برآمد, COMMIT_پیغام_CLEANUP_ALL, 0);

	اگر (!برآمد->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		واپس -1;
	}

	واپس 0;
}

شمار commit_درخت_flags {
	COMMIT_درخت_EDIT_پیغام = (1 << 0),
};

ساکن صحیح commit_درخت_ext(ساخت repository *repo,
			   ثابت حرف *action,
			   ساخت commit *commit_with_پیغام,
			   ثابت حرف *پیغام_template,
			   ثابت ساخت commit_list *parents,
			   ثابت ساخت object_id *old_درخت,
			   ثابت ساخت object_id *new_درخت,
			   ساخت commit **برآمد,
			   شمار commit_درخت_flags flags)
{
	ثابت حرف *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		تہی,
	};
	ثابت حرف *original_پیغام, *original_body, *ptr;
	ساخت commit_extra_سرخی *original_extra_سرخیاں = تہی;
	ساخت strbuf commit_پیغام_2 = STRBUF_INIT;
	ساخت object_id rewritten_commit_oid;
	حرف *original_author = تہی;
	سائز_t len;
	صحیح ret;

	/* We retain authorship of the original commit. */
	original_پیغام = repo_logmsg_reencode(repo, commit_with_پیغام, تہی, تہی);
	ptr = ڈھونڈو_commit_سرخی(original_پیغام, "author", &len);
	اگر (ptr)
		original_author = xmemdupz(ptr, len);
	ڈھونڈو_commit_subject(original_پیغام, &original_body);

	اگر (!پیغام_template)
		پیغام_template = original_body;

	اگر (flags & COMMIT_درخت_EDIT_پیغام) {
		ret = fill_commit_پیغام(repo, old_درخت, new_درخت,
					  پیغام_template, action, &commit_پیغام_2);
		اگر (ret < 0)
			جاؤ برآمد;
	} ورنہ {
		strbuf_addstr(&commit_پیغام_2, پیغام_template);
	}

	original_extra_سرخیاں = read_commit_extra_سرخیاں(commit_with_پیغام,
							   exclude_gpgsig);

	ret = commit_درخت_extended(commit_پیغام_2.buf, commit_پیغام_2.len, new_درخت,
				   parents, &rewritten_commit_oid, original_author,
				   تہی, تہی, original_extra_سرخیاں);
	اگر (ret < 0)
		جاؤ برآمد;

	*برآمد = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

برآمد:
	free_commit_extra_سرخیاں(original_extra_سرخیاں);
	strbuf_release(&commit_پیغام_2);
	آزاد(original_author);
	repo_unuse_commit_بفر(repo, commit_with_پیغام, original_پیغام);
	واپس ret;
}

ساکن صحیح پہلا_والد_درخت_oid(ساخت repository *repo,
				 ساخت commit *commit,
				 ساخت object_id *برآمد)
{
	ساخت commit *والد = commit->parents ? commit->parents->شے : تہی;

	اگر (!والد) {
		oidcpy(برآمد, repo->hash_algo->خالی_خانے_درخت);
		واپس 0;
	}

	اگر (repo_parse_commit(repo, والد))
		واپس error(_("unable to parse parent commit %s"),
			     oid_مقصد_hex(&والد->object.oid));

	oidcpy(برآمد, &repo_get_commit_درخت(repo, والد)->object.oid);
	واپس 0;
}

ساکن صحیح commit_درخت_with_edited_پیغام(ساخت repository *repo,
					   ثابت حرف *action,
					   ساخت commit *original,
					   ساخت commit **برآمد)
{
	ساخت object_id والد_درخت_oid;
	ثابت ساخت object_id *درخت_oid;

	درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	اگر (پہلا_والد_درخت_oid(repo, original, &والد_درخت_oid) < 0)
		واپس -1;

	واپس commit_درخت_ext(repo, action, original, تہی, original->parents,
			       &والد_درخت_oid, درخت_oid, برآمد, COMMIT_درخت_EDIT_پیغام);
}

شمار ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_سر,
};

ساکن صحیح parse_ref_action(ثابت ساخت option *opt, ثابت حرف *قدر, صحیح unset)
{
	شمار ref_action *action = opt->قدر;

	BUG_ON_OPT_NEG_NOARG(unset, قدر);
	اگر (!strcmp(قدر, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} ورنہ اگر (!strcmp(قدر, "head")) {
		*action = REF_ACTION_سر;
	} ورنہ {
		واپس error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	واپس 0;
}

ساکن صحیح revwalk_شامل_ہے_merges(ساخت repository *repo,
				   ثابت ساخت strvec *revwalk_دلائل)
{
	ساخت strvec دلائل = STRVEC_INIT;
	ساخت rev_info revs;
	صحیح ret;

	strvec_pushv(&دلائل, revwalk_دلائل->v);
	strvec_ڈالو(&دلائل, "--min-parents=2");

	repo_init_revisions(repo, &revs, تہی);

	تیاری_revisions_from_strvec(&دلائل, &revs, تہی);
	اگر (دلائل.nr != 1)
		BUG("revisions were set up with invalid argument");

	اگر (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		جاؤ برآمد;
	}

	اگر (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		جاؤ برآمد;
	}

	reset_revision_walk();
	ret = 0;

برآمد:
	release_revisions(&revs);
	strvec_clear(&دلائل);
	واپس ret;
}

ساکن صحیح تیاری_revwalk(ساخت repository *repo,
			 شمار ref_action action,
			 ساخت commit *original,
			 ساخت rev_info *revs)
{
	ساخت strvec دلائل = STRVEC_INIT;
	صحیح ret;

	repo_init_revisions(repo, revs, تہی);
	strvec_ڈالو(&دلائل, "ignored");
	strvec_ڈالو(&دلائل, "--reverse");
	strvec_ڈالو(&دلائل, "--topo-order");
	strvec_ڈالو(&دلائل, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&دلائل, "--ancestry-path=%s",
		     oid_مقصد_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&دلائل, "^%s", oid_مقصد_hex(&original->object.oid));

	/*
	 * When we're asked to update HEAD we need to verify that the commit
	 * that we want to rewrite is actually an ancestor of it and, if so,
	 * update it. Otherwise we'll update (or print) all descendant
	 * branches.
	 */
	اگر (action == REF_ACTION_سر) {
		ساخت commit_list *from_list = تہی;
		ساخت commit *سر;

		سر = lookup_commit_reference_by_name("HEAD");
		اگر (!سر) {
			ret = error(_("cannot look up HEAD"));
			جاؤ برآمد;
		}

		commit_list_داخل_کرو(original, &from_list);
		ret = repo_is_descendant_کا(repo, سر, from_list);
		commit_list_free(from_list);

		اگر (ret < 0) {
			ret = error(_("cannot determine descendance"));
			جاؤ برآمد;
		} ورنہ اگر (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			جاؤ برآمد;
		}

		strvec_ڈالو(&دلائل, "HEAD");
	} ورنہ {
		strvec_ڈالو(&دلائل, "--branches");
		strvec_ڈالو(&دلائل, "HEAD");
	}

	ret = revwalk_شامل_ہے_merges(repo, &دلائل);
	اگر (ret < 0)
		جاؤ برآمد;

	تیاری_revisions_from_strvec(&دلائل, revs, تہی);
	اگر (دلائل.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

برآمد:
	strvec_clear(&دلائل);
	واپس ret;
}

ساکن صحیح handle_ref_update(ساخت ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت ساخت object_id *new_oid,
			     ثابت ساخت object_id *old_oid,
			     ثابت حرف *reflog_پیغام,
			     ساخت strbuf *err)
{
	اگر (!transaction) {
		لکھو("update %s %s %s\n",
		       refname, oid_مقصد_hex(new_oid), oid_مقصد_hex(old_oid));
		واپس 0;
	}

	واپس ref_transaction_update(transaction, refname, new_oid, old_oid,
				      تہی, تہی, 0, reflog_پیغام, err);
}

ساکن صحیح compute_pending_ref_updates(ساخت rev_info *revs,
				       شمار ref_action action,
				       ساخت commit *original,
				       ساخت commit *rewritten,
				       شمار replay_خالی_خانے_commit_action خالی_خانے,
				       ساخت replay_نتیجہ *نتیجہ)
{
	ثابت ساخت name_decoration *decoration;
	ساخت replay_revisions_options opts = {
		.خالی_خانے = خالی_خانے,
	};
	حرف hex[GIT_MAX_HEXSZ + 1];
	bool detached_سر;
	صحیح سر_flags = 0;
	صحیح ret;

	refs_read_ref_بھرا(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, تہی, &سر_flags);
	detached_سر = !(سر_flags & REF_ISSYMREF);

	opts.onto = oid_مقصد_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, نتیجہ);
	اگر (ret)
		واپس ret;

	اگر (action != REF_ACTION_BRANCHES && action != REF_ACTION_سر)
		BUG("unsupported ref action %d", action);

	/*
	 * `replay_revisions()` only updates references that are
	 * ancestors of `rewritten`, so we need to manually
	 * handle updating references that point to `original`.
	 */
	برائے (decoration = get_name_decoration(&original->object);
	     decoration;
	     decoration = decoration->اگلا)
	{
		اگر (decoration->type != DECORATION_REF_LOCAL &&
		    decoration->type != DECORATION_REF_سر)
			جاری;

		اگر (action == REF_ACTION_سر &&
		    decoration->type != DECORATION_REF_سر)
			جاری;

		/*
		 * We only need to update HEAD separately in case it's
		 * detached. If it's not we'd already update the branch
		 * it is pointing to.
		 */
		اگر (action == REF_ACTION_BRANCHES &&
		    decoration->type == DECORATION_REF_سر &&
		    !detached_سر)
			جاری;

		replay_نتیجہ_قطار_update(نتیجہ, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	واپس 0;
}

ساکن صحیح apply_pending_ref_updates(ساخت repository *repo,
				     ثابت ساخت replay_نتیجہ *نتیجہ,
				     ثابت حرف *reflog_پیغام,
				     صحیح dry_تسلسل)
{
	ساخت ref_transaction *transaction = تہی;
	ساخت strbuf err = STRBUF_INIT;
	صحیح ret;

	اگر (!dry_تسلسل) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		اگر (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			جاؤ برآمد;
		}
	}

	برائے (سائز_t i = 0; i < نتیجہ->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					نتیجہ->updates[i].refname,
					&نتیجہ->updates[i].new_oid,
					&نتیجہ->updates[i].old_oid,
					reflog_پیغام, &err);
		اگر (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتیجہ->updates[i].refname, err.buf);
			جاؤ برآمد;
		}
	}

	اگر (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	واپس ret;
}

ساکن صحیح handle_reference_updates(ساخت rev_info *revs,
				    شمار ref_action action,
				    ساخت commit *original,
				    ساخت commit *rewritten,
				    ثابت حرف *reflog_پیغام,
				    صحیح dry_تسلسل,
				    شمار replay_خالی_خانے_commit_action خالی_خانے)
{
	ساخت replay_نتیجہ نتیجہ = { 0 };
	صحیح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  خالی_خانے, &نتیجہ);
	اگر (ret)
		جاؤ برآمد;

	ret = apply_pending_ref_updates(revs->repo, &نتیجہ, reflog_پیغام, dry_تسلسل);

برآمد:
	replay_نتیجہ_release(&نتیجہ);
	واپس ret;
}

ساکن صحیح commit_became_خالی_خانے(ساخت repository *repo,
			       ساخت commit *original,
			       ساخت درخت *نتیجہ)
{
	ساخت object_id والد_درخت_oid;

	اگر (پہلا_والد_درخت_oid(repo, original, &والد_درخت_oid) < 0)
		واپس -1;

	واپس oideq(&نتیجہ->object.oid, &والد_درخت_oid);
}

ساکن صحیح parse_opt_خالی_خانے(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	شمار replay_خالی_خانے_commit_action *قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "drop"))
		*قدر = REPLAY_خالی_خانے_COMMIT_DROP;
	ورنہ اگر (!strcmp(arg, "keep"))
		*قدر = REPLAY_خالی_خانے_COMMIT_KEEP;
	ورنہ اگر (!strcmp(arg, "abort"))
		*قدر = REPLAY_خالی_خانے_COMMIT_ABORT;
	ورنہ
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	واپس 0;
}

ساکن صحیح cmd_history_fixup(صحیح argc,
			     ثابت حرف **argv,
			     ثابت حرف *سابقہ,
			     ساخت repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		تہی,
	};
	شمار replay_خالی_خانے_commit_action خالی_خانے = REPLAY_خالی_خانے_COMMIT_DROP;
	شمار ref_action action = REF_ACTION_DEFAULT;
	شمار commit_درخت_flags flags = 0;
	صحیح dry_تسلسل = 0;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_تسلسل,
			 N_("perform a dry-run without updating any refs")),
		OPT_ثنائی_ہندسہ(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_درخت_EDIT_پیغام),
		OPT_CALLBACK_F(0, "empty", &خالی_خانے, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی_خانے),
		OPT_انتہا(),
	};
	ساخت ملاؤ_نتیجہ ملاؤ_نتیجہ = { 0 };
	ساخت ملاؤ_options ملاؤ_opts = { 0 };
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	ساخت commit *سر_commit, *original, *rewritten;
	ساخت درخت *سر_درخت, *original_درخت, *اشاریہ_درخت;
	ساخت rev_info revs = { 0 };
	bool skip_commit = false;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	اگر (argc != 1) {
		ret = error(_("command expects a single revision"));
		جاؤ برآمد;
	}
	repo_config(repo, git_default_config, تہی);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	اگر (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		جاؤ برآمد;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		جاؤ برآمد;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	سر_commit = lookup_commit_reference_by_name("HEAD");
	اگر (!سر_commit) {
		ret = error(_("cannot look up HEAD"));
		جاؤ برآمد;
	}

	سر_درخت = repo_get_commit_درخت(repo, سر_commit);
	اگر (!سر_درخت) {
		ret = error(_("cannot get tree for HEAD"));
		جاؤ برآمد;
	}

	اگر (repo_read_اشاریہ(repo) < 0) {
		ret = error(_("unable to read index"));
		جاؤ برآمد;
	}

	اگر (!repo_اشاریہ_has_changes(repo, سر_درخت, تہی)) {
		ret = error(_("nothing to fixup: no staged changes"));
		جاؤ برآمد;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	اشاریہ_درخت = write_اندر_core_اشاریہ_as_درخت(repo, repo->اشاریہ);
	اگر (!اشاریہ_درخت) {
		ret = error(_("unable to write index as a tree"));
		جاؤ برآمد;
	}

	original_درخت = repo_get_commit_درخت(repo, original);
	اگر (!original_درخت) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		جاؤ برآمد;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_ملاؤ_options(&ملاؤ_opts, repo);
	ملاؤ_opts.ancestor = "HEAD";
	ملاؤ_opts.branch1 = argv[0];
	ملاؤ_opts.branch2 = "staged";
	ملاؤ_incore_nonrecursive(&ملاؤ_opts, سر_درخت,
				  original_درخت, اشاریہ_درخت, &ملاؤ_نتیجہ);

	اگر (ملاؤ_نتیجہ.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		جاؤ برآمد;
	}

	اگر (!ملاؤ_نتیجہ.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		جاؤ برآمد;
	}

	ret = commit_became_خالی_خانے(repo, original, ملاؤ_نتیجہ.درخت);
	اگر (ret < 0)
		جاؤ برآمد;
	اگر (ret > 0) {
		انتخاب (خالی_خانے) {
		حالت REPLAY_خالی_خانے_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->شے : تہی;

			/*
			 * TODO: we don't yet have the ability to drop root
			 * commits, but there's ultimately no good reason for
			 * this restriction to exist other than a technical
			 * limitation.
			 */
			اگر (!rewritten) {
				ret = error(_("cannot drop root commit %s: "
					      "it has no parent to replay onto"),
					    argv[0]);
				جاؤ برآمد;
			}

			skip_commit = true;
			توڑو;
		حالت REPLAY_خالی_خانے_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			توڑو;
		حالت REPLAY_خالی_خانے_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			جاؤ برآمد;
		}
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	اگر (ret)
		جاؤ برآمد;

	اگر (!skip_commit) {
		ret = commit_درخت_ext(repo, "fixup", original, تہی, original->parents,
				      &original_درخت->object.oid, &ملاؤ_نتیجہ.درخت->object.oid,
				      &rewritten, flags);
		اگر (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			جاؤ برآمد;
		}
	}

	strbuf_addf(&reflog_پیغام, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_تسلسل, خالی_خانے);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	ملاؤ_finalize(&ملاؤ_opts, &ملاؤ_نتیجہ);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	واپس ret;
}

ساکن صحیح cmd_history_reword(صحیح argc,
			      ثابت حرف **argv,
			      ثابت حرف *سابقہ,
			      ساخت repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		تہی,
	};
	شمار ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_تسلسل = 0;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_تسلسل,
			 N_("perform a dry-run without updating any refs")),
		OPT_انتہا(),
	};
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	ساخت commit *original, *rewritten;
	ساخت rev_info revs = { 0 };
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	اگر (argc != 1) {
		ret = error(_("command expects a single revision"));
		جاؤ برآمد;
	}
	repo_config(repo, git_default_config, تہی);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		جاؤ برآمد;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	اگر (ret)
		جاؤ برآمد;

	ret = commit_درخت_with_edited_پیغام(repo, "reworded", original, &rewritten);
	اگر (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		جاؤ برآمد;
	}

	strbuf_addf(&reflog_پیغام, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_تسلسل, REPLAY_خالی_خانے_COMMIT_ABORT);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	واپس ret;
}

ساکن صحیح write_ondisk_اشاریہ(ساخت repository *repo,
			      ساخت object_id *oid,
			      ثابت حرف *راستہ)
{
	ساخت unpack_trees_options opts = { 0 };
	ساخت تالا_file تالا = تالا_INIT;
	ساخت درخت_desc درخت_desc;
	ساخت اشاریہ_state اشاریہ;
	ساخت درخت *درخت;
	صحیح ret;

	اشاریہ_state_init(&اشاریہ, repo);

	opts.سر_idx = -1;
	opts.src_اشاریہ = &اشاریہ;
	opts.dst_اشاریہ = &اشاریہ;

	درخت = repo_parse_درخت_بالواسطہ(repo, oid);
	اگر (!درخت) {
		ret = error(_("unable to parse tree %s"), oid_مقصد_hex(oid));
		جاؤ برآمد;
	}
	init_درخت_desc(&درخت_desc, &درخت->object.oid, درخت->بفر, درخت->سائز);

	اگر (unpack_trees(1, &درخت_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		جاؤ برآمد;
	}

	مفرد_cache_درخت(repo, &اشاریہ, درخت);

	اگر (repo_hold_تالا_file_for_update(repo, &تالا, راستہ, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		جاؤ برآمد;
	}

	اگر (write_locked_اشاریہ(&اشاریہ, &تالا, COMMIT_تالا)) {
		ret = error(_("unable to write new index file"));
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	rollback_تالا_file(&تالا);
	release_اشاریہ(&اشاریہ);
	واپس ret;
}

ساکن صحیح بانٹو_commit(ساخت repository *repo,
			ساخت commit *original,
			ساخت pathspec *pathspec,
			ساخت commit **برآمد)
{
	ساخت interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	ساخت strbuf اشاریہ_file = STRBUF_INIT;
	ساخت اشاریہ_state اشاریہ = اشاریہ_STATE_INIT(repo);
	ثابت ساخت object_id *original_commit_درخت_oid;
	ثابت ساخت object_id *old_درخت_oid, *new_درخت_oid;
	ساخت object_id والد_درخت_oid;
	حرف original_commit_oid[GIT_MAX_HEXSZ + 1];
	ساخت commit *پہلا_commit, *دوسرا_commit;
	ساخت commit_list *parents = تہی;
	ساخت درخت *بانٹو_درخت;
	صحیح ret;

	اگر (پہلا_والد_درخت_oid(repo, original, &والد_درخت_oid) < 0) {
		ret = -1;
		جاؤ برآمد;
	}
	original_commit_درخت_oid = get_commit_درخت_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_راستہ_replace(repo, &اشاریہ_file, "%s", "history-split.index");

	ret = write_ondisk_اشاریہ(repo, &والد_درخت_oid, اشاریہ_file.buf);
	اگر (ret < 0)
		جاؤ برآمد;

	ret = read_اشاریہ_from(&اشاریہ, اشاریہ_file.buf, repo->gitdir);
	اگر (ret < 0) {
		ret = error(_("failed reading temporary index"));
		جاؤ برآمد;
	}

	oid_مقصد_hex_r(original_commit_oid, &original->object.oid);
	ret = تسلسل_جوڑو_p_اشاریہ(repo, &اشاریہ, اشاریہ_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, جوڑو_P_DISALLOW_EDIT);
	اگر (ret < 0)
		جاؤ برآمد;

	بانٹو_درخت = write_اندر_core_اشاریہ_as_درخت(repo, &اشاریہ);
	اگر (!بانٹو_درخت) {
		ret = error(_("failed split tree"));
		جاؤ برآمد;
	}

	unlink(اشاریہ_file.buf);
	strbuf_release(&اشاریہ_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	اگر (oideq(&بانٹو_درخت->object.oid, &والد_درخت_oid)) {
		ret = error(_("split commit is empty"));
		جاؤ برآمد;
	} ورنہ اگر (oideq(&بانٹو_درخت->object.oid, original_commit_درخت_oid)) {
		ret = error(_("split commit tree matches original commit"));
		جاؤ برآمد;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_درخت_ext(repo, "split-out", original, تہی, original->parents, &والد_درخت_oid,
			      &بانٹو_درخت->object.oid, &پہلا_commit, COMMIT_درخت_EDIT_پیغام);
	اگر (ret < 0) {
		ret = error(_("failed writing first commit"));
		جاؤ برآمد;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(پہلا_commit, &parents);

	old_درخت_oid = &repo_get_commit_درخت(repo, پہلا_commit)->object.oid;
	new_درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	ret = commit_درخت_ext(repo, "split-out", original, تہی, parents, old_درخت_oid,
			      new_درخت_oid, &دوسرا_commit, COMMIT_درخت_EDIT_پیغام);
	اگر (ret < 0) {
		ret = error(_("failed writing second commit"));
		جاؤ برآمد;
	}

	*برآمد = دوسرا_commit;
	ret = 0;

برآمد:
	اگر (اشاریہ_file.len)
		unlink(اشاریہ_file.buf);
	strbuf_release(&اشاریہ_file);
	commit_list_free(parents);
	release_اشاریہ(&اشاریہ);
	واپس ret;
}

ساکن صحیح cmd_history_بانٹو(صحیح argc,
			     ثابت حرف **argv,
			     ثابت حرف *سابقہ,
			     ساخت repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_بانٹو_USAGE,
		تہی,
	};
	شمار ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_تسلسل = 0;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_تسلسل,
			 N_("perform a dry-run without updating any refs")),
		OPT_انتہا(),
	};
	ساخت commit *original, *rewritten = تہی;
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	ساخت pathspec pathspec = { 0 };
	ساخت rev_info revs = { 0 };
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	اگر (argc < 1) {
		ret = error(_("command expects a committish"));
		جاؤ برآمد;
	}
	repo_config(repo, git_default_config, تہی);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_بھرا |
		       PATHSPEC_SYMLINK_LEADING_راستہ |
		       PATHSPEC_سابقہ_ORIGIN,
		       سابقہ, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		جاؤ برآمد;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	اگر (ret < 0)
		جاؤ برآمد;

	اگر (original->parents && original->parents->اگلا) {
		ret = error(_("cannot split up merge commit"));
		جاؤ برآمد;
	}

	ret = بانٹو_commit(repo, original, &pathspec, &rewritten);
	اگر (ret < 0)
		جاؤ برآمد;

	strbuf_addf(&reflog_پیغام, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_تسلسل, REPLAY_خالی_خانے_COMMIT_ABORT);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	strbuf_release(&reflog_پیغام);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	واپس ret;
}

#تعریف_کرو SQUASH_نظر_آیا (1u << 11)
#تعریف_کرو SQUASH_TIP (1u << 12)
#تعریف_کرو SQUASH_AMEND_ہدف (1u << 13)

ساکن bool is_autosquash_subject(ثابت حرف *s)
{
	واپس starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ساکن bool skip_one_autosquash_سابقہ(ثابت حرف *s, ثابت حرف **برآمد)
{
	اگر (skip_سابقہ(s, "amend!", برآمد) || skip_سابقہ(s, "fixup!", برآمد) ||
	    skip_سابقہ(s, "squash!", برآمد)) {
		جب (**برآمد == ' ')
			(*برآمد)++;
		واپس true;
	}
	واپس false;
}

ساکن خالی truncate_پیغام_مقصد_subject(ساخت strbuf *پیغام)
{
	ثابت حرف *eos = لڑی_تلاش(پیغام->buf, "\n\n");

	اگر (eos)
		strbuf_setlen(پیغام, eos - پیغام->buf + 1);
}

ساخت subject_ڈیٹا {
	ساخت strintmap subjects;
	ساخت strbuf subject;
	ساخت strbuf squash_پیغام;
	ثابت حرف *پیغام_2;
	bool edit_پیغام;
};

#تعریف_کرو SUBJECT_ڈیٹا_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_پیغام = STRBUF_INIT,	\
}

ساکن خالی subject_ڈیٹا_clear(ساخت subject_ڈیٹا *ڈیٹا)
{
	strintmap_clear(&ڈیٹا->subjects);
	strbuf_release(&ڈیٹا->subject);
	strbuf_release(&ڈیٹا->squash_پیغام);
}

ساکن صحیح squash_amend_پیغام(ساخت repository *repo,
				ساخت commit *commit,
				ساخت subject_ڈیٹا *ڈیٹا,
				غیرعلامتی flags)
{
	ثابت حرف *body = ڈیٹا->پیغام_2 + ڈیٹا->subject.len;

	جب (خلا_ہے(*body))
		body++;

	اگر (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_ڈھونڈو_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			ڈیٹا->subject.buf);
		واپس 0;
	}

	اگر (ڈیٹا->edit_پیغام) {
		واپس 0;
	} ورنہ اگر (flags & SQUASH_AMEND_ہدف) {
		اگر (starts_with(ڈیٹا->squash_پیغام.buf, "squash!"))
			واپس error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_ڈھونڈو_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     ڈیٹا->subject.buf);
		اگر (starts_with(ڈیٹا->squash_پیغام.buf, "fixup!"))
			strbuf_splice(&ڈیٹا->squash_پیغام, 0, 5, "amend!", 5);
		اگر (starts_with(ڈیٹا->squash_پیغام.buf, "amend!")) {
			truncate_پیغام_مقصد_subject(&ڈیٹا->squash_پیغام);
			strbuf_addch(&ڈیٹا->squash_پیغام, '\n');
		} ورنہ {
			strbuf_reset(&ڈیٹا->squash_پیغام);
		}
		strbuf_addstr(&ڈیٹا->squash_پیغام, body);
		strbuf_complete_سطر(&ڈیٹا->squash_پیغام);
	} ورنہ {
		واپس error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_ڈھونڈو_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     ڈیٹا->subject.buf);
	}
	واپس 0;
}

ساکن صحیح squash_squash_پیغام(ساخت repository *repo,
				ساخت commit *commit,
				ساخت subject_ڈیٹا *ڈیٹا,
				غیرعلامتی flags)
{
	ثابت حرف *body = ڈیٹا->پیغام_2 + ڈیٹا->subject.len;

	جب (خلا_ہے(*body))
		body++;

	اگر (ڈیٹا->edit_پیغام) {
		واپس 0;
	} ورنہ اگر (flags & SQUASH_AMEND_ہدف) {
		اگر (starts_with(ڈیٹا->squash_پیغام.buf, "fixup!")) {
			truncate_پیغام_مقصد_subject(&ڈیٹا->squash_پیغام);
			strbuf_splice(&ڈیٹا->squash_پیغام, 0, 5, "squash", 6);
		}
		اگر (starts_with(ڈیٹا->squash_پیغام.buf, "squash!")) {
			strbuf_addch(&ڈیٹا->squash_پیغام, '\n');
			strbuf_addstr(&ڈیٹا->squash_پیغام, body);
			strbuf_complete_سطر(&ڈیٹا->squash_پیغام);
		} ورنہ {
			واپس error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_ڈھونڈو_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     ڈیٹا->subject.buf);
		}
	} ورنہ {
		واپس error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_ڈھونڈو_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      ڈیٹا->subject.buf);
	}
	واپس 0;
}

ساکن صحیح squash_check_can_autosquash(ساخت repository *repo,
				       ساخت commit *commit,
				       ساخت subject_ڈیٹا *ڈیٹا,
				       غیرعلامتی flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_ہدف;
	اگر (starts_with(ڈیٹا->subject.buf, "amend!"))
		واپس squash_amend_پیغام(repo, commit, ڈیٹا, flags);
	ورنہ اگر (starts_with(ڈیٹا->subject.buf, "squash!"))
		واپس squash_squash_پیغام(repo, commit, ڈیٹا, flags);

	واپس 0;
}

ساکن صحیح squash_check_autosquash_subject(ساخت repository *repo,
					   ساخت commit *commit,
					   ساخت subject_ڈیٹا *ڈیٹا)
{
	ثابت حرف* s = ڈیٹا->subject.buf;
	ساخت commit *ہدف;
	ساخت hashmap_iter iter;
	ساخت strmap_اندراج *اندراج_2;
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
	جب (skip_one_autosquash_سابقہ(s, &s)) {
		غیرعلامتی flags = strintmap_get(&ڈیٹا->subjects, s);
		اگر (flags)
			واپس squash_check_can_autosquash(repo, commit, ڈیٹا, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	ہدف = lookup_commit_reference_by_name(s);
	اگر (ہدف && starts_with(oid_مقصد_hex(&ہدف->object.oid), s)) {
		غیرعلامتی flags =
			ہدف->object.flags & (SQUASH_نظر_آیا | SQUASH_AMEND_ہدف);
		اگر (!flags) {
			ثابت حرف *subject_آغاز;
			ثابت حرف *بفر = repo_logmsg_reencode(repo, ہدف,
								  تہی, تہی);
			سائز_t subject_len = ڈھونڈو_commit_subject(بفر,
								 &subject_آغاز);
			حرف *subject = xmemdupz(subject_آغاز, subject_len);

			flags = strintmap_get(&ڈیٹا->subjects, subject);
			آزاد(subject);
			repo_unuse_commit_بفر(repo, ہدف, بفر);
		}
		اگر (flags)
			واپس squash_check_can_autosquash(repo, commit,
							   ڈیٹا, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_اندراج(&ڈیٹا->subjects, &iter, اندراج_2) {
		s = ڈیٹا->subject.buf;
		جب(skip_one_autosquash_سابقہ(s, &s)) {
			اگر (starts_with(اندراج_2->کلید, s)) {
				غیرعلامتی قدر = (intptr_t)اندراج_2->قدر;

				واپس squash_check_can_autosquash(repo, commit,
								   ڈیٹا, قدر);
			}
		}
	}
	واپس error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_ڈھونڈو_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       ڈیٹا->subject.buf);
}

ساکن صحیح squash_check_subject(ساخت repository *repo,
				ساخت commit *commit,
				ساخت subject_ڈیٹا *ڈیٹا)
{
	صحیح ret = 0;
	ثابت حرف *buf = repo_logmsg_reencode(repo, commit, تہی, تہی);
	سائز_t subject_len = ڈھونڈو_commit_subject(buf, &ڈیٹا->پیغام_2);

	strbuf_reset(&ڈیٹا->subject);
	strbuf_جوڑو(&ڈیٹا->subject, ڈیٹا->پیغام_2, subject_len);

	اگر (!strintmap_get_سائز(&ڈیٹا->subjects)) {
		ثابت حرف *s;

		strbuf_addstr(&ڈیٹا->squash_پیغام, ڈیٹا->پیغام_2);
		strbuf_complete_سطر(&ڈیٹا->squash_پیغام);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = ڈیٹا->subject.buf;
		skip_one_autosquash_سابقہ(s, &s);
		strintmap_سیٹ(&ڈیٹا->subjects, s, SQUASH_AMEND_ہدف | SQUASH_نظر_آیا);
		commit->object.flags |= SQUASH_AMEND_ہدف;
	} ورنہ اگر (is_autosquash_subject(ڈیٹا->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, ڈیٹا);
	} ورنہ {
		strintmap_سیٹ(&ڈیٹا->subjects, ڈیٹا->subject.buf, SQUASH_نظر_آیا);
	}
	repo_unuse_commit_بفر(repo, commit, buf);
	واپس ret;
}

ساکن صحیح build_squash_پیغام(ساخت repository *repo,
				ثابت ساخت strbuf *todo_buf,
				ساخت strbuf *برآمد);

ساکن صحیح تیاری_squash_revisions(ساخت repository *repo,
				  صحیح argc, ثابت حرف **argv,
				  ساخت rev_info *revs)
{
	repo_init_revisions(repo, revs, تہی);
	revs->الٹ = 1;
	revs->topo_ترتیب = 1;
	revs->ترتیب_دو_ترتیب = REV_ترتیب_دو_اندر_گراف_ترتیب;
	revs->simplify_history = 0;
	revs->ancestry_راستہ = 1;
	revs->limited = 1;
	revs->ancestry_راستہ_implicit_bottoms = 1;

	argc = تیاری_revisions(argc, argv, revs, تہی);
	اگر (argc > 1)
		واپس error(_("unrecognized argument: %s"), argv[1]);

	اگر (revs->الٹ != 1 || revs->topo_ترتیب != 1 ||
	    revs->ترتیب_دو_ترتیب != REV_ترتیب_دو_اندر_گراف_ترتیب ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_راستہ != 1 || revs->limited != 1 ||
	    revs->ancestry_راستہ_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->الٹ = 1;
		revs->topo_ترتیب = 1;
		revs->ترتیب_دو_ترتیب = REV_ترتیب_دو_اندر_گراف_ترتیب;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_راستہ = 1;
		revs->limited = 1;
		revs->ancestry_راستہ_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	برائے (سائز_t i = 0; i < revs->cmdline.nr; i++)
		اگر (revs->cmdline.rev[i].flags & BOTTOM)
			واپس 0;

	واپس error(_("not a '<base>..<tip>' revision range"));
}

/*
 * Resolve a revision range into its oldest commit and single tip. Every
 * parent after the oldest commit must either be selected or also be a parent
 * of the oldest commit.
 */
ساکن صحیح resolve_squash_range(ساخت repository *repo,
				bool update_branches,
				bool edit_پیغام,
				صحیح argc, ثابت حرف **argv,
				ساخت commit **oldest_برآمد,
				ساخت commit **tip_برآمد,
				حرف **پیغام_برآمد)
{
	ساخت rev_info revs;
	ساخت subject_ڈیٹا subject_ڈیٹا = SUBJECT_ڈیٹا_INIT;
	ساخت commit *commit, *oldest = تہی, *tip = تہی;
	ساخت strbuf todo_buf = STRBUF_INIT;
	صحیح ret, tip_گنتی = 0;
	bool walk_started = false;
	ساخت ref_filter filter = REF_FILTER_INIT;
	ساخت ref_array refs = { 0 };

	subject_ڈیٹا.edit_پیغام = edit_پیغام;
	ret = تیاری_squash_revisions(repo, argc, argv, &revs);
	اگر (ret < 0)
		جاؤ برآمد;

	اگر (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		جاؤ برآمد;
	}
	walk_started = true;
	جب ((commit = get_revision(&revs))) {
		ساخت commit_list *p;

		اگر (edit_پیغام)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_مقصد_hex(&commit->object.oid));

		اگر (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			جاؤ برآمد;
		}
		برائے (p = commit->parents; oldest && p; p = p->اگلا) {
			ساخت commit_list *q;
			ساخت object *o;
			bool نظر_آیا;

			اگر (repo_parse_commit(repo, p->شے)) {
				ret = error(_("cannot parse commit"));
				جاؤ برآمد;
			}
			o = &p->شے->object;
			نظر_آیا = o->flags & SQUASH_نظر_آیا;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			برائے (q = oldest->parents; !نظر_آیا && q; q = q->اگلا) {
				اگر (p->شے == q->شے) {
					نظر_آیا = true;
					commit_list_داخل_کرو(commit, &filter.with_commit);
				}
			}
			اگر (!نظر_آیا) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_ڈھونڈو_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_ڈھونڈو_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				جاؤ برآمد;
			}
			اگر (o->flags & SQUASH_TIP) {
				tip_گنتی--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		اگر (!oldest) {
			commit_list_داخل_کرو(commit, &filter.with_commit);
			oldest = commit;
		}
		اگر (squash_check_subject(repo, commit, &subject_ڈیٹا)) {
			ret = -1;
			جاؤ برآمد;
		}
		tip = commit;
		tip->object.flags |= SQUASH_نظر_آیا | SQUASH_TIP;
		tip_گنتی++;
	}
	clear_object_flags(repo, SQUASH_نظر_آیا | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	اگر (!tip_گنتی) {
		ret = error(_("the revision range is empty"));
		جاؤ برآمد;
	} ورنہ اگر (tip_گنتی != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		جاؤ برآمد;
	} ورنہ اگر (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		جاؤ برآمد;
	} ورنہ اگر (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_داخل_کرو(tip, &filter.no_commit);
	filter.قسم = FILTER_REFS_BRANCHES;
	اگر (update_branches &&
	    filter_refs(&refs, &filter, filter.قسم)) {
		ret = error(_("could not filter refs"));
		جاؤ برآمد;
	}
	اگر (refs.nr) {
		ساخت ref_format format = REF_FORMAT_INIT;
		ساخت ref_ترتیب_کاری *ترتیب_کاری;
		ساخت string_list ترتیب_کاری_options = STRING_LIST_INIT_DUP;
		ساخت strbuf branches = STRBUF_INIT;
		ساخت strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		اگر (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&ترتیب_کاری_options, "refname");
		ترتیب_کاری = ref_ترتیب_کاری_options(&ترتیب_کاری_options);
		ref_array_ترتیب_دو(ترتیب_کاری, &refs);
		برائے (صحیح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			اگر (format_ref_array_شے(refs.اشیاء[i], &format,
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
		ref_ترتیب_کاری_release(ترتیب_کاری);
		string_list_clear(&ترتیب_کاری_options, 0);
		جاؤ برآمد;
	}
	اگر (edit_پیغام) {
		strbuf_reset(&subject_ڈیٹا.squash_پیغام);
		ret = build_squash_پیغام(repo, &todo_buf,
					   &subject_ڈیٹا.squash_پیغام);
		اگر (ret < 0)
			جاؤ برآمد;
	}

	*oldest_برآمد = oldest;
	*tip_برآمد = tip;
	*پیغام_برآمد = strbuf_detach(&subject_ڈیٹا.squash_پیغام, تہی);
	ret = 0;

برآمد:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_نظر_آیا | SQUASH_TIP |
			   SQUASH_AMEND_ہدف);
	اگر (walk_started)
		reset_revision_walk();
	subject_ڈیٹا_clear(&subject_ڈیٹا);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	واپس ret;
}

ساکن bool amend_replaces_ہدف(ساخت todo_list *todo, صحیح ہدف)
{
	برائے (صحیح i = ہدف + 1; i < todo->nr &&
				 todo->اشیاء[i].command != TODO_PICK; i++) {
		اگر (todo->اشیاء[i].command == TODO_SQUASH)
			واپس false;
		اگر (todo->اشیاء[i].flags & TODO_REPLACE_FIXUP_پیغام)
			واپس true;
	}
	واپس false;
}

ساکن صحیح build_squash_پیغام(ساخت repository *repo,
				ثابت ساخت strbuf *todo_buf,
				ساخت strbuf *برآمد)
{
	ساخت todo_list todo = TODO_LIST_INIT;
	ساخت replay_opts opts = REPLAY_OPTS_INIT;
	صحیح nr_commits, ret;

	اگر (todo_list_parse_insn_بفر(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		جاؤ برآمد;
	}

	nr_commits = todo.nr;
	برائے (صحیح i = 0; i < nr_commits; i++) {
		ساخت todo_شے *شے = &todo.اشیاء[i];
		ثابت حرف *پیغام_2, *body;
		سائز_t commented_len;
		bool skip, squashing;

		squashing = شے->command == TODO_SQUASH ||
			    (شے->flags & TODO_REPLACE_FIXUP_پیغام);
		اگر (شے->command == TODO_PICK)
			skip = amend_replaces_ہدف(&todo, i);
		ورنہ
			skip = !squashing;

		پیغام_2 = repo_logmsg_reencode(repo, شے->commit, تہی, تہی);
		ڈھونڈو_commit_subject(پیغام_2, &body);

		اگر (skip)
			commented_len = لڑی_لمبائی(body);
		ورنہ اگر (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		ورنہ
			commented_len = 0;

		اگر (!i)
			جوڑو_squash_combination_سرخی(برآمد, nr_commits);
		strbuf_addch(برآمد, '\n');
		جوڑو_squash_پیغام_سرخی(برآمد, i + 1, skip);
		strbuf_addstr(برآمد, "\n\n");
		strbuf_جوڑو_commented_سطریں(برآمد, body, commented_len, comment_سطر_str);
		strbuf_addstr(برآمد, body + commented_len);
		strbuf_complete_سطر(برآمد);

		repo_unuse_commit_بفر(repo, شے->commit, پیغام_2);
	}

	ret = 0;

برآمد:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	واپس ret;
}

ساکن صحیح cmd_history_squash(صحیح argc,
			      ثابت حرف **argv,
			      ثابت حرف *سابقہ,
			      ساخت repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		تہی,
	};
	شمار ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_تسلسل = 0;
	صحیح edit = 1;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_تسلسل,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_انتہا(),
	};
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	ساخت commit *oldest, *tip, *rewritten;
	ثابت ساخت object_id *بنیاد_درخت_oid, *tip_درخت_oid;
	حرف *پیغام_template = تہی;
	ساخت rev_info revs = { 0 };
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	اگر (argc < 2) {
		ret = error(_("command expects a revision range"));
		جاؤ برآمد;
	}
	repo_config(repo, git_default_config, تہی);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_پیغام, "squash: updating ");
	strbuf_join_argv(&reflog_پیغام, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &پیغام_template);
	اگر (ret < 0)
		جاؤ برآمد;

	ret = تیاری_revwalk(repo, action, tip, &revs);
	اگر (ret < 0)
		جاؤ برآمد;

	بنیاد_درخت_oid = &repo_get_commit_درخت(repo,
					oldest->parents->شے)->object.oid;
	tip_درخت_oid = &repo_get_commit_درخت(repo, tip)->object.oid;

	ret = commit_درخت_ext(repo, "squash", oldest, پیغام_template,
			      oldest->parents, بنیاد_درخت_oid, tip_درخت_oid,
			      &rewritten,
			      edit ? COMMIT_درخت_EDIT_پیغام : 0);
	اگر (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		جاؤ برآمد;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_پیغام.buf, dry_تسلسل,
				       REPLAY_خالی_خانے_COMMIT_ABORT);
	اگر (ret < 0) {
		ret = error(_("failed replaying descendants"));
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	آزاد(پیغام_template);
	واپس ret;
}

ساکن صحیح update_worktree(ساخت repository *repo,
			   ثابت ساخت commit *old_سر,
			   ثابت ساخت commit *new_سر,
			   bool dry_تسلسل)
{
	ساخت reset_working_درخت_options opts = {
		.oid_from = &old_سر->object.oid,
		.oid = &new_سر->object.oid,
	};
	اگر (dry_تسلسل)
		opts.flags |= RESET_WORKING_درخت_DRY_تسلسل;
	واپس reset_working_درخت(repo, &opts);
}

ساکن صحیح ڈھونڈو_سر_درخت_change(ساخت repository *repo,
				 ثابت ساخت replay_نتیجہ *نتیجہ,
				 ساخت commit **old_سر,
				 ساخت commit **new_سر,
				 bool *بدلا)
{
	ثابت ساخت replay_ref_update *سر_update = تہی;
	ساخت commit *old_سر_commit, *new_سر_commit;
	ساخت درخت *old_سر_درخت, *new_سر_درخت;
	ثابت حرف *سر_ہدف;
	صحیح سر_flags;

	*بدلا = false;

	سر_ہدف = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      تہی, &سر_flags);
	اگر (!سر_ہدف)
		واپس error(_("cannot look up HEAD"));

	برائے (سائز_t i = 0; i < نتیجہ->updates_nr; i++) {
		اگر (!strcmp(نتیجہ->updates[i].refname, سر_ہدف)) {
			سر_update = &نتیجہ->updates[i];
			توڑو;
		}
	}

	اگر (!سر_update)
		واپس 0;

	old_سر_commit = lookup_commit_reference(repo, &سر_update->old_oid);
	new_سر_commit = lookup_commit_reference(repo, &سر_update->new_oid);
	اگر (!old_سر_commit || !new_سر_commit)
		واپس error(_("cannot resolve HEAD commit"));

	old_سر_درخت = repo_get_commit_درخت(repo, old_سر_commit);
	new_سر_درخت = repo_get_commit_درخت(repo, new_سر_commit);
	اگر (!old_سر_درخت || !new_سر_درخت)
		واپس error(_("cannot resolve tree for HEAD"));

	اگر (oideq(&old_سر_درخت->object.oid, &new_سر_درخت->object.oid))
		واپس 0;

	*old_سر = old_سر_commit;
	*new_سر = new_سر_commit;
	*بدلا = true;

	واپس 0;
}

ساکن صحیح cmd_history_drop(صحیح argc,
			    ثابت حرف **argv,
			    ثابت حرف *سابقہ,
			    ساخت repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		تہی,
	};
	شمار replay_خالی_خانے_commit_action خالی_خانے = REPLAY_خالی_خانے_COMMIT_DROP;
	شمار ref_action action = REF_ACTION_DEFAULT;
	صحیح dry_تسلسل = 0;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_تسلسل,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &خالی_خانے, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی_خانے),
		OPT_انتہا(),
	};
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	ساخت commit *original, *rewritten;
	ساخت rev_info revs = { 0 };
	ساخت replay_نتیجہ نتیجہ = { 0 };
	ساخت commit *old_سر, *new_سر;
	bool سر_چالیں = false;
	صحیح ret;

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	اگر (argc != 1) {
		ret = error(_("command expects a single revision"));
		جاؤ برآمد;
	}
	repo_config(repo, git_default_config, تہی);

	اگر (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	اگر (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		جاؤ برآمد;
	}

	اگر (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		جاؤ برآمد;
	} ورنہ اگر (original->parents->اگلا) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		جاؤ برآمد;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	اگر (ret)
		جاؤ برآمد;

	rewritten = original->parents->شے;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  خالی_خانے, &نتیجہ);
	اگر (ret) {
		ret = error(_("failed replaying descendants"));
		جاؤ برآمد;
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
	اگر (!is_bare_repository(repo)) {
		ret = ڈھونڈو_سر_درخت_change(repo, &نتیجہ, &old_سر,
					    &new_سر, &سر_چالیں);
		اگر (ret < 0)
			جاؤ برآمد;

		اگر (سر_چالیں && update_worktree(repo, old_سر, new_سر, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			جاؤ برآمد;
		}
	}

	strbuf_addf(&reflog_پیغام, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &نتیجہ, reflog_پیغام.buf, dry_تسلسل);
	اگر (ret < 0) {
		ret = error(_("failed to update references"));
		جاؤ برآمد;
	}

	اگر (!dry_تسلسل && سر_چالیں && update_worktree(repo, old_سر, new_سر, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_مقصد_hex(&new_سر->object.oid));
		جاؤ برآمد;
	}

	ret = 0;

برآمد:
	replay_نتیجہ_release(&نتیجہ);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	واپس ret;
}

صحیح cmd_history(صحیح argc,
		ثابت حرف **argv,
		ثابت حرف *سابقہ,
		ساخت repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_بانٹو_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		تہی,
	};
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_بانٹو),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);
	واپس fn(argc, argv, سابقہ, repo);
}
