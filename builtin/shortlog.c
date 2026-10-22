#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "mailmap.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "shortlog.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "trailer.h"
#ଅନ୍ତର୍ଭାଵ "strmap.h"

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ଖାଲି
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_by_counter(ଅଚଳ ଶୂନ୍ୟ *a1, ଅଚଳ ଶୂନ୍ୟ *a2)
{
	ଅଚଳ ସଂରଚନା string_list_item *i1 = a1, *i2 = a2;
	ଫେରନ୍ତୁ UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_by_list(ଅଚଳ ଶୂନ୍ୟ *a1, ଅଚଳ ଶୂନ୍ୟ *a2)
{
	ଅଚଳ ସଂରଚନା string_list_item *i1 = a1, *i2 = a2;
	ଅଚଳ ସଂରଚନା string_list *l1 = i1->util, *l2 = i2->util;

	ଯଦି (l1->nr < l2->nr)
		ଫେରନ୍ତୁ 1;
	ଅନ୍ୟଥା ଯଦି (l1->nr == l2->nr)
		ଫେରନ୍ତୁ 0;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_one_record(ସଂରଚନା shortlog *log,
			      ଅଚଳ ଅକ୍ଷର *ident,
			      ଅଚଳ ଅକ୍ଷର *oneline)
{
	ସଂରଚନା string_list_item *item;

	item = string_list_ଭର୍ତ୍ତି(&log->list, ident);

	ଯଦି (log->summary)
		item->util = (ଶୂନ୍ୟ *)(UTIL_TO_INT(item) + 1);
	ଅନ୍ୟଥା {
		ଅକ୍ଷର *buffer;
		ସଂରଚନା strbuf subject = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *eol;

		/* Skip any leading whitespace, including any blank lines. */
		ଯେତେବେଳେ (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		ଯଦି (!eol)
			eol = oneline + strlen(oneline);
		ଯଦି (starts_with(oneline, "[PATCH")) {
			ଅଚଳ ଅକ୍ଷର *eob = strchr(oneline, ']');
			ଯଦି (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		ଯେତେବେଳେ (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, ଖାଲି);

		ଯଦି (!item->util) {
			item->util = xmalloc(ଆକାର(ସଂରଚନା string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_ident(ସଂରଚନା shortlog *log,
		       ସଂରଚନା strbuf *out, ଅଚଳ ଅକ୍ଷର *in)
{
	ଅଚଳ ଅକ୍ଷର *mailbuf, *namebuf;
	size_t namelen, maillen;
	ସଂରଚନା ident_split ident;

	ଯଦି (split_ident_ଧାଡ଼ି(&ident, in, strlen(in)))
		ଫେରନ୍ତୁ -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	ଯଦି (log->email)
		strbuf_addf(out, " <%.*s>", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)maillen, mailbuf);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ read_from_stdin(ସଂରଚନା shortlog *log)
{
	ସଂରଚନା strbuf ident = STRBUF_INIT;
	ସଂରଚନା strbuf mapped_ident = STRBUF_INIT;
	ସଂରଚନା strbuf oneline = STRBUF_INIT;
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *author_match[2] = { "Author: ", "author " };
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *committer_match[2] = { "Commit: ", "committer " };
	ଅଚଳ ଅକ୍ଷର **match;

	ଯଦି (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	ବାଛ (log->groups) {
	ସ୍ଥିତି SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	ସ୍ଥିତି SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unhandled shortlog group");
	}

	ଯେତେବେଳେ (strbuf_getline_lf(&ident, stdin) != EOF) {
		ଅଚଳ ଅକ୍ଷର *v;
		ଯଦି (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			ଜାରି;
		ଯେତେବେଳେ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		ଯେତେବେଳେ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		ଯଦି (parse_ident(log, &mapped_ident, v) < 0)
			ଜାରି;

		ଭର୍ତ୍ତି_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_records_from_trailers(ସଂରଚନା shortlog *log,
					 ସଂରଚନା strset *dups,
					 ସଂରଚନା commit *commit,
					 ସଂରଚନା pretty_print_context *ctx,
					 ଅଚଳ ଅକ୍ଷର *oneline)
{
	ସଂରଚନା trailer_iterator iter;
	ଅଚଳ ଅକ୍ଷର *commit_buffer, *body;
	ସଂରଚନା strbuf ident = STRBUF_INIT;

	ଯଦି (!log->trailers.nr)
		ଫେରନ୍ତୁ;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, ଖାଲି,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	ଯଦି (!body)
		ଯାଅ out;

	trailer_iterator_init(&iter, body);
	ଯେତେବେଳେ (trailer_iterator_advance(&iter)) {
		ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ = iter.val.buf;

		ଯଦି (!string_list_has_string(&log->trailers, iter.ଚାବି.buf))
			ଜାରି;

		strbuf_reset(&ident);
		ଯଦି (!parse_ident(log, &ident, ମୂଲ୍ୟ))
			ମୂଲ୍ୟ = ident.buf;

		ଯଦି (!strset_add(dups, ମୂଲ୍ୟ))
			ଜାରି;
		ଭର୍ତ୍ତି_one_record(log, ମୂଲ୍ୟ, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା shortlog_needs_dedup(ଅଚଳ ସଂରଚନା shortlog *log)
{
	ଫେରନ୍ତୁ HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

ସ୍ଥିର ଶୂନ୍ୟ ଭର୍ତ୍ତି_records_from_format(ସଂରଚନା shortlog *log,
				       ସଂରଚନା strset *dups,
				       ସଂରଚନା commit *commit,
				       ସଂରଚନା pretty_print_context *ctx,
				       ଅଚଳ ଅକ୍ଷର *oneline)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		ଯଦି (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			ଭର୍ତ୍ତି_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

ଶୂନ୍ୟ shortlog_add_commit(ସଂରଚନା shortlog *log, ସଂରଚନା commit *commit)
{
	ସଂରଚନା strbuf oneline = STRBUF_INIT;
	ସଂରଚନା strset dups = STRSET_INIT;
	ସଂରଚନା pretty_print_context ctx = {0};
	ଅଚଳ ଅକ୍ଷର *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	ଯଦି (!log->summary) {
		ଯଦି (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		ଅନ୍ୟଥା
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ଭର୍ତ୍ତି_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	ଭର୍ତ୍ତି_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ସ୍ଥିର ଶୂନ୍ୟ get_from_rev(ସଂରଚନା rev_info *rev, ସଂରଚନା shortlog *log)
{
	ସଂରଚନା commit *commit;

	ଯଦି (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	ଯେତେବେଳେ ((commit = get_revision(rev)) != ଖାଲି)
		shortlog_add_commit(log, commit);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_uint(ଅକ୍ଷର ଅଚଳ **arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା comma, ପୂର୍ଣ୍ଣସଂଖ୍ୟା defval)
{
	unsigned long ul;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ଅକ୍ଷର *endp;

	ul = strtoul(*arg, &endp, 10);
	ଯଦି (*endp && *endp != comma)
		ଫେରନ୍ତୁ -1;
	ଯଦି (ul > INT_MAX)
		ଫେରନ୍ତୁ -1;
	ret = *arg == endp ? defval : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ul;
	*arg = *endp ? endp + 1 : endp;
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_wrap_args(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା shortlog *log = opt->ମୂଲ୍ୟ;

	log->wrap_ଧାଡ଼ିଗୁଡ଼ିକ = !unset;
	ଯଦି (unset)
		ଫେରନ୍ତୁ 0;
	ଯଦି (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		ଫେରନ୍ତୁ 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	ଯଦି (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		ଫେରନ୍ତୁ error(wrap_arg_usage);
	ଯଦି (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		ଫେରନ୍ତୁ error(wrap_arg_usage);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_group_option(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା shortlog *log = opt->ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *field;

	ଯଦି (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} ଅନ୍ୟଥା ଯଦି (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} ଅନ୍ୟଥା ଯଦି (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} ଅନ୍ୟଥା {
		ଫେରନ୍ତୁ error(_("unknown group type: %s"), arg);
	}

	ଫେରନ୍ତୁ 0;
}


ଶୂନ୍ୟ shortlog_init(ସଂରଚନା shortlog *log)
{
	memset(log, 0, ଆକାର(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

ଶୂନ୍ୟ shortlog_finish_setup(ସଂରଚନା shortlog *log)
{
	ଯଦି (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	ଯଦି (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_shortlog(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା shortlog log = { STRING_LIST_INIT_NODUP };
	ସଂରଚନା rev_info rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nongit = !startup_info->have_repository;

	ଅଚଳ ସଂରଚନା option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_ସଂଖ୍ୟା,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ଖାଲି, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	ସଂରଚନା parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	ଯଦି (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ଖାଲି);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	ପାଇଁ (;;) {
		ବାଛ (parse_options_step(&ctx, options, shortlog_usage)) {
		ସ୍ଥିତି PARSE_OPT_NON_OPTION:
		ସ୍ଥିତି PARSE_OPT_UNKNOWN:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି PARSE_OPT_HELP:
			exit(0);
		ସ୍ଥିତି PARSE_OPT_HELP_ERROR:
		ସ୍ଥିତି PARSE_OPT_ERROR:
		ସ୍ଥିତି PARSE_OPT_SUBCOMMAND:
			exit(129);
		ସ୍ଥିତି PARSE_OPT_COMPLETE:
			exit(0);
		ସ୍ଥିତି PARSE_OPT_DONE:
			ଯାଅ parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	ଯଦି (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	ଯଦି (!nongit && setup_revisions(argc, argv, &rev, ଖାଲି) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	ଯଦି (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	ଯଦି (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	ଯଦି (rev.pending.nr == 0) {
		ଯଦି (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	ଅନ୍ୟଥା
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ add_wrapped_shortlog_ସନ୍ଦେଶ(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *s,
				     ଅଚଳ ସଂରଚନା shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

ଶୂନ୍ୟ shortlog_output(ସଂରଚନା shortlog *log)
{
	size_t i, j;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (log->sort_by_ସଂଖ୍ୟା)
		STABLE_QSORT(log->list.ବସ୍ତୁଗୁଡ଼ିକ, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	ପାଇଁ (i = 0; i < log->list.nr; i++) {
		ଅଚଳ ସଂରଚନା string_list_item *item = &log->list.ବସ୍ତୁଗୁଡ଼ିକ[i];
		ଯଦି (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)UTIL_TO_INT(item), item->string);
		} ଅନ୍ୟଥା {
			ସଂରଚନା string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			ପାଇଁ (j = onelines->nr; j >= 1; j--) {
				ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = onelines->ବସ୍ତୁଗୁଡ଼ିକ[j - 1].string;

				ଯଦି (log->wrap_ଧାଡ଼ିଗୁଡ଼ିକ) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_ସନ୍ଦେଶ(&sb, ସନ୍ଦେଶ, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				ଅନ୍ୟଥା
					fprintf(log->file, "      %s\n", ସନ୍ଦେଶ);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			ମୁକ୍ତ(onelines);
		}

		log->list.ବସ୍ତୁଗୁଡ଼ିକ[i].util = ଖାଲି;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
