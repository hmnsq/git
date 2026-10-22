#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

#وصف_ڪريو GIT_HISTORY_DROP_USAGE \
	N_("git history drop <commit> [--dry-run] [--update-refs=(branches|head)] [--empty=(drop|keep|abort)]")
#وصف_ڪريو GIT_HISTORY_FIXUP_USAGE \
	N_("git history fixup <commit> [--dry-run] [--update-refs=(branches|head)] [--reedit-message] [--empty=(drop|keep|abort)]")
#وصف_ڪريو GIT_HISTORY_REWORD_USAGE \
	N_("git history reword <commit> [--dry-run] [--update-refs=(branches|head)]")
#وصف_ڪريو GIT_HISTORY_ورهايو_USAGE \
	N_("git history split <commit> [--dry-run] [--update-refs=(branches|head)] [--] [<pathspec>...]")
#وصف_ڪريو GIT_HISTORY_SQUASH_USAGE \
	N_("git history squash [--dry-run] [--update-refs=(branches|head)] [--[no-]edit] <revision-range>")

ساكن فراغ change_معلومات_free(فراغ *util, ثابت حرف *str اڻ_استعمال_ٿيل)
{
	بنية wt_حالت_change_معلومات *d = util;
	حرر(d->rename_ذريعو);
	حرر(d);
}

ساكن صحيح fill_commit_پيغام(بنية repository *repo,
			       ثابت بنية object_id *old_وڻ,
			       ثابت بنية object_id *new_وڻ,
			       ثابت حرف *default_پيغام,
			       ثابت حرف *action,
			       بنية strbuf *ٻاهريون)
{
	ثابت حرف *رستو = git_رستو_commit_editmsg();
	ثابت حرف *hint =
		_("Please enter the commit message for the %s changes."
		  " Lines starting\nwith '%s' will be ignored, and an"
		  " empty message aborts the commit.\n");
	بنية wt_حالت s;

	wt_حالت_prepare(repo, &s);
	FREE_AND_NULL(s.branch);
	s.ahead_behind_flags = AHEAD_BEHIND_تيز;
	s.commit_template = 1;
	s.colopts = 0;
	s.display_comment_اڳياڙي = 1;
	s.hints = 0;
	s.use_color = 0;
	s.whence = FROM_COMMIT;
	s.committable = 1;

	s.fp = fopen(رستو, "w");
	إذا (!s.fp)
		أرجع error_errno(_("could not open '%s'"), رستو);

	strbuf_addstr(ٻاهريون, default_پيغام);
	strbuf_addch(ٻاهريون, '\n');
	strbuf_commented_addf(ٻاهريون, comment_سٽ_str, hint, action, comment_سٽ_str);
	إذا (fwrite(ٻاهريون->buf, 1, ٻاهريون->len, s.fp) != ٻاهريون->len)
		die_errno(_("could not write to '%s'"), رستو);

	wt_حالت_collect_changes_trees(&s, old_وڻ, new_وڻ);
	wt_حالت_print(&s);
	wt_حالت_collect_free_buffers(&s);
	string_list_clear_func(&s.change, change_معلومات_free);
	إذا (fclose(s.fp))
		die_errno(_("could not write to '%s'"), رستو);

	strbuf_reset(ٻاهريون);
	إذا (launch_editor(رستو, ٻاهريون, عدم)) {
		fprintf(stderr, _("Aborting commit as launching the editor failed.\n"));
		أرجع -1;
	}
	strbuf_stripspace(ٻاهريون, comment_سٽ_str);

	cleanup_پيغام(ٻاهريون, COMMIT_نياپو_CLEANUP_ALL, 0);

	إذا (!ٻاهريون->len) {
		fprintf(stderr, _("Aborting commit due to empty commit message.\n"));
		أرجع -1;
	}

	أرجع 0;
}

تعداد commit_وڻ_flags {
	COMMIT_وڻ_EDIT_پيغام = (1 << 0),
};

ساكن صحيح commit_وڻ_ext(بنية repository *repo,
			   ثابت حرف *action,
			   بنية commit *commit_with_پيغام,
			   ثابت حرف *پيغام_template,
			   ثابت بنية commit_list *parents,
			   ثابت بنية object_id *old_وڻ,
			   ثابت بنية object_id *new_وڻ,
			   بنية commit **ٻاهريون,
			   تعداد commit_وڻ_flags flags)
{
	ثابت حرف *exclude_gpgsig[] = {
		/* We reencode the message, so the encoding needs to be stripped. */
		"encoding",
		/* We need to strip signatures as those will become invalid. */
		"gpgsig",
		"gpgsig-sha256",
		عدم,
	};
	ثابت حرف *original_پيغام, *original_body, *ptr;
	بنية commit_extra_سرو *original_extra_سرا = عدم;
	بنية strbuf commit_پيغام = STRBUF_INIT;
	بنية object_id rewritten_commit_oid;
	حرف *original_author = عدم;
	سائيز_t len;
	صحيح ret;

	/* We retain authorship of the original commit. */
	original_پيغام = repo_logmsg_reencode(repo, commit_with_پيغام, عدم, عدم);
	ptr = ڳوليو_commit_سرو(original_پيغام, "author", &len);
	إذا (ptr)
		original_author = xmemdupz(ptr, len);
	ڳوليو_commit_subject(original_پيغام, &original_body);

	إذا (!پيغام_template)
		پيغام_template = original_body;

	إذا (flags & COMMIT_وڻ_EDIT_پيغام) {
		ret = fill_commit_پيغام(repo, old_وڻ, new_وڻ,
					  پيغام_template, action, &commit_پيغام);
		إذا (ret < 0)
			اذهب ٻاهريون;
	} وإلا {
		strbuf_addstr(&commit_پيغام, پيغام_template);
	}

	original_extra_سرا = read_commit_extra_سرا(commit_with_پيغام,
							   exclude_gpgsig);

	ret = commit_وڻ_extended(commit_پيغام.buf, commit_پيغام.len, new_وڻ,
				   parents, &rewritten_commit_oid, original_author,
				   عدم, عدم, original_extra_سرا);
	إذا (ret < 0)
		اذهب ٻاهريون;

	*ٻاهريون = lookup_commit_or_die(&rewritten_commit_oid, "rewritten commit");

ٻاهريون:
	free_commit_extra_سرا(original_extra_سرا);
	strbuf_release(&commit_پيغام);
	حرر(original_author);
	repo_unuse_commit_بفر(repo, commit_with_پيغام, original_پيغام);
	أرجع ret;
}

ساكن صحيح پهريون_والد_وڻ_oid(بنية repository *repo,
				 بنية commit *commit,
				 بنية object_id *ٻاهريون)
{
	بنية commit *والد = commit->parents ? commit->parents->شي : عدم;

	إذا (!والد) {
		oidcpy(ٻاهريون, repo->hash_algo->خالي_جايون_وڻ);
		أرجع 0;
	}

	إذا (repo_parse_commit(repo, والد))
		أرجع error(_("unable to parse parent commit %s"),
			     oid_منزل_hex(&والد->object.oid));

	oidcpy(ٻاهريون, &repo_get_commit_وڻ(repo, والد)->object.oid);
	أرجع 0;
}

ساكن صحيح commit_وڻ_with_edited_پيغام(بنية repository *repo,
					   ثابت حرف *action,
					   بنية commit *original,
					   بنية commit **ٻاهريون)
{
	بنية object_id والد_وڻ_oid;
	ثابت بنية object_id *وڻ_oid;

	وڻ_oid = &repo_get_commit_وڻ(repo, original)->object.oid;

	إذا (پهريون_والد_وڻ_oid(repo, original, &والد_وڻ_oid) < 0)
		أرجع -1;

	أرجع commit_وڻ_ext(repo, action, original, عدم, original->parents,
			       &والد_وڻ_oid, وڻ_oid, ٻاهريون, COMMIT_وڻ_EDIT_پيغام);
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

ساكن صحيح revwalk_شامل_آهي_merges(بنية repository *repo,
				   ثابت بنية strvec *revwalk_دليلون)
{
	بنية strvec دليلون = STRVEC_INIT;
	بنية rev_info revs;
	صحيح ret;

	strvec_pushv(&دليلون, revwalk_دليلون->v);
	strvec_ڌڪو(&دليلون, "--min-parents=2");

	repo_init_revisions(repo, &revs, عدم);

	تياري_revisions_from_strvec(&دليلون, &revs, عدم);
	إذا (دليلون.nr != 1)
		BUG("revisions were set up with invalid argument");

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب ٻاهريون;
	}

	إذا (get_revision(&revs)) {
		ret = error(_("replaying merge commits is not supported yet!"));
		اذهب ٻاهريون;
	}

	reset_revision_walk();
	ret = 0;

ٻاهريون:
	release_revisions(&revs);
	strvec_clear(&دليلون);
	أرجع ret;
}

ساكن صحيح تياري_revwalk(بنية repository *repo,
			 تعداد ref_action action,
			 بنية commit *original,
			 بنية rev_info *revs)
{
	بنية strvec دليلون = STRVEC_INIT;
	صحيح ret;

	repo_init_revisions(repo, revs, عدم);
	strvec_ڌڪو(&دليلون, "ignored");
	strvec_ڌڪو(&دليلون, "--reverse");
	strvec_ڌڪو(&دليلون, "--topo-order");
	strvec_ڌڪو(&دليلون, "--full-history");

	/* We only want to see commits that are descendants of the old commit. */
	strvec_pushf(&دليلون, "--ancestry-path=%s",
		     oid_منزل_hex(&original->object.oid));

	/*
	 * Ancestry path may also show ancestors of the old commit, but we
	 * don't want to see those, either.
	 */
	strvec_pushf(&دليلون, "^%s", oid_منزل_hex(&original->object.oid));

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
			اذهب ٻاهريون;
		}

		commit_list_وجهو(original, &from_list);
		ret = repo_is_descendant_جو(repo, سر, from_list);
		commit_list_free(from_list);

		إذا (ret < 0) {
			ret = error(_("cannot determine descendance"));
			اذهب ٻاهريون;
		} وإلا إذا (!ret) {
			ret = error(_("rewritten commit must be an ancestor "
				      "of HEAD when using --update-refs=head"));
			اذهب ٻاهريون;
		}

		strvec_ڌڪو(&دليلون, "HEAD");
	} وإلا {
		strvec_ڌڪو(&دليلون, "--branches");
		strvec_ڌڪو(&دليلون, "HEAD");
	}

	ret = revwalk_شامل_آهي_merges(repo, &دليلون);
	إذا (ret < 0)
		اذهب ٻاهريون;

	تياري_revisions_from_strvec(&دليلون, revs, عدم);
	إذا (دليلون.nr != 1)
		BUG("revisions were set up with invalid argument");

	ret = 0;

ٻاهريون:
	strvec_clear(&دليلون);
	أرجع ret;
}

ساكن صحيح handle_ref_update(بنية ref_transaction *transaction,
			     ثابت حرف *refname,
			     ثابت بنية object_id *new_oid,
			     ثابت بنية object_id *old_oid,
			     ثابت حرف *reflog_نياپو,
			     بنية strbuf *err)
{
	إذا (!transaction) {
		لکو("update %s %s %s\n",
		       refname, oid_منزل_hex(new_oid), oid_منزل_hex(old_oid));
		أرجع 0;
	}

	أرجع ref_transaction_update(transaction, refname, new_oid, old_oid,
				      عدم, عدم, 0, reflog_نياپو, err);
}

ساكن صحيح compute_pending_ref_updates(بنية rev_info *revs,
				       تعداد ref_action action,
				       بنية commit *original,
				       بنية commit *rewritten,
				       تعداد replay_خالي_جايون_commit_action خالي_جايون,
				       بنية replay_نتيجو *نتيجو)
{
	ثابت بنية name_decoration *decoration;
	بنية replay_revisions_options opts = {
		.خالي_جايون = خالي_جايون,
	};
	حرف hex[GIT_MAX_HEXSZ + 1];
	bool detached_سر;
	صحيح سر_flags = 0;
	صحيح ret;

	refs_read_ref_ڀريل(get_main_ref_store(revs->repo), "HEAD",
			   RESOLVE_REF_NO_RECURSE, عدم, &سر_flags);
	detached_سر = !(سر_flags & REF_ISSYMREF);

	opts.onto = oid_منزل_hex_r(hex, &rewritten->object.oid);

	ret = replay_revisions(revs, &opts, نتيجو);
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
	     decoration = decoration->ايندڙ)
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

		replay_نتيجو_قطار_update(نتيجو, decoration->name,
					   &original->object.oid,
					   &rewritten->object.oid);
	}

	أرجع 0;
}

ساكن صحيح apply_pending_ref_updates(بنية repository *repo,
				     ثابت بنية replay_نتيجو *نتيجو,
				     ثابت حرف *reflog_نياپو,
				     صحيح dry_لاڳيتو)
{
	بنية ref_transaction *transaction = عدم;
	بنية strbuf err = STRBUF_INIT;
	صحيح ret;

	إذا (!dry_لاڳيتو) {
		transaction = ref_store_transaction_begin(get_main_ref_store(repo),
							  0, &err);
		إذا (!transaction) {
			ret = error(_("failed to begin ref transaction: %s"), err.buf);
			اذهب ٻاهريون;
		}
	}

	لكل (سائيز_t i = 0; i < نتيجو->updates_nr; i++) {
		ret = handle_ref_update(transaction,
					نتيجو->updates[i].refname,
					&نتيجو->updates[i].new_oid,
					&نتيجو->updates[i].old_oid,
					reflog_نياپو, &err);
		إذا (ret) {
			ret = error(_("failed to update ref '%s': %s"),
				    نتيجو->updates[i].refname, err.buf);
			اذهب ٻاهريون;
		}
	}

	إذا (transaction && ref_transaction_commit(transaction, &err)) {
		ret = error(_("failed to commit ref transaction: %s"), err.buf);
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	ref_transaction_free(transaction);
	strbuf_release(&err);
	أرجع ret;
}

ساكن صحيح handle_reference_updates(بنية rev_info *revs,
				    تعداد ref_action action,
				    بنية commit *original,
				    بنية commit *rewritten,
				    ثابت حرف *reflog_نياپو,
				    صحيح dry_لاڳيتو,
				    تعداد replay_خالي_جايون_commit_action خالي_جايون)
{
	بنية replay_نتيجو نتيجو = { 0 };
	صحيح ret;

	ret = compute_pending_ref_updates(revs, action, original, rewritten,
					  خالي_جايون, &نتيجو);
	إذا (ret)
		اذهب ٻاهريون;

	ret = apply_pending_ref_updates(revs->repo, &نتيجو, reflog_نياپو, dry_لاڳيتو);

ٻاهريون:
	replay_نتيجو_release(&نتيجو);
	أرجع ret;
}

ساكن صحيح commit_became_خالي_جايون(بنية repository *repo,
			       بنية commit *original,
			       بنية وڻ *نتيجو)
{
	بنية object_id والد_وڻ_oid;

	إذا (پهريون_والد_وڻ_oid(repo, original, &والد_وڻ_oid) < 0)
		أرجع -1;

	أرجع oideq(&نتيجو->object.oid, &والد_وڻ_oid);
}

ساكن صحيح parse_opt_خالي_جايون(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	تعداد replay_خالي_جايون_commit_action *قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "drop"))
		*قدر = REPLAY_خالي_جايون_COMMIT_DROP;
	وإلا إذا (!strcmp(arg, "keep"))
		*قدر = REPLAY_خالي_جايون_COMMIT_KEEP;
	وإلا إذا (!strcmp(arg, "abort"))
		*قدر = REPLAY_خالي_جايون_COMMIT_ABORT;
	وإلا
		die(_("unrecognized '--empty=' action '%s'; "
		      "valid values are \"drop\", \"keep\", and \"abort\"."), arg);

	أرجع 0;
}

ساكن صحيح cmd_history_fixup(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *اڳياڙي,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_FIXUP_USAGE,
		عدم,
	};
	تعداد replay_خالي_جايون_commit_action خالي_جايون = REPLAY_خالي_جايون_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	تعداد commit_وڻ_flags flags = 0;
	صحيح dry_لاڳيتو = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لاڳيتو,
			 N_("perform a dry-run without updating any refs")),
		OPT_ٻٽو_انگ(0, "reedit-message", &flags,
			N_("open an editor to modify the commit message"),
			COMMIT_وڻ_EDIT_پيغام),
		OPT_CALLBACK_F(0, "empty", &خالي_جايون, "(drop|keep|abort)",
			       N_("how to handle commits that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالي_جايون),
		OPT_پڇاڙي(),
	};
	بنية ملايو_نتيجو ملايو_نتيجو = { 0 };
	بنية ملايو_options ملايو_opts = { 0 };
	بنية strbuf reflog_نياپو = STRBUF_INIT;
	بنية commit *سر_commit, *original, *rewritten;
	بنية وڻ *سر_وڻ, *original_وڻ, *اشاريو_وڻ;
	بنية rev_info revs = { 0 };
	bool skip_commit = false;
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب ٻاهريون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	إذا (is_bare_repository(repo)) {
		ret = error(_("cannot run fixup in a bare repository"));
		اذهب ٻاهريون;
	}

	/* Resolve the original commit, which is the one we want to fix up. */
	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب ٻاهريون;
	}

	/*
	 * Resolve HEAD so we can use its tree as the merge base: the staged
	 * changes are expressed as a diff from HEAD's tree to the index tree.
	 */
	سر_commit = lookup_commit_reference_by_name("HEAD");
	إذا (!سر_commit) {
		ret = error(_("cannot look up HEAD"));
		اذهب ٻاهريون;
	}

	سر_وڻ = repo_get_commit_وڻ(repo, سر_commit);
	إذا (!سر_وڻ) {
		ret = error(_("cannot get tree for HEAD"));
		اذهب ٻاهريون;
	}

	إذا (repo_read_اشاريو(repo) < 0) {
		ret = error(_("unable to read index"));
		اذهب ٻاهريون;
	}

	إذا (!repo_اشاريو_has_changes(repo, سر_وڻ, عدم)) {
		ret = error(_("nothing to fixup: no staged changes"));
		اذهب ٻاهريون;
	}

	/*
	 * Write the index as a tree object. This is the "theirs" side of the
	 * three-way merge: it is HEAD's tree with the staged changes applied.
	 */
	اشاريو_وڻ = write_اندريون_core_اشاريو_as_وڻ(repo, repo->اشاريو);
	إذا (!اشاريو_وڻ) {
		ret = error(_("unable to write index as a tree"));
		اذهب ٻاهريون;
	}

	original_وڻ = repo_get_commit_وڻ(repo, original);
	إذا (!original_وڻ) {
		ret = error(_("cannot get tree for commit %s"), argv[0]);
		اذهب ٻاهريون;
	}

	/*
	 * Perform the three-way merge to reapply changes in the index onto the
	 * target commit. This is using basically the same logic as a
	 * cherry-pick, where the base commit is our HEAD, ours is the original
	 * tree and theirs is the index tree.
	 */
	init_basic_ملايو_options(&ملايو_opts, repo);
	ملايو_opts.ancestor = "HEAD";
	ملايو_opts.branch1 = argv[0];
	ملايو_opts.branch2 = "staged";
	ملايو_incore_nonrecursive(&ملايو_opts, سر_وڻ,
				  original_وڻ, اشاريو_وڻ, &ملايو_نتيجو);

	إذا (ملايو_نتيجو.clean < 0) {
		ret = error(_("merge failed while applying fixup"));
		اذهب ٻاهريون;
	}

	إذا (!ملايو_نتيجو.clean) {
		ret = error(_("fixup would produce conflicts; aborting"));
		اذهب ٻاهريون;
	}

	ret = commit_became_خالي_جايون(repo, original, ملايو_نتيجو.وڻ);
	إذا (ret < 0)
		اذهب ٻاهريون;
	إذا (ret > 0) {
		بدل (خالي_جايون) {
		حالة REPLAY_خالي_جايون_COMMIT_DROP:
			/*
			 * Drop the target commit by replaying its descendants
			 * directly onto its parent.
			 */
			rewritten = original->parents ? original->parents->شي : عدم;

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
				اذهب ٻاهريون;
			}

			skip_commit = true;
			اكسر;
		حالة REPLAY_خالي_جايون_COMMIT_KEEP:
			/* Proceed and record the empty commit. */
			اكسر;
		حالة REPLAY_خالي_جايون_COMMIT_ABORT:
			ret = error(_("fixup makes commit %s empty"), argv[0]);
			اذهب ٻاهريون;
		}
	}

	ret = تياري_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب ٻاهريون;

	إذا (!skip_commit) {
		ret = commit_وڻ_ext(repo, "fixup", original, عدم, original->parents,
				      &original_وڻ->object.oid, &ملايو_نتيجو.وڻ->object.oid,
				      &rewritten, flags);
		إذا (ret < 0) {
			ret = error(_("failed writing fixed-up commit"));
			اذهب ٻاهريون;
		}
	}

	strbuf_addf(&reflog_نياپو, "fixup: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_نياپو.buf, dry_لاڳيتو, خالي_جايون);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	ملايو_finalize(&ملايو_opts, &ملايو_نتيجو);
	strbuf_release(&reflog_نياپو);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح cmd_history_reword(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *اڳياڙي,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_REWORD_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لاڳيتو = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لاڳيتو,
			 N_("perform a dry-run without updating any refs")),
		OPT_پڇاڙي(),
	};
	بنية strbuf reflog_نياپو = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب ٻاهريون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب ٻاهريون;
	}

	ret = تياري_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب ٻاهريون;

	ret = commit_وڻ_with_edited_پيغام(repo, "reworded", original, &rewritten);
	إذا (ret < 0) {
		ret = error(_("failed writing reworded commit"));
		اذهب ٻاهريون;
	}

	strbuf_addf(&reflog_نياپو, "reword: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_نياپو.buf, dry_لاڳيتو, REPLAY_خالي_جايون_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	strbuf_release(&reflog_نياپو);
	release_revisions(&revs);
	أرجع ret;
}

ساكن صحيح write_ondisk_اشاريو(بنية repository *repo,
			      بنية object_id *oid,
			      ثابت حرف *رستو)
{
	بنية unpack_trees_options opts = { 0 };
	بنية تالو_file تالو = تالو_INIT;
	بنية وڻ_desc وڻ_desc;
	بنية اشاريو_state اشاريو;
	بنية وڻ *وڻ;
	صحيح ret;

	اشاريو_state_init(&اشاريو, repo);

	opts.سر_idx = -1;
	opts.src_اشاريو = &اشاريو;
	opts.dst_اشاريو = &اشاريو;

	وڻ = repo_parse_وڻ_اڻسڌو(repo, oid);
	إذا (!وڻ) {
		ret = error(_("unable to parse tree %s"), oid_منزل_hex(oid));
		اذهب ٻاهريون;
	}
	init_وڻ_desc(&وڻ_desc, &وڻ->object.oid, وڻ->بفر, وڻ->سائيز);

	إذا (unpack_trees(1, &وڻ_desc, &opts)) {
		ret = error(_("unable to populate index with tree"));
		اذهب ٻاهريون;
	}

	مفرد_cache_وڻ(repo, &اشاريو, وڻ);

	إذا (repo_hold_تالو_file_for_update(repo, &تالو, رستو, 0) < 0) {
		ret = error_errno(_("unable to acquire index lock"));
		اذهب ٻاهريون;
	}

	إذا (write_locked_اشاريو(&اشاريو, &تالو, COMMIT_تالو)) {
		ret = error(_("unable to write new index file"));
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	rollback_تالو_file(&تالو);
	release_اشاريو(&اشاريو);
	أرجع ret;
}

ساكن صحيح ورهايو_commit(بنية repository *repo,
			بنية commit *original,
			بنية pathspec *pathspec,
			بنية commit **ٻاهريون)
{
	بنية interactive_options interactive_opts = INTERACTIVE_OPTIONS_INIT;
	بنية strbuf اشاريو_file = STRBUF_INIT;
	بنية اشاريو_state اشاريو = اشاريو_STATE_INIT(repo);
	ثابت بنية object_id *original_commit_وڻ_oid;
	ثابت بنية object_id *old_وڻ_oid, *new_وڻ_oid;
	بنية object_id والد_وڻ_oid;
	حرف original_commit_oid[GIT_MAX_HEXSZ + 1];
	بنية commit *پهريون_commit, *ٻيو_commit;
	بنية commit_list *parents = عدم;
	بنية وڻ *ورهايو_وڻ;
	صحيح ret;

	إذا (پهريون_والد_وڻ_oid(repo, original, &والد_وڻ_oid) < 0) {
		ret = -1;
		اذهب ٻاهريون;
	}
	original_commit_وڻ_oid = get_commit_وڻ_oid(original);

	/*
	 * Construct the first commit. This is done by taking the original
	 * commit parent's tree and selectively patching changes from the diff
	 * between that parent and its child.
	 */
	repo_git_رستو_replace(repo, &اشاريو_file, "%s", "history-split.index");

	ret = write_ondisk_اشاريو(repo, &والد_وڻ_oid, اشاريو_file.buf);
	إذا (ret < 0)
		اذهب ٻاهريون;

	ret = read_اشاريو_from(&اشاريو, اشاريو_file.buf, repo->gitdir);
	إذا (ret < 0) {
		ret = error(_("failed reading temporary index"));
		اذهب ٻاهريون;
	}

	oid_منزل_hex_r(original_commit_oid, &original->object.oid);
	ret = لاڳيتو_جوڙيو_p_اشاريو(repo, &اشاريو, اشاريو_file.buf, &interactive_opts,
			      original_commit_oid, pathspec, جوڙيو_P_DISALLOW_EDIT);
	إذا (ret < 0)
		اذهب ٻاهريون;

	ورهايو_وڻ = write_اندريون_core_اشاريو_as_وڻ(repo, &اشاريو);
	إذا (!ورهايو_وڻ) {
		ret = error(_("failed split tree"));
		اذهب ٻاهريون;
	}

	unlink(اشاريو_file.buf);
	strbuf_release(&اشاريو_file);

	/*
	 * We disallow the cases where either the split-out commit or the
	 * original commit would become empty. Consequently, if we see that the
	 * new tree ID matches either of those trees we abort.
	 */
	إذا (oideq(&ورهايو_وڻ->object.oid, &والد_وڻ_oid)) {
		ret = error(_("split commit is empty"));
		اذهب ٻاهريون;
	} وإلا إذا (oideq(&ورهايو_وڻ->object.oid, original_commit_وڻ_oid)) {
		ret = error(_("split commit tree matches original commit"));
		اذهب ٻاهريون;
	}

	/*
	 * The first commit is constructed from the split-out tree. The base
	 * that shall be diffed against is the parent of the original commit.
	 */
	ret = commit_وڻ_ext(repo, "split-out", original, عدم, original->parents, &والد_وڻ_oid,
			      &ورهايو_وڻ->object.oid, &پهريون_commit, COMMIT_وڻ_EDIT_پيغام);
	إذا (ret < 0) {
		ret = error(_("failed writing first commit"));
		اذهب ٻاهريون;
	}

	/*
	 * The second commit is constructed from the original tree. The base to
	 * diff against and the parent in this case is the first split-out
	 * commit.
	 */
	commit_list_append(پهريون_commit, &parents);

	old_وڻ_oid = &repo_get_commit_وڻ(repo, پهريون_commit)->object.oid;
	new_وڻ_oid = &repo_get_commit_وڻ(repo, original)->object.oid;

	ret = commit_وڻ_ext(repo, "split-out", original, عدم, parents, old_وڻ_oid,
			      new_وڻ_oid, &ٻيو_commit, COMMIT_وڻ_EDIT_پيغام);
	إذا (ret < 0) {
		ret = error(_("failed writing second commit"));
		اذهب ٻاهريون;
	}

	*ٻاهريون = ٻيو_commit;
	ret = 0;

ٻاهريون:
	إذا (اشاريو_file.len)
		unlink(اشاريو_file.buf);
	strbuf_release(&اشاريو_file);
	commit_list_free(parents);
	release_اشاريو(&اشاريو);
	أرجع ret;
}

ساكن صحيح cmd_history_ورهايو(صحيح argc,
			     ثابت حرف **argv,
			     ثابت حرف *اڳياڙي,
			     بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_ورهايو_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لاڳيتو = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control ref update behavior"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لاڳيتو,
			 N_("perform a dry-run without updating any refs")),
		OPT_پڇاڙي(),
	};
	بنية commit *original, *rewritten = عدم;
	بنية strbuf reflog_نياپو = STRBUF_INIT;
	بنية pathspec pathspec = { 0 };
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, options, usage, 0);
	إذا (argc < 1) {
		ret = error(_("command expects a committish"));
		اذهب ٻاهريون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_ڀريل |
		       PATHSPEC_SYMLINK_LEADING_رستو |
		       PATHSPEC_اڳياڙي_ORIGIN,
		       اڳياڙي, argv + 1);

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب ٻاهريون;
	}

	ret = تياري_revwalk(repo, action, original, &revs);
	إذا (ret < 0)
		اذهب ٻاهريون;

	إذا (original->parents && original->parents->ايندڙ) {
		ret = error(_("cannot split up merge commit"));
		اذهب ٻاهريون;
	}

	ret = ورهايو_commit(repo, original, &pathspec, &rewritten);
	إذا (ret < 0)
		اذهب ٻاهريون;

	strbuf_addf(&reflog_نياپو, "split: updating %s", argv[0]);

	ret = handle_reference_updates(&revs, action, original, rewritten,
				       reflog_نياپو.buf, dry_لاڳيتو, REPLAY_خالي_جايون_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	strbuf_release(&reflog_نياپو);
	clear_pathspec(&pathspec);
	release_revisions(&revs);
	أرجع ret;
}

#وصف_ڪريو SQUASH_ڏٺل (1u << 11)
#وصف_ڪريو SQUASH_TIP (1u << 12)
#وصف_ڪريو SQUASH_AMEND_هدف (1u << 13)

ساكن bool is_autosquash_subject(ثابت حرف *s)
{
	أرجع starts_with(s, "amend!") || starts_with(s, "fixup!") ||
		starts_with(s, "squash!");
}

ساكن bool skip_one_autosquash_اڳياڙي(ثابت حرف *s, ثابت حرف **ٻاهريون)
{
	إذا (skip_اڳياڙي(s, "amend!", ٻاهريون) || skip_اڳياڙي(s, "fixup!", ٻاهريون) ||
	    skip_اڳياڙي(s, "squash!", ٻاهريون)) {
		طالما (**ٻاهريون == ' ')
			(*ٻاهريون)++;
		أرجع true;
	}
	أرجع false;
}

ساكن فراغ truncate_پيغام_منزل_subject(بنية strbuf *نياپو)
{
	ثابت حرف *eos = لڙي_ڳولا(نياپو->buf, "\n\n");

	إذا (eos)
		strbuf_setlen(نياپو, eos - نياپو->buf + 1);
}

بنية subject_معلومات {
	بنية strintmap subjects;
	بنية strbuf subject;
	بنية strbuf squash_پيغام;
	ثابت حرف *پيغام;
	bool edit_پيغام;
};

#وصف_ڪريو SUBJECT_معلومات_INIT {		\
	.subjects = STRINTMAP_INIT,	\
	.subject = STRBUF_INIT,		\
	.squash_پيغام = STRBUF_INIT,	\
}

ساكن فراغ subject_معلومات_clear(بنية subject_معلومات *معلومات)
{
	strintmap_clear(&معلومات->subjects);
	strbuf_release(&معلومات->subject);
	strbuf_release(&معلومات->squash_پيغام);
}

ساكن صحيح squash_amend_پيغام(بنية repository *repo,
				بنية commit *commit,
				بنية subject_معلومات *معلومات,
				بنانشان flags)
{
	ثابت حرف *body = معلومات->پيغام + معلومات->subject.len;

	طالما (خال_آهي(*body))
		body++;

	إذا (!*body) {
		warning(_("ignoring %s (%s): message body is empty"),
			repo_ڳوليو_unique_abbrev(repo, &commit->object.oid,
						DEFAULT_ABBREV),
			معلومات->subject.buf);
		أرجع 0;
	}

	إذا (معلومات->edit_پيغام) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_هدف) {
		إذا (starts_with(معلومات->squash_پيغام.buf, "squash!"))
			أرجع error(_("squashing %s (%s) would overwrite "
				       "'squash!' message, please combine them "
				       "using '--edit'"),
				     repo_ڳوليو_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     معلومات->subject.buf);
		إذا (starts_with(معلومات->squash_پيغام.buf, "fixup!"))
			strbuf_splice(&معلومات->squash_پيغام, 0, 5, "amend!", 5);
		إذا (starts_with(معلومات->squash_پيغام.buf, "amend!")) {
			truncate_پيغام_منزل_subject(&معلومات->squash_پيغام);
			strbuf_addch(&معلومات->squash_پيغام, '\n');
		} وإلا {
			strbuf_reset(&معلومات->squash_پيغام);
		}
		strbuf_addstr(&معلومات->squash_پيغام, body);
		strbuf_complete_سٽ(&معلومات->squash_پيغام);
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			     repo_ڳوليو_unique_abbrev(repo, &commit->object.oid,
						     DEFAULT_ABBREV),
			     معلومات->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_squash_پيغام(بنية repository *repo,
				بنية commit *commit,
				بنية subject_معلومات *معلومات,
				بنانشان flags)
{
	ثابت حرف *body = معلومات->پيغام + معلومات->subject.len;

	طالما (خال_آهي(*body))
		body++;

	إذا (معلومات->edit_پيغام) {
		أرجع 0;
	} وإلا إذا (flags & SQUASH_AMEND_هدف) {
		إذا (starts_with(معلومات->squash_پيغام.buf, "fixup!")) {
			truncate_پيغام_منزل_subject(&معلومات->squash_پيغام);
			strbuf_splice(&معلومات->squash_پيغام, 0, 5, "squash", 6);
		}
		إذا (starts_with(معلومات->squash_پيغام.buf, "squash!")) {
			strbuf_addch(&معلومات->squash_پيغام, '\n');
			strbuf_addstr(&معلومات->squash_پيغام, body);
			strbuf_complete_سٽ(&معلومات->squash_پيغام);
		} وإلا {
			أرجع error(_("squashing %s (%s) would discard its "
				       "message, please combine them using "
				       "'--edit'"),
				     repo_ڳوليو_unique_abbrev(repo,
							     &commit->object.oid,
							     DEFAULT_ABBREV),
				     معلومات->subject.buf);
		}
	} وإلا {
		أرجع error(_("cannot squash %s (%s) that does not target "
			       "base commit without '--edit'"),
			      repo_ڳوليو_unique_abbrev(repo, &commit->object.oid,
						      DEFAULT_ABBREV),
			      معلومات->subject.buf);
	}
	أرجع 0;
}

ساكن صحيح squash_check_can_autosquash(بنية repository *repo,
				       بنية commit *commit,
				       بنية subject_معلومات *معلومات,
				       بنانشان flags)
{
	commit->object.flags |= flags & SQUASH_AMEND_هدف;
	إذا (starts_with(معلومات->subject.buf, "amend!"))
		أرجع squash_amend_پيغام(repo, commit, معلومات, flags);
	وإلا إذا (starts_with(معلومات->subject.buf, "squash!"))
		أرجع squash_squash_پيغام(repo, commit, معلومات, flags);

	أرجع 0;
}

ساكن صحيح squash_check_autosquash_subject(بنية repository *repo,
					   بنية commit *commit,
					   بنية subject_معلومات *معلومات)
{
	ثابت حرف* s = معلومات->subject.buf;
	بنية commit *هدف;
	بنية hashmap_iter iter;
	بنية strmap_داخلا *داخلا;
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
	طالما (skip_one_autosquash_اڳياڙي(s, &s)) {
		بنانشان flags = strintmap_get(&معلومات->subjects, s);
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit, معلومات, flags);
	}
	/*
	 * Allow "fixup! <hex object id>", but not "fixup! HEAD^" or
	 * "fixup! main". If the target is not being squashed check the subject
	 * to allow "fixup! abc123" and "fixup! <subject of abc123>" to be
	 * squashed together.
	 */
	هدف = lookup_commit_reference_by_name(s);
	إذا (هدف && starts_with(oid_منزل_hex(&هدف->object.oid), s)) {
		بنانشان flags =
			هدف->object.flags & (SQUASH_ڏٺل | SQUASH_AMEND_هدف);
		إذا (!flags) {
			ثابت حرف *subject_شروعات;
			ثابت حرف *بفر = repo_logmsg_reencode(repo, هدف,
								  عدم, عدم);
			سائيز_t subject_len = ڳوليو_commit_subject(بفر,
								 &subject_شروعات);
			حرف *subject = xmemdupz(subject_شروعات, subject_len);

			flags = strintmap_get(&معلومات->subjects, subject);
			حرر(subject);
			repo_unuse_commit_بفر(repo, هدف, بفر);
		}
		إذا (flags)
			أرجع squash_check_can_autosquash(repo, commit,
							   معلومات, flags);
	}
	/* Try subject prefix matches */
	strintmap_for_each_داخلا(&معلومات->subjects, &iter, داخلا) {
		s = معلومات->subject.buf;
		طالما(skip_one_autosquash_اڳياڙي(s, &s)) {
			إذا (starts_with(داخلا->ڪنجي, s)) {
				بنانشان قدر = (intptr_t)داخلا->قدر;

				أرجع squash_check_can_autosquash(repo, commit,
								   معلومات, قدر);
			}
		}
	}
	أرجع error(_("cannot squash %s (%s): its target is not being "
		       "squashed"),
		       repo_ڳوليو_unique_abbrev(repo, &commit->object.oid,
					       DEFAULT_ABBREV),
		       معلومات->subject.buf);
}

ساكن صحيح squash_check_subject(بنية repository *repo,
				بنية commit *commit,
				بنية subject_معلومات *معلومات)
{
	صحيح ret = 0;
	ثابت حرف *buf = repo_logmsg_reencode(repo, commit, عدم, عدم);
	سائيز_t subject_len = ڳوليو_commit_subject(buf, &معلومات->پيغام);

	strbuf_reset(&معلومات->subject);
	strbuf_جوڙيو(&معلومات->subject, معلومات->پيغام, subject_len);

	إذا (!strintmap_get_سائيز(&معلومات->subjects)) {
		ثابت حرف *s;

		strbuf_addstr(&معلومات->squash_پيغام, معلومات->پيغام);
		strbuf_complete_سٽ(&معلومات->squash_پيغام);
		/*
		 * Strip a single autosquash prefix to allow squashing
		 *     fixup! base
		 *     amend! base
		 */
		s = معلومات->subject.buf;
		skip_one_autosquash_اڳياڙي(s, &s);
		strintmap_سيٽ(&معلومات->subjects, s, SQUASH_AMEND_هدف | SQUASH_ڏٺل);
		commit->object.flags |= SQUASH_AMEND_هدف;
	} وإلا إذا (is_autosquash_subject(معلومات->subject.buf)) {
		ret = squash_check_autosquash_subject(repo, commit, معلومات);
	} وإلا {
		strintmap_سيٽ(&معلومات->subjects, معلومات->subject.buf, SQUASH_ڏٺل);
	}
	repo_unuse_commit_بفر(repo, commit, buf);
	أرجع ret;
}

ساكن صحيح build_squash_پيغام(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *ٻاهريون);

ساكن صحيح تياري_squash_revisions(بنية repository *repo,
				  صحيح argc, ثابت حرف **argv,
				  بنية rev_info *revs)
{
	repo_init_revisions(repo, revs, عدم);
	revs->ابتو = 1;
	revs->topo_ترتيب = 1;
	revs->ترتيب_ڏيو_ترتيب = REV_ترتيب_ڏيو_اندريون_گراف_ترتيب;
	revs->simplify_history = 0;
	revs->ancestry_رستو = 1;
	revs->limited = 1;
	revs->ancestry_رستو_implicit_bottoms = 1;

	argc = تياري_revisions(argc, argv, revs, عدم);
	إذا (argc > 1)
		أرجع error(_("unrecognized argument: %s"), argv[1]);

	إذا (revs->ابتو != 1 || revs->topo_ترتيب != 1 ||
	    revs->ترتيب_ڏيو_ترتيب != REV_ترتيب_ڏيو_اندريون_گراف_ترتيب ||
	    revs->simplify_history != 0 || revs->boundary == 1 ||
	    revs->ancestry_رستو != 1 || revs->limited != 1 ||
	    revs->ancestry_رستو_implicit_bottoms != 1) {
		warning(_("ignoring rev-list options that would change how the "
			  "range is walked"));
		revs->ابتو = 1;
		revs->topo_ترتيب = 1;
		revs->ترتيب_ڏيو_ترتيب = REV_ترتيب_ڏيو_اندريون_گراف_ترتيب;
		revs->simplify_history = 0;
		revs->boundary = 0;
		revs->ancestry_رستو = 1;
		revs->limited = 1;
		revs->ancestry_رستو_implicit_bottoms = 1;
	}

	/*
	 * A squash needs a base to reparent onto, so the range has to exclude
	 * something, as in "<base>..<tip>". A revision range with no such
	 * bottom commit cannot be squashed.
	 */
	لكل (سائيز_t i = 0; i < revs->cmdline.nr; i++)
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
				bool edit_پيغام,
				صحيح argc, ثابت حرف **argv,
				بنية commit **oldest_ٻاهريون,
				بنية commit **tip_ٻاهريون,
				حرف **پيغام_ٻاهريون)
{
	بنية rev_info revs;
	بنية subject_معلومات subject_معلومات = SUBJECT_معلومات_INIT;
	بنية commit *commit, *oldest = عدم, *tip = عدم;
	بنية strbuf todo_buf = STRBUF_INIT;
	صحيح ret, tip_ڳڻپ = 0;
	bool walk_started = false;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_array refs = { 0 };

	subject_معلومات.edit_پيغام = edit_پيغام;
	ret = تياري_squash_revisions(repo, argc, argv, &revs);
	إذا (ret < 0)
		اذهب ٻاهريون;

	إذا (prepare_revision_walk(&revs) < 0) {
		ret = error(_("error preparing revisions"));
		اذهب ٻاهريون;
	}
	walk_started = true;
	طالما ((commit = get_revision(&revs))) {
		بنية commit_list *p;

		إذا (edit_پيغام)
			strbuf_addf(&todo_buf, "pick %s\n",
				    oid_منزل_hex(&commit->object.oid));

		إذا (!commit->parents) {
			ret = error(_("cannot squash down to root commit"));
			اذهب ٻاهريون;
		}
		لكل (p = commit->parents; oldest && p; p = p->ايندڙ) {
			بنية commit_list *q;
			بنية object *o;
			bool ڏٺل;

			إذا (repo_parse_commit(repo, p->شي)) {
				ret = error(_("cannot parse commit"));
				اذهب ٻاهريون;
			}
			o = &p->شي->object;
			ڏٺل = o->flags & SQUASH_ڏٺل;
			/*
			 * Allow parents that match the parents of the
			 * squashed commit.
			 */
			لكل (q = oldest->parents; !ڏٺل && q; q = q->ايندڙ) {
				إذا (p->شي == q->شي) {
					ڏٺل = true;
					commit_list_وجهو(commit, &filter.with_commit);
				}
			}
			إذا (!ڏٺل) {
				ret = error(_("parent %s of commit %s is "
					      "outside the revision range"),
					    repo_ڳوليو_unique_abbrev(repo, &o->oid,
								    DEFAULT_ABBREV),
					    repo_ڳوليو_unique_abbrev(repo,
								    &commit->object.oid,
								    DEFAULT_ABBREV));
				اذهب ٻاهريون;
			}
			إذا (o->flags & SQUASH_TIP) {
				tip_ڳڻپ--;
				o->flags &= ~SQUASH_TIP;
			}
		}
		إذا (!oldest) {
			commit_list_وجهو(commit, &filter.with_commit);
			oldest = commit;
		}
		إذا (squash_check_subject(repo, commit, &subject_معلومات)) {
			ret = -1;
			اذهب ٻاهريون;
		}
		tip = commit;
		tip->object.flags |= SQUASH_ڏٺل | SQUASH_TIP;
		tip_ڳڻپ++;
	}
	clear_object_flags(repo, SQUASH_ڏٺل | SQUASH_TIP);
	reset_revision_walk();
	walk_started = false;

	إذا (!tip_ڳڻپ) {
		ret = error(_("the revision range is empty"));
		اذهب ٻاهريون;
	} وإلا إذا (tip_ڳڻپ != 1) {
		ret = error(_("the revision range contains more than one tip "
			      "commit"));
		اذهب ٻاهريون;
	} وإلا إذا (oldest == tip) {
		ret = error(_("the revision range holds a single commit; "
			      "nothing to squash"));
		اذهب ٻاهريون;
	} وإلا إذا (!oldest->parents) {
		BUG("an in-range commit must have a parent");
	}

	commit_list_وجهو(tip, &filter.no_commit);
	filter.قسم = FILTER_REFS_BRANCHES;
	إذا (update_branches &&
	    filter_refs(&refs, &filter, filter.قسم)) {
		ret = error(_("could not filter refs"));
		اذهب ٻاهريون;
	}
	إذا (refs.nr) {
		بنية ref_format format = REF_FORMAT_INIT;
		بنية ref_ترتيب_ڏيڻ *ترتيب_ڏيڻ;
		بنية string_list ترتيب_ڏيڻ_options = STRING_LIST_INIT_DUP;
		بنية strbuf branches = STRBUF_INIT;
		بنية strbuf err = STRBUF_INIT;

		format.format = "%(refname:short)";
		إذا (verify_ref_format(&format))
			BUG("invalid branch format");
		string_list_append(&ترتيب_ڏيڻ_options, "refname");
		ترتيب_ڏيڻ = ref_ترتيب_ڏيڻ_options(&ترتيب_ڏيڻ_options);
		ref_array_ترتيب_ڏيو(ترتيب_ڏيڻ, &refs);
		لكل (صحيح i = 0; i < refs.nr; i++) {
			strbuf_reset(&err);
			strbuf_addstr(&branches, "\n  ");
			إذا (format_ref_array_شي(refs.شيون[i], &format,
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
		ref_ترتيب_ڏيڻ_release(ترتيب_ڏيڻ);
		string_list_clear(&ترتيب_ڏيڻ_options, 0);
		اذهب ٻاهريون;
	}
	إذا (edit_پيغام) {
		strbuf_reset(&subject_معلومات.squash_پيغام);
		ret = build_squash_پيغام(repo, &todo_buf,
					   &subject_معلومات.squash_پيغام);
		إذا (ret < 0)
			اذهب ٻاهريون;
	}

	*oldest_ٻاهريون = oldest;
	*tip_ٻاهريون = tip;
	*پيغام_ٻاهريون = strbuf_detach(&subject_معلومات.squash_پيغام, عدم);
	ret = 0;

ٻاهريون:
	strbuf_release(&todo_buf);
	clear_object_flags(repo, SQUASH_ڏٺل | SQUASH_TIP |
			   SQUASH_AMEND_هدف);
	إذا (walk_started)
		reset_revision_walk();
	subject_معلومات_clear(&subject_معلومات);
	release_revisions(&revs);
	ref_filter_clear(&filter);
	ref_array_clear(&refs);
	أرجع ret;
}

ساكن bool amend_replaces_هدف(بنية todo_list *todo, صحيح هدف)
{
	لكل (صحيح i = هدف + 1; i < todo->nr &&
				 todo->شيون[i].command != TODO_PICK; i++) {
		إذا (todo->شيون[i].command == TODO_SQUASH)
			أرجع false;
		إذا (todo->شيون[i].flags & TODO_REPLACE_FIXUP_نياپو)
			أرجع true;
	}
	أرجع false;
}

ساكن صحيح build_squash_پيغام(بنية repository *repo,
				ثابت بنية strbuf *todo_buf,
				بنية strbuf *ٻاهريون)
{
	بنية todo_list todo = TODO_LIST_INIT;
	بنية replay_opts opts = REPLAY_OPTS_INIT;
	صحيح nr_commits, ret;

	إذا (todo_list_parse_insn_بفر(repo, &opts, todo_buf->buf, &todo) < 0 ||
	    todo_list_rearrange_squash(&todo) < 0) {
		ret = error(_("could not prepare the squash message"));
		اذهب ٻاهريون;
	}

	nr_commits = todo.nr;
	لكل (صحيح i = 0; i < nr_commits; i++) {
		بنية todo_شي *شي = &todo.شيون[i];
		ثابت حرف *پيغام, *body;
		سائيز_t commented_len;
		bool skip, squashing;

		squashing = شي->command == TODO_SQUASH ||
			    (شي->flags & TODO_REPLACE_FIXUP_نياپو);
		إذا (شي->command == TODO_PICK)
			skip = amend_replaces_هدف(&todo, i);
		وإلا
			skip = !squashing;

		پيغام = repo_logmsg_reencode(repo, شي->commit, عدم, عدم);
		ڳوليو_commit_subject(پيغام, &body);

		إذا (skip)
			commented_len = لڙي_ڊيگهه(body);
		وإلا إذا (squashing)
			commented_len = squash_subject_comment_len(body, 1);
		وإلا
			commented_len = 0;

		إذا (!i)
			جوڙيو_squash_combination_سرو(ٻاهريون, nr_commits);
		strbuf_addch(ٻاهريون, '\n');
		جوڙيو_squash_پيغام_سرو(ٻاهريون, i + 1, skip);
		strbuf_addstr(ٻاهريون, "\n\n");
		strbuf_جوڙيو_commented_سٽون(ٻاهريون, body, commented_len, comment_سٽ_str);
		strbuf_addstr(ٻاهريون, body + commented_len);
		strbuf_complete_سٽ(ٻاهريون);

		repo_unuse_commit_بفر(repo, شي->commit, پيغام);
	}

	ret = 0;

ٻاهريون:
	todo_list_release(&todo);
	replay_opts_release(&opts);
	أرجع ret;
}

ساكن صحيح cmd_history_squash(صحيح argc,
			      ثابت حرف **argv,
			      ثابت حرف *اڳياڙي,
			      بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لاڳيتو = 0;
	صحيح edit = 1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لاڳيتو,
			 N_("perform a dry-run without updating any refs")),
		OPT_BOOL('e', "edit", &edit,
			 N_("edit the commit message")),
		OPT_پڇاڙي(),
	};
	بنية strbuf reflog_نياپو = STRBUF_INIT;
	بنية commit *oldest, *tip, *rewritten;
	ثابت بنية object_id *بنياد_وڻ_oid, *tip_وڻ_oid;
	حرف *پيغام_template = عدم;
	بنية rev_info revs = { 0 };
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, options, usage,
			     PARSE_OPT_KEEP_UNKNOWN_OPT | PARSE_OPT_KEEP_ARGV0);
	إذا (argc < 2) {
		ret = error(_("command expects a revision range"));
		اذهب ٻاهريون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	strbuf_addstr(&reflog_نياپو, "squash: updating ");
	strbuf_join_argv(&reflog_نياپو, argc - 1, argv + 1, ' ');

	ret = resolve_squash_range(repo, action == REF_ACTION_BRANCHES,
				   edit,
				   argc, argv, &oldest, &tip,
				   &پيغام_template);
	إذا (ret < 0)
		اذهب ٻاهريون;

	ret = تياري_revwalk(repo, action, tip, &revs);
	إذا (ret < 0)
		اذهب ٻاهريون;

	بنياد_وڻ_oid = &repo_get_commit_وڻ(repo,
					oldest->parents->شي)->object.oid;
	tip_وڻ_oid = &repo_get_commit_وڻ(repo, tip)->object.oid;

	ret = commit_وڻ_ext(repo, "squash", oldest, پيغام_template,
			      oldest->parents, بنياد_وڻ_oid, tip_وڻ_oid,
			      &rewritten,
			      edit ? COMMIT_وڻ_EDIT_پيغام : 0);
	إذا (ret < 0) {
		ret = error(_("failed writing squashed commit"));
		اذهب ٻاهريون;
	}

	ret = handle_reference_updates(&revs, action, tip, rewritten,
				       reflog_نياپو.buf, dry_لاڳيتو,
				       REPLAY_خالي_جايون_COMMIT_ABORT);
	إذا (ret < 0) {
		ret = error(_("failed replaying descendants"));
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	strbuf_release(&reflog_نياپو);
	release_revisions(&revs);
	حرر(پيغام_template);
	أرجع ret;
}

ساكن صحيح update_worktree(بنية repository *repo,
			   ثابت بنية commit *old_سر,
			   ثابت بنية commit *new_سر,
			   bool dry_لاڳيتو)
{
	بنية reset_working_وڻ_options opts = {
		.oid_from = &old_سر->object.oid,
		.oid = &new_سر->object.oid,
	};
	إذا (dry_لاڳيتو)
		opts.flags |= RESET_WORKING_وڻ_DRY_لاڳيتو;
	أرجع reset_working_وڻ(repo, &opts);
}

ساكن صحيح ڳوليو_سر_وڻ_change(بنية repository *repo,
				 ثابت بنية replay_نتيجو *نتيجو,
				 بنية commit **old_سر,
				 بنية commit **new_سر,
				 bool *بدليو)
{
	ثابت بنية replay_ref_update *سر_update = عدم;
	بنية commit *old_سر_commit, *new_سر_commit;
	بنية وڻ *old_سر_وڻ, *new_سر_وڻ;
	ثابت حرف *سر_هدف;
	صحيح سر_flags;

	*بدليو = false;

	سر_هدف = refs_resolve_ref_unsafe(get_main_ref_store(repo), "HEAD",
					      RESOLVE_REF_NO_RECURSE | RESOLVE_REF_READING,
					      عدم, &سر_flags);
	إذا (!سر_هدف)
		أرجع error(_("cannot look up HEAD"));

	لكل (سائيز_t i = 0; i < نتيجو->updates_nr; i++) {
		إذا (!strcmp(نتيجو->updates[i].refname, سر_هدف)) {
			سر_update = &نتيجو->updates[i];
			اكسر;
		}
	}

	إذا (!سر_update)
		أرجع 0;

	old_سر_commit = lookup_commit_reference(repo, &سر_update->old_oid);
	new_سر_commit = lookup_commit_reference(repo, &سر_update->new_oid);
	إذا (!old_سر_commit || !new_سر_commit)
		أرجع error(_("cannot resolve HEAD commit"));

	old_سر_وڻ = repo_get_commit_وڻ(repo, old_سر_commit);
	new_سر_وڻ = repo_get_commit_وڻ(repo, new_سر_commit);
	إذا (!old_سر_وڻ || !new_سر_وڻ)
		أرجع error(_("cannot resolve tree for HEAD"));

	إذا (oideq(&old_سر_وڻ->object.oid, &new_سر_وڻ->object.oid))
		أرجع 0;

	*old_سر = old_سر_commit;
	*new_سر = new_سر_commit;
	*بدليو = true;

	أرجع 0;
}

ساكن صحيح cmd_history_drop(صحيح argc,
			    ثابت حرف **argv,
			    ثابت حرف *اڳياڙي,
			    بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		عدم,
	};
	تعداد replay_خالي_جايون_commit_action خالي_جايون = REPLAY_خالي_جايون_COMMIT_DROP;
	تعداد ref_action action = REF_ACTION_DEFAULT;
	صحيح dry_لاڳيتو = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "update-refs", &action, "(branches|head)",
			       N_("control which refs should be updated"),
			       PARSE_OPT_NONEG, parse_ref_action),
		OPT_BOOL('n', "dry-run", &dry_لاڳيتو,
			 N_("perform a dry-run without updating any refs")),
		OPT_CALLBACK_F(0, "empty", &خالي_جايون, "(drop|keep|abort)",
			       N_("how to handle descendants that become empty"),
			       PARSE_OPT_NONEG, parse_opt_خالي_جايون),
		OPT_پڇاڙي(),
	};
	بنية strbuf reflog_نياپو = STRBUF_INIT;
	بنية commit *original, *rewritten;
	بنية rev_info revs = { 0 };
	بنية replay_نتيجو نتيجو = { 0 };
	بنية commit *old_سر, *new_سر;
	bool سر_چالون = false;
	صحيح ret;

	argc = parse_options(argc, argv, اڳياڙي, options, usage, 0);
	إذا (argc != 1) {
		ret = error(_("command expects a single revision"));
		اذهب ٻاهريون;
	}
	repo_config(repo, git_default_config, عدم);

	إذا (action == REF_ACTION_DEFAULT)
		action = REF_ACTION_BRANCHES;

	original = lookup_commit_reference_by_name(argv[0]);
	إذا (!original) {
		ret = error(_("commit cannot be found: %s"), argv[0]);
		اذهب ٻاهريون;
	}

	إذا (!original->parents) {
		ret = error(_("cannot drop root commit %s: "
			      "it has no parent to replay onto"),
			    argv[0]);
		اذهب ٻاهريون;
	} وإلا إذا (original->parents->ايندڙ) {
		ret = error(_("cannot drop merge commit: %s"), argv[0]);
		اذهب ٻاهريون;
	}

	ret = تياري_revwalk(repo, action, original, &revs);
	إذا (ret)
		اذهب ٻاهريون;

	rewritten = original->parents->شي;

	ret = compute_pending_ref_updates(&revs, action, original, rewritten,
					  خالي_جايون, &نتيجو);
	إذا (ret) {
		ret = error(_("failed replaying descendants"));
		اذهب ٻاهريون;
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
		ret = ڳوليو_سر_وڻ_change(repo, &نتيجو, &old_سر,
					    &new_سر, &سر_چالون);
		إذا (ret < 0)
			اذهب ٻاهريون;

		إذا (سر_چالون && update_worktree(repo, old_سر, new_سر, true) < 0) {
			ret = error(_("dropping this commit would "
				      "overwrite local changes; aborting"));
			اذهب ٻاهريون;
		}
	}

	strbuf_addf(&reflog_نياپو, "drop: dropping %s", argv[0]);
	ret = apply_pending_ref_updates(repo, &نتيجو, reflog_نياپو.buf, dry_لاڳيتو);
	إذا (ret < 0) {
		ret = error(_("failed to update references"));
		اذهب ٻاهريون;
	}

	إذا (!dry_لاڳيتو && سر_چالون && update_worktree(repo, old_سر, new_سر, false) < 0) {
		ret = error(_("could not update working tree to new commit %s"),
			    oid_منزل_hex(&new_سر->object.oid));
		اذهب ٻاهريون;
	}

	ret = 0;

ٻاهريون:
	replay_نتيجو_release(&نتيجو);
	strbuf_release(&reflog_نياپو);
	release_revisions(&revs);
	أرجع ret;
}

صحيح cmd_history(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *اڳياڙي,
		بنية repository *repo)
{
	ثابت حرف * ثابت usage[] = {
		GIT_HISTORY_DROP_USAGE,
		GIT_HISTORY_FIXUP_USAGE,
		GIT_HISTORY_REWORD_USAGE,
		GIT_HISTORY_ورهايو_USAGE,
		GIT_HISTORY_SQUASH_USAGE,
		عدم,
	};
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("drop", &fn, cmd_history_drop),
		OPT_SUBCOMMAND("fixup", &fn, cmd_history_fixup),
		OPT_SUBCOMMAND("reword", &fn, cmd_history_reword),
		OPT_SUBCOMMAND("split", &fn, cmd_history_ورهايو),
		OPT_SUBCOMMAND("squash", &fn, cmd_history_squash),
		OPT_پڇاڙي(),
	};

	argc = parse_options(argc, argv, اڳياڙي, options, usage, 0);
	أرجع fn(argc, argv, اڳياڙي, repo);
}
