/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "odb/streaming.h"
#অন্তর্ভাব "pager.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "diff-merges.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "log-tree.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "oidset.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "reflog-walk.h"
#অন্তর্ভাব "patch-ids.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "shortlog.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "line-log.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "version.h"
#অন্তর্ভাব "mailmap.h"
#অন্তর্ভাব "progress.h"
#অন্তর্ভাব "commit-slab.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "utf8.h"

#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "promisor-remote.h"
#অন্তর্ভাব "range-diff.h"
#অন্তর্ভাব "tmp-objdir.h"
#অন্তর্ভাব "tree.h"
#অন্তর্ভাব "userdiff.h"
#অন্তর্ভাব "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

স্থির unsigned পূর্ণাঙ্ক force_in_body_from;
স্থির পূর্ণাঙ্ক stdout_mboxrd;
স্থির পূর্ণাঙ্ক format_no_prefix;

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	রিক্ত
};

সংরচনা সারি_opt_callback_data {
	সংরচনা rev_info *rev;
	ধ্রুব বর্ণ *prefix;
	সংরচনা string_list args;
};

স্থির পূর্ণাঙ্ক session_is_interactive(শূন্য)
{
	প্রত্যাগম isatty(1) || pager_in_use();
}

স্থির পূর্ণাঙ্ক auto_decoration_style(শূন্য)
{
	প্রত্যাগম session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

স্থির পূর্ণাঙ্ক parse_decoration_style(ধ্রুব বর্ণ *মান)
{
	বিকল্প (git_parse_maybe_bool(মান)) {
	পক্ষ 1:
		প্রত্যাগম DECORATE_SHORT_REFS;
	পক্ষ 0:
		প্রত্যাগম 0;
	স্বভাব:
		ভঙ্গ;
	}
	যদি (!strcmp(মান, "full"))
		প্রত্যাগম DECORATE_FULL_REFS;
	অন্যথা যদি (!strcmp(মান, "short"))
		প্রত্যাগম DECORATE_SHORT_REFS;
	অন্যথা যদি (!strcmp(মান, "auto"))
		প্রত্যাগম auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	প্রত্যাগম -1;
}

সংরচনা log_config {
	পূর্ণাঙ্ক default_abbrev_commit;
	পূর্ণাঙ্ক default_show_মূল;
	পূর্ণাঙ্ক default_follow;
	পূর্ণাঙ্ক default_show_signature;
	পূর্ণাঙ্ক default_encode_email_headers;
	পূর্ণাঙ্ক decoration_style;
	পূর্ণাঙ্ক decoration_given;
	পূর্ণাঙ্ক use_mailmap_config;
	বর্ণ *fmt_patch_subject_prefix;
	পূর্ণাঙ্ক fmt_patch_name_max;
	বর্ণ *fmt_pretty;
	বর্ণ *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	পূর্ণাঙ্ক i_still_use_this;
#endif
};

স্থির শূন্য log_config_init(সংরচনা log_config *cfg)
{
	memset(cfg, 0, পরিমাণ(*cfg));
	cfg->default_show_মূল = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

স্থির শূন্য log_config_release(সংরচনা log_config *cfg)
{
	মুক্ত(cfg->default_date_mode);
	মুক্ত(cfg->fmt_pretty);
	মুক্ত(cfg->fmt_patch_subject_prefix);
}

স্থির পূর্ণাঙ্ক use_default_decoration_filter = 1;
স্থির সংরচনা string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
স্থির সংরচনা string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
স্থির সংরচনা string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

স্থির পূর্ণাঙ্ক clear_decorations_callback(ধ্রুব সংরচনা option *opt UNUSED,
				      ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক decorate_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			     পূর্ণাঙ্ক unset)
{
	সংরচনা log_config *cfg = opt->মান;

	যদি (unset)
		cfg->decoration_style = 0;
	অন্যথা যদি (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	অন্যথা
		cfg->decoration_style = DECORATE_SHORT_REFS;

	যদি (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক log_সারি_range_callback(ধ্রুব সংরচনা option *option, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা সারি_opt_callback_data *data = option->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!arg)
		প্রত্যাগম -1;

	data->rev->সারি_level_traverse = 1;
	string_list_append(&data->args, arg);

	প্রত্যাগম 0;
}

স্থির শূন্য cmd_log_init_defaults(সংরচনা rev_info *rev,
				  সংরচনা log_config *cfg)
{
	যদি (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	যদি (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_মূল_diff = cfg->default_show_মূল;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	যদি (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

স্থির শূন্য set_default_decoration_filter(সংরচনা decoration_filter *decoration_filter)
{
	বর্ণ *মান = রিক্ত;
	সংরচনা string_list *include = decoration_filter->include_ref_pattern;
	ধ্রুব সংরচনা string_list *config_exclude;

	যদি (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		সংরচনা string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	যদি (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &মান) &&
	    !strcmp("all", মান))
		use_default_decoration_filter = 0;
	মুক্ত(মান);

	যদি (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		প্রত্যাগম;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	প্রতি (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		যদি (!ref_namespace[i].decoration)
			অনুবর্ত;

		string_list_append(include, ref_namespace[i].ref);
	}
}

স্থির শূন্য cmd_log_init_finish(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা rev_info *rev, সংরচনা setup_revision_opt *opt,
			 সংরচনা log_config *cfg)
{
	সংরচনা userformat_want w;
	পূর্ণাঙ্ক quiet = 0, source = 0, mailmap;
	স্থির সংরচনা সারি_opt_callback_data সারি_cb = {রিক্ত, রিক্ত, STRING_LIST_INIT_DUP};
	সংরচনা decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	স্থির সংরচনা revision_sources revision_sources;

	ধ্রুব সংরচনা option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", রিক্ত, রিক্ত,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, রিক্ত, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', রিক্ত, &সারি_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_সারি_range_callback),
		OPT_END()
	};

	সারি_cb.rev = rev;
	সারি_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	যদি (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	যদি (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	যদি (rev->সারি_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, পরিমাণ(w));
	userformat_find_requirements(রিক্ত, &w);

	যদি (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	যদি (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	যদি ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	যদি (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	যদি (mailmap) {
		rev->mailmap = xmalloc(পরিমাণ(সংরচনা string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	যদি (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		যদি (!cfg->decoration_given)
			cfg->decoration_style = 0;
		যদি (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	যদি (rev->commit_format == CMIT_FMT_USERFORMAT) {
		যদি (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} অন্যথা যদি (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	যদি (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		যদি (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	যদি (rev->সারি_level_traverse)
		সারি_log_init(rev, সারি_cb.prefix, &সারি_cb.args);

	setup_pager(the_repository);
}

স্থির শূন্য cmd_log_init(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			 সংরচনা rev_info *rev, সংরচনা setup_revision_opt *opt,
			 সংরচনা log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

স্থির পূর্ণাঙ্ক cmd_log_walk_no_free(সংরচনা rev_info *rev)
{
	সংরচনা commit *commit;
	পূর্ণাঙ্ক saved_nrl = 0;
	পূর্ণাঙ্ক saved_dcctc = 0;
	পূর্ণাঙ্ক result;

	যদি (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	যাবত্ ((commit = get_revision(rev)) != রিক্ত) {
		যদি (!log_বৃক্ষ_commit(rev, commit) && rev->max_গুনতি >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_গুনতি++;
		যদি (!rev->reflog_info && !rev->remerge_diff) {
			/*
			 * We may show a given commit multiple times when
			 * walking the reflogs. Therefore we still need it.
			 *
			 * Likewise, we potentially still need the parents
			 * of * already shown commits to determine merge
			 * bases when showing remerge diffs.
			 */
			free_commit_buffer(the_repository->parsed_objects,
					   commit);
			commit_list_free(commit->parents);
			commit->parents = রিক্ত;
		}
		যদি (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		যদি (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	যদি (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	প্রত্যাগম result;
}

স্থির পূর্ণাঙ্ক cmd_log_walk(সংরচনা rev_info *rev)
{
	পূর্ণাঙ্ক retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	প্রত্যাগম retval;
}

স্থির পূর্ণাঙ্ক git_log_config(ধ্রুব বর্ণ *var, ধ্রুব বর্ণ *মান,
			  ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	সংরচনা log_config *cfg = cb;
	ধ্রুব বর্ণ *slot_name;

	যদি (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		প্রত্যাগম git_config_string(&cfg->fmt_pretty, var, মান);
	}
	যদি (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		প্রত্যাগম git_config_string(&cfg->fmt_patch_subject_prefix, var, মান);
	}
	যদি (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, মান, ctx->kvi);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		প্রত্যাগম git_config_string(&cfg->default_date_mode, var, মান);
	}
	যদি (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(মান);
		যদি (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "log.diffmerges")) {
		যদি (!মান)
			প্রত্যাগম config_error_nonbool(var);
		প্রত্যাগম diff_merges_config(মান);
	}
	যদি (!strcmp(var, "log.showroot")) {
		cfg->default_show_মূল = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (skip_prefix(var, "color.decorate.", &slot_name))
		প্রত্যাগম parse_decorate_color_config(var, slot_name, মান);
	যদি (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}

	প্রত্যাগম git_diff_ui_config(var, মান, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
পূর্ণাঙ্ক cmd_whatchanged(পূর্ণাঙ্ক argc,
		    ধ্রুব বর্ণ **argv,
		    ধ্রুব বর্ণ *prefix,
		    সংরচনা repository *repo UNUSED)
{
	সংরচনা log_config cfg;
	সংরচনা rev_info rev;
	সংরচনা setup_revision_opt opt;
	পূর্ণাঙ্ক ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, পরিমাণ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	যদি (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	যদি (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	প্রত্যাগম ret;
}
#endif

স্থির শূন্য show_tagger(ধ্রুব বর্ণ *buf, সংরচনা rev_info *rev)
{
	সংরচনা strbuf out = STRBUF_INIT;
	সংরচনা pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

স্থির পূর্ণাঙ্ক show_blob_object(ধ্রুব সংরচনা object_id *oid, সংরচনা rev_info *rev, ধ্রুব বর্ণ *obj_name)
{
	সংরচনা object_id oidc;
	সংরচনা object_context obj_context = {0};
	বর্ণ *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	যদি (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		প্রত্যাগম odb_stream_blob_to_fd(the_repository->objects, 1, oid, রিক্ত, 0);

	যদি (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	যদি (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		প্রত্যাগম odb_stream_blob_to_fd(the_repository->objects, 1, oid, রিক্ত, 0);
	}

	যদি (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	মুক্ত(buf);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_tag_object(ধ্রুব সংরচনা object_id *oid, সংরচনা rev_info *rev)
{
	size_t size;
	গণনা object_type type;
	বর্ণ *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	যদি (!buf)
		প্রত্যাগম error(_("could not read object %s"), oid_to_hex(oid));

	প্রতিজ্ঞা(type == OBJ_TAG);
	যাবত্ (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		ধ্রুব বর্ণ *ident;
		যাবত্ (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		যদি (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	যদি (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	মুক্ত(buf);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_বৃক্ষ_object(ধ্রুব সংরচনা object_id *oid UNUSED,
			    সংরচনা strbuf *base UNUSED,
			    ধ্রুব বর্ণ *pathname, unsigned mode,
			    শূন্য *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	প্রত্যাগম 0;
}

স্থির শূন্য show_setup_revisions_tweak(সংরচনা rev_info *rev)
{
	যদি (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	অন্যথা
		diff_merges_default_to_dense_combined(rev);
	যদি (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

পূর্ণাঙ্ক cmd_show(পূর্ণাঙ্ক argc,
	     ধ্রুব বর্ণ **argv,
	     ধ্রুব বর্ণ *prefix,
	     সংরচনা repository *repo UNUSED)
{
	সংরচনা log_config cfg;
	সংরচনা rev_info rev;
	unsigned পূর্ণাঙ্ক i;
	সংরচনা setup_revision_opt opt;
	সংরচনা pathspec match_all;
	পূর্ণাঙ্ক ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	যদি (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, পরিমাণ(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, পরিমাণ(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	যদি (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		প্রত্যাগম ret;
	}

	rev.diffopt.no_free = 1;
	প্রতি (i = 0; i < rev.pending.nr && !ret; i++) {
		সংরচনা object *o = rev.pending.objects[i].item;
		ধ্রুব বর্ণ *name = rev.pending.objects[i].name;
		বিকল্প (o->type) {
		পক্ষ OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			ভঙ্গ;
		পক্ষ OBJ_TAG: {
			সংরচনা tag *t = (সংরচনা tag *)o;
			সংরচনা object_id *oid = get_tagged_oid(t);

			যদি (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			যদি (ret)
				ভঙ্গ;
			o = parse_object(the_repository, oid);
			যদি (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			ভঙ্গ;
		}
		পক্ষ OBJ_বৃক্ষ:
			যদি (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_বৃক্ষ(the_repository, (সংরচনা বৃক্ষ *)o,
				  &match_all, show_বৃক্ষ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			ভঙ্গ;
		পক্ষ OBJ_COMMIT:
		{
			সংরচনা object_array old;
			সংরচনা object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, পরিমাণ(old));
			memcpy(&rev.pending, &blank, পরিমাণ(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, পরিমাণ(rev.pending));
			ভঙ্গ;
		}
		স্বভাব:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	প্রত্যাগম ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
পূর্ণাঙ্ক cmd_log_reflog(পূর্ণাঙ্ক argc,
		   ধ্রুব বর্ণ **argv,
		   ধ্রুব বর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	সংরচনা log_config cfg;
	সংরচনা rev_info rev;
	সংরচনা setup_revision_opt opt;
	পূর্ণাঙ্ক ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, পরিমাণ(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_header = 1;
	cmd_log_init_finish(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	প্রত্যাগম ret;
}

স্থির শূন্য log_setup_revisions_tweak(সংরচনা rev_info *rev)
{
	যদি (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	যদি (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

পূর্ণাঙ্ক cmd_log(পূর্ণাঙ্ক argc,
	    ধ্রুব বর্ণ **argv,
	    ধ্রুব বর্ণ *prefix,
	    সংরচনা repository *repo UNUSED)
{
	সংরচনা log_config cfg;
	সংরচনা rev_info rev;
	সংরচনা setup_revision_opt opt;
	পূর্ণাঙ্ক ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, পরিমাণ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	প্রত্যাগম ret;
}

/* format-patch */

গণনা cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

গণনা thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

গণনা cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

গণনা auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

সংরচনা format_config {
	সংরচনা log_config log;
	গণনা thread_level thread;
	পূর্ণাঙ্ক do_signoff;
	গণনা auto_base_setting auto_base;
	বর্ণ *base_commit;
	বর্ণ *from;
	বর্ণ *signature;
	বর্ণ *signature_file;
	গণনা cover_setting config_cover_letter;
	বর্ণ *fmt_cover_letter_commit_list;
	বর্ণ *config_output_directory;
	গণনা cover_from_description cover_from_description_mode;
	পূর্ণাঙ্ক show_notes;
	সংরচনা display_notes_opt notes_opt;
	পূর্ণাঙ্ক numbered_cmdline_opt;
	পূর্ণাঙ্ক numbered;
	পূর্ণাঙ্ক auto_সংখ্যা;
	বর্ণ *default_attach;
	সংরচনা string_list extra_hdr;
	সংরচনা string_list extra_to;
	সংরচনা string_list extra_cc;
	পূর্ণাঙ্ক keep_subject;
	পূর্ণাঙ্ক subject_prefix;
	সংরচনা strbuf sprefix;
	বর্ণ *fmt_patch_suffix;
};

স্থির শূন্য format_config_init(সংরচনা format_config *cfg)
{
	memset(cfg, 0, পরিমাণ(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_সংখ্যা = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

স্থির শূন্য format_config_release(সংরচনা format_config *cfg)
{
	log_config_release(&cfg->log);
	মুক্ত(cfg->base_commit);
	মুক্ত(cfg->from);
	মুক্ত(cfg->signature);
	মুক্ত(cfg->signature_file);
	মুক্ত(cfg->config_output_directory);
	মুক্ত(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	মুক্ত(cfg->fmt_patch_suffix);
	মুক্ত(cfg->fmt_cover_letter_commit_list);
}

স্থির গণনা cover_from_description parse_cover_from_description(ধ্রুব বর্ণ *arg)
{
	যদি (!arg || !strcmp(arg, "default"))
		প্রত্যাগম COVER_FROM_MESSAGE;
	অন্যথা যদি (!strcmp(arg, "none"))
		প্রত্যাগম COVER_FROM_NONE;
	অন্যথা যদি (!strcmp(arg, "message"))
		প্রত্যাগম COVER_FROM_MESSAGE;
	অন্যথা যদি (!strcmp(arg, "subject"))
		প্রত্যাগম COVER_FROM_SUBJECT;
	অন্যথা যদি (!strcmp(arg, "auto"))
		প্রত্যাগম COVER_FROM_AUTO;
	অন্যথা
		die(_("%s: invalid cover from description mode"), arg);
}

স্থির শূন্য add_header(সংরচনা format_config *cfg, ধ্রুব বর্ণ *মান)
{
	সংরচনা string_list_item *item;
	পূর্ণাঙ্ক len = strlen(মান);
	যাবত্ (len && মান[len - 1] == '\n')
		len--;

	যদি (!strncasecmp(মান, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, মান + 4);
		len -= 4;
	} অন্যথা যদি (!strncasecmp(মান, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, মান + 4);
		len -= 4;
	} অন্যথা {
		item = string_list_append(&cfg->extra_hdr, মান);
	}

	item->string[len] = '\0';
}

স্থির পূর্ণাঙ্ক git_format_config(ধ্রুব বর্ণ *var, ধ্রুব বর্ণ *মান,
			     ধ্রুব সংরচনা config_context *ctx, শূন্য *cb)
{
	সংরচনা format_config *cfg = cb;

	যদি (!strcmp(var, "format.headers")) {
		যদি (!মান)
			die(_("format.headers without value"));
		add_header(cfg, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		প্রত্যাগম git_config_string(&cfg->fmt_patch_suffix, var, মান);
	}
	যদি (!strcmp(var, "format.to")) {
		যদি (!মান)
			প্রত্যাগম config_error_nonbool(var);
		string_list_append(&cfg->extra_to, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.cc")) {
		যদি (!মান)
			প্রত্যাগম config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.numbered")) {
		যদি (মান && !strcasecmp(মান, "auto")) {
			cfg->auto_সংখ্যা = 1;
			প্রত্যাগম 0;
		}
		cfg->numbered = git_config_bool(var, মান);
		cfg->auto_সংখ্যা = cfg->auto_সংখ্যা && cfg->numbered;
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.attach")) {
		যদি (মান && *মান) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(মান);
		} অন্যথা যদি (মান && !*মান) {
			FREE_AND_NULL(cfg->default_attach);
		} অন্যথা {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.thread")) {
		যদি (মান && !strcasecmp(মান, "deep")) {
			cfg->thread = THREAD_DEEP;
			প্রত্যাগম 0;
		}
		যদি (মান && !strcasecmp(মান, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			প্রত্যাগম 0;
		}
		cfg->thread = git_config_bool(var, মান) ? THREAD_SHALLOW : THREAD_UNSET;
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		প্রত্যাগম git_config_string(&cfg->signature, var, মান);
	}
	যদি (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		প্রত্যাগম git_config_pathname(&cfg->signature_file, var, মান);
	}
	যদি (!strcmp(var, "format.coverletter")) {
		যদি (মান && !strcasecmp(মান, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			প্রত্যাগম 0;
		}
		cfg->config_cover_letter = git_config_bool(var, মান) ? COVER_ON : COVER_OFF;
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		প্রত্যাগম git_config_string(&cfg->fmt_cover_letter_commit_list, var, মান);
	}
	যদি (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		প্রত্যাগম git_config_string(&cfg->config_output_directory, var, মান);
	}
	যদি (!strcmp(var, "format.useautobase")) {
		যদি (মান && !strcasecmp(মান, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			প্রত্যাগম 0;
		}
		cfg->auto_base = git_config_bool(var, মান) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.from")) {
		পূর্ণাঙ্ক b = git_parse_maybe_bool(মান);
		FREE_AND_NULL(cfg->from);
		যদি (b < 0)
			cfg->from = xstrdup(মান);
		অন্যথা যদি (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.notes")) {
		পূর্ণাঙ্ক b = git_parse_maybe_bool(মান);
		যদি (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, মান);
		অন্যথা যদি (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		অন্যথা
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, মান);
		প্রত্যাগম 0;
	}
	যদি (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(মান);
		যদি (format_no_prefix < 0) {
			পূর্ণাঙ্ক status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				মান, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		প্রত্যাগম 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	যদি (!strcmp(var, "diff.noprefix"))
		প্রত্যাগম 0;

	প্রত্যাগম git_log_config(var, মান, ctx, &cfg->log);
}

স্থির ধ্রুব বর্ণ *output_directory = রিক্ত;
স্থির পূর্ণাঙ্ক outdir_offset;

স্থির পূর্ণাঙ্ক open_next_file(সংরচনা commit *commit, ধ্রুব বর্ণ *subject,
			 সংরচনা rev_info *rev, পূর্ণাঙ্ক quiet)
{
	সংরচনা strbuf filename = STRBUF_INIT;

	যদি (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	যদি (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	অন্যথা যদি (commit)
		fmt_output_commit(&filename, commit, rev);
	অন্যথা
		fmt_output_subject(&filename, subject, rev);

	যদি (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	যদি (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		প্রত্যাগম -1;
	}

	strbuf_release(&filename);
	প্রত্যাগম 0;
}

স্থির শূন্য get_patch_ids(সংরচনা rev_info *rev, সংরচনা patch_ids *ids)
{
	সংরচনা rev_info check_rev;
	সংরচনা commit *commit, *c1, *c2;
	সংরচনা object *o1, *o2;
	unsigned flags1, flags2;

	যদি (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	যদি ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	যদি (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	যাবত্ ((commit = get_revision(&check_rev)) != রিক্ত) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

স্থির শূন্য gen_message_id(সংরচনা rev_info *info, ধ্রুব বর্ণ *base)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(রিক্ত),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, রিক্ত);
}

স্থির শূন্য print_signature(ধ্রুব বর্ণ *signature, FILE *file)
{
	যদি (!signature || !*signature)
		প্রত্যাগম;

	fprintf(file, "-- \n%s", signature);
	যদি (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

স্থির বর্ণ *find_branch_name(সংরচনা rev_info *rev)
{
	সংরচনা object_id branch_oid;
	ধ্রুব সংরচনা object_id *tip_oid;
	ধ্রুব বর্ণ *ref, *v;
	বর্ণ *full_ref, *branch = রিক্ত;
	পূর্ণাঙ্ক interesting_found = 0;
	size_t idx;

	প্রতি (size_t i = 0; i < rev->cmdline.nr; i++) {
		যদি (rev->cmdline.rev[i].flags & UNINTERESTING)
			অনুবর্ত;
		যদি (interesting_found)
			প্রত্যাগম রিক্ত;
		interesting_found = 1;
		idx = i;
	}
	যদি (!interesting_found)
		প্রত্যাগম রিক্ত;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	যদি (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	মুক্ত(full_ref);
	প্রত্যাগম branch;
}

স্থির শূন্য show_diffstat(সংরচনা rev_info *rev,
			  সংরচনা commit *origin, সংরচনা commit *head)
{
	সংরচনা diff_options opts;

	memcpy(&opts, &rev->diffopt, পরিমাণ(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_বৃক্ষ_oid(get_commit_বৃক্ষ_oid(origin),
		      get_commit_বৃক্ষ_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

স্থির শূন্য read_desc_file(সংরচনা strbuf *buf, ধ্রুব বর্ণ *desc_file)
{
	যদি (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

স্থির শূন্য prepare_cover_text(সংরচনা pretty_print_context *pp,
			       ধ্রুব বর্ণ *description_file,
			       ধ্রুব বর্ণ *branch_name,
			       সংরচনা strbuf *sb,
			       ধ্রুব বর্ণ *encoding,
			       পূর্ণাঙ্ক need_8bit_cte,
			       ধ্রুব সংরচনা format_config *cfg)
{
	ধ্রুব বর্ণ *subject = "*** SUBJECT HERE ***";
	ধ্রুব বর্ণ *body = "*** BLURB HERE ***";
	সংরচনা strbuf description_sb = STRBUF_INIT;
	সংরচনা strbuf subject_sb = STRBUF_INIT;

	যদি (cfg->cover_from_description_mode == COVER_FROM_NONE)
		গচ্ছ do_pp;

	যদি (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	অন্যথা যদি (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	যদি (!description_sb.len)
		গচ্ছ do_pp;

	যদি (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	যদি (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	অন্যথা
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

সংরচনা rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	সংরচনা string_list notes;
};

স্থির পূর্ণাঙ্ক rdiff_notes_cb(ধ্রুব সংরচনা option *option,
			  ধ্রুব বর্ণ *arg,
			  পূর্ণাঙ্ক unset)
{
	সংরচনা option opt = *option;
	সংরচনা rdiff_notes *rdiff_notes = option->মান;

	rdiff_notes->override = 1;
	opt.মান = &rdiff_notes->notes;
	প্রত্যাগম parse_opt_string_list(&opt, arg, unset);
}

স্থির পূর্ণাঙ্ক get_notes_refs(সংরচনা string_list_item *item, শূন্য *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	প্রত্যাগম 0;
}

স্থির শূন্য get_notes_args(সংরচনা rdiff_notes *rdiff_notes,
			   সংরচনা rev_info *rev)
{
	যদি (rdiff_notes->override) {
		যদি (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		অন্যথা
			strvec_ঠেলো(&rev->rdiff_log_arg, "--no-notes");
	} অন্যথা যদি (!rev->show_notes) {
		strvec_ঠেলো(&rev->rdiff_log_arg, "--no-notes");
	} অন্যথা যদি (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ঠেলো(&rev->rdiff_log_arg, "--notes");
	} অন্যথা {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

স্থির শূন্য generate_shortlog_cover_letter(সংরচনা shortlog *log,
					   সংরচনা rev_info *rev,
					   সংরচনা commit **list,
					   পূর্ণাঙ্ক nr)
{
	shortlog_init(log);
	log->wrap_সারিগুলি = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	প্রতি (পূর্ণাঙ্ক i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

স্থির শূন্য generate_commit_list_cover(FILE *cover_file, ধ্রুব বর্ণ *format,
				       সংরচনা commit **list, পূর্ণাঙ্ক n)
{
	সংরচনা strbuf commit_সারি = STRBUF_INIT;
	সংরচনা pretty_print_context ctx = {0};
	সংরচনা rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	প্রতি (পূর্ণাঙ্ক i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_সারি, &ctx);
		fprintf(cover_file, "%s\n", commit_সারি.buf);
		strbuf_reset(&commit_সারি);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_সারি);
}

স্থির শূন্য make_cover_letter(সংরচনা rev_info *rev, পূর্ণাঙ্ক use_separate_file,
			      সংরচনা commit *origin,
			      পূর্ণাঙ্ক nr, সংরচনা commit **list,
			      ধ্রুব বর্ণ *description_file,
			      ধ্রুব বর্ণ *branch_name,
			      পূর্ণাঙ্ক quiet,
			      ধ্রুব সংরচনা format_config *cfg,
			      ধ্রুব বর্ণ *format)
{
	ধ্রুব বর্ণ *from;
	সংরচনা shortlog log;
	সংরচনা strbuf sb = STRBUF_INIT;
	পূর্ণাঙ্ক i;
	ধ্রুব বর্ণ *encoding = "UTF-8";
	পূর্ণাঙ্ক need_8bit_cte = 0;
	সংরচনা pretty_print_context pp = {0};
	সংরচনা commit *head = list[0];
	বর্ণ *to_free = রিক্ত;

	যদি (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	যদি (use_separate_file &&
	    open_next_file(রিক্ত, rev->numbered_files ? রিক্ত : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	প্রতি (i = 0; !need_8bit_cte && i < nr; i++) {
		ধ্রুব বর্ণ *buf = repo_get_commit_buffer(the_repository,
							 list[i], রিক্ত);
		যদি (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	যদি (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, রিক্ত, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	মুক্ত(to_free);
	মুক্ত(pp.after_subject);
	strbuf_release(&sb);

	যদি (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	অন্যথা যদি (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	অন্যথা যদি (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	অন্যথা যদি (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	অন্যথা
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	যদি (origin)
		show_diffstat(rev, origin, head);

	যদি (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	যদি (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		সংরচনা diff_options opts;
		সংরচনা range_diff_options range_diff_opts = {
			.creation_factor = rev->creation_factor,
			.dual_color = 1,
			.max_memory = RANGE_DIFF_MAX_MEMORY_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_setup(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_setup_done(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

স্থির বর্ণ *clean_message_id(ধ্রুব বর্ণ *বার্তা_id)
{
	বর্ণ ch;
	ধ্রুব বর্ণ *a, *z, *m;

	m = বার্তা_id;
	যাবত্ ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = রিক্ত;
	যাবত্ ((ch = *m)) {
		যদি (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	যদি (!z)
		die(_("insane in-reply-to: %s"), বার্তা_id);
	যদি (++z == m)
		প্রত্যাগম xstrdup(a);
	প্রত্যাগম xmemdupz(a, z - a);
}

স্থির ধ্রুব বর্ণ *set_outdir(ধ্রুব বর্ণ *prefix, ধ্রুব বর্ণ *output_directory)
{
	যদি (output_directory && is_absolute_path(output_directory))
		প্রত্যাগম output_directory;

	যদি (!prefix || !*prefix) {
		যদি (output_directory)
			প্রত্যাগম output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		প্রত্যাগম "./";
	}

	outdir_offset = strlen(prefix);
	যদি (!output_directory)
		প্রত্যাগম prefix;

	প্রত্যাগম prefix_filename(prefix, output_directory);
}

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	রিক্ত
};

সংরচনা keep_callback_data {
	সংরচনা format_config *cfg;
	সংরচনা rev_info *revs;
};

স্থির পূর্ণাঙ্ক keep_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা keep_callback_data *data = opt->মান;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক subject_prefix_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			    পূর্ণাঙ্ক unset)
{
	সংরচনা format_config *cfg = opt->মান;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক rfc_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			পূর্ণাঙ্ক unset)
{
	ধ্রুব বর্ণ **rfc = opt->মান;

	*rfc = opt->মান;
	যদি (unset)
		*rfc = রিক্ত;
	অন্যথা
		*rfc = arg ? arg : "RFC";
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক numbered_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			     পূর্ণাঙ্ক unset)
{
	সংরচনা format_config *cfg = opt->মান;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	যদি (unset)
		cfg->auto_সংখ্যা =  0;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক no_numbered_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
				পূর্ণাঙ্ক unset)
{
	BUG_ON_OPT_NEG(unset);
	প্রত্যাগম numbered_callback(opt, arg, 1);
}

স্থির পূর্ণাঙ্ক output_directory_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			      পূর্ণাঙ্ক unset)
{
	ধ্রুব বর্ণ **dir = (ধ্রুব বর্ণ **)opt->মান;
	BUG_ON_OPT_NEG(unset);
	যদি (*dir)
		die(_("two output directories?"));
	*dir = arg;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক thread_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা format_config *cfg = opt->মান;

	যদি (unset)
		cfg->thread = THREAD_UNSET;
	অন্যথা যদি (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	অন্যথা যদি (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	অন্যথা
		প্রত্যাগম 1;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক attach_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা rev_info *rev = (সংরচনা rev_info *)opt->মান;
	যদি (unset)
		rev->mime_boundary = রিক্ত;
	অন্যথা যদি (arg)
		rev->mime_boundary = arg;
	অন্যথা
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক inline_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা rev_info *rev = (সংরচনা rev_info *)opt->মান;
	যদি (unset)
		rev->mime_boundary = রিক্ত;
	অন্যথা যদি (arg)
		rev->mime_boundary = arg;
	অন্যথা
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক header_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			   পূর্ণাঙ্ক unset)
{
	সংরচনা format_config *cfg = opt->মান;

	যদি (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} অন্যথা {
		add_header(cfg, arg);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক from_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	বর্ণ **from = opt->মান;

	মুক্ত(*from);

	যদি (unset)
		*from = রিক্ত;
	অন্যথা যদি (arg)
		*from = xstrdup(arg);
	অন্যথা
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক base_callback(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা format_config *cfg = opt->মান;

	যদি (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} অন্যথা যদি (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} অন্যথা {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	প্রত্যাগম 0;
}

সংরচনা base_বৃক্ষ_info {
	সংরচনা object_id base_commit;
	পূর্ণাঙ্ক nr_patch_id, alloc_patch_id;
	সংরচনা object_id *patch_id;
};

স্থির সংরচনা commit *get_base_commit(ধ্রুব সংরচনা format_config *cfg,
				      সংরচনা commit **list,
				      size_t total)
{
	সংরচনা commit *base = রিক্ত;
	সংরচনা commit **rev;
	পূর্ণাঙ্ক auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	বিকল্প (cfg->auto_base) {
	পক্ষ AUTO_BASE_NEVER:
		যদি (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} অন্যথা {
			/* no base information is requested */
			প্রত্যাগম রিক্ত;
		}
		ভঙ্গ;
	পক্ষ AUTO_BASE_ALWAYS:
	পক্ষ AUTO_BASE_WHEN_ABLE:
		যদি (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} অন্যথা {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		ভঙ্গ;
	স্বভাব:
		BUG("unexpected automatic base selection method");
	}

	যদি (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		যদি (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} অন্যথা {
		সংরচনা branch *curr_branch = branch_get(রিক্ত);
		ধ্রুব বর্ণ *upstream = branch_get_upstream(curr_branch, রিক্ত);
		যদি (upstream) {
			সংরচনা commit_list *base_list = রিক্ত;
			সংরচনা commit *commit;
			সংরচনা object_id oid;

			যদি (repo_get_oid(the_repository, upstream, &oid)) {
				যদি (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				অন্যথা
					প্রত্যাগম রিক্ত;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			যদি (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				যদি (die_on_failure) {
					die(_("could not find exact merge base"));
				} অন্যথা {
					commit_list_free(base_list);
					প্রত্যাগম রিক্ত;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} অন্যথা {
			যদি (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			অন্যথা
				প্রত্যাগম রিক্ত;
		}
	}

	ALLOC_ARRAY(rev, total);
	প্রতি (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	যাবত্ (rev_nr > 1) {
		প্রতি (i = 0; i < rev_nr / 2; i++) {
			সংরচনা commit_list *merge_base = রিক্ত;
			যদি (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				যদি (die_on_failure) {
					die(_("failed to find exact merge base"));
				} অন্যথা {
					commit_list_free(merge_base);
					মুক্ত(rev);
					প্রত্যাগম রিক্ত;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		যদি (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	যদি (ret < 0)
		exit(128);
	যদি (!ret) {
		যদি (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} অন্যথা {
			মুক্ত(rev);
			প্রত্যাগম রিক্ত;
		}
	}

	প্রতি (i = 0; i < total; i++) {
		যদি (base == list[i]) {
			যদি (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} অন্যথা {
				মুক্ত(rev);
				প্রত্যাগম রিক্ত;
			}
		}
	}

	মুক্ত(rev);
	প্রত্যাগম base;
}

define_commit_slab(commit_base, পূর্ণাঙ্ক);

স্থির শূন্য prepare_bases(সংরচনা base_বৃক্ষ_info *bases,
			  সংরচনা commit *base,
			  সংরচনা commit **list,
			  size_t total)
{
	সংরচনা commit *commit;
	সংরচনা rev_info revs;
	সংরচনা diff_options diffopt;
	সংরচনা commit_base commit_base;

	যদি (!base)
		প্রত্যাগম;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, রিক্ত);
	revs.max_parents = 1;
	revs.topo_order = 1;
	প্রতি (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	যদি (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	যাবত্ ((commit = get_revision(&revs)) != রিক্ত) {
		সংরচনা object_id oid;
		সংরচনা object_id *patch_id;
		যদি (*commit_base_at(&commit_base, commit))
			অনুবর্ত;
		যদি (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

স্থির শূন্য print_bases(সংরচনা base_বৃক্ষ_info *bases, FILE *file)
{
	পূর্ণাঙ্ক i;

	/* Only do this once, either for the cover or for the first one */
	যদি (is_null_oid(&bases->base_commit))
		প্রত্যাগম;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	প্রতি (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	মুক্ত(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

স্থির ধ্রুব বর্ণ *diff_title(সংরচনা strbuf *sb,
			      ধ্রুব বর্ণ *reroll_গুনতি,
			      ধ্রুব বর্ণ *generic,
			      ধ্রুব বর্ণ *rerolled)
{
	পূর্ণাঙ্ক v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	যদি (reroll_গুনতি && !strtol_i(reroll_গুনতি, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	অন্যথা
		strbuf_addstr(sb, generic);
	প্রত্যাগম sb->buf;
}

স্থির শূন্য infer_range_diff_ranges(সংরচনা strbuf *r1,
				    সংরচনা strbuf *r2,
				    ধ্রুব বর্ণ *prev,
				    সংরচনা commit *origin,
				    সংরচনা commit *head)
{
	ধ্রুব বর্ণ *head_oid = oid_to_hex(&head->object.oid);
	পূর্ণাঙ্ক prev_is_range = is_range_diff_range(prev);

	যদি (prev_is_range)
		strbuf_addstr(r1, prev);
	অন্যথা
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	যদি (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	অন্যথা যদি (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	অন্যথা {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

পূর্ণাঙ্ক cmd_format_patch(পূর্ণাঙ্ক argc,
		     ধ্রুব বর্ণ **argv,
		     ধ্রুব বর্ণ *prefix,
		     সংরচনা repository *repo UNUSED)
{
	সংরচনা format_config cfg;
	সংরচনা commit *commit;
	সংরচনা commit_স্তূপ list = COMMIT_স্তূপ_INIT;
	সংরচনা rev_info rev;
	বর্ণ *to_free = রিক্ত;
	সংরচনা setup_revision_opt s_r_opt;
	size_t total, i;
	পূর্ণাঙ্ক use_stdout = 0;
	পূর্ণাঙ্ক start_সংখ্যা = -1;
	পূর্ণাঙ্ক just_numbers = 0;
	পূর্ণাঙ্ক ignore_if_in_upstream = 0;
	পূর্ণাঙ্ক cover_letter = -1;
	ধ্রুব বর্ণ *cover_letter_fmt = রিক্ত;
	পূর্ণাঙ্ক boundary_গুনতি = 0;
	পূর্ণাঙ্ক no_দ্বিমিক_diff = 0;
	পূর্ণাঙ্ক zero_commit = 0;
	সংরচনা commit *origin = রিক্ত;
	ধ্রুব বর্ণ *in_reply_to = রিক্ত;
	সংরচনা patch_ids ids;
	সংরচনা strbuf buf = STRBUF_INIT;
	পূর্ণাঙ্ক use_patch_format = 0;
	পূর্ণাঙ্ক quiet = 0;
	ধ্রুব বর্ণ *reroll_গুনতি = রিক্ত;
	বর্ণ *cover_from_description_arg = রিক্ত;
	বর্ণ *description_file = রিক্ত;
	বর্ণ *branch_name = রিক্ত;
	সংরচনা base_বৃক্ষ_info bases;
	সংরচনা commit *base;
	পূর্ণাঙ্ক show_progress = 0;
	সংরচনা progress *progress = রিক্ত;
	সংরচনা oid_array idiff_prev = OID_ARRAY_INIT;
	সংরচনা strbuf idiff_title = STRBUF_INIT;
	ধ্রুব বর্ণ *rdiff_prev = রিক্ত;
	সংরচনা strbuf rdiff1 = STRBUF_INIT;
	সংরচনা strbuf rdiff2 = STRBUF_INIT;
	সংরচনা strbuf rdiff_title = STRBUF_INIT;
	সংরচনা rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ধ্রুব বর্ণ *rfc = রিক্ত;
	পূর্ণাঙ্ক creation_factor = -1;
	ধ্রুব বর্ণ *signature = git_version_string;
	বর্ণ *signature_to_free = রিক্ত;
	বর্ণ *signature_file_arg = রিক্ত;
	সংরচনা keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ধ্রুব বর্ণ *fmt_patch_suffix = রিক্ত;

	ধ্রুব সংরচনা option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, রিক্ত,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, রিক্ত,
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
		OPT_INTEGER(0, "start-number", &start_সংখ্যা,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_গুনতি, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.log.fmt_patch_name_max,
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
			    PARSE_OPT_NONEG, subject_prefix_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, রিক্ত,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_দ্বিমিক_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_in_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_SET_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_GROUP(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), header_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_to, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &in_reply_to, N_("message-id"),
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
			       0, base_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_progress,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_prev, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_prev, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_factor,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_in_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_END()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	memcpy(&rev.notes_opt, &cfg.notes_opt, পরিমাণ(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, পরিমাণ(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	যদি (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	যদি (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, prefix, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_in_body_from = force_in_body_from;

	যদি (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	যদি (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	যদি (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	যদি (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		যদি (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		অন্যথা
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	যদি (reroll_গুনতি) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_গুনতি);
		rev.reroll_গুনতি = reroll_গুনতি;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	প্রতি (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.বস্তুসমূহ[i].string);
		strbuf_addch(&buf, '\n');
	}

	যদি (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	প্রতি (i = 0; i < cfg.extra_to.nr; i++) {
		যদি (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.বস্তুসমূহ[i].string);
		যদি (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	যদি (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	প্রতি (i = 0; i < cfg.extra_cc.nr; i++) {
		যদি (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.বস্তুসমূহ[i].string);
		যদি (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, রিক্ত);

	যদি (cfg.from) {
		যদি (split_ident_সারি(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	যদি (start_সংখ্যা < 0)
		start_সংখ্যা = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	যদি (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	যদি (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	যদি (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	যদি (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	যদি (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	যদি (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	যদি (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	যদি (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	যদি (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	যদি (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	যদি (!rev.diffopt.flags.text && !no_দ্বিমিক_diff)
		rev.diffopt.flags.দ্বিমিক = 1;

	যদি (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	যদি (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	যদি (use_stdout) {
		setup_pager(the_repository);
	} অন্যথা যদি (!rev.diffopt.close_file) {
		পূর্ণাঙ্ক saved;

		যদি (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		যদি (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		বিকল্প (safe_create_leading_directories_const(the_repository, output_directory)) {
		পক্ষ SCLD_OK:
		পক্ষ SCLD_EXISTS:
			ভঙ্গ;
		স্বভাব:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		যদি (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	যদি (rev.pending.nr == 1) {
		পূর্ণাঙ্ক check_head = 0;

		যদি (rev.max_গুনতি < 0 && !rev.show_মূল_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].item->flags |= UNINTERESTING;
			add_head_to_pending(&rev);
			check_head = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		যদি (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		যদি (check_head) {
			ধ্রুব বর্ণ *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      রিক্ত, রিক্ত);
			যদি (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			অন্যথা
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_মূল_diff = 1;

	যদি (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		যদি (rev.pending.nr == 2) {
			সংরচনা object_array_entry *o = rev.pending.objects;
			যদি (oideq(&o[0].item->oid, &o[1].item->oid))
				গচ্ছ done;
		}
		get_patch_ids(&rev, &ids);
	}

	যদি (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	যাবত্ ((commit = get_revision(&rev)) != রিক্ত) {
		যদি (commit->object.flags & BOUNDARY) {
			boundary_গুনতি++;
			origin = (boundary_গুনতি == 1) ? commit : রিক্ত;
			অনুবর্ত;
		}

		যদি (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			অনুবর্ত;

		commit_স্তূপ_ঠেলো(&list, commit);
	}
	যদি (!list.nr)
		/* nothing to do */
		গচ্ছ done;
	total = list.nr;

	যদি (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		যদি (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} অন্যথা যদি (cover_letter == -1) {
		cover_letter = 1;
	}

	যদি (cover_letter == -1) {
		যদি (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		অন্যথা যদি ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		অন্যথা
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	যদি (!cfg.keep_subject && cfg.auto_সংখ্যা && (total > 1 || cover_letter))
		cfg.numbered = 1;
	যদি (cfg.numbered)
		rev.total = total + start_সংখ্যা - 1;

	যদি (idiff_prev.nr) {
		যদি (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_বৃক্ষ_oid(list.বস্তুসমূহ[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_গুনতি,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	যদি (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	অন্যথা যদি (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	যদি (rdiff_prev) {
		যদি (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.বস্তুসমূহ[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_গুনতি,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_args(&rdiff_notes, &rev);
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
	যদি (!signature) {
		; /* --no-signature inhibits all signatures */
	} অন্যথা যদি (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} অন্যথা যদি (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		সংরচনা strbuf buf = STRBUF_INIT;
		ধ্রুব বর্ণ *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		যদি (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, রিক্ত);
	} অন্যথা যদি (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, পরিমাণ(bases));
	base = get_base_commit(&cfg, list.বস্তুসমূহ, list.nr);
	যদি (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.বস্তুসমূহ, list.nr);
	}

	যদি (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(পরিমাণ(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	যদি (in_reply_to) {
		বর্ণ *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	যদি (cover_letter) {
		যদি (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.বস্তুসমূহ,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_সংখ্যা--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = রিক্ত;
		rev.rdiff1 = রিক্ত;
	}
	rev.add_signoff = cfg.do_signoff;

	যদি (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	যাবত্ (list.nr) {
		size_t idx = list.nr - 1;
		পূর্ণাঙ্ক shown;

		display_progress(progress, total - idx);
		commit = commit_স্তূপ_বের_করো(&list);
		rev.nr = total - idx + (start_সংখ্যা - 1);

		/* Make the second and subsequent mails replies to the first */
		যদি (cfg.thread) {
			/* Have we already had a message ID? */
			যদি (rev.message_id) {
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
				যদি (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					মুক্ত(rev.message_id);
				অন্যথা
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		যদি (output_directory &&
		    open_next_file(rev.numbered_files ? রিক্ত : commit, রিক্ত, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_বৃক্ষ_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		যদি (output_directory)
			rev.shown_one = 0;
		যদি (shown) {
			print_bases(&bases, rev.diffopt.file);
			যদি (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			অন্যথা
				print_signature(signature, rev.diffopt.file);
		}
		যদি (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = রিক্ত;
		}
	}
	stop_progress(&progress);
	commit_স্তূপ_clear(&list);
	যদি (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	মুক্ত(description_file);
	মুক্ত(signature_file_arg);
	মুক্ত(signature_to_free);
	মুক্ত(branch_name);
	মুক্ত(to_free);
	মুক্ত(rev.message_id);
	যদি (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	মুক্ত(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক add_pending_commit(ধ্রুব বর্ণ *arg, সংরচনা rev_info *revs, পূর্ণাঙ্ক flags)
{
	সংরচনা object_id oid;
	যদি (repo_get_oid(the_repository, arg, &oid) == 0) {
		সংরচনা commit *commit = lookup_commit_reference(the_repository,
								&oid);
		যদি (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			প্রত্যাগম 0;
		}
	}
	প্রত্যাগম -1;
}

স্থির ধ্রুব বর্ণ * ধ্রুব cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	রিক্ত
};

স্থির শূন্য print_commit(বর্ণ sign, সংরচনা commit *commit, পূর্ণাঙ্ক verbose,
			 পূর্ণাঙ্ক abbrev, FILE *file)
{
	যদি (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} অন্যথা {
		সংরচনা strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		fprintf(file, "%c %s %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev),
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
স্থির শূন্য collect_diff_blob_oids(সংরচনা commit *commit,
				   সংরচনা diff_options *opts,
				   সংরচনা oidset *blobs)
{
	সংরচনা diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	প্রতিজ্ঞা(!commit->parents || !commit->parents->next);

	যদি (commit->parents)
		diff_বৃক্ষ_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	অন্যথা
		diff_মূল_বৃক্ষ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	প্রতি (পূর্ণাঙ্ক i = 0; i < q->nr; i++) {
		সংরচনা diff_filepair *p = q->queue[i];
		সংরচনা userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		যদি (drv && drv->দ্বিমিক > 0)
			অনুবর্ত;

		যদি (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, রিক্ত,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ঢোকাও(blobs, &p->one->oid);
		যদি (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, রিক্ত,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ঢোকাও(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

স্থির পূর্ণাঙ্ক always_match(ধ্রুব শূন্য *cmp_data UNUSED,
			ধ্রুব সংরচনা hashmap_entry *entry1 UNUSED,
			ধ্রুব সংরচনা hashmap_entry *entry2 UNUSED,
			ধ্রুব শূন্য *keydata UNUSED)
{
	প্রত্যাগম 0;
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
স্থির শূন্য prefetch_cherry_blobs(সংরচনা repository *repo,
				  সংরচনা commit_list *list,
				  সংরচনা patch_ids *ids)
{
	সংরচনা oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	যদি (!repo_has_promisor_remote(repo))
		প্রত্যাগম;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	প্রতি (সংরচনা commit_list *l = list; l; l = l->next) {
		সংরচনা commit *c = l->item;
		bool match_found = false;
		প্রতি (সংরচনা patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		যদি (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	যদি (oidset_size(&blobs)) {
		সংরচনা oid_array to_fetch = OID_ARRAY_INIT;
		সংরচনা oidset_iter iter;
		ধ্রুব সংরচনা object_id *oid;

		oidset_iter_init(&blobs, &iter);
		যাবত্ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

পূর্ণাঙ্ক cmd_cherry(পূর্ণাঙ্ক argc,
	       ধ্রুব বর্ণ **argv,
	       ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo UNUSED)
{
	সংরচনা rev_info revs;
	সংরচনা patch_ids ids;
	সংরচনা commit *commit;
	সংরচনা commit_list *list = রিক্ত;
	সংরচনা branch *current_branch;
	ধ্রুব বর্ণ *upstream;
	ধ্রুব বর্ণ *head = "HEAD";
	ধ্রুব বর্ণ *limit = রিক্ত;
	পূর্ণাঙ্ক verbose = 0, abbrev = 0;

	সংরচনা option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	বিকল্প (argc) {
	পক্ষ 3:
		limit = argv[2];
		/* FALLTHROUGH */
	পক্ষ 2:
		head = argv[1];
		/* FALLTHROUGH */
	পক্ষ 1:
		upstream = argv[0];
		ভঙ্গ;
	স্বভাব:
		current_branch = branch_get(রিক্ত);
		upstream = branch_get_upstream(current_branch, রিক্ত);
		যদি (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	যদি (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	যদি (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	যদি (revs.pending.nr == 2) {
		সংরচনা object_array_entry *o = revs.pending.objects;
		যদি (oideq(&o[0].item->oid, &o[1].item->oid))
			প্রত্যাগম 0;
	}

	get_patch_ids(&revs, &ids);

	যদি (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	যদি (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	যাবত্ ((commit = get_revision(&revs)) != রিক্ত) {
		commit_list_ঢোকাও(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	প্রতি (সংরচনা commit_list *l = list; l; l = l->next) {
		বর্ণ sign = '+';

		commit = l->item;
		যদি (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	প্রত্যাগম 0;
}
