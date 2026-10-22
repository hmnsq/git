/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "odb/streaming.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diffcore.h"
#ଅନ୍ତର୍ଭାଵ "diff-merges.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "log-tree.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "oidset.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "reflog-walk.h"
#ଅନ୍ତର୍ଭାଵ "patch-ids.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "shortlog.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "line-log.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "version.h"
#ଅନ୍ତର୍ଭାଵ "mailmap.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "commit-slab.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"

#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "promisor-remote.h"
#ଅନ୍ତର୍ଭାଵ "range-diff.h"
#ଅନ୍ତର୍ଭାଵ "tmp-objdir.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "userdiff.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_in_body_from;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stdout_mboxrd;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା format_no_prefix;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ଖାଲି
};

ସଂରଚନା ଧାଡ଼ି_opt_callback_data {
	ସଂରଚନା rev_info *rev;
	ଅଚଳ ଅକ୍ଷର *prefix;
	ସଂରଚନା string_list args;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା session_is_interactive(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ isatty(1) || pager_in_use();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_decoration_style(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_decoration_style(ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ବାଛ (git_parse_maybe_bool(ମୂଲ୍ୟ)) {
	ସ୍ଥିତି 1:
		ଫେରନ୍ତୁ DECORATE_SHORT_REFS;
	ସ୍ଥିତି 0:
		ଫେରନ୍ତୁ 0;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଭାଙ୍ଗ;
	}
	ଯଦି (!strcmp(ମୂଲ୍ୟ, "full"))
		ଫେରନ୍ତୁ DECORATE_FULL_REFS;
	ଅନ୍ୟଥା ଯଦି (!strcmp(ମୂଲ୍ୟ, "short"))
		ଫେରନ୍ତୁ DECORATE_SHORT_REFS;
	ଅନ୍ୟଥା ଯଦି (!strcmp(ମୂଲ୍ୟ, "auto"))
		ଫେରନ୍ତୁ auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	ଫେରନ୍ତୁ -1;
}

ସଂରଚନା log_config {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_abbrev_commit;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_show_ମୂଳ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_follow;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_show_signature;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_encode_email_headers;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା decoration_style;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା decoration_given;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_mailmap_config;
	ଅକ୍ଷର *fmt_patch_subject_prefix;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_patch_name_max;
	ଅକ୍ଷର *fmt_pretty;
	ଅକ୍ଷର *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_still_use_this;
#endif
};

ସ୍ଥିର ଶୂନ୍ୟ log_config_init(ସଂରଚନା log_config *cfg)
{
	memset(cfg, 0, ଆକାର(*cfg));
	cfg->default_show_ମୂଳ = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ସ୍ଥିର ଶୂନ୍ୟ log_config_release(ସଂରଚନା log_config *cfg)
{
	ମୁକ୍ତ(cfg->default_date_mode);
	ମୁକ୍ତ(cfg->fmt_pretty);
	ମୁକ୍ତ(cfg->fmt_patch_subject_prefix);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_default_decoration_filter = 1;
ସ୍ଥିର ସଂରଚନା string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ସଂରଚନା string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ସ୍ଥିର ସଂରଚନା string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear_decorations_callback(ଅଚଳ ସଂରଚନା option *opt UNUSED,
				      ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା decorate_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା log_config *cfg = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		cfg->decoration_style = 0;
	ଅନ୍ୟଥା ଯଦି (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	ଅନ୍ୟଥା
		cfg->decoration_style = DECORATE_SHORT_REFS;

	ଯଦି (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା log_ଧାଡ଼ି_range_callback(ଅଚଳ ସଂରଚନା option *option, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା ଧାଡ଼ି_opt_callback_data *data = option->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!arg)
		ଫେରନ୍ତୁ -1;

	data->rev->ଧାଡ଼ି_level_traverse = 1;
	string_list_append(&data->args, arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ cmd_log_init_defaults(ସଂରଚନା rev_info *rev,
				  ସଂରଚନା log_config *cfg)
{
	ଯଦି (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	ଯଦି (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_ମୂଳ_diff = cfg->default_show_ମୂଳ;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	ଯଦି (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

ସ୍ଥିର ଶୂନ୍ୟ set_default_decoration_filter(ସଂରଚନା decoration_filter *decoration_filter)
{
	ଅକ୍ଷର *ମୂଲ୍ୟ = ଖାଲି;
	ସଂରଚନା string_list *include = decoration_filter->include_ref_pattern;
	ଅଚଳ ସଂରଚନା string_list *config_exclude;

	ଯଦି (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ସଂରଚନା string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	ଯଦି (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &ମୂଲ୍ୟ) &&
	    !strcmp("all", ମୂଲ୍ୟ))
		use_default_decoration_filter = 0;
	ମୁକ୍ତ(ମୂଲ୍ୟ);

	ଯଦି (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		ଫେରନ୍ତୁ;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		ଯଦି (!ref_namespace[i].decoration)
			ଜାରି;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ cmd_log_init_finish(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା rev_info *rev, ସଂରଚନା setup_revision_opt *opt,
			 ସଂରଚନା log_config *cfg)
{
	ସଂରଚନା userformat_want w;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0, source = 0, mailmap;
	ସ୍ଥିର ସଂରଚନା ଧାଡ଼ି_opt_callback_data ଧାଡ଼ି_cb = {ଖାଲି, ଖାଲି, STRING_LIST_INIT_DUP};
	ସଂରଚନା decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	ସ୍ଥିର ସଂରଚନା revision_sources revision_sources;

	ଅଚଳ ସଂରଚନା option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ଖାଲି, ଖାଲି,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ଖାଲି, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ଖାଲି, &ଧାଡ଼ି_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_ଧାଡ଼ି_range_callback),
		OPT_END()
	};

	ଧାଡ଼ି_cb.rev = rev;
	ଧାଡ଼ି_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ଯଦି (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	ଯଦି (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ଯଦି (rev->ଧାଡ଼ି_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, ଆକାର(w));
	userformat_find_requirements(ଖାଲି, &w);

	ଯଦି (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	ଯଦି (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	ଯଦି ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	ଯଦି (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	ଯଦି (mailmap) {
		rev->mailmap = xmalloc(ଆକାର(ସଂରଚନା string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	ଯଦି (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		ଯଦି (!cfg->decoration_given)
			cfg->decoration_style = 0;
		ଯଦି (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	ଯଦି (rev->commit_format == CMIT_FMT_USERFORMAT) {
		ଯଦି (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} ଅନ୍ୟଥା ଯଦି (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	ଯଦି (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		ଯଦି (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	ଯଦି (rev->ଧାଡ଼ି_level_traverse)
		ଧାଡ଼ି_log_init(rev, ଧାଡ଼ି_cb.prefix, &ଧାଡ଼ି_cb.args);

	setup_pager(the_repository);
}

ସ୍ଥିର ଶୂନ୍ୟ cmd_log_init(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା rev_info *rev, ସଂରଚନା setup_revision_opt *opt,
			 ସଂରଚନା log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_log_walk_no_free(ସଂରଚନା rev_info *rev)
{
	ସଂରଚନା commit *commit;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_nrl = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_dcctc = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	ଯଦି (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	ଯେତେବେଳେ ((commit = get_revision(rev)) != ଖାଲି) {
		ଯଦି (!log_ବୃକ୍ଷ_commit(rev, commit) && rev->max_ଗଣନା >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_ଗଣନା++;
		ଯଦି (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = ଖାଲି;
		}
		ଯଦି (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		ଯଦି (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	ଯଦି (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_log_walk(ସଂରଚନା rev_info *rev)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_log_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			  ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା log_config *cfg = cb;
	ଅଚଳ ଅକ୍ଷର *slot_name;

	ଯଦି (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		ଫେରନ୍ତୁ git_config_string(&cfg->fmt_pretty, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		ଫେରନ୍ତୁ git_config_string(&cfg->fmt_patch_subject_prefix, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, ମୂଲ୍ୟ, ctx->kvi);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		ଫେରନ୍ତୁ git_config_string(&cfg->default_date_mode, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(ମୂଲ୍ୟ);
		ଯଦି (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "log.diffmerges")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଫେରନ୍ତୁ diff_merges_config(ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "log.showroot")) {
		cfg->default_show_ମୂଳ = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (skip_prefix(var, "color.decorate.", &slot_name))
		ଫେରନ୍ତୁ parse_decorate_color_config(var, slot_name, ମୂଲ୍ୟ);
	ଯଦି (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_diff_ui_config(var, ମୂଲ୍ୟ, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_whatchanged(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା log_config cfg;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା setup_revision_opt opt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, ଆକାର(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ଯଦି (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	ଯଦି (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ଫେରନ୍ତୁ ret;
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ show_tagger(ଅଚଳ ଅକ୍ଷର *buf, ସଂରଚନା rev_info *rev)
{
	ସଂରଚନା strbuf out = STRBUF_INIT;
	ସଂରଚନା pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_blob_object(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା rev_info *rev, ଅଚଳ ଅକ୍ଷର *obj_name)
{
	ସଂରଚନା object_id oidc;
	ସଂରଚନା object_context obj_context = {0};
	ଅକ୍ଷର *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	ଯଦି (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		ଫେରନ୍ତୁ odb_stream_blob_to_fd(the_repository->objects, 1, oid, ଖାଲି, 0);

	ଯଦି (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	ଯଦି (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		ଫେରନ୍ତୁ odb_stream_blob_to_fd(the_repository->objects, 1, oid, ଖାଲି, 0);
	}

	ଯଦି (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_tag_object(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା rev_info *rev)
{
	size_t size;
	ଗଣନା object_type type;
	ଅକ୍ଷର *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	ଯଦି (!buf)
		ଫେରନ୍ତୁ error(_("could not read object %s"), oid_to_hex(oid));

	ପ୍ରତିଜ୍ଞା(type == OBJ_TAG);
	ଯେତେବେଳେ (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		ଅଚଳ ଅକ୍ଷର *ident;
		ଯେତେବେଳେ (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		ଯଦି (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	ଯଦି (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ବୃକ୍ଷ_object(ଅଚଳ ସଂରଚନା object_id *oid UNUSED,
			    ସଂରଚନା strbuf *base UNUSED,
			    ଅଚଳ ଅକ୍ଷର *pathname, unsigned mode,
			    ଶୂନ୍ୟ *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_setup_revisions_tweak(ସଂରଚନା rev_info *rev)
{
	ଯଦି (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	ଅନ୍ୟଥା
		diff_merges_default_to_dense_combined(rev);
	ଯଦି (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା log_config cfg;
	ସଂରଚନା rev_info rev;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା setup_revision_opt opt;
	ସଂରଚନା pathspec match_all;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	ଯଦି (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, ଆକାର(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, ଆକାର(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ଯଦି (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		ଫେରନ୍ତୁ ret;
	}

	rev.diffopt.no_free = 1;
	ପାଇଁ (i = 0; i < rev.pending.nr && !ret; i++) {
		ସଂରଚନା object *o = rev.pending.objects[i].item;
		ଅଚଳ ଅକ୍ଷର *name = rev.pending.objects[i].name;
		ବାଛ (o->type) {
		ସ୍ଥିତି OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_TAG: {
			ସଂରଚନା tag *t = (ସଂରଚନା tag *)o;
			ସଂରଚନା object_id *oid = get_tagged_oid(t);

			ଯଦି (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			ଯଦି (ret)
				ଭାଙ୍ଗ;
			o = parse_object(the_repository, oid);
			ଯଦି (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			ଭାଙ୍ଗ;
		}
		ସ୍ଥିତି OBJ_ବୃକ୍ଷ:
			ଯଦି (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_ବୃକ୍ଷ(the_repository, (ସଂରଚନା ବୃକ୍ଷ *)o,
				  &match_all, show_ବୃକ୍ଷ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_COMMIT:
		{
			ସଂରଚନା object_array old;
			ସଂରଚନା object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, ଆକାର(old));
			memcpy(&rev.pending, &blank, ଆକାର(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, ଆକାର(rev.pending));
			ଭାଙ୍ଗ;
		}
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	ଫେରନ୍ତୁ ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_log_reflog(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା log_config cfg;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା setup_revision_opt opt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, ଆକାର(opt));
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
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ log_setup_revisions_tweak(ସଂରଚନା rev_info *rev)
{
	ଯଦି (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	ଯଦି (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_log(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	    ଅଚଳ ଅକ୍ଷର **argv,
	    ଅଚଳ ଅକ୍ଷର *prefix,
	    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା log_config cfg;
	ସଂରଚନା rev_info rev;
	ସଂରଚନା setup_revision_opt opt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, ଆକାର(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ଫେରନ୍ତୁ ret;
}

/* format-patch */

ଗଣନା cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

ଗଣନା thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

ଗଣନା cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

ଗଣନା auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

ସଂରଚନା format_config {
	ସଂରଚନା log_config log;
	ଗଣନା thread_level thread;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_signoff;
	ଗଣନା auto_base_setting auto_base;
	ଅକ୍ଷର *base_commit;
	ଅକ୍ଷର *from;
	ଅକ୍ଷର *signature;
	ଅକ୍ଷର *signature_file;
	ଗଣନା cover_setting config_cover_letter;
	ଅକ୍ଷର *fmt_cover_letter_commit_list;
	ଅକ୍ଷର *config_output_directory;
	ଗଣନା cover_from_description cover_from_description_mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_notes;
	ସଂରଚନା display_notes_opt notes_opt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା numbered_cmdline_opt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା numbered;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_ସଂଖ୍ୟା;
	ଅକ୍ଷର *default_attach;
	ସଂରଚନା string_list extra_hdr;
	ସଂରଚନା string_list extra_to;
	ସଂରଚନା string_list extra_cc;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_subject;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା subject_prefix;
	ସଂରଚନା strbuf sprefix;
	ଅକ୍ଷର *fmt_patch_suffix;
};

ସ୍ଥିର ଶୂନ୍ୟ format_config_init(ସଂରଚନା format_config *cfg)
{
	memset(cfg, 0, ଆକାର(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_ସଂଖ୍ୟା = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ସ୍ଥିର ଶୂନ୍ୟ format_config_release(ସଂରଚନା format_config *cfg)
{
	log_config_release(&cfg->log);
	ମୁକ୍ତ(cfg->base_commit);
	ମୁକ୍ତ(cfg->from);
	ମୁକ୍ତ(cfg->signature);
	ମୁକ୍ତ(cfg->signature_file);
	ମୁକ୍ତ(cfg->config_output_directory);
	ମୁକ୍ତ(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	ମୁକ୍ତ(cfg->fmt_patch_suffix);
	ମୁକ୍ତ(cfg->fmt_cover_letter_commit_list);
}

ସ୍ଥିର ଗଣନା cover_from_description parse_cover_from_description(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (!arg || !strcmp(arg, "default"))
		ଫେରନ୍ତୁ COVER_FROM_MESSAGE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "none"))
		ଫେରନ୍ତୁ COVER_FROM_NONE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "message"))
		ଫେରନ୍ତୁ COVER_FROM_MESSAGE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "subject"))
		ଫେରନ୍ତୁ COVER_FROM_SUBJECT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "auto"))
		ଫେରନ୍ତୁ COVER_FROM_AUTO;
	ଅନ୍ୟଥା
		die(_("%s: invalid cover from description mode"), arg);
}

ସ୍ଥିର ଶୂନ୍ୟ add_header(ସଂରଚନା format_config *cfg, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ସଂରଚନା string_list_item *item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(ମୂଲ୍ୟ);
	ଯେତେବେଳେ (len && ମୂଲ୍ୟ[len - 1] == '\n')
		len--;

	ଯଦି (!strncasecmp(ମୂଲ୍ୟ, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, ମୂଲ୍ୟ + 4);
		len -= 4;
	} ଅନ୍ୟଥା ଯଦି (!strncasecmp(ମୂଲ୍ୟ, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, ମୂଲ୍ୟ + 4);
		len -= 4;
	} ଅନ୍ୟଥା {
		item = string_list_append(&cfg->extra_hdr, ମୂଲ୍ୟ);
	}

	item->string[len] = '\0';
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_format_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			     ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା format_config *cfg = cb;

	ଯଦି (!strcmp(var, "format.headers")) {
		ଯଦି (!ମୂଲ୍ୟ)
			die(_("format.headers without value"));
		add_header(cfg, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		ଫେରନ୍ତୁ git_config_string(&cfg->fmt_patch_suffix, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.to")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		string_list_append(&cfg->extra_to, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.cc")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.numbered")) {
		ଯଦି (ମୂଲ୍ୟ && !strcasecmp(ମୂଲ୍ୟ, "auto")) {
			cfg->auto_ସଂଖ୍ୟା = 1;
			ଫେରନ୍ତୁ 0;
		}
		cfg->numbered = git_config_bool(var, ମୂଲ୍ୟ);
		cfg->auto_ସଂଖ୍ୟା = cfg->auto_ସଂଖ୍ୟା && cfg->numbered;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.attach")) {
		ଯଦି (ମୂଲ୍ୟ && *ମୂଲ୍ୟ) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(ମୂଲ୍ୟ);
		} ଅନ୍ୟଥା ଯଦି (ମୂଲ୍ୟ && !*ମୂଲ୍ୟ) {
			FREE_AND_NULL(cfg->default_attach);
		} ଅନ୍ୟଥା {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.thread")) {
		ଯଦି (ମୂଲ୍ୟ && !strcasecmp(ମୂଲ୍ୟ, "deep")) {
			cfg->thread = THREAD_DEEP;
			ଫେରନ୍ତୁ 0;
		}
		ଯଦି (ମୂଲ୍ୟ && !strcasecmp(ମୂଲ୍ୟ, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			ଫେରନ୍ତୁ 0;
		}
		cfg->thread = git_config_bool(var, ମୂଲ୍ୟ) ? THREAD_SHALLOW : THREAD_UNSET;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		ଫେରନ୍ତୁ git_config_string(&cfg->signature, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		ଫେରନ୍ତୁ git_config_pathname(&cfg->signature_file, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.coverletter")) {
		ଯଦି (ମୂଲ୍ୟ && !strcasecmp(ମୂଲ୍ୟ, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			ଫେରନ୍ତୁ 0;
		}
		cfg->config_cover_letter = git_config_bool(var, ମୂଲ୍ୟ) ? COVER_ON : COVER_OFF;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		ଫେରନ୍ତୁ git_config_string(&cfg->fmt_cover_letter_commit_list, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		ଫେରନ୍ତୁ git_config_string(&cfg->config_output_directory, var, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(var, "format.useautobase")) {
		ଯଦି (ମୂଲ୍ୟ && !strcasecmp(ମୂଲ୍ୟ, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			ଫେରନ୍ତୁ 0;
		}
		cfg->auto_base = git_config_bool(var, ମୂଲ୍ୟ) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.from")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = git_parse_maybe_bool(ମୂଲ୍ୟ);
		FREE_AND_NULL(cfg->from);
		ଯଦି (b < 0)
			cfg->from = xstrdup(ମୂଲ୍ୟ);
		ଅନ୍ୟଥା ଯଦି (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.notes")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = git_parse_maybe_bool(ମୂଲ୍ୟ);
		ଯଦି (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, ମୂଲ୍ୟ);
		ଅନ୍ୟଥା ଯଦି (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ଅନ୍ୟଥା
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(ମୂଲ୍ୟ);
		ଯଦି (format_no_prefix < 0) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				ମୂଲ୍ୟ, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	ଯଦି (!strcmp(var, "diff.noprefix"))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ git_log_config(var, ମୂଲ୍ୟ, ctx, &cfg->log);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *output_directory = ଖାଲି;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା outdir_offset;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_next_file(ସଂରଚନା commit *commit, ଅଚଳ ଅକ୍ଷର *subject,
			 ସଂରଚନା rev_info *rev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ସଂରଚନା strbuf filename = STRBUF_INIT;

	ଯଦି (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	ଯଦି (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	ଅନ୍ୟଥା ଯଦି (commit)
		fmt_output_commit(&filename, commit, rev);
	ଅନ୍ୟଥା
		fmt_output_subject(&filename, subject, rev);

	ଯଦି (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	ଯଦି (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		ଫେରନ୍ତୁ -1;
	}

	strbuf_release(&filename);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ get_patch_ids(ସଂରଚନା rev_info *rev, ସଂରଚନା patch_ids *ids)
{
	ସଂରଚନା rev_info check_rev;
	ସଂରଚନା commit *commit, *c1, *c2;
	ସଂରଚନା object *o1, *o2;
	unsigned flags1, flags2;

	ଯଦି (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	ଯଦି ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	ଯଦି (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	ଯେତେବେଳେ ((commit = get_revision(&check_rev)) != ଖାଲି) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ସ୍ଥିର ଶୂନ୍ୟ gen_message_id(ସଂରଚନା rev_info *info, ଅଚଳ ଅକ୍ଷର *base)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(ଖାଲି),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ print_signature(ଅଚଳ ଅକ୍ଷର *signature, FILE *file)
{
	ଯଦି (!signature || !*signature)
		ଫେରନ୍ତୁ;

	fprintf(file, "-- \n%s", signature);
	ଯଦି (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ସ୍ଥିର ଅକ୍ଷର *find_branch_name(ସଂରଚନା rev_info *rev)
{
	ସଂରଚନା object_id branch_oid;
	ଅଚଳ ସଂରଚନା object_id *tip_oid;
	ଅଚଳ ଅକ୍ଷର *ref, *v;
	ଅକ୍ଷର *full_ref, *branch = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା interesting_found = 0;
	size_t idx;

	ପାଇଁ (size_t i = 0; i < rev->cmdline.nr; i++) {
		ଯଦି (rev->cmdline.rev[i].flags & UNINTERESTING)
			ଜାରି;
		ଯଦି (interesting_found)
			ଫେରନ୍ତୁ ଖାଲି;
		interesting_found = 1;
		idx = i;
	}
	ଯଦି (!interesting_found)
		ଫେରନ୍ତୁ ଖାଲି;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	ଯଦି (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	ମୁକ୍ତ(full_ref);
	ଫେରନ୍ତୁ branch;
}

ସ୍ଥିର ଶୂନ୍ୟ show_diffstat(ସଂରଚନା rev_info *rev,
			  ସଂରଚନା commit *origin, ସଂରଚନା commit *head)
{
	ସଂରଚନା diff_options opts;

	memcpy(&opts, &rev->diffopt, ଆକାର(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_ବୃକ୍ଷ_oid(get_commit_ବୃକ୍ଷ_oid(origin),
		      get_commit_ବୃକ୍ଷ_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ସ୍ଥିର ଶୂନ୍ୟ read_desc_file(ସଂରଚନା strbuf *buf, ଅଚଳ ଅକ୍ଷର *desc_file)
{
	ଯଦି (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_cover_text(ସଂରଚନା pretty_print_context *pp,
			       ଅଚଳ ଅକ୍ଷର *description_file,
			       ଅଚଳ ଅକ୍ଷର *branch_name,
			       ସଂରଚନା strbuf *sb,
			       ଅଚଳ ଅକ୍ଷର *encoding,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_8bit_cte,
			       ଅଚଳ ସଂରଚନା format_config *cfg)
{
	ଅଚଳ ଅକ୍ଷର *subject = "*** SUBJECT HERE ***";
	ଅଚଳ ଅକ୍ଷର *body = "*** BLURB HERE ***";
	ସଂରଚନା strbuf description_sb = STRBUF_INIT;
	ସଂରଚନା strbuf subject_sb = STRBUF_INIT;

	ଯଦି (cfg->cover_from_description_mode == COVER_FROM_NONE)
		ଯାଅ do_pp;

	ଯଦି (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	ଅନ୍ୟଥା ଯଦି (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	ଯଦି (!description_sb.len)
		ଯାଅ do_pp;

	ଯଦି (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	ଯଦି (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	ଅନ୍ୟଥା
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ସଂରଚନା rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ସଂରଚନା string_list notes;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdiff_notes_cb(ଅଚଳ ସଂରଚନା option *option,
			  ଅଚଳ ଅକ୍ଷର *arg,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା option opt = *option;
	ସଂରଚନା rdiff_notes *rdiff_notes = option->ମୂଲ୍ୟ;

	rdiff_notes->override = 1;
	opt.ମୂଲ୍ୟ = &rdiff_notes->notes;
	ଫେରନ୍ତୁ parse_opt_string_list(&opt, arg, unset);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_notes_refs(ସଂରଚନା string_list_item *item, ଶୂନ୍ୟ *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ get_notes_args(ସଂରଚନା rdiff_notes *rdiff_notes,
			   ସଂରଚନା rev_info *rev)
{
	ଯଦି (rdiff_notes->override) {
		ଯଦି (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		ଅନ୍ୟଥା
			strvec_ଠେଲ(&rev->rdiff_log_arg, "--no-notes");
	} ଅନ୍ୟଥା ଯଦି (!rev->show_notes) {
		strvec_ଠେଲ(&rev->rdiff_log_arg, "--no-notes");
	} ଅନ୍ୟଥା ଯଦି (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ଠେଲ(&rev->rdiff_log_arg, "--notes");
	} ଅନ୍ୟଥା {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ generate_shortlog_cover_letter(ସଂରଚନା shortlog *log,
					   ସଂରଚନା rev_info *rev,
					   ସଂରଚନା commit **list,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr)
{
	shortlog_init(log);
	log->wrap_ଧାଡ଼ିଗୁଡ଼ିକ = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

ସ୍ଥିର ଶୂନ୍ୟ generate_commit_list_cover(FILE *cover_file, ଅଚଳ ଅକ୍ଷର *format,
				       ସଂରଚନା commit **list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
	ସଂରଚନା strbuf commit_ଧାଡ଼ି = STRBUF_INIT;
	ସଂରଚନା pretty_print_context ctx = {0};
	ସଂରଚନା rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_ଧାଡ଼ି, &ctx);
		fprintf(cover_file, "%s\n", commit_ଧାଡ଼ି.buf);
		strbuf_reset(&commit_ଧାଡ଼ି);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_ଧାଡ଼ି);
}

ସ୍ଥିର ଶୂନ୍ୟ make_cover_letter(ସଂରଚନା rev_info *rev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_separate_file,
			      ସଂରଚନା commit *origin,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr, ସଂରଚନା commit **list,
			      ଅଚଳ ଅକ୍ଷର *description_file,
			      ଅଚଳ ଅକ୍ଷର *branch_name,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet,
			      ଅଚଳ ସଂରଚନା format_config *cfg,
			      ଅଚଳ ଅକ୍ଷର *format)
{
	ଅଚଳ ଅକ୍ଷର *from;
	ସଂରଚନା shortlog log;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଅଚଳ ଅକ୍ଷର *encoding = "UTF-8";
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_8bit_cte = 0;
	ସଂରଚନା pretty_print_context pp = {0};
	ସଂରଚନା commit *head = list[0];
	ଅକ୍ଷର *to_free = ଖାଲି;

	ଯଦି (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	ଯଦି (use_separate_file &&
	    open_next_file(ଖାଲି, rev->numbered_files ? ଖାଲି : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	ପାଇଁ (i = 0; !need_8bit_cte && i < nr; i++) {
		ଅଚଳ ଅକ୍ଷର *buf = repo_get_commit_buffer(the_repository,
							 list[i], ଖାଲି);
		ଯଦି (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	ଯଦି (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, ଖାଲି, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	ମୁକ୍ତ(to_free);
	ମୁକ୍ତ(pp.after_subject);
	strbuf_release(&sb);

	ଯଦି (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ଅନ୍ୟଥା ଯଦି (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	ଅନ୍ୟଥା ଯଦି (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	ଅନ୍ୟଥା ଯଦି (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ଅନ୍ୟଥା
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	ଯଦି (origin)
		show_diffstat(rev, origin, head);

	ଯଦି (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	ଯଦି (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ସଂରଚନା diff_options opts;
		ସଂରଚନା range_diff_options range_diff_opts = {
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

ସ୍ଥିର ଅକ୍ଷର *clean_message_id(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_id)
{
	ଅକ୍ଷର ch;
	ଅଚଳ ଅକ୍ଷର *a, *z, *m;

	m = ସନ୍ଦେଶ_id;
	ଯେତେବେଳେ ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = ଖାଲି;
	ଯେତେବେଳେ ((ch = *m)) {
		ଯଦି (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	ଯଦି (!z)
		die(_("insane in-reply-to: %s"), ସନ୍ଦେଶ_id);
	ଯଦି (++z == m)
		ଫେରନ୍ତୁ xstrdup(a);
	ଫେରନ୍ତୁ xmemdupz(a, z - a);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *set_outdir(ଅଚଳ ଅକ୍ଷର *prefix, ଅଚଳ ଅକ୍ଷର *output_directory)
{
	ଯଦି (output_directory && is_absolute_path(output_directory))
		ଫେରନ୍ତୁ output_directory;

	ଯଦି (!prefix || !*prefix) {
		ଯଦି (output_directory)
			ଫେରନ୍ତୁ output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		ଫେରନ୍ତୁ "./";
	}

	outdir_offset = strlen(prefix);
	ଯଦି (!output_directory)
		ଫେରନ୍ତୁ prefix;

	ଫେରନ୍ତୁ prefix_filename(prefix, output_directory);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ଖାଲି
};

ସଂରଚନା keep_callback_data {
	ସଂରଚନା format_config *cfg;
	ସଂରଚନା rev_info *revs;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା keep_callback_data *data = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା subject_prefix_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା format_config *cfg = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rfc_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅଚଳ ଅକ୍ଷର **rfc = opt->ମୂଲ୍ୟ;

	*rfc = opt->ମୂଲ୍ୟ;
	ଯଦି (unset)
		*rfc = ଖାଲି;
	ଅନ୍ୟଥା
		*rfc = arg ? arg : "RFC";
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା numbered_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା format_config *cfg = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	ଯଦି (unset)
		cfg->auto_ସଂଖ୍ୟା =  0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_numbered_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);
	ଫେରନ୍ତୁ numbered_callback(opt, arg, 1);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_directory_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅଚଳ ଅକ୍ଷର **dir = (ଅଚଳ ଅକ୍ଷର **)opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	ଯଦି (*dir)
		die(_("two output directories?"));
	*dir = arg;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା thread_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା format_config *cfg = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		cfg->thread = THREAD_UNSET;
	ଅନ୍ୟଥା ଯଦି (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା attach_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା rev_info *rev = (ସଂରଚନା rev_info *)opt->ମୂଲ୍ୟ;
	ଯଦି (unset)
		rev->mime_boundary = ଖାଲି;
	ଅନ୍ୟଥା ଯଦି (arg)
		rev->mime_boundary = arg;
	ଅନ୍ୟଥା
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା inline_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା rev_info *rev = (ସଂରଚନା rev_info *)opt->ମୂଲ୍ୟ;
	ଯଦି (unset)
		rev->mime_boundary = ଖାଲି;
	ଅନ୍ୟଥା ଯଦି (arg)
		rev->mime_boundary = arg;
	ଅନ୍ୟଥା
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା header_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା format_config *cfg = opt->ମୂଲ୍ୟ;

	ଯଦି (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} ଅନ୍ୟଥା {
		add_header(cfg, arg);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅକ୍ଷର **from = opt->ମୂଲ୍ୟ;

	ମୁକ୍ତ(*from);

	ଯଦି (unset)
		*from = ଖାଲି;
	ଅନ୍ୟଥା ଯଦି (arg)
		*from = xstrdup(arg);
	ଅନ୍ୟଥା
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_callback(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା format_config *cfg = opt->ମୂଲ୍ୟ;

	ଯଦି (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} ଅନ୍ୟଥା {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା base_ବୃକ୍ଷ_info {
	ସଂରଚନା object_id base_commit;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_patch_id, alloc_patch_id;
	ସଂରଚନା object_id *patch_id;
};

ସ୍ଥିର ସଂରଚନା commit *get_base_commit(ଅଚଳ ସଂରଚନା format_config *cfg,
				      ସଂରଚନା commit **list,
				      size_t total)
{
	ସଂରଚନା commit *base = ଖାଲି;
	ସଂରଚନା commit **rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	ବାଛ (cfg->auto_base) {
	ସ୍ଥିତି AUTO_BASE_NEVER:
		ଯଦି (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} ଅନ୍ୟଥା {
			/* no base information is requested */
			ଫେରନ୍ତୁ ଖାଲି;
		}
		ଭାଙ୍ଗ;
	ସ୍ଥିତି AUTO_BASE_ALWAYS:
	ସ୍ଥିତି AUTO_BASE_WHEN_ABLE:
		ଯଦି (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} ଅନ୍ୟଥା {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected automatic base selection method");
	}

	ଯଦି (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		ଯଦି (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} ଅନ୍ୟଥା {
		ସଂରଚନା branch *curr_branch = branch_get(ଖାଲି);
		ଅଚଳ ଅକ୍ଷର *upstream = branch_get_upstream(curr_branch, ଖାଲି);
		ଯଦି (upstream) {
			ସଂରଚନା commit_list *base_list = ଖାଲି;
			ସଂରଚନା commit *commit;
			ସଂରଚନା object_id oid;

			ଯଦି (repo_get_oid(the_repository, upstream, &oid)) {
				ଯଦି (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				ଅନ୍ୟଥା
					ଫେରନ୍ତୁ ଖାଲି;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			ଯଦି (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				ଯଦି (die_on_failure) {
					die(_("could not find exact merge base"));
				} ଅନ୍ୟଥା {
					commit_list_free(base_list);
					ଫେରନ୍ତୁ ଖାଲି;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} ଅନ୍ୟଥା {
			ଯଦି (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ ଖାଲି;
		}
	}

	ALLOC_ARRAY(rev, total);
	ପାଇଁ (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	ଯେତେବେଳେ (rev_nr > 1) {
		ପାଇଁ (i = 0; i < rev_nr / 2; i++) {
			ସଂରଚନା commit_list *merge_base = ଖାଲି;
			ଯଦି (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				ଯଦି (die_on_failure) {
					die(_("failed to find exact merge base"));
				} ଅନ୍ୟଥା {
					commit_list_free(merge_base);
					ମୁକ୍ତ(rev);
					ଫେରନ୍ତୁ ଖାଲି;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		ଯଦି (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	ଯଦି (ret < 0)
		exit(128);
	ଯଦି (!ret) {
		ଯଦି (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} ଅନ୍ୟଥା {
			ମୁକ୍ତ(rev);
			ଫେରନ୍ତୁ ଖାଲି;
		}
	}

	ପାଇଁ (i = 0; i < total; i++) {
		ଯଦି (base == list[i]) {
			ଯଦି (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} ଅନ୍ୟଥା {
				ମୁକ୍ତ(rev);
				ଫେରନ୍ତୁ ଖାଲି;
			}
		}
	}

	ମୁକ୍ତ(rev);
	ଫେରନ୍ତୁ base;
}

define_commit_slab(commit_base, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

ସ୍ଥିର ଶୂନ୍ୟ prepare_bases(ସଂରଚନା base_ବୃକ୍ଷ_info *bases,
			  ସଂରଚନା commit *base,
			  ସଂରଚନା commit **list,
			  size_t total)
{
	ସଂରଚନା commit *commit;
	ସଂରଚନା rev_info revs;
	ସଂରଚନା diff_options diffopt;
	ସଂରଚନା commit_base commit_base;

	ଯଦି (!base)
		ଫେରନ୍ତୁ;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, ଖାଲି);
	revs.max_parents = 1;
	revs.topo_order = 1;
	ପାଇଁ (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	ଯଦି (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	ଯେତେବେଳେ ((commit = get_revision(&revs)) != ଖାଲି) {
		ସଂରଚନା object_id oid;
		ସଂରଚନା object_id *patch_id;
		ଯଦି (*commit_base_at(&commit_base, commit))
			ଜାରି;
		ଯଦି (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

ସ୍ଥିର ଶୂନ୍ୟ print_bases(ସଂରଚନା base_ବୃକ୍ଷ_info *bases, FILE *file)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* Only do this once, either for the cover or for the first one */
	ଯଦି (is_null_oid(&bases->base_commit))
		ଫେରନ୍ତୁ;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	ପାଇଁ (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	ମୁକ୍ତ(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *diff_title(ସଂରଚନା strbuf *sb,
			      ଅଚଳ ଅକ୍ଷର *reroll_ଗଣନା,
			      ଅଚଳ ଅକ୍ଷର *generic,
			      ଅଚଳ ଅକ୍ଷର *rerolled)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	ଯଦି (reroll_ଗଣନା && !strtol_i(reroll_ଗଣନା, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	ଅନ୍ୟଥା
		strbuf_addstr(sb, generic);
	ଫେରନ୍ତୁ sb->buf;
}

ସ୍ଥିର ଶୂନ୍ୟ infer_range_diff_ranges(ସଂରଚନା strbuf *r1,
				    ସଂରଚନା strbuf *r2,
				    ଅଚଳ ଅକ୍ଷର *prev,
				    ସଂରଚନା commit *origin,
				    ସଂରଚନା commit *head)
{
	ଅଚଳ ଅକ୍ଷର *head_oid = oid_to_hex(&head->object.oid);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_is_range = is_range_diff_range(prev);

	ଯଦି (prev_is_range)
		strbuf_addstr(r1, prev);
	ଅନ୍ୟଥା
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	ଯଦି (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	ଅନ୍ୟଥା ଯଦି (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	ଅନ୍ୟଥା {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_format_patch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା format_config cfg;
	ସଂରଚନା commit *commit;
	ସଂରଚନା commit_ଗଦା list = COMMIT_ଗଦା_INIT;
	ସଂରଚନା rev_info rev;
	ଅକ୍ଷର *to_free = ଖାଲି;
	ସଂରଚନା setup_revision_opt s_r_opt;
	size_t total, i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdout = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_ସଂଖ୍ୟା = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା just_numbers = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_if_in_upstream = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cover_letter = -1;
	ଅଚଳ ଅକ୍ଷର *cover_letter_fmt = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା boundary_ଗଣନା = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_ଦ୍ୱିମିକ_diff = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା zero_commit = 0;
	ସଂରଚନା commit *origin = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *in_reply_to = ଖାଲି;
	ସଂରଚନା patch_ids ids;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_patch_format = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ଅଚଳ ଅକ୍ଷର *reroll_ଗଣନା = ଖାଲି;
	ଅକ୍ଷର *cover_from_description_arg = ଖାଲି;
	ଅକ୍ଷର *description_file = ଖାଲି;
	ଅକ୍ଷର *branch_name = ଖାଲି;
	ସଂରଚନା base_ବୃକ୍ଷ_info bases;
	ସଂରଚନା commit *base;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress = 0;
	ସଂରଚନା progress *progress = ଖାଲି;
	ସଂରଚନା oid_array idiff_prev = OID_ARRAY_INIT;
	ସଂରଚନା strbuf idiff_title = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *rdiff_prev = ଖାଲି;
	ସଂରଚନା strbuf rdiff1 = STRBUF_INIT;
	ସଂରଚନା strbuf rdiff2 = STRBUF_INIT;
	ସଂରଚନା strbuf rdiff_title = STRBUF_INIT;
	ସଂରଚନା rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ଅଚଳ ଅକ୍ଷର *rfc = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା creation_factor = -1;
	ଅଚଳ ଅକ୍ଷର *signature = git_version_string;
	ଅକ୍ଷର *signature_to_free = ଖାଲି;
	ଅକ୍ଷର *signature_file_arg = ଖାଲି;
	ସଂରଚନା keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ଅଚଳ ଅକ୍ଷର *fmt_patch_suffix = ଖାଲି;

	ଅଚଳ ସଂରଚନା option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ଖାଲି,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ଖାଲି,
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
		OPT_INTEGER(0, "start-number", &start_ସଂଖ୍ୟା,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_ଗଣନା, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, ଖାଲି,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ଦ୍ୱିମିକ_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, ଆକାର(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, ଆକାର(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	ଯଦି (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	ଯଦି (cfg.default_attach) {
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

	ଯଦି (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	ଯଦି (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	ଯଦି (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	ଯଦି (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		ଯଦି (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		ଅନ୍ୟଥା
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	ଯଦି (reroll_ଗଣନା) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_ଗଣନା);
		rev.reroll_ଗଣନା = reroll_ଗଣନା;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	ପାଇଁ (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		strbuf_addch(&buf, '\n');
	}

	ଯଦି (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	ପାଇଁ (i = 0; i < cfg.extra_to.nr; i++) {
		ଯଦି (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		ଯଦି (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	ଯଦି (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	ପାଇଁ (i = 0; i < cfg.extra_cc.nr; i++) {
		ଯଦି (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		ଯଦି (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, ଖାଲି);

	ଯଦି (cfg.from) {
		ଯଦି (split_ident_ଧାଡ଼ି(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	ଯଦି (start_ସଂଖ୍ୟା < 0)
		start_ସଂଖ୍ୟା = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	ଯଦି (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	ଯଦି (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	ଯଦି (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	ଯଦି (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ଯଦି (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	ଯଦି (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	ଯଦି (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	ଯଦି (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	ଯଦି (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	ଯଦି (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	ଯଦି (!rev.diffopt.flags.text && !no_ଦ୍ୱିମିକ_diff)
		rev.diffopt.flags.ଦ୍ୱିମିକ = 1;

	ଯଦି (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	ଯଦି (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	ଯଦି (use_stdout) {
		setup_pager(the_repository);
	} ଅନ୍ୟଥା ଯଦି (!rev.diffopt.close_file) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved;

		ଯଦି (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		ଯଦି (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		ବାଛ (safe_create_leading_directories_const(the_repository, output_directory)) {
		ସ୍ଥିତି SCLD_OK:
		ସ୍ଥିତି SCLD_EXISTS:
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		ଯଦି (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	ଯଦି (rev.pending.nr == 1) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_head = 0;

		ଯଦି (rev.max_ଗଣନା < 0 && !rev.show_ମୂଳ_diff) {
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

		ଯଦି (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		ଯଦି (check_head) {
			ଅଚଳ ଅକ୍ଷର *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ଖାଲି, ଖାଲି);
			ଯଦି (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			ଅନ୍ୟଥା
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_ମୂଳ_diff = 1;

	ଯଦି (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		ଯଦି (rev.pending.nr == 2) {
			ସଂରଚନା object_array_entry *o = rev.pending.objects;
			ଯଦି (oideq(&o[0].item->oid, &o[1].item->oid))
				ଯାଅ done;
		}
		get_patch_ids(&rev, &ids);
	}

	ଯଦି (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	ଯେତେବେଳେ ((commit = get_revision(&rev)) != ଖାଲି) {
		ଯଦି (commit->object.flags & BOUNDARY) {
			boundary_ଗଣନା++;
			origin = (boundary_ଗଣନା == 1) ? commit : ଖାଲି;
			ଜାରି;
		}

		ଯଦି (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			ଜାରି;

		commit_ଗଦା_ଠେଲ(&list, commit);
	}
	ଯଦି (!list.nr)
		/* nothing to do */
		ଯାଅ done;
	total = list.nr;

	ଯଦି (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		ଯଦି (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} ଅନ୍ୟଥା ଯଦି (cover_letter == -1) {
		cover_letter = 1;
	}

	ଯଦି (cover_letter == -1) {
		ଯଦି (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		ଅନ୍ୟଥା ଯଦି ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		ଅନ୍ୟଥା
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	ଯଦି (!cfg.keep_subject && cfg.auto_ସଂଖ୍ୟା && (total > 1 || cover_letter))
		cfg.numbered = 1;
	ଯଦି (cfg.numbered)
		rev.total = total + start_ସଂଖ୍ୟା - 1;

	ଯଦି (idiff_prev.nr) {
		ଯଦି (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_ବୃକ୍ଷ_oid(list.ବସ୍ତୁଗୁଡ଼ିକ[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_ଗଣନା,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	ଯଦି (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	ଅନ୍ୟଥା ଯଦି (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	ଯଦି (rdiff_prev) {
		ଯଦି (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.ବସ୍ତୁଗୁଡ଼ିକ[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_ଗଣନା,
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
	ଯଦି (!signature) {
		; /* --no-signature inhibits all signatures */
	} ଅନ୍ୟଥା ଯଦି (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} ଅନ୍ୟଥା ଯଦି (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		ଯଦି (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, ଖାଲି);
	} ଅନ୍ୟଥା ଯଦି (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, ଆକାର(bases));
	base = get_base_commit(&cfg, list.ବସ୍ତୁଗୁଡ଼ିକ, list.nr);
	ଯଦି (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.ବସ୍ତୁଗୁଡ଼ିକ, list.nr);
	}

	ଯଦି (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(ଆକାର(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	ଯଦି (in_reply_to) {
		ଅକ୍ଷର *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	ଯଦି (cover_letter) {
		ଯଦି (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.ବସ୍ତୁଗୁଡ଼ିକ,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_ସଂଖ୍ୟା--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ଖାଲି;
		rev.rdiff1 = ଖାଲି;
	}
	rev.add_signoff = cfg.do_signoff;

	ଯଦି (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	ଯେତେବେଳେ (list.nr) {
		size_t idx = list.nr - 1;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା shown;

		display_progress(progress, total - idx);
		commit = commit_ଗଦା_କାଢ଼(&list);
		rev.nr = total - idx + (start_ସଂଖ୍ୟା - 1);

		/* Make the second and subsequent mails replies to the first */
		ଯଦି (cfg.thread) {
			/* Have we already had a message ID? */
			ଯଦି (rev.message_id) {
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
				ଯଦି (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					ମୁକ୍ତ(rev.message_id);
				ଅନ୍ୟଥା
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		ଯଦି (output_directory &&
		    open_next_file(rev.numbered_files ? ଖାଲି : commit, ଖାଲି, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_ବୃକ୍ଷ_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		ଯଦି (output_directory)
			rev.shown_one = 0;
		ଯଦି (shown) {
			print_bases(&bases, rev.diffopt.file);
			ଯଦି (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			ଅନ୍ୟଥା
				print_signature(signature, rev.diffopt.file);
		}
		ଯଦି (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ଖାଲି;
		}
	}
	stop_progress(&progress);
	commit_ଗଦା_clear(&list);
	ଯଦି (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	ମୁକ୍ତ(description_file);
	ମୁକ୍ତ(signature_file_arg);
	ମୁକ୍ତ(signature_to_free);
	ମୁକ୍ତ(branch_name);
	ମୁକ୍ତ(to_free);
	ମୁକ୍ତ(rev.message_id);
	ଯଦି (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	ମୁକ୍ତ(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_pending_commit(ଅଚଳ ଅକ୍ଷର *arg, ସଂରଚନା rev_info *revs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା object_id oid;
	ଯଦି (repo_get_oid(the_repository, arg, &oid) == 0) {
		ସଂରଚନା commit *commit = lookup_commit_reference(the_repository,
								&oid);
		ଯଦି (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			ଫେରନ୍ତୁ 0;
		}
	}
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ଖାଲି
};

ସ୍ଥିର ଶୂନ୍ୟ print_commit(ଅକ୍ଷର sign, ସଂରଚନା commit *commit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev, FILE *file)
{
	ଯଦି (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} ଅନ୍ୟଥା {
		ସଂରଚନା strbuf buf = STRBUF_INIT;
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
ସ୍ଥିର ଶୂନ୍ୟ collect_diff_blob_oids(ସଂରଚନା commit *commit,
				   ସଂରଚନା diff_options *opts,
				   ସଂରଚନା oidset *blobs)
{
	ସଂରଚନା diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	ପ୍ରତିଜ୍ଞା(!commit->parents || !commit->parents->next);

	ଯଦି (commit->parents)
		diff_ବୃକ୍ଷ_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	ଅନ୍ୟଥା
		diff_ମୂଳ_ବୃକ୍ଷ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < q->nr; i++) {
		ସଂରଚନା diff_filepair *p = q->queue[i];
		ସଂରଚନା userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		ଯଦି (drv && drv->ଦ୍ୱିମିକ > 0)
			ଜାରି;

		ଯଦି (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ଖାଲି,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ଭର୍ତ୍ତି(blobs, &p->one->oid);
		ଯଦି (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ଖାଲି,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ଭର୍ତ୍ତି(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା always_match(ଅଚଳ ଶୂନ୍ୟ *cmp_data UNUSED,
			ଅଚଳ ସଂରଚନା hashmap_entry *entry1 UNUSED,
			ଅଚଳ ସଂରଚନା hashmap_entry *entry2 UNUSED,
			ଅଚଳ ଶୂନ୍ୟ *keydata UNUSED)
{
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ଶୂନ୍ୟ prefetch_cherry_blobs(ସଂରଚନା repository *repo,
				  ସଂରଚନା commit_list *list,
				  ସଂରଚନା patch_ids *ids)
{
	ସଂରଚନା oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	ଯଦି (!repo_has_promisor_remote(repo))
		ଫେରନ୍ତୁ;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	ପାଇଁ (ସଂରଚନା commit_list *l = list; l; l = l->next) {
		ସଂରଚନା commit *c = l->item;
		bool match_found = false;
		ପାଇଁ (ସଂରଚନା patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		ଯଦି (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	ଯଦି (oidset_size(&blobs)) {
		ସଂରଚନା oid_array to_fetch = OID_ARRAY_INIT;
		ସଂରଚନା oidset_iter iter;
		ଅଚଳ ସଂରଚନା object_id *oid;

		oidset_iter_init(&blobs, &iter);
		ଯେତେବେଳେ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_cherry(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା rev_info revs;
	ସଂରଚନା patch_ids ids;
	ସଂରଚନା commit *commit;
	ସଂରଚନା commit_list *list = ଖାଲି;
	ସଂରଚନା branch *current_branch;
	ଅଚଳ ଅକ୍ଷର *upstream;
	ଅଚଳ ଅକ୍ଷର *head = "HEAD";
	ଅଚଳ ଅକ୍ଷର *limit = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose = 0, abbrev = 0;

	ସଂରଚନା option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	ବାଛ (argc) {
	ସ୍ଥିତି 3:
		limit = argv[2];
		/* FALLTHROUGH */
	ସ୍ଥିତି 2:
		head = argv[1];
		/* FALLTHROUGH */
	ସ୍ଥିତି 1:
		upstream = argv[0];
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		current_branch = branch_get(ଖାଲି);
		upstream = branch_get_upstream(current_branch, ଖାଲି);
		ଯଦି (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	ଯଦି (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	ଯଦି (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	ଯଦି (revs.pending.nr == 2) {
		ସଂରଚନା object_array_entry *o = revs.pending.objects;
		ଯଦି (oideq(&o[0].item->oid, &o[1].item->oid))
			ଫେରନ୍ତୁ 0;
	}

	get_patch_ids(&revs, &ids);

	ଯଦି (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	ଯଦି (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ଯେତେବେଳେ ((commit = get_revision(&revs)) != ଖାଲି) {
		commit_list_ଭର୍ତ୍ତି(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	ପାଇଁ (ସଂରଚନା commit_list *l = list; l; l = l->next) {
		ଅକ୍ଷର sign = '+';

		commit = l->item;
		ଯଦି (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	ଫେରନ୍ତୁ 0;
}
