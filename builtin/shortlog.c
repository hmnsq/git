#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "commit.h"
#include "diff.h"
#include "environment.h"
#include "gettext.h"
#include "string-list.h"
#include "revision.h"
#include "utf8.h"
#include "mailmap.h"
#include "setup.h"
#include "shortlog.h"
#include "parse-options.h"
#include "trailer.h"
#include "strmap.h"

static var_na const * const shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	NULL
};

/*
 * The util field of our string_list_items will contain one of two things:
 *
 *   - if --summary is not in use, it will point to a string list of the
 *     oneline subjects assigned to this author
 *
 *   - if --summary is in use, we don't need that list; we only need to know
 *     its size. So we abuse the pointer slot to store our integer counter.
 *
 *  This macro accesses the latter.
 */
#define UTIL_TO_INT(x) ((intptr_t)(x)->util)

static පූර්ණ_සංඛ්‍යා compare_by_counter(const void *a1, const void *a2)
{
	const sa_nracanaa string_list_item *i1 = a1, *i2 = a2;
	ආපසු UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

static පූර්ණ_සංඛ්‍යා compare_by_list(const void *a1, const void *a2)
{
	const sa_nracanaa string_list_item *i1 = a1, *i2 = a2;
	const sa_nracanaa string_list *l1 = i1->util, *l2 = i2->util;

	නම් (l1->nr < l2->nr)
		ආපසු 1;
	නැතහොත් නම් (l1->nr == l2->nr)
		ආපසු 0;
	නැතහොත්
		ආපසු -1;
}

static void insert_one_record(sa_nracanaa shortlog *log,
			      const var_na *ident,
			      const var_na *oneline)
{
	sa_nracanaa string_list_item *item;

	item = string_list_insert(&log->list, ident);

	නම් (log->summary)
		item->util = (void *)(UTIL_TO_INT(item) + 1);
	නැතහොත් {
		var_na *buffer;
		sa_nracanaa strbuf subject = STRBUF_INIT;
		const var_na *eol;

		/* Skip any leading whitespace, including any blank lines. */
		අතරතුර (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		නම් (!eol)
			eol = oneline + strlen(oneline);
		නම් (starts_with(oneline, "[PATCH")) {
			const var_na *eob = strchr(oneline, ']');
			නම් (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		අතරතුර (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, NULL);

		නම් (!item->util) {
			item->util = xmalloc(parimaa_na(sa_nracanaa string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

static පූර්ණ_සංඛ්‍යා parse_ident(sa_nracanaa shortlog *log,
		       sa_nracanaa strbuf *out, const var_na *in)
{
	const var_na *mailbuf, *namebuf;
	size_t namelen, maillen;
	sa_nracanaa ident_split ident;

	නම් (split_ident_line(&ident, in, strlen(in)))
		ආපසු -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	නම් (log->email)
		strbuf_addf(out, " <%.*s>", (පූර්ණ_සංඛ්‍යා)maillen, mailbuf);

	ආපසු 0;
}

static void read_from_stdin(sa_nracanaa shortlog *log)
{
	sa_nracanaa strbuf ident = STRBUF_INIT;
	sa_nracanaa strbuf mapped_ident = STRBUF_INIT;
	sa_nracanaa strbuf oneline = STRBUF_INIT;
	static const var_na *author_match[2] = { "Author: ", "author " };
	static const var_na *committer_match[2] = { "Commit: ", "committer " };
	const var_na **match;

	නම් (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	switch (log->groups) {
	pak_sa SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		bha_nga;
	pak_sa SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		bha_nga;
	pak_sa SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	pak_sa SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	default:
		BUG("unhandled shortlog group");
	}

	අතරතුර (strbuf_getline_lf(&ident, stdin) != EOF) {
		const var_na *v;
		නම් (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			continue;
		අතරතුර (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		අතරතුර (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		නම් (parse_ident(log, &mapped_ident, v) < 0)
			continue;

		insert_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

static void insert_records_from_trailers(sa_nracanaa shortlog *log,
					 sa_nracanaa strset *dups,
					 sa_nracanaa commit *commit,
					 sa_nracanaa pretty_print_context *ctx,
					 const var_na *oneline)
{
	sa_nracanaa trailer_iterator iter;
	const var_na *commit_buffer, *body;
	sa_nracanaa strbuf ident = STRBUF_INIT;

	නම් (!log->trailers.nr)
		ආපසු;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, NULL,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	නම් (!body)
		goto out;

	trailer_iterator_init(&iter, body);
	අතරතුර (trailer_iterator_advance(&iter)) {
		const var_na *value = iter.val.buf;

		නම් (!string_list_has_string(&log->trailers, iter.key.buf))
			continue;

		strbuf_reset(&ident);
		නම් (!parse_ident(log, &ident, value))
			value = ident.buf;

		නම් (!strset_add(dups, value))
			continue;
		insert_one_record(log, value, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

static පූර්ණ_සංඛ්‍යා shortlog_needs_dedup(const sa_nracanaa shortlog *log)
{
	ආපසු HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

static void insert_records_from_format(sa_nracanaa shortlog *log,
				       sa_nracanaa strset *dups,
				       sa_nracanaa commit *commit,
				       sa_nracanaa pretty_print_context *ctx,
				       const var_na *oneline)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		නම් (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			insert_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

void shortlog_add_commit(sa_nracanaa shortlog *log, sa_nracanaa commit *commit)
{
	sa_nracanaa strbuf oneline = STRBUF_INIT;
	sa_nracanaa strset dups = STRSET_INIT;
	sa_nracanaa pretty_print_context ctx = {0};
	const var_na *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	නම් (!log->summary) {
		නම් (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		නැතහොත්
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	insert_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	insert_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

static void get_from_rev(sa_nracanaa rev_info *rev, sa_nracanaa shortlog *log)
{
	sa_nracanaa commit *commit;

	නම් (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	අතරතුර ((commit = get_revision(rev)) != NULL)
		shortlog_add_commit(log, commit);
}

static පූර්ණ_සංඛ්‍යා parse_uint(var_na const **arg, පූර්ණ_සංඛ්‍යා comma, පූර්ණ_සංඛ්‍යා defval)
{
	unsigned long ul;
	පූර්ණ_සංඛ්‍යා ret;
	var_na *endp;

	ul = strtoul(*arg, &endp, 10);
	නම් (*endp && *endp != comma)
		ආපසු -1;
	නම් (ul > INT_MAX)
		ආපසු -1;
	ret = *arg == endp ? defval : (පූර්ණ_සංඛ්‍යා)ul;
	*arg = *endp ? endp + 1 : endp;
	ආපසු ret;
}

static const var_na wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

static පූර්ණ_සංඛ්‍යා parse_wrap_args(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa shortlog *log = opt->value;

	log->wrap_lines = !unset;
	නම් (unset)
		ආපසු 0;
	නම් (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		ආපසු 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	නම් (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		ආපසු error(wrap_arg_usage);
	නම් (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		ආපසු error(wrap_arg_usage);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා parse_group_option(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa shortlog *log = opt->value;
	const var_na *field;

	නම් (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} නැතහොත් නම් (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	නැතහොත් නම් (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	නැතහොත් නම් (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} නැතහොත් නම් (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} නැතහොත් නම් (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} නැතහොත් {
		ආපසු error(_("unknown group type: %s"), arg);
	}

	ආපසු 0;
}


void shortlog_init(sa_nracanaa shortlog *log)
{
	memset(log, 0, parimaa_na(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

void shortlog_finish_setup(sa_nracanaa shortlog *log)
{
	නම් (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	නම් (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

පූර්ණ_සංඛ්‍යා cmd_shortlog(පූර්ණ_සංඛ්‍යා argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa shortlog log = { STRING_LIST_INIT_NODUP };
	sa_nracanaa rev_info rev;
	පූර්ණ_සංඛ්‍යා nongit = !startup_info->have_repository;

	const sa_nracanaa option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_number,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', NULL, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	sa_nracanaa parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	නම් (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, NULL);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	සඳහා (;;) {
		switch (parse_options_step(&ctx, options, shortlog_usage)) {
		pak_sa PARSE_OPT_NON_OPTION:
		pak_sa PARSE_OPT_UNKNOWN:
			bha_nga;
		pak_sa PARSE_OPT_HELP:
			exit(0);
		pak_sa PARSE_OPT_HELP_ERROR:
		pak_sa PARSE_OPT_ERROR:
		pak_sa PARSE_OPT_SUBCOMMAND:
			exit(129);
		pak_sa PARSE_OPT_COMPLETE:
			exit(0);
		pak_sa PARSE_OPT_DONE:
			goto parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	නම් (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	නම් (!nongit && setup_revisions(argc, argv, &rev, NULL) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	නම් (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	නම් (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	නම් (rev.pending.nr == 0) {
		නම් (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	නැතහොත්
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	ආපසු 0;
}

static void add_wrapped_shortlog_msg(sa_nracanaa strbuf *sb, const var_na *s,
				     const sa_nracanaa shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

void shortlog_output(sa_nracanaa shortlog *log)
{
	size_t i, j;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	නම් (log->sort_by_number)
		STABLE_QSORT(log->list.items, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	සඳහා (i = 0; i < log->list.nr; i++) {
		const sa_nracanaa string_list_item *item = &log->list.items[i];
		නම් (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(පූර්ණ_සංඛ්‍යා)UTIL_TO_INT(item), item->string);
		} නැතහොත් {
			sa_nracanaa string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			සඳහා (j = onelines->nr; j >= 1; j--) {
				const var_na *msg = onelines->items[j - 1].string;

				නම් (log->wrap_lines) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_msg(&sb, msg, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				නැතහොත්
					fprintf(log->file, "      %s\n", msg);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			free(onelines);
		}

		log->list.items[i].util = NULL;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
