/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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

#ವ್ಯಾಖ್ಯಾನಿಸು MAIL_DEFAULT_WRAP 72
#ವ್ಯಾಖ್ಯಾನಿಸು COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#ವ್ಯಾಖ್ಯಾನಿಸು FORMAT_PATCH_NAME_MAX_DEFAULT 64

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ force_ಒಳ_body_from;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdout_mboxrd;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ format_no_ಪೂರ್ವಪ್ರತ್ಯಯ;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ಖಾಲಿ
};

ರಚನೆ ಸಾಲು_opt_callback_ದತ್ತಾಂಶ {
	ರಚನೆ rev_info *rev;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ರಚನೆ string_list ವಾದಗಳು;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ session_is_interactive(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ isatty(1) || pager_ಒಳ_use();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ auto_decoration_style(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_decoration_style(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆಯ್ಕೆ (git_parse_maybe_bool(ಮೌಲ್ಯ)) {
	ಸ್ಥಿತಿ 1:
		ಹಿಂತಿರುಗಿ DECORATE_SHORT_REFS;
	ಸ್ಥಿತಿ 0:
		ಹಿಂತಿರುಗಿ 0;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "full"))
		ಹಿಂತಿರುಗಿ DECORATE_ತುಂಬಿದ_REFS;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "short"))
		ಹಿಂತಿರುಗಿ DECORATE_SHORT_REFS;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "auto"))
		ಹಿಂತಿರುಗಿ auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	ಹಿಂತಿರುಗಿ -1;
}

ರಚನೆ log_config {
	ಪೂರ್ಣಾಂಕ default_abbrev_commit;
	ಪೂರ್ಣಾಂಕ default_show_ಬೇರು;
	ಪೂರ್ಣಾಂಕ default_follow;
	ಪೂರ್ಣಾಂಕ default_show_signature;
	ಪೂರ್ಣಾಂಕ default_encode_email_ಶೀರ್ಷಿಕೆಗಳು;
	ಪೂರ್ಣಾಂಕ decoration_style;
	ಪೂರ್ಣಾಂಕ decoration_given;
	ಪೂರ್ಣಾಂಕ use_mailmap_config;
	ಅಕ್ಷರ *fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಪೂರ್ಣಾಂಕ fmt_patch_name_max;
	ಅಕ್ಷರ *fmt_pretty;
	ಅಕ್ಷರ *default_date_ಬಹುಲಕ;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	ಪೂರ್ಣಾಂಕ i_still_use_this;
#ಆದರೆ_ಅಂತ್ಯ
};

ಸ್ಥಿರ ಶೂನ್ಯ log_config_init(ರಚನೆ log_config *cfg)
{
	memset(cfg, 0, ಗಾತ್ರ(*cfg));
	cfg->default_show_ಬೇರು = 1;
	cfg->default_encode_email_ಶೀರ್ಷಿಕೆಗಳು = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ಸ್ಥಿರ ಶೂನ್ಯ log_config_release(ರಚನೆ log_config *cfg)
{
	ಬಿಡುಗಡೆ(cfg->default_date_ಬಹುಲಕ);
	ಬಿಡುಗಡೆ(cfg->fmt_pretty);
	ಬಿಡುಗಡೆ(cfg->fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ use_default_decoration_filter = 1;
ಸ್ಥಿರ ರಚನೆ string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ರಚನೆ string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ರಚನೆ string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clear_decorations_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ decorate_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			     ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ log_config *cfg = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		cfg->decoration_style = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	ಇಲ್ಲದಿದ್ದರೆ
		cfg->decoration_style = DECORATE_SHORT_REFS;

	ಆದರೆ (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ log_ಸಾಲು_range_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ ಸಾಲು_opt_callback_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!arg)
		ಹಿಂತಿರುಗಿ -1;

	ದತ್ತಾಂಶ->rev->ಸಾಲು_level_traverse = 1;
	string_list_append(&ದತ್ತಾಂಶ->ವಾದಗಳು, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ cmd_log_init_defaults(ರಚನೆ rev_info *rev,
				  ರಚನೆ log_config *cfg)
{
	ಆದರೆ (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	ಆದರೆ (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_ಶೀರ್ಷಿಕೆ = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_ಬೇರು_diff = cfg->default_show_ಬೇರು;
	rev->subject_ಪೂರ್ವಪ್ರತ್ಯಯ = cfg->fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_ಶೀರ್ಷಿಕೆಗಳು = cfg->default_encode_email_ಶೀರ್ಷಿಕೆಗಳು;

	ಆದರೆ (cfg->default_date_ಬಹುಲಕ)
		parse_date_format(cfg->default_date_ಬಹುಲಕ, &rev->date_ಬಹುಲಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_default_decoration_filter(ರಚನೆ decoration_filter *decoration_filter)
{
	ಅಕ್ಷರ *ಮೌಲ್ಯ = ಖಾಲಿ;
	ರಚನೆ string_list *include = decoration_filter->include_ref_ನಮೂನೆ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *config_exclude;

	ಆದರೆ (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		for_each_string_list_ಅಂಶ(ಅಂಶ, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_ನಮೂನೆ,
					   ಅಂಶ->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	ಆದರೆ (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &ಮೌಲ್ಯ) &&
	    !strcmp("all", ಮೌಲ್ಯ))
		use_default_decoration_filter = 0;
	ಬಿಡುಗಡೆ(ಮೌಲ್ಯ);

	ಆದರೆ (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_ನಮೂನೆ->nr ||
	    decoration_filter->include_ref_ನಮೂನೆ->nr ||
	    decoration_filter->exclude_ref_config_ನಮೂನೆ->nr)
		ಹಿಂತಿರುಗಿ;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(ref_namespace); i++) {
		ಆದರೆ (!ref_namespace[i].decoration)
			ಮುಂದುವರಿ;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ cmd_log_init_ಮುಕ್ತಾಯ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ rev_info *rev, ರಚನೆ ಸಿದ್ಧತೆ_revision_opt *opt,
			 ರಚನೆ log_config *cfg)
{
	ರಚನೆ userformat_want w;
	ಪೂರ್ಣಾಂಕ quiet = 0, ಮೂಲ = 0, mailmap;
	ಸ್ಥಿರ ರಚನೆ ಸಾಲು_opt_callback_ದತ್ತಾಂಶ ಸಾಲು_cb = {ಖಾಲಿ, ಖಾಲಿ, STRING_LIST_INIT_DUP};
	ರಚನೆ decoration_filter decoration_filter = {
		.exclude_ref_ನಮೂನೆ = &decorate_refs_exclude,
		.include_ref_ನಮೂನೆ = &decorate_refs_include,
		.exclude_ref_config_ನಮೂನೆ = &decorate_refs_exclude_config,
	};
	ಸ್ಥಿರ ರಚನೆ revision_sources revision_sources;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &ಮೂಲ, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#ಆದರೆ_ಅಂತ್ಯ
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ಖಾಲಿ, ಖಾಲಿ,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ಖಾಲಿ, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ಖಾಲಿ, &ಸಾಲು_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_ಸಾಲು_range_callback),
		OPT_ಕೊನೆ()
	};

	ಸಾಲು_cb.rev = rev;
	ಸಾಲು_cb.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ಆದರೆ (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	ಆದರೆ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ಆದರೆ (rev->ಸಾಲು_level_traverse && rev->prune_ದತ್ತಾಂಶ.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, ಗಾತ್ರ(w));
	userformat_ಕಂಡುಹಿಡಿ_requirements(ಖಾಲಿ, &w);

	ಆದರೆ (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	ಆದರೆ (rev->show_notes)
		ತುಂಬು_display_notes(&rev->notes_opt);

	ಆದರೆ ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_ಮುಖವಾಡ) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_ಶೀರ್ಷಿಕೆ = 0;

	ಆದರೆ (ಮೂಲ || w.ಮೂಲ) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	ಆದರೆ (mailmap) {
		rev->mailmap = xmalloc(ಗಾತ್ರ(ರಚನೆ string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	ಆದರೆ (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		ಆದರೆ (!cfg->decoration_given)
			cfg->decoration_style = 0;
		ಆದರೆ (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	ಆದರೆ (rev->commit_format == CMIT_FMT_USERFORMAT) {
		ಆದರೆ (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	ಆದರೆ (cfg->decoration_style || rev->simplify_by_decoration) {
		ಗಣ_default_decoration_filter(&decoration_filter);

		ಆದರೆ (cfg->decoration_style)
			rev->show_decorations = 1;

		ತುಂಬು_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	ಆದರೆ (rev->ಸಾಲು_level_traverse)
		ಸಾಲು_log_init(rev, ಸಾಲು_cb.ಪೂರ್ವಪ್ರತ್ಯಯ, &ಸಾಲು_cb.ವಾದಗಳು);

	ಸಿದ್ಧತೆ_pager(the_repository);
}

ಸ್ಥಿರ ಶೂನ್ಯ cmd_log_init(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ rev_info *rev, ರಚನೆ ಸಿದ್ಧತೆ_revision_opt *opt,
			 ರಚನೆ log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_ಮುಕ್ತಾಯ(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, rev, opt, cfg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_log_walk_no_free(ರಚನೆ rev_info *rev)
{
	ರಚನೆ commit *commit;
	ಪೂರ್ಣಾಂಕ saved_nrl = 0;
	ಪೂರ್ಣಾಂಕ saved_dcctc = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	ಆದರೆ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	ಆಗ ((commit = get_revision(rev)) != ಖಾಲಿ) {
		ಆದರೆ (!log_ವೃಕ್ಷ_commit(rev, commit) && rev->max_ಎಣಿಕೆ >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_ಎಣಿಕೆ++;
		ಆದರೆ (!rev->reflog_info && !rev->remerge_diff) {
			/*
			 * We may show a given commit multiple times when
			 * walking the reflogs. Therefore we still need it.
			 *
			 * Likewise, we potentially still need the parents
			 * of * already shown commits to determine merge
			 * bases when showing remerge diffs.
			 */
			free_commit_ಬಫರ್(the_repository->parsed_objects,
					   commit);
			commit_list_free(commit->parents);
			commit->parents = ಖಾಲಿ;
		}
		ಆದರೆ (saved_nrl < rev->diffopt.needed_rename_ಎಲ್ಲೆ)
			saved_nrl = rev->diffopt.needed_rename_ಎಲ್ಲೆ;
		ಆದರೆ (rev->diffopt.degraded_cc_ಗಮ್ಯ_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_ಗಮ್ಯ_c = saved_dcctc;
	rev->diffopt.needed_rename_ಎಲ್ಲೆ = saved_nrl;

	ಫಲಿತಾಂಶ = diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(rev);
	ಆದರೆ (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		ಫಲಿತಾಂಶ = 02;
	}
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_log_walk(ರಚನೆ rev_info *rev)
{
	ಪೂರ್ಣಾಂಕ retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_log_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ log_config *cfg = cb;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ_name;

	ಆದರೆ (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->fmt_pretty, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, ಮೌಲ್ಯ, ctx->kvi);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_ಶೀರ್ಷಿಕೆಗಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_ಬಹುಲಕ);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->default_date_ಬಹುಲಕ, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(ಮೌಲ್ಯ);
		ಆದರೆ (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "log.diffmerges")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಹಿಂತಿರುಗಿ diff_merges_config(ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "log.showroot")) {
		cfg->default_show_ಬೇರು = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(var, "color.decorate.", &ಸ್ಥಾನ_name))
		ಹಿಂತಿರುಗಿ parse_decorate_color_config(var, ಸ್ಥಾನ_name, ಮೌಲ್ಯ);
	ಆದರೆ (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_diff_ui_config(var, ಮೌಲ್ಯ, ctx, cb);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
ಪೂರ್ಣಾಂಕ cmd_whatchanged(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ log_config cfg;
	ರಚನೆ rev_info rev;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt opt;
	ಪೂರ್ಣಾಂಕ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, ಗಾತ್ರ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &rev, &opt, &cfg);

	ಆದರೆ (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	ಆದರೆ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ show_tagger(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf, ರಚನೆ rev_info *rev)
{
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	ರಚನೆ pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_ಬಹುಲಕ = rev->date_ಬಹುಲಕ;
	pp_user_info(&pp, "Tagger", &ಹೊರಹೋಗು, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", ಹೊರಹೋಗು.buf);
	strbuf_release(&ಹೊರಹೋಗು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_blob_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ rev_info *rev, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *obj_name)
{
	ರಚನೆ object_id oidc;
	ರಚನೆ object_context obj_context = {0};
	ಅಕ್ಷರ *buf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;

	fflush(rev->diffopt.file);
	ಆದರೆ (!rev->diffopt.flags.textconv_ಗಣ_ಮೂಲಕ_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		ಹಿಂತಿರುಗಿ odb_ಹರಿವು_blob_ಗಮ್ಯ_fd(the_repository->objects, 1, oid, ಖಾಲಿ, 0);

	ಆದರೆ (get_oid_with_context(the_repository, obj_name,
				 GET_OID_ದಾಖಲೆ_ಮಾರ್ಗ,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	ಆದರೆ (!obj_context.ಮಾರ್ಗ ||
	    !textconv_object(the_repository, obj_context.ಮಾರ್ಗ,
			     obj_context.ಬಹುಲಕ, &oidc, 1, &buf, &ಗಾತ್ರ_2)) {
		object_context_release(&obj_context);
		ಹಿಂತಿರುಗಿ odb_ಹರಿವು_blob_ಗಮ್ಯ_fd(the_repository->objects, 1, oid, ಖಾಲಿ, 0);
	}

	ಆದರೆ (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, ಗಾತ್ರ_2);
	object_context_release(&obj_context);
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಟ್ಯಾಗ್_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ rev_info *rev)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಎಣಿಕೆ object_type type;
	ಅಕ್ಷರ *buf = odb_read_object(the_repository->objects, oid, &type, &ಗಾತ್ರ_2);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ = 0;

	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ error(_("could not read object %s"), oid_ಗಮ್ಯ_hex(oid));

	ಪ್ರತಿಜ್ಞಾ(type == OBJ_ಟ್ಯಾಗ್);
	ಆಗ (ಸ್ಥಾನಾಂತರ < ಗಾತ್ರ_2 && buf[ಸ್ಥಾನಾಂತರ] != '\n') {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ new_ಸ್ಥಾನಾಂತರ = ಸ್ಥಾನಾಂತರ + 1;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ident;
		ಆಗ (new_ಸ್ಥಾನಾಂತರ < ಗಾತ್ರ_2 && buf[new_ಸ್ಥಾನಾಂತರ++] != '\n')
			; /* do nothing */
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(buf + ಸ್ಥಾನಾಂತರ, "tagger ", &ident))
			show_tagger(ident, rev);
		ಸ್ಥಾನಾಂತರ = new_ಸ್ಥಾನಾಂತರ;
	}

	ಆದರೆ (ಸ್ಥಾನಾಂತರ < ಗಾತ್ರ_2)
		fwrite(buf + ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ_2 - ಸ್ಥಾನಾಂತರ, 1, rev->diffopt.file);
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ವೃಕ್ಷ_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid ಬಳಸದ,
			    ರಚನೆ strbuf *ತಳ ಬಳಸದ,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ,
			    ಶೂನ್ಯ *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(ಬಹುಲಕ) ? "/" : "");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ಸಿದ್ಧತೆ_revisions_tweak(ರಚನೆ rev_info *rev)
{
	ಆದರೆ (rev->ಮೊದಲ_ಪೋಷಕ_only)
		diff_merges_default_ಗಮ್ಯ_ಮೊದಲ_ಪೋಷಕ(rev);
	ಇಲ್ಲದಿದ್ದರೆ
		diff_merges_default_ಗಮ್ಯ_dense_combined(rev);
	ಆದರೆ (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

ಪೂರ್ಣಾಂಕ cmd_show(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ log_config cfg;
	ರಚನೆ rev_info rev;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt opt;
	ರಚನೆ pathspec ಹೊಂದಿಕೆ_all;
	ಪೂರ್ಣಾಂಕ ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	ಆದರೆ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
	}

	memset(&ಹೊಂದಿಕೆ_all, 0, ಗಾತ್ರ(ಹೊಂದಿಕೆ_all));
	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_ಶೀರ್ಷಿಕೆ = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_ಅಗಲ = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, ಗಾತ್ರ(opt));
	opt.def = "HEAD";
	opt.tweak = show_ಸಿದ್ಧತೆ_revisions_tweak;
	cmd_log_init(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &rev, &opt, &cfg);

	ಆದರೆ (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		ಹಿಂತಿರುಗಿ ret;
	}

	rev.diffopt.no_free = 1;
	ಗಾಗಿ (i = 0; i < rev.pending.nr && !ret; i++) {
		ರಚನೆ object *o = rev.pending.objects[i].ಅಂಶ;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = rev.pending.objects[i].name;
		ಆಯ್ಕೆ (o->type) {
		ಸ್ಥಿತಿ OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್: {
			ರಚನೆ ಟ್ಯಾಗ್ *t = (ರಚನೆ ಟ್ಯಾಗ್ *)o;
			ರಚನೆ object_id *oid = get_tagged_oid(t);

			ಆದರೆ (rev.shown_one)
				ಅಕ್ಷರ_ಬರೆ('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->ಟ್ಯಾಗ್,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_ಟ್ಯಾಗ್_object(&o->oid, &rev);
			rev.shown_one = 1;
			ಆದರೆ (ret)
				ನಿಲ್ಲಿಸು;
			o = parse_object(the_repository, oid);
			ಆದರೆ (!o)
				ret = error(_("could not read object %s"),
					    oid_ಗಮ್ಯ_hex(oid));
			rev.pending.objects[i].ಅಂಶ = o;
			i--;
			ನಿಲ್ಲಿಸು;
		}
		ಸ್ಥಿತಿ OBJ_ವೃಕ್ಷ:
			ಆದರೆ (rev.shown_one)
				ಅಕ್ಷರ_ಬರೆ('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_ವೃಕ್ಷ(the_repository, (ರಚನೆ ವೃಕ್ಷ *)o,
				  &ಹೊಂದಿಕೆ_all, show_ವೃಕ್ಷ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_COMMIT:
		{
			ರಚನೆ object_array old;
			ರಚನೆ object_array blank = OBJECT_ARRAY_INIT;

			ಸ್ಮರಣೆ_ನಕಲು(&old, &rev.pending, ಗಾತ್ರ(old));
			ಸ್ಮರಣೆ_ನಕಲು(&rev.pending, &blank, ಗಾತ್ರ(rev.pending));

			ಕೂಡಿಸು_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			ಸ್ಮರಣೆ_ನಕಲು(&rev.pending, &old, ಗಾತ್ರ(rev.pending));
			ನಿಲ್ಲಿಸು;
		}
		ಪೂರ್ವನಿಯೋಜಿತ:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	ಹಿಂತಿರುಗಿ ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
ಪೂರ್ಣಾಂಕ cmd_log_reflog(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ log_config cfg;
	ರಚನೆ rev_info rev;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt opt;
	ಪೂರ್ಣಾಂಕ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_ಶೀರ್ಷಿಕೆ = 1;
	memset(&opt, 0, ಗಾತ್ರ(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_ಶೀರ್ಷಿಕೆ = 1;
	cmd_log_init_ಮುಕ್ತಾಯ(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ log_ಸಿದ್ಧತೆ_revisions_tweak(ರಚನೆ rev_info *rev)
{
	ಆದರೆ (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_ದತ್ತಾಂಶ, 0))
		rev->diffopt.flags.follow_renames = 1;

	ಆದರೆ (rev->ಮೊದಲ_ಪೋಷಕ_only)
		diff_merges_default_ಗಮ್ಯ_ಮೊದಲ_ಪೋಷಕ(rev);
}

ಪೂರ್ಣಾಂಕ cmd_log(ಪೂರ್ಣಾಂಕ argc,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ log_config cfg;
	ರಚನೆ rev_info rev;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt opt;
	ಪೂರ್ಣಾಂಕ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_ಶೀರ್ಷಿಕೆ = 1;
	memset(&opt, 0, ಗಾತ್ರ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_ಸಿದ್ಧತೆ_revisions_tweak;
	cmd_log_init(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ಹಿಂತಿರುಗಿ ret;
}

/* format-patch */

ಎಣಿಕೆ cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

ಎಣಿಕೆ thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

ಎಣಿಕೆ cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_ಸಂದೇಶ,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

ಎಣಿಕೆ auto_ತಳ_setting {
	AUTO_ತಳ_NEVER,
	AUTO_ತಳ_ALWAYS,
	AUTO_ತಳ_WHEN_ABLE
};

ರಚನೆ format_config {
	ರಚನೆ log_config ಲಘುಗಣಕ;
	ಎಣಿಕೆ thread_level thread;
	ಪೂರ್ಣಾಂಕ do_signoff;
	ಎಣಿಕೆ auto_ತಳ_setting auto_ತಳ;
	ಅಕ್ಷರ *ತಳ_commit;
	ಅಕ್ಷರ *from;
	ಅಕ್ಷರ *signature;
	ಅಕ್ಷರ *signature_file;
	ಎಣಿಕೆ cover_setting config_cover_letter;
	ಅಕ್ಷರ *fmt_cover_letter_commit_list;
	ಅಕ್ಷರ *config_output_directory;
	ಎಣಿಕೆ cover_from_description cover_from_description_ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ show_notes;
	ರಚನೆ display_notes_opt notes_opt;
	ಪೂರ್ಣಾಂಕ numbered_cmdline_opt;
	ಪೂರ್ಣಾಂಕ numbered;
	ಪೂರ್ಣಾಂಕ auto_ಸಂಖ್ಯೆ;
	ಅಕ್ಷರ *default_attach;
	ರಚನೆ string_list extra_hdr;
	ರಚನೆ string_list extra_ಗಮ್ಯ;
	ರಚನೆ string_list extra_cc;
	ಪೂರ್ಣಾಂಕ keep_subject;
	ಪೂರ್ಣಾಂಕ subject_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ರಚನೆ strbuf sprefix;
	ಅಕ್ಷರ *fmt_patch_suffix;
};

ಸ್ಥಿರ ಶೂನ್ಯ format_config_init(ರಚನೆ format_config *cfg)
{
	memset(cfg, 0, ಗಾತ್ರ(*cfg));
	log_config_init(&cfg->ಲಘುಗಣಕ);
	cfg->cover_from_description_ಬಹುಲಕ = COVER_FROM_ಸಂದೇಶ;
	cfg->auto_ಸಂಖ್ಯೆ = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_ಗಮ್ಯ);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ಸ್ಥಿರ ಶೂನ್ಯ format_config_release(ರಚನೆ format_config *cfg)
{
	log_config_release(&cfg->ಲಘುಗಣಕ);
	ಬಿಡುಗಡೆ(cfg->ತಳ_commit);
	ಬಿಡುಗಡೆ(cfg->from);
	ಬಿಡುಗಡೆ(cfg->signature);
	ಬಿಡುಗಡೆ(cfg->signature_file);
	ಬಿಡುಗಡೆ(cfg->config_output_directory);
	ಬಿಡುಗಡೆ(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_ಗಮ್ಯ, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	ಬಿಡುಗಡೆ(cfg->fmt_patch_suffix);
	ಬಿಡುಗಡೆ(cfg->fmt_cover_letter_commit_list);
}

ಸ್ಥಿರ ಎಣಿಕೆ cover_from_description parse_cover_from_description(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ (!arg || !strcmp(arg, "default"))
		ಹಿಂತಿರುಗಿ COVER_FROM_ಸಂದೇಶ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "none"))
		ಹಿಂತಿರುಗಿ COVER_FROM_NONE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "message"))
		ಹಿಂತಿರುಗಿ COVER_FROM_ಸಂದೇಶ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "subject"))
		ಹಿಂತಿರುಗಿ COVER_FROM_SUBJECT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "auto"))
		ಹಿಂತಿರುಗಿ COVER_FROM_AUTO;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("%s: invalid cover from description mode"), arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ(ರಚನೆ format_config *cfg, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮೌಲ್ಯ);
	ಆಗ (len && ಮೌಲ್ಯ[len - 1] == '\n')
		len--;

	ಆದರೆ (!strncasecmp(ಮೌಲ್ಯ, "to: ", 4)) {
		ಅಂಶ = string_list_append(&cfg->extra_ಗಮ್ಯ, ಮೌಲ್ಯ + 4);
		len -= 4;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strncasecmp(ಮೌಲ್ಯ, "cc: ", 4)) {
		ಅಂಶ = string_list_append(&cfg->extra_cc, ಮೌಲ್ಯ + 4);
		len -= 4;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಅಂಶ = string_list_append(&cfg->extra_hdr, ಮೌಲ್ಯ);
	}

	ಅಂಶ->string[len] = '\0';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_format_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ರಚನೆ format_config *cfg = cb;

	ಆದರೆ (!strcmp(var, "format.headers")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			die(_("format.headers without value"));
		ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ(cfg, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->fmt_patch_suffix, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.to")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		string_list_append(&cfg->extra_ಗಮ್ಯ, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.cc")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.numbered")) {
		ಆದರೆ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "auto")) {
			cfg->auto_ಸಂಖ್ಯೆ = 1;
			ಹಿಂತಿರುಗಿ 0;
		}
		cfg->numbered = git_config_bool(var, ಮೌಲ್ಯ);
		cfg->auto_ಸಂಖ್ಯೆ = cfg->auto_ಸಂಖ್ಯೆ && cfg->numbered;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.attach")) {
		ಆದರೆ (ಮೌಲ್ಯ && *ಮೌಲ್ಯ) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(ಮೌಲ್ಯ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮೌಲ್ಯ && !*ಮೌಲ್ಯ) {
			FREE_AND_NULL(cfg->default_attach);
		} ಇಲ್ಲದಿದ್ದರೆ {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_ಆವೃತ್ತಿ_string);
		}
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.thread")) {
		ಆದರೆ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "deep")) {
			cfg->thread = THREAD_DEEP;
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆದರೆ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			ಹಿಂತಿರುಗಿ 0;
		}
		cfg->thread = git_config_bool(var, ಮೌಲ್ಯ) ? THREAD_SHALLOW : THREAD_UNSET;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->signature, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		ಹಿಂತಿರುಗಿ git_config_pathname(&cfg->signature_file, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.coverletter")) {
		ಆದರೆ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			ಹಿಂತಿರುಗಿ 0;
		}
		cfg->config_cover_letter = git_config_bool(var, ಮೌಲ್ಯ) ? COVER_ON : COVER_OFF;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->fmt_cover_letter_commit_list, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		ಹಿಂತಿರುಗಿ git_config_string(&cfg->config_output_directory, var, ಮೌಲ್ಯ);
	}
	ಆದರೆ (!strcmp(var, "format.useautobase")) {
		ಆದರೆ (ಮೌಲ್ಯ && !strcasecmp(ಮೌಲ್ಯ, "whenAble")) {
			cfg->auto_ತಳ = AUTO_ತಳ_WHEN_ABLE;
			ಹಿಂತಿರುಗಿ 0;
		}
		cfg->auto_ತಳ = git_config_bool(var, ಮೌಲ್ಯ) ? AUTO_ತಳ_ALWAYS : AUTO_ತಳ_NEVER;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.from")) {
		ಪೂರ್ಣಾಂಕ b = git_parse_maybe_bool(ಮೌಲ್ಯ);
		FREE_AND_NULL(cfg->from);
		ಆದರೆ (b < 0)
			cfg->from = xstrdup(ಮೌಲ್ಯ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.forceinbodyfrom")) {
		force_ಒಳ_body_from = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.notes")) {
		ಪೂರ್ಣಾಂಕ b = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಆದರೆ (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, ಮೌಲ್ಯ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ಇಲ್ಲದಿದ್ದರೆ
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_ಬಹುಲಕ = parse_cover_from_description(ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "format.noprefix")) {
		format_no_ಪೂರ್ವಪ್ರತ್ಯಯ = git_parse_maybe_bool(ಮೌಲ್ಯ);
		ಆದರೆ (format_no_ಪೂರ್ವಪ್ರತ್ಯಯ < 0) {
			ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = die_ಸಂದೇಶ(
				_("bad boolean config value '%s' for '%s'"),
				ಮೌಲ್ಯ, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			ನಿರ್ಗಮಿಸು(ಸ್ಥಿತಿ_2);
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	ಆದರೆ (!strcmp(var, "diff.noprefix"))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ git_log_config(var, ಮೌಲ್ಯ, ctx, &cfg->ಲಘುಗಣಕ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *output_directory = ಖಾಲಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ outdir_ಸ್ಥಾನಾಂತರ;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ open_ಮುಂದಿನ_file(ರಚನೆ commit *commit, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subject,
			 ರಚನೆ rev_info *rev, ಪೂರ್ಣಾಂಕ quiet)
{
	ರಚನೆ strbuf filename = STRBUF_INIT;

	ಆದರೆ (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	ಆದರೆ (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (commit)
		fmt_output_commit(&filename, commit, rev);
	ಇಲ್ಲದಿದ್ದರೆ
		fmt_output_subject(&filename, subject, rev);

	ಆದರೆ (!quiet)
		ಬರೆ("%s\n", filename.buf + outdir_ಸ್ಥಾನಾಂತರ);

	ಆದರೆ (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		ಹಿಂತಿರುಗಿ -1;
	}

	strbuf_release(&filename);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_patch_ids(ರಚನೆ rev_info *rev, ರಚನೆ patch_ids *ids)
{
	ರಚನೆ rev_info check_rev;
	ರಚನೆ commit *commit, *c1, *c2;
	ರಚನೆ object *o1, *o2;
	ಚಿಹ್ನೆರಹಿತ flags1, flags2;

	ಆದರೆ (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].ಅಂಶ;
	o2 = rev->pending.objects[1].ಅಂಶ;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	ಆದರೆ ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->ಪೂರ್ವಪ್ರತ್ಯಯ);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	ಕೂಡಿಸು_pending_object(&check_rev, o1, "o1");
	ಕೂಡಿಸು_pending_object(&check_rev, o2, "o2");
	ಆದರೆ (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	ಆಗ ((commit = get_revision(&check_rev)) != ಖಾಲಿ) {
		ಕೂಡಿಸು_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, ನೋಡಿದ_2 | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, ನೋಡಿದ_2 | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ಸ್ಥಿರ ಶೂನ್ಯ gen_ಸಂದೇಶ_id(ರಚನೆ rev_info *info, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", ತಳ,
		    (timestamp_t) ಸಮಯ(ಖಾಲಿ),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->ಸಂದೇಶ_id = strbuf_detach(&buf, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_signature(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signature, FILE *file)
{
	ಆದರೆ (!signature || !*signature)
		ಹಿಂತಿರುಗಿ;

	fprintf(file, "-- \n%s", signature);
	ಆದರೆ (signature[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ಸ್ಥಿರ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_branch_name(ರಚನೆ rev_info *rev)
{
	ರಚನೆ object_id branch_oid;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *tip_oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref, *v;
	ಅಕ್ಷರ *ತುಂಬಿದ_ref, *branch = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ interesting_ಸಿಕ್ಕಿತು = 0;
	ಗಾತ್ರ_t idx;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < rev->cmdline.nr; i++) {
		ಆದರೆ (rev->cmdline.rev[i].flags & UNINTERESTING)
			ಮುಂದುವರಿ;
		ಆದರೆ (interesting_ಸಿಕ್ಕಿತು)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		interesting_ಸಿಕ್ಕಿತು = 1;
		idx = i;
	}
	ಆದರೆ (!interesting_ಸಿಕ್ಕಿತು)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].ಅಂಶ->oid;
	ಆದರೆ (repo_dwim_ref(the_repository, ref, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ref), &branch_oid,
			  &ತುಂಬಿದ_ref, 0) &&
	    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ತುಂಬಿದ_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	ಬಿಡುಗಡೆ(ತುಂಬಿದ_ref);
	ಹಿಂತಿರುಗಿ branch;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_diffstat(ರಚನೆ rev_info *rev,
			  ರಚನೆ commit *origin, ರಚನೆ commit *ತಲೆ)
{
	ರಚನೆ diff_options opts;

	ಸ್ಮರಣೆ_ನಕಲು(&opts, &rev->diffopt, ಗಾತ್ರ(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&opts);

	diff_ವೃಕ್ಷ_oid(get_commit_ವೃಕ್ಷ_oid(origin),
		      get_commit_ವೃಕ್ಷ_oid(ತಲೆ),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ read_desc_file(ರಚನೆ strbuf *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *desc_file)
{
	ಆದರೆ (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_cover_ಪಠ್ಯ(ರಚನೆ pretty_print_context *pp,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description_file,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name,
			       ರಚನೆ strbuf *sb,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *encoding,
			       ಪೂರ್ಣಾಂಕ need_8bit_cte,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ format_config *cfg)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subject = "*** SUBJECT HERE ***";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *body = "*** BLURB HERE ***";
	ರಚನೆ strbuf description_sb = STRBUF_INIT;
	ರಚನೆ strbuf subject_sb = STRBUF_INIT;

	ಆದರೆ (cfg->cover_from_description_ಬಹುಲಕ == COVER_FROM_NONE)
		ಹೋಗು do_pp;

	ಆದರೆ (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	ಆದರೆ (!description_sb.len)
		ಹೋಗು do_pp;

	ಆದರೆ (cfg->cover_from_description_ಬಹುಲಕ == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_ಬಹುಲಕ == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	ಆದರೆ (cfg->cover_from_description_ಬಹುಲಕ == COVER_FROM_ಸಂದೇಶ ||
	    (cfg->cover_from_description_ಬಹುಲಕ == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	ಇಲ್ಲದಿದ್ದರೆ
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ರಚನೆ rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ರಚನೆ string_list notes;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rdiff_notes_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			  ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ option opt = *option;
	ರಚನೆ rdiff_notes *rdiff_notes = option->ಮೌಲ್ಯ;

	rdiff_notes->override = 1;
	opt.ಮೌಲ್ಯ = &rdiff_notes->notes;
	ಹಿಂತಿರುಗಿ parse_opt_string_list(&opt, arg, unset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_notes_refs(ರಚನೆ string_list_ಅಂಶ *ಅಂಶ, ಶೂನ್ಯ *arg)
{
	strvec_pushf(arg, "--notes=%s", ಅಂಶ->string);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_notes_ವಾದಗಳು(ರಚನೆ rdiff_notes *rdiff_notes,
			   ರಚನೆ rev_info *rev)
{
	ಆದರೆ (rdiff_notes->override) {
		ಆದರೆ (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		ಇಲ್ಲದಿದ್ದರೆ
			strvec_ತಳ್ಳು(&rev->rdiff_log_arg, "--no-notes");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!rev->show_notes) {
		strvec_ತಳ್ಳು(&rev->rdiff_log_arg, "--no-notes");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ತಳ್ಳು(&rev->rdiff_log_arg, "--notes");
	} ಇಲ್ಲದಿದ್ದರೆ {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ generate_shortlog_cover_letter(ರಚನೆ shortlog *ಲಘುಗಣಕ,
					   ರಚನೆ rev_info *rev,
					   ರಚನೆ commit **list,
					   ಪೂರ್ಣಾಂಕ nr)
{
	shortlog_init(ಲಘುಗಣಕ);
	ಲಘುಗಣಕ->wrap_ಸಾಲುಗಳು = 1;
	ಲಘುಗಣಕ->wrap = MAIL_DEFAULT_WRAP;
	ಲಘುಗಣಕ->in1 = 2;
	ಲಘುಗಣಕ->in2 = 4;
	ಲಘುಗಣಕ->file = rev->diffopt.file;
	ಲಘುಗಣಕ->groups = SHORTLOG_ಗುಂಪು_AUTHOR;
	shortlog_ಮುಕ್ತಾಯ_ಸಿದ್ಧತೆ(ಲಘುಗಣಕ);
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nr; i++)
		shortlog_ಕೂಡಿಸು_commit(ಲಘುಗಣಕ, list[i]);

	shortlog_output(ಲಘುಗಣಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ generate_commit_list_cover(FILE *cover_file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format,
				       ರಚನೆ commit **list, ಪೂರ್ಣಾಂಕ n)
{
	ರಚನೆ strbuf commit_ಸಾಲು = STRBUF_INIT;
	ರಚನೆ pretty_print_context ctx = {0};
	ರಚನೆ rev_info rev = REV_INFO_INIT;

	rev.ಒಟ್ಟು = n;
	ctx.rev = &rev;
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_ಸಂದೇಶ(the_repository, list[n - i], format,
				&commit_ಸಾಲು, &ctx);
		fprintf(cover_file, "%s\n", commit_ಸಾಲು.buf);
		strbuf_reset(&commit_ಸಾಲು);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_ಸಾಲು);
}

ಸ್ಥಿರ ಶೂನ್ಯ make_cover_letter(ರಚನೆ rev_info *rev, ಪೂರ್ಣಾಂಕ use_separate_file,
			      ರಚನೆ commit *origin,
			      ಪೂರ್ಣಾಂಕ nr, ರಚನೆ commit **list,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description_file,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name,
			      ಪೂರ್ಣಾಂಕ quiet,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ format_config *cfg,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from;
	ರಚನೆ shortlog ಲಘುಗಣಕ;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ i;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *encoding = "UTF-8";
	ಪೂರ್ಣಾಂಕ need_8bit_cte = 0;
	ರಚನೆ pretty_print_context pp = {0};
	ರಚನೆ commit *ತಲೆ = list[0];
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;

	ಆದರೆ (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	ಆದರೆ (use_separate_file &&
	    open_ಮುಂದಿನ_file(ಖಾಲಿ, rev->numbered_files ? ಖಾಲಿ : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_ಶೀರ್ಷಿಕೆಗಳು(rev, ತಲೆ, &pp.after_subject, &need_8bit_cte, 0);

	ಗಾಗಿ (i = 0; !need_8bit_cte && i < nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = repo_get_commit_ಬಫರ್(the_repository,
							 list[i], ಖಾಲಿ);
		ಆದರೆ (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_ಬಫರ್(the_repository, list[i], buf);
	}

	ಆದರೆ (!branch_name)
		branch_name = ಗಮ್ಯ_free = ಕಂಡುಹಿಡಿ_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_ಬಹುಲಕ.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_ಶೀರ್ಷಿಕೆಗಳು = rev->encode_email_ಶೀರ್ಷಿಕೆಗಳು;
	pp_user_info(&pp, ಖಾಲಿ, &sb, from, encoding);
	prepare_cover_ಪಠ್ಯ(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(pp.after_subject);
	strbuf_release(&sb);

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&ಲಘುಗಣಕ, rev, list, nr);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	ಆದರೆ (origin)
		show_diffstat(rev, origin, ತಲೆ);

	ಆದರೆ (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	ಆದರೆ (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ರಚನೆ diff_options opts;
		ರಚನೆ range_diff_options range_diff_opts = {
			.creation_ಗುಣಕ = rev->creation_ಗುಣಕ,
			.dual_color = 1,
			.max_ಸ್ಮರಣೆ = RANGE_DIFF_MAX_ಸ್ಮರಣೆ_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_ಸಿದ್ಧತೆ(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ *clean_ಸಂದೇಶ_id(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_id_2)
{
	ಅಕ್ಷರ ch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, *z, *m;

	m = ಸಂದೇಶ_id_2;
	ಆಗ ((ch = *m) && (ಖಾಲಿಯೇ(ch) || (ch == '<')))
		m++;
	a = m;
	z = ಖಾಲಿ;
	ಆಗ ((ch = *m)) {
		ಆದರೆ (!ಖಾಲಿಯೇ(ch) && (ch != '>'))
			z = m;
		m++;
	}
	ಆದರೆ (!z)
		die(_("insane in-reply-to: %s"), ಸಂದೇಶ_id_2);
	ಆದರೆ (++z == m)
		ಹಿಂತಿರುಗಿ xstrdup(a);
	ಹಿಂತಿರುಗಿ xmemdupz(a, z - a);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗಣ_outdir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *output_directory)
{
	ಆದರೆ (output_directory && is_absolute_ಮಾರ್ಗ(output_directory))
		ಹಿಂತಿರುಗಿ output_directory;

	ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ || !*ಪೂರ್ವಪ್ರತ್ಯಯ) {
		ಆದರೆ (output_directory)
			ಹಿಂತಿರುಗಿ output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_ಸ್ಥಾನಾಂತರ = 2;
		ಹಿಂತಿರುಗಿ "./";
	}

	outdir_ಸ್ಥಾನಾಂತರ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಆದರೆ (!output_directory)
		ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ;

	ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, output_directory);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ಖಾಲಿ
};

ರಚನೆ keep_callback_ದತ್ತಾಂಶ {
	ರಚನೆ format_config *cfg;
	ರಚನೆ rev_info *revs;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ keep_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ keep_callback_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ದತ್ತಾಂಶ->revs->ಒಟ್ಟು = -1;
	ದತ್ತಾಂಶ->cfg->keep_subject = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ subject_ಪೂರ್ವಪ್ರತ್ಯಯ_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			    ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ format_config *cfg = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_ಪೂರ್ವಪ್ರತ್ಯಯ = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rfc_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			ಪೂರ್ಣಾಂಕ unset)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **rfc = opt->ಮೌಲ್ಯ;

	*rfc = opt->ಮೌಲ್ಯ;
	ಆದರೆ (unset)
		*rfc = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		*rfc = arg ? arg : "RFC";
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ numbered_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			     ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ format_config *cfg = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	ಆದರೆ (unset)
		cfg->auto_ಸಂಖ್ಯೆ =  0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_numbered_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	ಹಿಂತಿರುಗಿ numbered_callback(opt, arg, 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ output_directory_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			      ಪೂರ್ಣಾಂಕ unset)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **dir = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	ಆದರೆ (*dir)
		die(_("two output directories?"));
	*dir = arg;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ thread_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ format_config *cfg = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		cfg->thread = THREAD_UNSET;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ attach_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rev_info *rev = (ರಚನೆ rev_info *)opt->ಮೌಲ್ಯ;
	ಆದರೆ (unset)
		rev->mime_boundary = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg)
		rev->mime_boundary = arg;
	ಇಲ್ಲದಿದ್ದರೆ
		rev->mime_boundary = git_ಆವೃತ್ತಿ_string;
	rev->no_inline = unset ? 0 : 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ inline_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ rev_info *rev = (ರಚನೆ rev_info *)opt->ಮೌಲ್ಯ;
	ಆದರೆ (unset)
		rev->mime_boundary = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg)
		rev->mime_boundary = arg;
	ಇಲ್ಲದಿದ್ದರೆ
		rev->mime_boundary = git_ಆವೃತ್ತಿ_string;
	rev->no_inline = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಶೀರ್ಷಿಕೆ_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			   ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ format_config *cfg = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_ಗಮ್ಯ, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ(cfg, arg);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ from_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಅಕ್ಷರ **from = opt->ಮೌಲ್ಯ;

	ಬಿಡುಗಡೆ(*from);

	ಆದರೆ (unset)
		*from = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg)
		*from = xstrdup(arg);
	ಇಲ್ಲದಿದ್ದರೆ
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ format_config *cfg = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset) {
		cfg->auto_ತಳ = AUTO_ತಳ_NEVER;
		FREE_AND_NULL(cfg->ತಳ_commit);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "auto")) {
		cfg->auto_ತಳ = AUTO_ತಳ_ALWAYS;
		FREE_AND_NULL(cfg->ತಳ_commit);
	} ಇಲ್ಲದಿದ್ದರೆ {
		cfg->auto_ತಳ = AUTO_ತಳ_NEVER;
		cfg->ತಳ_commit = xstrdup(arg);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ತಳ_ವೃಕ್ಷ_info {
	ರಚನೆ object_id ತಳ_commit;
	ಪೂರ್ಣಾಂಕ nr_patch_id, alloc_patch_id;
	ರಚನೆ object_id *patch_id;
};

ಸ್ಥಿರ ರಚನೆ commit *get_ತಳ_commit(ಸ್ಥಿರಾಂಕ ರಚನೆ format_config *cfg,
				      ರಚನೆ commit **list,
				      ಗಾತ್ರ_t ಒಟ್ಟು)
{
	ರಚನೆ commit *ತಳ = ಖಾಲಿ;
	ರಚನೆ commit **rev;
	ಪೂರ್ಣಾಂಕ auto_select, die_on_failure, ret;
	ಗಾತ್ರ_t i = 0, rev_nr = 0;

	ಆಯ್ಕೆ (cfg->auto_ತಳ) {
	ಸ್ಥಿತಿ AUTO_ತಳ_NEVER:
		ಆದರೆ (cfg->ತಳ_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* no base information is requested */
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ AUTO_ತಳ_ALWAYS:
	ಸ್ಥಿತಿ AUTO_ತಳ_WHEN_ABLE:
		ಆದರೆ (cfg->ತಳ_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} ಇಲ್ಲದಿದ್ದರೆ {
			auto_select = 1;
			die_on_failure = cfg->auto_ತಳ == AUTO_ತಳ_ALWAYS;
		}
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected automatic base selection method");
	}

	ಆದರೆ (!auto_select) {
		ತಳ = lookup_commit_reference_by_name(cfg->ತಳ_commit);
		ಆದರೆ (!ತಳ)
			die(_("unknown commit %s"), cfg->ತಳ_commit);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ branch *curr_branch = branch_get(ಖಾಲಿ);
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream = branch_get_upstream(curr_branch, ಖಾಲಿ);
		ಆದರೆ (upstream) {
			ರಚನೆ commit_list *ತಳ_list = ಖಾಲಿ;
			ರಚನೆ commit *commit;
			ರಚನೆ object_id oid;

			ಆದರೆ (repo_get_oid(the_repository, upstream, &oid)) {
				ಆದರೆ (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				ಇಲ್ಲದಿದ್ದರೆ
					ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			ಆದರೆ (repo_get_ವಿಲೀನ_bases_many(the_repository,
						      commit, ಒಟ್ಟು,
						      list,
						      &ತಳ_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !ತಳ_list || ತಳ_list->ಮುಂದಿನ) {
				ಆದರೆ (die_on_failure) {
					die(_("could not find exact merge base"));
				} ಇಲ್ಲದಿದ್ದರೆ {
					commit_list_free(ತಳ_list);
					ಹಿಂತಿರುಗಿ ಖಾಲಿ;
				}
			}
			ತಳ = ತಳ_list->ಅಂಶ;
			commit_list_free(ತಳ_list);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}
	}

	ALLOC_ARRAY(rev, ಒಟ್ಟು);
	ಗಾಗಿ (i = 0; i < ಒಟ್ಟು; i++)
		rev[i] = list[i];

	rev_nr = ಒಟ್ಟು;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	ಆಗ (rev_nr > 1) {
		ಗಾಗಿ (i = 0; i < rev_nr / 2; i++) {
			ರಚನೆ commit_list *ವಿಲೀನ_ತಳ = ಖಾಲಿ;
			ಆದರೆ (repo_get_ವಿಲೀನ_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &ವಿಲೀನ_ತಳ) < 0 ||
			    !ವಿಲೀನ_ತಳ || ವಿಲೀನ_ತಳ->ಮುಂದಿನ) {
				ಆದರೆ (die_on_failure) {
					die(_("failed to find exact merge base"));
				} ಇಲ್ಲದಿದ್ದರೆ {
					commit_list_free(ವಿಲೀನ_ತಳ);
					ಬಿಡುಗಡೆ(rev);
					ಹಿಂತಿರುಗಿ ಖಾಲಿ;
				}
			}

			rev[i] = ವಿಲೀನ_ತಳ->ಅಂಶ;
			commit_list_free(ವಿಲೀನ_ತಳ);
		}

		ಆದರೆ (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ಸುತ್ತು_ಮೇಲೆ(rev_nr, 2);
	}

	ret = repo_ಒಳ_ವಿಲೀನ_bases(the_repository, ತಳ, rev[0]);
	ಆದರೆ (ret < 0)
		ನಿರ್ಗಮಿಸು(128);
	ಆದರೆ (!ret) {
		ಆದರೆ (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಬಿಡುಗಡೆ(rev);
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}
	}

	ಗಾಗಿ (i = 0; i < ಒಟ್ಟು; i++) {
		ಆದರೆ (ತಳ == list[i]) {
			ಆದರೆ (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಬಿಡುಗಡೆ(rev);
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
		}
	}

	ಬಿಡುಗಡೆ(rev);
	ಹಿಂತಿರುಗಿ ತಳ;
}

define_commit_slab(commit_ತಳ, ಪೂರ್ಣಾಂಕ);

ಸ್ಥಿರ ಶೂನ್ಯ prepare_bases(ರಚನೆ ತಳ_ವೃಕ್ಷ_info *bases,
			  ರಚನೆ commit *ತಳ,
			  ರಚನೆ commit **list,
			  ಗಾತ್ರ_t ಒಟ್ಟು)
{
	ರಚನೆ commit *commit;
	ರಚನೆ rev_info revs;
	ರಚನೆ diff_options diffopt;
	ರಚನೆ commit_ತಳ commit_ತಳ;

	ಆದರೆ (!ತಳ)
		ಹಿಂತಿರುಗಿ;

	init_commit_ತಳ(&commit_ತಳ);
	repo_diff_ಸಿದ್ಧತೆ(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&diffopt);

	oidcpy(&bases->ತಳ_commit, &ತಳ->object.oid);

	repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
	revs.max_parents = 1;
	revs.topo_ಕ್ರಮ = 1;
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಒಟ್ಟು; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		ಕೂಡಿಸು_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_ತಳ_at(&commit_ತಳ, list[i]) = 1;
	}
	ತಳ->object.flags |= UNINTERESTING;
	ಕೂಡಿಸು_pending_object(&revs, &ತಳ->object, "base");

	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	ಆಗ ((commit = get_revision(&revs)) != ಖಾಲಿ) {
		ರಚನೆ object_id oid;
		ರಚನೆ object_id *patch_id;
		ಆದರೆ (*commit_ತಳ_at(&commit_ತಳ, commit))
			ಮುಂದುವರಿ;
		ಆದರೆ (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_ತಳ(&commit_ತಳ);
	release_revisions(&revs);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_bases(ರಚನೆ ತಳ_ವೃಕ್ಷ_info *bases, FILE *file)
{
	ಪೂರ್ಣಾಂಕ i;

	/* Only do this once, either for the cover or for the first one */
	ಆದರೆ (is_null_oid(&bases->ತಳ_commit))
		ಹಿಂತಿರುಗಿ;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_ಗಮ್ಯ_hex(&bases->ತಳ_commit));

	/* Show the prerequisite patches */
	ಗಾಗಿ (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_ಗಮ್ಯ_hex(&bases->patch_id[i]));

	ಬಿಡುಗಡೆ(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->ತಳ_commit, the_repository->hash_algo);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *diff_title(ರಚನೆ strbuf *sb,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reroll_ಎಣಿಕೆ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *generic,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rerolled)
{
	ಪೂರ್ಣಾಂಕ v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	ಆದರೆ (reroll_ಎಣಿಕೆ && !strtol_i(reroll_ಎಣಿಕೆ, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addstr(sb, generic);
	ಹಿಂತಿರುಗಿ sb->buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ infer_range_diff_ranges(ರಚನೆ strbuf *r1,
				    ರಚನೆ strbuf *r2,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prev,
				    ರಚನೆ commit *origin,
				    ರಚನೆ commit *ತಲೆ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_oid = oid_ಗಮ್ಯ_hex(&ತಲೆ->object.oid);
	ಪೂರ್ಣಾಂಕ prev_is_range = is_range_diff_range(prev);

	ಆದರೆ (prev_is_range)
		strbuf_addstr(r1, prev);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(r1, "%s..%s", ತಲೆ_oid, prev);

	ಆದರೆ (origin)
		strbuf_addf(r2, "%s..%s", oid_ಗಮ್ಯ_hex(&origin->object.oid), ತಲೆ_oid);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	ಇಲ್ಲದಿದ್ದರೆ {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, ತಲೆ_oid);
	}
}

ಪೂರ್ಣಾಂಕ cmd_format_patch(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ format_config cfg;
	ರಚನೆ commit *commit;
	ರಚನೆ commit_ರಾಶಿ list = COMMIT_ರಾಶಿ_INIT;
	ರಚನೆ rev_info rev;
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;
	ರಚನೆ ಸಿದ್ಧತೆ_revision_opt s_r_opt;
	ಗಾತ್ರ_t ಒಟ್ಟು, i;
	ಪೂರ್ಣಾಂಕ use_stdout = 0;
	ಪೂರ್ಣಾಂಕ ಆರಂಭ_ಸಂಖ್ಯೆ = -1;
	ಪೂರ್ಣಾಂಕ just_numbers = 0;
	ಪೂರ್ಣಾಂಕ ignore_if_ಒಳ_upstream = 0;
	ಪೂರ್ಣಾಂಕ cover_letter = -1;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cover_letter_fmt = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ boundary_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಂಕ no_ದ್ವಿಮಾನ_diff = 0;
	ಪೂರ್ಣಾಂಕ zero_commit = 0;
	ರಚನೆ commit *origin = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಒಳ_ಉತ್ತರ_ಗಮ್ಯ = ಖಾಲಿ;
	ರಚನೆ patch_ids ids;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ use_patch_format = 0;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reroll_ಎಣಿಕೆ = ಖಾಲಿ;
	ಅಕ್ಷರ *cover_from_description_arg = ಖಾಲಿ;
	ಅಕ್ಷರ *description_file = ಖಾಲಿ;
	ಅಕ್ಷರ *branch_name = ಖಾಲಿ;
	ರಚನೆ ತಳ_ವೃಕ್ಷ_info bases;
	ರಚನೆ commit *ತಳ;
	ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ = 0;
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ = ಖಾಲಿ;
	ರಚನೆ oid_array idiff_prev = OID_ARRAY_INIT;
	ರಚನೆ strbuf idiff_title = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rdiff_prev = ಖಾಲಿ;
	ರಚನೆ strbuf rdiff1 = STRBUF_INIT;
	ರಚನೆ strbuf rdiff2 = STRBUF_INIT;
	ರಚನೆ strbuf rdiff_title = STRBUF_INIT;
	ರಚನೆ rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rfc = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ creation_ಗುಣಕ = -1;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signature = git_ಆವೃತ್ತಿ_string;
	ಅಕ್ಷರ *signature_ಗಮ್ಯ_free = ಖಾಲಿ;
	ಅಕ್ಷರ *signature_file_arg = ಖಾಲಿ;
	ರಚನೆ keep_callback_ದತ್ತಾಂಶ keep_callback_ದತ್ತಾಂಶ = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt_patch_suffix = ಖಾಲಿ;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ಖಾಲಿ,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ಖಾಲಿ,
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
		OPT_INTEGER(0, "start-number", &ಆರಂಭ_ಸಂಖ್ಯೆ,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_ಎಣಿಕೆ, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.ಲಘುಗಣಕ.fmt_patch_name_max,
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
			    PARSE_OPT_NONEG, subject_ಪೂರ್ವಪ್ರತ್ಯಯ_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_ದತ್ತಾಂಶ, ಖಾಲಿ,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ದ್ವಿಮಾನ_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_ಒಳ_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_ಗಣ_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_ಗುಂಪು(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), ಶೀರ್ಷಿಕೆ_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_ಗಮ್ಯ, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &ಒಳ_ಉತ್ತರ_ಗಮ್ಯ, N_("message-id"),
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
			       0, ತಳ_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_ಪ್ರಗತಿ,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_prev, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_prev, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_ಗುಣಕ,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_ಒಳ_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_ಕೊನೆ()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	ಸ್ಮರಣೆ_ನಕಲು(&rev.notes_opt, &cfg.notes_opt, ಗಾತ್ರ(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_ಶೀರ್ಷಿಕೆಗಳು = cfg.ಲಘುಗಣಕ.default_encode_email_ಶೀರ್ಷಿಕೆಗಳು;
	rev.expand_tabs_ಒಳ_log_default = 0;
	rev.verbose_ಶೀರ್ಷಿಕೆ = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, ಗಾತ್ರ(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.ಲಘುಗಣಕ.fmt_patch_subject_ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಆದರೆ (format_no_ಪೂರ್ವಪ್ರತ್ಯಯ)
		diff_ಗಣ_noprefix(&rev.diffopt);

	ಆದರೆ (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_ಒಳ_body_from = force_ಒಳ_body_from;

	ಆದರೆ (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	ಆದರೆ (cfg.ಲಘುಗಣಕ.fmt_patch_name_max <= cast_ಗಾತ್ರ_t_ಗಮ್ಯ_int(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("0000-") + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(fmt_patch_suffix)))
		cfg.ಲಘುಗಣಕ.fmt_patch_name_max = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("0000-") + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(fmt_patch_suffix);

	ಆದರೆ (cover_from_description_arg)
		cfg.cover_from_description_ಬಹುಲಕ = parse_cover_from_description(cover_from_description_arg);

	ಆದರೆ (rfc && rfc[0]) {
		cfg.subject_ಪೂರ್ವಪ್ರತ್ಯಯ = 1;
		ಆದರೆ (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	ಆದರೆ (reroll_ಎಣಿಕೆ) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_ಎಣಿಕೆ);
		rev.reroll_ಎಣಿಕೆ = reroll_ಎಣಿಕೆ;
	}

	rev.subject_ಪೂರ್ವಪ್ರತ್ಯಯ = cfg.sprefix.buf;

	ಗಾಗಿ (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.ವಸ್ತುಗಳು[i].string);
		strbuf_addch(&buf, '\n');
	}

	ಆದರೆ (cfg.extra_ಗಮ್ಯ.nr)
		strbuf_addstr(&buf, "To: ");
	ಗಾಗಿ (i = 0; i < cfg.extra_ಗಮ್ಯ.nr; i++) {
		ಆದರೆ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_ಗಮ್ಯ.ವಸ್ತುಗಳು[i].string);
		ಆದರೆ (i + 1 < cfg.extra_ಗಮ್ಯ.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	ಆದರೆ (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	ಗಾಗಿ (i = 0; i < cfg.extra_cc.nr; i++) {
		ಆದರೆ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.ವಸ್ತುಗಳು[i].string);
		ಆದರೆ (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_ಶೀರ್ಷಿಕೆಗಳು = ಗಮ್ಯ_free = strbuf_detach(&buf, ಖಾಲಿ);

	ಆದರೆ (cfg.from) {
		ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&rev.from_ident, cfg.from, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	ಆದರೆ (ಆರಂಭ_ಸಂಖ್ಯೆ < 0)
		ಆರಂಭ_ಸಂಖ್ಯೆ = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	ಆದರೆ (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	ಆದರೆ (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	ಆದರೆ (cfg.keep_subject && cfg.subject_ಪೂರ್ವಪ್ರತ್ಯಯ)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = ಸಿದ್ಧತೆ_revisions(argc, argv, &rev, &s_r_opt);
	ಆದರೆ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ಆದರೆ (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	ಆದರೆ (rev.diffopt.output_format & DIFF_FORMAT_NAME_ಸ್ಥಿತಿ)
		die(_("--name-status does not make sense"));
	ಆದರೆ (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	ಆದರೆ (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	ಆದರೆ (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	ಆದರೆ (!rev.diffopt.stat_ಅಗಲ)
		rev.diffopt.stat_ಅಗಲ = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_ಶೀರ್ಷಿಕೆ = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.ಲಘುಗಣಕ.fmt_patch_name_max;

	ಆದರೆ (!rev.diffopt.flags.ಪಠ್ಯ && !no_ದ್ವಿಮಾನ_diff)
		rev.diffopt.flags.ದ್ವಿಮಾನ = 1;

	ಆದರೆ (rev.show_notes)
		ತುಂಬು_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	ಆದರೆ (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	ಆದರೆ (use_stdout) {
		ಸಿದ್ಧತೆ_pager(the_repository);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!rev.diffopt.close_file) {
		ಪೂರ್ಣಾಂಕ saved;

		ಆದರೆ (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = ಗಣ_outdir(ಪೂರ್ವಪ್ರತ್ಯಯ, output_directory);

		ಆದರೆ (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_ಗಣ_shared_repository(the_repository, 0);
		ಆಯ್ಕೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, output_directory)) {
		ಸ್ಥಿತಿ SCLD_OK:
		ಸ್ಥಿತಿ SCLD_EXISTS:
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_ಗಣ_shared_repository(the_repository, saved);
		ಆದರೆ (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	ಆದರೆ (rev.pending.nr == 1) {
		ಪೂರ್ಣಾಂಕ check_ತಲೆ = 0;

		ಆದರೆ (rev.max_ಎಣಿಕೆ < 0 && !rev.show_ಬೇರು_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].ಅಂಶ->flags |= UNINTERESTING;
			ಕೂಡಿಸು_ತಲೆ_ಗಮ್ಯ_pending(&rev);
			check_ತಲೆ = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		ಆದರೆ (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_ತಲೆ = 1;

		ಆದರೆ (check_ತಲೆ) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (ref && skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			ಇಲ್ಲದಿದ್ದರೆ
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_ಬೇರು_diff = 1;

	ಆದರೆ (ignore_if_ಒಳ_upstream) {
		/* Don't say anything if head and upstream are the same. */
		ಆದರೆ (rev.pending.nr == 2) {
			ರಚನೆ object_array_ನಮೂದು *o = rev.pending.objects;
			ಆದರೆ (oideq(&o[0].ಅಂಶ->oid, &o[1].ಅಂಶ->oid))
				ಹೋಗು ಮುಗಿದ;
		}
		get_patch_ids(&rev, &ids);
	}

	ಆದರೆ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	ಆಗ ((commit = get_revision(&rev)) != ಖಾಲಿ) {
		ಆದರೆ (commit->object.flags & BOUNDARY) {
			boundary_ಎಣಿಕೆ++;
			origin = (boundary_ಎಣಿಕೆ == 1) ? commit : ಖಾಲಿ;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (ignore_if_ಒಳ_upstream && has_commit_patch_id(commit, &ids))
			ಮುಂದುವರಿ;

		commit_ರಾಶಿ_ತಳ್ಳು(&list, commit);
	}
	ಆದರೆ (!list.nr)
		/* nothing to do */
		ಹೋಗು ಮುಗಿದ;
	ಒಟ್ಟು = list.nr;

	ಆದರೆ (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		ಆದರೆ (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cover_letter == -1) {
		cover_letter = 1;
	}

	ಆದರೆ (cover_letter == -1) {
		ಆದರೆ (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (ಒಟ್ಟು > 1);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((idiff_prev.nr || rdiff_prev) && (ಒಟ್ಟು > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		ಇಲ್ಲದಿದ್ದರೆ
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	ಆದರೆ (!cfg.keep_subject && cfg.auto_ಸಂಖ್ಯೆ && (ಒಟ್ಟು > 1 || cover_letter))
		cfg.numbered = 1;
	ಆದರೆ (cfg.numbered)
		rev.ಒಟ್ಟು = ಒಟ್ಟು + ಆರಂಭ_ಸಂಖ್ಯೆ - 1;

	ಆದರೆ (idiff_prev.nr) {
		ಆದರೆ (!cover_letter && ಒಟ್ಟು != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_ವೃಕ್ಷ_oid(list.ವಸ್ತುಗಳು[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_ಎಣಿಕೆ,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	ಆದರೆ (creation_ಗುಣಕ < 0)
		creation_ಗುಣಕ = CREATION_ಗುಣಕ_FOR_THE_SAME_SERIES;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	ಆದರೆ (rdiff_prev) {
		ಆದರೆ (!cover_letter && ಒಟ್ಟು != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.ವಸ್ತುಗಳು[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_ಗುಣಕ = creation_ಗುಣಕ;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_ಎಣಿಕೆ,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_ವಾದಗಳು(&rdiff_notes, &rev);
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
	ಆದರೆ (!signature) {
		; /* --no-signature inhibits all signatures */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (signature && signature != git_ಆವೃತ್ತಿ_string) {
		; /* non-default signature already set */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		ಆದರೆ (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_ಗಮ್ಯ_free = strbuf_detach(&buf, ಖಾಲಿ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, ಗಾತ್ರ(bases));
	ತಳ = get_ತಳ_commit(&cfg, list.ವಸ್ತುಗಳು, list.nr);
	ಆದರೆ (ತಳ) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, ತಳ, list.ವಸ್ತುಗಳು, list.nr);
	}

	ಆದರೆ (ಒಳ_ಉತ್ತರ_ಗಮ್ಯ || cfg.thread || cover_letter) {
		rev.ref_ಸಂದೇಶ_ids = xmalloc(ಗಾತ್ರ(*rev.ref_ಸಂದೇಶ_ids));
		string_list_init_dup(rev.ref_ಸಂದೇಶ_ids);
	}
	ಆದರೆ (ಒಳ_ಉತ್ತರ_ಗಮ್ಯ) {
		ಅಕ್ಷರ *msgid = clean_ಸಂದೇಶ_id(ಒಳ_ಉತ್ತರ_ಗಮ್ಯ);
		string_list_append_nodup(rev.ref_ಸಂದೇಶ_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	ಆದರೆ (cover_letter) {
		ಆದರೆ (cfg.thread)
			gen_ಸಂದೇಶ_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.ವಸ್ತುಗಳು,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		ಒಟ್ಟು++;
		ಆರಂಭ_ಸಂಖ್ಯೆ--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ಖಾಲಿ;
		rev.rdiff1 = ಖಾಲಿ;
	}
	rev.ಕೂಡಿಸು_signoff = cfg.do_signoff;

	ಆದರೆ (show_ಪ್ರಗತಿ)
		ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(the_repository,
						  _("Generating patches"), ಒಟ್ಟು);
	ಆಗ (list.nr) {
		ಗಾತ್ರ_t idx = list.nr - 1;
		ಪೂರ್ಣಾಂಕ shown;

		display_ಪ್ರಗತಿ(ಪ್ರಗತಿ, ಒಟ್ಟು - idx);
		commit = commit_ರಾಶಿ_ತೆಗೆ(&list);
		rev.nr = ಒಟ್ಟು - idx + (ಆರಂಭ_ಸಂಖ್ಯೆ - 1);

		/* Make the second and subsequent mails replies to the first */
		ಆದರೆ (cfg.thread) {
			/* Have we already had a message ID? */
			ಆದರೆ (rev.ಸಂದೇಶ_id) {
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
				ಆದರೆ (cfg.thread == THREAD_SHALLOW
				    && rev.ref_ಸಂದೇಶ_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					ಬಿಡುಗಡೆ(rev.ಸಂದೇಶ_id);
				ಇಲ್ಲದಿದ್ದರೆ
					string_list_append_nodup(rev.ref_ಸಂದೇಶ_ids,
								 rev.ಸಂದೇಶ_id);
			}
			gen_ಸಂದೇಶ_id(&rev, oid_ಗಮ್ಯ_hex(&commit->object.oid));
		}

		ಆದರೆ (output_directory &&
		    open_ಮುಂದಿನ_file(rev.numbered_files ? ಖಾಲಿ : commit, ಖಾಲಿ, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_ವೃಕ್ಷ_commit(&rev, commit);
		free_commit_ಬಫರ್(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		ಆದರೆ (output_directory)
			rev.shown_one = 0;
		ಆದರೆ (shown) {
			print_bases(&bases, rev.diffopt.file);
			ಆದರೆ (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			ಇಲ್ಲದಿದ್ದರೆ
				print_signature(signature, rev.diffopt.file);
		}
		ಆದರೆ (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ಖಾಲಿ;
		}
	}
	stop_ಪ್ರಗತಿ(&ಪ್ರಗತಿ);
	commit_ರಾಶಿ_clear(&list);
	ಆದರೆ (ignore_if_ಒಳ_upstream)
		free_patch_ids(&ids);

ಮುಗಿದ:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	ಬಿಡುಗಡೆ(description_file);
	ಬಿಡುಗಡೆ(signature_file_arg);
	ಬಿಡುಗಡೆ(signature_ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(branch_name);
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	ಬಿಡುಗಡೆ(rev.ಸಂದೇಶ_id);
	ಆದರೆ (rev.ref_ಸಂದೇಶ_ids)
		string_list_clear(rev.ref_ಸಂದೇಶ_ids, 0);
	ಬಿಡುಗಡೆ(rev.ref_ಸಂದೇಶ_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_pending_commit(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ರಚನೆ rev_info *revs, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ object_id oid;
	ಆದರೆ (repo_get_oid(the_repository, arg, &oid) == 0) {
		ರಚನೆ commit *commit = lookup_commit_reference(the_repository,
								&oid);
		ಆದರೆ (commit) {
			commit->object.flags |= flags;
			ಕೂಡಿಸು_pending_object(revs, &commit->object, arg);
			ಹಿಂತಿರುಗಿ 0;
		}
	}
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಶೂನ್ಯ print_commit(ಅಕ್ಷರ sign, ರಚನೆ commit *commit, ಪೂರ್ಣಾಂಕ verbose,
			 ಪೂರ್ಣಾಂಕ abbrev, FILE *file)
{
	ಆದರೆ (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		fprintf(file, "%c %s %s\n", sign,
		       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &commit->object.oid, abbrev),
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
ಸ್ಥಿರ ಶೂನ್ಯ collect_diff_blob_oids(ರಚನೆ commit *commit,
				   ರಚನೆ diff_options *opts,
				   ರಚನೆ oidset *blobs)
{
	ರಚನೆ diff_ಸರತಿ_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	ಪ್ರತಿಜ್ಞಾ(!commit->parents || !commit->parents->ಮುಂದಿನ);

	ಆದರೆ (commit->parents)
		diff_ವೃಕ್ಷ_oid(&commit->parents->ಅಂಶ->object.oid,
			      &commit->object.oid, "", opts);
	ಇಲ್ಲದಿದ್ದರೆ
		diff_ಬೇರು_ವೃಕ್ಷ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < q->nr; i++) {
		ರಚನೆ diff_filepair *p = q->ಸರತಿ[i];
		ರಚನೆ userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_ಕಂಡುಹಿಡಿ_by_ಮಾರ್ಗ(opts->repo->ಸೂಚ್ಯಂಕ, p->one->ಮಾರ್ಗ);
		ಆದರೆ (drv && drv->ದ್ವಿಮಾನ > 0)
			ಮುಂದುವರಿ;

		ಆದರೆ (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ಖಾಲಿ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ಸೇರಿಸು(blobs, &p->one->oid);
		ಆದರೆ (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ಖಾಲಿ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ಸೇರಿಸು(blobs, &p->two->oid);
	}
	diff_ಸರತಿ_clear(q);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ always_ಹೊಂದಿಕೆ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
			ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *entry1 ಬಳಸದ,
			ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *entry2 ಬಳಸದ,
			ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಶೂನ್ಯ prefetch_cherry_blobs(ರಚನೆ repository *repo,
				  ರಚನೆ commit_list *list,
				  ರಚನೆ patch_ids *ids)
{
	ರಚನೆ oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	ಆದರೆ (!repo_has_promisor_remote(repo))
		ಹಿಂತಿರುಗಿ;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_ಹೊಂದಿಕೆ;

	/* Find header-only collisions, gather blobs from those commits */
	ಗಾಗಿ (ರಚನೆ commit_list *l = list; l; l = l->ಮುಂದಿನ) {
		ರಚನೆ commit *c = l->ಅಂಶ;
		bool ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = false;
		ಗಾಗಿ (ರಚನೆ patch_id *cur = patch_id_iter_ಮೊದಲ(c, ids);
		     cur;
		     cur = patch_id_iter_ಮುಂದಿನ(cur, ids)) {
			ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		ಆದರೆ (ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	ಆದರೆ (oidset_ಗಾತ್ರ(&blobs)) {
		ರಚನೆ oid_array ಗಮ್ಯ_fetch = OID_ARRAY_INIT;
		ರಚನೆ oidset_iter iter;
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid;

		oidset_iter_init(&blobs, &iter);
		ಆಗ ((oid = oidset_iter_ಮುಂದಿನ(&iter)))
			oid_array_append(&ಗಮ್ಯ_fetch, oid);

		promisor_remote_get_ನೇರ(repo, ಗಮ್ಯ_fetch.oid, ಗಮ್ಯ_fetch.nr);

		oid_array_clear(&ಗಮ್ಯ_fetch);
	}

	oidset_clear(&blobs);
}

ಪೂರ್ಣಾಂಕ cmd_cherry(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ patch_ids ids;
	ರಚನೆ commit *commit;
	ರಚನೆ commit_list *list = ಖಾಲಿ;
	ರಚನೆ branch *current_branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *upstream;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ = "HEAD";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಎಲ್ಲೆ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ verbose = 0, abbrev = 0;

	ರಚನೆ option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, cherry_usage, 0);

	ಆಯ್ಕೆ (argc) {
	ಸ್ಥಿತಿ 3:
		ಎಲ್ಲೆ = argv[2];
		/* FALLTHROUGH */
	ಸ್ಥಿತಿ 2:
		ತಲೆ = argv[1];
		/* FALLTHROUGH */
	ಸ್ಥಿತಿ 1:
		upstream = argv[0];
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		current_branch = branch_get(ಖಾಲಿ);
		upstream = branch_get_upstream(current_branch, ಖಾಲಿ);
		ಆದರೆ (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);
	revs.max_parents = 1;

	ಆದರೆ (ಕೂಡಿಸು_pending_commit(ತಲೆ, &revs, 0))
		die(_("unknown commit %s"), ತಲೆ);
	ಆದರೆ (ಕೂಡಿಸು_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	ಆದರೆ (revs.pending.nr == 2) {
		ರಚನೆ object_array_ನಮೂದು *o = revs.pending.objects;
		ಆದರೆ (oideq(&o[0].ಅಂಶ->oid, &o[1].ಅಂಶ->oid))
			ಹಿಂತಿರುಗಿ 0;
	}

	get_patch_ids(&revs, &ids);

	ಆದರೆ (ಎಲ್ಲೆ && ಕೂಡಿಸು_pending_commit(ಎಲ್ಲೆ, &revs, UNINTERESTING))
		die(_("unknown commit %s"), ಎಲ್ಲೆ);

	/* reverse the list of commits */
	ಆದರೆ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ಆಗ ((commit = get_revision(&revs)) != ಖಾಲಿ) {
		commit_list_ಸೇರಿಸು(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	ಗಾಗಿ (ರಚನೆ commit_list *l = list; l; l = l->ಮುಂದಿನ) {
		ಅಕ್ಷರ sign = '+';

		commit = l->ಅಂಶ;
		ಆದರೆ (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	ಹಿಂತಿರುಗಿ 0;
}
