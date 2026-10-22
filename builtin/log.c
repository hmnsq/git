/*
 * Builtin "git log" and related commands (show, whatchanged)
 *
 * (C) Copyright 2006 Linus Torvalds
 *		 2006 Junio Hamano
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "abspath.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "refs.h"
#include "object-name.h"
#include "odb.h"
#include "odb/streaming.h"
#include "pager.h"
#include "color.h"
#include "commit.h"
#include "diff.h"
#include "diffcore.h"
#include "diff-merges.h"
#include "revision.h"
#include "log-tree.h"
#include "oid-array.h"
#include "oidset.h"
#include "tag.h"
#include "reflog-walk.h"
#include "patch-ids.h"
#include "path.h"
#include "shortlog.h"
#include "remote.h"
#include "string-list.h"
#include "parse-options.h"
#include "line-log.h"
#include "branch.h"
#include "version.h"
#include "mailmap.h"
#include "progress.h"
#include "commit-slab.h"
#include "advice.h"
#include "utf8.h"

#include "commit-reach.h"
#include "promisor-remote.h"
#include "range-diff.h"
#include "tmp-objdir.h"
#include "tree.h"
#include "userdiff.h"
#include "write-or-die.h"

#define MAIL_DEFAULT_WRAP 72
#define COVER_FROM_AUTO_MAX_SUBJECT_LEN 100
#define FORMAT_PATCH_NAME_MAX_DEFAULT 64

static unsigned puur_naa_nka force_in_body_from;
static puur_naa_nka stdout_mboxrd;
static puur_naa_nka format_no_prefix;

static const var_na * const builtin_log_usage[] = {
	N_("git log [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git show [<options>] <object>..."),
	NULL
};

sa_nracanaa line_opt_callback_data {
	sa_nracanaa rev_info *rev;
	const var_na *prefix;
	sa_nracanaa string_list args;
};

static puur_naa_nka session_is_interactive(void)
{
	ᱨᱩᱣᱟᱹᱲ isatty(1) || pager_in_use();
}

static puur_naa_nka auto_decoration_style(void)
{
	ᱨᱩᱣᱟᱹᱲ session_is_interactive() ? DECORATE_SHORT_REFS : 0;
}

static puur_naa_nka parse_decoration_style(const var_na *value)
{
	switch (git_parse_maybe_bool(value)) {
	pak_sa 1:
		ᱨᱩᱣᱟᱹᱲ DECORATE_SHORT_REFS;
	pak_sa 0:
		ᱨᱩᱣᱟᱹᱲ 0;
	default:
		bha_nga;
	}
	ᱡᱩᱫᱤ (!strcmp(value, "full"))
		ᱨᱩᱣᱟᱹᱲ DECORATE_FULL_REFS;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(value, "short"))
		ᱨᱩᱣᱟᱹᱲ DECORATE_SHORT_REFS;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(value, "auto"))
		ᱨᱩᱣᱟᱹᱲ auto_decoration_style();
	/*
	 * Please update _git_log() in git-completion.bash when you
	 * add new decoration styles.
	 */
	ᱨᱩᱣᱟᱹᱲ -1;
}

sa_nracanaa log_config {
	puur_naa_nka default_abbrev_commit;
	puur_naa_nka default_show_root;
	puur_naa_nka default_follow;
	puur_naa_nka default_show_signature;
	puur_naa_nka default_encode_email_headers;
	puur_naa_nka decoration_style;
	puur_naa_nka decoration_given;
	puur_naa_nka use_mailmap_config;
	var_na *fmt_patch_subject_prefix;
	puur_naa_nka fmt_patch_name_max;
	var_na *fmt_pretty;
	var_na *default_date_mode;

#ifndef WITH_BREAKING_CHANGES
	/*
	 * Note: git_log_config() does not touch this member and that
	 * is very deliberate.  This member is only to be used to
	 * resurrect whatchanged that is deprecated.
	 */
	puur_naa_nka i_still_use_this;
#endif
};

static void log_config_init(sa_nracanaa log_config *cfg)
{
	memset(cfg, 0, parimaa_na(*cfg));
	cfg->default_show_root = 1;
	cfg->default_encode_email_headers = 1;
	cfg->use_mailmap_config = 1;
	cfg->fmt_patch_subject_prefix = xstrdup("PATCH");
	cfg->fmt_patch_name_max = FORMAT_PATCH_NAME_MAX_DEFAULT;
	cfg->decoration_style = auto_decoration_style();
}

static void log_config_release(sa_nracanaa log_config *cfg)
{
	free(cfg->default_date_mode);
	free(cfg->fmt_pretty);
	free(cfg->fmt_patch_subject_prefix);
}

static puur_naa_nka use_default_decoration_filter = 1;
static sa_nracanaa string_list decorate_refs_exclude = STRING_LIST_INIT_NODUP;
static sa_nracanaa string_list decorate_refs_exclude_config = STRING_LIST_INIT_NODUP;
static sa_nracanaa string_list decorate_refs_include = STRING_LIST_INIT_NODUP;

static puur_naa_nka clear_decorations_callback(const sa_nracanaa option *opt UNUSED,
				      const var_na *arg, puur_naa_nka unset)
{
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	string_list_clear(&decorate_refs_include, 0);
	string_list_clear(&decorate_refs_exclude, 0);
	use_default_decoration_filter = 0;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka decorate_callback(const sa_nracanaa option *opt, const var_na *arg,
			     puur_naa_nka unset)
{
	sa_nracanaa log_config *cfg = opt->value;

	ᱡᱩᱫᱤ (unset)
		cfg->decoration_style = 0;
	ᱵᱟᱝ ᱡᱩᱫᱤ (arg)
		cfg->decoration_style = parse_decoration_style(arg);
	ᱵᱟᱝ
		cfg->decoration_style = DECORATE_SHORT_REFS;

	ᱡᱩᱫᱤ (cfg->decoration_style < 0)
		die(_("invalid --decorate option: %s"), arg);

	cfg->decoration_given = 1;

	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka log_line_range_callback(const sa_nracanaa option *option, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa line_opt_callback_data *data = option->value;

	BUG_ON_OPT_NEG(unset);

	ᱡᱩᱫᱤ (!arg)
		ᱨᱩᱣᱟᱹᱲ -1;

	data->rev->line_level_traverse = 1;
	string_list_append(&data->args, arg);

	ᱨᱩᱣᱟᱹᱲ 0;
}

static void cmd_log_init_defaults(sa_nracanaa rev_info *rev,
				  sa_nracanaa log_config *cfg)
{
	ᱡᱩᱫᱤ (cfg->fmt_pretty)
		get_commit_format(cfg->fmt_pretty, rev);
	ᱡᱩᱫᱤ (cfg->default_follow)
		rev->diffopt.flags.default_follow_renames = 1;
	rev->verbose_header = 1;
	init_diffstat_widths(&rev->diffopt);
	rev->diffopt.flags.recursive = 1;
	rev->diffopt.flags.allow_textconv = 1;
	rev->abbrev_commit = cfg->default_abbrev_commit;
	rev->show_root_diff = cfg->default_show_root;
	rev->subject_prefix = cfg->fmt_patch_subject_prefix;
	rev->patch_name_max = cfg->fmt_patch_name_max;
	rev->show_signature = cfg->default_show_signature;
	rev->encode_email_headers = cfg->default_encode_email_headers;

	ᱡᱩᱫᱤ (cfg->default_date_mode)
		parse_date_format(cfg->default_date_mode, &rev->date_mode);
}

static void set_default_decoration_filter(sa_nracanaa decoration_filter *decoration_filter)
{
	var_na *value = NULL;
	sa_nracanaa string_list *include = decoration_filter->include_ref_pattern;
	const sa_nracanaa string_list *config_exclude;

	ᱡᱩᱫᱤ (!repo_config_get_string_multi(the_repository, "log.excludeDecoration",
					 &config_exclude)) {
		sa_nracanaa string_list_item *item;
		for_each_string_list_item(item, config_exclude)
			string_list_append(decoration_filter->exclude_ref_config_pattern,
					   item->string);
	}

	/*
	 * By default, decorate_all is disabled. Enable it if
	 * log.initialDecorationSet=all. Don't ever disable it by config,
	 * since the command-line takes precedent.
	 */
	ᱡᱩᱫᱤ (use_default_decoration_filter &&
	    !repo_config_get_string(the_repository, "log.initialdecorationset", &value) &&
	    !strcmp("all", value))
		use_default_decoration_filter = 0;
	free(value);

	ᱡᱩᱫᱤ (!use_default_decoration_filter ||
	    decoration_filter->exclude_ref_pattern->nr ||
	    decoration_filter->include_ref_pattern->nr ||
	    decoration_filter->exclude_ref_config_pattern->nr)
		ᱨᱩᱣᱟᱹᱲ;

	/*
	 * No command-line or config options were given, so
	 * populate with sensible defaults.
	 */
	for (size_t i = 0; i < ARRAY_SIZE(ref_namespace); i++) {
		ᱡᱩᱫᱤ (!ref_namespace[i].decoration)
			continue;

		string_list_append(include, ref_namespace[i].ref);
	}
}

static void cmd_log_init_finish(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa rev_info *rev, sa_nracanaa setup_revision_opt *opt,
			 sa_nracanaa log_config *cfg)
{
	sa_nracanaa userformat_want w;
	puur_naa_nka quiet = 0, source = 0, mailmap;
	static sa_nracanaa line_opt_callback_data line_cb = {NULL, NULL, STRING_LIST_INIT_DUP};
	sa_nracanaa decoration_filter decoration_filter = {
		.exclude_ref_pattern = &decorate_refs_exclude,
		.include_ref_pattern = &decorate_refs_include,
		.exclude_ref_config_pattern = &decorate_refs_exclude_config,
	};
	static sa_nracanaa revision_sources revision_sources;

	const sa_nracanaa option builtin_log_options[] = {
		OPT__QUIET(&quiet, N_("suppress diff output")),
		OPT_BOOL(0, "source", &source, N_("show source")),
		OPT_BOOL(0, "use-mailmap", &mailmap, N_("use mail map file")),
#ifndef WITH_BREAKING_CHANGES
		OPT_HIDDEN_BOOL(0, "i-still-use-this", &cfg->i_still_use_this,
				"<use this deprecated command>"),
#endif
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		OPT_CALLBACK_F(0, "clear-decorations", NULL, NULL,
			       N_("clear all previously-defined decoration filters"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       clear_decorations_callback),
		OPT_STRING_LIST(0, "decorate-refs", &decorate_refs_include,
				N_("pattern"), N_("only decorate refs that match <pattern>")),
		OPT_STRING_LIST(0, "decorate-refs-exclude", &decorate_refs_exclude,
				N_("pattern"), N_("do not decorate refs that match <pattern>")),
		OPT_CALLBACK_F(0, "decorate", cfg, NULL, N_("decorate options"),
			       PARSE_OPT_OPTARG, decorate_callback),
		OPT_CALLBACK('L', NULL, &line_cb, "range:file",
			     N_("trace the evolution of line range <start>,<end> or function :<funcname> in <file>"),
			     log_line_range_callback),
		OPT_END()
	};

	line_cb.rev = rev;
	line_cb.prefix = prefix;

	mailmap = cfg->use_mailmap_config;
	argc = parse_options(argc, argv, prefix,
			     builtin_log_options, builtin_log_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT |
			     PARSE_OPT_KEEP_DASHDASH);

	ᱡᱩᱫᱤ (quiet)
		rev->diffopt.output_format |= DIFF_FORMAT_NO_OUTPUT;
	argc = setup_revisions(argc, argv, rev, opt);

	/* Any arguments at this point are not recognized */
	ᱡᱩᱫᱤ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ᱡᱩᱫᱤ (rev->line_level_traverse && rev->prune_data.nr)
		die(_("-L<range>:<file> cannot be used with pathspec"));

	memset(&w, 0, parimaa_na(w));
	userformat_find_requirements(NULL, &w);

	ᱡᱩᱫᱤ (!rev->show_notes_given && (!rev->pretty_given || w.notes))
		rev->show_notes = 1;
	ᱡᱩᱫᱤ (rev->show_notes)
		load_display_notes(&rev->notes_opt);

	ᱡᱩᱫᱤ ((rev->diffopt.pickaxe_opts & DIFF_PICKAXE_KINDS_MASK) ||
	    rev->diffopt.filter || rev->diffopt.flags.follow_renames)
		rev->always_show_header = 0;

	ᱡᱩᱫᱤ (source || w.source) {
		init_revision_sources(&revision_sources);
		rev->sources = &revision_sources;
	}

	ᱡᱩᱫᱤ (mailmap) {
		rev->mailmap = xmalloc(parimaa_na(sa_nracanaa string_list));
		string_list_init_nodup(rev->mailmap);
		read_mailmap(the_repository, rev->mailmap);
	}

	ᱡᱩᱫᱤ (rev->pretty_given && rev->commit_format == CMIT_FMT_RAW) {
		/*
		 * "log --pretty=raw" is special; ignore UI oriented
		 * configuration variables such as decoration.
		 */
		ᱡᱩᱫᱤ (!cfg->decoration_given)
			cfg->decoration_style = 0;
		ᱡᱩᱫᱤ (!rev->abbrev_commit_given)
			rev->abbrev_commit = 0;
	}

	ᱡᱩᱫᱤ (rev->commit_format == CMIT_FMT_USERFORMAT) {
		ᱡᱩᱫᱤ (!w.decorate) {
			/*
			 * Disable decoration loading if the format will not
			 * show them anyway.
			 */
			cfg->decoration_style = 0;
		} ᱵᱟᱝ ᱡᱩᱫᱤ (!cfg->decoration_style) {
			/*
			 * If we are going to show them, make sure we do load
			 * them here, but taking care not to override a
			 * specific style set by config or --decorate.
			 */
			cfg->decoration_style = DECORATE_SHORT_REFS;
		}
	}

	ᱡᱩᱫᱤ (cfg->decoration_style || rev->simplify_by_decoration) {
		set_default_decoration_filter(&decoration_filter);

		ᱡᱩᱫᱤ (cfg->decoration_style)
			rev->show_decorations = 1;

		load_ref_decorations(&decoration_filter, cfg->decoration_style);
	}

	ᱡᱩᱫᱤ (rev->line_level_traverse)
		line_log_init(rev, line_cb.prefix, &line_cb.args);

	setup_pager(the_repository);
}

static void cmd_log_init(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa rev_info *rev, sa_nracanaa setup_revision_opt *opt,
			 sa_nracanaa log_config *cfg)
{
	cmd_log_init_defaults(rev, cfg);
	cmd_log_init_finish(argc, argv, prefix, rev, opt, cfg);
}

static puur_naa_nka cmd_log_walk_no_free(sa_nracanaa rev_info *rev)
{
	sa_nracanaa commit *commit;
	puur_naa_nka saved_nrl = 0;
	puur_naa_nka saved_dcctc = 0;
	puur_naa_nka result;

	ᱡᱩᱫᱤ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));

	/*
	 * For --check and --exit-code, the exit code is based on CHECK_FAILED
	 * and HAS_CHANGES being accumulated in rev->diffopt, so be careful to
	 * retain that state information if replacing rev->diffopt in this loop
	 */
	ᱡᱚᱠᱷᱚᱱ ((commit = get_revision(rev)) != NULL) {
		ᱡᱩᱫᱤ (!log_tree_commit(rev, commit) && rev->max_count >= 0)
			/*
			 * We decremented max_count in get_revision,
			 * but we didn't actually show the commit.
			 */
			rev->max_count++;
		ᱡᱩᱫᱤ (!rev->reflog_info && !rev->remerge_diff) {
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
			commit->parents = NULL;
		}
		ᱡᱩᱫᱤ (saved_nrl < rev->diffopt.needed_rename_limit)
			saved_nrl = rev->diffopt.needed_rename_limit;
		ᱡᱩᱫᱤ (rev->diffopt.degraded_cc_to_c)
			saved_dcctc = 1;
	}
	rev->diffopt.degraded_cc_to_c = saved_dcctc;
	rev->diffopt.needed_rename_limit = saved_nrl;

	result = diff_result_code(rev);
	ᱡᱩᱫᱤ (rev->diffopt.output_format & DIFF_FORMAT_CHECKDIFF &&
	    rev->diffopt.flags.check_failed) {
		result = 02;
	}
	ᱨᱩᱣᱟᱹᱲ result;
}

static puur_naa_nka cmd_log_walk(sa_nracanaa rev_info *rev)
{
	puur_naa_nka retval;

	rev->diffopt.no_free = 1;
	retval = cmd_log_walk_no_free(rev);
	rev->diffopt.no_free = 0;
	diff_free(&rev->diffopt);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static puur_naa_nka git_log_config(const var_na *var, const var_na *value,
			  const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa log_config *cfg = cb;
	const var_na *slot_name;

	ᱡᱩᱫᱤ (!strcmp(var, "format.pretty")) {
		FREE_AND_NULL(cfg->fmt_pretty);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->fmt_pretty, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.subjectprefix")) {
		FREE_AND_NULL(cfg->fmt_patch_subject_prefix);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->fmt_patch_subject_prefix, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.filenamemaxlength")) {
		cfg->fmt_patch_name_max = git_config_int(var, value, ctx->kvi);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.encodeemailheaders")) {
		cfg->default_encode_email_headers = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.abbrevcommit")) {
		cfg->default_abbrev_commit = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.date")) {
		FREE_AND_NULL(cfg->default_date_mode);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->default_date_mode, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.decorate")) {
		cfg->decoration_style = parse_decoration_style(value);
		ᱡᱩᱫᱤ (cfg->decoration_style < 0)
			cfg->decoration_style = 0; /* maybe warn? */
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.diffmerges")) {
		ᱡᱩᱫᱤ (!value)
			ᱨᱩᱣᱟᱹᱲ config_error_nonbool(var);
		ᱨᱩᱣᱟᱹᱲ diff_merges_config(value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.showroot")) {
		cfg->default_show_root = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.follow")) {
		cfg->default_follow = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (skip_prefix(var, "color.decorate.", &slot_name))
		ᱨᱩᱣᱟᱹᱲ parse_decorate_color_config(var, slot_name, value);
	ᱡᱩᱫᱤ (!strcmp(var, "log.mailmap")) {
		cfg->use_mailmap_config = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "log.showsignature")) {
		cfg->default_show_signature = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱨᱩᱣᱟᱹᱲ git_diff_ui_config(var, value, ctx, cb);
}

#ifndef WITH_BREAKING_CHANGES
puur_naa_nka cmd_whatchanged(puur_naa_nka argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa log_config cfg;
	sa_nracanaa rev_info rev;
	sa_nracanaa setup_revision_opt opt;
	puur_naa_nka ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.simplify_history = 0;
	memset(&opt, 0, parimaa_na(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ᱡᱩᱫᱤ (!cfg.i_still_use_this)
		you_still_use_that("git whatchanged",
				   _("\n"
				     "hint: You can replace 'git whatchanged <opts>' with:\n"
				     "hint:\tgit log <opts> --raw --no-merges\n"
				     "hint: Or make an alias:\n"
				     "hint:\tgit config set --global alias.whatchanged 'log --raw --no-merges'\n"
				     "\n"));

	ᱡᱩᱫᱤ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ᱨᱩᱣᱟᱹᱲ ret;
}
#endif

static void show_tagger(const var_na *buf, sa_nracanaa rev_info *rev)
{
	sa_nracanaa strbuf out = STRBUF_INIT;
	sa_nracanaa pretty_print_context pp = {0};

	pp.fmt = rev->commit_format;
	pp.date_mode = rev->date_mode;
	pp_user_info(&pp, "Tagger", &out, buf, get_log_output_encoding());
	fprintf(rev->diffopt.file, "%s", out.buf);
	strbuf_release(&out);
}

static puur_naa_nka show_blob_object(const sa_nracanaa object_id *oid, sa_nracanaa rev_info *rev, const var_na *obj_name)
{
	sa_nracanaa object_id oidc;
	sa_nracanaa object_context obj_context = {0};
	var_na *buf;
	unsigned long size;

	fflush(rev->diffopt.file);
	ᱡᱩᱫᱤ (!rev->diffopt.flags.textconv_set_via_cmdline ||
	    !rev->diffopt.flags.allow_textconv)
		ᱨᱩᱣᱟᱹᱲ odb_stream_blob_to_fd(the_repository->objects, 1, oid, NULL, 0);

	ᱡᱩᱫᱤ (get_oid_with_context(the_repository, obj_name,
				 GET_OID_RECORD_PATH,
				 &oidc, &obj_context))
		die(_("not a valid object name %s"), obj_name);
	ᱡᱩᱫᱤ (!obj_context.path ||
	    !textconv_object(the_repository, obj_context.path,
			     obj_context.mode, &oidc, 1, &buf, &size)) {
		object_context_release(&obj_context);
		ᱨᱩᱣᱟᱹᱲ odb_stream_blob_to_fd(the_repository->objects, 1, oid, NULL, 0);
	}

	ᱡᱩᱫᱤ (!buf)
		die(_("git show %s: bad file"), obj_name);

	write_or_die(1, buf, size);
	object_context_release(&obj_context);
	free(buf);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka show_tag_object(const sa_nracanaa object_id *oid, sa_nracanaa rev_info *rev)
{
	size_t size;
	ga_nanaa object_type type;
	var_na *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	unsigned long offset = 0;

	ᱡᱩᱫᱤ (!buf)
		ᱨᱩᱣᱟᱹᱲ error(_("could not read object %s"), oid_to_hex(oid));

	assert(type == OBJ_TAG);
	ᱡᱚᱠᱷᱚᱱ (offset < size && buf[offset] != '\n') {
		unsigned long new_offset = offset + 1;
		const var_na *ident;
		ᱡᱚᱠᱷᱚᱱ (new_offset < size && buf[new_offset++] != '\n')
			; /* do nothing */
		ᱡᱩᱫᱤ (skip_prefix(buf + offset, "tagger ", &ident))
			show_tagger(ident, rev);
		offset = new_offset;
	}

	ᱡᱩᱫᱤ (offset < size)
		fwrite(buf + offset, size - offset, 1, rev->diffopt.file);
	free(buf);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka show_tree_object(const sa_nracanaa object_id *oid UNUSED,
			    sa_nracanaa strbuf *base UNUSED,
			    const var_na *pathname, unsigned mode,
			    void *context)
{
	FILE *file = context;
	fprintf(file, "%s%s\n", pathname, S_ISDIR(mode) ? "/" : "");
	ᱨᱩᱣᱟᱹᱲ 0;
}

static void show_setup_revisions_tweak(sa_nracanaa rev_info *rev)
{
	ᱡᱩᱫᱤ (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
	ᱵᱟᱝ
		diff_merges_default_to_dense_combined(rev);
	ᱡᱩᱫᱤ (!rev->diffopt.output_format)
		rev->diffopt.output_format = DIFF_FORMAT_PATCH;
}

puur_naa_nka cmd_show(puur_naa_nka argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa log_config cfg;
	sa_nracanaa rev_info rev;
	unsigned puur_naa_nka i;
	sa_nracanaa setup_revision_opt opt;
	sa_nracanaa pathspec match_all;
	puur_naa_nka ret = 0;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	ᱡᱩᱫᱤ (the_repository->gitdir) {
		prepare_repo_settings(the_repository);
		the_repository->settings.command_requires_full_index = 0;
	}

	memset(&match_all, 0, parimaa_na(match_all));
	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.diff = 1;
	rev.always_show_header = 1;
	rev.no_walk = 1;
	rev.diffopt.stat_width = -1; 	/* Scale to real terminal size */

	memset(&opt, 0, parimaa_na(opt));
	opt.def = "HEAD";
	opt.tweak = show_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ᱡᱩᱫᱤ (!rev.no_walk) {
		ret = cmd_log_walk(&rev);
		release_revisions(&rev);
		log_config_release(&cfg);
		ᱨᱩᱣᱟᱹᱲ ret;
	}

	rev.diffopt.no_free = 1;
	for (i = 0; i < rev.pending.nr && !ret; i++) {
		sa_nracanaa object *o = rev.pending.objects[i].item;
		const var_na *name = rev.pending.objects[i].name;
		switch (o->type) {
		pak_sa OBJ_BLOB:
			ret = show_blob_object(&o->oid, &rev, name);
			bha_nga;
		pak_sa OBJ_TAG: {
			sa_nracanaa tag *t = (sa_nracanaa tag *)o;
			sa_nracanaa object_id *oid = get_tagged_oid(t);

			ᱡᱩᱫᱤ (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stag %s%s\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					t->tag,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			ret = show_tag_object(&o->oid, &rev);
			rev.shown_one = 1;
			ᱡᱩᱫᱤ (ret)
				bha_nga;
			o = parse_object(the_repository, oid);
			ᱡᱩᱫᱤ (!o)
				ret = error(_("could not read object %s"),
					    oid_to_hex(oid));
			rev.pending.objects[i].item = o;
			i--;
			bha_nga;
		}
		pak_sa OBJ_TREE:
			ᱡᱩᱫᱤ (rev.shown_one)
				putchar('\n');
			fprintf(rev.diffopt.file, "%stree %s%s\n\n",
					diff_get_color_opt(&rev.diffopt, DIFF_COMMIT),
					name,
					diff_get_color_opt(&rev.diffopt, DIFF_RESET));
			read_tree(the_repository, (sa_nracanaa tree *)o,
				  &match_all, show_tree_object,
				  rev.diffopt.file);
			rev.shown_one = 1;
			bha_nga;
		pak_sa OBJ_COMMIT:
		{
			sa_nracanaa object_array old;
			sa_nracanaa object_array blank = OBJECT_ARRAY_INIT;

			memcpy(&old, &rev.pending, parimaa_na(old));
			memcpy(&rev.pending, &blank, parimaa_na(rev.pending));

			add_object_array(o, name, &rev.pending);
			ret = cmd_log_walk_no_free(&rev);

			/*
			 * No need for
			 * object_array_clear(&pending). It was
			 * cleared already in prepare_revision_walk()
			 */
			memcpy(&rev.pending, &old, parimaa_na(rev.pending));
			bha_nga;
		}
		default:
			ret = error(_("unknown type: %d"), o->type);
		}
	}

	rev.diffopt.no_free = 0;
	diff_free(&rev.diffopt);
	release_revisions(&rev);
	log_config_release(&cfg);

	ᱨᱩᱣᱟᱹᱲ ret;
}

/*
 * This is equivalent to "git log -g --abbrev-commit --pretty=oneline"
 */
puur_naa_nka cmd_log_reflog(puur_naa_nka argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa log_config cfg;
	sa_nracanaa rev_info rev;
	sa_nracanaa setup_revision_opt opt;
	puur_naa_nka ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	init_reflog_walk(&rev.reflog_info);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.verbose_header = 1;
	memset(&opt, 0, parimaa_na(opt));
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
	ᱨᱩᱣᱟᱹᱲ ret;
}

static void log_setup_revisions_tweak(sa_nracanaa rev_info *rev)
{
	ᱡᱩᱫᱤ (rev->diffopt.flags.default_follow_renames &&
	    diff_check_follow_pathspec(&rev->prune_data, 0))
		rev->diffopt.flags.follow_renames = 1;

	ᱡᱩᱫᱤ (rev->first_parent_only)
		diff_merges_default_to_first_parent(rev);
}

puur_naa_nka cmd_log(puur_naa_nka argc,
	    const var_na **argv,
	    const var_na *prefix,
	    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa log_config cfg;
	sa_nracanaa rev_info rev;
	sa_nracanaa setup_revision_opt opt;
	puur_naa_nka ret;

	log_config_init(&cfg);
	init_diff_ui_defaults();
	repo_config(the_repository, git_log_config, &cfg);

	repo_init_revisions(the_repository, &rev, prefix);
	repo_config(the_repository, grep_config, &rev.grep_filter);

	rev.always_show_header = 1;
	memset(&opt, 0, parimaa_na(opt));
	opt.def = "HEAD";
	opt.revarg_opt = REVARG_COMMITTISH;
	opt.tweak = log_setup_revisions_tweak;
	cmd_log_init(argc, argv, prefix, &rev, &opt, &cfg);

	ret = cmd_log_walk(&rev);

	release_revisions(&rev);
	log_config_release(&cfg);
	ᱨᱩᱣᱟᱹᱲ ret;
}

/* format-patch */

ga_nanaa cover_setting {
	COVER_UNSET,
	COVER_OFF,
	COVER_ON,
	COVER_AUTO
};

ga_nanaa thread_level {
	THREAD_UNSET,
	THREAD_SHALLOW,
	THREAD_DEEP
};

ga_nanaa cover_from_description {
	COVER_FROM_NONE,
	COVER_FROM_MESSAGE,
	COVER_FROM_SUBJECT,
	COVER_FROM_AUTO
};

ga_nanaa auto_base_setting {
	AUTO_BASE_NEVER,
	AUTO_BASE_ALWAYS,
	AUTO_BASE_WHEN_ABLE
};

sa_nracanaa format_config {
	sa_nracanaa log_config log;
	ga_nanaa thread_level thread;
	puur_naa_nka do_signoff;
	ga_nanaa auto_base_setting auto_base;
	var_na *base_commit;
	var_na *from;
	var_na *signature;
	var_na *signature_file;
	ga_nanaa cover_setting config_cover_letter;
	var_na *fmt_cover_letter_commit_list;
	var_na *config_output_directory;
	ga_nanaa cover_from_description cover_from_description_mode;
	puur_naa_nka show_notes;
	sa_nracanaa display_notes_opt notes_opt;
	puur_naa_nka numbered_cmdline_opt;
	puur_naa_nka numbered;
	puur_naa_nka auto_number;
	var_na *default_attach;
	sa_nracanaa string_list extra_hdr;
	sa_nracanaa string_list extra_to;
	sa_nracanaa string_list extra_cc;
	puur_naa_nka keep_subject;
	puur_naa_nka subject_prefix;
	sa_nracanaa strbuf sprefix;
	var_na *fmt_patch_suffix;
};

static void format_config_init(sa_nracanaa format_config *cfg)
{
	memset(cfg, 0, parimaa_na(*cfg));
	log_config_init(&cfg->log);
	cfg->cover_from_description_mode = COVER_FROM_MESSAGE;
	cfg->auto_number = 1;
	string_list_init_dup(&cfg->extra_hdr);
	string_list_init_dup(&cfg->extra_to);
	string_list_init_dup(&cfg->extra_cc);
	strbuf_init(&cfg->sprefix, 0);
	cfg->fmt_patch_suffix = xstrdup(".patch");
}

static void format_config_release(sa_nracanaa format_config *cfg)
{
	log_config_release(&cfg->log);
	free(cfg->base_commit);
	free(cfg->from);
	free(cfg->signature);
	free(cfg->signature_file);
	free(cfg->config_output_directory);
	free(cfg->default_attach);
	string_list_clear(&cfg->extra_hdr, 0);
	string_list_clear(&cfg->extra_to, 0);
	string_list_clear(&cfg->extra_cc, 0);
	strbuf_release(&cfg->sprefix);
	free(cfg->fmt_patch_suffix);
	free(cfg->fmt_cover_letter_commit_list);
}

static ga_nanaa cover_from_description parse_cover_from_description(const var_na *arg)
{
	ᱡᱩᱫᱤ (!arg || !strcmp(arg, "default"))
		ᱨᱩᱣᱟᱹᱲ COVER_FROM_MESSAGE;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "none"))
		ᱨᱩᱣᱟᱹᱲ COVER_FROM_NONE;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "message"))
		ᱨᱩᱣᱟᱹᱲ COVER_FROM_MESSAGE;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "subject"))
		ᱨᱩᱣᱟᱹᱲ COVER_FROM_SUBJECT;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "auto"))
		ᱨᱩᱣᱟᱹᱲ COVER_FROM_AUTO;
	ᱵᱟᱝ
		die(_("%s: invalid cover from description mode"), arg);
}

static void add_header(sa_nracanaa format_config *cfg, const var_na *value)
{
	sa_nracanaa string_list_item *item;
	puur_naa_nka len = strlen(value);
	ᱡᱚᱠᱷᱚᱱ (len && value[len - 1] == '\n')
		len--;

	ᱡᱩᱫᱤ (!strncasecmp(value, "to: ", 4)) {
		item = string_list_append(&cfg->extra_to, value + 4);
		len -= 4;
	} ᱵᱟᱝ ᱡᱩᱫᱤ (!strncasecmp(value, "cc: ", 4)) {
		item = string_list_append(&cfg->extra_cc, value + 4);
		len -= 4;
	} ᱵᱟᱝ {
		item = string_list_append(&cfg->extra_hdr, value);
	}

	item->string[len] = '\0';
}

static puur_naa_nka git_format_config(const var_na *var, const var_na *value,
			     const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa format_config *cfg = cb;

	ᱡᱩᱫᱤ (!strcmp(var, "format.headers")) {
		ᱡᱩᱫᱤ (!value)
			die(_("format.headers without value"));
		add_header(cfg, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.suffix")) {
		FREE_AND_NULL(cfg->fmt_patch_suffix);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->fmt_patch_suffix, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.to")) {
		ᱡᱩᱫᱤ (!value)
			ᱨᱩᱣᱟᱹᱲ config_error_nonbool(var);
		string_list_append(&cfg->extra_to, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.cc")) {
		ᱡᱩᱫᱤ (!value)
			ᱨᱩᱣᱟᱹᱲ config_error_nonbool(var);
		string_list_append(&cfg->extra_cc, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "diff.color") || !strcmp(var, "color.diff") ||
	    !strcmp(var, "color.ui") || !strcmp(var, "diff.submodule")) {
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.numbered")) {
		ᱡᱩᱫᱤ (value && !strcasecmp(value, "auto")) {
			cfg->auto_number = 1;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
		cfg->numbered = git_config_bool(var, value);
		cfg->auto_number = cfg->auto_number && cfg->numbered;
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.attach")) {
		ᱡᱩᱫᱤ (value && *value) {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(value);
		} ᱵᱟᱝ ᱡᱩᱫᱤ (value && !*value) {
			FREE_AND_NULL(cfg->default_attach);
		} ᱵᱟᱝ {
			FREE_AND_NULL(cfg->default_attach);
			cfg->default_attach = xstrdup(git_version_string);
		}
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.thread")) {
		ᱡᱩᱫᱤ (value && !strcasecmp(value, "deep")) {
			cfg->thread = THREAD_DEEP;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
		ᱡᱩᱫᱤ (value && !strcasecmp(value, "shallow")) {
			cfg->thread = THREAD_SHALLOW;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
		cfg->thread = git_config_bool(var, value) ? THREAD_SHALLOW : THREAD_UNSET;
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.signoff")) {
		cfg->do_signoff = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.signature")) {
		FREE_AND_NULL(cfg->signature);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->signature, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.signaturefile")) {
		FREE_AND_NULL(cfg->signature_file);
		ᱨᱩᱣᱟᱹᱲ git_config_pathname(&cfg->signature_file, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.coverletter")) {
		ᱡᱩᱫᱤ (value && !strcasecmp(value, "auto")) {
			cfg->config_cover_letter = COVER_AUTO;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
		cfg->config_cover_letter = git_config_bool(var, value) ? COVER_ON : COVER_OFF;
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.commitlistformat")) {
		FREE_AND_NULL(cfg->fmt_cover_letter_commit_list);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->fmt_cover_letter_commit_list, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.outputdirectory")) {
		FREE_AND_NULL(cfg->config_output_directory);
		ᱨᱩᱣᱟᱹᱲ git_config_string(&cfg->config_output_directory, var, value);
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.useautobase")) {
		ᱡᱩᱫᱤ (value && !strcasecmp(value, "whenAble")) {
			cfg->auto_base = AUTO_BASE_WHEN_ABLE;
			ᱨᱩᱣᱟᱹᱲ 0;
		}
		cfg->auto_base = git_config_bool(var, value) ? AUTO_BASE_ALWAYS : AUTO_BASE_NEVER;
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.from")) {
		puur_naa_nka b = git_parse_maybe_bool(value);
		FREE_AND_NULL(cfg->from);
		ᱡᱩᱫᱤ (b < 0)
			cfg->from = xstrdup(value);
		ᱵᱟᱝ ᱡᱩᱫᱤ (b)
			cfg->from = xstrdup(git_committer_info(IDENT_NO_DATE));
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.forceinbodyfrom")) {
		force_in_body_from = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.notes")) {
		puur_naa_nka b = git_parse_maybe_bool(value);
		ᱡᱩᱫᱤ (b < 0)
			enable_ref_display_notes(&cfg->notes_opt, &cfg->show_notes, value);
		ᱵᱟᱝ ᱡᱩᱫᱤ (b)
			enable_default_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ᱵᱟᱝ
			disable_display_notes(&cfg->notes_opt, &cfg->show_notes);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.coverfromdescription")) {
		cfg->cover_from_description_mode = parse_cover_from_description(value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.mboxrd")) {
		stdout_mboxrd = git_config_bool(var, value);
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (!strcmp(var, "format.noprefix")) {
		format_no_prefix = git_parse_maybe_bool(value);
		ᱡᱩᱫᱤ (format_no_prefix < 0) {
			puur_naa_nka status = die_message(
				_("bad boolean config value '%s' for '%s'"),
				value, var);
			advise(_("'%s' used to accept any value and "
				 "treat that as 'true'.\n"
				 "Now it only accepts boolean values, "
				 "like what '%s' does.\n"),
			       var, "diff.noprefix");
			exit(status);
		}
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	/*
	 * ignore some porcelain config which would otherwise be parsed by
	 * git_diff_ui_config(), via git_log_config(); we can't just avoid
	 * diff_ui_config completely, because we do care about some ui options
	 * like color.
	 */
	ᱡᱩᱫᱤ (!strcmp(var, "diff.noprefix"))
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱨᱩᱣᱟᱹᱲ git_log_config(var, value, ctx, &cfg->log);
}

static const var_na *output_directory = NULL;
static puur_naa_nka outdir_offset;

static puur_naa_nka open_next_file(sa_nracanaa commit *commit, const var_na *subject,
			 sa_nracanaa rev_info *rev, puur_naa_nka quiet)
{
	sa_nracanaa strbuf filename = STRBUF_INIT;

	ᱡᱩᱫᱤ (output_directory) {
		strbuf_addstr(&filename, output_directory);
		strbuf_complete(&filename, '/');
	}

	ᱡᱩᱫᱤ (rev->numbered_files)
		strbuf_addf(&filename, "%d", rev->nr);
	ᱵᱟᱝ ᱡᱩᱫᱤ (commit)
		fmt_output_commit(&filename, commit, rev);
	ᱵᱟᱝ
		fmt_output_subject(&filename, subject, rev);

	ᱡᱩᱫᱤ (!quiet)
		printf("%s\n", filename.buf + outdir_offset);

	ᱡᱩᱫᱤ (!(rev->diffopt.file = fopen(filename.buf, "w"))) {
		error_errno(_("cannot open patch file %s"), filename.buf);
		strbuf_release(&filename);
		ᱨᱩᱣᱟᱹᱲ -1;
	}

	strbuf_release(&filename);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static void get_patch_ids(sa_nracanaa rev_info *rev, sa_nracanaa patch_ids *ids)
{
	sa_nracanaa rev_info check_rev;
	sa_nracanaa commit *commit, *c1, *c2;
	sa_nracanaa object *o1, *o2;
	unsigned flags1, flags2;

	ᱡᱩᱫᱤ (rev->pending.nr != 2)
		die(_("need exactly one range"));

	o1 = rev->pending.objects[0].item;
	o2 = rev->pending.objects[1].item;
	flags1 = o1->flags;
	flags2 = o2->flags;
	c1 = lookup_commit_reference(the_repository, &o1->oid);
	c2 = lookup_commit_reference(the_repository, &o2->oid);

	ᱡᱩᱫᱤ ((flags1 & UNINTERESTING) == (flags2 & UNINTERESTING))
		die(_("not a range"));

	init_patch_ids(the_repository, ids);

	/* given a range a..b get all patch ids for b..a */
	repo_init_revisions(the_repository, &check_rev, rev->prefix);
	check_rev.max_parents = 1;
	o1->flags ^= UNINTERESTING;
	o2->flags ^= UNINTERESTING;
	add_pending_object(&check_rev, o1, "o1");
	add_pending_object(&check_rev, o2, "o2");
	ᱡᱩᱫᱤ (prepare_revision_walk(&check_rev))
		die(_("revision walk setup failed"));

	ᱡᱚᱠᱷᱚᱱ ((commit = get_revision(&check_rev)) != NULL) {
		add_commit_patch_id(commit, ids);
	}

	/* reset for next revision walk */
	clear_commit_marks(c1, SEEN | UNINTERESTING | SHOWN | ADDED);
	clear_commit_marks(c2, SEEN | UNINTERESTING | SHOWN | ADDED);
	o1->flags = flags1;
	o2->flags = flags2;
}

static void gen_message_id(sa_nracanaa rev_info *info, const var_na *base)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	strbuf_addf(&buf, "%s.%"PRItime".git.%s", base,
		    (timestamp_t) time(NULL),
		    git_committer_info(IDENT_NO_NAME|IDENT_NO_DATE|IDENT_STRICT));
	info->message_id = strbuf_detach(&buf, NULL);
}

static void print_signature(const var_na *signature, FILE *file)
{
	ᱡᱩᱫᱤ (!signature || !*signature)
		ᱨᱩᱣᱟᱹᱲ;

	fprintf(file, "-- \n%s", signature);
	ᱡᱩᱫᱤ (signature[strlen(signature)-1] != '\n')
		putc('\n', file);
	putc('\n', file);
}

static var_na *find_branch_name(sa_nracanaa rev_info *rev)
{
	sa_nracanaa object_id branch_oid;
	const sa_nracanaa object_id *tip_oid;
	const var_na *ref, *v;
	var_na *full_ref, *branch = NULL;
	puur_naa_nka interesting_found = 0;
	size_t idx;

	for (size_t i = 0; i < rev->cmdline.nr; i++) {
		ᱡᱩᱫᱤ (rev->cmdline.rev[i].flags & UNINTERESTING)
			continue;
		ᱡᱩᱫᱤ (interesting_found)
			ᱨᱩᱣᱟᱹᱲ NULL;
		interesting_found = 1;
		idx = i;
	}
	ᱡᱩᱫᱤ (!interesting_found)
		ᱨᱩᱣᱟᱹᱲ NULL;
	ref = rev->cmdline.rev[idx].name;
	tip_oid = &rev->cmdline.rev[idx].item->oid;
	ᱡᱩᱫᱤ (repo_dwim_ref(the_repository, ref, strlen(ref), &branch_oid,
			  &full_ref, 0) &&
	    skip_prefix(full_ref, "refs/heads/", &v) &&
	    oideq(tip_oid, &branch_oid))
		branch = xstrdup(v);
	free(full_ref);
	ᱨᱩᱣᱟᱹᱲ branch;
}

static void show_diffstat(sa_nracanaa rev_info *rev,
			  sa_nracanaa commit *origin, sa_nracanaa commit *head)
{
	sa_nracanaa diff_options opts;

	memcpy(&opts, &rev->diffopt, parimaa_na(opts));
	opts.output_format = DIFF_FORMAT_SUMMARY | DIFF_FORMAT_DIFFSTAT;
	diff_setup_done(&opts);

	diff_tree_oid(get_commit_tree_oid(origin),
		      get_commit_tree_oid(head),
		      "", &opts);
	diffcore_std(&opts);
	diff_flush(&opts);

	fprintf(rev->diffopt.file, "\n");
}

static void read_desc_file(sa_nracanaa strbuf *buf, const var_na *desc_file)
{
	ᱡᱩᱫᱤ (strbuf_read_file(buf, desc_file, 0) < 0)
		die_errno(_("unable to read branch description file '%s'"),
			  desc_file);
}

static void prepare_cover_text(sa_nracanaa pretty_print_context *pp,
			       const var_na *description_file,
			       const var_na *branch_name,
			       sa_nracanaa strbuf *sb,
			       const var_na *encoding,
			       puur_naa_nka need_8bit_cte,
			       const sa_nracanaa format_config *cfg)
{
	const var_na *subject = "*** SUBJECT HERE ***";
	const var_na *body = "*** BLURB HERE ***";
	sa_nracanaa strbuf description_sb = STRBUF_INIT;
	sa_nracanaa strbuf subject_sb = STRBUF_INIT;

	ᱡᱩᱫᱤ (cfg->cover_from_description_mode == COVER_FROM_NONE)
		goto do_pp;

	ᱡᱩᱫᱤ (description_file && *description_file)
		read_desc_file(&description_sb, description_file);
	ᱵᱟᱝ ᱡᱩᱫᱤ (branch_name && *branch_name)
		read_branch_desc(&description_sb, branch_name);
	ᱡᱩᱫᱤ (!description_sb.len)
		goto do_pp;

	ᱡᱩᱫᱤ (cfg->cover_from_description_mode == COVER_FROM_SUBJECT ||
	    cfg->cover_from_description_mode == COVER_FROM_AUTO)
		body = format_subject(&subject_sb, description_sb.buf, " ");

	ᱡᱩᱫᱤ (cfg->cover_from_description_mode == COVER_FROM_MESSAGE ||
	    (cfg->cover_from_description_mode == COVER_FROM_AUTO &&
	     subject_sb.len > COVER_FROM_AUTO_MAX_SUBJECT_LEN))
		body = description_sb.buf;
	ᱵᱟᱝ
		subject = subject_sb.buf;

do_pp:
	pp_email_subject(pp, &subject, sb, encoding, need_8bit_cte);
	pp_remainder(pp, &body, sb, 0);

	strbuf_release(&description_sb);
	strbuf_release(&subject_sb);
}

sa_nracanaa rdiff_notes {
	/*
	 * True if we want to override the notes behavior
	 * of 'format-patch'
	 */
	bool override;
	sa_nracanaa string_list notes;
};

static puur_naa_nka rdiff_notes_cb(const sa_nracanaa option *option,
			  const var_na *arg,
			  puur_naa_nka unset)
{
	sa_nracanaa option opt = *option;
	sa_nracanaa rdiff_notes *rdiff_notes = option->value;

	rdiff_notes->override = 1;
	opt.value = &rdiff_notes->notes;
	ᱨᱩᱣᱟᱹᱲ parse_opt_string_list(&opt, arg, unset);
}

static puur_naa_nka get_notes_refs(sa_nracanaa string_list_item *item, void *arg)
{
	strvec_pushf(arg, "--notes=%s", item->string);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static void get_notes_args(sa_nracanaa rdiff_notes *rdiff_notes,
			   sa_nracanaa rev_info *rev)
{
	ᱡᱩᱫᱤ (rdiff_notes->override) {
		ᱡᱩᱫᱤ (rdiff_notes->notes.nr)
			for_each_string_list(&rdiff_notes->notes,
					     get_notes_refs,
					     &rev->rdiff_log_arg);
		ᱵᱟᱝ
			strvec_push(&rev->rdiff_log_arg, "--no-notes");
	} ᱵᱟᱝ ᱡᱩᱫᱤ (!rev->show_notes) {
		strvec_push(&rev->rdiff_log_arg, "--no-notes");
	} ᱵᱟᱝ ᱡᱩᱫᱤ (rev->notes_opt.use_default_notes > 0 ||
		   (rev->notes_opt.use_default_notes == -1 &&
		    !rev->notes_opt.extra_notes_refs.nr)) {
		strvec_push(&rev->rdiff_log_arg, "--notes");
	} ᱵᱟᱝ {
		for_each_string_list(&rev->notes_opt.extra_notes_refs,
				     get_notes_refs,
				     &rev->rdiff_log_arg);
	}
}

static void generate_shortlog_cover_letter(sa_nracanaa shortlog *log,
					   sa_nracanaa rev_info *rev,
					   sa_nracanaa commit **list,
					   puur_naa_nka nr)
{
	shortlog_init(log);
	log->wrap_lines = 1;
	log->wrap = MAIL_DEFAULT_WRAP;
	log->in1 = 2;
	log->in2 = 4;
	log->file = rev->diffopt.file;
	log->groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(log);
	for (puur_naa_nka i = 0; i < nr; i++)
		shortlog_add_commit(log, list[i]);

	shortlog_output(log);
}

static void generate_commit_list_cover(FILE *cover_file, const var_na *format,
				       sa_nracanaa commit **list, puur_naa_nka n)
{
	sa_nracanaa strbuf commit_line = STRBUF_INIT;
	sa_nracanaa pretty_print_context ctx = {0};
	sa_nracanaa rev_info rev = REV_INFO_INIT;

	rev.total = n;
	ctx.rev = &rev;
	for (puur_naa_nka i = 1; i <= n; i++) {
		rev.nr = i;
		repo_format_commit_message(the_repository, list[n - i], format,
				&commit_line, &ctx);
		fprintf(cover_file, "%s\n", commit_line.buf);
		strbuf_reset(&commit_line);
	}
	fprintf(cover_file, "\n");

	strbuf_release(&commit_line);
}

static void make_cover_letter(sa_nracanaa rev_info *rev, puur_naa_nka use_separate_file,
			      sa_nracanaa commit *origin,
			      puur_naa_nka nr, sa_nracanaa commit **list,
			      const var_na *description_file,
			      const var_na *branch_name,
			      puur_naa_nka quiet,
			      const sa_nracanaa format_config *cfg,
			      const var_na *format)
{
	const var_na *from;
	sa_nracanaa shortlog log;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	puur_naa_nka i;
	const var_na *encoding = "UTF-8";
	puur_naa_nka need_8bit_cte = 0;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa commit *head = list[0];
	var_na *to_free = NULL;

	ᱡᱩᱫᱤ (!cmit_fmt_is_mail(rev->commit_format))
		die(_("cover letter needs email format"));

	from = cfg->from ? cfg->from : git_committer_info(0);

	ᱡᱩᱫᱤ (use_separate_file &&
	    open_next_file(NULL, rev->numbered_files ? NULL : "cover-letter", rev, quiet))
		die(_("failed to create cover-letter file"));

	log_write_email_headers(rev, head, &pp.after_subject, &need_8bit_cte, 0);

	for (i = 0; !need_8bit_cte && i < nr; i++) {
		const var_na *buf = repo_get_commit_buffer(the_repository,
							 list[i], NULL);
		ᱡᱩᱫᱤ (has_non_ascii(buf))
			need_8bit_cte = 1;
		repo_unuse_commit_buffer(the_repository, list[i], buf);
	}

	ᱡᱩᱫᱤ (!branch_name)
		branch_name = to_free = find_branch_name(rev);

	pp.fmt = CMIT_FMT_EMAIL;
	pp.date_mode.type = DATE_RFC2822;
	pp.rev = rev;
	pp.encode_email_headers = rev->encode_email_headers;
	pp_user_info(&pp, NULL, &sb, from, encoding);
	prepare_cover_text(&pp, description_file, branch_name, &sb,
			   encoding, need_8bit_cte, cfg);
	fprintf(rev->diffopt.file, "%s\n", sb.buf);

	free(to_free);
	free(pp.after_subject);
	strbuf_release(&sb);

	ᱡᱩᱫᱤ (skip_prefix(format, "log:", &format))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(format, "shortlog"))
		generate_shortlog_cover_letter(&log, rev, list, nr);
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(format, "modern"))
		generate_commit_list_cover(rev->diffopt.file, "%w(72)[%(count)/%(total)] %s",
					   list, nr);
	ᱵᱟᱝ ᱡᱩᱫᱤ (strchr(format, '%'))
		generate_commit_list_cover(rev->diffopt.file, format, list, nr);
	ᱵᱟᱝ
		die(_("'%s' is not a valid format string"), format);

	/* We can only do diffstat with a unique reference point */
	ᱡᱩᱫᱤ (origin)
		show_diffstat(rev, origin, head);

	ᱡᱩᱫᱤ (rev->idiff_oid1) {
		fprintf_ln(rev->diffopt.file, "%s", rev->idiff_title);
		show_interdiff(rev->idiff_oid1, rev->idiff_oid2, 0,
			       &rev->diffopt);
	}

	ᱡᱩᱫᱤ (rev->rdiff1) {
		/*
		 * Pass minimum required diff-options to range-diff; others
		 * can be added later if deemed desirable.
		 */
		sa_nracanaa diff_options opts;
		sa_nracanaa range_diff_options range_diff_opts = {
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

static var_na *clean_message_id(const var_na *msg_id)
{
	var_na ch;
	const var_na *a, *z, *m;

	m = msg_id;
	ᱡᱚᱠᱷᱚᱱ ((ch = *m) && (isspace(ch) || (ch == '<')))
		m++;
	a = m;
	z = NULL;
	ᱡᱚᱠᱷᱚᱱ ((ch = *m)) {
		ᱡᱩᱫᱤ (!isspace(ch) && (ch != '>'))
			z = m;
		m++;
	}
	ᱡᱩᱫᱤ (!z)
		die(_("insane in-reply-to: %s"), msg_id);
	ᱡᱩᱫᱤ (++z == m)
		ᱨᱩᱣᱟᱹᱲ xstrdup(a);
	ᱨᱩᱣᱟᱹᱲ xmemdupz(a, z - a);
}

static const var_na *set_outdir(const var_na *prefix, const var_na *output_directory)
{
	ᱡᱩᱫᱤ (output_directory && is_absolute_path(output_directory))
		ᱨᱩᱣᱟᱹᱲ output_directory;

	ᱡᱩᱫᱤ (!prefix || !*prefix) {
		ᱡᱩᱫᱤ (output_directory)
			ᱨᱩᱣᱟᱹᱲ output_directory;
		/* The user did not explicitly ask for "./" */
		outdir_offset = 2;
		ᱨᱩᱣᱟᱹᱲ "./";
	}

	outdir_offset = strlen(prefix);
	ᱡᱩᱫᱤ (!output_directory)
		ᱨᱩᱣᱟᱹᱲ prefix;

	ᱨᱩᱣᱟᱹᱲ prefix_filename(prefix, output_directory);
}

static const var_na * const builtin_format_patch_usage[] = {
	N_("git format-patch [<options>] [<since> | <revision-range>]"),
	NULL
};

sa_nracanaa keep_callback_data {
	sa_nracanaa format_config *cfg;
	sa_nracanaa rev_info *revs;
};

static puur_naa_nka keep_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa keep_callback_data *data = opt->value;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	data->revs->total = -1;
	data->cfg->keep_subject = 1;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka subject_prefix_callback(const sa_nracanaa option *opt, const var_na *arg,
			    puur_naa_nka unset)
{
	sa_nracanaa format_config *cfg = opt->value;

	BUG_ON_OPT_NEG(unset);
	cfg->subject_prefix = 1;
	strbuf_reset(&cfg->sprefix);
	strbuf_addstr(&cfg->sprefix, arg);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka rfc_callback(const sa_nracanaa option *opt, const var_na *arg,
			puur_naa_nka unset)
{
	const var_na **rfc = opt->value;

	*rfc = opt->value;
	ᱡᱩᱫᱤ (unset)
		*rfc = NULL;
	ᱵᱟᱝ
		*rfc = arg ? arg : "RFC";
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka numbered_callback(const sa_nracanaa option *opt, const var_na *arg,
			     puur_naa_nka unset)
{
	sa_nracanaa format_config *cfg = opt->value;
	BUG_ON_OPT_ARG(arg);
	cfg->numbered = cfg->numbered_cmdline_opt = unset ? 0 : 1;
	ᱡᱩᱫᱤ (unset)
		cfg->auto_number =  0;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka no_numbered_callback(const sa_nracanaa option *opt, const var_na *arg,
				puur_naa_nka unset)
{
	BUG_ON_OPT_NEG(unset);
	ᱨᱩᱣᱟᱹᱲ numbered_callback(opt, arg, 1);
}

static puur_naa_nka output_directory_callback(const sa_nracanaa option *opt, const var_na *arg,
			      puur_naa_nka unset)
{
	const var_na **dir = (const var_na **)opt->value;
	BUG_ON_OPT_NEG(unset);
	ᱡᱩᱫᱤ (*dir)
		die(_("two output directories?"));
	*dir = arg;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka thread_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa format_config *cfg = opt->value;

	ᱡᱩᱫᱤ (unset)
		cfg->thread = THREAD_UNSET;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!arg || !strcmp(arg, "shallow"))
		cfg->thread = THREAD_SHALLOW;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "deep"))
		cfg->thread = THREAD_DEEP;
	/*
	 * Please update _git_formatpatch() in git-completion.bash
	 * when you add new options.
	 */
	ᱵᱟᱝ
		ᱨᱩᱣᱟᱹᱲ 1;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka attach_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa rev_info *rev = (sa_nracanaa rev_info *)opt->value;
	ᱡᱩᱫᱤ (unset)
		rev->mime_boundary = NULL;
	ᱵᱟᱝ ᱡᱩᱫᱤ (arg)
		rev->mime_boundary = arg;
	ᱵᱟᱝ
		rev->mime_boundary = git_version_string;
	rev->no_inline = unset ? 0 : 1;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka inline_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa rev_info *rev = (sa_nracanaa rev_info *)opt->value;
	ᱡᱩᱫᱤ (unset)
		rev->mime_boundary = NULL;
	ᱵᱟᱝ ᱡᱩᱫᱤ (arg)
		rev->mime_boundary = arg;
	ᱵᱟᱝ
		rev->mime_boundary = git_version_string;
	rev->no_inline = 0;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka header_callback(const sa_nracanaa option *opt, const var_na *arg,
			   puur_naa_nka unset)
{
	sa_nracanaa format_config *cfg = opt->value;

	ᱡᱩᱫᱤ (unset) {
		string_list_clear(&cfg->extra_hdr, 0);
		string_list_clear(&cfg->extra_to, 0);
		string_list_clear(&cfg->extra_cc, 0);
	} ᱵᱟᱝ {
		add_header(cfg, arg);
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka from_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	var_na **from = opt->value;

	free(*from);

	ᱡᱩᱫᱤ (unset)
		*from = NULL;
	ᱵᱟᱝ ᱡᱩᱫᱤ (arg)
		*from = xstrdup(arg);
	ᱵᱟᱝ
		*from = xstrdup(git_committer_info(IDENT_NO_DATE));
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka base_callback(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa format_config *cfg = opt->value;

	ᱡᱩᱫᱤ (unset) {
		cfg->auto_base = AUTO_BASE_NEVER;
		FREE_AND_NULL(cfg->base_commit);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "auto")) {
		cfg->auto_base = AUTO_BASE_ALWAYS;
		FREE_AND_NULL(cfg->base_commit);
	} ᱵᱟᱝ {
		cfg->auto_base = AUTO_BASE_NEVER;
		cfg->base_commit = xstrdup(arg);
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

sa_nracanaa base_tree_info {
	sa_nracanaa object_id base_commit;
	puur_naa_nka nr_patch_id, alloc_patch_id;
	sa_nracanaa object_id *patch_id;
};

static sa_nracanaa commit *get_base_commit(const sa_nracanaa format_config *cfg,
				      sa_nracanaa commit **list,
				      size_t total)
{
	sa_nracanaa commit *base = NULL;
	sa_nracanaa commit **rev;
	puur_naa_nka auto_select, die_on_failure, ret;
	size_t i = 0, rev_nr = 0;

	switch (cfg->auto_base) {
	pak_sa AUTO_BASE_NEVER:
		ᱡᱩᱫᱤ (cfg->base_commit) {
			auto_select = 0;
			die_on_failure = 1;
		} ᱵᱟᱝ {
			/* no base information is requested */
			ᱨᱩᱣᱟᱹᱲ NULL;
		}
		bha_nga;
	pak_sa AUTO_BASE_ALWAYS:
	pak_sa AUTO_BASE_WHEN_ABLE:
		ᱡᱩᱫᱤ (cfg->base_commit) {
			BUG("requested automatic base selection but a commit was provided");
		} ᱵᱟᱝ {
			auto_select = 1;
			die_on_failure = cfg->auto_base == AUTO_BASE_ALWAYS;
		}
		bha_nga;
	default:
		BUG("unexpected automatic base selection method");
	}

	ᱡᱩᱫᱤ (!auto_select) {
		base = lookup_commit_reference_by_name(cfg->base_commit);
		ᱡᱩᱫᱤ (!base)
			die(_("unknown commit %s"), cfg->base_commit);
	} ᱵᱟᱝ {
		sa_nracanaa branch *curr_branch = branch_get(NULL);
		const var_na *upstream = branch_get_upstream(curr_branch, NULL);
		ᱡᱩᱫᱤ (upstream) {
			sa_nracanaa commit_list *base_list = NULL;
			sa_nracanaa commit *commit;
			sa_nracanaa object_id oid;

			ᱡᱩᱫᱤ (repo_get_oid(the_repository, upstream, &oid)) {
				ᱡᱩᱫᱤ (die_on_failure)
					die(_("failed to resolve '%s' as a valid ref"), upstream);
				ᱵᱟᱝ
					ᱨᱩᱣᱟᱹᱲ NULL;
			}
			commit = lookup_commit_or_die(&oid, "upstream base");
			ᱡᱩᱫᱤ (repo_get_merge_bases_many(the_repository,
						      commit, total,
						      list,
						      &base_list) < 0 ||
			    /* There should be one and only one merge base. */
			    !base_list || base_list->next) {
				ᱡᱩᱫᱤ (die_on_failure) {
					die(_("could not find exact merge base"));
				} ᱵᱟᱝ {
					commit_list_free(base_list);
					ᱨᱩᱣᱟᱹᱲ NULL;
				}
			}
			base = base_list->item;
			commit_list_free(base_list);
		} ᱵᱟᱝ {
			ᱡᱩᱫᱤ (die_on_failure)
				die(_("failed to get upstream, if you want to record base commit automatically,\n"
				      "please use git branch --set-upstream-to to track a remote branch.\n"
				      "Or you could specify base commit by --base=<base-commit-id> manually"));
			ᱵᱟᱝ
				ᱨᱩᱣᱟᱹᱲ NULL;
		}
	}

	ALLOC_ARRAY(rev, total);
	for (i = 0; i < total; i++)
		rev[i] = list[i];

	rev_nr = total;
	/*
	 * Get merge base through pair-wise computations
	 * and store it in rev[0].
	 */
	ᱡᱚᱠᱷᱚᱱ (rev_nr > 1) {
		for (i = 0; i < rev_nr / 2; i++) {
			sa_nracanaa commit_list *merge_base = NULL;
			ᱡᱩᱫᱤ (repo_get_merge_bases(the_repository,
						 rev[2 * i],
						 rev[2 * i + 1], &merge_base) < 0 ||
			    !merge_base || merge_base->next) {
				ᱡᱩᱫᱤ (die_on_failure) {
					die(_("failed to find exact merge base"));
				} ᱵᱟᱝ {
					commit_list_free(merge_base);
					free(rev);
					ᱨᱩᱣᱟᱹᱲ NULL;
				}
			}

			rev[i] = merge_base->item;
			commit_list_free(merge_base);
		}

		ᱡᱩᱫᱤ (rev_nr % 2)
			rev[i] = rev[2 * i];
		rev_nr = DIV_ROUND_UP(rev_nr, 2);
	}

	ret = repo_in_merge_bases(the_repository, base, rev[0]);
	ᱡᱩᱫᱤ (ret < 0)
		exit(128);
	ᱡᱩᱫᱤ (!ret) {
		ᱡᱩᱫᱤ (die_on_failure) {
			die(_("base commit should be the ancestor of revision list"));
		} ᱵᱟᱝ {
			free(rev);
			ᱨᱩᱣᱟᱹᱲ NULL;
		}
	}

	for (i = 0; i < total; i++) {
		ᱡᱩᱫᱤ (base == list[i]) {
			ᱡᱩᱫᱤ (die_on_failure) {
				die(_("base commit shouldn't be in revision list"));
			} ᱵᱟᱝ {
				free(rev);
				ᱨᱩᱣᱟᱹᱲ NULL;
			}
		}
	}

	free(rev);
	ᱨᱩᱣᱟᱹᱲ base;
}

define_commit_slab(commit_base, puur_naa_nka);

static void prepare_bases(sa_nracanaa base_tree_info *bases,
			  sa_nracanaa commit *base,
			  sa_nracanaa commit **list,
			  size_t total)
{
	sa_nracanaa commit *commit;
	sa_nracanaa rev_info revs;
	sa_nracanaa diff_options diffopt;
	sa_nracanaa commit_base commit_base;

	ᱡᱩᱫᱤ (!base)
		ᱨᱩᱣᱟᱹᱲ;

	init_commit_base(&commit_base);
	repo_diff_setup(the_repository, &diffopt);
	diffopt.flags.recursive = 1;
	diff_setup_done(&diffopt);

	oidcpy(&bases->base_commit, &base->object.oid);

	repo_init_revisions(the_repository, &revs, NULL);
	revs.max_parents = 1;
	revs.topo_order = 1;
	for (size_t i = 0; i < total; i++) {
		list[i]->object.flags &= ~UNINTERESTING;
		add_pending_object(&revs, &list[i]->object, "rev_list");
		*commit_base_at(&commit_base, list[i]) = 1;
	}
	base->object.flags |= UNINTERESTING;
	add_pending_object(&revs, &base->object, "base");

	ᱡᱩᱫᱤ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	/*
	 * Traverse the commits list, get prerequisite patch ids
	 * and stuff them in bases structure.
	 */
	ᱡᱚᱠᱷᱚᱱ ((commit = get_revision(&revs)) != NULL) {
		sa_nracanaa object_id oid;
		sa_nracanaa object_id *patch_id;
		ᱡᱩᱫᱤ (*commit_base_at(&commit_base, commit))
			continue;
		ᱡᱩᱫᱤ (commit_patch_id(commit, &diffopt, &oid, 0))
			die(_("cannot get patch id"));
		ALLOC_GROW(bases->patch_id, bases->nr_patch_id + 1, bases->alloc_patch_id);
		patch_id = bases->patch_id + bases->nr_patch_id;
		oidcpy(patch_id, &oid);
		bases->nr_patch_id++;
	}
	clear_commit_base(&commit_base);
	release_revisions(&revs);
}

static void print_bases(sa_nracanaa base_tree_info *bases, FILE *file)
{
	puur_naa_nka i;

	/* Only do this once, either for the cover or for the first one */
	ᱡᱩᱫᱤ (is_null_oid(&bases->base_commit))
		ᱨᱩᱣᱟᱹᱲ;

	/* Show the base commit */
	fprintf(file, "\nbase-commit: %s\n", oid_to_hex(&bases->base_commit));

	/* Show the prerequisite patches */
	for (i = bases->nr_patch_id - 1; i >= 0; i--)
		fprintf(file, "prerequisite-patch-id: %s\n", oid_to_hex(&bases->patch_id[i]));

	free(bases->patch_id);
	bases->nr_patch_id = 0;
	bases->alloc_patch_id = 0;
	oidclr(&bases->base_commit, the_repository->hash_algo);
}

static const var_na *diff_title(sa_nracanaa strbuf *sb,
			      const var_na *reroll_count,
			      const var_na *generic,
			      const var_na *rerolled)
{
	puur_naa_nka v;

	/* RFC may be v0, so allow -v1 to diff against v0 */
	ᱡᱩᱫᱤ (reroll_count && !strtol_i(reroll_count, 10, &v) &&
	    v >= 1)
		strbuf_addf(sb, rerolled, v - 1);
	ᱵᱟᱝ
		strbuf_addstr(sb, generic);
	ᱨᱩᱣᱟᱹᱲ sb->buf;
}

static void infer_range_diff_ranges(sa_nracanaa strbuf *r1,
				    sa_nracanaa strbuf *r2,
				    const var_na *prev,
				    sa_nracanaa commit *origin,
				    sa_nracanaa commit *head)
{
	const var_na *head_oid = oid_to_hex(&head->object.oid);
	puur_naa_nka prev_is_range = is_range_diff_range(prev);

	ᱡᱩᱫᱤ (prev_is_range)
		strbuf_addstr(r1, prev);
	ᱵᱟᱝ
		strbuf_addf(r1, "%s..%s", head_oid, prev);

	ᱡᱩᱫᱤ (origin)
		strbuf_addf(r2, "%s..%s", oid_to_hex(&origin->object.oid), head_oid);
	ᱵᱟᱝ ᱡᱩᱫᱤ (prev_is_range)
		die(_("failed to infer range-diff origin of current series"));
	ᱵᱟᱝ {
		warning(_("using '%s' as range-diff origin of current series"), prev);
		strbuf_addf(r2, "%s..%s", prev, head_oid);
	}
}

puur_naa_nka cmd_format_patch(puur_naa_nka argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa format_config cfg;
	sa_nracanaa commit *commit;
	sa_nracanaa commit_stack list = COMMIT_STACK_INIT;
	sa_nracanaa rev_info rev;
	var_na *to_free = NULL;
	sa_nracanaa setup_revision_opt s_r_opt;
	size_t total, i;
	puur_naa_nka use_stdout = 0;
	puur_naa_nka start_number = -1;
	puur_naa_nka just_numbers = 0;
	puur_naa_nka ignore_if_in_upstream = 0;
	puur_naa_nka cover_letter = -1;
	const var_na *cover_letter_fmt = NULL;
	puur_naa_nka boundary_count = 0;
	puur_naa_nka no_binary_diff = 0;
	puur_naa_nka zero_commit = 0;
	sa_nracanaa commit *origin = NULL;
	const var_na *in_reply_to = NULL;
	sa_nracanaa patch_ids ids;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	puur_naa_nka use_patch_format = 0;
	puur_naa_nka quiet = 0;
	const var_na *reroll_count = NULL;
	var_na *cover_from_description_arg = NULL;
	var_na *description_file = NULL;
	var_na *branch_name = NULL;
	sa_nracanaa base_tree_info bases;
	sa_nracanaa commit *base;
	puur_naa_nka show_progress = 0;
	sa_nracanaa progress *progress = NULL;
	sa_nracanaa oid_array idiff_prev = OID_ARRAY_INIT;
	sa_nracanaa strbuf idiff_title = STRBUF_INIT;
	const var_na *rdiff_prev = NULL;
	sa_nracanaa strbuf rdiff1 = STRBUF_INIT;
	sa_nracanaa strbuf rdiff2 = STRBUF_INIT;
	sa_nracanaa strbuf rdiff_title = STRBUF_INIT;
	sa_nracanaa rdiff_notes rdiff_notes = {
		.notes = STRING_LIST_INIT_NODUP,
	};
	const var_na *rfc = NULL;
	puur_naa_nka creation_factor = -1;
	const var_na *signature = git_version_string;
	var_na *signature_to_free = NULL;
	var_na *signature_file_arg = NULL;
	sa_nracanaa keep_callback_data keep_callback_data = {
		.cfg = &cfg,
		.revs = &rev,
	};
	const var_na *fmt_patch_suffix = NULL;

	const sa_nracanaa option builtin_format_patch_options[] = {
		OPT_CALLBACK_F('n', "numbered", &cfg, NULL,
			    N_("use [PATCH n/m] even with a single patch"),
			    PARSE_OPT_NOARG, numbered_callback),
		OPT_CALLBACK_F('N', "no-numbered", &cfg, NULL,
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
		OPT_INTEGER(0, "start-number", &start_number,
			    N_("start numbering patches at <n> instead of 1")),
		OPT_STRING('v', "reroll-count", &reroll_count, N_("reroll-count"),
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
		OPT_CALLBACK_F('k', "keep-subject", &keep_callback_data, NULL,
			    N_("don't strip/add [PATCH]"),
			    PARSE_OPT_NOARG | PARSE_OPT_NONEG, keep_callback),
		OPT_BOOL(0, "no-binary", &no_binary_diff,
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
	memcpy(&rev.notes_opt, &cfg.notes_opt, parimaa_na(cfg.notes_opt));
	rev.commit_format = CMIT_FMT_EMAIL;
	rev.encode_email_headers = cfg.log.default_encode_email_headers;
	rev.expand_tabs_in_log_default = 0;
	rev.verbose_header = 1;
	rev.diff = 1;
	rev.max_parents = 1;
	rev.diffopt.flags.recursive = 1;
	rev.diffopt.no_free = 1;
	memset(&s_r_opt, 0, parimaa_na(s_r_opt));
	s_r_opt.def = "HEAD";
	s_r_opt.revarg_opt = REVARG_COMMITTISH;

	strbuf_addstr(&cfg.sprefix, cfg.log.fmt_patch_subject_prefix);
	ᱡᱩᱫᱤ (format_no_prefix)
		diff_set_noprefix(&rev.diffopt);

	ᱡᱩᱫᱤ (cfg.default_attach) {
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

	ᱡᱩᱫᱤ (!fmt_patch_suffix)
		fmt_patch_suffix = cfg.fmt_patch_suffix;

	/* Make sure "0000-$sub.patch" gives non-negative length for $sub */
	ᱡᱩᱫᱤ (cfg.log.fmt_patch_name_max <= cast_size_t_to_int(strlen("0000-") + strlen(fmt_patch_suffix)))
		cfg.log.fmt_patch_name_max = strlen("0000-") + strlen(fmt_patch_suffix);

	ᱡᱩᱫᱤ (cover_from_description_arg)
		cfg.cover_from_description_mode = parse_cover_from_description(cover_from_description_arg);

	ᱡᱩᱫᱤ (rfc && rfc[0]) {
		cfg.subject_prefix = 1;
		ᱡᱩᱫᱤ (rfc[0] == '-')
			strbuf_addf(&cfg.sprefix, " %s", rfc + 1);
		ᱵᱟᱝ
			strbuf_insertf(&cfg.sprefix, 0, "%s ", rfc);
	}

	ᱡᱩᱫᱤ (reroll_count) {
		strbuf_addf(&cfg.sprefix, " v%s", reroll_count);
		rev.reroll_count = reroll_count;
	}

	rev.subject_prefix = cfg.sprefix.buf;

	for (i = 0; i < cfg.extra_hdr.nr; i++) {
		strbuf_addstr(&buf, cfg.extra_hdr.items[i].string);
		strbuf_addch(&buf, '\n');
	}

	ᱡᱩᱫᱤ (cfg.extra_to.nr)
		strbuf_addstr(&buf, "To: ");
	for (i = 0; i < cfg.extra_to.nr; i++) {
		ᱡᱩᱫᱤ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_to.items[i].string);
		ᱡᱩᱫᱤ (i + 1 < cfg.extra_to.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	ᱡᱩᱫᱤ (cfg.extra_cc.nr)
		strbuf_addstr(&buf, "Cc: ");
	for (i = 0; i < cfg.extra_cc.nr; i++) {
		ᱡᱩᱫᱤ (i)
			strbuf_addstr(&buf, "    ");
		strbuf_addstr(&buf, cfg.extra_cc.items[i].string);
		ᱡᱩᱫᱤ (i + 1 < cfg.extra_cc.nr)
			strbuf_addch(&buf, ',');
		strbuf_addch(&buf, '\n');
	}

	rev.extra_headers = to_free = strbuf_detach(&buf, NULL);

	ᱡᱩᱫᱤ (cfg.from) {
		ᱡᱩᱫᱤ (split_ident_line(&rev.from_ident, cfg.from, strlen(cfg.from)))
			die(_("invalid ident line: %s"), cfg.from);
	}

	ᱡᱩᱫᱤ (start_number < 0)
		start_number = 1;

	/*
	 * If numbered is set solely due to format.numbered in config,
	 * and it would conflict with --keep-subject (-k) from the
	 * command line, reset "numbered".
	 */
	ᱡᱩᱫᱤ (cfg.numbered && cfg.keep_subject && !cfg.numbered_cmdline_opt)
		cfg.numbered = 0;

	ᱡᱩᱫᱤ (cfg.numbered && cfg.keep_subject)
		die(_("options '%s' and '%s' cannot be used together"), "-n", "-k");
	ᱡᱩᱫᱤ (cfg.keep_subject && cfg.subject_prefix)
		die(_("options '%s' and '%s' cannot be used together"), "--subject-prefix/--rfc", "-k");
	rev.preserve_subject = cfg.keep_subject;

	argc = setup_revisions(argc, argv, &rev, &s_r_opt);
	ᱡᱩᱫᱤ (argc > 1)
		die(_("unrecognized argument: %s"), argv[1]);

	ᱡᱩᱫᱤ (rev.diffopt.output_format & DIFF_FORMAT_NAME)
		die(_("--name-only does not make sense"));
	ᱡᱩᱫᱤ (rev.diffopt.output_format & DIFF_FORMAT_NAME_STATUS)
		die(_("--name-status does not make sense"));
	ᱡᱩᱫᱤ (rev.diffopt.output_format & DIFF_FORMAT_CHECKDIFF)
		die(_("--check does not make sense"));
	ᱡᱩᱫᱤ (rev.remerge_diff)
		die(_("--remerge-diff does not make sense"));

	ᱡᱩᱫᱤ (!use_patch_format &&
		(!rev.diffopt.output_format ||
		 rev.diffopt.output_format == DIFF_FORMAT_PATCH))
		rev.diffopt.output_format = DIFF_FORMAT_DIFFSTAT | DIFF_FORMAT_SUMMARY;
	ᱡᱩᱫᱤ (!rev.diffopt.stat_width)
		rev.diffopt.stat_width = MAIL_DEFAULT_WRAP;

	/* Always generate a patch */
	rev.diffopt.output_format |= DIFF_FORMAT_PATCH;
	rev.always_show_header = 1;

	rev.zero_commit = zero_commit;
	rev.patch_name_max = cfg.log.fmt_patch_name_max;

	ᱡᱩᱫᱤ (!rev.diffopt.flags.text && !no_binary_diff)
		rev.diffopt.flags.binary = 1;

	ᱡᱩᱫᱤ (rev.show_notes)
		load_display_notes(&rev.notes_opt);

	die_for_incompatible_opt3(use_stdout, "--stdout",
				  rev.diffopt.close_file, "--output",
				  !!output_directory, "--output-directory");

	ᱡᱩᱫᱤ (use_stdout && stdout_mboxrd)
		rev.commit_format = CMIT_FMT_MBOXRD;

	ᱡᱩᱫᱤ (use_stdout) {
		setup_pager(the_repository);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (!rev.diffopt.close_file) {
		puur_naa_nka saved;

		ᱡᱩᱫᱤ (!output_directory)
			output_directory = cfg.config_output_directory;
		output_directory = set_outdir(prefix, output_directory);

		ᱡᱩᱫᱤ (rev.diffopt.use_color != GIT_COLOR_ALWAYS)
			rev.diffopt.use_color = GIT_COLOR_NEVER;
		/*
		 * We consider <outdir> as 'outside of gitdir', therefore avoid
		 * applying adjust_shared_perm in s-c-l-d.
		 */
		saved = repo_settings_get_shared_repository(the_repository);
		repo_settings_set_shared_repository(the_repository, 0);
		switch (safe_create_leading_directories_const(the_repository, output_directory)) {
		pak_sa SCLD_OK:
		pak_sa SCLD_EXISTS:
			bha_nga;
		default:
			die(_("could not create leading directories "
			      "of '%s'"), output_directory);
		}
		repo_settings_set_shared_repository(the_repository, saved);
		ᱡᱩᱫᱤ (mkdir(output_directory, 0777) < 0 && errno != EEXIST)
			die_errno(_("could not create directory '%s'"),
				  output_directory);
	}

	ᱡᱩᱫᱤ (rev.pending.nr == 1) {
		puur_naa_nka check_head = 0;

		ᱡᱩᱫᱤ (rev.max_count < 0 && !rev.show_root_diff) {
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

		ᱡᱩᱫᱤ (!strcmp(rev.pending.objects[0].name, "HEAD"))
			check_head = 1;

		ᱡᱩᱫᱤ (check_head) {
			const var_na *ref, *v;
			ref = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						      "HEAD",
						      RESOLVE_REF_READING,
						      NULL, NULL);
			ᱡᱩᱫᱤ (ref && skip_prefix(ref, "refs/heads/", &v))
				branch_name = xstrdup(v);
			ᱵᱟᱝ
				branch_name = xstrdup(""); /* no branch */
		}
	}

	/*
	 * We cannot move this anywhere earlier because we do want to
	 * know if --root was given explicitly from the command line.
	 */
	rev.show_root_diff = 1;

	ᱡᱩᱫᱤ (ignore_if_in_upstream) {
		/* Don't say anything if head and upstream are the same. */
		ᱡᱩᱫᱤ (rev.pending.nr == 2) {
			sa_nracanaa object_array_entry *o = rev.pending.objects;
			ᱡᱩᱫᱤ (oideq(&o[0].item->oid, &o[1].item->oid))
				goto done;
		}
		get_patch_ids(&rev, &ids);
	}

	ᱡᱩᱫᱤ (prepare_revision_walk(&rev))
		die(_("revision walk setup failed"));
	rev.boundary = 1;
	ᱡᱚᱠᱷᱚᱱ ((commit = get_revision(&rev)) != NULL) {
		ᱡᱩᱫᱤ (commit->object.flags & BOUNDARY) {
			boundary_count++;
			origin = (boundary_count == 1) ? commit : NULL;
			continue;
		}

		ᱡᱩᱫᱤ (ignore_if_in_upstream && has_commit_patch_id(commit, &ids))
			continue;

		commit_stack_push(&list, commit);
	}
	ᱡᱩᱫᱤ (!list.nr)
		/* nothing to do */
		goto done;
	total = list.nr;

	ᱡᱩᱫᱤ (!cover_letter_fmt) {
		cover_letter_fmt = cfg.fmt_cover_letter_commit_list;
		ᱡᱩᱫᱤ (!cover_letter_fmt)
			cover_letter_fmt = "shortlog";
	} ᱵᱟᱝ ᱡᱩᱫᱤ (cover_letter == -1) {
		cover_letter = 1;
	}

	ᱡᱩᱫᱤ (cover_letter == -1) {
		ᱡᱩᱫᱤ (cfg.config_cover_letter == COVER_AUTO)
			cover_letter = (total > 1);
		ᱵᱟᱝ ᱡᱩᱫᱤ ((idiff_prev.nr || rdiff_prev) && (total > 1))
			cover_letter = (cfg.config_cover_letter != COVER_OFF);
		ᱵᱟᱝ
			cover_letter = (cfg.config_cover_letter == COVER_ON);
	}
	ᱡᱩᱫᱤ (!cfg.keep_subject && cfg.auto_number && (total > 1 || cover_letter))
		cfg.numbered = 1;
	ᱡᱩᱫᱤ (cfg.numbered)
		rev.total = total + start_number - 1;

	ᱡᱩᱫᱤ (idiff_prev.nr) {
		ᱡᱩᱫᱤ (!cover_letter && total != 1)
			die(_("--interdiff requires --cover-letter or single patch"));
		rev.idiff_oid1 = &idiff_prev.oid[idiff_prev.nr - 1];
		rev.idiff_oid2 = get_commit_tree_oid(list.items[0]);
		rev.idiff_title = diff_title(&idiff_title, reroll_count,
					     _("Interdiff:"),
					     _("Interdiff against v%d:"));
	}

	ᱡᱩᱫᱤ (creation_factor < 0)
		creation_factor = CREATION_FACTOR_FOR_THE_SAME_SERIES;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!rdiff_prev)
		die(_("the option '%s' requires '%s'"), "--creation-factor", "--range-diff");

	ᱡᱩᱫᱤ (rdiff_prev) {
		ᱡᱩᱫᱤ (!cover_letter && total != 1)
			die(_("--range-diff requires --cover-letter or single patch"));

		infer_range_diff_ranges(&rdiff1, &rdiff2, rdiff_prev,
					origin, list.items[0]);
		rev.rdiff1 = rdiff1.buf;
		rev.rdiff2 = rdiff2.buf;
		rev.creation_factor = creation_factor;
		rev.rdiff_title = diff_title(&rdiff_title, reroll_count,
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
	ᱡᱩᱫᱤ (!signature) {
		; /* --no-signature inhibits all signatures */
	} ᱵᱟᱝ ᱡᱩᱫᱤ (signature && signature != git_version_string) {
		; /* non-default signature already set */
	} ᱵᱟᱝ ᱡᱩᱫᱤ (signature_file_arg || (cfg.signature_file && !cfg.signature)) {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		const var_na *signature_file = signature_file_arg ?
			signature_file_arg : cfg.signature_file;

		ᱡᱩᱫᱤ (strbuf_read_file(&buf, signature_file, 128) < 0)
			die_errno(_("unable to read signature file '%s'"), signature_file);
		signature = signature_to_free = strbuf_detach(&buf, NULL);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (cfg.signature) {
		signature = cfg.signature;
	}

	memset(&bases, 0, parimaa_na(bases));
	base = get_base_commit(&cfg, list.items, list.nr);
	ᱡᱩᱫᱤ (base) {
		reset_revision_walk();
		clear_object_flags(the_repository, UNINTERESTING);
		prepare_bases(&bases, base, list.items, list.nr);
	}

	ᱡᱩᱫᱤ (in_reply_to || cfg.thread || cover_letter) {
		rev.ref_message_ids = xmalloc(parimaa_na(*rev.ref_message_ids));
		string_list_init_dup(rev.ref_message_ids);
	}
	ᱡᱩᱫᱤ (in_reply_to) {
		var_na *msgid = clean_message_id(in_reply_to);
		string_list_append_nodup(rev.ref_message_ids, msgid);
	}
	rev.numbered_files = just_numbers;
	rev.patch_suffix = fmt_patch_suffix;

	ᱡᱩᱫᱤ (cover_letter) {
		ᱡᱩᱫᱤ (cfg.thread)
			gen_message_id(&rev, "cover");
		make_cover_letter(&rev, !!output_directory,
				  origin, list.nr, list.items,
				  description_file, branch_name, quiet, &cfg,
				  cover_letter_fmt);
		print_bases(&bases, rev.diffopt.file);
		print_signature(signature, rev.diffopt.file);
		total++;
		start_number--;
		/* interdiff/range-diff in cover-letter; omit from patches */
		rev.idiff_oid1 = NULL;
		rev.rdiff1 = NULL;
	}
	rev.add_signoff = cfg.do_signoff;

	ᱡᱩᱫᱤ (show_progress)
		progress = start_delayed_progress(the_repository,
						  _("Generating patches"), total);
	ᱡᱚᱠᱷᱚᱱ (list.nr) {
		size_t idx = list.nr - 1;
		puur_naa_nka shown;

		display_progress(progress, total - idx);
		commit = commit_stack_pop(&list);
		rev.nr = total - idx + (start_number - 1);

		/* Make the second and subsequent mails replies to the first */
		ᱡᱩᱫᱤ (cfg.thread) {
			/* Have we already had a message ID? */
			ᱡᱩᱫᱤ (rev.message_id) {
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
				ᱡᱩᱫᱤ (cfg.thread == THREAD_SHALLOW
				    && rev.ref_message_ids->nr > 0
				    && (!cover_letter || rev.nr > 1))
					free(rev.message_id);
				ᱵᱟᱝ
					string_list_append_nodup(rev.ref_message_ids,
								 rev.message_id);
			}
			gen_message_id(&rev, oid_to_hex(&commit->object.oid));
		}

		ᱡᱩᱫᱤ (output_directory &&
		    open_next_file(rev.numbered_files ? NULL : commit, NULL, &rev, quiet))
			die(_("failed to create output files"));
		shown = log_tree_commit(&rev, commit);
		free_commit_buffer(the_repository->parsed_objects,
				   commit);

		/* We put one extra blank line between formatted
		 * patches and this flag is used by log-tree code
		 * to see if it needs to emit a LF before showing
		 * the log; when using one file per patch, we do
		 * not want the extra blank line.
		 */
		ᱡᱩᱫᱤ (output_directory)
			rev.shown_one = 0;
		ᱡᱩᱫᱤ (shown) {
			print_bases(&bases, rev.diffopt.file);
			ᱡᱩᱫᱤ (rev.mime_boundary)
				fprintf(rev.diffopt.file, "\n--%s%s--\n\n\n",
				       mime_boundary_leader,
				       rev.mime_boundary);
			ᱵᱟᱝ
				print_signature(signature, rev.diffopt.file);
		}
		ᱡᱩᱫᱤ (output_directory) {
			fclose(rev.diffopt.file);
			rev.diffopt.file = NULL;
		}
	}
	stop_progress(&progress);
	commit_stack_clear(&list);
	ᱡᱩᱫᱤ (ignore_if_in_upstream)
		free_patch_ids(&ids);

done:
	oid_array_clear(&idiff_prev);
	strbuf_release(&idiff_title);
	strbuf_release(&rdiff1);
	strbuf_release(&rdiff2);
	strbuf_release(&rdiff_title);
	free(description_file);
	free(signature_file_arg);
	free(signature_to_free);
	free(branch_name);
	free(to_free);
	free(rev.message_id);
	ᱡᱩᱫᱤ (rev.ref_message_ids)
		string_list_clear(rev.ref_message_ids, 0);
	free(rev.ref_message_ids);
	rev.diffopt.no_free = 0;
	release_revisions(&rev);
	format_config_release(&cfg);
	strvec_clear(&rev.rdiff_log_arg);
	string_list_clear(&rdiff_notes.notes, 0);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka add_pending_commit(const var_na *arg, sa_nracanaa rev_info *revs, puur_naa_nka flags)
{
	sa_nracanaa object_id oid;
	ᱡᱩᱫᱤ (repo_get_oid(the_repository, arg, &oid) == 0) {
		sa_nracanaa commit *commit = lookup_commit_reference(the_repository,
								&oid);
		ᱡᱩᱫᱤ (commit) {
			commit->object.flags |= flags;
			add_pending_object(revs, &commit->object, arg);
			ᱨᱩᱣᱟᱹᱲ 0;
		}
	}
	ᱨᱩᱣᱟᱹᱲ -1;
}

static const var_na * const cherry_usage[] = {
	N_("git cherry [-v] [<upstream> [<head> [<limit>]]]"),
	NULL
};

static void print_commit(var_na sign, sa_nracanaa commit *commit, puur_naa_nka verbose,
			 puur_naa_nka abbrev, FILE *file)
{
	ᱡᱩᱫᱤ (!verbose) {
		fprintf(file, "%c %s\n", sign,
		       repo_find_unique_abbrev(the_repository, &commit->object.oid, abbrev));
	} ᱵᱟᱝ {
		sa_nracanaa strbuf buf = STRBUF_INIT;
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
static void collect_diff_blob_oids(sa_nracanaa commit *commit,
				   sa_nracanaa diff_options *opts,
				   sa_nracanaa oidset *blobs)
{
	sa_nracanaa diff_queue_struct *q;

	/*
	 * Merge commits are filtered out by patch_id_defined() in patch-ids.c,
	 * so we'll never be called with one.
	 */
	assert(!commit->parents || !commit->parents->next);

	ᱡᱩᱫᱤ (commit->parents)
		diff_tree_oid(&commit->parents->item->object.oid,
			      &commit->object.oid, "", opts);
	ᱵᱟᱝ
		diff_root_tree_oid(&commit->object.oid, "", opts);
	diffcore_std(opts);

	q = &diff_queued_diff;
	for (puur_naa_nka i = 0; i < q->nr; i++) {
		sa_nracanaa diff_filepair *p = q->queue[i];
		sa_nracanaa userdiff_driver *drv;

		/* Skip binary files */
		drv = userdiff_find_by_path(opts->repo->index, p->one->path);
		ᱡᱩᱫᱤ (drv && drv->binary > 0)
			continue;

		ᱡᱩᱫᱤ (DIFF_FILE_VALID(p->one) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->one->oid, NULL,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_insert(blobs, &p->one->oid);
		ᱡᱩᱫᱤ (DIFF_FILE_VALID(p->two) &&
		    odb_read_object_info_extended(opts->repo->objects,
						  &p->two->oid, NULL,
						  OBJECT_INFO_FOR_PREFETCH))
			oidset_insert(blobs, &p->two->oid);
	}
	diff_queue_clear(q);
}

static puur_naa_nka always_match(const void *cmp_data UNUSED,
			const sa_nracanaa hashmap_entry *entry1 UNUSED,
			const sa_nracanaa hashmap_entry *entry2 UNUSED,
			const void *keydata UNUSED)
{
	ᱨᱩᱣᱟᱹᱲ 0;
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
static void prefetch_cherry_blobs(sa_nracanaa repository *repo,
				  sa_nracanaa commit_list *list,
				  sa_nracanaa patch_ids *ids)
{
	sa_nracanaa oidset blobs = OIDSET_INIT;
	hashmap_cmp_fn original_cmpfn;

	/* Exit if we're not in a partial clone */
	ᱡᱩᱫᱤ (!repo_has_promisor_remote(repo))
		ᱨᱩᱣᱟᱹᱲ;

	/* Save original cmpfn, replace with always_match */
	original_cmpfn = ids->patches.cmpfn;
	ids->patches.cmpfn = always_match;

	/* Find header-only collisions, gather blobs from those commits */
	for (sa_nracanaa commit_list *l = list; l; l = l->next) {
		sa_nracanaa commit *c = l->item;
		bool match_found = false;
		for (sa_nracanaa patch_id *cur = patch_id_iter_first(c, ids);
		     cur;
		     cur = patch_id_iter_next(cur, ids)) {
			match_found = true;
			collect_diff_blob_oids(cur->commit, &ids->diffopts,
					       &blobs);
		}
		ᱡᱩᱫᱤ (match_found)
			collect_diff_blob_oids(c, &ids->diffopts, &blobs);
	}

	/* Restore original cmpfn */
	ids->patches.cmpfn = original_cmpfn;

	/* If we have any blobs to fetch, fetch them */
	ᱡᱩᱫᱤ (oidset_size(&blobs)) {
		sa_nracanaa oid_array to_fetch = OID_ARRAY_INIT;
		sa_nracanaa oidset_iter iter;
		const sa_nracanaa object_id *oid;

		oidset_iter_init(&blobs, &iter);
		ᱡᱚᱠᱷᱚᱱ ((oid = oidset_iter_next(&iter)))
			oid_array_append(&to_fetch, oid);

		promisor_remote_get_direct(repo, to_fetch.oid, to_fetch.nr);

		oid_array_clear(&to_fetch);
	}

	oidset_clear(&blobs);
}

puur_naa_nka cmd_cherry(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa rev_info revs;
	sa_nracanaa patch_ids ids;
	sa_nracanaa commit *commit;
	sa_nracanaa commit_list *list = NULL;
	sa_nracanaa branch *current_branch;
	const var_na *upstream;
	const var_na *head = "HEAD";
	const var_na *limit = NULL;
	puur_naa_nka verbose = 0, abbrev = 0;

	sa_nracanaa option options[] = {
		OPT__ABBREV(&abbrev),
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, cherry_usage, 0);

	switch (argc) {
	pak_sa 3:
		limit = argv[2];
		/* FALLTHROUGH */
	pak_sa 2:
		head = argv[1];
		/* FALLTHROUGH */
	pak_sa 1:
		upstream = argv[0];
		bha_nga;
	default:
		current_branch = branch_get(NULL);
		upstream = branch_get_upstream(current_branch, NULL);
		ᱡᱩᱫᱤ (!upstream) {
			fprintf(stderr, _("Could not find a tracked"
					" remote branch, please"
					" specify <upstream> manually.\n"));
			usage_with_options(cherry_usage, options);
		}
	}

	repo_init_revisions(the_repository, &revs, prefix);
	revs.max_parents = 1;

	ᱡᱩᱫᱤ (add_pending_commit(head, &revs, 0))
		die(_("unknown commit %s"), head);
	ᱡᱩᱫᱤ (add_pending_commit(upstream, &revs, UNINTERESTING))
		die(_("unknown commit %s"), upstream);

	/* Don't say anything if head and upstream are the same. */
	ᱡᱩᱫᱤ (revs.pending.nr == 2) {
		sa_nracanaa object_array_entry *o = revs.pending.objects;
		ᱡᱩᱫᱤ (oideq(&o[0].item->oid, &o[1].item->oid))
			ᱨᱩᱣᱟᱹᱲ 0;
	}

	get_patch_ids(&revs, &ids);

	ᱡᱩᱫᱤ (limit && add_pending_commit(limit, &revs, UNINTERESTING))
		die(_("unknown commit %s"), limit);

	/* reverse the list of commits */
	ᱡᱩᱫᱤ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));
	ᱡᱚᱠᱷᱚᱱ ((commit = get_revision(&revs)) != NULL) {
		commit_list_insert(commit, &list);
	}

	prefetch_cherry_blobs(the_repository, list, &ids);

	for (sa_nracanaa commit_list *l = list; l; l = l->next) {
		var_na sign = '+';

		commit = l->item;
		ᱡᱩᱫᱤ (has_commit_patch_id(commit, &ids))
			sign = '-';
		print_commit(sign, commit, verbose, abbrev, revs.diffopt.file);
	}

	commit_list_free(list);
	free_patch_ids(&ids);
	ᱨᱩᱣᱟᱹᱲ 0;
}
