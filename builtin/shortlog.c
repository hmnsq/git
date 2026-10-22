#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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
#تعریف_کن UTIL_مقصد_INT(x) ((intptr_t)(x)->util)

ساكن صحيح compare_by_counter(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_قلم *i1 = a1, *i2 = a2;
	أرجع UTIL_مقصد_INT(i2) - UTIL_مقصد_INT(i1);
}

ساكن صحيح compare_by_list(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_قلم *i1 = a1, *i2 = a2;
	ثابت بنية string_list *l1 = i1->util, *l2 = i2->util;

	إذا (l1->nr < l2->nr)
		أرجع 1;
	وإلا إذا (l1->nr == l2->nr)
		أرجع 0;
	وإلا
		أرجع -1;
}

ساكن فراغ درج_one_رکورد(بنية shortlog *لگاریتم,
			      ثابت حرف *ident,
			      ثابت حرف *oneline)
{
	بنية string_list_قلم *قلم;

	قلم = string_list_درج(&لگاریتم->list, ident);

	إذا (لگاریتم->summary)
		قلم->util = (فراغ *)(UTIL_مقصد_INT(قلم) + 1);
	وإلا {
		حرف *میانگیر;
		بنية strbuf subject = STRBUF_INIT;
		ثابت حرف *eol;

		/* Skip any leading whitespace, including any blank lines. */
		طالما (*oneline && آیا_فاصله(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		إذا (!eol)
			eol = oneline + طول_رشته(oneline);
		إذا (starts_with(oneline, "[PATCH")) {
			ثابت حرف *eob = strchr(oneline, ']');
			إذا (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		طالما (*oneline && آیا_فاصله(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		میانگیر = strbuf_detach(&subject, عدم);

		إذا (!قلم->util) {
			قلم->util = xmalloc(حجم(بنية string_list));
			string_list_init_nodup(قلم->util);
		}
		string_list_append(قلم->util, میانگیر);
	}
}

ساكن صحيح parse_ident(بنية shortlog *لگاریتم,
		       بنية strbuf *بیرون, ثابت حرف *درون)
{
	ثابت حرف *mailbuf, *namebuf;
	اندازه_t namelen, maillen;
	بنية ident_تقسیم ident;

	إذا (تقسیم_ident_سطر(&ident, درون, طول_رشته(درون)))
		أرجع -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_انتها - ident.name_begin;
	maillen = ident.mail_انتها - ident.mail_begin;

	map_user(&لگاریتم->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_جمع_کن(بیرون, namebuf, namelen);
	إذا (لگاریتم->email)
		strbuf_addf(بیرون, " <%.*s>", (صحيح)maillen, mailbuf);

	أرجع 0;
}

ساكن فراغ read_from_stdin(بنية shortlog *لگاریتم)
{
	بنية strbuf ident = STRBUF_INIT;
	بنية strbuf mapped_ident = STRBUF_INIT;
	بنية strbuf oneline = STRBUF_INIT;
	ساكن ثابت حرف *author_تطابق[2] = { "Author: ", "author " };
	ساكن ثابت حرف *committer_تطابق[2] = { "Commit: ", "committer " };
	ثابت حرف **تطابق;

	إذا (HAS_MULTI_بیتها(لگاریتم->groups))
		die(_("using multiple --group options with stdin is not supported"));

	بدل (لگاریتم->groups) {
	حالة SHORTLOG_گروه_AUTHOR:
		تطابق = author_تطابق;
		اكسر;
	حالة SHORTLOG_گروه_COMMITTER:
		تطابق = committer_تطابق;
		اكسر;
	حالة SHORTLOG_گروه_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالة SHORTLOG_گروه_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	افتراضي:
		BUG("unhandled shortlog group");
	}

	طالما (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت حرف *v;
		إذا (!skip_پیشوند(ident.buf, تطابق[0], &v) &&
		    !skip_پیشوند(ident.buf, تطابق[1], &v))
			استمر;
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		إذا (parse_ident(لگاریتم, &mapped_ident, v) < 0)
			استمر;

		درج_one_رکورد(لگاریتم, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ساكن فراغ درج_رکوردها_from_trailers(بنية shortlog *لگاریتم,
					 بنية strset *dups,
					 بنية commit *commit,
					 بنية pretty_print_context *ctx,
					 ثابت حرف *oneline)
{
	بنية trailer_iterator iter;
	ثابت حرف *commit_میانگیر, *body;
	بنية strbuf ident = STRBUF_INIT;

	إذا (!لگاریتم->trailers.nr)
		أرجع;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_میانگیر = repo_logmsg_reencode(the_repository, commit, عدم,
					     ctx->output_encoding);
	body = جستجوی_رشته(commit_میانگیر, "\n\n");
	إذا (!body)
		اذهب بیرون;

	trailer_iterator_init(&iter, body);
	طالما (trailer_iterator_advance(&iter)) {
		ثابت حرف *مقدار = iter.val.buf;

		إذا (!string_list_has_string(&لگاریتم->trailers, iter.کلید.buf))
			استمر;

		strbuf_reset(&ident);
		إذا (!parse_ident(لگاریتم, &ident, مقدار))
			مقدار = ident.buf;

		إذا (!strset_جمع_کن(dups, مقدار))
			استمر;
		درج_one_رکورد(لگاریتم, مقدار, oneline);
	}
	trailer_iterator_release(&iter);

بیرون:
	strbuf_release(&ident);
	repo_unuse_commit_میانگیر(the_repository, commit, commit_میانگیر);
}

ساكن صحيح shortlog_needs_dedup(ثابت بنية shortlog *لگاریتم)
{
	أرجع HAS_MULTI_بیتها(لگاریتم->groups) || لگاریتم->format.nr > 1 || لگاریتم->trailers.nr;
}

ساكن فراغ درج_رکوردها_from_format(بنية shortlog *لگاریتم,
				       بنية strset *dups,
				       بنية commit *commit,
				       بنية pretty_print_context *ctx,
				       ثابت حرف *oneline)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_قلم *قلم;

	for_each_string_list_قلم(قلم, &لگاریتم->format) {
		strbuf_reset(&buf);

		repo_format_commit_پیام(the_repository, commit,
					   قلم->string, &buf, ctx);

		إذا (!shortlog_needs_dedup(لگاریتم) || strset_جمع_کن(dups, buf.buf))
			درج_one_رکورد(لگاریتم, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

فراغ shortlog_جمع_کن_commit(بنية shortlog *لگاریتم, بنية commit *commit)
{
	بنية strbuf oneline = STRBUF_INIT;
	بنية strset dups = STRSET_INIT;
	بنية pretty_print_context ctx = {0};
	ثابت حرف *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = لگاریتم->abbrev;
	ctx.date_نما = لگاریتم->date_نما;
	ctx.output_encoding = get_log_output_encoding();

	إذا (!لگاریتم->summary) {
		إذا (لگاریتم->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		وإلا
			repo_format_commit_پیام(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	درج_رکوردها_from_trailers(لگاریتم, &dups, commit, &ctx, oneline_str);
	درج_رکوردها_from_format(لگاریتم, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ساكن فراغ get_from_rev(بنية rev_info *rev, بنية shortlog *لگاریتم)
{
	بنية commit *commit;

	إذا (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	طالما ((commit = get_revision(rev)) != عدم)
		shortlog_جمع_کن_commit(لگاریتم, commit);
}

ساكن صحيح parse_uint(حرف ثابت **arg, صحيح comma, صحيح defval)
{
	بیعلامت دراز ul;
	صحيح ret;
	حرف *endp;

	ul = strtoul(*arg, &endp, 10);
	إذا (*endp && *endp != comma)
		أرجع -1;
	إذا (ul > بیشترین_صحیح)
		أرجع -1;
	ret = *arg == endp ? defval : (صحيح)ul;
	*arg = *endp ? endp + 1 : endp;
	أرجع ret;
}

ساكن ثابت حرف wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#تعریف_کن DEFAULT_WRAPLEN 76
#تعریف_کن DEFAULT_INDENT1 6
#تعریف_کن DEFAULT_INDENT2 9

ساكن صحيح parse_wrap_آرگومانها(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لگاریتم = opt->مقدار;

	لگاریتم->wrap_سطرها = !unset;
	إذا (unset)
		أرجع 0;
	إذا (!arg) {
		لگاریتم->wrap = DEFAULT_WRAPLEN;
		لگاریتم->in1 = DEFAULT_INDENT1;
		لگاریتم->in2 = DEFAULT_INDENT2;
		أرجع 0;
	}

	لگاریتم->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	لگاریتم->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	لگاریتم->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	إذا (لگاریتم->wrap < 0 || لگاریتم->in1 < 0 || لگاریتم->in2 < 0)
		أرجع error(wrap_arg_usage);
	إذا (لگاریتم->wrap &&
	    ((لگاریتم->in1 && لگاریتم->wrap <= لگاریتم->in1) ||
	     (لگاریتم->in2 && لگاریتم->wrap <= لگاریتم->in2)))
		أرجع error(wrap_arg_usage);
	أرجع 0;
}

ساكن صحيح parse_گروه_option(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لگاریتم = opt->مقدار;
	ثابت حرف *فیلد;

	إذا (unset) {
		لگاریتم->groups = 0;
		string_list_clear(&لگاریتم->trailers, 0);
		string_list_clear(&لگاریتم->format, 0);
	} وإلا إذا (!strcasecmp(arg, "author"))
		لگاریتم->groups |= SHORTLOG_گروه_AUTHOR;
	وإلا إذا (!strcasecmp(arg, "committer"))
		لگاریتم->groups |= SHORTLOG_گروه_COMMITTER;
	وإلا إذا (skip_پیشوند(arg, "trailer:", &فیلد)) {
		لگاریتم->groups |= SHORTLOG_گروه_TRAILER;
		string_list_append(&لگاریتم->trailers, فیلد);
	} وإلا إذا (skip_پیشوند(arg, "format:", &فیلد)) {
		لگاریتم->groups |= SHORTLOG_گروه_FORMAT;
		string_list_append(&لگاریتم->format, فیلد);
	} وإلا إذا (strchr(arg, '%')) {
		لگاریتم->groups |= SHORTLOG_گروه_FORMAT;
		string_list_append(&لگاریتم->format, arg);
	} وإلا {
		أرجع error(_("unknown group type: %s"), arg);
	}

	أرجع 0;
}


فراغ shortlog_init(بنية shortlog *لگاریتم)
{
	memset(لگاریتم, 0, حجم(*لگاریتم));

	read_mailmap(the_repository, &لگاریتم->mailmap);

	لگاریتم->list.strdup_رشتهها = 1;
	لگاریتم->wrap = DEFAULT_WRAPLEN;
	لگاریتم->in1 = DEFAULT_INDENT1;
	لگاریتم->in2 = DEFAULT_INDENT2;
	لگاریتم->trailers.strdup_رشتهها = 1;
	لگاریتم->trailers.cmp = strcasecmp;
	لگاریتم->format.strdup_رشتهها = 1;
}

فراغ shortlog_پایان_راهاندازی(بنية shortlog *لگاریتم)
{
	إذا (لگاریتم->groups & SHORTLOG_گروه_AUTHOR)
		string_list_append(&لگاریتم->format,
				   لگاریتم->email ? "%aN <%aE>" : "%aN");
	إذا (لگاریتم->groups & SHORTLOG_گروه_COMMITTER)
		string_list_append(&لگاریتم->format,
				   لگاریتم->email ? "%cN <%cE>" : "%cN");

	string_list_مرتب_سازی(&لگاریتم->trailers);
}

صحيح cmd_shortlog(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *پیشوند,
		 بنية repository *repo استفادهنشده)
{
	بنية shortlog لگاریتم = { STRING_LIST_INIT_NODUP };
	بنية rev_info rev;
	صحيح nongit = !startup_info->have_repository;

	ثابت بنية option options[] = {
		OPT_بیت('c', "committer", &لگاریتم.groups,
			N_("group by committer rather than author"),
			SHORTLOG_گروه_COMMITTER),
		OPT_BOOL('n', "numbered", &لگاریتم.مرتب_سازی_by_عدد,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &لگاریتم.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &لگاریتم.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', عدم, &لگاریتم, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_آرگومانها),
		OPT_CALLBACK(0, "group", &لگاریتم, N_("field"),
			N_("group by field"), parse_گروه_option),
		OPT_انتها(),
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
		repo_مجموعه_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, عدم);
	shortlog_init(&لگاریتم);
	repo_init_revisions(the_repository, &rev, پیشوند);
	parse_options_شروع(&ctx, argc, argv, پیشوند, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	لكل (;;) {
		بدل (parse_options_گام(&ctx, options, shortlog_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			خروج(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			خروج(129);
		حالة PARSE_OPT_COMPLETE:
			خروج(0);
		حالة PARSE_OPT_انجامشده:
			اذهب parse_انجامشده;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_انجامشده:
	revision_opts_پایان(&rev);
	argc = parse_options_انتها(&ctx);

	إذا (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	إذا (!nongit && راهاندازی_revisions(argc, argv, &rev, عدم) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	لگاریتم.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	لگاریتم.abbrev = rev.abbrev;
	لگاریتم.file = rev.diffopt.file;
	لگاریتم.date_نما = rev.date_نما;

	إذا (!لگاریتم.groups)
		لگاریتم.groups = SHORTLOG_گروه_AUTHOR;
	shortlog_پایان_راهاندازی(&لگاریتم);

	/* assume HEAD if from a tty */
	إذا (!nongit && !rev.pending.nr && isatty(0))
		جمع_کن_سر_مقصد_pending(&rev);
	إذا (rev.pending.nr == 0) {
		إذا (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&لگاریتم);
	}
	وإلا
		get_from_rev(&rev, &لگاریتم);

	shortlog_output(&لگاریتم);
	release_revisions(&rev);
	أرجع 0;
}

ساكن فراغ جمع_کن_wrapped_shortlog_پیام(بنية strbuf *sb, ثابت حرف *s,
				     ثابت بنية shortlog *لگاریتم)
{
	strbuf_جمع_کن_wrapped_متن(sb, s, لگاریتم->in1, لگاریتم->in2, لگاریتم->wrap);
	strbuf_addch(sb, '\n');
}

فراغ shortlog_output(بنية shortlog *لگاریتم)
{
	اندازه_t i, j;
	بنية strbuf sb = STRBUF_INIT;

	إذا (لگاریتم->مرتب_سازی_by_عدد)
		STABLE_QSORT(لگاریتم->list.اقلام, لگاریتم->list.nr,
		      لگاریتم->summary ? compare_by_counter : compare_by_list);
	لكل (i = 0; i < لگاریتم->list.nr; i++) {
		ثابت بنية string_list_قلم *قلم = &لگاریتم->list.اقلام[i];
		إذا (لگاریتم->summary) {
			fprintf(لگاریتم->file, "%6d\t%s\n",
				(صحيح)UTIL_مقصد_INT(قلم), قلم->string);
		} وإلا {
			بنية string_list *onelines = قلم->util;
			fprintf(لگاریتم->file, "%s (%"PRIuMAX"):\n",
				قلم->string, (uintmax_t)onelines->nr);
			لكل (j = onelines->nr; j >= 1; j--) {
				ثابت حرف *پیام = onelines->اقلام[j - 1].string;

				إذا (لگاریتم->wrap_سطرها) {
					strbuf_reset(&sb);
					جمع_کن_wrapped_shortlog_پیام(&sb, پیام, لگاریتم);
					fwrite(sb.buf, sb.len, 1, لگاریتم->file);
				}
				وإلا
					fprintf(لگاریتم->file, "      %s\n", پیام);
			}
			putc('\n', لگاریتم->file);
			onelines->strdup_رشتهها = 1;
			string_list_clear(onelines, 0);
			حرر(onelines);
		}

		لگاریتم->list.اقلام[i].util = عدم;
	}

	strbuf_release(&sb);
	لگاریتم->list.strdup_رشتهها = 1;
	string_list_clear(&لگاریتم->list, 1);
	clear_mailmap(&لگاریتم->mailmap);
	string_list_clear(&لگاریتم->format, 0);
	string_list_clear(&لگاریتم->trailers, 0);
}
