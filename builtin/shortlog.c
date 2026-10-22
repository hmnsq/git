#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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
#تعریف_کرو UTIL_منزل_INT(x) ((intptr_t)(x)->util)

ساكن صحيح compare_by_counter(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_شے *i1 = a1, *i2 = a2;
	أرجع UTIL_منزل_INT(i2) - UTIL_منزل_INT(i1);
}

ساكن صحيح compare_by_list(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_شے *i1 = a1, *i2 = a2;
	ثابت بنية string_list *l1 = i1->util, *l2 = i2->util;

	إذا (l1->nr < l2->nr)
		أرجع 1;
	وإلا إذا (l1->nr == l2->nr)
		أرجع 0;
	وإلا
		أرجع -1;
}

ساكن فراغ داخل_کرو_one_ریکارڈ(بنية shortlog *لوگارتھم,
			      ثابت حرف *ident,
			      ثابت حرف *oneline)
{
	بنية string_list_شے *شے;

	شے = string_list_داخل_کرو(&لوگارتھم->list, ident);

	إذا (لوگارتھم->summary)
		شے->util = (فراغ *)(UTIL_منزل_INT(شے) + 1);
	وإلا {
		حرف *بفر;
		بنية strbuf subject = STRBUF_INIT;
		ثابت حرف *eol;

		/* Skip any leading whitespace, including any blank lines. */
		طالما (*oneline && خلا_اے(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		إذا (!eol)
			eol = oneline + لڑی_لمبائی(oneline);
		إذا (starts_with(oneline, "[PATCH")) {
			ثابت حرف *eob = strchr(oneline, ']');
			إذا (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		طالما (*oneline && خلا_اے(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		بفر = strbuf_detach(&subject, عدم);

		إذا (!شے->util) {
			شے->util = xmalloc(حجم(بنية string_list));
			string_list_init_nodup(شے->util);
		}
		string_list_append(شے->util, بفر);
	}
}

ساكن صحيح parse_ident(بنية shortlog *لوگارتھم,
		       بنية strbuf *باہرلا, ثابت حرف *وچ)
{
	ثابت حرف *mailbuf, *namebuf;
	ناپ_t namelen, maillen;
	بنية ident_ونڈو ident;

	إذا (ونڈو_ident_سطر(&ident, وچ, لڑی_لمبائی(وچ)))
		أرجع -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_سرا - ident.name_begin;
	maillen = ident.mail_سرا - ident.mail_begin;

	map_user(&لوگارتھم->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_جوڑو(باہرلا, namebuf, namelen);
	إذا (لوگارتھم->email)
		strbuf_addf(باہرلا, " <%.*s>", (صحيح)maillen, mailbuf);

	أرجع 0;
}

ساكن فراغ read_from_stdin(بنية shortlog *لوگارتھم)
{
	بنية strbuf ident = STRBUF_INIT;
	بنية strbuf mapped_ident = STRBUF_INIT;
	بنية strbuf oneline = STRBUF_INIT;
	ساكن ثابت حرف *author_میل[2] = { "Author: ", "author " };
	ساكن ثابت حرف *committer_میل[2] = { "Commit: ", "committer " };
	ثابت حرف **میل;

	إذا (HAS_MULTI_بٹ(لوگارتھم->groups))
		die(_("using multiple --group options with stdin is not supported"));

	بدل (لوگارتھم->groups) {
	حالة SHORTLOG_گروہ_AUTHOR:
		میل = author_میل;
		اكسر;
	حالة SHORTLOG_گروہ_COMMITTER:
		میل = committer_میل;
		اكسر;
	حالة SHORTLOG_گروہ_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالة SHORTLOG_گروہ_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	افتراضي:
		BUG("unhandled shortlog group");
	}

	طالما (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت حرف *v;
		إذا (!skip_اگیتر(ident.buf, میل[0], &v) &&
		    !skip_اگیتر(ident.buf, میل[1], &v))
			استمر;
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		إذا (parse_ident(لوگارتھم, &mapped_ident, v) < 0)
			استمر;

		داخل_کرو_one_ریکارڈ(لوگارتھم, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ساكن فراغ داخل_کرو_ریکارڈاں_from_trailers(بنية shortlog *لوگارتھم,
					 بنية strset *dups,
					 بنية commit *commit,
					 بنية pretty_print_context *ctx,
					 ثابت حرف *oneline)
{
	بنية trailer_iterator iter;
	ثابت حرف *commit_بفر, *body;
	بنية strbuf ident = STRBUF_INIT;

	إذا (!لوگارتھم->trailers.nr)
		أرجع;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_بفر = repo_logmsg_reencode(the_repository, commit, عدم,
					     ctx->output_encoding);
	body = لڑی_کھوج(commit_بفر, "\n\n");
	إذا (!body)
		اذهب باہرلا;

	trailer_iterator_init(&iter, body);
	طالما (trailer_iterator_advance(&iter)) {
		ثابت حرف *قدر = iter.val.buf;

		إذا (!string_list_has_string(&لوگارتھم->trailers, iter.کلید.buf))
			استمر;

		strbuf_reset(&ident);
		إذا (!parse_ident(لوگارتھم, &ident, قدر))
			قدر = ident.buf;

		إذا (!strset_جوڑو(dups, قدر))
			استمر;
		داخل_کرو_one_ریکارڈ(لوگارتھم, قدر, oneline);
	}
	trailer_iterator_release(&iter);

باہرلا:
	strbuf_release(&ident);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);
}

ساكن صحيح shortlog_needs_dedup(ثابت بنية shortlog *لوگارتھم)
{
	أرجع HAS_MULTI_بٹ(لوگارتھم->groups) || لوگارتھم->format.nr > 1 || لوگارتھم->trailers.nr;
}

ساكن فراغ داخل_کرو_ریکارڈاں_from_format(بنية shortlog *لوگارتھم,
				       بنية strset *dups,
				       بنية commit *commit,
				       بنية pretty_print_context *ctx,
				       ثابت حرف *oneline)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_شے *شے;

	for_each_string_list_شے(شے, &لوگارتھم->format) {
		strbuf_reset(&buf);

		repo_format_commit_سنیہا(the_repository, commit,
					   شے->string, &buf, ctx);

		إذا (!shortlog_needs_dedup(لوگارتھم) || strset_جوڑو(dups, buf.buf))
			داخل_کرو_one_ریکارڈ(لوگارتھم, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

فراغ shortlog_جوڑو_commit(بنية shortlog *لوگارتھم, بنية commit *commit)
{
	بنية strbuf oneline = STRBUF_INIT;
	بنية strset dups = STRSET_INIT;
	بنية pretty_print_context ctx = {0};
	ثابت حرف *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = لوگارتھم->abbrev;
	ctx.date_منوال = لوگارتھم->date_منوال;
	ctx.output_encoding = get_log_output_encoding();

	إذا (!لوگارتھم->summary) {
		إذا (لوگارتھم->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		وإلا
			repo_format_commit_سنیہا(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	داخل_کرو_ریکارڈاں_from_trailers(لوگارتھم, &dups, commit, &ctx, oneline_str);
	داخل_کرو_ریکارڈاں_from_format(لوگارتھم, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ساكن فراغ get_from_rev(بنية rev_info *rev, بنية shortlog *لوگارتھم)
{
	بنية commit *commit;

	إذا (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	طالما ((commit = get_revision(rev)) != عدم)
		shortlog_جوڑو_commit(لوگارتھم, commit);
}

ساكن صحيح parse_uint(حرف ثابت **arg, صحيح comma, صحيح defval)
{
	بنانشانی لما ul;
	صحيح ret;
	حرف *endp;

	ul = strtoul(*arg, &endp, 10);
	إذا (*endp && *endp != comma)
		أرجع -1;
	إذا (ul > ودھ_توں_ودھ_عدد)
		أرجع -1;
	ret = *arg == endp ? defval : (صحيح)ul;
	*arg = *endp ? endp + 1 : endp;
	أرجع ret;
}

ساكن ثابت حرف wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#تعریف_کرو DEFAULT_WRAPLEN 76
#تعریف_کرو DEFAULT_INDENT1 6
#تعریف_کرو DEFAULT_INDENT2 9

ساكن صحيح parse_wrap_دلیلاں(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لوگارتھم = opt->قدر;

	لوگارتھم->wrap_سطریں = !unset;
	إذا (unset)
		أرجع 0;
	إذا (!arg) {
		لوگارتھم->wrap = DEFAULT_WRAPLEN;
		لوگارتھم->in1 = DEFAULT_INDENT1;
		لوگارتھم->in2 = DEFAULT_INDENT2;
		أرجع 0;
	}

	لوگارتھم->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	لوگارتھم->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	لوگارتھم->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	إذا (لوگارتھم->wrap < 0 || لوگارتھم->in1 < 0 || لوگارتھم->in2 < 0)
		أرجع error(wrap_arg_usage);
	إذا (لوگارتھم->wrap &&
	    ((لوگارتھم->in1 && لوگارتھم->wrap <= لوگارتھم->in1) ||
	     (لوگارتھم->in2 && لوگارتھم->wrap <= لوگارتھم->in2)))
		أرجع error(wrap_arg_usage);
	أرجع 0;
}

ساكن صحيح parse_گروہ_option(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لوگارتھم = opt->قدر;
	ثابت حرف *میدان;

	إذا (unset) {
		لوگارتھم->groups = 0;
		string_list_clear(&لوگارتھم->trailers, 0);
		string_list_clear(&لوگارتھم->format, 0);
	} وإلا إذا (!strcasecmp(arg, "author"))
		لوگارتھم->groups |= SHORTLOG_گروہ_AUTHOR;
	وإلا إذا (!strcasecmp(arg, "committer"))
		لوگارتھم->groups |= SHORTLOG_گروہ_COMMITTER;
	وإلا إذا (skip_اگیتر(arg, "trailer:", &میدان)) {
		لوگارتھم->groups |= SHORTLOG_گروہ_TRAILER;
		string_list_append(&لوگارتھم->trailers, میدان);
	} وإلا إذا (skip_اگیتر(arg, "format:", &میدان)) {
		لوگارتھم->groups |= SHORTLOG_گروہ_FORMAT;
		string_list_append(&لوگارتھم->format, میدان);
	} وإلا إذا (strchr(arg, '%')) {
		لوگارتھم->groups |= SHORTLOG_گروہ_FORMAT;
		string_list_append(&لوگارتھم->format, arg);
	} وإلا {
		أرجع error(_("unknown group type: %s"), arg);
	}

	أرجع 0;
}


فراغ shortlog_init(بنية shortlog *لوگارتھم)
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

فراغ shortlog_انت_تیاری(بنية shortlog *لوگارتھم)
{
	إذا (لوگارتھم->groups & SHORTLOG_گروہ_AUTHOR)
		string_list_append(&لوگارتھم->format,
				   لوگارتھم->email ? "%aN <%aE>" : "%aN");
	إذا (لوگارتھم->groups & SHORTLOG_گروہ_COMMITTER)
		string_list_append(&لوگارتھم->format,
				   لوگارتھم->email ? "%cN <%cE>" : "%cN");

	string_list_ترتیب_دیو(&لوگارتھم->trailers);
}

صحيح cmd_shortlog(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اگیتر,
		 بنية repository *repo بن_ورتیا)
{
	بنية shortlog لوگارتھم = { STRING_LIST_INIT_NODUP };
	بنية rev_info rev;
	صحيح nongit = !startup_info->have_repository;

	ثابت بنية option options[] = {
		OPT_ثنائی_ہندسہ('c', "committer", &لوگارتھم.groups,
			N_("group by committer rather than author"),
			SHORTLOG_گروہ_COMMITTER),
		OPT_BOOL('n', "numbered", &لوگارتھم.ترتیب_دیو_by_نمبر,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &لوگارتھم.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &لوگارتھم.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', عدم, &لوگارتھم, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_دلیلاں),
		OPT_CALLBACK(0, "group", &لوگارتھم, N_("field"),
			N_("group by field"), parse_گروہ_option),
		OPT_سرا(),
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
	إذا (nongit && !the_hash_algo)
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, عدم);
	shortlog_init(&لوگارتھم);
	repo_init_revisions(the_repository, &rev, اگیتر);
	parse_options_شروع(&ctx, argc, argv, اگیتر, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	لكل (;;) {
		بدل (parse_options_قدم(&ctx, options, shortlog_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			باہر_نکلو(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			باہر_نکلو(129);
		حالة PARSE_OPT_COMPLETE:
			باہر_نکلو(0);
		حالة PARSE_OPT_پورا:
			اذهب parse_پورا;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_پورا:
	revision_opts_انت(&rev);
	argc = parse_options_سرا(&ctx);

	إذا (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	إذا (!nongit && تیاری_revisions(argc, argv, &rev, عدم) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	لوگارتھم.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	لوگارتھم.abbrev = rev.abbrev;
	لوگارتھم.file = rev.diffopt.file;
	لوگارتھم.date_منوال = rev.date_منوال;

	إذا (!لوگارتھم.groups)
		لوگارتھم.groups = SHORTLOG_گروہ_AUTHOR;
	shortlog_انت_تیاری(&لوگارتھم);

	/* assume HEAD if from a tty */
	إذا (!nongit && !rev.pending.nr && isatty(0))
		جوڑو_سر_منزل_pending(&rev);
	إذا (rev.pending.nr == 0) {
		إذا (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&لوگارتھم);
	}
	وإلا
		get_from_rev(&rev, &لوگارتھم);

	shortlog_output(&لوگارتھم);
	release_revisions(&rev);
	أرجع 0;
}

ساكن فراغ جوڑو_wrapped_shortlog_پیغام(بنية strbuf *sb, ثابت حرف *s,
				     ثابت بنية shortlog *لوگارتھم)
{
	strbuf_جوڑو_wrapped_متن(sb, s, لوگارتھم->in1, لوگارتھم->in2, لوگارتھم->wrap);
	strbuf_addch(sb, '\n');
}

فراغ shortlog_output(بنية shortlog *لوگارتھم)
{
	ناپ_t i, j;
	بنية strbuf sb = STRBUF_INIT;

	إذا (لوگارتھم->ترتیب_دیو_by_نمبر)
		STABLE_QSORT(لوگارتھم->list.اشیاء, لوگارتھم->list.nr,
		      لوگارتھم->summary ? compare_by_counter : compare_by_list);
	لكل (i = 0; i < لوگارتھم->list.nr; i++) {
		ثابت بنية string_list_شے *شے = &لوگارتھم->list.اشیاء[i];
		إذا (لوگارتھم->summary) {
			fprintf(لوگارتھم->file, "%6d\t%s\n",
				(صحيح)UTIL_منزل_INT(شے), شے->string);
		} وإلا {
			بنية string_list *onelines = شے->util;
			fprintf(لوگارتھم->file, "%s (%"PRIuMAX"):\n",
				شے->string, (uintmax_t)onelines->nr);
			لكل (j = onelines->nr; j >= 1; j--) {
				ثابت حرف *پیغام = onelines->اشیاء[j - 1].string;

				إذا (لوگارتھم->wrap_سطریں) {
					strbuf_reset(&sb);
					جوڑو_wrapped_shortlog_پیغام(&sb, پیغام, لوگارتھم);
					fwrite(sb.buf, sb.len, 1, لوگارتھم->file);
				}
				وإلا
					fprintf(لوگارتھم->file, "      %s\n", پیغام);
			}
			putc('\n', لوگارتھم->file);
			onelines->strdup_لڑیاں = 1;
			string_list_clear(onelines, 0);
			حرر(onelines);
		}

		لوگارتھم->list.اشیاء[i].util = عدم;
	}

	strbuf_release(&sb);
	لوگارتھم->list.strdup_لڑیاں = 1;
	string_list_clear(&لوگارتھم->list, 1);
	clear_mailmap(&لوگارتھم->mailmap);
	string_list_clear(&لوگارتھم->format, 0);
	string_list_clear(&لوگارتھم->trailers, 0);
}
