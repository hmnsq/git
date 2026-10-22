#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساکن حرف ثابت * ثابت shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	تہی
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
#تعریف_کرو UTIL_مقصد_INT(x) ((intptr_t)(x)->util)

ساکن صحیح compare_by_شمار(ثابت خالی *a1, ثابت خالی *a2)
{
	ثابت ساخت string_list_شے *i1 = a1, *i2 = a2;
	واپس UTIL_مقصد_INT(i2) - UTIL_مقصد_INT(i1);
}

ساکن صحیح compare_by_list(ثابت خالی *a1, ثابت خالی *a2)
{
	ثابت ساخت string_list_شے *i1 = a1, *i2 = a2;
	ثابت ساخت string_list *l1 = i1->util, *l2 = i2->util;

	اگر (l1->nr < l2->nr)
		واپس 1;
	ورنہ اگر (l1->nr == l2->nr)
		واپس 0;
	ورنہ
		واپس -1;
}

ساکن خالی داخل_کرو_one_ریکارڈ(ساخت shortlog *لوگارتھم,
			      ثابت حرف *ident,
			      ثابت حرف *oneline)
{
	ساخت string_list_شے *شے;

	شے = string_list_داخل_کرو(&لوگارتھم->list, ident);

	اگر (لوگارتھم->summary)
		شے->util = (خالی *)(UTIL_مقصد_INT(شے) + 1);
	ورنہ {
		حرف *بفر;
		ساخت strbuf subject = STRBUF_INIT;
		ثابت حرف *eol;

		/* Skip any leading whitespace, including any blank lines. */
		جب (*oneline && خلا_ہے(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		اگر (!eol)
			eol = oneline + لڑی_لمبائی(oneline);
		اگر (starts_with(oneline, "[PATCH")) {
			ثابت حرف *eob = strchr(oneline, ']');
			اگر (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		جب (*oneline && خلا_ہے(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		بفر = strbuf_detach(&subject, تہی);

		اگر (!شے->util) {
			شے->util = xmalloc(حجم(ساخت string_list));
			string_list_init_nodup(شے->util);
		}
		string_list_append(شے->util, بفر);
	}
}

ساکن صحیح parse_ident(ساخت shortlog *لوگارتھم,
		       ساخت strbuf *برآمد, ثابت حرف *اندر)
{
	ثابت حرف *mailbuf, *namebuf;
	سائز_t namelen, maillen;
	ساخت ident_بانٹو ident;

	اگر (بانٹو_ident_سطر(&ident, اندر, لڑی_لمبائی(اندر)))
		واپس -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_انتہا - ident.name_begin;
	maillen = ident.mail_انتہا - ident.mail_begin;

	map_user(&لوگارتھم->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_جوڑو(برآمد, namebuf, namelen);
	اگر (لوگارتھم->email)
		strbuf_addf(برآمد, " <%.*s>", (صحیح)maillen, mailbuf);

	واپس 0;
}

ساکن خالی read_from_stdin(ساخت shortlog *لوگارتھم)
{
	ساخت strbuf ident = STRBUF_INIT;
	ساخت strbuf mapped_ident = STRBUF_INIT;
	ساخت strbuf oneline = STRBUF_INIT;
	ساکن ثابت حرف *author_مطابقت[2] = { "Author: ", "author " };
	ساکن ثابت حرف *committer_مطابقت[2] = { "Commit: ", "committer " };
	ثابت حرف **مطابقت;

	اگر (HAS_MULTI_بٹ(لوگارتھم->groups))
		die(_("using multiple --group options with stdin is not supported"));

	انتخاب (لوگارتھم->groups) {
	حالت SHORTLOG_گروہ_AUTHOR:
		مطابقت = author_مطابقت;
		توڑو;
	حالت SHORTLOG_گروہ_COMMITTER:
		مطابقت = committer_مطابقت;
		توڑو;
	حالت SHORTLOG_گروہ_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالت SHORTLOG_گروہ_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	طےشدہ:
		BUG("unhandled shortlog group");
	}

	جب (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت حرف *v;
		اگر (!skip_سابقہ(ident.buf, مطابقت[0], &v) &&
		    !skip_سابقہ(ident.buf, مطابقت[1], &v))
			جاری;
		جب (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		جب (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		اگر (parse_ident(لوگارتھم, &mapped_ident, v) < 0)
			جاری;

		داخل_کرو_one_ریکارڈ(لوگارتھم, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ساکن خالی داخل_کرو_اندراجات_from_trailers(ساخت shortlog *لوگارتھم,
					 ساخت strset *dups,
					 ساخت commit *commit,
					 ساخت pretty_print_context *ctx,
					 ثابت حرف *oneline)
{
	ساخت trailer_iterator iter;
	ثابت حرف *commit_بفر, *body;
	ساخت strbuf ident = STRBUF_INIT;

	اگر (!لوگارتھم->trailers.nr)
		واپس;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_بفر = repo_logmsg_reencode(the_repository, commit, تہی,
					     ctx->output_encoding);
	body = لڑی_تلاش(commit_بفر, "\n\n");
	اگر (!body)
		جاؤ برآمد;

	trailer_iterator_init(&iter, body);
	جب (trailer_iterator_advance(&iter)) {
		ثابت حرف *قدر = iter.val.buf;

		اگر (!string_list_has_string(&لوگارتھم->trailers, iter.کلید.buf))
			جاری;

		strbuf_reset(&ident);
		اگر (!parse_ident(لوگارتھم, &ident, قدر))
			قدر = ident.buf;

		اگر (!strset_جوڑو(dups, قدر))
			جاری;
		داخل_کرو_one_ریکارڈ(لوگارتھم, قدر, oneline);
	}
	trailer_iterator_release(&iter);

برآمد:
	strbuf_release(&ident);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);
}

ساکن صحیح shortlog_needs_dedup(ثابت ساخت shortlog *لوگارتھم)
{
	واپس HAS_MULTI_بٹ(لوگارتھم->groups) || لوگارتھم->format.nr > 1 || لوگارتھم->trailers.nr;
}

ساکن خالی داخل_کرو_اندراجات_from_format(ساخت shortlog *لوگارتھم,
				       ساخت strset *dups,
				       ساخت commit *commit,
				       ساخت pretty_print_context *ctx,
				       ثابت حرف *oneline)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت string_list_شے *شے;

	for_each_string_list_شے(شے, &لوگارتھم->format) {
		strbuf_reset(&buf);

		repo_format_commit_پیغام(the_repository, commit,
					   شے->string, &buf, ctx);

		اگر (!shortlog_needs_dedup(لوگارتھم) || strset_جوڑو(dups, buf.buf))
			داخل_کرو_one_ریکارڈ(لوگارتھم, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

خالی shortlog_جوڑو_commit(ساخت shortlog *لوگارتھم, ساخت commit *commit)
{
	ساخت strbuf oneline = STRBUF_INIT;
	ساخت strset dups = STRSET_INIT;
	ساخت pretty_print_context ctx = {0};
	ثابت حرف *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = لوگارتھم->abbrev;
	ctx.date_منوال = لوگارتھم->date_منوال;
	ctx.output_encoding = get_log_output_encoding();

	اگر (!لوگارتھم->summary) {
		اگر (لوگارتھم->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		ورنہ
			repo_format_commit_پیغام(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	داخل_کرو_اندراجات_from_trailers(لوگارتھم, &dups, commit, &ctx, oneline_str);
	داخل_کرو_اندراجات_from_format(لوگارتھم, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ساکن خالی get_from_rev(ساخت rev_info *rev, ساخت shortlog *لوگارتھم)
{
	ساخت commit *commit;

	اگر (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	جب ((commit = get_revision(rev)) != تہی)
		shortlog_جوڑو_commit(لوگارتھم, commit);
}

ساکن صحیح parse_uint(حرف ثابت **arg, صحیح comma, صحیح defval)
{
	غیرعلامتی طویل ul;
	صحیح ret;
	حرف *endp;

	ul = strtoul(*arg, &endp, 10);
	اگر (*endp && *endp != comma)
		واپس -1;
	اگر (ul > زیادہ_سے_زیادہ_عدد)
		واپس -1;
	ret = *arg == endp ? defval : (صحیح)ul;
	*arg = *endp ? endp + 1 : endp;
	واپس ret;
}

ساکن ثابت حرف wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#تعریف_کرو DEFAULT_WRAPLEN 76
#تعریف_کرو DEFAULT_INDENT1 6
#تعریف_کرو DEFAULT_INDENT2 9

ساکن صحیح parse_wrap_دلائل(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت shortlog *لوگارتھم = opt->قدر;

	لوگارتھم->wrap_سطریں = !unset;
	اگر (unset)
		واپس 0;
	اگر (!arg) {
		لوگارتھم->wrap = DEFAULT_WRAPLEN;
		لوگارتھم->in1 = DEFAULT_INDENT1;
		لوگارتھم->in2 = DEFAULT_INDENT2;
		واپس 0;
	}

	لوگارتھم->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	لوگارتھم->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	لوگارتھم->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	اگر (لوگارتھم->wrap < 0 || لوگارتھم->in1 < 0 || لوگارتھم->in2 < 0)
		واپس error(wrap_arg_usage);
	اگر (لوگارتھم->wrap &&
	    ((لوگارتھم->in1 && لوگارتھم->wrap <= لوگارتھم->in1) ||
	     (لوگارتھم->in2 && لوگارتھم->wrap <= لوگارتھم->in2)))
		واپس error(wrap_arg_usage);
	واپس 0;
}

ساکن صحیح parse_گروہ_option(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت shortlog *لوگارتھم = opt->قدر;
	ثابت حرف *میدان;

	اگر (unset) {
		لوگارتھم->groups = 0;
		string_list_clear(&لوگارتھم->trailers, 0);
		string_list_clear(&لوگارتھم->format, 0);
	} ورنہ اگر (!strcasecmp(arg, "author"))
		لوگارتھم->groups |= SHORTLOG_گروہ_AUTHOR;
	ورنہ اگر (!strcasecmp(arg, "committer"))
		لوگارتھم->groups |= SHORTLOG_گروہ_COMMITTER;
	ورنہ اگر (skip_سابقہ(arg, "trailer:", &میدان)) {
		لوگارتھم->groups |= SHORTLOG_گروہ_TRAILER;
		string_list_append(&لوگارتھم->trailers, میدان);
	} ورنہ اگر (skip_سابقہ(arg, "format:", &میدان)) {
		لوگارتھم->groups |= SHORTLOG_گروہ_FORMAT;
		string_list_append(&لوگارتھم->format, میدان);
	} ورنہ اگر (strchr(arg, '%')) {
		لوگارتھم->groups |= SHORTLOG_گروہ_FORMAT;
		string_list_append(&لوگارتھم->format, arg);
	} ورنہ {
		واپس error(_("unknown group type: %s"), arg);
	}

	واپس 0;
}


خالی shortlog_init(ساخت shortlog *لوگارتھم)
{
	memset(لوگارتھم, 0, حجم(*لوگارتھم));

	read_mailmap(the_repository, &لوگارتھم->mailmap);

	لوگارتھم->list.strdup_لڑیاں = 1;
	لوگارتھم->wrap = DEFAULT_WRAPLEN;
	لوگارتھم->in1 = DEFAULT_INDENT1;
	لوگارتھم->in2 = DEFAULT_INDENT2;
	لوگارتھم->trailers.strdup_لڑیاں = 1;
	لوگارتھم->trailers.cmp = strcasecmp;
	لوگارتھم->format.strdup_لڑیاں = 1;
}

خالی shortlog_اختتام_تیاری(ساخت shortlog *لوگارتھم)
{
	اگر (لوگارتھم->groups & SHORTLOG_گروہ_AUTHOR)
		string_list_append(&لوگارتھم->format,
				   لوگارتھم->email ? "%aN <%aE>" : "%aN");
	اگر (لوگارتھم->groups & SHORTLOG_گروہ_COMMITTER)
		string_list_append(&لوگارتھم->format,
				   لوگارتھم->email ? "%cN <%cE>" : "%cN");

	string_list_ترتیب_دو(&لوگارتھم->trailers);
}

صحیح cmd_shortlog(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت shortlog لوگارتھم = { STRING_LIST_INIT_NODUP };
	ساخت rev_info rev;
	صحیح nongit = !startup_info->have_repository;

	ثابت ساخت option options[] = {
		OPT_ثنائی_ہندسہ('c', "committer", &لوگارتھم.groups,
			N_("group by committer rather than author"),
			SHORTLOG_گروہ_COMMITTER),
		OPT_BOOL('n', "numbered", &لوگارتھم.ترتیب_دو_by_نمبر,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &لوگارتھم.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &لوگارتھم.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', تہی, &لوگارتھم, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_دلائل),
		OPT_CALLBACK(0, "group", &لوگارتھم, N_("field"),
			N_("group by field"), parse_گروہ_option),
		OPT_انتہا(),
	};

	ساخت parse_opt_ctx_t ctx;

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
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, تہی);
	shortlog_init(&لوگارتھم);
	repo_init_revisions(the_repository, &rev, سابقہ);
	parse_options_آغاز(&ctx, argc, argv, سابقہ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	برائے (;;) {
		انتخاب (parse_options_قدم(&ctx, options, shortlog_usage)) {
		حالت PARSE_OPT_NON_OPTION:
		حالت PARSE_OPT_UNKNOWN:
			توڑو;
		حالت PARSE_OPT_HELP:
			باہر_نکلو(0);
		حالت PARSE_OPT_HELP_ERROR:
		حالت PARSE_OPT_ERROR:
		حالت PARSE_OPT_SUBCOMMAND:
			باہر_نکلو(129);
		حالت PARSE_OPT_COMPLETE:
			باہر_نکلو(0);
		حالت PARSE_OPT_مکمل:
			جاؤ parse_مکمل;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_مکمل:
	revision_opts_اختتام(&rev);
	argc = parse_options_انتہا(&ctx);

	اگر (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	اگر (!nongit && تیاری_revisions(argc, argv, &rev, تہی) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	لوگارتھم.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	لوگارتھم.abbrev = rev.abbrev;
	لوگارتھم.file = rev.diffopt.file;
	لوگارتھم.date_منوال = rev.date_منوال;

	اگر (!لوگارتھم.groups)
		لوگارتھم.groups = SHORTLOG_گروہ_AUTHOR;
	shortlog_اختتام_تیاری(&لوگارتھم);

	/* assume HEAD if from a tty */
	اگر (!nongit && !rev.pending.nr && isatty(0))
		جوڑو_سر_مقصد_pending(&rev);
	اگر (rev.pending.nr == 0) {
		اگر (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&لوگارتھم);
	}
	ورنہ
		get_from_rev(&rev, &لوگارتھم);

	shortlog_output(&لوگارتھم);
	release_revisions(&rev);
	واپس 0;
}

ساکن خالی جوڑو_wrapped_shortlog_پیغام(ساخت strbuf *sb, ثابت حرف *s,
				     ثابت ساخت shortlog *لوگارتھم)
{
	strbuf_جوڑو_wrapped_متن(sb, s, لوگارتھم->in1, لوگارتھم->in2, لوگارتھم->wrap);
	strbuf_addch(sb, '\n');
}

خالی shortlog_output(ساخت shortlog *لوگارتھم)
{
	سائز_t i, j;
	ساخت strbuf sb = STRBUF_INIT;

	اگر (لوگارتھم->ترتیب_دو_by_نمبر)
		STABLE_QSORT(لوگارتھم->list.اشیاء, لوگارتھم->list.nr,
		      لوگارتھم->summary ? compare_by_شمار : compare_by_list);
	برائے (i = 0; i < لوگارتھم->list.nr; i++) {
		ثابت ساخت string_list_شے *شے = &لوگارتھم->list.اشیاء[i];
		اگر (لوگارتھم->summary) {
			fprintf(لوگارتھم->file, "%6d\t%s\n",
				(صحیح)UTIL_مقصد_INT(شے), شے->string);
		} ورنہ {
			ساخت string_list *onelines = شے->util;
			fprintf(لوگارتھم->file, "%s (%"PRIuMAX"):\n",
				شے->string, (uintmax_t)onelines->nr);
			برائے (j = onelines->nr; j >= 1; j--) {
				ثابت حرف *پیغام = onelines->اشیاء[j - 1].string;

				اگر (لوگارتھم->wrap_سطریں) {
					strbuf_reset(&sb);
					جوڑو_wrapped_shortlog_پیغام(&sb, پیغام, لوگارتھم);
					fwrite(sb.buf, sb.len, 1, لوگارتھم->file);
				}
				ورنہ
					fprintf(لوگارتھم->file, "      %s\n", پیغام);
			}
			putc('\n', لوگارتھم->file);
			onelines->strdup_لڑیاں = 1;
			string_list_clear(onelines, 0);
			آزاد(onelines);
		}

		لوگارتھم->list.اشیاء[i].util = تہی;
	}

	strbuf_release(&sb);
	لوگارتھم->list.strdup_لڑیاں = 1;
	string_list_clear(&لوگارتھم->list, 1);
	clear_mailmap(&لوگارتھم->mailmap);
	string_list_clear(&لوگارتھم->format, 0);
	string_list_clear(&لوگارتھم->trailers, 0);
}
