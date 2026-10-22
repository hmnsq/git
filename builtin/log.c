/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "odb/streaming.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "diffcore.h"
#ಅನ್ತರ್ಭಾವ "diff-merges.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "reflog-walk.h"
#ಅನ್ತರ್ಭಾವ "patch-ids.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "shortlog.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "line-log.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "version.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "promisor-remote.h"
#ಅನ್ತರ್ಭಾವ "range-diff.h"
#ಅನ್ತರ್ಭಾವ "tmp-objdir.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "userdiff.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ force_in_body_from;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stdout_mboxrd;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ format_no_prefix;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ರಿಕ್ತ
};

ಸಂರಚನಾ ಸಾಲು_opt_callback_data {
	ಸಂರಚನಾ rev_info *rev;
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಸಂರಚನಾ string_list args;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ session_is_interactive(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ isatty(1) || pager_in_use();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ auto_decoration_style(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_decoration_style(ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ವಿಕಲ್ಪ (git_parse_maybe_bool(ಮೌಲ್ಯ)) {
	ಪಕ್ಷ 1:
		ಪ್ರತ್ಯಾಗಮ DECORATE_SHORT_REFS;
	ಪಕ್ಷ 0:
		ಪ್ರತ್ಯಾಗಮ 0;
	ಸ್ವಭಾವ:
		ಭಙ್ಗ;
	}
	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "full"))
		ಪ್ರತ್ಯಾಗಮ DECORATE_FULL_REFS;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಮೌಲ್ಯ, "short"))
		ಪ್ರತ್ಯಾಗಮ DECORATE_SHORT_REFS;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಮೌಲ್ಯ, "auto"))
		ಪ್ರತ್ಯಾಗಮ auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸಂರಚನಾ log_config {
	ಪೂರ್ಣಾಙ್ಕ default_abbrev_commit;
	ಪೂರ್ಣಾಙ್ಕ default_show_ಬೇರು;
	ಪೂರ್ಣಾಙ್ಕ default_follow;
	ಪೂರ್ಣಾಙ್ಕ default_show_signature;
	ಪೂರ್ಣಾಙ್ಕ default_encode_email_headers;
	ಪೂರ್ಣಾಙ್ಕ decoration_style;
	ಪೂರ್ಣಾಙ್ಕ decoration_given;
	ಪೂರ್ಣಾಙ್ಕ use_mailmap_config;
	ವರ್ಣ *fmt_patch_subject_prefix;
	ಪೂರ್ಣಾಙ್ಕ fmt_patch_name_max;
	ವರ್ಣ *fmt_pretty;
	ವರ್ಣ *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	ಪೂರ್ಣಾಙ್ಕ i_still_use_this;
#endif
};

ಸ್ಥಿರ ಶೂನ್ಯ log_config_init(ಸಂರಚನಾ log_config *cfg)
{
	memset(cfg, 0, ಪರಿಮಾಣ(*cfg));
	cfg->default_show_ಬೇರು = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ಸ್ಥಿರ ಶೂನ್ಯ log_config_release(ಸಂರಚನಾ log_config *cfg)
{
	ಮುಕ್ತ(cfg->default_date_mode);
	ಮುಕ್ತ(cfg->fmt_pretty);
	ಮುಕ್ತ(cfg->fmt_patch_subject_prefix);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ use_default_decoration_filter = 1;
ಸ್ಥಿರ ಸಂರಚನಾ string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಸಂರಚನಾ string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clear_decorations_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt UNUSED,
				      ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ decorate_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			     ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ log_config *cfg = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		cfg->decoration_style = 0;
	ಅನ್ಯಥಾ ಯದಿ (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	ಅನ್ಯಥಾ
		cfg->decoration_style = DECORATE_SHORT_REFS;

	ಯದಿ (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ log_ಸಾಲು_range_callback(ಧ್ರುವ ಸಂರಚನಾ option *option, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ ಸಾಲು_opt_callback_data *data = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!arg)
		ಪ್ರತ್ಯಾಗಮ -1;

	data->rev->ಸಾಲು_level_traverse = 1;
	string_list_append(&data->args, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ cmd_log_init_defaults(ಸಂರಚನಾ rev_info *rev,
				  ಸಂರಚನಾ log_config *cfg)
{
	ಯದಿ (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	ಯದಿ (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_ಬೇರು_diff = cfg->default_show_ಬೇರು;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	ಯದಿ (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_default_decoration_filter(ಸಂರಚನಾ decoration_filter *decoration_filter)
{
	ವರ್ಣ *ಮೌಲ್ಯ = ರಿಕ್ತ;
	ಸಂರಚನಾ string_list *include = decoration_filter->include_ref_pattern;
	ಧ್ರುವ ಸಂರಚನಾ string_list *config_exclude;

	ಯದಿ (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ಸಂರಚನಾ string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	ಯದಿ (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &ಮೌಲ್ಯ) &&
	    !strcmp("all", ಮೌಲ್ಯ))
		use_default_decoration_filter = 0;
	ಮುಕ್ತ(ಮೌಲ್ಯ);

	ಯದಿ (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		ಯದಿ (!ref_namespace[i].decoration)
			ಅನುವರ್ತ;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ cmd_log_init_finish(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ rev_info *rev, ಸಂರಚನಾ setup_revision_opt *opt,
			 ಸಂರಚನಾ log_config *cfg)
{
	ಸಂರಚನಾ userformat_want w;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0, source = 0, mailmap;
	ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು_opt_callback_data ಸಾಲು_cb = {ರಿಕ್ತ, ರಿಕ್ತ, STRING_LIST_INIT_DUP};
	ಸಂರಚನಾ decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	ಸ್ಥಿರ ಸಂರಚನಾ revision_sources revision_sources;

	ಧ್ರುವ ಸಂರಚನಾ option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ರಿಕ್ತ, ರಿಕ್ತ,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ರಿಕ್ತ, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ರಿಕ್ತ, &ಸಾಲು_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_ಸಾಲು_range_callback),
		OPT_END()
	};

	ಸಾಲು_cb.rev = rev;
	ಸಾಲು_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ಯದಿ (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	ಯದಿ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ಯದಿ (rev->ಸಾಲು_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, ಪರಿಮಾಣ(w));
	userformat_find_requirements(ರಿಕ್ತ, &w);

	ಯದಿ (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	ಯದಿ (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	ಯದಿ ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	ಯದಿ (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	ಯದಿ (mailmap) {
		rev->mailmap = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	ಯದಿ (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		ಯದಿ (!cfg->decoration_given)
			cfg->decoration_style = 0;
		ಯದಿ (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	ಯದಿ (rev->commit_format == CMIT_FMT_USERFORMAT) {
		ಯದಿ (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} ಅನ್ಯಥಾ ಯದಿ (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	ಯದಿ (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		ಯದಿ (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	ಯದಿ (rev->ಸಾಲು_level_traverse)
		ಸಾಲು_log_init(rev, ಸಾಲು_cb.prefix, &ಸಾಲು_cb.args);

	setup_pager(the_repository);
}

ಸ್ಥಿರ ಶೂನ್ಯ cmd_log_init(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ rev_info *rev, ಸಂರಚನಾ setup_revision_opt *opt,
			 ಸಂರಚನಾ log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_log_walk_no_free(ಸಂರಚನಾ rev_info *rev)
{
	ಸಂರಚನಾ commit *commit;
	ಪೂರ್ಣಾಙ್ಕ saved_nrl = 0;
	ಪೂರ್ಣಾಙ್ಕ saved_dcctc = 0;
	ಪೂರ್ಣಾಙ್ಕ result;

	ಯದಿ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	ಯಾವತ್ ((commit = get_revision(rev)) != ರಿಕ್ತ) {
		ಯದಿ (!log_ವೃಕ್ಷ_commit(rev, commit) && rev->max_ಎಣಿಕೆ >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_ಎಣಿಕೆ++;
		ಯದಿ (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = ರಿಕ್ತ;
		}
		ಯದಿ (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		ಯದಿ (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	ಯದಿ (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_log_walk(ಸಂರಚನಾ rev_info *rev)
{
	ಪೂರ್ಣಾಙ್ಕ retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_log_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			  ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ log_config *cfg = cb;
	ಧ್ರುವ ವರ್ಣ *slot_name;

	ಯದಿ (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->fmt_pretty, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->fmt_patch_subject_prefix, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->default_date_mode, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(ಮೌಲ್ಯ);
		ಯದಿ (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "log.diffmerges")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಪ್ರತ್ಯಾಗಮ diff_merges_config(ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "log.showroot")) {
		cfg->default_show_ಬೇರು = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (skip_prefix(var, "color.decorate.", &slot_name))
		ಪ್ರತ್ಯಾಗಮ parse_decorate_color_config(var, slot_name, ಮೌಲ್ಯ);
	ಯದಿ (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ git_diff_ui_config(var, ಮೌಲ್ಯ, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
ಪೂರ್ಣಾಙ್ಕ cmd_whatchanged(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ log_config cfg;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ setup_revision_opt opt;
	ಪೂರ್ಣಾಙ್ಕ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, ಪರಿಮಾಣ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ಯದಿ (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	ಯದಿ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ಪ್ರತ್ಯಾಗಮ ret;
}
#endif

ಸ್ಥಿರ ಶೂನ್ಯ show_tagger(ಧ್ರುವ ವರ್ಣ *buf, ಸಂರಚನಾ rev_info *rev)
{
	ಸಂರಚನಾ strbuf out = STRBUF_INIT;
	ಸಂರಚನಾ pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_blob_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ rev_info *rev, ಧ್ರುವ ವರ್ಣ *obj_name)
{
	ಸಂರಚನಾ object_id oidc;
	ಸಂರಚನಾ object_context obj_context = {0};
	ವರ್ಣ *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	ಯದಿ (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		ಪ್ರತ್ಯಾಗಮ odb_stream_blob_to_fd(the_repository->objects, 1, oid, ರಿಕ್ತ, 0);

	ಯದಿ (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	ಯದಿ (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		ಪ್ರತ್ಯಾಗಮ odb_stream_blob_to_fd(the_repository->objects, 1, oid, ರಿಕ್ತ, 0);
	}

	ಯದಿ (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_tag_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ rev_info *rev)
{
	size_t size;
	ಗಣನಾ object_type type;
	ವರ್ಣ *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ error(_("could not read object %s"), oid_to_hex(oid));

	ಪ್ರತಿಜ್ಞಾ(type == OBJ_TAG);
	ಯಾವತ್ (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		ಧ್ರುವ ವರ್ಣ *ident;
		ಯಾವತ್ (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		ಯದಿ (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	ಯದಿ (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ವೃಕ್ಷ_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid UNUSED,
			    ಸಂರಚನಾ strbuf *base UNUSED,
			    ಧ್ರುವ ವರ್ಣ *pathname, unsigned mode,
			    ಶೂನ್ಯ *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_setup_revisions_tweak(ಸಂರಚನಾ rev_info *rev)
{
	ಯದಿ (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	ಅನ್ಯಥಾ
		diff_merges_default_to_dense_combined(rev);
	ಯದಿ (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

ಪೂರ್ಣಾಙ್ಕ cmd_show(ಪೂರ್ಣಾಙ್ಕ argc,
	     ಧ್ರುವ ವರ್ಣ **argv,
	     ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ log_config cfg;
	ಸಂರಚನಾ rev_info rev;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ setup_revision_opt opt;
	ಸಂರಚನಾ pathspec match_all;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	ಯದಿ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, ಪರಿಮಾಣ(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, ಪರಿಮಾಣ(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ಯದಿ (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	rev.diffopt.no_free = 1;
	ಪ್ರತಿ (i = 0; i < rev.pending.nr && !ret; i++) {
		ಸಂರಚನಾ object *o = rev.pending.objects[i].item;
		ಧ್ರುವ ವರ್ಣ *name = rev.pending.objects[i].name;
		ವಿಕಲ್ಪ (o->type) {
		ಪಕ್ಷ OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_TAG: {
			ಸಂರಚನಾ tag *t = (ಸಂರಚನಾ tag *)o;
			ಸಂರಚನಾ object_id *oid = get_tagged_oid(t);

			ಯದಿ (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			ಯದಿ (ret)
				ಭಙ್ಗ;
			o = parse_object(the_repository, oid);
			ಯದಿ (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			ಭಙ್ಗ;
		}
		ಪಕ್ಷ OBJ_ವೃಕ್ಷ:
			ಯದಿ (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_ವೃಕ್ಷ(the_repository, (ಸಂರಚನಾ ವೃಕ್ಷ *)o,
				  &match_all, show_ವೃಕ್ಷ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_COMMIT:
		{
			ಸಂರಚನಾ object_array old;
			ಸಂರಚನಾ object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, ಪರಿಮಾಣ(old));
			memcpy(&rev.pending, &blank, ಪರಿಮಾಣ(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, ಪರಿಮಾಣ(rev.pending));
			ಭಙ್ಗ;
		}
		ಸ್ವಭಾವ:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
ಪೂರ್ಣಾಙ್ಕ cmd_log_reflog(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ log_config cfg;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ setup_revision_opt opt;
	ಪೂರ್ಣಾಙ್ಕ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, ಪರಿಮಾಣ(opt));
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
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ log_setup_revisions_tweak(ಸಂರಚನಾ rev_info *rev)
{
	ಯದಿ (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	ಯದಿ (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

ಪೂರ್ಣಾಙ್ಕ cmd_log(ಪೂರ್ಣಾಙ್ಕ argc,
	    ಧ್ರುವ ವರ್ಣ **argv,
	    ಧ್ರುವ ವರ್ಣ *prefix,
	    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ log_config cfg;
	ಸಂರಚನಾ rev_info rev;
	ಸಂರಚನಾ setup_revision_opt opt;
	ಪೂರ್ಣಾಙ್ಕ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, ಪರಿಮಾಣ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/* format-patch */

ಗಣನಾ cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

ಗಣನಾ thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

ಗಣನಾ cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

ಗಣನಾ auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

ಸಂರಚನಾ format_config {
	ಸಂರಚನಾ log_config log;
	ಗಣನಾ thread_level thread;
	ಪೂರ್ಣಾಙ್ಕ do_signoff;
	ಗಣನಾ auto_base_setting auto_base;
	ವರ್ಣ *base_commit;
	ವರ್ಣ *from;
	ವರ್ಣ *signature;
	ವರ್ಣ *signature_file;
	ಗಣನಾ cover_setting config_cover_letter;
	ವರ್ಣ *fmt_cover_letter_commit_list;
	ವರ್ಣ *config_output_directory;
	ಗಣನಾ cover_from_description cover_from_description_mode;
	ಪೂರ್ಣಾಙ್ಕ show_notes;
	ಸಂರಚನಾ display_notes_opt notes_opt;
	ಪೂರ್ಣಾಙ್ಕ numbered_cmdline_opt;
	ಪೂರ್ಣಾಙ್ಕ numbered;
	ಪೂರ್ಣಾಙ್ಕ auto_ಸಂಖ್ಯೆ;
	ವರ್ಣ *default_attach;
	ಸಂರಚನಾ string_list extra_hdr;
	ಸಂರಚನಾ string_list extra_to;
	ಸಂರಚನಾ string_list extra_cc;
	ಪೂರ್ಣಾಙ್ಕ keep_subject;
	ಪೂರ್ಣಾಙ್ಕ subject_prefix;
	ಸಂರಚನಾ strbuf sprefix;
	ವರ್ಣ *fmt_patch_suffix;
};

ಸ್ಥಿರ ಶೂನ್ಯ format_config_init(ಸಂರಚನಾ format_config *cfg)
{
	memset(cfg, 0, ಪರಿಮಾಣ(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_ಸಂಖ್ಯೆ = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ಸ್ಥಿರ ಶೂನ್ಯ format_config_release(ಸಂರಚನಾ format_config *cfg)
{
	log_config_release(&cfg->log);
	ಮುಕ್ತ(cfg->base_commit);
	ಮುಕ್ತ(cfg->from);
	ಮುಕ್ತ(cfg->signature);
	ಮುಕ್ತ(cfg->signature_file);
	ಮುಕ್ತ(cfg->config_output_directory);
	ಮುಕ್ತ(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	ಮುಕ್ತ(cfg->fmt_patch_suffix);
	ಮುಕ್ತ(cfg->fmt_cover_letter_commit_list);
}

ಸ್ಥಿರ ಗಣನಾ cover_from_description parse_cover_from_description(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ (!arg || !strcmp(arg, "default"))
		ಪ್ರತ್ಯಾಗಮ COVER_FROM_MESSAGE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "none"))
		ಪ್ರತ್ಯಾಗಮ COVER_FROM_NONE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "message"))
		ಪ್ರತ್ಯಾಗಮ COVER_FROM_MESSAGE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "subject"))
		ಪ್ರತ್ಯಾಗಮ COVER_FROM_SUBJECT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "auto"))
		ಪ್ರತ್ಯಾಗಮ COVER_FROM_AUTO;
	ಅನ್ಯಥಾ
		die(_("%s: invalid cover from description mode"), arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_header(ಸಂರಚನಾ format_config *cfg, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ಸಂರಚನಾ string_list_item *item;
	ಪೂರ್ಣಾಙ್ಕ len = strlen(ಮೌಲ್ಯ);
	ಯಾವತ್ (len && ಮೌಲ್ಯ[len - 1] == '\n')
		len--;

	ಯದಿ (!strncasecmp(ಮೌಲ್ಯ, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, ಮೌಲ್ಯ + 4);
		len -= 4;
	} ಅನ್ಯಥಾ ಯದಿ (!strncasecmp(ಮೌಲ್ಯ, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, ಮೌಲ್ಯ + 4);
		len -= 4;
	} ಅನ್ಯಥಾ {
		item = string_list_append(&cfg->extra_hdr, ಮೌಲ್ಯ);
	}

	item->string[len] = '\0';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_format_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			     ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ format_config *cfg = cb;

	ಯದಿ (!strcmp(var, "format.headers")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			die(_("format.headers without value"));
		add_header(cfg, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->fmt_patch_suffix, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.to")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		string_list_append(&cfg->extra_to, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.cc")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.numbered")) {
		ಯದಿ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "auto")) {
			cfg->auto_ಸಂಖ್ಯೆ = 1;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		cfg->numbered = git_config_bool(var, ಮೌಲ್ಯ);
		cfg->auto_ಸಂಖ್ಯೆ = cfg->auto_ಸಂಖ್ಯೆ && cfg->numbered;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.attach")) {
		ಯದಿ (ಮೌಲ್ಯ && *ಮೌಲ್ಯ) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(ಮೌಲ್ಯ);
		} ಅನ್ಯಥಾ ಯದಿ (ಮೌಲ್ಯ && !*ಮೌಲ್ಯ) {
			FREE_AND_NULL(cfg->default_attach);
		} ಅನ್ಯಥಾ {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.thread")) {
		ಯದಿ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "deep")) {
			cfg->thread = THREAD_DEEP;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		ಯದಿ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		cfg->thread = git_config_bool(var, ಮೌಲ್ಯ) ? THREAD_SHALLOW : THREAD_UNSET;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->signature, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		ಪ್ರತ್ಯಾಗಮ git_config_pathname(&cfg->signature_file, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.coverletter")) {
		ಯದಿ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		cfg->config_cover_letter = git_config_bool(var, ಮೌಲ್ಯ) ? COVER_ON : COVER_OFF;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->fmt_cover_letter_commit_list, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		ಪ್ರತ್ಯಾಗಮ git_config_string(&cfg->config_output_directory, var, ಮೌಲ್ಯ);
	}
	ಯದಿ (!strcmp(var, "format.useautobase")) {
		ಯದಿ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		cfg->auto_base = git_config_bool(var, ಮೌಲ್ಯ) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.from")) {
		ಪೂರ್ಣಾಙ್ಕ b = git_parse_maybe_bool(ಮೌಲ್ಯ);
		FREE_AND_NULL(cfg->from);
		ಯದಿ (b < 0)
			cfg->from = xstrdup(ಮೌಲ್ಯ);
		ಅನ್ಯಥಾ ಯದಿ (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.notes")) {
		ಪೂರ್ಣಾಙ್ಕ b = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಯದಿ (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, ಮೌಲ್ಯ);
		ಅನ್ಯಥಾ ಯದಿ (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ಅನ್ಯಥಾ
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಯದಿ (format_no_prefix < 0) {
			ಪೂರ್ಣಾಙ್ಕ status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				ಮೌಲ್ಯ, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	ಯದಿ (!strcmp(var, "diff.noprefix"))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ git_log_config(var, ಮೌಲ್ಯ, ctx, &cfg->log);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *output_directory = ರಿಕ್ತ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ outdir_offset;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ open_next_file(ಸಂರಚನಾ commit *commit, ಧ್ರುವ ವರ್ಣ *subject,
			 ಸಂರಚನಾ rev_info *rev, ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಸಂರಚನಾ strbuf filename = STRBUF_INIT;

	ಯದಿ (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	ಯದಿ (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	ಅನ್ಯಥಾ ಯದಿ (commit)
		fmt_output_commit(&filename, commit, rev);
	ಅನ್ಯಥಾ
		fmt_output_subject(&filename, subject, rev);

	ಯದಿ (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	ಯದಿ (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	strbuf_release(&filename);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_patch_ids(ಸಂರಚನಾ rev_info *rev, ಸಂರಚನಾ patch_ids *ids)
{
	ಸಂರಚನಾ rev_info check_rev;
	ಸಂರಚನಾ commit *commit, *c1, *c2;
	ಸಂರಚನಾ object *o1, *o2;
	unsigned flags1, flags2;

	ಯದಿ (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	ಯದಿ ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	ಯದಿ (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	ಯಾವತ್ ((commit = get_revision(&check_rev)) != ರಿಕ್ತ) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ಸ್ಥಿರ ಶೂನ್ಯ gen_message_id(ಸಂರಚನಾ rev_info *info, ಧ್ರುವ ವರ್ಣ *base)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(ರಿಕ್ತ),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_signature(ಧ್ರುವ ವರ್ಣ *signature, FILE *file)
{
	ಯದಿ (!signature || !*signature)
		ಪ್ರತ್ಯಾಗಮ;

	fprintf(file, "-- \n%s", signature);
	ಯದಿ (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ಸ್ಥಿರ ವರ್ಣ *find_branch_name(ಸಂರಚನಾ rev_info *rev)
{
	ಸಂರಚನಾ object_id branch_oid;
	ಧ್ರುವ ಸಂರಚನಾ object_id *tip_oid;
	ಧ್ರುವ ವರ್ಣ *ref, *v;
	ವರ್ಣ *full_ref, *branch = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ interesting_found = 0;
	size_t idx;

	ಪ್ರತಿ (size_t i = 0; i < rev->cmdline.nr; i++) {
		ಯದಿ (rev->cmdline.rev[i].flags & UNINTERESTING)
			ಅನುವರ್ತ;
		ಯದಿ (interesting_found)
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		interesting_found = 1;
		idx = i;
	}
	ಯದಿ (!interesting_found)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	ಯದಿ (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	ಮುಕ್ತ(full_ref);
	ಪ್ರತ್ಯಾಗಮ branch;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_diffstat(ಸಂರಚನಾ rev_info *rev,
			  ಸಂರಚನಾ commit *origin, ಸಂರಚನಾ commit *head)
{
	ಸಂರಚನಾ diff_options opts;

	memcpy(&opts, &rev->diffopt, ಪರಿಮಾಣ(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_ವೃಕ್ಷ_oid(get_commit_ವೃಕ್ಷ_oid(origin),
		      get_commit_ವೃಕ್ಷ_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ read_desc_file(ಸಂರಚನಾ strbuf *buf, ಧ್ರುವ ವರ್ಣ *desc_file)
{
	ಯದಿ (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_cover_text(ಸಂರಚನಾ pretty_print_context *pp,
			       ಧ್ರುವ ವರ್ಣ *description_file,
			       ಧ್ರುವ ವರ್ಣ *branch_name,
			       ಸಂರಚನಾ strbuf *sb,
			       ಧ್ರುವ ವರ್ಣ *encoding,
			       ಪೂರ್ಣಾಙ್ಕ need_8bit_cte,
			       ಧ್ರುವ ಸಂರಚನಾ format_config *cfg)
{
	ಧ್ರುವ ವರ್ಣ *subject = "*** SUBJECT HERE ***";
	ಧ್ರುವ ವರ್ಣ *body = "*** BLURB HERE ***";
	ಸಂರಚನಾ strbuf description_sb = STRBUF_INIT;
	ಸಂರಚನಾ strbuf subject_sb = STRBUF_INIT;

	ಯದಿ (cfg->cover_from_description_mode == COVER_FROM_NONE)
		ಗಚ್ಛ do_pp;

	ಯದಿ (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	ಅನ್ಯಥಾ ಯದಿ (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	ಯದಿ (!description_sb.len)
		ಗಚ್ಛ do_pp;

	ಯದಿ (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	ಯದಿ (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	ಅನ್ಯಥಾ
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ಸಂರಚನಾ rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ಸಂರಚನಾ string_list notes;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rdiff_notes_cb(ಧ್ರುವ ಸಂರಚನಾ option *option,
			  ಧ್ರುವ ವರ್ಣ *arg,
			  ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ option opt = *option;
	ಸಂರಚನಾ rdiff_notes *rdiff_notes = option->ಮೌಲ್ಯ;

	rdiff_notes->override = 1;
	opt.ಮೌಲ್ಯ = &rdiff_notes->notes;
	ಪ್ರತ್ಯಾಗಮ parse_opt_string_list(&opt, arg, unset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_notes_refs(ಸಂರಚನಾ string_list_item *item, ಶೂನ್ಯ *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_notes_args(ಸಂರಚನಾ rdiff_notes *rdiff_notes,
			   ಸಂರಚನಾ rev_info *rev)
{
	ಯದಿ (rdiff_notes->override) {
		ಯದಿ (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		ಅನ್ಯಥಾ
			strvec_ತಳ್ಳು(&rev->rdiff_log_arg, "--no-notes");
	} ಅನ್ಯಥಾ ಯದಿ (!rev->show_notes) {
		strvec_ತಳ್ಳು(&rev->rdiff_log_arg, "--no-notes");
	} ಅನ್ಯಥಾ ಯದಿ (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ತಳ್ಳು(&rev->rdiff_log_arg, "--notes");
	} ಅನ್ಯಥಾ {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ generate_shortlog_cover_letter(ಸಂರಚನಾ shortlog *log,
					   ಸಂರಚನಾ rev_info *rev,
					   ಸಂರಚನಾ commit **list,
					   ಪೂರ್ಣಾಙ್ಕ nr)
{
	shortlog_init(log);
	log->wrap_ಸಾಲುಗಳು = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

ಸ್ಥಿರ ಶೂನ್ಯ generate_commit_list_cover(FILE *cover_file, ಧ್ರುವ ವರ್ಣ *format,
				       ಸಂರಚನಾ commit **list, ಪೂರ್ಣಾಙ್ಕ n)
{
	ಸಂರಚನಾ strbuf commit_ಸಾಲು = STRBUF_INIT;
	ಸಂರಚನಾ pretty_print_context ctx = {0};
	ಸಂರಚನಾ rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_ಸಾಲು, &ctx);
		fprintf(cover_file, "%s\n", commit_ಸಾಲು.buf);
		strbuf_reset(&commit_ಸಾಲು);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_ಸಾಲು);
}

ಸ್ಥಿರ ಶೂನ್ಯ make_cover_letter(ಸಂರಚನಾ rev_info *rev, ಪೂರ್ಣಾಙ್ಕ use_separate_file,
			      ಸಂರಚನಾ commit *origin,
			      ಪೂರ್ಣಾಙ್ಕ nr, ಸಂರಚನಾ commit **list,
			      ಧ್ರುವ ವರ್ಣ *description_file,
			      ಧ್ರುವ ವರ್ಣ *branch_name,
			      ಪೂರ್ಣಾಙ್ಕ quiet,
			      ಧ್ರುವ ಸಂರಚನಾ format_config *cfg,
			      ಧ್ರುವ ವರ್ಣ *format)
{
	ಧ್ರುವ ವರ್ಣ *from;
	ಸಂರಚನಾ shortlog log;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಧ್ರುವ ವರ್ಣ *encoding = "UTF-8";
	ಪೂರ್ಣಾಙ್ಕ need_8bit_cte = 0;
	ಸಂರಚನಾ pretty_print_context pp = {0};
	ಸಂರಚನಾ commit *head = list[0];
	ವರ್ಣ *to_free = ರಿಕ್ತ;

	ಯದಿ (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	ಯದಿ (use_separate_file &&
	    open_next_file(ರಿಕ್ತ, rev->numbered_files ? ರಿಕ್ತ : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	ಪ್ರತಿ (i = 0; !need_8bit_cte && i < nr; i++) {
		ಧ್ರುವ ವರ್ಣ *buf = repo_get_commit_buffer(the_repository,
							 list[i], ರಿಕ್ತ);
		ಯದಿ (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	ಯದಿ (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, ರಿಕ್ತ, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	ಮುಕ್ತ(to_free);
	ಮುಕ್ತ(pp.after_subject);
	strbuf_release(&sb);

	ಯದಿ (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	ಅನ್ಯಥಾ ಯದಿ (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ಅನ್ಯಥಾ
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	ಯದಿ (origin)
		show_diffstat(rev, origin, head);

	ಯದಿ (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	ಯದಿ (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ಸಂರಚನಾ diff_options opts;
		ಸಂರಚನಾ range_diff_options range_diff_opts = {
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

ಸ್ಥಿರ ವರ್ಣ *clean_message_id(ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_id)
{
	ವರ್ಣ ch;
	ಧ್ರುವ ವರ್ಣ *a, *z, *m;

	m = ಸಂದೇಶ_id;
	ಯಾವತ್ ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = ರಿಕ್ತ;
	ಯಾವತ್ ((ch = *m)) {
		ಯದಿ (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	ಯದಿ (!z)
		die(_("insane in-reply-to: %s"), ಸಂದೇಶ_id);
	ಯದಿ (++z == m)
		ಪ್ರತ್ಯಾಗಮ xstrdup(a);
	ಪ್ರತ್ಯಾಗಮ xmemdupz(a, z - a);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *set_outdir(ಧ್ರುವ ವರ್ಣ *prefix, ಧ್ರುವ ವರ್ಣ *output_directory)
{
	ಯದಿ (output_directory && is_absolute_path(output_directory))
		ಪ್ರತ್ಯಾಗಮ output_directory;

	ಯದಿ (!prefix || !*prefix) {
		ಯದಿ (output_directory)
			ಪ್ರತ್ಯಾಗಮ output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		ಪ್ರತ್ಯಾಗಮ "./";
	}

	outdir_offset = strlen(prefix);
	ಯದಿ (!output_directory)
		ಪ್ರತ್ಯಾಗಮ prefix;

	ಪ್ರತ್ಯಾಗಮ prefix_filename(prefix, output_directory);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ರಿಕ್ತ
};

ಸಂರಚನಾ keep_callback_data {
	ಸಂರಚನಾ format_config *cfg;
	ಸಂರಚನಾ rev_info *revs;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ keep_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ keep_callback_data *data = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ subject_prefix_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			    ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ format_config *cfg = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rfc_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಧ್ರುವ ವರ್ಣ **rfc = opt->ಮೌಲ್ಯ;

	*rfc = opt->ಮೌಲ್ಯ;
	ಯದಿ (unset)
		*rfc = ರಿಕ್ತ;
	ಅನ್ಯಥಾ
		*rfc = arg ? arg : "RFC";
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ numbered_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			     ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ format_config *cfg = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	ಯದಿ (unset)
		cfg->auto_ಸಂಖ್ಯೆ =  0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_numbered_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
				ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	ಪ್ರತ್ಯಾಗಮ numbered_callback(opt, arg, 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ output_directory_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			      ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಧ್ರುವ ವರ್ಣ **dir = (ಧ್ರುವ ವರ್ಣ **)opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	ಯದಿ (*dir)
		die(_("two output directories?"));
	*dir = arg;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ thread_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ format_config *cfg = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		cfg->thread = THREAD_UNSET;
	ಅನ್ಯಥಾ ಯದಿ (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ attach_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rev_info *rev = (ಸಂರಚನಾ rev_info *)opt->ಮೌಲ್ಯ;
	ಯದಿ (unset)
		rev->mime_boundary = ರಿಕ್ತ;
	ಅನ್ಯಥಾ ಯದಿ (arg)
		rev->mime_boundary = arg;
	ಅನ್ಯಥಾ
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ inline_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ rev_info *rev = (ಸಂರಚನಾ rev_info *)opt->ಮೌಲ್ಯ;
	ಯದಿ (unset)
		rev->mime_boundary = ರಿಕ್ತ;
	ಅನ್ಯಥಾ ಯದಿ (arg)
		rev->mime_boundary = arg;
	ಅನ್ಯಥಾ
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ header_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			   ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ format_config *cfg = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} ಅನ್ಯಥಾ {
		add_header(cfg, arg);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ from_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ವರ್ಣ **from = opt->ಮೌಲ್ಯ;

	ಮುಕ್ತ(*from);

	ಯದಿ (unset)
		*from = ರಿಕ್ತ;
	ಅನ್ಯಥಾ ಯದಿ (arg)
		*from = xstrdup(arg);
	ಅನ್ಯಥಾ
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ base_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ format_config *cfg = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} ಅನ್ಯಥಾ {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ base_ವೃಕ್ಷ_info {
	ಸಂರಚನಾ object_id base_commit;
	ಪೂರ್ಣಾಙ್ಕ nr_patch_id, alloc_patch_id;
	ಸಂರಚನಾ object_id *patch_id;
};

ಸ್ಥಿರ ಸಂರಚನಾ commit *get_base_commit(ಧ್ರುವ ಸಂರಚನಾ format_config *cfg,
				      ಸಂರಚನಾ commit **list,
				      size_t total)
{
	ಸಂರಚನಾ commit *base = ರಿಕ್ತ;
	ಸಂರಚನಾ commit **rev;
	ಪೂರ್ಣಾಙ್ಕ auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	ವಿಕಲ್ಪ (cfg->auto_base) {
	ಪಕ್ಷ AUTO_BASE_NEVER:
		ಯದಿ (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} ಅನ್ಯಥಾ {
			/* no base information is requested */
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		}
		ಭಙ್ಗ;
	ಪಕ್ಷ AUTO_BASE_ALWAYS:
	ಪಕ್ಷ AUTO_BASE_WHEN_ABLE:
		ಯದಿ (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} ಅನ್ಯಥಾ {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected automatic base selection method");
	}

	ಯದಿ (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		ಯದಿ (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ branch *curr_branch = branch_get(ರಿಕ್ತ);
		ಧ್ರುವ ವರ್ಣ *upstream = branch_get_upstream(curr_branch, ರಿಕ್ತ);
		ಯದಿ (upstream) {
			ಸಂರಚನಾ commit_list *base_list = ರಿಕ್ತ;
			ಸಂರಚನಾ commit *commit;
			ಸಂರಚನಾ object_id oid;

			ಯದಿ (repo_get_oid(the_repository, upstream, &oid)) {
				ಯದಿ (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				ಅನ್ಯಥಾ
					ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			ಯದಿ (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				ಯದಿ (die_on_failure) {
					die(_("could not find exact merge base"));
				} ಅನ್ಯಥಾ {
					commit_list_free(base_list);
					ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} ಅನ್ಯಥಾ {
			ಯದಿ (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			ಅನ್ಯಥಾ
				ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		}
	}

	ALLOC_ARRAY(rev, total);
	ಪ್ರತಿ (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	ಯಾವತ್ (rev_nr > 1) {
		ಪ್ರತಿ (i = 0; i < rev_nr / 2; i++) {
			ಸಂರಚನಾ commit_list *merge_base = ರಿಕ್ತ;
			ಯದಿ (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				ಯದಿ (die_on_failure) {
					die(_("failed to find exact merge base"));
				} ಅನ್ಯಥಾ {
					commit_list_free(merge_base);
					ಮುಕ್ತ(rev);
					ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		ಯದಿ (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	ಯದಿ (ret < 0)
		exit(128);
	ಯದಿ (!ret) {
		ಯದಿ (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} ಅನ್ಯಥಾ {
			ಮುಕ್ತ(rev);
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		}
	}

	ಪ್ರತಿ (i = 0; i < total; i++) {
		ಯದಿ (base == list[i]) {
			ಯದಿ (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} ಅನ್ಯಥಾ {
				ಮುಕ್ತ(rev);
				ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
			}
		}
	}

	ಮುಕ್ತ(rev);
	ಪ್ರತ್ಯಾಗಮ base;
}

define_commit_slab(commit_base, ಪೂರ್ಣಾಙ್ಕ);

ಸ್ಥಿರ ಶೂನ್ಯ prepare_bases(ಸಂರಚನಾ base_ವೃಕ್ಷ_info *bases,
			  ಸಂರಚನಾ commit *base,
			  ಸಂರಚನಾ commit **list,
			  size_t total)
{
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ diff_options diffopt;
	ಸಂರಚನಾ commit_base commit_base;

	ಯದಿ (!base)
		ಪ್ರತ್ಯಾಗಮ;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);
	revs.max_parents = 1;
	revs.topo_order = 1;
	ಪ್ರತಿ (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	ಯದಿ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	ಯಾವತ್ ((commit = get_revision(&revs)) != ರಿಕ್ತ) {
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ object_id *patch_id;
		ಯದಿ (*commit_base_at(&commit_base, commit))
			ಅನುವರ್ತ;
		ಯದಿ (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_bases(ಸಂರಚನಾ base_ವೃಕ್ಷ_info *bases, FILE *file)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	/* Only do this once, either for the cover or for the first one */
	ಯದಿ (is_null_oid(&bases->base_commit))
		ಪ್ರತ್ಯಾಗಮ;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	ಪ್ರತಿ (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	ಮುಕ್ತ(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *diff_title(ಸಂರಚನಾ strbuf *sb,
			      ಧ್ರುವ ವರ್ಣ *reroll_ಎಣಿಕೆ,
			      ಧ್ರುವ ವರ್ಣ *generic,
			      ಧ್ರುವ ವರ್ಣ *rerolled)
{
	ಪೂರ್ಣಾಙ್ಕ v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	ಯದಿ (reroll_ಎಣಿಕೆ && !strtol_i(reroll_ಎಣಿಕೆ, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	ಅನ್ಯಥಾ
		strbuf_addstr(sb, generic);
	ಪ್ರತ್ಯಾಗಮ sb->buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ infer_range_diff_ranges(ಸಂರಚನಾ strbuf *r1,
				    ಸಂರಚನಾ strbuf *r2,
				    ಧ್ರುವ ವರ್ಣ *prev,
				    ಸಂರಚನಾ commit *origin,
				    ಸಂರಚನಾ commit *head)
{
	ಧ್ರುವ ವರ್ಣ *head_oid = oid_to_hex(&head->object.oid);
	ಪೂರ್ಣಾಙ್ಕ prev_is_range = is_range_diff_range(prev);

	ಯದಿ (prev_is_range)
		strbuf_addstr(r1, prev);
	ಅನ್ಯಥಾ
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	ಯದಿ (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	ಅನ್ಯಥಾ ಯದಿ (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	ಅನ್ಯಥಾ {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

ಪೂರ್ಣಾಙ್ಕ cmd_format_patch(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ format_config cfg;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ commit_ರಾಶಿ list = COMMIT_ರಾಶಿ_INIT;
	ಸಂರಚನಾ rev_info rev;
	ವರ್ಣ *to_free = ರಿಕ್ತ;
	ಸಂರಚನಾ setup_revision_opt s_r_opt;
	size_t total, i;
	ಪೂರ್ಣಾಙ್ಕ use_stdout = 0;
	ಪೂರ್ಣಾಙ್ಕ start_ಸಂಖ್ಯೆ = -1;
	ಪೂರ್ಣಾಙ್ಕ just_numbers = 0;
	ಪೂರ್ಣಾಙ್ಕ ignore_if_in_upstream = 0;
	ಪೂರ್ಣಾಙ್ಕ cover_letter = -1;
	ಧ್ರುವ ವರ್ಣ *cover_letter_fmt = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ boundary_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಙ್ಕ no_ದ್ವಿಮಾನ_diff = 0;
	ಪೂರ್ಣಾಙ್ಕ zero_commit = 0;
	ಸಂರಚನಾ commit *origin = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *in_reply_to = ರಿಕ್ತ;
	ಸಂರಚನಾ patch_ids ids;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ use_patch_format = 0;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಧ್ರುವ ವರ್ಣ *reroll_ಎಣಿಕೆ = ರಿಕ್ತ;
	ವರ್ಣ *cover_from_description_arg = ರಿಕ್ತ;
	ವರ್ಣ *description_file = ರಿಕ್ತ;
	ವರ್ಣ *branch_name = ರಿಕ್ತ;
	ಸಂರಚನಾ base_ವೃಕ್ಷ_info bases;
	ಸಂರಚನಾ commit *base;
	ಪೂರ್ಣಾಙ್ಕ show_progress = 0;
	ಸಂರಚನಾ progress *progress = ರಿಕ್ತ;
	ಸಂರಚನಾ oid_array idiff_prev = OID_ARRAY_INIT;
	ಸಂರಚನಾ strbuf idiff_title = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *rdiff_prev = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf rdiff1 = STRBUF_INIT;
	ಸಂರಚನಾ strbuf rdiff2 = STRBUF_INIT;
	ಸಂರಚನಾ strbuf rdiff_title = STRBUF_INIT;
	ಸಂರಚನಾ rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ಧ್ರುವ ವರ್ಣ *rfc = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ creation_factor = -1;
	ಧ್ರುವ ವರ್ಣ *signature = git_version_string;
	ವರ್ಣ *signature_to_free = ರಿಕ್ತ;
	ವರ್ಣ *signature_file_arg = ರಿಕ್ತ;
	ಸಂರಚನಾ keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ಧ್ರುವ ವರ್ಣ *fmt_patch_suffix = ರಿಕ್ತ;

	ಧ್ರುವ ಸಂರಚನಾ option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ರಿಕ್ತ,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ರಿಕ್ತ,
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
		OPT_INTEGER(0, "start-number", &start_ಸಂಖ್ಯೆ,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_ಎಣಿಕೆ, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, ರಿಕ್ತ,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ದ್ವಿಮಾನ_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, ಪರಿಮಾಣ(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, ಪರಿಮಾಣ(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	ಯದಿ (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	ಯದಿ (cfg.default_attach) {
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

	ಯದಿ (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	ಯದಿ (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	ಯದಿ (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	ಯದಿ (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		ಯದಿ (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		ಅನ್ಯಥಾ
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	ಯದಿ (reroll_ಎಣಿಕೆ) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_ಎಣಿಕೆ);
		rev.reroll_ಎಣಿಕೆ = reroll_ಎಣಿಕೆ;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	ಪ್ರತಿ (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.ವಸ್ತುಗಳು[i].string);
		strbuf_addch(&buf, '\n');
	}

	ಯದಿ (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	ಪ್ರತಿ (i = 0; i < cfg.extra_to.nr; i++) {
		ಯದಿ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.ವಸ್ತುಗಳು[i].string);
		ಯದಿ (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	ಯದಿ (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	ಪ್ರತಿ (i = 0; i < cfg.extra_cc.nr; i++) {
		ಯದಿ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.ವಸ್ತುಗಳು[i].string);
		ಯದಿ (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, ರಿಕ್ತ);

	ಯದಿ (cfg.from) {
		ಯದಿ (split_ident_ಸಾಲು(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	ಯದಿ (start_ಸಂಖ್ಯೆ < 0)
		start_ಸಂಖ್ಯೆ = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	ಯದಿ (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	ಯದಿ (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	ಯದಿ (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	ಯದಿ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ಯದಿ (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	ಯದಿ (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	ಯದಿ (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	ಯದಿ (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	ಯದಿ (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	ಯದಿ (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	ಯದಿ (!rev.diffopt.flags.text && !no_ದ್ವಿಮಾನ_diff)
		rev.diffopt.flags.ದ್ವಿಮಾನ = 1;

	ಯದಿ (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	ಯದಿ (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	ಯದಿ (use_stdout) {
		setup_pager(the_repository);
	} ಅನ್ಯಥಾ ಯದಿ (!rev.diffopt.close_file) {
		ಪೂರ್ಣಾಙ್ಕ saved;

		ಯದಿ (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		ಯದಿ (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		ವಿಕಲ್ಪ (safe_create_leading_directories_const(the_repository, output_directory)) {
		ಪಕ್ಷ SCLD_OK:
		ಪಕ್ಷ SCLD_EXISTS:
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		ಯದಿ (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	ಯದಿ (rev.pending.nr == 1) {
		ಪೂರ್ಣಾಙ್ಕ check_head = 0;

		ಯದಿ (rev.max_ಎಣಿಕೆ < 0 && !rev.show_ಬೇರು_diff) {
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

		ಯದಿ (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		ಯದಿ (check_head) {
			ಧ್ರುವ ವರ್ಣ *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			ಅನ್ಯಥಾ
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_ಬೇರು_diff = 1;

	ಯದಿ (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		ಯದಿ (rev.pending.nr == 2) {
			ಸಂರಚನಾ object_array_entry *o = rev.pending.objects;
			ಯದಿ (oideq(&o[0].item->oid, &o[1].item->oid))
				ಗಚ್ಛ done;
		}
		get_patch_ids(&rev, &ids);
	}

	ಯದಿ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	ಯಾವತ್ ((commit = get_revision(&rev)) != ರಿಕ್ತ) {
		ಯದಿ (commit->object.flags & BOUNDARY) {
			boundary_ಎಣಿಕೆ++;
			origin = (boundary_ಎಣಿಕೆ == 1) ? commit : ರಿಕ್ತ;
			ಅನುವರ್ತ;
		}

		ಯದಿ (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			ಅನುವರ್ತ;

		commit_ರಾಶಿ_ತಳ್ಳು(&list, commit);
	}
	ಯದಿ (!list.nr)
		/* nothing to do */
		ಗಚ್ಛ done;
	total = list.nr;

	ಯದಿ (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		ಯದಿ (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} ಅನ್ಯಥಾ ಯದಿ (cover_letter == -1) {
		cover_letter = 1;
	}

	ಯದಿ (cover_letter == -1) {
		ಯದಿ (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		ಅನ್ಯಥಾ ಯದಿ ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		ಅನ್ಯಥಾ
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	ಯದಿ (!cfg.keep_subject && cfg.auto_ಸಂಖ್ಯೆ && (total > 1 || cover_letter))
		cfg.numbered = 1;
	ಯದಿ (cfg.numbered)
		rev.total = total + start_ಸಂಖ್ಯೆ - 1;

	ಯದಿ (idiff_prev.nr) {
		ಯದಿ (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_ವೃಕ್ಷ_oid(list.ವಸ್ತುಗಳು[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_ಎಣಿಕೆ,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	ಯದಿ (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	ಅನ್ಯಥಾ ಯದಿ (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	ಯದಿ (rdiff_prev) {
		ಯದಿ (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.ವಸ್ತುಗಳು[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_ಎಣಿಕೆ,
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
	ಯದಿ (!signature) {
		; /* --no-signature inhibits all signatures */
	} ಅನ್ಯಥಾ ಯದಿ (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} ಅನ್ಯಥಾ ಯದಿ (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		ಯದಿ (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, ರಿಕ್ತ);
	} ಅನ್ಯಥಾ ಯದಿ (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, ಪರಿಮಾಣ(bases));
	base = get_base_commit(&cfg, list.ವಸ್ತುಗಳು, list.nr);
	ಯದಿ (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.ವಸ್ತುಗಳು, list.nr);
	}

	ಯದಿ (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(ಪರಿಮಾಣ(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	ಯದಿ (in_reply_to) {
		ವರ್ಣ *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	ಯದಿ (cover_letter) {
		ಯದಿ (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.ವಸ್ತುಗಳು,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_ಸಂಖ್ಯೆ--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ರಿಕ್ತ;
		rev.rdiff1 = ರಿಕ್ತ;
	}
	rev.add_signoff = cfg.do_signoff;

	ಯದಿ (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	ಯಾವತ್ (list.nr) {
		size_t idx = list.nr - 1;
		ಪೂರ್ಣಾಙ್ಕ shown;

		display_progress(progress, total - idx);
		commit = commit_ರಾಶಿ_ತೆಗೆ(&list);
		rev.nr = total - idx + (start_ಸಂಖ್ಯೆ - 1);

		/* Make the second and subsequent mails replies to the first */
		ಯದಿ (cfg.thread) {
			/* Have we already had a message ID? */
			ಯದಿ (rev.message_id) {
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
				ಯದಿ (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					ಮುಕ್ತ(rev.message_id);
				ಅನ್ಯಥಾ
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		ಯದಿ (output_directory &&
		    open_next_file(rev.numbered_files ? ರಿಕ್ತ : commit, ರಿಕ್ತ, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_ವೃಕ್ಷ_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		ಯದಿ (output_directory)
			rev.shown_one = 0;
		ಯದಿ (shown) {
			print_bases(&bases, rev.diffopt.file);
			ಯದಿ (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			ಅನ್ಯಥಾ
				print_signature(signature, rev.diffopt.file);
		}
		ಯದಿ (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ರಿಕ್ತ;
		}
	}
	stop_progress(&progress);
	commit_ರಾಶಿ_clear(&list);
	ಯದಿ (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	ಮುಕ್ತ(description_file);
	ಮುಕ್ತ(signature_file_arg);
	ಮುಕ್ತ(signature_to_free);
	ಮುಕ್ತ(branch_name);
	ಮುಕ್ತ(to_free);
	ಮುಕ್ತ(rev.message_id);
	ಯದಿ (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	ಮುಕ್ತ(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_pending_commit(ಧ್ರುವ ವರ್ಣ *arg, ಸಂರಚನಾ rev_info *revs, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ object_id oid;
	ಯದಿ (repo_get_oid(the_repository, arg, &oid) == 0) {
		ಸಂರಚನಾ commit *commit = lookup_commit_reference(the_repository,
								&oid);
		ಯದಿ (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಶೂನ್ಯ print_commit(ವರ್ಣ sign, ಸಂರಚನಾ commit *commit, ಪೂರ್ಣಾಙ್ಕ verbose,
			 ಪೂರ್ಣಾಙ್ಕ abbrev, FILE *file)
{
	ಯದಿ (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
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
ಸ್ಥಿರ ಶೂನ್ಯ collect_diff_blob_oids(ಸಂರಚನಾ commit *commit,
				   ಸಂರಚನಾ diff_options *opts,
				   ಸಂರಚನಾ oidset *blobs)
{
	ಸಂರಚನಾ diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	ಪ್ರತಿಜ್ಞಾ(!commit->parents || !commit->parents->next);

	ಯದಿ (commit->parents)
		diff_ವೃಕ್ಷ_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	ಅನ್ಯಥಾ
		diff_ಬೇರು_ವೃಕ್ಷ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < q->nr; i++) {
		ಸಂರಚನಾ diff_filepair *p = q->queue[i];
		ಸಂರಚನಾ userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		ಯದಿ (drv && drv->ದ್ವಿಮಾನ > 0)
			ಅನುವರ್ತ;

		ಯದಿ (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ರಿಕ್ತ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ಸೇರಿಸು(blobs, &p->one->oid);
		ಯದಿ (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ರಿಕ್ತ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ಸೇರಿಸು(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ always_match(ಧ್ರುವ ಶೂನ್ಯ *cmp_data UNUSED,
			ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry1 UNUSED,
			ಧ್ರುವ ಸಂರಚನಾ hashmap_entry *entry2 UNUSED,
			ಧ್ರುವ ಶೂನ್ಯ *keydata UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಶೂನ್ಯ prefetch_cherry_blobs(ಸಂರಚನಾ repository *repo,
				  ಸಂರಚನಾ commit_list *list,
				  ಸಂರಚನಾ patch_ids *ids)
{
	ಸಂರಚನಾ oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	ಯದಿ (!repo_has_promisor_remote(repo))
		ಪ್ರತ್ಯಾಗಮ;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	ಪ್ರತಿ (ಸಂರಚನಾ commit_list *l = list; l; l = l->next) {
		ಸಂರಚನಾ commit *c = l->item;
		bool match_found = false;
		ಪ್ರತಿ (ಸಂರಚನಾ patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		ಯದಿ (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	ಯದಿ (oidset_size(&blobs)) {
		ಸಂರಚನಾ oid_array to_fetch = OID_ARRAY_INIT;
		ಸಂರಚನಾ oidset_iter iter;
		ಧ್ರುವ ಸಂರಚನಾ object_id *oid;

		oidset_iter_init(&blobs, &iter);
		ಯಾವತ್ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

ಪೂರ್ಣಾಙ್ಕ cmd_cherry(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ patch_ids ids;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ commit_list *list = ರಿಕ್ತ;
	ಸಂರಚನಾ branch *current_branch;
	ಧ್ರುವ ವರ್ಣ *upstream;
	ಧ್ರುವ ವರ್ಣ *head = "HEAD";
	ಧ್ರುವ ವರ್ಣ *limit = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ verbose = 0, abbrev = 0;

	ಸಂರಚನಾ option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	ವಿಕಲ್ಪ (argc) {
	ಪಕ್ಷ 3:
		limit = argv[2];
		/* FALLTHROUGH */
	ಪಕ್ಷ 2:
		head = argv[1];
		/* FALLTHROUGH */
	ಪಕ್ಷ 1:
		upstream = argv[0];
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		current_branch = branch_get(ರಿಕ್ತ);
		upstream = branch_get_upstream(current_branch, ರಿಕ್ತ);
		ಯದಿ (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	ಯದಿ (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	ಯದಿ (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	ಯದಿ (revs.pending.nr == 2) {
		ಸಂರಚನಾ object_array_entry *o = revs.pending.objects;
		ಯದಿ (oideq(&o[0].item->oid, &o[1].item->oid))
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	get_patch_ids(&revs, &ids);

	ಯದಿ (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	ಯದಿ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ಯಾವತ್ ((commit = get_revision(&revs)) != ರಿಕ್ತ) {
		commit_list_ಸೇರಿಸು(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	ಪ್ರತಿ (ಸಂರಚನಾ commit_list *l = list; l; l = l->next) {
		ವರ್ಣ sign = '+';

		commit = l->item;
		ಯದಿ (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	ಪ್ರತ್ಯಾಗಮ 0;
}
