#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "string-list.h"
#ضمن "revision.h"
#ضمن "utf8.h"
#ضمن "mailmap.h"
#ضمن "setup.h"
#ضمن "shortlog.h"
#ضمن "parse-options.h"
#ضمن "trailer.h"
#ضمن "strmap.h"

ساكن حرف ثابت * ثابت shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	عدم
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

ساكن صحيح compare_by_counter(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_item *i1 = a1, *i2 = a2;
	ܦܢܐ UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

ساكن صحيح compare_by_list(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_item *i1 = a1, *i2 = a2;
	ثابت بنية string_list *l1 = i1->util, *l2 = i2->util;

	ܐܢ (l1->nr < l2->nr)
		ܦܢܐ 1;
	ܐܚܪܢܐ ܐܢ (l1->nr == l2->nr)
		ܦܢܐ 0;
	ܐܚܪܢܐ
		ܦܢܐ -1;
}

ساكن فراغ insert_one_record(بنية shortlog *log,
			      ثابت حرف *ident,
			      ثابت حرف *oneline)
{
	بنية string_list_item *item;

	item = string_list_insert(&log->list, ident);

	ܐܢ (log->summary)
		item->util = (فراغ *)(UTIL_TO_INT(item) + 1);
	ܐܚܪܢܐ {
		حرف *buffer;
		بنية strbuf subject = STRBUF_INIT;
		ثابت حرف *eol;

		/* Skip any leading whitespace, including any blank lines. */
		ܟܡܐ (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		ܐܢ (!eol)
			eol = oneline + strlen(oneline);
		ܐܢ (starts_with(oneline, "[PATCH")) {
			ثابت حرف *eob = strchr(oneline, ']');
			ܐܢ (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		ܟܡܐ (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, عدم);

		ܐܢ (!item->util) {
			item->util = xmalloc(حجم(بنية string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

ساكن صحيح parse_ident(بنية shortlog *log,
		       بنية strbuf *out, ثابت حرف *in)
{
	ثابت حرف *mailbuf, *namebuf;
	size_t namelen, maillen;
	بنية ident_split ident;

	ܐܢ (split_ident_line(&ident, in, strlen(in)))
		ܦܢܐ -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	ܐܢ (log->email)
		strbuf_addf(out, " <%.*s>", (صحيح)maillen, mailbuf);

	ܦܢܐ 0;
}

ساكن فراغ read_from_stdin(بنية shortlog *log)
{
	بنية strbuf ident = STRBUF_INIT;
	بنية strbuf mapped_ident = STRBUF_INIT;
	بنية strbuf oneline = STRBUF_INIT;
	ساكن ثابت حرف *author_match[2] = { "Author: ", "author " };
	ساكن ثابت حرف *committer_match[2] = { "Commit: ", "committer " };
	ثابت حرف **match;

	ܐܢ (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	بدل (log->groups) {
	حالة SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		اكسر;
	حالة SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		اكسر;
	حالة SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالة SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	افتراضي:
		BUG("unhandled shortlog group");
	}

	ܟܡܐ (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت حرف *v;
		ܐܢ (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			استمر;
		ܟܡܐ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		ܟܡܐ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		ܐܢ (parse_ident(log, &mapped_ident, v) < 0)
			استمر;

		insert_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ساكن فراغ insert_records_from_trailers(بنية shortlog *log,
					 بنية strset *dups,
					 بنية commit *commit,
					 بنية pretty_print_context *ctx,
					 ثابت حرف *oneline)
{
	بنية trailer_iterator iter;
	ثابت حرف *commit_buffer, *body;
	بنية strbuf ident = STRBUF_INIT;

	ܐܢ (!log->trailers.nr)
		ܦܢܐ;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, عدم,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	ܐܢ (!body)
		اذهب out;

	trailer_iterator_init(&iter, body);
	ܟܡܐ (trailer_iterator_advance(&iter)) {
		ثابت حرف *value = iter.val.buf;

		ܐܢ (!string_list_has_string(&log->trailers, iter.key.buf))
			استمر;

		strbuf_reset(&ident);
		ܐܢ (!parse_ident(log, &ident, value))
			value = ident.buf;

		ܐܢ (!strset_add(dups, value))
			استمر;
		insert_one_record(log, value, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

ساكن صحيح shortlog_needs_dedup(ثابت بنية shortlog *log)
{
	ܦܢܐ HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

ساكن فراغ insert_records_from_format(بنية shortlog *log,
				       بنية strset *dups,
				       بنية commit *commit,
				       بنية pretty_print_context *ctx,
				       ثابت حرف *oneline)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		ܐܢ (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			insert_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

فراغ shortlog_add_commit(بنية shortlog *log, بنية commit *commit)
{
	بنية strbuf oneline = STRBUF_INIT;
	بنية strset dups = STRSET_INIT;
	بنية pretty_print_context ctx = {0};
	ثابت حرف *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	ܐܢ (!log->summary) {
		ܐܢ (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		ܐܚܪܢܐ
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	insert_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	insert_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ساكن فراغ get_from_rev(بنية rev_info *rev, بنية shortlog *log)
{
	بنية commit *commit;

	ܐܢ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	ܟܡܐ ((commit = get_revision(rev)) != عدم)
		shortlog_add_commit(log, commit);
}

ساكن صحيح parse_uint(حرف ثابت **arg, صحيح comma, صحيح defval)
{
	unsigned long ul;
	صحيح ret;
	حرف *endp;

	ul = strtoul(*arg, &endp, 10);
	ܐܢ (*endp && *endp != comma)
		ܦܢܐ -1;
	ܐܢ (ul > INT_MAX)
		ܦܢܐ -1;
	ret = *arg == endp ? defval : (صحيح)ul;
	*arg = *endp ? endp + 1 : endp;
	ܦܢܐ ret;
}

ساكن ثابت حرف wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

ساكن صحيح parse_wrap_args(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *log = opt->value;

	log->wrap_lines = !unset;
	ܐܢ (unset)
		ܦܢܐ 0;
	ܐܢ (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		ܦܢܐ 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	ܐܢ (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		ܦܢܐ error(wrap_arg_usage);
	ܐܢ (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		ܦܢܐ error(wrap_arg_usage);
	ܦܢܐ 0;
}

ساكن صحيح parse_group_option(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *log = opt->value;
	ثابت حرف *field;

	ܐܢ (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} ܐܚܪܢܐ ܐܢ (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	ܐܚܪܢܐ ܐܢ (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	ܐܚܪܢܐ ܐܢ (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} ܐܚܪܢܐ ܐܢ (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} ܐܚܪܢܐ ܐܢ (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} ܐܚܪܢܐ {
		ܦܢܐ error(_("unknown group type: %s"), arg);
	}

	ܦܢܐ 0;
}


فراغ shortlog_init(بنية shortlog *log)
{
	memset(log, 0, حجم(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

فراغ shortlog_finish_setup(بنية shortlog *log)
{
	ܐܢ (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	ܐܢ (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

صحيح cmd_shortlog(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	بنية shortlog log = { STRING_LIST_INIT_NODUP };
	بنية rev_info rev;
	صحيح nongit = !startup_info->have_repository;

	ثابت بنية option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_number,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', عدم, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	بنية parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	ܐܢ (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, عدم);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	لكل (;;) {
		بدل (parse_options_step(&ctx, options, shortlog_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			exit(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			exit(129);
		حالة PARSE_OPT_COMPLETE:
			exit(0);
		حالة PARSE_OPT_DONE:
			اذهب parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	ܐܢ (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	ܐܢ (!nongit && setup_revisions(argc, argv, &rev, عدم) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	ܐܢ (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	ܐܢ (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	ܐܢ (rev.pending.nr == 0) {
		ܐܢ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	ܐܚܪܢܐ
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	ܦܢܐ 0;
}

ساكن فراغ add_wrapped_shortlog_msg(بنية strbuf *sb, ثابت حرف *s,
				     ثابت بنية shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

فراغ shortlog_output(بنية shortlog *log)
{
	size_t i, j;
	بنية strbuf sb = STRBUF_INIT;

	ܐܢ (log->sort_by_number)
		STABLE_QSORT(log->list.items, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	لكل (i = 0; i < log->list.nr; i++) {
		ثابت بنية string_list_item *item = &log->list.items[i];
		ܐܢ (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(صحيح)UTIL_TO_INT(item), item->string);
		} ܐܚܪܢܐ {
			بنية string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			لكل (j = onelines->nr; j >= 1; j--) {
				ثابت حرف *msg = onelines->items[j - 1].string;

				ܐܢ (log->wrap_lines) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_msg(&sb, msg, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				ܐܚܪܢܐ
					fprintf(log->file, "      %s\n", msg);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			حرر(onelines);
		}

		log->list.items[i].util = عدم;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
