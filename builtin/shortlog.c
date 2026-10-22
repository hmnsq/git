#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "mailmap.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "shortlog.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "trailer.h"
#અન્તર્ભાવ "strmap.h"

સ્થિર અક્ષર અચલ * અચલ shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ખાલી
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

સ્થિર પૂર્ણાંક compare_by_counter(અચલ શૂન્ય *a1, અચલ શૂન્ય *a2)
{
	અચલ સંરચના string_list_item *i1 = a1, *i2 = a2;
	પાછા UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

સ્થિર પૂર્ણાંક compare_by_list(અચલ શૂન્ય *a1, અચલ શૂન્ય *a2)
{
	અચલ સંરચના string_list_item *i1 = a1, *i2 = a2;
	અચલ સંરચના string_list *l1 = i1->util, *l2 = i2->util;

	જો (l1->nr < l2->nr)
		પાછા 1;
	નહીંતર જો (l1->nr == l2->nr)
		પાછા 0;
	નહીંતર
		પાછા -1;
}

સ્થિર શૂન્ય ઉમેરો_one_record(સંરચના shortlog *log,
			      અચલ અક્ષર *ident,
			      અચલ અક્ષર *oneline)
{
	સંરચના string_list_item *item;

	item = string_list_ઉમેરો(&log->list, ident);

	જો (log->summary)
		item->util = (શૂન્ય *)(UTIL_TO_INT(item) + 1);
	નહીંતર {
		અક્ષર *buffer;
		સંરચના strbuf subject = STRBUF_INIT;
		અચલ અક્ષર *eol;

		/* Skip any leading whitespace, including any blank lines. */
		જ્યારે (*oneline && isspace(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		જો (!eol)
			eol = oneline + strlen(oneline);
		જો (starts_with(oneline, "[PATCH")) {
			અચલ અક્ષર *eob = strchr(oneline, ']');
			જો (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		જ્યારે (*oneline && isspace(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, ખાલી);

		જો (!item->util) {
			item->util = xmalloc(કદ(સંરચના string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

સ્થિર પૂર્ણાંક parse_ident(સંરચના shortlog *log,
		       સંરચના strbuf *out, અચલ અક્ષર *in)
{
	અચલ અક્ષર *mailbuf, *namebuf;
	size_t namelen, maillen;
	સંરચના ident_split ident;

	જો (split_ident_લીટી(&ident, in, strlen(in)))
		પાછા -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	જો (log->email)
		strbuf_addf(out, " <%.*s>", (પૂર્ણાંક)maillen, mailbuf);

	પાછા 0;
}

સ્થિર શૂન્ય read_from_stdin(સંરચના shortlog *log)
{
	સંરચના strbuf ident = STRBUF_INIT;
	સંરચના strbuf mapped_ident = STRBUF_INIT;
	સંરચના strbuf oneline = STRBUF_INIT;
	સ્થિર અચલ અક્ષર *author_match[2] = { "Author: ", "author " };
	સ્થિર અચલ અક્ષર *committer_match[2] = { "Commit: ", "committer " };
	અચલ અક્ષર **match;

	જો (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	પસંદગી (log->groups) {
	સ્થિતિ SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		તોડો;
	સ્થિતિ SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		તોડો;
	સ્થિતિ SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	સ્થિતિ SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	મૂળભૂત:
		BUG("unhandled shortlog group");
	}

	જ્યારે (strbuf_getline_lf(&ident, stdin) != EOF) {
		અચલ અક્ષર *v;
		જો (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			ચાલુ;
		જ્યારે (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		જ્યારે (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		જો (parse_ident(log, &mapped_ident, v) < 0)
			ચાલુ;

		ઉમેરો_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

સ્થિર શૂન્ય ઉમેરો_records_from_trailers(સંરચના shortlog *log,
					 સંરચના strset *dups,
					 સંરચના commit *commit,
					 સંરચના pretty_print_context *ctx,
					 અચલ અક્ષર *oneline)
{
	સંરચના trailer_iterator iter;
	અચલ અક્ષર *commit_buffer, *body;
	સંરચના strbuf ident = STRBUF_INIT;

	જો (!log->trailers.nr)
		પાછા;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, ખાલી,
					     ctx->output_encoding);
	body = strstr(commit_buffer, "\n\n");
	જો (!body)
		જાઓ out;

	trailer_iterator_init(&iter, body);
	જ્યારે (trailer_iterator_advance(&iter)) {
		અચલ અક્ષર *કિંમત = iter.val.buf;

		જો (!string_list_has_string(&log->trailers, iter.ચાવી.buf))
			ચાલુ;

		strbuf_reset(&ident);
		જો (!parse_ident(log, &ident, કિંમત))
			કિંમત = ident.buf;

		જો (!strset_add(dups, કિંમત))
			ચાલુ;
		ઉમેરો_one_record(log, કિંમત, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

સ્થિર પૂર્ણાંક shortlog_needs_dedup(અચલ સંરચના shortlog *log)
{
	પાછા HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

સ્થિર શૂન્ય ઉમેરો_records_from_format(સંરચના shortlog *log,
				       સંરચના strset *dups,
				       સંરચના commit *commit,
				       સંરચના pretty_print_context *ctx,
				       અચલ અક્ષર *oneline)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		જો (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			ઉમેરો_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

શૂન્ય shortlog_add_commit(સંરચના shortlog *log, સંરચના commit *commit)
{
	સંરચના strbuf oneline = STRBUF_INIT;
	સંરચના strset dups = STRSET_INIT;
	સંરચના pretty_print_context ctx = {0};
	અચલ અક્ષર *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	જો (!log->summary) {
		જો (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		નહીંતર
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ઉમેરો_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	ઉમેરો_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

સ્થિર શૂન્ય get_from_rev(સંરચના rev_info *rev, સંરચના shortlog *log)
{
	સંરચના commit *commit;

	જો (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	જ્યારે ((commit = get_revision(rev)) != ખાલી)
		shortlog_add_commit(log, commit);
}

સ્થિર પૂર્ણાંક parse_uint(અક્ષર અચલ **arg, પૂર્ણાંક comma, પૂર્ણાંક defval)
{
	unsigned long ul;
	પૂર્ણાંક ret;
	અક્ષર *endp;

	ul = strtoul(*arg, &endp, 10);
	જો (*endp && *endp != comma)
		પાછા -1;
	જો (ul > INT_MAX)
		પાછા -1;
	ret = *arg == endp ? defval : (પૂર્ણાંક)ul;
	*arg = *endp ? endp + 1 : endp;
	પાછા ret;
}

સ્થિર અચલ અક્ષર wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

સ્થિર પૂર્ણાંક parse_wrap_args(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના shortlog *log = opt->કિંમત;

	log->wrap_લીટીઓ = !unset;
	જો (unset)
		પાછા 0;
	જો (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		પાછા 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	જો (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		પાછા error(wrap_arg_usage);
	જો (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		પાછા error(wrap_arg_usage);
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_group_option(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના shortlog *log = opt->કિંમત;
	અચલ અક્ષર *field;

	જો (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} નહીંતર જો (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	નહીંતર જો (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	નહીંતર જો (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} નહીંતર જો (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} નહીંતર જો (strchr(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} નહીંતર {
		પાછા error(_("unknown group type: %s"), arg);
	}

	પાછા 0;
}


શૂન્ય shortlog_init(સંરચના shortlog *log)
{
	memset(log, 0, કદ(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

શૂન્ય shortlog_finish_setup(સંરચના shortlog *log)
{
	જો (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	જો (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

પૂર્ણાંક cmd_shortlog(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	સંરચના shortlog log = { STRING_LIST_INIT_NODUP };
	સંરચના rev_info rev;
	પૂર્ણાંક nongit = !startup_info->have_repository;

	અચલ સંરચના option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_સંખ્યા,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ખાલી, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	સંરચના parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	જો (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ખાલી);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	માટે (;;) {
		પસંદગી (parse_options_step(&ctx, options, shortlog_usage)) {
		સ્થિતિ PARSE_OPT_NON_OPTION:
		સ્થિતિ PARSE_OPT_UNKNOWN:
			તોડો;
		સ્થિતિ PARSE_OPT_HELP:
			exit(0);
		સ્થિતિ PARSE_OPT_HELP_ERROR:
		સ્થિતિ PARSE_OPT_ERROR:
		સ્થિતિ PARSE_OPT_SUBCOMMAND:
			exit(129);
		સ્થિતિ PARSE_OPT_COMPLETE:
			exit(0);
		સ્થિતિ PARSE_OPT_DONE:
			જાઓ parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	જો (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	જો (!nongit && setup_revisions(argc, argv, &rev, ખાલી) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	જો (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	જો (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	જો (rev.pending.nr == 0) {
		જો (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	નહીંતર
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	પાછા 0;
}

સ્થિર શૂન્ય add_wrapped_shortlog_સંદેશ(સંરચના strbuf *sb, અચલ અક્ષર *s,
				     અચલ સંરચના shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

શૂન્ય shortlog_output(સંરચના shortlog *log)
{
	size_t i, j;
	સંરચના strbuf sb = STRBUF_INIT;

	જો (log->sort_by_સંખ્યા)
		STABLE_QSORT(log->list.વસ્તુઓ, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	માટે (i = 0; i < log->list.nr; i++) {
		અચલ સંરચના string_list_item *item = &log->list.વસ્તુઓ[i];
		જો (log->summary) {
			fprintf(log->file, "%6d\t%s\n",
				(પૂર્ણાંક)UTIL_TO_INT(item), item->string);
		} નહીંતર {
			સંરચના string_list *onelines = item->util;
			fprintf(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			માટે (j = onelines->nr; j >= 1; j--) {
				અચલ અક્ષર *સંદેશ = onelines->વસ્તુઓ[j - 1].string;

				જો (log->wrap_લીટીઓ) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_સંદેશ(&sb, સંદેશ, log);
					fwrite(sb.buf, sb.len, 1, log->file);
				}
				નહીંતર
					fprintf(log->file, "      %s\n", સંદેશ);
			}
			putc('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			મુક્ત(onelines);
		}

		log->list.વસ્તુઓ[i].util = ખાલી;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
