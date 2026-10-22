/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb/streaming.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diffcore.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff-merges.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reflog-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "patch-ids.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shortlog.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "line-log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "version.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-slab.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "promisor-remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "range-diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tmp-objdir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "userdiff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAIL_DEFAULT_WRAP 72
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FORMAT_PATCH_NAME_MAX_DEFAULT 64

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ force_ਵਿੱਚ_body_from;
ਸਥਿਰ ਪੂਰਨ stdout_mboxrd;
ਸਥਿਰ ਪੂਰਨ format_no_ਅਗੇਤਰ;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ਨਿਰਾ
};

ਬਣਤਰ ਸਤਰ_opt_callback_ਅੰਕੜੇ {
	ਬਣਤਰ rev_info *rev;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਬਣਤਰ string_list ਦਲੀਲਾਂ;
};

ਸਥਿਰ ਪੂਰਨ session_is_interactive(ਖਾਲੀ)
{
	ਵਾਪਸ isatty(1) || pager_ਵਿੱਚ_use();
}

ਸਥਿਰ ਪੂਰਨ auto_decoration_style(ਖਾਲੀ)
{
	ਵਾਪਸ session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ਸਥਿਰ ਪੂਰਨ parse_decoration_style(ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਚੋਣ (git_parse_maybe_bool(ਮੁੱਲ)) {
	ਹਾਲਤ 1:
		ਵਾਪਸ DECORATE_SHORT_REFS;
	ਹਾਲਤ 0:
		ਵਾਪਸ 0;
	ਮੂਲ:
		ਰੋਕੋ;
	}
	ਜੇ (!strcmp(ਮੁੱਲ, "full"))
		ਵਾਪਸ DECORATE_ਭਰਿਆ_REFS;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਮੁੱਲ, "short"))
		ਵਾਪਸ DECORATE_SHORT_REFS;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਮੁੱਲ, "auto"))
		ਵਾਪਸ auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	ਵਾਪਸ -1;
}

ਬਣਤਰ log_config {
	ਪੂਰਨ default_abbrev_commit;
	ਪੂਰਨ default_show_ਜੜ੍ਹ;
	ਪੂਰਨ default_follow;
	ਪੂਰਨ default_show_signature;
	ਪੂਰਨ default_encode_email_ਸਿਰਲੇਖਾਂ;
	ਪੂਰਨ decoration_style;
	ਪੂਰਨ decoration_given;
	ਪੂਰਨ use_mailmap_config;
	ਅੱਖਰ *fmt_patch_subject_ਅਗੇਤਰ;
	ਪੂਰਨ fmt_patch_name_max;
	ਅੱਖਰ *fmt_pretty;
	ਅੱਖਰ *default_date_ਬਹੁਲਕ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	ਪੂਰਨ i_still_use_this;
#ਜੇ_ਅੰਤ
};

ਸਥਿਰ ਖਾਲੀ log_config_init(ਬਣਤਰ log_config *cfg)
{
	memset(cfg, 0, ਆਕਾਰ(*cfg));
	cfg->default_show_ਜੜ੍ਹ = 1;
	cfg->default_encode_email_ਸਿਰਲੇਖਾਂ = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_ਅਗੇਤਰ = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ਸਥਿਰ ਖਾਲੀ log_config_release(ਬਣਤਰ log_config *cfg)
{
	ਮੁਕਤ(cfg->default_date_ਬਹੁਲਕ);
	ਮੁਕਤ(cfg->fmt_pretty);
	ਮੁਕਤ(cfg->fmt_patch_subject_ਅਗੇਤਰ);
}

ਸਥਿਰ ਪੂਰਨ use_default_decoration_filter = 1;
ਸਥਿਰ ਬਣਤਰ string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਬਣਤਰ string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਬਣਤਰ string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ਸਥਿਰ ਪੂਰਨ clear_decorations_callback(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ decorate_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			     ਪੂਰਨ unset)
{
	ਬਣਤਰ log_config *cfg = opt->ਮੁੱਲ;

	ਜੇ (unset)
		cfg->decoration_style = 0;
	ਨਹੀਂਤਾਂ ਜੇ (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	ਨਹੀਂਤਾਂ
		cfg->decoration_style = DECORATE_SHORT_REFS;

	ਜੇ (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ log_ਸਤਰ_range_callback(ਅਚਲ ਬਣਤਰ option *option, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ ਸਤਰ_opt_callback_ਅੰਕੜੇ *ਅੰਕੜੇ = option->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!arg)
		ਵਾਪਸ -1;

	ਅੰਕੜੇ->rev->ਸਤਰ_level_traverse = 1;
	string_list_append(&ਅੰਕੜੇ->ਦਲੀਲਾਂ, arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ cmd_log_init_defaults(ਬਣਤਰ rev_info *rev,
				  ਬਣਤਰ log_config *cfg)
{
	ਜੇ (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	ਜੇ (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_ਸਿਰਲੇਖ = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_ਜੜ੍ਹ_diff = cfg->default_show_ਜੜ੍ਹ;
	rev->subject_ਅਗੇਤਰ = cfg->fmt_patch_subject_ਅਗੇਤਰ;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_ਸਿਰਲੇਖਾਂ = cfg->default_encode_email_ਸਿਰਲੇਖਾਂ;

	ਜੇ (cfg->default_date_ਬਹੁਲਕ)
		parse_date_format(cfg->default_date_ਬਹੁਲਕ, &rev->date_ਬਹੁਲਕ);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_default_decoration_filter(ਬਣਤਰ decoration_filter *decoration_filter)
{
	ਅੱਖਰ *ਮੁੱਲ = ਨਿਰਾ;
	ਬਣਤਰ string_list *include = decoration_filter->include_ref_ਨਮੂਨਾ;
	ਅਚਲ ਬਣਤਰ string_list *config_exclude;

	ਜੇ (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_ਨਮੂਨਾ,
					   ਵਸਤੂ->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	ਜੇ (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &ਮੁੱਲ) &&
	    !strcmp("all", ਮੁੱਲ))
		use_default_decoration_filter = 0;
	ਮੁਕਤ(ਮੁੱਲ);

	ਜੇ (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_ਨਮੂਨਾ->nr ||
	    decoration_filter->include_ref_ਨਮੂਨਾ->nr ||
	    decoration_filter->exclude_ref_config_ਨਮੂਨਾ->nr)
		ਵਾਪਸ;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(ref_namespace); i++) {
		ਜੇ (!ref_namespace[i].decoration)
			ਜਾਰੀ;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ਸਥਿਰ ਖਾਲੀ cmd_log_init_ਅੰਤ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ rev_info *rev, ਬਣਤਰ ਤਿਆਰੀ_revision_opt *opt,
			 ਬਣਤਰ log_config *cfg)
{
	ਬਣਤਰ userformat_want w;
	ਪੂਰਨ quiet = 0, ਸਰੋਤ = 0, mailmap;
	ਸਥਿਰ ਬਣਤਰ ਸਤਰ_opt_callback_ਅੰਕੜੇ ਸਤਰ_cb = {ਨਿਰਾ, ਨਿਰਾ, STRING_LIST_INIT_DUP};
	ਬਣਤਰ decoration_filter decoration_filter = {
		.exclude_ref_ਨਮੂਨਾ = &decorate_refs_exclude,
		.include_ref_ਨਮੂਨਾ = &decorate_refs_include,
		.exclude_ref_config_ਨਮੂਨਾ = &decorate_refs_exclude_config,
	};
	ਸਥਿਰ ਬਣਤਰ revision_sources revision_sources;

	ਅਚਲ ਬਣਤਰ option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &ਸਰੋਤ, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#ਜੇ_ਅੰਤ
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ਨਿਰਾ, ਨਿਰਾ,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ਨਿਰਾ, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ਨਿਰਾ, &ਸਤਰ_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_ਸਤਰ_range_callback),
		OPT_ਸਿਰਾ()
	};

	ਸਤਰ_cb.rev = rev;
	ਸਤਰ_cb.ਅਗੇਤਰ = ਅਗੇਤਰ;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ਜੇ (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = ਤਿਆਰੀ_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	ਜੇ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ਜੇ (rev->ਸਤਰ_level_traverse && rev->prune_ਅੰਕੜੇ.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, ਆਕਾਰ(w));
	userformat_ਲੱਭੋ_requirements(ਨਿਰਾ, &w);

	ਜੇ (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	ਜੇ (rev->show_notes)
		ਲੱਦੋ_display_notes(&rev->notes_opt);

	ਜੇ ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_ਨਕਾਬ) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_ਸਿਰਲੇਖ = 0;

	ਜੇ (ਸਰੋਤ || w.ਸਰੋਤ) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	ਜੇ (mailmap) {
		rev->mailmap = xmalloc(ਆਕਾਰ(ਬਣਤਰ string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	ਜੇ (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		ਜੇ (!cfg->decoration_given)
			cfg->decoration_style = 0;
		ਜੇ (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	ਜੇ (rev->commit_format == CMIT_FMT_USERFORMAT) {
		ਜੇ (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} ਨਹੀਂਤਾਂ ਜੇ (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	ਜੇ (cfg->decoration_style || rev->simplify_by_decoration) {
		ਸੈੱਟ_default_decoration_filter(&decoration_filter);

		ਜੇ (cfg->decoration_style)
			rev->show_decorations = 1;

		ਲੱਦੋ_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	ਜੇ (rev->ਸਤਰ_level_traverse)
		ਸਤਰ_log_init(rev, ਸਤਰ_cb.ਅਗੇਤਰ, &ਸਤਰ_cb.ਦਲੀਲਾਂ);

	ਤਿਆਰੀ_pager(the_repository);
}

ਸਥਿਰ ਖਾਲੀ cmd_log_init(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ rev_info *rev, ਬਣਤਰ ਤਿਆਰੀ_revision_opt *opt,
			 ਬਣਤਰ log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_ਅੰਤ(argc, argv, ਅਗੇਤਰ, rev, opt, cfg);
}

ਸਥਿਰ ਪੂਰਨ cmd_log_walk_no_free(ਬਣਤਰ rev_info *rev)
{
	ਬਣਤਰ commit *commit;
	ਪੂਰਨ saved_nrl = 0;
	ਪੂਰਨ saved_dcctc = 0;
	ਪੂਰਨ ਨਤੀਜਾ;

	ਜੇ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	ਜਦੋਂ ((commit = get_revision(rev)) != ਨਿਰਾ) {
		ਜੇ (!log_ਰੁੱਖ_commit(rev, commit) && rev->max_ਗਿਣਤੀ >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_ਗਿਣਤੀ++;
		ਜੇ (!rev->reflog_info && !rev->remerge_diff) {
			/*
			 * We may show a given commit multiple times when
			 * walking the reflogs. Therefore we still need it.
			 *
			 * Likewise, we potentially still need the parents
			 * of * already shown commits to determine merge
			 * bases when showing remerge diffs.
			 */
			free_commit_ਬਫਰ(the_repository->parsed_objects,
					   commit);
			commit_list_free(commit->parents);
			commit->parents = ਨਿਰਾ;
		}
		ਜੇ (saved_nrl < rev->diffopt.needed_rename_ਹੱਦਬੰਦੀ)
			saved_nrl = rev->diffopt.needed_rename_ਹੱਦਬੰਦੀ;
		ਜੇ (rev->diffopt.degraded_cc_ਮੰਜ਼ਿਲ_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_ਮੰਜ਼ਿਲ_c = saved_dcctc;
	rev->diffopt.needed_rename_ਹੱਦਬੰਦੀ = saved_nrl;

	ਨਤੀਜਾ = diff_ਨਤੀਜਾ_ਕੋਡ(rev);
	ਜੇ (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		ਨਤੀਜਾ = 02;
	}
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ cmd_log_walk(ਬਣਤਰ rev_info *rev)
{
	ਪੂਰਨ retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ git_log_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			  ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ log_config *cfg = cb;
	ਅਚਲ ਅੱਖਰ *ਥਾਂ_name;

	ਜੇ (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		ਵਾਪਸ git_config_string(&cfg->fmt_pretty, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_ਅਗੇਤਰ);
		ਵਾਪਸ git_config_string(&cfg->fmt_patch_subject_ਅਗੇਤਰ, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, ਮੁੱਲ, ctx->kvi);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_ਸਿਰਲੇਖਾਂ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_ਬਹੁਲਕ);
		ਵਾਪਸ git_config_string(&cfg->default_date_ਬਹੁਲਕ, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(ਮੁੱਲ);
		ਜੇ (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "log.diffmerges")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਵਾਪਸ diff_merges_config(ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "log.showroot")) {
		cfg->default_show_ਜੜ੍ਹ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (skip_ਅਗੇਤਰ(var, "color.decorate.", &ਥਾਂ_name))
		ਵਾਪਸ parse_decorate_color_config(var, ਥਾਂ_name, ਮੁੱਲ);
	ਜੇ (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_diff_ui_config(var, ਮੁੱਲ, ctx, cb);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
ਪੂਰਨ cmd_whatchanged(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ log_config cfg;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt opt;
	ਪੂਰਨ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, ਆਕਾਰ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, ਅਗੇਤਰ, &rev, &opt, &cfg);

	ਜੇ (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	ਜੇ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ show_tagger(ਅਚਲ ਅੱਖਰ *buf, ਬਣਤਰ rev_info *rev)
{
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	ਬਣਤਰ pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_ਬਹੁਲਕ = rev->date_ਬਹੁਲਕ;
	pp_user_info(&pp, "Tagger", &ਬਾਹਰਲਾ, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", ਬਾਹਰਲਾ.buf);
	strbuf_release(&ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਪੂਰਨ show_blob_object(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ rev_info *rev, ਅਚਲ ਅੱਖਰ *obj_name)
{
	ਬਣਤਰ object_id oidc;
	ਬਣਤਰ object_context obj_context = {0};
	ਅੱਖਰ *buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;

	fflush(rev->diffopt.file);
	ਜੇ (!rev->diffopt.flags.textconv_ਸੈੱਟ_ਰਾਹੀਂ_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		ਵਾਪਸ odb_ਧਾਰਾ_blob_ਮੰਜ਼ਿਲ_fd(the_repository->objects, 1, oid, ਨਿਰਾ, 0);

	ਜੇ (get_oid_with_context(the_repository, obj_name,
				 GET_OID_ਰਿਕਾਰਡ_ਰਸਤਾ,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	ਜੇ (!obj_context.ਰਸਤਾ ||
	    !textconv_object(the_repository, obj_context.ਰਸਤਾ,
			     obj_context.ਬਹੁਲਕ, &oidc, 1, &buf, &ਮਾਪ)) {
		object_context_release(&obj_context);
		ਵਾਪਸ odb_ਧਾਰਾ_blob_ਮੰਜ਼ਿਲ_fd(the_repository->objects, 1, oid, ਨਿਰਾ, 0);
	}

	ਜੇ (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, ਮਾਪ);
	object_context_release(&obj_context);
	ਮੁਕਤ(buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_ਟੈਗ_object(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ rev_info *rev)
{
	ਮਾਪ_t ਮਾਪ;
	ਗਿਣਤੀ object_type type;
	ਅੱਖਰ *buf = odb_read_object(the_repository->objects, oid, &type, &ਮਾਪ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ = 0;

	ਜੇ (!buf)
		ਵਾਪਸ error(_("could not read object %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));

	ਪ੍ਰਤਿਜ੍ਞਾ(type == OBJ_ਟੈਗ);
	ਜਦੋਂ (ਵਿੱਥ < ਮਾਪ && buf[ਵਿੱਥ] != '\n') {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ new_ਵਿੱਥ = ਵਿੱਥ + 1;
		ਅਚਲ ਅੱਖਰ *ident;
		ਜਦੋਂ (new_ਵਿੱਥ < ਮਾਪ && buf[new_ਵਿੱਥ++] != '\n')
			; /* do nothing */
		ਜੇ (skip_ਅਗੇਤਰ(buf + ਵਿੱਥ, "tagger ", &ident))
			show_tagger(ident, rev);
		ਵਿੱਥ = new_ਵਿੱਥ;
	}

	ਜੇ (ਵਿੱਥ < ਮਾਪ)
		fwrite(buf + ਵਿੱਥ, ਮਾਪ - ਵਿੱਥ, 1, rev->diffopt.file);
	ਮੁਕਤ(buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_ਰੁੱਖ_object(ਅਚਲ ਬਣਤਰ object_id *oid ਅਣਵਰਤਿਆ,
			    ਬਣਤਰ strbuf *ਅਧਾਰ ਅਣਵਰਤਿਆ,
			    ਅਚਲ ਅੱਖਰ *pathname, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ,
			    ਖਾਲੀ *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(ਬਹੁਲਕ) ? "/" : "");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_ਤਿਆਰੀ_revisions_tweak(ਬਣਤਰ rev_info *rev)
{
	ਜੇ (rev->ਪਹਿਲਾ_ਮਾਪਾ_only)
		diff_merges_default_ਮੰਜ਼ਿਲ_ਪਹਿਲਾ_ਮਾਪਾ(rev);
	ਨਹੀਂਤਾਂ
		diff_merges_default_ਮੰਜ਼ਿਲ_dense_combined(rev);
	ਜੇ (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

ਪੂਰਨ cmd_show(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ log_config cfg;
	ਬਣਤਰ rev_info rev;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt opt;
	ਬਣਤਰ pathspec ਮੇਲ_all;
	ਪੂਰਨ ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	ਜੇ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
	}

	memset(&ਮੇਲ_all, 0, ਆਕਾਰ(ਮੇਲ_all));
	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_ਸਿਰਲੇਖ = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_ਚੌੜਾਈ = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, ਆਕਾਰ(opt));
	opt.def = "HEAD";
	opt.tweak = show_ਤਿਆਰੀ_revisions_tweak;
	cmd_log_init(argc, argv, ਅਗੇਤਰ, &rev, &opt, &cfg);

	ਜੇ (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		ਵਾਪਸ ret;
	}

	rev.diffopt.no_free = 1;
	ਲਈ (i = 0; i < rev.pending.nr && !ret; i++) {
		ਬਣਤਰ object *o = rev.pending.objects[i].ਵਸਤੂ;
		ਅਚਲ ਅੱਖਰ *name = rev.pending.objects[i].name;
		ਚੋਣ (o->type) {
		ਹਾਲਤ OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_ਟੈਗ: {
			ਬਣਤਰ ਟੈਗ *t = (ਬਣਤਰ ਟੈਗ *)o;
			ਬਣਤਰ object_id *oid = get_tagged_oid(t);

			ਜੇ (rev.shown_one)
				ਅੱਖਰ_ਲਿਖੋ('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->ਟੈਗ,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_ਟੈਗ_object(&o->oid, &rev);
			rev.shown_one = 1;
			ਜੇ (ret)
				ਰੋਕੋ;
			o = parse_object(the_repository, oid);
			ਜੇ (!o)
				ret = error(_("could not read object %s"),
					    oid_ਮੰਜ਼ਿਲ_hex(oid));
			rev.pending.objects[i].ਵਸਤੂ = o;
			i--;
			ਰੋਕੋ;
		}
		ਹਾਲਤ OBJ_ਰੁੱਖ:
			ਜੇ (rev.shown_one)
				ਅੱਖਰ_ਲਿਖੋ('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_ਰੁੱਖ(the_repository, (ਬਣਤਰ ਰੁੱਖ *)o,
				  &ਮੇਲ_all, show_ਰੁੱਖ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_COMMIT:
		{
			ਬਣਤਰ object_array old;
			ਬਣਤਰ object_array blank = OBJECT_ARRAY_INIT;

			ਸਮ੍ਰਿਤੀ_ਨਕਲ(&old, &rev.pending, ਆਕਾਰ(old));
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(&rev.pending, &blank, ਆਕਾਰ(rev.pending));

			ਜੋੜੋ_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(&rev.pending, &old, ਆਕਾਰ(rev.pending));
			ਰੋਕੋ;
		}
		ਮੂਲ:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	ਵਾਪਸ ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
ਪੂਰਨ cmd_log_reflog(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ log_config cfg;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt opt;
	ਪੂਰਨ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_ਸਿਰਲੇਖ = 1;
	memset(&opt, 0, ਆਕਾਰ(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_ਸਿਰਲੇਖ = 1;
	cmd_log_init_ਅੰਤ(argc, argv, ਅਗੇਤਰ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ log_ਤਿਆਰੀ_revisions_tweak(ਬਣਤਰ rev_info *rev)
{
	ਜੇ (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_ਅੰਕੜੇ, 0))
		rev->diffopt.flags.follow_renames = 1;

	ਜੇ (rev->ਪਹਿਲਾ_ਮਾਪਾ_only)
		diff_merges_default_ਮੰਜ਼ਿਲ_ਪਹਿਲਾ_ਮਾਪਾ(rev);
}

ਪੂਰਨ cmd_log(ਪੂਰਨ argc,
	    ਅਚਲ ਅੱਖਰ **argv,
	    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ log_config cfg;
	ਬਣਤਰ rev_info rev;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt opt;
	ਪੂਰਨ ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_ਸਿਰਲੇਖ = 1;
	memset(&opt, 0, ਆਕਾਰ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_ਤਿਆਰੀ_revisions_tweak;
	cmd_log_init(argc, argv, ਅਗੇਤਰ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ਵਾਪਸ ret;
}

/* format-patch */

ਗਿਣਤੀ cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

ਗਿਣਤੀ thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

ਗਿਣਤੀ cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_ਸੁਨੇਹਾ,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

ਗਿਣਤੀ auto_ਅਧਾਰ_setting {
	AUTO_ਅਧਾਰ_NEVER,
	AUTO_ਅਧਾਰ_ALWAYS,
	AUTO_ਅਧਾਰ_WHEN_ABLE
};

ਬਣਤਰ format_config {
	ਬਣਤਰ log_config ਲਘੂਗਣਕ;
	ਗਿਣਤੀ thread_level thread;
	ਪੂਰਨ do_signoff;
	ਗਿਣਤੀ auto_ਅਧਾਰ_setting auto_ਅਧਾਰ;
	ਅੱਖਰ *ਅਧਾਰ_commit;
	ਅੱਖਰ *from;
	ਅੱਖਰ *signature;
	ਅੱਖਰ *signature_file;
	ਗਿਣਤੀ cover_setting config_cover_letter;
	ਅੱਖਰ *fmt_cover_letter_commit_list;
	ਅੱਖਰ *config_output_directory;
	ਗਿਣਤੀ cover_from_description cover_from_description_ਬਹੁਲਕ;
	ਪੂਰਨ show_notes;
	ਬਣਤਰ display_notes_opt notes_opt;
	ਪੂਰਨ numbered_cmdline_opt;
	ਪੂਰਨ numbered;
	ਪੂਰਨ auto_ਸੰਖਿਆ;
	ਅੱਖਰ *default_attach;
	ਬਣਤਰ string_list extra_hdr;
	ਬਣਤਰ string_list extra_ਮੰਜ਼ਿਲ;
	ਬਣਤਰ string_list extra_cc;
	ਪੂਰਨ keep_subject;
	ਪੂਰਨ subject_ਅਗੇਤਰ;
	ਬਣਤਰ strbuf sprefix;
	ਅੱਖਰ *fmt_patch_suffix;
};

ਸਥਿਰ ਖਾਲੀ format_config_init(ਬਣਤਰ format_config *cfg)
{
	memset(cfg, 0, ਆਕਾਰ(*cfg));
	log_config_init(&cfg->ਲਘੂਗਣਕ);
	cfg->cover_from_description_ਬਹੁਲਕ = COVER_FROM_ਸੁਨੇਹਾ;
	cfg->auto_ਸੰਖਿਆ = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_ਮੰਜ਼ਿਲ);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ਸਥਿਰ ਖਾਲੀ format_config_release(ਬਣਤਰ format_config *cfg)
{
	log_config_release(&cfg->ਲਘੂਗਣਕ);
	ਮੁਕਤ(cfg->ਅਧਾਰ_commit);
	ਮੁਕਤ(cfg->from);
	ਮੁਕਤ(cfg->signature);
	ਮੁਕਤ(cfg->signature_file);
	ਮੁਕਤ(cfg->config_output_directory);
	ਮੁਕਤ(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_ਮੰਜ਼ਿਲ, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	ਮੁਕਤ(cfg->fmt_patch_suffix);
	ਮੁਕਤ(cfg->fmt_cover_letter_commit_list);
}

ਸਥਿਰ ਗਿਣਤੀ cover_from_description parse_cover_from_description(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (!arg || !strcmp(arg, "default"))
		ਵਾਪਸ COVER_FROM_ਸੁਨੇਹਾ;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "none"))
		ਵਾਪਸ COVER_FROM_NONE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "message"))
		ਵਾਪਸ COVER_FROM_ਸੁਨੇਹਾ;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "subject"))
		ਵਾਪਸ COVER_FROM_SUBJECT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "auto"))
		ਵਾਪਸ COVER_FROM_AUTO;
	ਨਹੀਂਤਾਂ
		die(_("%s: invalid cover from description mode"), arg);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਸਿਰਲੇਖ(ਬਣਤਰ format_config *cfg, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(ਮੁੱਲ);
	ਜਦੋਂ (len && ਮੁੱਲ[len - 1] == '\n')
		len--;

	ਜੇ (!strncasecmp(ਮੁੱਲ, "to: ", 4)) {
		ਵਸਤੂ = string_list_append(&cfg->extra_ਮੰਜ਼ਿਲ, ਮੁੱਲ + 4);
		len -= 4;
	} ਨਹੀਂਤਾਂ ਜੇ (!strncasecmp(ਮੁੱਲ, "cc: ", 4)) {
		ਵਸਤੂ = string_list_append(&cfg->extra_cc, ਮੁੱਲ + 4);
		len -= 4;
	} ਨਹੀਂਤਾਂ {
		ਵਸਤੂ = string_list_append(&cfg->extra_hdr, ਮੁੱਲ);
	}

	ਵਸਤੂ->string[len] = '\0';
}

ਸਥਿਰ ਪੂਰਨ git_format_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			     ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਬਣਤਰ format_config *cfg = cb;

	ਜੇ (!strcmp(var, "format.headers")) {
		ਜੇ (!ਮੁੱਲ)
			die(_("format.headers without value"));
		ਜੋੜੋ_ਸਿਰਲੇਖ(cfg, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		ਵਾਪਸ git_config_string(&cfg->fmt_patch_suffix, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.to")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		string_list_append(&cfg->extra_ਮੰਜ਼ਿਲ, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.cc")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.numbered")) {
		ਜੇ (ਮੁੱਲ && !strcasecmp(ਮੁੱਲ, "auto")) {
			cfg->auto_ਸੰਖਿਆ = 1;
			ਵਾਪਸ 0;
		}
		cfg->numbered = git_config_bool(var, ਮੁੱਲ);
		cfg->auto_ਸੰਖਿਆ = cfg->auto_ਸੰਖਿਆ && cfg->numbered;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.attach")) {
		ਜੇ (ਮੁੱਲ && *ਮੁੱਲ) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(ਮੁੱਲ);
		} ਨਹੀਂਤਾਂ ਜੇ (ਮੁੱਲ && !*ਮੁੱਲ) {
			FREE_AND_NULL(cfg->default_attach);
		} ਨਹੀਂਤਾਂ {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_ਸੰਸਕਰਣ_string);
		}
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.thread")) {
		ਜੇ (ਮੁੱਲ && !strcasecmp(ਮੁੱਲ, "deep")) {
			cfg->thread = THREAD_DEEP;
			ਵਾਪਸ 0;
		}
		ਜੇ (ਮੁੱਲ && !strcasecmp(ਮੁੱਲ, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			ਵਾਪਸ 0;
		}
		cfg->thread = git_config_bool(var, ਮੁੱਲ) ? THREAD_SHALLOW : THREAD_UNSET;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		ਵਾਪਸ git_config_string(&cfg->signature, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		ਵਾਪਸ git_config_pathname(&cfg->signature_file, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.coverletter")) {
		ਜੇ (ਮੁੱਲ && !strcasecmp(ਮੁੱਲ, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			ਵਾਪਸ 0;
		}
		cfg->config_cover_letter = git_config_bool(var, ਮੁੱਲ) ? COVER_ON : COVER_OFF;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		ਵਾਪਸ git_config_string(&cfg->fmt_cover_letter_commit_list, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		ਵਾਪਸ git_config_string(&cfg->config_output_directory, var, ਮੁੱਲ);
	}
	ਜੇ (!strcmp(var, "format.useautobase")) {
		ਜੇ (ਮੁੱਲ && !strcasecmp(ਮੁੱਲ, "whenAble")) {
			cfg->auto_ਅਧਾਰ = AUTO_ਅਧਾਰ_WHEN_ABLE;
			ਵਾਪਸ 0;
		}
		cfg->auto_ਅਧਾਰ = git_config_bool(var, ਮੁੱਲ) ? AUTO_ਅਧਾਰ_ALWAYS : AUTO_ਅਧਾਰ_NEVER;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.from")) {
		ਪੂਰਨ b = git_parse_maybe_bool(ਮੁੱਲ);
		FREE_AND_NULL(cfg->from);
		ਜੇ (b < 0)
			cfg->from = xstrdup(ਮੁੱਲ);
		ਨਹੀਂਤਾਂ ਜੇ (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.forceinbodyfrom")) {
		force_ਵਿੱਚ_body_from = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.notes")) {
		ਪੂਰਨ b = git_parse_maybe_bool(ਮੁੱਲ);
		ਜੇ (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, ਮੁੱਲ);
		ਨਹੀਂਤਾਂ ਜੇ (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ਨਹੀਂਤਾਂ
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_ਬਹੁਲਕ = parse_cover_from_description(ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "format.noprefix")) {
		format_no_ਅਗੇਤਰ = git_parse_maybe_bool(ਮੁੱਲ);
		ਜੇ (format_no_ਅਗੇਤਰ < 0) {
			ਪੂਰਨ ਸਥਿਤੀ = die_ਸੁਨੇਹਾ(
				_("bad boolean config value '%s' for '%s'"),
				ਮੁੱਲ, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			ਬਾਹਰ_ਨਿਕਲੋ(ਸਥਿਤੀ);
		}
		ਵਾਪਸ 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	ਜੇ (!strcmp(var, "diff.noprefix"))
		ਵਾਪਸ 0;

	ਵਾਪਸ git_log_config(var, ਮੁੱਲ, ctx, &cfg->ਲਘੂਗਣਕ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *output_directory = ਨਿਰਾ;
ਸਥਿਰ ਪੂਰਨ outdir_ਵਿੱਥ;

ਸਥਿਰ ਪੂਰਨ open_ਅਗਲਾ_file(ਬਣਤਰ commit *commit, ਅਚਲ ਅੱਖਰ *subject,
			 ਬਣਤਰ rev_info *rev, ਪੂਰਨ quiet)
{
	ਬਣਤਰ strbuf filename = STRBUF_INIT;

	ਜੇ (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	ਜੇ (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	ਨਹੀਂਤਾਂ ਜੇ (commit)
		fmt_output_commit(&filename, commit, rev);
	ਨਹੀਂਤਾਂ
		fmt_output_subject(&filename, subject, rev);

	ਜੇ (!quiet)
		ਲਿਖੋ("%s\n", filename.buf + outdir_ਵਿੱਥ);

	ਜੇ (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		ਵਾਪਸ -1;
	}

	strbuf_release(&filename);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ get_patch_ids(ਬਣਤਰ rev_info *rev, ਬਣਤਰ patch_ids *ids)
{
	ਬਣਤਰ rev_info check_rev;
	ਬਣਤਰ commit *commit, *c1, *c2;
	ਬਣਤਰ object *o1, *o2;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags1, flags2;

	ਜੇ (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].ਵਸਤੂ;
	o2 = rev->pending.objects[1].ਵਸਤੂ;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	ਜੇ ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->ਅਗੇਤਰ);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	ਜੋੜੋ_pending_object(&check_rev, o1, "o1");
	ਜੋੜੋ_pending_object(&check_rev, o2, "o2");
	ਜੇ (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	ਜਦੋਂ ((commit = get_revision(&check_rev)) != ਨਿਰਾ) {
		ਜੋੜੋ_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, ਵੇਖਿਆ_2 | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, ਵੇਖਿਆ_2 | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ਸਥਿਰ ਖਾਲੀ gen_ਸੁਨੇਹਾ_id(ਬਣਤਰ rev_info *info, ਅਚਲ ਅੱਖਰ *ਅਧਾਰ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", ਅਧਾਰ,
		    (timestamp_t) ਸਮਾਂ(ਨਿਰਾ),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->ਸੁਨੇਹਾ_id = strbuf_detach(&buf, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ print_signature(ਅਚਲ ਅੱਖਰ *signature, FILE *file)
{
	ਜੇ (!signature || !*signature)
		ਵਾਪਸ;

	fprintf(file, "-- \n%s", signature);
	ਜੇ (signature[ਲੜੀ_ਲੰਬਾਈ(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ਸਥਿਰ ਅੱਖਰ *ਲੱਭੋ_branch_name(ਬਣਤਰ rev_info *rev)
{
	ਬਣਤਰ object_id branch_oid;
	ਅਚਲ ਬਣਤਰ object_id *tip_oid;
	ਅਚਲ ਅੱਖਰ *ref, *v;
	ਅੱਖਰ *ਭਰਿਆ_ref, *branch = ਨਿਰਾ;
	ਪੂਰਨ interesting_ਮਿਲਿਆ = 0;
	ਮਾਪ_t idx;

	ਲਈ (ਮਾਪ_t i = 0; i < rev->cmdline.nr; i++) {
		ਜੇ (rev->cmdline.rev[i].flags & UNINTERESTING)
			ਜਾਰੀ;
		ਜੇ (interesting_ਮਿਲਿਆ)
			ਵਾਪਸ ਨਿਰਾ;
		interesting_ਮਿਲਿਆ = 1;
		idx = i;
	}
	ਜੇ (!interesting_ਮਿਲਿਆ)
		ਵਾਪਸ ਨਿਰਾ;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].ਵਸਤੂ->oid;
	ਜੇ (repo_dwim_ref(the_repository, ref, ਲੜੀ_ਲੰਬਾਈ(ref), &branch_oid,
			  &ਭਰਿਆ_ref, 0) &&
	    skip_ਅਗੇਤਰ(ਭਰਿਆ_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	ਮੁਕਤ(ਭਰਿਆ_ref);
	ਵਾਪਸ branch;
}

ਸਥਿਰ ਖਾਲੀ show_diffstat(ਬਣਤਰ rev_info *rev,
			  ਬਣਤਰ commit *origin, ਬਣਤਰ commit *ਸਿਰ)
{
	ਬਣਤਰ diff_options opts;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&opts, &rev->diffopt, ਆਕਾਰ(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&opts);

	diff_ਰੁੱਖ_oid(get_commit_ਰੁੱਖ_oid(origin),
		      get_commit_ਰੁੱਖ_oid(ਸਿਰ),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ਸਥਿਰ ਖਾਲੀ read_desc_file(ਬਣਤਰ strbuf *buf, ਅਚਲ ਅੱਖਰ *desc_file)
{
	ਜੇ (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ਸਥਿਰ ਖਾਲੀ prepare_cover_ਪਾਠ(ਬਣਤਰ pretty_print_context *pp,
			       ਅਚਲ ਅੱਖਰ *description_file,
			       ਅਚਲ ਅੱਖਰ *branch_name,
			       ਬਣਤਰ strbuf *sb,
			       ਅਚਲ ਅੱਖਰ *encoding,
			       ਪੂਰਨ need_8bit_cte,
			       ਅਚਲ ਬਣਤਰ format_config *cfg)
{
	ਅਚਲ ਅੱਖਰ *subject = "*** SUBJECT HERE ***";
	ਅਚਲ ਅੱਖਰ *body = "*** BLURB HERE ***";
	ਬਣਤਰ strbuf description_sb = STRBUF_INIT;
	ਬਣਤਰ strbuf subject_sb = STRBUF_INIT;

	ਜੇ (cfg->cover_from_description_ਬਹੁਲਕ == COVER_FROM_NONE)
		ਜਾਓ do_pp;

	ਜੇ (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	ਨਹੀਂਤਾਂ ਜੇ (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	ਜੇ (!description_sb.len)
		ਜਾਓ do_pp;

	ਜੇ (cfg->cover_from_description_ਬਹੁਲਕ == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_ਬਹੁਲਕ == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	ਜੇ (cfg->cover_from_description_ਬਹੁਲਕ == COVER_FROM_ਸੁਨੇਹਾ ||
	    (cfg->cover_from_description_ਬਹੁਲਕ == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	ਨਹੀਂਤਾਂ
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ਬਣਤਰ rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ਬਣਤਰ string_list notes;
};

ਸਥਿਰ ਪੂਰਨ rdiff_notes_cb(ਅਚਲ ਬਣਤਰ option *option,
			  ਅਚਲ ਅੱਖਰ *arg,
			  ਪੂਰਨ unset)
{
	ਬਣਤਰ option opt = *option;
	ਬਣਤਰ rdiff_notes *rdiff_notes = option->ਮੁੱਲ;

	rdiff_notes->override = 1;
	opt.ਮੁੱਲ = &rdiff_notes->notes;
	ਵਾਪਸ parse_opt_string_list(&opt, arg, unset);
}

ਸਥਿਰ ਪੂਰਨ get_notes_refs(ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ, ਖਾਲੀ *arg)
{
	strvec_pushf(arg, "--notes=%s", ਵਸਤੂ->string);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ get_notes_ਦਲੀਲਾਂ(ਬਣਤਰ rdiff_notes *rdiff_notes,
			   ਬਣਤਰ rev_info *rev)
{
	ਜੇ (rdiff_notes->override) {
		ਜੇ (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		ਨਹੀਂਤਾਂ
			strvec_ਧੱਕੋ(&rev->rdiff_log_arg, "--no-notes");
	} ਨਹੀਂਤਾਂ ਜੇ (!rev->show_notes) {
		strvec_ਧੱਕੋ(&rev->rdiff_log_arg, "--no-notes");
	} ਨਹੀਂਤਾਂ ਜੇ (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ਧੱਕੋ(&rev->rdiff_log_arg, "--notes");
	} ਨਹੀਂਤਾਂ {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ਸਥਿਰ ਖਾਲੀ generate_shortlog_cover_letter(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ,
					   ਬਣਤਰ rev_info *rev,
					   ਬਣਤਰ commit **list,
					   ਪੂਰਨ nr)
{
	shortlog_init(ਲਘੂਗਣਕ);
	ਲਘੂਗਣਕ->wrap_ਸਤਰਾਂ = 1;
	ਲਘੂਗਣਕ->wrap = MAIL_DEFAULT_WRAP;
	ਲਘੂਗਣਕ->in1 = 2;
	ਲਘੂਗਣਕ->in2 = 4;
	ਲਘੂਗਣਕ->file = rev->diffopt.file;
	ਲਘੂਗਣਕ->groups = SHORTLOG_ਸਮੂਹ_AUTHOR;
	shortlog_ਅੰਤ_ਤਿਆਰੀ(ਲਘੂਗਣਕ);
	ਲਈ (ਪੂਰਨ i = 0; i < nr; i++)
		shortlog_ਜੋੜੋ_commit(ਲਘੂਗਣਕ, list[i]);

	shortlog_output(ਲਘੂਗਣਕ);
}

ਸਥਿਰ ਖਾਲੀ generate_commit_list_cover(FILE *cover_file, ਅਚਲ ਅੱਖਰ *format,
				       ਬਣਤਰ commit **list, ਪੂਰਨ n)
{
	ਬਣਤਰ strbuf commit_ਸਤਰ = STRBUF_INIT;
	ਬਣਤਰ pretty_print_context ctx = {0};
	ਬਣਤਰ rev_info rev = REV_INFO_INIT;

	rev.ਕੁੱਲ = n;
	ctx.rev = &rev;
	ਲਈ (ਪੂਰਨ i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_ਸੁਨੇਹਾ(the_repository, list[n - i], format,
				&commit_ਸਤਰ, &ctx);
		fprintf(cover_file, "%s\n", commit_ਸਤਰ.buf);
		strbuf_reset(&commit_ਸਤਰ);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_ਸਤਰ);
}

ਸਥਿਰ ਖਾਲੀ make_cover_letter(ਬਣਤਰ rev_info *rev, ਪੂਰਨ use_separate_file,
			      ਬਣਤਰ commit *origin,
			      ਪੂਰਨ nr, ਬਣਤਰ commit **list,
			      ਅਚਲ ਅੱਖਰ *description_file,
			      ਅਚਲ ਅੱਖਰ *branch_name,
			      ਪੂਰਨ quiet,
			      ਅਚਲ ਬਣਤਰ format_config *cfg,
			      ਅਚਲ ਅੱਖਰ *format)
{
	ਅਚਲ ਅੱਖਰ *from;
	ਬਣਤਰ shortlog ਲਘੂਗਣਕ;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ i;
	ਅਚਲ ਅੱਖਰ *encoding = "UTF-8";
	ਪੂਰਨ need_8bit_cte = 0;
	ਬਣਤਰ pretty_print_context pp = {0};
	ਬਣਤਰ commit *ਸਿਰ = list[0];
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

	ਜੇ (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	ਜੇ (use_separate_file &&
	    open_ਅਗਲਾ_file(ਨਿਰਾ, rev->numbered_files ? ਨਿਰਾ : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_ਸਿਰਲੇਖਾਂ(rev, ਸਿਰ, &pp.after_subject, &need_8bit_cte, 0);

	ਲਈ (i = 0; !need_8bit_cte && i < nr; i++) {
		ਅਚਲ ਅੱਖਰ *buf = repo_get_commit_ਬਫਰ(the_repository,
							 list[i], ਨਿਰਾ);
		ਜੇ (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_ਬਫਰ(the_repository, list[i], buf);
	}

	ਜੇ (!branch_name)
		branch_name = ਮੰਜ਼ਿਲ_free = ਲੱਭੋ_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_ਬਹੁਲਕ.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_ਸਿਰਲੇਖਾਂ = rev->encode_email_ਸਿਰਲੇਖਾਂ;
	pp_user_info(&pp, ਨਿਰਾ, &sb, from, encoding);
	prepare_cover_ਪਾਠ(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(pp.after_subject);
	strbuf_release(&sb);

	ਜੇ (skip_ਅਗੇਤਰ(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&ਲਘੂਗਣਕ, rev, list, nr);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	ਨਹੀਂਤਾਂ ਜੇ (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ਨਹੀਂਤਾਂ
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	ਜੇ (origin)
		show_diffstat(rev, origin, ਸਿਰ);

	ਜੇ (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	ਜੇ (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ਬਣਤਰ diff_options opts;
		ਬਣਤਰ range_diff_options range_diff_opts = {
			.creation_ਗੁਣਕ = rev->creation_ਗੁਣਕ,
			.dual_color = 1,
			.max_ਸਮ੍ਰਿਤੀ = RANGE_DIFF_MAX_ਸਮ੍ਰਿਤੀ_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_ਤਿਆਰੀ(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_ਤਿਆਰੀ_ਪੂਰਾ(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

ਸਥਿਰ ਅੱਖਰ *clean_ਸੁਨੇਹਾ_id(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_id_2)
{
	ਅੱਖਰ ch;
	ਅਚਲ ਅੱਖਰ *a, *z, *m;

	m = ਸੁਨੇਹਾ_id_2;
	ਜਦੋਂ ((ch = *m) && (ਖਾਲੀ_ਥਾਂ_ਹੈ(ch) || (ch == '<')))
		m++;
	a = m;
	z = ਨਿਰਾ;
	ਜਦੋਂ ((ch = *m)) {
		ਜੇ (!ਖਾਲੀ_ਥਾਂ_ਹੈ(ch) && (ch != '>'))
			z = m;
		m++;
	}
	ਜੇ (!z)
		die(_("insane in-reply-to: %s"), ਸੁਨੇਹਾ_id_2);
	ਜੇ (++z == m)
		ਵਾਪਸ xstrdup(a);
	ਵਾਪਸ xmemdupz(a, z - a);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੈੱਟ_outdir(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਅਚਲ ਅੱਖਰ *output_directory)
{
	ਜੇ (output_directory && is_absolute_ਰਸਤਾ(output_directory))
		ਵਾਪਸ output_directory;

	ਜੇ (!ਅਗੇਤਰ || !*ਅਗੇਤਰ) {
		ਜੇ (output_directory)
			ਵਾਪਸ output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_ਵਿੱਥ = 2;
		ਵਾਪਸ "./";
	}

	outdir_ਵਿੱਥ = ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ);
	ਜੇ (!output_directory)
		ਵਾਪਸ ਅਗੇਤਰ;

	ਵਾਪਸ ਅਗੇਤਰ_filename(ਅਗੇਤਰ, output_directory);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ਨਿਰਾ
};

ਬਣਤਰ keep_callback_ਅੰਕੜੇ {
	ਬਣਤਰ format_config *cfg;
	ਬਣਤਰ rev_info *revs;
};

ਸਥਿਰ ਪੂਰਨ keep_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ keep_callback_ਅੰਕੜੇ *ਅੰਕੜੇ = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ਅੰਕੜੇ->revs->ਕੁੱਲ = -1;
	ਅੰਕੜੇ->cfg->keep_subject = 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ subject_ਅਗੇਤਰ_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			    ਪੂਰਨ unset)
{
	ਬਣਤਰ format_config *cfg = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_ਅਗੇਤਰ = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ rfc_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			ਪੂਰਨ unset)
{
	ਅਚਲ ਅੱਖਰ **rfc = opt->ਮੁੱਲ;

	*rfc = opt->ਮੁੱਲ;
	ਜੇ (unset)
		*rfc = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ
		*rfc = arg ? arg : "RFC";
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ numbered_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			     ਪੂਰਨ unset)
{
	ਬਣਤਰ format_config *cfg = opt->ਮੁੱਲ;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	ਜੇ (unset)
		cfg->auto_ਸੰਖਿਆ =  0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ no_numbered_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	ਵਾਪਸ numbered_callback(opt, arg, 1);
}

ਸਥਿਰ ਪੂਰਨ output_directory_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			      ਪੂਰਨ unset)
{
	ਅਚਲ ਅੱਖਰ **dir = (ਅਚਲ ਅੱਖਰ **)opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	ਜੇ (*dir)
		die(_("two output directories?"));
	*dir = arg;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ thread_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ format_config *cfg = opt->ਮੁੱਲ;

	ਜੇ (unset)
		cfg->thread = THREAD_UNSET;
	ਨਹੀਂਤਾਂ ਜੇ (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ attach_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ rev_info *rev = (ਬਣਤਰ rev_info *)opt->ਮੁੱਲ;
	ਜੇ (unset)
		rev->mime_boundary = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ ਜੇ (arg)
		rev->mime_boundary = arg;
	ਨਹੀਂਤਾਂ
		rev->mime_boundary = git_ਸੰਸਕਰਣ_string;
	rev->no_inline = unset ? 0 : 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ inline_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ rev_info *rev = (ਬਣਤਰ rev_info *)opt->ਮੁੱਲ;
	ਜੇ (unset)
		rev->mime_boundary = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ ਜੇ (arg)
		rev->mime_boundary = arg;
	ਨਹੀਂਤਾਂ
		rev->mime_boundary = git_ਸੰਸਕਰਣ_string;
	rev->no_inline = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਸਿਰਲੇਖ_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			   ਪੂਰਨ unset)
{
	ਬਣਤਰ format_config *cfg = opt->ਮੁੱਲ;

	ਜੇ (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_ਮੰਜ਼ਿਲ, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} ਨਹੀਂਤਾਂ {
		ਜੋੜੋ_ਸਿਰਲੇਖ(cfg, arg);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ from_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਅੱਖਰ **from = opt->ਮੁੱਲ;

	ਮੁਕਤ(*from);

	ਜੇ (unset)
		*from = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ ਜੇ (arg)
		*from = xstrdup(arg);
	ਨਹੀਂਤਾਂ
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਅਧਾਰ_callback(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ format_config *cfg = opt->ਮੁੱਲ;

	ਜੇ (unset) {
		cfg->auto_ਅਧਾਰ = AUTO_ਅਧਾਰ_NEVER;
		FREE_AND_NULL(cfg->ਅਧਾਰ_commit);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "auto")) {
		cfg->auto_ਅਧਾਰ = AUTO_ਅਧਾਰ_ALWAYS;
		FREE_AND_NULL(cfg->ਅਧਾਰ_commit);
	} ਨਹੀਂਤਾਂ {
		cfg->auto_ਅਧਾਰ = AUTO_ਅਧਾਰ_NEVER;
		cfg->ਅਧਾਰ_commit = xstrdup(arg);
	}
	ਵਾਪਸ 0;
}

ਬਣਤਰ ਅਧਾਰ_ਰੁੱਖ_info {
	ਬਣਤਰ object_id ਅਧਾਰ_commit;
	ਪੂਰਨ nr_patch_id, alloc_patch_id;
	ਬਣਤਰ object_id *patch_id;
};

ਸਥਿਰ ਬਣਤਰ commit *get_ਅਧਾਰ_commit(ਅਚਲ ਬਣਤਰ format_config *cfg,
				      ਬਣਤਰ commit **list,
				      ਮਾਪ_t ਕੁੱਲ)
{
	ਬਣਤਰ commit *ਅਧਾਰ = ਨਿਰਾ;
	ਬਣਤਰ commit **rev;
	ਪੂਰਨ auto_select, die_on_failure, ret;
	ਮਾਪ_t i = 0, rev_nr = 0;

	ਚੋਣ (cfg->auto_ਅਧਾਰ) {
	ਹਾਲਤ AUTO_ਅਧਾਰ_NEVER:
		ਜੇ (cfg->ਅਧਾਰ_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} ਨਹੀਂਤਾਂ {
			/* no base information is requested */
			ਵਾਪਸ ਨਿਰਾ;
		}
		ਰੋਕੋ;
	ਹਾਲਤ AUTO_ਅਧਾਰ_ALWAYS:
	ਹਾਲਤ AUTO_ਅਧਾਰ_WHEN_ABLE:
		ਜੇ (cfg->ਅਧਾਰ_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} ਨਹੀਂਤਾਂ {
			auto_select = 1;
			die_on_failure = cfg->auto_ਅਧਾਰ == AUTO_ਅਧਾਰ_ALWAYS;
		}
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected automatic base selection method");
	}

	ਜੇ (!auto_select) {
		ਅਧਾਰ = lookup_commit_reference_by_name(cfg->ਅਧਾਰ_commit);
		ਜੇ (!ਅਧਾਰ)
			die(_("unknown commit %s"), cfg->ਅਧਾਰ_commit);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ branch *curr_branch = branch_get(ਨਿਰਾ);
		ਅਚਲ ਅੱਖਰ *upstream = branch_get_upstream(curr_branch, ਨਿਰਾ);
		ਜੇ (upstream) {
			ਬਣਤਰ commit_list *ਅਧਾਰ_list = ਨਿਰਾ;
			ਬਣਤਰ commit *commit;
			ਬਣਤਰ object_id oid;

			ਜੇ (repo_get_oid(the_repository, upstream, &oid)) {
				ਜੇ (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				ਨਹੀਂਤਾਂ
					ਵਾਪਸ ਨਿਰਾ;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			ਜੇ (repo_get_ਮਿਲਾਓ_bases_many(the_repository,
						      commit, ਕੁੱਲ,
						      list,
						      &ਅਧਾਰ_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !ਅਧਾਰ_list || ਅਧਾਰ_list->ਅਗਲਾ) {
				ਜੇ (die_on_failure) {
					die(_("could not find exact merge base"));
				} ਨਹੀਂਤਾਂ {
					commit_list_free(ਅਧਾਰ_list);
					ਵਾਪਸ ਨਿਰਾ;
				}
			}
			ਅਧਾਰ = ਅਧਾਰ_list->ਵਸਤੂ;
			commit_list_free(ਅਧਾਰ_list);
		} ਨਹੀਂਤਾਂ {
			ਜੇ (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ ਨਿਰਾ;
		}
	}

	ALLOC_ARRAY(rev, ਕੁੱਲ);
	ਲਈ (i = 0; i < ਕੁੱਲ; i++)
		rev[i] = list[i];

	rev_nr = ਕੁੱਲ;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	ਜਦੋਂ (rev_nr > 1) {
		ਲਈ (i = 0; i < rev_nr / 2; i++) {
			ਬਣਤਰ commit_list *ਮਿਲਾਓ_ਅਧਾਰ = ਨਿਰਾ;
			ਜੇ (repo_get_ਮਿਲਾਓ_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &ਮਿਲਾਓ_ਅਧਾਰ) < 0 ||
			    !ਮਿਲਾਓ_ਅਧਾਰ || ਮਿਲਾਓ_ਅਧਾਰ->ਅਗਲਾ) {
				ਜੇ (die_on_failure) {
					die(_("failed to find exact merge base"));
				} ਨਹੀਂਤਾਂ {
					commit_list_free(ਮਿਲਾਓ_ਅਧਾਰ);
					ਮੁਕਤ(rev);
					ਵਾਪਸ ਨਿਰਾ;
				}
			}

			rev[i] = ਮਿਲਾਓ_ਅਧਾਰ->ਵਸਤੂ;
			commit_list_free(ਮਿਲਾਓ_ਅਧਾਰ);
		}

		ਜੇ (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ਗੇੜਾ_ਉੱਪਰ(rev_nr, 2);
	}

	ret = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, ਅਧਾਰ, rev[0]);
	ਜੇ (ret < 0)
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	ਜੇ (!ret) {
		ਜੇ (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} ਨਹੀਂਤਾਂ {
			ਮੁਕਤ(rev);
			ਵਾਪਸ ਨਿਰਾ;
		}
	}

	ਲਈ (i = 0; i < ਕੁੱਲ; i++) {
		ਜੇ (ਅਧਾਰ == list[i]) {
			ਜੇ (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} ਨਹੀਂਤਾਂ {
				ਮੁਕਤ(rev);
				ਵਾਪਸ ਨਿਰਾ;
			}
		}
	}

	ਮੁਕਤ(rev);
	ਵਾਪਸ ਅਧਾਰ;
}

define_commit_slab(commit_ਅਧਾਰ, ਪੂਰਨ);

ਸਥਿਰ ਖਾਲੀ prepare_bases(ਬਣਤਰ ਅਧਾਰ_ਰੁੱਖ_info *bases,
			  ਬਣਤਰ commit *ਅਧਾਰ,
			  ਬਣਤਰ commit **list,
			  ਮਾਪ_t ਕੁੱਲ)
{
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ diff_options diffopt;
	ਬਣਤਰ commit_ਅਧਾਰ commit_ਅਧਾਰ;

	ਜੇ (!ਅਧਾਰ)
		ਵਾਪਸ;

	init_commit_ਅਧਾਰ(&commit_ਅਧਾਰ);
	repo_diff_ਤਿਆਰੀ(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&diffopt);

	oidcpy(&bases->ਅਧਾਰ_commit, &ਅਧਾਰ->object.oid);

	repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
	revs.max_parents = 1;
	revs.topo_ਕ੍ਰਮ = 1;
	ਲਈ (ਮਾਪ_t i = 0; i < ਕੁੱਲ; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		ਜੋੜੋ_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_ਅਧਾਰ_at(&commit_ਅਧਾਰ, list[i]) = 1;
	}
	ਅਧਾਰ->object.flags |= UNINTERESTING;
	ਜੋੜੋ_pending_object(&revs, &ਅਧਾਰ->object, "base");

	ਜੇ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	ਜਦੋਂ ((commit = get_revision(&revs)) != ਨਿਰਾ) {
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ object_id *patch_id;
		ਜੇ (*commit_ਅਧਾਰ_at(&commit_ਅਧਾਰ, commit))
			ਜਾਰੀ;
		ਜੇ (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_ਅਧਾਰ(&commit_ਅਧਾਰ);
	release_revisions(&revs);
}

ਸਥਿਰ ਖਾਲੀ print_bases(ਬਣਤਰ ਅਧਾਰ_ਰੁੱਖ_info *bases, FILE *file)
{
	ਪੂਰਨ i;

	/* Only do this once, either for the cover or for the first one */
	ਜੇ (is_null_oid(&bases->ਅਧਾਰ_commit))
		ਵਾਪਸ;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_ਮੰਜ਼ਿਲ_hex(&bases->ਅਧਾਰ_commit));

	/* Show the prerequisite patches */
	ਲਈ (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_ਮੰਜ਼ਿਲ_hex(&bases->patch_id[i]));

	ਮੁਕਤ(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->ਅਧਾਰ_commit, the_repository->hash_algo);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *diff_title(ਬਣਤਰ strbuf *sb,
			      ਅਚਲ ਅੱਖਰ *reroll_ਗਿਣਤੀ,
			      ਅਚਲ ਅੱਖਰ *generic,
			      ਅਚਲ ਅੱਖਰ *rerolled)
{
	ਪੂਰਨ v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	ਜੇ (reroll_ਗਿਣਤੀ && !strtol_i(reroll_ਗਿਣਤੀ, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	ਨਹੀਂਤਾਂ
		strbuf_addstr(sb, generic);
	ਵਾਪਸ sb->buf;
}

ਸਥਿਰ ਖਾਲੀ infer_range_diff_ranges(ਬਣਤਰ strbuf *r1,
				    ਬਣਤਰ strbuf *r2,
				    ਅਚਲ ਅੱਖਰ *prev,
				    ਬਣਤਰ commit *origin,
				    ਬਣਤਰ commit *ਸਿਰ)
{
	ਅਚਲ ਅੱਖਰ *ਸਿਰ_oid = oid_ਮੰਜ਼ਿਲ_hex(&ਸਿਰ->object.oid);
	ਪੂਰਨ prev_is_range = is_range_diff_range(prev);

	ਜੇ (prev_is_range)
		strbuf_addstr(r1, prev);
	ਨਹੀਂਤਾਂ
		strbuf_addf(r1, "%s..%s", ਸਿਰ_oid, prev);

	ਜੇ (origin)
		strbuf_addf(r2, "%s..%s", oid_ਮੰਜ਼ਿਲ_hex(&origin->object.oid), ਸਿਰ_oid);
	ਨਹੀਂਤਾਂ ਜੇ (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	ਨਹੀਂਤਾਂ {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, ਸਿਰ_oid);
	}
}

ਪੂਰਨ cmd_format_patch(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ format_config cfg;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ commit_ਢੇਰ list = COMMIT_ਢੇਰ_INIT;
	ਬਣਤਰ rev_info rev;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਬਣਤਰ ਤਿਆਰੀ_revision_opt s_r_opt;
	ਮਾਪ_t ਕੁੱਲ, i;
	ਪੂਰਨ use_stdout = 0;
	ਪੂਰਨ ਸ਼ੁਰੂ_ਸੰਖਿਆ = -1;
	ਪੂਰਨ just_numbers = 0;
	ਪੂਰਨ ignore_if_ਵਿੱਚ_upstream = 0;
	ਪੂਰਨ cover_letter = -1;
	ਅਚਲ ਅੱਖਰ *cover_letter_fmt = ਨਿਰਾ;
	ਪੂਰਨ boundary_ਗਿਣਤੀ = 0;
	ਪੂਰਨ no_ਦੁਆਧਾਰੀ_diff = 0;
	ਪੂਰਨ zero_commit = 0;
	ਬਣਤਰ commit *origin = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਵਿੱਚ_ਜਵਾਬ_ਮੰਜ਼ਿਲ = ਨਿਰਾ;
	ਬਣਤਰ patch_ids ids;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਪੂਰਨ use_patch_format = 0;
	ਪੂਰਨ quiet = 0;
	ਅਚਲ ਅੱਖਰ *reroll_ਗਿਣਤੀ = ਨਿਰਾ;
	ਅੱਖਰ *cover_from_description_arg = ਨਿਰਾ;
	ਅੱਖਰ *description_file = ਨਿਰਾ;
	ਅੱਖਰ *branch_name = ਨਿਰਾ;
	ਬਣਤਰ ਅਧਾਰ_ਰੁੱਖ_info bases;
	ਬਣਤਰ commit *ਅਧਾਰ;
	ਪੂਰਨ show_ਤਰੱਕੀ = 0;
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ = ਨਿਰਾ;
	ਬਣਤਰ oid_array idiff_prev = OID_ARRAY_INIT;
	ਬਣਤਰ strbuf idiff_title = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *rdiff_prev = ਨਿਰਾ;
	ਬਣਤਰ strbuf rdiff1 = STRBUF_INIT;
	ਬਣਤਰ strbuf rdiff2 = STRBUF_INIT;
	ਬਣਤਰ strbuf rdiff_title = STRBUF_INIT;
	ਬਣਤਰ rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ਅਚਲ ਅੱਖਰ *rfc = ਨਿਰਾ;
	ਪੂਰਨ creation_ਗੁਣਕ = -1;
	ਅਚਲ ਅੱਖਰ *signature = git_ਸੰਸਕਰਣ_string;
	ਅੱਖਰ *signature_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਅੱਖਰ *signature_file_arg = ਨਿਰਾ;
	ਬਣਤਰ keep_callback_ਅੰਕੜੇ keep_callback_ਅੰਕੜੇ = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ਅਚਲ ਅੱਖਰ *fmt_patch_suffix = ਨਿਰਾ;

	ਅਚਲ ਬਣਤਰ option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ਨਿਰਾ,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ਨਿਰਾ,
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
		OPT_INTEGER(0, "start-number", &ਸ਼ੁਰੂ_ਸੰਖਿਆ,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_ਗਿਣਤੀ, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.ਲਘੂਗਣਕ.fmt_patch_name_max,
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
			    PARSE_OPT_NONEG, subject_ਅਗੇਤਰ_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_ਅੰਕੜੇ, ਨਿਰਾ,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ਦੁਆਧਾਰੀ_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_ਵਿੱਚ_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_ਸੈੱਟ_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸਮੂਹ(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), ਸਿਰਲੇਖ_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_ਮੰਜ਼ਿਲ, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &ਵਿੱਚ_ਜਵਾਬ_ਮੰਜ਼ਿਲ, N_("message-id"),
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
			       0, ਅਧਾਰ_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_ਤਰੱਕੀ,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_prev, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_prev, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_ਗੁਣਕ,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_ਵਿੱਚ_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_ਸਿਰਾ()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&rev.notes_opt, &cfg.notes_opt, ਆਕਾਰ(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_ਸਿਰਲੇਖਾਂ = cfg.ਲਘੂਗਣਕ.default_encode_email_ਸਿਰਲੇਖਾਂ;
	rev.expand_tabs_ਵਿੱਚ_log_default = 0;
	rev.verbose_ਸਿਰਲੇਖ = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, ਆਕਾਰ(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.ਲਘੂਗਣਕ.fmt_patch_subject_ਅਗੇਤਰ);
	ਜੇ (format_no_ਅਗੇਤਰ)
		diff_ਸੈੱਟ_noprefix(&rev.diffopt);

	ਜੇ (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_ਵਿੱਚ_body_from = force_ਵਿੱਚ_body_from;

	ਜੇ (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	ਜੇ (cfg.ਲਘੂਗਣਕ.fmt_patch_name_max <= cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_int(ਲੜੀ_ਲੰਬਾਈ("0000-") + ਲੜੀ_ਲੰਬਾਈ(fmt_patch_suffix)))
		cfg.ਲਘੂਗਣਕ.fmt_patch_name_max = ਲੜੀ_ਲੰਬਾਈ("0000-") + ਲੜੀ_ਲੰਬਾਈ(fmt_patch_suffix);

	ਜੇ (cover_from_description_arg)
		cfg.cover_from_description_ਬਹੁਲਕ = parse_cover_from_description(cover_from_description_arg);

	ਜੇ (rfc && rfc[0]) {
		cfg.subject_ਅਗੇਤਰ = 1;
		ਜੇ (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		ਨਹੀਂਤਾਂ
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	ਜੇ (reroll_ਗਿਣਤੀ) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_ਗਿਣਤੀ);
		rev.reroll_ਗਿਣਤੀ = reroll_ਗਿਣਤੀ;
	}

	rev.subject_ਅਗੇਤਰ = cfg.sprefix.buf;

	ਲਈ (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.ਚੀਜ਼ਾਂ[i].string);
		strbuf_addch(&buf, '\n');
	}

	ਜੇ (cfg.extra_ਮੰਜ਼ਿਲ.nr)
		strbuf_addstr(&buf, "To: ");
	ਲਈ (i = 0; i < cfg.extra_ਮੰਜ਼ਿਲ.nr; i++) {
		ਜੇ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_ਮੰਜ਼ਿਲ.ਚੀਜ਼ਾਂ[i].string);
		ਜੇ (i + 1 < cfg.extra_ਮੰਜ਼ਿਲ.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	ਜੇ (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	ਲਈ (i = 0; i < cfg.extra_cc.nr; i++) {
		ਜੇ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.ਚੀਜ਼ਾਂ[i].string);
		ਜੇ (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_ਸਿਰਲੇਖਾਂ = ਮੰਜ਼ਿਲ_free = strbuf_detach(&buf, ਨਿਰਾ);

	ਜੇ (cfg.from) {
		ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&rev.from_ident, cfg.from, ਲੜੀ_ਲੰਬਾਈ(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	ਜੇ (ਸ਼ੁਰੂ_ਸੰਖਿਆ < 0)
		ਸ਼ੁਰੂ_ਸੰਖਿਆ = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	ਜੇ (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	ਜੇ (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	ਜੇ (cfg.keep_subject && cfg.subject_ਅਗੇਤਰ)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = ਤਿਆਰੀ_revisions(argc, argv, &rev, &s_r_opt);
	ਜੇ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ਜੇ (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	ਜੇ (rev.diffopt.output_format & DIFF_FORMAT_NAME_ਸਥਿਤੀ)
		die(_("--name-status does not make sense"));
	ਜੇ (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	ਜੇ (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	ਜੇ (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	ਜੇ (!rev.diffopt.stat_ਚੌੜਾਈ)
		rev.diffopt.stat_ਚੌੜਾਈ = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_ਸਿਰਲੇਖ = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.ਲਘੂਗਣਕ.fmt_patch_name_max;

	ਜੇ (!rev.diffopt.flags.ਪਾਠ && !no_ਦੁਆਧਾਰੀ_diff)
		rev.diffopt.flags.ਦੁਆਧਾਰੀ = 1;

	ਜੇ (rev.show_notes)
		ਲੱਦੋ_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	ਜੇ (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	ਜੇ (use_stdout) {
		ਤਿਆਰੀ_pager(the_repository);
	} ਨਹੀਂਤਾਂ ਜੇ (!rev.diffopt.close_file) {
		ਪੂਰਨ saved;

		ਜੇ (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = ਸੈੱਟ_outdir(ਅਗੇਤਰ, output_directory);

		ਜੇ (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_ਸੈੱਟ_shared_repository(the_repository, 0);
		ਚੋਣ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, output_directory)) {
		ਹਾਲਤ SCLD_OK:
		ਹਾਲਤ SCLD_EXISTS:
			ਰੋਕੋ;
		ਮੂਲ:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_ਸੈੱਟ_shared_repository(the_repository, saved);
		ਜੇ (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	ਜੇ (rev.pending.nr == 1) {
		ਪੂਰਨ check_ਸਿਰ = 0;

		ਜੇ (rev.max_ਗਿਣਤੀ < 0 && !rev.show_ਜੜ੍ਹ_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].ਵਸਤੂ->flags |= UNINTERESTING;
			ਜੋੜੋ_ਸਿਰ_ਮੰਜ਼ਿਲ_pending(&rev);
			check_ਸਿਰ = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		ਜੇ (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_ਸਿਰ = 1;

		ਜੇ (check_ਸਿਰ) {
			ਅਚਲ ਅੱਖਰ *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (ref && skip_ਅਗੇਤਰ(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			ਨਹੀਂਤਾਂ
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_ਜੜ੍ਹ_diff = 1;

	ਜੇ (ignore_if_ਵਿੱਚ_upstream) {
		/* Don't say anything if head and upstream are the same. */
		ਜੇ (rev.pending.nr == 2) {
			ਬਣਤਰ object_array_ਇੰਦਰਾਜ *o = rev.pending.objects;
			ਜੇ (oideq(&o[0].ਵਸਤੂ->oid, &o[1].ਵਸਤੂ->oid))
				ਜਾਓ ਪੂਰਾ;
		}
		get_patch_ids(&rev, &ids);
	}

	ਜੇ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	ਜਦੋਂ ((commit = get_revision(&rev)) != ਨਿਰਾ) {
		ਜੇ (commit->object.flags & BOUNDARY) {
			boundary_ਗਿਣਤੀ++;
			origin = (boundary_ਗਿਣਤੀ == 1) ? commit : ਨਿਰਾ;
			ਜਾਰੀ;
		}

		ਜੇ (ignore_if_ਵਿੱਚ_upstream && has_commit_patch_id(commit, &ids))
			ਜਾਰੀ;

		commit_ਢੇਰ_ਧੱਕੋ(&list, commit);
	}
	ਜੇ (!list.nr)
		/* nothing to do */
		ਜਾਓ ਪੂਰਾ;
	ਕੁੱਲ = list.nr;

	ਜੇ (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		ਜੇ (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} ਨਹੀਂਤਾਂ ਜੇ (cover_letter == -1) {
		cover_letter = 1;
	}

	ਜੇ (cover_letter == -1) {
		ਜੇ (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (ਕੁੱਲ > 1);
		ਨਹੀਂਤਾਂ ਜੇ ((idiff_prev.nr || rdiff_prev) && (ਕੁੱਲ > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		ਨਹੀਂਤਾਂ
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	ਜੇ (!cfg.keep_subject && cfg.auto_ਸੰਖਿਆ && (ਕੁੱਲ > 1 || cover_letter))
		cfg.numbered = 1;
	ਜੇ (cfg.numbered)
		rev.ਕੁੱਲ = ਕੁੱਲ + ਸ਼ੁਰੂ_ਸੰਖਿਆ - 1;

	ਜੇ (idiff_prev.nr) {
		ਜੇ (!cover_letter && ਕੁੱਲ != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_ਰੁੱਖ_oid(list.ਚੀਜ਼ਾਂ[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_ਗਿਣਤੀ,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	ਜੇ (creation_ਗੁਣਕ < 0)
		creation_ਗੁਣਕ = CREATION_ਗੁਣਕ_FOR_THE_SAME_SERIES;
	ਨਹੀਂਤਾਂ ਜੇ (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	ਜੇ (rdiff_prev) {
		ਜੇ (!cover_letter && ਕੁੱਲ != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.ਚੀਜ਼ਾਂ[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_ਗੁਣਕ = creation_ਗੁਣਕ;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_ਗਿਣਤੀ,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_ਦਲੀਲਾਂ(&rdiff_notes, &rev);
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
	ਜੇ (!signature) {
		; /* --no-signature inhibits all signatures */
	} ਨਹੀਂਤਾਂ ਜੇ (signature && signature != git_ਸੰਸਕਰਣ_string) {
		; /* non-default signature already set */
	} ਨਹੀਂਤਾਂ ਜੇ (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		ਜੇ (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_ਮੰਜ਼ਿਲ_free = strbuf_detach(&buf, ਨਿਰਾ);
	} ਨਹੀਂਤਾਂ ਜੇ (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, ਆਕਾਰ(bases));
	ਅਧਾਰ = get_ਅਧਾਰ_commit(&cfg, list.ਚੀਜ਼ਾਂ, list.nr);
	ਜੇ (ਅਧਾਰ) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, ਅਧਾਰ, list.ਚੀਜ਼ਾਂ, list.nr);
	}

	ਜੇ (ਵਿੱਚ_ਜਵਾਬ_ਮੰਜ਼ਿਲ || cfg.thread || cover_letter) {
		rev.ref_ਸੁਨੇਹਾ_ids = xmalloc(ਆਕਾਰ(*rev.ref_ਸੁਨੇਹਾ_ids));
		string_list_init_dup(rev.ref_ਸੁਨੇਹਾ_ids);
	}
	ਜੇ (ਵਿੱਚ_ਜਵਾਬ_ਮੰਜ਼ਿਲ) {
		ਅੱਖਰ *msgid = clean_ਸੁਨੇਹਾ_id(ਵਿੱਚ_ਜਵਾਬ_ਮੰਜ਼ਿਲ);
		string_list_append_nodup(rev.ref_ਸੁਨੇਹਾ_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	ਜੇ (cover_letter) {
		ਜੇ (cfg.thread)
			gen_ਸੁਨੇਹਾ_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.ਚੀਜ਼ਾਂ,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		ਕੁੱਲ++;
		ਸ਼ੁਰੂ_ਸੰਖਿਆ--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ਨਿਰਾ;
		rev.rdiff1 = ਨਿਰਾ;
	}
	rev.ਜੋੜੋ_signoff = cfg.do_signoff;

	ਜੇ (show_ਤਰੱਕੀ)
		ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(the_repository,
						  _("Generating patches"), ਕੁੱਲ);
	ਜਦੋਂ (list.nr) {
		ਮਾਪ_t idx = list.nr - 1;
		ਪੂਰਨ shown;

		display_ਤਰੱਕੀ(ਤਰੱਕੀ, ਕੁੱਲ - idx);
		commit = commit_ਢੇਰ_ਕੱਢੋ(&list);
		rev.nr = ਕੁੱਲ - idx + (ਸ਼ੁਰੂ_ਸੰਖਿਆ - 1);

		/* Make the second and subsequent mails replies to the first */
		ਜੇ (cfg.thread) {
			/* Have we already had a message ID? */
			ਜੇ (rev.ਸੁਨੇਹਾ_id) {
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
				ਜੇ (cfg.thread == THREAD_SHALLOW
				    && rev.ref_ਸੁਨੇਹਾ_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					ਮੁਕਤ(rev.ਸੁਨੇਹਾ_id);
				ਨਹੀਂਤਾਂ
					string_list_append_nodup(rev.ref_ਸੁਨੇਹਾ_ids,
								 rev.ਸੁਨੇਹਾ_id);
			}
			gen_ਸੁਨੇਹਾ_id(&rev, oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
		}

		ਜੇ (output_directory &&
		    open_ਅਗਲਾ_file(rev.numbered_files ? ਨਿਰਾ : commit, ਨਿਰਾ, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_ਰੁੱਖ_commit(&rev, commit);
		free_commit_ਬਫਰ(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		ਜੇ (output_directory)
			rev.shown_one = 0;
		ਜੇ (shown) {
			print_bases(&bases, rev.diffopt.file);
			ਜੇ (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			ਨਹੀਂਤਾਂ
				print_signature(signature, rev.diffopt.file);
		}
		ਜੇ (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ਨਿਰਾ;
		}
	}
	stop_ਤਰੱਕੀ(&ਤਰੱਕੀ);
	commit_ਢੇਰ_clear(&list);
	ਜੇ (ignore_if_ਵਿੱਚ_upstream)
		free_patch_ids(&ids);

ਪੂਰਾ:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	ਮੁਕਤ(description_file);
	ਮੁਕਤ(signature_file_arg);
	ਮੁਕਤ(signature_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(branch_name);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(rev.ਸੁਨੇਹਾ_id);
	ਜੇ (rev.ref_ਸੁਨੇਹਾ_ids)
		string_list_clear(rev.ref_ਸੁਨੇਹਾ_ids, 0);
	ਮੁਕਤ(rev.ref_ਸੁਨੇਹਾ_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_pending_commit(ਅਚਲ ਅੱਖਰ *arg, ਬਣਤਰ rev_info *revs, ਪੂਰਨ flags)
{
	ਬਣਤਰ object_id oid;
	ਜੇ (repo_get_oid(the_repository, arg, &oid) == 0) {
		ਬਣਤਰ commit *commit = lookup_commit_reference(the_repository,
								&oid);
		ਜੇ (commit) {
			commit->object.flags |= flags;
			ਜੋੜੋ_pending_object(revs, &commit->object, arg);
			ਵਾਪਸ 0;
		}
	}
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਖਾਲੀ print_commit(ਅੱਖਰ sign, ਬਣਤਰ commit *commit, ਪੂਰਨ verbose,
			 ਪੂਰਨ abbrev, FILE *file)
{
	ਜੇ (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		fprintf(file, "%c %s %s\n", sign,
		       repo_ਲੱਭੋ_unique_abbrev(the_repository, &commit->object.oid, abbrev),
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
ਸਥਿਰ ਖਾਲੀ collect_diff_blob_oids(ਬਣਤਰ commit *commit,
				   ਬਣਤਰ diff_options *opts,
				   ਬਣਤਰ oidset *blobs)
{
	ਬਣਤਰ diff_ਕਤਾਰ_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	ਪ੍ਰਤਿਜ੍ਞਾ(!commit->parents || !commit->parents->ਅਗਲਾ);

	ਜੇ (commit->parents)
		diff_ਰੁੱਖ_oid(&commit->parents->ਵਸਤੂ->object.oid,
			      &commit->object.oid, "", opts);
	ਨਹੀਂਤਾਂ
		diff_ਜੜ੍ਹ_ਰੁੱਖ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	ਲਈ (ਪੂਰਨ i = 0; i < q->nr; i++) {
		ਬਣਤਰ diff_filepair *p = q->ਕਤਾਰ[i];
		ਬਣਤਰ userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_ਲੱਭੋ_by_ਰਸਤਾ(opts->repo->ਸੂਚਕਾਂਕ, p->one->ਰਸਤਾ);
		ਜੇ (drv && drv->ਦੁਆਧਾਰੀ > 0)
			ਜਾਰੀ;

		ਜੇ (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ਨਿਰਾ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ਪਾਓ(blobs, &p->one->oid);
		ਜੇ (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ਨਿਰਾ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ਪਾਓ(blobs, &p->two->oid);
	}
	diff_ਕਤਾਰ_clear(q);
}

ਸਥਿਰ ਪੂਰਨ always_ਮੇਲ(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
			ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *entry1 ਅਣਵਰਤਿਆ,
			ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *entry2 ਅਣਵਰਤਿਆ,
			ਅਚਲ ਖਾਲੀ *keydata ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਖਾਲੀ prefetch_cherry_blobs(ਬਣਤਰ repository *repo,
				  ਬਣਤਰ commit_list *list,
				  ਬਣਤਰ patch_ids *ids)
{
	ਬਣਤਰ oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	ਜੇ (!repo_has_promisor_remote(repo))
		ਵਾਪਸ;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_ਮੇਲ;

	/* Find header-only collisions, gather blobs from those commits */
	ਲਈ (ਬਣਤਰ commit_list *l = list; l; l = l->ਅਗਲਾ) {
		ਬਣਤਰ commit *c = l->ਵਸਤੂ;
		bool ਮੇਲ_ਮਿਲਿਆ = false;
		ਲਈ (ਬਣਤਰ patch_id *cur = patch_id_iter_ਪਹਿਲਾ(c, ids);
		     cur;
		     cur = patch_id_iter_ਅਗਲਾ(cur, ids)) {
			ਮੇਲ_ਮਿਲਿਆ = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		ਜੇ (ਮੇਲ_ਮਿਲਿਆ)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	ਜੇ (oidset_ਮਾਪ(&blobs)) {
		ਬਣਤਰ oid_array ਮੰਜ਼ਿਲ_fetch = OID_ARRAY_INIT;
		ਬਣਤਰ oidset_iter iter;
		ਅਚਲ ਬਣਤਰ object_id *oid;

		oidset_iter_init(&blobs, &iter);
		ਜਦੋਂ ((oid = oidset_iter_ਅਗਲਾ(&iter)))
			oid_array_append(&ਮੰਜ਼ਿਲ_fetch, oid);

		promisor_remote_get_ਸਿੱਧਾ(repo, ਮੰਜ਼ਿਲ_fetch.oid, ਮੰਜ਼ਿਲ_fetch.nr);

		oid_array_clear(&ਮੰਜ਼ਿਲ_fetch);
	}

	oidset_clear(&blobs);
}

ਪੂਰਨ cmd_cherry(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ patch_ids ids;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ commit_list *list = ਨਿਰਾ;
	ਬਣਤਰ branch *current_branch;
	ਅਚਲ ਅੱਖਰ *upstream;
	ਅਚਲ ਅੱਖਰ *ਸਿਰ = "HEAD";
	ਅਚਲ ਅੱਖਰ *ਹੱਦਬੰਦੀ = ਨਿਰਾ;
	ਪੂਰਨ verbose = 0, abbrev = 0;

	ਬਣਤਰ option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, cherry_usage, 0);

	ਚੋਣ (argc) {
	ਹਾਲਤ 3:
		ਹੱਦਬੰਦੀ = argv[2];
		/* FALLTHROUGH */
	ਹਾਲਤ 2:
		ਸਿਰ = argv[1];
		/* FALLTHROUGH */
	ਹਾਲਤ 1:
		upstream = argv[0];
		ਰੋਕੋ;
	ਮੂਲ:
		current_branch = branch_get(ਨਿਰਾ);
		upstream = branch_get_upstream(current_branch, ਨਿਰਾ);
		ਜੇ (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, ਅਗੇਤਰ);
	revs.max_parents = 1;

	ਜੇ (ਜੋੜੋ_pending_commit(ਸਿਰ, &revs, 0))
		die(_("unknown commit %s"), ਸਿਰ);
	ਜੇ (ਜੋੜੋ_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	ਜੇ (revs.pending.nr == 2) {
		ਬਣਤਰ object_array_ਇੰਦਰਾਜ *o = revs.pending.objects;
		ਜੇ (oideq(&o[0].ਵਸਤੂ->oid, &o[1].ਵਸਤੂ->oid))
			ਵਾਪਸ 0;
	}

	get_patch_ids(&revs, &ids);

	ਜੇ (ਹੱਦਬੰਦੀ && ਜੋੜੋ_pending_commit(ਹੱਦਬੰਦੀ, &revs, UNINTERESTING))
		die(_("unknown commit %s"), ਹੱਦਬੰਦੀ);

	/* reverse the list of commits */
	ਜੇ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ਜਦੋਂ ((commit = get_revision(&revs)) != ਨਿਰਾ) {
		commit_list_ਪਾਓ(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	ਲਈ (ਬਣਤਰ commit_list *l = list; l; l = l->ਅਗਲਾ) {
		ਅੱਖਰ sign = '+';

		commit = l->ਵਸਤੂ;
		ਜੇ (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	ਵਾਪਸ 0;
}
