#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "string-list.h"
#شامل "revision.h"
#شامل "utf8.h"
#شامل "mailmap.h"
#شامل "setup.h"
#شامل "shortlog.h"
#شامل "parse-options.h"
#شامل "trailer.h"
#شامل "strmap.h"

ایستا نویسه ثابت * ثابت shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	پوچ
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

ایستا صحیح compare_by_counter(ثابت تهی *a1, ثابت تهی *a2)
{
	ثابت ساختار string_list_item *i1 = a1, *i2 = a2;
	بازگشت UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

ایستا صحیح compare_by_list(ثابت تهی *a1, ثابت تهی *a2)
{
	ثابت ساختار string_list_item *i1 = a1, *i2 = a2;
	ثابت ساختار string_list *l1 = i1->util, *l2 = i2->util;

	اگر (l1->nr < l2->nr)
		بازگشت 1;
	وگرنه اگر (l1->nr == l2->nr)
		بازگشت 0;
	وگرنه
		بازگشت -1;
}

ایستا تهی درج_one_record(ساختار shortlog *log,
			      ثابت نویسه *ident,
			      ثابت نویسه *oneline)
{
	ساختار string_list_item *item;

	item = string_list_درج(&log->list, ident);

	اگر (log->summary)
		item->util = (تهی *)(UTIL_TO_INT(item) + 1);
	وگرنه {
		نویسه *buffer;
		ساختار strbuf subject = STRBUF_INIT;
		ثابت نویسه *eol;

		/* Skip any leading whitespace, including any blank lines. */
		وقتی (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		اگر (!eol)
			eol = oneline + strlen(oneline);
		اگر (starts_with(oneline, "[PATCH")) {
			ثابت نویسه *eob = strchr(oneline, ']');
			اگر (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		وقتی (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, پوچ);

		اگر (!item->util) {
			item->util = xmalloc(اندازه(ساختار string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

ایستا صحیح parse_ident(ساختار shortlog *log,
		       ساختار strbuf *out, ثابت نویسه *in)
{
	ثابت نویسه *mailbuf, *namebuf;
	size_t namelen, maillen;
	ساختار ident_split ident;

	اگر (split_ident_سطر(&ident, in, strlen(in)))
		بازگشت -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	اگر (log->email)
		strbuf_addf(out, " <%.*s>", (صحیح)maillen, mailbuf);

	بازگشت 0;
}

ایستا تهی read_from_stdin(ساختار shortlog *log)
{
	ساختار strbuf ident = STRBUF_INIT;
	ساختار strbuf mapped_ident = STRBUF_INIT;
	ساختار strbuf oneline = STRBUF_INIT;
	ایستا ثابت نویسه *author_match[2] = { "Author: ", "author " };
	ایستا ثابت نویسه *committer_match[2] = { "Commit: ", "committer " };
	ثابت نویسه **match;

	اگر (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	انتخاب (log->groups) {
	حالت SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		بشکن;
	حالت SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		بشکن;
	حالت SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالت SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	پیشفرض:
		BUG("unhandled shortlog group");
	}

	وقتی (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت نویسه *v;
		اگر (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			ادامه;
		وقتی (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		وقتی (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		اگر (parse_ident(log, &mapped_ident, v) < 0)
			ادامه;

		درج_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ایستا تهی درج_records_from_trailers(ساختار shortlog *log,
					 ساختار strset *dups,
					 ساختار commit *commit,
					 ساختار pretty_print_context *ctx,
					 ثابت نویسه *oneline)
{
	ساختار trailer_iterator iter;
	ثابت نویسه *commit_buffer, *body;
	ساختار strbuf ident = STRBUF_INIT;

	اگر (!log->trailers.nr)
		بازگشت;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, پوچ,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	اگر (!body)
		برو out;

	trailer_iterator_init(&iter, body);
	وقتی (trailer_iterator_advance(&iter)) {
		ثابت نویسه *مقدار = iter.val.buf;

		اگر (!string_list_has_string(&log->trailers, iter.کلید.buf))
			ادامه;

		strbuf_reset(&ident);
		اگر (!parse_ident(log, &ident, مقدار))
			مقدار = ident.buf;

		اگر (!strset_add(dups, مقدار))
			ادامه;
		درج_one_record(log, مقدار, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

ایستا صحیح shortlog_needs_dedup(ثابت ساختار shortlog *log)
{
	بازگشت HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

ایستا تهی درج_records_from_format(ساختار shortlog *log,
				       ساختار strset *dups,
				       ساختار commit *commit,
				       ساختار pretty_print_context *ctx,
				       ثابت نویسه *oneline)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		اگر (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			درج_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

تهی shortlog_add_commit(ساختار shortlog *log, ساختار commit *commit)
{
	ساختار strbuf oneline = STRBUF_INIT;
	ساختار strset dups = STRSET_INIT;
	ساختار pretty_print_context ctx = {0};
	ثابت نویسه *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	اگر (!log->summary) {
		اگر (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		وگرنه
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	درج_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	درج_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ایستا تهی get_from_rev(ساختار rev_info *rev, ساختار shortlog *log)
{
	ساختار commit *commit;

	اگر (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	وقتی ((commit = get_revision(rev)) != پوچ)
		shortlog_add_commit(log, commit);
}

ایستا صحیح parse_uint(نویسه ثابت **arg, صحیح comma, صحیح defval)
{
	unsigned long ul;
	صحیح ret;
	نویسه *endp;

	ul = strtoul(*arg, &endp, 10);
	اگر (*endp && *endp != comma)
		بازگشت -1;
	اگر (ul > INT_MAX)
		بازگشت -1;
	ret = *arg == endp ? defval : (صحیح)ul;
	*arg = *endp ? endp + 1 : endp;
	بازگشت ret;
}

ایستا ثابت نویسه wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

ایستا صحیح parse_wrap_args(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار shortlog *log = opt->مقدار;

	log->wrap_سطرها = !unset;
	اگر (unset)
		بازگشت 0;
	اگر (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		بازگشت 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	اگر (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		بازگشت error(wrap_arg_usage);
	اگر (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		بازگشت error(wrap_arg_usage);
	بازگشت 0;
}

ایستا صحیح parse_group_option(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار shortlog *log = opt->مقدار;
	ثابت نویسه *field;

	اگر (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} وگرنه اگر (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	وگرنه اگر (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	وگرنه اگر (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} وگرنه اگر (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} وگرنه اگر (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} وگرنه {
		بازگشت error(_("unknown group type: %s"), arg);
	}

	بازگشت 0;
}


تهی shortlog_init(ساختار shortlog *log)
{
	memset(log, 0, اندازه(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

تهی shortlog_finish_setup(ساختار shortlog *log)
{
	اگر (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	اگر (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

صحیح cmd_shortlog(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED)
{
	ساختار shortlog log = { STRING_LIST_INIT_NODUP };
	ساختار rev_info rev;
	صحیح nongit = !startup_info->have_repository;

	ثابت ساختار option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_عدد,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', پوچ, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	ساختار parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	اگر (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, پوچ);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	برای (;;) {
		انتخاب (parse_options_step(&ctx, options, shortlog_usage)) {
		حالت PARSE_OPT_NON_OPTION:
		حالت PARSE_OPT_UNKNOWN:
			بشکن;
		حالت PARSE_OPT_HELP:
			exit(0);
		حالت PARSE_OPT_HELP_ERROR:
		حالت PARSE_OPT_ERROR:
		حالت PARSE_OPT_SUBCOMMAND:
			exit(129);
		حالت PARSE_OPT_COMPLETE:
			exit(0);
		حالت PARSE_OPT_DONE:
			برو parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	اگر (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	اگر (!nongit && setup_revisions(argc, argv, &rev, پوچ) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	اگر (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	اگر (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	اگر (rev.pending.nr == 0) {
		اگر (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	وگرنه
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	بازگشت 0;
}

ایستا تهی add_wrapped_shortlog_پیام(ساختار strbuf *sb, ثابت نویسه *s,
				     ثابت ساختار shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

تهی shortlog_output(ساختار shortlog *log)
{
	size_t i, j;
	ساختار strbuf sb = STRBUF_INIT;

	اگر (log->sort_by_عدد)
		STABLE_QSORT(log->list.اقلام, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	برای (i = 0; i < log->list.nr; i++) {
		ثابت ساختار string_list_item *item = &log->list.اقلام[i];
		اگر (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(صحیح)UTIL_TO_INT(item), item->string);
		} وگرنه {
			ساختار string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			برای (j = onelines->nr; j >= 1; j--) {
				ثابت نویسه *پیام = onelines->اقلام[j - 1].string;

				اگر (log->wrap_سطرها) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_پیام(&sb, پیام, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				وگرنه
					fprintf(log->file, "      %s\n", پیام);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			آزاد(onelines);
		}

		log->list.اقلام[i].util = پوچ;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
