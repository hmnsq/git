#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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
#تعریف_کړه UTIL_منزل_INT(x) ((intptr_t)(x)->util)

ساكن صحيح compare_by_counter(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_توکی *i1 = a1, *i2 = a2;
	أرجع UTIL_منزل_INT(i2) - UTIL_منزل_INT(i1);
}

ساكن صحيح compare_by_list(ثابت فراغ *a1, ثابت فراغ *a2)
{
	ثابت بنية string_list_توکی *i1 = a1, *i2 = a2;
	ثابت بنية string_list *l1 = i1->util, *l2 = i2->util;

	إذا (l1->nr < l2->nr)
		أرجع 1;
	وإلا إذا (l1->nr == l2->nr)
		أرجع 0;
	وإلا
		أرجع -1;
}

ساكن فراغ ورننباسه_one_ریکارډ(بنية shortlog *لوګاریتم,
			      ثابت حرف *ident,
			      ثابت حرف *oneline)
{
	بنية string_list_توکی *توکی;

	توکی = string_list_ورننباسه(&لوګاریتم->list, ident);

	إذا (لوګاریتم->summary)
		توکی->util = (فراغ *)(UTIL_منزل_INT(توکی) + 1);
	وإلا {
		حرف *بفر;
		بنية strbuf subject = STRBUF_INIT;
		ثابت حرف *eol;

		/* Skip any leading whitespace, including any blank lines. */
		طالما (*oneline && که_تشه(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		إذا (!eol)
			eol = oneline + مزي_اوږدوالی(oneline);
		إذا (starts_with(oneline, "[PATCH")) {
			ثابت حرف *eob = strchr(oneline, ']');
			إذا (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		طالما (*oneline && که_تشه(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		بفر = strbuf_detach(&subject, عدم);

		إذا (!توکی->util) {
			توکی->util = xmalloc(حجم(بنية string_list));
			string_list_init_nodup(توکی->util);
		}
		string_list_append(توکی->util, بفر);
	}
}

ساكن صحيح parse_ident(بنية shortlog *لوګاریتم,
		       بنية strbuf *بهر, ثابت حرف *دننه)
{
	ثابت حرف *mailbuf, *namebuf;
	اندازه_t namelen, maillen;
	بنية ident_وېشل ident;

	إذا (وېشل_ident_کرښه(&ident, دننه, مزي_اوږدوالی(دننه)))
		أرجع -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_پای_ټکی - ident.name_begin;
	maillen = ident.mail_پای_ټکی - ident.mail_begin;

	map_user(&لوګاریتم->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_جمع_کړه(بهر, namebuf, namelen);
	إذا (لوګاریتم->email)
		strbuf_addf(بهر, " <%.*s>", (صحيح)maillen, mailbuf);

	أرجع 0;
}

ساكن فراغ read_from_stdin(بنية shortlog *لوګاریتم)
{
	بنية strbuf ident = STRBUF_INIT;
	بنية strbuf mapped_ident = STRBUF_INIT;
	بنية strbuf oneline = STRBUF_INIT;
	ساكن ثابت حرف *author_سمون[2] = { "Author: ", "author " };
	ساكن ثابت حرف *committer_سمون[2] = { "Commit: ", "committer " };
	ثابت حرف **سمون;

	إذا (HAS_MULTI_بټونه(لوګاریتم->groups))
		die(_("using multiple --group options with stdin is not supported"));

	بدل (لوګاریتم->groups) {
	حالة SHORTLOG_ډله_AUTHOR:
		سمون = author_سمون;
		اكسر;
	حالة SHORTLOG_ډله_COMMITTER:
		سمون = committer_سمون;
		اكسر;
	حالة SHORTLOG_ډله_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	حالة SHORTLOG_ډله_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	افتراضي:
		BUG("unhandled shortlog group");
	}

	طالما (strbuf_getline_lf(&ident, stdin) != EOF) {
		ثابت حرف *v;
		إذا (!skip_مختاړی(ident.buf, سمون[0], &v) &&
		    !skip_مختاړی(ident.buf, سمون[1], &v))
			استمر;
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		طالما (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		إذا (parse_ident(لوګاریتم, &mapped_ident, v) < 0)
			استمر;

		ورننباسه_one_ریکارډ(لوګاریتم, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ساكن فراغ ورننباسه_ریکارډونه_from_trailers(بنية shortlog *لوګاریتم,
					 بنية strset *dups,
					 بنية commit *commit,
					 بنية pretty_print_context *ctx,
					 ثابت حرف *oneline)
{
	بنية trailer_iterator iter;
	ثابت حرف *commit_بفر, *body;
	بنية strbuf ident = STRBUF_INIT;

	إذا (!لوګاریتم->trailers.nr)
		أرجع;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_بفر = repo_logmsg_reencode(the_repository, commit, عدم,
					     ctx->output_encoding);
	body = مزي_پلټنه(commit_بفر, "\n\n");
	إذا (!body)
		اذهب بهر;

	trailer_iterator_init(&iter, body);
	طالما (trailer_iterator_advance(&iter)) {
		ثابت حرف *ارزښت = iter.val.buf;

		إذا (!string_list_has_string(&لوګاریتم->trailers, iter.کلی.buf))
			استمر;

		strbuf_reset(&ident);
		إذا (!parse_ident(لوګاریتم, &ident, ارزښت))
			ارزښت = ident.buf;

		إذا (!strset_جمع_کړه(dups, ارزښت))
			استمر;
		ورننباسه_one_ریکارډ(لوګاریتم, ارزښت, oneline);
	}
	trailer_iterator_release(&iter);

بهر:
	strbuf_release(&ident);
	repo_unuse_commit_بفر(the_repository, commit, commit_بفر);
}

ساكن صحيح shortlog_needs_dedup(ثابت بنية shortlog *لوګاریتم)
{
	أرجع HAS_MULTI_بټونه(لوګاریتم->groups) || لوګاریتم->format.nr > 1 || لوګاریتم->trailers.nr;
}

ساكن فراغ ورننباسه_ریکارډونه_from_format(بنية shortlog *لوګاریتم,
				       بنية strset *dups,
				       بنية commit *commit,
				       بنية pretty_print_context *ctx,
				       ثابت حرف *oneline)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_توکی *توکی;

	for_each_string_list_توکی(توکی, &لوګاریتم->format) {
		strbuf_reset(&buf);

		repo_format_commit_پیغام(the_repository, commit,
					   توکی->string, &buf, ctx);

		إذا (!shortlog_needs_dedup(لوګاریتم) || strset_جمع_کړه(dups, buf.buf))
			ورننباسه_one_ریکارډ(لوګاریتم, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

فراغ shortlog_جمع_کړه_commit(بنية shortlog *لوګاریتم, بنية commit *commit)
{
	بنية strbuf oneline = STRBUF_INIT;
	بنية strset dups = STRSET_INIT;
	بنية pretty_print_context ctx = {0};
	ثابت حرف *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = لوګاریتم->abbrev;
	ctx.date_ډېرپېښ = لوګاریتم->date_ډېرپېښ;
	ctx.output_encoding = get_log_output_encoding();

	إذا (!لوګاریتم->summary) {
		إذا (لوګاریتم->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		وإلا
			repo_format_commit_پیغام(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ورننباسه_ریکارډونه_from_trailers(لوګاریتم, &dups, commit, &ctx, oneline_str);
	ورننباسه_ریکارډونه_from_format(لوګاریتم, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ساكن فراغ get_from_rev(بنية rev_info *rev, بنية shortlog *لوګاریتم)
{
	بنية commit *commit;

	إذا (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	طالما ((commit = get_revision(rev)) != عدم)
		shortlog_جمع_کړه_commit(لوګاریتم, commit);
}

ساكن صحيح parse_uint(حرف ثابت **arg, صحيح comma, صحيح defval)
{
	بینښه اوږد ul;
	صحيح ret;
	حرف *endp;

	ul = strtoul(*arg, &endp, 10);
	إذا (*endp && *endp != comma)
		أرجع -1;
	إذا (ul > لوی_عدد)
		أرجع -1;
	ret = *arg == endp ? defval : (صحيح)ul;
	*arg = *endp ? endp + 1 : endp;
	أرجع ret;
}

ساكن ثابت حرف wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#تعریف_کړه DEFAULT_WRAPLEN 76
#تعریف_کړه DEFAULT_INDENT1 6
#تعریف_کړه DEFAULT_INDENT2 9

ساكن صحيح parse_wrap_دلیلونه(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لوګاریتم = opt->ارزښت;

	لوګاریتم->wrap_کرښې = !unset;
	إذا (unset)
		أرجع 0;
	إذا (!arg) {
		لوګاریتم->wrap = DEFAULT_WRAPLEN;
		لوګاریتم->in1 = DEFAULT_INDENT1;
		لوګاریتم->in2 = DEFAULT_INDENT2;
		أرجع 0;
	}

	لوګاریتم->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	لوګاریتم->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	لوګاریتم->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	إذا (لوګاریتم->wrap < 0 || لوګاریتم->in1 < 0 || لوګاریتم->in2 < 0)
		أرجع error(wrap_arg_usage);
	إذا (لوګاریتم->wrap &&
	    ((لوګاریتم->in1 && لوګاریتم->wrap <= لوګاریتم->in1) ||
	     (لوګاریتم->in2 && لوګاریتم->wrap <= لوګاریتم->in2)))
		أرجع error(wrap_arg_usage);
	أرجع 0;
}

ساكن صحيح parse_ډله_option(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية shortlog *لوګاریتم = opt->ارزښت;
	ثابت حرف *ډګر;

	إذا (unset) {
		لوګاریتم->groups = 0;
		string_list_clear(&لوګاریتم->trailers, 0);
		string_list_clear(&لوګاریتم->format, 0);
	} وإلا إذا (!strcasecmp(arg, "author"))
		لوګاریتم->groups |= SHORTLOG_ډله_AUTHOR;
	وإلا إذا (!strcasecmp(arg, "committer"))
		لوګاریتم->groups |= SHORTLOG_ډله_COMMITTER;
	وإلا إذا (skip_مختاړی(arg, "trailer:", &ډګر)) {
		لوګاریتم->groups |= SHORTLOG_ډله_TRAILER;
		string_list_append(&لوګاریتم->trailers, ډګر);
	} وإلا إذا (skip_مختاړی(arg, "format:", &ډګر)) {
		لوګاریتم->groups |= SHORTLOG_ډله_FORMAT;
		string_list_append(&لوګاریتم->format, ډګر);
	} وإلا إذا (strchr(arg, '%')) {
		لوګاریتم->groups |= SHORTLOG_ډله_FORMAT;
		string_list_append(&لوګاریتم->format, arg);
	} وإلا {
		أرجع error(_("unknown group type: %s"), arg);
	}

	أرجع 0;
}


فراغ shortlog_init(بنية shortlog *لوګاریتم)
{
	memset(لوګاریتم, 0, حجم(*لوګاریتم));

	read_mailmap(the_repository, &لوګاریتم->mailmap);

	لوګاریتم->list.strdup_مزي_ګانې = 1;
	لوګاریتم->wrap = DEFAULT_WRAPLEN;
	لوګاریتم->in1 = DEFAULT_INDENT1;
	لوګاریتم->in2 = DEFAULT_INDENT2;
	لوګاریتم->trailers.strdup_مزي_ګانې = 1;
	لوګاریتم->trailers.cmp = strcasecmp;
	لوګاریتم->format.strdup_مزي_ګانې = 1;
}

فراغ shortlog_پای_تیاری(بنية shortlog *لوګاریتم)
{
	إذا (لوګاریتم->groups & SHORTLOG_ډله_AUTHOR)
		string_list_append(&لوګاریتم->format,
				   لوګاریتم->email ? "%aN <%aE>" : "%aN");
	إذا (لوګاریتم->groups & SHORTLOG_ډله_COMMITTER)
		string_list_append(&لوګاریتم->format,
				   لوګاریتم->email ? "%cN <%cE>" : "%cN");

	string_list_ترتیبول(&لوګاریتم->trailers);
}

صحيح cmd_shortlog(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *مختاړی,
		 بنية repository *repo ناکارول_شوی)
{
	بنية shortlog لوګاریتم = { STRING_LIST_INIT_NODUP };
	بنية rev_info rev;
	صحيح nongit = !startup_info->have_repository;

	ثابت بنية option options[] = {
		OPT_بټ('c', "committer", &لوګاریتم.groups,
			N_("group by committer rather than author"),
			SHORTLOG_ډله_COMMITTER),
		OPT_BOOL('n', "numbered", &لوګاریتم.ترتیبول_by_شمېره,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &لوګاریتم.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &لوګاریتم.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', عدم, &لوګاریتم, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_دلیلونه),
		OPT_CALLBACK(0, "group", &لوګاریتم, N_("field"),
			N_("group by field"), parse_ډله_option),
		OPT_پای_ټکی(),
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
		repo_سېټ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, عدم);
	shortlog_init(&لوګاریتم);
	repo_init_revisions(the_repository, &rev, مختاړی);
	parse_options_پیل(&ctx, argc, argv, مختاړی, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	لكل (;;) {
		بدل (parse_options_ګام(&ctx, options, shortlog_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			وتل(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			وتل(129);
		حالة PARSE_OPT_COMPLETE:
			وتل(0);
		حالة PARSE_OPT_بشپړ:
			اذهب parse_بشپړ;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_بشپړ:
	revision_opts_پای(&rev);
	argc = parse_options_پای_ټکی(&ctx);

	إذا (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	إذا (!nongit && تیاری_revisions(argc, argv, &rev, عدم) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	لوګاریتم.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	لوګاریتم.abbrev = rev.abbrev;
	لوګاریتم.file = rev.diffopt.file;
	لوګاریتم.date_ډېرپېښ = rev.date_ډېرپېښ;

	إذا (!لوګاریتم.groups)
		لوګاریتم.groups = SHORTLOG_ډله_AUTHOR;
	shortlog_پای_تیاری(&لوګاریتم);

	/* assume HEAD if from a tty */
	إذا (!nongit && !rev.pending.nr && isatty(0))
		جمع_کړه_سر_منزل_pending(&rev);
	إذا (rev.pending.nr == 0) {
		إذا (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&لوګاریتم);
	}
	وإلا
		get_from_rev(&rev, &لوګاریتم);

	shortlog_output(&لوګاریتم);
	release_revisions(&rev);
	أرجع 0;
}

ساكن فراغ جمع_کړه_wrapped_shortlog_پیغام(بنية strbuf *sb, ثابت حرف *s,
				     ثابت بنية shortlog *لوګاریتم)
{
	strbuf_جمع_کړه_wrapped_متن(sb, s, لوګاریتم->in1, لوګاریتم->in2, لوګاریتم->wrap);
	strbuf_addch(sb, '\n');
}

فراغ shortlog_output(بنية shortlog *لوګاریتم)
{
	اندازه_t i, j;
	بنية strbuf sb = STRBUF_INIT;

	إذا (لوګاریتم->ترتیبول_by_شمېره)
		STABLE_QSORT(لوګاریتم->list.توکي, لوګاریتم->list.nr,
		      لوګاریتم->summary ? compare_by_counter : compare_by_list);
	لكل (i = 0; i < لوګاریتم->list.nr; i++) {
		ثابت بنية string_list_توکی *توکی = &لوګاریتم->list.توکي[i];
		إذا (لوګاریتم->summary) {
			fprintf(لوګاریتم->file, "%6d\t%s\n",
				(صحيح)UTIL_منزل_INT(توکی), توکی->string);
		} وإلا {
			بنية string_list *onelines = توکی->util;
			fprintf(لوګاریتم->file, "%s (%"PRIuMAX"):\n",
				توکی->string, (uintmax_t)onelines->nr);
			لكل (j = onelines->nr; j >= 1; j--) {
				ثابت حرف *پیغام = onelines->توکي[j - 1].string;

				إذا (لوګاریتم->wrap_کرښې) {
					strbuf_reset(&sb);
					جمع_کړه_wrapped_shortlog_پیغام(&sb, پیغام, لوګاریتم);
					fwrite(sb.buf, sb.len, 1, لوګاریتم->file);
				}
				وإلا
					fprintf(لوګاریتم->file, "      %s\n", پیغام);
			}
			putc('\n', لوګاریتم->file);
			onelines->strdup_مزي_ګانې = 1;
			string_list_clear(onelines, 0);
			حرر(onelines);
		}

		لوګاریتم->list.توکي[i].util = عدم;
	}

	strbuf_release(&sb);
	لوګاریتم->list.strdup_مزي_ګانې = 1;
	string_list_clear(&لوګاریتم->list, 1);
	clear_mailmap(&لوګاریتم->mailmap);
	string_list_clear(&لوګاریتم->format, 0);
	string_list_clear(&لوګاریتم->trailers, 0);
}
