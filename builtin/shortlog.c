#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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
#وصف_ڪريو UTIL_منزل_INT(x) ((intptr_t)(x)->util)

ساكن صحيح compare_by_counter(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_شي *i1 = a1, *i2 = a2;
	أرجع UTIL_منزل_INT(i2) - UTIL_منزل_INT(i1);
}

ساكن صحيح compare_by_list(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_شي *i1 = a1, *i2 = a2;
	ثابت بنية string_list *l1 = i1->util, *l2 = i2->util;

	إذا (l1->nr < l2->nr)
		أرجع 1;
	وإلا إذا (l1->nr == l2->nr)
		أرجع 0;
	وإلا
		أرجع -1;
}

ساكن فراغ وجهو_one_رڪارڊ(بنية shortlog *لاگرٿم,
			      ثابت حرف *ident,
			      ثابت حرف *oneline)
{
	بنية string_list_شي *شي;

	شي = string_list_وجهو(&لاگرٿم->list, ident);

	إذا (لاگرٿم->summary)
		شي->util = (فراغ *)(UTIL_منزل_INT(شي) + 1);
	وإلا {
		حرف *بفر;
		بنية strbuf subject = STRBUF_INIT;
		ثابت حرف *eol;

		/* Skip any leading whitespace, including any blank lines. */
		طالما (*oneline && خال_آهي(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		إذا (!eol)
			eol = oneline + لڙي_ڊيگهه(oneline);
		إذا (starts_with(oneline, "[PATCH")) {
			ثابت حرف *eob = strchr(oneline, ']');
			إذا (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		طالما (*oneline && خال_آهي(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		بفر = strbuf_detach(&subject, عدم);

		إذا (!شي->util) {
			شي->util = xmalloc(حجم(بنية string_list));
			string_list_init_nodup(شي->util);
		}
		string_list_append(شي->util, بفر);
	}
}

ساكن صحيح parse_ident(بنية shortlog *لاگرٿم,
		       بنية strbuf *ٻاهريون, ثابت حرف *اندريون)
{
	ثابت حرف *mailbuf, *namebuf;
	سائيز_t namelen, maillen;
	بنية ident_ورهايو ident;

	إذا (ورهايو_ident_سٽ(&ident, اندريون, لڙي_ڊيگهه(اندريون)))
		أرجع -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_پڇاڙي - ident.name_begin;
	maillen = ident.mail_پڇاڙي - ident.mail_begin;

	map_user(&لاگرٿم->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_جوڙيو(ٻاهريون, namebuf, namelen);
	إذا (لاگرٿم->email)
		strbuf_addf(ٻاهريون, " <%.*s>", (صحيح)maillen, mailbuf);

	أرجع 0;
}

ساكن فراغ read_from_stdin(بنية shortlog *لاگرٿم)
{
	بنية strbuf ident = STRBUF_INIT;
	بنية strbuf mapped_ident = STRBUF_INIT;
	بنية strbuf oneline = STRBUF_INIT;
	ساكن ثابت حرف *author_ميلاپ[2] = { "Author: ", "author " };
	ساكن ثابت حرف *committer_ميلاپ[2] = { "Commit: ", "committer " };
	ثابت حرف **ميلاپ;

	إذا (HAS_MULTI_بٽ(لاگرٿم->groups))
		die(_("using multiple --group options with stdin is not supported"));

	بدل (لاگرٿم->groups) {
	حالة SHORTLOG_گروهه_AUTHOR:
		ميلاپ = author_ميلاپ;
		اكسر;
	حالة SHORTLOG_گروهه_COMMITTER:
		ميلاپ = committer_ميلاپ;
		اكسر;
	حالة SHORTLOG_گروهه_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالة SHORTLOG_گروهه_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	افتراضي:
		BUG("unhandled shortlog group");
	}

	طالما (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت حرف *v;
		إذا (!skip_اڳياڙي(ident.buf, ميلاپ[0], &v) &&
		    !skip_اڳياڙي(ident.buf, ميلاپ[1], &v))
			استمر;
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		إذا (parse_ident(لاگرٿم, &mapped_ident, v) < 0)
			استمر;

		وجهو_one_رڪارڊ(لاگرٿم, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ساكن فراغ وجهو_رڪارڊون_from_trailers(بنية shortlog *لاگرٿم,
					 بنية strset *dups,
					 بنية commit *commit,
					 بنية pretty_print_context *ctx,
					 ثابت حرف *oneline)
{
	بنية trailer_iterator iter;
	ثابت حرف *commit_بفر, *body;
	بنية strbuf ident = STRBUF_INIT;

	إذا (!لاگرٿم->trailers.nr)
		أرجع;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_بفر = repo_logmsg_reencode(the_repository, commit, عدم,
					     ctx->output_encoding);
	body = لڙي_ڳولا(commit_بفر, "\n\n");
	إذا (!body)
		اذهب ٻاهريون;

	trailer_iterator_init(&iter, body);
	طالما (trailer_iterator_advance(&iter)) {
		ثابت حرف *قدر = iter.val.buf;

		إذا (!string_list_has_string(&لاگرٿم->trailers, iter.ڪنجي.buf))
			استمر;

		strbuf_reset(&ident);
		إذا (!parse_ident(لاگرٿم, &ident, قدر))
			قدر = ident.buf;

		إذا (!strset_جوڙيو(dups, قدر))
			استمر;
		وجهو_one_رڪارڊ(لاگرٿم, قدر, oneline);
	}
	trailer_iterator_release(&iter);

ٻاهريون:
	strbuf_release(&ident);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);
}

ساكن صحيح shortlog_needs_dedup(ثابت بنية shortlog *لاگرٿم)
{
	أرجع HAS_MULTI_بٽ(لاگرٿم->groups) || لاگرٿم->format.nr > 1 || لاگرٿم->trailers.nr;
}

ساكن فراغ وجهو_رڪارڊون_from_format(بنية shortlog *لاگرٿم,
				       بنية strset *dups,
				       بنية commit *commit,
				       بنية pretty_print_context *ctx,
				       ثابت حرف *oneline)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_شي *شي;

	for_each_string_list_شي(شي, &لاگرٿم->format) {
		strbuf_reset(&buf);

		repo_format_commit_پيغام(the_repository, commit,
					   شي->string, &buf, ctx);

		إذا (!shortlog_needs_dedup(لاگرٿم) || strset_جوڙيو(dups, buf.buf))
			وجهو_one_رڪارڊ(لاگرٿم, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

فراغ shortlog_جوڙيو_commit(بنية shortlog *لاگرٿم, بنية commit *commit)
{
	بنية strbuf oneline = STRBUF_INIT;
	بنية strset dups = STRSET_INIT;
	بنية pretty_print_context ctx = {0};
	ثابت حرف *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = لاگرٿم->abbrev;
	ctx.date_گهڻائي = لاگرٿم->date_گهڻائي;
	ctx.output_encoding = get_log_output_encoding();

	إذا (!لاگرٿم->summary) {
		إذا (لاگرٿم->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		وإلا
			repo_format_commit_پيغام(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	وجهو_رڪارڊون_from_trailers(لاگرٿم, &dups, commit, &ctx, oneline_str);
	وجهو_رڪارڊون_from_format(لاگرٿم, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ساكن فراغ get_from_rev(بنية rev_info *rev, بنية shortlog *لاگرٿم)
{
	بنية commit *commit;

	إذا (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	طالما ((commit = get_revision(rev)) != عدم)
		shortlog_جوڙيو_commit(لاگرٿم, commit);
}

ساكن صحيح parse_uint(حرف ثابت **arg, صحيح comma, صحيح defval)
{
	بنانشان ڊگهو ul;
	صحيح ret;
	حرف *endp;

	ul = strtoul(*arg, &endp, 10);
	إذا (*endp && *endp != comma)
		أرجع -1;
	إذا (ul > وڌ_کان_وڌ_عدد)
		أرجع -1;
	ret = *arg == endp ? defval : (صحيح)ul;
	*arg = *endp ? endp + 1 : endp;
	أرجع ret;
}

ساكن ثابت حرف wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#وصف_ڪريو DEFAULT_WRAPLEN 76
#وصف_ڪريو DEFAULT_INDENT1 6
#وصف_ڪريو DEFAULT_INDENT2 9

ساكن صحيح parse_wrap_دليلون(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لاگرٿم = opt->قدر;

	لاگرٿم->wrap_سٽون = !unset;
	إذا (unset)
		أرجع 0;
	إذا (!arg) {
		لاگرٿم->wrap = DEFAULT_WRAPLEN;
		لاگرٿم->in1 = DEFAULT_INDENT1;
		لاگرٿم->in2 = DEFAULT_INDENT2;
		أرجع 0;
	}

	لاگرٿم->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	لاگرٿم->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	لاگرٿم->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	إذا (لاگرٿم->wrap < 0 || لاگرٿم->in1 < 0 || لاگرٿم->in2 < 0)
		أرجع error(wrap_arg_usage);
	إذا (لاگرٿم->wrap &&
	    ((لاگرٿم->in1 && لاگرٿم->wrap <= لاگرٿم->in1) ||
	     (لاگرٿم->in2 && لاگرٿم->wrap <= لاگرٿم->in2)))
		أرجع error(wrap_arg_usage);
	أرجع 0;
}

ساكن صحيح parse_گروهه_option(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لاگرٿم = opt->قدر;
	ثابت حرف *ميدان;

	إذا (unset) {
		لاگرٿم->groups = 0;
		string_list_clear(&لاگرٿم->trailers, 0);
		string_list_clear(&لاگرٿم->format, 0);
	} وإلا إذا (!strcasecmp(arg, "author"))
		لاگرٿم->groups |= SHORTLOG_گروهه_AUTHOR;
	وإلا إذا (!strcasecmp(arg, "committer"))
		لاگرٿم->groups |= SHORTLOG_گروهه_COMMITTER;
	وإلا إذا (skip_اڳياڙي(arg, "trailer:", &ميدان)) {
		لاگرٿم->groups |= SHORTLOG_گروهه_TRAILER;
		string_list_append(&لاگرٿم->trailers, ميدان);
	} وإلا إذا (skip_اڳياڙي(arg, "format:", &ميدان)) {
		لاگرٿم->groups |= SHORTLOG_گروهه_FORMAT;
		string_list_append(&لاگرٿم->format, ميدان);
	} وإلا إذا (strchr(arg, '%')) {
		لاگرٿم->groups |= SHORTLOG_گروهه_FORMAT;
		string_list_append(&لاگرٿم->format, arg);
	} وإلا {
		أرجع error(_("unknown group type: %s"), arg);
	}

	أرجع 0;
}


فراغ shortlog_init(بنية shortlog *لاگرٿم)
{
	memset(لاگرٿم, 0, حجم(*لاگرٿم));

	read_mailmap(the_repository, &لاگرٿم->mailmap);

	لاگرٿم->list.strdup_لڙيون = 1;
	لاگرٿم->wrap = DEFAULT_WRAPLEN;
	لاگرٿم->in1 = DEFAULT_INDENT1;
	لاگرٿم->in2 = DEFAULT_INDENT2;
	لاگرٿم->trailers.strdup_لڙيون = 1;
	لاگرٿم->trailers.cmp = strcasecmp;
	لاگرٿم->format.strdup_لڙيون = 1;
}

فراغ shortlog_پڄاڻي_تياري(بنية shortlog *لاگرٿم)
{
	إذا (لاگرٿم->groups & SHORTLOG_گروهه_AUTHOR)
		string_list_append(&لاگرٿم->format,
				   لاگرٿم->email ? "%aN <%aE>" : "%aN");
	إذا (لاگرٿم->groups & SHORTLOG_گروهه_COMMITTER)
		string_list_append(&لاگرٿم->format,
				   لاگرٿم->email ? "%cN <%cE>" : "%cN");

	string_list_ترتيب_ڏيو(&لاگرٿم->trailers);
}

صحيح cmd_shortlog(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية shortlog لاگرٿم = { STRING_LIST_INIT_NODUP };
	بنية rev_info rev;
	صحيح nongit = !startup_info->have_repository;

	ثابت بنية option options[] = {
		OPT_ٻٽو_انگ('c', "committer", &لاگرٿم.groups,
			N_("group by committer rather than author"),
			SHORTLOG_گروهه_COMMITTER),
		OPT_BOOL('n', "numbered", &لاگرٿم.ترتيب_ڏيو_by_عدد,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &لاگرٿم.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &لاگرٿم.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', عدم, &لاگرٿم, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_دليلون),
		OPT_CALLBACK(0, "group", &لاگرٿم, N_("field"),
			N_("group by field"), parse_گروهه_option),
		OPT_پڇاڙي(),
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
		repo_سيٽ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, عدم);
	shortlog_init(&لاگرٿم);
	repo_init_revisions(the_repository, &rev, اڳياڙي);
	parse_options_شروعات(&ctx, argc, argv, اڳياڙي, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	لكل (;;) {
		بدل (parse_options_قدم(&ctx, options, shortlog_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			ٻاهر_نڪرو(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			ٻاهر_نڪرو(129);
		حالة PARSE_OPT_COMPLETE:
			ٻاهر_نڪرو(0);
		حالة PARSE_OPT_پورو:
			اذهب parse_پورو;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_پورو:
	revision_opts_پڄاڻي(&rev);
	argc = parse_options_پڇاڙي(&ctx);

	إذا (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	إذا (!nongit && تياري_revisions(argc, argv, &rev, عدم) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	لاگرٿم.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	لاگرٿم.abbrev = rev.abbrev;
	لاگرٿم.file = rev.diffopt.file;
	لاگرٿم.date_گهڻائي = rev.date_گهڻائي;

	إذا (!لاگرٿم.groups)
		لاگرٿم.groups = SHORTLOG_گروهه_AUTHOR;
	shortlog_پڄاڻي_تياري(&لاگرٿم);

	/* assume HEAD if from a tty */
	إذا (!nongit && !rev.pending.nr && isatty(0))
		جوڙيو_سر_منزل_pending(&rev);
	إذا (rev.pending.nr == 0) {
		إذا (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&لاگرٿم);
	}
	وإلا
		get_from_rev(&rev, &لاگرٿم);

	shortlog_output(&لاگرٿم);
	release_revisions(&rev);
	أرجع 0;
}

ساكن فراغ جوڙيو_wrapped_shortlog_نياپو(بنية strbuf *sb, ثابت حرف *s,
				     ثابت بنية shortlog *لاگرٿم)
{
	strbuf_جوڙيو_wrapped_متن(sb, s, لاگرٿم->in1, لاگرٿم->in2, لاگرٿم->wrap);
	strbuf_addch(sb, '\n');
}

فراغ shortlog_output(بنية shortlog *لاگرٿم)
{
	سائيز_t i, j;
	بنية strbuf sb = STRBUF_INIT;

	إذا (لاگرٿم->ترتيب_ڏيو_by_عدد)
		STABLE_QSORT(لاگرٿم->list.شيون, لاگرٿم->list.nr,
		      لاگرٿم->summary ? compare_by_counter : compare_by_list);
	لكل (i = 0; i < لاگرٿم->list.nr; i++) {
		ثابت بنية string_list_شي *شي = &لاگرٿم->list.شيون[i];
		إذا (لاگرٿم->summary) {
			fprintf(لاگرٿم->file, "%6d\t%s\n",
				(صحيح)UTIL_منزل_INT(شي), شي->string);
		} وإلا {
			بنية string_list *onelines = شي->util;
			fprintf(لاگرٿم->file, "%s (%"PRIuMAX"):\n",
				شي->string, (uintmax_t)onelines->nr);
			لكل (j = onelines->nr; j >= 1; j--) {
				ثابت حرف *نياپو = onelines->شيون[j - 1].string;

				إذا (لاگرٿم->wrap_سٽون) {
					strbuf_reset(&sb);
					جوڙيو_wrapped_shortlog_نياپو(&sb, نياپو, لاگرٿم);
					fwrite(sb.buf, sb.len, 1, لاگرٿم->file);
				}
				وإلا
					fprintf(لاگرٿم->file, "      %s\n", نياپو);
			}
			putc('\n', لاگرٿم->file);
			onelines->strdup_لڙيون = 1;
			string_list_clear(onelines, 0);
			حرر(onelines);
		}

		لاگرٿم->list.شيون[i].util = عدم;
	}

	strbuf_release(&sb);
	لاگرٿم->list.strdup_لڙيون = 1;
	string_list_clear(&لاگرٿم->list, 1);
	clear_mailmap(&لاگرٿم->mailmap);
	string_list_clear(&لاگرٿم->format, 0);
	string_list_clear(&لاگرٿم->trailers, 0);
}
