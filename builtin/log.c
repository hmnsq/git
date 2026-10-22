/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "odb/streaming.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "diff-merges.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "oidset.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "reflog-walk.h"
#અન્તર્ભાવ "patch-ids.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "shortlog.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "line-log.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "version.h"
#અન્તર્ભાવ "mailmap.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "commit-slab.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "utf8.h"

#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "promisor-remote.h"
#અન્તર્ભાવ "range-diff.h"
#અન્તર્ભાવ "tmp-objdir.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "userdiff.h"
#અન્તર્ભાવ "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

સ્થિર unsigned પૂર્ણાંક force_in_body_from;
સ્થિર પૂર્ણાંક stdout_mboxrd;
સ્થિર પૂર્ણાંક format_no_prefix;

સ્થિર અચલ અક્ષર * અચલ builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	ખાલી
};

સંરચના લીટી_opt_callback_data {
	સંરચના rev_info *rev;
	અચલ અક્ષર *prefix;
	સંરચના string_list args;
};

સ્થિર પૂર્ણાંક session_is_interactive(શૂન્ય)
{
	પાછા isatty(1) || pager_in_use();
}

સ્થિર પૂર્ણાંક auto_decoration_style(શૂન્ય)
{
	પાછા session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

સ્થિર પૂર્ણાંક parse_decoration_style(અચલ અક્ષર *કિંમત)
{
	પસંદગી (git_parse_maybe_bool(કિંમત)) {
	સ્થિતિ 1:
		પાછા DECORATE_SHORT_REFS;
	સ્થિતિ 0:
		પાછા 0;
	મૂળભૂત:
		તોડો;
	}
	જો (!strcmp(કિંમત, "full"))
		પાછા DECORATE_FULL_REFS;
	નહીંતર જો (!strcmp(કિંમત, "short"))
		પાછા DECORATE_SHORT_REFS;
	નહીંતર જો (!strcmp(કિંમત, "auto"))
		પાછા auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	પાછા -1;
}

સંરચના log_config {
	પૂર્ણાંક default_abbrev_commit;
	પૂર્ણાંક default_show_મૂળ;
	પૂર્ણાંક default_follow;
	પૂર્ણાંક default_show_signature;
	પૂર્ણાંક default_encode_email_headers;
	પૂર્ણાંક decoration_style;
	પૂર્ણાંક decoration_given;
	પૂર્ણાંક use_mailmap_config;
	અક્ષર *fmt_patch_subject_prefix;
	પૂર્ણાંક fmt_patch_name_max;
	અક્ષર *fmt_pretty;
	અક્ષર *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	પૂર્ણાંક i_still_use_this;
#endif
};

સ્થિર શૂન્ય log_config_init(સંરચના log_config *cfg)
{
	memset(cfg, 0, કદ(*cfg));
	cfg->default_show_મૂળ = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

સ્થિર શૂન્ય log_config_release(સંરચના log_config *cfg)
{
	મુક્ત(cfg->default_date_mode);
	મુક્ત(cfg->fmt_pretty);
	મુક્ત(cfg->fmt_patch_subject_prefix);
}

સ્થિર પૂર્ણાંક use_default_decoration_filter = 1;
સ્થિર સંરચના string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
સ્થિર સંરચના string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
સ્થિર સંરચના string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

સ્થિર પૂર્ણાંક clear_decorations_callback(અચલ સંરચના option *opt UNUSED,
				      અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	પાછા 0;
}

સ્થિર પૂર્ણાંક decorate_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			     પૂર્ણાંક unset)
{
	સંરચના log_config *cfg = opt->કિંમત;

	જો (unset)
		cfg->decoration_style = 0;
	નહીંતર જો (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	નહીંતર
		cfg->decoration_style = DECORATE_SHORT_REFS;

	જો (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	પાછા 0;
}

સ્થિર પૂર્ણાંક log_લીટી_range_callback(અચલ સંરચના option *option, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના લીટી_opt_callback_data *data = option->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!arg)
		પાછા -1;

	data->rev->લીટી_level_traverse = 1;
	string_list_append(&data->args, arg);

	પાછા 0;
}

સ્થિર શૂન્ય cmd_log_init_defaults(સંરચના rev_info *rev,
				  સંરચના log_config *cfg)
{
	જો (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	જો (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_મૂળ_diff = cfg->default_show_મૂળ;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	જો (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

સ્થિર શૂન્ય set_default_decoration_filter(સંરચના decoration_filter *decoration_filter)
{
	અક્ષર *કિંમત = ખાલી;
	સંરચના string_list *include = decoration_filter->include_ref_pattern;
	અચલ સંરચના string_list *config_exclude;

	જો (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		સંરચના string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	જો (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &કિંમત) &&
	    !strcmp("all", કિંમત))
		use_default_decoration_filter = 0;
	મુક્ત(કિંમત);

	જો (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		પાછા;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	માટે (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		જો (!ref_namespace[i].decoration)
			ચાલુ;

		string_list_append(include, ref_namespace[i].ref);
	}
}

સ્થિર શૂન્ય cmd_log_init_finish(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના rev_info *rev, સંરચના setup_revision_opt *opt,
			 સંરચના log_config *cfg)
{
	સંરચના userformat_want w;
	પૂર્ણાંક quiet = 0, source = 0, mailmap;
	સ્થિર સંરચના લીટી_opt_callback_data લીટી_cb = {ખાલી, ખાલી, STRING_LIST_INIT_DUP};
	સંરચના decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	સ્થિર સંરચના revision_sources revision_sources;

	અચલ સંરચના option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", ખાલી, ખાલી,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, ખાલી, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', ખાલી, &લીટી_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_લીટી_range_callback),
		OPT_END()
	};

	લીટી_cb.rev = rev;
	લીટી_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	જો (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	જો (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	જો (rev->લીટી_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, કદ(w));
	userformat_find_requirements(ખાલી, &w);

	જો (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	જો (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	જો ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	જો (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	જો (mailmap) {
		rev->mailmap = xmalloc(કદ(સંરચના string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	જો (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		જો (!cfg->decoration_given)
			cfg->decoration_style = 0;
		જો (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	જો (rev->commit_format == CMIT_FMT_USERFORMAT) {
		જો (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} નહીંતર જો (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	જો (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		જો (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	જો (rev->લીટી_level_traverse)
		લીટી_log_init(rev, લીટી_cb.prefix, &લીટી_cb.args);

	setup_pager(the_repository);
}

સ્થિર શૂન્ય cmd_log_init(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના rev_info *rev, સંરચના setup_revision_opt *opt,
			 સંરચના log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

સ્થિર પૂર્ણાંક cmd_log_walk_no_free(સંરચના rev_info *rev)
{
	સંરચના commit *commit;
	પૂર્ણાંક saved_nrl = 0;
	પૂર્ણાંક saved_dcctc = 0;
	પૂર્ણાંક result;

	જો (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	જ્યારે ((commit = get_revision(rev)) != ખાલી) {
		જો (!log_વૃક્ષ_commit(rev, commit) && rev->max_ગણતરી >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_ગણતરી++;
		જો (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = ખાલી;
		}
		જો (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		જો (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	જો (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	પાછા result;
}

સ્થિર પૂર્ણાંક cmd_log_walk(સંરચના rev_info *rev)
{
	પૂર્ણાંક retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	પાછા retval;
}

સ્થિર પૂર્ણાંક git_log_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			  અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના log_config *cfg = cb;
	અચલ અક્ષર *slot_name;

	જો (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		પાછા git_config_string(&cfg->fmt_pretty, var, કિંમત);
	}
	જો (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		પાછા git_config_string(&cfg->fmt_patch_subject_prefix, var, કિંમત);
	}
	જો (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, કિંમત, ctx->kvi);
		પાછા 0;
	}
	જો (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		પાછા git_config_string(&cfg->default_date_mode, var, કિંમત);
	}
	જો (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(કિંમત);
		જો (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		પાછા 0;
	}
	જો (!strcmp(var, "log.diffmerges")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		પાછા diff_merges_config(કિંમત);
	}
	જો (!strcmp(var, "log.showroot")) {
		cfg->default_show_મૂળ = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (skip_prefix(var, "color.decorate.", &slot_name))
		પાછા parse_decorate_color_config(var, slot_name, કિંમત);
	જો (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	પાછા git_diff_ui_config(var, કિંમત, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
પૂર્ણાંક cmd_whatchanged(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    સંરચના repository *repo UNUSED)
{
	સંરચના log_config cfg;
	સંરચના rev_info rev;
	સંરચના setup_revision_opt opt;
	પૂર્ણાંક ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, કદ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	જો (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	જો (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	પાછા ret;
}
#endif

સ્થિર શૂન્ય show_tagger(અચલ અક્ષર *buf, સંરચના rev_info *rev)
{
	સંરચના strbuf out = STRBUF_INIT;
	સંરચના pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

સ્થિર પૂર્ણાંક show_blob_object(અચલ સંરચના object_id *oid, સંરચના rev_info *rev, અચલ અક્ષર *obj_name)
{
	સંરચના object_id oidc;
	સંરચના object_context obj_context = {0};
	અક્ષર *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	જો (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		પાછા odb_stream_blob_to_fd(the_repository->objects, 1, oid, ખાલી, 0);

	જો (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	જો (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		પાછા odb_stream_blob_to_fd(the_repository->objects, 1, oid, ખાલી, 0);
	}

	જો (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	મુક્ત(buf);
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_tag_object(અચલ સંરચના object_id *oid, સંરચના rev_info *rev)
{
	size_t size;
	ગણના object_type type;
	અક્ષર *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	જો (!buf)
		પાછા error(_("could not read object %s"), oid_to_hex(oid));

	પ્રતિજ્ઞા(type == OBJ_TAG);
	જ્યારે (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		અચલ અક્ષર *ident;
		જ્યારે (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		જો (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	જો (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	મુક્ત(buf);
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_વૃક્ષ_object(અચલ સંરચના object_id *oid UNUSED,
			    સંરચના strbuf *base UNUSED,
			    અચલ અક્ષર *pathname, unsigned mode,
			    શૂન્ય *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	પાછા 0;
}

સ્થિર શૂન્ય show_setup_revisions_tweak(સંરચના rev_info *rev)
{
	જો (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	નહીંતર
		diff_merges_default_to_dense_combined(rev);
	જો (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

પૂર્ણાંક cmd_show(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repo UNUSED)
{
	સંરચના log_config cfg;
	સંરચના rev_info rev;
	unsigned પૂર્ણાંક i;
	સંરચના setup_revision_opt opt;
	સંરચના pathspec match_all;
	પૂર્ણાંક ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	જો (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, કદ(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, કદ(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	જો (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		પાછા ret;
	}

	rev.diffopt.no_free = 1;
	માટે (i = 0; i < rev.pending.nr && !ret; i++) {
		સંરચના object *o = rev.pending.objects[i].item;
		અચલ અક્ષર *name = rev.pending.objects[i].name;
		પસંદગી (o->type) {
		સ્થિતિ OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			તોડો;
		સ્થિતિ OBJ_TAG: {
			સંરચના tag *t = (સંરચના tag *)o;
			સંરચના object_id *oid = get_tagged_oid(t);

			જો (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			જો (ret)
				તોડો;
			o = parse_object(the_repository, oid);
			જો (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			તોડો;
		}
		સ્થિતિ OBJ_વૃક્ષ:
			જો (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_વૃક્ષ(the_repository, (સંરચના વૃક્ષ *)o,
				  &match_all, show_વૃક્ષ_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			તોડો;
		સ્થિતિ OBJ_COMMIT:
		{
			સંરચના object_array old;
			સંરચના object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, કદ(old));
			memcpy(&rev.pending, &blank, કદ(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, કદ(rev.pending));
			તોડો;
		}
		મૂળભૂત:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	પાછા ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
પૂર્ણાંક cmd_log_reflog(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના log_config cfg;
	સંરચના rev_info rev;
	સંરચના setup_revision_opt opt;
	પૂર્ણાંક ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, કદ(opt));
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
	પાછા ret;
}

સ્થિર શૂન્ય log_setup_revisions_tweak(સંરચના rev_info *rev)
{
	જો (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	જો (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

પૂર્ણાંક cmd_log(પૂર્ણાંક argc,
	    અચલ અક્ષર **argv,
	    અચલ અક્ષર *prefix,
	    સંરચના repository *repo UNUSED)
{
	સંરચના log_config cfg;
	સંરચના rev_info rev;
	સંરચના setup_revision_opt opt;
	પૂર્ણાંક ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, કદ(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	પાછા ret;
}

/* format-patch */

ગણના cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

ગણના thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

ગણના cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

ગણના auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

સંરચના format_config {
	સંરચના log_config log;
	ગણના thread_level thread;
	પૂર્ણાંક do_signoff;
	ગણના auto_base_setting auto_base;
	અક્ષર *base_commit;
	અક્ષર *from;
	અક્ષર *signature;
	અક્ષર *signature_file;
	ગણના cover_setting config_cover_letter;
	અક્ષર *fmt_cover_letter_commit_list;
	અક્ષર *config_output_directory;
	ગણના cover_from_description cover_from_description_mode;
	પૂર્ણાંક show_notes;
	સંરચના display_notes_opt notes_opt;
	પૂર્ણાંક numbered_cmdline_opt;
	પૂર્ણાંક numbered;
	પૂર્ણાંક auto_સંખ્યા;
	અક્ષર *default_attach;
	સંરચના string_list extra_hdr;
	સંરચના string_list extra_to;
	સંરચના string_list extra_cc;
	પૂર્ણાંક keep_subject;
	પૂર્ણાંક subject_prefix;
	સંરચના strbuf sprefix;
	અક્ષર *fmt_patch_suffix;
};

સ્થિર શૂન્ય format_config_init(સંરચના format_config *cfg)
{
	memset(cfg, 0, કદ(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_સંખ્યા = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

સ્થિર શૂન્ય format_config_release(સંરચના format_config *cfg)
{
	log_config_release(&cfg->log);
	મુક્ત(cfg->base_commit);
	મુક્ત(cfg->from);
	મુક્ત(cfg->signature);
	મુક્ત(cfg->signature_file);
	મુક્ત(cfg->config_output_directory);
	મુક્ત(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	મુક્ત(cfg->fmt_patch_suffix);
	મુક્ત(cfg->fmt_cover_letter_commit_list);
}

સ્થિર ગણના cover_from_description parse_cover_from_description(અચલ અક્ષર *arg)
{
	જો (!arg || !strcmp(arg, "default"))
		પાછા COVER_FROM_MESSAGE;
	નહીંતર જો (!strcmp(arg, "none"))
		પાછા COVER_FROM_NONE;
	નહીંતર જો (!strcmp(arg, "message"))
		પાછા COVER_FROM_MESSAGE;
	નહીંતર જો (!strcmp(arg, "subject"))
		પાછા COVER_FROM_SUBJECT;
	નહીંતર જો (!strcmp(arg, "auto"))
		પાછા COVER_FROM_AUTO;
	નહીંતર
		die(_("%s: invalid cover from description mode"), arg);
}

સ્થિર શૂન્ય add_header(સંરચના format_config *cfg, અચલ અક્ષર *કિંમત)
{
	સંરચના string_list_item *item;
	પૂર્ણાંક len = strlen(કિંમત);
	જ્યારે (len && કિંમત[len - 1] == '\n')
		len--;

	જો (!strncasecmp(કિંમત, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, કિંમત + 4);
		len -= 4;
	} નહીંતર જો (!strncasecmp(કિંમત, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, કિંમત + 4);
		len -= 4;
	} નહીંતર {
		item = string_list_append(&cfg->extra_hdr, કિંમત);
	}

	item->string[len] = '\0';
}

સ્થિર પૂર્ણાંક git_format_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			     અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	સંરચના format_config *cfg = cb;

	જો (!strcmp(var, "format.headers")) {
		જો (!કિંમત)
			die(_("format.headers without value"));
		add_header(cfg, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		પાછા git_config_string(&cfg->fmt_patch_suffix, var, કિંમત);
	}
	જો (!strcmp(var, "format.to")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		string_list_append(&cfg->extra_to, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "format.cc")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		પાછા 0;
	}
	જો (!strcmp(var, "format.numbered")) {
		જો (કિંમત && !strcasecmp(કિંમત, "auto")) {
			cfg->auto_સંખ્યા = 1;
			પાછા 0;
		}
		cfg->numbered = git_config_bool(var, કિંમત);
		cfg->auto_સંખ્યા = cfg->auto_સંખ્યા && cfg->numbered;
		પાછા 0;
	}
	જો (!strcmp(var, "format.attach")) {
		જો (કિંમત && *કિંમત) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(કિંમત);
		} નહીંતર જો (કિંમત && !*કિંમત) {
			FREE_AND_NULL(cfg->default_attach);
		} નહીંતર {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		પાછા 0;
	}
	જો (!strcmp(var, "format.thread")) {
		જો (કિંમત && !strcasecmp(કિંમત, "deep")) {
			cfg->thread = THREAD_DEEP;
			પાછા 0;
		}
		જો (કિંમત && !strcasecmp(કિંમત, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			પાછા 0;
		}
		cfg->thread = git_config_bool(var, કિંમત) ? THREAD_SHALLOW : THREAD_UNSET;
		પાછા 0;
	}
	જો (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		પાછા git_config_string(&cfg->signature, var, કિંમત);
	}
	જો (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		પાછા git_config_pathname(&cfg->signature_file, var, કિંમત);
	}
	જો (!strcmp(var, "format.coverletter")) {
		જો (કિંમત && !strcasecmp(કિંમત, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			પાછા 0;
		}
		cfg->config_cover_letter = git_config_bool(var, કિંમત) ? COVER_ON : COVER_OFF;
		પાછા 0;
	}
	જો (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		પાછા git_config_string(&cfg->fmt_cover_letter_commit_list, var, કિંમત);
	}
	જો (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		પાછા git_config_string(&cfg->config_output_directory, var, કિંમત);
	}
	જો (!strcmp(var, "format.useautobase")) {
		જો (કિંમત && !strcasecmp(કિંમત, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			પાછા 0;
		}
		cfg->auto_base = git_config_bool(var, કિંમત) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		પાછા 0;
	}
	જો (!strcmp(var, "format.from")) {
		પૂર્ણાંક b = git_parse_maybe_bool(કિંમત);
		FREE_AND_NULL(cfg->from);
		જો (b < 0)
			cfg->from = xstrdup(કિંમત);
		નહીંતર જો (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		પાછા 0;
	}
	જો (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "format.notes")) {
		પૂર્ણાંક b = git_parse_maybe_bool(કિંમત);
		જો (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, કિંમત);
		નહીંતર જો (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		નહીંતર
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		પાછા 0;
	}
	જો (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(કિંમત);
		જો (format_no_prefix < 0) {
			પૂર્ણાંક status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				કિંમત, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		પાછા 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	જો (!strcmp(var, "diff.noprefix"))
		પાછા 0;

	પાછા git_log_config(var, કિંમત, ctx, &cfg->log);
}

સ્થિર અચલ અક્ષર *output_directory = ખાલી;
સ્થિર પૂર્ણાંક outdir_offset;

સ્થિર પૂર્ણાંક open_next_file(સંરચના commit *commit, અચલ અક્ષર *subject,
			 સંરચના rev_info *rev, પૂર્ણાંક quiet)
{
	સંરચના strbuf filename = STRBUF_INIT;

	જો (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	જો (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	નહીંતર જો (commit)
		fmt_output_commit(&filename, commit, rev);
	નહીંતર
		fmt_output_subject(&filename, subject, rev);

	જો (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	જો (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		પાછા -1;
	}

	strbuf_release(&filename);
	પાછા 0;
}

સ્થિર શૂન્ય get_patch_ids(સંરચના rev_info *rev, સંરચના patch_ids *ids)
{
	સંરચના rev_info check_rev;
	સંરચના commit *commit, *c1, *c2;
	સંરચના object *o1, *o2;
	unsigned flags1, flags2;

	જો (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	જો ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	જો (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	જ્યારે ((commit = get_revision(&check_rev)) != ખાલી) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

સ્થિર શૂન્ય gen_message_id(સંરચના rev_info *info, અચલ અક્ષર *base)
{
	સંરચના strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(ખાલી),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, ખાલી);
}

સ્થિર શૂન્ય print_signature(અચલ અક્ષર *signature, FILE *file)
{
	જો (!signature || !*signature)
		પાછા;

	fprintf(file, "-- \n%s", signature);
	જો (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

સ્થિર અક્ષર *find_branch_name(સંરચના rev_info *rev)
{
	સંરચના object_id branch_oid;
	અચલ સંરચના object_id *tip_oid;
	અચલ અક્ષર *ref, *v;
	અક્ષર *full_ref, *branch = ખાલી;
	પૂર્ણાંક interesting_found = 0;
	size_t idx;

	માટે (size_t i = 0; i < rev->cmdline.nr; i++) {
		જો (rev->cmdline.rev[i].flags & UNINTERESTING)
			ચાલુ;
		જો (interesting_found)
			પાછા ખાલી;
		interesting_found = 1;
		idx = i;
	}
	જો (!interesting_found)
		પાછા ખાલી;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	જો (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	મુક્ત(full_ref);
	પાછા branch;
}

સ્થિર શૂન્ય show_diffstat(સંરચના rev_info *rev,
			  સંરચના commit *origin, સંરચના commit *head)
{
	સંરચના diff_options opts;

	memcpy(&opts, &rev->diffopt, કદ(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_વૃક્ષ_oid(get_commit_વૃક્ષ_oid(origin),
		      get_commit_વૃક્ષ_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

સ્થિર શૂન્ય read_desc_file(સંરચના strbuf *buf, અચલ અક્ષર *desc_file)
{
	જો (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

સ્થિર શૂન્ય prepare_cover_text(સંરચના pretty_print_context *pp,
			       અચલ અક્ષર *description_file,
			       અચલ અક્ષર *branch_name,
			       સંરચના strbuf *sb,
			       અચલ અક્ષર *encoding,
			       પૂર્ણાંક need_8bit_cte,
			       અચલ સંરચના format_config *cfg)
{
	અચલ અક્ષર *subject = "*** SUBJECT HERE ***";
	અચલ અક્ષર *body = "*** BLURB HERE ***";
	સંરચના strbuf description_sb = STRBUF_INIT;
	સંરચના strbuf subject_sb = STRBUF_INIT;

	જો (cfg->cover_from_description_mode == COVER_FROM_NONE)
		જાઓ do_pp;

	જો (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	નહીંતર જો (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	જો (!description_sb.len)
		જાઓ do_pp;

	જો (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	જો (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	નહીંતર
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

સંરચના rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	સંરચના string_list notes;
};

સ્થિર પૂર્ણાંક rdiff_notes_cb(અચલ સંરચના option *option,
			  અચલ અક્ષર *arg,
			  પૂર્ણાંક unset)
{
	સંરચના option opt = *option;
	સંરચના rdiff_notes *rdiff_notes = option->કિંમત;

	rdiff_notes->override = 1;
	opt.કિંમત = &rdiff_notes->notes;
	પાછા parse_opt_string_list(&opt, arg, unset);
}

સ્થિર પૂર્ણાંક get_notes_refs(સંરચના string_list_item *item, શૂન્ય *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	પાછા 0;
}

સ્થિર શૂન્ય get_notes_args(સંરચના rdiff_notes *rdiff_notes,
			   સંરચના rev_info *rev)
{
	જો (rdiff_notes->override) {
		જો (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		નહીંતર
			strvec_ધકેલો(&rev->rdiff_log_arg, "--no-notes");
	} નહીંતર જો (!rev->show_notes) {
		strvec_ધકેલો(&rev->rdiff_log_arg, "--no-notes");
	} નહીંતર જો (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_ધકેલો(&rev->rdiff_log_arg, "--notes");
	} નહીંતર {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

સ્થિર શૂન્ય generate_shortlog_cover_letter(સંરચના shortlog *log,
					   સંરચના rev_info *rev,
					   સંરચના commit **list,
					   પૂર્ણાંક nr)
{
	shortlog_init(log);
	log->wrap_લીટીઓ = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	માટે (પૂર્ણાંક i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

સ્થિર શૂન્ય generate_commit_list_cover(FILE *cover_file, અચલ અક્ષર *format,
				       સંરચના commit **list, પૂર્ણાંક n)
{
	સંરચના strbuf commit_લીટી = STRBUF_INIT;
	સંરચના pretty_print_context ctx = {0};
	સંરચના rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	માટે (પૂર્ણાંક i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_લીટી, &ctx);
		fprintf(cover_file, "%s\n", commit_લીટી.buf);
		strbuf_reset(&commit_લીટી);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_લીટી);
}

સ્થિર શૂન્ય make_cover_letter(સંરચના rev_info *rev, પૂર્ણાંક use_separate_file,
			      સંરચના commit *origin,
			      પૂર્ણાંક nr, સંરચના commit **list,
			      અચલ અક્ષર *description_file,
			      અચલ અક્ષર *branch_name,
			      પૂર્ણાંક quiet,
			      અચલ સંરચના format_config *cfg,
			      અચલ અક્ષર *format)
{
	અચલ અક્ષર *from;
	સંરચના shortlog log;
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક i;
	અચલ અક્ષર *encoding = "UTF-8";
	પૂર્ણાંક need_8bit_cte = 0;
	સંરચના pretty_print_context pp = {0};
	સંરચના commit *head = list[0];
	અક્ષર *to_free = ખાલી;

	જો (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	જો (use_separate_file &&
	    open_next_file(ખાલી, rev->numbered_files ? ખાલી : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	માટે (i = 0; !need_8bit_cte && i < nr; i++) {
		અચલ અક્ષર *buf = repo_get_commit_buffer(the_repository,
							 list[i], ખાલી);
		જો (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	જો (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, ખાલી, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	મુક્ત(to_free);
	મુક્ત(pp.after_subject);
	strbuf_release(&sb);

	જો (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	નહીંતર જો (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	નહીંતર જો (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	નહીંતર જો (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	નહીંતર
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	જો (origin)
		show_diffstat(rev, origin, head);

	જો (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	જો (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		સંરચના diff_options opts;
		સંરચના range_diff_options range_diff_opts = {
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

સ્થિર અક્ષર *clean_message_id(અચલ અક્ષર *સંદેશ_id)
{
	અક્ષર ch;
	અચલ અક્ષર *a, *z, *m;

	m = સંદેશ_id;
	જ્યારે ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = ખાલી;
	જ્યારે ((ch = *m)) {
		જો (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	જો (!z)
		die(_("insane in-reply-to: %s"), સંદેશ_id);
	જો (++z == m)
		પાછા xstrdup(a);
	પાછા xmemdupz(a, z - a);
}

સ્થિર અચલ અક્ષર *set_outdir(અચલ અક્ષર *prefix, અચલ અક્ષર *output_directory)
{
	જો (output_directory && is_absolute_path(output_directory))
		પાછા output_directory;

	જો (!prefix || !*prefix) {
		જો (output_directory)
			પાછા output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		પાછા "./";
	}

	outdir_offset = strlen(prefix);
	જો (!output_directory)
		પાછા prefix;

	પાછા prefix_filename(prefix, output_directory);
}

સ્થિર અચલ અક્ષર * અચલ builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	ખાલી
};

સંરચના keep_callback_data {
	સંરચના format_config *cfg;
	સંરચના rev_info *revs;
};

સ્થિર પૂર્ણાંક keep_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના keep_callback_data *data = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક subject_prefix_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			    પૂર્ણાંક unset)
{
	સંરચના format_config *cfg = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક rfc_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			પૂર્ણાંક unset)
{
	અચલ અક્ષર **rfc = opt->કિંમત;

	*rfc = opt->કિંમત;
	જો (unset)
		*rfc = ખાલી;
	નહીંતર
		*rfc = arg ? arg : "RFC";
	પાછા 0;
}

સ્થિર પૂર્ણાંક numbered_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			     પૂર્ણાંક unset)
{
	સંરચના format_config *cfg = opt->કિંમત;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	જો (unset)
		cfg->auto_સંખ્યા =  0;
	પાછા 0;
}

સ્થિર પૂર્ણાંક no_numbered_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
				પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);
	પાછા numbered_callback(opt, arg, 1);
}

સ્થિર પૂર્ણાંક output_directory_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			      પૂર્ણાંક unset)
{
	અચલ અક્ષર **dir = (અચલ અક્ષર **)opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	જો (*dir)
		die(_("two output directories?"));
	*dir = arg;
	પાછા 0;
}

સ્થિર પૂર્ણાંક thread_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના format_config *cfg = opt->કિંમત;

	જો (unset)
		cfg->thread = THREAD_UNSET;
	નહીંતર જો (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	નહીંતર જો (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	નહીંતર
		પાછા 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક attach_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના rev_info *rev = (સંરચના rev_info *)opt->કિંમત;
	જો (unset)
		rev->mime_boundary = ખાલી;
	નહીંતર જો (arg)
		rev->mime_boundary = arg;
	નહીંતર
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	પાછા 0;
}

સ્થિર પૂર્ણાંક inline_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના rev_info *rev = (સંરચના rev_info *)opt->કિંમત;
	જો (unset)
		rev->mime_boundary = ખાલી;
	નહીંતર જો (arg)
		rev->mime_boundary = arg;
	નહીંતર
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	પાછા 0;
}

સ્થિર પૂર્ણાંક header_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			   પૂર્ણાંક unset)
{
	સંરચના format_config *cfg = opt->કિંમત;

	જો (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} નહીંતર {
		add_header(cfg, arg);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક from_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	અક્ષર **from = opt->કિંમત;

	મુક્ત(*from);

	જો (unset)
		*from = ખાલી;
	નહીંતર જો (arg)
		*from = xstrdup(arg);
	નહીંતર
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	પાછા 0;
}

સ્થિર પૂર્ણાંક base_callback(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના format_config *cfg = opt->કિંમત;

	જો (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} નહીંતર જો (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} નહીંતર {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	પાછા 0;
}

સંરચના base_વૃક્ષ_info {
	સંરચના object_id base_commit;
	પૂર્ણાંક nr_patch_id, alloc_patch_id;
	સંરચના object_id *patch_id;
};

સ્થિર સંરચના commit *get_base_commit(અચલ સંરચના format_config *cfg,
				      સંરચના commit **list,
				      size_t total)
{
	સંરચના commit *base = ખાલી;
	સંરચના commit **rev;
	પૂર્ણાંક auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	પસંદગી (cfg->auto_base) {
	સ્થિતિ AUTO_BASE_NEVER:
		જો (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} નહીંતર {
			/* no base information is requested */
			પાછા ખાલી;
		}
		તોડો;
	સ્થિતિ AUTO_BASE_ALWAYS:
	સ્થિતિ AUTO_BASE_WHEN_ABLE:
		જો (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} નહીંતર {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		તોડો;
	મૂળભૂત:
		BUG("unexpected automatic base selection method");
	}

	જો (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		જો (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} નહીંતર {
		સંરચના branch *curr_branch = branch_get(ખાલી);
		અચલ અક્ષર *upstream = branch_get_upstream(curr_branch, ખાલી);
		જો (upstream) {
			સંરચના commit_list *base_list = ખાલી;
			સંરચના commit *commit;
			સંરચના object_id oid;

			જો (repo_get_oid(the_repository, upstream, &oid)) {
				જો (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				નહીંતર
					પાછા ખાલી;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			જો (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				જો (die_on_failure) {
					die(_("could not find exact merge base"));
				} નહીંતર {
					commit_list_free(base_list);
					પાછા ખાલી;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} નહીંતર {
			જો (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			નહીંતર
				પાછા ખાલી;
		}
	}

	ALLOC_ARRAY(rev, total);
	માટે (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	જ્યારે (rev_nr > 1) {
		માટે (i = 0; i < rev_nr / 2; i++) {
			સંરચના commit_list *merge_base = ખાલી;
			જો (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				જો (die_on_failure) {
					die(_("failed to find exact merge base"));
				} નહીંતર {
					commit_list_free(merge_base);
					મુક્ત(rev);
					પાછા ખાલી;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		જો (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	જો (ret < 0)
		exit(128);
	જો (!ret) {
		જો (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} નહીંતર {
			મુક્ત(rev);
			પાછા ખાલી;
		}
	}

	માટે (i = 0; i < total; i++) {
		જો (base == list[i]) {
			જો (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} નહીંતર {
				મુક્ત(rev);
				પાછા ખાલી;
			}
		}
	}

	મુક્ત(rev);
	પાછા base;
}

define_commit_slab(commit_base, પૂર્ણાંક);

સ્થિર શૂન્ય prepare_bases(સંરચના base_વૃક્ષ_info *bases,
			  સંરચના commit *base,
			  સંરચના commit **list,
			  size_t total)
{
	સંરચના commit *commit;
	સંરચના rev_info revs;
	સંરચના diff_options diffopt;
	સંરચના commit_base commit_base;

	જો (!base)
		પાછા;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, ખાલી);
	revs.max_parents = 1;
	revs.topo_order = 1;
	માટે (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	જો (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	જ્યારે ((commit = get_revision(&revs)) != ખાલી) {
		સંરચના object_id oid;
		સંરચના object_id *patch_id;
		જો (*commit_base_at(&commit_base, commit))
			ચાલુ;
		જો (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

સ્થિર શૂન્ય print_bases(સંરચના base_વૃક્ષ_info *bases, FILE *file)
{
	પૂર્ણાંક i;

	/* Only do this once, either for the cover or for the first one */
	જો (is_null_oid(&bases->base_commit))
		પાછા;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	માટે (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	મુક્ત(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

સ્થિર અચલ અક્ષર *diff_title(સંરચના strbuf *sb,
			      અચલ અક્ષર *reroll_ગણતરી,
			      અચલ અક્ષર *generic,
			      અચલ અક્ષર *rerolled)
{
	પૂર્ણાંક v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	જો (reroll_ગણતરી && !strtol_i(reroll_ગણતરી, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	નહીંતર
		strbuf_addstr(sb, generic);
	પાછા sb->buf;
}

સ્થિર શૂન્ય infer_range_diff_ranges(સંરચના strbuf *r1,
				    સંરચના strbuf *r2,
				    અચલ અક્ષર *prev,
				    સંરચના commit *origin,
				    સંરચના commit *head)
{
	અચલ અક્ષર *head_oid = oid_to_hex(&head->object.oid);
	પૂર્ણાંક prev_is_range = is_range_diff_range(prev);

	જો (prev_is_range)
		strbuf_addstr(r1, prev);
	નહીંતર
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	જો (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	નહીંતર જો (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	નહીંતર {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

પૂર્ણાંક cmd_format_patch(પૂર્ણાંક argc,
		     અચલ અક્ષર **argv,
		     અચલ અક્ષર *prefix,
		     સંરચના repository *repo UNUSED)
{
	સંરચના format_config cfg;
	સંરચના commit *commit;
	સંરચના commit_ઢગલો list = COMMIT_ઢગલો_INIT;
	સંરચના rev_info rev;
	અક્ષર *to_free = ખાલી;
	સંરચના setup_revision_opt s_r_opt;
	size_t total, i;
	પૂર્ણાંક use_stdout = 0;
	પૂર્ણાંક start_સંખ્યા = -1;
	પૂર્ણાંક just_numbers = 0;
	પૂર્ણાંક ignore_if_in_upstream = 0;
	પૂર્ણાંક cover_letter = -1;
	અચલ અક્ષર *cover_letter_fmt = ખાલી;
	પૂર્ણાંક boundary_ગણતરી = 0;
	પૂર્ણાંક no_દ્વિઅંકી_diff = 0;
	પૂર્ણાંક zero_commit = 0;
	સંરચના commit *origin = ખાલી;
	અચલ અક્ષર *in_reply_to = ખાલી;
	સંરચના patch_ids ids;
	સંરચના strbuf buf = STRBUF_INIT;
	પૂર્ણાંક use_patch_format = 0;
	પૂર્ણાંક quiet = 0;
	અચલ અક્ષર *reroll_ગણતરી = ખાલી;
	અક્ષર *cover_from_description_arg = ખાલી;
	અક્ષર *description_file = ખાલી;
	અક્ષર *branch_name = ખાલી;
	સંરચના base_વૃક્ષ_info bases;
	સંરચના commit *base;
	પૂર્ણાંક show_progress = 0;
	સંરચના progress *progress = ખાલી;
	સંરચના oid_array idiff_prev = OID_ARRAY_INIT;
	સંરચના strbuf idiff_title = STRBUF_INIT;
	અચલ અક્ષર *rdiff_prev = ખાલી;
	સંરચના strbuf rdiff1 = STRBUF_INIT;
	સંરચના strbuf rdiff2 = STRBUF_INIT;
	સંરચના strbuf rdiff_title = STRBUF_INIT;
	સંરચના rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	અચલ અક્ષર *rfc = ખાલી;
	પૂર્ણાંક creation_factor = -1;
	અચલ અક્ષર *signature = git_version_string;
	અક્ષર *signature_to_free = ખાલી;
	અક્ષર *signature_file_arg = ખાલી;
	સંરચના keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	અચલ અક્ષર *fmt_patch_suffix = ખાલી;

	અચલ સંરચના option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, ખાલી,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, ખાલી,
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
		OPT_INTEGER(0, "start-number", &start_સંખ્યા,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_ગણતરી, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, ખાલી,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_દ્વિઅંકી_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, કદ(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, કદ(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	જો (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	જો (cfg.default_attach) {
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

	જો (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	જો (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	જો (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	જો (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		જો (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		નહીંતર
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	જો (reroll_ગણતરી) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_ગણતરી);
		rev.reroll_ગણતરી = reroll_ગણતરી;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	માટે (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.વસ્તુઓ[i].string);
		strbuf_addch(&buf, '\n');
	}

	જો (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	માટે (i = 0; i < cfg.extra_to.nr; i++) {
		જો (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.વસ્તુઓ[i].string);
		જો (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	જો (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	માટે (i = 0; i < cfg.extra_cc.nr; i++) {
		જો (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.વસ્તુઓ[i].string);
		જો (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, ખાલી);

	જો (cfg.from) {
		જો (split_ident_લીટી(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	જો (start_સંખ્યા < 0)
		start_સંખ્યા = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	જો (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	જો (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	જો (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	જો (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	જો (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	જો (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	જો (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	જો (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	જો (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	જો (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	જો (!rev.diffopt.flags.text && !no_દ્વિઅંકી_diff)
		rev.diffopt.flags.દ્વિઅંકી = 1;

	જો (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	જો (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	જો (use_stdout) {
		setup_pager(the_repository);
	} નહીંતર જો (!rev.diffopt.close_file) {
		પૂર્ણાંક saved;

		જો (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		જો (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		પસંદગી (safe_create_leading_directories_const(the_repository, output_directory)) {
		સ્થિતિ SCLD_OK:
		સ્થિતિ SCLD_EXISTS:
			તોડો;
		મૂળભૂત:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		જો (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	જો (rev.pending.nr == 1) {
		પૂર્ણાંક check_head = 0;

		જો (rev.max_ગણતરી < 0 && !rev.show_મૂળ_diff) {
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

		જો (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		જો (check_head) {
			અચલ અક્ષર *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      ખાલી, ખાલી);
			જો (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			નહીંતર
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_મૂળ_diff = 1;

	જો (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		જો (rev.pending.nr == 2) {
			સંરચના object_array_entry *o = rev.pending.objects;
			જો (oideq(&o[0].item->oid, &o[1].item->oid))
				જાઓ done;
		}
		get_patch_ids(&rev, &ids);
	}

	જો (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	જ્યારે ((commit = get_revision(&rev)) != ખાલી) {
		જો (commit->object.flags & BOUNDARY) {
			boundary_ગણતરી++;
			origin = (boundary_ગણતરી == 1) ? commit : ખાલી;
			ચાલુ;
		}

		જો (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			ચાલુ;

		commit_ઢગલો_ધકેલો(&list, commit);
	}
	જો (!list.nr)
		/* nothing to do */
		જાઓ done;
	total = list.nr;

	જો (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		જો (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} નહીંતર જો (cover_letter == -1) {
		cover_letter = 1;
	}

	જો (cover_letter == -1) {
		જો (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		નહીંતર જો ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		નહીંતર
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	જો (!cfg.keep_subject && cfg.auto_સંખ્યા && (total > 1 || cover_letter))
		cfg.numbered = 1;
	જો (cfg.numbered)
		rev.total = total + start_સંખ્યા - 1;

	જો (idiff_prev.nr) {
		જો (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_વૃક્ષ_oid(list.વસ્તુઓ[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_ગણતરી,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	જો (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	નહીંતર જો (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	જો (rdiff_prev) {
		જો (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.વસ્તુઓ[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_ગણતરી,
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
	જો (!signature) {
		; /* --no-signature inhibits all signatures */
	} નહીંતર જો (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} નહીંતર જો (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		સંરચના strbuf buf = STRBUF_INIT;
		અચલ અક્ષર *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		જો (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, ખાલી);
	} નહીંતર જો (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, કદ(bases));
	base = get_base_commit(&cfg, list.વસ્તુઓ, list.nr);
	જો (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.વસ્તુઓ, list.nr);
	}

	જો (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(કદ(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	જો (in_reply_to) {
		અક્ષર *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	જો (cover_letter) {
		જો (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.વસ્તુઓ,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_સંખ્યા--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = ખાલી;
		rev.rdiff1 = ખાલી;
	}
	rev.add_signoff = cfg.do_signoff;

	જો (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	જ્યારે (list.nr) {
		size_t idx = list.nr - 1;
		પૂર્ણાંક shown;

		display_progress(progress, total - idx);
		commit = commit_ઢગલો_કાઢો(&list);
		rev.nr = total - idx + (start_સંખ્યા - 1);

		/* Make the second and subsequent mails replies to the first */
		જો (cfg.thread) {
			/* Have we already had a message ID? */
			જો (rev.message_id) {
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
				જો (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					મુક્ત(rev.message_id);
				નહીંતર
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		જો (output_directory &&
		    open_next_file(rev.numbered_files ? ખાલી : commit, ખાલી, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_વૃક્ષ_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		જો (output_directory)
			rev.shown_one = 0;
		જો (shown) {
			print_bases(&bases, rev.diffopt.file);
			જો (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			નહીંતર
				print_signature(signature, rev.diffopt.file);
		}
		જો (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = ખાલી;
		}
	}
	stop_progress(&progress);
	commit_ઢગલો_clear(&list);
	જો (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	મુક્ત(description_file);
	મુક્ત(signature_file_arg);
	મુક્ત(signature_to_free);
	મુક્ત(branch_name);
	મુક્ત(to_free);
	મુક્ત(rev.message_id);
	જો (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	મુક્ત(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	પાછા 0;
}

સ્થિર પૂર્ણાંક add_pending_commit(અચલ અક્ષર *arg, સંરચના rev_info *revs, પૂર્ણાંક flags)
{
	સંરચના object_id oid;
	જો (repo_get_oid(the_repository, arg, &oid) == 0) {
		સંરચના commit *commit = lookup_commit_reference(the_repository,
								&oid);
		જો (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			પાછા 0;
		}
	}
	પાછા -1;
}

સ્થિર અચલ અક્ષર * અચલ cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	ખાલી
};

સ્થિર શૂન્ય print_commit(અક્ષર sign, સંરચના commit *commit, પૂર્ણાંક verbose,
			 પૂર્ણાંક abbrev, FILE *file)
{
	જો (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} નહીંતર {
		સંરચના strbuf buf = STRBUF_INIT;
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
સ્થિર શૂન્ય collect_diff_blob_oids(સંરચના commit *commit,
				   સંરચના diff_options *opts,
				   સંરચના oidset *blobs)
{
	સંરચના diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	પ્રતિજ્ઞા(!commit->parents || !commit->parents->next);

	જો (commit->parents)
		diff_વૃક્ષ_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	નહીંતર
		diff_મૂળ_વૃક્ષ_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	માટે (પૂર્ણાંક i = 0; i < q->nr; i++) {
		સંરચના diff_filepair *p = q->queue[i];
		સંરચના userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		જો (drv && drv->દ્વિઅંકી > 0)
			ચાલુ;

		જો (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, ખાલી,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ઉમેરો(blobs, &p->one->oid);
		જો (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, ખાલી,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_ઉમેરો(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

સ્થિર પૂર્ણાંક always_match(અચલ શૂન્ય *cmp_data UNUSED,
			અચલ સંરચના hashmap_entry *entry1 UNUSED,
			અચલ સંરચના hashmap_entry *entry2 UNUSED,
			અચલ શૂન્ય *keydata UNUSED)
{
	પાછા 0;
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
સ્થિર શૂન્ય prefetch_cherry_blobs(સંરચના repository *repo,
				  સંરચના commit_list *list,
				  સંરચના patch_ids *ids)
{
	સંરચના oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	જો (!repo_has_promisor_remote(repo))
		પાછા;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	માટે (સંરચના commit_list *l = list; l; l = l->next) {
		સંરચના commit *c = l->item;
		bool match_found = false;
		માટે (સંરચના patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		જો (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	જો (oidset_size(&blobs)) {
		સંરચના oid_array to_fetch = OID_ARRAY_INIT;
		સંરચના oidset_iter iter;
		અચલ સંરચના object_id *oid;

		oidset_iter_init(&blobs, &iter);
		જ્યારે ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

પૂર્ણાંક cmd_cherry(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo UNUSED)
{
	સંરચના rev_info revs;
	સંરચના patch_ids ids;
	સંરચના commit *commit;
	સંરચના commit_list *list = ખાલી;
	સંરચના branch *current_branch;
	અચલ અક્ષર *upstream;
	અચલ અક્ષર *head = "HEAD";
	અચલ અક્ષર *limit = ખાલી;
	પૂર્ણાંક verbose = 0, abbrev = 0;

	સંરચના option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	પસંદગી (argc) {
	સ્થિતિ 3:
		limit = argv[2];
		/* FALLTHROUGH */
	સ્થિતિ 2:
		head = argv[1];
		/* FALLTHROUGH */
	સ્થિતિ 1:
		upstream = argv[0];
		તોડો;
	મૂળભૂત:
		current_branch = branch_get(ખાલી);
		upstream = branch_get_upstream(current_branch, ખાલી);
		જો (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	જો (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	જો (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	જો (revs.pending.nr == 2) {
		સંરચના object_array_entry *o = revs.pending.objects;
		જો (oideq(&o[0].item->oid, &o[1].item->oid))
			પાછા 0;
	}

	get_patch_ids(&revs, &ids);

	જો (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	જો (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	જ્યારે ((commit = get_revision(&revs)) != ખાલી) {
		commit_list_ઉમેરો(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	માટે (સંરચના commit_list *l = list; l; l = l->next) {
		અક્ષર sign = '+';

		commit = l->item;
		જો (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	પાછા 0;
}
