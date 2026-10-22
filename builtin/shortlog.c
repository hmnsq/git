#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "commit.h"
#समावेश "diff.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "string-list.h"
#समावेश "revision.h"
#समावेश "utf8.h"
#समावेश "mailmap.h"
#समावेश "setup.h"
#समावेश "shortlog.h"
#समावेश "parse-options.h"
#समावेश "trailer.h"
#समावेश "strmap.h"

अटल अक्षर स्थिर * स्थिर shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	शून्य
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
#घोषणा UTIL_TO_INT(x) ((intptr_t)(x)->util)

अटल पूर्णांक compare_by_गणक(स्थिर व्योम *a1, स्थिर व्योम *a2)
{
	स्थिर काष्ठा string_list_item *i1 = a1, *i2 = a2;
	वापस UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

अटल पूर्णांक compare_by_list(स्थिर व्योम *a1, स्थिर व्योम *a2)
{
	स्थिर काष्ठा string_list_item *i1 = a1, *i2 = a2;
	स्थिर काष्ठा string_list *l1 = i1->util, *l2 = i2->util;

	अगर (l1->nr < l2->nr)
		वापस 1;
	अन्यथा अगर (l1->nr == l2->nr)
		वापस 0;
	अन्यथा
		वापस -1;
}

अटल व्योम जोड़ो_one_record(काष्ठा shortlog *log,
			      स्थिर अक्षर *ident,
			      स्थिर अक्षर *oneline)
{
	काष्ठा string_list_item *item;

	item = string_list_जोड़ो(&log->list, ident);

	अगर (log->summary)
		item->util = (व्योम *)(UTIL_TO_INT(item) + 1);
	अन्यथा {
		अक्षर *बफर;
		काष्ठा strbuf subject = STRBUF_INIT;
		स्थिर अक्षर *eol;

		/* Skip any leading whitespace, including any blank lines. */
		जबतक (*oneline && है_खाली(*oneline))
			oneline++;
		eol = म_अक्षर(oneline, '\n');
		अगर (!eol)
			eol = oneline + म_माप(oneline);
		अगर (starts_with(oneline, "[PATCH")) {
			स्थिर अक्षर *eob = म_अक्षर(oneline, ']');
			अगर (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		जबतक (*oneline && है_खाली(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		बफर = strbuf_detach(&subject, शून्य);

		अगर (!item->util) {
			item->util = xmalloc(माप(काष्ठा string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, बफर);
	}
}

अटल पूर्णांक parse_ident(काष्ठा shortlog *log,
		       काष्ठा strbuf *out, स्थिर अक्षर *in)
{
	स्थिर अक्षर *mailbuf, *namebuf;
	माप_प्रकार namelen, maillen;
	काष्ठा ident_split ident;

	अगर (split_ident_पंक्ति(&ident, in, म_माप(in)))
		वापस -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	अगर (log->email)
		strbuf_addf(out, " <%.*s>", (पूर्णांक)maillen, mailbuf);

	वापस 0;
}

अटल व्योम read_from_stdin(काष्ठा shortlog *log)
{
	काष्ठा strbuf ident = STRBUF_INIT;
	काष्ठा strbuf mapped_ident = STRBUF_INIT;
	काष्ठा strbuf oneline = STRBUF_INIT;
	अटल स्थिर अक्षर *author_match[2] = { "Author: ", "author " };
	अटल स्थिर अक्षर *committer_match[2] = { "Commit: ", "committer " };
	स्थिर अक्षर **match;

	अगर (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	चयन (log->groups) {
	हाल SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		अवरोध;
	हाल SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		अवरोध;
	हाल SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	हाल SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	शेष:
		BUG("unhandled shortlog group");
	}

	जबतक (strbuf_getline_lf(&ident, मानक_निवेश) != खातापूर्ण) {
		स्थिर अक्षर *v;
		अगर (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			जारी;
		जबतक (strbuf_getline_lf(&oneline, मानक_निवेश) != खातापूर्ण &&
		       oneline.len)
			; /* discard headers */
		जबतक (strbuf_getline_lf(&oneline, मानक_निवेश) != खातापूर्ण &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		अगर (parse_ident(log, &mapped_ident, v) < 0)
			जारी;

		जोड़ो_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

अटल व्योम जोड़ो_records_from_trailers(काष्ठा shortlog *log,
					 काष्ठा strset *dups,
					 काष्ठा commit *commit,
					 काष्ठा pretty_print_context *ctx,
					 स्थिर अक्षर *oneline)
{
	काष्ठा trailer_iterator iter;
	स्थिर अक्षर *commit_बफर, *body;
	काष्ठा strbuf ident = STRBUF_INIT;

	अगर (!log->trailers.nr)
		वापस;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_बफर = repo_logmsg_reencode(the_repository, commit, शून्य,
					     ctx->output_encoding);
	body = म_माला(commit_बफर, "\n\n");
	अगर (!body)
		जाओ out;

	trailer_iterator_init(&iter, body);
	जबतक (trailer_iterator_advance(&iter)) {
		स्थिर अक्षर *मूल्य = iter.val.buf;

		अगर (!string_list_has_string(&log->trailers, iter.कुंजी.buf))
			जारी;

		strbuf_reset(&ident);
		अगर (!parse_ident(log, &ident, मूल्य))
			मूल्य = ident.buf;

		अगर (!strset_add(dups, मूल्य))
			जारी;
		जोड़ो_one_record(log, मूल्य, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_बफर(the_repository, commit, commit_बफर);
}

अटल पूर्णांक shortlog_needs_dedup(स्थिर काष्ठा shortlog *log)
{
	वापस HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

अटल व्योम जोड़ो_records_from_format(काष्ठा shortlog *log,
				       काष्ठा strset *dups,
				       काष्ठा commit *commit,
				       काष्ठा pretty_print_context *ctx,
				       स्थिर अक्षर *oneline)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		अगर (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			जोड़ो_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

व्योम shortlog_add_commit(काष्ठा shortlog *log, काष्ठा commit *commit)
{
	काष्ठा strbuf oneline = STRBUF_INIT;
	काष्ठा strset dups = STRSET_INIT;
	काष्ठा pretty_print_context ctx = {0};
	स्थिर अक्षर *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	अगर (!log->summary) {
		अगर (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		अन्यथा
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	जोड़ो_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	जोड़ो_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

अटल व्योम get_from_rev(काष्ठा rev_info *rev, काष्ठा shortlog *log)
{
	काष्ठा commit *commit;

	अगर (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	जबतक ((commit = get_revision(rev)) != शून्य)
		shortlog_add_commit(log, commit);
}

अटल पूर्णांक parse_uint(अक्षर स्थिर **arg, पूर्णांक comma, पूर्णांक defval)
{
	अचिन्हित दीर्घ ul;
	पूर्णांक ret;
	अक्षर *endp;

	ul = म_से_अदीर्घ(*arg, &endp, 10);
	अगर (*endp && *endp != comma)
		वापस -1;
	अगर (ul > पूर्णांक_उच्च)
		वापस -1;
	ret = *arg == endp ? defval : (पूर्णांक)ul;
	*arg = *endp ? endp + 1 : endp;
	वापस ret;
}

अटल स्थिर अक्षर wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#घोषणा DEFAULT_WRAPLEN 76
#घोषणा DEFAULT_INDENT1 6
#घोषणा DEFAULT_INDENT2 9

अटल पूर्णांक parse_wrap_args(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा shortlog *log = opt->मूल्य;

	log->wrap_पंक्तियाँ = !unset;
	अगर (unset)
		वापस 0;
	अगर (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		वापस 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	अगर (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		वापस error(wrap_arg_usage);
	अगर (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		वापस error(wrap_arg_usage);
	वापस 0;
}

अटल पूर्णांक parse_group_option(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा shortlog *log = opt->मूल्य;
	स्थिर अक्षर *field;

	अगर (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} अन्यथा अगर (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	अन्यथा अगर (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	अन्यथा अगर (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} अन्यथा अगर (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} अन्यथा अगर (म_अक्षर(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} अन्यथा {
		वापस error(_("unknown group type: %s"), arg);
	}

	वापस 0;
}


व्योम shortlog_init(काष्ठा shortlog *log)
{
	स_रखो(log, 0, माप(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

व्योम shortlog_finish_setup(काष्ठा shortlog *log)
{
	अगर (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	अगर (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

पूर्णांक cmd_shortlog(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा shortlog log = { STRING_LIST_INIT_NODUP };
	काष्ठा rev_info rev;
	पूर्णांक nongit = !startup_info->have_repository;

	स्थिर काष्ठा option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_संख्या,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', शून्य, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
	};

	काष्ठा parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	अगर (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, शून्य);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	क्रम (;;) {
		चयन (parse_options_step(&ctx, options, shortlog_usage)) {
		हाल PARSE_OPT_NON_OPTION:
		हाल PARSE_OPT_UNKNOWN:
			अवरोध;
		हाल PARSE_OPT_HELP:
			निकास(0);
		हाल PARSE_OPT_HELP_ERROR:
		हाल PARSE_OPT_ERROR:
		हाल PARSE_OPT_SUBCOMMAND:
			निकास(129);
		हाल PARSE_OPT_COMPLETE:
			निकास(0);
		हाल PARSE_OPT_DONE:
			जाओ parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	अगर (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	अगर (!nongit && setup_revisions(argc, argv, &rev, शून्य) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	अगर (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	अगर (!nongit && !rev.pending.nr && isatty(0))
		add_शीर्ष_to_pending(&rev);
	अगर (rev.pending.nr == 0) {
		अगर (isatty(0))
			ख_लिखो(मानक_त्रुटि, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	अन्यथा
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	वापस 0;
}

अटल व्योम add_wrapped_shortlog_संदेश(काष्ठा strbuf *sb, स्थिर अक्षर *s,
				     स्थिर काष्ठा shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

व्योम shortlog_output(काष्ठा shortlog *log)
{
	माप_प्रकार i, j;
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (log->sort_by_संख्या)
		STABLE_QSORT(log->list.वस्तुएँ, log->list.nr,
		      log->summary ? compare_by_गणक : compare_by_list);
	क्रम (i = 0; i < log->list.nr; i++) {
		स्थिर काष्ठा string_list_item *item = &log->list.वस्तुएँ[i];
		अगर (log->summary) {
			ख_लिखो(log->file, "%6d\t%s\n",
				(पूर्णांक)UTIL_TO_INT(item), item->string);
		} अन्यथा {
			काष्ठा string_list *onelines = item->util;
			ख_लिखो(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			क्रम (j = onelines->nr; j >= 1; j--) {
				स्थिर अक्षर *संदेश = onelines->वस्तुएँ[j - 1].string;

				अगर (log->wrap_पंक्तियाँ) {
					strbuf_reset(&sb);
					add_wrapped_shortlog_संदेश(&sb, संदेश, log);
					ख_डालो(sb.buf, sb.len, 1, log->file);
				}
				अन्यथा
					ख_लिखो(log->file, "      %s\n", संदेश);
			}
			अ_दो('\n', log->file);
			onelines->strdup_strings = 1;
			string_list_clear(onelines, 0);
			मुक्त(onelines);
		}

		log->list.वस्तुएँ[i].util = शून्य;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
