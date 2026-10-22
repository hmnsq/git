/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/streaming.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diffcore.h"
#అన్తర్భావ "diff-merges.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "reflog-walk.h"
#అన్తర్భావ "patch-ids.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "shortlog.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "line-log.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "version.h"
#అన్తర్భావ "mailmap.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "commit-slab.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "utf8.h"

#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "range-diff.h"
#అన్తర్భావ "tmp-objdir.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "userdiff.h"
#అన్తర్భావ "write-or-die.h"

#నిర్వచించు MAIL_DEFAULT_WRAP 72
#నిర్వచించు COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#నిర్వచించు FORMAT_PATCH_NAME_MAX_DEFAULT 64

స్థిర గుర్తులేని పూర్ణాంకం force_లోపల_body_from;
స్థిర పూర్ణాంకం stdout_mboxrd;
స్థిర పూర్ణాంకం format_no_ఉపసర్గ;

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ఖాళీ
};

నిర్మాణం పంక్తి_opt_callback_దత్తాంశం {
	నిర్మాణం rev_info *rev;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	నిర్మాణం string_list వాదనలు;
};

స్థిర పూర్ణాంకం session_is_interactive(శూన్య)
{
	తిరిగి isatty(1) || pager_లోపల_use();
}

స్థిర పూర్ణాంకం auto_decoration_style(శూన్య)
{
	తిరిగి session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

స్థిర పూర్ణాంకం parse_decoration_style(స్థిరరాశి అక్షరం *విలువ)
{
	ఎంపిక (git_parse_maybe_bool(విలువ)) {
	స్థితి 1:
		తిరిగి DECORATE_SHORT_REFS;
	స్థితి 0:
		తిరిగి 0;
	అప్రమేయ:
		ఆపు;
	}
	ఒకవేళ (!strcmp(విలువ, "full"))
		తిరిగి DECORATE_నిండిన_REFS;
	లేకపోతే ఒకవేళ (!strcmp(విలువ, "short"))
		తిరిగి DECORATE_SHORT_REFS;
	లేకపోతే ఒకవేళ (!strcmp(విలువ, "auto"))
		తిరిగి auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	తిరిగి -1;
}

నిర్మాణం log_config {
	పూర్ణాంకం default_abbrev_commit;
	పూర్ణాంకం default_show_మూలం;
	పూర్ణాంకం default_follow;
	పూర్ణాంకం default_show_signature;
	పూర్ణాంకం default_encode_email_శీర్షికలు;
	పూర్ణాంకం decoration_style;
	పూర్ణాంకం decoration_given;
	పూర్ణాంకం use_mailmap_config;
	అక్షరం *fmt_patch_subject_ఉపసర్గ;
	పూర్ణాంకం fmt_patch_name_max;
	అక్షరం *fmt_pretty;
	అక్షరం *default_date_బహుళకం;

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	పూర్ణాంకం i_still_use_this;
#అయితే_ముగింపు
};

స్థిర శూన్య log_config_init(నిర్మాణం log_config *cfg)
{
	memset(cfg, 0, పరిమాణం(*cfg));
	cfg->default_show_మూలం = 1;
	cfg->default_encode_email_శీర్షికలు = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_ఉపసర్గ = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

స్థిర శూన్య log_config_release(నిర్మాణం log_config *cfg)
{
	విడుదల(cfg->default_date_బహుళకం);
	విడుదల(cfg->fmt_pretty);
	విడుదల(cfg->fmt_patch_subject_ఉపసర్గ);
}

స్థిర పూర్ణాంకం use_default_decoration_filter = 1;
స్థిర నిర్మాణం string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
స్థిర నిర్మాణం string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
స్థిర నిర్మాణం string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

స్థిర పూర్ణాంకం clear_decorations_callback(స్థిరరాశి నిర్మాణం option *opt వాడని,
				      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం decorate_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			     పూర్ణాంకం unset)
{
	నిర్మాణం log_config *cfg = opt->విలువ;

	ఒకవేళ (unset)
		cfg->decoration_style = 0;
	లేకపోతే ఒకవేళ (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	లేకపోతే
		cfg->decoration_style = DECORATE_SHORT_REFS;

	ఒకవేళ (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం log_పంక్తి_range_callback(స్థిరరాశి నిర్మాణం option *option, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం పంక్తి_opt_callback_దత్తాంశం *దత్తాంశం = option->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!arg)
		తిరిగి -1;

	దత్తాంశం->rev->పంక్తి_level_traverse = 1;
	string_list_append(&దత్తాంశం->వాదనలు, arg);

	తిరిగి 0;
}

స్థిర శూన్య cmd_log_init_defaults(నిర్మాణం rev_info *rev,
				  నిర్మాణం log_config *cfg)
{
	ఒకవేళ (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	ఒకవేళ (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_శీర్షిక = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_మూలం_diff = cfg->default_show_మూలం;
	rev->subject_ఉపసర్గ = cfg->fmt_patch_subject_ఉపసర్గ;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_శీర్షికలు = cfg->default_encode_email_శీర్షికలు;

	ఒకవేళ (cfg->default_date_బహుళకం)
		parse_date_format(cfg->default_date_బహుళకం, &rev->date_బహుళకం);
}

స్థిర శూన్య సమితి_default_decoration_filter(నిర్మాణం decoration_filter *decoration_filter)
{
	అక్షరం *విలువ = ఖాళీ;
	నిర్మాణం string_list *include = decoration_filter->include_ref_నమూనా;
	స్థిరరాశి నిర్మాణం string_list *config_exclude;

	ఒకవేళ (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		నిర్మాణం string_list_అంశం *అంశం;
		for_each_string_list_అంశం(అంశం, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_నమూనా,
					   అంశం->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	ఒకవేళ (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &విలువ) &&
	    !strcmp("all", విలువ))
		use_default_decoration_filter = 0;
	విడుదల(విలువ);

	ఒకవేళ (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_నమూనా->nr ||
	    decoration_filter->include_ref_నమూనా->nr ||
	    decoration_filter->exclude_ref_config_నమూనా->nr)
		తిరిగి;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(ref_namespace); i++) {
		ఒకవేళ (!ref_namespace[i].decoration)
			కొనసాగు;

		string_list_append(include, ref_namespace[i].ref);
	}
}

స్థిర శూన్య cmd_log_init_ముగింపు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం rev_info *rev, నిర్మాణం అమరిక_revision_opt *opt,
			 నిర్మాణం log_config *cfg)
{
	నిర్మాణం userformat_want w;
	పూర్ణాంకం quiet = 0, ఆధారం = 0, mailmap;
	స్థిర నిర్మాణం పంక్తి_opt_callback_దత్తాంశం పంక్తి_cb = {ఖాళీ, ఖాళీ, STRING_LIST_INIT_DUP};
	నిర్మాణం decoration_filter decoration_filter = {
		.exclude_ref_నమూనా = &decorate_refs_exclude,
		.include_ref_నమూనా = &decorate_refs_include,
		.exclude_ref_config_నమూనా = &decorate_refs_exclude_config,
	};
	స్థిర నిర్మాణం revision_sources revision_sources;

	స్థిరరాశి నిర్మాణం option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &ఆధారం, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#అయితే_ముగింపు
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ఖాళీ, ఖాళీ,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ఖాళీ, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ఖాళీ, &పంక్తి_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_పంక్తి_range_callback),
		OPT_చివర()
	};

	పంక్తి_cb.rev = rev;
	పంక్తి_cb.ఉపసర్గ = ఉపసర్గ;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ఒకవేళ (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = అమరిక_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	ఒకవేళ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ఒకవేళ (rev->పంక్తి_level_traverse && rev->prune_దత్తాంశం.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, పరిమాణం(w));
	userformat_కనుగొను_requirements(ఖాళీ, &w);

	ఒకవేళ (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	ఒకవేళ (rev->show_notes)
		ఎక్కించు_display_notes(&rev->notes_opt);

	ఒకవేళ ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_ముసుగు) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_శీర్షిక = 0;

	ఒకవేళ (ఆధారం || w.ఆధారం) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	ఒకవేళ (mailmap) {
		rev->mailmap = xmalloc(పరిమాణం(నిర్మాణం string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	ఒకవేళ (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		ఒకవేళ (!cfg->decoration_given)
			cfg->decoration_style = 0;
		ఒకవేళ (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	ఒకవేళ (rev->commit_format == CMIT_FMT_USERFORMAT) {
		ఒకవేళ (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} లేకపోతే ఒకవేళ (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	ఒకవేళ (cfg->decoration_style || rev->simplify_by_decoration) {
		సమితి_default_decoration_filter(&decoration_filter);

		ఒకవేళ (cfg->decoration_style)
			rev->show_decorations = 1;

		ఎక్కించు_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	ఒకవేళ (rev->పంక్తి_level_traverse)
		పంక్తి_log_init(rev, పంక్తి_cb.ఉపసర్గ, &పంక్తి_cb.వాదనలు);

	అమరిక_pager(the_repository);
}

స్థిర శూన్య cmd_log_init(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం rev_info *rev, నిర్మాణం అమరిక_revision_opt *opt,
			 నిర్మాణం log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_ముగింపు(argc, argv, ఉపసర్గ, rev, opt, cfg);
}

స్థిర పూర్ణాంకం cmd_log_walk_no_free(నిర్మాణం rev_info *rev)
{
	నిర్మాణం commit *commit;
	పూర్ణాంకం saved_nrl = 0;
	పూర్ణాంకం saved_dcctc = 0;
	పూర్ణాంకం ఫలితం;

	ఒకవేళ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	అయితే ((commit = get_revision(rev)) != ఖాళీ) {
		ఒకవేళ (!log_వృక్షం_commit(rev, commit) && rev->max_లెక్క >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_లెక్క++;
		ఒకవేళ (!rev->reflog_info && !rev->remerge_diff) {
			/*
			 * We may show a given commit multiple times when
			 * walking the reflogs. Therefore we still need it.
			 *
			 * Likewise, we potentially still need the parents
			 * of * already shown commits to determine merge
			 * bases when showing remerge diffs.
			 */
			free_commit_బఫరు(the_repository->parsed_objects,
					   commit);
			commit_list_free(commit->parents);
			commit->parents = ఖాళీ;
		}
		ఒకవేళ (saved_nrl < rev->diffopt.needed_rename_హద్దు)
			saved_nrl = rev->diffopt.needed_rename_హద్దు;
		ఒకవేళ (rev->diffopt.degraded_cc_గమ్యం_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_గమ్యం_c = saved_dcctc;
	rev->diffopt.needed_rename_హద్దు = saved_nrl;

	ఫలితం = diff_ఫలితం_సంకేతం(rev);
	ఒకవేళ (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		ఫలితం = 02;
	}
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం cmd_log_walk(నిర్మాణం rev_info *rev)
{
	పూర్ణాంకం retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	తిరిగి retval;
}

స్థిర పూర్ణాంకం git_log_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			  స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం log_config *cfg = cb;
	స్థిరరాశి అక్షరం *స్థానం_name;

	ఒకవేళ (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		తిరిగి git_config_string(&cfg->fmt_pretty, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_ఉపసర్గ);
		తిరిగి git_config_string(&cfg->fmt_patch_subject_ఉపసర్గ, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, విలువ, ctx->kvi);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_శీర్షికలు = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_బహుళకం);
		తిరిగి git_config_string(&cfg->default_date_బహుళకం, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(విలువ);
		ఒకవేళ (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "log.diffmerges")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		తిరిగి diff_merges_config(విలువ);
	}
	ఒకవేళ (!strcmp(var, "log.showroot")) {
		cfg->default_show_మూలం = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (skip_ఉపసర్గ(var, "color.decorate.", &స్థానం_name))
		తిరిగి parse_decorate_color_config(var, స్థానం_name, విలువ);
	ఒకవేళ (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	తిరిగి git_diff_ui_config(var, విలువ, ctx, cb);
}

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
పూర్ణాంకం cmd_whatchanged(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం log_config cfg;
	నిర్మాణం rev_info rev;
	నిర్మాణం అమరిక_revision_opt opt;
	పూర్ణాంకం ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, పరిమాణం(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, ఉపసర్గ, &rev, &opt, &cfg);

	ఒకవేళ (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	ఒకవేళ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	తిరిగి ret;
}
#అయితే_ముగింపు

స్థిర శూన్య show_tagger(స్థిరరాశి అక్షరం *buf, నిర్మాణం rev_info *rev)
{
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	నిర్మాణం pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_బహుళకం = rev->date_బహుళకం;
	pp_user_info(&pp, "Tagger", &బహిర్గతం, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", బహిర్గతం.buf);
	strbuf_release(&బహిర్గతం);
}

స్థిర పూర్ణాంకం show_blob_object(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం rev_info *rev, స్థిరరాశి అక్షరం *obj_name)
{
	నిర్మాణం object_id oidc;
	నిర్మాణం object_context obj_context = {0};
	అక్షరం *buf;
	గుర్తులేని దీర్ఘ కొలత;

	fflush(rev->diffopt.file);
	ఒకవేళ (!rev->diffopt.flags.textconv_సమితి_ద్వారా_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		తిరిగి odb_ప్రవాహం_blob_గమ్యం_fd(the_repository->objects, 1, oid, ఖాళీ, 0);

	ఒకవేళ (get_oid_with_context(the_repository, obj_name,
				 GET_OID_రికార్డు_మార్గం,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	ఒకవేళ (!obj_context.మార్గం ||
	    !textconv_object(the_repository, obj_context.మార్గం,
			     obj_context.బహుళకం, &oidc, 1, &buf, &కొలత)) {
		object_context_release(&obj_context);
		తిరిగి odb_ప్రవాహం_blob_గమ్యం_fd(the_repository->objects, 1, oid, ఖాళీ, 0);
	}

	ఒకవేళ (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, కొలత);
	object_context_release(&obj_context);
	విడుదల(buf);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_ట్యాగ్_object(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం rev_info *rev)
{
	కొలత_t కొలత;
	గణన object_type type;
	అక్షరం *buf = odb_read_object(the_repository->objects, oid, &type, &కొలత);
	గుర్తులేని దీర్ఘ స్థానభ్రంశం = 0;

	ఒకవేళ (!buf)
		తిరిగి error(_("could not read object %s"), oid_గమ్యం_hex(oid));

	ప్రతిజ్ఞా(type == OBJ_ట్యాగ్);
	అయితే (స్థానభ్రంశం < కొలత && buf[స్థానభ్రంశం] != '\n') {
		గుర్తులేని దీర్ఘ new_స్థానభ్రంశం = స్థానభ్రంశం + 1;
		స్థిరరాశి అక్షరం *ident;
		అయితే (new_స్థానభ్రంశం < కొలత && buf[new_స్థానభ్రంశం++] != '\n')
			; /* do nothing */
		ఒకవేళ (skip_ఉపసర్గ(buf + స్థానభ్రంశం, "tagger ", &ident))
			show_tagger(ident, rev);
		స్థానభ్రంశం = new_స్థానభ్రంశం;
	}

	ఒకవేళ (స్థానభ్రంశం < కొలత)
		fwrite(buf + స్థానభ్రంశం, కొలత - స్థానభ్రంశం, 1, rev->diffopt.file);
	విడుదల(buf);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_వృక్షం_object(స్థిరరాశి నిర్మాణం object_id *oid వాడని,
			    నిర్మాణం strbuf *పునాది వాడని,
			    స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం,
			    శూన్య *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(బహుళకం) ? "/" : "");
	తిరిగి 0;
}

స్థిర శూన్య show_అమరిక_revisions_tweak(నిర్మాణం rev_info *rev)
{
	ఒకవేళ (rev->మొదటి_మాతృ_only)
		diff_merges_default_గమ్యం_మొదటి_మాతృ(rev);
	లేకపోతే
		diff_merges_default_గమ్యం_dense_combined(rev);
	ఒకవేళ (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

పూర్ణాంకం cmd_show(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం log_config cfg;
	నిర్మాణం rev_info rev;
	గుర్తులేని పూర్ణాంకం i;
	నిర్మాణం అమరిక_revision_opt opt;
	నిర్మాణం pathspec సరిపోలిక_all;
	పూర్ణాంకం ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	ఒకవేళ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_నిండిన_సూచీ = 0;
	}

	memset(&సరిపోలిక_all, 0, పరిమాణం(సరిపోలిక_all));
	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_శీర్షిక = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_వెడల్పు = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, పరిమాణం(opt));
	opt.def = "HEAD";
	opt.tweak = show_అమరిక_revisions_tweak;
	cmd_log_init(argc, argv, ఉపసర్గ, &rev, &opt, &cfg);

	ఒకవేళ (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		తిరిగి ret;
	}

	rev.diffopt.no_free = 1;
	కొరకు (i = 0; i < rev.pending.nr && !ret; i++) {
		నిర్మాణం object *o = rev.pending.objects[i].అంశం;
		స్థిరరాశి అక్షరం *name = rev.pending.objects[i].name;
		ఎంపిక (o->type) {
		స్థితి OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			ఆపు;
		స్థితి OBJ_ట్యాగ్: {
			నిర్మాణం ట్యాగ్ *t = (నిర్మాణం ట్యాగ్ *)o;
			నిర్మాణం object_id *oid = get_tagged_oid(t);

			ఒకవేళ (rev.shown_one)
				అక్షరం_రాయి('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->ట్యాగ్,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_ట్యాగ్_object(&o->oid, &rev);
			rev.shown_one = 1;
			ఒకవేళ (ret)
				ఆపు;
			o = parse_object(the_repository, oid);
			ఒకవేళ (!o)
				ret = error(_("could not read object %s"),
					    oid_గమ్యం_hex(oid));
			rev.pending.objects[i].అంశం = o;
			i--;
			ఆపు;
		}
		స్థితి OBJ_వృక్షం:
			ఒకవేళ (rev.shown_one)
				అక్షరం_రాయి('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_వృక్షం(the_repository, (నిర్మాణం వృక్షం *)o,
				  &సరిపోలిక_all, show_వృక్షం_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			ఆపు;
		స్థితి OBJ_COMMIT:
		{
			నిర్మాణం object_array old;
			నిర్మాణం object_array blank = OBJECT_ARRAY_INIT;

			జ్ఞాపకం_నకలు(&old, &rev.pending, పరిమాణం(old));
			జ్ఞాపకం_నకలు(&rev.pending, &blank, పరిమాణం(rev.pending));

			కూడు_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			జ్ఞాపకం_నకలు(&rev.pending, &old, పరిమాణం(rev.pending));
			ఆపు;
		}
		అప్రమేయ:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	తిరిగి ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
పూర్ణాంకం cmd_log_reflog(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం log_config cfg;
	నిర్మాణం rev_info rev;
	నిర్మాణం అమరిక_revision_opt opt;
	పూర్ణాంకం ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_శీర్షిక = 1;
	memset(&opt, 0, పరిమాణం(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_శీర్షిక = 1;
	cmd_log_init_ముగింపు(argc, argv, ఉపసర్గ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	తిరిగి ret;
}

స్థిర శూన్య log_అమరిక_revisions_tweak(నిర్మాణం rev_info *rev)
{
	ఒకవేళ (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_దత్తాంశం, 0))
		rev->diffopt.flags.follow_renames = 1;

	ఒకవేళ (rev->మొదటి_మాతృ_only)
		diff_merges_default_గమ్యం_మొదటి_మాతృ(rev);
}

పూర్ణాంకం cmd_log(పూర్ణాంకం argc,
	    స్థిరరాశి అక్షరం **argv,
	    స్థిరరాశి అక్షరం *ఉపసర్గ,
	    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం log_config cfg;
	నిర్మాణం rev_info rev;
	నిర్మాణం అమరిక_revision_opt opt;
	పూర్ణాంకం ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_శీర్షిక = 1;
	memset(&opt, 0, పరిమాణం(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_అమరిక_revisions_tweak;
	cmd_log_init(argc, argv, ఉపసర్గ, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	తిరిగి ret;
}

/* format-patch */

గణన cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

గణన thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

గణన cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_సందేశం,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

గణన auto_పునాది_setting {
	AUTO_పునాది_NEVER,
	AUTO_పునాది_ALWAYS,
	AUTO_పునాది_WHEN_ABLE
};

నిర్మాణం format_config {
	నిర్మాణం log_config సంవర్గమానం;
	గణన thread_level thread;
	పూర్ణాంకం do_signoff;
	గణన auto_పునాది_setting auto_పునాది;
	అక్షరం *పునాది_commit;
	అక్షరం *from;
	అక్షరం *signature;
	అక్షరం *signature_file;
	గణన cover_setting config_cover_letter;
	అక్షరం *fmt_cover_letter_commit_list;
	అక్షరం *config_output_directory;
	గణన cover_from_description cover_from_description_బహుళకం;
	పూర్ణాంకం show_notes;
	నిర్మాణం display_notes_opt notes_opt;
	పూర్ణాంకం numbered_cmdline_opt;
	పూర్ణాంకం numbered;
	పూర్ణాంకం auto_సంఖ్య;
	అక్షరం *default_attach;
	నిర్మాణం string_list extra_hdr;
	నిర్మాణం string_list extra_గమ్యం;
	నిర్మాణం string_list extra_cc;
	పూర్ణాంకం keep_subject;
	పూర్ణాంకం subject_ఉపసర్గ;
	నిర్మాణం strbuf sprefix;
	అక్షరం *fmt_patch_suffix;
};

స్థిర శూన్య format_config_init(నిర్మాణం format_config *cfg)
{
	memset(cfg, 0, పరిమాణం(*cfg));
	log_config_init(&cfg->సంవర్గమానం);
	cfg->cover_from_description_బహుళకం = COVER_FROM_సందేశం;
	cfg->auto_సంఖ్య = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_గమ్యం);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

స్థిర శూన్య format_config_release(నిర్మాణం format_config *cfg)
{
	log_config_release(&cfg->సంవర్గమానం);
	విడుదల(cfg->పునాది_commit);
	విడుదల(cfg->from);
	విడుదల(cfg->signature);
	విడుదల(cfg->signature_file);
	విడుదల(cfg->config_output_directory);
	విడుదల(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_గమ్యం, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	విడుదల(cfg->fmt_patch_suffix);
	విడుదల(cfg->fmt_cover_letter_commit_list);
}

స్థిర గణన cover_from_description parse_cover_from_description(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (!arg || !strcmp(arg, "default"))
		తిరిగి COVER_FROM_సందేశం;
	లేకపోతే ఒకవేళ (!strcmp(arg, "none"))
		తిరిగి COVER_FROM_NONE;
	లేకపోతే ఒకవేళ (!strcmp(arg, "message"))
		తిరిగి COVER_FROM_సందేశం;
	లేకపోతే ఒకవేళ (!strcmp(arg, "subject"))
		తిరిగి COVER_FROM_SUBJECT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "auto"))
		తిరిగి COVER_FROM_AUTO;
	లేకపోతే
		die(_("%s: invalid cover from description mode"), arg);
}

స్థిర శూన్య కూడు_శీర్షిక(నిర్మాణం format_config *cfg, స్థిరరాశి అక్షరం *విలువ)
{
	నిర్మాణం string_list_అంశం *అంశం;
	పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(విలువ);
	అయితే (len && విలువ[len - 1] == '\n')
		len--;

	ఒకవేళ (!strncasecmp(విలువ, "to: ", 4)) {
		అంశం = string_list_append(&cfg->extra_గమ్యం, విలువ + 4);
		len -= 4;
	} లేకపోతే ఒకవేళ (!strncasecmp(విలువ, "cc: ", 4)) {
		అంశం = string_list_append(&cfg->extra_cc, విలువ + 4);
		len -= 4;
	} లేకపోతే {
		అంశం = string_list_append(&cfg->extra_hdr, విలువ);
	}

	అంశం->string[len] = '\0';
}

స్థిర పూర్ణాంకం git_format_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			     స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం format_config *cfg = cb;

	ఒకవేళ (!strcmp(var, "format.headers")) {
		ఒకవేళ (!విలువ)
			die(_("format.headers without value"));
		కూడు_శీర్షిక(cfg, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		తిరిగి git_config_string(&cfg->fmt_patch_suffix, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.to")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		string_list_append(&cfg->extra_గమ్యం, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.cc")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.numbered")) {
		ఒకవేళ (విలువ && !strcasecmp(విలువ, "auto")) {
			cfg->auto_సంఖ్య = 1;
			తిరిగి 0;
		}
		cfg->numbered = git_config_bool(var, విలువ);
		cfg->auto_సంఖ్య = cfg->auto_సంఖ్య && cfg->numbered;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.attach")) {
		ఒకవేళ (విలువ && *విలువ) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(విలువ);
		} లేకపోతే ఒకవేళ (విలువ && !*విలువ) {
			FREE_AND_NULL(cfg->default_attach);
		} లేకపోతే {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_సంస్కరణ_string);
		}
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.thread")) {
		ఒకవేళ (విలువ && !strcasecmp(విలువ, "deep")) {
			cfg->thread = THREAD_DEEP;
			తిరిగి 0;
		}
		ఒకవేళ (విలువ && !strcasecmp(విలువ, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			తిరిగి 0;
		}
		cfg->thread = git_config_bool(var, విలువ) ? THREAD_SHALLOW : THREAD_UNSET;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		తిరిగి git_config_string(&cfg->signature, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		తిరిగి git_config_pathname(&cfg->signature_file, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.coverletter")) {
		ఒకవేళ (విలువ && !strcasecmp(విలువ, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			తిరిగి 0;
		}
		cfg->config_cover_letter = git_config_bool(var, విలువ) ? COVER_ON : COVER_OFF;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		తిరిగి git_config_string(&cfg->fmt_cover_letter_commit_list, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		తిరిగి git_config_string(&cfg->config_output_directory, var, విలువ);
	}
	ఒకవేళ (!strcmp(var, "format.useautobase")) {
		ఒకవేళ (విలువ && !strcasecmp(విలువ, "whenAble")) {
			cfg->auto_పునాది = AUTO_పునాది_WHEN_ABLE;
			తిరిగి 0;
		}
		cfg->auto_పునాది = git_config_bool(var, విలువ) ? AUTO_పునాది_ALWAYS : AUTO_పునాది_NEVER;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.from")) {
		పూర్ణాంకం b = git_parse_maybe_bool(విలువ);
		FREE_AND_NULL(cfg->from);
		ఒకవేళ (b < 0)
			cfg->from = xstrdup(విలువ);
		లేకపోతే ఒకవేళ (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.forceinbodyfrom")) {
		force_లోపల_body_from = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.notes")) {
		పూర్ణాంకం b = git_parse_maybe_bool(విలువ);
		ఒకవేళ (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, విలువ);
		లేకపోతే ఒకవేళ (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		లేకపోతే
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_బహుళకం = parse_cover_from_description(విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "format.noprefix")) {
		format_no_ఉపసర్గ = git_parse_maybe_bool(విలువ);
		ఒకవేళ (format_no_ఉపసర్గ < 0) {
			పూర్ణాంకం స్థితి_2 = die_సందేశం(
				_("bad boolean config value '%s' for '%s'"),
				విలువ, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			నిష్క్రమించు(స్థితి_2);
		}
		తిరిగి 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	ఒకవేళ (!strcmp(var, "diff.noprefix"))
		తిరిగి 0;

	తిరిగి git_log_config(var, విలువ, ctx, &cfg->సంవర్గమానం);
}

స్థిర స్థిరరాశి అక్షరం *output_directory = ఖాళీ;
స్థిర పూర్ణాంకం outdir_స్థానభ్రంశం;

స్థిర పూర్ణాంకం open_తదుపరి_file(నిర్మాణం commit *commit, స్థిరరాశి అక్షరం *subject,
			 నిర్మాణం rev_info *rev, పూర్ణాంకం quiet)
{
	నిర్మాణం strbuf filename = STRBUF_INIT;

	ఒకవేళ (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	ఒకవేళ (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	లేకపోతే ఒకవేళ (commit)
		fmt_output_commit(&filename, commit, rev);
	లేకపోతే
		fmt_output_subject(&filename, subject, rev);

	ఒకవేళ (!quiet)
		రాయి("%s\n", filename.buf + outdir_స్థానభ్రంశం);

	ఒకవేళ (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		తిరిగి -1;
	}

	strbuf_release(&filename);
	తిరిగి 0;
}

స్థిర శూన్య get_patch_ids(నిర్మాణం rev_info *rev, నిర్మాణం patch_ids *ids)
{
	నిర్మాణం rev_info check_rev;
	నిర్మాణం commit *commit, *c1, *c2;
	నిర్మాణం object *o1, *o2;
	గుర్తులేని flags1, flags2;

	ఒకవేళ (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].అంశం;
	o2 = rev->pending.objects[1].అంశం;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	ఒకవేళ ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->ఉపసర్గ);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	కూడు_pending_object(&check_rev, o1, "o1");
	కూడు_pending_object(&check_rev, o2, "o2");
	ఒకవేళ (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	అయితే ((commit = get_revision(&check_rev)) != ఖాళీ) {
		కూడు_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, చూసిన_2 | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, చూసిన_2 | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

స్థిర శూన్య gen_సందేశం_id(నిర్మాణం rev_info *info, స్థిరరాశి అక్షరం *పునాది)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", పునాది,
		    (timestamp_t) సమయం(ఖాళీ),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->సందేశం_id = strbuf_detach(&buf, ఖాళీ);
}

స్థిర శూన్య print_signature(స్థిరరాశి అక్షరం *signature, FILE *file)
{
	ఒకవేళ (!signature || !*signature)
		తిరిగి;

	fprintf(file, "-- \n%s", signature);
	ఒకవేళ (signature[అక్షరశ్రేణి_పొడవు(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

స్థిర అక్షరం *కనుగొను_branch_name(నిర్మాణం rev_info *rev)
{
	నిర్మాణం object_id branch_oid;
	స్థిరరాశి నిర్మాణం object_id *tip_oid;
	స్థిరరాశి అక్షరం *ref, *v;
	అక్షరం *నిండిన_ref, *branch = ఖాళీ;
	పూర్ణాంకం interesting_దొరికింది = 0;
	కొలత_t idx;

	కొరకు (కొలత_t i = 0; i < rev->cmdline.nr; i++) {
		ఒకవేళ (rev->cmdline.rev[i].flags & UNINTERESTING)
			కొనసాగు;
		ఒకవేళ (interesting_దొరికింది)
			తిరిగి ఖాళీ;
		interesting_దొరికింది = 1;
		idx = i;
	}
	ఒకవేళ (!interesting_దొరికింది)
		తిరిగి ఖాళీ;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].అంశం->oid;
	ఒకవేళ (repo_dwim_ref(the_repository, ref, అక్షరశ్రేణి_పొడవు(ref), &branch_oid,
			  &నిండిన_ref, 0) &&
	    skip_ఉపసర్గ(నిండిన_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	విడుదల(నిండిన_ref);
	తిరిగి branch;
}

స్థిర శూన్య show_diffstat(నిర్మాణం rev_info *rev,
			  నిర్మాణం commit *origin, నిర్మాణం commit *తల)
{
	నిర్మాణం diff_options opts;

	జ్ఞాపకం_నకలు(&opts, &rev->diffopt, పరిమాణం(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_అమరిక_పూర్తి(&opts);

	diff_వృక్షం_oid(get_commit_వృక్షం_oid(origin),
		      get_commit_వృక్షం_oid(తల),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

స్థిర శూన్య read_desc_file(నిర్మాణం strbuf *buf, స్థిరరాశి అక్షరం *desc_file)
{
	ఒకవేళ (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

స్థిర శూన్య prepare_cover_పాఠ్యం(నిర్మాణం pretty_print_context *pp,
			       స్థిరరాశి అక్షరం *description_file,
			       స్థిరరాశి అక్షరం *branch_name,
			       నిర్మాణం strbuf *sb,
			       స్థిరరాశి అక్షరం *encoding,
			       పూర్ణాంకం need_8bit_cte,
			       స్థిరరాశి నిర్మాణం format_config *cfg)
{
	స్థిరరాశి అక్షరం *subject = "*** SUBJECT HERE ***";
	స్థిరరాశి అక్షరం *body = "*** BLURB HERE ***";
	నిర్మాణం strbuf description_sb = STRBUF_INIT;
	నిర్మాణం strbuf subject_sb = STRBUF_INIT;

	ఒకవేళ (cfg->cover_from_description_బహుళకం == COVER_FROM_NONE)
		వెళ్ళు do_pp;

	ఒకవేళ (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	లేకపోతే ఒకవేళ (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	ఒకవేళ (!description_sb.len)
		వెళ్ళు do_pp;

	ఒకవేళ (cfg->cover_from_description_బహుళకం == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_బహుళకం == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	ఒకవేళ (cfg->cover_from_description_బహుళకం == COVER_FROM_సందేశం ||
	    (cfg->cover_from_description_బహుళకం == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	లేకపోతే
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

నిర్మాణం rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	నిర్మాణం string_list notes;
};

స్థిర పూర్ణాంకం rdiff_notes_cb(స్థిరరాశి నిర్మాణం option *option,
			  స్థిరరాశి అక్షరం *arg,
			  పూర్ణాంకం unset)
{
	నిర్మాణం option opt = *option;
	నిర్మాణం rdiff_notes *rdiff_notes = option->విలువ;

	rdiff_notes->override = 1;
	opt.విలువ = &rdiff_notes->notes;
	తిరిగి parse_opt_string_list(&opt, arg, unset);
}

స్థిర పూర్ణాంకం get_notes_refs(నిర్మాణం string_list_అంశం *అంశం, శూన్య *arg)
{
	strvec_pushf(arg, "--notes=%s", అంశం->string);
	తిరిగి 0;
}

స్థిర శూన్య get_notes_వాదనలు(నిర్మాణం rdiff_notes *rdiff_notes,
			   నిర్మాణం rev_info *rev)
{
	ఒకవేళ (rdiff_notes->override) {
		ఒకవేళ (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		లేకపోతే
			strvec_నెట్టు(&rev->rdiff_log_arg, "--no-notes");
	} లేకపోతే ఒకవేళ (!rev->show_notes) {
		strvec_నెట్టు(&rev->rdiff_log_arg, "--no-notes");
	} లేకపోతే ఒకవేళ (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_నెట్టు(&rev->rdiff_log_arg, "--notes");
	} లేకపోతే {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

స్థిర శూన్య generate_shortlog_cover_letter(నిర్మాణం shortlog *సంవర్గమానం,
					   నిర్మాణం rev_info *rev,
					   నిర్మాణం commit **list,
					   పూర్ణాంకం nr)
{
	shortlog_init(సంవర్గమానం);
	సంవర్గమానం->wrap_పంక్తులు = 1;
	సంవర్గమానం->wrap = MAIL_DEFAULT_WRAP;
	సంవర్గమానం->in1 = 2;
	సంవర్గమానం->in2 = 4;
	సంవర్గమానం->file = rev->diffopt.file;
	సంవర్గమానం->groups = SHORTLOG_సమూహం_AUTHOR;
	shortlog_ముగింపు_అమరిక(సంవర్గమానం);
	కొరకు (పూర్ణాంకం i = 0; i < nr; i++)
		shortlog_కూడు_commit(సంవర్గమానం, list[i]);

	shortlog_output(సంవర్గమానం);
}

స్థిర శూన్య generate_commit_list_cover(FILE *cover_file, స్థిరరాశి అక్షరం *format,
				       నిర్మాణం commit **list, పూర్ణాంకం n)
{
	నిర్మాణం strbuf commit_పంక్తి = STRBUF_INIT;
	నిర్మాణం pretty_print_context ctx = {0};
	నిర్మాణం rev_info rev = REV_INFO_INIT;

	rev.మొత్తం = n;
	ctx.rev = &rev;
	కొరకు (పూర్ణాంకం i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_సందేశం(the_repository, list[n - i], format,
				&commit_పంక్తి, &ctx);
		fprintf(cover_file, "%s\n", commit_పంక్తి.buf);
		strbuf_reset(&commit_పంక్తి);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_పంక్తి);
}

స్థిర శూన్య make_cover_letter(నిర్మాణం rev_info *rev, పూర్ణాంకం use_separate_file,
			      నిర్మాణం commit *origin,
			      పూర్ణాంకం nr, నిర్మాణం commit **list,
			      స్థిరరాశి అక్షరం *description_file,
			      స్థిరరాశి అక్షరం *branch_name,
			      పూర్ణాంకం quiet,
			      స్థిరరాశి నిర్మాణం format_config *cfg,
			      స్థిరరాశి అక్షరం *format)
{
	స్థిరరాశి అక్షరం *from;
	నిర్మాణం shortlog సంవర్గమానం;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం i;
	స్థిరరాశి అక్షరం *encoding = "UTF-8";
	పూర్ణాంకం need_8bit_cte = 0;
	నిర్మాణం pretty_print_context pp = {0};
	నిర్మాణం commit *తల = list[0];
	అక్షరం *గమ్యం_free = ఖాళీ;

	ఒకవేళ (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	ఒకవేళ (use_separate_file &&
	    open_తదుపరి_file(ఖాళీ, rev->numbered_files ? ఖాళీ : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_శీర్షికలు(rev, తల, &pp.after_subject, &need_8bit_cte, 0);

	కొరకు (i = 0; !need_8bit_cte && i < nr; i++) {
		స్థిరరాశి అక్షరం *buf = repo_get_commit_బఫరు(the_repository,
							 list[i], ఖాళీ);
		ఒకవేళ (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_బఫరు(the_repository, list[i], buf);
	}

	ఒకవేళ (!branch_name)
		branch_name = గమ్యం_free = కనుగొను_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_బహుళకం.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_శీర్షికలు = rev->encode_email_శీర్షికలు;
	pp_user_info(&pp, ఖాళీ, &sb, from, encoding);
	prepare_cover_పాఠ్యం(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	విడుదల(గమ్యం_free);
	విడుదల(pp.after_subject);
	strbuf_release(&sb);

	ఒకవేళ (skip_ఉపసర్గ(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	లేకపోతే ఒకవేళ (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&సంవర్గమానం, rev, list, nr);
	లేకపోతే ఒకవేళ (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	లేకపోతే ఒకవేళ (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	లేకపోతే
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	ఒకవేళ (origin)
		show_diffstat(rev, origin, తల);

	ఒకవేళ (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	ఒకవేళ (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		నిర్మాణం diff_options opts;
		నిర్మాణం range_diff_options range_diff_opts = {
			.creation_గుణకం = rev->creation_గుణకం,
			.dual_color = 1,
			.max_జ్ఞాపకం = RANGE_DIFF_MAX_జ్ఞాపకం_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_అమరిక(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_అమరిక_పూర్తి(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

స్థిర అక్షరం *clean_సందేశం_id(స్థిరరాశి అక్షరం *సందేశం_id_2)
{
	అక్షరం ch;
	స్థిరరాశి అక్షరం *a, *z, *m;

	m = సందేశం_id_2;
	అయితే ((ch = *m) && (ఖాళీనా(ch) || (ch == '<')))
		m++;
	a = m;
	z = ఖాళీ;
	అయితే ((ch = *m)) {
		ఒకవేళ (!ఖాళీనా(ch) && (ch != '>'))
			z = m;
		m++;
	}
	ఒకవేళ (!z)
		die(_("insane in-reply-to: %s"), సందేశం_id_2);
	ఒకవేళ (++z == m)
		తిరిగి xstrdup(a);
	తిరిగి xmemdupz(a, z - a);
}

స్థిర స్థిరరాశి అక్షరం *సమితి_outdir(స్థిరరాశి అక్షరం *ఉపసర్గ, స్థిరరాశి అక్షరం *output_directory)
{
	ఒకవేళ (output_directory && is_absolute_మార్గం(output_directory))
		తిరిగి output_directory;

	ఒకవేళ (!ఉపసర్గ || !*ఉపసర్గ) {
		ఒకవేళ (output_directory)
			తిరిగి output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_స్థానభ్రంశం = 2;
		తిరిగి "./";
	}

	outdir_స్థానభ్రంశం = అక్షరశ్రేణి_పొడవు(ఉపసర్గ);
	ఒకవేళ (!output_directory)
		తిరిగి ఉపసర్గ;

	తిరిగి ఉపసర్గ_filename(ఉపసర్గ, output_directory);
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ఖాళీ
};

నిర్మాణం keep_callback_దత్తాంశం {
	నిర్మాణం format_config *cfg;
	నిర్మాణం rev_info *revs;
};

స్థిర పూర్ణాంకం keep_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం keep_callback_దత్తాంశం *దత్తాంశం = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	దత్తాంశం->revs->మొత్తం = -1;
	దత్తాంశం->cfg->keep_subject = 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం subject_ఉపసర్గ_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			    పూర్ణాంకం unset)
{
	నిర్మాణం format_config *cfg = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_ఉపసర్గ = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం rfc_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			పూర్ణాంకం unset)
{
	స్థిరరాశి అక్షరం **rfc = opt->విలువ;

	*rfc = opt->విలువ;
	ఒకవేళ (unset)
		*rfc = ఖాళీ;
	లేకపోతే
		*rfc = arg ? arg : "RFC";
	తిరిగి 0;
}

స్థిర పూర్ణాంకం numbered_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			     పూర్ణాంకం unset)
{
	నిర్మాణం format_config *cfg = opt->విలువ;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	ఒకవేళ (unset)
		cfg->auto_సంఖ్య =  0;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం no_numbered_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	తిరిగి numbered_callback(opt, arg, 1);
}

స్థిర పూర్ణాంకం output_directory_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			      పూర్ణాంకం unset)
{
	స్థిరరాశి అక్షరం **dir = (స్థిరరాశి అక్షరం **)opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	ఒకవేళ (*dir)
		die(_("two output directories?"));
	*dir = arg;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం thread_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం format_config *cfg = opt->విలువ;

	ఒకవేళ (unset)
		cfg->thread = THREAD_UNSET;
	లేకపోతే ఒకవేళ (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	లేకపోతే ఒకవేళ (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	లేకపోతే
		తిరిగి 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం attach_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం rev_info *rev = (నిర్మాణం rev_info *)opt->విలువ;
	ఒకవేళ (unset)
		rev->mime_boundary = ఖాళీ;
	లేకపోతే ఒకవేళ (arg)
		rev->mime_boundary = arg;
	లేకపోతే
		rev->mime_boundary = git_సంస్కరణ_string;
	rev->no_inline = unset ? 0 : 1;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం inline_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం rev_info *rev = (నిర్మాణం rev_info *)opt->విలువ;
	ఒకవేళ (unset)
		rev->mime_boundary = ఖాళీ;
	లేకపోతే ఒకవేళ (arg)
		rev->mime_boundary = arg;
	లేకపోతే
		rev->mime_boundary = git_సంస్కరణ_string;
	rev->no_inline = 0;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం శీర్షిక_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			   పూర్ణాంకం unset)
{
	నిర్మాణం format_config *cfg = opt->విలువ;

	ఒకవేళ (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_గమ్యం, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} లేకపోతే {
		కూడు_శీర్షిక(cfg, arg);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం from_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	అక్షరం **from = opt->విలువ;

	విడుదల(*from);

	ఒకవేళ (unset)
		*from = ఖాళీ;
	లేకపోతే ఒకవేళ (arg)
		*from = xstrdup(arg);
	లేకపోతే
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	తిరిగి 0;
}

స్థిర పూర్ణాంకం పునాది_callback(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం format_config *cfg = opt->విలువ;

	ఒకవేళ (unset) {
		cfg->auto_పునాది = AUTO_పునాది_NEVER;
		FREE_AND_NULL(cfg->పునాది_commit);
	} లేకపోతే ఒకవేళ (!strcmp(arg, "auto")) {
		cfg->auto_పునాది = AUTO_పునాది_ALWAYS;
		FREE_AND_NULL(cfg->పునాది_commit);
	} లేకపోతే {
		cfg->auto_పునాది = AUTO_పునాది_NEVER;
		cfg->పునాది_commit = xstrdup(arg);
	}
	తిరిగి 0;
}

నిర్మాణం పునాది_వృక్షం_info {
	నిర్మాణం object_id పునాది_commit;
	పూర్ణాంకం nr_patch_id, alloc_patch_id;
	నిర్మాణం object_id *patch_id;
};

స్థిర నిర్మాణం commit *get_పునాది_commit(స్థిరరాశి నిర్మాణం format_config *cfg,
				      నిర్మాణం commit **list,
				      కొలత_t మొత్తం)
{
	నిర్మాణం commit *పునాది = ఖాళీ;
	నిర్మాణం commit **rev;
	పూర్ణాంకం auto_select, die_on_failure, ret;
	కొలత_t i = 0, rev_nr = 0;

	ఎంపిక (cfg->auto_పునాది) {
	స్థితి AUTO_పునాది_NEVER:
		ఒకవేళ (cfg->పునాది_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} లేకపోతే {
			/* no base information is requested */
			తిరిగి ఖాళీ;
		}
		ఆపు;
	స్థితి AUTO_పునాది_ALWAYS:
	స్థితి AUTO_పునాది_WHEN_ABLE:
		ఒకవేళ (cfg->పునాది_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} లేకపోతే {
			auto_select = 1;
			die_on_failure = cfg->auto_పునాది == AUTO_పునాది_ALWAYS;
		}
		ఆపు;
	అప్రమేయ:
		BUG("unexpected automatic base selection method");
	}

	ఒకవేళ (!auto_select) {
		పునాది = lookup_commit_reference_by_name(cfg->పునాది_commit);
		ఒకవేళ (!పునాది)
			die(_("unknown commit %s"), cfg->పునాది_commit);
	} లేకపోతే {
		నిర్మాణం branch *curr_branch = branch_get(ఖాళీ);
		స్థిరరాశి అక్షరం *upstream = branch_get_upstream(curr_branch, ఖాళీ);
		ఒకవేళ (upstream) {
			నిర్మాణం commit_list *పునాది_list = ఖాళీ;
			నిర్మాణం commit *commit;
			నిర్మాణం object_id oid;

			ఒకవేళ (repo_get_oid(the_repository, upstream, &oid)) {
				ఒకవేళ (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				లేకపోతే
					తిరిగి ఖాళీ;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			ఒకవేళ (repo_get_విలీనం_bases_many(the_repository,
						      commit, మొత్తం,
						      list,
						      &పునాది_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !పునాది_list || పునాది_list->తదుపరి) {
				ఒకవేళ (die_on_failure) {
					die(_("could not find exact merge base"));
				} లేకపోతే {
					commit_list_free(పునాది_list);
					తిరిగి ఖాళీ;
				}
			}
			పునాది = పునాది_list->అంశం;
			commit_list_free(పునాది_list);
		} లేకపోతే {
			ఒకవేళ (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			లేకపోతే
				తిరిగి ఖాళీ;
		}
	}

	ALLOC_ARRAY(rev, మొత్తం);
	కొరకు (i = 0; i < మొత్తం; i++)
		rev[i] = list[i];

	rev_nr = మొత్తం;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	అయితే (rev_nr > 1) {
		కొరకు (i = 0; i < rev_nr / 2; i++) {
			నిర్మాణం commit_list *విలీనం_పునాది = ఖాళీ;
			ఒకవేళ (repo_get_విలీనం_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &విలీనం_పునాది) < 0 ||
			    !విలీనం_పునాది || విలీనం_పునాది->తదుపరి) {
				ఒకవేళ (die_on_failure) {
					die(_("failed to find exact merge base"));
				} లేకపోతే {
					commit_list_free(విలీనం_పునాది);
					విడుదల(rev);
					తిరిగి ఖాళీ;
				}
			}

			rev[i] = విలీనం_పునాది->అంశం;
			commit_list_free(విలీనం_పునాది);
		}

		ఒకవేళ (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_విడత_పైకి(rev_nr, 2);
	}

	ret = repo_లోపల_విలీనం_bases(the_repository, పునాది, rev[0]);
	ఒకవేళ (ret < 0)
		నిష్క్రమించు(128);
	ఒకవేళ (!ret) {
		ఒకవేళ (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} లేకపోతే {
			విడుదల(rev);
			తిరిగి ఖాళీ;
		}
	}

	కొరకు (i = 0; i < మొత్తం; i++) {
		ఒకవేళ (పునాది == list[i]) {
			ఒకవేళ (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} లేకపోతే {
				విడుదల(rev);
				తిరిగి ఖాళీ;
			}
		}
	}

	విడుదల(rev);
	తిరిగి పునాది;
}

define_commit_slab(commit_పునాది, పూర్ణాంకం);

స్థిర శూన్య prepare_bases(నిర్మాణం పునాది_వృక్షం_info *bases,
			  నిర్మాణం commit *పునాది,
			  నిర్మాణం commit **list,
			  కొలత_t మొత్తం)
{
	నిర్మాణం commit *commit;
	నిర్మాణం rev_info revs;
	నిర్మాణం diff_options diffopt;
	నిర్మాణం commit_పునాది commit_పునాది;

	ఒకవేళ (!పునాది)
		తిరిగి;

	init_commit_పునాది(&commit_పునాది);
	repo_diff_అమరిక(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_అమరిక_పూర్తి(&diffopt);

	oidcpy(&bases->పునాది_commit, &పునాది->object.oid);

	repo_init_revisions(the_repository, &revs, ఖాళీ);
	revs.max_parents = 1;
	revs.topo_క్రమం = 1;
	కొరకు (కొలత_t i = 0; i < మొత్తం; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		కూడు_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_పునాది_at(&commit_పునాది, list[i]) = 1;
	}
	పునాది->object.flags |= UNINTERESTING;
	కూడు_pending_object(&revs, &పునాది->object, "base");

	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	అయితే ((commit = get_revision(&revs)) != ఖాళీ) {
		నిర్మాణం object_id oid;
		నిర్మాణం object_id *patch_id;
		ఒకవేళ (*commit_పునాది_at(&commit_పునాది, commit))
			కొనసాగు;
		ఒకవేళ (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_పునాది(&commit_పునాది);
	release_revisions(&revs);
}

స్థిర శూన్య print_bases(నిర్మాణం పునాది_వృక్షం_info *bases, FILE *file)
{
	పూర్ణాంకం i;

	/* Only do this once, either for the cover or for the first one */
	ఒకవేళ (is_null_oid(&bases->పునాది_commit))
		తిరిగి;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_గమ్యం_hex(&bases->పునాది_commit));

	/* Show the prerequisite patches */
	కొరకు (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_గమ్యం_hex(&bases->patch_id[i]));

	విడుదల(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->పునాది_commit, the_repository->hash_algo);
}

స్థిర స్థిరరాశి అక్షరం *diff_title(నిర్మాణం strbuf *sb,
			      స్థిరరాశి అక్షరం *reroll_లెక్క,
			      స్థిరరాశి అక్షరం *generic,
			      స్థిరరాశి అక్షరం *rerolled)
{
	పూర్ణాంకం v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	ఒకవేళ (reroll_లెక్క && !strtol_i(reroll_లెక్క, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	లేకపోతే
		strbuf_addstr(sb, generic);
	తిరిగి sb->buf;
}

స్థిర శూన్య infer_range_diff_ranges(నిర్మాణం strbuf *r1,
				    నిర్మాణం strbuf *r2,
				    స్థిరరాశి అక్షరం *prev,
				    నిర్మాణం commit *origin,
				    నిర్మాణం commit *తల)
{
	స్థిరరాశి అక్షరం *తల_oid = oid_గమ్యం_hex(&తల->object.oid);
	పూర్ణాంకం prev_is_range = is_range_diff_range(prev);

	ఒకవేళ (prev_is_range)
		strbuf_addstr(r1, prev);
	లేకపోతే
		strbuf_addf(r1, "%s..%s", తల_oid, prev);

	ఒకవేళ (origin)
		strbuf_addf(r2, "%s..%s", oid_గమ్యం_hex(&origin->object.oid), తల_oid);
	లేకపోతే ఒకవేళ (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	లేకపోతే {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, తల_oid);
	}
}

పూర్ణాంకం cmd_format_patch(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం format_config cfg;
	నిర్మాణం commit *commit;
	నిర్మాణం commit_దొంతర list = COMMIT_దొంతర_INIT;
	నిర్మాణం rev_info rev;
	అక్షరం *గమ్యం_free = ఖాళీ;
	నిర్మాణం అమరిక_revision_opt s_r_opt;
	కొలత_t మొత్తం, i;
	పూర్ణాంకం use_stdout = 0;
	పూర్ణాంకం ప్రారంభం_సంఖ్య = -1;
	పూర్ణాంకం just_numbers = 0;
	పూర్ణాంకం ignore_if_లోపల_upstream = 0;
	పూర్ణాంకం cover_letter = -1;
	స్థిరరాశి అక్షరం *cover_letter_fmt = ఖాళీ;
	పూర్ణాంకం boundary_లెక్క = 0;
	పూర్ణాంకం no_ద్విమాన_diff = 0;
	పూర్ణాంకం zero_commit = 0;
	నిర్మాణం commit *origin = ఖాళీ;
	స్థిరరాశి అక్షరం *లోపల_జవాబు_గమ్యం = ఖాళీ;
	నిర్మాణం patch_ids ids;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	పూర్ణాంకం use_patch_format = 0;
	పూర్ణాంకం quiet = 0;
	స్థిరరాశి అక్షరం *reroll_లెక్క = ఖాళీ;
	అక్షరం *cover_from_description_arg = ఖాళీ;
	అక్షరం *description_file = ఖాళీ;
	అక్షరం *branch_name = ఖాళీ;
	నిర్మాణం పునాది_వృక్షం_info bases;
	నిర్మాణం commit *పునాది;
	పూర్ణాంకం show_పురోగతి = 0;
	నిర్మాణం పురోగతి *పురోగతి = ఖాళీ;
	నిర్మాణం oid_array idiff_prev = OID_ARRAY_INIT;
	నిర్మాణం strbuf idiff_title = STRBUF_INIT;
	స్థిరరాశి అక్షరం *rdiff_prev = ఖాళీ;
	నిర్మాణం strbuf rdiff1 = STRBUF_INIT;
	నిర్మాణం strbuf rdiff2 = STRBUF_INIT;
	నిర్మాణం strbuf rdiff_title = STRBUF_INIT;
	నిర్మాణం rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	స్థిరరాశి అక్షరం *rfc = ఖాళీ;
	పూర్ణాంకం creation_గుణకం = -1;
	స్థిరరాశి అక్షరం *signature = git_సంస్కరణ_string;
	అక్షరం *signature_గమ్యం_free = ఖాళీ;
	అక్షరం *signature_file_arg = ఖాళీ;
	నిర్మాణం keep_callback_దత్తాంశం keep_callback_దత్తాంశం = {
		.cfg = &cfg,
		.revs = &rev,
	};
	స్థిరరాశి అక్షరం *fmt_patch_suffix = ఖాళీ;

	స్థిరరాశి నిర్మాణం option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ఖాళీ,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ఖాళీ,
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
		OPT_INTEGER(0, "start-number", &ప్రారంభం_సంఖ్య,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_లెక్క, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.సంవర్గమానం.fmt_patch_name_max,
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
			    PARSE_OPT_NONEG, subject_ఉపసర్గ_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_దత్తాంశం, ఖాళీ,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_ద్విమాన_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_లోపల_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_సమితి_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_సమూహం(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), శీర్షిక_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_గమ్యం, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &లోపల_జవాబు_గమ్యం, N_("message-id"),
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
			       0, పునాది_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_పురోగతి,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_prev, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_prev, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_గుణకం,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_లోపల_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_చివర()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	జ్ఞాపకం_నకలు(&rev.notes_opt, &cfg.notes_opt, పరిమాణం(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_శీర్షికలు = cfg.సంవర్గమానం.default_encode_email_శీర్షికలు;
	rev.expand_tabs_లోపల_log_default = 0;
	rev.verbose_శీర్షిక = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, పరిమాణం(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.సంవర్గమానం.fmt_patch_subject_ఉపసర్గ);
	ఒకవేళ (format_no_ఉపసర్గ)
		diff_సమితి_noprefix(&rev.diffopt);

	ఒకవేళ (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, ఉపసర్గ, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_లోపల_body_from = force_లోపల_body_from;

	ఒకవేళ (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	ఒకవేళ (cfg.సంవర్గమానం.fmt_patch_name_max <= cast_కొలత_t_గమ్యం_int(అక్షరశ్రేణి_పొడవు("0000-") + అక్షరశ్రేణి_పొడవు(fmt_patch_suffix)))
		cfg.సంవర్గమానం.fmt_patch_name_max = అక్షరశ్రేణి_పొడవు("0000-") + అక్షరశ్రేణి_పొడవు(fmt_patch_suffix);

	ఒకవేళ (cover_from_description_arg)
		cfg.cover_from_description_బహుళకం = parse_cover_from_description(cover_from_description_arg);

	ఒకవేళ (rfc && rfc[0]) {
		cfg.subject_ఉపసర్గ = 1;
		ఒకవేళ (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		లేకపోతే
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	ఒకవేళ (reroll_లెక్క) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_లెక్క);
		rev.reroll_లెక్క = reroll_లెక్క;
	}

	rev.subject_ఉపసర్గ = cfg.sprefix.buf;

	కొరకు (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.వస్తువులు[i].string);
		strbuf_addch(&buf, '\n');
	}

	ఒకవేళ (cfg.extra_గమ్యం.nr)
		strbuf_addstr(&buf, "To: ");
	కొరకు (i = 0; i < cfg.extra_గమ్యం.nr; i++) {
		ఒకవేళ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_గమ్యం.వస్తువులు[i].string);
		ఒకవేళ (i + 1 < cfg.extra_గమ్యం.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	ఒకవేళ (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	కొరకు (i = 0; i < cfg.extra_cc.nr; i++) {
		ఒకవేళ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.వస్తువులు[i].string);
		ఒకవేళ (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_శీర్షికలు = గమ్యం_free = strbuf_detach(&buf, ఖాళీ);

	ఒకవేళ (cfg.from) {
		ఒకవేళ (విభజించు_ident_పంక్తి(&rev.from_ident, cfg.from, అక్షరశ్రేణి_పొడవు(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	ఒకవేళ (ప్రారంభం_సంఖ్య < 0)
		ప్రారంభం_సంఖ్య = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	ఒకవేళ (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	ఒకవేళ (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	ఒకవేళ (cfg.keep_subject && cfg.subject_ఉపసర్గ)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = అమరిక_revisions(argc, argv, &rev, &s_r_opt);
	ఒకవేళ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ఒకవేళ (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	ఒకవేళ (rev.diffopt.output_format & DIFF_FORMAT_NAME_స్థితి)
		die(_("--name-status does not make sense"));
	ఒకవేళ (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	ఒకవేళ (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	ఒకవేళ (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	ఒకవేళ (!rev.diffopt.stat_వెడల్పు)
		rev.diffopt.stat_వెడల్పు = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_శీర్షిక = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.సంవర్గమానం.fmt_patch_name_max;

	ఒకవేళ (!rev.diffopt.flags.పాఠ్యం && !no_ద్విమాన_diff)
		rev.diffopt.flags.ద్విమాన = 1;

	ఒకవేళ (rev.show_notes)
		ఎక్కించు_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	ఒకవేళ (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	ఒకవేళ (use_stdout) {
		అమరిక_pager(the_repository);
	} లేకపోతే ఒకవేళ (!rev.diffopt.close_file) {
		పూర్ణాంకం saved;

		ఒకవేళ (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = సమితి_outdir(ఉపసర్గ, output_directory);

		ఒకవేళ (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_సమితి_shared_repository(the_repository, 0);
		ఎంపిక (సురక్షిత_create_leading_directories_const(the_repository, output_directory)) {
		స్థితి SCLD_OK:
		స్థితి SCLD_EXISTS:
			ఆపు;
		అప్రమేయ:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_సమితి_shared_repository(the_repository, saved);
		ఒకవేళ (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	ఒకవేళ (rev.pending.nr == 1) {
		పూర్ణాంకం check_తల = 0;

		ఒకవేళ (rev.max_లెక్క < 0 && !rev.show_మూలం_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].అంశం->flags |= UNINTERESTING;
			కూడు_తల_గమ్యం_pending(&rev);
			check_తల = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		ఒకవేళ (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_తల = 1;

		ఒకవేళ (check_తల) {
			స్థిరరాశి అక్షరం *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ఖాళీ, ఖాళీ);
			ఒకవేళ (ref && skip_ఉపసర్గ(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			లేకపోతే
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_మూలం_diff = 1;

	ఒకవేళ (ignore_if_లోపల_upstream) {
		/* Don't say anything if head and upstream are the same. */
		ఒకవేళ (rev.pending.nr == 2) {
			నిర్మాణం object_array_నమోదు *o = rev.pending.objects;
			ఒకవేళ (oideq(&o[0].అంశం->oid, &o[1].అంశం->oid))
				వెళ్ళు పూర్తి;
		}
		get_patch_ids(&rev, &ids);
	}

	ఒకవేళ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	అయితే ((commit = get_revision(&rev)) != ఖాళీ) {
		ఒకవేళ (commit->object.flags & BOUNDARY) {
			boundary_లెక్క++;
			origin = (boundary_లెక్క == 1) ? commit : ఖాళీ;
			కొనసాగు;
		}

		ఒకవేళ (ignore_if_లోపల_upstream && has_commit_patch_id(commit, &ids))
			కొనసాగు;

		commit_దొంతర_నెట్టు(&list, commit);
	}
	ఒకవేళ (!list.nr)
		/* nothing to do */
		వెళ్ళు పూర్తి;
	మొత్తం = list.nr;

	ఒకవేళ (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		ఒకవేళ (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} లేకపోతే ఒకవేళ (cover_letter == -1) {
		cover_letter = 1;
	}

	ఒకవేళ (cover_letter == -1) {
		ఒకవేళ (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (మొత్తం > 1);
		లేకపోతే ఒకవేళ ((idiff_prev.nr || rdiff_prev) && (మొత్తం > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		లేకపోతే
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	ఒకవేళ (!cfg.keep_subject && cfg.auto_సంఖ్య && (మొత్తం > 1 || cover_letter))
		cfg.numbered = 1;
	ఒకవేళ (cfg.numbered)
		rev.మొత్తం = మొత్తం + ప్రారంభం_సంఖ్య - 1;

	ఒకవేళ (idiff_prev.nr) {
		ఒకవేళ (!cover_letter && మొత్తం != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_వృక్షం_oid(list.వస్తువులు[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_లెక్క,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	ఒకవేళ (creation_గుణకం < 0)
		creation_గుణకం = CREATION_గుణకం_FOR_THE_SAME_SERIES;
	లేకపోతే ఒకవేళ (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	ఒకవేళ (rdiff_prev) {
		ఒకవేళ (!cover_letter && మొత్తం != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.వస్తువులు[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_గుణకం = creation_గుణకం;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_లెక్క,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_వాదనలు(&rdiff_notes, &rev);
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
	ఒకవేళ (!signature) {
		; /* --no-signature inhibits all signatures */
	} లేకపోతే ఒకవేళ (signature && signature != git_సంస్కరణ_string) {
		; /* non-default signature already set */
	} లేకపోతే ఒకవేళ (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		స్థిరరాశి అక్షరం *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		ఒకవేళ (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_గమ్యం_free = strbuf_detach(&buf, ఖాళీ);
	} లేకపోతే ఒకవేళ (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, పరిమాణం(bases));
	పునాది = get_పునాది_commit(&cfg, list.వస్తువులు, list.nr);
	ఒకవేళ (పునాది) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, పునాది, list.వస్తువులు, list.nr);
	}

	ఒకవేళ (లోపల_జవాబు_గమ్యం || cfg.thread || cover_letter) {
		rev.ref_సందేశం_ids = xmalloc(పరిమాణం(*rev.ref_సందేశం_ids));
		string_list_init_dup(rev.ref_సందేశం_ids);
	}
	ఒకవేళ (లోపల_జవాబు_గమ్యం) {
		అక్షరం *msgid = clean_సందేశం_id(లోపల_జవాబు_గమ్యం);
		string_list_append_nodup(rev.ref_సందేశం_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	ఒకవేళ (cover_letter) {
		ఒకవేళ (cfg.thread)
			gen_సందేశం_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.వస్తువులు,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		మొత్తం++;
		ప్రారంభం_సంఖ్య--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ఖాళీ;
		rev.rdiff1 = ఖాళీ;
	}
	rev.కూడు_signoff = cfg.do_signoff;

	ఒకవేళ (show_పురోగతి)
		పురోగతి = ప్రారంభం_delayed_పురోగతి(the_repository,
						  _("Generating patches"), మొత్తం);
	అయితే (list.nr) {
		కొలత_t idx = list.nr - 1;
		పూర్ణాంకం shown;

		display_పురోగతి(పురోగతి, మొత్తం - idx);
		commit = commit_దొంతర_తీయి(&list);
		rev.nr = మొత్తం - idx + (ప్రారంభం_సంఖ్య - 1);

		/* Make the second and subsequent mails replies to the first */
		ఒకవేళ (cfg.thread) {
			/* Have we already had a message ID? */
			ఒకవేళ (rev.సందేశం_id) {
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
				ఒకవేళ (cfg.thread == THREAD_SHALLOW
				    && rev.ref_సందేశం_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					విడుదల(rev.సందేశం_id);
				లేకపోతే
					string_list_append_nodup(rev.ref_సందేశం_ids,
								 rev.సందేశం_id);
			}
			gen_సందేశం_id(&rev, oid_గమ్యం_hex(&commit->object.oid));
		}

		ఒకవేళ (output_directory &&
		    open_తదుపరి_file(rev.numbered_files ? ఖాళీ : commit, ఖాళీ, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_వృక్షం_commit(&rev, commit);
		free_commit_బఫరు(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		ఒకవేళ (output_directory)
			rev.shown_one = 0;
		ఒకవేళ (shown) {
			print_bases(&bases, rev.diffopt.file);
			ఒకవేళ (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			లేకపోతే
				print_signature(signature, rev.diffopt.file);
		}
		ఒకవేళ (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ఖాళీ;
		}
	}
	stop_పురోగతి(&పురోగతి);
	commit_దొంతర_clear(&list);
	ఒకవేళ (ignore_if_లోపల_upstream)
		free_patch_ids(&ids);

పూర్తి:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	విడుదల(description_file);
	విడుదల(signature_file_arg);
	విడుదల(signature_గమ్యం_free);
	విడుదల(branch_name);
	విడుదల(గమ్యం_free);
	విడుదల(rev.సందేశం_id);
	ఒకవేళ (rev.ref_సందేశం_ids)
		string_list_clear(rev.ref_సందేశం_ids, 0);
	విడుదల(rev.ref_సందేశం_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం కూడు_pending_commit(స్థిరరాశి అక్షరం *arg, నిర్మాణం rev_info *revs, పూర్ణాంకం flags)
{
	నిర్మాణం object_id oid;
	ఒకవేళ (repo_get_oid(the_repository, arg, &oid) == 0) {
		నిర్మాణం commit *commit = lookup_commit_reference(the_repository,
								&oid);
		ఒకవేళ (commit) {
			commit->object.flags |= flags;
			కూడు_pending_object(revs, &commit->object, arg);
			తిరిగి 0;
		}
	}
	తిరిగి -1;
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ఖాళీ
};

స్థిర శూన్య print_commit(అక్షరం sign, నిర్మాణం commit *commit, పూర్ణాంకం verbose,
			 పూర్ణాంకం abbrev, FILE *file)
{
	ఒకవేళ (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} లేకపోతే {
		నిర్మాణం strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		fprintf(file, "%c %s %s\n", sign,
		       repo_కనుగొను_unique_abbrev(the_repository, &commit->object.oid, abbrev),
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
స్థిర శూన్య collect_diff_blob_oids(నిర్మాణం commit *commit,
				   నిర్మాణం diff_options *opts,
				   నిర్మాణం oidset *blobs)
{
	నిర్మాణం diff_వరుస_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	ప్రతిజ్ఞా(!commit->parents || !commit->parents->తదుపరి);

	ఒకవేళ (commit->parents)
		diff_వృక్షం_oid(&commit->parents->అంశం->object.oid,
			      &commit->object.oid, "", opts);
	లేకపోతే
		diff_మూలం_వృక్షం_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	కొరకు (పూర్ణాంకం i = 0; i < q->nr; i++) {
		నిర్మాణం diff_filepair *p = q->వరుస[i];
		నిర్మాణం userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_కనుగొను_by_మార్గం(opts->repo->సూచీ, p->one->మార్గం);
		ఒకవేళ (drv && drv->ద్విమాన > 0)
			కొనసాగు;

		ఒకవేళ (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ఖాళీ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_చొప్పించు(blobs, &p->one->oid);
		ఒకవేళ (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ఖాళీ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_చొప్పించు(blobs, &p->two->oid);
	}
	diff_వరుస_clear(q);
}

స్థిర పూర్ణాంకం always_సరిపోలిక(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
			స్థిరరాశి నిర్మాణం hashmap_నమోదు *entry1 వాడని,
			స్థిరరాశి నిర్మాణం hashmap_నమోదు *entry2 వాడని,
			స్థిరరాశి శూన్య *keydata వాడని)
{
	తిరిగి 0;
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
స్థిర శూన్య prefetch_cherry_blobs(నిర్మాణం repository *repo,
				  నిర్మాణం commit_list *list,
				  నిర్మాణం patch_ids *ids)
{
	నిర్మాణం oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	ఒకవేళ (!repo_has_promisor_remote(repo))
		తిరిగి;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_సరిపోలిక;

	/* Find header-only collisions, gather blobs from those commits */
	కొరకు (నిర్మాణం commit_list *l = list; l; l = l->తదుపరి) {
		నిర్మాణం commit *c = l->అంశం;
		bool సరిపోలిక_దొరికింది = false;
		కొరకు (నిర్మాణం patch_id *cur = patch_id_iter_మొదటి(c, ids);
		     cur;
		     cur = patch_id_iter_తదుపరి(cur, ids)) {
			సరిపోలిక_దొరికింది = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		ఒకవేళ (సరిపోలిక_దొరికింది)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	ఒకవేళ (oidset_కొలత(&blobs)) {
		నిర్మాణం oid_array గమ్యం_fetch = OID_ARRAY_INIT;
		నిర్మాణం oidset_iter iter;
		స్థిరరాశి నిర్మాణం object_id *oid;

		oidset_iter_init(&blobs, &iter);
		అయితే ((oid = oidset_iter_తదుపరి(&iter)))
			oid_array_append(&గమ్యం_fetch, oid);

		promisor_remote_get_ప్రత్యక్ష(repo, గమ్యం_fetch.oid, గమ్యం_fetch.nr);

		oid_array_clear(&గమ్యం_fetch);
	}

	oidset_clear(&blobs);
}

పూర్ణాంకం cmd_cherry(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం patch_ids ids;
	నిర్మాణం commit *commit;
	నిర్మాణం commit_list *list = ఖాళీ;
	నిర్మాణం branch *current_branch;
	స్థిరరాశి అక్షరం *upstream;
	స్థిరరాశి అక్షరం *తల = "HEAD";
	స్థిరరాశి అక్షరం *హద్దు = ఖాళీ;
	పూర్ణాంకం verbose = 0, abbrev = 0;

	నిర్మాణం option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, cherry_usage, 0);

	ఎంపిక (argc) {
	స్థితి 3:
		హద్దు = argv[2];
		/* FALLTHROUGH */
	స్థితి 2:
		తల = argv[1];
		/* FALLTHROUGH */
	స్థితి 1:
		upstream = argv[0];
		ఆపు;
	అప్రమేయ:
		current_branch = branch_get(ఖాళీ);
		upstream = branch_get_upstream(current_branch, ఖాళీ);
		ఒకవేళ (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, ఉపసర్గ);
	revs.max_parents = 1;

	ఒకవేళ (కూడు_pending_commit(తల, &revs, 0))
		die(_("unknown commit %s"), తల);
	ఒకవేళ (కూడు_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	ఒకవేళ (revs.pending.nr == 2) {
		నిర్మాణం object_array_నమోదు *o = revs.pending.objects;
		ఒకవేళ (oideq(&o[0].అంశం->oid, &o[1].అంశం->oid))
			తిరిగి 0;
	}

	get_patch_ids(&revs, &ids);

	ఒకవేళ (హద్దు && కూడు_pending_commit(హద్దు, &revs, UNINTERESTING))
		die(_("unknown commit %s"), హద్దు);

	/* reverse the list of commits */
	ఒకవేళ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	అయితే ((commit = get_revision(&revs)) != ఖాళీ) {
		commit_list_చొప్పించు(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	కొరకు (నిర్మాణం commit_list *l = list; l; l = l->తదుపరి) {
		అక్షరం sign = '+';

		commit = l->అంశం;
		ఒకవేళ (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	తిరిగి 0;
}
