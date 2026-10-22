#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "mailmap.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "shortlog.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "trailer.h"
#അന്തര്ഭാവ "strmap.h"

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ശൂന്യ
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

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_by_counter(സ്ഥിരം ശൂന്യം *a1, സ്ഥിരം ശൂന്യം *a2)
{
	സ്ഥിരം ഘടന string_list_item *i1 = a1, *i2 = a2;
	തിരികെ UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_by_list(സ്ഥിരം ശൂന്യം *a1, സ്ഥിരം ശൂന്യം *a2)
{
	സ്ഥിരം ഘടന string_list_item *i1 = a1, *i2 = a2;
	സ്ഥിരം ഘടന string_list *l1 = i1->util, *l2 = i2->util;

	എങ്കിൽ (l1->nr < l2->nr)
		തിരികെ 1;
	അല്ലെങ്കിൽ എങ്കിൽ (l1->nr == l2->nr)
		തിരികെ 0;
	അല്ലെങ്കിൽ
		തിരികെ -1;
}

സ്ഥിതം ശൂന്യം തിരുകുക_one_record(ഘടന shortlog *log,
			      സ്ഥിരം അക്ഷരം *ident,
			      സ്ഥിരം അക്ഷരം *oneline)
{
	ഘടന string_list_item *item;

	item = string_list_തിരുകുക(&log->list, ident);

	എങ്കിൽ (log->summary)
		item->util = (ശൂന്യം *)(UTIL_TO_INT(item) + 1);
	അല്ലെങ്കിൽ {
		അക്ഷരം *buffer;
		ഘടന strbuf subject = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *eol;

		/* Skip any leading whitespace, including any blank lines. */
		ആകുമ്പോൾ (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		എങ്കിൽ (!eol)
			eol = oneline + strlen(oneline);
		എങ്കിൽ (starts_with(oneline, "[PATCH")) {
			സ്ഥിരം അക്ഷരം *eob = strchr(oneline, ']');
			എങ്കിൽ (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		ആകുമ്പോൾ (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, ശൂന്യ);

		എങ്കിൽ (!item->util) {
			item->util = xmalloc(വലുപ്പം(ഘടന string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_ident(ഘടന shortlog *log,
		       ഘടന strbuf *out, സ്ഥിരം അക്ഷരം *in)
{
	സ്ഥിരം അക്ഷരം *mailbuf, *namebuf;
	size_t namelen, maillen;
	ഘടന ident_split ident;

	എങ്കിൽ (split_ident_വരി(&ident, in, strlen(in)))
		തിരികെ -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	എങ്കിൽ (log->email)
		strbuf_addf(out, " <%.*s>", (പൂർണ്ണസംഖ്യ)maillen, mailbuf);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം read_from_stdin(ഘടന shortlog *log)
{
	ഘടന strbuf ident = STRBUF_INIT;
	ഘടന strbuf mapped_ident = STRBUF_INIT;
	ഘടന strbuf oneline = STRBUF_INIT;
	സ്ഥിതം സ്ഥിരം അക്ഷരം *author_match[2] = { "Author: ", "author " };
	സ്ഥിതം സ്ഥിരം അക്ഷരം *committer_match[2] = { "Commit: ", "committer " };
	സ്ഥിരം അക്ഷരം **match;

	എങ്കിൽ (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	തിരഞ്ഞെടുപ്പ് (log->groups) {
	അവസ്ഥ SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		നിർത്തുക;
	അവസ്ഥ SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		നിർത്തുക;
	അവസ്ഥ SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	അവസ്ഥ SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	സ്ഥിരസ്ഥിതി:
		BUG("unhandled shortlog group");
	}

	ആകുമ്പോൾ (strbuf_getline_lf(&ident, stdin) != EOF) {
		സ്ഥിരം അക്ഷരം *v;
		എങ്കിൽ (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			തുടരുക;
		ആകുമ്പോൾ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		ആകുമ്പോൾ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		എങ്കിൽ (parse_ident(log, &mapped_ident, v) < 0)
			തുടരുക;

		തിരുകുക_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

സ്ഥിതം ശൂന്യം തിരുകുക_records_from_trailers(ഘടന shortlog *log,
					 ഘടന strset *dups,
					 ഘടന commit *commit,
					 ഘടന pretty_print_context *ctx,
					 സ്ഥിരം അക്ഷരം *oneline)
{
	ഘടന trailer_iterator iter;
	സ്ഥിരം അക്ഷരം *commit_buffer, *body;
	ഘടന strbuf ident = STRBUF_INIT;

	എങ്കിൽ (!log->trailers.nr)
		തിരികെ;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, ശൂന്യ,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	എങ്കിൽ (!body)
		പോകുക out;

	trailer_iterator_init(&iter, body);
	ആകുമ്പോൾ (trailer_iterator_advance(&iter)) {
		സ്ഥിരം അക്ഷരം *മൂല്യം = iter.val.buf;

		എങ്കിൽ (!string_list_has_string(&log->trailers, iter.താക്കോൽ.buf))
			തുടരുക;

		strbuf_reset(&ident);
		എങ്കിൽ (!parse_ident(log, &ident, മൂല്യം))
			മൂല്യം = ident.buf;

		എങ്കിൽ (!strset_add(dups, മൂല്യം))
			തുടരുക;
		തിരുകുക_one_record(log, മൂല്യം, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ shortlog_needs_dedup(സ്ഥിരം ഘടന shortlog *log)
{
	തിരികെ HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

സ്ഥിതം ശൂന്യം തിരുകുക_records_from_format(ഘടന shortlog *log,
				       ഘടന strset *dups,
				       ഘടന commit *commit,
				       ഘടന pretty_print_context *ctx,
				       സ്ഥിരം അക്ഷരം *oneline)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		എങ്കിൽ (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			തിരുകുക_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

ശൂന്യം shortlog_add_commit(ഘടന shortlog *log, ഘടന commit *commit)
{
	ഘടന strbuf oneline = STRBUF_INIT;
	ഘടന strset dups = STRSET_INIT;
	ഘടന pretty_print_context ctx = {0};
	സ്ഥിരം അക്ഷരം *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	എങ്കിൽ (!log->summary) {
		എങ്കിൽ (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		അല്ലെങ്കിൽ
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	തിരുകുക_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	തിരുകുക_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

സ്ഥിതം ശൂന്യം get_from_rev(ഘടന rev_info *rev, ഘടന shortlog *log)
{
	ഘടന commit *commit;

	എങ്കിൽ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	ആകുമ്പോൾ ((commit = get_revision(rev)) != ശൂന്യ)
		shortlog_add_commit(log, commit);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_uint(അക്ഷരം സ്ഥിരം **arg, പൂർണ്ണസംഖ്യ comma, പൂർണ്ണസംഖ്യ defval)
{
	unsigned long ul;
	പൂർണ്ണസംഖ്യ ret;
	അക്ഷരം *endp;

	ul = strtoul(*arg, &endp, 10);
	എങ്കിൽ (*endp && *endp != comma)
		തിരികെ -1;
	എങ്കിൽ (ul > INT_MAX)
		തിരികെ -1;
	ret = *arg == endp ? defval : (പൂർണ്ണസംഖ്യ)ul;
	*arg = *endp ? endp + 1 : endp;
	തിരികെ ret;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_wrap_args(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന shortlog *log = opt->മൂല്യം;

	log->wrap_വരികൾ = !unset;
	എങ്കിൽ (unset)
		തിരികെ 0;
	എങ്കിൽ (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		തിരികെ 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	എങ്കിൽ (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		തിരികെ error(wrap_arg_usage);
	എങ്കിൽ (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		തിരികെ error(wrap_arg_usage);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_group_option(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന shortlog *log = opt->മൂല്യം;
	സ്ഥിരം അക്ഷരം *field;

	എങ്കിൽ (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} അല്ലെങ്കിൽ എങ്കിൽ (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} അല്ലെങ്കിൽ {
		തിരികെ error(_("unknown group type: %s"), arg);
	}

	തിരികെ 0;
}


ശൂന്യം shortlog_init(ഘടന shortlog *log)
{
	memset(log, 0, വലുപ്പം(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

ശൂന്യം shortlog_finish_setup(ഘടന shortlog *log)
{
	എങ്കിൽ (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	എങ്കിൽ (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

പൂർണ്ണസംഖ്യ cmd_shortlog(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന shortlog log = { STRING_LIST_INIT_NODUP };
	ഘടന rev_info rev;
	പൂർണ്ണസംഖ്യ nongit = !startup_info->have_repository;

	സ്ഥിരം ഘടന option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_സംഖ്യ,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ശൂന്യ, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	ഘടന parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	എങ്കിൽ (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ശൂന്യ);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	വേണ്ടി (;;) {
		തിരഞ്ഞെടുപ്പ് (parse_options_step(&ctx, options, shortlog_usage)) {
		അവസ്ഥ PARSE_OPT_NON_OPTION:
		അവസ്ഥ PARSE_OPT_UNKNOWN:
			നിർത്തുക;
		അവസ്ഥ PARSE_OPT_HELP:
			exit(0);
		അവസ്ഥ PARSE_OPT_HELP_ERROR:
		അവസ്ഥ PARSE_OPT_ERROR:
		അവസ്ഥ PARSE_OPT_SUBCOMMAND:
			exit(129);
		അവസ്ഥ PARSE_OPT_COMPLETE:
			exit(0);
		അവസ്ഥ PARSE_OPT_DONE:
			പോകുക parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	എങ്കിൽ (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	എങ്കിൽ (!nongit && setup_revisions(argc, argv, &rev, ശൂന്യ) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	എങ്കിൽ (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	എങ്കിൽ (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	എങ്കിൽ (rev.pending.nr == 0) {
		എങ്കിൽ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	അല്ലെങ്കിൽ
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം add_wrapped_shortlog_സന്ദേശം(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *s,
				     സ്ഥിരം ഘടന shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

ശൂന്യം shortlog_output(ഘടന shortlog *log)
{
	size_t i, j;
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (log->sort_by_സംഖ്യ)
		STABLE_QSORT(log->list.ഇനങ്ങൾ, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	വേണ്ടി (i = 0; i < log->list.nr; i++) {
		സ്ഥിരം ഘടന string_list_item *item = &log->list.ഇനങ്ങൾ[i];
		എങ്കിൽ (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(പൂർണ്ണസംഖ്യ)UTIL_TO_INT(item), item->string);
		} അല്ലെങ്കിൽ {
			ഘടന string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			വേണ്ടി (j = onelines->nr; j >= 1; j--) {
				സ്ഥിരം അക്ഷരം *സന്ദേശം = onelines->ഇനങ്ങൾ[j - 1].string;

				എങ്കിൽ (log->wrap_വരികൾ) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_സന്ദേശം(&sb, സന്ദേശം, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				അല്ലെങ്കിൽ
					fprintf(log->file, "      %s\n", സന്ദേശം);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			സ്വതന്ത്രം(onelines);
		}

		log->list.ഇനങ്ങൾ[i].util = ശൂന്യ;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
