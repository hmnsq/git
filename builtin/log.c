/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "pager.h"
#ضمن "color.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "diff-merges.h"
#ضمن "revision.h"
#ضمن "log-tree.h"
#ضمن "oid-array.h"
#ضمن "oidset.h"
#ضمن "tag.h"
#ضمن "reflog-walk.h"
#ضمن "patch-ids.h"
#ضمن "path.h"
#ضمن "shortlog.h"
#ضمن "remote.h"
#ضمن "string-list.h"
#ضمن "parse-options.h"
#ضمن "line-log.h"
#ضمن "branch.h"
#ضمن "version.h"
#ضمن "mailmap.h"
#ضمن "progress.h"
#ضمن "commit-slab.h"
#ضمن "advice.h"
#ضمن "utf8.h"

#ضمن "commit-reach.h"
#ضمن "promisor-remote.h"
#ضمن "range-diff.h"
#ضمن "tmp-objdir.h"
#ضمن "tree.h"
#ضمن "userdiff.h"
#ضمن "write-or-die.h"

#وصف_ڪريو MAIL_DEFAULT_WRAP 72
#وصف_ڪريو COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#وصف_ڪريو FORMAT_PATCH_NAME_MAX_DEFAULT 64

ساكن بنانشان صحيح force_اندريون_body_from;
ساكن صحيح stdout_mboxrd;
ساكن صحيح format_no_اڳياڙي;

ساكن ثابت حرف * ثابت builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	عدم
};

بنية سٽ_opt_callback_معلومات {
	بنية rev_info *rev;
	ثابت حرف *اڳياڙي;
	بنية string_list دليلون;
};

ساكن صحيح session_is_interactive(فراغ)
{
	أرجع isatty(1) || pager_اندريون_use();
}

ساكن صحيح auto_decoration_style(فراغ)
{
	أرجع session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ساكن صحيح parse_decoration_style(ثابت حرف *قدر)
{
	بدل (git_parse_maybe_bool(قدر)) {
	حالة 1:
		أرجع DECORATE_SHORT_REFS;
	حالة 0:
		أرجع 0;
	افتراضي:
		اكسر;
	}
	إذا (!strcmp(قدر, "full"))
		أرجع DECORATE_ڀريل_REFS;
	وإلا إذا (!strcmp(قدر, "short"))
		أرجع DECORATE_SHORT_REFS;
	وإلا إذا (!strcmp(قدر, "auto"))
		أرجع auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	أرجع -1;
}

بنية log_config {
	صحيح default_abbrev_commit;
	صحيح default_show_پاڙ;
	صحيح default_follow;
	صحيح default_show_signature;
	صحيح default_encode_email_سرا;
	صحيح decoration_style;
	صحيح decoration_given;
	صحيح use_mailmap_config;
	حرف *fmt_patch_subject_اڳياڙي;
	صحيح fmt_patch_name_max;
	حرف *fmt_pretty;
	حرف *default_date_گهڻائي;

#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	صحيح i_still_use_this;
#جيڪڏهن_پورو
};

ساكن فراغ log_config_init(بنية log_config *cfg)
{
	memset(cfg, 0, حجم(*cfg));
	cfg->default_show_پاڙ = 1;
	cfg->default_encode_email_سرا = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_اڳياڙي = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ساكن فراغ log_config_release(بنية log_config *cfg)
{
	حرر(cfg->default_date_گهڻائي);
	حرر(cfg->fmt_pretty);
	حرر(cfg->fmt_patch_subject_اڳياڙي);
}

ساكن صحيح use_default_decoration_filter = 1;
ساكن بنية string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ساكن بنية string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ساكن بنية string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ساكن صحيح clear_decorations_callback(ثابت بنية option *opt اڻ_استعمال_ٿيل,
				      ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	أرجع 0;
}

ساكن صحيح decorate_callback(ثابت بنية option *opt, ثابت حرف *arg,
			     صحيح unset)
{
	بنية log_config *cfg = opt->قدر;

	إذا (unset)
		cfg->decoration_style = 0;
	وإلا إذا (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	وإلا
		cfg->decoration_style = DECORATE_SHORT_REFS;

	إذا (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	أرجع 0;
}

ساكن صحيح log_سٽ_range_callback(ثابت بنية option *option, ثابت حرف *arg, صحيح unset)
{
	بنية سٽ_opt_callback_معلومات *معلومات = option->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (!arg)
		أرجع -1;

	معلومات->rev->سٽ_level_traverse = 1;
	string_list_append(&معلومات->دليلون, arg);

	أرجع 0;
}

ساكن فراغ cmd_log_init_defaults(بنية rev_info *rev,
				  بنية log_config *cfg)
{
	إذا (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	إذا (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_سرو = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_پاڙ_diff = cfg->default_show_پاڙ;
	rev->subject_اڳياڙي = cfg->fmt_patch_subject_اڳياڙي;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_سرا = cfg->default_encode_email_سرا;

	إذا (cfg->default_date_گهڻائي)
		parse_date_format(cfg->default_date_گهڻائي, &rev->date_گهڻائي);
}

ساكن فراغ سيٽ_default_decoration_filter(بنية decoration_filter *decoration_filter)
{
	حرف *قدر = عدم;
	بنية string_list *include = decoration_filter->include_ref_نمونو;
	ثابت بنية string_list *config_exclude;

	إذا (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		بنية string_list_شي *شي;
		for_each_string_list_شي(شي, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_نمونو,
					   شي->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	إذا (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &قدر) &&
	    !strcmp("all", قدر))
		use_default_decoration_filter = 0;
	حرر(قدر);

	إذا (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_نمونو->nr ||
	    decoration_filter->include_ref_نمونو->nr ||
	    decoration_filter->exclude_ref_config_نمونو->nr)
		أرجع;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	لكل (سائيز_t i = 0; i < ARRAY_سائيز(ref_namespace); i++) {
		إذا (!ref_namespace[i].decoration)
			استمر;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ساكن فراغ cmd_log_init_پڄاڻي(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			 بنية rev_info *rev, بنية تياري_revision_opt *opt,
			 بنية log_config *cfg)
{
	بنية userformat_want w;
	صحيح quiet = 0, ذريعو = 0, mailmap;
	ساكن بنية سٽ_opt_callback_معلومات سٽ_cb = {عدم, عدم, STRING_LIST_INIT_DUP};
	بنية decoration_filter decoration_filter = {
		.exclude_ref_نمونو = &decorate_refs_exclude,
		.include_ref_نمونو = &decorate_refs_include,
		.exclude_ref_config_نمونو = &decorate_refs_exclude_config,
	};
	ساكن بنية revision_sources revision_sources;

	ثابت بنية option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &ذريعو, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#جيڪڏهن_پورو
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", عدم, عدم,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, عدم, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', عدم, &سٽ_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_سٽ_range_callback),
		OPT_پڇاڙي()
	};

	سٽ_cb.rev = rev;
	سٽ_cb.اڳياڙي = اڳياڙي;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, اڳياڙي,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	إذا (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = تياري_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	إذا (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	إذا (rev->سٽ_level_traverse && rev->prune_معلومات.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, حجم(w));
	userformat_ڳوليو_requirements(عدم, &w);

	إذا (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	إذا (rev->show_notes)
		ڀريو_display_notes(&rev->notes_opt);

	إذا ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_نقاب) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_سرو = 0;

	إذا (ذريعو || w.ذريعو) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	إذا (mailmap) {
		rev->mailmap = xmalloc(حجم(بنية string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	إذا (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		إذا (!cfg->decoration_given)
			cfg->decoration_style = 0;
		إذا (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	إذا (rev->commit_format == CMIT_FMT_USERFORMAT) {
		إذا (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} وإلا إذا (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	إذا (cfg->decoration_style || rev->simplify_by_decoration) {
		سيٽ_default_decoration_filter(&decoration_filter);

		إذا (cfg->decoration_style)
			rev->show_decorations = 1;

		ڀريو_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	إذا (rev->سٽ_level_traverse)
		سٽ_log_init(rev, سٽ_cb.اڳياڙي, &سٽ_cb.دليلون);

	تياري_pager(the_repository);
}

ساكن فراغ cmd_log_init(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			 بنية rev_info *rev, بنية تياري_revision_opt *opt,
			 بنية log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_پڄاڻي(argc, argv, اڳياڙي, rev, opt, cfg);
}

ساكن صحيح cmd_log_walk_no_free(بنية rev_info *rev)
{
	بنية commit *commit;
	صحيح saved_nrl = 0;
	صحيح saved_dcctc = 0;
	صحيح نتيجو;

	إذا (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	طالما ((commit = get_revision(rev)) != عدم) {
		إذا (!log_وڻ_commit(rev, commit) && rev->max_ڳڻپ >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_ڳڻپ++;
		إذا (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = عدم;
		}
		إذا (saved_nrl < rev->diffopt.needed_rename_حد_بندي)
			saved_nrl = rev->diffopt.needed_rename_حد_بندي;
		إذا (rev->diffopt.degraded_cc_منزل_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_منزل_c = saved_dcctc;
	rev->diffopt.needed_rename_حد_بندي = saved_nrl;

	نتيجو = diff_نتيجو_ڪوڊ(rev);
	إذا (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		نتيجو = 02;
	}
	أرجع نتيجو;
}

ساكن صحيح cmd_log_walk(بنية rev_info *rev)
{
	صحيح retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	أرجع retval;
}

ساكن صحيح git_log_config(ثابت حرف *var, ثابت حرف *قدر,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية log_config *cfg = cb;
	ثابت حرف *جڳهه_name;

	إذا (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		أرجع git_config_string(&cfg->fmt_pretty, var, قدر);
	}
	إذا (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_اڳياڙي);
		أرجع git_config_string(&cfg->fmt_patch_subject_اڳياڙي, var, قدر);
	}
	إذا (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, قدر, ctx->kvi);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_سرا = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_گهڻائي);
		أرجع git_config_string(&cfg->default_date_گهڻائي, var, قدر);
	}
	إذا (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(قدر);
		إذا (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		أرجع 0;
	}
	إذا (!strcmp(var, "log.diffmerges")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		أرجع diff_merges_config(قدر);
	}
	إذا (!strcmp(var, "log.showroot")) {
		cfg->default_show_پاڙ = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (skip_اڳياڙي(var, "color.decorate.", &جڳهه_name))
		أرجع parse_decorate_color_config(var, جڳهه_name, قدر);
	إذا (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, قدر);
		أرجع 0;
	}

	أرجع git_diff_ui_config(var, قدر, ctx, cb);
}

#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
صحيح cmd_whatchanged(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *اڳياڙي,
		    بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية log_config cfg;
	بنية rev_info rev;
	بنية تياري_revision_opt opt;
	صحيح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, اڳياڙي);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, اڳياڙي, &rev, &opt, &cfg);

	إذا (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	إذا (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	أرجع ret;
}
#جيڪڏهن_پورو

ساكن فراغ show_tagger(ثابت حرف *buf, بنية rev_info *rev)
{
	بنية strbuf ٻاهريون = STRBUF_INIT;
	بنية pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_گهڻائي = rev->date_گهڻائي;
	pp_user_info(&pp, "Tagger", &ٻاهريون, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", ٻاهريون.buf);
	strbuf_release(&ٻاهريون);
}

ساكن صحيح show_blob_object(ثابت بنية object_id *oid, بنية rev_info *rev, ثابت حرف *obj_name)
{
	بنية object_id oidc;
	بنية object_context obj_context = {0};
	حرف *buf;
	بنانشان ڊگهو سائيز;

	fflush(rev->diffopt.file);
	إذا (!rev->diffopt.flags.textconv_سيٽ_ذريعي_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		أرجع odb_وهڪرو_blob_منزل_fd(the_repository->objects, 1, oid, عدم, 0);

	إذا (get_oid_with_context(the_repository, obj_name,
				 GET_OID_رڪارڊ_رستو,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	إذا (!obj_context.رستو ||
	    !textconv_object(the_repository, obj_context.رستو,
			     obj_context.گهڻائي, &oidc, 1, &buf, &سائيز)) {
		object_context_release(&obj_context);
		أرجع odb_وهڪرو_blob_منزل_fd(the_repository->objects, 1, oid, عدم, 0);
	}

	إذا (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, سائيز);
	object_context_release(&obj_context);
	حرر(buf);
	أرجع 0;
}

ساكن صحيح show_ٽيگ_object(ثابت بنية object_id *oid, بنية rev_info *rev)
{
	سائيز_t سائيز;
	تعداد object_type type;
	حرف *buf = odb_read_object(the_repository->objects, oid, &type, &سائيز);
	بنانشان ڊگهو وٿي = 0;

	إذا (!buf)
		أرجع error(_("could not read object %s"), oid_منزل_hex(oid));

	أكد(type == OBJ_ٽيگ);
	طالما (وٿي < سائيز && buf[وٿي] != '\n') {
		بنانشان ڊگهو new_وٿي = وٿي + 1;
		ثابت حرف *ident;
		طالما (new_وٿي < سائيز && buf[new_وٿي++] != '\n')
			; /* do nothing */
		إذا (skip_اڳياڙي(buf + وٿي, "tagger ", &ident))
			show_tagger(ident, rev);
		وٿي = new_وٿي;
	}

	إذا (وٿي < سائيز)
		fwrite(buf + وٿي, سائيز - وٿي, 1, rev->diffopt.file);
	حرر(buf);
	أرجع 0;
}

ساكن صحيح show_وڻ_object(ثابت بنية object_id *oid اڻ_استعمال_ٿيل,
			    بنية strbuf *بنياد اڻ_استعمال_ٿيل,
			    ثابت حرف *pathname, بنانشان گهڻائي,
			    فراغ *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(گهڻائي) ? "/" : "");
	أرجع 0;
}

ساكن فراغ show_تياري_revisions_tweak(بنية rev_info *rev)
{
	إذا (rev->پهريون_والد_only)
		diff_merges_default_منزل_پهريون_والد(rev);
	وإلا
		diff_merges_default_منزل_dense_combined(rev);
	إذا (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

صحيح cmd_show(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *اڳياڙي,
	     بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية log_config cfg;
	بنية rev_info rev;
	بنانشان صحيح i;
	بنية تياري_revision_opt opt;
	بنية pathspec ميلاپ_all;
	صحيح ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	إذا (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ڀريل_اشاريو = 0;
	}

	memset(&ميلاپ_all, 0, حجم(ميلاپ_all));
	repo_init_revisions(the_repository, &rev, اڳياڙي);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_سرو = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_ويڪر = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	opt.tweak = show_تياري_revisions_tweak;
	cmd_log_init(argc, argv, اڳياڙي, &rev, &opt, &cfg);

	إذا (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		أرجع ret;
	}

	rev.diffopt.no_free = 1;
	لكل (i = 0; i < rev.pending.nr && !ret; i++) {
		بنية object *o = rev.pending.objects[i].شي;
		ثابت حرف *name = rev.pending.objects[i].name;
		بدل (o->type) {
		حالة OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			اكسر;
		حالة OBJ_ٽيگ: {
			بنية ٽيگ *t = (بنية ٽيگ *)o;
			بنية object_id *oid = get_tagged_oid(t);

			إذا (rev.shown_one)
				اکر_لکو('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->ٽيگ,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_ٽيگ_object(&o->oid, &rev);
			rev.shown_one = 1;
			إذا (ret)
				اكسر;
			o = parse_object(the_repository, oid);
			إذا (!o)
				ret = error(_("could not read object %s"),
					    oid_منزل_hex(oid));
			rev.pending.objects[i].شي = o;
			i--;
			اكسر;
		}
		حالة OBJ_وڻ:
			إذا (rev.shown_one)
				اکر_لکو('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_وڻ(the_repository, (بنية وڻ *)o,
				  &ميلاپ_all, show_وڻ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			اكسر;
		حالة OBJ_COMMIT:
		{
			بنية object_array old;
			بنية object_array blank = OBJECT_ARRAY_INIT;

			يادگيري_نقل(&old, &rev.pending, حجم(old));
			يادگيري_نقل(&rev.pending, &blank, حجم(rev.pending));

			جوڙيو_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			يادگيري_نقل(&rev.pending, &old, حجم(rev.pending));
			اكسر;
		}
		افتراضي:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	أرجع ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
صحيح cmd_log_reflog(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية log_config cfg;
	بنية rev_info rev;
	بنية تياري_revision_opt opt;
	صحيح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, اڳياڙي);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_سرو = 1;
	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_سرو = 1;
	cmd_log_init_پڄاڻي(argc, argv, اڳياڙي, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	أرجع ret;
}

ساكن فراغ log_تياري_revisions_tweak(بنية rev_info *rev)
{
	إذا (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_معلومات, 0))
		rev->diffopt.flags.follow_renames = 1;

	إذا (rev->پهريون_والد_only)
		diff_merges_default_منزل_پهريون_والد(rev);
}

صحيح cmd_log(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *اڳياڙي,
	    بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية log_config cfg;
	بنية rev_info rev;
	بنية تياري_revision_opt opt;
	صحيح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, اڳياڙي);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_سرو = 1;
	memset(&opt, 0, حجم(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_تياري_revisions_tweak;
	cmd_log_init(argc, argv, اڳياڙي, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	أرجع ret;
}

/* format-patch */

تعداد cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

تعداد thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

تعداد cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_پيغام,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

تعداد auto_بنياد_setting {
	AUTO_بنياد_NEVER,
	AUTO_بنياد_ALWAYS,
	AUTO_بنياد_WHEN_ABLE
};

بنية format_config {
	بنية log_config لاگرٿم;
	تعداد thread_level thread;
	صحيح do_signoff;
	تعداد auto_بنياد_setting auto_بنياد;
	حرف *بنياد_commit;
	حرف *from;
	حرف *signature;
	حرف *signature_file;
	تعداد cover_setting config_cover_letter;
	حرف *fmt_cover_letter_commit_list;
	حرف *config_output_directory;
	تعداد cover_from_description cover_from_description_گهڻائي;
	صحيح show_notes;
	بنية display_notes_opt notes_opt;
	صحيح numbered_cmdline_opt;
	صحيح numbered;
	صحيح auto_عدد;
	حرف *default_attach;
	بنية string_list extra_hdr;
	بنية string_list extra_منزل;
	بنية string_list extra_cc;
	صحيح keep_subject;
	صحيح subject_اڳياڙي;
	بنية strbuf sprefix;
	حرف *fmt_patch_suffix;
};

ساكن فراغ format_config_init(بنية format_config *cfg)
{
	memset(cfg, 0, حجم(*cfg));
	log_config_init(&cfg->لاگرٿم);
	cfg->cover_from_description_گهڻائي = COVER_FROM_پيغام;
	cfg->auto_عدد = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_منزل);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ساكن فراغ format_config_release(بنية format_config *cfg)
{
	log_config_release(&cfg->لاگرٿم);
	حرر(cfg->بنياد_commit);
	حرر(cfg->from);
	حرر(cfg->signature);
	حرر(cfg->signature_file);
	حرر(cfg->config_output_directory);
	حرر(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_منزل, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	حرر(cfg->fmt_patch_suffix);
	حرر(cfg->fmt_cover_letter_commit_list);
}

ساكن تعداد cover_from_description parse_cover_from_description(ثابت حرف *arg)
{
	إذا (!arg || !strcmp(arg, "default"))
		أرجع COVER_FROM_پيغام;
	وإلا إذا (!strcmp(arg, "none"))
		أرجع COVER_FROM_NONE;
	وإلا إذا (!strcmp(arg, "message"))
		أرجع COVER_FROM_پيغام;
	وإلا إذا (!strcmp(arg, "subject"))
		أرجع COVER_FROM_SUBJECT;
	وإلا إذا (!strcmp(arg, "auto"))
		أرجع COVER_FROM_AUTO;
	وإلا
		die(_("%s: invalid cover from description mode"), arg);
}

ساكن فراغ جوڙيو_سرو(بنية format_config *cfg, ثابت حرف *قدر)
{
	بنية string_list_شي *شي;
	صحيح len = لڙي_ڊيگهه(قدر);
	طالما (len && قدر[len - 1] == '\n')
		len--;

	إذا (!strncasecmp(قدر, "to: ", 4)) {
		شي = string_list_append(&cfg->extra_منزل, قدر + 4);
		len -= 4;
	} وإلا إذا (!strncasecmp(قدر, "cc: ", 4)) {
		شي = string_list_append(&cfg->extra_cc, قدر + 4);
		len -= 4;
	} وإلا {
		شي = string_list_append(&cfg->extra_hdr, قدر);
	}

	شي->string[len] = '\0';
}

ساكن صحيح git_format_config(ثابت حرف *var, ثابت حرف *قدر,
			     ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية format_config *cfg = cb;

	إذا (!strcmp(var, "format.headers")) {
		إذا (!قدر)
			die(_("format.headers without value"));
		جوڙيو_سرو(cfg, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		أرجع git_config_string(&cfg->fmt_patch_suffix, var, قدر);
	}
	إذا (!strcmp(var, "format.to")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		string_list_append(&cfg->extra_منزل, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.cc")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		أرجع 0;
	}
	إذا (!strcmp(var, "format.numbered")) {
		إذا (قدر && !strcasecmp(قدر, "auto")) {
			cfg->auto_عدد = 1;
			أرجع 0;
		}
		cfg->numbered = git_config_bool(var, قدر);
		cfg->auto_عدد = cfg->auto_عدد && cfg->numbered;
		أرجع 0;
	}
	إذا (!strcmp(var, "format.attach")) {
		إذا (قدر && *قدر) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(قدر);
		} وإلا إذا (قدر && !*قدر) {
			FREE_AND_NULL(cfg->default_attach);
		} وإلا {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_نسخو_string);
		}
		أرجع 0;
	}
	إذا (!strcmp(var, "format.thread")) {
		إذا (قدر && !strcasecmp(قدر, "deep")) {
			cfg->thread = THREAD_DEEP;
			أرجع 0;
		}
		إذا (قدر && !strcasecmp(قدر, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			أرجع 0;
		}
		cfg->thread = git_config_bool(var, قدر) ? THREAD_SHALLOW : THREAD_UNSET;
		أرجع 0;
	}
	إذا (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		أرجع git_config_string(&cfg->signature, var, قدر);
	}
	إذا (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		أرجع git_config_pathname(&cfg->signature_file, var, قدر);
	}
	إذا (!strcmp(var, "format.coverletter")) {
		إذا (قدر && !strcasecmp(قدر, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			أرجع 0;
		}
		cfg->config_cover_letter = git_config_bool(var, قدر) ? COVER_ON : COVER_OFF;
		أرجع 0;
	}
	إذا (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		أرجع git_config_string(&cfg->fmt_cover_letter_commit_list, var, قدر);
	}
	إذا (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		أرجع git_config_string(&cfg->config_output_directory, var, قدر);
	}
	إذا (!strcmp(var, "format.useautobase")) {
		إذا (قدر && !strcasecmp(قدر, "whenAble")) {
			cfg->auto_بنياد = AUTO_بنياد_WHEN_ABLE;
			أرجع 0;
		}
		cfg->auto_بنياد = git_config_bool(var, قدر) ? AUTO_بنياد_ALWAYS : AUTO_بنياد_NEVER;
		أرجع 0;
	}
	إذا (!strcmp(var, "format.from")) {
		صحيح b = git_parse_maybe_bool(قدر);
		FREE_AND_NULL(cfg->from);
		إذا (b < 0)
			cfg->from = xstrdup(قدر);
		وإلا إذا (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		أرجع 0;
	}
	إذا (!strcmp(var, "format.forceinbodyfrom")) {
		force_اندريون_body_from = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.notes")) {
		صحيح b = git_parse_maybe_bool(قدر);
		إذا (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, قدر);
		وإلا إذا (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		وإلا
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_گهڻائي = parse_cover_from_description(قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "format.noprefix")) {
		format_no_اڳياڙي = git_parse_maybe_bool(قدر);
		إذا (format_no_اڳياڙي < 0) {
			صحيح حالت = die_پيغام(
				_("bad boolean config value '%s' for '%s'"),
				قدر, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			ٻاهر_نڪرو(حالت);
		}
		أرجع 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	إذا (!strcmp(var, "diff.noprefix"))
		أرجع 0;

	أرجع git_log_config(var, قدر, ctx, &cfg->لاگرٿم);
}

ساكن ثابت حرف *output_directory = عدم;
ساكن صحيح outdir_وٿي;

ساكن صحيح open_ايندڙ_file(بنية commit *commit, ثابت حرف *subject,
			 بنية rev_info *rev, صحيح quiet)
{
	بنية strbuf filename = STRBUF_INIT;

	إذا (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	إذا (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	وإلا إذا (commit)
		fmt_output_commit(&filename, commit, rev);
	وإلا
		fmt_output_subject(&filename, subject, rev);

	إذا (!quiet)
		لکو("%s\n", filename.buf + outdir_وٿي);

	إذا (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		أرجع -1;
	}

	strbuf_release(&filename);
	أرجع 0;
}

ساكن فراغ get_patch_ids(بنية rev_info *rev, بنية patch_ids *ids)
{
	بنية rev_info check_rev;
	بنية commit *commit, *c1, *c2;
	بنية object *o1, *o2;
	بنانشان flags1, flags2;

	إذا (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].شي;
	o2 = rev->pending.objects[1].شي;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	إذا ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->اڳياڙي);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	جوڙيو_pending_object(&check_rev, o1, "o1");
	جوڙيو_pending_object(&check_rev, o2, "o2");
	إذا (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	طالما ((commit = get_revision(&check_rev)) != عدم) {
		جوڙيو_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, ڏٺل_2 | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, ڏٺل_2 | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ساكن فراغ gen_پيغام_id(بنية rev_info *info, ثابت حرف *بنياد)
{
	بنية strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", بنياد,
		    (timestamp_t) وقت(عدم),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->پيغام_id = strbuf_detach(&buf, عدم);
}

ساكن فراغ print_signature(ثابت حرف *signature, FILE *file)
{
	إذا (!signature || !*signature)
		أرجع;

	fprintf(file, "-- \n%s", signature);
	إذا (signature[لڙي_ڊيگهه(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ساكن حرف *ڳوليو_branch_name(بنية rev_info *rev)
{
	بنية object_id branch_oid;
	ثابت بنية object_id *tip_oid;
	ثابت حرف *ref, *v;
	حرف *ڀريل_ref, *branch = عدم;
	صحيح interesting_مليو = 0;
	سائيز_t idx;

	لكل (سائيز_t i = 0; i < rev->cmdline.nr; i++) {
		إذا (rev->cmdline.rev[i].flags & UNINTERESTING)
			استمر;
		إذا (interesting_مليو)
			أرجع عدم;
		interesting_مليو = 1;
		idx = i;
	}
	إذا (!interesting_مليو)
		أرجع عدم;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].شي->oid;
	إذا (repo_dwim_ref(the_repository, ref, لڙي_ڊيگهه(ref), &branch_oid,
			  &ڀريل_ref, 0) &&
	    skip_اڳياڙي(ڀريل_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	حرر(ڀريل_ref);
	أرجع branch;
}

ساكن فراغ show_diffstat(بنية rev_info *rev,
			  بنية commit *origin, بنية commit *سر)
{
	بنية diff_options opts;

	يادگيري_نقل(&opts, &rev->diffopt, حجم(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_تياري_پورو(&opts);

	diff_وڻ_oid(get_commit_وڻ_oid(origin),
		      get_commit_وڻ_oid(سر),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ساكن فراغ read_desc_file(بنية strbuf *buf, ثابت حرف *desc_file)
{
	إذا (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ساكن فراغ prepare_cover_متن(بنية pretty_print_context *pp,
			       ثابت حرف *description_file,
			       ثابت حرف *branch_name,
			       بنية strbuf *sb,
			       ثابت حرف *encoding,
			       صحيح need_8bit_cte,
			       ثابت بنية format_config *cfg)
{
	ثابت حرف *subject = "*** SUBJECT HERE ***";
	ثابت حرف *body = "*** BLURB HERE ***";
	بنية strbuf description_sb = STRBUF_INIT;
	بنية strbuf subject_sb = STRBUF_INIT;

	إذا (cfg->cover_from_description_گهڻائي == COVER_FROM_NONE)
		اذهب do_pp;

	إذا (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	وإلا إذا (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	إذا (!description_sb.len)
		اذهب do_pp;

	إذا (cfg->cover_from_description_گهڻائي == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_گهڻائي == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	إذا (cfg->cover_from_description_گهڻائي == COVER_FROM_پيغام ||
	    (cfg->cover_from_description_گهڻائي == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	وإلا
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

بنية rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	بنية string_list notes;
};

ساكن صحيح rdiff_notes_cb(ثابت بنية option *option,
			  ثابت حرف *arg,
			  صحيح unset)
{
	بنية option opt = *option;
	بنية rdiff_notes *rdiff_notes = option->قدر;

	rdiff_notes->override = 1;
	opt.قدر = &rdiff_notes->notes;
	أرجع parse_opt_string_list(&opt, arg, unset);
}

ساكن صحيح get_notes_refs(بنية string_list_شي *شي, فراغ *arg)
{
	strvec_pushf(arg, "--notes=%s", شي->string);
	أرجع 0;
}

ساكن فراغ get_notes_دليلون(بنية rdiff_notes *rdiff_notes,
			   بنية rev_info *rev)
{
	إذا (rdiff_notes->override) {
		إذا (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		وإلا
			strvec_ڌڪو(&rev->rdiff_log_arg, "--no-notes");
	} وإلا إذا (!rev->show_notes) {
		strvec_ڌڪو(&rev->rdiff_log_arg, "--no-notes");
	} وإلا إذا (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ڌڪو(&rev->rdiff_log_arg, "--notes");
	} وإلا {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ساكن فراغ generate_shortlog_cover_letter(بنية shortlog *لاگرٿم,
					   بنية rev_info *rev,
					   بنية commit **list,
					   صحيح nr)
{
	shortlog_init(لاگرٿم);
	لاگرٿم->wrap_سٽون = 1;
	لاگرٿم->wrap = MAIL_DEFAULT_WRAP;
	لاگرٿم->in1 = 2;
	لاگرٿم->in2 = 4;
	لاگرٿم->file = rev->diffopt.file;
	لاگرٿم->groups = SHORTLOG_گروهه_AUTHOR;
	shortlog_پڄاڻي_تياري(لاگرٿم);
	لكل (صحيح i = 0; i < nr; i++)
		shortlog_جوڙيو_commit(لاگرٿم, list[i]);

	shortlog_output(لاگرٿم);
}

ساكن فراغ generate_commit_list_cover(FILE *cover_file, ثابت حرف *format,
				       بنية commit **list, صحيح n)
{
	بنية strbuf commit_سٽ = STRBUF_INIT;
	بنية pretty_print_context ctx = {0};
	بنية rev_info rev = REV_INFO_INIT;

	rev.ڪل = n;
	ctx.rev = &rev;
	لكل (صحيح i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_پيغام(the_repository, list[n - i], format,
				&commit_سٽ, &ctx);
		fprintf(cover_file, "%s\n", commit_سٽ.buf);
		strbuf_reset(&commit_سٽ);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_سٽ);
}

ساكن فراغ make_cover_letter(بنية rev_info *rev, صحيح use_separate_file,
			      بنية commit *origin,
			      صحيح nr, بنية commit **list,
			      ثابت حرف *description_file,
			      ثابت حرف *branch_name,
			      صحيح quiet,
			      ثابت بنية format_config *cfg,
			      ثابت حرف *format)
{
	ثابت حرف *from;
	بنية shortlog لاگرٿم;
	بنية strbuf sb = STRBUF_INIT;
	صحيح i;
	ثابت حرف *encoding = "UTF-8";
	صحيح need_8bit_cte = 0;
	بنية pretty_print_context pp = {0};
	بنية commit *سر = list[0];
	حرف *منزل_free = عدم;

	إذا (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	إذا (use_separate_file &&
	    open_ايندڙ_file(عدم, rev->numbered_files ? عدم : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_سرا(rev, سر, &pp.after_subject, &need_8bit_cte, 0);

	لكل (i = 0; !need_8bit_cte && i < nr; i++) {
		ثابت حرف *buf = repo_get_commit_بفر(the_repository,
							 list[i], عدم);
		إذا (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_بفر(the_repository, list[i], buf);
	}

	إذا (!branch_name)
		branch_name = منزل_free = ڳوليو_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_گهڻائي.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_سرا = rev->encode_email_سرا;
	pp_user_info(&pp, عدم, &sb, from, encoding);
	prepare_cover_متن(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	حرر(منزل_free);
	حرر(pp.after_subject);
	strbuf_release(&sb);

	إذا (skip_اڳياڙي(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	وإلا إذا (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&لاگرٿم, rev, list, nr);
	وإلا إذا (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	وإلا إذا (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	وإلا
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	إذا (origin)
		show_diffstat(rev, origin, سر);

	إذا (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	إذا (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		بنية diff_options opts;
		بنية range_diff_options range_diff_opts = {
			.creation_ضرب_ڪندڙ = rev->creation_ضرب_ڪندڙ,
			.dual_color = 1,
			.max_يادگيري = RANGE_DIFF_MAX_يادگيري_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_تياري(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_تياري_پورو(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

ساكن حرف *clean_پيغام_id(ثابت حرف *نياپو_id)
{
	حرف ch;
	ثابت حرف *a, *z, *m;

	m = نياپو_id;
	طالما ((ch = *m) && (خال_آهي(ch) || (ch == '<')))
		m++;
	a = m;
	z = عدم;
	طالما ((ch = *m)) {
		إذا (!خال_آهي(ch) && (ch != '>'))
			z = m;
		m++;
	}
	إذا (!z)
		die(_("insane in-reply-to: %s"), نياپو_id);
	إذا (++z == m)
		أرجع xstrdup(a);
	أرجع xmemdupz(a, z - a);
}

ساكن ثابت حرف *سيٽ_outdir(ثابت حرف *اڳياڙي, ثابت حرف *output_directory)
{
	إذا (output_directory && is_absolute_رستو(output_directory))
		أرجع output_directory;

	إذا (!اڳياڙي || !*اڳياڙي) {
		إذا (output_directory)
			أرجع output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_وٿي = 2;
		أرجع "./";
	}

	outdir_وٿي = لڙي_ڊيگهه(اڳياڙي);
	إذا (!output_directory)
		أرجع اڳياڙي;

	أرجع اڳياڙي_filename(اڳياڙي, output_directory);
}

ساكن ثابت حرف * ثابت builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	عدم
};

بنية keep_callback_معلومات {
	بنية format_config *cfg;
	بنية rev_info *revs;
};

ساكن صحيح keep_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية keep_callback_معلومات *معلومات = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	معلومات->revs->ڪل = -1;
	معلومات->cfg->keep_subject = 1;
	أرجع 0;
}

ساكن صحيح subject_اڳياڙي_callback(ثابت بنية option *opt, ثابت حرف *arg,
			    صحيح unset)
{
	بنية format_config *cfg = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_اڳياڙي = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	أرجع 0;
}

ساكن صحيح rfc_callback(ثابت بنية option *opt, ثابت حرف *arg,
			صحيح unset)
{
	ثابت حرف **rfc = opt->قدر;

	*rfc = opt->قدر;
	إذا (unset)
		*rfc = عدم;
	وإلا
		*rfc = arg ? arg : "RFC";
	أرجع 0;
}

ساكن صحيح numbered_callback(ثابت بنية option *opt, ثابت حرف *arg,
			     صحيح unset)
{
	بنية format_config *cfg = opt->قدر;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	إذا (unset)
		cfg->auto_عدد =  0;
	أرجع 0;
}

ساكن صحيح no_numbered_callback(ثابت بنية option *opt, ثابت حرف *arg,
				صحيح unset)
{
	BUG_ON_OPT_NEG(unset);
	أرجع numbered_callback(opt, arg, 1);
}

ساكن صحيح output_directory_callback(ثابت بنية option *opt, ثابت حرف *arg,
			      صحيح unset)
{
	ثابت حرف **dir = (ثابت حرف **)opt->قدر;
	BUG_ON_OPT_NEG(unset);
	إذا (*dir)
		die(_("two output directories?"));
	*dir = arg;
	أرجع 0;
}

ساكن صحيح thread_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية format_config *cfg = opt->قدر;

	إذا (unset)
		cfg->thread = THREAD_UNSET;
	وإلا إذا (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	وإلا إذا (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	وإلا
		أرجع 1;
	أرجع 0;
}

ساكن صحيح attach_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية rev_info *rev = (بنية rev_info *)opt->قدر;
	إذا (unset)
		rev->mime_boundary = عدم;
	وإلا إذا (arg)
		rev->mime_boundary = arg;
	وإلا
		rev->mime_boundary = git_نسخو_string;
	rev->no_inline = unset ? 0 : 1;
	أرجع 0;
}

ساكن صحيح inline_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية rev_info *rev = (بنية rev_info *)opt->قدر;
	إذا (unset)
		rev->mime_boundary = عدم;
	وإلا إذا (arg)
		rev->mime_boundary = arg;
	وإلا
		rev->mime_boundary = git_نسخو_string;
	rev->no_inline = 0;
	أرجع 0;
}

ساكن صحيح سرو_callback(ثابت بنية option *opt, ثابت حرف *arg,
			   صحيح unset)
{
	بنية format_config *cfg = opt->قدر;

	إذا (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_منزل, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} وإلا {
		جوڙيو_سرو(cfg, arg);
	}
	أرجع 0;
}

ساكن صحيح from_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	حرف **from = opt->قدر;

	حرر(*from);

	إذا (unset)
		*from = عدم;
	وإلا إذا (arg)
		*from = xstrdup(arg);
	وإلا
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	أرجع 0;
}

ساكن صحيح بنياد_callback(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية format_config *cfg = opt->قدر;

	إذا (unset) {
		cfg->auto_بنياد = AUTO_بنياد_NEVER;
		FREE_AND_NULL(cfg->بنياد_commit);
	} وإلا إذا (!strcmp(arg, "auto")) {
		cfg->auto_بنياد = AUTO_بنياد_ALWAYS;
		FREE_AND_NULL(cfg->بنياد_commit);
	} وإلا {
		cfg->auto_بنياد = AUTO_بنياد_NEVER;
		cfg->بنياد_commit = xstrdup(arg);
	}
	أرجع 0;
}

بنية بنياد_وڻ_info {
	بنية object_id بنياد_commit;
	صحيح nr_patch_id, alloc_patch_id;
	بنية object_id *patch_id;
};

ساكن بنية commit *get_بنياد_commit(ثابت بنية format_config *cfg,
				      بنية commit **list,
				      سائيز_t ڪل)
{
	بنية commit *بنياد = عدم;
	بنية commit **rev;
	صحيح auto_select, die_on_failure, ret;
	سائيز_t i = 0, rev_nr = 0;

	بدل (cfg->auto_بنياد) {
	حالة AUTO_بنياد_NEVER:
		إذا (cfg->بنياد_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} وإلا {
			/* no base information is requested */
			أرجع عدم;
		}
		اكسر;
	حالة AUTO_بنياد_ALWAYS:
	حالة AUTO_بنياد_WHEN_ABLE:
		إذا (cfg->بنياد_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} وإلا {
			auto_select = 1;
			die_on_failure = cfg->auto_بنياد == AUTO_بنياد_ALWAYS;
		}
		اكسر;
	افتراضي:
		BUG("unexpected automatic base selection method");
	}

	إذا (!auto_select) {
		بنياد = lookup_commit_reference_by_name(cfg->بنياد_commit);
		إذا (!بنياد)
			die(_("unknown commit %s"), cfg->بنياد_commit);
	} وإلا {
		بنية branch *curr_branch = branch_get(عدم);
		ثابت حرف *upstream = branch_get_upstream(curr_branch, عدم);
		إذا (upstream) {
			بنية commit_list *بنياد_list = عدم;
			بنية commit *commit;
			بنية object_id oid;

			إذا (repo_get_oid(the_repository, upstream, &oid)) {
				إذا (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				وإلا
					أرجع عدم;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			إذا (repo_get_ملايو_bases_many(the_repository,
						      commit, ڪل,
						      list,
						      &بنياد_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !بنياد_list || بنياد_list->ايندڙ) {
				إذا (die_on_failure) {
					die(_("could not find exact merge base"));
				} وإلا {
					commit_list_free(بنياد_list);
					أرجع عدم;
				}
			}
			بنياد = بنياد_list->شي;
			commit_list_free(بنياد_list);
		} وإلا {
			إذا (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			وإلا
				أرجع عدم;
		}
	}

	ALLOC_ARRAY(rev, ڪل);
	لكل (i = 0; i < ڪل; i++)
		rev[i] = list[i];

	rev_nr = ڪل;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	طالما (rev_nr > 1) {
		لكل (i = 0; i < rev_nr / 2; i++) {
			بنية commit_list *ملايو_بنياد = عدم;
			إذا (repo_get_ملايو_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &ملايو_بنياد) < 0 ||
			    !ملايو_بنياد || ملايو_بنياد->ايندڙ) {
				إذا (die_on_failure) {
					die(_("failed to find exact merge base"));
				} وإلا {
					commit_list_free(ملايو_بنياد);
					حرر(rev);
					أرجع عدم;
				}
			}

			rev[i] = ملايو_بنياد->شي;
			commit_list_free(ملايو_بنياد);
		}

		إذا (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ڦيرو_مٿي(rev_nr, 2);
	}

	ret = repo_اندريون_ملايو_bases(the_repository, بنياد, rev[0]);
	إذا (ret < 0)
		ٻاهر_نڪرو(128);
	إذا (!ret) {
		إذا (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} وإلا {
			حرر(rev);
			أرجع عدم;
		}
	}

	لكل (i = 0; i < ڪل; i++) {
		إذا (بنياد == list[i]) {
			إذا (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} وإلا {
				حرر(rev);
				أرجع عدم;
			}
		}
	}

	حرر(rev);
	أرجع بنياد;
}

define_commit_slab(commit_بنياد, صحيح);

ساكن فراغ prepare_bases(بنية بنياد_وڻ_info *bases,
			  بنية commit *بنياد,
			  بنية commit **list,
			  سائيز_t ڪل)
{
	بنية commit *commit;
	بنية rev_info revs;
	بنية diff_options diffopt;
	بنية commit_بنياد commit_بنياد;

	إذا (!بنياد)
		أرجع;

	init_commit_بنياد(&commit_بنياد);
	repo_diff_تياري(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_تياري_پورو(&diffopt);

	oidcpy(&bases->بنياد_commit, &بنياد->object.oid);

	repo_init_revisions(the_repository, &revs, عدم);
	revs.max_parents = 1;
	revs.topo_ترتيب = 1;
	لكل (سائيز_t i = 0; i < ڪل; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		جوڙيو_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_بنياد_at(&commit_بنياد, list[i]) = 1;
	}
	بنياد->object.flags |= UNINTERESTING;
	جوڙيو_pending_object(&revs, &بنياد->object, "base");

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	طالما ((commit = get_revision(&revs)) != عدم) {
		بنية object_id oid;
		بنية object_id *patch_id;
		إذا (*commit_بنياد_at(&commit_بنياد, commit))
			استمر;
		إذا (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_بنياد(&commit_بنياد);
	release_revisions(&revs);
}

ساكن فراغ print_bases(بنية بنياد_وڻ_info *bases, FILE *file)
{
	صحيح i;

	/* Only do this once, either for the cover or for the first one */
	إذا (is_null_oid(&bases->بنياد_commit))
		أرجع;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_منزل_hex(&bases->بنياد_commit));

	/* Show the prerequisite patches */
	لكل (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_منزل_hex(&bases->patch_id[i]));

	حرر(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->بنياد_commit, the_repository->hash_algo);
}

ساكن ثابت حرف *diff_title(بنية strbuf *sb,
			      ثابت حرف *reroll_ڳڻپ,
			      ثابت حرف *generic,
			      ثابت حرف *rerolled)
{
	صحيح v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	إذا (reroll_ڳڻپ && !strtol_i(reroll_ڳڻپ, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	وإلا
		strbuf_addstr(sb, generic);
	أرجع sb->buf;
}

ساكن فراغ infer_range_diff_ranges(بنية strbuf *r1,
				    بنية strbuf *r2,
				    ثابت حرف *prev,
				    بنية commit *origin,
				    بنية commit *سر)
{
	ثابت حرف *سر_oid = oid_منزل_hex(&سر->object.oid);
	صحيح prev_is_range = is_range_diff_range(prev);

	إذا (prev_is_range)
		strbuf_addstr(r1, prev);
	وإلا
		strbuf_addf(r1, "%s..%s", سر_oid, prev);

	إذا (origin)
		strbuf_addf(r2, "%s..%s", oid_منزل_hex(&origin->object.oid), سر_oid);
	وإلا إذا (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	وإلا {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, سر_oid);
	}
}

صحيح cmd_format_patch(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اڳياڙي,
		     بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية format_config cfg;
	بنية commit *commit;
	بنية commit_ڍير list = COMMIT_ڍير_INIT;
	بنية rev_info rev;
	حرف *منزل_free = عدم;
	بنية تياري_revision_opt s_r_opt;
	سائيز_t ڪل, i;
	صحيح use_stdout = 0;
	صحيح شروعات_عدد = -1;
	صحيح just_numbers = 0;
	صحيح ignore_if_اندريون_upstream = 0;
	صحيح cover_letter = -1;
	ثابت حرف *cover_letter_fmt = عدم;
	صحيح boundary_ڳڻپ = 0;
	صحيح no_ٻٽو_diff = 0;
	صحيح zero_commit = 0;
	بنية commit *origin = عدم;
	ثابت حرف *اندريون_جواب_منزل = عدم;
	بنية patch_ids ids;
	بنية strbuf buf = STRBUF_INIT;
	صحيح use_patch_format = 0;
	صحيح quiet = 0;
	ثابت حرف *reroll_ڳڻپ = عدم;
	حرف *cover_from_description_arg = عدم;
	حرف *description_file = عدم;
	حرف *branch_name = عدم;
	بنية بنياد_وڻ_info bases;
	بنية commit *بنياد;
	صحيح show_ترقي = 0;
	بنية ترقي *ترقي = عدم;
	بنية oid_array idiff_prev = OID_ARRAY_INIT;
	بنية strbuf idiff_title = STRBUF_INIT;
	ثابت حرف *rdiff_prev = عدم;
	بنية strbuf rdiff1 = STRBUF_INIT;
	بنية strbuf rdiff2 = STRBUF_INIT;
	بنية strbuf rdiff_title = STRBUF_INIT;
	بنية rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ثابت حرف *rfc = عدم;
	صحيح creation_ضرب_ڪندڙ = -1;
	ثابت حرف *signature = git_نسخو_string;
	حرف *signature_منزل_free = عدم;
	حرف *signature_file_arg = عدم;
	بنية keep_callback_معلومات keep_callback_معلومات = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ثابت حرف *fmt_patch_suffix = عدم;

	ثابت بنية option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, عدم,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, عدم,
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
		OPT_INTEGER(0, "start-number", &شروعات_عدد,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_ڳڻپ, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.لاگرٿم.fmt_patch_name_max,
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
			    PARSE_OPT_NONEG, subject_اڳياڙي_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_معلومات, عدم,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ٻٽو_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_اندريون_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_سيٽ_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_گروهه(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), سرو_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_منزل, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &اندريون_جواب_منزل, N_("message-id"),
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
			       0, بنياد_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_ترقي,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_prev, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_prev, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_ضرب_ڪندڙ,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_اندريون_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_پڇاڙي()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, اڳياڙي);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	يادگيري_نقل(&rev.notes_opt, &cfg.notes_opt, حجم(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_سرا = cfg.لاگرٿم.default_encode_email_سرا;
	rev.expand_tabs_اندريون_log_default = 0;
	rev.verbose_سرو = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, حجم(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.لاگرٿم.fmt_patch_subject_اڳياڙي);
	إذا (format_no_اڳياڙي)
		diff_سيٽ_noprefix(&rev.diffopt);

	إذا (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, اڳياڙي, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_اندريون_body_from = force_اندريون_body_from;

	إذا (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	إذا (cfg.لاگرٿم.fmt_patch_name_max <= cast_سائيز_t_منزل_int(لڙي_ڊيگهه("0000-") + لڙي_ڊيگهه(fmt_patch_suffix)))
		cfg.لاگرٿم.fmt_patch_name_max = لڙي_ڊيگهه("0000-") + لڙي_ڊيگهه(fmt_patch_suffix);

	إذا (cover_from_description_arg)
		cfg.cover_from_description_گهڻائي = parse_cover_from_description(cover_from_description_arg);

	إذا (rfc && rfc[0]) {
		cfg.subject_اڳياڙي = 1;
		إذا (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		وإلا
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	إذا (reroll_ڳڻپ) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_ڳڻپ);
		rev.reroll_ڳڻپ = reroll_ڳڻپ;
	}

	rev.subject_اڳياڙي = cfg.sprefix.buf;

	لكل (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.شيون[i].string);
		strbuf_addch(&buf, '\n');
	}

	إذا (cfg.extra_منزل.nr)
		strbuf_addstr(&buf, "To: ");
	لكل (i = 0; i < cfg.extra_منزل.nr; i++) {
		إذا (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_منزل.شيون[i].string);
		إذا (i + 1 < cfg.extra_منزل.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	إذا (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	لكل (i = 0; i < cfg.extra_cc.nr; i++) {
		إذا (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.شيون[i].string);
		إذا (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_سرا = منزل_free = strbuf_detach(&buf, عدم);

	إذا (cfg.from) {
		إذا (ورهايو_ident_سٽ(&rev.from_ident, cfg.from, لڙي_ڊيگهه(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	إذا (شروعات_عدد < 0)
		شروعات_عدد = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	إذا (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	إذا (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	إذا (cfg.keep_subject && cfg.subject_اڳياڙي)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = تياري_revisions(argc, argv, &rev, &s_r_opt);
	إذا (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	إذا (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	إذا (rev.diffopt.output_format & DIFF_FORMAT_NAME_حالت)
		die(_("--name-status does not make sense"));
	إذا (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	إذا (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	إذا (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	إذا (!rev.diffopt.stat_ويڪر)
		rev.diffopt.stat_ويڪر = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_سرو = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.لاگرٿم.fmt_patch_name_max;

	إذا (!rev.diffopt.flags.متن && !no_ٻٽو_diff)
		rev.diffopt.flags.ٻٽو = 1;

	إذا (rev.show_notes)
		ڀريو_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	إذا (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	إذا (use_stdout) {
		تياري_pager(the_repository);
	} وإلا إذا (!rev.diffopt.close_file) {
		صحيح saved;

		إذا (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = سيٽ_outdir(اڳياڙي, output_directory);

		إذا (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_سيٽ_shared_repository(the_repository, 0);
		بدل (محفوظ_create_leading_directories_const(the_repository, output_directory)) {
		حالة SCLD_OK:
		حالة SCLD_EXISTS:
			اكسر;
		افتراضي:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_سيٽ_shared_repository(the_repository, saved);
		إذا (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	إذا (rev.pending.nr == 1) {
		صحيح check_سر = 0;

		إذا (rev.max_ڳڻپ < 0 && !rev.show_پاڙ_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].شي->flags |= UNINTERESTING;
			جوڙيو_سر_منزل_pending(&rev);
			check_سر = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		إذا (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_سر = 1;

		إذا (check_سر) {
			ثابت حرف *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      عدم, عدم);
			إذا (ref && skip_اڳياڙي(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			وإلا
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_پاڙ_diff = 1;

	إذا (ignore_if_اندريون_upstream) {
		/* Don't say anything if head and upstream are the same. */
		إذا (rev.pending.nr == 2) {
			بنية object_array_داخلا *o = rev.pending.objects;
			إذا (oideq(&o[0].شي->oid, &o[1].شي->oid))
				اذهب پورو;
		}
		get_patch_ids(&rev, &ids);
	}

	إذا (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	طالما ((commit = get_revision(&rev)) != عدم) {
		إذا (commit->object.flags & BOUNDARY) {
			boundary_ڳڻپ++;
			origin = (boundary_ڳڻپ == 1) ? commit : عدم;
			استمر;
		}

		إذا (ignore_if_اندريون_upstream && has_commit_patch_id(commit, &ids))
			استمر;

		commit_ڍير_ڌڪو(&list, commit);
	}
	إذا (!list.nr)
		/* nothing to do */
		اذهب پورو;
	ڪل = list.nr;

	إذا (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		إذا (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} وإلا إذا (cover_letter == -1) {
		cover_letter = 1;
	}

	إذا (cover_letter == -1) {
		إذا (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (ڪل > 1);
		وإلا إذا ((idiff_prev.nr || rdiff_prev) && (ڪل > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		وإلا
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	إذا (!cfg.keep_subject && cfg.auto_عدد && (ڪل > 1 || cover_letter))
		cfg.numbered = 1;
	إذا (cfg.numbered)
		rev.ڪل = ڪل + شروعات_عدد - 1;

	إذا (idiff_prev.nr) {
		إذا (!cover_letter && ڪل != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_وڻ_oid(list.شيون[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_ڳڻپ,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	إذا (creation_ضرب_ڪندڙ < 0)
		creation_ضرب_ڪندڙ = CREATION_ضرب_ڪندڙ_FOR_THE_SAME_SERIES;
	وإلا إذا (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	إذا (rdiff_prev) {
		إذا (!cover_letter && ڪل != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.شيون[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_ضرب_ڪندڙ = creation_ضرب_ڪندڙ;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_ڳڻپ,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_دليلون(&rdiff_notes, &rev);
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
	إذا (!signature) {
		; /* --no-signature inhibits all signatures */
	} وإلا إذا (signature && signature != git_نسخو_string) {
		; /* non-default signature already set */
	} وإلا إذا (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		بنية strbuf buf = STRBUF_INIT;
		ثابت حرف *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		إذا (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_منزل_free = strbuf_detach(&buf, عدم);
	} وإلا إذا (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, حجم(bases));
	بنياد = get_بنياد_commit(&cfg, list.شيون, list.nr);
	إذا (بنياد) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, بنياد, list.شيون, list.nr);
	}

	إذا (اندريون_جواب_منزل || cfg.thread || cover_letter) {
		rev.ref_پيغام_ids = xmalloc(حجم(*rev.ref_پيغام_ids));
		string_list_init_dup(rev.ref_پيغام_ids);
	}
	إذا (اندريون_جواب_منزل) {
		حرف *msgid = clean_پيغام_id(اندريون_جواب_منزل);
		string_list_append_nodup(rev.ref_پيغام_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	إذا (cover_letter) {
		إذا (cfg.thread)
			gen_پيغام_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.شيون,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		ڪل++;
		شروعات_عدد--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = عدم;
		rev.rdiff1 = عدم;
	}
	rev.جوڙيو_signoff = cfg.do_signoff;

	إذا (show_ترقي)
		ترقي = شروعات_delayed_ترقي(the_repository,
						  _("Generating patches"), ڪل);
	طالما (list.nr) {
		سائيز_t idx = list.nr - 1;
		صحيح shown;

		display_ترقي(ترقي, ڪل - idx);
		commit = commit_ڍير_ڪڍو(&list);
		rev.nr = ڪل - idx + (شروعات_عدد - 1);

		/* Make the second and subsequent mails replies to the first */
		إذا (cfg.thread) {
			/* Have we already had a message ID? */
			إذا (rev.پيغام_id) {
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
				إذا (cfg.thread == THREAD_SHALLOW
				    && rev.ref_پيغام_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					حرر(rev.پيغام_id);
				وإلا
					string_list_append_nodup(rev.ref_پيغام_ids,
								 rev.پيغام_id);
			}
			gen_پيغام_id(&rev, oid_منزل_hex(&commit->object.oid));
		}

		إذا (output_directory &&
		    open_ايندڙ_file(rev.numbered_files ? عدم : commit, عدم, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_وڻ_commit(&rev, commit);
		free_commit_بفر(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		إذا (output_directory)
			rev.shown_one = 0;
		إذا (shown) {
			print_bases(&bases, rev.diffopt.file);
			إذا (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			وإلا
				print_signature(signature, rev.diffopt.file);
		}
		إذا (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = عدم;
		}
	}
	stop_ترقي(&ترقي);
	commit_ڍير_clear(&list);
	إذا (ignore_if_اندريون_upstream)
		free_patch_ids(&ids);

پورو:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	حرر(description_file);
	حرر(signature_file_arg);
	حرر(signature_منزل_free);
	حرر(branch_name);
	حرر(منزل_free);
	حرر(rev.پيغام_id);
	إذا (rev.ref_پيغام_ids)
		string_list_clear(rev.ref_پيغام_ids, 0);
	حرر(rev.ref_پيغام_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	أرجع 0;
}

ساكن صحيح جوڙيو_pending_commit(ثابت حرف *arg, بنية rev_info *revs, صحيح flags)
{
	بنية object_id oid;
	إذا (repo_get_oid(the_repository, arg, &oid) == 0) {
		بنية commit *commit = lookup_commit_reference(the_repository,
								&oid);
		إذا (commit) {
			commit->object.flags |= flags;
			جوڙيو_pending_object(revs, &commit->object, arg);
			أرجع 0;
		}
	}
	أرجع -1;
}

ساكن ثابت حرف * ثابت cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	عدم
};

ساكن فراغ print_commit(حرف sign, بنية commit *commit, صحيح verbose,
			 صحيح abbrev, FILE *file)
{
	إذا (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} وإلا {
		بنية strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		fprintf(file, "%c %s %s\n", sign,
		       repo_ڳوليو_unique_abbrev(the_repository, &commit->object.oid, abbrev),
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
ساكن فراغ collect_diff_blob_oids(بنية commit *commit,
				   بنية diff_options *opts,
				   بنية oidset *blobs)
{
	بنية diff_قطار_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	أكد(!commit->parents || !commit->parents->ايندڙ);

	إذا (commit->parents)
		diff_وڻ_oid(&commit->parents->شي->object.oid,
			      &commit->object.oid, "", opts);
	وإلا
		diff_پاڙ_وڻ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	لكل (صحيح i = 0; i < q->nr; i++) {
		بنية diff_filepair *p = q->قطار[i];
		بنية userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_ڳوليو_by_رستو(opts->repo->اشاريو, p->one->رستو);
		إذا (drv && drv->ٻٽو > 0)
			استمر;

		إذا (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, عدم,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_وجهو(blobs, &p->one->oid);
		إذا (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, عدم,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_وجهو(blobs, &p->two->oid);
	}
	diff_قطار_clear(q);
}

ساكن صحيح always_ميلاپ(ثابت فراغ *cmp_معلومات اڻ_استعمال_ٿيل,
			ثابت بنية hashmap_داخلا *entry1 اڻ_استعمال_ٿيل,
			ثابت بنية hashmap_داخلا *entry2 اڻ_استعمال_ٿيل,
			ثابت فراغ *keydata اڻ_استعمال_ٿيل)
{
	أرجع 0;
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
ساكن فراغ prefetch_cherry_blobs(بنية repository *repo,
				  بنية commit_list *list,
				  بنية patch_ids *ids)
{
	بنية oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	إذا (!repo_has_promisor_remote(repo))
		أرجع;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_ميلاپ;

	/* Find header-only collisions, gather blobs from those commits */
	لكل (بنية commit_list *l = list; l; l = l->ايندڙ) {
		بنية commit *c = l->شي;
		bool ميلاپ_مليو = false;
		لكل (بنية patch_id *cur = patch_id_iter_پهريون(c, ids);
		     cur;
		     cur = patch_id_iter_ايندڙ(cur, ids)) {
			ميلاپ_مليو = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		إذا (ميلاپ_مليو)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	إذا (oidset_سائيز(&blobs)) {
		بنية oid_array منزل_fetch = OID_ARRAY_INIT;
		بنية oidset_iter iter;
		ثابت بنية object_id *oid;

		oidset_iter_init(&blobs, &iter);
		طالما ((oid = oidset_iter_ايندڙ(&iter)))
			oid_array_append(&منزل_fetch, oid);

		promisor_remote_get_سڌو(repo, منزل_fetch.oid, منزل_fetch.nr);

		oid_array_clear(&منزل_fetch);
	}

	oidset_clear(&blobs);
}

صحيح cmd_cherry(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اڳياڙي,
	       بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية rev_info revs;
	بنية patch_ids ids;
	بنية commit *commit;
	بنية commit_list *list = عدم;
	بنية branch *current_branch;
	ثابت حرف *upstream;
	ثابت حرف *سر = "HEAD";
	ثابت حرف *حد_بندي = عدم;
	صحيح verbose = 0, abbrev = 0;

	بنية option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options, cherry_usage, 0);

	بدل (argc) {
	حالة 3:
		حد_بندي = argv[2];
		/* FALLTHROUGH */
	حالة 2:
		سر = argv[1];
		/* FALLTHROUGH */
	حالة 1:
		upstream = argv[0];
		اكسر;
	افتراضي:
		current_branch = branch_get(عدم);
		upstream = branch_get_upstream(current_branch, عدم);
		إذا (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, اڳياڙي);
	revs.max_parents = 1;

	إذا (جوڙيو_pending_commit(سر, &revs, 0))
		die(_("unknown commit %s"), سر);
	إذا (جوڙيو_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	إذا (revs.pending.nr == 2) {
		بنية object_array_داخلا *o = revs.pending.objects;
		إذا (oideq(&o[0].شي->oid, &o[1].شي->oid))
			أرجع 0;
	}

	get_patch_ids(&revs, &ids);

	إذا (حد_بندي && جوڙيو_pending_commit(حد_بندي, &revs, UNINTERESTING))
		die(_("unknown commit %s"), حد_بندي);

	/* reverse the list of commits */
	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	طالما ((commit = get_revision(&revs)) != عدم) {
		commit_list_وجهو(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	لكل (بنية commit_list *l = list; l; l = l->ايندڙ) {
		حرف sign = '+';

		commit = l->شي;
		إذا (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	أرجع 0;
}
