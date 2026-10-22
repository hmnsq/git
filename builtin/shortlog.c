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
#define UTIL_TO_INT(x) ((intptr_t)(x)->util)

स्थिर पूर्णाङ्क compare_by_counter(ध्रुव शून्य *a1, ध्रुव शून्य *a2)
{
	ध्रुव संरचना string_list_item *i1 = a1, *i2 = a2;
	प्रत्यागम UTIL_TO_INT(i2) - UTIL_TO_INT(i1);
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

स्थिर शून्य जोड़ो_one_record(संरचना shortlog *log,
			      ध्रुव वर्ण *ident,
			      ध्रुव वर्ण *oneline)
{
	संरचना string_list_item *item;

	item = string_list_जोड़ो(&log->list, ident);

	यदि (log->summary)
		item->util = (शून्य *)(UTIL_TO_INT(item) + 1);
	अन्यथा {
		वर्ण *buffer;
		संरचना strbuf subject = STRBUF_INIT;
		ध्रुव वर्ण *eol;

		/* Skip any leading whitespace, including any blank lines. */
		यावत् (*oneline && है_खाली(*oneline))
			oneline++;
		eol = म_अक्षर(oneline, '\n');
		यदि (!eol)
			eol = oneline + म_माप(oneline);
		यदि (starts_with(oneline, "[PATCH")) {
			ध्रुव वर्ण *eob = म_अक्षर(oneline, ']');
			यदि (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		यावत् (*oneline && है_खाली(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		buffer = strbuf_detach(&subject, रिक्त);

		यदि (!item->util) {
			item->util = xmalloc(परिमाण(संरचना string_list));
			string_list_init_nodup(item->util);
		}
		string_list_append(item->util, buffer);
	}
}

स्थिर पूर्णाङ्क parse_ident(संरचना shortlog *log,
		       संरचना strbuf *out, ध्रुव वर्ण *in)
{
	ध्रुव वर्ण *mailbuf, *namebuf;
	माप_प्रकार namelen, maillen;
	संरचना ident_split ident;

	यदि (split_ident_पंक्ति(&ident, in, म_माप(in)))
		प्रत्यागम -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_end - ident.name_begin;
	maillen = ident.mail_end - ident.mail_begin;

	map_user(&log->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_add(out, namebuf, namelen);
	यदि (log->email)
		strbuf_addf(out, " <%.*s>", (पूर्णाङ्क)maillen, mailbuf);

	प्रत्यागम 0;
}

स्थिर शून्य read_from_stdin(संरचना shortlog *log)
{
	संरचना strbuf ident = STRBUF_INIT;
	संरचना strbuf mapped_ident = STRBUF_INIT;
	संरचना strbuf oneline = STRBUF_INIT;
	स्थिर ध्रुव वर्ण *author_match[2] = { "Author: ", "author " };
	स्थिर ध्रुव वर्ण *committer_match[2] = { "Commit: ", "committer " };
	ध्रुव वर्ण **match;

	यदि (HAS_MULTI_BITS(log->groups))
		die(_("using multiple --group options with stdin is not supported"));

	विकल्प (log->groups) {
	पक्ष SHORTLOG_GROUP_AUTHOR:
		match = author_match;
		भङ्ग;
	पक्ष SHORTLOG_GROUP_COMMITTER:
		match = committer_match;
		भङ्ग;
	पक्ष SHORTLOG_GROUP_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	पक्ष SHORTLOG_GROUP_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	स्वभाव:
		BUG("unhandled shortlog group");
	}

	यावत् (strbuf_getline_lf(&ident, मानक_निवेश) != खातापूर्ण) {
		ध्रुव वर्ण *v;
		यदि (!skip_prefix(ident.buf, match[0], &v) &&
		    !skip_prefix(ident.buf, match[1], &v))
			अनुवर्त;
		यावत् (strbuf_getline_lf(&oneline, मानक_निवेश) != खातापूर्ण &&
		       oneline.len)
			; /* discard headers */
		यावत् (strbuf_getline_lf(&oneline, मानक_निवेश) != खातापूर्ण &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		यदि (parse_ident(log, &mapped_ident, v) < 0)
			अनुवर्त;

		जोड़ो_one_record(log, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

स्थिर शून्य जोड़ो_records_from_trailers(संरचना shortlog *log,
					 संरचना strset *dups,
					 संरचना commit *commit,
					 संरचना pretty_print_context *ctx,
					 ध्रुव वर्ण *oneline)
{
	संरचना trailer_iterator iter;
	ध्रुव वर्ण *commit_buffer, *body;
	संरचना strbuf ident = STRBUF_INIT;

	यदि (!log->trailers.nr)
		प्रत्यागम;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_buffer = repo_logmsg_reencode(the_repository, commit, रिक्त,
					     ctx->output_encoding);
	body = म_माला(commit_buffer, "\n\n");
	यदि (!body)
		गच्छ out;

	trailer_iterator_init(&iter, body);
	यावत् (trailer_iterator_advance(&iter)) {
		ध्रुव वर्ण *मूल्य = iter.val.buf;

		यदि (!string_list_has_string(&log->trailers, iter.कुंजी.buf))
			अनुवर्त;

		strbuf_reset(&ident);
		यदि (!parse_ident(log, &ident, मूल्य))
			मूल्य = ident.buf;

		यदि (!strset_add(dups, मूल्य))
			अनुवर्त;
		जोड़ो_one_record(log, मूल्य, oneline);
	}
	trailer_iterator_release(&iter);

out:
	strbuf_release(&ident);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);
}

स्थिर पूर्णाङ्क shortlog_needs_dedup(ध्रुव संरचना shortlog *log)
{
	प्रत्यागम HAS_MULTI_BITS(log->groups) || log->format.nr > 1 || log->trailers.nr;
}

स्थिर शून्य जोड़ो_records_from_format(संरचना shortlog *log,
				       संरचना strset *dups,
				       संरचना commit *commit,
				       संरचना pretty_print_context *ctx,
				       ध्रुव वर्ण *oneline)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;

	for_each_string_list_item(item, &log->format) {
		strbuf_reset(&buf);

		repo_format_commit_message(the_repository, commit,
					   item->string, &buf, ctx);

		यदि (!shortlog_needs_dedup(log) || strset_add(dups, buf.buf))
			जोड़ो_one_record(log, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

शून्य shortlog_add_commit(संरचना shortlog *log, संरचना commit *commit)
{
	संरचना strbuf oneline = STRBUF_INIT;
	संरचना strset dups = STRSET_INIT;
	संरचना pretty_print_context ctx = {0};
	ध्रुव वर्ण *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = log->abbrev;
	ctx.date_mode = log->date_mode;
	ctx.output_encoding = get_log_output_encoding();

	यदि (!log->summary) {
		यदि (log->user_format)
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

स्थिर शून्य get_from_rev(संरचना rev_info *rev, संरचना shortlog *log)
{
	संरचना commit *commit;

	यदि (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	यावत् ((commit = get_revision(rev)) != रिक्त)
		shortlog_add_commit(log, commit);
}

स्थिर पूर्णाङ्क parse_uint(वर्ण ध्रुव **arg, पूर्णाङ्क comma, पूर्णाङ्क defval)
{
	अचिन्हित दीर्घ ul;
	पूर्णाङ्क ret;
	वर्ण *endp;

	ul = म_से_अदीर्घ(*arg, &endp, 10);
	यदि (*endp && *endp != comma)
		प्रत्यागम -1;
	यदि (ul > पूर्णांक_उच्च)
		प्रत्यागम -1;
	ret = *arg == endp ? defval : (पूर्णाङ्क)ul;
	*arg = *endp ? endp + 1 : endp;
	प्रत्यागम ret;
}

स्थिर ध्रुव वर्ण wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#define DEFAULT_WRAPLEN 76
#define DEFAULT_INDENT1 6
#define DEFAULT_INDENT2 9

स्थिर पूर्णाङ्क parse_wrap_args(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना shortlog *log = opt->मूल्य;

	log->wrap_पंक्तियाँ = !unset;
	यदि (unset)
		प्रत्यागम 0;
	यदि (!arg) {
		log->wrap = DEFAULT_WRAPLEN;
		log->in1 = DEFAULT_INDENT1;
		log->in2 = DEFAULT_INDENT2;
		प्रत्यागम 0;
	}

	log->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	log->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	log->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	यदि (log->wrap < 0 || log->in1 < 0 || log->in2 < 0)
		प्रत्यागम error(wrap_arg_usage);
	यदि (log->wrap &&
	    ((log->in1 && log->wrap <= log->in1) ||
	     (log->in2 && log->wrap <= log->in2)))
		प्रत्यागम error(wrap_arg_usage);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_group_option(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना shortlog *log = opt->मूल्य;
	ध्रुव वर्ण *field;

	यदि (unset) {
		log->groups = 0;
		string_list_clear(&log->trailers, 0);
		string_list_clear(&log->format, 0);
	} अन्यथा यदि (!strcasecmp(arg, "author"))
		log->groups |= SHORTLOG_GROUP_AUTHOR;
	अन्यथा यदि (!strcasecmp(arg, "committer"))
		log->groups |= SHORTLOG_GROUP_COMMITTER;
	अन्यथा यदि (skip_prefix(arg, "trailer:", &field)) {
		log->groups |= SHORTLOG_GROUP_TRAILER;
		string_list_append(&log->trailers, field);
	} अन्यथा यदि (skip_prefix(arg, "format:", &field)) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, field);
	} अन्यथा यदि (म_अक्षर(arg, '%')) {
		log->groups |= SHORTLOG_GROUP_FORMAT;
		string_list_append(&log->format, arg);
	} अन्यथा {
		प्रत्यागम error(_("unknown group type: %s"), arg);
	}

	प्रत्यागम 0;
}


शून्य shortlog_init(संरचना shortlog *log)
{
	स_रखो(log, 0, परिमाण(*log));

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
	यदि (log->groups & SHORTLOG_GROUP_AUTHOR)
		string_list_append(&log->format,
				   log->email ? "%aN <%aE>" : "%aN");
	यदि (log->groups & SHORTLOG_GROUP_COMMITTER)
		string_list_append(&log->format,
				   log->email ? "%cN <%cE>" : "%cN");

	string_list_sort(&log->trailers);
}

पूर्णाङ्क cmd_shortlog(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना shortlog log = { STRING_LIST_INIT_NODUP };
	संरचना rev_info rev;
	पूर्णाङ्क nongit = !startup_info->have_repository;

	ध्रुव संरचना option options[] = {
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
	यदि (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, रिक्त);
	shortlog_init(&log);
	repo_init_revisions(the_repository, &rev, prefix);
	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	प्रति (;;) {
		विकल्प (parse_options_step(&ctx, options, shortlog_usage)) {
		पक्ष PARSE_OPT_NON_OPTION:
		पक्ष PARSE_OPT_UNKNOWN:
			भङ्ग;
		पक्ष PARSE_OPT_HELP:
			निकास(0);
		पक्ष PARSE_OPT_HELP_ERROR:
		पक्ष PARSE_OPT_ERROR:
		पक्ष PARSE_OPT_SUBCOMMAND:
			निकास(129);
		पक्ष PARSE_OPT_COMPLETE:
			निकास(0);
		पक्ष PARSE_OPT_DONE:
			गच्छ parse_done;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_done:
	revision_opts_finish(&rev);
	argc = parse_options_end(&ctx);

	यदि (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	यदि (!nongit && setup_revisions(argc, argv, &rev, रिक्त) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	log.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	log.abbrev = rev.abbrev;
	log.file = rev.diffopt.file;
	log.date_mode = rev.date_mode;

	यदि (!log.groups)
		log.groups = SHORTLOG_GROUP_AUTHOR;
	shortlog_finish_setup(&log);

	/* assume HEAD if from a tty */
	यदि (!nongit && !rev.pending.nr && isatty(0))
		add_head_to_pending(&rev);
	यदि (rev.pending.nr == 0) {
		यदि (isatty(0))
			ख_लिखो(मानक_त्रुटि, _("(reading log message from standard input)\n"));
		read_from_stdin(&log);
	}
	अन्यथा
		get_from_rev(&rev, &log);

	shortlog_output(&log);
	release_revisions(&rev);
	प्रत्यागम 0;
}

स्थिर शून्य add_wrapped_shortlog_संदेश(संरचना strbuf *sb, ध्रुव वर्ण *s,
				     ध्रुव संरचना shortlog *log)
{
	strbuf_add_wrapped_text(sb, s, log->in1, log->in2, log->wrap);
	strbuf_addch(sb, '\n');
}

शून्य shortlog_output(संरचना shortlog *log)
{
	माप_प्रकार i, j;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (log->sort_by_संख्या)
		STABLE_QSORT(log->list.वस्तुएँ, log->list.nr,
		      log->summary ? compare_by_counter : compare_by_list);
	प्रति (i = 0; i < log->list.nr; i++) {
		ध्रुव संरचना string_list_item *item = &log->list.वस्तुएँ[i];
		यदि (log->summary) {
			ख_लिखो(log->file, "%6d\t%s\n",
				(पूर्णाङ्क)UTIL_TO_INT(item), item->string);
		} अन्यथा {
			संरचना string_list *onelines = item->util;
			ख_लिखो(log->file, "%s (%"PRIuMAX"):\n",
				item->string, (uintmax_t)onelines->nr);
			प्रति (j = onelines->nr; j >= 1; j--) {
				ध्रुव वर्ण *संदेश = onelines->वस्तुएँ[j - 1].string;

				यदि (log->wrap_पंक्तियाँ) {
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

		log->list.वस्तुएँ[i].util = रिक्त;
	}

	strbuf_release(&sb);
	log->list.strdup_strings = 1;
	string_list_clear(&log->list, 1);
	clear_mailmap(&log->mailmap);
	string_list_clear(&log->format, 0);
	string_list_clear(&log->trailers, 0);
}
