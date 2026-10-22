/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "diff-merges.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "reflog-walk.h"
#अन्तर्भाव "patch-ids.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "shortlog.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "line-log.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "version.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "utf8.h"

#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "range-diff.h"
#अन्तर्भाव "tmp-objdir.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "userdiff.h"
#अन्तर्भाव "write-or-die.h"

#परिभाषय MAIL_DEFAULT_WRAP 72
#परिभाषय COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#परिभाषय FORMAT_PATCH_NAME_MAX_DEFAULT 64

स्थिर अचिह्नित पूर्णाङ्क force_अन्तर्_body_from;
स्थिर पूर्णाङ्क stdout_mboxrd;
स्थिर पूर्णाङ्क format_no_उपसर्गः;

स्थिर ध्रुव वर्ण * ध्रुव builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	रिक्त
};

संरचना पङ्क्तिः_opt_callback_दत्त {
	संरचना rev_info *rev;
	ध्रुव वर्ण *उपसर्गः;
	संरचना string_list तर्काः;
};

स्थिर पूर्णाङ्क session_is_interactive(शून्य)
{
	प्रत्यागम isatty(1) || pager_अन्तर्_use();
}

स्थिर पूर्णाङ्क auto_decoration_style(शून्य)
{
	प्रत्यागम session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

स्थिर पूर्णाङ्क parse_decoration_style(ध्रुव वर्ण *मानम्)
{
	विकल्प (git_parse_maybe_bool(मानम्)) {
	पक्ष 1:
		प्रत्यागम DECORATE_SHORT_REFS;
	पक्ष 0:
		प्रत्यागम 0;
	स्वभाव:
		भङ्ग;
	}
	यदि (!वाक्यं_तुलय(मानम्, "full"))
		प्रत्यागम DECORATE_पूर्णम्_REFS;
	अन्यथा यदि (!वाक्यं_तुलय(मानम्, "short"))
		प्रत्यागम DECORATE_SHORT_REFS;
	अन्यथा यदि (!वाक्यं_तुलय(मानम्, "auto"))
		प्रत्यागम auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	प्रत्यागम -1;
}

संरचना log_config {
	पूर्णाङ्क default_abbrev_commit;
	पूर्णाङ्क default_show_मूलम्;
	पूर्णाङ्क default_follow;
	पूर्णाङ्क default_show_signature;
	पूर्णाङ्क default_encode_email_शीर्षकाणि;
	पूर्णाङ्क decoration_style;
	पूर्णाङ्क decoration_given;
	पूर्णाङ्क use_mailmap_config;
	वर्ण *fmt_patch_subject_उपसर्गः;
	पूर्णाङ्क fmt_patch_name_max;
	वर्ण *fmt_pretty;
	वर्ण *default_date_बहुलकम्;

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	पूर्णाङ्क i_still_use_this;
#यदि_अन्तः
};

स्थिर शून्य log_config_init(संरचना log_config *cfg)
{
	स्मृतिं_पूरय(cfg, 0, परिमाण(*cfg));
	cfg->default_show_मूलम् = 1;
	cfg->default_encode_email_शीर्षकाणि = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_उपसर्गः = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

स्थिर शून्य log_config_release(संरचना log_config *cfg)
{
	मुक्त(cfg->default_date_बहुलकम्);
	मुक्त(cfg->fmt_pretty);
	मुक्त(cfg->fmt_patch_subject_उपसर्गः);
}

स्थिर पूर्णाङ्क use_default_decoration_filter = 1;
स्थिर संरचना string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

स्थिर पूर्णाङ्क clear_decorations_callback(ध्रुव संरचना option *opt अप्रयुक्तम्,
				      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क decorate_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			     पूर्णाङ्क unset)
{
	संरचना log_config *cfg = opt->मानम्;

	यदि (unset)
		cfg->decoration_style = 0;
	अन्यथा यदि (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	अन्यथा
		cfg->decoration_style = DECORATE_SHORT_REFS;

	यदि (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क log_पङ्क्तिः_range_callback(ध्रुव संरचना option *option, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना पङ्क्तिः_opt_callback_दत्त *दत्त = option->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!arg)
		प्रत्यागम -1;

	दत्त->rev->पङ्क्तिः_level_traverse = 1;
	string_list_append(&दत्त->तर्काः, arg);

	प्रत्यागम 0;
}

स्थिर शून्य cmd_log_init_defaults(संरचना rev_info *rev,
				  संरचना log_config *cfg)
{
	यदि (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	यदि (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_शीर्षकम् = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_मूलम्_diff = cfg->default_show_मूलम्;
	rev->subject_उपसर्गः = cfg->fmt_patch_subject_उपसर्गः;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_शीर्षकाणि = cfg->default_encode_email_शीर्षकाणि;

	यदि (cfg->default_date_बहुलकम्)
		parse_date_format(cfg->default_date_बहुलकम्, &rev->date_बहुलकम्);
}

स्थिर शून्य समुच्चयः_default_decoration_filter(संरचना decoration_filter *decoration_filter)
{
	वर्ण *मानम् = रिक्त;
	संरचना string_list *include = decoration_filter->include_ref_आदर्शः;
	ध्रुव संरचना string_list *config_exclude;

	यदि (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		संरचना string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_आदर्शः,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	यदि (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &मानम्) &&
	    !वाक्यं_तुलय("all", मानम्))
		use_default_decoration_filter = 0;
	मुक्त(मानम्);

	यदि (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_आदर्शः->nr ||
	    decoration_filter->include_ref_आदर्शः->nr ||
	    decoration_filter->exclude_ref_config_आदर्शः->nr)
		प्रत्यागम;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	प्रति (आकार_t i = 0; i < ARRAY_आकार(ref_namespace); i++) {
		यदि (!ref_namespace[i].decoration)
			अनुवर्त;

		string_list_append(include, ref_namespace[i].ref);
	}
}

स्थिर शून्य cmd_log_init_finish(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना rev_info *rev, संरचना सज्जता_revision_opt *opt,
			 संरचना log_config *cfg)
{
	संरचना userformat_want w;
	पूर्णाङ्क quiet = 0, source = 0, mailmap;
	स्थिर संरचना पङ्क्तिः_opt_callback_दत्त पङ्क्तिः_cb = {रिक्त, रिक्त, STRING_LIST_INIT_DUP};
	संरचना decoration_filter decoration_filter = {
		.exclude_ref_आदर्शः = &decorate_refs_exclude,
		.include_ref_आदर्शः = &decorate_refs_include,
		.exclude_ref_config_आदर्शः = &decorate_refs_exclude_config,
	};
	स्थिर संरचना revision_sources revision_sources;

	ध्रुव संरचना option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#यदि_अन्तः
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", रिक्त, रिक्त,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, रिक्त, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', रिक्त, &पङ्क्तिः_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_पङ्क्तिः_range_callback),
		OPT_अन्तः()
	};

	पङ्क्तिः_cb.rev = rev;
	पङ्क्तिः_cb.उपसर्गः = उपसर्गः;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, उपसर्गः,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = सज्जता_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	यदि (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	यदि (rev->पङ्क्तिः_level_traverse && rev->prune_दत्त.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	स्मृतिं_पूरय(&w, 0, परिमाण(w));
	userformat_find_requirements(रिक्त, &w);

	यदि (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	यदि (rev->show_notes)
		आरोपय_display_notes(&rev->notes_opt);

	यदि ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_आवरणम्) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_शीर्षकम् = 0;

	यदि (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	यदि (mailmap) {
		rev->mailmap = xmalloc(परिमाण(संरचना string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	यदि (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		यदि (!cfg->decoration_given)
			cfg->decoration_style = 0;
		यदि (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	यदि (rev->commit_format == CMIT_FMT_USERFORMAT) {
		यदि (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} अन्यथा यदि (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	यदि (cfg->decoration_style || rev->simplify_by_decoration) {
		समुच्चयः_default_decoration_filter(&decoration_filter);

		यदि (cfg->decoration_style)
			rev->show_decorations = 1;

		आरोपय_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	यदि (rev->पङ्क्तिः_level_traverse)
		पङ्क्तिः_log_init(rev, पङ्क्तिः_cb.उपसर्गः, &पङ्क्तिः_cb.तर्काः);

	सज्जता_pager(the_repository);
}

स्थिर शून्य cmd_log_init(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना rev_info *rev, संरचना सज्जता_revision_opt *opt,
			 संरचना log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, उपसर्गः, rev, opt, cfg);
}

स्थिर पूर्णाङ्क cmd_log_walk_no_free(संरचना rev_info *rev)
{
	संरचना commit *commit;
	पूर्णाङ्क saved_nrl = 0;
	पूर्णाङ्क saved_dcctc = 0;
	पूर्णाङ्क परिणाम;

	यदि (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	यावत् ((commit = get_revision(rev)) != रिक्त) {
		यदि (!log_वृक्षः_commit(rev, commit) && rev->max_गणनम् >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_गणनम्++;
		यदि (!rev->reflog_info && !rev->remerge_diff) {
			/*
			 * We may show a given commit multiple times when
			 * walking the reflogs. Therefore we still need it.
			 *
			 * Likewise, we potentially still need the parents
			 * of * already shown commits to determine merge
			 * bases when showing remerge diffs.
			 */
			free_commit_बफर(the_repository->parsed_objects,
					   commit);
			commit_list_free(commit->parents);
			commit->parents = रिक्त;
		}
		यदि (saved_nrl < rev->diffopt.needed_rename_सीमा)
			saved_nrl = rev->diffopt.needed_rename_सीमा;
		यदि (rev->diffopt.degraded_cc_गन्तव्यम्_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_गन्तव्यम्_c = saved_dcctc;
	rev->diffopt.needed_rename_सीमा = saved_nrl;

	परिणाम = diff_परिणाम_संहिता(rev);
	यदि (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		परिणाम = 02;
	}
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क cmd_log_walk(संरचना rev_info *rev)
{
	पूर्णाङ्क retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क git_log_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना log_config *cfg = cb;
	ध्रुव वर्ण *slot_name;

	यदि (!वाक्यं_तुलय(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		प्रत्यागम git_config_string(&cfg->fmt_pretty, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_उपसर्गः);
		प्रत्यागम git_config_string(&cfg->fmt_patch_subject_उपसर्गः, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, मानम्, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_शीर्षकाणि = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_बहुलकम्);
		प्रत्यागम git_config_string(&cfg->default_date_बहुलकम्, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(मानम्);
		यदि (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "log.diffmerges")) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		प्रत्यागम diff_merges_config(मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "log.showroot")) {
		cfg->default_show_मूलम् = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (skip_उपसर्गः(var, "color.decorate.", &slot_name))
		प्रत्यागम parse_decorate_color_config(var, slot_name, मानम्);
	यदि (!वाक्यं_तुलय(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	प्रत्यागम git_diff_ui_config(var, मानम्, ctx, cb);
}

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
पूर्णाङ्क cmd_whatchanged(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	संरचना सज्जता_revision_opt opt;
	पूर्णाङ्क ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, उपसर्गः);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	स्मृतिं_पूरय(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, उपसर्गः, &rev, &opt, &cfg);

	यदि (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	यदि (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	प्रत्यागम ret;
}
#यदि_अन्तः

स्थिर शून्य show_tagger(ध्रुव वर्ण *buf, संरचना rev_info *rev)
{
	संरचना strbuf बहिर्गमनम् = STRBUF_INIT;
	संरचना pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_बहुलकम् = rev->date_बहुलकम्;
	pp_user_info(&pp, "Tagger", &बहिर्गमनम्, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", बहिर्गमनम्.buf);
	strbuf_release(&बहिर्गमनम्);
}

स्थिर पूर्णाङ्क show_blob_object(ध्रुव संरचना object_id *oid, संरचना rev_info *rev, ध्रुव वर्ण *obj_name)
{
	संरचना object_id oidc;
	संरचना object_context obj_context = {0};
	वर्ण *buf;
	अचिह्नित दीर्घ आकार;

	fflush(rev->diffopt.file);
	यदि (!rev->diffopt.flags.textconv_समुच्चयः_द्वारा_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		प्रत्यागम odb_प्रवाहः_blob_गन्तव्यम्_fd(the_repository->objects, 1, oid, रिक्त, 0);

	यदि (get_oid_with_context(the_repository, obj_name,
				 GET_OID_लेखः_मार्गः,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	यदि (!obj_context.मार्गः ||
	    !textconv_object(the_repository, obj_context.मार्गः,
			     obj_context.बहुलकम्, &oidc, 1, &buf, &आकार)) {
		object_context_release(&obj_context);
		प्रत्यागम odb_प्रवाहः_blob_गन्तव्यम्_fd(the_repository->objects, 1, oid, रिक्त, 0);
	}

	यदि (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, आकार);
	object_context_release(&obj_context);
	मुक्त(buf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_चिह्नकम्_object(ध्रुव संरचना object_id *oid, संरचना rev_info *rev)
{
	आकार_t आकार;
	गणना object_type type;
	वर्ण *buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	अचिह्नित दीर्घ विस्थापनम् = 0;

	यदि (!buf)
		प्रत्यागम error(_("could not read object %s"), oid_गन्तव्यम्_hex(oid));

	प्रतिज्ञा(type == OBJ_चिह्नकम्);
	यावत् (विस्थापनम् < आकार && buf[विस्थापनम्] != '\n') {
		अचिह्नित दीर्घ new_विस्थापनम् = विस्थापनम् + 1;
		ध्रुव वर्ण *ident;
		यावत् (new_विस्थापनम् < आकार && buf[new_विस्थापनम्++] != '\n')
			; /* do nothing */
		यदि (skip_उपसर्गः(buf + विस्थापनम्, "tagger ", &ident))
			show_tagger(ident, rev);
		विस्थापनम् = new_विस्थापनम्;
	}

	यदि (विस्थापनम् < आकार)
		fwrite(buf + विस्थापनम्, आकार - विस्थापनम्, 1, rev->diffopt.file);
	मुक्त(buf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_वृक्षः_object(ध्रुव संरचना object_id *oid अप्रयुक्तम्,
			    संरचना strbuf *आधारः अप्रयुक्तम्,
			    ध्रुव वर्ण *pathname, अचिह्नित बहुलकम्,
			    शून्य *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(बहुलकम्) ? "/" : "");
	प्रत्यागम 0;
}

स्थिर शून्य show_सज्जता_revisions_tweak(संरचना rev_info *rev)
{
	यदि (rev->first_parent_only)
		diff_merges_default_गन्तव्यम्_first_parent(rev);
	अन्यथा
		diff_merges_default_गन्तव्यम्_dense_combined(rev);
	यदि (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

पूर्णाङ्क cmd_show(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *उपसर्गः,
	     संरचना repository *repo अप्रयुक्तम्)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	अचिह्नित पूर्णाङ्क i;
	संरचना सज्जता_revision_opt opt;
	संरचना pathspec मेलनम्_all;
	पूर्णाङ्क ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;
	}

	स्मृतिं_पूरय(&मेलनम्_all, 0, परिमाण(मेलनम्_all));
	repo_init_revisions(the_repository, &rev, उपसर्गः);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_शीर्षकम् = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_विस्तारः = -1; 	/* Scale to real terminal size */

	स्मृतिं_पूरय(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	opt.tweak = show_सज्जता_revisions_tweak;
	cmd_log_init(argc, argv, उपसर्गः, &rev, &opt, &cfg);

	यदि (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		प्रत्यागम ret;
	}

	rev.diffopt.no_free = 1;
	प्रति (i = 0; i < rev.pending.nr && !ret; i++) {
		संरचना object *o = rev.pending.objects[i].item;
		ध्रुव वर्ण *name = rev.pending.objects[i].name;
		विकल्प (o->type) {
		पक्ष OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			भङ्ग;
		पक्ष OBJ_चिह्नकम्: {
			संरचना चिह्नकम् *t = (संरचना चिह्नकम् *)o;
			संरचना object_id *oid = get_tagged_oid(t);

			यदि (rev.shown_one)
				अक्षरं_लिख('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->चिह्नकम्,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_चिह्नकम्_object(&o->oid, &rev);
			rev.shown_one = 1;
			यदि (ret)
				भङ्ग;
			o = parse_object(the_repository, oid);
			यदि (!o)
				ret = error(_("could not read object %s"),
					    oid_गन्तव्यम्_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			भङ्ग;
		}
		पक्ष OBJ_वृक्षः:
			यदि (rev.shown_one)
				अक्षरं_लिख('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_वृक्षः(the_repository, (संरचना वृक्षः *)o,
				  &मेलनम्_all, show_वृक्षः_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			भङ्ग;
		पक्ष OBJ_COMMIT:
		{
			संरचना object_array old;
			संरचना object_array blank = OBJECT_ARRAY_INIT;

			स्मृतिं_प्रतिलिख(&old, &rev.pending, परिमाण(old));
			स्मृतिं_प्रतिलिख(&rev.pending, &blank, परिमाण(rev.pending));

			सङ्कलय_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			स्मृतिं_प्रतिलिख(&rev.pending, &old, परिमाण(rev.pending));
			भङ्ग;
		}
		स्वभाव:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	प्रत्यागम ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
पूर्णाङ्क cmd_log_reflog(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	संरचना सज्जता_revision_opt opt;
	पूर्णाङ्क ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, उपसर्गः);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_शीर्षकम् = 1;
	स्मृतिं_पूरय(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	cmd_log_init_defaults(&rev, &cfg);
	rev.abbrev_commit = 1;
	rev.commit_format = CMIT_FMT_ONELINE;
	rev.use_terminator = 1;
	rev.always_show_शीर्षकम् = 1;
	cmd_log_init_finish(argc, argv, उपसर्गः, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	प्रत्यागम ret;
}

स्थिर शून्य log_सज्जता_revisions_tweak(संरचना rev_info *rev)
{
	यदि (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_दत्त, 0))
		rev->diffopt.flags.follow_renames = 1;

	यदि (rev->first_parent_only)
		diff_merges_default_गन्तव्यम्_first_parent(rev);
}

पूर्णाङ्क cmd_log(पूर्णाङ्क argc,
	    ध्रुव वर्ण **argv,
	    ध्रुव वर्ण *उपसर्गः,
	    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	संरचना सज्जता_revision_opt opt;
	पूर्णाङ्क ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, उपसर्गः);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_शीर्षकम् = 1;
	स्मृतिं_पूरय(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_सज्जता_revisions_tweak;
	cmd_log_init(argc, argv, उपसर्गः, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	प्रत्यागम ret;
}

/* format-patch */

गणना cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

गणना thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

गणना cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_सन्देशः,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

गणना auto_आधारः_setting {
	AUTO_आधारः_NEVER,
	AUTO_आधारः_ALWAYS,
	AUTO_आधारः_WHEN_ABLE
};

संरचना format_config {
	संरचना log_config लघुगणकः;
	गणना thread_level thread;
	पूर्णाङ्क do_signoff;
	गणना auto_आधारः_setting auto_आधारः;
	वर्ण *आधारः_commit;
	वर्ण *from;
	वर्ण *signature;
	वर्ण *signature_file;
	गणना cover_setting config_cover_letter;
	वर्ण *fmt_cover_letter_commit_list;
	वर्ण *config_output_directory;
	गणना cover_from_description cover_from_description_बहुलकम्;
	पूर्णाङ्क show_notes;
	संरचना display_notes_opt notes_opt;
	पूर्णाङ्क numbered_cmdline_opt;
	पूर्णाङ्क numbered;
	पूर्णाङ्क auto_सङ्ख्या;
	वर्ण *default_attach;
	संरचना string_list extra_hdr;
	संरचना string_list extra_गन्तव्यम्;
	संरचना string_list extra_cc;
	पूर्णाङ्क keep_subject;
	पूर्णाङ्क subject_उपसर्गः;
	संरचना strbuf sprefix;
	वर्ण *fmt_patch_suffix;
};

स्थिर शून्य format_config_init(संरचना format_config *cfg)
{
	स्मृतिं_पूरय(cfg, 0, परिमाण(*cfg));
	log_config_init(&cfg->लघुगणकः);
	cfg->cover_from_description_बहुलकम् = COVER_FROM_सन्देशः;
	cfg->auto_सङ्ख्या = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_गन्तव्यम्);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

स्थिर शून्य format_config_release(संरचना format_config *cfg)
{
	log_config_release(&cfg->लघुगणकः);
	मुक्त(cfg->आधारः_commit);
	मुक्त(cfg->from);
	मुक्त(cfg->signature);
	मुक्त(cfg->signature_file);
	मुक्त(cfg->config_output_directory);
	मुक्त(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_गन्तव्यम्, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	मुक्त(cfg->fmt_patch_suffix);
	मुक्त(cfg->fmt_cover_letter_commit_list);
}

स्थिर गणना cover_from_description parse_cover_from_description(ध्रुव वर्ण *arg)
{
	यदि (!arg || !वाक्यं_तुलय(arg, "default"))
		प्रत्यागम COVER_FROM_सन्देशः;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "none"))
		प्रत्यागम COVER_FROM_NONE;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "message"))
		प्रत्यागम COVER_FROM_सन्देशः;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "subject"))
		प्रत्यागम COVER_FROM_SUBJECT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "auto"))
		प्रत्यागम COVER_FROM_AUTO;
	अन्यथा
		die(_("%s: invalid cover from description mode"), arg);
}

स्थिर शून्य सङ्कलय_शीर्षकम्(संरचना format_config *cfg, ध्रुव वर्ण *मानम्)
{
	संरचना string_list_item *item;
	पूर्णाङ्क len = वाक्यदैर्घ्यम्(मानम्);
	यावत् (len && मानम्[len - 1] == '\n')
		len--;

	यदि (!strncasecmp(मानम्, "to: ", 4)) {
		item = string_list_append(&cfg->extra_गन्तव्यम्, मानम् + 4);
		len -= 4;
	} अन्यथा यदि (!strncasecmp(मानम्, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, मानम् + 4);
		len -= 4;
	} अन्यथा {
		item = string_list_append(&cfg->extra_hdr, मानम्);
	}

	item->string[len] = '\0';
}

स्थिर पूर्णाङ्क git_format_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			     ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना format_config *cfg = cb;

	यदि (!वाक्यं_तुलय(var, "format.headers")) {
		यदि (!मानम्)
			die(_("format.headers without value"));
		सङ्कलय_शीर्षकम्(cfg, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		प्रत्यागम git_config_string(&cfg->fmt_patch_suffix, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.to")) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		string_list_append(&cfg->extra_गन्तव्यम्, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.cc")) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "diff.color") || !वाक्यं_तुलय(var, "color.diff") ||
	    !वाक्यं_तुलय(var, "color.ui") || !वाक्यं_तुलय(var, "diff.submodule")) {
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.numbered")) {
		यदि (मानम् && !strcasecmp(मानम्, "auto")) {
			cfg->auto_सङ्ख्या = 1;
			प्रत्यागम 0;
		}
		cfg->numbered = git_config_bool(var, मानम्);
		cfg->auto_सङ्ख्या = cfg->auto_सङ्ख्या && cfg->numbered;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.attach")) {
		यदि (मानम् && *मानम्) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(मानम्);
		} अन्यथा यदि (मानम् && !*मानम्) {
			FREE_AND_NULL(cfg->default_attach);
		} अन्यथा {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_संस्करणम्_string);
		}
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.thread")) {
		यदि (मानम् && !strcasecmp(मानम्, "deep")) {
			cfg->thread = THREAD_DEEP;
			प्रत्यागम 0;
		}
		यदि (मानम् && !strcasecmp(मानम्, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			प्रत्यागम 0;
		}
		cfg->thread = git_config_bool(var, मानम्) ? THREAD_SHALLOW : THREAD_UNSET;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		प्रत्यागम git_config_string(&cfg->signature, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		प्रत्यागम git_config_pathname(&cfg->signature_file, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.coverletter")) {
		यदि (मानम् && !strcasecmp(मानम्, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			प्रत्यागम 0;
		}
		cfg->config_cover_letter = git_config_bool(var, मानम्) ? COVER_ON : COVER_OFF;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		प्रत्यागम git_config_string(&cfg->fmt_cover_letter_commit_list, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		प्रत्यागम git_config_string(&cfg->config_output_directory, var, मानम्);
	}
	यदि (!वाक्यं_तुलय(var, "format.useautobase")) {
		यदि (मानम् && !strcasecmp(मानम्, "whenAble")) {
			cfg->auto_आधारः = AUTO_आधारः_WHEN_ABLE;
			प्रत्यागम 0;
		}
		cfg->auto_आधारः = git_config_bool(var, मानम्) ? AUTO_आधारः_ALWAYS : AUTO_आधारः_NEVER;
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.from")) {
		पूर्णाङ्क b = git_parse_maybe_bool(मानम्);
		FREE_AND_NULL(cfg->from);
		यदि (b < 0)
			cfg->from = xstrdup(मानम्);
		अन्यथा यदि (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.forceinbodyfrom")) {
		force_अन्तर्_body_from = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.notes")) {
		पूर्णाङ्क b = git_parse_maybe_bool(मानम्);
		यदि (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, मानम्);
		अन्यथा यदि (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		अन्यथा
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.coverfromdescription")) {
		cfg->cover_from_description_बहुलकम् = parse_cover_from_description(मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(var, "format.noprefix")) {
		format_no_उपसर्गः = git_parse_maybe_bool(मानम्);
		यदि (format_no_उपसर्गः < 0) {
			पूर्णाङ्क स्थितिः = die_सन्देशः(
				_("bad boolean config value '%s' for '%s'"),
				मानम्, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			निर्गच्छ(स्थितिः);
		}
		प्रत्यागम 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	यदि (!वाक्यं_तुलय(var, "diff.noprefix"))
		प्रत्यागम 0;

	प्रत्यागम git_log_config(var, मानम्, ctx, &cfg->लघुगणकः);
}

स्थिर ध्रुव वर्ण *output_directory = रिक्त;
स्थिर पूर्णाङ्क outdir_विस्थापनम्;

स्थिर पूर्णाङ्क open_अग्र_file(संरचना commit *commit, ध्रुव वर्ण *subject,
			 संरचना rev_info *rev, पूर्णाङ्क quiet)
{
	संरचना strbuf filename = STRBUF_INIT;

	यदि (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	यदि (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	अन्यथा यदि (commit)
		fmt_output_commit(&filename, commit, rev);
	अन्यथा
		fmt_output_subject(&filename, subject, rev);

	यदि (!quiet)
		प्रारूप_लिख("%s\n", filename.buf + outdir_विस्थापनम्);

	यदि (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		प्रत्यागम -1;
	}

	strbuf_release(&filename);
	प्रत्यागम 0;
}

स्थिर शून्य get_patch_ids(संरचना rev_info *rev, संरचना patch_ids *ids)
{
	संरचना rev_info check_rev;
	संरचना commit *commit, *c1, *c2;
	संरचना object *o1, *o2;
	अचिह्नित flags1, flags2;

	यदि (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	यदि ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->उपसर्गः);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	सङ्कलय_pending_object(&check_rev, o1, "o1");
	सङ्कलय_pending_object(&check_rev, o2, "o2");
	यदि (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	यावत् ((commit = get_revision(&check_rev)) != रिक्त) {
		सङ्कलय_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, आलोकितम्_2 | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, आलोकितम्_2 | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

स्थिर शून्य gen_सन्देशः_id(संरचना rev_info *info, ध्रुव वर्ण *आधारः)
{
	संरचना strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", आधारः,
		    (timestamp_t) time(रिक्त),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->सन्देशः_id = strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य print_signature(ध्रुव वर्ण *signature, FILE *file)
{
	यदि (!signature || !*signature)
		प्रत्यागम;

	fprintf(file, "-- \n%s", signature);
	यदि (signature[वाक्यदैर्घ्यम्(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

स्थिर वर्ण *find_branch_name(संरचना rev_info *rev)
{
	संरचना object_id branch_oid;
	ध्रुव संरचना object_id *tip_oid;
	ध्रुव वर्ण *ref, *v;
	वर्ण *पूर्णम्_ref, *branch = रिक्त;
	पूर्णाङ्क interesting_प्राप्तम् = 0;
	आकार_t idx;

	प्रति (आकार_t i = 0; i < rev->cmdline.nr; i++) {
		यदि (rev->cmdline.rev[i].flags & UNINTERESTING)
			अनुवर्त;
		यदि (interesting_प्राप्तम्)
			प्रत्यागम रिक्त;
		interesting_प्राप्तम् = 1;
		idx = i;
	}
	यदि (!interesting_प्राप्तम्)
		प्रत्यागम रिक्त;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	यदि (repo_dwim_ref(the_repository, ref, वाक्यदैर्घ्यम्(ref), &branch_oid,
			  &पूर्णम्_ref, 0) &&
	    skip_उपसर्गः(पूर्णम्_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	मुक्त(पूर्णम्_ref);
	प्रत्यागम branch;
}

स्थिर शून्य show_diffstat(संरचना rev_info *rev,
			  संरचना commit *origin, संरचना commit *शीर्ष)
{
	संरचना diff_options opts;

	स्मृतिं_प्रतिलिख(&opts, &rev->diffopt, परिमाण(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_सज्जता_समाप्तम्(&opts);

	diff_वृक्षः_oid(get_commit_वृक्षः_oid(origin),
		      get_commit_वृक्षः_oid(शीर्ष),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

स्थिर शून्य read_desc_file(संरचना strbuf *buf, ध्रुव वर्ण *desc_file)
{
	यदि (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

स्थिर शून्य prepare_cover_text(संरचना pretty_print_context *pp,
			       ध्रुव वर्ण *description_file,
			       ध्रुव वर्ण *branch_name,
			       संरचना strbuf *sb,
			       ध्रुव वर्ण *encoding,
			       पूर्णाङ्क need_8bit_cte,
			       ध्रुव संरचना format_config *cfg)
{
	ध्रुव वर्ण *subject = "*** SUBJECT HERE ***";
	ध्रुव वर्ण *body = "*** BLURB HERE ***";
	संरचना strbuf description_sb = STRBUF_INIT;
	संरचना strbuf subject_sb = STRBUF_INIT;

	यदि (cfg->cover_from_description_बहुलकम् == COVER_FROM_NONE)
		गच्छ do_pp;

	यदि (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	अन्यथा यदि (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	यदि (!description_sb.len)
		गच्छ do_pp;

	यदि (cfg->cover_from_description_बहुलकम् == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_बहुलकम् == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	यदि (cfg->cover_from_description_बहुलकम् == COVER_FROM_सन्देशः ||
	    (cfg->cover_from_description_बहुलकम् == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	अन्यथा
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

संरचना rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	संरचना string_list notes;
};

स्थिर पूर्णाङ्क rdiff_notes_cb(ध्रुव संरचना option *option,
			  ध्रुव वर्ण *arg,
			  पूर्णाङ्क unset)
{
	संरचना option opt = *option;
	संरचना rdiff_notes *rdiff_notes = option->मानम्;

	rdiff_notes->override = 1;
	opt.मानम् = &rdiff_notes->notes;
	प्रत्यागम parse_opt_string_list(&opt, arg, unset);
}

स्थिर पूर्णाङ्क get_notes_refs(संरचना string_list_item *item, शून्य *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	प्रत्यागम 0;
}

स्थिर शून्य get_notes_तर्काः(संरचना rdiff_notes *rdiff_notes,
			   संरचना rev_info *rev)
{
	यदि (rdiff_notes->override) {
		यदि (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		अन्यथा
			strvec_प्रक्षिप(&rev->rdiff_log_arg, "--no-notes");
	} अन्यथा यदि (!rev->show_notes) {
		strvec_प्रक्षिप(&rev->rdiff_log_arg, "--no-notes");
	} अन्यथा यदि (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_प्रक्षिप(&rev->rdiff_log_arg, "--notes");
	} अन्यथा {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

स्थिर शून्य generate_shortlog_cover_letter(संरचना shortlog *लघुगणकः,
					   संरचना rev_info *rev,
					   संरचना commit **list,
					   पूर्णाङ्क nr)
{
	shortlog_init(लघुगणकः);
	लघुगणकः->wrap_पङ्क्तयः = 1;
	लघुगणकः->wrap = MAIL_DEFAULT_WRAP;
	लघुगणकः->in1 = 2;
	लघुगणकः->in2 = 4;
	लघुगणकः->file = rev->diffopt.file;
	लघुगणकः->groups = SHORTLOG_गणः_AUTHOR;
	shortlog_finish_सज्जता(लघुगणकः);
	प्रति (पूर्णाङ्क i = 0; i < nr; i++)
		shortlog_सङ्कलय_commit(लघुगणकः, list[i]);

	shortlog_output(लघुगणकः);
}

स्थिर शून्य generate_commit_list_cover(FILE *cover_file, ध्रुव वर्ण *format,
				       संरचना commit **list, पूर्णाङ्क n)
{
	संरचना strbuf commit_पङ्क्तिः = STRBUF_INIT;
	संरचना pretty_print_context ctx = {0};
	संरचना rev_info rev = REV_INFO_INIT;

	rev.योग = n;
	ctx.rev = &rev;
	प्रति (पूर्णाङ्क i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_सन्देशः(the_repository, list[n - i], format,
				&commit_पङ्क्तिः, &ctx);
		fprintf(cover_file, "%s\n", commit_पङ्क्तिः.buf);
		strbuf_reset(&commit_पङ्क्तिः);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_पङ्क्तिः);
}

स्थिर शून्य make_cover_letter(संरचना rev_info *rev, पूर्णाङ्क use_separate_file,
			      संरचना commit *origin,
			      पूर्णाङ्क nr, संरचना commit **list,
			      ध्रुव वर्ण *description_file,
			      ध्रुव वर्ण *branch_name,
			      पूर्णाङ्क quiet,
			      ध्रुव संरचना format_config *cfg,
			      ध्रुव वर्ण *format)
{
	ध्रुव वर्ण *from;
	संरचना shortlog लघुगणकः;
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क i;
	ध्रुव वर्ण *encoding = "UTF-8";
	पूर्णाङ्क need_8bit_cte = 0;
	संरचना pretty_print_context pp = {0};
	संरचना commit *शीर्ष = list[0];
	वर्ण *गन्तव्यम्_free = रिक्त;

	यदि (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	यदि (use_separate_file &&
	    open_अग्र_file(रिक्त, rev->numbered_files ? रिक्त : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_शीर्षकाणि(rev, शीर्ष, &pp.after_subject, &need_8bit_cte, 0);

	प्रति (i = 0; !need_8bit_cte && i < nr; i++) {
		ध्रुव वर्ण *buf = repo_get_commit_बफर(the_repository,
							 list[i], रिक्त);
		यदि (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_बफर(the_repository, list[i], buf);
	}

	यदि (!branch_name)
		branch_name = गन्तव्यम्_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_बहुलकम्.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_शीर्षकाणि = rev->encode_email_शीर्षकाणि;
	pp_user_info(&pp, रिक्त, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	मुक्त(गन्तव्यम्_free);
	मुक्त(pp.after_subject);
	strbuf_release(&sb);

	यदि (skip_उपसर्गः(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	अन्यथा यदि (!वाक्यं_तुलय(format, "shortlog"))
		generate_shortlog_cover_letter(&लघुगणकः, rev, list, nr);
	अन्यथा यदि (!वाक्यं_तुलय(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	अन्यथा यदि (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	अन्यथा
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	यदि (origin)
		show_diffstat(rev, origin, शीर्ष);

	यदि (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	यदि (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		संरचना diff_options opts;
		संरचना range_diff_options range_diff_opts = {
			.creation_गुणकः = rev->creation_गुणकः,
			.dual_color = 1,
			.max_स्मृतिः = RANGE_DIFF_MAX_स्मृतिः_DEFAULT,
			.diffopt = &opts,
			.log_arg = &rev->rdiff_log_arg
		};

		repo_diff_सज्जता(the_repository, &opts);
		opts.file = rev->diffopt.file;
		opts.use_color = rev->diffopt.use_color;
		diff_सज्जता_समाप्तम्(&opts);
		fprintf_ln(rev->diffopt.file, "%s", rev->rdiff_title);
		show_range_diff(rev->rdiff1, rev->rdiff2, &range_diff_opts);
	}
}

स्थिर वर्ण *clean_सन्देशः_id(ध्रुव वर्ण *सन्देशः_id_2)
{
	वर्ण ch;
	ध्रुव वर्ण *a, *z, *m;

	m = सन्देशः_id_2;
	यावत् ((ch = *m) && (रिक्तम्_वा(ch) || (ch == '<')))
		m++;
	a = m;
	z = रिक्त;
	यावत् ((ch = *m)) {
		यदि (!रिक्तम्_वा(ch) && (ch != '>'))
			z = m;
		m++;
	}
	यदि (!z)
		die(_("insane in-reply-to: %s"), सन्देशः_id_2);
	यदि (++z == m)
		प्रत्यागम xstrdup(a);
	प्रत्यागम xmemdupz(a, z - a);
}

स्थिर ध्रुव वर्ण *समुच्चयः_outdir(ध्रुव वर्ण *उपसर्गः, ध्रुव वर्ण *output_directory)
{
	यदि (output_directory && is_absolute_मार्गः(output_directory))
		प्रत्यागम output_directory;

	यदि (!उपसर्गः || !*उपसर्गः) {
		यदि (output_directory)
			प्रत्यागम output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_विस्थापनम् = 2;
		प्रत्यागम "./";
	}

	outdir_विस्थापनम् = वाक्यदैर्घ्यम्(उपसर्गः);
	यदि (!output_directory)
		प्रत्यागम उपसर्गः;

	प्रत्यागम उपसर्गः_filename(उपसर्गः, output_directory);
}

स्थिर ध्रुव वर्ण * ध्रुव builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	रिक्त
};

संरचना keep_callback_दत्त {
	संरचना format_config *cfg;
	संरचना rev_info *revs;
};

स्थिर पूर्णाङ्क keep_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना keep_callback_दत्त *दत्त = opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	दत्त->revs->योग = -1;
	दत्त->cfg->keep_subject = 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क subject_उपसर्गः_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			    पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_उपसर्गः = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rfc_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			पूर्णाङ्क unset)
{
	ध्रुव वर्ण **rfc = opt->मानम्;

	*rfc = opt->मानम्;
	यदि (unset)
		*rfc = रिक्त;
	अन्यथा
		*rfc = arg ? arg : "RFC";
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क numbered_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			     पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मानम्;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	यदि (unset)
		cfg->auto_सङ्ख्या =  0;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क no_numbered_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	प्रत्यागम numbered_callback(opt, arg, 1);
}

स्थिर पूर्णाङ्क output_directory_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			      पूर्णाङ्क unset)
{
	ध्रुव वर्ण **dir = (ध्रुव वर्ण **)opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	यदि (*dir)
		die(_("two output directories?"));
	*dir = arg;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क thread_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मानम्;

	यदि (unset)
		cfg->thread = THREAD_UNSET;
	अन्यथा यदि (!arg || !वाक्यं_तुलय(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	अन्यथा
		प्रत्यागम 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क attach_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rev_info *rev = (संरचना rev_info *)opt->मानम्;
	यदि (unset)
		rev->mime_boundary = रिक्त;
	अन्यथा यदि (arg)
		rev->mime_boundary = arg;
	अन्यथा
		rev->mime_boundary = git_संस्करणम्_string;
	rev->no_inline = unset ? 0 : 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क inline_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rev_info *rev = (संरचना rev_info *)opt->मानम्;
	यदि (unset)
		rev->mime_boundary = रिक्त;
	अन्यथा यदि (arg)
		rev->mime_boundary = arg;
	अन्यथा
		rev->mime_boundary = git_संस्करणम्_string;
	rev->no_inline = 0;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क शीर्षकम्_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			   पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मानम्;

	यदि (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_गन्तव्यम्, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} अन्यथा {
		सङ्कलय_शीर्षकम्(cfg, arg);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क from_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	वर्ण **from = opt->मानम्;

	मुक्त(*from);

	यदि (unset)
		*from = रिक्त;
	अन्यथा यदि (arg)
		*from = xstrdup(arg);
	अन्यथा
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क आधारः_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मानम्;

	यदि (unset) {
		cfg->auto_आधारः = AUTO_आधारः_NEVER;
		FREE_AND_NULL(cfg->आधारः_commit);
	} अन्यथा यदि (!वाक्यं_तुलय(arg, "auto")) {
		cfg->auto_आधारः = AUTO_आधारः_ALWAYS;
		FREE_AND_NULL(cfg->आधारः_commit);
	} अन्यथा {
		cfg->auto_आधारः = AUTO_आधारः_NEVER;
		cfg->आधारः_commit = xstrdup(arg);
	}
	प्रत्यागम 0;
}

संरचना आधारः_वृक्षः_info {
	संरचना object_id आधारः_commit;
	पूर्णाङ्क nr_patch_id, alloc_patch_id;
	संरचना object_id *patch_id;
};

स्थिर संरचना commit *get_आधारः_commit(ध्रुव संरचना format_config *cfg,
				      संरचना commit **list,
				      आकार_t योग)
{
	संरचना commit *आधारः = रिक्त;
	संरचना commit **rev;
	पूर्णाङ्क auto_select, die_on_failure, ret;
	आकार_t i = 0, rev_nr = 0;

	विकल्प (cfg->auto_आधारः) {
	पक्ष AUTO_आधारः_NEVER:
		यदि (cfg->आधारः_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} अन्यथा {
			/* no base information is requested */
			प्रत्यागम रिक्त;
		}
		भङ्ग;
	पक्ष AUTO_आधारः_ALWAYS:
	पक्ष AUTO_आधारः_WHEN_ABLE:
		यदि (cfg->आधारः_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} अन्यथा {
			auto_select = 1;
			die_on_failure = cfg->auto_आधारः == AUTO_आधारः_ALWAYS;
		}
		भङ्ग;
	स्वभाव:
		BUG("unexpected automatic base selection method");
	}

	यदि (!auto_select) {
		आधारः = lookup_commit_reference_by_name(cfg->आधारः_commit);
		यदि (!आधारः)
			die(_("unknown commit %s"), cfg->आधारः_commit);
	} अन्यथा {
		संरचना branch *curr_branch = branch_get(रिक्त);
		ध्रुव वर्ण *upstream = branch_get_upstream(curr_branch, रिक्त);
		यदि (upstream) {
			संरचना commit_list *आधारः_list = रिक्त;
			संरचना commit *commit;
			संरचना object_id oid;

			यदि (repo_get_oid(the_repository, upstream, &oid)) {
				यदि (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				अन्यथा
					प्रत्यागम रिक्त;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			यदि (repo_get_merge_bases_many(the_repository,
						      commit, योग,
						      list,
						      &आधारः_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !आधारः_list || आधारः_list->अग्र) {
				यदि (die_on_failure) {
					die(_("could not find exact merge base"));
				} अन्यथा {
					commit_list_free(आधारः_list);
					प्रत्यागम रिक्त;
				}
			}
			आधारः = आधारः_list->item;
			commit_list_free(आधारः_list);
		} अन्यथा {
			यदि (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			अन्यथा
				प्रत्यागम रिक्त;
		}
	}

	ALLOC_ARRAY(rev, योग);
	प्रति (i = 0; i < योग; i++)
		rev[i] = list[i];

	rev_nr = योग;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	यावत् (rev_nr > 1) {
		प्रति (i = 0; i < rev_nr / 2; i++) {
			संरचना commit_list *merge_आधारः = रिक्त;
			यदि (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_आधारः) < 0 ||
			    !merge_आधारः || merge_आधारः->अग्र) {
				यदि (die_on_failure) {
					die(_("failed to find exact merge base"));
				} अन्यथा {
					commit_list_free(merge_आधारः);
					मुक्त(rev);
					प्रत्यागम रिक्त;
				}
			}

			rev[i] = merge_आधारः->item;
			commit_list_free(merge_आधारः);
		}

		यदि (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_आवर्तनम्_ऊर्ध्वम्(rev_nr, 2);
	}

	ret = repo_अन्तर्_merge_bases(the_repository, आधारः, rev[0]);
	यदि (ret < 0)
		निर्गच्छ(128);
	यदि (!ret) {
		यदि (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} अन्यथा {
			मुक्त(rev);
			प्रत्यागम रिक्त;
		}
	}

	प्रति (i = 0; i < योग; i++) {
		यदि (आधारः == list[i]) {
			यदि (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} अन्यथा {
				मुक्त(rev);
				प्रत्यागम रिक्त;
			}
		}
	}

	मुक्त(rev);
	प्रत्यागम आधारः;
}

define_commit_slab(commit_आधारः, पूर्णाङ्क);

स्थिर शून्य prepare_bases(संरचना आधारः_वृक्षः_info *bases,
			  संरचना commit *आधारः,
			  संरचना commit **list,
			  आकार_t योग)
{
	संरचना commit *commit;
	संरचना rev_info revs;
	संरचना diff_options diffopt;
	संरचना commit_आधारः commit_आधारः;

	यदि (!आधारः)
		प्रत्यागम;

	init_commit_आधारः(&commit_आधारः);
	repo_diff_सज्जता(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_सज्जता_समाप्तम्(&diffopt);

	oidcpy(&bases->आधारः_commit, &आधारः->object.oid);

	repo_init_revisions(the_repository, &revs, रिक्त);
	revs.max_parents = 1;
	revs.topo_क्रमः = 1;
	प्रति (आकार_t i = 0; i < योग; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		सङ्कलय_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_आधारः_at(&commit_आधारः, list[i]) = 1;
	}
	आधारः->object.flags |= UNINTERESTING;
	सङ्कलय_pending_object(&revs, &आधारः->object, "base");

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	यावत् ((commit = get_revision(&revs)) != रिक्त) {
		संरचना object_id oid;
		संरचना object_id *patch_id;
		यदि (*commit_आधारः_at(&commit_आधारः, commit))
			अनुवर्त;
		यदि (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_आधारः(&commit_आधारः);
	release_revisions(&revs);
}

स्थिर शून्य print_bases(संरचना आधारः_वृक्षः_info *bases, FILE *file)
{
	पूर्णाङ्क i;

	/* Only do this once, either for the cover or for the first one */
	यदि (is_null_oid(&bases->आधारः_commit))
		प्रत्यागम;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_गन्तव्यम्_hex(&bases->आधारः_commit));

	/* Show the prerequisite patches */
	प्रति (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_गन्तव्यम्_hex(&bases->patch_id[i]));

	मुक्त(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->आधारः_commit, the_repository->hash_algo);
}

स्थिर ध्रुव वर्ण *diff_title(संरचना strbuf *sb,
			      ध्रुव वर्ण *reroll_गणनम्,
			      ध्रुव वर्ण *generic,
			      ध्रुव वर्ण *rerolled)
{
	पूर्णाङ्क v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	यदि (reroll_गणनम् && !strtol_i(reroll_गणनम्, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	अन्यथा
		strbuf_addstr(sb, generic);
	प्रत्यागम sb->buf;
}

स्थिर शून्य infer_range_diff_ranges(संरचना strbuf *r1,
				    संरचना strbuf *r2,
				    ध्रुव वर्ण *पूर्व,
				    संरचना commit *origin,
				    संरचना commit *शीर्ष)
{
	ध्रुव वर्ण *शीर्ष_oid = oid_गन्तव्यम्_hex(&शीर्ष->object.oid);
	पूर्णाङ्क पूर्व_is_range = is_range_diff_range(पूर्व);

	यदि (पूर्व_is_range)
		strbuf_addstr(r1, पूर्व);
	अन्यथा
		strbuf_addf(r1, "%s..%s", शीर्ष_oid, पूर्व);

	यदि (origin)
		strbuf_addf(r2, "%s..%s", oid_गन्तव्यम्_hex(&origin->object.oid), शीर्ष_oid);
	अन्यथा यदि (पूर्व_is_range)
		die(_("failed to infer range-diff origin of current series"));
	अन्यथा {
		warning(_("using '%s' as range-diff origin of current series"), पूर्व);
		strbuf_addf(r2, "%s..%s", पूर्व, शीर्ष_oid);
	}
}

पूर्णाङ्क cmd_format_patch(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *उपसर्गः,
		     संरचना repository *repo अप्रयुक्तम्)
{
	संरचना format_config cfg;
	संरचना commit *commit;
	संरचना commit_राशिः list = COMMIT_राशिः_INIT;
	संरचना rev_info rev;
	वर्ण *गन्तव्यम्_free = रिक्त;
	संरचना सज्जता_revision_opt s_r_opt;
	आकार_t योग, i;
	पूर्णाङ्क use_stdout = 0;
	पूर्णाङ्क start_सङ्ख्या = -1;
	पूर्णाङ्क just_numbers = 0;
	पूर्णाङ्क ignore_if_अन्तर्_upstream = 0;
	पूर्णाङ्क cover_letter = -1;
	ध्रुव वर्ण *cover_letter_fmt = रिक्त;
	पूर्णाङ्क boundary_गणनम् = 0;
	पूर्णाङ्क no_द्विमान_diff = 0;
	पूर्णाङ्क zero_commit = 0;
	संरचना commit *origin = रिक्त;
	ध्रुव वर्ण *अन्तर्_प्रत्युत्तरम्_गन्तव्यम् = रिक्त;
	संरचना patch_ids ids;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क use_patch_format = 0;
	पूर्णाङ्क quiet = 0;
	ध्रुव वर्ण *reroll_गणनम् = रिक्त;
	वर्ण *cover_from_description_arg = रिक्त;
	वर्ण *description_file = रिक्त;
	वर्ण *branch_name = रिक्त;
	संरचना आधारः_वृक्षः_info bases;
	संरचना commit *आधारः;
	पूर्णाङ्क show_प्रगतिः = 0;
	संरचना प्रगतिः *प्रगतिः = रिक्त;
	संरचना oid_array idiff_पूर्व = OID_ARRAY_INIT;
	संरचना strbuf idiff_title = STRBUF_INIT;
	ध्रुव वर्ण *rdiff_पूर्व = रिक्त;
	संरचना strbuf rdiff1 = STRBUF_INIT;
	संरचना strbuf rdiff2 = STRBUF_INIT;
	संरचना strbuf rdiff_title = STRBUF_INIT;
	संरचना rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ध्रुव वर्ण *rfc = रिक्त;
	पूर्णाङ्क creation_गुणकः = -1;
	ध्रुव वर्ण *signature = git_संस्करणम्_string;
	वर्ण *signature_गन्तव्यम्_free = रिक्त;
	वर्ण *signature_file_arg = रिक्त;
	संरचना keep_callback_दत्त keep_callback_दत्त = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ध्रुव वर्ण *fmt_patch_suffix = रिक्त;

	ध्रुव संरचना option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, रिक्त,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, रिक्त,
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
		OPT_INTEGER(0, "start-number", &start_सङ्ख्या,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_गणनम्, N_("reroll-count"),
			    N_("mark the series as Nth re-roll")),
		OPT_INTEGER(0, "filename-max-length", &cfg.लघुगणकः.fmt_patch_name_max,
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
			    PARSE_OPT_NONEG, subject_उपसर्गः_callback),
		OPT_CALLBACK_F('o', "output-directory", &output_directory,
			    N_("dir"), N_("store resulting files in <dir>"),
			    PARSE_OPT_NONEG, output_directory_callback),
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_दत्त, रिक्त,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_द्विमान_diff,
			 N_("don't output binary diffs")),
		OPT_BOOL(0, "zero-commit", &zero_commit,
			 N_("output all-zero hash in From header")),
		OPT_BOOL(0, "ignore-if-in-upstream", &ignore_if_अन्तर्_upstream,
			 N_("don't include a patch matching a commit upstream")),
		OPT_समुच्चयः_INT_F('p', "no-stat", &use_patch_format,
			      N_("show patch format instead of default (patch + stat)"),
			      1, PARSE_OPT_NONEG),
		OPT_गणः(N_("Messaging")),
		OPT_CALLBACK(0, "add-header", &cfg, N_("header"),
			    N_("add email header"), शीर्षकम्_callback),
		OPT_STRING_LIST(0, "to", &cfg.extra_गन्तव्यम्, N_("email"), N_("add To: header")),
		OPT_STRING_LIST(0, "cc", &cfg.extra_cc, N_("email"), N_("add Cc: header")),
		OPT_CALLBACK_F(0, "from", &cfg.from, N_("ident"),
			    N_("set From address to <ident> (or committer ident if absent)"),
			    PARSE_OPT_OPTARG, from_callback),
		OPT_STRING(0, "in-reply-to", &अन्तर्_प्रत्युत्तरम्_गन्तव्यम्, N_("message-id"),
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
			       0, आधारः_callback),
		OPT_FILENAME(0, "signature-file", &signature_file_arg,
				N_("add a signature from a file")),
		OPT__QUIET(&quiet, N_("don't print the patch filenames")),
		OPT_BOOL(0, "progress", &show_प्रगतिः,
			 N_("show progress while generating patches")),
		OPT_CALLBACK(0, "interdiff", &idiff_पूर्व, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_पूर्व, N_("refspec"),
			   N_("show changes against <refspec> in cover letter or single patch")),
		OPT_CALLBACK_F(0, "range-diff-notes", &rdiff_notes, N_("note"),
			       N_("override notes behavior for the range diff"),
			       0, rdiff_notes_cb),
		OPT_INTEGER(0, "creation-factor", &creation_गुणकः,
			    N_("percentage by which creation is weighted")),
		OPT_BOOL(0, "force-in-body-from", &force_अन्तर्_body_from,
			 N_("show in-body From: even if identical to the e-mail header")),
		OPT_अन्तः()
	};

	format_config_init(&cfg);
	init_diff_ui_defaults();
	init_display_notes(&cfg.notes_opt);
	repo_config(the_repository, git_format_config, &cfg);
	repo_init_revisions(the_repository, &rev, उपसर्गः);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.show_notes = cfg.show_notes;
	स्मृतिं_प्रतिलिख(&rev.notes_opt, &cfg.notes_opt, परिमाण(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_शीर्षकाणि = cfg.लघुगणकः.default_encode_email_शीर्षकाणि;
	rev.expand_tabs_अन्तर्_log_default = 0;
	rev.verbose_शीर्षकम् = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	स्मृतिं_पूरय(&s_r_opt, 0, परिमाण(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.लघुगणकः.fmt_patch_subject_उपसर्गः);
	यदि (format_no_उपसर्गः)
		diff_समुच्चयः_noprefix(&rev.diffopt);

	यदि (cfg.default_attach) {
		rev.mime_boundary = cfg.default_attach;
		rev.no_inline = 1;
	}

	/*
	 * Parse the arguments before setup_revisions(), or something
	 * like "git format-patch -o a123 HEAD^.." may fail; a123 is
	 * possibly a valid SHA1.
	 */
	argc = parse_options(argc, argv, उपसर्गः, builtin_format_patch_options,
			     builtin_format_patch_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	rev.force_अन्तर्_body_from = force_अन्तर्_body_from;

	यदि (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	यदि (cfg.लघुगणकः.fmt_patch_name_max <= cast_आकार_t_गन्तव्यम्_int(वाक्यदैर्घ्यम्("0000-") + वाक्यदैर्घ्यम्(fmt_patch_suffix)))
		cfg.लघुगणकः.fmt_patch_name_max = वाक्यदैर्घ्यम्("0000-") + वाक्यदैर्घ्यम्(fmt_patch_suffix);

	यदि (cover_from_description_arg)
		cfg.cover_from_description_बहुलकम् = parse_cover_from_description(cover_from_description_arg);

	यदि (rfc && rfc[0]) {
		cfg.subject_उपसर्गः = 1;
		यदि (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		अन्यथा
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	यदि (reroll_गणनम्) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_गणनम्);
		rev.reroll_गणनम् = reroll_गणनम्;
	}

	rev.subject_उपसर्गः = cfg.sprefix.buf;

	प्रति (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.वस्तूनि[i].string);
		strbuf_addch(&buf, '\n');
	}

	यदि (cfg.extra_गन्तव्यम्.nr)
		strbuf_addstr(&buf, "To: ");
	प्रति (i = 0; i < cfg.extra_गन्तव्यम्.nr; i++) {
		यदि (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_गन्तव्यम्.वस्तूनि[i].string);
		यदि (i + 1 < cfg.extra_गन्तव्यम्.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	यदि (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	प्रति (i = 0; i < cfg.extra_cc.nr; i++) {
		यदि (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.वस्तूनि[i].string);
		यदि (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_शीर्षकाणि = गन्तव्यम्_free = strbuf_detach(&buf, रिक्त);

	यदि (cfg.from) {
		यदि (विभज_ident_पङ्क्तिः(&rev.from_ident, cfg.from, वाक्यदैर्घ्यम्(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	यदि (start_सङ्ख्या < 0)
		start_सङ्ख्या = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	यदि (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	यदि (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	यदि (cfg.keep_subject && cfg.subject_उपसर्गः)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = सज्जता_revisions(argc, argv, &rev, &s_r_opt);
	यदि (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	यदि (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	यदि (rev.diffopt.output_format & DIFF_FORMAT_NAME_स्थितिः)
		die(_("--name-status does not make sense"));
	यदि (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	यदि (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	यदि (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	यदि (!rev.diffopt.stat_विस्तारः)
		rev.diffopt.stat_विस्तारः = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_शीर्षकम् = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.लघुगणकः.fmt_patch_name_max;

	यदि (!rev.diffopt.flags.text && !no_द्विमान_diff)
		rev.diffopt.flags.द्विमान = 1;

	यदि (rev.show_notes)
		आरोपय_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	यदि (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	यदि (use_stdout) {
		सज्जता_pager(the_repository);
	} अन्यथा यदि (!rev.diffopt.close_file) {
		पूर्णाङ्क saved;

		यदि (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = समुच्चयः_outdir(उपसर्गः, output_directory);

		यदि (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_समुच्चयः_shared_repository(the_repository, 0);
		विकल्प (safe_create_leading_directories_const(the_repository, output_directory)) {
		पक्ष SCLD_OK:
		पक्ष SCLD_EXISTS:
			भङ्ग;
		स्वभाव:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_समुच्चयः_shared_repository(the_repository, saved);
		यदि (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	यदि (rev.pending.nr == 1) {
		पूर्णाङ्क check_शीर्ष = 0;

		यदि (rev.max_गणनम् < 0 && !rev.show_मूलम्_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].item->flags |= UNINTERESTING;
			सङ्कलय_शीर्ष_गन्तव्यम्_pending(&rev);
			check_शीर्ष = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		यदि (!वाक्यं_तुलय(rev.pending.objects[0].name, "HEAD"))
			check_शीर्ष = 1;

		यदि (check_शीर्ष) {
			ध्रुव वर्ण *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      रिक्त, रिक्त);
			यदि (ref && skip_उपसर्गः(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			अन्यथा
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_मूलम्_diff = 1;

	यदि (ignore_if_अन्तर्_upstream) {
		/* Don't say anything if head and upstream are the same. */
		यदि (rev.pending.nr == 2) {
			संरचना object_array_प्रविष्टिः *o = rev.pending.objects;
			यदि (oideq(&o[0].item->oid, &o[1].item->oid))
				गच्छ समाप्तम्;
		}
		get_patch_ids(&rev, &ids);
	}

	यदि (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	यावत् ((commit = get_revision(&rev)) != रिक्त) {
		यदि (commit->object.flags & BOUNDARY) {
			boundary_गणनम्++;
			origin = (boundary_गणनम् == 1) ? commit : रिक्त;
			अनुवर्त;
		}

		यदि (ignore_if_अन्तर्_upstream && has_commit_patch_id(commit, &ids))
			अनुवर्त;

		commit_राशिः_प्रक्षिप(&list, commit);
	}
	यदि (!list.nr)
		/* nothing to do */
		गच्छ समाप्तम्;
	योग = list.nr;

	यदि (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		यदि (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} अन्यथा यदि (cover_letter == -1) {
		cover_letter = 1;
	}

	यदि (cover_letter == -1) {
		यदि (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (योग > 1);
		अन्यथा यदि ((idiff_पूर्व.nr || rdiff_पूर्व) && (योग > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		अन्यथा
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	यदि (!cfg.keep_subject && cfg.auto_सङ्ख्या && (योग > 1 || cover_letter))
		cfg.numbered = 1;
	यदि (cfg.numbered)
		rev.योग = योग + start_सङ्ख्या - 1;

	यदि (idiff_पूर्व.nr) {
		यदि (!cover_letter && योग != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_पूर्व.oid[idiff_पूर्व.nr - 1];
		rev.idiff_oid2 = get_commit_वृक्षः_oid(list.वस्तूनि[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_गणनम्,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	यदि (creation_गुणकः < 0)
		creation_गुणकः = CREATION_गुणकः_FOR_THE_SAME_SERIES;
	अन्यथा यदि (!rdiff_पूर्व)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	यदि (rdiff_पूर्व) {
		यदि (!cover_letter && योग != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_पूर्व,
					origin, list.वस्तूनि[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_गुणकः = creation_गुणकः;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_गणनम्,
					     _("Range-diff:"),
					     _("Range-diff against v%d:"));
		get_notes_तर्काः(&rdiff_notes, &rev);
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
	यदि (!signature) {
		; /* --no-signature inhibits all signatures */
	} अन्यथा यदि (signature && signature != git_संस्करणम्_string) {
		; /* non-default signature already set */
	} अन्यथा यदि (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		संरचना strbuf buf = STRBUF_INIT;
		ध्रुव वर्ण *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		यदि (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_गन्तव्यम्_free = strbuf_detach(&buf, रिक्त);
	} अन्यथा यदि (cfg.signature) {
		signature = cfg.signature;
	}

	स्मृतिं_पूरय(&bases, 0, परिमाण(bases));
	आधारः = get_आधारः_commit(&cfg, list.वस्तूनि, list.nr);
	यदि (आधारः) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, आधारः, list.वस्तूनि, list.nr);
	}

	यदि (अन्तर्_प्रत्युत्तरम्_गन्तव्यम् || cfg.thread || cover_letter) {
		rev.ref_सन्देशः_ids = xmalloc(परिमाण(*rev.ref_सन्देशः_ids));
		string_list_init_dup(rev.ref_सन्देशः_ids);
	}
	यदि (अन्तर्_प्रत्युत्तरम्_गन्तव्यम्) {
		वर्ण *msgid = clean_सन्देशः_id(अन्तर्_प्रत्युत्तरम्_गन्तव्यम्);
		string_list_append_nodup(rev.ref_सन्देशः_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	यदि (cover_letter) {
		यदि (cfg.thread)
			gen_सन्देशः_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.वस्तूनि,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		योग++;
		start_सङ्ख्या--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = रिक्त;
		rev.rdiff1 = रिक्त;
	}
	rev.सङ्कलय_signoff = cfg.do_signoff;

	यदि (show_प्रगतिः)
		प्रगतिः = start_delayed_प्रगतिः(the_repository,
						  _("Generating patches"), योग);
	यावत् (list.nr) {
		आकार_t idx = list.nr - 1;
		पूर्णाङ्क shown;

		display_प्रगतिः(प्रगतिः, योग - idx);
		commit = commit_राशिः_निष्कासय(&list);
		rev.nr = योग - idx + (start_सङ्ख्या - 1);

		/* Make the second and subsequent mails replies to the first */
		यदि (cfg.thread) {
			/* Have we already had a message ID? */
			यदि (rev.सन्देशः_id) {
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
				यदि (cfg.thread == THREAD_SHALLOW
				    && rev.ref_सन्देशः_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					मुक्त(rev.सन्देशः_id);
				अन्यथा
					string_list_append_nodup(rev.ref_सन्देशः_ids,
								 rev.सन्देशः_id);
			}
			gen_सन्देशः_id(&rev, oid_गन्तव्यम्_hex(&commit->object.oid));
		}

		यदि (output_directory &&
		    open_अग्र_file(rev.numbered_files ? रिक्त : commit, रिक्त, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_वृक्षः_commit(&rev, commit);
		free_commit_बफर(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		यदि (output_directory)
			rev.shown_one = 0;
		यदि (shown) {
			print_bases(&bases, rev.diffopt.file);
			यदि (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			अन्यथा
				print_signature(signature, rev.diffopt.file);
		}
		यदि (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = रिक्त;
		}
	}
	stop_प्रगतिः(&प्रगतिः);
	commit_राशिः_clear(&list);
	यदि (ignore_if_अन्तर्_upstream)
		free_patch_ids(&ids);

समाप्तम्:
	oid_array_clear(&idiff_पूर्व);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	मुक्त(description_file);
	मुक्त(signature_file_arg);
	मुक्त(signature_गन्तव्यम्_free);
	मुक्त(branch_name);
	मुक्त(गन्तव्यम्_free);
	मुक्त(rev.सन्देशः_id);
	यदि (rev.ref_सन्देशः_ids)
		string_list_clear(rev.ref_सन्देशः_ids, 0);
	मुक्त(rev.ref_सन्देशः_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सङ्कलय_pending_commit(ध्रुव वर्ण *arg, संरचना rev_info *revs, पूर्णाङ्क flags)
{
	संरचना object_id oid;
	यदि (repo_get_oid(the_repository, arg, &oid) == 0) {
		संरचना commit *commit = lookup_commit_reference(the_repository,
								&oid);
		यदि (commit) {
			commit->object.flags |= flags;
			सङ्कलय_pending_object(revs, &commit->object, arg);
			प्रत्यागम 0;
		}
	}
	प्रत्यागम -1;
}

स्थिर ध्रुव वर्ण * ध्रुव cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	रिक्त
};

स्थिर शून्य print_commit(वर्ण sign, संरचना commit *commit, पूर्णाङ्क verbose,
			 पूर्णाङ्क abbrev, FILE *file)
{
	यदि (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} अन्यथा {
		संरचना strbuf buf = STRBUF_INIT;
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
स्थिर शून्य collect_diff_blob_oids(संरचना commit *commit,
				   संरचना diff_options *opts,
				   संरचना oidset *blobs)
{
	संरचना diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	प्रतिज्ञा(!commit->parents || !commit->parents->अग्र);

	यदि (commit->parents)
		diff_वृक्षः_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	अन्यथा
		diff_मूलम्_वृक्षः_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	प्रति (पूर्णाङ्क i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_मार्गः(opts->repo->सूचकांक, p->one->मार्गः);
		यदि (drv && drv->द्विमान > 0)
			अनुवर्त;

		यदि (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, रिक्त,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_निवेशय(blobs, &p->one->oid);
		यदि (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, रिक्त,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_निवेशय(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

स्थिर पूर्णाङ्क always_मेलनम्(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
			ध्रुव संरचना hashmap_प्रविष्टिः *entry1 अप्रयुक्तम्,
			ध्रुव संरचना hashmap_प्रविष्टिः *entry2 अप्रयुक्तम्,
			ध्रुव शून्य *keydata अप्रयुक्तम्)
{
	प्रत्यागम 0;
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
स्थिर शून्य prefetch_cherry_blobs(संरचना repository *repo,
				  संरचना commit_list *list,
				  संरचना patch_ids *ids)
{
	संरचना oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	यदि (!repo_has_promisor_remote(repo))
		प्रत्यागम;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_मेलनम्;

	/* Find header-only collisions, gather blobs from those commits */
	प्रति (संरचना commit_list *l = list; l; l = l->अग्र) {
		संरचना commit *c = l->item;
		bool मेलनम्_प्राप्तम् = false;
		प्रति (संरचना patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_अग्र(cur, ids)) {
			मेलनम्_प्राप्तम् = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		यदि (मेलनम्_प्राप्तम्)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	यदि (oidset_आकार(&blobs)) {
		संरचना oid_array गन्तव्यम्_fetch = OID_ARRAY_INIT;
		संरचना oidset_iter iter;
		ध्रुव संरचना object_id *oid;

		oidset_iter_init(&blobs, &iter);
		यावत् ((oid = oidset_iter_अग्र(&iter)))
			oid_array_append(&गन्तव्यम्_fetch, oid);

		promisor_remote_get_प्रत्यक्षम्(repo, गन्तव्यम्_fetch.oid, गन्तव्यम्_fetch.nr);

		oid_array_clear(&गन्तव्यम्_fetch);
	}

	oidset_clear(&blobs);
}

पूर्णाङ्क cmd_cherry(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo अप्रयुक्तम्)
{
	संरचना rev_info revs;
	संरचना patch_ids ids;
	संरचना commit *commit;
	संरचना commit_list *list = रिक्त;
	संरचना branch *current_branch;
	ध्रुव वर्ण *upstream;
	ध्रुव वर्ण *शीर्ष = "HEAD";
	ध्रुव वर्ण *सीमा = रिक्त;
	पूर्णाङ्क verbose = 0, abbrev = 0;

	संरचना option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, cherry_usage, 0);

	विकल्प (argc) {
	पक्ष 3:
		सीमा = argv[2];
		/* FALLTHROUGH */
	पक्ष 2:
		शीर्ष = argv[1];
		/* FALLTHROUGH */
	पक्ष 1:
		upstream = argv[0];
		भङ्ग;
	स्वभाव:
		current_branch = branch_get(रिक्त);
		upstream = branch_get_upstream(current_branch, रिक्त);
		यदि (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, उपसर्गः);
	revs.max_parents = 1;

	यदि (सङ्कलय_pending_commit(शीर्ष, &revs, 0))
		die(_("unknown commit %s"), शीर्ष);
	यदि (सङ्कलय_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	यदि (revs.pending.nr == 2) {
		संरचना object_array_प्रविष्टिः *o = revs.pending.objects;
		यदि (oideq(&o[0].item->oid, &o[1].item->oid))
			प्रत्यागम 0;
	}

	get_patch_ids(&revs, &ids);

	यदि (सीमा && सङ्कलय_pending_commit(सीमा, &revs, UNINTERESTING))
		die(_("unknown commit %s"), सीमा);

	/* reverse the list of commits */
	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	यावत् ((commit = get_revision(&revs)) != रिक्त) {
		commit_list_निवेशय(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	प्रति (संरचना commit_list *l = list; l; l = l->अग्र) {
		वर्ण sign = '+';

		commit = l->item;
		यदि (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	प्रत्यागम 0;
}
