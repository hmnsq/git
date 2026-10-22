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

স্থির বর্ণ ধ্রুব * ধ্রুব shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	রিক্ত
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

স্থির পূর্ণাঙ্ক compare_by_counter(ধ্রুব শূন্য *a1, ধ্রুব শূন্য *a2)
{
	ধ্রুব সংরচনা string_list_item *i1 = a1, *i2 = a2;
	প্রত্যাগম UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

স্থির পূর্ণাঙ্ক compare_by_list(ধ্রুব শূন্য *a1, ধ্রুব শূন্য *a2)
{
	ধ্রুব সংরচনা string_list_item *i1 = a1, *i2 = a2;
	ধ্রুব সংরচনা string_list *l1 = i1->util, *l2 = i2->util;

	যদি (l1->nr < l2->nr)
		প্রত্যাগম 1;
	অন্যথা যদি (l1->nr == l2->nr)
		প্রত্যাগম 0;
	অন্যথা
		প্রত্যাগম -1;
}

স্থির শূন্য ঢোকাও_one_record(সংরচনা shortlog *log,
			      ধ্রুব বর্ণ *ident,
			      ধ্রুব বর্ণ *oneline)
{
	সংরচনা string_list_item *item;

	item = string_list_ঢোকাও(&log->list, ident);

	যদি (log->summary)
		item->util = (শূন্য *)(UTIL_TO_INT(item) + 1);
	অন্যথা {
		বর্ণ *buffer;
		সংরচনা strbuf subject = STRBUF_INIT;
		ধ্রুব বর্ণ *eol;

		/* Skip any leading whitespace, including any blank lines. */
		যাবত্ (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		যদি (!eol)
			eol = oneline + strlen(oneline);
		যদি (starts_with(oneline, "[PATCH")) {
			ধ্রুব বর্ণ *eob = strchr(oneline, ']');
			যদি (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		যাবত্ (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, রিক্ত);

		যদি (!item->util) {
			item->util = xmalloc(পরিমাণ(সংরচনা string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

স্থির পূর্ণাঙ্ক parse_ident(সংরচনা shortlog *log,
		       সংরচনা strbuf *out, ধ্রুব বর্ণ *in)
{
	ধ্রুব বর্ণ *mailbuf, *namebuf;
	size_t namelen, maillen;
	সংরচনা ident_split ident;

	যদি (split_ident_সারি(&ident, in, strlen(in)))
		প্রত্যাগম -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	যদি (log->email)
		strbuf_addf(out, " <%.*s>", (পূর্ণাঙ্ক)maillen, mailbuf);

	প্রত্যাগম 0;
}

স্থির শূন্য read_from_stdin(সংরচনা shortlog *log)
{
	সংরচনা strbuf ident = STRBUF_INIT;
	সংরচনা strbuf mapped_ident = STRBUF_INIT;
	সংরচনা strbuf oneline = STRBUF_INIT;
	স্থির ধ্রুব বর্ণ *author_match[2] = { "Author: ", "author " };
	স্থির ধ্রুব বর্ণ *committer_match[2] = { "Commit: ", "committer " };
	ধ্রুব বর্ণ **match;

	যদি (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	বিকল্প (log->groups) {
	পক্ষ SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		ভঙ্গ;
	পক্ষ SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		ভঙ্গ;
	পক্ষ SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	পক্ষ SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	স্বভাব:
		BUG("unhandled shortlog group");
	}

	যাবত্ (strbuf_getline_lf(&ident, stdin) != EOF) {
		ধ্রুব বর্ণ *v;
		যদি (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			অনুবর্ত;
		যাবত্ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		যাবত্ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		যদি (parse_ident(log, &mapped_ident, v) < 0)
			অনুবর্ত;

		ঢোকাও_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

স্থির শূন্য ঢোকাও_records_from_trailers(সংরচনা shortlog *log,
					 সংরচনা strset *dups,
					 সংরচনা commit *commit,
					 সংরচনা pretty_print_context *ctx,
					 ধ্রুব বর্ণ *oneline)
{
	সংরচনা trailer_iterator iter;
	ধ্রুব বর্ণ *commit_buffer, *body;
	সংরচনা strbuf ident = STRBUF_INIT;

	যদি (!log->trailers.nr)
		প্রত্যাগম;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, রিক্ত,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	যদি (!body)
		গচ্ছ out;

	trailer_iterator_init(&iter, body);
	যাবত্ (trailer_iterator_advance(&iter)) {
		ধ্রুব বর্ণ *মান = iter.val.buf;

		যদি (!string_list_has_string(&log->trailers, iter.চাবি.buf))
			অনুবর্ত;

		strbuf_reset(&ident);
		যদি (!parse_ident(log, &ident, মান))
			মান = ident.buf;

		যদি (!strset_add(dups, মান))
			অনুবর্ত;
		ঢোকাও_one_record(log, মান, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

স্থির পূর্ণাঙ্ক shortlog_needs_dedup(ধ্রুব সংরচনা shortlog *log)
{
	প্রত্যাগম HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

স্থির শূন্য ঢোকাও_records_from_format(সংরচনা shortlog *log,
				       সংরচনা strset *dups,
				       সংরচনা commit *commit,
				       সংরচনা pretty_print_context *ctx,
				       ধ্রুব বর্ণ *oneline)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		যদি (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			ঢোকাও_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

শূন্য shortlog_add_commit(সংরচনা shortlog *log, সংরচনা commit *commit)
{
	সংরচনা strbuf oneline = STRBUF_INIT;
	সংরচনা strset dups = STRSET_INIT;
	সংরচনা pretty_print_context ctx = {0};
	ধ্রুব বর্ণ *oneline_str;

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

স্থির শূন্য get_from_rev(সংরচনা rev_info *rev, সংরচনা shortlog *log)
{
	সংরচনা commit *commit;

	যদি (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	যাবত্ ((commit = get_revision(rev)) != রিক্ত)
		shortlog_add_commit(log, commit);
}

স্থির পূর্ণাঙ্ক parse_uint(বর্ণ ধ্রুব **arg, পূর্ণাঙ্ক comma, পূর্ণাঙ্ক defval)
{
	unsigned long ul;
	পূর্ণাঙ্ক ret;
	বর্ণ *endp;

	ul = strtoul(*arg, &endp, 10);
	যদি (*endp && *endp != comma)
		প্রত্যাগম -1;
	যদি (ul > INT_MAX)
		প্রত্যাগম -1;
	ret = *arg == endp ? defval : (পূর্ণাঙ্ক)ul;
	*arg = *endp ? endp + 1 : endp;
	প্রত্যাগম ret;
}

স্থির ধ্রুব বর্ণ wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

স্থির পূর্ণাঙ্ক parse_wrap_args(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা shortlog *log = opt->মান;

	log->wrap_সারিগুলি = !unset;
	যদি (unset)
		প্রত্যাগম 0;
	যদি (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		প্রত্যাগম 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	যদি (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		প্রত্যাগম error(wrap_arg_usage);
	যদি (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		প্রত্যাগম error(wrap_arg_usage);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_group_option(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা shortlog *log = opt->মান;
	ধ্রুব বর্ণ *field;

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
		প্রত্যাগম error(_("unknown group type: %s"), arg);
	}

	প্রত্যাগম 0;
}


শূন্য shortlog_init(সংরচনা shortlog *log)
{
	memset(log, 0, পরিমাণ(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

শূন্য shortlog_finish_setup(সংরচনা shortlog *log)
{
	যদি (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	যদি (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

পূর্ণাঙ্ক cmd_shortlog(পূর্ণাঙ্ক argc,
		 ধ্রুব বর্ণ **argv,
		 ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	সংরচনা shortlog log = { STRING_LIST_INIT_NODUP };
	সংরচনা rev_info rev;
	পূর্ণাঙ্ক nongit = !startup_info->have_repository;

	ধ্রুব সংরচনা option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_সংখ্যা,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', রিক্ত, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	সংরচনা parse_opt_ctx_t ctx;

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

	repo_config(the_repository, git_default_config, রিক্ত);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	প্রতি (;;) {
		বিকল্প (parse_options_step(&ctx, options, shortlog_usage)) {
		পক্ষ PARSE_OPT_NON_OPTION:
		পক্ষ PARSE_OPT_UNKNOWN:
			ভঙ্গ;
		পক্ষ PARSE_OPT_HELP:
			exit(0);
		পক্ষ PARSE_OPT_HELP_ERROR:
		পক্ষ PARSE_OPT_ERROR:
		পক্ষ PARSE_OPT_SUBCOMMAND:
			exit(129);
		পক্ষ PARSE_OPT_COMPLETE:
			exit(0);
		পক্ষ PARSE_OPT_DONE:
			গচ্ছ parse_done;
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

	যদি (!nongit && setup_revisions(argc, argv, &rev, রিক্ত) != 1) {
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
		add_head_to_pending(&rev);
	যদি (rev.pending.nr == 0) {
		যদি (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	অন্যথা
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	প্রত্যাগম 0;
}

স্থির শূন্য add_wrapped_shortlog_বার্তা(সংরচনা strbuf *sb, ধ্রুব বর্ণ *s,
				     ধ্রুব সংরচনা shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

শূন্য shortlog_output(সংরচনা shortlog *log)
{
	size_t i, j;
	সংরচনা strbuf sb = STRBUF_INIT;

	যদি (log->sort_by_সংখ্যা)
		STABLE_QSORT(log->list.বস্তুসমূহ, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	প্রতি (i = 0; i < log->list.nr; i++) {
		ধ্রুব সংরচনা string_list_item *item = &log->list.বস্তুসমূহ[i];
		যদি (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(পূর্ণাঙ্ক)UTIL_TO_INT(item), item->string);
		} অন্যথা {
			সংরচনা string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			প্রতি (j = onelines->nr; j >= 1; j--) {
				ধ্রুব বর্ণ *বার্তা = onelines->বস্তুসমূহ[j - 1].string;

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

		log->list.বস্তুসমূহ[i].util = রিক্ত;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
