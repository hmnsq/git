#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "mailmap.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "shortlog.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "trailer.h"
#அந்தர்பாவ "strmap.h"

நிலையான எழுத்து மாறிலி * மாறிலி shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	வெற்று
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

நிலையான முழு compare_by_counter(மாறிலி வெறுமை *a1, மாறிலி வெறுமை *a2)
{
	மாறிலி கட்டமைப்பு string_list_item *i1 = a1, *i2 = a2;
	திரும்பு UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

நிலையான முழு compare_by_list(மாறிலி வெறுமை *a1, மாறிலி வெறுமை *a2)
{
	மாறிலி கட்டமைப்பு string_list_item *i1 = a1, *i2 = a2;
	மாறிலி கட்டமைப்பு string_list *l1 = i1->util, *l2 = i2->util;

	எனில் (l1->nr < l2->nr)
		திரும்பு 1;
	இல்லையெனில் எனில் (l1->nr == l2->nr)
		திரும்பு 0;
	இல்லையெனில்
		திரும்பு -1;
}

நிலையான வெறுமை செருகு_one_record(கட்டமைப்பு shortlog *log,
			      மாறிலி எழுத்து *ident,
			      மாறிலி எழுத்து *oneline)
{
	கட்டமைப்பு string_list_item *item;

	item = string_list_செருகு(&log->list, ident);

	எனில் (log->summary)
		item->util = (வெறுமை *)(UTIL_TO_INT(item) + 1);
	இல்லையெனில் {
		எழுத்து *buffer;
		கட்டமைப்பு strbuf subject = STRBUF_INIT;
		மாறிலி எழுத்து *eol;

		/* Skip any leading whitespace, including any blank lines. */
		போது (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		எனில் (!eol)
			eol = oneline + strlen(oneline);
		எனில் (starts_with(oneline, "[PATCH")) {
			மாறிலி எழுத்து *eob = strchr(oneline, ']');
			எனில் (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		போது (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, வெற்று);

		எனில் (!item->util) {
			item->util = xmalloc(அளவு(கட்டமைப்பு string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

நிலையான முழு parse_ident(கட்டமைப்பு shortlog *log,
		       கட்டமைப்பு strbuf *out, மாறிலி எழுத்து *in)
{
	மாறிலி எழுத்து *mailbuf, *namebuf;
	size_t namelen, maillen;
	கட்டமைப்பு ident_split ident;

	எனில் (split_ident_வரி(&ident, in, strlen(in)))
		திரும்பு -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	எனில் (log->email)
		strbuf_addf(out, " <%.*s>", (முழு)maillen, mailbuf);

	திரும்பு 0;
}

நிலையான வெறுமை read_from_stdin(கட்டமைப்பு shortlog *log)
{
	கட்டமைப்பு strbuf ident = STRBUF_INIT;
	கட்டமைப்பு strbuf mapped_ident = STRBUF_INIT;
	கட்டமைப்பு strbuf oneline = STRBUF_INIT;
	நிலையான மாறிலி எழுத்து *author_match[2] = { "Author: ", "author " };
	நிலையான மாறிலி எழுத்து *committer_match[2] = { "Commit: ", "committer " };
	மாறிலி எழுத்து **match;

	எனில் (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	தேர்வு (log->groups) {
	நிலை SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		நிறுத்து;
	நிலை SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		நிறுத்து;
	நிலை SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	நிலை SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	இயல்பு:
		BUG("unhandled shortlog group");
	}

	போது (strbuf_getline_lf(&ident, stdin) != EOF) {
		மாறிலி எழுத்து *v;
		எனில் (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			தொடர்;
		போது (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		போது (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		எனில் (parse_ident(log, &mapped_ident, v) < 0)
			தொடர்;

		செருகு_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

நிலையான வெறுமை செருகு_records_from_trailers(கட்டமைப்பு shortlog *log,
					 கட்டமைப்பு strset *dups,
					 கட்டமைப்பு commit *commit,
					 கட்டமைப்பு pretty_print_context *ctx,
					 மாறிலி எழுத்து *oneline)
{
	கட்டமைப்பு trailer_iterator iter;
	மாறிலி எழுத்து *commit_buffer, *body;
	கட்டமைப்பு strbuf ident = STRBUF_INIT;

	எனில் (!log->trailers.nr)
		திரும்பு;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, வெற்று,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	எனில் (!body)
		செல் out;

	trailer_iterator_init(&iter, body);
	போது (trailer_iterator_advance(&iter)) {
		மாறிலி எழுத்து *மதிப்பு = iter.val.buf;

		எனில் (!string_list_has_string(&log->trailers, iter.சாவி.buf))
			தொடர்;

		strbuf_reset(&ident);
		எனில் (!parse_ident(log, &ident, மதிப்பு))
			மதிப்பு = ident.buf;

		எனில் (!strset_add(dups, மதிப்பு))
			தொடர்;
		செருகு_one_record(log, மதிப்பு, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

நிலையான முழு shortlog_needs_dedup(மாறிலி கட்டமைப்பு shortlog *log)
{
	திரும்பு HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

நிலையான வெறுமை செருகு_records_from_format(கட்டமைப்பு shortlog *log,
				       கட்டமைப்பு strset *dups,
				       கட்டமைப்பு commit *commit,
				       கட்டமைப்பு pretty_print_context *ctx,
				       மாறிலி எழுத்து *oneline)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		எனில் (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			செருகு_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

வெறுமை shortlog_add_commit(கட்டமைப்பு shortlog *log, கட்டமைப்பு commit *commit)
{
	கட்டமைப்பு strbuf oneline = STRBUF_INIT;
	கட்டமைப்பு strset dups = STRSET_INIT;
	கட்டமைப்பு pretty_print_context ctx = {0};
	மாறிலி எழுத்து *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	எனில் (!log->summary) {
		எனில் (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		இல்லையெனில்
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	செருகு_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	செருகு_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

நிலையான வெறுமை get_from_rev(கட்டமைப்பு rev_info *rev, கட்டமைப்பு shortlog *log)
{
	கட்டமைப்பு commit *commit;

	எனில் (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	போது ((commit = get_revision(rev)) != வெற்று)
		shortlog_add_commit(log, commit);
}

நிலையான முழு parse_uint(எழுத்து மாறிலி **arg, முழு comma, முழு defval)
{
	unsigned long ul;
	முழு ret;
	எழுத்து *endp;

	ul = strtoul(*arg, &endp, 10);
	எனில் (*endp && *endp != comma)
		திரும்பு -1;
	எனில் (ul > INT_MAX)
		திரும்பு -1;
	ret = *arg == endp ? defval : (முழு)ul;
	*arg = *endp ? endp + 1 : endp;
	திரும்பு ret;
}

நிலையான மாறிலி எழுத்து wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

நிலையான முழு parse_wrap_args(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு shortlog *log = opt->மதிப்பு;

	log->wrap_வரிகள் = !unset;
	எனில் (unset)
		திரும்பு 0;
	எனில் (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		திரும்பு 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	எனில் (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		திரும்பு error(wrap_arg_usage);
	எனில் (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		திரும்பு error(wrap_arg_usage);
	திரும்பு 0;
}

நிலையான முழு parse_group_option(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு shortlog *log = opt->மதிப்பு;
	மாறிலி எழுத்து *field;

	எனில் (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} இல்லையெனில் எனில் (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	இல்லையெனில் எனில் (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	இல்லையெனில் எனில் (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} இல்லையெனில் எனில் (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} இல்லையெனில் எனில் (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} இல்லையெனில் {
		திரும்பு error(_("unknown group type: %s"), arg);
	}

	திரும்பு 0;
}


வெறுமை shortlog_init(கட்டமைப்பு shortlog *log)
{
	memset(log, 0, அளவு(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

வெறுமை shortlog_finish_setup(கட்டமைப்பு shortlog *log)
{
	எனில் (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	எனில் (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

முழு cmd_shortlog(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு shortlog log = { STRING_LIST_INIT_NODUP };
	கட்டமைப்பு rev_info rev;
	முழு nongit = !startup_info->have_repository;

	மாறிலி கட்டமைப்பு option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_எண்,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', வெற்று, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	கட்டமைப்பு parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	எனில் (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, வெற்று);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	க்கு (;;) {
		தேர்வு (parse_options_step(&ctx, options, shortlog_usage)) {
		நிலை PARSE_OPT_NON_OPTION:
		நிலை PARSE_OPT_UNKNOWN:
			நிறுத்து;
		நிலை PARSE_OPT_HELP:
			exit(0);
		நிலை PARSE_OPT_HELP_ERROR:
		நிலை PARSE_OPT_ERROR:
		நிலை PARSE_OPT_SUBCOMMAND:
			exit(129);
		நிலை PARSE_OPT_COMPLETE:
			exit(0);
		நிலை PARSE_OPT_DONE:
			செல் parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	எனில் (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	எனில் (!nongit && setup_revisions(argc, argv, &rev, வெற்று) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	எனில் (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	எனில் (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	எனில் (rev.pending.nr == 0) {
		எனில் (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	இல்லையெனில்
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	திரும்பு 0;
}

நிலையான வெறுமை add_wrapped_shortlog_செய்தி(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *s,
				     மாறிலி கட்டமைப்பு shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

வெறுமை shortlog_output(கட்டமைப்பு shortlog *log)
{
	size_t i, j;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (log->sort_by_எண்)
		STABLE_QSORT(log->list.பொருட்கள், log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	க்கு (i = 0; i < log->list.nr; i++) {
		மாறிலி கட்டமைப்பு string_list_item *item = &log->list.பொருட்கள்[i];
		எனில் (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(முழு)UTIL_TO_INT(item), item->string);
		} இல்லையெனில் {
			கட்டமைப்பு string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			க்கு (j = onelines->nr; j >= 1; j--) {
				மாறிலி எழுத்து *செய்தி = onelines->பொருட்கள்[j - 1].string;

				எனில் (log->wrap_வரிகள்) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_செய்தி(&sb, செய்தி, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				இல்லையெனில்
					fprintf(log->file, "      %s\n", செய்தி);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			விடுவி(onelines);
		}

		log->list.பொருட்கள்[i].util = வெற்று;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
