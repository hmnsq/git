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

static char const * const shortlog_usage[] = {
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

static 정수 compare_by_counter(const void *a1, const void *a2)
{
	const struct string_list_item *i1 = a1, *i2 = a2;
	반환 UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

static 정수 compare_by_list(const void *a1, const void *a2)
{
	const struct string_list_item *i1 = a1, *i2 = a2;
	const struct string_list *l1 = i1->util, *l2 = i2->util;

	만약 (l1->nr < l2->nr)
		반환 1;
	아니면 만약 (l1->nr == l2->nr)
		반환 0;
	아니면
		반환 -1;
}

static void insert_one_record(struct shortlog *log,
			      const char *ident,
			      const char *oneline)
{
	struct string_list_item *item;

	item = string_list_insert(&log->list, ident);

	만약 (log->summary)
		item->util = (void *)(UTIL_TO_INT(item) + 1);
	아니면 {
		char *buffer;
		struct strbuf subject = STRBUF_INIT;
		const char *eol;

		/* Skip any leading whitespace, including any blank lines. */
		동안 (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		만약 (!eol)
			eol = oneline + strlen(oneline);
		만약 (starts_with(oneline, "[PATCH")) {
			const char *eob = strchr(oneline, ']');
			만약 (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		동안 (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, NULL);

		만약 (!item->util) {
			item->util = xmalloc(sizeof(struct string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

static 정수 parse_ident(struct shortlog *log,
		       struct strbuf *out, const char *in)
{
	const char *mailbuf, *namebuf;
	size_t namelen, maillen;
	struct ident_split ident;

	만약 (split_ident_line(&ident, in, strlen(in)))
		반환 -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	만약 (log->email)
		strbuf_addf(out, " <%.*s>", (정수)maillen, mailbuf);

	반환 0;
}

static void read_from_stdin(struct shortlog *log)
{
	struct strbuf ident = STRBUF_INIT;
	struct strbuf mapped_ident = STRBUF_INIT;
	struct strbuf oneline = STRBUF_INIT;
	static const char *author_match[2] = { "Author: ", "author " };
	static const char *committer_match[2] = { "Commit: ", "committer " };
	const char **match;

	만약 (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	switch (log->groups) {
	case SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		break;
	case SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		break;
	case SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	case SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	default:
		BUG("unhandled shortlog group");
	}

	동안 (strbuf_getline_lf(&ident, stdin) != EOF) {
		const char *v;
		만약 (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			continue;
		동안 (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		동안 (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		만약 (parse_ident(log, &mapped_ident, v) < 0)
			continue;

		insert_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

static void insert_records_from_trailers(struct shortlog *log,
					 struct strset *dups,
					 struct commit *commit,
					 struct pretty_print_context *ctx,
					 const char *oneline)
{
	struct trailer_iterator iter;
	const char *commit_buffer, *body;
	struct strbuf ident = STRBUF_INIT;

	만약 (!log->trailers.nr)
		반환;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, NULL,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	만약 (!body)
		goto out;

	trailer_iterator_init(&iter, body);
	동안 (trailer_iterator_advance(&iter)) {
		const char *value = iter.val.buf;

		만약 (!string_list_has_string(&log->trailers, iter.key.buf))
			continue;

		strbuf_reset(&ident);
		만약 (!parse_ident(log, &ident, value))
			value = ident.buf;

		만약 (!strset_add(dups, value))
			continue;
		insert_one_record(log, value, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

static 정수 shortlog_needs_dedup(const struct shortlog *log)
{
	반환 HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

static void insert_records_from_format(struct shortlog *log,
				       struct strset *dups,
				       struct commit *commit,
				       struct pretty_print_context *ctx,
				       const char *oneline)
{
	struct strbuf buf = STRBUF_INIT;
	struct string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		만약 (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			insert_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

void shortlog_add_commit(struct shortlog *log, struct commit *commit)
{
	struct strbuf oneline = STRBUF_INIT;
	struct strset dups = STRSET_INIT;
	struct pretty_print_context ctx = {0};
	const char *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	만약 (!log->summary) {
		만약 (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		아니면
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	insert_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	insert_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

static void get_from_rev(struct rev_info *rev, struct shortlog *log)
{
	struct commit *commit;

	만약 (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	동안 ((commit = get_revision(rev)) != NULL)
		shortlog_add_commit(log, commit);
}

static 정수 parse_uint(char const **arg, 정수 comma, 정수 defval)
{
	unsigned long ul;
	정수 ret;
	char *endp;

	ul = strtoul(*arg, &endp, 10);
	만약 (*endp && *endp != comma)
		반환 -1;
	만약 (ul > INT_MAX)
		반환 -1;
	ret = *arg == endp ? defval : (정수)ul;
	*arg = *endp ? endp + 1 : endp;
	반환 ret;
}

static const char wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

static 정수 parse_wrap_args(const struct option *opt, const char *arg, 정수 unset)
{
	struct shortlog *log = opt->value;

	log->wrap_lines = !unset;
	만약 (unset)
		반환 0;
	만약 (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		반환 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	만약 (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		반환 error(wrap_arg_usage);
	만약 (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		반환 error(wrap_arg_usage);
	반환 0;
}

static 정수 parse_group_option(const struct option *opt, const char *arg, 정수 unset)
{
	struct shortlog *log = opt->value;
	const char *field;

	만약 (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} 아니면 만약 (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	아니면 만약 (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	아니면 만약 (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} 아니면 만약 (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} 아니면 만약 (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} 아니면 {
		반환 error(_("unknown group type: %s"), arg);
	}

	반환 0;
}


void shortlog_init(struct shortlog *log)
{
	memset(log, 0, sizeof(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

void shortlog_finish_setup(struct shortlog *log)
{
	만약 (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	만약 (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

정수 cmd_shortlog(정수 argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo UNUSED)
{
	struct shortlog log = { STRING_LIST_INIT_NODUP };
	struct rev_info rev;
	정수 nongit = !startup_info->have_repository;

	const struct option options[] = {
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

	struct parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	만약 (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, NULL);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	위해 (;;) {
		switch (parse_options_step(&ctx, options, shortlog_usage)) {
		case PARSE_OPT_NON_OPTION:
		case PARSE_OPT_UNKNOWN:
			break;
		case PARSE_OPT_HELP:
			exit(0);
		case PARSE_OPT_HELP_ERROR:
		case PARSE_OPT_ERROR:
		case PARSE_OPT_SUBCOMMAND:
			exit(129);
		case PARSE_OPT_COMPLETE:
			exit(0);
		case PARSE_OPT_DONE:
			goto parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	만약 (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	만약 (!nongit && setup_revisions(argc, argv, &rev, NULL) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	만약 (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	만약 (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	만약 (rev.pending.nr == 0) {
		만약 (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	아니면
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	반환 0;
}

static void add_wrapped_shortlog_msg(struct strbuf *sb, const char *s,
				     const struct shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

void shortlog_output(struct shortlog *log)
{
	size_t i, j;
	struct strbuf sb = STRBUF_INIT;

	만약 (log->sort_by_number)
		STABLE_QSORT(log->list.items, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	위해 (i = 0; i < log->list.nr; i++) {
		const struct string_list_item *item = &log->list.items[i];
		만약 (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(정수)UTIL_TO_INT(item), item->string);
		} 아니면 {
			struct string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			위해 (j = onelines->nr; j >= 1; j--) {
				const char *msg = onelines->items[j - 1].string;

				만약 (log->wrap_lines) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_msg(&sb, msg, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				아니면
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
