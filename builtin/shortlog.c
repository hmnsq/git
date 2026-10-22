#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "mailmap.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "shortlog.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "trailer.h"
#অন্তর্ভাব "strmap.h"

স্থির অক্ষর ধ্রুবক * ধ্রুবক shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ফাঁকা
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

স্থির পূর্ণসংখ্যা compare_by_গণক(ধ্রুবক শূন্য *a1, ধ্রুবক শূন্য *a2)
{
	ধ্রুবক কাঠামো string_list_item *i1 = a1, *i2 = a2;
	ফেরত UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

স্থির পূর্ণসংখ্যা compare_by_list(ধ্রুবক শূন্য *a1, ধ্রুবক শূন্য *a2)
{
	ধ্রুবক কাঠামো string_list_item *i1 = a1, *i2 = a2;
	ধ্রুবক কাঠামো string_list *l1 = i1->util, *l2 = i2->util;

	যদি (l1->nr < l2->nr)
		ফেরত 1;
	অন্যথা যদি (l1->nr == l2->nr)
		ফেরত 0;
	অন্যথা
		ফেরত -1;
}

স্থির শূন্য ঢোকাও_one_record(কাঠামো shortlog *log,
			      ধ্রুবক অক্ষর *ident,
			      ধ্রুবক অক্ষর *oneline)
{
	কাঠামো string_list_item *item;

	item = string_list_ঢোকাও(&log->list, ident);

	যদি (log->summary)
		item->util = (শূন্য *)(UTIL_TO_INT(item) + 1);
	অন্যথা {
		অক্ষর *বাফার;
		কাঠামো strbuf subject = STRBUF_INIT;
		ধ্রুবক অক্ষর *eol;

		/* Skip any leading whitespace, including any blank lines. */
		যখন (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		যদি (!eol)
			eol = oneline + strlen(oneline);
		যদি (starts_with(oneline, "[PATCH")) {
			ধ্রুবক অক্ষর *eob = strchr(oneline, ']');
			যদি (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		যখন (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		বাফার = strbuf_detach(&subject, ফাঁকা);

		যদি (!item->util) {
			item->util = xmalloc(আকার(কাঠামো string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, বাফার);
	}
}

স্থির পূর্ণসংখ্যা parse_ident(কাঠামো shortlog *log,
		       কাঠামো strbuf *out, ধ্রুবক অক্ষর *in)
{
	ধ্রুবক অক্ষর *mailbuf, *namebuf;
	আকার_t namelen, maillen;
	কাঠামো ident_split ident;

	যদি (split_ident_সারি(&ident, in, strlen(in)))
		ফেরত -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	যদি (log->email)
		strbuf_addf(out, " <%.*s>", (পূর্ণসংখ্যা)maillen, mailbuf);

	ফেরত 0;
}

স্থির শূন্য read_from_stdin(কাঠামো shortlog *log)
{
	কাঠামো strbuf ident = STRBUF_INIT;
	কাঠামো strbuf mapped_ident = STRBUF_INIT;
	কাঠামো strbuf oneline = STRBUF_INIT;
	স্থির ধ্রুবক অক্ষর *author_match[2] = { "Author: ", "author " };
	স্থির ধ্রুবক অক্ষর *committer_match[2] = { "Commit: ", "committer " };
	ধ্রুবক অক্ষর **match;

	যদি (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	নির্বাচন (log->groups) {
	ক্ষেত্র SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		বিরতি;
	ক্ষেত্র SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		বিরতি;
	ক্ষেত্র SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	ক্ষেত্র SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	পূর্বনির্ধারিত:
		BUG("unhandled shortlog group");
	}

	যখন (strbuf_getline_lf(&ident, stdin) != EOF) {
		ধ্রুবক অক্ষর *v;
		যদি (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			চালু;
		যখন (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		যখন (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		যদি (parse_ident(log, &mapped_ident, v) < 0)
			চালু;

		ঢোকাও_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

স্থির শূন্য ঢোকাও_records_from_trailers(কাঠামো shortlog *log,
					 কাঠামো strset *dups,
					 কাঠামো commit *commit,
					 কাঠামো pretty_print_context *ctx,
					 ধ্রুবক অক্ষর *oneline)
{
	কাঠামো trailer_iterator iter;
	ধ্রুবক অক্ষর *commit_বাফার, *body;
	কাঠামো strbuf ident = STRBUF_INIT;

	যদি (!log->trailers.nr)
		ফেরত;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_বাফার = repo_logmsg_reencode(the_repository, commit, ফাঁকা,
					     ctx->output_encoding);
	body = strstr(commit_বাফার, "\n\n");
	যদি (!body)
		যাও out;

	trailer_iterator_init(&iter, body);
	যখন (trailer_iterator_advance(&iter)) {
		ধ্রুবক অক্ষর *মান = iter.val.buf;

		যদি (!string_list_has_string(&log->trailers, iter.চাবি.buf))
			চালু;

		strbuf_reset(&ident);
		যদি (!parse_ident(log, &ident, মান))
			মান = ident.buf;

		যদি (!strset_add(dups, মান))
			চালু;
		ঢোকাও_one_record(log, মান, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_বাফার(the_repository, commit, commit_বাফার);
}

স্থির পূর্ণসংখ্যা shortlog_needs_dedup(ধ্রুবক কাঠামো shortlog *log)
{
	ফেরত HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

স্থির শূন্য ঢোকাও_records_from_format(কাঠামো shortlog *log,
				       কাঠামো strset *dups,
				       কাঠামো commit *commit,
				       কাঠামো pretty_print_context *ctx,
				       ধ্রুবক অক্ষর *oneline)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		যদি (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			ঢোকাও_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

শূন্য shortlog_add_commit(কাঠামো shortlog *log, কাঠামো commit *commit)
{
	কাঠামো strbuf oneline = STRBUF_INIT;
	কাঠামো strset dups = STRSET_INIT;
	কাঠামো pretty_print_context ctx = {0};
	ধ্রুবক অক্ষর *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	যদি (!log->summary) {
		যদি (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		অন্যথা
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ঢোকাও_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	ঢোকাও_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

স্থির শূন্য get_from_rev(কাঠামো rev_info *rev, কাঠামো shortlog *log)
{
	কাঠামো commit *commit;

	যদি (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	যখন ((commit = get_revision(rev)) != ফাঁকা)
		shortlog_add_commit(log, commit);
}

স্থির পূর্ণসংখ্যা parse_uint(অক্ষর ধ্রুবক **arg, পূর্ণসংখ্যা comma, পূর্ণসংখ্যা defval)
{
	unsigned long ul;
	পূর্ণসংখ্যা ret;
	অক্ষর *endp;

	ul = strtoul(*arg, &endp, 10);
	যদি (*endp && *endp != comma)
		ফেরত -1;
	যদি (ul > INT_MAX)
		ফেরত -1;
	ret = *arg == endp ? defval : (পূর্ণসংখ্যা)ul;
	*arg = *endp ? endp + 1 : endp;
	ফেরত ret;
}

স্থির ধ্রুবক অক্ষর wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

স্থির পূর্ণসংখ্যা parse_wrap_args(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো shortlog *log = opt->মান;

	log->wrap_সারিগুলি = !unset;
	যদি (unset)
		ফেরত 0;
	যদি (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		ফেরত 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	যদি (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		ফেরত error(wrap_arg_usage);
	যদি (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		ফেরত error(wrap_arg_usage);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_group_option(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো shortlog *log = opt->মান;
	ধ্রুবক অক্ষর *field;

	যদি (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} অন্যথা যদি (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	অন্যথা যদি (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	অন্যথা যদি (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} অন্যথা যদি (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} অন্যথা যদি (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} অন্যথা {
		ফেরত error(_("unknown group type: %s"), arg);
	}

	ফেরত 0;
}


শূন্য shortlog_init(কাঠামো shortlog *log)
{
	memset(log, 0, আকার(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

শূন্য shortlog_finish_setup(কাঠামো shortlog *log)
{
	যদি (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	যদি (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

পূর্ণসংখ্যা cmd_shortlog(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *prefix,
		 কাঠামো repository *repo UNUSED)
{
	কাঠামো shortlog log = { STRING_LIST_INIT_NODUP };
	কাঠামো rev_info rev;
	পূর্ণসংখ্যা nongit = !startup_info->have_repository;

	ধ্রুবক কাঠামো option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_সংখ্যা,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ফাঁকা, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	কাঠামো parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	যদি (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ফাঁকা);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	জন্য (;;) {
		নির্বাচন (parse_options_step(&ctx, options, shortlog_usage)) {
		ক্ষেত্র PARSE_OPT_NON_OPTION:
		ক্ষেত্র PARSE_OPT_UNKNOWN:
			বিরতি;
		ক্ষেত্র PARSE_OPT_HELP:
			exit(0);
		ক্ষেত্র PARSE_OPT_HELP_ERROR:
		ক্ষেত্র PARSE_OPT_ERROR:
		ক্ষেত্র PARSE_OPT_SUBCOMMAND:
			exit(129);
		ক্ষেত্র PARSE_OPT_COMPLETE:
			exit(0);
		ক্ষেত্র PARSE_OPT_DONE:
			যাও parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	যদি (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	যদি (!nongit && setup_revisions(argc, argv, &rev, ফাঁকা) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	যদি (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	যদি (!nongit && !rev.pending.nr && isatty(0))
		add_শীর্ষ_to_pending(&rev);
	যদি (rev.pending.nr == 0) {
		যদি (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	অন্যথা
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	ফেরত 0;
}

স্থির শূন্য add_wrapped_shortlog_বার্তা(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *s,
				     ধ্রুবক কাঠামো shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

শূন্য shortlog_output(কাঠামো shortlog *log)
{
	আকার_t i, j;
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (log->sort_by_সংখ্যা)
		STABLE_QSORT(log->list.বস্তুসমূহ, log->list.nr,
		      log->summary ? compare_by_গণক : compare_by_list);
	জন্য (i = 0; i < log->list.nr; i++) {
		ধ্রুবক কাঠামো string_list_item *item = &log->list.বস্তুসমূহ[i];
		যদি (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(পূর্ণসংখ্যা)UTIL_TO_INT(item), item->string);
		} অন্যথা {
			কাঠামো string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			জন্য (j = onelines->nr; j >= 1; j--) {
				ধ্রুবক অক্ষর *বার্তা = onelines->বস্তুসমূহ[j - 1].string;

				যদি (log->wrap_সারিগুলি) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_বার্তা(&sb, বার্তা, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				অন্যথা
					fprintf(log->file, "      %s\n", বার্তা);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			মুক্ত(onelines);
		}

		log->list.বস্তুসমূহ[i].util = ফাঁকা;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
