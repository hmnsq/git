/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "pager.h"
#شامل "color.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "diff-merges.h"
#شامل "revision.h"
#شامل "log-tree.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "tag.h"
#شامل "reflog-walk.h"
#شامل "patch-ids.h"
#شامل "path.h"
#شامل "shortlog.h"
#شامل "remote.h"
#شامل "string-list.h"
#شامل "parse-options.h"
#شامل "line-log.h"
#شامل "branch.h"
#شامل "version.h"
#شامل "mailmap.h"
#شامل "progress.h"
#شامل "commit-slab.h"
#شامل "advice.h"
#شامل "utf8.h"

#شامل "commit-reach.h"
#شامل "promisor-remote.h"
#شامل "range-diff.h"
#شامل "tmp-objdir.h"
#شامل "tree.h"
#شامل "userdiff.h"
#شامل "write-or-die.h"

#تعریف_کرو MAIL_DEFAULT_WRAP 72
#تعریف_کرو COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#تعریف_کرو FORMAT_PATCH_NAME_MAX_DEFAULT 64

ساکن غیرعلامتی صحیح force_اندر_body_from;
ساکن صحیح stdout_mboxrd;
ساکن صحیح format_no_سابقہ;

ساکن ثابت حرف * ثابت builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	تہی
};

ساخت سطر_opt_callback_ڈیٹا {
	ساخت rev_info *rev;
	ثابت حرف *سابقہ;
	ساخت string_list دلائل;
};

ساکن صحیح session_is_interactive(خالی)
{
	واپس isatty(1) || pager_اندر_use();
}

ساکن صحیح auto_decoration_style(خالی)
{
	واپس session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ساکن صحیح parse_decoration_style(ثابت حرف *قدر)
{
	انتخاب (git_parse_maybe_bool(قدر)) {
	حالت 1:
		واپس DECORATE_SHORT_REFS;
	حالت 0:
		واپس 0;
	طےشدہ:
		توڑو;
	}
	اگر (!strcmp(قدر, "full"))
		واپس DECORATE_بھرا_REFS;
	ورنہ اگر (!strcmp(قدر, "short"))
		واپس DECORATE_SHORT_REFS;
	ورنہ اگر (!strcmp(قدر, "auto"))
		واپس auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	واپس -1;
}

ساخت log_config {
	صحیح default_abbrev_commit;
	صحیح default_show_جڑ;
	صحیح default_follow;
	صحیح default_show_signature;
	صحیح default_encode_email_سرخیاں;
	صحیح decoration_style;
	صحیح decoration_given;
	صحیح use_mailmap_config;
	حرف *fmt_patch_subject_سابقہ;
	صحیح fmt_patch_name_max;
	حرف *fmt_pretty;
	حرف *default_date_منوال;

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	صحیح i_still_use_this;
#اگر_ختم
};

ساکن خالی log_config_init(ساخت log_config *cfg)
{
	memset(cfg, 0, حجم(*cfg));
	cfg->default_show_جڑ = 1;
	cfg->default_encode_email_سرخیاں = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_سابقہ = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ساکن خالی log_config_release(ساخت log_config *cfg)
{
	آزاد(cfg->default_date_منوال);
	آزاد(cfg->fmt_pretty);
	آزاد(cfg->fmt_patch_subject_سابقہ);
}

ساکن صحیح use_default_decoration_filter = 1;
ساکن ساخت string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ساکن ساخت string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ساکن ساخت string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ساکن صحیح clear_decorations_callback(ثابت ساخت option *opt غیر_مستعمل,
				      ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	واپس 0;
}

ساکن صحیح decorate_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			     صحیح unset)
{
	ساخت log_config *cfg = opt->قدر;

	اگر (unset)
		cfg->decoration_style = 0;
	ورنہ اگر (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	ورنہ
		cfg->decoration_style = DECORATE_SHORT_REFS;

	اگر (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	واپس 0;
}

ساکن صحیح log_سطر_range_callback(ثابت ساخت option *option, ثابت حرف *arg, صحیح unset)
{
	ساخت سطر_opt_callback_ڈیٹا *ڈیٹا = option->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!arg)
		واپس -1;

	ڈیٹا->rev->سطر_level_traverse = 1;
	string_list_append(&ڈیٹا->دلائل, arg);

	واپس 0;
}

ساکن خالی cmd_log_init_defaults(ساخت rev_info *rev,
				  ساخت log_config *cfg)
{
	اگر (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	اگر (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_سرخی = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_جڑ_diff = cfg->default_show_جڑ;
	rev->subject_سابقہ = cfg->fmt_patch_subject_سابقہ;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_سرخیاں = cfg->default_encode_email_سرخیاں;

	اگر (cfg->default_date_منوال)
		parse_date_format(cfg->default_date_منوال, &rev->date_منوال);
}

ساکن خالی سیٹ_default_decoration_filter(ساخت decoration_filter *decoration_filter)
{
	حرف *قدر = تہی;
	ساخت string_list *include = decoration_filter->include_ref_نمونہ;
	ثابت ساخت string_list *config_exclude;

	اگر (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ساخت string_list_شے *شے;
		for_each_string_list_شے(شے, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_نمونہ,
					   شے->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	اگر (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &قدر) &&
	    !strcmp("all", قدر))
		use_default_decoration_filter = 0;
	آزاد(قدر);

	اگر (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_نمونہ->nr ||
	    decoration_filter->include_ref_نمونہ->nr ||
	    decoration_filter->exclude_ref_config_نمونہ->nr)
		واپس;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	برائے (سائز_t i = 0; i < ARRAY_سائز(ref_namespace); i++) {
		اگر (!ref_namespace[i].decoration)
			جاری;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ساکن خالی cmd_log_init_اختتام(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت rev_info *rev, ساخت تیاری_revision_opt *opt,
			 ساخت log_config *cfg)
{
	ساخت userformat_want w;
	صحیح quiet = 0, ماخذ = 0, mailmap;
	ساکن ساخت سطر_opt_callback_ڈیٹا سطر_cb = {تہی, تہی, STRING_LIST_INIT_DUP};
	ساخت decoration_filter decoration_filter = {
		.exclude_ref_نمونہ = &decorate_refs_exclude,
		.include_ref_نمونہ = &decorate_refs_include,
		.exclude_ref_config_نمونہ = &decorate_refs_exclude_config,
	};
	ساکن ساخت revision_sources revision_sources;

	ثابت ساخت option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &ماخذ, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#اگر_ختم
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", تہی, تہی,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, تہی, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', تہی, &سطر_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_سطر_range_callback),
		OPT_انتہا()
	};

	سطر_cb.rev = rev;
	سطر_cb.سابقہ = سابقہ;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, سابقہ,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = تیاری_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	اگر (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	اگر (rev->سطر_level_traverse && rev->prune_ڈیٹا.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, حجم(w));
	userformat_ڈھونڈو_requirements(تہی, &w);

	اگر (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	اگر (rev->show_notes)
		بھرو_display_notes(&rev->notes_opt);

	اگر ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_نقاب) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_سرخی = 0;

	اگر (ماخذ || w.ماخذ) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	اگر (mailmap) {
		rev->mailmap = xmalloc(حجم(ساخت string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	اگر (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		اگر (!cfg->decoration_given)
			cfg->decoration_style = 0;
		اگر (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	اگر (rev->commit_format == CMIT_FMT_USERFORMAT) {
		اگر (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} ورنہ اگر (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	اگر (cfg->decoration_style || rev->simplify_by_decoration) {
		سیٹ_default_decoration_filter(&decoration_filter);

		اگر (cfg->decoration_style)
			rev->show_decorations = 1;

		بھرو_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	اگر (rev->سطر_level_traverse)
		سطر_log_init(rev, سطر_cb.سابقہ, &سطر_cb.دلائل);

	تیاری_pager(the_repository);
}

ساکن خالی cmd_log_init(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت rev_info *rev, ساخت تیاری_revision_opt *opt,
			 ساخت log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_اختتام(argc, argv, سابقہ, rev, opt, cfg);
}

ساکن صحیح cmd_log_walk_no_free(ساخت rev_info *rev)
{
	ساخت commit *commit;
	صحیح saved_nrl = 0;
	صحیح saved_dcctc = 0;
	صحیح نتیجہ;

	اگر (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	جب ((commit = get_revision(rev)) != تہی) {
		اگر (!log_درخت_commit(rev, commit) && rev->max_گنتی >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_گنتی++;
		اگر (!rev->reflog_info && !rev->remerge_diff) {
			/*
			 * We may show a given commit multiple times when
			 * walking the reflogs. Therefore we still need it.
			 *
			 * Likewise, we potentially still need the parents
			 * of * already shown commits to determine merge
			 * bases when showing remerge diffs.
			 */
			free_commit_بفر(the_repository->parsed_objects,
					   commit);
			commit_list_free(commit->parents);
			commit->parents = تہی;
		}
		اگر (saved_nrl < rev->diffopt.needed_rename_حد_بندی)
			saved_nrl = rev->diffopt.needed_rename_حد_بندی;
		اگر (rev->diffopt.degraded_cc_مقصد_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_مقصد_c = saved_dcctc;
	rev->diffopt.needed_rename_حد_بندی = saved_nrl;

	نتیجہ = diff_نتیجہ_رمز(rev);
	اگر (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		نتیجہ = 02;
	}
	واپس نتیجہ;
}

ساکن صحیح cmd_log_walk(ساخت rev_info *rev)
{
	صحیح retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	واپس retval;
}

ساکن صحیح git_log_config(ثابت حرف *var, ثابت حرف *قدر,
			  ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت log_config *cfg = cb;
	ثابت حرف *خانہ_name;

	اگر (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		واپس git_config_string(&cfg->fmt_pretty, var, قدر);
	}
	اگر (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_سابقہ);
		واپس git_config_string(&cfg->fmt_patch_subject_سابقہ, var, قدر);
	}
	اگر (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, قدر, ctx->kvi);
		واپس 0;
	}
	اگر (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_سرخیاں = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_منوال);
		واپس git_config_string(&cfg->default_date_منوال, var, قدر);
	}
	اگر (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(قدر);
		اگر (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		واپس 0;
	}
	اگر (!strcmp(var, "log.diffmerges")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		واپس diff_merges_config(قدر);
	}
	اگر (!strcmp(var, "log.showroot")) {
		cfg->default_show_جڑ = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (skip_سابقہ(var, "color.decorate.", &خانہ_name))
		واپس parse_decorate_color_config(var, خانہ_name, قدر);
	اگر (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, قدر);
		واپس 0;
	}

	واپس git_diff_ui_config(var, قدر, ctx, cb);
}

#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
صحیح cmd_whatchanged(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ,
		    ساخت repository *repo غیر_مستعمل)
{
	ساخت log_config cfg;
	ساخت rev_info rev;
	ساخت تیاری_revision_opt opt;
	صحیح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, سابقہ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, سابقہ, &rev, &opt, &cfg);

	اگر (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	اگر (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	واپس ret;
}
#اگر_ختم

ساکن خالی show_tagger(ثابت حرف *buf, ساخت rev_info *rev)
{
	ساخت strbuf برآمد = STRBUF_INIT;
	ساخت pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_منوال = rev->date_منوال;
	pp_user_info(&pp, "Tagger", &برآمد, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", برآمد.buf);
	strbuf_release(&برآمد);
}

ساکن صحیح show_blob_object(ثابت ساخت object_id *oid, ساخت rev_info *rev, ثابت حرف *obj_name)
{
	ساخت object_id oidc;
	ساخت object_context obj_context = {0};
	حرف *buf;
	غیرعلامتی طویل سائز;

	fflush(rev->diffopt.file);
	اگر (!rev->diffopt.flags.textconv_سیٹ_بذریعہ_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		واپس odb_دھارا_blob_مقصد_fd(the_repository->objects, 1, oid, تہی, 0);

	اگر (get_oid_with_context(the_repository, obj_name,
				 GET_OID_ریکارڈ_راستہ,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	اگر (!obj_context.راستہ ||
	    !textconv_object(the_repository, obj_context.راستہ,
			     obj_context.منوال, &oidc, 1, &buf, &سائز)) {
		object_context_release(&obj_context);
		واپس odb_دھارا_blob_مقصد_fd(the_repository->objects, 1, oid, تہی, 0);
	}

	اگر (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, سائز);
	object_context_release(&obj_context);
	آزاد(buf);
	واپس 0;
}

ساکن صحیح show_لیبل_object(ثابت ساخت object_id *oid, ساخت rev_info *rev)
{
	سائز_t سائز;
	شمار object_type type;
	حرف *buf = odb_read_object(the_repository->objects, oid, &type, &سائز);
	غیرعلامتی طویل انحراف = 0;

	اگر (!buf)
		واپس error(_("could not read object %s"), oid_مقصد_hex(oid));

	أكد(type == OBJ_لیبل);
	جب (انحراف < سائز && buf[انحراف] != '\n') {
		غیرعلامتی طویل new_انحراف = انحراف + 1;
		ثابت حرف *ident;
		جب (new_انحراف < سائز && buf[new_انحراف++] != '\n')
			; /* do nothing */
		اگر (skip_سابقہ(buf + انحراف, "tagger ", &ident))
			show_tagger(ident, rev);
		انحراف = new_انحراف;
	}

	اگر (انحراف < سائز)
		fwrite(buf + انحراف, سائز - انحراف, 1, rev->diffopt.file);
	آزاد(buf);
	واپس 0;
}

ساکن صحیح show_درخت_object(ثابت ساخت object_id *oid غیر_مستعمل,
			    ساخت strbuf *بنیاد غیر_مستعمل,
			    ثابت حرف *pathname, غیرعلامتی منوال,
			    خالی *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(منوال) ? "/" : "");
	واپس 0;
}

ساکن خالی show_تیاری_revisions_tweak(ساخت rev_info *rev)
{
	اگر (rev->پہلا_والد_only)
		diff_merges_default_مقصد_پہلا_والد(rev);
	ورنہ
		diff_merges_default_مقصد_dense_combined(rev);
	اگر (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

صحیح cmd_show(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repo غیر_مستعمل)
{
	ساخت log_config cfg;
	ساخت rev_info rev;
	غیرعلامتی صحیح i;
	ساخت تیاری_revision_opt opt;
	ساخت pathspec مطابقت_all;
	صحیح ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_بھرا_اشاریہ = 0;
	}

	memset(&مطابقت_all, 0, حجم(مطابقت_all));
	repo_init_revisions(the_repository, &rev, سابقہ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_سرخی = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_چوڑائی = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	opt.tweak = show_تیاری_revisions_tweak;
	cmd_log_init(argc, argv, سابقہ, &rev, &opt, &cfg);

	اگر (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		واپس ret;
	}

	rev.diffopt.no_free = 1;
	برائے (i = 0; i < rev.pending.nr && !ret; i++) {
		ساخت object *o = rev.pending.objects[i].شے;
		ثابت حرف *name = rev.pending.objects[i].name;
		انتخاب (o->type) {
		حالت OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			توڑو;
		حالت OBJ_لیبل: {
			ساخت لیبل *t = (ساخت لیبل *)o;
			ساخت object_id *oid = get_tagged_oid(t);

			اگر (rev.shown_one)
				حرف_لکھو('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->لیبل,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_لیبل_object(&o->oid, &rev);
			rev.shown_one = 1;
			اگر (ret)
				توڑو;
			o = parse_object(the_repository, oid);
			اگر (!o)
				ret = error(_("could not read object %s"),
					    oid_مقصد_hex(oid));
			rev.pending.objects[i].شے = o;
			i--;
			توڑو;
		}
		حالت OBJ_درخت:
			اگر (rev.shown_one)
				حرف_لکھو('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_درخت(the_repository, (ساخت درخت *)o,
				  &مطابقت_all, show_درخت_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			توڑو;
		حالت OBJ_COMMIT:
		{
			ساخت object_array old;
			ساخت object_array blank = OBJECT_ARRAY_INIT;

			حافظہ_نقل(&old, &rev.pending, حجم(old));
			حافظہ_نقل(&rev.pending, &blank, حجم(rev.pending));

			جوڑو_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			حافظہ_نقل(&rev.pending, &old, حجم(rev.pending));
			توڑو;
		}
		طےشدہ:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	واپس ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
صحیح cmd_log_reflog(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ساخت log_config cfg;
	ساخت rev_info rev;
	ساخت تیاری_revision_opt opt;
	صحیح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, سابقہ);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_سرخی = 1;
	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_سرخی = 1;
	cmd_log_init_اختتام(argc, argv, سابقہ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	واپس ret;
}

ساکن خالی log_تیاری_revisions_tweak(ساخت rev_info *rev)
{
	اگر (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_ڈیٹا, 0))
		rev->diffopt.flags.follow_renames = 1;

	اگر (rev->پہلا_والد_only)
		diff_merges_default_مقصد_پہلا_والد(rev);
}

صحیح cmd_log(صحیح argc,
	    ثابت حرف **argv,
	    ثابت حرف *سابقہ,
	    ساخت repository *repo غیر_مستعمل)
{
	ساخت log_config cfg;
	ساخت rev_info rev;
	ساخت تیاری_revision_opt opt;
	صحیح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, سابقہ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_سرخی = 1;
	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_تیاری_revisions_tweak;
	cmd_log_init(argc, argv, سابقہ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	واپس ret;
}

/* format-patch */

شمار cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

شمار thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

شمار cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_پیغام,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

شمار auto_بنیاد_setting {
	AUTO_بنیاد_NEVER,
	AUTO_بنیاد_ALWAYS,
	AUTO_بنیاد_WHEN_ABLE
};

ساخت format_config {
	ساخت log_config لوگارتھم;
	شمار thread_level thread;
	صحیح do_signoff;
	شمار auto_بنیاد_setting auto_بنیاد;
	حرف *بنیاد_commit;
	حرف *from;
	حرف *signature;
	حرف *signature_file;
	شمار cover_setting config_cover_letter;
	حرف *fmt_cover_letter_commit_list;
	حرف *config_output_directory;
	شمار cover_from_description cover_from_description_منوال;
	صحیح show_notes;
	ساخت display_notes_opt notes_opt;
	صحیح numbered_cmdline_opt;
	صحیح numbered;
	صحیح auto_نمبر;
	حرف *default_attach;
	ساخت string_list extra_hdr;
	ساخت string_list extra_مقصد;
	ساخت string_list extra_cc;
	صحیح keep_subject;
	صحیح subject_سابقہ;
	ساخت strbuf sprefix;
	حرف *fmt_patch_suffix;
};

ساکن خالی format_config_init(ساخت format_config *cfg)
{
	memset(cfg, 0, حجم(*cfg));
	log_config_init(&cfg->لوگارتھم);
	cfg->cover_from_description_منوال = COVER_FROM_پیغام;
	cfg->auto_نمبر = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_مقصد);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ساکن خالی format_config_release(ساخت format_config *cfg)
{
	log_config_release(&cfg->لوگارتھم);
	آزاد(cfg->بنیاد_commit);
	آزاد(cfg->from);
	آزاد(cfg->signature);
	آزاد(cfg->signature_file);
	آزاد(cfg->config_output_directory);
	آزاد(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_مقصد, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	آزاد(cfg->fmt_patch_suffix);
	آزاد(cfg->fmt_cover_letter_commit_list);
}

ساکن شمار cover_from_description parse_cover_from_description(ثابت حرف *arg)
{
	اگر (!arg || !strcmp(arg, "default"))
		واپس COVER_FROM_پیغام;
	ورنہ اگر (!strcmp(arg, "none"))
		واپس COVER_FROM_NONE;
	ورنہ اگر (!strcmp(arg, "message"))
		واپس COVER_FROM_پیغام;
	ورنہ اگر (!strcmp(arg, "subject"))
		واپس COVER_FROM_SUBJECT;
	ورنہ اگر (!strcmp(arg, "auto"))
		واپس COVER_FROM_AUTO;
	ورنہ
		die(_("%s: invalid cover from description mode"), arg);
}

ساکن خالی جوڑو_سرخی(ساخت format_config *cfg, ثابت حرف *قدر)
{
	ساخت string_list_شے *شے;
	صحیح len = لڑی_لمبائی(قدر);
	جب (len && قدر[len - 1] == '\n')
		len--;

	اگر (!strncasecmp(قدر, "to: ", 4)) {
		شے = string_list_append(&cfg->extra_مقصد, قدر + 4);
		len -= 4;
	} ورنہ اگر (!strncasecmp(قدر, "cc: ", 4)) {
		شے = string_list_append(&cfg->extra_cc, قدر + 4);
		len -= 4;
	} ورنہ {
		شے = string_list_append(&cfg->extra_hdr, قدر);
	}

	شے->string[len] = '\0';
}

ساکن صحیح git_format_config(ثابت حرف *var, ثابت حرف *قدر,
			     ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت format_config *cfg = cb;

	اگر (!strcmp(var, "format.headers")) {
		اگر (!قدر)
			die(_("format.headers without value"));
		جوڑو_سرخی(cfg, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		واپس git_config_string(&cfg->fmt_patch_suffix, var, قدر);
	}
	اگر (!strcmp(var, "format.to")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		string_list_append(&cfg->extra_مقصد, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "format.cc")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		واپس 0;
	}
	اگر (!strcmp(var, "format.numbered")) {
		اگر (قدر && !strcasecmp(قدر, "auto")) {
			cfg->auto_نمبر = 1;
			واپس 0;
		}
		cfg->numbered = git_config_bool(var, قدر);
		cfg->auto_نمبر = cfg->auto_نمبر && cfg->numbered;
		واپس 0;
	}
	اگر (!strcmp(var, "format.attach")) {
		اگر (قدر && *قدر) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(قدر);
		} ورنہ اگر (قدر && !*قدر) {
			FREE_AND_NULL(cfg->default_attach);
		} ورنہ {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_نسخہ_string);
		}
		واپس 0;
	}
	اگر (!strcmp(var, "format.thread")) {
		اگر (قدر && !strcasecmp(قدر, "deep")) {
			cfg->thread = THREAD_DEEP;
			واپس 0;
		}
		اگر (قدر && !strcasecmp(قدر, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			واپس 0;
		}
		cfg->thread = git_config_bool(var, قدر) ? THREAD_SHALLOW : THREAD_UNSET;
		واپس 0;
	}
	اگر (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		واپس git_config_string(&cfg->signature, var, قدر);
	}
	اگر (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		واپس git_config_pathname(&cfg->signature_file, var, قدر);
	}
	اگر (!strcmp(var, "format.coverletter")) {
		اگر (قدر && !strcasecmp(قدر, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			واپس 0;
		}
		cfg->config_cover_letter = git_config_bool(var, قدر) ? COVER_ON : COVER_OFF;
		واپس 0;
	}
	اگر (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		واپس git_config_string(&cfg->fmt_cover_letter_commit_list, var, قدر);
	}
	اگر (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		واپس git_config_string(&cfg->config_output_directory, var, قدر);
	}
	اگر (!strcmp(var, "format.useautobase")) {
		اگر (قدر && !strcasecmp(قدر, "whenAble")) {
			cfg->auto_بنیاد = AUTO_بنیاد_WHEN_ABLE;
			واپس 0;
		}
		cfg->auto_بنیاد = git_config_bool(var, قدر) ? AUTO_بنیاد_ALWAYS : AUTO_بنیاد_NEVER;
		واپس 0;
	}
	اگر (!strcmp(var, "format.from")) {
		صحیح b = git_parse_maybe_bool(قدر);
		FREE_AND_NULL(cfg->from);
		اگر (b < 0)
			cfg->from = xstrdup(قدر);
		ورنہ اگر (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		واپس 0;
	}
	اگر (!strcmp(var, "format.forceinbodyfrom")) {
		force_اندر_body_from = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "format.notes")) {
		صحیح b = git_parse_maybe_bool(قدر);
		اگر (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, قدر);
		ورنہ اگر (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ورنہ
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		واپس 0;
	}
	اگر (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_منوال = parse_cover_from_description(قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "format.noprefix")) {
		format_no_سابقہ = git_parse_maybe_bool(قدر);
		اگر (format_no_سابقہ < 0) {
			صحیح حالت_2 = die_پیغام(
				_("bad boolean config value '%s' for '%s'"),
				قدر, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			باہر_نکلو(حالت_2);
		}
		واپس 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	اگر (!strcmp(var, "diff.noprefix"))
		واپس 0;

	واپس git_log_config(var, قدر, ctx, &cfg->لوگارتھم);
}

ساکن ثابت حرف *output_directory = تہی;
ساکن صحیح outdir_انحراف;

ساکن صحیح open_اگلا_file(ساخت commit *commit, ثابت حرف *subject,
			 ساخت rev_info *rev, صحیح quiet)
{
	ساخت strbuf filename = STRBUF_INIT;

	اگر (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	اگر (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	ورنہ اگر (commit)
		fmt_output_commit(&filename, commit, rev);
	ورنہ
		fmt_output_subject(&filename, subject, rev);

	اگر (!quiet)
		لکھو("%s\n", filename.buf + outdir_انحراف);

	اگر (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		واپس -1;
	}

	strbuf_release(&filename);
	واپس 0;
}

ساکن خالی get_patch_ids(ساخت rev_info *rev, ساخت patch_ids *ids)
{
	ساخت rev_info check_rev;
	ساخت commit *commit, *c1, *c2;
	ساخت object *o1, *o2;
	غیرعلامتی flags1, flags2;

	اگر (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].شے;
	o2 = rev->pending.objects[1].شے;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	اگر ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->سابقہ);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	جوڑو_pending_object(&check_rev, o1, "o1");
	جوڑو_pending_object(&check_rev, o2, "o2");
	اگر (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	جب ((commit = get_revision(&check_rev)) != تہی) {
		جوڑو_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, نظر_آیا_2 | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, نظر_آیا_2 | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ساکن خالی gen_پیغام_id(ساخت rev_info *info, ثابت حرف *بنیاد)
{
	ساخت strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", بنیاد,
		    (timestamp_t) وقت(تہی),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->پیغام_id = strbuf_detach(&buf, تہی);
}

ساکن خالی print_signature(ثابت حرف *signature, FILE *file)
{
	اگر (!signature || !*signature)
		واپس;

	fprintf(file, "-- \n%s", signature);
	اگر (signature[لڑی_لمبائی(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ساکن حرف *ڈھونڈو_branch_name(ساخت rev_info *rev)
{
	ساخت object_id branch_oid;
	ثابت ساخت object_id *tip_oid;
	ثابت حرف *ref, *v;
	حرف *بھرا_ref, *branch = تہی;
	صحیح interesting_ملا = 0;
	سائز_t idx;

	برائے (سائز_t i = 0; i < rev->cmdline.nr; i++) {
		اگر (rev->cmdline.rev[i].flags & UNINTERESTING)
			جاری;
		اگر (interesting_ملا)
			واپس تہی;
		interesting_ملا = 1;
		idx = i;
	}
	اگر (!interesting_ملا)
		واپس تہی;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].شے->oid;
	اگر (repo_dwim_ref(the_repository, ref, لڑی_لمبائی(ref), &branch_oid,
			  &بھرا_ref, 0) &&
	    skip_سابقہ(بھرا_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	آزاد(بھرا_ref);
	واپس branch;
}

ساکن خالی show_diffstat(ساخت rev_info *rev,
			  ساخت commit *origin, ساخت commit *سر)
{
	ساخت diff_options opts;

	حافظہ_نقل(&opts, &rev->diffopt, حجم(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_تیاری_مکمل(&opts);

	diff_درخت_oid(get_commit_درخت_oid(origin),
		      get_commit_درخت_oid(سر),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ساکن خالی read_desc_file(ساخت strbuf *buf, ثابت حرف *desc_file)
{
	اگر (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ساکن خالی prepare_cover_متن(ساخت pretty_print_context *pp,
			       ثابت حرف *description_file,
			       ثابت حرف *branch_name,
			       ساخت strbuf *sb,
			       ثابت حرف *encoding,
			       صحیح need_8bit_cte,
			       ثابت ساخت format_config *cfg)
{
	ثابت حرف *subject = "*** SUBJECT HERE ***";
	ثابت حرف *body = "*** BLURB HERE ***";
	ساخت strbuf description_sb = STRBUF_INIT;
	ساخت strbuf subject_sb = STRBUF_INIT;

	اگر (cfg->cover_from_description_منوال == COVER_FROM_NONE)
		جاؤ do_pp;

	اگر (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	ورنہ اگر (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	اگر (!description_sb.len)
		جاؤ do_pp;

	اگر (cfg->cover_from_description_منوال == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_منوال == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	اگر (cfg->cover_from_description_منوال == COVER_FROM_پیغام ||
	    (cfg->cover_from_description_منوال == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	ورنہ
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ساخت rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ساخت string_list notes;
};

ساکن صحیح rdiff_notes_cb(ثابت ساخت option *option,
			  ثابت حرف *arg,
			  صحیح unset)
{
	ساخت option opt = *option;
	ساخت rdiff_notes *rdiff_notes = option->قدر;

	rdiff_notes->override = 1;
	opt.قدر = &rdiff_notes->notes;
	واپس parse_opt_string_list(&opt, arg, unset);
}

ساکن صحیح get_notes_refs(ساخت string_list_شے *شے, خالی *arg)
{
	strvec_pushf(arg, "--notes=%s", شے->string);
	واپس 0;
}

ساکن خالی get_notes_دلائل(ساخت rdiff_notes *rdiff_notes,
			   ساخت rev_info *rev)
{
	اگر (rdiff_notes->override) {
		اگر (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		ورنہ
			strvec_ڈالو(&rev->rdiff_log_arg, "--no-notes");
	} ورنہ اگر (!rev->show_notes) {
		strvec_ڈالو(&rev->rdiff_log_arg, "--no-notes");
	} ورنہ اگر (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ڈالو(&rev->rdiff_log_arg, "--notes");
	} ورنہ {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ساکن خالی generate_shortlog_cover_letter(ساخت shortlog *لوگارتھم,
					   ساخت rev_info *rev,
					   ساخت commit **list,
					   صحیح nr)
{
	shortlog_init(لوگارتھم);
	لوگارتھم->wrap_سطریں = 1;
	لوگارتھم->wrap = MAIL_DEFAULT_WRAP;
	لوگارتھم->in1 = 2;
	لوگارتھم->in2 = 4;
	لوگارتھم->file = rev->diffopt.file;
	لوگارتھم->groups = SHORTLOG_گروہ_AUTHOR;
	shortlog_اختتام_تیاری(لوگارتھم);
	برائے (صحیح i = 0; i < nr; i++)
		shortlog_جوڑو_commit(لوگارتھم, list[i]);

	shortlog_output(لوگارتھم);
}

ساکن خالی generate_commit_list_cover(FILE *cover_file, ثابت حرف *format,
				       ساخت commit **list, صحیح n)
{
	ساخت strbuf commit_سطر = STRBUF_INIT;
	ساخت pretty_print_context ctx = {0};
	ساخت rev_info rev = REV_INFO_INIT;

	rev.کل = n;
	ctx.rev = &rev;
	برائے (صحیح i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_پیغام(the_repository, list[n - i], format,
				&commit_سطر, &ctx);
		fprintf(cover_file, "%s\n", commit_سطر.buf);
		strbuf_reset(&commit_سطر);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_سطر);
}

ساکن خالی make_cover_letter(ساخت rev_info *rev, صحیح use_separate_file,
			      ساخت commit *origin,
			      صحیح nr, ساخت commit **list,
			      ثابت حرف *description_file,
			      ثابت حرف *branch_name,
			      صحیح quiet,
			      ثابت ساخت format_config *cfg,
			      ثابت حرف *format)
{
	ثابت حرف *from;
	ساخت shortlog لوگارتھم;
	ساخت strbuf sb = STRBUF_INIT;
	صحیح i;
	ثابت حرف *encoding = "UTF-8";
	صحیح need_8bit_cte = 0;
	ساخت pretty_print_context pp = {0};
	ساخت commit *سر = list[0];
	حرف *مقصد_free = تہی;

	اگر (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	اگر (use_separate_file &&
	    open_اگلا_file(تہی, rev->numbered_files ? تہی : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_سرخیاں(rev, سر, &pp.after_subject, &need_8bit_cte, 0);

	برائے (i = 0; !need_8bit_cte && i < nr; i++) {
		ثابت حرف *buf = repo_get_commit_بفر(the_repository,
							 list[i], تہی);
		اگر (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_بفر(the_repository, list[i], buf);
	}

	اگر (!branch_name)
		branch_name = مقصد_free = ڈھونڈو_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_منوال.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_سرخیاں = rev->encode_email_سرخیاں;
	pp_user_info(&pp, تہی, &sb, from, encoding);
	prepare_cover_متن(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	آزاد(مقصد_free);
	آزاد(pp.after_subject);
	strbuf_release(&sb);

	اگر (skip_سابقہ(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ورنہ اگر (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&لوگارتھم, rev, list, nr);
	ورنہ اگر (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	ورنہ اگر (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ورنہ
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	اگر (origin)
		show_diffstat(rev, origin, سر);

	اگر (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	اگر (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ساخت diff_options opts;
		ساخت range_diff_options range_diff_opts = {
			.creation_ضربی_عدد = rev->creation_ضربی_عدد,
			.dual_color = 1,
			.max_حافظہ = RANGE_DIFF_MAX_حافظہ_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_تیاری(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_تیاری_مکمل(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

ساکن حرف *clean_پیغام_id(ثابت حرف *پیغام_id_2)
{
	حرف ch;
	ثابت حرف *a, *z, *m;

	m = پیغام_id_2;
	جب ((ch = *m) && (خلا_ہے(ch) || (ch == '<')))
		m++;
	a = m;
	z = تہی;
	جب ((ch = *m)) {
		اگر (!خلا_ہے(ch) && (ch != '>'))
			z = m;
		m++;
	}
	اگر (!z)
		die(_("insane in-reply-to: %s"), پیغام_id_2);
	اگر (++z == m)
		واپس xstrdup(a);
	واپس xmemdupz(a, z - a);
}

ساکن ثابت حرف *سیٹ_outdir(ثابت حرف *سابقہ, ثابت حرف *output_directory)
{
	اگر (output_directory && is_absolute_راستہ(output_directory))
		واپس output_directory;

	اگر (!سابقہ || !*سابقہ) {
		اگر (output_directory)
			واپس output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_انحراف = 2;
		واپس "./";
	}

	outdir_انحراف = لڑی_لمبائی(سابقہ);
	اگر (!output_directory)
		واپس سابقہ;

	واپس سابقہ_filename(سابقہ, output_directory);
}

ساکن ثابت حرف * ثابت builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	تہی
};

ساخت keep_callback_ڈیٹا {
	ساخت format_config *cfg;
	ساخت rev_info *revs;
};

ساکن صحیح keep_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت keep_callback_ڈیٹا *ڈیٹا = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ڈیٹا->revs->کل = -1;
	ڈیٹا->cfg->keep_subject = 1;
	واپس 0;
}

ساکن صحیح subject_سابقہ_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			    صحیح unset)
{
	ساخت format_config *cfg = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_سابقہ = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	واپس 0;
}

ساکن صحیح rfc_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			صحیح unset)
{
	ثابت حرف **rfc = opt->قدر;

	*rfc = opt->قدر;
	اگر (unset)
		*rfc = تہی;
	ورنہ
		*rfc = arg ? arg : "RFC";
	واپس 0;
}

ساکن صحیح numbered_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			     صحیح unset)
{
	ساخت format_config *cfg = opt->قدر;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	اگر (unset)
		cfg->auto_نمبر =  0;
	واپس 0;
}

ساکن صحیح no_numbered_callback(ثابت ساخت option *opt, ثابت حرف *arg,
				صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	واپس numbered_callback(opt, arg, 1);
}

ساکن صحیح output_directory_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			      صحیح unset)
{
	ثابت حرف **dir = (ثابت حرف **)opt->قدر;
	BUG_ON_OPT_NEG(unset);
	اگر (*dir)
		die(_("two output directories?"));
	*dir = arg;
	واپس 0;
}

ساکن صحیح thread_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت format_config *cfg = opt->قدر;

	اگر (unset)
		cfg->thread = THREAD_UNSET;
	ورنہ اگر (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	ورنہ اگر (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	ورنہ
		واپس 1;
	واپس 0;
}

ساکن صحیح attach_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت rev_info *rev = (ساخت rev_info *)opt->قدر;
	اگر (unset)
		rev->mime_boundary = تہی;
	ورنہ اگر (arg)
		rev->mime_boundary = arg;
	ورنہ
		rev->mime_boundary = git_نسخہ_string;
	rev->no_inline = unset ? 0 : 1;
	واپس 0;
}

ساکن صحیح inline_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت rev_info *rev = (ساخت rev_info *)opt->قدر;
	اگر (unset)
		rev->mime_boundary = تہی;
	ورنہ اگر (arg)
		rev->mime_boundary = arg;
	ورنہ
		rev->mime_boundary = git_نسخہ_string;
	rev->no_inline = 0;
	واپس 0;
}

ساکن صحیح سرخی_callback(ثابت ساخت option *opt, ثابت حرف *arg,
			   صحیح unset)
{
	ساخت format_config *cfg = opt->قدر;

	اگر (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_مقصد, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} ورنہ {
		جوڑو_سرخی(cfg, arg);
	}
	واپس 0;
}

ساکن صحیح from_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	حرف **from = opt->قدر;

	آزاد(*from);

	اگر (unset)
		*from = تہی;
	ورنہ اگر (arg)
		*from = xstrdup(arg);
	ورنہ
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	واپس 0;
}

ساکن صحیح بنیاد_callback(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت format_config *cfg = opt->قدر;

	اگر (unset) {
		cfg->auto_بنیاد = AUTO_بنیاد_NEVER;
		FREE_AND_NULL(cfg->بنیاد_commit);
	} ورنہ اگر (!strcmp(arg, "auto")) {
		cfg->auto_بنیاد = AUTO_بنیاد_ALWAYS;
		FREE_AND_NULL(cfg->بنیاد_commit);
	} ورنہ {
		cfg->auto_بنیاد = AUTO_بنیاد_NEVER;
		cfg->بنیاد_commit = xstrdup(arg);
	}
	واپس 0;
}

ساخت بنیاد_درخت_info {
	ساخت object_id بنیاد_commit;
	صحیح nr_patch_id, alloc_patch_id;
	ساخت object_id *patch_id;
};

ساکن ساخت commit *get_بنیاد_commit(ثابت ساخت format_config *cfg,
				      ساخت commit **list,
				      سائز_t کل)
{
	ساخت commit *بنیاد = تہی;
	ساخت commit **rev;
	صحیح auto_select, die_on_failure, ret;
	سائز_t i = 0, rev_nr = 0;

	انتخاب (cfg->auto_بنیاد) {
	حالت AUTO_بنیاد_NEVER:
		اگر (cfg->بنیاد_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} ورنہ {
			/* no base information is requested */
			واپس تہی;
		}
		توڑو;
	حالت AUTO_بنیاد_ALWAYS:
	حالت AUTO_بنیاد_WHEN_ABLE:
		اگر (cfg->بنیاد_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} ورنہ {
			auto_select = 1;
			die_on_failure = cfg->auto_بنیاد == AUTO_بنیاد_ALWAYS;
		}
		توڑو;
	طےشدہ:
		BUG("unexpected automatic base selection method");
	}

	اگر (!auto_select) {
		بنیاد = lookup_commit_reference_by_name(cfg->بنیاد_commit);
		اگر (!بنیاد)
			die(_("unknown commit %s"), cfg->بنیاد_commit);
	} ورنہ {
		ساخت branch *curr_branch = branch_get(تہی);
		ثابت حرف *upstream = branch_get_upstream(curr_branch, تہی);
		اگر (upstream) {
			ساخت commit_list *بنیاد_list = تہی;
			ساخت commit *commit;
			ساخت object_id oid;

			اگر (repo_get_oid(the_repository, upstream, &oid)) {
				اگر (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				ورنہ
					واپس تہی;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			اگر (repo_get_ملاؤ_bases_many(the_repository,
						      commit, کل,
						      list,
						      &بنیاد_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !بنیاد_list || بنیاد_list->اگلا) {
				اگر (die_on_failure) {
					die(_("could not find exact merge base"));
				} ورنہ {
					commit_list_free(بنیاد_list);
					واپس تہی;
				}
			}
			بنیاد = بنیاد_list->شے;
			commit_list_free(بنیاد_list);
		} ورنہ {
			اگر (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			ورنہ
				واپس تہی;
		}
	}

	ALLOC_ARRAY(rev, کل);
	برائے (i = 0; i < کل; i++)
		rev[i] = list[i];

	rev_nr = کل;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	جب (rev_nr > 1) {
		برائے (i = 0; i < rev_nr / 2; i++) {
			ساخت commit_list *ملاؤ_بنیاد = تہی;
			اگر (repo_get_ملاؤ_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &ملاؤ_بنیاد) < 0 ||
			    !ملاؤ_بنیاد || ملاؤ_بنیاد->اگلا) {
				اگر (die_on_failure) {
					die(_("failed to find exact merge base"));
				} ورنہ {
					commit_list_free(ملاؤ_بنیاد);
					آزاد(rev);
					واپس تہی;
				}
			}

			rev[i] = ملاؤ_بنیاد->شے;
			commit_list_free(ملاؤ_بنیاد);
		}

		اگر (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_باری_اوپر(rev_nr, 2);
	}

	ret = repo_اندر_ملاؤ_bases(the_repository, بنیاد, rev[0]);
	اگر (ret < 0)
		باہر_نکلو(128);
	اگر (!ret) {
		اگر (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} ورنہ {
			آزاد(rev);
			واپس تہی;
		}
	}

	برائے (i = 0; i < کل; i++) {
		اگر (بنیاد == list[i]) {
			اگر (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} ورنہ {
				آزاد(rev);
				واپس تہی;
			}
		}
	}

	آزاد(rev);
	واپس بنیاد;
}

define_commit_slab(commit_بنیاد, صحیح);

ساکن خالی prepare_bases(ساخت بنیاد_درخت_info *bases,
			  ساخت commit *بنیاد,
			  ساخت commit **list,
			  سائز_t کل)
{
	ساخت commit *commit;
	ساخت rev_info revs;
	ساخت diff_options diffopt;
	ساخت commit_بنیاد commit_بنیاد;

	اگر (!بنیاد)
		واپس;

	init_commit_بنیاد(&commit_بنیاد);
	repo_diff_تیاری(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_تیاری_مکمل(&diffopt);

	oidcpy(&bases->بنیاد_commit, &بنیاد->object.oid);

	repo_init_revisions(the_repository, &revs, تہی);
	revs.max_parents = 1;
	revs.topo_ترتیب = 1;
	برائے (سائز_t i = 0; i < کل; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		جوڑو_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_بنیاد_at(&commit_بنیاد, list[i]) = 1;
	}
	بنیاد->object.flags |= UNINTERESTING;
	جوڑو_pending_object(&revs, &بنیاد->object, "base");

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	جب ((commit = get_revision(&revs)) != تہی) {
		ساخت object_id oid;
		ساخت object_id *patch_id;
		اگر (*commit_بنیاد_at(&commit_بنیاد, commit))
			جاری;
		اگر (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_بنیاد(&commit_بنیاد);
	release_revisions(&revs);
}

ساکن خالی print_bases(ساخت بنیاد_درخت_info *bases, FILE *file)
{
	صحیح i;

	/* Only do this once, either for the cover or for the first one */
	اگر (is_null_oid(&bases->بنیاد_commit))
		واپس;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_مقصد_hex(&bases->بنیاد_commit));

	/* Show the prerequisite patches */
	برائے (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_مقصد_hex(&bases->patch_id[i]));

	آزاد(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->بنیاد_commit, the_repository->hash_algo);
}

ساکن ثابت حرف *diff_title(ساخت strbuf *sb,
			      ثابت حرف *reroll_گنتی,
			      ثابت حرف *generic,
			      ثابت حرف *rerolled)
{
	صحیح v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	اگر (reroll_گنتی && !strtol_i(reroll_گنتی, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	ورنہ
		strbuf_addstr(sb, generic);
	واپس sb->buf;
}

ساکن خالی infer_range_diff_ranges(ساخت strbuf *r1,
				    ساخت strbuf *r2,
				    ثابت حرف *پچھلا_2,
				    ساخت commit *origin,
				    ساخت commit *سر)
{
	ثابت حرف *سر_oid = oid_مقصد_hex(&سر->object.oid);
	صحیح پچھلا_is_range = is_range_diff_range(پچھلا_2);

	اگر (پچھلا_is_range)
		strbuf_addstr(r1, پچھلا_2);
	ورنہ
		strbuf_addf(r1, "%s..%s", سر_oid, پچھلا_2);

	اگر (origin)
		strbuf_addf(r2, "%s..%s", oid_مقصد_hex(&origin->object.oid), سر_oid);
	ورنہ اگر (پچھلا_is_range)
		die(_("failed to infer range-diff origin of current series"));
	ورنہ {
		warning(_("using '%s' as range-diff origin of current series"), پچھلا_2);
		strbuf_addf(r2, "%s..%s", پچھلا_2, سر_oid);
	}
}

صحیح cmd_format_patch(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo غیر_مستعمل)
{
	ساخت format_config cfg;
	ساخت commit *commit;
	ساخت commit_ڈھیر list = COMMIT_ڈھیر_INIT;
	ساخت rev_info rev;
	حرف *مقصد_free = تہی;
	ساخت تیاری_revision_opt s_r_opt;
	سائز_t کل, i;
	صحیح use_stdout = 0;
	صحیح آغاز_نمبر = -1;
	صحیح just_numbers = 0;
	صحیح ignore_if_اندر_upstream = 0;
	صحیح cover_letter = -1;
	ثابت حرف *cover_letter_fmt = تہی;
	صحیح boundary_گنتی = 0;
	صحیح no_ثنائی_diff = 0;
	صحیح zero_commit = 0;
	ساخت commit *origin = تہی;
	ثابت حرف *اندر_جواب_مقصد = تہی;
	ساخت patch_ids ids;
	ساخت strbuf buf = STRBUF_INIT;
	صحیح use_patch_format = 0;
	صحیح quiet = 0;
	ثابت حرف *reroll_گنتی = تہی;
	حرف *cover_from_description_arg = تہی;
	حرف *description_file = تہی;
	حرف *branch_name = تہی;
	ساخت بنیاد_درخت_info bases;
	ساخت commit *بنیاد;
	صحیح show_پیش_رفت = 0;
	ساخت پیش_رفت *پیش_رفت = تہی;
	ساخت oid_array idiff_پچھلا = OID_ARRAY_INIT;
	ساخت strbuf idiff_title = STRBUF_INIT;
	ثابت حرف *rdiff_پچھلا = تہی;
	ساخت strbuf rdiff1 = STRBUF_INIT;
	ساخت strbuf rdiff2 = STRBUF_INIT;
	ساخت strbuf rdiff_title = STRBUF_INIT;
	ساخت rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ثابت حرف *rfc = تہی;
	صحیح creation_ضربی_عدد = -1;
	ثابت حرف *signature = git_نسخہ_string;
	حرف *signature_مقصد_free = تہی;
	حرف *signature_file_arg = تہی;
	ساخت keep_callback_ڈیٹا keep_callback_ڈیٹا = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ثابت حرف *fmt_patch_suffix = تہی;

	ثابت ساخت option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, تہی,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, تہی,
			    N_("use [PATCH] even with multiple patches"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, no_numbered_callback),
		OPT_BOOL('s', "signoff", &cfg.do_signoff, N_("add a Signed-off-by trailer")),
		OPT_BOOL(0, "stdout", &use_stdout,
			    N_("print patches to standard out")),
		OPT_BOOL(0, "cover-letter", &cover_letter,
			    N_("generate a cover letter")),
		OPT_STRING(0, "commit-list-format", &cover_letter_fmt, N_("format-spec"),
			    N_("format spec used for the commit list in the cover letter")),
		OPT_BOOL(0, "numbered-files", &just_numbers,
			    N_("use simple number sequence for output file names")),
		OPT_STRING(0, "suffix", &fmt_patch_suffix, N_("sfx"),
			    N_("use <sfx> instead of '.patch'")),
		OPT_INTEGER(0, "start-number", &آغاز_نمبر,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_گنتی, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.لوگارتھم.fmt_patch_name_max,
			    N_("max length of output filename")),
		OPT_CALLBACK_F(0, "rfc", &rfc, N_("rfc"),
			       N_("add <rfc> (default 'RFC') before 'PATCH'"),
			       PARSE_OPT_OPTARG, rfc_callback),
		OPT_STRING(0, "cover-from-description", &cover_from_description_arg,
			    N_("cover-from-description-mode"),
			    N_("generate parts of a cover letter based on a branch's description")),
		OPT_FILENAME(0, "description-file", &description_file,
			     N_("use branch description from file")),
		OPT_CALLBACK_F(0, "subject-prefix", &cfg, N_("prefix"),
			    N_("use [<prefix>] instead of [PATCH]"),
			    PARSE_OPT_NONEG, subject_سابقہ_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_ڈیٹا, تہی,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ثنائی_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_اندر_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_سیٹ_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_گروہ(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), سرخی_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_مقصد, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &اندر_جواب_مقصد, N_("message-id"),
			    N_("make first mail a reply to <message-id>")),
		OPT_CALLBACK_F(0, "attach", &rev, N_("boundary"),
			    N_("attach the patch"), PARSE_OPT_OPTARG,
			    attach_callback),
		OPT_CALLBACK_F(0, "inline", &rev, N_("boundary"),
			    N_("inline the patch"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    inline_callback),
		OPT_CALLBACK_F(0, "thread", &cfg, N_("style"),
			    N_("enable message threading, styles: shallow, deep"),
			    PARSE_OPT_OPTARG, thread_callback),
		OPT_STRING(0, "signature", &signature, N_("signature"),
			    N_("add a signature")),
		OPT_CALLBACK_F(0, "base", &cfg, N_("base-commit"),
			       N_("add prerequisite tree info to the patch series"),
			       0, بنیاد_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_پیش_رفت,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_پچھلا, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_پچھلا, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_ضربی_عدد,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_اندر_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_انتہا()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, سابقہ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	حافظہ_نقل(&rev.notes_opt, &cfg.notes_opt, حجم(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_سرخیاں = cfg.لوگارتھم.default_encode_email_سرخیاں;
	rev.expand_tabs_اندر_log_default = 0;
	rev.verbose_سرخی = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, حجم(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.لوگارتھم.fmt_patch_subject_سابقہ);
	اگر (format_no_سابقہ)
		diff_سیٹ_noprefix(&rev.diffopt);

	اگر (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, سابقہ, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_اندر_body_from = force_اندر_body_from;

	اگر (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	اگر (cfg.لوگارتھم.fmt_patch_name_max <= cast_سائز_t_مقصد_int(لڑی_لمبائی("0000-") + لڑی_لمبائی(fmt_patch_suffix)))
		cfg.لوگارتھم.fmt_patch_name_max = لڑی_لمبائی("0000-") + لڑی_لمبائی(fmt_patch_suffix);

	اگر (cover_from_description_arg)
		cfg.cover_from_description_منوال = parse_cover_from_description(cover_from_description_arg);

	اگر (rfc && rfc[0]) {
		cfg.subject_سابقہ = 1;
		اگر (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		ورنہ
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	اگر (reroll_گنتی) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_گنتی);
		rev.reroll_گنتی = reroll_گنتی;
	}

	rev.subject_سابقہ = cfg.sprefix.buf;

	برائے (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.اشیاء[i].string);
		strbuf_addch(&buf, '\n');
	}

	اگر (cfg.extra_مقصد.nr)
		strbuf_addstr(&buf, "To: ");
	برائے (i = 0; i < cfg.extra_مقصد.nr; i++) {
		اگر (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_مقصد.اشیاء[i].string);
		اگر (i + 1 < cfg.extra_مقصد.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	اگر (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	برائے (i = 0; i < cfg.extra_cc.nr; i++) {
		اگر (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.اشیاء[i].string);
		اگر (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_سرخیاں = مقصد_free = strbuf_detach(&buf, تہی);

	اگر (cfg.from) {
		اگر (بانٹو_ident_سطر(&rev.from_ident, cfg.from, لڑی_لمبائی(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	اگر (آغاز_نمبر < 0)
		آغاز_نمبر = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	اگر (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	اگر (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	اگر (cfg.keep_subject && cfg.subject_سابقہ)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = تیاری_revisions(argc, argv, &rev, &s_r_opt);
	اگر (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	اگر (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	اگر (rev.diffopt.output_format & DIFF_FORMAT_NAME_حالت)
		die(_("--name-status does not make sense"));
	اگر (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	اگر (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	اگر (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	اگر (!rev.diffopt.stat_چوڑائی)
		rev.diffopt.stat_چوڑائی = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_سرخی = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.لوگارتھم.fmt_patch_name_max;

	اگر (!rev.diffopt.flags.متن && !no_ثنائی_diff)
		rev.diffopt.flags.ثنائی = 1;

	اگر (rev.show_notes)
		بھرو_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	اگر (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	اگر (use_stdout) {
		تیاری_pager(the_repository);
	} ورنہ اگر (!rev.diffopt.close_file) {
		صحیح saved;

		اگر (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = سیٹ_outdir(سابقہ, output_directory);

		اگر (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_سیٹ_shared_repository(the_repository, 0);
		انتخاب (محفوظ_create_leading_directories_const(the_repository, output_directory)) {
		حالت SCLD_OK:
		حالت SCLD_EXISTS:
			توڑو;
		طےشدہ:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_سیٹ_shared_repository(the_repository, saved);
		اگر (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	اگر (rev.pending.nr == 1) {
		صحیح check_سر = 0;

		اگر (rev.max_گنتی < 0 && !rev.show_جڑ_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].شے->flags |= UNINTERESTING;
			جوڑو_سر_مقصد_pending(&rev);
			check_سر = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		اگر (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_سر = 1;

		اگر (check_سر) {
			ثابت حرف *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      تہی, تہی);
			اگر (ref && skip_سابقہ(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			ورنہ
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_جڑ_diff = 1;

	اگر (ignore_if_اندر_upstream) {
		/* Don't say anything if head and upstream are the same. */
		اگر (rev.pending.nr == 2) {
			ساخت object_array_اندراج *o = rev.pending.objects;
			اگر (oideq(&o[0].شے->oid, &o[1].شے->oid))
				جاؤ مکمل;
		}
		get_patch_ids(&rev, &ids);
	}

	اگر (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	جب ((commit = get_revision(&rev)) != تہی) {
		اگر (commit->object.flags & BOUNDARY) {
			boundary_گنتی++;
			origin = (boundary_گنتی == 1) ? commit : تہی;
			جاری;
		}

		اگر (ignore_if_اندر_upstream && has_commit_patch_id(commit, &ids))
			جاری;

		commit_ڈھیر_ڈالو(&list, commit);
	}
	اگر (!list.nr)
		/* nothing to do */
		جاؤ مکمل;
	کل = list.nr;

	اگر (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		اگر (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} ورنہ اگر (cover_letter == -1) {
		cover_letter = 1;
	}

	اگر (cover_letter == -1) {
		اگر (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (کل > 1);
		ورنہ اگر ((idiff_پچھلا.nr || rdiff_پچھلا) && (کل > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		ورنہ
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	اگر (!cfg.keep_subject && cfg.auto_نمبر && (کل > 1 || cover_letter))
		cfg.numbered = 1;
	اگر (cfg.numbered)
		rev.کل = کل + آغاز_نمبر - 1;

	اگر (idiff_پچھلا.nr) {
		اگر (!cover_letter && کل != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_پچھلا.oid[idiff_پچھلا.nr - 1];
		rev.idiff_oid2 = get_commit_درخت_oid(list.اشیاء[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_گنتی,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	اگر (creation_ضربی_عدد < 0)
		creation_ضربی_عدد = CREATION_ضربی_عدد_FOR_THE_SAME_SERIES;
	ورنہ اگر (!rdiff_پچھلا)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	اگر (rdiff_پچھلا) {
		اگر (!cover_letter && کل != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_پچھلا,
					origin, list.اشیاء[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_ضربی_عدد = creation_ضربی_عدد;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_گنتی,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_دلائل(&rdiff_notes, &rev);
	}

	/*
	 * The order of precedence is:
	 *
	 *   1. The `--signature` and `--no-signature` options.
	 *   2. The `--signature-file` option.
	 *   3. The `format.signature` config.
	 *   4. The `format.signatureFile` config.
	 *   5. Default `git_version_string`.
	 */
	اگر (!signature) {
		; /* --no-signature inhibits all signatures */
	} ورنہ اگر (signature && signature != git_نسخہ_string) {
		; /* non-default signature already set */
	} ورنہ اگر (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ساخت strbuf buf = STRBUF_INIT;
		ثابت حرف *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		اگر (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_مقصد_free = strbuf_detach(&buf, تہی);
	} ورنہ اگر (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, حجم(bases));
	بنیاد = get_بنیاد_commit(&cfg, list.اشیاء, list.nr);
	اگر (بنیاد) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, بنیاد, list.اشیاء, list.nr);
	}

	اگر (اندر_جواب_مقصد || cfg.thread || cover_letter) {
		rev.ref_پیغام_ids = xmalloc(حجم(*rev.ref_پیغام_ids));
		string_list_init_dup(rev.ref_پیغام_ids);
	}
	اگر (اندر_جواب_مقصد) {
		حرف *msgid = clean_پیغام_id(اندر_جواب_مقصد);
		string_list_append_nodup(rev.ref_پیغام_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	اگر (cover_letter) {
		اگر (cfg.thread)
			gen_پیغام_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.اشیاء,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		کل++;
		آغاز_نمبر--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = تہی;
		rev.rdiff1 = تہی;
	}
	rev.جوڑو_signoff = cfg.do_signoff;

	اگر (show_پیش_رفت)
		پیش_رفت = آغاز_delayed_پیش_رفت(the_repository,
						  _("Generating patches"), کل);
	جب (list.nr) {
		سائز_t idx = list.nr - 1;
		صحیح shown;

		display_پیش_رفت(پیش_رفت, کل - idx);
		commit = commit_ڈھیر_نکالو(&list);
		rev.nr = کل - idx + (آغاز_نمبر - 1);

		/* Make the second and subsequent mails replies to the first */
		اگر (cfg.thread) {
			/* Have we already had a message ID? */
			اگر (rev.پیغام_id) {
				/*
				 * For deep threading: make every mail
				 * a reply to the previous one, no
				 * matter what other options are set.
				 *
				 * For shallow threading:
				 *
				 * Without --cover-letter and
				 * --in-reply-to, make every mail a
				 * reply to the one before.
				 *
				 * With --in-reply-to but no
				 * --cover-letter, make every mail a
				 * reply to the <reply-to>.
				 *
				 * With --cover-letter, make every
				 * mail but the cover letter a reply
				 * to the cover letter.  The cover
				 * letter is a reply to the
				 * --in-reply-to, if specified.
				 */
				اگر (cfg.thread == THREAD_SHALLOW
				    && rev.ref_پیغام_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					آزاد(rev.پیغام_id);
				ورنہ
					string_list_append_nodup(rev.ref_پیغام_ids,
								 rev.پیغام_id);
			}
			gen_پیغام_id(&rev, oid_مقصد_hex(&commit->object.oid));
		}

		اگر (output_directory &&
		    open_اگلا_file(rev.numbered_files ? تہی : commit, تہی, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_درخت_commit(&rev, commit);
		free_commit_بفر(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		اگر (output_directory)
			rev.shown_one = 0;
		اگر (shown) {
			print_bases(&bases, rev.diffopt.file);
			اگر (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			ورنہ
				print_signature(signature, rev.diffopt.file);
		}
		اگر (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = تہی;
		}
	}
	stop_پیش_رفت(&پیش_رفت);
	commit_ڈھیر_clear(&list);
	اگر (ignore_if_اندر_upstream)
		free_patch_ids(&ids);

مکمل:
	oid_array_clear(&idiff_پچھلا);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	آزاد(description_file);
	آزاد(signature_file_arg);
	آزاد(signature_مقصد_free);
	آزاد(branch_name);
	آزاد(مقصد_free);
	آزاد(rev.پیغام_id);
	اگر (rev.ref_پیغام_ids)
		string_list_clear(rev.ref_پیغام_ids, 0);
	آزاد(rev.ref_پیغام_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	واپس 0;
}

ساکن صحیح جوڑو_pending_commit(ثابت حرف *arg, ساخت rev_info *revs, صحیح flags)
{
	ساخت object_id oid;
	اگر (repo_get_oid(the_repository, arg, &oid) == 0) {
		ساخت commit *commit = lookup_commit_reference(the_repository,
								&oid);
		اگر (commit) {
			commit->object.flags |= flags;
			جوڑو_pending_object(revs, &commit->object, arg);
			واپس 0;
		}
	}
	واپس -1;
}

ساکن ثابت حرف * ثابت cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	تہی
};

ساکن خالی print_commit(حرف sign, ساخت commit *commit, صحیح verbose,
			 صحیح abbrev, FILE *file)
{
	اگر (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} ورنہ {
		ساخت strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		fprintf(file, "%c %s %s\n", sign,
		       repo_ڈھونڈو_unique_abbrev(the_repository, &commit->object.oid, abbrev),
		       buf.buf);
		strbuf_release(&buf);
	}
}

/*
 * Enumerate blob OIDs from a single commit's diff, inserting them into blobs.
 * Skips files whose userdiff driver explicitly declares binary status
 * (drv->binary > 0), since patch-ID uses oid_to_hex() for those and
 * never reads blob content.  Use userdiff_find_by_path() since
 * diff_filespec_load_driver() is static in diff.c.
 *
 * Clean up with diff_queue_clear() (from diffcore.h).
 */
ساکن خالی collect_diff_blob_oids(ساخت commit *commit,
				   ساخت diff_options *opts,
				   ساخت oidset *blobs)
{
	ساخت diff_قطار_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	أكد(!commit->parents || !commit->parents->اگلا);

	اگر (commit->parents)
		diff_درخت_oid(&commit->parents->شے->object.oid,
			      &commit->object.oid, "", opts);
	ورنہ
		diff_جڑ_درخت_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	برائے (صحیح i = 0; i < q->nr; i++) {
		ساخت diff_filepair *p = q->قطار[i];
		ساخت userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_ڈھونڈو_by_راستہ(opts->repo->اشاریہ, p->one->راستہ);
		اگر (drv && drv->ثنائی > 0)
			جاری;

		اگر (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, تہی,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_داخل_کرو(blobs, &p->one->oid);
		اگر (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, تہی,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_داخل_کرو(blobs, &p->two->oid);
	}
	diff_قطار_clear(q);
}

ساکن صحیح always_مطابقت(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
			ثابت ساخت hashmap_اندراج *entry1 غیر_مستعمل,
			ثابت ساخت hashmap_اندراج *entry2 غیر_مستعمل,
			ثابت خالی *keydata غیر_مستعمل)
{
	واپس 0;
}

/*
 * Prefetch blobs for git cherry in partial clones.
 *
 * Called between the revision walk (which builds the head-side
 * commit list) and the has_commit_patch_id() comparison loop.
 *
 * Uses a cmpfn-swap trick to avoid reading blobs: temporarily
 * replaces the hashmap's comparison function with a trivial
 * always-match function, so hashmap_get()/hashmap_get_next() match
 * any entry with the same oidhash bucket.  These are the set of oids
 * that would trigger patch_id_neq() during normal lookup and cause
 * blobs to be read on demand, and we want to prefetch them all at
 * once instead.
 */
ساکن خالی prefetch_cherry_blobs(ساخت repository *repo,
				  ساخت commit_list *list,
				  ساخت patch_ids *ids)
{
	ساخت oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	اگر (!repo_has_promisor_remote(repo))
		واپس;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_مطابقت;

	/* Find header-only collisions, gather blobs from those commits */
	برائے (ساخت commit_list *l = list; l; l = l->اگلا) {
		ساخت commit *c = l->شے;
		bool مطابقت_ملا = false;
		برائے (ساخت patch_id *cur = patch_id_iter_پہلا(c, ids);
		     cur;
		     cur = patch_id_iter_اگلا(cur, ids)) {
			مطابقت_ملا = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		اگر (مطابقت_ملا)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	اگر (oidset_سائز(&blobs)) {
		ساخت oid_array مقصد_fetch = OID_ARRAY_INIT;
		ساخت oidset_iter iter;
		ثابت ساخت object_id *oid;

		oidset_iter_init(&blobs, &iter);
		جب ((oid = oidset_iter_اگلا(&iter)))
			oid_array_append(&مقصد_fetch, oid);

		promisor_remote_get_براہ_راست(repo, مقصد_fetch.oid, مقصد_fetch.nr);

		oid_array_clear(&مقصد_fetch);
	}

	oidset_clear(&blobs);
}

صحیح cmd_cherry(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	ساخت rev_info revs;
	ساخت patch_ids ids;
	ساخت commit *commit;
	ساخت commit_list *list = تہی;
	ساخت branch *current_branch;
	ثابت حرف *upstream;
	ثابت حرف *سر = "HEAD";
	ثابت حرف *حد_بندی = تہی;
	صحیح verbose = 0, abbrev = 0;

	ساخت option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, cherry_usage, 0);

	انتخاب (argc) {
	حالت 3:
		حد_بندی = argv[2];
		/* FALLTHROUGH */
	حالت 2:
		سر = argv[1];
		/* FALLTHROUGH */
	حالت 1:
		upstream = argv[0];
		توڑو;
	طےشدہ:
		current_branch = branch_get(تہی);
		upstream = branch_get_upstream(current_branch, تہی);
		اگر (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, سابقہ);
	revs.max_parents = 1;

	اگر (جوڑو_pending_commit(سر, &revs, 0))
		die(_("unknown commit %s"), سر);
	اگر (جوڑو_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	اگر (revs.pending.nr == 2) {
		ساخت object_array_اندراج *o = revs.pending.objects;
		اگر (oideq(&o[0].شے->oid, &o[1].شے->oid))
			واپس 0;
	}

	get_patch_ids(&revs, &ids);

	اگر (حد_بندی && جوڑو_pending_commit(حد_بندی, &revs, UNINTERESTING))
		die(_("unknown commit %s"), حد_بندی);

	/* reverse the list of commits */
	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	جب ((commit = get_revision(&revs)) != تہی) {
		commit_list_داخل_کرو(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	برائے (ساخت commit_list *l = list; l; l = l->اگلا) {
		حرف sign = '+';

		commit = l->شے;
		اگر (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	واپس 0;
}
