#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "shortlog.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"
#ಅನ್ತರ್ಭಾವ "strmap.h"

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ರಿಕ್ತ
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_by_counter(ಧ್ರುವ ಶೂನ್ಯ *a1, ಧ್ರುವ ಶೂನ್ಯ *a2)
{
	ಧ್ರುವ ಸಂರಚನಾ string_list_item *i1 = a1, *i2 = a2;
	ಪ್ರತ್ಯಾಗಮ UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_by_list(ಧ್ರುವ ಶೂನ್ಯ *a1, ಧ್ರುವ ಶೂನ್ಯ *a2)
{
	ಧ್ರುವ ಸಂರಚನಾ string_list_item *i1 = a1, *i2 = a2;
	ಧ್ರುವ ಸಂರಚನಾ string_list *l1 = i1->util, *l2 = i2->util;

	ಯದಿ (l1->nr < l2->nr)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಅನ್ಯಥಾ ಯದಿ (l1->nr == l2->nr)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_one_record(ಸಂರಚನಾ shortlog *log,
			      ಧ್ರುವ ವರ್ಣ *ident,
			      ಧ್ರುವ ವರ್ಣ *oneline)
{
	ಸಂರಚನಾ string_list_item *item;

	item = string_list_ಸೇರಿಸು(&log->list, ident);

	ಯದಿ (log->summary)
		item->util = (ಶೂನ್ಯ *)(UTIL_TO_INT(item) + 1);
	ಅನ್ಯಥಾ {
		ವರ್ಣ *buffer;
		ಸಂರಚನಾ strbuf subject = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *eol;

		/* Skip any leading whitespace, including any blank lines. */
		ಯಾವತ್ (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		ಯದಿ (!eol)
			eol = oneline + strlen(oneline);
		ಯದಿ (starts_with(oneline, "[PATCH")) {
			ಧ್ರುವ ವರ್ಣ *eob = strchr(oneline, ']');
			ಯದಿ (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		ಯಾವತ್ (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, ರಿಕ್ತ);

		ಯದಿ (!item->util) {
			item->util = xmalloc(ಪರಿಮಾಣ(ಸಂರಚನಾ string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_ident(ಸಂರಚನಾ shortlog *log,
		       ಸಂರಚನಾ strbuf *out, ಧ್ರುವ ವರ್ಣ *in)
{
	ಧ್ರುವ ವರ್ಣ *mailbuf, *namebuf;
	size_t namelen, maillen;
	ಸಂರಚನಾ ident_split ident;

	ಯದಿ (split_ident_ಸಾಲು(&ident, in, strlen(in)))
		ಪ್ರತ್ಯಾಗಮ -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	ಯದಿ (log->email)
		strbuf_addf(out, " <%.*s>", (ಪೂರ್ಣಾಙ್ಕ)maillen, mailbuf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_from_stdin(ಸಂರಚನಾ shortlog *log)
{
	ಸಂರಚನಾ strbuf ident = STRBUF_INIT;
	ಸಂರಚನಾ strbuf mapped_ident = STRBUF_INIT;
	ಸಂರಚನಾ strbuf oneline = STRBUF_INIT;
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *author_match[2] = { "Author: ", "author " };
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *committer_match[2] = { "Commit: ", "committer " };
	ಧ್ರುವ ವರ್ಣ **match;

	ಯದಿ (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	ವಿಕಲ್ಪ (log->groups) {
	ಪಕ್ಷ SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		ಭಙ್ಗ;
	ಪಕ್ಷ SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		ಭಙ್ಗ;
	ಪಕ್ಷ SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	ಪಕ್ಷ SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	ಸ್ವಭಾವ:
		BUG("unhandled shortlog group");
	}

	ಯಾವತ್ (strbuf_getline_lf(&ident, stdin) != EOF) {
		ಧ್ರುವ ವರ್ಣ *v;
		ಯದಿ (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			ಅನುವರ್ತ;
		ಯಾವತ್ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		ಯಾವತ್ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		ಯದಿ (parse_ident(log, &mapped_ident, v) < 0)
			ಅನುವರ್ತ;

		ಸೇರಿಸು_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_records_from_trailers(ಸಂರಚನಾ shortlog *log,
					 ಸಂರಚನಾ strset *dups,
					 ಸಂರಚನಾ commit *commit,
					 ಸಂರಚನಾ pretty_print_context *ctx,
					 ಧ್ರುವ ವರ್ಣ *oneline)
{
	ಸಂರಚನಾ trailer_iterator iter;
	ಧ್ರುವ ವರ್ಣ *commit_buffer, *body;
	ಸಂರಚನಾ strbuf ident = STRBUF_INIT;

	ಯದಿ (!log->trailers.nr)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, ರಿಕ್ತ,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	ಯದಿ (!body)
		ಗಚ್ಛ out;

	trailer_iterator_init(&iter, body);
	ಯಾವತ್ (trailer_iterator_advance(&iter)) {
		ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ = iter.val.buf;

		ಯದಿ (!string_list_has_string(&log->trailers, iter.ಕೀಲಿ.buf))
			ಅನುವರ್ತ;

		strbuf_reset(&ident);
		ಯದಿ (!parse_ident(log, &ident, ಮೌಲ್ಯ))
			ಮೌಲ್ಯ = ident.buf;

		ಯದಿ (!strset_add(dups, ಮೌಲ್ಯ))
			ಅನುವರ್ತ;
		ಸೇರಿಸು_one_record(log, ಮೌಲ್ಯ, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ shortlog_needs_dedup(ಧ್ರುವ ಸಂರಚನಾ shortlog *log)
{
	ಪ್ರತ್ಯಾಗಮ HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_records_from_format(ಸಂರಚನಾ shortlog *log,
				       ಸಂರಚನಾ strset *dups,
				       ಸಂರಚನಾ commit *commit,
				       ಸಂರಚನಾ pretty_print_context *ctx,
				       ಧ್ರುವ ವರ್ಣ *oneline)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		ಯದಿ (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			ಸೇರಿಸು_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

ಶೂನ್ಯ shortlog_add_commit(ಸಂರಚನಾ shortlog *log, ಸಂರಚನಾ commit *commit)
{
	ಸಂರಚನಾ strbuf oneline = STRBUF_INIT;
	ಸಂರಚನಾ strset dups = STRSET_INIT;
	ಸಂರಚನಾ pretty_print_context ctx = {0};
	ಧ್ರುವ ವರ್ಣ *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	ಯದಿ (!log->summary) {
		ಯದಿ (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		ಅನ್ಯಥಾ
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ಸೇರಿಸು_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	ಸೇರಿಸು_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ಸ್ಥಿರ ಶೂನ್ಯ get_from_rev(ಸಂರಚನಾ rev_info *rev, ಸಂರಚನಾ shortlog *log)
{
	ಸಂರಚನಾ commit *commit;

	ಯದಿ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	ಯಾವತ್ ((commit = get_revision(rev)) != ರಿಕ್ತ)
		shortlog_add_commit(log, commit);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_uint(ವರ್ಣ ಧ್ರುವ **arg, ಪೂರ್ಣಾಙ್ಕ comma, ಪೂರ್ಣಾಙ್ಕ defval)
{
	unsigned long ul;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ವರ್ಣ *endp;

	ul = strtoul(*arg, &endp, 10);
	ಯದಿ (*endp && *endp != comma)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (ul > INT_MAX)
		ಪ್ರತ್ಯಾಗಮ -1;
	ret = *arg == endp ? defval : (ಪೂರ್ಣಾಙ್ಕ)ul;
	*arg = *endp ? endp + 1 : endp;
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_wrap_args(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ shortlog *log = opt->ಮೌಲ್ಯ;

	log->wrap_ಸಾಲುಗಳು = !unset;
	ಯದಿ (unset)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	ಯದಿ (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		ಪ್ರತ್ಯಾಗಮ error(wrap_arg_usage);
	ಯದಿ (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		ಪ್ರತ್ಯಾಗಮ error(wrap_arg_usage);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_group_option(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ shortlog *log = opt->ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *field;

	ಯದಿ (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	ಅನ್ಯಥಾ ಯದಿ (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	ಅನ್ಯಥಾ ಯದಿ (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} ಅನ್ಯಥಾ ಯದಿ (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ error(_("unknown group type: %s"), arg);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}


ಶೂನ್ಯ shortlog_init(ಸಂರಚನಾ shortlog *log)
{
	memset(log, 0, ಪರಿಮಾಣ(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

ಶೂನ್ಯ shortlog_finish_setup(ಸಂರಚನಾ shortlog *log)
{
	ಯದಿ (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	ಯದಿ (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

ಪೂರ್ಣಾಙ್ಕ cmd_shortlog(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ shortlog log = { STRING_LIST_INIT_NODUP };
	ಸಂರಚನಾ rev_info rev;
	ಪೂರ್ಣಾಙ್ಕ nongit = !startup_info->have_repository;

	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_ಸಂಖ್ಯೆ,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ರಿಕ್ತ, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	ಸಂರಚನಾ parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	ಯದಿ (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	ಪ್ರತಿ (;;) {
		ವಿಕಲ್ಪ (parse_options_step(&ctx, options, shortlog_usage)) {
		ಪಕ್ಷ PARSE_OPT_NON_OPTION:
		ಪಕ್ಷ PARSE_OPT_UNKNOWN:
			ಭಙ್ಗ;
		ಪಕ್ಷ PARSE_OPT_HELP:
			exit(0);
		ಪಕ್ಷ PARSE_OPT_HELP_ERROR:
		ಪಕ್ಷ PARSE_OPT_ERROR:
		ಪಕ್ಷ PARSE_OPT_SUBCOMMAND:
			exit(129);
		ಪಕ್ಷ PARSE_OPT_COMPLETE:
			exit(0);
		ಪಕ್ಷ PARSE_OPT_DONE:
			ಗಚ್ಛ parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	ಯದಿ (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	ಯದಿ (!nongit && setup_revisions(argc, argv, &rev, ರಿಕ್ತ) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	ಯದಿ (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	ಯದಿ (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	ಯದಿ (rev.pending.nr == 0) {
		ಯದಿ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	ಅನ್ಯಥಾ
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ add_wrapped_shortlog_ಸಂದೇಶ(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *s,
				     ಧ್ರುವ ಸಂರಚನಾ shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

ಶೂನ್ಯ shortlog_output(ಸಂರಚನಾ shortlog *log)
{
	size_t i, j;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (log->sort_by_ಸಂಖ್ಯೆ)
		STABLE_QSORT(log->list.ವಸ್ತುಗಳು, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	ಪ್ರತಿ (i = 0; i < log->list.nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ string_list_item *item = &log->list.ವಸ್ತುಗಳು[i];
		ಯದಿ (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(ಪೂರ್ಣಾಙ್ಕ)UTIL_TO_INT(item), item->string);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			ಪ್ರತಿ (j = onelines->nr; j >= 1; j--) {
				ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = onelines->ವಸ್ತುಗಳು[j - 1].string;

				ಯದಿ (log->wrap_ಸಾಲುಗಳು) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_ಸಂದೇಶ(&sb, ಸಂದೇಶ, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				ಅನ್ಯಥಾ
					fprintf(log->file, "      %s\n", ಸಂದೇಶ);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			ಮುಕ್ತ(onelines);
		}

		log->list.ವಸ್ತುಗಳು[i].util = ರಿಕ್ತ;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
