#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کرو GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#تعریف_کرو GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#تعریف_کرو GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#تعریف_کرو GIT_HISTORY_ونڈو_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#تعریف_کرو GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ساكن فراغ change_انکڑے_free(فراغ *util, ثابت حرف *str بن_ورتیا)
{
	بنية wt_حالت_change_انکڑے *d = util;
	حرر(d->rename_سروت);
	حرر(d);
}

ساكن صحيح fill_commit_سنیہا(بنية repository *repo,
			       ثابت بنية object_id *old_درخت,
			       ثابت بنية object_id *new_درخت,
			       ثابت حرف *default_سنیہا,
			       ثابت حرف *action,
			       بنية strbuf *باہرلا)
{
	ثابت حرف *رستہ = git_رستہ_commit_editmsg();
	ثابت حرف *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	بنية wt_حالت s;

	wt_حالت_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_تیز;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_اگیتر = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(رستہ, "w");
	إذا (!s.fp)
		أرجع error_errno(_("could not open '%s'"), رستہ);

	strbuf_addstr(باہرلا, default_سنیہا);
	strbuf_addch(باہرلا, '\n');
	strbuf_commented_addf(باہرلا, comment_سطر_str, hint, action, comment_سطر_str);
	إذا (fwrite(باہرلا->buf, 1, باہرلا->len, s.fp) != باہرلا->len)
		die_errno(_("could not write to '%s'"), رستہ);

	wt_حالت_collect_changes_trees(&s, old_درخت, new_درخت);
	wt_حالت_print(&s);
	wt_حالت_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_انکڑے_free);
	إذا (fclose(s.fp))
		die_errno(_("could not write to '%s'"), رستہ);

	strbuf_reset(باہرلا);
	إذا (launch_editor(رستہ, باہرلا, عدم)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		أرجع -1;
	}
	strbuf_stripspace(باہرلا, comment_سطر_str);

	cleanup_سنیہا(باہرلا, COMMIT_پیغام_CLEANUP_ALL, 0);

	إذا (!باہرلا->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		أرجع -1;
	}

	أرجع 0;
}

تعداد commit_درخت_flags {
	COMMIT_درخت_EDIT_سنیہا = (1 << 0),
};

ساكن صحيح commit_درخت_ext(بنية repository *repo,
			   ثابت حرف *action,
			   بنية commit *commit_with_سنیہا,
			   ثابت حرف *سنیہا_template,
			   ثابت بنية commit_list *parents,
			   ثابت بنية object_id *old_درخت,
			   ثابت بنية object_id *new_درخت,
			   بنية commit **باہرلا,
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
	ثابت حرف *original_سنیہا, *original_body, *ptr;
	بنية commit_extra_سرخی *original_extra_سرخیاں = عدم;
	بنية strbuf commit_سنیہا = STRBUF_INIT;
	بنية object_id rewritten_commit_oid;
	حرف *original_author = عدم;
	ناپ_t len;
	صحيح ret;

	/* We retain authorship of the original commit. */
	original_سنیہا = repo_logmsg_reencode(repo, commit_with_سنیہا, عدم, عدم);
	ptr = لبھو_commit_سرخی(original_سنیہا, "author", &len);
	إذا (ptr)
		original_author = xmemdupz(ptr, len);
	لبھو_commit_subject(original_سنیہا, &original_body);

	إذا (!سنیہا_template)
		سنیہا_template = original_body;

	إذا (flags & COMMIT_درخت_EDIT_سنیہا) {
		ret = fill_commit_سنیہا(repo, old_درخت, new_درخت,
					  سنیہا_template, action, &commit_سنیہا);
		إذا (ret < 0)
			اذهب باہرلا;
	} وإلا {
		strbuf_addstr(&commit_سنیہا, سنیہا_template);
	}

	original_extra_سرخیاں = read_commit_extra_سرخیاں(commit_with_سنیہا,
							   exclude_gpgsig);

	ret = commit_درخت_extended(commit_سنیہا.buf, commit_سنیہا.len, new_درخت,
				   parents, &rewritten_commit_oid, original_author,
				   عدم, عدم, original_extra_سرخیاں);
	إذا (ret < 0)
		اذهب باہرلا;

	*باہرلا = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

باہرلا:
	free_commit_extra_سرخیاں(original_extra_سرخیاں);
	strbuf_release(&commit_سنیہا);
	حرر(original_author);
	repo_unuse_commit_بفر(repo, commit_with_سنیہا, original_سنیہا);
	أرجع ret;
}

ساكن صحيح پہلا_ماپا_درخت_oid(بنية repository *repo,
				 بنية commit *commit,
				 بنية object_id *باہرلا)
{
	بنية commit *ماپا = commit->parents ? commit->parents->شے : عدم;

	إذا (!ماپا) {
		oidcpy(باہرلا, repo->hash_algo->خالی_تھاواں_درخت);
		أرجع 0;
	}

	إذا (repo_parse_commit(repo, ماپا))
		أرجع error(_("unable to parse parent commit %s"),
			     oid_منزل_hex(&ماپا->object.oid));

	oidcpy(باہرلا, &repo_get_commit_درخت(repo, ماپا)->object.oid);
	أرجع 0;
}

ساكن صحيح commit_درخت_with_edited_سنیہا(بنية repository *repo,
					   ثابت حرف *action,
					   بنية commit *original,
					   بنية commit **باہرلا)
{
	بنية object_id ماپا_درخت_oid;
	ثابت بنية object_id *درخت_oid;

	درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	إذا (پہلا_ماپا_درخت_oid(repo, original, &ماپا_درخت_oid) < 0)
		أرجع -1;

	أرجع commit_درخت_ext(repo, action, original, عدم, original->parents,
			       &ماپا_درخت_oid, درخت_oid, باہرلا, COMMIT_درخت_EDIT_سنیہا);
}

تعداد ref_action {
	REF_ACTION_DEFAULT,
	REF_ACTION_BRANCHES,
	REF_ACTION_سر,
};

ساكن صحيح parse_ref_action(ثابت بنية option *opt, ثابت حرف *قدر, صحيح unset)
{
	تعداد ref_action *action = opt->قدر;

	BUG_ON_OPT_NEG_NOARG(unset, قدر);
	إذا (!strcmp(قدر, "branches")) {
		*action = REF_ACTION_BRANCHES;
	} وإلا إذا (!strcmp(قدر, "head")) {
		*action = REF_ACTION_سر;
	} وإلا {
		أرجع error(_("%s expects one of 'branches' or 'head'"),
			     opt->long_name);
	}

	أرجع 0;
}

ساكن صحيح revwalk_شامل_اے_merges(بنية repository *repo,
				   ثابت بنية strvec *revwalk_دلیلاں)
{
	بنية strvec دلیلاں = STRVEC_INIT;
	بنية rev_info revs;
	صحيح ret;

	strvec_pushv(&دلیلاں, revwalk_دلیلاں->v);
	strvec_ڈالو(&دلیلاں, "--min-parents=2");

	repo_init_revisions(repo, &revs, عدم);

	تیاری_revisions_from_strvec(&دلیلاں, &revs, عدم);
	إذا (دلیلاں.nr != 1)
		BUG("revisions were set up with invalid argument");

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب باہرلا;
	}

	إذا (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		اذهب باہرلا;
	}

	reset_revision_walk();
	ret = 0;

باہرلا:
	release_revisions(&revs);
	strvec_clear(&دلیلاں);
	أرجع ret;
}

ساكن صحيح تیاری_revwalk(بنية repository *repo,
			 تعداد ref_action action,
			 بنية commit *original,
			 بنية rev_info *revs)
{
	بنية strvec دلیلاں = STRVEC_INIT;
	صحيح ret;

	repo_init_revisions(repo, revs, عدم);
	strvec_ڈالو(&دلیلاں, "ignored");
	strvec_ڈالو(&دلیلاں, "--reverse");
	strvec_ڈالو(&دلیلاں, "--topo-order");
	strvec_ڈالو(&دلیلاں, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&دلیلاں, "--ancestry-path=%s",
		     oid_منزل_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&دلیلاں, "^%s", oid_منزل_hex(&original->object.oid));

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
			اذهب باہرلا;
		}

		commit_list_داخل_کرو(original, &from_list);
		ret = repo_is_descendant_دا(repo, سر, from_list);
		commit_list_free(from_list);

		إذا (ret < 0) {
			ret = error(_("cannot determine descendance"));
			اذهب باہرلا;
		} وإلا إذا (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			اذهب باہرلا;
		}

		strvec_ڈالو(&دلیلاں, "HEAD");
	} وإلا {
		strvec_ڈالو(&دلیلاں, "--branches");
		strvec_ڈالو(&دلیلاں, "HEAD");
	}

	ret = revwalk_شامل_اے_merges(repo, &دلیلاں);
	إذا (ret < 0)
		اذهب باہرلا;

	تیاری_revisions_from_strvec(&دلیلاں, revs, عدم);
	إذا (دلیلاں.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

باہرلا:
	strvec_clear(&دلیلاں);
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
		لکھو("update %s %s %s\n",
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
				       تعداد replay_خالی_تھاواں_commit_action خالی_تھاواں,
				       بنية replay_نتیجہ *نتیجہ)
{
	ثابت بنية name_decoration *decoration;
	بنية replay_revisions_options opts = {
		.خالی_تھاواں = خالی_تھاواں,
	};
	حرف hex[GIT_MAX_HEXSZ + 1];
	bool detached_سر;
	صحيح سر_flags = 0;
	صحيح ret;

	refs_read_ref_بھریا(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, عدم, &سر_flags);
	detached_سر = !(سر_flags & REF_ISSYMREF);

	opts.onto = oid_منزل_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, نتیجہ);
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
	     decoration = decoration->اگلا)
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

		replay_نتیجہ_قطار_update(نتیجہ, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	أرجع 0;
}

ساكن صحيح apply_pending_ref_updates(بنية repository *repo,
				     ثابت بنية replay_نتیجہ *نتیجہ,
				     ثابت حرف *reflog_پیغام,
				     صحيح dry_لگاتار)
{
	بنية ref_transaction *transaction = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret;

	إذا (!dry_لگاتار) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			اذهب باہرلا;
		}
	}

	لكل (ناپ_t i = 0; i < نتیجہ->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					نتیجہ->updates[i].refname,
					&نتیجہ->updates[i].new_oid,
					&نتیجہ->updates[i].old_oid,
					reflog_پیغام, &err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتیجہ->updates[i].refname, err.buf);
			اذهب باہرلا;
		}
	}

	إذا (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	أرجع ret;
}

ساكن صحيح handle_reference_updates(بنية rev_info *revs,
				    تعداد ref_action action,
				    بنية commit *original,
				    بنية commit *rewritten,
				    ثابت حرف *reflog_پیغام,
				    صحيح dry_لگاتار,
				    تعداد replay_خالی_تھاواں_commit_action خالی_تھاواں)
{
	بنية replay_نتیجہ نتیجہ = { 0 };
	صحيح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  خالی_تھاواں, &نتیجہ);
	إذا (ret)
		اذهب باہرلا;

	ret = apply_pending_ref_updates(revs->repo, &نتیجہ, reflog_پیغام, dry_لگاتار);

باہرلا:
	replay_نتیجہ_release(&نتیجہ);
	أرجع ret;
}

ساكن صحيح commit_became_خالی_تھاواں(بنية repository *repo,
			       بنية commit *original,
			       بنية درخت *نتیجہ)
{
	بنية object_id ماپا_درخت_oid;

	إذا (پہلا_ماپا_درخت_oid(repo, original, &ماپا_درخت_oid) < 0)
		أرجع -1;

	أرجع oideq(&نتیجہ->object.oid, &ماپا_درخت_oid);
}

ساكن صحيح parse_opt_خالی_تھاواں(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد replay_خالی_تھاواں_commit_action *قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "drop"))
		*قدر = REPLAY_خالی_تھاواں_COMMIT_DROP;
	وإلا إذا (!strcmp(arg, "keep"))
		*قدر = REPLAY_خالی_تھاواں_COMMIT_KEEP;
	وإلا إذا (!strcmp(arg, "abort"))
		*قدر = REPLAY_خالی_تھاواں_COMMIT_ABORT;
	وإلا
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	أرجع 0;
}

ساكن صحيح cmd_history_fixup(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *اگیتر,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		عدم,
	};
	تعداد replay_خالی_تھاواں_commit_action خالی_تھاواں = REPLAY_خالی_تھاواں_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	تعداد commit_درخت_flags flags = 0;
	صحيح dry_لگاتار = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لگاتار,
			 N_("perform a dry-run without updating any refs")),
		OPT_ثنائی_ہندسہ(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_درخت_EDIT_سنیہا),
		OPT_CALLBACK_F(0, "empty", &خالی_تھاواں, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی_تھاواں),
		OPT_سرا(),
	};
	بنية ملاؤ_نتیجہ ملاؤ_نتیجہ = { 0 };
	بنية ملاؤ_options ملاؤ_opts = { 0 };
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *سر_commit, *original, *rewritten;
	بنية درخت *سر_درخت, *original_درخت, *اشاریہ_درخت;
	بنية rev_info revs = { 0 };
	bool skip_commit = false;
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب باہرلا;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	إذا (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		اذهب باہرلا;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب باہرلا;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	سر_commit = lookup_commit_reference_by_name("HEAD");
	إذا (!سر_commit) {
		ret = error(_("cannot look up HEAD"));
		اذهب باہرلا;
	}

	سر_درخت = repo_get_commit_درخت(repo, سر_commit);
	إذا (!سر_درخت) {
		ret = error(_("cannot get tree for HEAD"));
		اذهب باہرلا;
	}

	إذا (repo_read_اشاریہ(repo) < 0) {
		ret = error(_("unable to read index"));
		اذهب باہرلا;
	}

	إذا (!repo_اشاریہ_has_changes(repo, سر_درخت, عدم)) {
		ret = error(_("nothing to fixup: no staged changes"));
		اذهب باہرلا;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	اشاریہ_درخت = write_وچ_core_اشاریہ_as_درخت(repo, repo->اشاریہ);
	إذا (!اشاریہ_درخت) {
		ret = error(_("unable to write index as a tree"));
		اذهب باہرلا;
	}

	original_درخت = repo_get_commit_درخت(repo, original);
	إذا (!original_درخت) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		اذهب باہرلا;
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

	إذا (ملاؤ_نتیجہ.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		اذهب باہرلا;
	}

	إذا (!ملاؤ_نتیجہ.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		اذهب باہرلا;
	}

	ret = commit_became_خالی_تھاواں(repo, original, ملاؤ_نتیجہ.درخت);
	إذا (ret < 0)
		اذهب باہرلا;
	إذا (ret > 0) {
		بدل (خالی_تھاواں) {
		حالة REPLAY_خالی_تھاواں_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->شے : عدم;

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
				اذهب باہرلا;
			}

			skip_commit = true;
			اكسر;
		حالة REPLAY_خالی_تھاواں_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			اكسر;
		حالة REPLAY_خالی_تھاواں_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			اذهب باہرلا;
		}
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب باہرلا;

	إذا (!skip_commit) {
		ret = commit_درخت_ext(repo, "fixup", original, عدم, original->parents,
				      &original_درخت->object.oid, &ملاؤ_نتیجہ.درخت->object.oid,
				      &rewritten, flags);
		إذا (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			اذهب باہرلا;
		}
	}

	strbuf_addf(&reflog_پیغام, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_لگاتار, خالی_تھاواں);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	ملاؤ_finalize(&ملاؤ_opts, &ملاؤ_نتیجہ);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح cmd_history_reword(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *اگیتر,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لگاتار = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لگاتار,
			 N_("perform a dry-run without updating any refs")),
		OPT_سرا(),
	};
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب باہرلا;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب باہرلا;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب باہرلا;

	ret = commit_درخت_with_edited_سنیہا(repo, "reworded", original, &rewritten);
	إذا (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		اذهب باہرلا;
	}

	strbuf_addf(&reflog_پیغام, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_لگاتار, REPLAY_خالی_تھاواں_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح write_ondisk_اشاریہ(بنية repository *repo,
			      بنية object_id *oid,
			      ثابت حرف *رستہ)
{
	بنية unpack_trees_options opts = { 0 };
	بنية جندرا_file جندرا = جندرا_INIT;
	بنية درخت_desc درخت_desc;
	بنية اشاریہ_state اشاریہ;
	بنية درخت *درخت;
	صحيح ret;

	اشاریہ_state_init(&اشاریہ, repo);

	opts.سر_idx = -1;
	opts.src_اشاریہ = &اشاریہ;
	opts.dst_اشاریہ = &اشاریہ;

	درخت = repo_parse_درخت_اسدھا(repo, oid);
	إذا (!درخت) {
		ret = error(_("unable to parse tree %s"), oid_منزل_hex(oid));
		اذهب باہرلا;
	}
	init_درخت_desc(&درخت_desc, &درخت->object.oid, درخت->بفر, درخت->ناپ);

	إذا (unpack_trees(1, &درخت_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		اذهب باہرلا;
	}

	مفرد_cache_درخت(repo, &اشاریہ, درخت);

	إذا (repo_hold_جندرا_file_for_update(repo, &جندرا, رستہ, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		اذهب باہرلا;
	}

	إذا (write_locked_اشاریہ(&اشاریہ, &جندرا, COMMIT_جندرا)) {
		ret = error(_("unable to write new index file"));
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	rollback_جندرا_file(&جندرا);
	release_اشاریہ(&اشاریہ);
	أرجع ret;
}

ساكن صحيح ونڈو_commit(بنية repository *repo,
			بنية commit *original,
			بنية pathspec *pathspec,
			بنية commit **باہرلا)
{
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية strbuf اشاریہ_file = STRBUF_INIT;
	بنية اشاریہ_state اشاریہ = اشاریہ_STATE_INIT(repo);
	ثابت بنية object_id *original_commit_درخت_oid;
	ثابت بنية object_id *old_درخت_oid, *new_درخت_oid;
	بنية object_id ماپا_درخت_oid;
	حرف original_commit_oid[GIT_MAX_HEXSZ + 1];
	بنية commit *پہلا_commit, *دوجا_commit;
	بنية commit_list *parents = عدم;
	بنية درخت *ونڈو_درخت;
	صحيح ret;

	إذا (پہلا_ماپا_درخت_oid(repo, original, &ماپا_درخت_oid) < 0) {
		ret = -1;
		اذهب باہرلا;
	}
	original_commit_درخت_oid = get_commit_درخت_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_رستہ_replace(repo, &اشاریہ_file, "%s", "history-split.index");

	ret = write_ondisk_اشاریہ(repo, &ماپا_درخت_oid, اشاریہ_file.buf);
	إذا (ret < 0)
		اذهب باہرلا;

	ret = read_اشاریہ_from(&اشاریہ, اشاریہ_file.buf, repo->gitdir);
	إذا (ret < 0) {
		ret = error(_("failed reading temporary index"));
		اذهب باہرلا;
	}

	oid_منزل_hex_r(original_commit_oid, &original->object.oid);
	ret = لگاتار_جوڑو_p_اشاریہ(repo, &اشاریہ, اشاریہ_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, جوڑو_P_DISALLOW_EDIT);
	إذا (ret < 0)
		اذهب باہرلا;

	ونڈو_درخت = write_وچ_core_اشاریہ_as_درخت(repo, &اشاریہ);
	إذا (!ونڈو_درخت) {
		ret = error(_("failed split tree"));
		اذهب باہرلا;
	}

	unlink(اشاریہ_file.buf);
	strbuf_release(&اشاریہ_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	إذا (oideq(&ونڈو_درخت->object.oid, &ماپا_درخت_oid)) {
		ret = error(_("split commit is empty"));
		اذهب باہرلا;
	} وإلا إذا (oideq(&ونڈو_درخت->object.oid, original_commit_درخت_oid)) {
		ret = error(_("split commit tree matches original commit"));
		اذهب باہرلا;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_درخت_ext(repo, "split-out", original, عدم, original->parents, &ماپا_درخت_oid,
			      &ونڈو_درخت->object.oid, &پہلا_commit, COMMIT_درخت_EDIT_سنیہا);
	إذا (ret < 0) {
		ret = error(_("failed writing first commit"));
		اذهب باہرلا;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(پہلا_commit, &parents);

	old_درخت_oid = &repo_get_commit_درخت(repo, پہلا_commit)->object.oid;
	new_درخت_oid = &repo_get_commit_درخت(repo, original)->object.oid;

	ret = commit_درخت_ext(repo, "split-out", original, عدم, parents, old_درخت_oid,
			      new_درخت_oid, &دوجا_commit, COMMIT_درخت_EDIT_سنیہا);
	إذا (ret < 0) {
		ret = error(_("failed writing second commit"));
		اذهب باہرلا;
	}

	*باہرلا = دوجا_commit;
	ret = 0;

باہرلا:
	إذا (اشاریہ_file.len)
		unlink(اشاریہ_file.buf);
	strbuf_release(&اشاریہ_file);
	commit_list_free(parents);
	release_اشاریہ(&اشاریہ);
	أرجع ret;
}

ساكن صحيح cmd_history_ونڈو(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *اگیتر,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_ونڈو_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لگاتار = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لگاتار,
			 N_("perform a dry-run without updating any refs")),
		OPT_سرا(),
	};
	بنية commit *original, *rewritten = عدم;
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية pathspec pathspec = { 0 };
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, options, usage, 0);
	إذا (argc < 1) {
		ret = error(_("command expects a committish"));
		اذهب باہرلا;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_بھریا |
		       PATHSPEC_SYMLINK_LEADING_رستہ |
		       PATHSPEC_اگیتر_ORIGIN,
		       اگیتر, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب باہرلا;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret < 0)
		اذهب باہرلا;

	إذا (original->parents && original->parents->اگلا) {
		ret = error(_("cannot split up merge commit"));
		اذهب باہرلا;
	}

	ret = ونڈو_commit(repo, original, &pathspec, &rewritten);
	إذا (ret < 0)
		اذهب باہرلا;

	strbuf_addf(&reflog_پیغام, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_پیغام.buf, dry_لگاتار, REPLAY_خالی_تھاواں_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	strbuf_release(&reflog_پیغام);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	أرجع ret;
}

#تعریف_کرو SQUASH_تکیا (1u << 11)
#تعریف_کرو SQUASH_TIP (1u << 12)
#تعریف_کرو SQUASH_AMEND_نشانہ (1u << 13)

ساكن bool is_autosquash_subject(ثابت حرف *s)
{
	أرجع starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ساكن bool skip_one_autosquash_اگیتر(ثابت حرف *s, ثابت حرف **باہرلا)
{
	إذا (skip_اگیتر(s, "amend!", باہرلا) || skip_اگیتر(s, "fixup!", باہرلا) ||
	    skip_اگیتر(s, "squash!", باہرلا)) {
		طالما (**باہرلا == ' ')
			(*باہرلا)++;
		أرجع true;
	}
	أرجع false;
}

ساكن فراغ truncate_سنیہا_منزل_subject(بنية strbuf *پیغام)
{
	ثابت حرف *eos = لڑی_کھوج(پیغام->buf, "\n\n");

	إذا (eos)
		strbuf_setlen(پیغام, eos - پیغام->buf + 1);
}

بنية subject_انکڑے {
	بنية strintmap subjects;
	بنية strbuf subject;
	بنية strbuf squash_سنیہا;
	ثابت حرف *سنیہا;
	bool edit_سنیہا;
};

#تعریف_کرو SUBJECT_انکڑے_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_سنیہا = STRBUF_INIT,	\
}

ساكن فراغ subject_انکڑے_clear(بنية subject_انکڑے *انکڑے)
{
	strintmap_clear(&انکڑے->subjects);
	strbuf_release(&انکڑے->subject);
	strbuf_release(&انکڑے->squash_سنیہا);
}

ساكن صحيح squash_amend_سنیہا(بنية repository *repo,
				بنية commit *commit,
				بنية subject_انکڑے *انکڑے,
				بنانشانی flags)
{
	ثابت حرف *body = انکڑے->سنیہا + انکڑے->subject.len;

	طالما (خلا_اے(*body))
		body++;

	إذا (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_لبھو_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			انکڑے->subject.buf);
		أرجع 0;
	}

	إذا (انکڑے->edit_سنیہا) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_نشانہ) {
		إذا (starts_with(انکڑے->squash_سنیہا.buf, "squash!"))
			أرجع error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_لبھو_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     انکڑے->subject.buf);
		إذا (starts_with(انکڑے->squash_سنیہا.buf, "fixup!"))
			strbuf_splice(&انکڑے->squash_سنیہا, 0, 5, "amend!", 5);
		إذا (starts_with(انکڑے->squash_سنیہا.buf, "amend!")) {
			truncate_سنیہا_منزل_subject(&انکڑے->squash_سنیہا);
			strbuf_addch(&انکڑے->squash_سنیہا, '\n');
		} وإلا {
			strbuf_reset(&انکڑے->squash_سنیہا);
		}
		strbuf_addstr(&انکڑے->squash_سنیہا, body);
		strbuf_complete_سطر(&انکڑے->squash_سنیہا);
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_لبھو_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     انکڑے->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_squash_سنیہا(بنية repository *repo,
				بنية commit *commit,
				بنية subject_انکڑے *انکڑے,
				بنانشانی flags)
{
	ثابت حرف *body = انکڑے->سنیہا + انکڑے->subject.len;

	طالما (خلا_اے(*body))
		body++;

	إذا (انکڑے->edit_سنیہا) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_نشانہ) {
		إذا (starts_with(انکڑے->squash_سنیہا.buf, "fixup!")) {
			truncate_سنیہا_منزل_subject(&انکڑے->squash_سنیہا);
			strbuf_splice(&انکڑے->squash_سنیہا, 0, 5, "squash", 6);
		}
		إذا (starts_with(انکڑے->squash_سنیہا.buf, "squash!")) {
			strbuf_addch(&انکڑے->squash_سنیہا, '\n');
			strbuf_addstr(&انکڑے->squash_سنیہا, body);
			strbuf_complete_سطر(&انکڑے->squash_سنیہا);
		} وإلا {
			أرجع error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_لبھو_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     انکڑے->subject.buf);
		}
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_لبھو_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      انکڑے->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_check_can_autosquash(بنية repository *repo,
				       بنية commit *commit,
				       بنية subject_انکڑے *انکڑے,
				       بنانشانی flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_نشانہ;
	إذا (starts_with(انکڑے->subject.buf, "amend!"))
		أرجع squash_amend_سنیہا(repo, commit, انکڑے, flags);
	وإلا إذا (starts_with(انکڑے->subject.buf, "squash!"))
		أرجع squash_squash_سنیہا(repo, commit, انکڑے, flags);

	أرجع 0;
}

ساكن صحيح squash_check_autosquash_subject(بنية repository *repo,
					   بنية commit *commit,
					   بنية subject_انکڑے *انکڑے)
{
	ثابت حرف* s = انکڑے->subject.buf;
	بنية commit *نشانہ;
	بنية hashmap_iter iter;
	بنية strmap_اندراج *اندراج;
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
	طالما (skip_one_autosquash_اگیتر(s, &s)) {
		بنانشانی flags = strintmap_get(&انکڑے->subjects, s);
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit, انکڑے, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	نشانہ = lookup_commit_reference_by_name(s);
	إذا (نشانہ && starts_with(oid_منزل_hex(&نشانہ->object.oid), s)) {
		بنانشانی flags =
			نشانہ->object.flags & (SQUASH_تکیا | SQUASH_AMEND_نشانہ);
		إذا (!flags) {
			ثابت حرف *subject_شروع;
			ثابت حرف *بفر = repo_logmsg_reencode(repo, نشانہ,
								  عدم, عدم);
			ناپ_t subject_len = لبھو_commit_subject(بفر,
								 &subject_شروع);
			حرف *subject = xmemdupz(subject_شروع, subject_len);

			flags = strintmap_get(&انکڑے->subjects, subject);
			حرر(subject);
			repo_unuse_commit_بفر(repo, نشانہ, بفر);
		}
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit,
							   انکڑے, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_اندراج(&انکڑے->subjects, &iter, اندراج) {
		s = انکڑے->subject.buf;
		طالما(skip_one_autosquash_اگیتر(s, &s)) {
			إذا (starts_with(اندراج->کلید, s)) {
				بنانشانی قدر = (intptr_t)اندراج->قدر;

				أرجع squash_check_can_autosquash(repo, commit,
								   انکڑے, قدر);
			}
		}
	}
	أرجع error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_لبھو_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       انکڑے->subject.buf);
}

ساكن صحيح squash_check_subject(بنية repository *repo,
				بنية commit *commit,
				بنية subject_انکڑے *انکڑے)
{
	صحيح ret = 0;
	ثابت حرف *buf = repo_logmsg_reencode(repo, commit, عدم, عدم);
	ناپ_t subject_len = لبھو_commit_subject(buf, &انکڑے->سنیہا);

	strbuf_reset(&انکڑے->subject);
	strbuf_جوڑو(&انکڑے->subject, انکڑے->سنیہا, subject_len);

	إذا (!strintmap_get_ناپ(&انکڑے->subjects)) {
		ثابت حرف *s;

		strbuf_addstr(&انکڑے->squash_سنیہا, انکڑے->سنیہا);
		strbuf_complete_سطر(&انکڑے->squash_سنیہا);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = انکڑے->subject.buf;
		skip_one_autosquash_اگیتر(s, &s);
		strintmap_سیٹ(&انکڑے->subjects, s, SQUASH_AMEND_نشانہ | SQUASH_تکیا);
		commit->object.flags |= SQUASH_AMEND_نشانہ;
	} وإلا إذا (is_autosquash_subject(انکڑے->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, انکڑے);
	} وإلا {
		strintmap_سیٹ(&انکڑے->subjects, انکڑے->subject.buf, SQUASH_تکیا);
	}
	repo_unuse_commit_بفر(repo, commit, buf);
	أرجع ret;
}

ساكن صحيح build_squash_سنیہا(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *باہرلا);

ساكن صحيح تیاری_squash_revisions(بنية repository *repo,
				  صحيح argc, ثابت حرف **argv,
				  بنية rev_info *revs)
{
	repo_init_revisions(repo, revs, عدم);
	revs->الٹ = 1;
	revs->topo_ترتیب = 1;
	revs->ترتیب_دیو_ترتیب = REV_ترتیب_دیو_وچ_گراف_ترتیب;
	revs->simplify_history = 0;
	revs->ancestry_رستہ = 1;
	revs->limited = 1;
	revs->ancestry_رستہ_implicit_bottoms = 1;

	argc = تیاری_revisions(argc, argv, revs, عدم);
	إذا (argc > 1)
		أرجع error(_("unrecognized argument: %s"), argv[1]);

	إذا (revs->الٹ != 1 || revs->topo_ترتیب != 1 ||
	    revs->ترتیب_دیو_ترتیب != REV_ترتیب_دیو_وچ_گراف_ترتیب ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_رستہ != 1 || revs->limited != 1 ||
	    revs->ancestry_رستہ_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->الٹ = 1;
		revs->topo_ترتیب = 1;
		revs->ترتیب_دیو_ترتیب = REV_ترتیب_دیو_وچ_گراف_ترتیب;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_رستہ = 1;
		revs->limited = 1;
		revs->ancestry_رستہ_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	لكل (ناپ_t i = 0; i < revs->cmdline.nr; i++)
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
				bool edit_سنیہا,
				صحيح argc, ثابت حرف **argv,
				بنية commit **oldest_باہرلا,
				بنية commit **tip_باہرلا,
				حرف **سنیہا_باہرلا)
{
	بنية rev_info revs;
	بنية subject_انکڑے subject_انکڑے = SUBJECT_انکڑے_INIT;
	بنية commit *commit, *oldest = عدم, *tip = عدم;
	بنية strbuf todo_buf = STRBUF_INIT;
	صحيح ret, tip_گنتی = 0;
	bool walk_started = false;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_array refs = { 0 };

	subject_انکڑے.edit_سنیہا = edit_سنیہا;
	ret = تیاری_squash_revisions(repo, argc, argv, &revs);
	إذا (ret < 0)
		اذهب باہرلا;

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب باہرلا;
	}
	walk_started = true;
	طالما ((commit = get_revision(&revs))) {
		بنية commit_list *p;

		إذا (edit_سنیہا)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_منزل_hex(&commit->object.oid));

		إذا (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			اذهب باہرلا;
		}
		لكل (p = commit->parents; oldest && p; p = p->اگلا) {
			بنية commit_list *q;
			بنية object *o;
			bool تکیا;

			إذا (repo_parse_commit(repo, p->شے)) {
				ret = error(_("cannot parse commit"));
				اذهب باہرلا;
			}
			o = &p->شے->object;
			تکیا = o->flags & SQUASH_تکیا;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			لكل (q = oldest->parents; !تکیا && q; q = q->اگلا) {
				إذا (p->شے == q->شے) {
					تکیا = true;
					commit_list_داخل_کرو(commit, &filter.with_commit);
				}
			}
			إذا (!تکیا) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_لبھو_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_لبھو_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				اذهب باہرلا;
			}
			إذا (o->flags & SQUASH_TIP) {
				tip_گنتی--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		إذا (!oldest) {
			commit_list_داخل_کرو(commit, &filter.with_commit);
			oldest = commit;
		}
		إذا (squash_check_subject(repo, commit, &subject_انکڑے)) {
			ret = -1;
			اذهب باہرلا;
		}
		tip = commit;
		tip->object.flags |= SQUASH_تکیا | SQUASH_TIP;
		tip_گنتی++;
	}
	clear_object_flags(repo, SQUASH_تکیا | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	إذا (!tip_گنتی) {
		ret = error(_("the revision range is empty"));
		اذهب باہرلا;
	} وإلا إذا (tip_گنتی != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		اذهب باہرلا;
	} وإلا إذا (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		اذهب باہرلا;
	} وإلا إذا (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_داخل_کرو(tip, &filter.no_commit);
	filter.قسم = FILTER_REFS_BRANCHES;
	إذا (update_branches &&
	    filter_refs(&refs, &filter, filter.قسم)) {
		ret = error(_("could not filter refs"));
		اذهب باہرلا;
	}
	إذا (refs.nr) {
		بنية ref_format format = REF_FORMAT_INIT;
		بنية ref_چھانٹی *چھانٹی;
		بنية string_list چھانٹی_options = STRING_LIST_INIT_DUP;
		بنية strbuf branches = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		إذا (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&چھانٹی_options, "refname");
		چھانٹی = ref_چھانٹی_options(&چھانٹی_options);
		ref_array_ترتیب_دیو(چھانٹی, &refs);
		لكل (صحيح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			إذا (format_ref_array_شے(refs.اشیاء[i], &format,
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
		ref_چھانٹی_release(چھانٹی);
		string_list_clear(&چھانٹی_options, 0);
		اذهب باہرلا;
	}
	إذا (edit_سنیہا) {
		strbuf_reset(&subject_انکڑے.squash_سنیہا);
		ret = build_squash_سنیہا(repo, &todo_buf,
					   &subject_انکڑے.squash_سنیہا);
		إذا (ret < 0)
			اذهب باہرلا;
	}

	*oldest_باہرلا = oldest;
	*tip_باہرلا = tip;
	*سنیہا_باہرلا = strbuf_detach(&subject_انکڑے.squash_سنیہا, عدم);
	ret = 0;

باہرلا:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_تکیا | SQUASH_TIP |
			   SQUASH_AMEND_نشانہ);
	إذا (walk_started)
		reset_revision_walk();
	subject_انکڑے_clear(&subject_انکڑے);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	أرجع ret;
}

ساكن bool amend_replaces_نشانہ(بنية todo_list *todo, صحيح نشانہ)
{
	لكل (صحيح i = نشانہ + 1; i < todo->nr &&
				 todo->اشیاء[i].command != TODO_PICK; i++) {
		إذا (todo->اشیاء[i].command == TODO_SQUASH)
			أرجع false;
		إذا (todo->اشیاء[i].flags & TODO_REPLACE_FIXUP_پیغام)
			أرجع true;
	}
	أرجع false;
}

ساكن صحيح build_squash_سنیہا(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *باہرلا)
{
	بنية todo_list todo = TODO_LIST_INIT;
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح nr_commits, ret;

	إذا (todo_list_parse_insn_بفر(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		اذهب باہرلا;
	}

	nr_commits = todo.nr;
	لكل (صحيح i = 0; i < nr_commits; i++) {
		بنية todo_شے *شے = &todo.اشیاء[i];
		ثابت حرف *سنیہا, *body;
		ناپ_t commented_len;
		bool skip, squashing;

		squashing = شے->command == TODO_SQUASH ||
			    (شے->flags & TODO_REPLACE_FIXUP_پیغام);
		إذا (شے->command == TODO_PICK)
			skip = amend_replaces_نشانہ(&todo, i);
		وإلا
			skip = !squashing;

		سنیہا = repo_logmsg_reencode(repo, شے->commit, عدم, عدم);
		لبھو_commit_subject(سنیہا, &body);

		إذا (skip)
			commented_len = لڑی_لمبائی(body);
		وإلا إذا (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		وإلا
			commented_len = 0;

		إذا (!i)
			جوڑو_squash_combination_سرخی(باہرلا, nr_commits);
		strbuf_addch(باہرلا, '\n');
		جوڑو_squash_سنیہا_سرخی(باہرلا, i + 1, skip);
		strbuf_addstr(باہرلا, "\n\n");
		strbuf_جوڑو_commented_سطریں(باہرلا, body, commented_len, comment_سطر_str);
		strbuf_addstr(باہرلا, body + commented_len);
		strbuf_complete_سطر(باہرلا);

		repo_unuse_commit_بفر(repo, شے->commit, سنیہا);
	}

	ret = 0;

باہرلا:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	أرجع ret;
}

ساكن صحيح cmd_history_squash(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *اگیتر,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لگاتار = 0;
	صحيح edit = 1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لگاتار,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_سرا(),
	};
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *oldest, *tip, *rewritten;
	ثابت بنية object_id *ادھار_درخت_oid, *tip_درخت_oid;
	حرف *سنیہا_template = عدم;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	إذا (argc < 2) {
		ret = error(_("command expects a revision range"));
		اذهب باہرلا;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_پیغام, "squash: updating ");
	strbuf_join_argv(&reflog_پیغام, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &سنیہا_template);
	إذا (ret < 0)
		اذهب باہرلا;

	ret = تیاری_revwalk(repo, action, tip, &revs);
	إذا (ret < 0)
		اذهب باہرلا;

	ادھار_درخت_oid = &repo_get_commit_درخت(repo,
					oldest->parents->شے)->object.oid;
	tip_درخت_oid = &repo_get_commit_درخت(repo, tip)->object.oid;

	ret = commit_درخت_ext(repo, "squash", oldest, سنیہا_template,
			      oldest->parents, ادھار_درخت_oid, tip_درخت_oid,
			      &rewritten,
			      edit ? COMMIT_درخت_EDIT_سنیہا : 0);
	إذا (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		اذهب باہرلا;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_پیغام.buf, dry_لگاتار,
				       REPLAY_خالی_تھاواں_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	حرر(سنیہا_template);
	أرجع ret;
}

ساكن صحيح update_worktree(بنية repository *repo,
			   ثابت بنية commit *old_سر,
			   ثابت بنية commit *new_سر,
			   bool dry_لگاتار)
{
	بنية reset_working_درخت_options opts = {
		.oid_from = &old_سر->object.oid,
		.oid = &new_سر->object.oid,
	};
	إذا (dry_لگاتار)
		opts.flags |= RESET_WORKING_درخت_DRY_لگاتار;
	أرجع reset_working_درخت(repo, &opts);
}

ساكن صحيح لبھو_سر_درخت_change(بنية repository *repo,
				 ثابت بنية replay_نتیجہ *نتیجہ,
				 بنية commit **old_سر,
				 بنية commit **new_سر,
				 bool *بدلیا)
{
	ثابت بنية replay_ref_update *سر_update = عدم;
	بنية commit *old_سر_commit, *new_سر_commit;
	بنية درخت *old_سر_درخت, *new_سر_درخت;
	ثابت حرف *سر_نشانہ;
	صحيح سر_flags;

	*بدلیا = false;

	سر_نشانہ = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      عدم, &سر_flags);
	إذا (!سر_نشانہ)
		أرجع error(_("cannot look up HEAD"));

	لكل (ناپ_t i = 0; i < نتیجہ->updates_nr; i++) {
		إذا (!strcmp(نتیجہ->updates[i].refname, سر_نشانہ)) {
			سر_update = &نتیجہ->updates[i];
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
	*بدلیا = true;

	أرجع 0;
}

ساكن صحيح cmd_history_drop(صحيح argc,
			    ثابت حرف **argv,
			    ثابت حرف *اگیتر,
			    بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		عدم,
	};
	تعداد replay_خالی_تھاواں_commit_action خالی_تھاواں = REPLAY_خالی_تھاواں_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لگاتار = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لگاتار,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &خالی_تھاواں, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالی_تھاواں),
		OPT_سرا(),
	};
	بنية strbuf reflog_پیغام = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	بنية replay_نتیجہ نتیجہ = { 0 };
	بنية commit *old_سر, *new_سر;
	bool سر_چالاں = false;
	صحيح ret;

	argc = parse_options(argc, argv, اگیتر, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب باہرلا;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب باہرلا;
	}

	إذا (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		اذهب باہرلا;
	} وإلا إذا (original->parents->اگلا) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		اذهب باہرلا;
	}

	ret = تیاری_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب باہرلا;

	rewritten = original->parents->شے;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  خالی_تھاواں, &نتیجہ);
	إذا (ret) {
		ret = error(_("failed replaying descendants"));
		اذهب باہرلا;
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
		ret = لبھو_سر_درخت_change(repo, &نتیجہ, &old_سر,
					    &new_سر, &سر_چالاں);
		إذا (ret < 0)
			اذهب باہرلا;

		إذا (سر_چالاں && update_worktree(repo, old_سر, new_سر, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			اذهب باہرلا;
		}
	}

	strbuf_addf(&reflog_پیغام, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &نتیجہ, reflog_پیغام.buf, dry_لگاتار);
	إذا (ret < 0) {
		ret = error(_("failed to update references"));
		اذهب باہرلا;
	}

	إذا (!dry_لگاتار && سر_چالاں && update_worktree(repo, old_سر, new_سر, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_منزل_hex(&new_سر->object.oid));
		اذهب باہرلا;
	}

	ret = 0;

باہرلا:
	replay_نتیجہ_release(&نتیجہ);
	strbuf_release(&reflog_پیغام);
	release_revisions(&revs);
	أرجع ret;
}

صحيح cmd_history(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *اگیتر,
		بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_ونڈو_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_ونڈو),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_سرا(),
	};

	argc = parse_options(argc, argv, اگیتر, options, usage, 0);
	أرجع fn(argc, argv, اگیتر, repo);
}
