/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "odb/streaming.h"
#समावेश "pager.h"
#समावेश "color.h"
#समावेश "commit.h"
#समावेश "diff.h"
#समावेश "diffcore.h"
#समावेश "diff-merges.h"
#समावेश "revision.h"
#समावेश "log-tree.h"
#समावेश "oid-array.h"
#समावेश "oidset.h"
#समावेश "tag.h"
#समावेश "reflog-walk.h"
#समावेश "patch-ids.h"
#समावेश "path.h"
#समावेश "shortlog.h"
#समावेश "remote.h"
#समावेश "string-list.h"
#समावेश "parse-options.h"
#समावेश "line-log.h"
#समावेश "branch.h"
#समावेश "version.h"
#समावेश "mailmap.h"
#समावेश "progress.h"
#समावेश "commit-slab.h"
#समावेश "advice.h"
#समावेश "utf8.h"

#समावेश "commit-reach.h"
#समावेश "promisor-remote.h"
#समावेश "range-diff.h"
#समावेश "tmp-objdir.h"
#समावेश "tree.h"
#समावेश "userdiff.h"
#समावेश "write-or-die.h"

#घोषणा MAIL_DEFAULT_WRAP 72
#घोषणा COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#घोषणा FORMAT_PATCH_NAME_MAX_DEFAULT 64

अटल अचिन्हित पूर्णांक force_in_body_from;
अटल पूर्णांक stdout_mboxrd;
अटल पूर्णांक format_no_prefix;

अटल स्थिर अक्षर * स्थिर builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	शून्य
};

काष्ठा पंक्ति_opt_callback_तथ्य {
	काष्ठा rev_info *rev;
	स्थिर अक्षर *prefix;
	काष्ठा string_list args;
};

अटल पूर्णांक session_is_interactive(व्योम)
{
	वापस isatty(1) || pager_in_use();
}

अटल पूर्णांक auto_decoration_style(व्योम)
{
	वापस session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

अटल पूर्णांक parse_decoration_style(स्थिर अक्षर *मूल्य)
{
	चयन (git_parse_maybe_bool(मूल्य)) {
	हाल 1:
		वापस DECORATE_SHORT_REFS;
	हाल 0:
		वापस 0;
	शेष:
		अवरोध;
	}
	अगर (!म_भेद(मूल्य, "full"))
		वापस DECORATE_FULL_REFS;
	अन्यथा अगर (!म_भेद(मूल्य, "short"))
		वापस DECORATE_SHORT_REFS;
	अन्यथा अगर (!म_भेद(मूल्य, "auto"))
		वापस auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	वापस -1;
}

काष्ठा log_config {
	पूर्णांक default_abbrev_commit;
	पूर्णांक default_show_मूल;
	पूर्णांक default_follow;
	पूर्णांक default_show_signature;
	पूर्णांक default_encode_email_headers;
	पूर्णांक decoration_style;
	पूर्णांक decoration_given;
	पूर्णांक use_mailmap_config;
	अक्षर *fmt_patch_subject_prefix;
	पूर्णांक fmt_patch_name_max;
	अक्षर *fmt_pretty;
	अक्षर *default_date_mode;

#अगर_अघोषित WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	पूर्णांक i_still_use_this;
#पूर्ण_अगर
};

अटल व्योम log_config_init(काष्ठा log_config *cfg)
{
	स_रखो(cfg, 0, माप(*cfg));
	cfg->default_show_मूल = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

अटल व्योम log_config_release(काष्ठा log_config *cfg)
{
	मुक्त(cfg->default_date_mode);
	मुक्त(cfg->fmt_pretty);
	मुक्त(cfg->fmt_patch_subject_prefix);
}

अटल पूर्णांक use_default_decoration_filter = 1;
अटल काष्ठा string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
अटल काष्ठा string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
अटल काष्ठा string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

अटल पूर्णांक clear_decorations_callback(स्थिर काष्ठा option *opt UNUSED,
				      स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	वापस 0;
}

अटल पूर्णांक decorate_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			     पूर्णांक unset)
{
	काष्ठा log_config *cfg = opt->मूल्य;

	अगर (unset)
		cfg->decoration_style = 0;
	अन्यथा अगर (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	अन्यथा
		cfg->decoration_style = DECORATE_SHORT_REFS;

	अगर (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	वापस 0;
}

अटल पूर्णांक log_पंक्ति_range_callback(स्थिर काष्ठा option *option, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा पंक्ति_opt_callback_तथ्य *तथ्य = option->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!arg)
		वापस -1;

	तथ्य->rev->पंक्ति_level_traverse = 1;
	string_list_append(&तथ्य->args, arg);

	वापस 0;
}

अटल व्योम cmd_log_init_defaults(काष्ठा rev_info *rev,
				  काष्ठा log_config *cfg)
{
	अगर (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	अगर (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_मूल_diff = cfg->default_show_मूल;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	अगर (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

अटल व्योम set_default_decoration_filter(काष्ठा decoration_filter *decoration_filter)
{
	अक्षर *मूल्य = शून्य;
	काष्ठा string_list *include = decoration_filter->include_ref_pattern;
	स्थिर काष्ठा string_list *config_exclude;

	अगर (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		काष्ठा string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	अगर (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &मूल्य) &&
	    !म_भेद("all", मूल्य))
		use_default_decoration_filter = 0;
	मुक्त(मूल्य);

	अगर (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		वापस;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(ref_namespace); i++) {
		अगर (!ref_namespace[i].decoration)
			जारी;

		string_list_append(include, ref_namespace[i].ref);
	}
}

अटल व्योम cmd_log_init_finish(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा rev_info *rev, काष्ठा setup_revision_opt *opt,
			 काष्ठा log_config *cfg)
{
	काष्ठा userformat_want w;
	पूर्णांक quiet = 0, source = 0, mailmap;
	अटल काष्ठा पंक्ति_opt_callback_तथ्य पंक्ति_cb = {शून्य, शून्य, STRING_LIST_INIT_DUP};
	काष्ठा decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	अटल काष्ठा revision_sources revision_sources;

	स्थिर काष्ठा option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#अगर_अघोषित WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#पूर्ण_अगर
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", शून्य, शून्य,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, शून्य, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', शून्य, &पंक्ति_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_पंक्ति_range_callback),
		OPT_END()
	};

	पंक्ति_cb.rev = rev;
	पंक्ति_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	अगर (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	अगर (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	अगर (rev->पंक्ति_level_traverse && rev->prune_तथ्य.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	स_रखो(&w, 0, माप(w));
	userformat_find_requirements(शून्य, &w);

	अगर (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	अगर (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	अगर ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	अगर (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	अगर (mailmap) {
		rev->mailmap = xmalloc(माप(काष्ठा string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	अगर (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		अगर (!cfg->decoration_given)
			cfg->decoration_style = 0;
		अगर (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	अगर (rev->commit_format == CMIT_FMT_USERFORMAT) {
		अगर (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} अन्यथा अगर (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	अगर (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		अगर (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	अगर (rev->पंक्ति_level_traverse)
		पंक्ति_log_init(rev, पंक्ति_cb.prefix, &पंक्ति_cb.args);

	setup_pager(the_repository);
}

अटल व्योम cmd_log_init(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा rev_info *rev, काष्ठा setup_revision_opt *opt,
			 काष्ठा log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

अटल पूर्णांक cmd_log_walk_no_free(काष्ठा rev_info *rev)
{
	काष्ठा commit *commit;
	पूर्णांक saved_nrl = 0;
	पूर्णांक saved_dcctc = 0;
	पूर्णांक परिणाम;

	अगर (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	जबतक ((commit = get_revision(rev)) != शून्य) {
		अगर (!log_वृक्ष_commit(rev, commit) && rev->max_गिनती >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_गिनती++;
		अगर (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = शून्य;
		}
		अगर (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		अगर (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	परिणाम = diff_परिणाम_code(rev);
	अगर (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		परिणाम = 02;
	}
	वापस परिणाम;
}

अटल पूर्णांक cmd_log_walk(काष्ठा rev_info *rev)
{
	पूर्णांक retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	वापस retval;
}

अटल पूर्णांक git_log_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			  स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा log_config *cfg = cb;
	स्थिर अक्षर *slot_name;

	अगर (!म_भेद(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		वापस git_config_string(&cfg->fmt_pretty, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		वापस git_config_string(&cfg->fmt_patch_subject_prefix, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, मूल्य, ctx->kvi);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		वापस git_config_string(&cfg->default_date_mode, var, मूल्य);
	}
	अगर (!म_भेद(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(मूल्य);
		अगर (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		वापस 0;
	}
	अगर (!म_भेद(var, "log.diffmerges")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		वापस diff_merges_config(मूल्य);
	}
	अगर (!म_भेद(var, "log.showroot")) {
		cfg->default_show_मूल = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (skip_prefix(var, "color.decorate.", &slot_name))
		वापस parse_decorate_color_config(var, slot_name, मूल्य);
	अगर (!म_भेद(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, मूल्य);
		वापस 0;
	}

	वापस git_diff_ui_config(var, मूल्य, ctx, cb);
}

#अगर_अघोषित WITH_BREAKING_CHANGES
पूर्णांक cmd_whatchanged(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo UNUSED)
{
	काष्ठा log_config cfg;
	काष्ठा rev_info rev;
	काष्ठा setup_revision_opt opt;
	पूर्णांक ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	स_रखो(&opt, 0, माप(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	अगर (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	अगर (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	वापस ret;
}
#पूर्ण_अगर

अटल व्योम show_tagger(स्थिर अक्षर *buf, काष्ठा rev_info *rev)
{
	काष्ठा strbuf out = STRBUF_INIT;
	काष्ठा pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	ख_लिखो(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

अटल पूर्णांक show_blob_object(स्थिर काष्ठा object_id *oid, काष्ठा rev_info *rev, स्थिर अक्षर *obj_name)
{
	काष्ठा object_id oidc;
	काष्ठा object_context obj_context = {0};
	अक्षर *buf;
	अचिन्हित दीर्घ आकार;

	ख_साफ(rev->diffopt.file);
	अगर (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		वापस odb_stream_blob_to_fd(the_repository->objects, 1, oid, शून्य, 0);

	अगर (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	अगर (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &आकार)) {
		object_context_release(&obj_context);
		वापस odb_stream_blob_to_fd(the_repository->objects, 1, oid, शून्य, 0);
	}

	अगर (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, आकार);
	object_context_release(&obj_context);
	मुक्त(buf);
	वापस 0;
}

अटल पूर्णांक show_tag_object(स्थिर काष्ठा object_id *oid, काष्ठा rev_info *rev)
{
	माप_प्रकार आकार;
	क्रमागत object_type type;
	अक्षर *buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	अचिन्हित दीर्घ offset = 0;

	अगर (!buf)
		वापस error(_("could not read object %s"), oid_to_hex(oid));

	निश्चित(type == OBJ_TAG);
	जबतक (offset < आकार && buf[offset] != '\n') {
		अचिन्हित दीर्घ new_offset = offset + 1;
		स्थिर अक्षर *ident;
		जबतक (new_offset < आकार && buf[new_offset++] != '\n')
			; /* do nothing */
		अगर (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	अगर (offset < आकार)
		ख_डालो(buf + offset, आकार - offset, 1, rev->diffopt.file);
	मुक्त(buf);
	वापस 0;
}

अटल पूर्णांक show_वृक्ष_object(स्थिर काष्ठा object_id *oid UNUSED,
			    काष्ठा strbuf *base UNUSED,
			    स्थिर अक्षर *pathname, अचिन्हित mode,
			    व्योम *context)
{
	खाता *file = context;
	ख_लिखो(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	वापस 0;
}

अटल व्योम show_setup_revisions_tweak(काष्ठा rev_info *rev)
{
	अगर (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	अन्यथा
		diff_merges_default_to_dense_combined(rev);
	अगर (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

पूर्णांक cmd_show(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repo UNUSED)
{
	काष्ठा log_config cfg;
	काष्ठा rev_info rev;
	अचिन्हित पूर्णांक i;
	काष्ठा setup_revision_opt opt;
	काष्ठा pathspec match_all;
	पूर्णांक ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	अगर (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_सूचकांक = 0;
	}

	स_रखो(&match_all, 0, माप(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	स_रखो(&opt, 0, माप(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	अगर (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		वापस ret;
	}

	rev.diffopt.no_free = 1;
	क्रम (i = 0; i < rev.pending.nr && !ret; i++) {
		काष्ठा object *o = rev.pending.objects[i].item;
		स्थिर अक्षर *name = rev.pending.objects[i].name;
		चयन (o->type) {
		हाल OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			अवरोध;
		हाल OBJ_TAG: {
			काष्ठा tag *t = (काष्ठा tag *)o;
			काष्ठा object_id *oid = get_tagged_oid(t);

			अगर (rev.shown_one)
				अक्षर_दो('\n');
			ख_लिखो(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			अगर (ret)
				अवरोध;
			o = parse_object(the_repository, oid);
			अगर (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			अवरोध;
		}
		हाल OBJ_वृक्ष:
			अगर (rev.shown_one)
				अक्षर_दो('\n');
			ख_लिखो(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_वृक्ष(the_repository, (काष्ठा वृक्ष *)o,
				  &match_all, show_वृक्ष_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			अवरोध;
		हाल OBJ_COMMIT:
		{
			काष्ठा object_array old;
			काष्ठा object_array blank = OBJECT_ARRAY_INIT;

			स_नकल(&old, &rev.pending, माप(old));
			स_नकल(&rev.pending, &blank, माप(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			स_नकल(&rev.pending, &old, माप(rev.pending));
			अवरोध;
		}
		शेष:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	वापस ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
पूर्णांक cmd_log_reflog(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	काष्ठा log_config cfg;
	काष्ठा rev_info rev;
	काष्ठा setup_revision_opt opt;
	पूर्णांक ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	स_रखो(&opt, 0, माप(opt));
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
	वापस ret;
}

अटल व्योम log_setup_revisions_tweak(काष्ठा rev_info *rev)
{
	अगर (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_तथ्य, 0))
		rev->diffopt.flags.follow_renames = 1;

	अगर (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

पूर्णांक cmd_log(पूर्णांक argc,
	    स्थिर अक्षर **argv,
	    स्थिर अक्षर *prefix,
	    काष्ठा repository *repo UNUSED)
{
	काष्ठा log_config cfg;
	काष्ठा rev_info rev;
	काष्ठा setup_revision_opt opt;
	पूर्णांक ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	स_रखो(&opt, 0, माप(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	वापस ret;
}

/* format-patch */

क्रमागत cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

क्रमागत thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

क्रमागत cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

क्रमागत auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

काष्ठा format_config {
	काष्ठा log_config log;
	क्रमागत thread_level thread;
	पूर्णांक do_signoff;
	क्रमागत auto_base_setting auto_base;
	अक्षर *base_commit;
	अक्षर *from;
	अक्षर *signature;
	अक्षर *signature_file;
	क्रमागत cover_setting config_cover_letter;
	अक्षर *fmt_cover_letter_commit_list;
	अक्षर *config_output_directory;
	क्रमागत cover_from_description cover_from_description_mode;
	पूर्णांक show_notes;
	काष्ठा display_notes_opt notes_opt;
	पूर्णांक numbered_cmdline_opt;
	पूर्णांक numbered;
	पूर्णांक auto_संख्या;
	अक्षर *default_attach;
	काष्ठा string_list extra_hdr;
	काष्ठा string_list extra_to;
	काष्ठा string_list extra_cc;
	पूर्णांक keep_subject;
	पूर्णांक subject_prefix;
	काष्ठा strbuf sprefix;
	अक्षर *fmt_patch_suffix;
};

अटल व्योम format_config_init(काष्ठा format_config *cfg)
{
	स_रखो(cfg, 0, माप(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_संख्या = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

अटल व्योम format_config_release(काष्ठा format_config *cfg)
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

अटल क्रमागत cover_from_description parse_cover_from_description(स्थिर अक्षर *arg)
{
	अगर (!arg || !म_भेद(arg, "default"))
		वापस COVER_FROM_MESSAGE;
	अन्यथा अगर (!म_भेद(arg, "none"))
		वापस COVER_FROM_NONE;
	अन्यथा अगर (!म_भेद(arg, "message"))
		वापस COVER_FROM_MESSAGE;
	अन्यथा अगर (!म_भेद(arg, "subject"))
		वापस COVER_FROM_SUBJECT;
	अन्यथा अगर (!म_भेद(arg, "auto"))
		वापस COVER_FROM_AUTO;
	अन्यथा
		die(_("%s: invalid cover from description mode"), arg);
}

अटल व्योम add_header(काष्ठा format_config *cfg, स्थिर अक्षर *मूल्य)
{
	काष्ठा string_list_item *item;
	पूर्णांक len = म_माप(मूल्य);
	जबतक (len && मूल्य[len - 1] == '\n')
		len--;

	अगर (!strncasecmp(मूल्य, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, मूल्य + 4);
		len -= 4;
	} अन्यथा अगर (!strncasecmp(मूल्य, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, मूल्य + 4);
		len -= 4;
	} अन्यथा {
		item = string_list_append(&cfg->extra_hdr, मूल्य);
	}

	item->string[len] = '\0';
}

अटल पूर्णांक git_format_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			     स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	काष्ठा format_config *cfg = cb;

	अगर (!म_भेद(var, "format.headers")) {
		अगर (!मूल्य)
			die(_("format.headers without value"));
		add_header(cfg, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		वापस git_config_string(&cfg->fmt_patch_suffix, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.to")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		string_list_append(&cfg->extra_to, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.cc")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "diff.color") || !म_भेद(var, "color.diff") ||
	    !म_भेद(var, "color.ui") || !म_भेद(var, "diff.submodule")) {
		वापस 0;
	}
	अगर (!म_भेद(var, "format.numbered")) {
		अगर (मूल्य && !strcasecmp(मूल्य, "auto")) {
			cfg->auto_संख्या = 1;
			वापस 0;
		}
		cfg->numbered = git_config_bool(var, मूल्य);
		cfg->auto_संख्या = cfg->auto_संख्या && cfg->numbered;
		वापस 0;
	}
	अगर (!म_भेद(var, "format.attach")) {
		अगर (मूल्य && *मूल्य) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(मूल्य);
		} अन्यथा अगर (मूल्य && !*मूल्य) {
			FREE_AND_NULL(cfg->default_attach);
		} अन्यथा {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		वापस 0;
	}
	अगर (!म_भेद(var, "format.thread")) {
		अगर (मूल्य && !strcasecmp(मूल्य, "deep")) {
			cfg->thread = THREAD_DEEP;
			वापस 0;
		}
		अगर (मूल्य && !strcasecmp(मूल्य, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			वापस 0;
		}
		cfg->thread = git_config_bool(var, मूल्य) ? THREAD_SHALLOW : THREAD_UNSET;
		वापस 0;
	}
	अगर (!म_भेद(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		वापस git_config_string(&cfg->signature, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		वापस git_config_pathname(&cfg->signature_file, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.coverletter")) {
		अगर (मूल्य && !strcasecmp(मूल्य, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			वापस 0;
		}
		cfg->config_cover_letter = git_config_bool(var, मूल्य) ? COVER_ON : COVER_OFF;
		वापस 0;
	}
	अगर (!म_भेद(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		वापस git_config_string(&cfg->fmt_cover_letter_commit_list, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		वापस git_config_string(&cfg->config_output_directory, var, मूल्य);
	}
	अगर (!म_भेद(var, "format.useautobase")) {
		अगर (मूल्य && !strcasecmp(मूल्य, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			वापस 0;
		}
		cfg->auto_base = git_config_bool(var, मूल्य) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		वापस 0;
	}
	अगर (!म_भेद(var, "format.from")) {
		पूर्णांक b = git_parse_maybe_bool(मूल्य);
		FREE_AND_NULL(cfg->from);
		अगर (b < 0)
			cfg->from = xstrdup(मूल्य);
		अन्यथा अगर (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		वापस 0;
	}
	अगर (!म_भेद(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.notes")) {
		पूर्णांक b = git_parse_maybe_bool(मूल्य);
		अगर (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, मूल्य);
		अन्यथा अगर (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		अन्यथा
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(मूल्य);
		अगर (format_no_prefix < 0) {
			पूर्णांक status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				मूल्य, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			निकास(status);
		}
		वापस 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	अगर (!म_भेद(var, "diff.noprefix"))
		वापस 0;

	वापस git_log_config(var, मूल्य, ctx, &cfg->log);
}

अटल स्थिर अक्षर *output_directory = शून्य;
अटल पूर्णांक outdir_offset;

अटल पूर्णांक open_अग्र_file(काष्ठा commit *commit, स्थिर अक्षर *subject,
			 काष्ठा rev_info *rev, पूर्णांक quiet)
{
	काष्ठा strbuf filename = STRBUF_INIT;

	अगर (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	अगर (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	अन्यथा अगर (commit)
		fmt_output_commit(&filename, commit, rev);
	अन्यथा
		fmt_output_subject(&filename, subject, rev);

	अगर (!quiet)
		म_लिखो("%s\n", filename.buf + outdir_offset);

	अगर (!(rev->diffopt.file = ख_खोलो(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		वापस -1;
	}

	strbuf_release(&filename);
	वापस 0;
}

अटल व्योम get_patch_ids(काष्ठा rev_info *rev, काष्ठा patch_ids *ids)
{
	काष्ठा rev_info check_rev;
	काष्ठा commit *commit, *c1, *c2;
	काष्ठा object *o1, *o2;
	अचिन्हित flags1, flags2;

	अगर (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	अगर ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	अगर (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	जबतक ((commit = get_revision(&check_rev)) != शून्य) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

अटल व्योम gen_message_id(काष्ठा rev_info *info, स्थिर अक्षर *base)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) समय(शून्य),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, शून्य);
}

अटल व्योम print_signature(स्थिर अक्षर *signature, खाता *file)
{
	अगर (!signature || !*signature)
		वापस;

	ख_लिखो(file, "-- \n%s", signature);
	अगर (signature[म_माप(signature)-1] != '\n')
		अ_दो('\n', file);
	अ_दो('\n', file);
}

अटल अक्षर *find_branch_name(काष्ठा rev_info *rev)
{
	काष्ठा object_id branch_oid;
	स्थिर काष्ठा object_id *tip_oid;
	स्थिर अक्षर *ref, *v;
	अक्षर *full_ref, *branch = शून्य;
	पूर्णांक interesting_found = 0;
	माप_प्रकार idx;

	क्रम (माप_प्रकार i = 0; i < rev->cmdline.nr; i++) {
		अगर (rev->cmdline.rev[i].flags & UNINTERESTING)
			जारी;
		अगर (interesting_found)
			वापस शून्य;
		interesting_found = 1;
		idx = i;
	}
	अगर (!interesting_found)
		वापस शून्य;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	अगर (repo_dwim_ref(the_repository, ref, म_माप(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	मुक्त(full_ref);
	वापस branch;
}

अटल व्योम show_diffstat(काष्ठा rev_info *rev,
			  काष्ठा commit *origin, काष्ठा commit *शीर्ष)
{
	काष्ठा diff_options opts;

	स_नकल(&opts, &rev->diffopt, माप(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_वृक्ष_oid(get_commit_वृक्ष_oid(origin),
		      get_commit_वृक्ष_oid(शीर्ष),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	ख_लिखो(rev->diffopt.file, "\n");
}

अटल व्योम read_desc_file(काष्ठा strbuf *buf, स्थिर अक्षर *desc_file)
{
	अगर (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

अटल व्योम prepare_cover_text(काष्ठा pretty_print_context *pp,
			       स्थिर अक्षर *description_file,
			       स्थिर अक्षर *branch_name,
			       काष्ठा strbuf *sb,
			       स्थिर अक्षर *encoding,
			       पूर्णांक need_8bit_cte,
			       स्थिर काष्ठा format_config *cfg)
{
	स्थिर अक्षर *subject = "*** SUBJECT HERE ***";
	स्थिर अक्षर *body = "*** BLURB HERE ***";
	काष्ठा strbuf description_sb = STRBUF_INIT;
	काष्ठा strbuf subject_sb = STRBUF_INIT;

	अगर (cfg->cover_from_description_mode == COVER_FROM_NONE)
		जाओ do_pp;

	अगर (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	अन्यथा अगर (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	अगर (!description_sb.len)
		जाओ do_pp;

	अगर (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	अगर (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
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

काष्ठा rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	काष्ठा string_list notes;
};

अटल पूर्णांक rdiff_notes_cb(स्थिर काष्ठा option *option,
			  स्थिर अक्षर *arg,
			  पूर्णांक unset)
{
	काष्ठा option opt = *option;
	काष्ठा rdiff_notes *rdiff_notes = option->मूल्य;

	rdiff_notes->override = 1;
	opt.मूल्य = &rdiff_notes->notes;
	वापस parse_opt_string_list(&opt, arg, unset);
}

अटल पूर्णांक get_notes_refs(काष्ठा string_list_item *item, व्योम *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	वापस 0;
}

अटल व्योम get_notes_args(काष्ठा rdiff_notes *rdiff_notes,
			   काष्ठा rev_info *rev)
{
	अगर (rdiff_notes->override) {
		अगर (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		अन्यथा
			strvec_धकेलो(&rev->rdiff_log_arg, "--no-notes");
	} अन्यथा अगर (!rev->show_notes) {
		strvec_धकेलो(&rev->rdiff_log_arg, "--no-notes");
	} अन्यथा अगर (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_धकेलो(&rev->rdiff_log_arg, "--notes");
	} अन्यथा {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

अटल व्योम generate_shortlog_cover_letter(काष्ठा shortlog *log,
					   काष्ठा rev_info *rev,
					   काष्ठा commit **list,
					   पूर्णांक nr)
{
	shortlog_init(log);
	log->wrap_पंक्तियाँ = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	क्रम (पूर्णांक i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

अटल व्योम generate_commit_list_cover(खाता *cover_file, स्थिर अक्षर *format,
				       काष्ठा commit **list, पूर्णांक n)
{
	काष्ठा strbuf commit_पंक्ति = STRBUF_INIT;
	काष्ठा pretty_print_context ctx = {0};
	काष्ठा rev_info rev = REV_INFO_INIT;

	rev.योग = n;
	ctx.rev = &rev;
	क्रम (पूर्णांक i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_पंक्ति, &ctx);
		ख_लिखो(cover_file, "%s\n", commit_पंक्ति.buf);
		strbuf_reset(&commit_पंक्ति);
	}
	ख_लिखो(cover_file, "\n");

	strbuf_release(&commit_पंक्ति);
}

अटल व्योम make_cover_letter(काष्ठा rev_info *rev, पूर्णांक use_separate_file,
			      काष्ठा commit *origin,
			      पूर्णांक nr, काष्ठा commit **list,
			      स्थिर अक्षर *description_file,
			      स्थिर अक्षर *branch_name,
			      पूर्णांक quiet,
			      स्थिर काष्ठा format_config *cfg,
			      स्थिर अक्षर *format)
{
	स्थिर अक्षर *from;
	काष्ठा shortlog log;
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक i;
	स्थिर अक्षर *encoding = "UTF-8";
	पूर्णांक need_8bit_cte = 0;
	काष्ठा pretty_print_context pp = {0};
	काष्ठा commit *शीर्ष = list[0];
	अक्षर *to_free = शून्य;

	अगर (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	अगर (use_separate_file &&
	    open_अग्र_file(शून्य, rev->numbered_files ? शून्य : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, शीर्ष, &pp.after_subject, &need_8bit_cte, 0);

	क्रम (i = 0; !need_8bit_cte && i < nr; i++) {
		स्थिर अक्षर *buf = repo_get_commit_बफर(the_repository,
							 list[i], शून्य);
		अगर (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_बफर(the_repository, list[i], buf);
	}

	अगर (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, शून्य, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	ख_लिखो(rev->diffopt.file, "%s\n", sb.buf);

	मुक्त(to_free);
	मुक्त(pp.after_subject);
	strbuf_release(&sb);

	अगर (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	अन्यथा अगर (!म_भेद(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	अन्यथा अगर (!म_भेद(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	अन्यथा अगर (म_अक्षर(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	अन्यथा
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	अगर (origin)
		show_diffstat(rev, origin, शीर्ष);

	अगर (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	अगर (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		काष्ठा diff_options opts;
		काष्ठा range_diff_options range_diff_opts = {
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

अटल अक्षर *clean_message_id(स्थिर अक्षर *संदेश_id)
{
	अक्षर ch;
	स्थिर अक्षर *a, *z, *m;

	m = संदेश_id;
	जबतक ((ch = *m) && (है_खाली(ch) || (ch == '<')))
		m++;
	a = m;
	z = शून्य;
	जबतक ((ch = *m)) {
		अगर (!है_खाली(ch) && (ch != '>'))
			z = m;
		m++;
	}
	अगर (!z)
		die(_("insane in-reply-to: %s"), संदेश_id);
	अगर (++z == m)
		वापस xstrdup(a);
	वापस xmemdupz(a, z - a);
}

अटल स्थिर अक्षर *set_outdir(स्थिर अक्षर *prefix, स्थिर अक्षर *output_directory)
{
	अगर (output_directory && is_absolute_path(output_directory))
		वापस output_directory;

	अगर (!prefix || !*prefix) {
		अगर (output_directory)
			वापस output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		वापस "./";
	}

	outdir_offset = म_माप(prefix);
	अगर (!output_directory)
		वापस prefix;

	वापस prefix_filename(prefix, output_directory);
}

अटल स्थिर अक्षर * स्थिर builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	शून्य
};

काष्ठा keep_callback_तथ्य {
	काष्ठा format_config *cfg;
	काष्ठा rev_info *revs;
};

अटल पूर्णांक keep_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा keep_callback_तथ्य *तथ्य = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	तथ्य->revs->योग = -1;
	तथ्य->cfg->keep_subject = 1;
	वापस 0;
}

अटल पूर्णांक subject_prefix_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			    पूर्णांक unset)
{
	काष्ठा format_config *cfg = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	वापस 0;
}

अटल पूर्णांक rfc_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			पूर्णांक unset)
{
	स्थिर अक्षर **rfc = opt->मूल्य;

	*rfc = opt->मूल्य;
	अगर (unset)
		*rfc = शून्य;
	अन्यथा
		*rfc = arg ? arg : "RFC";
	वापस 0;
}

अटल पूर्णांक numbered_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			     पूर्णांक unset)
{
	काष्ठा format_config *cfg = opt->मूल्य;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	अगर (unset)
		cfg->auto_संख्या =  0;
	वापस 0;
}

अटल पूर्णांक no_numbered_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
				पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);
	वापस numbered_callback(opt, arg, 1);
}

अटल पूर्णांक output_directory_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			      पूर्णांक unset)
{
	स्थिर अक्षर **dir = (स्थिर अक्षर **)opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	अगर (*dir)
		die(_("two output directories?"));
	*dir = arg;
	वापस 0;
}

अटल पूर्णांक thread_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा format_config *cfg = opt->मूल्य;

	अगर (unset)
		cfg->thread = THREAD_UNSET;
	अन्यथा अगर (!arg || !म_भेद(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	अन्यथा अगर (!म_भेद(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	अन्यथा
		वापस 1;
	वापस 0;
}

अटल पूर्णांक attach_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा rev_info *rev = (काष्ठा rev_info *)opt->मूल्य;
	अगर (unset)
		rev->mime_boundary = शून्य;
	अन्यथा अगर (arg)
		rev->mime_boundary = arg;
	अन्यथा
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	वापस 0;
}

अटल पूर्णांक inline_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा rev_info *rev = (काष्ठा rev_info *)opt->मूल्य;
	अगर (unset)
		rev->mime_boundary = शून्य;
	अन्यथा अगर (arg)
		rev->mime_boundary = arg;
	अन्यथा
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	वापस 0;
}

अटल पूर्णांक header_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			   पूर्णांक unset)
{
	काष्ठा format_config *cfg = opt->मूल्य;

	अगर (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} अन्यथा {
		add_header(cfg, arg);
	}
	वापस 0;
}

अटल पूर्णांक from_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	अक्षर **from = opt->मूल्य;

	मुक्त(*from);

	अगर (unset)
		*from = शून्य;
	अन्यथा अगर (arg)
		*from = xstrdup(arg);
	अन्यथा
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	वापस 0;
}

अटल पूर्णांक base_callback(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा format_config *cfg = opt->मूल्य;

	अगर (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} अन्यथा अगर (!म_भेद(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} अन्यथा {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	वापस 0;
}

काष्ठा base_वृक्ष_info {
	काष्ठा object_id base_commit;
	पूर्णांक nr_patch_id, alloc_patch_id;
	काष्ठा object_id *patch_id;
};

अटल काष्ठा commit *get_base_commit(स्थिर काष्ठा format_config *cfg,
				      काष्ठा commit **list,
				      माप_प्रकार योग)
{
	काष्ठा commit *base = शून्य;
	काष्ठा commit **rev;
	पूर्णांक auto_select, die_on_failure, ret;
	माप_प्रकार i = 0, rev_nr = 0;

	चयन (cfg->auto_base) {
	हाल AUTO_BASE_NEVER:
		अगर (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} अन्यथा {
			/* no base information is requested */
			वापस शून्य;
		}
		अवरोध;
	हाल AUTO_BASE_ALWAYS:
	हाल AUTO_BASE_WHEN_ABLE:
		अगर (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} अन्यथा {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		अवरोध;
	शेष:
		BUG("unexpected automatic base selection method");
	}

	अगर (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		अगर (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} अन्यथा {
		काष्ठा branch *curr_branch = branch_get(शून्य);
		स्थिर अक्षर *upstream = branch_get_upstream(curr_branch, शून्य);
		अगर (upstream) {
			काष्ठा commit_list *base_list = शून्य;
			काष्ठा commit *commit;
			काष्ठा object_id oid;

			अगर (repo_get_oid(the_repository, upstream, &oid)) {
				अगर (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				अन्यथा
					वापस शून्य;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			अगर (repo_get_merge_bases_many(the_repository,
						      commit, योग,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->अग्र) {
				अगर (die_on_failure) {
					die(_("could not find exact merge base"));
				} अन्यथा {
					commit_list_free(base_list);
					वापस शून्य;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} अन्यथा {
			अगर (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			अन्यथा
				वापस शून्य;
		}
	}

	ALLOC_ARRAY(rev, योग);
	क्रम (i = 0; i < योग; i++)
		rev[i] = list[i];

	rev_nr = योग;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	जबतक (rev_nr > 1) {
		क्रम (i = 0; i < rev_nr / 2; i++) {
			काष्ठा commit_list *merge_base = शून्य;
			अगर (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->अग्र) {
				अगर (die_on_failure) {
					die(_("failed to find exact merge base"));
				} अन्यथा {
					commit_list_free(merge_base);
					मुक्त(rev);
					वापस शून्य;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		अगर (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	अगर (ret < 0)
		निकास(128);
	अगर (!ret) {
		अगर (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} अन्यथा {
			मुक्त(rev);
			वापस शून्य;
		}
	}

	क्रम (i = 0; i < योग; i++) {
		अगर (base == list[i]) {
			अगर (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} अन्यथा {
				मुक्त(rev);
				वापस शून्य;
			}
		}
	}

	मुक्त(rev);
	वापस base;
}

define_commit_slab(commit_base, पूर्णांक);

अटल व्योम prepare_bases(काष्ठा base_वृक्ष_info *bases,
			  काष्ठा commit *base,
			  काष्ठा commit **list,
			  माप_प्रकार योग)
{
	काष्ठा commit *commit;
	काष्ठा rev_info revs;
	काष्ठा diff_options diffopt;
	काष्ठा commit_base commit_base;

	अगर (!base)
		वापस;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, शून्य);
	revs.max_parents = 1;
	revs.topo_order = 1;
	क्रम (माप_प्रकार i = 0; i < योग; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	अगर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	जबतक ((commit = get_revision(&revs)) != शून्य) {
		काष्ठा object_id oid;
		काष्ठा object_id *patch_id;
		अगर (*commit_base_at(&commit_base, commit))
			जारी;
		अगर (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

अटल व्योम print_bases(काष्ठा base_वृक्ष_info *bases, खाता *file)
{
	पूर्णांक i;

	/* Only do this once, either for the cover or for the first one */
	अगर (is_null_oid(&bases->base_commit))
		वापस;

	/* Show the base commit */
	ख_लिखो(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	क्रम (i = bases->nr_patch_id - 1; i >= 0; i--)
		ख_लिखो(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	मुक्त(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

अटल स्थिर अक्षर *diff_title(काष्ठा strbuf *sb,
			      स्थिर अक्षर *reroll_गिनती,
			      स्थिर अक्षर *generic,
			      स्थिर अक्षर *rerolled)
{
	पूर्णांक v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	अगर (reroll_गिनती && !strtol_i(reroll_गिनती, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	अन्यथा
		strbuf_addstr(sb, generic);
	वापस sb->buf;
}

अटल व्योम infer_range_diff_ranges(काष्ठा strbuf *r1,
				    काष्ठा strbuf *r2,
				    स्थिर अक्षर *पूर्व,
				    काष्ठा commit *origin,
				    काष्ठा commit *शीर्ष)
{
	स्थिर अक्षर *शीर्ष_oid = oid_to_hex(&शीर्ष->object.oid);
	पूर्णांक पूर्व_is_range = is_range_diff_range(पूर्व);

	अगर (पूर्व_is_range)
		strbuf_addstr(r1, पूर्व);
	अन्यथा
		strbuf_addf(r1, "%s..%s", शीर्ष_oid, पूर्व);

	अगर (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), शीर्ष_oid);
	अन्यथा अगर (पूर्व_is_range)
		die(_("failed to infer range-diff origin of current series"));
	अन्यथा {
		warning(_("using '%s' as range-diff origin of current series"), पूर्व);
		strbuf_addf(r2, "%s..%s", पूर्व, शीर्ष_oid);
	}
}

पूर्णांक cmd_format_patch(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	काष्ठा format_config cfg;
	काष्ठा commit *commit;
	काष्ठा commit_ढेर list = COMMIT_ढेर_INIT;
	काष्ठा rev_info rev;
	अक्षर *to_free = शून्य;
	काष्ठा setup_revision_opt s_r_opt;
	माप_प्रकार योग, i;
	पूर्णांक use_stdout = 0;
	पूर्णांक start_संख्या = -1;
	पूर्णांक just_numbers = 0;
	पूर्णांक ignore_if_in_upstream = 0;
	पूर्णांक cover_letter = -1;
	स्थिर अक्षर *cover_letter_fmt = शून्य;
	पूर्णांक boundary_गिनती = 0;
	पूर्णांक no_द्विआधारी_diff = 0;
	पूर्णांक zero_commit = 0;
	काष्ठा commit *origin = शून्य;
	स्थिर अक्षर *in_reply_to = शून्य;
	काष्ठा patch_ids ids;
	काष्ठा strbuf buf = STRBUF_INIT;
	पूर्णांक use_patch_format = 0;
	पूर्णांक quiet = 0;
	स्थिर अक्षर *reroll_गिनती = शून्य;
	अक्षर *cover_from_description_arg = शून्य;
	अक्षर *description_file = शून्य;
	अक्षर *branch_name = शून्य;
	काष्ठा base_वृक्ष_info bases;
	काष्ठा commit *base;
	पूर्णांक show_progress = 0;
	काष्ठा progress *progress = शून्य;
	काष्ठा oid_array idiff_पूर्व = OID_ARRAY_INIT;
	काष्ठा strbuf idiff_title = STRBUF_INIT;
	स्थिर अक्षर *rdiff_पूर्व = शून्य;
	काष्ठा strbuf rdiff1 = STRBUF_INIT;
	काष्ठा strbuf rdiff2 = STRBUF_INIT;
	काष्ठा strbuf rdiff_title = STRBUF_INIT;
	काष्ठा rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	स्थिर अक्षर *rfc = शून्य;
	पूर्णांक creation_factor = -1;
	स्थिर अक्षर *signature = git_version_string;
	अक्षर *signature_to_free = शून्य;
	अक्षर *signature_file_arg = शून्य;
	काष्ठा keep_callback_तथ्य keep_callback_तथ्य = {
		.cfg = &cfg,
		.revs = &rev,
	};
	स्थिर अक्षर *fmt_patch_suffix = शून्य;

	स्थिर काष्ठा option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, शून्य,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, शून्य,
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
		OPT_STRING('v', "reroll-count", &reroll_गिनती, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_तथ्य, शून्य,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_द्विआधारी_diff,
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
		OPT_CALLBACK(0, "interdiff", &idiff_पूर्व, N_("rev"),
			     N_("show changes against <rev> in cover letter or single patch"),
			     parse_opt_object_name),
		OPT_STRING(0, "range-diff", &rdiff_पूर्व, N_("refspec"),
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
	स_नकल(&rev.notes_opt, &cfg.notes_opt, माप(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	स_रखो(&s_r_opt, 0, माप(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	अगर (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	अगर (cfg.default_attach) {
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

	अगर (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	अगर (cfg.log.fmt_patch_name_max <= cast_आकार_t_to_int(म_माप("0000-") + म_माप(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = म_माप("0000-") + म_माप(fmt_patch_suffix);

	अगर (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	अगर (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		अगर (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		अन्यथा
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	अगर (reroll_गिनती) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_गिनती);
		rev.reroll_गिनती = reroll_गिनती;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	क्रम (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.वस्तुएँ[i].string);
		strbuf_addch(&buf, '\n');
	}

	अगर (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	क्रम (i = 0; i < cfg.extra_to.nr; i++) {
		अगर (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.वस्तुएँ[i].string);
		अगर (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	अगर (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	क्रम (i = 0; i < cfg.extra_cc.nr; i++) {
		अगर (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.वस्तुएँ[i].string);
		अगर (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, शून्य);

	अगर (cfg.from) {
		अगर (split_ident_पंक्ति(&rev.from_ident, cfg.from, म_माप(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	अगर (start_संख्या < 0)
		start_संख्या = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	अगर (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	अगर (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	अगर (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	अगर (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	अगर (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	अगर (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	अगर (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	अगर (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	अगर (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	अगर (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	अगर (!rev.diffopt.flags.text && !no_द्विआधारी_diff)
		rev.diffopt.flags.द्विआधारी = 1;

	अगर (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	अगर (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	अगर (use_stdout) {
		setup_pager(the_repository);
	} अन्यथा अगर (!rev.diffopt.close_file) {
		पूर्णांक saved;

		अगर (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		अगर (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		चयन (safe_create_leading_directories_const(the_repository, output_directory)) {
		हाल SCLD_OK:
		हाल SCLD_EXISTS:
			अवरोध;
		शेष:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		अगर (सूची_गढ़ो(output_directory, 0777) < 0 && त्रुटि_सं != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	अगर (rev.pending.nr == 1) {
		पूर्णांक check_शीर्ष = 0;

		अगर (rev.max_गिनती < 0 && !rev.show_मूल_diff) {
			/*
			 * This is traditional behaviour of "git format-patch
			 * origin" that prepares what the origin side still
			 * does not have.
			 */
			rev.pending.objects[0].item->flags |= UNINTERESTING;
			add_शीर्ष_to_pending(&rev);
			check_शीर्ष = 1;
		}
		/*
		 * Otherwise, it is "format-patch -22 HEAD", and/or
		 * "format-patch --root HEAD".  The user wants
		 * get_revision() to do the usual traversal.
		 */

		अगर (!म_भेद(rev.pending.objects[0].name, "HEAD"))
			check_शीर्ष = 1;

		अगर (check_शीर्ष) {
			स्थिर अक्षर *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      शून्य, शून्य);
			अगर (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			अन्यथा
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_मूल_diff = 1;

	अगर (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		अगर (rev.pending.nr == 2) {
			काष्ठा object_array_entry *o = rev.pending.objects;
			अगर (oideq(&o[0].item->oid, &o[1].item->oid))
				जाओ done;
		}
		get_patch_ids(&rev, &ids);
	}

	अगर (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	जबतक ((commit = get_revision(&rev)) != शून्य) {
		अगर (commit->object.flags & BOUNDARY) {
			boundary_गिनती++;
			origin = (boundary_गिनती == 1) ? commit : शून्य;
			जारी;
		}

		अगर (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			जारी;

		commit_ढेर_धकेलो(&list, commit);
	}
	अगर (!list.nr)
		/* nothing to do */
		जाओ done;
	योग = list.nr;

	अगर (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		अगर (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} अन्यथा अगर (cover_letter == -1) {
		cover_letter = 1;
	}

	अगर (cover_letter == -1) {
		अगर (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (योग > 1);
		अन्यथा अगर ((idiff_पूर्व.nr || rdiff_पूर्व) && (योग > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		अन्यथा
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	अगर (!cfg.keep_subject && cfg.auto_संख्या && (योग > 1 || cover_letter))
		cfg.numbered = 1;
	अगर (cfg.numbered)
		rev.योग = योग + start_संख्या - 1;

	अगर (idiff_पूर्व.nr) {
		अगर (!cover_letter && योग != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_पूर्व.oid[idiff_पूर्व.nr - 1];
		rev.idiff_oid2 = get_commit_वृक्ष_oid(list.वस्तुएँ[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_गिनती,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	अगर (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	अन्यथा अगर (!rdiff_पूर्व)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	अगर (rdiff_पूर्व) {
		अगर (!cover_letter && योग != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_पूर्व,
					origin, list.वस्तुएँ[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_गिनती,
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
	अगर (!signature) {
		; /* --no-signature inhibits all signatures */
	} अन्यथा अगर (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} अन्यथा अगर (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		काष्ठा strbuf buf = STRBUF_INIT;
		स्थिर अक्षर *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		अगर (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, शून्य);
	} अन्यथा अगर (cfg.signature) {
		signature = cfg.signature;
	}

	स_रखो(&bases, 0, माप(bases));
	base = get_base_commit(&cfg, list.वस्तुएँ, list.nr);
	अगर (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.वस्तुएँ, list.nr);
	}

	अगर (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(माप(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	अगर (in_reply_to) {
		अक्षर *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	अगर (cover_letter) {
		अगर (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.वस्तुएँ,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		योग++;
		start_संख्या--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = शून्य;
		rev.rdiff1 = शून्य;
	}
	rev.add_signoff = cfg.do_signoff;

	अगर (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), योग);
	जबतक (list.nr) {
		माप_प्रकार idx = list.nr - 1;
		पूर्णांक shown;

		display_progress(progress, योग - idx);
		commit = commit_ढेर_निकालो(&list);
		rev.nr = योग - idx + (start_संख्या - 1);

		/* Make the second and subsequent mails replies to the first */
		अगर (cfg.thread) {
			/* Have we already had a message ID? */
			अगर (rev.message_id) {
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
				अगर (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					मुक्त(rev.message_id);
				अन्यथा
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		अगर (output_directory &&
		    open_अग्र_file(rev.numbered_files ? शून्य : commit, शून्य, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_वृक्ष_commit(&rev, commit);
		free_commit_बफर(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		अगर (output_directory)
			rev.shown_one = 0;
		अगर (shown) {
			print_bases(&bases, rev.diffopt.file);
			अगर (rev.mime_boundary)
				ख_लिखो(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			अन्यथा
				print_signature(signature, rev.diffopt.file);
		}
		अगर (output_directory) {
			ख_बंद(rev.diffopt.file);
			rev.diffopt.file = शून्य;
		}
	}
	stop_progress(&progress);
	commit_ढेर_clear(&list);
	अगर (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_पूर्व);
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
	अगर (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	मुक्त(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	वापस 0;
}

अटल पूर्णांक add_pending_commit(स्थिर अक्षर *arg, काष्ठा rev_info *revs, पूर्णांक flags)
{
	काष्ठा object_id oid;
	अगर (repo_get_oid(the_repository, arg, &oid) == 0) {
		काष्ठा commit *commit = lookup_commit_reference(the_repository,
								&oid);
		अगर (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			वापस 0;
		}
	}
	वापस -1;
}

अटल स्थिर अक्षर * स्थिर cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	शून्य
};

अटल व्योम print_commit(अक्षर sign, काष्ठा commit *commit, पूर्णांक verbose,
			 पूर्णांक abbrev, खाता *file)
{
	अगर (!verbose) {
		ख_लिखो(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} अन्यथा {
		काष्ठा strbuf buf = STRBUF_INIT;
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &buf);
		ख_लिखो(file, "%c %s %s\n", sign,
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
अटल व्योम collect_diff_blob_oids(काष्ठा commit *commit,
				   काष्ठा diff_options *opts,
				   काष्ठा oidset *blobs)
{
	काष्ठा diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	निश्चित(!commit->parents || !commit->parents->अग्र);

	अगर (commit->parents)
		diff_वृक्ष_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	अन्यथा
		diff_मूल_वृक्ष_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	क्रम (पूर्णांक i = 0; i < q->nr; i++) {
		काष्ठा diff_filepair *p = q->queue[i];
		काष्ठा userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->सूचकांक, p->one->path);
		अगर (drv && drv->द्विआधारी > 0)
			जारी;

		अगर (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, शून्य,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_जोड़ो(blobs, &p->one->oid);
		अगर (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, शून्य,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_जोड़ो(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

अटल पूर्णांक always_match(स्थिर व्योम *cmp_तथ्य UNUSED,
			स्थिर काष्ठा hashmap_entry *entry1 UNUSED,
			स्थिर काष्ठा hashmap_entry *entry2 UNUSED,
			स्थिर व्योम *keydata UNUSED)
{
	वापस 0;
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
अटल व्योम prefetch_cherry_blobs(काष्ठा repository *repo,
				  काष्ठा commit_list *list,
				  काष्ठा patch_ids *ids)
{
	काष्ठा oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	अगर (!repo_has_promisor_remote(repo))
		वापस;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	क्रम (काष्ठा commit_list *l = list; l; l = l->अग्र) {
		काष्ठा commit *c = l->item;
		bool match_found = false;
		क्रम (काष्ठा patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_अग्र(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		अगर (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	अगर (oidset_आकार(&blobs)) {
		काष्ठा oid_array to_fetch = OID_ARRAY_INIT;
		काष्ठा oidset_iter iter;
		स्थिर काष्ठा object_id *oid;

		oidset_iter_init(&blobs, &iter);
		जबतक ((oid = oidset_iter_अग्र(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

पूर्णांक cmd_cherry(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo UNUSED)
{
	काष्ठा rev_info revs;
	काष्ठा patch_ids ids;
	काष्ठा commit *commit;
	काष्ठा commit_list *list = शून्य;
	काष्ठा branch *current_branch;
	स्थिर अक्षर *upstream;
	स्थिर अक्षर *शीर्ष = "HEAD";
	स्थिर अक्षर *limit = शून्य;
	पूर्णांक verbose = 0, abbrev = 0;

	काष्ठा option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	चयन (argc) {
	हाल 3:
		limit = argv[2];
		/* FALLTHROUGH */
	हाल 2:
		शीर्ष = argv[1];
		/* FALLTHROUGH */
	हाल 1:
		upstream = argv[0];
		अवरोध;
	शेष:
		current_branch = branch_get(शून्य);
		upstream = branch_get_upstream(current_branch, शून्य);
		अगर (!upstream) {
			ख_लिखो(मानक_त्रुटि, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	अगर (add_pending_commit(शीर्ष, &revs, 0))
		die(_("unknown commit %s"), शीर्ष);
	अगर (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	अगर (revs.pending.nr == 2) {
		काष्ठा object_array_entry *o = revs.pending.objects;
		अगर (oideq(&o[0].item->oid, &o[1].item->oid))
			वापस 0;
	}

	get_patch_ids(&revs, &ids);

	अगर (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	अगर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	जबतक ((commit = get_revision(&revs)) != शून्य) {
		commit_list_जोड़ो(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	क्रम (काष्ठा commit_list *l = list; l; l = l->अग्र) {
		अक्षर sign = '+';

		commit = l->item;
		अगर (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	वापस 0;
}
