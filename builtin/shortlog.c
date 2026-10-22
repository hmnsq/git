#define USE_THE_REPOSITORY_VARIABLE

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

स्थिर अक्षर स्थिरांक * स्थिरांक shortlog_usage[] = {
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
#define UTIL_TO_INT(x) ((intptr_t)(x)->util)

स्थिर पूर्णांक compare_by_counter(स्थिरांक शून्य *a1, स्थिरांक शून्य *a2)
{
	स्थिरांक संरचना string_list_item *i1 = a1, *i2 = a2;
	परत UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
}

स्थिर पूर्णांक compare_by_list(स्थिरांक शून्य *a1, स्थिरांक शून्य *a2)
{
	स्थिरांक संरचना string_list_item *i1 = a1, *i2 = a2;
	स्थिरांक संरचना string_list *l1 = i1->util, *l2 = i2->util;

	जर (l1->nr < l2->nr)
		परत 1;
	अन्यथा जर (l1->nr == l2->nr)
		परत 0;
	अन्यथा
		परत -1;
}

स्थिर शून्य घाला_one_record(संरचना shortlog *log,
			      स्थिरांक अक्षर *ident,
			      स्थिरांक अक्षर *oneline)
{
	संरचना string_list_item *item;

	item = string_list_घाला(&log->list, ident);

	जर (log->summary)
		item->util = (शून्य *)(UTIL_TO_INT(item) + 1);
	अन्यथा {
		अक्षर *buffer;
		संरचना strbuf subject = STRBUF_INIT;
		स्थिरांक अक्षर *eol;

		/* Skip any leading whitespace, including any blank lines. */
		पर्यंत (*oneline && है_खाली(*oneline))
			oneline++;
		eol = म_अक्षर(oneline, '\n');
		जर (!eol)
			eol = oneline + म_माप(oneline);
		जर (starts_with(oneline, "[PATCH")) {
			स्थिरांक अक्षर *eob = म_अक्षर(oneline, ']');
			जर (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		पर्यंत (*oneline && है_खाली(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, रिक्त);

		जर (!item->util) {
			item->util = xmalloc(आकार(संरचना string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

स्थिर पूर्णांक parse_ident(संरचना shortlog *log,
		       संरचना strbuf *out, स्थिरांक अक्षर *in)
{
	स्थिरांक अक्षर *mailbuf, *namebuf;
	माप_प्रकार namelen, maillen;
	संरचना ident_split ident;

	जर (split_ident_ओळ(&ident, in, म_माप(in)))
		परत -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	जर (log->email)
		strbuf_addf(out, " <%.*s>", (पूर्णांक)maillen, mailbuf);

	परत 0;
}

स्थिर शून्य read_from_stdin(संरचना shortlog *log)
{
	संरचना strbuf ident = STRBUF_INIT;
	संरचना strbuf mapped_ident = STRBUF_INIT;
	संरचना strbuf oneline = STRBUF_INIT;
	स्थिर स्थिरांक अक्षर *author_match[2] = { "Author: ", "author " };
	स्थिर स्थिरांक अक्षर *committer_match[2] = { "Commit: ", "committer " };
	स्थिरांक अक्षर **match;

	जर (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	निवड (log->groups) {
	स्थिती SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		थांबा;
	स्थिती SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		थांबा;
	स्थिती SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	स्थिती SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	पूर्वनिर्धारित:
		BUG("unhandled shortlog group");
	}

	पर्यंत (strbuf_getline_lf(&ident, मानक_निवेश) != खातापूर्ण) {
		स्थिरांक अक्षर *v;
		जर (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			सुरूठेवा;
		पर्यंत (strbuf_getline_lf(&oneline, मानक_निवेश) != खातापूर्ण &&
		       oneline.len)
			; /* discard headers */
		पर्यंत (strbuf_getline_lf(&oneline, मानक_निवेश) != खातापूर्ण &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		जर (parse_ident(log, &mapped_ident, v) < 0)
			सुरूठेवा;

		घाला_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

स्थिर शून्य घाला_records_from_trailers(संरचना shortlog *log,
					 संरचना strset *dups,
					 संरचना commit *commit,
					 संरचना pretty_print_context *ctx,
					 स्थिरांक अक्षर *oneline)
{
	संरचना trailer_iterator iter;
	स्थिरांक अक्षर *commit_buffer, *body;
	संरचना strbuf ident = STRBUF_INIT;

	जर (!log->trailers.nr)
		परत;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, रिक्त,
					     ctx->output_encoding);
	body = म_माला(commit_buffer, "\n\n");
	जर (!body)
		जा out;

	trailer_iterator_init(&iter, body);
	पर्यंत (trailer_iterator_advance(&iter)) {
		स्थिरांक अक्षर *मूल्य = iter.val.buf;

		जर (!string_list_has_string(&log->trailers, iter.किल्ली.buf))
			सुरूठेवा;

		strbuf_reset(&ident);
		जर (!parse_ident(log, &ident, मूल्य))
			मूल्य = ident.buf;

		जर (!strset_add(dups, मूल्य))
			सुरूठेवा;
		घाला_one_record(log, मूल्य, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

स्थिर पूर्णांक shortlog_needs_dedup(स्थिरांक संरचना shortlog *log)
{
	परत HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

स्थिर शून्य घाला_records_from_format(संरचना shortlog *log,
				       संरचना strset *dups,
				       संरचना commit *commit,
				       संरचना pretty_print_context *ctx,
				       स्थिरांक अक्षर *oneline)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		जर (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			घाला_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

शून्य shortlog_add_commit(संरचना shortlog *log, संरचना commit *commit)
{
	संरचना strbuf oneline = STRBUF_INIT;
	संरचना strset dups = STRSET_INIT;
	संरचना pretty_print_context ctx = {0};
	स्थिरांक अक्षर *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	जर (!log->summary) {
		जर (log->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		अन्यथा
			repo_format_commit_message(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	घाला_records_from_trailers(log, &dups, commit, &ctx, oneline_str);
	घाला_records_from_format(log, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

स्थिर शून्य get_from_rev(संरचना rev_info *rev, संरचना shortlog *log)
{
	संरचना commit *commit;

	जर (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	पर्यंत ((commit = get_revision(rev)) != रिक्त)
		shortlog_add_commit(log, commit);
}

स्थिर पूर्णांक parse_uint(अक्षर स्थिरांक **arg, पूर्णांक comma, पूर्णांक defval)
{
	अचिन्हित दीर्घ ul;
	पूर्णांक ret;
	अक्षर *endp;

	ul = म_से_अदीर्घ(*arg, &endp, 10);
	जर (*endp && *endp != comma)
		परत -1;
	जर (ul > पूर्णांक_उच्च)
		परत -1;
	ret = *arg == endp ? defval : (पूर्णांक)ul;
	*arg = *endp ? endp + 1 : endp;
	परत ret;
}

स्थिर स्थिरांक अक्षर wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

स्थिर पूर्णांक parse_wrap_args(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना shortlog *log = opt->मूल्य;

	log->wrap_ओळी = !unset;
	जर (unset)
		परत 0;
	जर (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		परत 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	जर (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		परत error(wrap_arg_usage);
	जर (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		परत error(wrap_arg_usage);
	परत 0;
}

स्थिर पूर्णांक parse_group_option(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना shortlog *log = opt->मूल्य;
	स्थिरांक अक्षर *field;

	जर (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} अन्यथा जर (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	अन्यथा जर (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	अन्यथा जर (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} अन्यथा जर (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} अन्यथा जर (म_अक्षर(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} अन्यथा {
		परत error(_("unknown group type: %s"), arg);
	}

	परत 0;
}


शून्य shortlog_init(संरचना shortlog *log)
{
	स_रखो(log, 0, आकार(*log));

	read_mailmap(the_repository, &log->mailmap);

	log->list.strdup_strings = 1;
	log->wrap = DEFAULT_WRAPLEN;
	log->in1 = DEFAULT_INDENT1;
	log->in2 = DEFAULT_INDENT2;
	log->trailers.strdup_strings = 1;
	log->trailers.cmp = strcasecmp;
	log->format.strdup_strings = 1;
}

शून्य shortlog_finish_setup(संरचना shortlog *log)
{
	जर (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	जर (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

पूर्णांक cmd_shortlog(पूर्णांक argc,
		 स्थिरांक अक्षर **argv,
		 स्थिरांक अक्षर *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना shortlog log = { STRING_LIST_INIT_NODUP };
	संरचना rev_info rev;
	पूर्णांक nongit = !startup_info->have_repository;

	स्थिरांक संरचना option options[] = {
		OPT_BIT('c', "committer", &log.groups,
			N_("group by committer rather than author"),
			SHORTLOG_GROUP_COMMITTER),
		OPT_BOOL('n', "numbered", &log.sort_by_संख्या,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &log.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &log.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', रिक्त, &log, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_args),
		OPT_CALLBACK(0, "group", &log, N_("field"),
			N_("group by field"), parse_group_option),
		OPT_END(),
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
	जर (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, रिक्त);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	साठी (;;) {
		निवड (parse_options_step(&ctx, options, shortlog_usage)) {
		स्थिती PARSE_OPT_NON_OPTION:
		स्थिती PARSE_OPT_UNKNOWN:
			थांबा;
		स्थिती PARSE_OPT_HELP:
			निकास(0);
		स्थिती PARSE_OPT_HELP_ERROR:
		स्थिती PARSE_OPT_ERROR:
		स्थिती PARSE_OPT_SUBCOMMAND:
			निकास(129);
		स्थिती PARSE_OPT_COMPLETE:
			निकास(0);
		स्थिती PARSE_OPT_DONE:
			जा parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	जर (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	जर (!nongit && setup_revisions(argc, argv, &rev, रिक्त) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	जर (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	जर (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	जर (rev.pending.nr == 0) {
		जर (isatty(0))
			ख_लिखो(मानक_त्रुटि, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	अन्यथा
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	परत 0;
}

स्थिर शून्य add_wrapped_shortlog_संदेश(संरचना strbuf *sb, स्थिरांक अक्षर *s,
				     स्थिरांक संरचना shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

शून्य shortlog_output(संरचना shortlog *log)
{
	माप_प्रकार i, j;
	संरचना strbuf sb = STRBUF_INIT;

	जर (log->sort_by_संख्या)
		STABLE_QSORT(log->list.वस्तुसंच, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	साठी (i = 0; i < log->list.nr; i++) {
		स्थिरांक संरचना string_list_item *item = &log->list.वस्तुसंच[i];
		जर (log->summary) {
			ख_लिखो(log->file, "%6d\t%s\n",
				(पूर्णांक)UTIL_TO_INT(item), item->string);
		} अन्यथा {
			संरचना string_list *onelines = item->util;
			ख_लिखो(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			साठी (j = onelines->nr; j >= 1; j--) {
				स्थिरांक अक्षर *संदेश = onelines->वस्तुसंच[j - 1].string;

				जर (log->wrap_ओळी) {
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

		log->list.वस्तुसंच[i].util = रिक्त;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
