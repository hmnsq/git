#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "mailmap.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "shortlog.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "trailer.h"
#అన్తర్భావ "strmap.h"

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ఖాళీ
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
#నిర్వచించు UTIL_గమ్యం_INT(x) ((intptr_t)(x)->util)

స్థిర పూర్ణాంకం compare_by_counter(స్థిరరాశి శూన్య *a1, స్థిరరాశి శూన్య *a2)
{
	స్థిరరాశి నిర్మాణం string_list_అంశం *i1 = a1, *i2 = a2;
	తిరిగి UTIL_గమ్యం_INT(i2) - UTIL_గమ్యం_INT(i1);
}

స్థిర పూర్ణాంకం compare_by_list(స్థిరరాశి శూన్య *a1, స్థిరరాశి శూన్య *a2)
{
	స్థిరరాశి నిర్మాణం string_list_అంశం *i1 = a1, *i2 = a2;
	స్థిరరాశి నిర్మాణం string_list *l1 = i1->util, *l2 = i2->util;

	ఒకవేళ (l1->nr < l2->nr)
		తిరిగి 1;
	లేకపోతే ఒకవేళ (l1->nr == l2->nr)
		తిరిగి 0;
	లేకపోతే
		తిరిగి -1;
}

స్థిర శూన్య చొప్పించు_one_రికార్డు(నిర్మాణం shortlog *సంవర్గమానం,
			      స్థిరరాశి అక్షరం *ident,
			      స్థిరరాశి అక్షరం *oneline)
{
	నిర్మాణం string_list_అంశం *అంశం;

	అంశం = string_list_చొప్పించు(&సంవర్గమానం->list, ident);

	ఒకవేళ (సంవర్గమానం->summary)
		అంశం->util = (శూన్య *)(UTIL_గమ్యం_INT(అంశం) + 1);
	లేకపోతే {
		అక్షరం *బఫరు;
		నిర్మాణం strbuf subject = STRBUF_INIT;
		స్థిరరాశి అక్షరం *eol;

		/* Skip any leading whitespace, including any blank lines. */
		అయితే (*oneline && ఖాళీనా(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		ఒకవేళ (!eol)
			eol = oneline + అక్షరశ్రేణి_పొడవు(oneline);
		ఒకవేళ (starts_with(oneline, "[PATCH")) {
			స్థిరరాశి అక్షరం *eob = strchr(oneline, ']');
			ఒకవేళ (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		అయితే (*oneline && ఖాళీనా(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		బఫరు = strbuf_detach(&subject, ఖాళీ);

		ఒకవేళ (!అంశం->util) {
			అంశం->util = xmalloc(పరిమాణం(నిర్మాణం string_list));
			string_list_init_nodup(అంశం->util);
		}
		string_list_append(అంశం->util, బఫరు);
	}
}

స్థిర పూర్ణాంకం parse_ident(నిర్మాణం shortlog *సంవర్గమానం,
		       నిర్మాణం strbuf *బహిర్గతం, స్థిరరాశి అక్షరం *లోపల)
{
	స్థిరరాశి అక్షరం *mailbuf, *namebuf;
	కొలత_t namelen, maillen;
	నిర్మాణం ident_విభజించు ident;

	ఒకవేళ (విభజించు_ident_పంక్తి(&ident, లోపల, అక్షరశ్రేణి_పొడవు(లోపల)))
		తిరిగి -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_చివర - ident.name_begin;
	maillen = ident.mail_చివర - ident.mail_begin;

	map_user(&సంవర్గమానం->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_కూడు(బహిర్గతం, namebuf, namelen);
	ఒకవేళ (సంవర్గమానం->email)
		strbuf_addf(బహిర్గతం, " <%.*s>", (పూర్ణాంకం)maillen, mailbuf);

	తిరిగి 0;
}

స్థిర శూన్య read_from_stdin(నిర్మాణం shortlog *సంవర్గమానం)
{
	నిర్మాణం strbuf ident = STRBUF_INIT;
	నిర్మాణం strbuf mapped_ident = STRBUF_INIT;
	నిర్మాణం strbuf oneline = STRBUF_INIT;
	స్థిర స్థిరరాశి అక్షరం *author_సరిపోలిక[2] = { "Author: ", "author " };
	స్థిర స్థిరరాశి అక్షరం *committer_సరిపోలిక[2] = { "Commit: ", "committer " };
	స్థిరరాశి అక్షరం **సరిపోలిక;

	ఒకవేళ (HAS_MULTI_బిట్లు(సంవర్గమానం->groups))
		die(_("using multiple --group options with stdin is not supported"));

	ఎంపిక (సంవర్గమానం->groups) {
	స్థితి SHORTLOG_సమూహం_AUTHOR:
		సరిపోలిక = author_సరిపోలిక;
		ఆపు;
	స్థితి SHORTLOG_సమూహం_COMMITTER:
		సరిపోలిక = committer_సరిపోలిక;
		ఆపు;
	స్థితి SHORTLOG_సమూహం_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	స్థితి SHORTLOG_సమూహం_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	అప్రమేయ:
		BUG("unhandled shortlog group");
	}

	అయితే (strbuf_getline_lf(&ident, stdin) != EOF) {
		స్థిరరాశి అక్షరం *v;
		ఒకవేళ (!skip_ఉపసర్గ(ident.buf, సరిపోలిక[0], &v) &&
		    !skip_ఉపసర్గ(ident.buf, సరిపోలిక[1], &v))
			కొనసాగు;
		అయితే (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		అయితే (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		ఒకవేళ (parse_ident(సంవర్గమానం, &mapped_ident, v) < 0)
			కొనసాగు;

		చొప్పించు_one_రికార్డు(సంవర్గమానం, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

స్థిర శూన్య చొప్పించు_రికార్డులు_from_trailers(నిర్మాణం shortlog *సంవర్గమానం,
					 నిర్మాణం strset *dups,
					 నిర్మాణం commit *commit,
					 నిర్మాణం pretty_print_context *ctx,
					 స్థిరరాశి అక్షరం *oneline)
{
	నిర్మాణం trailer_iterator iter;
	స్థిరరాశి అక్షరం *commit_బఫరు, *body;
	నిర్మాణం strbuf ident = STRBUF_INIT;

	ఒకవేళ (!సంవర్గమానం->trailers.nr)
		తిరిగి;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_బఫరు = repo_logmsg_reencode(the_repository, commit, ఖాళీ,
					     ctx->output_encoding);
	body = అక్షరశ్రేణి_శోధన(commit_బఫరు, "\n\n");
	ఒకవేళ (!body)
		వెళ్ళు బహిర్గతం;

	trailer_iterator_init(&iter, body);
	అయితే (trailer_iterator_advance(&iter)) {
		స్థిరరాశి అక్షరం *విలువ = iter.val.buf;

		ఒకవేళ (!string_list_has_string(&సంవర్గమానం->trailers, iter.కీలకం.buf))
			కొనసాగు;

		strbuf_reset(&ident);
		ఒకవేళ (!parse_ident(సంవర్గమానం, &ident, విలువ))
			విలువ = ident.buf;

		ఒకవేళ (!strset_కూడు(dups, విలువ))
			కొనసాగు;
		చొప్పించు_one_రికార్డు(సంవర్గమానం, విలువ, oneline);
	}
	trailer_iterator_release(&iter);

బహిర్గతం:
	strbuf_release(&ident);
	repo_unuse_commit_బఫరు(the_repository, commit, commit_బఫరు);
}

స్థిర పూర్ణాంకం shortlog_needs_dedup(స్థిరరాశి నిర్మాణం shortlog *సంవర్గమానం)
{
	తిరిగి HAS_MULTI_బిట్లు(సంవర్గమానం->groups) || సంవర్గమానం->format.nr > 1 || సంవర్గమానం->trailers.nr;
}

స్థిర శూన్య చొప్పించు_రికార్డులు_from_format(నిర్మాణం shortlog *సంవర్గమానం,
				       నిర్మాణం strset *dups,
				       నిర్మాణం commit *commit,
				       నిర్మాణం pretty_print_context *ctx,
				       స్థిరరాశి అక్షరం *oneline)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం string_list_అంశం *అంశం;

	for_each_string_list_అంశం(అంశం, &సంవర్గమానం->format) {
		strbuf_reset(&buf);

		repo_format_commit_సందేశం(the_repository, commit,
					   అంశం->string, &buf, ctx);

		ఒకవేళ (!shortlog_needs_dedup(సంవర్గమానం) || strset_కూడు(dups, buf.buf))
			చొప్పించు_one_రికార్డు(సంవర్గమానం, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

శూన్య shortlog_కూడు_commit(నిర్మాణం shortlog *సంవర్గమానం, నిర్మాణం commit *commit)
{
	నిర్మాణం strbuf oneline = STRBUF_INIT;
	నిర్మాణం strset dups = STRSET_INIT;
	నిర్మాణం pretty_print_context ctx = {0};
	స్థిరరాశి అక్షరం *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = సంవర్గమానం->abbrev;
	ctx.date_బహుళకం = సంవర్గమానం->date_బహుళకం;
	ctx.output_encoding = get_log_output_encoding();

	ఒకవేళ (!సంవర్గమానం->summary) {
		ఒకవేళ (సంవర్గమానం->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		లేకపోతే
			repo_format_commit_సందేశం(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	చొప్పించు_రికార్డులు_from_trailers(సంవర్గమానం, &dups, commit, &ctx, oneline_str);
	చొప్పించు_రికార్డులు_from_format(సంవర్గమానం, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

స్థిర శూన్య get_from_rev(నిర్మాణం rev_info *rev, నిర్మాణం shortlog *సంవర్గమానం)
{
	నిర్మాణం commit *commit;

	ఒకవేళ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	అయితే ((commit = get_revision(rev)) != ఖాళీ)
		shortlog_కూడు_commit(సంవర్గమానం, commit);
}

స్థిర పూర్ణాంకం parse_uint(అక్షరం స్థిరరాశి **arg, పూర్ణాంకం comma, పూర్ణాంకం defval)
{
	గుర్తులేని దీర్ఘ ul;
	పూర్ణాంకం ret;
	అక్షరం *endp;

	ul = strtoul(*arg, &endp, 10);
	ఒకవేళ (*endp && *endp != comma)
		తిరిగి -1;
	ఒకవేళ (ul > గరిష్ఠ_పూర్ణాంకం)
		తిరిగి -1;
	ret = *arg == endp ? defval : (పూర్ణాంకం)ul;
	*arg = *endp ? endp + 1 : endp;
	తిరిగి ret;
}

స్థిర స్థిరరాశి అక్షరం wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#నిర్వచించు DEFAULT_WRAPLEN 76
#నిర్వచించు DEFAULT_INDENT1 6
#నిర్వచించు DEFAULT_INDENT2 9

స్థిర పూర్ణాంకం parse_wrap_వాదనలు(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం shortlog *సంవర్గమానం = opt->విలువ;

	సంవర్గమానం->wrap_పంక్తులు = !unset;
	ఒకవేళ (unset)
		తిరిగి 0;
	ఒకవేళ (!arg) {
		సంవర్గమానం->wrap = DEFAULT_WRAPLEN;
		సంవర్గమానం->in1 = DEFAULT_INDENT1;
		సంవర్గమానం->in2 = DEFAULT_INDENT2;
		తిరిగి 0;
	}

	సంవర్గమానం->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	సంవర్గమానం->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	సంవర్గమానం->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	ఒకవేళ (సంవర్గమానం->wrap < 0 || సంవర్గమానం->in1 < 0 || సంవర్గమానం->in2 < 0)
		తిరిగి error(wrap_arg_usage);
	ఒకవేళ (సంవర్గమానం->wrap &&
	    ((సంవర్గమానం->in1 && సంవర్గమానం->wrap <= సంవర్గమానం->in1) ||
	     (సంవర్గమానం->in2 && సంవర్గమానం->wrap <= సంవర్గమానం->in2)))
		తిరిగి error(wrap_arg_usage);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_సమూహం_option(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం shortlog *సంవర్గమానం = opt->విలువ;
	స్థిరరాశి అక్షరం *క్షేత్రం;

	ఒకవేళ (unset) {
		సంవర్గమానం->groups = 0;
		string_list_clear(&సంవర్గమానం->trailers, 0);
		string_list_clear(&సంవర్గమానం->format, 0);
	} లేకపోతే ఒకవేళ (!strcasecmp(arg, "author"))
		సంవర్గమానం->groups |= SHORTLOG_సమూహం_AUTHOR;
	లేకపోతే ఒకవేళ (!strcasecmp(arg, "committer"))
		సంవర్గమానం->groups |= SHORTLOG_సమూహం_COMMITTER;
	లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "trailer:", &క్షేత్రం)) {
		సంవర్గమానం->groups |= SHORTLOG_సమూహం_TRAILER;
		string_list_append(&సంవర్గమానం->trailers, క్షేత్రం);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "format:", &క్షేత్రం)) {
		సంవర్గమానం->groups |= SHORTLOG_సమూహం_FORMAT;
		string_list_append(&సంవర్గమానం->format, క్షేత్రం);
	} లేకపోతే ఒకవేళ (strchr(arg, '%')) {
		సంవర్గమానం->groups |= SHORTLOG_సమూహం_FORMAT;
		string_list_append(&సంవర్గమానం->format, arg);
	} లేకపోతే {
		తిరిగి error(_("unknown group type: %s"), arg);
	}

	తిరిగి 0;
}


శూన్య shortlog_init(నిర్మాణం shortlog *సంవర్గమానం)
{
	memset(సంవర్గమానం, 0, పరిమాణం(*సంవర్గమానం));

	read_mailmap(the_repository, &సంవర్గమానం->mailmap);

	సంవర్గమానం->list.strdup_అక్షరశ్రేణులు = 1;
	సంవర్గమానం->wrap = DEFAULT_WRAPLEN;
	సంవర్గమానం->in1 = DEFAULT_INDENT1;
	సంవర్గమానం->in2 = DEFAULT_INDENT2;
	సంవర్గమానం->trailers.strdup_అక్షరశ్రేణులు = 1;
	సంవర్గమానం->trailers.cmp = strcasecmp;
	సంవర్గమానం->format.strdup_అక్షరశ్రేణులు = 1;
}

శూన్య shortlog_ముగింపు_అమరిక(నిర్మాణం shortlog *సంవర్గమానం)
{
	ఒకవేళ (సంవర్గమానం->groups & SHORTLOG_సమూహం_AUTHOR)
		string_list_append(&సంవర్గమానం->format,
				   సంవర్గమానం->email ? "%aN <%aE>" : "%aN");
	ఒకవేళ (సంవర్గమానం->groups & SHORTLOG_సమూహం_COMMITTER)
		string_list_append(&సంవర్గమానం->format,
				   సంవర్గమానం->email ? "%cN <%cE>" : "%cN");

	string_list_క్రమీకరించు(&సంవర్గమానం->trailers);
}

పూర్ణాంకం cmd_shortlog(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం shortlog సంవర్గమానం = { STRING_LIST_INIT_NODUP };
	నిర్మాణం rev_info rev;
	పూర్ణాంకం nongit = !startup_info->have_repository;

	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_బిట్టు('c', "committer", &సంవర్గమానం.groups,
			N_("group by committer rather than author"),
			SHORTLOG_సమూహం_COMMITTER),
		OPT_BOOL('n', "numbered", &సంవర్గమానం.క్రమీకరించు_by_సంఖ్య,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &సంవర్గమానం.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &సంవర్గమానం.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ఖాళీ, &సంవర్గమానం, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_వాదనలు),
		OPT_CALLBACK(0, "group", &సంవర్గమానం, N_("field"),
			N_("group by field"), parse_సమూహం_option),
		OPT_చివర(),
	};

	నిర్మాణం parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	ఒకవేళ (nongit && !the_hash_algo)
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ఖాళీ);
	shortlog_init(&సంవర్గమానం);
	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	parse_options_ప్రారంభం(&ctx, argc, argv, ఉపసర్గ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	కొరకు (;;) {
		ఎంపిక (parse_options_అడుగు(&ctx, options, shortlog_usage)) {
		స్థితి PARSE_OPT_NON_OPTION:
		స్థితి PARSE_OPT_UNKNOWN:
			ఆపు;
		స్థితి PARSE_OPT_HELP:
			నిష్క్రమించు(0);
		స్థితి PARSE_OPT_HELP_ERROR:
		స్థితి PARSE_OPT_ERROR:
		స్థితి PARSE_OPT_SUBCOMMAND:
			నిష్క్రమించు(129);
		స్థితి PARSE_OPT_COMPLETE:
			నిష్క్రమించు(0);
		స్థితి PARSE_OPT_పూర్తి:
			వెళ్ళు parse_పూర్తి;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_పూర్తి:
	revision_opts_ముగింపు(&rev);
	argc = parse_options_చివర(&ctx);

	ఒకవేళ (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	ఒకవేళ (!nongit && అమరిక_revisions(argc, argv, &rev, ఖాళీ) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	సంవర్గమానం.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	సంవర్గమానం.abbrev = rev.abbrev;
	సంవర్గమానం.file = rev.diffopt.file;
	సంవర్గమానం.date_బహుళకం = rev.date_బహుళకం;

	ఒకవేళ (!సంవర్గమానం.groups)
		సంవర్గమానం.groups = SHORTLOG_సమూహం_AUTHOR;
	shortlog_ముగింపు_అమరిక(&సంవర్గమానం);

	/* assume HEAD if from a tty */
	ఒకవేళ (!nongit && !rev.pending.nr && isatty(0))
		కూడు_తల_గమ్యం_pending(&rev);
	ఒకవేళ (rev.pending.nr == 0) {
		ఒకవేళ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&సంవర్గమానం);
	}
	లేకపోతే
		get_from_rev(&rev, &సంవర్గమానం);

	shortlog_output(&సంవర్గమానం);
	release_revisions(&rev);
	తిరిగి 0;
}

స్థిర శూన్య కూడు_wrapped_shortlog_సందేశం(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *s,
				     స్థిరరాశి నిర్మాణం shortlog *సంవర్గమానం)
{
	strbuf_కూడు_wrapped_పాఠ్యం(sb, s, సంవర్గమానం->in1, సంవర్గమానం->in2, సంవర్గమానం->wrap);
	strbuf_addch(sb, '\n');
}

శూన్య shortlog_output(నిర్మాణం shortlog *సంవర్గమానం)
{
	కొలత_t i, j;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (సంవర్గమానం->క్రమీకరించు_by_సంఖ్య)
		STABLE_QSORT(సంవర్గమానం->list.వస్తువులు, సంవర్గమానం->list.nr,
		      సంవర్గమానం->summary ? compare_by_counter : compare_by_list);
	కొరకు (i = 0; i < సంవర్గమానం->list.nr; i++) {
		స్థిరరాశి నిర్మాణం string_list_అంశం *అంశం = &సంవర్గమానం->list.వస్తువులు[i];
		ఒకవేళ (సంవర్గమానం->summary) {
			fprintf(సంవర్గమానం->file, "%6d\t%s\n",
				(పూర్ణాంకం)UTIL_గమ్యం_INT(అంశం), అంశం->string);
		} లేకపోతే {
			నిర్మాణం string_list *onelines = అంశం->util;
			fprintf(సంవర్గమానం->file, "%s (%"PRIuMAX"):\n",
				అంశం->string, (uintmax_t)onelines->nr);
			కొరకు (j = onelines->nr; j >= 1; j--) {
				స్థిరరాశి అక్షరం *సందేశం = onelines->వస్తువులు[j - 1].string;

				ఒకవేళ (సంవర్గమానం->wrap_పంక్తులు) {
					strbuf_reset(&sb);
					కూడు_wrapped_shortlog_సందేశం(&sb, సందేశం, సంవర్గమానం);
					fwrite(sb.buf, sb.len, 1, సంవర్గమానం->file);
				}
				లేకపోతే
					fprintf(సంవర్గమానం->file, "      %s\n", సందేశం);
			}
			putc('\n', సంవర్గమానం->file);
			onelines->strdup_అక్షరశ్రేణులు = 1;
			string_list_clear(onelines, 0);
			విడుదల(onelines);
		}

		సంవర్గమానం->list.వస్తువులు[i].util = ఖాళీ;
	}

	strbuf_release(&sb);
	సంవర్గమానం->list.strdup_అక్షరశ్రేణులు = 1;
	string_list_clear(&సంవర్గమానం->list, 1);
	clear_mailmap(&సంవర్గమానం->mailmap);
	string_list_clear(&సంవర్గమానం->format, 0);
	string_list_clear(&సంవర్గమానం->trailers, 0);
}
