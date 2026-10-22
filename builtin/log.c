/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "odb/streaming.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "diff-merges.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "log-tree.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "oidset.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "reflog-walk.h"
#അന്തര്ഭാവ "patch-ids.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "shortlog.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "line-log.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "version.h"
#അന്തര്ഭാവ "mailmap.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "commit-slab.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "utf8.h"

#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "promisor-remote.h"
#അന്തര്ഭാവ "range-diff.h"
#അന്തര്ഭാവ "tmp-objdir.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "userdiff.h"
#അന്തര്ഭാവ "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ force_in_body_from;
സ്ഥിതം പൂർണ്ണസംഖ്യ stdout_mboxrd;
സ്ഥിതം പൂർണ്ണസംഖ്യ format_no_prefix;

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ശൂന്യ
};

ഘടന വരി_opt_callback_data {
	ഘടന rev_info *rev;
	സ്ഥിരം അക്ഷരം *prefix;
	ഘടന string_list args;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ session_is_interactive(ശൂന്യം)
{
	തിരികെ isatty(1) || pager_in_use();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ auto_decoration_style(ശൂന്യം)
{
	തിരികെ session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_decoration_style(സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	തിരഞ്ഞെടുപ്പ് (git_parse_maybe_bool(മൂല്യം)) {
	അവസ്ഥ 1:
		തിരികെ DECORATE_SHORT_REFS;
	അവസ്ഥ 0:
		തിരികെ 0;
	സ്ഥിരസ്ഥിതി:
		നിർത്തുക;
	}
	എങ്കിൽ (!strcmp(മൂല്യം, "full"))
		തിരികെ DECORATE_FULL_REFS;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(മൂല്യം, "short"))
		തിരികെ DECORATE_SHORT_REFS;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(മൂല്യം, "auto"))
		തിരികെ auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	തിരികെ -1;
}

ഘടന log_config {
	പൂർണ്ണസംഖ്യ default_abbrev_commit;
	പൂർണ്ണസംഖ്യ default_show_വേര്;
	പൂർണ്ണസംഖ്യ default_follow;
	പൂർണ്ണസംഖ്യ default_show_signature;
	പൂർണ്ണസംഖ്യ default_encode_email_headers;
	പൂർണ്ണസംഖ്യ decoration_style;
	പൂർണ്ണസംഖ്യ decoration_given;
	പൂർണ്ണസംഖ്യ use_mailmap_config;
	അക്ഷരം *fmt_patch_subject_prefix;
	പൂർണ്ണസംഖ്യ fmt_patch_name_max;
	അക്ഷരം *fmt_pretty;
	അക്ഷരം *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	പൂർണ്ണസംഖ്യ i_still_use_this;
#endif
};

സ്ഥിതം ശൂന്യം log_config_init(ഘടന log_config *cfg)
{
	memset(cfg, 0, വലുപ്പം(*cfg));
	cfg->default_show_വേര് = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

സ്ഥിതം ശൂന്യം log_config_release(ഘടന log_config *cfg)
{
	സ്വതന്ത്രം(cfg->default_date_mode);
	സ്വതന്ത്രം(cfg->fmt_pretty);
	സ്വതന്ത്രം(cfg->fmt_patch_subject_prefix);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ use_default_decoration_filter = 1;
സ്ഥിതം ഘടന string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
സ്ഥിതം ഘടന string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
സ്ഥിതം ഘടന string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

സ്ഥിതം പൂർണ്ണസംഖ്യ clear_decorations_callback(സ്ഥിരം ഘടന option *opt UNUSED,
				      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ decorate_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			     പൂർണ്ണസംഖ്യ unset)
{
	ഘടന log_config *cfg = opt->മൂല്യം;

	എങ്കിൽ (unset)
		cfg->decoration_style = 0;
	അല്ലെങ്കിൽ എങ്കിൽ (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	അല്ലെങ്കിൽ
		cfg->decoration_style = DECORATE_SHORT_REFS;

	എങ്കിൽ (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ log_വരി_range_callback(സ്ഥിരം ഘടന option *option, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന വരി_opt_callback_data *data = option->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!arg)
		തിരികെ -1;

	data->rev->വരി_level_traverse = 1;
	string_list_append(&data->args, arg);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം cmd_log_init_defaults(ഘടന rev_info *rev,
				  ഘടന log_config *cfg)
{
	എങ്കിൽ (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	എങ്കിൽ (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_വേര്_diff = cfg->default_show_വേര്;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	എങ്കിൽ (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

സ്ഥിതം ശൂന്യം set_default_decoration_filter(ഘടന decoration_filter *decoration_filter)
{
	അക്ഷരം *മൂല്യം = ശൂന്യ;
	ഘടന string_list *include = decoration_filter->include_ref_pattern;
	സ്ഥിരം ഘടന string_list *config_exclude;

	എങ്കിൽ (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ഘടന string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	എങ്കിൽ (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &മൂല്യം) &&
	    !strcmp("all", മൂല്യം))
		use_default_decoration_filter = 0;
	സ്വതന്ത്രം(മൂല്യം);

	എങ്കിൽ (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		തിരികെ;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		എങ്കിൽ (!ref_namespace[i].decoration)
			തുടരുക;

		string_list_append(include, ref_namespace[i].ref);
	}
}

സ്ഥിതം ശൂന്യം cmd_log_init_finish(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന rev_info *rev, ഘടന setup_revision_opt *opt,
			 ഘടന log_config *cfg)
{
	ഘടന userformat_want w;
	പൂർണ്ണസംഖ്യ quiet = 0, source = 0, mailmap;
	സ്ഥിതം ഘടന വരി_opt_callback_data വരി_cb = {ശൂന്യ, ശൂന്യ, STRING_LIST_INIT_DUP};
	ഘടന decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	സ്ഥിതം ഘടന revision_sources revision_sources;

	സ്ഥിരം ഘടന option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ശൂന്യ, ശൂന്യ,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ശൂന്യ, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ശൂന്യ, &വരി_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_വരി_range_callback),
		OPT_END()
	};

	വരി_cb.rev = rev;
	വരി_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	എങ്കിൽ (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	എങ്കിൽ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	എങ്കിൽ (rev->വരി_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, വലുപ്പം(w));
	userformat_find_requirements(ശൂന്യ, &w);

	എങ്കിൽ (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	എങ്കിൽ (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	എങ്കിൽ ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	എങ്കിൽ (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	എങ്കിൽ (mailmap) {
		rev->mailmap = xmalloc(വലുപ്പം(ഘടന string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	എങ്കിൽ (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		എങ്കിൽ (!cfg->decoration_given)
			cfg->decoration_style = 0;
		എങ്കിൽ (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	എങ്കിൽ (rev->commit_format == CMIT_FMT_USERFORMAT) {
		എങ്കിൽ (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} അല്ലെങ്കിൽ എങ്കിൽ (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	എങ്കിൽ (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		എങ്കിൽ (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	എങ്കിൽ (rev->വരി_level_traverse)
		വരി_log_init(rev, വരി_cb.prefix, &വരി_cb.args);

	setup_pager(the_repository);
}

സ്ഥിതം ശൂന്യം cmd_log_init(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന rev_info *rev, ഘടന setup_revision_opt *opt,
			 ഘടന log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_log_walk_no_free(ഘടന rev_info *rev)
{
	ഘടന commit *commit;
	പൂർണ്ണസംഖ്യ saved_nrl = 0;
	പൂർണ്ണസംഖ്യ saved_dcctc = 0;
	പൂർണ്ണസംഖ്യ result;

	എങ്കിൽ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	ആകുമ്പോൾ ((commit = get_revision(rev)) != ശൂന്യ) {
		എങ്കിൽ (!log_വൃക്ഷം_commit(rev, commit) && rev->max_എണ്ണം >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_എണ്ണം++;
		എങ്കിൽ (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = ശൂന്യ;
		}
		എങ്കിൽ (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		എങ്കിൽ (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	എങ്കിൽ (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_log_walk(ഘടന rev_info *rev)
{
	പൂർണ്ണസംഖ്യ retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_log_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			  സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന log_config *cfg = cb;
	സ്ഥിരം അക്ഷരം *slot_name;

	എങ്കിൽ (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		തിരികെ git_config_string(&cfg->fmt_pretty, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		തിരികെ git_config_string(&cfg->fmt_patch_subject_prefix, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, മൂല്യം, ctx->kvi);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		തിരികെ git_config_string(&cfg->default_date_mode, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(മൂല്യം);
		എങ്കിൽ (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "log.diffmerges")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		തിരികെ diff_merges_config(മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "log.showroot")) {
		cfg->default_show_വേര് = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (skip_prefix(var, "color.decorate.", &slot_name))
		തിരികെ parse_decorate_color_config(var, slot_name, മൂല്യം);
	എങ്കിൽ (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	തിരികെ git_diff_ui_config(var, മൂല്യം, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
പൂർണ്ണസംഖ്യ cmd_whatchanged(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo UNUSED)
{
	ഘടന log_config cfg;
	ഘടന rev_info rev;
	ഘടന setup_revision_opt opt;
	പൂർണ്ണസംഖ്യ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, വലുപ്പം(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	എങ്കിൽ (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	എങ്കിൽ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	തിരികെ ret;
}
#endif

സ്ഥിതം ശൂന്യം show_tagger(സ്ഥിരം അക്ഷരം *buf, ഘടന rev_info *rev)
{
	ഘടന strbuf out = STRBUF_INIT;
	ഘടന pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_blob_object(സ്ഥിരം ഘടന object_id *oid, ഘടന rev_info *rev, സ്ഥിരം അക്ഷരം *obj_name)
{
	ഘടന object_id oidc;
	ഘടന object_context obj_context = {0};
	അക്ഷരം *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	എങ്കിൽ (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		തിരികെ odb_stream_blob_to_fd(the_repository->objects, 1, oid, ശൂന്യ, 0);

	എങ്കിൽ (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	എങ്കിൽ (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		തിരികെ odb_stream_blob_to_fd(the_repository->objects, 1, oid, ശൂന്യ, 0);
	}

	എങ്കിൽ (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	സ്വതന്ത്രം(buf);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_tag_object(സ്ഥിരം ഘടന object_id *oid, ഘടന rev_info *rev)
{
	size_t size;
	എണ്ണൽ object_type type;
	അക്ഷരം *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	എങ്കിൽ (!buf)
		തിരികെ error(_("could not read object %s"), oid_to_hex(oid));

	പ്രതിജ്ഞാ(type == OBJ_TAG);
	ആകുമ്പോൾ (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		സ്ഥിരം അക്ഷരം *ident;
		ആകുമ്പോൾ (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		എങ്കിൽ (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	എങ്കിൽ (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	സ്വതന്ത്രം(buf);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_വൃക്ഷം_object(സ്ഥിരം ഘടന object_id *oid UNUSED,
			    ഘടന strbuf *base UNUSED,
			    സ്ഥിരം അക്ഷരം *pathname, unsigned mode,
			    ശൂന്യം *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_setup_revisions_tweak(ഘടന rev_info *rev)
{
	എങ്കിൽ (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	അല്ലെങ്കിൽ
		diff_merges_default_to_dense_combined(rev);
	എങ്കിൽ (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

പൂർണ്ണസംഖ്യ cmd_show(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo UNUSED)
{
	ഘടന log_config cfg;
	ഘടന rev_info rev;
	unsigned പൂർണ്ണസംഖ്യ i;
	ഘടന setup_revision_opt opt;
	ഘടന pathspec match_all;
	പൂർണ്ണസംഖ്യ ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	എങ്കിൽ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, വലുപ്പം(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, വലുപ്പം(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	എങ്കിൽ (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		തിരികെ ret;
	}

	rev.diffopt.no_free = 1;
	വേണ്ടി (i = 0; i < rev.pending.nr && !ret; i++) {
		ഘടന object *o = rev.pending.objects[i].item;
		സ്ഥിരം അക്ഷരം *name = rev.pending.objects[i].name;
		തിരഞ്ഞെടുപ്പ് (o->type) {
		അവസ്ഥ OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			നിർത്തുക;
		അവസ്ഥ OBJ_TAG: {
			ഘടന tag *t = (ഘടന tag *)o;
			ഘടന object_id *oid = get_tagged_oid(t);

			എങ്കിൽ (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			എങ്കിൽ (ret)
				നിർത്തുക;
			o = parse_object(the_repository, oid);
			എങ്കിൽ (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			നിർത്തുക;
		}
		അവസ്ഥ OBJ_വൃക്ഷം:
			എങ്കിൽ (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_വൃക്ഷം(the_repository, (ഘടന വൃക്ഷം *)o,
				  &match_all, show_വൃക്ഷം_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			നിർത്തുക;
		അവസ്ഥ OBJ_COMMIT:
		{
			ഘടന object_array old;
			ഘടന object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, വലുപ്പം(old));
			memcpy(&rev.pending, &blank, വലുപ്പം(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, വലുപ്പം(rev.pending));
			നിർത്തുക;
		}
		സ്ഥിരസ്ഥിതി:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	തിരികെ ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
പൂർണ്ണസംഖ്യ cmd_log_reflog(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന log_config cfg;
	ഘടന rev_info rev;
	ഘടന setup_revision_opt opt;
	പൂർണ്ണസംഖ്യ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, വലുപ്പം(opt));
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
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം log_setup_revisions_tweak(ഘടന rev_info *rev)
{
	എങ്കിൽ (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	എങ്കിൽ (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

പൂർണ്ണസംഖ്യ cmd_log(പൂർണ്ണസംഖ്യ argc,
	    സ്ഥിരം അക്ഷരം **argv,
	    സ്ഥിരം അക്ഷരം *prefix,
	    ഘടന repository *repo UNUSED)
{
	ഘടന log_config cfg;
	ഘടന rev_info rev;
	ഘടന setup_revision_opt opt;
	പൂർണ്ണസംഖ്യ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, വലുപ്പം(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	തിരികെ ret;
}

/* format-patch */

എണ്ണൽ cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

എണ്ണൽ thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

എണ്ണൽ cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

എണ്ണൽ auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

ഘടന format_config {
	ഘടന log_config log;
	എണ്ണൽ thread_level thread;
	പൂർണ്ണസംഖ്യ do_signoff;
	എണ്ണൽ auto_base_setting auto_base;
	അക്ഷരം *base_commit;
	അക്ഷരം *from;
	അക്ഷരം *signature;
	അക്ഷരം *signature_file;
	എണ്ണൽ cover_setting config_cover_letter;
	അക്ഷരം *fmt_cover_letter_commit_list;
	അക്ഷരം *config_output_directory;
	എണ്ണൽ cover_from_description cover_from_description_mode;
	പൂർണ്ണസംഖ്യ show_notes;
	ഘടന display_notes_opt notes_opt;
	പൂർണ്ണസംഖ്യ numbered_cmdline_opt;
	പൂർണ്ണസംഖ്യ numbered;
	പൂർണ്ണസംഖ്യ auto_സംഖ്യ;
	അക്ഷരം *default_attach;
	ഘടന string_list extra_hdr;
	ഘടന string_list extra_to;
	ഘടന string_list extra_cc;
	പൂർണ്ണസംഖ്യ keep_subject;
	പൂർണ്ണസംഖ്യ subject_prefix;
	ഘടന strbuf sprefix;
	അക്ഷരം *fmt_patch_suffix;
};

സ്ഥിതം ശൂന്യം format_config_init(ഘടന format_config *cfg)
{
	memset(cfg, 0, വലുപ്പം(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_സംഖ്യ = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

സ്ഥിതം ശൂന്യം format_config_release(ഘടന format_config *cfg)
{
	log_config_release(&cfg->log);
	സ്വതന്ത്രം(cfg->base_commit);
	സ്വതന്ത്രം(cfg->from);
	സ്വതന്ത്രം(cfg->signature);
	സ്വതന്ത്രം(cfg->signature_file);
	സ്വതന്ത്രം(cfg->config_output_directory);
	സ്വതന്ത്രം(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	സ്വതന്ത്രം(cfg->fmt_patch_suffix);
	സ്വതന്ത്രം(cfg->fmt_cover_letter_commit_list);
}

സ്ഥിതം എണ്ണൽ cover_from_description parse_cover_from_description(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (!arg || !strcmp(arg, "default"))
		തിരികെ COVER_FROM_MESSAGE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "none"))
		തിരികെ COVER_FROM_NONE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "message"))
		തിരികെ COVER_FROM_MESSAGE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "subject"))
		തിരികെ COVER_FROM_SUBJECT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "auto"))
		തിരികെ COVER_FROM_AUTO;
	അല്ലെങ്കിൽ
		die(_("%s: invalid cover from description mode"), arg);
}

സ്ഥിതം ശൂന്യം add_header(ഘടന format_config *cfg, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	ഘടന string_list_item *item;
	പൂർണ്ണസംഖ്യ len = strlen(മൂല്യം);
	ആകുമ്പോൾ (len && മൂല്യം[len - 1] == '\n')
		len--;

	എങ്കിൽ (!strncasecmp(മൂല്യം, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, മൂല്യം + 4);
		len -= 4;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strncasecmp(മൂല്യം, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, മൂല്യം + 4);
		len -= 4;
	} അല്ലെങ്കിൽ {
		item = string_list_append(&cfg->extra_hdr, മൂല്യം);
	}

	item->string[len] = '\0';
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_format_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			     സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	ഘടന format_config *cfg = cb;

	എങ്കിൽ (!strcmp(var, "format.headers")) {
		എങ്കിൽ (!മൂല്യം)
			die(_("format.headers without value"));
		add_header(cfg, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		തിരികെ git_config_string(&cfg->fmt_patch_suffix, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.to")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		string_list_append(&cfg->extra_to, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.cc")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.numbered")) {
		എങ്കിൽ (മൂല്യം && !strcasecmp(മൂല്യം, "auto")) {
			cfg->auto_സംഖ്യ = 1;
			തിരികെ 0;
		}
		cfg->numbered = git_config_bool(var, മൂല്യം);
		cfg->auto_സംഖ്യ = cfg->auto_സംഖ്യ && cfg->numbered;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.attach")) {
		എങ്കിൽ (മൂല്യം && *മൂല്യം) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(മൂല്യം);
		} അല്ലെങ്കിൽ എങ്കിൽ (മൂല്യം && !*മൂല്യം) {
			FREE_AND_NULL(cfg->default_attach);
		} അല്ലെങ്കിൽ {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.thread")) {
		എങ്കിൽ (മൂല്യം && !strcasecmp(മൂല്യം, "deep")) {
			cfg->thread = THREAD_DEEP;
			തിരികെ 0;
		}
		എങ്കിൽ (മൂല്യം && !strcasecmp(മൂല്യം, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			തിരികെ 0;
		}
		cfg->thread = git_config_bool(var, മൂല്യം) ? THREAD_SHALLOW : THREAD_UNSET;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		തിരികെ git_config_string(&cfg->signature, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		തിരികെ git_config_pathname(&cfg->signature_file, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.coverletter")) {
		എങ്കിൽ (മൂല്യം && !strcasecmp(മൂല്യം, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			തിരികെ 0;
		}
		cfg->config_cover_letter = git_config_bool(var, മൂല്യം) ? COVER_ON : COVER_OFF;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		തിരികെ git_config_string(&cfg->fmt_cover_letter_commit_list, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		തിരികെ git_config_string(&cfg->config_output_directory, var, മൂല്യം);
	}
	എങ്കിൽ (!strcmp(var, "format.useautobase")) {
		എങ്കിൽ (മൂല്യം && !strcasecmp(മൂല്യം, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			തിരികെ 0;
		}
		cfg->auto_base = git_config_bool(var, മൂല്യം) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.from")) {
		പൂർണ്ണസംഖ്യ b = git_parse_maybe_bool(മൂല്യം);
		FREE_AND_NULL(cfg->from);
		എങ്കിൽ (b < 0)
			cfg->from = xstrdup(മൂല്യം);
		അല്ലെങ്കിൽ എങ്കിൽ (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.notes")) {
		പൂർണ്ണസംഖ്യ b = git_parse_maybe_bool(മൂല്യം);
		എങ്കിൽ (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, മൂല്യം);
		അല്ലെങ്കിൽ എങ്കിൽ (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		അല്ലെങ്കിൽ
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(മൂല്യം);
		എങ്കിൽ (format_no_prefix < 0) {
			പൂർണ്ണസംഖ്യ status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				മൂല്യം, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		തിരികെ 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	എങ്കിൽ (!strcmp(var, "diff.noprefix"))
		തിരികെ 0;

	തിരികെ git_log_config(var, മൂല്യം, ctx, &cfg->log);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *output_directory = ശൂന്യ;
സ്ഥിതം പൂർണ്ണസംഖ്യ outdir_offset;

സ്ഥിതം പൂർണ്ണസംഖ്യ open_next_file(ഘടന commit *commit, സ്ഥിരം അക്ഷരം *subject,
			 ഘടന rev_info *rev, പൂർണ്ണസംഖ്യ quiet)
{
	ഘടന strbuf filename = STRBUF_INIT;

	എങ്കിൽ (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	എങ്കിൽ (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	അല്ലെങ്കിൽ എങ്കിൽ (commit)
		fmt_output_commit(&filename, commit, rev);
	അല്ലെങ്കിൽ
		fmt_output_subject(&filename, subject, rev);

	എങ്കിൽ (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	എങ്കിൽ (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		തിരികെ -1;
	}

	strbuf_release(&filename);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം get_patch_ids(ഘടന rev_info *rev, ഘടന patch_ids *ids)
{
	ഘടന rev_info check_rev;
	ഘടന commit *commit, *c1, *c2;
	ഘടന object *o1, *o2;
	unsigned flags1, flags2;

	എങ്കിൽ (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	എങ്കിൽ ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	എങ്കിൽ (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	ആകുമ്പോൾ ((commit = get_revision(&check_rev)) != ശൂന്യ) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

സ്ഥിതം ശൂന്യം gen_message_id(ഘടന rev_info *info, സ്ഥിരം അക്ഷരം *base)
{
	ഘടന strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(ശൂന്യ),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം print_signature(സ്ഥിരം അക്ഷരം *signature, FILE *file)
{
	എങ്കിൽ (!signature || !*signature)
		തിരികെ;

	fprintf(file, "-- \n%s", signature);
	എങ്കിൽ (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

സ്ഥിതം അക്ഷരം *find_branch_name(ഘടന rev_info *rev)
{
	ഘടന object_id branch_oid;
	സ്ഥിരം ഘടന object_id *tip_oid;
	സ്ഥിരം അക്ഷരം *ref, *v;
	അക്ഷരം *full_ref, *branch = ശൂന്യ;
	പൂർണ്ണസംഖ്യ interesting_found = 0;
	size_t idx;

	വേണ്ടി (size_t i = 0; i < rev->cmdline.nr; i++) {
		എങ്കിൽ (rev->cmdline.rev[i].flags & UNINTERESTING)
			തുടരുക;
		എങ്കിൽ (interesting_found)
			തിരികെ ശൂന്യ;
		interesting_found = 1;
		idx = i;
	}
	എങ്കിൽ (!interesting_found)
		തിരികെ ശൂന്യ;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	എങ്കിൽ (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	സ്വതന്ത്രം(full_ref);
	തിരികെ branch;
}

സ്ഥിതം ശൂന്യം show_diffstat(ഘടന rev_info *rev,
			  ഘടന commit *origin, ഘടന commit *head)
{
	ഘടന diff_options opts;

	memcpy(&opts, &rev->diffopt, വലുപ്പം(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_വൃക്ഷം_oid(get_commit_വൃക്ഷം_oid(origin),
		      get_commit_വൃക്ഷം_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

സ്ഥിതം ശൂന്യം read_desc_file(ഘടന strbuf *buf, സ്ഥിരം അക്ഷരം *desc_file)
{
	എങ്കിൽ (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

സ്ഥിതം ശൂന്യം prepare_cover_text(ഘടന pretty_print_context *pp,
			       സ്ഥിരം അക്ഷരം *description_file,
			       സ്ഥിരം അക്ഷരം *branch_name,
			       ഘടന strbuf *sb,
			       സ്ഥിരം അക്ഷരം *encoding,
			       പൂർണ്ണസംഖ്യ need_8bit_cte,
			       സ്ഥിരം ഘടന format_config *cfg)
{
	സ്ഥിരം അക്ഷരം *subject = "*** SUBJECT HERE ***";
	സ്ഥിരം അക്ഷരം *body = "*** BLURB HERE ***";
	ഘടന strbuf description_sb = STRBUF_INIT;
	ഘടന strbuf subject_sb = STRBUF_INIT;

	എങ്കിൽ (cfg->cover_from_description_mode == COVER_FROM_NONE)
		പോകുക do_pp;

	എങ്കിൽ (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	അല്ലെങ്കിൽ എങ്കിൽ (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	എങ്കിൽ (!description_sb.len)
		പോകുക do_pp;

	എങ്കിൽ (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	എങ്കിൽ (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	അല്ലെങ്കിൽ
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ഘടന rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ഘടന string_list notes;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ rdiff_notes_cb(സ്ഥിരം ഘടന option *option,
			  സ്ഥിരം അക്ഷരം *arg,
			  പൂർണ്ണസംഖ്യ unset)
{
	ഘടന option opt = *option;
	ഘടന rdiff_notes *rdiff_notes = option->മൂല്യം;

	rdiff_notes->override = 1;
	opt.മൂല്യം = &rdiff_notes->notes;
	തിരികെ parse_opt_string_list(&opt, arg, unset);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_notes_refs(ഘടന string_list_item *item, ശൂന്യം *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം get_notes_args(ഘടന rdiff_notes *rdiff_notes,
			   ഘടന rev_info *rev)
{
	എങ്കിൽ (rdiff_notes->override) {
		എങ്കിൽ (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		അല്ലെങ്കിൽ
			strvec_തള്ളുക(&rev->rdiff_log_arg, "--no-notes");
	} അല്ലെങ്കിൽ എങ്കിൽ (!rev->show_notes) {
		strvec_തള്ളുക(&rev->rdiff_log_arg, "--no-notes");
	} അല്ലെങ്കിൽ എങ്കിൽ (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_തള്ളുക(&rev->rdiff_log_arg, "--notes");
	} അല്ലെങ്കിൽ {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

സ്ഥിതം ശൂന്യം generate_shortlog_cover_letter(ഘടന shortlog *log,
					   ഘടന rev_info *rev,
					   ഘടന commit **list,
					   പൂർണ്ണസംഖ്യ nr)
{
	shortlog_init(log);
	log->wrap_വരികൾ = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

സ്ഥിതം ശൂന്യം generate_commit_list_cover(FILE *cover_file, സ്ഥിരം അക്ഷരം *format,
				       ഘടന commit **list, പൂർണ്ണസംഖ്യ n)
{
	ഘടന strbuf commit_വരി = STRBUF_INIT;
	ഘടന pretty_print_context ctx = {0};
	ഘടന rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_വരി, &ctx);
		fprintf(cover_file, "%s\n", commit_വരി.buf);
		strbuf_reset(&commit_വരി);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_വരി);
}

സ്ഥിതം ശൂന്യം make_cover_letter(ഘടന rev_info *rev, പൂർണ്ണസംഖ്യ use_separate_file,
			      ഘടന commit *origin,
			      പൂർണ്ണസംഖ്യ nr, ഘടന commit **list,
			      സ്ഥിരം അക്ഷരം *description_file,
			      സ്ഥിരം അക്ഷരം *branch_name,
			      പൂർണ്ണസംഖ്യ quiet,
			      സ്ഥിരം ഘടന format_config *cfg,
			      സ്ഥിരം അക്ഷരം *format)
{
	സ്ഥിരം അക്ഷരം *from;
	ഘടന shortlog log;
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ i;
	സ്ഥിരം അക്ഷരം *encoding = "UTF-8";
	പൂർണ്ണസംഖ്യ need_8bit_cte = 0;
	ഘടന pretty_print_context pp = {0};
	ഘടന commit *head = list[0];
	അക്ഷരം *to_free = ശൂന്യ;

	എങ്കിൽ (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	എങ്കിൽ (use_separate_file &&
	    open_next_file(ശൂന്യ, rev->numbered_files ? ശൂന്യ : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	വേണ്ടി (i = 0; !need_8bit_cte && i < nr; i++) {
		സ്ഥിരം അക്ഷരം *buf = repo_get_commit_buffer(the_repository,
							 list[i], ശൂന്യ);
		എങ്കിൽ (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	എങ്കിൽ (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, ശൂന്യ, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	സ്വതന്ത്രം(to_free);
	സ്വതന്ത്രം(pp.after_subject);
	strbuf_release(&sb);

	എങ്കിൽ (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	അല്ലെങ്കിൽ എങ്കിൽ (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	അല്ലെങ്കിൽ
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	എങ്കിൽ (origin)
		show_diffstat(rev, origin, head);

	എങ്കിൽ (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	എങ്കിൽ (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ഘടന diff_options opts;
		ഘടന range_diff_options range_diff_opts = {
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

സ്ഥിതം അക്ഷരം *clean_message_id(സ്ഥിരം അക്ഷരം *സന്ദേശം_id)
{
	അക്ഷരം ch;
	സ്ഥിരം അക്ഷരം *a, *z, *m;

	m = സന്ദേശം_id;
	ആകുമ്പോൾ ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = ശൂന്യ;
	ആകുമ്പോൾ ((ch = *m)) {
		എങ്കിൽ (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	എങ്കിൽ (!z)
		die(_("insane in-reply-to: %s"), സന്ദേശം_id);
	എങ്കിൽ (++z == m)
		തിരികെ xstrdup(a);
	തിരികെ xmemdupz(a, z - a);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *set_outdir(സ്ഥിരം അക്ഷരം *prefix, സ്ഥിരം അക്ഷരം *output_directory)
{
	എങ്കിൽ (output_directory && is_absolute_path(output_directory))
		തിരികെ output_directory;

	എങ്കിൽ (!prefix || !*prefix) {
		എങ്കിൽ (output_directory)
			തിരികെ output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		തിരികെ "./";
	}

	outdir_offset = strlen(prefix);
	എങ്കിൽ (!output_directory)
		തിരികെ prefix;

	തിരികെ prefix_filename(prefix, output_directory);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ശൂന്യ
};

ഘടന keep_callback_data {
	ഘടന format_config *cfg;
	ഘടന rev_info *revs;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ keep_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന keep_callback_data *data = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ subject_prefix_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			    പൂർണ്ണസംഖ്യ unset)
{
	ഘടന format_config *cfg = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rfc_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			പൂർണ്ണസംഖ്യ unset)
{
	സ്ഥിരം അക്ഷരം **rfc = opt->മൂല്യം;

	*rfc = opt->മൂല്യം;
	എങ്കിൽ (unset)
		*rfc = ശൂന്യ;
	അല്ലെങ്കിൽ
		*rfc = arg ? arg : "RFC";
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ numbered_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			     പൂർണ്ണസംഖ്യ unset)
{
	ഘടന format_config *cfg = opt->മൂല്യം;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	എങ്കിൽ (unset)
		cfg->auto_സംഖ്യ =  0;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ no_numbered_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
				പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);
	തിരികെ numbered_callback(opt, arg, 1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ output_directory_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			      പൂർണ്ണസംഖ്യ unset)
{
	സ്ഥിരം അക്ഷരം **dir = (സ്ഥിരം അക്ഷരം **)opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	എങ്കിൽ (*dir)
		die(_("two output directories?"));
	*dir = arg;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ thread_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന format_config *cfg = opt->മൂല്യം;

	എങ്കിൽ (unset)
		cfg->thread = THREAD_UNSET;
	അല്ലെങ്കിൽ എങ്കിൽ (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	അല്ലെങ്കിൽ
		തിരികെ 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ attach_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rev_info *rev = (ഘടന rev_info *)opt->മൂല്യം;
	എങ്കിൽ (unset)
		rev->mime_boundary = ശൂന്യ;
	അല്ലെങ്കിൽ എങ്കിൽ (arg)
		rev->mime_boundary = arg;
	അല്ലെങ്കിൽ
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ inline_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന rev_info *rev = (ഘടന rev_info *)opt->മൂല്യം;
	എങ്കിൽ (unset)
		rev->mime_boundary = ശൂന്യ;
	അല്ലെങ്കിൽ എങ്കിൽ (arg)
		rev->mime_boundary = arg;
	അല്ലെങ്കിൽ
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ header_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			   പൂർണ്ണസംഖ്യ unset)
{
	ഘടന format_config *cfg = opt->മൂല്യം;

	എങ്കിൽ (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} അല്ലെങ്കിൽ {
		add_header(cfg, arg);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ from_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	അക്ഷരം **from = opt->മൂല്യം;

	സ്വതന്ത്രം(*from);

	എങ്കിൽ (unset)
		*from = ശൂന്യ;
	അല്ലെങ്കിൽ എങ്കിൽ (arg)
		*from = xstrdup(arg);
	അല്ലെങ്കിൽ
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ base_callback(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന format_config *cfg = opt->മൂല്യം;

	എങ്കിൽ (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} അല്ലെങ്കിൽ {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	തിരികെ 0;
}

ഘടന base_വൃക്ഷം_info {
	ഘടന object_id base_commit;
	പൂർണ്ണസംഖ്യ nr_patch_id, alloc_patch_id;
	ഘടന object_id *patch_id;
};

സ്ഥിതം ഘടന commit *get_base_commit(സ്ഥിരം ഘടന format_config *cfg,
				      ഘടന commit **list,
				      size_t total)
{
	ഘടന commit *base = ശൂന്യ;
	ഘടന commit **rev;
	പൂർണ്ണസംഖ്യ auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	തിരഞ്ഞെടുപ്പ് (cfg->auto_base) {
	അവസ്ഥ AUTO_BASE_NEVER:
		എങ്കിൽ (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} അല്ലെങ്കിൽ {
			/* no base information is requested */
			തിരികെ ശൂന്യ;
		}
		നിർത്തുക;
	അവസ്ഥ AUTO_BASE_ALWAYS:
	അവസ്ഥ AUTO_BASE_WHEN_ABLE:
		എങ്കിൽ (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} അല്ലെങ്കിൽ {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected automatic base selection method");
	}

	എങ്കിൽ (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		എങ്കിൽ (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} അല്ലെങ്കിൽ {
		ഘടന branch *curr_branch = branch_get(ശൂന്യ);
		സ്ഥിരം അക്ഷരം *upstream = branch_get_upstream(curr_branch, ശൂന്യ);
		എങ്കിൽ (upstream) {
			ഘടന commit_list *base_list = ശൂന്യ;
			ഘടന commit *commit;
			ഘടന object_id oid;

			എങ്കിൽ (repo_get_oid(the_repository, upstream, &oid)) {
				എങ്കിൽ (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				അല്ലെങ്കിൽ
					തിരികെ ശൂന്യ;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			എങ്കിൽ (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				എങ്കിൽ (die_on_failure) {
					die(_("could not find exact merge base"));
				} അല്ലെങ്കിൽ {
					commit_list_free(base_list);
					തിരികെ ശൂന്യ;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			അല്ലെങ്കിൽ
				തിരികെ ശൂന്യ;
		}
	}

	ALLOC_ARRAY(rev, total);
	വേണ്ടി (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	ആകുമ്പോൾ (rev_nr > 1) {
		വേണ്ടി (i = 0; i < rev_nr / 2; i++) {
			ഘടന commit_list *merge_base = ശൂന്യ;
			എങ്കിൽ (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				എങ്കിൽ (die_on_failure) {
					die(_("failed to find exact merge base"));
				} അല്ലെങ്കിൽ {
					commit_list_free(merge_base);
					സ്വതന്ത്രം(rev);
					തിരികെ ശൂന്യ;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		എങ്കിൽ (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	എങ്കിൽ (ret < 0)
		exit(128);
	എങ്കിൽ (!ret) {
		എങ്കിൽ (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} അല്ലെങ്കിൽ {
			സ്വതന്ത്രം(rev);
			തിരികെ ശൂന്യ;
		}
	}

	വേണ്ടി (i = 0; i < total; i++) {
		എങ്കിൽ (base == list[i]) {
			എങ്കിൽ (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} അല്ലെങ്കിൽ {
				സ്വതന്ത്രം(rev);
				തിരികെ ശൂന്യ;
			}
		}
	}

	സ്വതന്ത്രം(rev);
	തിരികെ base;
}

define_commit_slab(commit_base, പൂർണ്ണസംഖ്യ);

സ്ഥിതം ശൂന്യം prepare_bases(ഘടന base_വൃക്ഷം_info *bases,
			  ഘടന commit *base,
			  ഘടന commit **list,
			  size_t total)
{
	ഘടന commit *commit;
	ഘടന rev_info revs;
	ഘടന diff_options diffopt;
	ഘടന commit_base commit_base;

	എങ്കിൽ (!base)
		തിരികെ;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, ശൂന്യ);
	revs.max_parents = 1;
	revs.topo_order = 1;
	വേണ്ടി (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	എങ്കിൽ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	ആകുമ്പോൾ ((commit = get_revision(&revs)) != ശൂന്യ) {
		ഘടന object_id oid;
		ഘടന object_id *patch_id;
		എങ്കിൽ (*commit_base_at(&commit_base, commit))
			തുടരുക;
		എങ്കിൽ (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

സ്ഥിതം ശൂന്യം print_bases(ഘടന base_വൃക്ഷം_info *bases, FILE *file)
{
	പൂർണ്ണസംഖ്യ i;

	/* Only do this once, either for the cover or for the first one */
	എങ്കിൽ (is_null_oid(&bases->base_commit))
		തിരികെ;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	വേണ്ടി (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	സ്വതന്ത്രം(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *diff_title(ഘടന strbuf *sb,
			      സ്ഥിരം അക്ഷരം *reroll_എണ്ണം,
			      സ്ഥിരം അക്ഷരം *generic,
			      സ്ഥിരം അക്ഷരം *rerolled)
{
	പൂർണ്ണസംഖ്യ v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	എങ്കിൽ (reroll_എണ്ണം && !strtol_i(reroll_എണ്ണം, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	അല്ലെങ്കിൽ
		strbuf_addstr(sb, generic);
	തിരികെ sb->buf;
}

സ്ഥിതം ശൂന്യം infer_range_diff_ranges(ഘടന strbuf *r1,
				    ഘടന strbuf *r2,
				    സ്ഥിരം അക്ഷരം *prev,
				    ഘടന commit *origin,
				    ഘടന commit *head)
{
	സ്ഥിരം അക്ഷരം *head_oid = oid_to_hex(&head->object.oid);
	പൂർണ്ണസംഖ്യ prev_is_range = is_range_diff_range(prev);

	എങ്കിൽ (prev_is_range)
		strbuf_addstr(r1, prev);
	അല്ലെങ്കിൽ
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	എങ്കിൽ (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	അല്ലെങ്കിൽ എങ്കിൽ (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	അല്ലെങ്കിൽ {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

പൂർണ്ണസംഖ്യ cmd_format_patch(പൂർണ്ണസംഖ്യ argc,
		     സ്ഥിരം അക്ഷരം **argv,
		     സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo UNUSED)
{
	ഘടന format_config cfg;
	ഘടന commit *commit;
	ഘടന commit_അട്ടി list = COMMIT_അട്ടി_INIT;
	ഘടന rev_info rev;
	അക്ഷരം *to_free = ശൂന്യ;
	ഘടന setup_revision_opt s_r_opt;
	size_t total, i;
	പൂർണ്ണസംഖ്യ use_stdout = 0;
	പൂർണ്ണസംഖ്യ start_സംഖ്യ = -1;
	പൂർണ്ണസംഖ്യ just_numbers = 0;
	പൂർണ്ണസംഖ്യ ignore_if_in_upstream = 0;
	പൂർണ്ണസംഖ്യ cover_letter = -1;
	സ്ഥിരം അക്ഷരം *cover_letter_fmt = ശൂന്യ;
	പൂർണ്ണസംഖ്യ boundary_എണ്ണം = 0;
	പൂർണ്ണസംഖ്യ no_ദ്വിമാന_diff = 0;
	പൂർണ്ണസംഖ്യ zero_commit = 0;
	ഘടന commit *origin = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *in_reply_to = ശൂന്യ;
	ഘടന patch_ids ids;
	ഘടന strbuf buf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ use_patch_format = 0;
	പൂർണ്ണസംഖ്യ quiet = 0;
	സ്ഥിരം അക്ഷരം *reroll_എണ്ണം = ശൂന്യ;
	അക്ഷരം *cover_from_description_arg = ശൂന്യ;
	അക്ഷരം *description_file = ശൂന്യ;
	അക്ഷരം *branch_name = ശൂന്യ;
	ഘടന base_വൃക്ഷം_info bases;
	ഘടന commit *base;
	പൂർണ്ണസംഖ്യ show_progress = 0;
	ഘടന progress *progress = ശൂന്യ;
	ഘടന oid_array idiff_prev = OID_ARRAY_INIT;
	ഘടന strbuf idiff_title = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *rdiff_prev = ശൂന്യ;
	ഘടന strbuf rdiff1 = STRBUF_INIT;
	ഘടന strbuf rdiff2 = STRBUF_INIT;
	ഘടന strbuf rdiff_title = STRBUF_INIT;
	ഘടന rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	സ്ഥിരം അക്ഷരം *rfc = ശൂന്യ;
	പൂർണ്ണസംഖ്യ creation_factor = -1;
	സ്ഥിരം അക്ഷരം *signature = git_version_string;
	അക്ഷരം *signature_to_free = ശൂന്യ;
	അക്ഷരം *signature_file_arg = ശൂന്യ;
	ഘടന keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	സ്ഥിരം അക്ഷരം *fmt_patch_suffix = ശൂന്യ;

	സ്ഥിരം ഘടന option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ശൂന്യ,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ശൂന്യ,
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
		OPT_INTEGER(0, "start-number", &start_സംഖ്യ,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_എണ്ണം, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, ശൂന്യ,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ദ്വിമാന_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, വലുപ്പം(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, വലുപ്പം(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	എങ്കിൽ (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	എങ്കിൽ (cfg.default_attach) {
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

	എങ്കിൽ (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	എങ്കിൽ (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	എങ്കിൽ (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	എങ്കിൽ (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		എങ്കിൽ (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		അല്ലെങ്കിൽ
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	എങ്കിൽ (reroll_എണ്ണം) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_എണ്ണം);
		rev.reroll_എണ്ണം = reroll_എണ്ണം;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	വേണ്ടി (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.ഇനങ്ങൾ[i].string);
		strbuf_addch(&buf, '\n');
	}

	എങ്കിൽ (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	വേണ്ടി (i = 0; i < cfg.extra_to.nr; i++) {
		എങ്കിൽ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.ഇനങ്ങൾ[i].string);
		എങ്കിൽ (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	എങ്കിൽ (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	വേണ്ടി (i = 0; i < cfg.extra_cc.nr; i++) {
		എങ്കിൽ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.ഇനങ്ങൾ[i].string);
		എങ്കിൽ (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, ശൂന്യ);

	എങ്കിൽ (cfg.from) {
		എങ്കിൽ (split_ident_വരി(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	എങ്കിൽ (start_സംഖ്യ < 0)
		start_സംഖ്യ = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	എങ്കിൽ (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	എങ്കിൽ (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	എങ്കിൽ (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	എങ്കിൽ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	എങ്കിൽ (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	എങ്കിൽ (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	എങ്കിൽ (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	എങ്കിൽ (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	എങ്കിൽ (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	എങ്കിൽ (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	എങ്കിൽ (!rev.diffopt.flags.text && !no_ദ്വിമാന_diff)
		rev.diffopt.flags.ദ്വിമാന = 1;

	എങ്കിൽ (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	എങ്കിൽ (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	എങ്കിൽ (use_stdout) {
		setup_pager(the_repository);
	} അല്ലെങ്കിൽ എങ്കിൽ (!rev.diffopt.close_file) {
		പൂർണ്ണസംഖ്യ saved;

		എങ്കിൽ (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		എങ്കിൽ (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		തിരഞ്ഞെടുപ്പ് (safe_create_leading_directories_const(the_repository, output_directory)) {
		അവസ്ഥ SCLD_OK:
		അവസ്ഥ SCLD_EXISTS:
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		എങ്കിൽ (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	എങ്കിൽ (rev.pending.nr == 1) {
		പൂർണ്ണസംഖ്യ check_head = 0;

		എങ്കിൽ (rev.max_എണ്ണം < 0 && !rev.show_വേര്_diff) {
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

		എങ്കിൽ (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		എങ്കിൽ (check_head) {
			സ്ഥിരം അക്ഷരം *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			അല്ലെങ്കിൽ
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_വേര്_diff = 1;

	എങ്കിൽ (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		എങ്കിൽ (rev.pending.nr == 2) {
			ഘടന object_array_entry *o = rev.pending.objects;
			എങ്കിൽ (oideq(&o[0].item->oid, &o[1].item->oid))
				പോകുക done;
		}
		get_patch_ids(&rev, &ids);
	}

	എങ്കിൽ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	ആകുമ്പോൾ ((commit = get_revision(&rev)) != ശൂന്യ) {
		എങ്കിൽ (commit->object.flags & BOUNDARY) {
			boundary_എണ്ണം++;
			origin = (boundary_എണ്ണം == 1) ? commit : ശൂന്യ;
			തുടരുക;
		}

		എങ്കിൽ (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			തുടരുക;

		commit_അട്ടി_തള്ളുക(&list, commit);
	}
	എങ്കിൽ (!list.nr)
		/* nothing to do */
		പോകുക done;
	total = list.nr;

	എങ്കിൽ (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		എങ്കിൽ (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} അല്ലെങ്കിൽ എങ്കിൽ (cover_letter == -1) {
		cover_letter = 1;
	}

	എങ്കിൽ (cover_letter == -1) {
		എങ്കിൽ (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		അല്ലെങ്കിൽ എങ്കിൽ ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		അല്ലെങ്കിൽ
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	എങ്കിൽ (!cfg.keep_subject && cfg.auto_സംഖ്യ && (total > 1 || cover_letter))
		cfg.numbered = 1;
	എങ്കിൽ (cfg.numbered)
		rev.total = total + start_സംഖ്യ - 1;

	എങ്കിൽ (idiff_prev.nr) {
		എങ്കിൽ (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_വൃക്ഷം_oid(list.ഇനങ്ങൾ[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_എണ്ണം,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	എങ്കിൽ (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	അല്ലെങ്കിൽ എങ്കിൽ (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	എങ്കിൽ (rdiff_prev) {
		എങ്കിൽ (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.ഇനങ്ങൾ[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_എണ്ണം,
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
	എങ്കിൽ (!signature) {
		; /* --no-signature inhibits all signatures */
	} അല്ലെങ്കിൽ എങ്കിൽ (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} അല്ലെങ്കിൽ എങ്കിൽ (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ഘടന strbuf buf = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		എങ്കിൽ (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, ശൂന്യ);
	} അല്ലെങ്കിൽ എങ്കിൽ (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, വലുപ്പം(bases));
	base = get_base_commit(&cfg, list.ഇനങ്ങൾ, list.nr);
	എങ്കിൽ (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.ഇനങ്ങൾ, list.nr);
	}

	എങ്കിൽ (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(വലുപ്പം(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	എങ്കിൽ (in_reply_to) {
		അക്ഷരം *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	എങ്കിൽ (cover_letter) {
		എങ്കിൽ (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.ഇനങ്ങൾ,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_സംഖ്യ--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ശൂന്യ;
		rev.rdiff1 = ശൂന്യ;
	}
	rev.add_signoff = cfg.do_signoff;

	എങ്കിൽ (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	ആകുമ്പോൾ (list.nr) {
		size_t idx = list.nr - 1;
		പൂർണ്ണസംഖ്യ shown;

		display_progress(progress, total - idx);
		commit = commit_അട്ടി_എടുക്കുക(&list);
		rev.nr = total - idx + (start_സംഖ്യ - 1);

		/* Make the second and subsequent mails replies to the first */
		എങ്കിൽ (cfg.thread) {
			/* Have we already had a message ID? */
			എങ്കിൽ (rev.message_id) {
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
				എങ്കിൽ (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					സ്വതന്ത്രം(rev.message_id);
				അല്ലെങ്കിൽ
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		എങ്കിൽ (output_directory &&
		    open_next_file(rev.numbered_files ? ശൂന്യ : commit, ശൂന്യ, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_വൃക്ഷം_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		എങ്കിൽ (output_directory)
			rev.shown_one = 0;
		എങ്കിൽ (shown) {
			print_bases(&bases, rev.diffopt.file);
			എങ്കിൽ (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			അല്ലെങ്കിൽ
				print_signature(signature, rev.diffopt.file);
		}
		എങ്കിൽ (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ശൂന്യ;
		}
	}
	stop_progress(&progress);
	commit_അട്ടി_clear(&list);
	എങ്കിൽ (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	സ്വതന്ത്രം(description_file);
	സ്വതന്ത്രം(signature_file_arg);
	സ്വതന്ത്രം(signature_to_free);
	സ്വതന്ത്രം(branch_name);
	സ്വതന്ത്രം(to_free);
	സ്വതന്ത്രം(rev.message_id);
	എങ്കിൽ (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	സ്വതന്ത്രം(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_pending_commit(സ്ഥിരം അക്ഷരം *arg, ഘടന rev_info *revs, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന object_id oid;
	എങ്കിൽ (repo_get_oid(the_repository, arg, &oid) == 0) {
		ഘടന commit *commit = lookup_commit_reference(the_repository,
								&oid);
		എങ്കിൽ (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			തിരികെ 0;
		}
	}
	തിരികെ -1;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ശൂന്യ
};

സ്ഥിതം ശൂന്യം print_commit(അക്ഷരം sign, ഘടന commit *commit, പൂർണ്ണസംഖ്യ verbose,
			 പൂർണ്ണസംഖ്യ abbrev, FILE *file)
{
	എങ്കിൽ (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} അല്ലെങ്കിൽ {
		ഘടന strbuf buf = STRBUF_INIT;
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
സ്ഥിതം ശൂന്യം collect_diff_blob_oids(ഘടന commit *commit,
				   ഘടന diff_options *opts,
				   ഘടന oidset *blobs)
{
	ഘടന diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	പ്രതിജ്ഞാ(!commit->parents || !commit->parents->next);

	എങ്കിൽ (commit->parents)
		diff_വൃക്ഷം_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	അല്ലെങ്കിൽ
		diff_വേര്_വൃക്ഷം_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < q->nr; i++) {
		ഘടന diff_filepair *p = q->queue[i];
		ഘടന userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		എങ്കിൽ (drv && drv->ദ്വിമാന > 0)
			തുടരുക;

		എങ്കിൽ (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ശൂന്യ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_തിരുകുക(blobs, &p->one->oid);
		എങ്കിൽ (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ശൂന്യ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_തിരുകുക(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ always_match(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
			സ്ഥിരം ഘടന hashmap_entry *entry1 UNUSED,
			സ്ഥിരം ഘടന hashmap_entry *entry2 UNUSED,
			സ്ഥിരം ശൂന്യം *keydata UNUSED)
{
	തിരികെ 0;
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
സ്ഥിതം ശൂന്യം prefetch_cherry_blobs(ഘടന repository *repo,
				  ഘടന commit_list *list,
				  ഘടന patch_ids *ids)
{
	ഘടന oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	എങ്കിൽ (!repo_has_promisor_remote(repo))
		തിരികെ;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	വേണ്ടി (ഘടന commit_list *l = list; l; l = l->next) {
		ഘടന commit *c = l->item;
		bool match_found = false;
		വേണ്ടി (ഘടന patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		എങ്കിൽ (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	എങ്കിൽ (oidset_size(&blobs)) {
		ഘടന oid_array to_fetch = OID_ARRAY_INIT;
		ഘടന oidset_iter iter;
		സ്ഥിരം ഘടന object_id *oid;

		oidset_iter_init(&blobs, &iter);
		ആകുമ്പോൾ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

പൂർണ്ണസംഖ്യ cmd_cherry(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo UNUSED)
{
	ഘടന rev_info revs;
	ഘടന patch_ids ids;
	ഘടന commit *commit;
	ഘടന commit_list *list = ശൂന്യ;
	ഘടന branch *current_branch;
	സ്ഥിരം അക്ഷരം *upstream;
	സ്ഥിരം അക്ഷരം *head = "HEAD";
	സ്ഥിരം അക്ഷരം *limit = ശൂന്യ;
	പൂർണ്ണസംഖ്യ verbose = 0, abbrev = 0;

	ഘടന option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	തിരഞ്ഞെടുപ്പ് (argc) {
	അവസ്ഥ 3:
		limit = argv[2];
		/* FALLTHROUGH */
	അവസ്ഥ 2:
		head = argv[1];
		/* FALLTHROUGH */
	അവസ്ഥ 1:
		upstream = argv[0];
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		current_branch = branch_get(ശൂന്യ);
		upstream = branch_get_upstream(current_branch, ശൂന്യ);
		എങ്കിൽ (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	എങ്കിൽ (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	എങ്കിൽ (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	എങ്കിൽ (revs.pending.nr == 2) {
		ഘടന object_array_entry *o = revs.pending.objects;
		എങ്കിൽ (oideq(&o[0].item->oid, &o[1].item->oid))
			തിരികെ 0;
	}

	get_patch_ids(&revs, &ids);

	എങ്കിൽ (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	എങ്കിൽ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ആകുമ്പോൾ ((commit = get_revision(&revs)) != ശൂന്യ) {
		commit_list_തിരുകുക(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	വേണ്ടി (ഘടന commit_list *l = list; l; l = l->next) {
		അക്ഷരം sign = '+';

		commit = l->item;
		എങ്കിൽ (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	തിരികെ 0;
}
