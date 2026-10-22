/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "odb/streaming.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "diff-merges.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "log-tree.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "oidset.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "reflog-walk.h"
#அந்தர்பாவ "patch-ids.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "shortlog.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "line-log.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "version.h"
#அந்தர்பாவ "mailmap.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "commit-slab.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "utf8.h"

#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "promisor-remote.h"
#அந்தர்பாவ "range-diff.h"
#அந்தர்பாவ "tmp-objdir.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "userdiff.h"
#அந்தர்பாவ "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

நிலையான unsigned முழு force_in_body_from;
நிலையான முழு stdout_mboxrd;
நிலையான முழு format_no_prefix;

நிலையான மாறிலி எழுத்து * மாறிலி builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	வெற்று
};

கட்டமைப்பு வரி_opt_callback_data {
	கட்டமைப்பு rev_info *rev;
	மாறிலி எழுத்து *prefix;
	கட்டமைப்பு string_list args;
};

நிலையான முழு session_is_interactive(வெறுமை)
{
	திரும்பு isatty(1) || pager_in_use();
}

நிலையான முழு auto_decoration_style(வெறுமை)
{
	திரும்பு session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

நிலையான முழு parse_decoration_style(மாறிலி எழுத்து *மதிப்பு)
{
	தேர்வு (git_parse_maybe_bool(மதிப்பு)) {
	நிலை 1:
		திரும்பு DECORATE_SHORT_REFS;
	நிலை 0:
		திரும்பு 0;
	இயல்பு:
		நிறுத்து;
	}
	எனில் (!strcmp(மதிப்பு, "full"))
		திரும்பு DECORATE_FULL_REFS;
	இல்லையெனில் எனில் (!strcmp(மதிப்பு, "short"))
		திரும்பு DECORATE_SHORT_REFS;
	இல்லையெனில் எனில் (!strcmp(மதிப்பு, "auto"))
		திரும்பு auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	திரும்பு -1;
}

கட்டமைப்பு log_config {
	முழு default_abbrev_commit;
	முழு default_show_வேர்;
	முழு default_follow;
	முழு default_show_signature;
	முழு default_encode_email_headers;
	முழு decoration_style;
	முழு decoration_given;
	முழு use_mailmap_config;
	எழுத்து *fmt_patch_subject_prefix;
	முழு fmt_patch_name_max;
	எழுத்து *fmt_pretty;
	எழுத்து *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	முழு i_still_use_this;
#endif
};

நிலையான வெறுமை log_config_init(கட்டமைப்பு log_config *cfg)
{
	memset(cfg, 0, அளவு(*cfg));
	cfg->default_show_வேர் = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

நிலையான வெறுமை log_config_release(கட்டமைப்பு log_config *cfg)
{
	விடுவி(cfg->default_date_mode);
	விடுவி(cfg->fmt_pretty);
	விடுவி(cfg->fmt_patch_subject_prefix);
}

நிலையான முழு use_default_decoration_filter = 1;
நிலையான கட்டமைப்பு string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
நிலையான கட்டமைப்பு string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
நிலையான கட்டமைப்பு string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

நிலையான முழு clear_decorations_callback(மாறிலி கட்டமைப்பு option *opt UNUSED,
				      மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	திரும்பு 0;
}

நிலையான முழு decorate_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			     முழு unset)
{
	கட்டமைப்பு log_config *cfg = opt->மதிப்பு;

	எனில் (unset)
		cfg->decoration_style = 0;
	இல்லையெனில் எனில் (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	இல்லையெனில்
		cfg->decoration_style = DECORATE_SHORT_REFS;

	எனில் (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	திரும்பு 0;
}

நிலையான முழு log_வரி_range_callback(மாறிலி கட்டமைப்பு option *option, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு வரி_opt_callback_data *data = option->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!arg)
		திரும்பு -1;

	data->rev->வரி_level_traverse = 1;
	string_list_append(&data->args, arg);

	திரும்பு 0;
}

நிலையான வெறுமை cmd_log_init_defaults(கட்டமைப்பு rev_info *rev,
				  கட்டமைப்பு log_config *cfg)
{
	எனில் (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	எனில் (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_வேர்_diff = cfg->default_show_வேர்;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	எனில் (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

நிலையான வெறுமை set_default_decoration_filter(கட்டமைப்பு decoration_filter *decoration_filter)
{
	எழுத்து *மதிப்பு = வெற்று;
	கட்டமைப்பு string_list *include = decoration_filter->include_ref_pattern;
	மாறிலி கட்டமைப்பு string_list *config_exclude;

	எனில் (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		கட்டமைப்பு string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	எனில் (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &மதிப்பு) &&
	    !strcmp("all", மதிப்பு))
		use_default_decoration_filter = 0;
	விடுவி(மதிப்பு);

	எனில் (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		திரும்பு;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	க்கு (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		எனில் (!ref_namespace[i].decoration)
			தொடர்;

		string_list_append(include, ref_namespace[i].ref);
	}
}

நிலையான வெறுமை cmd_log_init_finish(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு rev_info *rev, கட்டமைப்பு setup_revision_opt *opt,
			 கட்டமைப்பு log_config *cfg)
{
	கட்டமைப்பு userformat_want w;
	முழு quiet = 0, source = 0, mailmap;
	நிலையான கட்டமைப்பு வரி_opt_callback_data வரி_cb = {வெற்று, வெற்று, STRING_LIST_INIT_DUP};
	கட்டமைப்பு decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	நிலையான கட்டமைப்பு revision_sources revision_sources;

	மாறிலி கட்டமைப்பு option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", வெற்று, வெற்று,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, வெற்று, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', வெற்று, &வரி_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_வரி_range_callback),
		OPT_END()
	};

	வரி_cb.rev = rev;
	வரி_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	எனில் (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	எனில் (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	எனில் (rev->வரி_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, அளவு(w));
	userformat_find_requirements(வெற்று, &w);

	எனில் (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	எனில் (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	எனில் ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	எனில் (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	எனில் (mailmap) {
		rev->mailmap = xmalloc(அளவு(கட்டமைப்பு string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	எனில் (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		எனில் (!cfg->decoration_given)
			cfg->decoration_style = 0;
		எனில் (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	எனில் (rev->commit_format == CMIT_FMT_USERFORMAT) {
		எனில் (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} இல்லையெனில் எனில் (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	எனில் (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		எனில் (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	எனில் (rev->வரி_level_traverse)
		வரி_log_init(rev, வரி_cb.prefix, &வரி_cb.args);

	setup_pager(the_repository);
}

நிலையான வெறுமை cmd_log_init(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு rev_info *rev, கட்டமைப்பு setup_revision_opt *opt,
			 கட்டமைப்பு log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

நிலையான முழு cmd_log_walk_no_free(கட்டமைப்பு rev_info *rev)
{
	கட்டமைப்பு commit *commit;
	முழு saved_nrl = 0;
	முழு saved_dcctc = 0;
	முழு result;

	எனில் (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	போது ((commit = get_revision(rev)) != வெற்று) {
		எனில் (!log_மரம்_commit(rev, commit) && rev->max_மொத்தம் >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_மொத்தம்++;
		எனில் (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = வெற்று;
		}
		எனில் (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		எனில் (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	எனில் (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	திரும்பு result;
}

நிலையான முழு cmd_log_walk(கட்டமைப்பு rev_info *rev)
{
	முழு retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	திரும்பு retval;
}

நிலையான முழு git_log_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			  மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு log_config *cfg = cb;
	மாறிலி எழுத்து *slot_name;

	எனில் (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		திரும்பு git_config_string(&cfg->fmt_pretty, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		திரும்பு git_config_string(&cfg->fmt_patch_subject_prefix, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, மதிப்பு, ctx->kvi);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		திரும்பு git_config_string(&cfg->default_date_mode, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(மதிப்பு);
		எனில் (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "log.diffmerges")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		திரும்பு diff_merges_config(மதிப்பு);
	}
	எனில் (!strcmp(var, "log.showroot")) {
		cfg->default_show_வேர் = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (skip_prefix(var, "color.decorate.", &slot_name))
		திரும்பு parse_decorate_color_config(var, slot_name, மதிப்பு);
	எனில் (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	திரும்பு git_diff_ui_config(var, மதிப்பு, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
முழு cmd_whatchanged(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு log_config cfg;
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு setup_revision_opt opt;
	முழு ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, அளவு(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	எனில் (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	எனில் (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	திரும்பு ret;
}
#endif

நிலையான வெறுமை show_tagger(மாறிலி எழுத்து *buf, கட்டமைப்பு rev_info *rev)
{
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	கட்டமைப்பு pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

நிலையான முழு show_blob_object(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு rev_info *rev, மாறிலி எழுத்து *obj_name)
{
	கட்டமைப்பு object_id oidc;
	கட்டமைப்பு object_context obj_context = {0};
	எழுத்து *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	எனில் (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		திரும்பு odb_stream_blob_to_fd(the_repository->objects, 1, oid, வெற்று, 0);

	எனில் (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	எனில் (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		திரும்பு odb_stream_blob_to_fd(the_repository->objects, 1, oid, வெற்று, 0);
	}

	எனில் (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	விடுவி(buf);
	திரும்பு 0;
}

நிலையான முழு show_tag_object(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு rev_info *rev)
{
	size_t size;
	எண்ணிக்கை object_type type;
	எழுத்து *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	எனில் (!buf)
		திரும்பு error(_("could not read object %s"), oid_to_hex(oid));

	ப்ரதிஜ்ஞா(type == OBJ_TAG);
	போது (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		மாறிலி எழுத்து *ident;
		போது (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		எனில் (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	எனில் (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	விடுவி(buf);
	திரும்பு 0;
}

நிலையான முழு show_மரம்_object(மாறிலி கட்டமைப்பு object_id *oid UNUSED,
			    கட்டமைப்பு strbuf *base UNUSED,
			    மாறிலி எழுத்து *pathname, unsigned mode,
			    வெறுமை *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	திரும்பு 0;
}

நிலையான வெறுமை show_setup_revisions_tweak(கட்டமைப்பு rev_info *rev)
{
	எனில் (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	இல்லையெனில்
		diff_merges_default_to_dense_combined(rev);
	எனில் (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

முழு cmd_show(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு log_config cfg;
	கட்டமைப்பு rev_info rev;
	unsigned முழு i;
	கட்டமைப்பு setup_revision_opt opt;
	கட்டமைப்பு pathspec match_all;
	முழு ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	எனில் (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, அளவு(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, அளவு(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	எனில் (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		திரும்பு ret;
	}

	rev.diffopt.no_free = 1;
	க்கு (i = 0; i < rev.pending.nr && !ret; i++) {
		கட்டமைப்பு object *o = rev.pending.objects[i].item;
		மாறிலி எழுத்து *name = rev.pending.objects[i].name;
		தேர்வு (o->type) {
		நிலை OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			நிறுத்து;
		நிலை OBJ_TAG: {
			கட்டமைப்பு tag *t = (கட்டமைப்பு tag *)o;
			கட்டமைப்பு object_id *oid = get_tagged_oid(t);

			எனில் (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			எனில் (ret)
				நிறுத்து;
			o = parse_object(the_repository, oid);
			எனில் (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			நிறுத்து;
		}
		நிலை OBJ_மரம்:
			எனில் (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_மரம்(the_repository, (கட்டமைப்பு மரம் *)o,
				  &match_all, show_மரம்_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			நிறுத்து;
		நிலை OBJ_COMMIT:
		{
			கட்டமைப்பு object_array old;
			கட்டமைப்பு object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, அளவு(old));
			memcpy(&rev.pending, &blank, அளவு(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, அளவு(rev.pending));
			நிறுத்து;
		}
		இயல்பு:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	திரும்பு ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
முழு cmd_log_reflog(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு log_config cfg;
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு setup_revision_opt opt;
	முழு ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, அளவு(opt));
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
	திரும்பு ret;
}

நிலையான வெறுமை log_setup_revisions_tweak(கட்டமைப்பு rev_info *rev)
{
	எனில் (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	எனில் (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

முழு cmd_log(முழு argc,
	    மாறிலி எழுத்து **argv,
	    மாறிலி எழுத்து *prefix,
	    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு log_config cfg;
	கட்டமைப்பு rev_info rev;
	கட்டமைப்பு setup_revision_opt opt;
	முழு ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, அளவு(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	திரும்பு ret;
}

/* format-patch */

எண்ணிக்கை cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

எண்ணிக்கை thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

எண்ணிக்கை cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

எண்ணிக்கை auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

கட்டமைப்பு format_config {
	கட்டமைப்பு log_config log;
	எண்ணிக்கை thread_level thread;
	முழு do_signoff;
	எண்ணிக்கை auto_base_setting auto_base;
	எழுத்து *base_commit;
	எழுத்து *from;
	எழுத்து *signature;
	எழுத்து *signature_file;
	எண்ணிக்கை cover_setting config_cover_letter;
	எழுத்து *fmt_cover_letter_commit_list;
	எழுத்து *config_output_directory;
	எண்ணிக்கை cover_from_description cover_from_description_mode;
	முழு show_notes;
	கட்டமைப்பு display_notes_opt notes_opt;
	முழு numbered_cmdline_opt;
	முழு numbered;
	முழு auto_எண்;
	எழுத்து *default_attach;
	கட்டமைப்பு string_list extra_hdr;
	கட்டமைப்பு string_list extra_to;
	கட்டமைப்பு string_list extra_cc;
	முழு keep_subject;
	முழு subject_prefix;
	கட்டமைப்பு strbuf sprefix;
	எழுத்து *fmt_patch_suffix;
};

நிலையான வெறுமை format_config_init(கட்டமைப்பு format_config *cfg)
{
	memset(cfg, 0, அளவு(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_எண் = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

நிலையான வெறுமை format_config_release(கட்டமைப்பு format_config *cfg)
{
	log_config_release(&cfg->log);
	விடுவி(cfg->base_commit);
	விடுவி(cfg->from);
	விடுவி(cfg->signature);
	விடுவி(cfg->signature_file);
	விடுவி(cfg->config_output_directory);
	விடுவி(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	விடுவி(cfg->fmt_patch_suffix);
	விடுவி(cfg->fmt_cover_letter_commit_list);
}

நிலையான எண்ணிக்கை cover_from_description parse_cover_from_description(மாறிலி எழுத்து *arg)
{
	எனில் (!arg || !strcmp(arg, "default"))
		திரும்பு COVER_FROM_MESSAGE;
	இல்லையெனில் எனில் (!strcmp(arg, "none"))
		திரும்பு COVER_FROM_NONE;
	இல்லையெனில் எனில் (!strcmp(arg, "message"))
		திரும்பு COVER_FROM_MESSAGE;
	இல்லையெனில் எனில் (!strcmp(arg, "subject"))
		திரும்பு COVER_FROM_SUBJECT;
	இல்லையெனில் எனில் (!strcmp(arg, "auto"))
		திரும்பு COVER_FROM_AUTO;
	இல்லையெனில்
		die(_("%s: invalid cover from description mode"), arg);
}

நிலையான வெறுமை add_header(கட்டமைப்பு format_config *cfg, மாறிலி எழுத்து *மதிப்பு)
{
	கட்டமைப்பு string_list_item *item;
	முழு len = strlen(மதிப்பு);
	போது (len && மதிப்பு[len - 1] == '\n')
		len--;

	எனில் (!strncasecmp(மதிப்பு, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, மதிப்பு + 4);
		len -= 4;
	} இல்லையெனில் எனில் (!strncasecmp(மதிப்பு, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, மதிப்பு + 4);
		len -= 4;
	} இல்லையெனில் {
		item = string_list_append(&cfg->extra_hdr, மதிப்பு);
	}

	item->string[len] = '\0';
}

நிலையான முழு git_format_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			     மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	கட்டமைப்பு format_config *cfg = cb;

	எனில் (!strcmp(var, "format.headers")) {
		எனில் (!மதிப்பு)
			die(_("format.headers without value"));
		add_header(cfg, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		திரும்பு git_config_string(&cfg->fmt_patch_suffix, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.to")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		string_list_append(&cfg->extra_to, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.cc")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.numbered")) {
		எனில் (மதிப்பு && !strcasecmp(மதிப்பு, "auto")) {
			cfg->auto_எண் = 1;
			திரும்பு 0;
		}
		cfg->numbered = git_config_bool(var, மதிப்பு);
		cfg->auto_எண் = cfg->auto_எண் && cfg->numbered;
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.attach")) {
		எனில் (மதிப்பு && *மதிப்பு) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(மதிப்பு);
		} இல்லையெனில் எனில் (மதிப்பு && !*மதிப்பு) {
			FREE_AND_NULL(cfg->default_attach);
		} இல்லையெனில் {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.thread")) {
		எனில் (மதிப்பு && !strcasecmp(மதிப்பு, "deep")) {
			cfg->thread = THREAD_DEEP;
			திரும்பு 0;
		}
		எனில் (மதிப்பு && !strcasecmp(மதிப்பு, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			திரும்பு 0;
		}
		cfg->thread = git_config_bool(var, மதிப்பு) ? THREAD_SHALLOW : THREAD_UNSET;
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		திரும்பு git_config_string(&cfg->signature, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		திரும்பு git_config_pathname(&cfg->signature_file, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.coverletter")) {
		எனில் (மதிப்பு && !strcasecmp(மதிப்பு, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			திரும்பு 0;
		}
		cfg->config_cover_letter = git_config_bool(var, மதிப்பு) ? COVER_ON : COVER_OFF;
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		திரும்பு git_config_string(&cfg->fmt_cover_letter_commit_list, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		திரும்பு git_config_string(&cfg->config_output_directory, var, மதிப்பு);
	}
	எனில் (!strcmp(var, "format.useautobase")) {
		எனில் (மதிப்பு && !strcasecmp(மதிப்பு, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			திரும்பு 0;
		}
		cfg->auto_base = git_config_bool(var, மதிப்பு) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.from")) {
		முழு b = git_parse_maybe_bool(மதிப்பு);
		FREE_AND_NULL(cfg->from);
		எனில் (b < 0)
			cfg->from = xstrdup(மதிப்பு);
		இல்லையெனில் எனில் (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.notes")) {
		முழு b = git_parse_maybe_bool(மதிப்பு);
		எனில் (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, மதிப்பு);
		இல்லையெனில் எனில் (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		இல்லையெனில்
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(மதிப்பு);
		எனில் (format_no_prefix < 0) {
			முழு status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				மதிப்பு, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		திரும்பு 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	எனில் (!strcmp(var, "diff.noprefix"))
		திரும்பு 0;

	திரும்பு git_log_config(var, மதிப்பு, ctx, &cfg->log);
}

நிலையான மாறிலி எழுத்து *output_directory = வெற்று;
நிலையான முழு outdir_offset;

நிலையான முழு open_next_file(கட்டமைப்பு commit *commit, மாறிலி எழுத்து *subject,
			 கட்டமைப்பு rev_info *rev, முழு quiet)
{
	கட்டமைப்பு strbuf filename = STRBUF_INIT;

	எனில் (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	எனில் (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	இல்லையெனில் எனில் (commit)
		fmt_output_commit(&filename, commit, rev);
	இல்லையெனில்
		fmt_output_subject(&filename, subject, rev);

	எனில் (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	எனில் (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		திரும்பு -1;
	}

	strbuf_release(&filename);
	திரும்பு 0;
}

நிலையான வெறுமை get_patch_ids(கட்டமைப்பு rev_info *rev, கட்டமைப்பு patch_ids *ids)
{
	கட்டமைப்பு rev_info check_rev;
	கட்டமைப்பு commit *commit, *c1, *c2;
	கட்டமைப்பு object *o1, *o2;
	unsigned flags1, flags2;

	எனில் (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	எனில் ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	எனில் (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	போது ((commit = get_revision(&check_rev)) != வெற்று) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

நிலையான வெறுமை gen_message_id(கட்டமைப்பு rev_info *info, மாறிலி எழுத்து *base)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(வெற்று),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, வெற்று);
}

நிலையான வெறுமை print_signature(மாறிலி எழுத்து *signature, FILE *file)
{
	எனில் (!signature || !*signature)
		திரும்பு;

	fprintf(file, "-- \n%s", signature);
	எனில் (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

நிலையான எழுத்து *find_branch_name(கட்டமைப்பு rev_info *rev)
{
	கட்டமைப்பு object_id branch_oid;
	மாறிலி கட்டமைப்பு object_id *tip_oid;
	மாறிலி எழுத்து *ref, *v;
	எழுத்து *full_ref, *branch = வெற்று;
	முழு interesting_found = 0;
	size_t idx;

	க்கு (size_t i = 0; i < rev->cmdline.nr; i++) {
		எனில் (rev->cmdline.rev[i].flags & UNINTERESTING)
			தொடர்;
		எனில் (interesting_found)
			திரும்பு வெற்று;
		interesting_found = 1;
		idx = i;
	}
	எனில் (!interesting_found)
		திரும்பு வெற்று;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	எனில் (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	விடுவி(full_ref);
	திரும்பு branch;
}

நிலையான வெறுமை show_diffstat(கட்டமைப்பு rev_info *rev,
			  கட்டமைப்பு commit *origin, கட்டமைப்பு commit *head)
{
	கட்டமைப்பு diff_options opts;

	memcpy(&opts, &rev->diffopt, அளவு(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_மரம்_oid(get_commit_மரம்_oid(origin),
		      get_commit_மரம்_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

நிலையான வெறுமை read_desc_file(கட்டமைப்பு strbuf *buf, மாறிலி எழுத்து *desc_file)
{
	எனில் (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

நிலையான வெறுமை prepare_cover_text(கட்டமைப்பு pretty_print_context *pp,
			       மாறிலி எழுத்து *description_file,
			       மாறிலி எழுத்து *branch_name,
			       கட்டமைப்பு strbuf *sb,
			       மாறிலி எழுத்து *encoding,
			       முழு need_8bit_cte,
			       மாறிலி கட்டமைப்பு format_config *cfg)
{
	மாறிலி எழுத்து *subject = "*** SUBJECT HERE ***";
	மாறிலி எழுத்து *body = "*** BLURB HERE ***";
	கட்டமைப்பு strbuf description_sb = STRBUF_INIT;
	கட்டமைப்பு strbuf subject_sb = STRBUF_INIT;

	எனில் (cfg->cover_from_description_mode == COVER_FROM_NONE)
		செல் do_pp;

	எனில் (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	இல்லையெனில் எனில் (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	எனில் (!description_sb.len)
		செல் do_pp;

	எனில் (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	எனில் (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	இல்லையெனில்
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

கட்டமைப்பு rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	கட்டமைப்பு string_list notes;
};

நிலையான முழு rdiff_notes_cb(மாறிலி கட்டமைப்பு option *option,
			  மாறிலி எழுத்து *arg,
			  முழு unset)
{
	கட்டமைப்பு option opt = *option;
	கட்டமைப்பு rdiff_notes *rdiff_notes = option->மதிப்பு;

	rdiff_notes->override = 1;
	opt.மதிப்பு = &rdiff_notes->notes;
	திரும்பு parse_opt_string_list(&opt, arg, unset);
}

நிலையான முழு get_notes_refs(கட்டமைப்பு string_list_item *item, வெறுமை *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	திரும்பு 0;
}

நிலையான வெறுமை get_notes_args(கட்டமைப்பு rdiff_notes *rdiff_notes,
			   கட்டமைப்பு rev_info *rev)
{
	எனில் (rdiff_notes->override) {
		எனில் (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		இல்லையெனில்
			strvec_தள்ளு(&rev->rdiff_log_arg, "--no-notes");
	} இல்லையெனில் எனில் (!rev->show_notes) {
		strvec_தள்ளு(&rev->rdiff_log_arg, "--no-notes");
	} இல்லையெனில் எனில் (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_தள்ளு(&rev->rdiff_log_arg, "--notes");
	} இல்லையெனில் {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

நிலையான வெறுமை generate_shortlog_cover_letter(கட்டமைப்பு shortlog *log,
					   கட்டமைப்பு rev_info *rev,
					   கட்டமைப்பு commit **list,
					   முழு nr)
{
	shortlog_init(log);
	log->wrap_வரிகள் = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	க்கு (முழு i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

நிலையான வெறுமை generate_commit_list_cover(FILE *cover_file, மாறிலி எழுத்து *format,
				       கட்டமைப்பு commit **list, முழு n)
{
	கட்டமைப்பு strbuf commit_வரி = STRBUF_INIT;
	கட்டமைப்பு pretty_print_context ctx = {0};
	கட்டமைப்பு rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	க்கு (முழு i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_வரி, &ctx);
		fprintf(cover_file, "%s\n", commit_வரி.buf);
		strbuf_reset(&commit_வரி);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_வரி);
}

நிலையான வெறுமை make_cover_letter(கட்டமைப்பு rev_info *rev, முழு use_separate_file,
			      கட்டமைப்பு commit *origin,
			      முழு nr, கட்டமைப்பு commit **list,
			      மாறிலி எழுத்து *description_file,
			      மாறிலி எழுத்து *branch_name,
			      முழு quiet,
			      மாறிலி கட்டமைப்பு format_config *cfg,
			      மாறிலி எழுத்து *format)
{
	மாறிலி எழுத்து *from;
	கட்டமைப்பு shortlog log;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு i;
	மாறிலி எழுத்து *encoding = "UTF-8";
	முழு need_8bit_cte = 0;
	கட்டமைப்பு pretty_print_context pp = {0};
	கட்டமைப்பு commit *head = list[0];
	எழுத்து *to_free = வெற்று;

	எனில் (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	எனில் (use_separate_file &&
	    open_next_file(வெற்று, rev->numbered_files ? வெற்று : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	க்கு (i = 0; !need_8bit_cte && i < nr; i++) {
		மாறிலி எழுத்து *buf = repo_get_commit_buffer(the_repository,
							 list[i], வெற்று);
		எனில் (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	எனில் (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, வெற்று, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	விடுவி(to_free);
	விடுவி(pp.after_subject);
	strbuf_release(&sb);

	எனில் (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	இல்லையெனில் எனில் (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	இல்லையெனில் எனில் (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	இல்லையெனில் எனில் (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	இல்லையெனில்
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	எனில் (origin)
		show_diffstat(rev, origin, head);

	எனில் (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	எனில் (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		கட்டமைப்பு diff_options opts;
		கட்டமைப்பு range_diff_options range_diff_opts = {
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

நிலையான எழுத்து *clean_message_id(மாறிலி எழுத்து *செய்தி_id)
{
	எழுத்து ch;
	மாறிலி எழுத்து *a, *z, *m;

	m = செய்தி_id;
	போது ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = வெற்று;
	போது ((ch = *m)) {
		எனில் (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	எனில் (!z)
		die(_("insane in-reply-to: %s"), செய்தி_id);
	எனில் (++z == m)
		திரும்பு xstrdup(a);
	திரும்பு xmemdupz(a, z - a);
}

நிலையான மாறிலி எழுத்து *set_outdir(மாறிலி எழுத்து *prefix, மாறிலி எழுத்து *output_directory)
{
	எனில் (output_directory && is_absolute_path(output_directory))
		திரும்பு output_directory;

	எனில் (!prefix || !*prefix) {
		எனில் (output_directory)
			திரும்பு output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		திரும்பு "./";
	}

	outdir_offset = strlen(prefix);
	எனில் (!output_directory)
		திரும்பு prefix;

	திரும்பு prefix_filename(prefix, output_directory);
}

நிலையான மாறிலி எழுத்து * மாறிலி builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	வெற்று
};

கட்டமைப்பு keep_callback_data {
	கட்டமைப்பு format_config *cfg;
	கட்டமைப்பு rev_info *revs;
};

நிலையான முழு keep_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு keep_callback_data *data = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	திரும்பு 0;
}

நிலையான முழு subject_prefix_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			    முழு unset)
{
	கட்டமைப்பு format_config *cfg = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	திரும்பு 0;
}

நிலையான முழு rfc_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			முழு unset)
{
	மாறிலி எழுத்து **rfc = opt->மதிப்பு;

	*rfc = opt->மதிப்பு;
	எனில் (unset)
		*rfc = வெற்று;
	இல்லையெனில்
		*rfc = arg ? arg : "RFC";
	திரும்பு 0;
}

நிலையான முழு numbered_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			     முழு unset)
{
	கட்டமைப்பு format_config *cfg = opt->மதிப்பு;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	எனில் (unset)
		cfg->auto_எண் =  0;
	திரும்பு 0;
}

நிலையான முழு no_numbered_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
				முழு unset)
{
	BUG_ON_OPT_NEG(unset);
	திரும்பு numbered_callback(opt, arg, 1);
}

நிலையான முழு output_directory_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			      முழு unset)
{
	மாறிலி எழுத்து **dir = (மாறிலி எழுத்து **)opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	எனில் (*dir)
		die(_("two output directories?"));
	*dir = arg;
	திரும்பு 0;
}

நிலையான முழு thread_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு format_config *cfg = opt->மதிப்பு;

	எனில் (unset)
		cfg->thread = THREAD_UNSET;
	இல்லையெனில் எனில் (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	இல்லையெனில் எனில் (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	இல்லையெனில்
		திரும்பு 1;
	திரும்பு 0;
}

நிலையான முழு attach_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு rev_info *rev = (கட்டமைப்பு rev_info *)opt->மதிப்பு;
	எனில் (unset)
		rev->mime_boundary = வெற்று;
	இல்லையெனில் எனில் (arg)
		rev->mime_boundary = arg;
	இல்லையெனில்
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	திரும்பு 0;
}

நிலையான முழு inline_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு rev_info *rev = (கட்டமைப்பு rev_info *)opt->மதிப்பு;
	எனில் (unset)
		rev->mime_boundary = வெற்று;
	இல்லையெனில் எனில் (arg)
		rev->mime_boundary = arg;
	இல்லையெனில்
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	திரும்பு 0;
}

நிலையான முழு header_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			   முழு unset)
{
	கட்டமைப்பு format_config *cfg = opt->மதிப்பு;

	எனில் (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} இல்லையெனில் {
		add_header(cfg, arg);
	}
	திரும்பு 0;
}

நிலையான முழு from_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	எழுத்து **from = opt->மதிப்பு;

	விடுவி(*from);

	எனில் (unset)
		*from = வெற்று;
	இல்லையெனில் எனில் (arg)
		*from = xstrdup(arg);
	இல்லையெனில்
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	திரும்பு 0;
}

நிலையான முழு base_callback(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு format_config *cfg = opt->மதிப்பு;

	எனில் (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} இல்லையெனில் எனில் (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} இல்லையெனில் {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	திரும்பு 0;
}

கட்டமைப்பு base_மரம்_info {
	கட்டமைப்பு object_id base_commit;
	முழு nr_patch_id, alloc_patch_id;
	கட்டமைப்பு object_id *patch_id;
};

நிலையான கட்டமைப்பு commit *get_base_commit(மாறிலி கட்டமைப்பு format_config *cfg,
				      கட்டமைப்பு commit **list,
				      size_t total)
{
	கட்டமைப்பு commit *base = வெற்று;
	கட்டமைப்பு commit **rev;
	முழு auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	தேர்வு (cfg->auto_base) {
	நிலை AUTO_BASE_NEVER:
		எனில் (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} இல்லையெனில் {
			/* no base information is requested */
			திரும்பு வெற்று;
		}
		நிறுத்து;
	நிலை AUTO_BASE_ALWAYS:
	நிலை AUTO_BASE_WHEN_ABLE:
		எனில் (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} இல்லையெனில் {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		நிறுத்து;
	இயல்பு:
		BUG("unexpected automatic base selection method");
	}

	எனில் (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		எனில் (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} இல்லையெனில் {
		கட்டமைப்பு branch *curr_branch = branch_get(வெற்று);
		மாறிலி எழுத்து *upstream = branch_get_upstream(curr_branch, வெற்று);
		எனில் (upstream) {
			கட்டமைப்பு commit_list *base_list = வெற்று;
			கட்டமைப்பு commit *commit;
			கட்டமைப்பு object_id oid;

			எனில் (repo_get_oid(the_repository, upstream, &oid)) {
				எனில் (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				இல்லையெனில்
					திரும்பு வெற்று;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			எனில் (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				எனில் (die_on_failure) {
					die(_("could not find exact merge base"));
				} இல்லையெனில் {
					commit_list_free(base_list);
					திரும்பு வெற்று;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} இல்லையெனில் {
			எனில் (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			இல்லையெனில்
				திரும்பு வெற்று;
		}
	}

	ALLOC_ARRAY(rev, total);
	க்கு (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	போது (rev_nr > 1) {
		க்கு (i = 0; i < rev_nr / 2; i++) {
			கட்டமைப்பு commit_list *merge_base = வெற்று;
			எனில் (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				எனில் (die_on_failure) {
					die(_("failed to find exact merge base"));
				} இல்லையெனில் {
					commit_list_free(merge_base);
					விடுவி(rev);
					திரும்பு வெற்று;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		எனில் (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	எனில் (ret < 0)
		exit(128);
	எனில் (!ret) {
		எனில் (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} இல்லையெனில் {
			விடுவி(rev);
			திரும்பு வெற்று;
		}
	}

	க்கு (i = 0; i < total; i++) {
		எனில் (base == list[i]) {
			எனில் (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} இல்லையெனில் {
				விடுவி(rev);
				திரும்பு வெற்று;
			}
		}
	}

	விடுவி(rev);
	திரும்பு base;
}

define_commit_slab(commit_base, முழு);

நிலையான வெறுமை prepare_bases(கட்டமைப்பு base_மரம்_info *bases,
			  கட்டமைப்பு commit *base,
			  கட்டமைப்பு commit **list,
			  size_t total)
{
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு diff_options diffopt;
	கட்டமைப்பு commit_base commit_base;

	எனில் (!base)
		திரும்பு;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, வெற்று);
	revs.max_parents = 1;
	revs.topo_order = 1;
	க்கு (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	எனில் (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	போது ((commit = get_revision(&revs)) != வெற்று) {
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு object_id *patch_id;
		எனில் (*commit_base_at(&commit_base, commit))
			தொடர்;
		எனில் (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

நிலையான வெறுமை print_bases(கட்டமைப்பு base_மரம்_info *bases, FILE *file)
{
	முழு i;

	/* Only do this once, either for the cover or for the first one */
	எனில் (is_null_oid(&bases->base_commit))
		திரும்பு;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	க்கு (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	விடுவி(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

நிலையான மாறிலி எழுத்து *diff_title(கட்டமைப்பு strbuf *sb,
			      மாறிலி எழுத்து *reroll_மொத்தம்,
			      மாறிலி எழுத்து *generic,
			      மாறிலி எழுத்து *rerolled)
{
	முழு v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	எனில் (reroll_மொத்தம் && !strtol_i(reroll_மொத்தம், 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	இல்லையெனில்
		strbuf_addstr(sb, generic);
	திரும்பு sb->buf;
}

நிலையான வெறுமை infer_range_diff_ranges(கட்டமைப்பு strbuf *r1,
				    கட்டமைப்பு strbuf *r2,
				    மாறிலி எழுத்து *prev,
				    கட்டமைப்பு commit *origin,
				    கட்டமைப்பு commit *head)
{
	மாறிலி எழுத்து *head_oid = oid_to_hex(&head->object.oid);
	முழு prev_is_range = is_range_diff_range(prev);

	எனில் (prev_is_range)
		strbuf_addstr(r1, prev);
	இல்லையெனில்
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	எனில் (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	இல்லையெனில் எனில் (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	இல்லையெனில் {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

முழு cmd_format_patch(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு format_config cfg;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு commit_அடுக்கு list = COMMIT_அடுக்கு_INIT;
	கட்டமைப்பு rev_info rev;
	எழுத்து *to_free = வெற்று;
	கட்டமைப்பு setup_revision_opt s_r_opt;
	size_t total, i;
	முழு use_stdout = 0;
	முழு start_எண் = -1;
	முழு just_numbers = 0;
	முழு ignore_if_in_upstream = 0;
	முழு cover_letter = -1;
	மாறிலி எழுத்து *cover_letter_fmt = வெற்று;
	முழு boundary_மொத்தம் = 0;
	முழு no_இருமம்_diff = 0;
	முழு zero_commit = 0;
	கட்டமைப்பு commit *origin = வெற்று;
	மாறிலி எழுத்து *in_reply_to = வெற்று;
	கட்டமைப்பு patch_ids ids;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	முழு use_patch_format = 0;
	முழு quiet = 0;
	மாறிலி எழுத்து *reroll_மொத்தம் = வெற்று;
	எழுத்து *cover_from_description_arg = வெற்று;
	எழுத்து *description_file = வெற்று;
	எழுத்து *branch_name = வெற்று;
	கட்டமைப்பு base_மரம்_info bases;
	கட்டமைப்பு commit *base;
	முழு show_progress = 0;
	கட்டமைப்பு progress *progress = வெற்று;
	கட்டமைப்பு oid_array idiff_prev = OID_ARRAY_INIT;
	கட்டமைப்பு strbuf idiff_title = STRBUF_INIT;
	மாறிலி எழுத்து *rdiff_prev = வெற்று;
	கட்டமைப்பு strbuf rdiff1 = STRBUF_INIT;
	கட்டமைப்பு strbuf rdiff2 = STRBUF_INIT;
	கட்டமைப்பு strbuf rdiff_title = STRBUF_INIT;
	கட்டமைப்பு rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	மாறிலி எழுத்து *rfc = வெற்று;
	முழு creation_factor = -1;
	மாறிலி எழுத்து *signature = git_version_string;
	எழுத்து *signature_to_free = வெற்று;
	எழுத்து *signature_file_arg = வெற்று;
	கட்டமைப்பு keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	மாறிலி எழுத்து *fmt_patch_suffix = வெற்று;

	மாறிலி கட்டமைப்பு option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, வெற்று,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, வெற்று,
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
		OPT_INTEGER(0, "start-number", &start_எண்,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_மொத்தம், N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, வெற்று,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_இருமம்_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, அளவு(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, அளவு(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	எனில் (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	எனில் (cfg.default_attach) {
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

	எனில் (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	எனில் (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	எனில் (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	எனில் (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		எனில் (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		இல்லையெனில்
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	எனில் (reroll_மொத்தம்) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_மொத்தம்);
		rev.reroll_மொத்தம் = reroll_மொத்தம்;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	க்கு (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.பொருட்கள்[i].string);
		strbuf_addch(&buf, '\n');
	}

	எனில் (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	க்கு (i = 0; i < cfg.extra_to.nr; i++) {
		எனில் (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.பொருட்கள்[i].string);
		எனில் (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	எனில் (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	க்கு (i = 0; i < cfg.extra_cc.nr; i++) {
		எனில் (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.பொருட்கள்[i].string);
		எனில் (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, வெற்று);

	எனில் (cfg.from) {
		எனில் (split_ident_வரி(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	எனில் (start_எண் < 0)
		start_எண் = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	எனில் (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	எனில் (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	எனில் (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	எனில் (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	எனில் (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	எனில் (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	எனில் (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	எனில் (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	எனில் (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	எனில் (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	எனில் (!rev.diffopt.flags.text && !no_இருமம்_diff)
		rev.diffopt.flags.இருமம் = 1;

	எனில் (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	எனில் (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	எனில் (use_stdout) {
		setup_pager(the_repository);
	} இல்லையெனில் எனில் (!rev.diffopt.close_file) {
		முழு saved;

		எனில் (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		எனில் (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		தேர்வு (safe_create_leading_directories_const(the_repository, output_directory)) {
		நிலை SCLD_OK:
		நிலை SCLD_EXISTS:
			நிறுத்து;
		இயல்பு:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		எனில் (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	எனில் (rev.pending.nr == 1) {
		முழு check_head = 0;

		எனில் (rev.max_மொத்தம் < 0 && !rev.show_வேர்_diff) {
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

		எனில் (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		எனில் (check_head) {
			மாறிலி எழுத்து *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      வெற்று, வெற்று);
			எனில் (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			இல்லையெனில்
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_வேர்_diff = 1;

	எனில் (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		எனில் (rev.pending.nr == 2) {
			கட்டமைப்பு object_array_entry *o = rev.pending.objects;
			எனில் (oideq(&o[0].item->oid, &o[1].item->oid))
				செல் done;
		}
		get_patch_ids(&rev, &ids);
	}

	எனில் (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	போது ((commit = get_revision(&rev)) != வெற்று) {
		எனில் (commit->object.flags & BOUNDARY) {
			boundary_மொத்தம்++;
			origin = (boundary_மொத்தம் == 1) ? commit : வெற்று;
			தொடர்;
		}

		எனில் (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			தொடர்;

		commit_அடுக்கு_தள்ளு(&list, commit);
	}
	எனில் (!list.nr)
		/* nothing to do */
		செல் done;
	total = list.nr;

	எனில் (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		எனில் (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} இல்லையெனில் எனில் (cover_letter == -1) {
		cover_letter = 1;
	}

	எனில் (cover_letter == -1) {
		எனில் (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		இல்லையெனில் எனில் ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		இல்லையெனில்
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	எனில் (!cfg.keep_subject && cfg.auto_எண் && (total > 1 || cover_letter))
		cfg.numbered = 1;
	எனில் (cfg.numbered)
		rev.total = total + start_எண் - 1;

	எனில் (idiff_prev.nr) {
		எனில் (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_மரம்_oid(list.பொருட்கள்[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_மொத்தம்,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	எனில் (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	இல்லையெனில் எனில் (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	எனில் (rdiff_prev) {
		எனில் (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.பொருட்கள்[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_மொத்தம்,
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
	எனில் (!signature) {
		; /* --no-signature inhibits all signatures */
	} இல்லையெனில் எனில் (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} இல்லையெனில் எனில் (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		மாறிலி எழுத்து *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		எனில் (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, வெற்று);
	} இல்லையெனில் எனில் (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, அளவு(bases));
	base = get_base_commit(&cfg, list.பொருட்கள், list.nr);
	எனில் (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.பொருட்கள், list.nr);
	}

	எனில் (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(அளவு(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	எனில் (in_reply_to) {
		எழுத்து *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	எனில் (cover_letter) {
		எனில் (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.பொருட்கள்,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_எண்--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = வெற்று;
		rev.rdiff1 = வெற்று;
	}
	rev.add_signoff = cfg.do_signoff;

	எனில் (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	போது (list.nr) {
		size_t idx = list.nr - 1;
		முழு shown;

		display_progress(progress, total - idx);
		commit = commit_அடுக்கு_எடு(&list);
		rev.nr = total - idx + (start_எண் - 1);

		/* Make the second and subsequent mails replies to the first */
		எனில் (cfg.thread) {
			/* Have we already had a message ID? */
			எனில் (rev.message_id) {
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
				எனில் (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					விடுவி(rev.message_id);
				இல்லையெனில்
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		எனில் (output_directory &&
		    open_next_file(rev.numbered_files ? வெற்று : commit, வெற்று, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_மரம்_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		எனில் (output_directory)
			rev.shown_one = 0;
		எனில் (shown) {
			print_bases(&bases, rev.diffopt.file);
			எனில் (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			இல்லையெனில்
				print_signature(signature, rev.diffopt.file);
		}
		எனில் (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = வெற்று;
		}
	}
	stop_progress(&progress);
	commit_அடுக்கு_clear(&list);
	எனில் (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	விடுவி(description_file);
	விடுவி(signature_file_arg);
	விடுவி(signature_to_free);
	விடுவி(branch_name);
	விடுவி(to_free);
	விடுவி(rev.message_id);
	எனில் (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	விடுவி(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	திரும்பு 0;
}

நிலையான முழு add_pending_commit(மாறிலி எழுத்து *arg, கட்டமைப்பு rev_info *revs, முழு flags)
{
	கட்டமைப்பு object_id oid;
	எனில் (repo_get_oid(the_repository, arg, &oid) == 0) {
		கட்டமைப்பு commit *commit = lookup_commit_reference(the_repository,
								&oid);
		எனில் (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			திரும்பு 0;
		}
	}
	திரும்பு -1;
}

நிலையான மாறிலி எழுத்து * மாறிலி cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	வெற்று
};

நிலையான வெறுமை print_commit(எழுத்து sign, கட்டமைப்பு commit *commit, முழு verbose,
			 முழு abbrev, FILE *file)
{
	எனில் (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} இல்லையெனில் {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
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
நிலையான வெறுமை collect_diff_blob_oids(கட்டமைப்பு commit *commit,
				   கட்டமைப்பு diff_options *opts,
				   கட்டமைப்பு oidset *blobs)
{
	கட்டமைப்பு diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	ப்ரதிஜ்ஞா(!commit->parents || !commit->parents->next);

	எனில் (commit->parents)
		diff_மரம்_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	இல்லையெனில்
		diff_வேர்_மரம்_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	க்கு (முழு i = 0; i < q->nr; i++) {
		கட்டமைப்பு diff_filepair *p = q->queue[i];
		கட்டமைப்பு userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		எனில் (drv && drv->இருமம் > 0)
			தொடர்;

		எனில் (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, வெற்று,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_செருகு(blobs, &p->one->oid);
		எனில் (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, வெற்று,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_செருகு(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

நிலையான முழு always_match(மாறிலி வெறுமை *cmp_data UNUSED,
			மாறிலி கட்டமைப்பு hashmap_entry *entry1 UNUSED,
			மாறிலி கட்டமைப்பு hashmap_entry *entry2 UNUSED,
			மாறிலி வெறுமை *keydata UNUSED)
{
	திரும்பு 0;
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
நிலையான வெறுமை prefetch_cherry_blobs(கட்டமைப்பு repository *repo,
				  கட்டமைப்பு commit_list *list,
				  கட்டமைப்பு patch_ids *ids)
{
	கட்டமைப்பு oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	எனில் (!repo_has_promisor_remote(repo))
		திரும்பு;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	க்கு (கட்டமைப்பு commit_list *l = list; l; l = l->next) {
		கட்டமைப்பு commit *c = l->item;
		bool match_found = false;
		க்கு (கட்டமைப்பு patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		எனில் (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	எனில் (oidset_size(&blobs)) {
		கட்டமைப்பு oid_array to_fetch = OID_ARRAY_INIT;
		கட்டமைப்பு oidset_iter iter;
		மாறிலி கட்டமைப்பு object_id *oid;

		oidset_iter_init(&blobs, &iter);
		போது ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

முழு cmd_cherry(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு patch_ids ids;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு commit_list *list = வெற்று;
	கட்டமைப்பு branch *current_branch;
	மாறிலி எழுத்து *upstream;
	மாறிலி எழுத்து *head = "HEAD";
	மாறிலி எழுத்து *limit = வெற்று;
	முழு verbose = 0, abbrev = 0;

	கட்டமைப்பு option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	தேர்வு (argc) {
	நிலை 3:
		limit = argv[2];
		/* FALLTHROUGH */
	நிலை 2:
		head = argv[1];
		/* FALLTHROUGH */
	நிலை 1:
		upstream = argv[0];
		நிறுத்து;
	இயல்பு:
		current_branch = branch_get(வெற்று);
		upstream = branch_get_upstream(current_branch, வெற்று);
		எனில் (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	எனில் (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	எனில் (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	எனில் (revs.pending.nr == 2) {
		கட்டமைப்பு object_array_entry *o = revs.pending.objects;
		எனில் (oideq(&o[0].item->oid, &o[1].item->oid))
			திரும்பு 0;
	}

	get_patch_ids(&revs, &ids);

	எனில் (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	எனில் (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	போது ((commit = get_revision(&revs)) != வெற்று) {
		commit_list_செருகு(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	க்கு (கட்டமைப்பு commit_list *l = list; l; l = l->next) {
		எழுத்து sign = '+';

		commit = l->item;
		எனில் (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	திரும்பு 0;
}
