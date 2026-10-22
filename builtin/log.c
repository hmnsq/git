/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "pager.h"
#شامل "color.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "diff-merges.h"
#شامل "revision.h"
#شامل "log-tree.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "tag.h"
#شامل "reflog-walk.h"
#شامل "patch-ids.h"
#شامل "path.h"
#شامل "shortlog.h"
#شامل "remote.h"
#شامل "string-list.h"
#شامل "parse-options.h"
#شامل "line-log.h"
#شامل "branch.h"
#شامل "version.h"
#شامل "mailmap.h"
#شامل "progress.h"
#شامل "commit-slab.h"
#شامل "advice.h"
#شامل "utf8.h"

#شامل "commit-reach.h"
#شامل "promisor-remote.h"
#شامل "range-diff.h"
#شامل "tmp-objdir.h"
#شامل "tree.h"
#شامل "userdiff.h"
#شامل "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

ایستا unsigned صحیح force_in_body_from;
ایستا صحیح stdout_mboxrd;
ایستا صحیح format_no_prefix;

ایستا ثابت نویسه * ثابت builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	پوچ
};

ساختار سطر_opt_callback_data {
	ساختار rev_info *rev;
	ثابت نویسه *prefix;
	ساختار string_list args;
};

ایستا صحیح session_is_interactive(تهی)
{
	بازگشت isatty(1) || pager_in_use();
}

ایستا صحیح auto_decoration_style(تهی)
{
	بازگشت session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

ایستا صحیح parse_decoration_style(ثابت نویسه *مقدار)
{
	انتخاب (git_parse_maybe_bool(مقدار)) {
	حالت 1:
		بازگشت DECORATE_SHORT_REFS;
	حالت 0:
		بازگشت 0;
	پیشفرض:
		بشکن;
	}
	اگر (!strcmp(مقدار, "full"))
		بازگشت DECORATE_FULL_REFS;
	وگرنه اگر (!strcmp(مقدار, "short"))
		بازگشت DECORATE_SHORT_REFS;
	وگرنه اگر (!strcmp(مقدار, "auto"))
		بازگشت auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	بازگشت -1;
}

ساختار log_config {
	صحیح default_abbrev_commit;
	صحیح default_show_ریشه;
	صحیح default_follow;
	صحیح default_show_signature;
	صحیح default_encode_email_headers;
	صحیح decoration_style;
	صحیح decoration_given;
	صحیح use_mailmap_config;
	نویسه *fmt_patch_subject_prefix;
	صحیح fmt_patch_name_max;
	نویسه *fmt_pretty;
	نویسه *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	صحیح i_still_use_this;
#endif
};

ایستا تهی log_config_init(ساختار log_config *cfg)
{
	memset(cfg, 0, اندازه(*cfg));
	cfg->default_show_ریشه = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

ایستا تهی log_config_release(ساختار log_config *cfg)
{
	آزاد(cfg->default_date_mode);
	آزاد(cfg->fmt_pretty);
	آزاد(cfg->fmt_patch_subject_prefix);
}

ایستا صحیح use_default_decoration_filter = 1;
ایستا ساختار string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
ایستا ساختار string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
ایستا ساختار string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

ایستا صحیح clear_decorations_callback(ثابت ساختار option *opt UNUSED,
				      ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	بازگشت 0;
}

ایستا صحیح decorate_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			     صحیح unset)
{
	ساختار log_config *cfg = opt->مقدار;

	اگر (unset)
		cfg->decoration_style = 0;
	وگرنه اگر (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	وگرنه
		cfg->decoration_style = DECORATE_SHORT_REFS;

	اگر (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	بازگشت 0;
}

ایستا صحیح log_سطر_range_callback(ثابت ساختار option *option, ثابت نویسه *arg, صحیح unset)
{
	ساختار سطر_opt_callback_data *data = option->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!arg)
		بازگشت -1;

	data->rev->سطر_level_traverse = 1;
	string_list_append(&data->args, arg);

	بازگشت 0;
}

ایستا تهی cmd_log_init_defaults(ساختار rev_info *rev,
				  ساختار log_config *cfg)
{
	اگر (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	اگر (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_ریشه_diff = cfg->default_show_ریشه;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	اگر (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

ایستا تهی set_default_decoration_filter(ساختار decoration_filter *decoration_filter)
{
	نویسه *مقدار = پوچ;
	ساختار string_list *include = decoration_filter->include_ref_pattern;
	ثابت ساختار string_list *config_exclude;

	اگر (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		ساختار string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	اگر (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &مقدار) &&
	    !strcmp("all", مقدار))
		use_default_decoration_filter = 0;
	آزاد(مقدار);

	اگر (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		بازگشت;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	برای (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		اگر (!ref_namespace[i].decoration)
			ادامه;

		string_list_append(include, ref_namespace[i].ref);
	}
}

ایستا تهی cmd_log_init_finish(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار rev_info *rev, ساختار setup_revision_opt *opt,
			 ساختار log_config *cfg)
{
	ساختار userformat_want w;
	صحیح quiet = 0, source = 0, mailmap;
	ایستا ساختار سطر_opt_callback_data سطر_cb = {پوچ, پوچ, STRING_LIST_INIT_DUP};
	ساختار decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	ایستا ساختار revision_sources revision_sources;

	ثابت ساختار option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", پوچ, پوچ,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, پوچ, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', پوچ, &سطر_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_سطر_range_callback),
		OPT_END()
	};

	سطر_cb.rev = rev;
	سطر_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	اگر (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	اگر (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	اگر (rev->سطر_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, اندازه(w));
	userformat_find_requirements(پوچ, &w);

	اگر (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	اگر (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	اگر ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	اگر (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	اگر (mailmap) {
		rev->mailmap = xmalloc(اندازه(ساختار string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	اگر (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		اگر (!cfg->decoration_given)
			cfg->decoration_style = 0;
		اگر (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	اگر (rev->commit_format == CMIT_FMT_USERFORMAT) {
		اگر (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} وگرنه اگر (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	اگر (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		اگر (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	اگر (rev->سطر_level_traverse)
		سطر_log_init(rev, سطر_cb.prefix, &سطر_cb.args);

	setup_pager(the_repository);
}

ایستا تهی cmd_log_init(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار rev_info *rev, ساختار setup_revision_opt *opt,
			 ساختار log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

ایستا صحیح cmd_log_walk_no_free(ساختار rev_info *rev)
{
	ساختار commit *commit;
	صحیح saved_nrl = 0;
	صحیح saved_dcctc = 0;
	صحیح result;

	اگر (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	وقتی ((commit = get_revision(rev)) != پوچ) {
		اگر (!log_درخت_commit(rev, commit) && rev->max_شمار >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_شمار++;
		اگر (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = پوچ;
		}
		اگر (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		اگر (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	اگر (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	بازگشت result;
}

ایستا صحیح cmd_log_walk(ساختار rev_info *rev)
{
	صحیح retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	بازگشت retval;
}

ایستا صحیح git_log_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			  ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار log_config *cfg = cb;
	ثابت نویسه *slot_name;

	اگر (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		بازگشت git_config_string(&cfg->fmt_pretty, var, مقدار);
	}
	اگر (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		بازگشت git_config_string(&cfg->fmt_patch_subject_prefix, var, مقدار);
	}
	اگر (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, مقدار, ctx->kvi);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		بازگشت git_config_string(&cfg->default_date_mode, var, مقدار);
	}
	اگر (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(مقدار);
		اگر (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		بازگشت 0;
	}
	اگر (!strcmp(var, "log.diffmerges")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		بازگشت diff_merges_config(مقدار);
	}
	اگر (!strcmp(var, "log.showroot")) {
		cfg->default_show_ریشه = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (skip_prefix(var, "color.decorate.", &slot_name))
		بازگشت parse_decorate_color_config(var, slot_name, مقدار);
	اگر (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	بازگشت git_diff_ui_config(var, مقدار, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
صحیح cmd_whatchanged(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo UNUSED)
{
	ساختار log_config cfg;
	ساختار rev_info rev;
	ساختار setup_revision_opt opt;
	صحیح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, اندازه(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	اگر (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	اگر (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	بازگشت ret;
}
#endif

ایستا تهی show_tagger(ثابت نویسه *buf, ساختار rev_info *rev)
{
	ساختار strbuf out = STRBUF_INIT;
	ساختار pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

ایستا صحیح show_blob_object(ثابت ساختار object_id *oid, ساختار rev_info *rev, ثابت نویسه *obj_name)
{
	ساختار object_id oidc;
	ساختار object_context obj_context = {0};
	نویسه *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	اگر (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		بازگشت odb_stream_blob_to_fd(the_repository->objects, 1, oid, پوچ, 0);

	اگر (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	اگر (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		بازگشت odb_stream_blob_to_fd(the_repository->objects, 1, oid, پوچ, 0);
	}

	اگر (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	آزاد(buf);
	بازگشت 0;
}

ایستا صحیح show_tag_object(ثابت ساختار object_id *oid, ساختار rev_info *rev)
{
	size_t size;
	شمارش object_type type;
	نویسه *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	اگر (!buf)
		بازگشت error(_("could not read object %s"), oid_to_hex(oid));

	أكد(type == OBJ_TAG);
	وقتی (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		ثابت نویسه *ident;
		وقتی (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		اگر (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	اگر (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	آزاد(buf);
	بازگشت 0;
}

ایستا صحیح show_درخت_object(ثابت ساختار object_id *oid UNUSED,
			    ساختار strbuf *base UNUSED,
			    ثابت نویسه *pathname, unsigned mode,
			    تهی *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	بازگشت 0;
}

ایستا تهی show_setup_revisions_tweak(ساختار rev_info *rev)
{
	اگر (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	وگرنه
		diff_merges_default_to_dense_combined(rev);
	اگر (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

صحیح cmd_show(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repo UNUSED)
{
	ساختار log_config cfg;
	ساختار rev_info rev;
	unsigned صحیح i;
	ساختار setup_revision_opt opt;
	ساختار pathspec match_all;
	صحیح ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	اگر (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, اندازه(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, اندازه(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	اگر (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		بازگشت ret;
	}

	rev.diffopt.no_free = 1;
	برای (i = 0; i < rev.pending.nr && !ret; i++) {
		ساختار object *o = rev.pending.objects[i].item;
		ثابت نویسه *name = rev.pending.objects[i].name;
		انتخاب (o->type) {
		حالت OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			بشکن;
		حالت OBJ_TAG: {
			ساختار tag *t = (ساختار tag *)o;
			ساختار object_id *oid = get_tagged_oid(t);

			اگر (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			اگر (ret)
				بشکن;
			o = parse_object(the_repository, oid);
			اگر (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			بشکن;
		}
		حالت OBJ_درخت:
			اگر (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_درخت(the_repository, (ساختار درخت *)o,
				  &match_all, show_درخت_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			بشکن;
		حالت OBJ_COMMIT:
		{
			ساختار object_array old;
			ساختار object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, اندازه(old));
			memcpy(&rev.pending, &blank, اندازه(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, اندازه(rev.pending));
			بشکن;
		}
		پیشفرض:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	بازگشت ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
صحیح cmd_log_reflog(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ساختار log_config cfg;
	ساختار rev_info rev;
	ساختار setup_revision_opt opt;
	صحیح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, اندازه(opt));
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
	بازگشت ret;
}

ایستا تهی log_setup_revisions_tweak(ساختار rev_info *rev)
{
	اگر (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	اگر (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

صحیح cmd_log(صحیح argc,
	    ثابت نویسه **argv,
	    ثابت نویسه *prefix,
	    ساختار repository *repo UNUSED)
{
	ساختار log_config cfg;
	ساختار rev_info rev;
	ساختار setup_revision_opt opt;
	صحیح ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, اندازه(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	بازگشت ret;
}

/* format-patch */

شمارش cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

شمارش thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

شمارش cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

شمارش auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

ساختار format_config {
	ساختار log_config log;
	شمارش thread_level thread;
	صحیح do_signoff;
	شمارش auto_base_setting auto_base;
	نویسه *base_commit;
	نویسه *from;
	نویسه *signature;
	نویسه *signature_file;
	شمارش cover_setting config_cover_letter;
	نویسه *fmt_cover_letter_commit_list;
	نویسه *config_output_directory;
	شمارش cover_from_description cover_from_description_mode;
	صحیح show_notes;
	ساختار display_notes_opt notes_opt;
	صحیح numbered_cmdline_opt;
	صحیح numbered;
	صحیح auto_عدد;
	نویسه *default_attach;
	ساختار string_list extra_hdr;
	ساختار string_list extra_to;
	ساختار string_list extra_cc;
	صحیح keep_subject;
	صحیح subject_prefix;
	ساختار strbuf sprefix;
	نویسه *fmt_patch_suffix;
};

ایستا تهی format_config_init(ساختار format_config *cfg)
{
	memset(cfg, 0, اندازه(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_عدد = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

ایستا تهی format_config_release(ساختار format_config *cfg)
{
	log_config_release(&cfg->log);
	آزاد(cfg->base_commit);
	آزاد(cfg->from);
	آزاد(cfg->signature);
	آزاد(cfg->signature_file);
	آزاد(cfg->config_output_directory);
	آزاد(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	آزاد(cfg->fmt_patch_suffix);
	آزاد(cfg->fmt_cover_letter_commit_list);
}

ایستا شمارش cover_from_description parse_cover_from_description(ثابت نویسه *arg)
{
	اگر (!arg || !strcmp(arg, "default"))
		بازگشت COVER_FROM_MESSAGE;
	وگرنه اگر (!strcmp(arg, "none"))
		بازگشت COVER_FROM_NONE;
	وگرنه اگر (!strcmp(arg, "message"))
		بازگشت COVER_FROM_MESSAGE;
	وگرنه اگر (!strcmp(arg, "subject"))
		بازگشت COVER_FROM_SUBJECT;
	وگرنه اگر (!strcmp(arg, "auto"))
		بازگشت COVER_FROM_AUTO;
	وگرنه
		die(_("%s: invalid cover from description mode"), arg);
}

ایستا تهی add_header(ساختار format_config *cfg, ثابت نویسه *مقدار)
{
	ساختار string_list_item *item;
	صحیح len = strlen(مقدار);
	وقتی (len && مقدار[len - 1] == '\n')
		len--;

	اگر (!strncasecmp(مقدار, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, مقدار + 4);
		len -= 4;
	} وگرنه اگر (!strncasecmp(مقدار, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, مقدار + 4);
		len -= 4;
	} وگرنه {
		item = string_list_append(&cfg->extra_hdr, مقدار);
	}

	item->string[len] = '\0';
}

ایستا صحیح git_format_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			     ثابت ساختار config_context *ctx, تهی *cb)
{
	ساختار format_config *cfg = cb;

	اگر (!strcmp(var, "format.headers")) {
		اگر (!مقدار)
			die(_("format.headers without value"));
		add_header(cfg, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		بازگشت git_config_string(&cfg->fmt_patch_suffix, var, مقدار);
	}
	اگر (!strcmp(var, "format.to")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		string_list_append(&cfg->extra_to, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.cc")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.numbered")) {
		اگر (مقدار && !strcasecmp(مقدار, "auto")) {
			cfg->auto_عدد = 1;
			بازگشت 0;
		}
		cfg->numbered = git_config_bool(var, مقدار);
		cfg->auto_عدد = cfg->auto_عدد && cfg->numbered;
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.attach")) {
		اگر (مقدار && *مقدار) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(مقدار);
		} وگرنه اگر (مقدار && !*مقدار) {
			FREE_AND_NULL(cfg->default_attach);
		} وگرنه {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.thread")) {
		اگر (مقدار && !strcasecmp(مقدار, "deep")) {
			cfg->thread = THREAD_DEEP;
			بازگشت 0;
		}
		اگر (مقدار && !strcasecmp(مقدار, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			بازگشت 0;
		}
		cfg->thread = git_config_bool(var, مقدار) ? THREAD_SHALLOW : THREAD_UNSET;
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		بازگشت git_config_string(&cfg->signature, var, مقدار);
	}
	اگر (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		بازگشت git_config_pathname(&cfg->signature_file, var, مقدار);
	}
	اگر (!strcmp(var, "format.coverletter")) {
		اگر (مقدار && !strcasecmp(مقدار, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			بازگشت 0;
		}
		cfg->config_cover_letter = git_config_bool(var, مقدار) ? COVER_ON : COVER_OFF;
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		بازگشت git_config_string(&cfg->fmt_cover_letter_commit_list, var, مقدار);
	}
	اگر (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		بازگشت git_config_string(&cfg->config_output_directory, var, مقدار);
	}
	اگر (!strcmp(var, "format.useautobase")) {
		اگر (مقدار && !strcasecmp(مقدار, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			بازگشت 0;
		}
		cfg->auto_base = git_config_bool(var, مقدار) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.from")) {
		صحیح b = git_parse_maybe_bool(مقدار);
		FREE_AND_NULL(cfg->from);
		اگر (b < 0)
			cfg->from = xstrdup(مقدار);
		وگرنه اگر (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.notes")) {
		صحیح b = git_parse_maybe_bool(مقدار);
		اگر (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, مقدار);
		وگرنه اگر (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		وگرنه
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(مقدار);
		اگر (format_no_prefix < 0) {
			صحیح status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				مقدار, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		بازگشت 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	اگر (!strcmp(var, "diff.noprefix"))
		بازگشت 0;

	بازگشت git_log_config(var, مقدار, ctx, &cfg->log);
}

ایستا ثابت نویسه *output_directory = پوچ;
ایستا صحیح outdir_offset;

ایستا صحیح open_next_file(ساختار commit *commit, ثابت نویسه *subject,
			 ساختار rev_info *rev, صحیح quiet)
{
	ساختار strbuf filename = STRBUF_INIT;

	اگر (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	اگر (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	وگرنه اگر (commit)
		fmt_output_commit(&filename, commit, rev);
	وگرنه
		fmt_output_subject(&filename, subject, rev);

	اگر (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	اگر (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		بازگشت -1;
	}

	strbuf_release(&filename);
	بازگشت 0;
}

ایستا تهی get_patch_ids(ساختار rev_info *rev, ساختار patch_ids *ids)
{
	ساختار rev_info check_rev;
	ساختار commit *commit, *c1, *c2;
	ساختار object *o1, *o2;
	unsigned flags1, flags2;

	اگر (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	اگر ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	اگر (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	وقتی ((commit = get_revision(&check_rev)) != پوچ) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

ایستا تهی gen_message_id(ساختار rev_info *info, ثابت نویسه *base)
{
	ساختار strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(پوچ),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, پوچ);
}

ایستا تهی print_signature(ثابت نویسه *signature, FILE *file)
{
	اگر (!signature || !*signature)
		بازگشت;

	fprintf(file, "-- \n%s", signature);
	اگر (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

ایستا نویسه *find_branch_name(ساختار rev_info *rev)
{
	ساختار object_id branch_oid;
	ثابت ساختار object_id *tip_oid;
	ثابت نویسه *ref, *v;
	نویسه *full_ref, *branch = پوچ;
	صحیح interesting_found = 0;
	size_t idx;

	برای (size_t i = 0; i < rev->cmdline.nr; i++) {
		اگر (rev->cmdline.rev[i].flags & UNINTERESTING)
			ادامه;
		اگر (interesting_found)
			بازگشت پوچ;
		interesting_found = 1;
		idx = i;
	}
	اگر (!interesting_found)
		بازگشت پوچ;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	اگر (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	آزاد(full_ref);
	بازگشت branch;
}

ایستا تهی show_diffstat(ساختار rev_info *rev,
			  ساختار commit *origin, ساختار commit *head)
{
	ساختار diff_options opts;

	memcpy(&opts, &rev->diffopt, اندازه(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_درخت_oid(get_commit_درخت_oid(origin),
		      get_commit_درخت_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

ایستا تهی read_desc_file(ساختار strbuf *buf, ثابت نویسه *desc_file)
{
	اگر (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

ایستا تهی prepare_cover_text(ساختار pretty_print_context *pp,
			       ثابت نویسه *description_file,
			       ثابت نویسه *branch_name,
			       ساختار strbuf *sb,
			       ثابت نویسه *encoding,
			       صحیح need_8bit_cte,
			       ثابت ساختار format_config *cfg)
{
	ثابت نویسه *subject = "*** SUBJECT HERE ***";
	ثابت نویسه *body = "*** BLURB HERE ***";
	ساختار strbuf description_sb = STRBUF_INIT;
	ساختار strbuf subject_sb = STRBUF_INIT;

	اگر (cfg->cover_from_description_mode == COVER_FROM_NONE)
		برو do_pp;

	اگر (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	وگرنه اگر (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	اگر (!description_sb.len)
		برو do_pp;

	اگر (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	اگر (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	وگرنه
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

ساختار rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	ساختار string_list notes;
};

ایستا صحیح rdiff_notes_cb(ثابت ساختار option *option,
			  ثابت نویسه *arg,
			  صحیح unset)
{
	ساختار option opt = *option;
	ساختار rdiff_notes *rdiff_notes = option->مقدار;

	rdiff_notes->override = 1;
	opt.مقدار = &rdiff_notes->notes;
	بازگشت parse_opt_string_list(&opt, arg, unset);
}

ایستا صحیح get_notes_refs(ساختار string_list_item *item, تهی *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	بازگشت 0;
}

ایستا تهی get_notes_args(ساختار rdiff_notes *rdiff_notes,
			   ساختار rev_info *rev)
{
	اگر (rdiff_notes->override) {
		اگر (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		وگرنه
			strvec_افزودن(&rev->rdiff_log_arg, "--no-notes");
	} وگرنه اگر (!rev->show_notes) {
		strvec_افزودن(&rev->rdiff_log_arg, "--no-notes");
	} وگرنه اگر (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_افزودن(&rev->rdiff_log_arg, "--notes");
	} وگرنه {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

ایستا تهی generate_shortlog_cover_letter(ساختار shortlog *log,
					   ساختار rev_info *rev,
					   ساختار commit **list,
					   صحیح nr)
{
	shortlog_init(log);
	log->wrap_سطرها = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	برای (صحیح i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

ایستا تهی generate_commit_list_cover(FILE *cover_file, ثابت نویسه *format,
				       ساختار commit **list, صحیح n)
{
	ساختار strbuf commit_سطر = STRBUF_INIT;
	ساختار pretty_print_context ctx = {0};
	ساختار rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	برای (صحیح i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_سطر, &ctx);
		fprintf(cover_file, "%s\n", commit_سطر.buf);
		strbuf_reset(&commit_سطر);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_سطر);
}

ایستا تهی make_cover_letter(ساختار rev_info *rev, صحیح use_separate_file,
			      ساختار commit *origin,
			      صحیح nr, ساختار commit **list,
			      ثابت نویسه *description_file,
			      ثابت نویسه *branch_name,
			      صحیح quiet,
			      ثابت ساختار format_config *cfg,
			      ثابت نویسه *format)
{
	ثابت نویسه *from;
	ساختار shortlog log;
	ساختار strbuf sb = STRBUF_INIT;
	صحیح i;
	ثابت نویسه *encoding = "UTF-8";
	صحیح need_8bit_cte = 0;
	ساختار pretty_print_context pp = {0};
	ساختار commit *head = list[0];
	نویسه *to_free = پوچ;

	اگر (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	اگر (use_separate_file &&
	    open_next_file(پوچ, rev->numbered_files ? پوچ : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	برای (i = 0; !need_8bit_cte && i < nr; i++) {
		ثابت نویسه *buf = repo_get_commit_buffer(the_repository,
							 list[i], پوچ);
		اگر (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	اگر (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, پوچ, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	آزاد(to_free);
	آزاد(pp.after_subject);
	strbuf_release(&sb);

	اگر (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	وگرنه اگر (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	وگرنه اگر (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	وگرنه اگر (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	وگرنه
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	اگر (origin)
		show_diffstat(rev, origin, head);

	اگر (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	اگر (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		ساختار diff_options opts;
		ساختار range_diff_options range_diff_opts = {
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

ایستا نویسه *clean_message_id(ثابت نویسه *پیام_id)
{
	نویسه ch;
	ثابت نویسه *a, *z, *m;

	m = پیام_id;
	وقتی ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = پوچ;
	وقتی ((ch = *m)) {
		اگر (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	اگر (!z)
		die(_("insane in-reply-to: %s"), پیام_id);
	اگر (++z == m)
		بازگشت xstrdup(a);
	بازگشت xmemdupz(a, z - a);
}

ایستا ثابت نویسه *set_outdir(ثابت نویسه *prefix, ثابت نویسه *output_directory)
{
	اگر (output_directory && is_absolute_path(output_directory))
		بازگشت output_directory;

	اگر (!prefix || !*prefix) {
		اگر (output_directory)
			بازگشت output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		بازگشت "./";
	}

	outdir_offset = strlen(prefix);
	اگر (!output_directory)
		بازگشت prefix;

	بازگشت prefix_filename(prefix, output_directory);
}

ایستا ثابت نویسه * ثابت builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	پوچ
};

ساختار keep_callback_data {
	ساختار format_config *cfg;
	ساختار rev_info *revs;
};

ایستا صحیح keep_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار keep_callback_data *data = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	بازگشت 0;
}

ایستا صحیح subject_prefix_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			    صحیح unset)
{
	ساختار format_config *cfg = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	بازگشت 0;
}

ایستا صحیح rfc_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			صحیح unset)
{
	ثابت نویسه **rfc = opt->مقدار;

	*rfc = opt->مقدار;
	اگر (unset)
		*rfc = پوچ;
	وگرنه
		*rfc = arg ? arg : "RFC";
	بازگشت 0;
}

ایستا صحیح numbered_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			     صحیح unset)
{
	ساختار format_config *cfg = opt->مقدار;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	اگر (unset)
		cfg->auto_عدد =  0;
	بازگشت 0;
}

ایستا صحیح no_numbered_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
				صحیح unset)
{
	BUG_ON_OPT_NEG(unset);
	بازگشت numbered_callback(opt, arg, 1);
}

ایستا صحیح output_directory_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			      صحیح unset)
{
	ثابت نویسه **dir = (ثابت نویسه **)opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	اگر (*dir)
		die(_("two output directories?"));
	*dir = arg;
	بازگشت 0;
}

ایستا صحیح thread_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار format_config *cfg = opt->مقدار;

	اگر (unset)
		cfg->thread = THREAD_UNSET;
	وگرنه اگر (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	وگرنه اگر (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	وگرنه
		بازگشت 1;
	بازگشت 0;
}

ایستا صحیح attach_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار rev_info *rev = (ساختار rev_info *)opt->مقدار;
	اگر (unset)
		rev->mime_boundary = پوچ;
	وگرنه اگر (arg)
		rev->mime_boundary = arg;
	وگرنه
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	بازگشت 0;
}

ایستا صحیح inline_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار rev_info *rev = (ساختار rev_info *)opt->مقدار;
	اگر (unset)
		rev->mime_boundary = پوچ;
	وگرنه اگر (arg)
		rev->mime_boundary = arg;
	وگرنه
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	بازگشت 0;
}

ایستا صحیح header_callback(ثابت ساختار option *opt, ثابت نویسه *arg,
			   صحیح unset)
{
	ساختار format_config *cfg = opt->مقدار;

	اگر (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} وگرنه {
		add_header(cfg, arg);
	}
	بازگشت 0;
}

ایستا صحیح from_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	نویسه **from = opt->مقدار;

	آزاد(*from);

	اگر (unset)
		*from = پوچ;
	وگرنه اگر (arg)
		*from = xstrdup(arg);
	وگرنه
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	بازگشت 0;
}

ایستا صحیح base_callback(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار format_config *cfg = opt->مقدار;

	اگر (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} وگرنه اگر (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} وگرنه {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	بازگشت 0;
}

ساختار base_درخت_info {
	ساختار object_id base_commit;
	صحیح nr_patch_id, alloc_patch_id;
	ساختار object_id *patch_id;
};

ایستا ساختار commit *get_base_commit(ثابت ساختار format_config *cfg,
				      ساختار commit **list,
				      size_t total)
{
	ساختار commit *base = پوچ;
	ساختار commit **rev;
	صحیح auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	انتخاب (cfg->auto_base) {
	حالت AUTO_BASE_NEVER:
		اگر (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} وگرنه {
			/* no base information is requested */
			بازگشت پوچ;
		}
		بشکن;
	حالت AUTO_BASE_ALWAYS:
	حالت AUTO_BASE_WHEN_ABLE:
		اگر (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} وگرنه {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		بشکن;
	پیشفرض:
		BUG("unexpected automatic base selection method");
	}

	اگر (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		اگر (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} وگرنه {
		ساختار branch *curr_branch = branch_get(پوچ);
		ثابت نویسه *upstream = branch_get_upstream(curr_branch, پوچ);
		اگر (upstream) {
			ساختار commit_list *base_list = پوچ;
			ساختار commit *commit;
			ساختار object_id oid;

			اگر (repo_get_oid(the_repository, upstream, &oid)) {
				اگر (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				وگرنه
					بازگشت پوچ;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			اگر (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				اگر (die_on_failure) {
					die(_("could not find exact merge base"));
				} وگرنه {
					commit_list_free(base_list);
					بازگشت پوچ;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} وگرنه {
			اگر (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			وگرنه
				بازگشت پوچ;
		}
	}

	ALLOC_ARRAY(rev, total);
	برای (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	وقتی (rev_nr > 1) {
		برای (i = 0; i < rev_nr / 2; i++) {
			ساختار commit_list *merge_base = پوچ;
			اگر (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				اگر (die_on_failure) {
					die(_("failed to find exact merge base"));
				} وگرنه {
					commit_list_free(merge_base);
					آزاد(rev);
					بازگشت پوچ;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		اگر (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	اگر (ret < 0)
		exit(128);
	اگر (!ret) {
		اگر (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} وگرنه {
			آزاد(rev);
			بازگشت پوچ;
		}
	}

	برای (i = 0; i < total; i++) {
		اگر (base == list[i]) {
			اگر (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} وگرنه {
				آزاد(rev);
				بازگشت پوچ;
			}
		}
	}

	آزاد(rev);
	بازگشت base;
}

define_commit_slab(commit_base, صحیح);

ایستا تهی prepare_bases(ساختار base_درخت_info *bases,
			  ساختار commit *base,
			  ساختار commit **list,
			  size_t total)
{
	ساختار commit *commit;
	ساختار rev_info revs;
	ساختار diff_options diffopt;
	ساختار commit_base commit_base;

	اگر (!base)
		بازگشت;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, پوچ);
	revs.max_parents = 1;
	revs.topo_order = 1;
	برای (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	وقتی ((commit = get_revision(&revs)) != پوچ) {
		ساختار object_id oid;
		ساختار object_id *patch_id;
		اگر (*commit_base_at(&commit_base, commit))
			ادامه;
		اگر (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

ایستا تهی print_bases(ساختار base_درخت_info *bases, FILE *file)
{
	صحیح i;

	/* Only do this once, either for the cover or for the first one */
	اگر (is_null_oid(&bases->base_commit))
		بازگشت;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	برای (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	آزاد(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

ایستا ثابت نویسه *diff_title(ساختار strbuf *sb,
			      ثابت نویسه *reroll_شمار,
			      ثابت نویسه *generic,
			      ثابت نویسه *rerolled)
{
	صحیح v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	اگر (reroll_شمار && !strtol_i(reroll_شمار, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	وگرنه
		strbuf_addstr(sb, generic);
	بازگشت sb->buf;
}

ایستا تهی infer_range_diff_ranges(ساختار strbuf *r1,
				    ساختار strbuf *r2,
				    ثابت نویسه *prev,
				    ساختار commit *origin,
				    ساختار commit *head)
{
	ثابت نویسه *head_oid = oid_to_hex(&head->object.oid);
	صحیح prev_is_range = is_range_diff_range(prev);

	اگر (prev_is_range)
		strbuf_addstr(r1, prev);
	وگرنه
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	اگر (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	وگرنه اگر (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	وگرنه {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

صحیح cmd_format_patch(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	ساختار format_config cfg;
	ساختار commit *commit;
	ساختار commit_پشته list = COMMIT_پشته_INIT;
	ساختار rev_info rev;
	نویسه *to_free = پوچ;
	ساختار setup_revision_opt s_r_opt;
	size_t total, i;
	صحیح use_stdout = 0;
	صحیح start_عدد = -1;
	صحیح just_numbers = 0;
	صحیح ignore_if_in_upstream = 0;
	صحیح cover_letter = -1;
	ثابت نویسه *cover_letter_fmt = پوچ;
	صحیح boundary_شمار = 0;
	صحیح no_دودویی_diff = 0;
	صحیح zero_commit = 0;
	ساختار commit *origin = پوچ;
	ثابت نویسه *in_reply_to = پوچ;
	ساختار patch_ids ids;
	ساختار strbuf buf = STRBUF_INIT;
	صحیح use_patch_format = 0;
	صحیح quiet = 0;
	ثابت نویسه *reroll_شمار = پوچ;
	نویسه *cover_from_description_arg = پوچ;
	نویسه *description_file = پوچ;
	نویسه *branch_name = پوچ;
	ساختار base_درخت_info bases;
	ساختار commit *base;
	صحیح show_progress = 0;
	ساختار progress *progress = پوچ;
	ساختار oid_array idiff_prev = OID_ARRAY_INIT;
	ساختار strbuf idiff_title = STRBUF_INIT;
	ثابت نویسه *rdiff_prev = پوچ;
	ساختار strbuf rdiff1 = STRBUF_INIT;
	ساختار strbuf rdiff2 = STRBUF_INIT;
	ساختار strbuf rdiff_title = STRBUF_INIT;
	ساختار rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	ثابت نویسه *rfc = پوچ;
	صحیح creation_factor = -1;
	ثابت نویسه *signature = git_version_string;
	نویسه *signature_to_free = پوچ;
	نویسه *signature_file_arg = پوچ;
	ساختار keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	ثابت نویسه *fmt_patch_suffix = پوچ;

	ثابت ساختار option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, پوچ,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, پوچ,
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
		OPT_INTEGER(0, "start-number", &start_عدد,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_شمار, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, پوچ,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_دودویی_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, اندازه(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, اندازه(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	اگر (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	اگر (cfg.default_attach) {
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

	اگر (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	اگر (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	اگر (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	اگر (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		اگر (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		وگرنه
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	اگر (reroll_شمار) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_شمار);
		rev.reroll_شمار = reroll_شمار;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	برای (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.اقلام[i].string);
		strbuf_addch(&buf, '\n');
	}

	اگر (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	برای (i = 0; i < cfg.extra_to.nr; i++) {
		اگر (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.اقلام[i].string);
		اگر (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	اگر (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	برای (i = 0; i < cfg.extra_cc.nr; i++) {
		اگر (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.اقلام[i].string);
		اگر (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, پوچ);

	اگر (cfg.from) {
		اگر (split_ident_سطر(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	اگر (start_عدد < 0)
		start_عدد = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	اگر (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	اگر (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	اگر (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	اگر (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	اگر (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	اگر (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	اگر (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	اگر (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	اگر (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	اگر (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	اگر (!rev.diffopt.flags.text && !no_دودویی_diff)
		rev.diffopt.flags.دودویی = 1;

	اگر (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	اگر (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	اگر (use_stdout) {
		setup_pager(the_repository);
	} وگرنه اگر (!rev.diffopt.close_file) {
		صحیح saved;

		اگر (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		اگر (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		انتخاب (safe_create_leading_directories_const(the_repository, output_directory)) {
		حالت SCLD_OK:
		حالت SCLD_EXISTS:
			بشکن;
		پیشفرض:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		اگر (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	اگر (rev.pending.nr == 1) {
		صحیح check_head = 0;

		اگر (rev.max_شمار < 0 && !rev.show_ریشه_diff) {
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

		اگر (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		اگر (check_head) {
			ثابت نویسه *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      پوچ, پوچ);
			اگر (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			وگرنه
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_ریشه_diff = 1;

	اگر (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		اگر (rev.pending.nr == 2) {
			ساختار object_array_entry *o = rev.pending.objects;
			اگر (oideq(&o[0].item->oid, &o[1].item->oid))
				برو done;
		}
		get_patch_ids(&rev, &ids);
	}

	اگر (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	وقتی ((commit = get_revision(&rev)) != پوچ) {
		اگر (commit->object.flags & BOUNDARY) {
			boundary_شمار++;
			origin = (boundary_شمار == 1) ? commit : پوچ;
			ادامه;
		}

		اگر (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			ادامه;

		commit_پشته_افزودن(&list, commit);
	}
	اگر (!list.nr)
		/* nothing to do */
		برو done;
	total = list.nr;

	اگر (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		اگر (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} وگرنه اگر (cover_letter == -1) {
		cover_letter = 1;
	}

	اگر (cover_letter == -1) {
		اگر (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		وگرنه اگر ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		وگرنه
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	اگر (!cfg.keep_subject && cfg.auto_عدد && (total > 1 || cover_letter))
		cfg.numbered = 1;
	اگر (cfg.numbered)
		rev.total = total + start_عدد - 1;

	اگر (idiff_prev.nr) {
		اگر (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_درخت_oid(list.اقلام[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_شمار,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	اگر (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	وگرنه اگر (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	اگر (rdiff_prev) {
		اگر (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.اقلام[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_شمار,
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
	اگر (!signature) {
		; /* --no-signature inhibits all signatures */
	} وگرنه اگر (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} وگرنه اگر (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		ساختار strbuf buf = STRBUF_INIT;
		ثابت نویسه *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		اگر (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, پوچ);
	} وگرنه اگر (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, اندازه(bases));
	base = get_base_commit(&cfg, list.اقلام, list.nr);
	اگر (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.اقلام, list.nr);
	}

	اگر (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(اندازه(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	اگر (in_reply_to) {
		نویسه *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	اگر (cover_letter) {
		اگر (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.اقلام,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_عدد--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = پوچ;
		rev.rdiff1 = پوچ;
	}
	rev.add_signoff = cfg.do_signoff;

	اگر (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	وقتی (list.nr) {
		size_t idx = list.nr - 1;
		صحیح shown;

		display_progress(progress, total - idx);
		commit = commit_پشته_برداشت(&list);
		rev.nr = total - idx + (start_عدد - 1);

		/* Make the second and subsequent mails replies to the first */
		اگر (cfg.thread) {
			/* Have we already had a message ID? */
			اگر (rev.message_id) {
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
				اگر (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					آزاد(rev.message_id);
				وگرنه
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		اگر (output_directory &&
		    open_next_file(rev.numbered_files ? پوچ : commit, پوچ, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_درخت_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		اگر (output_directory)
			rev.shown_one = 0;
		اگر (shown) {
			print_bases(&bases, rev.diffopt.file);
			اگر (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			وگرنه
				print_signature(signature, rev.diffopt.file);
		}
		اگر (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = پوچ;
		}
	}
	stop_progress(&progress);
	commit_پشته_clear(&list);
	اگر (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	آزاد(description_file);
	آزاد(signature_file_arg);
	آزاد(signature_to_free);
	آزاد(branch_name);
	آزاد(to_free);
	آزاد(rev.message_id);
	اگر (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	آزاد(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	بازگشت 0;
}

ایستا صحیح add_pending_commit(ثابت نویسه *arg, ساختار rev_info *revs, صحیح flags)
{
	ساختار object_id oid;
	اگر (repo_get_oid(the_repository, arg, &oid) == 0) {
		ساختار commit *commit = lookup_commit_reference(the_repository,
								&oid);
		اگر (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			بازگشت 0;
		}
	}
	بازگشت -1;
}

ایستا ثابت نویسه * ثابت cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	پوچ
};

ایستا تهی print_commit(نویسه sign, ساختار commit *commit, صحیح verbose,
			 صحیح abbrev, FILE *file)
{
	اگر (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} وگرنه {
		ساختار strbuf buf = STRBUF_INIT;
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
ایستا تهی collect_diff_blob_oids(ساختار commit *commit,
				   ساختار diff_options *opts,
				   ساختار oidset *blobs)
{
	ساختار diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	أكد(!commit->parents || !commit->parents->next);

	اگر (commit->parents)
		diff_درخت_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	وگرنه
		diff_ریشه_درخت_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	برای (صحیح i = 0; i < q->nr; i++) {
		ساختار diff_filepair *p = q->queue[i];
		ساختار userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		اگر (drv && drv->دودویی > 0)
			ادامه;

		اگر (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, پوچ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_درج(blobs, &p->one->oid);
		اگر (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, پوچ,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_درج(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

ایستا صحیح always_match(ثابت تهی *cmp_data UNUSED,
			ثابت ساختار hashmap_entry *entry1 UNUSED,
			ثابت ساختار hashmap_entry *entry2 UNUSED,
			ثابت تهی *keydata UNUSED)
{
	بازگشت 0;
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
ایستا تهی prefetch_cherry_blobs(ساختار repository *repo,
				  ساختار commit_list *list,
				  ساختار patch_ids *ids)
{
	ساختار oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	اگر (!repo_has_promisor_remote(repo))
		بازگشت;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	برای (ساختار commit_list *l = list; l; l = l->next) {
		ساختار commit *c = l->item;
		bool match_found = false;
		برای (ساختار patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		اگر (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	اگر (oidset_size(&blobs)) {
		ساختار oid_array to_fetch = OID_ARRAY_INIT;
		ساختار oidset_iter iter;
		ثابت ساختار object_id *oid;

		oidset_iter_init(&blobs, &iter);
		وقتی ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

صحیح cmd_cherry(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	ساختار rev_info revs;
	ساختار patch_ids ids;
	ساختار commit *commit;
	ساختار commit_list *list = پوچ;
	ساختار branch *current_branch;
	ثابت نویسه *upstream;
	ثابت نویسه *head = "HEAD";
	ثابت نویسه *limit = پوچ;
	صحیح verbose = 0, abbrev = 0;

	ساختار option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	انتخاب (argc) {
	حالت 3:
		limit = argv[2];
		/* FALLTHROUGH */
	حالت 2:
		head = argv[1];
		/* FALLTHROUGH */
	حالت 1:
		upstream = argv[0];
		بشکن;
	پیشفرض:
		current_branch = branch_get(پوچ);
		upstream = branch_get_upstream(current_branch, پوچ);
		اگر (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	اگر (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	اگر (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	اگر (revs.pending.nr == 2) {
		ساختار object_array_entry *o = revs.pending.objects;
		اگر (oideq(&o[0].item->oid, &o[1].item->oid))
			بازگشت 0;
	}

	get_patch_ids(&revs, &ids);

	اگر (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	وقتی ((commit = get_revision(&revs)) != پوچ) {
		commit_list_درج(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	برای (ساختار commit_list *l = list; l; l = l->next) {
		نویسه sign = '+';

		commit = l->item;
		اگر (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	بازگشت 0;
}
