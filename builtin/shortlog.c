#परिभाषय USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "shortlog.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "trailer.h"
#अन्तर्भाव "strmap.h"

स्थिर वर्ण ध्रुव * ध्रुव shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	रिक्त
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
#परिभाषय UTIL_गन्तव्यम्_INT(x) ((intptr_t)(x)->util)

स्थिर पूर्णाङ्क compare_by_गणक(ध्रुव शून्य *a1, ध्रुव शून्य *a2)
{
	ध्रुव संरचना string_list_item *i1 = a1, *i2 = a2;
	प्रत्यागम UTIL_गन्तव्यम्_INT(i2) - UTIL_गन्तव्यम्_INT(i1);
}

स्थिर पूर्णाङ्क compare_by_list(ध्रुव शून्य *a1, ध्रुव शून्य *a2)
{
	ध्रुव संरचना string_list_item *i1 = a1, *i2 = a2;
	ध्रुव संरचना string_list *l1 = i1->util, *l2 = i2->util;

	यदि (l1->nr < l2->nr)
		प्रत्यागम 1;
	अन्यथा यदि (l1->nr == l2->nr)
		प्रत्यागम 0;
	अन्यथा
		प्रत्यागम -1;
}

स्थिर शून्य निवेशय_one_लेखः(संरचना shortlog *लघुगणकः,
			      ध्रुव वर्ण *ident,
			      ध्रुव वर्ण *oneline)
{
	संरचना string_list_item *item;

	item = string_list_निवेशय(&लघुगणकः->list, ident);

	यदि (लघुगणकः->summary)
		item->util = (शून्य *)(UTIL_गन्तव्यम्_INT(item) + 1);
	अन्यथा {
		वर्ण *बफर;
		संरचना strbuf subject = STRBUF_INIT;
		ध्रुव वर्ण *eol;

		/* Skip any leading whitespace, including any blank lines. */
		यावत् (*oneline && रिक्तम्_वा(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		यदि (!eol)
			eol = oneline + वाक्यदैर्घ्यम्(oneline);
		यदि (starts_with(oneline, "[PATCH")) {
			ध्रुव वर्ण *eob = strchr(oneline, ']');
			यदि (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		यावत् (*oneline && रिक्तम्_वा(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		बफर = strbuf_detach(&subject, रिक्त);

		यदि (!item->util) {
			item->util = xmalloc(परिमाण(संरचना string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, बफर);
	}
}

स्थिर पूर्णाङ्क parse_ident(संरचना shortlog *लघुगणकः,
		       संरचना strbuf *बहिर्गमनम्, ध्रुव वर्ण *अन्तर्)
{
	ध्रुव वर्ण *mailbuf, *namebuf;
	आकार_t namelen, maillen;
	संरचना ident_विभज ident;

	यदि (विभज_ident_पङ्क्तिः(&ident, अन्तर्, वाक्यदैर्घ्यम्(अन्तर्)))
		प्रत्यागम -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_अन्तः - ident.name_begin;
	maillen = ident.mail_अन्तः - ident.mail_begin;

	map_user(&लघुगणकः->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_सङ्कलय(बहिर्गमनम्, namebuf, namelen);
	यदि (लघुगणकः->email)
		strbuf_addf(बहिर्गमनम्, " <%.*s>", (पूर्णाङ्क)maillen, mailbuf);

	प्रत्यागम 0;
}

स्थिर शून्य read_from_stdin(संरचना shortlog *लघुगणकः)
{
	संरचना strbuf ident = STRBUF_INIT;
	संरचना strbuf mapped_ident = STRBUF_INIT;
	संरचना strbuf oneline = STRBUF_INIT;
	स्थिर ध्रुव वर्ण *author_मेलनम्[2] = { "Author: ", "author " };
	स्थिर ध्रुव वर्ण *committer_मेलनम्[2] = { "Commit: ", "committer " };
	ध्रुव वर्ण **मेलनम्;

	यदि (HAS_MULTI_द्वयङ्काः(लघुगणकः->groups))
		die(_("using multiple --group options with stdin is not supported"));

	विकल्प (लघुगणकः->groups) {
	पक्ष SHORTLOG_गणः_AUTHOR:
		मेलनम् = author_मेलनम्;
		भङ्ग;
	पक्ष SHORTLOG_गणः_COMMITTER:
		मेलनम् = committer_मेलनम्;
		भङ्ग;
	पक्ष SHORTLOG_गणः_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	पक्ष SHORTLOG_गणः_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	स्वभाव:
		BUG("unhandled shortlog group");
	}

	यावत् (strbuf_getline_lf(&ident, stdin) != EOF) {
		ध्रुव वर्ण *v;
		यदि (!skip_उपसर्गः(ident.buf, मेलनम्[0], &v) &&
		    !skip_उपसर्गः(ident.buf, मेलनम्[1], &v))
			अनुवर्त;
		यावत् (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		यावत् (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		यदि (parse_ident(लघुगणकः, &mapped_ident, v) < 0)
			अनुवर्त;

		निवेशय_one_लेखः(लघुगणकः, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

स्थिर शून्य निवेशय_लेखाः_from_trailers(संरचना shortlog *लघुगणकः,
					 संरचना strset *dups,
					 संरचना commit *commit,
					 संरचना pretty_print_context *ctx,
					 ध्रुव वर्ण *oneline)
{
	संरचना trailer_iterator iter;
	ध्रुव वर्ण *commit_बफर, *body;
	संरचना strbuf ident = STRBUF_INIT;

	यदि (!लघुगणकः->trailers.nr)
		प्रत्यागम;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_बफर = repo_logmsg_reencode(the_repository, commit, रिक्त,
					     ctx->output_encoding);
	body = वाक्ये_अन्विष(commit_बफर, "\n\n");
	यदि (!body)
		गच्छ बहिर्गमनम्;

	trailer_iterator_init(&iter, body);
	यावत् (trailer_iterator_advance(&iter)) {
		ध्रुव वर्ण *मानम् = iter.val.buf;

		यदि (!string_list_has_string(&लघुगणकः->trailers, iter.कुञ्चिका.buf))
			अनुवर्त;

		strbuf_reset(&ident);
		यदि (!parse_ident(लघुगणकः, &ident, मानम्))
			मानम् = ident.buf;

		यदि (!strset_सङ्कलय(dups, मानम्))
			अनुवर्त;
		निवेशय_one_लेखः(लघुगणकः, मानम्, oneline);
	}
	trailer_iterator_release(&iter);

बहिर्गमनम्:
	strbuf_release(&ident);
	repo_unuse_commit_बफर(the_repository, commit, commit_बफर);
}

स्थिर पूर्णाङ्क shortlog_needs_dedup(ध्रुव संरचना shortlog *लघुगणकः)
{
	प्रत्यागम HAS_MULTI_द्वयङ्काः(लघुगणकः->groups) || लघुगणकः->format.nr > 1 || लघुगणकः->trailers.nr;
}

स्थिर शून्य निवेशय_लेखाः_from_format(संरचना shortlog *लघुगणकः,
				       संरचना strset *dups,
				       संरचना commit *commit,
				       संरचना pretty_print_context *ctx,
				       ध्रुव वर्ण *oneline)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;

	for_each_string_list_item(item, &लघुगणकः->format) {
		strbuf_reset(&buf);

		repo_format_commit_सन्देशः(the_repository, commit,
					   item->string, &buf, ctx);

		यदि (!shortlog_needs_dedup(लघुगणकः) || strset_सङ्कलय(dups, buf.buf))
			निवेशय_one_लेखः(लघुगणकः, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

शून्य shortlog_सङ्कलय_commit(संरचना shortlog *लघुगणकः, संरचना commit *commit)
{
	संरचना strbuf oneline = STRBUF_INIT;
	संरचना strset dups = STRSET_INIT;
	संरचना pretty_print_context ctx = {0};
	ध्रुव वर्ण *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = लघुगणकः->abbrev;
	ctx.date_बहुलकम् = लघुगणकः->date_बहुलकम्;
	ctx.output_encoding = get_log_output_encoding();

	यदि (!लघुगणकः->summary) {
		यदि (लघुगणकः->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		अन्यथा
			repo_format_commit_सन्देशः(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	निवेशय_लेखाः_from_trailers(लघुगणकः, &dups, commit, &ctx, oneline_str);
	निवेशय_लेखाः_from_format(लघुगणकः, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

स्थिर शून्य get_from_rev(संरचना rev_info *rev, संरचना shortlog *लघुगणकः)
{
	संरचना commit *commit;

	यदि (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	यावत् ((commit = get_revision(rev)) != रिक्त)
		shortlog_सङ्कलय_commit(लघुगणकः, commit);
}

स्थिर पूर्णाङ्क parse_uint(वर्ण ध्रुव **arg, पूर्णाङ्क comma, पूर्णाङ्क defval)
{
	अचिह्नित दीर्घ ul;
	पूर्णाङ्क ret;
	वर्ण *endp;

	ul = strtoul(*arg, &endp, 10);
	यदि (*endp && *endp != comma)
		प्रत्यागम -1;
	यदि (ul > अधिकतम_पूर्णाङ्कः)
		प्रत्यागम -1;
	ret = *arg == endp ? defval : (पूर्णाङ्क)ul;
	*arg = *endp ? endp + 1 : endp;
	प्रत्यागम ret;
}

स्थिर ध्रुव वर्ण wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#परिभाषय DEFAULT_WRAPLEN 76
#परिभाषय DEFAULT_INDENT1 6
#परिभाषय DEFAULT_INDENT2 9

स्थिर पूर्णाङ्क parse_wrap_तर्काः(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना shortlog *लघुगणकः = opt->मानम्;

	लघुगणकः->wrap_पङ्क्तयः = !unset;
	यदि (unset)
		प्रत्यागम 0;
	यदि (!arg) {
		लघुगणकः->wrap = DEFAULT_WRAPLEN;
		लघुगणकः->in1 = DEFAULT_INDENT1;
		लघुगणकः->in2 = DEFAULT_INDENT2;
		प्रत्यागम 0;
	}

	लघुगणकः->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	लघुगणकः->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	लघुगणकः->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	यदि (लघुगणकः->wrap < 0 || लघुगणकः->in1 < 0 || लघुगणकः->in2 < 0)
		प्रत्यागम error(wrap_arg_usage);
	यदि (लघुगणकः->wrap &&
	    ((लघुगणकः->in1 && लघुगणकः->wrap <= लघुगणकः->in1) ||
	     (लघुगणकः->in2 && लघुगणकः->wrap <= लघुगणकः->in2)))
		प्रत्यागम error(wrap_arg_usage);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_गणः_option(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना shortlog *लघुगणकः = opt->मानम्;
	ध्रुव वर्ण *क्षेत्रम्;

	यदि (unset) {
		लघुगणकः->groups = 0;
		string_list_clear(&लघुगणकः->trailers, 0);
		string_list_clear(&लघुगणकः->format, 0);
	} अन्यथा यदि (!strcasecmp(arg, "author"))
		लघुगणकः->groups |= SHORTLOG_गणः_AUTHOR;
	अन्यथा यदि (!strcasecmp(arg, "committer"))
		लघुगणकः->groups |= SHORTLOG_गणः_COMMITTER;
	अन्यथा यदि (skip_उपसर्गः(arg, "trailer:", &क्षेत्रम्)) {
		लघुगणकः->groups |= SHORTLOG_गणः_TRAILER;
		string_list_append(&लघुगणकः->trailers, क्षेत्रम्);
	} अन्यथा यदि (skip_उपसर्गः(arg, "format:", &क्षेत्रम्)) {
		लघुगणकः->groups |= SHORTLOG_गणः_FORMAT;
		string_list_append(&लघुगणकः->format, क्षेत्रम्);
	} अन्यथा यदि (strchr(arg, '%')) {
		लघुगणकः->groups |= SHORTLOG_गणः_FORMAT;
		string_list_append(&लघुगणकः->format, arg);
	} अन्यथा {
		प्रत्यागम error(_("unknown group type: %s"), arg);
	}

	प्रत्यागम 0;
}


शून्य shortlog_init(संरचना shortlog *लघुगणकः)
{
	स्मृतिं_पूरय(लघुगणकः, 0, परिमाण(*लघुगणकः));

	read_mailmap(the_repository, &लघुगणकः->mailmap);

	लघुगणकः->list.strdup_वाक्यानि = 1;
	लघुगणकः->wrap = DEFAULT_WRAPLEN;
	लघुगणकः->in1 = DEFAULT_INDENT1;
	लघुगणकः->in2 = DEFAULT_INDENT2;
	लघुगणकः->trailers.strdup_वाक्यानि = 1;
	लघुगणकः->trailers.cmp = strcasecmp;
	लघुगणकः->format.strdup_वाक्यानि = 1;
}

शून्य shortlog_finish_सज्जता(संरचना shortlog *लघुगणकः)
{
	यदि (लघुगणकः->groups & SHORTLOG_गणः_AUTHOR)
		string_list_append(&लघुगणकः->format,
				   लघुगणकः->email ? "%aN <%aE>" : "%aN");
	यदि (लघुगणकः->groups & SHORTLOG_गणः_COMMITTER)
		string_list_append(&लघुगणकः->format,
				   लघुगणकः->email ? "%cN <%cE>" : "%cN");

	string_list_क्रमय(&लघुगणकः->trailers);
}

पूर्णाङ्क cmd_shortlog(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना shortlog लघुगणकः = { STRING_LIST_INIT_NODUP };
	संरचना rev_info rev;
	पूर्णाङ्क nongit = !startup_info->have_repository;

	ध्रुव संरचना option options[] = {
		OPT_BIT('c', "committer", &लघुगणकः.groups,
			N_("group by committer rather than author"),
			SHORTLOG_गणः_COMMITTER),
		OPT_BOOL('n', "numbered", &लघुगणकः.क्रमय_by_सङ्ख्या,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &लघुगणकः.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &लघुगणकः.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', रिक्त, &लघुगणकः, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_तर्काः),
		OPT_CALLBACK(0, "group", &लघुगणकः, N_("field"),
			N_("group by field"), parse_गणः_option),
		OPT_अन्तः(),
	};

	संरचना parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	यदि (nongit && !the_hash_algo)
		repo_समुच्चयः_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, रिक्त);
	shortlog_init(&लघुगणकः);
	repo_init_revisions(the_repository, &rev, उपसर्गः);
	parse_options_start(&ctx, argc, argv, उपसर्गः, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	प्रति (;;) {
		विकल्प (parse_options_चरणः(&ctx, options, shortlog_usage)) {
		पक्ष PARSE_OPT_NON_OPTION:
		पक्ष PARSE_OPT_UNKNOWN:
			भङ्ग;
		पक्ष PARSE_OPT_HELP:
			निर्गच्छ(0);
		पक्ष PARSE_OPT_HELP_ERROR:
		पक्ष PARSE_OPT_ERROR:
		पक्ष PARSE_OPT_SUBCOMMAND:
			निर्गच्छ(129);
		पक्ष PARSE_OPT_COMPLETE:
			निर्गच्छ(0);
		पक्ष PARSE_OPT_समाप्तम्:
			गच्छ parse_समाप्तम्;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_समाप्तम्:
	revision_opts_finish(&rev);
	argc = parse_options_अन्तः(&ctx);

	यदि (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	यदि (!nongit && सज्जता_revisions(argc, argv, &rev, रिक्त) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	लघुगणकः.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	लघुगणकः.abbrev = rev.abbrev;
	लघुगणकः.file = rev.diffopt.file;
	लघुगणकः.date_बहुलकम् = rev.date_बहुलकम्;

	यदि (!लघुगणकः.groups)
		लघुगणकः.groups = SHORTLOG_गणः_AUTHOR;
	shortlog_finish_सज्जता(&लघुगणकः);

	/* assume HEAD if from a tty */
	यदि (!nongit && !rev.pending.nr && isatty(0))
		सङ्कलय_शीर्ष_गन्तव्यम्_pending(&rev);
	यदि (rev.pending.nr == 0) {
		यदि (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&लघुगणकः);
	}
	अन्यथा
		get_from_rev(&rev, &लघुगणकः);

	shortlog_output(&लघुगणकः);
	release_revisions(&rev);
	प्रत्यागम 0;
}

स्थिर शून्य सङ्कलय_wrapped_shortlog_सन्देशः(संरचना strbuf *sb, ध्रुव वर्ण *s,
				     ध्रुव संरचना shortlog *लघुगणकः)
{
	strbuf_सङ्कलय_wrapped_text(sb, s, लघुगणकः->in1, लघुगणकः->in2, लघुगणकः->wrap);
	strbuf_addch(sb, '\n');
}

शून्य shortlog_output(संरचना shortlog *लघुगणकः)
{
	आकार_t i, j;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (लघुगणकः->क्रमय_by_सङ्ख्या)
		STABLE_QSORT(लघुगणकः->list.वस्तूनि, लघुगणकः->list.nr,
		      लघुगणकः->summary ? compare_by_गणक : compare_by_list);
	प्रति (i = 0; i < लघुगणकः->list.nr; i++) {
		ध्रुव संरचना string_list_item *item = &लघुगणकः->list.वस्तूनि[i];
		यदि (लघुगणकः->summary) {
			fprintf(लघुगणकः->file, "%6d\t%s\n",
				(पूर्णाङ्क)UTIL_गन्तव्यम्_INT(item), item->string);
		} अन्यथा {
			संरचना string_list *onelines = item->util;
			fprintf(लघुगणकः->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			प्रति (j = onelines->nr; j >= 1; j--) {
				ध्रुव वर्ण *सन्देशः = onelines->वस्तूनि[j - 1].string;

				यदि (लघुगणकः->wrap_पङ्क्तयः) {
					strbuf_reset(&sb);
					सङ्कलय_wrapped_shortlog_सन्देशः(&sb, सन्देशः, लघुगणकः);
					fwrite(sb.buf, sb.len, 1, लघुगणकः->file);
				}
				अन्यथा
					fprintf(लघुगणकः->file, "      %s\n", सन्देशः);
			}
			putc('\n', लघुगणकः->file);
			onelines->strdup_वाक्यानि = 1;
			string_list_clear(onelines, 0);
			मुक्त(onelines);
		}

		लघुगणकः->list.वस्तूनि[i].util = रिक्त;
	}

	strbuf_release(&sb);
	लघुगणकः->list.strdup_वाक्यानि = 1;
	string_list_clear(&लघुगणकः->list, 1);
	clear_mailmap(&लघुगणकः->mailmap);
	string_list_clear(&लघुगणकः->format, 0);
	string_list_clear(&लघुगणकः->trailers, 0);
}
