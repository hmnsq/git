/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

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

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

स्थिर unsigned पूर्णाङ्क force_in_body_from;
स्थिर पूर्णाङ्क stdout_mboxrd;
स्थिर पूर्णाङ्क format_no_prefix;

स्थिर ध्रुव वर्ण * ध्रुव builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	रिक्त
};

संरचना ओळ_opt_callback_data {
	संरचना rev_info *rev;
	ध्रुव वर्ण *prefix;
	संरचना string_list args;
};

स्थिर पूर्णाङ्क session_is_interactive(शून्य)
{
	प्रत्यागम isatty(1) || pager_in_use();
}

स्थिर पूर्णाङ्क auto_decoration_style(शून्य)
{
	प्रत्यागम session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

स्थिर पूर्णाङ्क parse_decoration_style(ध्रुव वर्ण *मूल्य)
{
	विकल्प (git_parse_maybe_bool(मूल्य)) {
	पक्ष 1:
		प्रत्यागम DECORATE_SHORT_REFS;
	पक्ष 0:
		प्रत्यागम 0;
	स्वभाव:
		भङ्ग;
	}
	यदि (!strcmp(मूल्य, "full"))
		प्रत्यागम DECORATE_FULL_REFS;
	अन्यथा यदि (!strcmp(मूल्य, "short"))
		प्रत्यागम DECORATE_SHORT_REFS;
	अन्यथा यदि (!strcmp(मूल्य, "auto"))
		प्रत्यागम auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	प्रत्यागम -1;
}

संरचना log_config {
	पूर्णाङ्क default_abbrev_commit;
	पूर्णाङ्क default_show_मूळ;
	पूर्णाङ्क default_follow;
	पूर्णाङ्क default_show_signature;
	पूर्णाङ्क default_encode_email_headers;
	पूर्णाङ्क decoration_style;
	पूर्णाङ्क decoration_given;
	पूर्णाङ्क use_mailmap_config;
	वर्ण *fmt_patch_subject_prefix;
	पूर्णाङ्क fmt_patch_name_max;
	वर्ण *fmt_pretty;
	वर्ण *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	पूर्णाङ्क i_still_use_this;
#endif
};

स्थिर शून्य log_config_init(संरचना log_config *cfg)
{
	memset(cfg, 0, परिमाण(*cfg));
	cfg->default_show_मूळ = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

स्थिर शून्य log_config_release(संरचना log_config *cfg)
{
	मुक्त(cfg->default_date_mode);
	मुक्त(cfg->fmt_pretty);
	मुक्त(cfg->fmt_patch_subject_prefix);
}

स्थिर पूर्णाङ्क use_default_decoration_filter = 1;
स्थिर संरचना string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

स्थिर पूर्णाङ्क clear_decorations_callback(ध्रुव संरचना option *opt UNUSED,
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
	संरचना log_config *cfg = opt->मूल्य;

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

स्थिर पूर्णाङ्क log_ओळ_range_callback(ध्रुव संरचना option *option, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना ओळ_opt_callback_data *data = option->मूल्य;

	BUG_ON_OPT_NEG(unset);

	यदि (!arg)
		प्रत्यागम -1;

	data->rev->ओळ_level_traverse = 1;
	string_list_append(&data->args, arg);

	प्रत्यागम 0;
}

स्थिर शून्य cmd_log_init_defaults(संरचना rev_info *rev,
				  संरचना log_config *cfg)
{
	यदि (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	यदि (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_मूळ_diff = cfg->default_show_मूळ;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	यदि (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

स्थिर शून्य set_default_decoration_filter(संरचना decoration_filter *decoration_filter)
{
	वर्ण *मूल्य = रिक्त;
	संरचना string_list *include = decoration_filter->include_ref_pattern;
	ध्रुव संरचना string_list *config_exclude;

	यदि (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		संरचना string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	यदि (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &मूल्य) &&
	    !strcmp("all", मूल्य))
		use_default_decoration_filter = 0;
	मुक्त(मूल्य);

	यदि (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		प्रत्यागम;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	प्रति (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		यदि (!ref_namespace[i].decoration)
			अनुवर्त;

		string_list_append(include, ref_namespace[i].ref);
	}
}

स्थिर शून्य cmd_log_init_finish(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			 संरचना rev_info *rev, संरचना setup_revision_opt *opt,
			 संरचना log_config *cfg)
{
	संरचना userformat_want w;
	पूर्णाङ्क quiet = 0, source = 0, mailmap;
	स्थिर संरचना ओळ_opt_callback_data ओळ_cb = {रिक्त, रिक्त, STRING_LIST_INIT_DUP};
	संरचना decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	स्थिर संरचना revision_sources revision_sources;

	ध्रुव संरचना option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
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
		OPT_CALLBACK('L', रिक्त, &ओळ_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_ओळ_range_callback),
		OPT_END()
	};

	ओळ_cb.rev = rev;
	ओळ_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	यदि (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	यदि (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	यदि (rev->ओळ_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, परिमाण(w));
	userformat_find_requirements(रिक्त, &w);

	यदि (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	यदि (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	यदि ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

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
		set_default_decoration_filter(&decoration_filter);

		यदि (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	यदि (rev->ओळ_level_traverse)
		ओळ_log_init(rev, ओळ_cb.prefix, &ओळ_cb.args);

	setup_pager(the_repository);
}

स्थिर शून्य cmd_log_init(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
			 संरचना rev_info *rev, संरचना setup_revision_opt *opt,
			 संरचना log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

स्थिर पूर्णाङ्क cmd_log_walk_no_free(संरचना rev_info *rev)
{
	संरचना commit *commit;
	पूर्णाङ्क saved_nrl = 0;
	पूर्णाङ्क saved_dcctc = 0;
	पूर्णाङ्क result;

	यदि (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	यावत् ((commit = get_revision(rev)) != रिक्त) {
		यदि (!log_वृक्ष_commit(rev, commit) && rev->max_मोजणी >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_मोजणी++;
		यदि (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = रिक्त;
		}
		यदि (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		यदि (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	यदि (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	प्रत्यागम result;
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

स्थिर पूर्णाङ्क git_log_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना log_config *cfg = cb;
	ध्रुव वर्ण *slot_name;

	यदि (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		प्रत्यागम git_config_string(&cfg->fmt_pretty, var, मूल्य);
	}
	यदि (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		प्रत्यागम git_config_string(&cfg->fmt_patch_subject_prefix, var, मूल्य);
	}
	यदि (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, मूल्य, ctx->kvi);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		प्रत्यागम git_config_string(&cfg->default_date_mode, var, मूल्य);
	}
	यदि (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(मूल्य);
		यदि (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "log.diffmerges")) {
		यदि (!मूल्य)
			प्रत्यागम config_error_nonbool(var);
		प्रत्यागम diff_merges_config(मूल्य);
	}
	यदि (!strcmp(var, "log.showroot")) {
		cfg->default_show_मूळ = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (skip_prefix(var, "color.decorate.", &slot_name))
		प्रत्यागम parse_decorate_color_config(var, slot_name, मूल्य);
	यदि (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}

	प्रत्यागम git_diff_ui_config(var, मूल्य, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
पूर्णाङ्क cmd_whatchanged(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *prefix,
		    संरचना repository *repo UNUSED)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	संरचना setup_revision_opt opt;
	पूर्णाङ्क ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

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
#endif

स्थिर शून्य show_tagger(ध्रुव वर्ण *buf, संरचना rev_info *rev)
{
	संरचना strbuf out = STRBUF_INIT;
	संरचना pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

स्थिर पूर्णाङ्क show_blob_object(ध्रुव संरचना object_id *oid, संरचना rev_info *rev, ध्रुव वर्ण *obj_name)
{
	संरचना object_id oidc;
	संरचना object_context obj_context = {0};
	वर्ण *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	यदि (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		प्रत्यागम odb_stream_blob_to_fd(the_repository->objects, 1, oid, रिक्त, 0);

	यदि (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	यदि (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		प्रत्यागम odb_stream_blob_to_fd(the_repository->objects, 1, oid, रिक्त, 0);
	}

	यदि (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	मुक्त(buf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_tag_object(ध्रुव संरचना object_id *oid, संरचना rev_info *rev)
{
	size_t size;
	गणना object_type type;
	वर्ण *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	यदि (!buf)
		प्रत्यागम error(_("could not read object %s"), oid_to_hex(oid));

	प्रतिज्ञा(type == OBJ_TAG);
	यावत् (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		ध्रुव वर्ण *ident;
		यावत् (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		यदि (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	यदि (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	मुक्त(buf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_वृक्ष_object(ध्रुव संरचना object_id *oid UNUSED,
			    संरचना strbuf *base UNUSED,
			    ध्रुव वर्ण *pathname, unsigned mode,
			    शून्य *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	प्रत्यागम 0;
}

स्थिर शून्य show_setup_revisions_tweak(संरचना rev_info *rev)
{
	यदि (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	अन्यथा
		diff_merges_default_to_dense_combined(rev);
	यदि (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

पूर्णाङ्क cmd_show(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *prefix,
	     संरचना repository *repo UNUSED)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	unsigned पूर्णाङ्क i;
	संरचना setup_revision_opt opt;
	संरचना pathspec match_all;
	पूर्णाङ्क ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	यदि (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, परिमाण(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

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
		पक्ष OBJ_TAG: {
			संरचना tag *t = (संरचना tag *)o;
			संरचना object_id *oid = get_tagged_oid(t);

			यदि (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			यदि (ret)
				भङ्ग;
			o = parse_object(the_repository, oid);
			यदि (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			भङ्ग;
		}
		पक्ष OBJ_वृक्ष:
			यदि (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_वृक्ष(the_repository, (संरचना वृक्ष *)o,
				  &match_all, show_वृक्ष_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			भङ्ग;
		पक्ष OBJ_COMMIT:
		{
			संरचना object_array old;
			संरचना object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, परिमाण(old));
			memcpy(&rev.pending, &blank, परिमाण(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, परिमाण(rev.pending));
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
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	संरचना setup_revision_opt opt;
	पूर्णाङ्क ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, परिमाण(opt));
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
	प्रत्यागम ret;
}

स्थिर शून्य log_setup_revisions_tweak(संरचना rev_info *rev)
{
	यदि (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	यदि (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

पूर्णाङ्क cmd_log(पूर्णाङ्क argc,
	    ध्रुव वर्ण **argv,
	    ध्रुव वर्ण *prefix,
	    संरचना repository *repo UNUSED)
{
	संरचना log_config cfg;
	संरचना rev_info rev;
	संरचना setup_revision_opt opt;
	पूर्णाङ्क ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, परिमाण(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

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
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

गणना auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

संरचना format_config {
	संरचना log_config log;
	गणना thread_level thread;
	पूर्णाङ्क do_signoff;
	गणना auto_base_setting auto_base;
	वर्ण *base_commit;
	वर्ण *from;
	वर्ण *signature;
	वर्ण *signature_file;
	गणना cover_setting config_cover_letter;
	वर्ण *fmt_cover_letter_commit_list;
	वर्ण *config_output_directory;
	गणना cover_from_description cover_from_description_mode;
	पूर्णाङ्क show_notes;
	संरचना display_notes_opt notes_opt;
	पूर्णाङ्क numbered_cmdline_opt;
	पूर्णाङ्क numbered;
	पूर्णाङ्क auto_संख्या;
	वर्ण *default_attach;
	संरचना string_list extra_hdr;
	संरचना string_list extra_to;
	संरचना string_list extra_cc;
	पूर्णाङ्क keep_subject;
	पूर्णाङ्क subject_prefix;
	संरचना strbuf sprefix;
	वर्ण *fmt_patch_suffix;
};

स्थिर शून्य format_config_init(संरचना format_config *cfg)
{
	memset(cfg, 0, परिमाण(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_संख्या = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

स्थिर शून्य format_config_release(संरचना format_config *cfg)
{
	log_config_release(&cfg->log);
	मुक्त(cfg->base_commit);
	मुक्त(cfg->from);
	मुक्त(cfg->signature);
	मुक्त(cfg->signature_file);
	मुक्त(cfg->config_output_directory);
	मुक्त(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	मुक्त(cfg->fmt_patch_suffix);
	मुक्त(cfg->fmt_cover_letter_commit_list);
}

स्थिर गणना cover_from_description parse_cover_from_description(ध्रुव वर्ण *arg)
{
	यदि (!arg || !strcmp(arg, "default"))
		प्रत्यागम COVER_FROM_MESSAGE;
	अन्यथा यदि (!strcmp(arg, "none"))
		प्रत्यागम COVER_FROM_NONE;
	अन्यथा यदि (!strcmp(arg, "message"))
		प्रत्यागम COVER_FROM_MESSAGE;
	अन्यथा यदि (!strcmp(arg, "subject"))
		प्रत्यागम COVER_FROM_SUBJECT;
	अन्यथा यदि (!strcmp(arg, "auto"))
		प्रत्यागम COVER_FROM_AUTO;
	अन्यथा
		die(_("%s: invalid cover from description mode"), arg);
}

स्थिर शून्य add_header(संरचना format_config *cfg, ध्रुव वर्ण *मूल्य)
{
	संरचना string_list_item *item;
	पूर्णाङ्क len = strlen(मूल्य);
	यावत् (len && मूल्य[len - 1] == '\n')
		len--;

	यदि (!strncasecmp(मूल्य, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, मूल्य + 4);
		len -= 4;
	} अन्यथा यदि (!strncasecmp(मूल्य, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, मूल्य + 4);
		len -= 4;
	} अन्यथा {
		item = string_list_append(&cfg->extra_hdr, मूल्य);
	}

	item->string[len] = '\0';
}

स्थिर पूर्णाङ्क git_format_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
			     ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना format_config *cfg = cb;

	यदि (!strcmp(var, "format.headers")) {
		यदि (!मूल्य)
			die(_("format.headers without value"));
		add_header(cfg, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		प्रत्यागम git_config_string(&cfg->fmt_patch_suffix, var, मूल्य);
	}
	यदि (!strcmp(var, "format.to")) {
		यदि (!मूल्य)
			प्रत्यागम config_error_nonbool(var);
		string_list_append(&cfg->extra_to, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.cc")) {
		यदि (!मूल्य)
			प्रत्यागम config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.numbered")) {
		यदि (मूल्य && !strcasecmp(मूल्य, "auto")) {
			cfg->auto_संख्या = 1;
			प्रत्यागम 0;
		}
		cfg->numbered = git_config_bool(var, मूल्य);
		cfg->auto_संख्या = cfg->auto_संख्या && cfg->numbered;
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.attach")) {
		यदि (मूल्य && *मूल्य) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(मूल्य);
		} अन्यथा यदि (मूल्य && !*मूल्य) {
			FREE_AND_NULL(cfg->default_attach);
		} अन्यथा {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.thread")) {
		यदि (मूल्य && !strcasecmp(मूल्य, "deep")) {
			cfg->thread = THREAD_DEEP;
			प्रत्यागम 0;
		}
		यदि (मूल्य && !strcasecmp(मूल्य, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			प्रत्यागम 0;
		}
		cfg->thread = git_config_bool(var, मूल्य) ? THREAD_SHALLOW : THREAD_UNSET;
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		प्रत्यागम git_config_string(&cfg->signature, var, मूल्य);
	}
	यदि (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		प्रत्यागम git_config_pathname(&cfg->signature_file, var, मूल्य);
	}
	यदि (!strcmp(var, "format.coverletter")) {
		यदि (मूल्य && !strcasecmp(मूल्य, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			प्रत्यागम 0;
		}
		cfg->config_cover_letter = git_config_bool(var, मूल्य) ? COVER_ON : COVER_OFF;
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		प्रत्यागम git_config_string(&cfg->fmt_cover_letter_commit_list, var, मूल्य);
	}
	यदि (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		प्रत्यागम git_config_string(&cfg->config_output_directory, var, मूल्य);
	}
	यदि (!strcmp(var, "format.useautobase")) {
		यदि (मूल्य && !strcasecmp(मूल्य, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			प्रत्यागम 0;
		}
		cfg->auto_base = git_config_bool(var, मूल्य) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.from")) {
		पूर्णाङ्क b = git_parse_maybe_bool(मूल्य);
		FREE_AND_NULL(cfg->from);
		यदि (b < 0)
			cfg->from = xstrdup(मूल्य);
		अन्यथा यदि (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.notes")) {
		पूर्णाङ्क b = git_parse_maybe_bool(मूल्य);
		यदि (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, मूल्य);
		अन्यथा यदि (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		अन्यथा
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, मूल्य);
		प्रत्यागम 0;
	}
	यदि (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(मूल्य);
		यदि (format_no_prefix < 0) {
			पूर्णाङ्क status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				मूल्य, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		प्रत्यागम 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	यदि (!strcmp(var, "diff.noprefix"))
		प्रत्यागम 0;

	प्रत्यागम git_log_config(var, मूल्य, ctx, &cfg->log);
}

स्थिर ध्रुव वर्ण *output_directory = रिक्त;
स्थिर पूर्णाङ्क outdir_offset;

स्थिर पूर्णाङ्क open_next_file(संरचना commit *commit, ध्रुव वर्ण *subject,
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
		printf("%s\n", filename.buf + outdir_offset);

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
	unsigned flags1, flags2;

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
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	यदि (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	यावत् ((commit = get_revision(&check_rev)) != रिक्त) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

स्थिर शून्य gen_message_id(संरचना rev_info *info, ध्रुव वर्ण *base)
{
	संरचना strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(रिक्त),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य print_signature(ध्रुव वर्ण *signature, FILE *file)
{
	यदि (!signature || !*signature)
		प्रत्यागम;

	fprintf(file, "-- \n%s", signature);
	यदि (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

स्थिर वर्ण *find_branch_name(संरचना rev_info *rev)
{
	संरचना object_id branch_oid;
	ध्रुव संरचना object_id *tip_oid;
	ध्रुव वर्ण *ref, *v;
	वर्ण *full_ref, *branch = रिक्त;
	पूर्णाङ्क interesting_found = 0;
	size_t idx;

	प्रति (size_t i = 0; i < rev->cmdline.nr; i++) {
		यदि (rev->cmdline.rev[i].flags & UNINTERESTING)
			अनुवर्त;
		यदि (interesting_found)
			प्रत्यागम रिक्त;
		interesting_found = 1;
		idx = i;
	}
	यदि (!interesting_found)
		प्रत्यागम रिक्त;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	यदि (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	मुक्त(full_ref);
	प्रत्यागम branch;
}

स्थिर शून्य show_diffstat(संरचना rev_info *rev,
			  संरचना commit *origin, संरचना commit *head)
{
	संरचना diff_options opts;

	memcpy(&opts, &rev->diffopt, परिमाण(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_वृक्ष_oid(get_commit_वृक्ष_oid(origin),
		      get_commit_वृक्ष_oid(head),
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

	यदि (cfg->cover_from_description_mode == COVER_FROM_NONE)
		गच्छ do_pp;

	यदि (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	अन्यथा यदि (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	यदि (!description_sb.len)
		गच्छ do_pp;

	यदि (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	यदि (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
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
	संरचना rdiff_notes *rdiff_notes = option->मूल्य;

	rdiff_notes->override = 1;
	opt.मूल्य = &rdiff_notes->notes;
	प्रत्यागम parse_opt_string_list(&opt, arg, unset);
}

स्थिर पूर्णाङ्क get_notes_refs(संरचना string_list_item *item, शून्य *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	प्रत्यागम 0;
}

स्थिर शून्य get_notes_args(संरचना rdiff_notes *rdiff_notes,
			   संरचना rev_info *rev)
{
	यदि (rdiff_notes->override) {
		यदि (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		अन्यथा
			strvec_ढकला(&rev->rdiff_log_arg, "--no-notes");
	} अन्यथा यदि (!rev->show_notes) {
		strvec_ढकला(&rev->rdiff_log_arg, "--no-notes");
	} अन्यथा यदि (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ढकला(&rev->rdiff_log_arg, "--notes");
	} अन्यथा {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

स्थिर शून्य generate_shortlog_cover_letter(संरचना shortlog *log,
					   संरचना rev_info *rev,
					   संरचना commit **list,
					   पूर्णाङ्क nr)
{
	shortlog_init(log);
	log->wrap_ओळी = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	प्रति (पूर्णाङ्क i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

स्थिर शून्य generate_commit_list_cover(FILE *cover_file, ध्रुव वर्ण *format,
				       संरचना commit **list, पूर्णाङ्क n)
{
	संरचना strbuf commit_ओळ = STRBUF_INIT;
	संरचना pretty_print_context ctx = {0};
	संरचना rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	प्रति (पूर्णाङ्क i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_ओळ, &ctx);
		fprintf(cover_file, "%s\n", commit_ओळ.buf);
		strbuf_reset(&commit_ओळ);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_ओळ);
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
	संरचना shortlog log;
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क i;
	ध्रुव वर्ण *encoding = "UTF-8";
	पूर्णाङ्क need_8bit_cte = 0;
	संरचना pretty_print_context pp = {0};
	संरचना commit *head = list[0];
	वर्ण *to_free = रिक्त;

	यदि (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	यदि (use_separate_file &&
	    open_next_file(रिक्त, rev->numbered_files ? रिक्त : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	प्रति (i = 0; !need_8bit_cte && i < nr; i++) {
		ध्रुव वर्ण *buf = repo_get_commit_buffer(the_repository,
							 list[i], रिक्त);
		यदि (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	यदि (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, रिक्त, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	मुक्त(to_free);
	मुक्त(pp.after_subject);
	strbuf_release(&sb);

	यदि (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	अन्यथा यदि (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	अन्यथा यदि (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	अन्यथा यदि (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	अन्यथा
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	यदि (origin)
		show_diffstat(rev, origin, head);

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

स्थिर वर्ण *clean_message_id(ध्रुव वर्ण *संदेश_id)
{
	वर्ण ch;
	ध्रुव वर्ण *a, *z, *m;

	m = संदेश_id;
	यावत् ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = रिक्त;
	यावत् ((ch = *m)) {
		यदि (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	यदि (!z)
		die(_("insane in-reply-to: %s"), संदेश_id);
	यदि (++z == m)
		प्रत्यागम xstrdup(a);
	प्रत्यागम xmemdupz(a, z - a);
}

स्थिर ध्रुव वर्ण *set_outdir(ध्रुव वर्ण *prefix, ध्रुव वर्ण *output_directory)
{
	यदि (output_directory && is_absolute_path(output_directory))
		प्रत्यागम output_directory;

	यदि (!prefix || !*prefix) {
		यदि (output_directory)
			प्रत्यागम output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		प्रत्यागम "./";
	}

	outdir_offset = strlen(prefix);
	यदि (!output_directory)
		प्रत्यागम prefix;

	प्रत्यागम prefix_filename(prefix, output_directory);
}

स्थिर ध्रुव वर्ण * ध्रुव builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	रिक्त
};

संरचना keep_callback_data {
	संरचना format_config *cfg;
	संरचना rev_info *revs;
};

स्थिर पूर्णाङ्क keep_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना keep_callback_data *data = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क subject_prefix_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			    पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rfc_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			पूर्णाङ्क unset)
{
	ध्रुव वर्ण **rfc = opt->मूल्य;

	*rfc = opt->मूल्य;
	यदि (unset)
		*rfc = रिक्त;
	अन्यथा
		*rfc = arg ? arg : "RFC";
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क numbered_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			     पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मूल्य;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	यदि (unset)
		cfg->auto_संख्या =  0;
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
	ध्रुव वर्ण **dir = (ध्रुव वर्ण **)opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	यदि (*dir)
		die(_("two output directories?"));
	*dir = arg;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क thread_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मूल्य;

	यदि (unset)
		cfg->thread = THREAD_UNSET;
	अन्यथा यदि (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	अन्यथा यदि (!strcmp(arg, "deep"))
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
	संरचना rev_info *rev = (संरचना rev_info *)opt->मूल्य;
	यदि (unset)
		rev->mime_boundary = रिक्त;
	अन्यथा यदि (arg)
		rev->mime_boundary = arg;
	अन्यथा
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क inline_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना rev_info *rev = (संरचना rev_info *)opt->मूल्य;
	यदि (unset)
		rev->mime_boundary = रिक्त;
	अन्यथा यदि (arg)
		rev->mime_boundary = arg;
	अन्यथा
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क header_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			   पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मूल्य;

	यदि (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} अन्यथा {
		add_header(cfg, arg);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क from_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	वर्ण **from = opt->मूल्य;

	मुक्त(*from);

	यदि (unset)
		*from = रिक्त;
	अन्यथा यदि (arg)
		*from = xstrdup(arg);
	अन्यथा
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क base_callback(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना format_config *cfg = opt->मूल्य;

	यदि (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} अन्यथा यदि (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} अन्यथा {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	प्रत्यागम 0;
}

संरचना base_वृक्ष_info {
	संरचना object_id base_commit;
	पूर्णाङ्क nr_patch_id, alloc_patch_id;
	संरचना object_id *patch_id;
};

स्थिर संरचना commit *get_base_commit(ध्रुव संरचना format_config *cfg,
				      संरचना commit **list,
				      size_t total)
{
	संरचना commit *base = रिक्त;
	संरचना commit **rev;
	पूर्णाङ्क auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	विकल्प (cfg->auto_base) {
	पक्ष AUTO_BASE_NEVER:
		यदि (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} अन्यथा {
			/* no base information is requested */
			प्रत्यागम रिक्त;
		}
		भङ्ग;
	पक्ष AUTO_BASE_ALWAYS:
	पक्ष AUTO_BASE_WHEN_ABLE:
		यदि (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} अन्यथा {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		भङ्ग;
	स्वभाव:
		BUG("unexpected automatic base selection method");
	}

	यदि (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		यदि (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} अन्यथा {
		संरचना branch *curr_branch = branch_get(रिक्त);
		ध्रुव वर्ण *upstream = branch_get_upstream(curr_branch, रिक्त);
		यदि (upstream) {
			संरचना commit_list *base_list = रिक्त;
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
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				यदि (die_on_failure) {
					die(_("could not find exact merge base"));
				} अन्यथा {
					commit_list_free(base_list);
					प्रत्यागम रिक्त;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} अन्यथा {
			यदि (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			अन्यथा
				प्रत्यागम रिक्त;
		}
	}

	ALLOC_ARRAY(rev, total);
	प्रति (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	यावत् (rev_nr > 1) {
		प्रति (i = 0; i < rev_nr / 2; i++) {
			संरचना commit_list *merge_base = रिक्त;
			यदि (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				यदि (die_on_failure) {
					die(_("failed to find exact merge base"));
				} अन्यथा {
					commit_list_free(merge_base);
					मुक्त(rev);
					प्रत्यागम रिक्त;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		यदि (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	यदि (ret < 0)
		exit(128);
	यदि (!ret) {
		यदि (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} अन्यथा {
			मुक्त(rev);
			प्रत्यागम रिक्त;
		}
	}

	प्रति (i = 0; i < total; i++) {
		यदि (base == list[i]) {
			यदि (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} अन्यथा {
				मुक्त(rev);
				प्रत्यागम रिक्त;
			}
		}
	}

	मुक्त(rev);
	प्रत्यागम base;
}

define_commit_slab(commit_base, पूर्णाङ्क);

स्थिर शून्य prepare_bases(संरचना base_वृक्ष_info *bases,
			  संरचना commit *base,
			  संरचना commit **list,
			  size_t total)
{
	संरचना commit *commit;
	संरचना rev_info revs;
	संरचना diff_options diffopt;
	संरचना commit_base commit_base;

	यदि (!base)
		प्रत्यागम;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, रिक्त);
	revs.max_parents = 1;
	revs.topo_order = 1;
	प्रति (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	यावत् ((commit = get_revision(&revs)) != रिक्त) {
		संरचना object_id oid;
		संरचना object_id *patch_id;
		यदि (*commit_base_at(&commit_base, commit))
			अनुवर्त;
		यदि (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

स्थिर शून्य print_bases(संरचना base_वृक्ष_info *bases, FILE *file)
{
	पूर्णाङ्क i;

	/* Only do this once, either for the cover or for the first one */
	यदि (is_null_oid(&bases->base_commit))
		प्रत्यागम;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	प्रति (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	मुक्त(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

स्थिर ध्रुव वर्ण *diff_title(संरचना strbuf *sb,
			      ध्रुव वर्ण *reroll_मोजणी,
			      ध्रुव वर्ण *generic,
			      ध्रुव वर्ण *rerolled)
{
	पूर्णाङ्क v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	यदि (reroll_मोजणी && !strtol_i(reroll_मोजणी, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	अन्यथा
		strbuf_addstr(sb, generic);
	प्रत्यागम sb->buf;
}

स्थिर शून्य infer_range_diff_ranges(संरचना strbuf *r1,
				    संरचना strbuf *r2,
				    ध्रुव वर्ण *prev,
				    संरचना commit *origin,
				    संरचना commit *head)
{
	ध्रुव वर्ण *head_oid = oid_to_hex(&head->object.oid);
	पूर्णाङ्क prev_is_range = is_range_diff_range(prev);

	यदि (prev_is_range)
		strbuf_addstr(r1, prev);
	अन्यथा
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	यदि (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	अन्यथा यदि (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	अन्यथा {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

पूर्णाङ्क cmd_format_patch(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	संरचना format_config cfg;
	संरचना commit *commit;
	संरचना commit_चळत list = COMMIT_चळत_INIT;
	संरचना rev_info rev;
	वर्ण *to_free = रिक्त;
	संरचना setup_revision_opt s_r_opt;
	size_t total, i;
	पूर्णाङ्क use_stdout = 0;
	पूर्णाङ्क start_संख्या = -1;
	पूर्णाङ्क just_numbers = 0;
	पूर्णाङ्क ignore_if_in_upstream = 0;
	पूर्णाङ्क cover_letter = -1;
	ध्रुव वर्ण *cover_letter_fmt = रिक्त;
	पूर्णाङ्क boundary_मोजणी = 0;
	पूर्णाङ्क no_द्विमान_diff = 0;
	पूर्णाङ्क zero_commit = 0;
	संरचना commit *origin = रिक्त;
	ध्रुव वर्ण *in_reply_to = रिक्त;
	संरचना patch_ids ids;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क use_patch_format = 0;
	पूर्णाङ्क quiet = 0;
	ध्रुव वर्ण *reroll_मोजणी = रिक्त;
	वर्ण *cover_from_description_arg = रिक्त;
	वर्ण *description_file = रिक्त;
	वर्ण *branch_name = रिक्त;
	संरचना base_वृक्ष_info bases;
	संरचना commit *base;
	पूर्णाङ्क show_progress = 0;
	संरचना progress *progress = रिक्त;
	संरचना oid_array idiff_prev = OID_ARRAY_INIT;
	संरचना strbuf idiff_title = STRBUF_INIT;
	ध्रुव वर्ण *rdiff_prev = रिक्त;
	संरचना strbuf rdiff1 = STRBUF_INIT;
	संरचना strbuf rdiff2 = STRBUF_INIT;
	संरचना strbuf rdiff_title = STRBUF_INIT;
	संरचना rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ध्रुव वर्ण *rfc = रिक्त;
	पूर्णाङ्क creation_factor = -1;
	ध्रुव वर्ण *signature = git_version_string;
	वर्ण *signature_to_free = रिक्त;
	वर्ण *signature_file_arg = रिक्त;
	संरचना keep_callback_data keep_callback_data = {
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
		OPT_INTEGER(0, "start-number", &start_संख्या,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_मोजणी, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, रिक्त,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_द्विमान_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, परिमाण(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, परिमाण(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	यदि (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	यदि (cfg.default_attach) {
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

	यदि (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	यदि (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	यदि (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	यदि (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		यदि (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		अन्यथा
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	यदि (reroll_मोजणी) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_मोजणी);
		rev.reroll_मोजणी = reroll_मोजणी;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	प्रति (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.वस्तुसंच[i].string);
		strbuf_addch(&buf, '\n');
	}

	यदि (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	प्रति (i = 0; i < cfg.extra_to.nr; i++) {
		यदि (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.वस्तुसंच[i].string);
		यदि (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	यदि (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	प्रति (i = 0; i < cfg.extra_cc.nr; i++) {
		यदि (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.वस्तुसंच[i].string);
		यदि (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, रिक्त);

	यदि (cfg.from) {
		यदि (split_ident_ओळ(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	यदि (start_संख्या < 0)
		start_संख्या = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	यदि (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	यदि (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	यदि (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	यदि (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	यदि (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	यदि (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	यदि (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	यदि (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	यदि (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	यदि (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	यदि (!rev.diffopt.flags.text && !no_द्विमान_diff)
		rev.diffopt.flags.द्विमान = 1;

	यदि (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	यदि (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	यदि (use_stdout) {
		setup_pager(the_repository);
	} अन्यथा यदि (!rev.diffopt.close_file) {
		पूर्णाङ्क saved;

		यदि (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		यदि (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		विकल्प (safe_create_leading_directories_const(the_repository, output_directory)) {
		पक्ष SCLD_OK:
		पक्ष SCLD_EXISTS:
			भङ्ग;
		स्वभाव:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		यदि (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	यदि (rev.pending.nr == 1) {
		पूर्णाङ्क check_head = 0;

		यदि (rev.max_मोजणी < 0 && !rev.show_मूळ_diff) {
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

		यदि (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		यदि (check_head) {
			ध्रुव वर्ण *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      रिक्त, रिक्त);
			यदि (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			अन्यथा
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_मूळ_diff = 1;

	यदि (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		यदि (rev.pending.nr == 2) {
			संरचना object_array_entry *o = rev.pending.objects;
			यदि (oideq(&o[0].item->oid, &o[1].item->oid))
				गच्छ done;
		}
		get_patch_ids(&rev, &ids);
	}

	यदि (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	यावत् ((commit = get_revision(&rev)) != रिक्त) {
		यदि (commit->object.flags & BOUNDARY) {
			boundary_मोजणी++;
			origin = (boundary_मोजणी == 1) ? commit : रिक्त;
			अनुवर्त;
		}

		यदि (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			अनुवर्त;

		commit_चळत_ढकला(&list, commit);
	}
	यदि (!list.nr)
		/* nothing to do */
		गच्छ done;
	total = list.nr;

	यदि (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		यदि (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} अन्यथा यदि (cover_letter == -1) {
		cover_letter = 1;
	}

	यदि (cover_letter == -1) {
		यदि (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		अन्यथा यदि ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		अन्यथा
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	यदि (!cfg.keep_subject && cfg.auto_संख्या && (total > 1 || cover_letter))
		cfg.numbered = 1;
	यदि (cfg.numbered)
		rev.total = total + start_संख्या - 1;

	यदि (idiff_prev.nr) {
		यदि (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_वृक्ष_oid(list.वस्तुसंच[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_मोजणी,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	यदि (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	अन्यथा यदि (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	यदि (rdiff_prev) {
		यदि (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.वस्तुसंच[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_मोजणी,
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
	यदि (!signature) {
		; /* --no-signature inhibits all signatures */
	} अन्यथा यदि (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} अन्यथा यदि (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		संरचना strbuf buf = STRBUF_INIT;
		ध्रुव वर्ण *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		यदि (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, रिक्त);
	} अन्यथा यदि (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, परिमाण(bases));
	base = get_base_commit(&cfg, list.वस्तुसंच, list.nr);
	यदि (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.वस्तुसंच, list.nr);
	}

	यदि (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(परिमाण(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	यदि (in_reply_to) {
		वर्ण *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	यदि (cover_letter) {
		यदि (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.वस्तुसंच,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_संख्या--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = रिक्त;
		rev.rdiff1 = रिक्त;
	}
	rev.add_signoff = cfg.do_signoff;

	यदि (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	यावत् (list.nr) {
		size_t idx = list.nr - 1;
		पूर्णाङ्क shown;

		display_progress(progress, total - idx);
		commit = commit_चळत_काढा(&list);
		rev.nr = total - idx + (start_संख्या - 1);

		/* Make the second and subsequent mails replies to the first */
		यदि (cfg.thread) {
			/* Have we already had a message ID? */
			यदि (rev.message_id) {
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
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					मुक्त(rev.message_id);
				अन्यथा
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		यदि (output_directory &&
		    open_next_file(rev.numbered_files ? रिक्त : commit, रिक्त, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_वृक्ष_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
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
	stop_progress(&progress);
	commit_चळत_clear(&list);
	यदि (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	मुक्त(description_file);
	मुक्त(signature_file_arg);
	मुक्त(signature_to_free);
	मुक्त(branch_name);
	मुक्त(to_free);
	मुक्त(rev.message_id);
	यदि (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	मुक्त(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क add_pending_commit(ध्रुव वर्ण *arg, संरचना rev_info *revs, पूर्णाङ्क flags)
{
	संरचना object_id oid;
	यदि (repo_get_oid(the_repository, arg, &oid) == 0) {
		संरचना commit *commit = lookup_commit_reference(the_repository,
								&oid);
		यदि (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
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
	प्रतिज्ञा(!commit->parents || !commit->parents->next);

	यदि (commit->parents)
		diff_वृक्ष_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	अन्यथा
		diff_मूळ_वृक्ष_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	प्रति (पूर्णाङ्क i = 0; i < q->nr; i++) {
		संरचना diff_filepair *p = q->queue[i];
		संरचना userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		यदि (drv && drv->द्विमान > 0)
			अनुवर्त;

		यदि (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, रिक्त,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_घाला(blobs, &p->one->oid);
		यदि (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, रिक्त,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_घाला(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

स्थिर पूर्णाङ्क always_match(ध्रुव शून्य *cmp_data UNUSED,
			ध्रुव संरचना hashmap_entry *entry1 UNUSED,
			ध्रुव संरचना hashmap_entry *entry2 UNUSED,
			ध्रुव शून्य *keydata UNUSED)
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
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	प्रति (संरचना commit_list *l = list; l; l = l->next) {
		संरचना commit *c = l->item;
		bool match_found = false;
		प्रति (संरचना patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		यदि (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	यदि (oidset_size(&blobs)) {
		संरचना oid_array to_fetch = OID_ARRAY_INIT;
		संरचना oidset_iter iter;
		ध्रुव संरचना object_id *oid;

		oidset_iter_init(&blobs, &iter);
		यावत् ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

पूर्णाङ्क cmd_cherry(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo UNUSED)
{
	संरचना rev_info revs;
	संरचना patch_ids ids;
	संरचना commit *commit;
	संरचना commit_list *list = रिक्त;
	संरचना branch *current_branch;
	ध्रुव वर्ण *upstream;
	ध्रुव वर्ण *head = "HEAD";
	ध्रुव वर्ण *limit = रिक्त;
	पूर्णाङ्क verbose = 0, abbrev = 0;

	संरचना option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	विकल्प (argc) {
	पक्ष 3:
		limit = argv[2];
		/* FALLTHROUGH */
	पक्ष 2:
		head = argv[1];
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

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	यदि (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	यदि (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	यदि (revs.pending.nr == 2) {
		संरचना object_array_entry *o = revs.pending.objects;
		यदि (oideq(&o[0].item->oid, &o[1].item->oid))
			प्रत्यागम 0;
	}

	get_patch_ids(&revs, &ids);

	यदि (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	यावत् ((commit = get_revision(&revs)) != रिक्त) {
		commit_list_घाला(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	प्रति (संरचना commit_list *l = list; l; l = l->next) {
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
