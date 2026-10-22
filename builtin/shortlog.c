#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "shortlog.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"
#ಅನ್ತರ್ಭಾವ "strmap.h"

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ಖಾಲಿ
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
#ವ್ಯಾಖ್ಯಾನಿಸು UTIL_ಗಮ್ಯ_INT(x) ((intptr_t)(x)->util)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_by_counter(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a2)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *i1 = a1, *i2 = a2;
	ಹಿಂತಿರುಗಿ UTIL_ಗಮ್ಯ_INT(i2) - UTIL_ಗಮ್ಯ_INT(i1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_by_list(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a2)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *i1 = a1, *i2 = a2;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *l1 = i1->util, *l2 = i2->util;

	ಆದರೆ (l1->nr < l2->nr)
		ಹಿಂತಿರುಗಿ 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l1->nr == l2->nr)
		ಹಿಂತಿರುಗಿ 0;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_one_ದಾಖಲೆ(ರಚನೆ shortlog *ಲಘುಗಣಕ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ident,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oneline)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	ಅಂಶ = string_list_ಸೇರಿಸು(&ಲಘುಗಣಕ->list, ident);

	ಆದರೆ (ಲಘುಗಣಕ->summary)
		ಅಂಶ->util = (ಶೂನ್ಯ *)(UTIL_ಗಮ್ಯ_INT(ಅಂಶ) + 1);
	ಇಲ್ಲದಿದ್ದರೆ {
		ಅಕ್ಷರ *ಬಫರ್;
		ರಚನೆ strbuf subject = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *eol;

		/* Skip any leading whitespace, including any blank lines. */
		ಆಗ (*oneline && ಖಾಲಿಯೇ(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		ಆದರೆ (!eol)
			eol = oneline + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(oneline);
		ಆದರೆ (starts_with(oneline, "[PATCH")) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *eob = strchr(oneline, ']');
			ಆದರೆ (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		ಆಗ (*oneline && ಖಾಲಿಯೇ(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		ಬಫರ್ = strbuf_detach(&subject, ಖಾಲಿ);

		ಆದರೆ (!ಅಂಶ->util) {
			ಅಂಶ->util = xmalloc(ಗಾತ್ರ(ರಚನೆ string_list));
			string_list_init_nodup(ಅಂಶ->util);
		}
		string_list_append(ಅಂಶ->util, ಬಫರ್);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ident(ರಚನೆ shortlog *ಲಘುಗಣಕ,
		       ರಚನೆ strbuf *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಒಳ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mailbuf, *namebuf;
	ಗಾತ್ರ_t namelen, maillen;
	ರಚನೆ ident_ವಿಭಜಿಸು ident;

	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&ident, ಒಳ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಒಳ)))
		ಹಿಂತಿರುಗಿ -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_ಕೊನೆ - ident.name_begin;
	maillen = ident.mail_ಕೊನೆ - ident.mail_begin;

	map_user(&ಲಘುಗಣಕ->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_ಕೂಡಿಸು(ಹೊರಹೋಗು, namebuf, namelen);
	ಆದರೆ (ಲಘುಗಣಕ->email)
		strbuf_addf(ಹೊರಹೋಗು, " <%.*s>", (ಪೂರ್ಣಾಂಕ)maillen, mailbuf);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_from_stdin(ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	ರಚನೆ strbuf ident = STRBUF_INIT;
	ರಚನೆ strbuf mapped_ident = STRBUF_INIT;
	ರಚನೆ strbuf oneline = STRBUF_INIT;
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *author_ಹೊಂದಿಕೆ[2] = { "Author: ", "author " };
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *committer_ಹೊಂದಿಕೆ[2] = { "Commit: ", "committer " };
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಹೊಂದಿಕೆ;

	ಆದರೆ (HAS_MULTI_ಬಿಟ್ಟುಗಳು(ಲಘುಗಣಕ->groups))
		die(_("using multiple --group options with stdin is not supported"));

	ಆಯ್ಕೆ (ಲಘುಗಣಕ->groups) {
	ಸ್ಥಿತಿ SHORTLOG_ಗುಂಪು_AUTHOR:
		ಹೊಂದಿಕೆ = author_ಹೊಂದಿಕೆ;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SHORTLOG_ಗುಂಪು_COMMITTER:
		ಹೊಂದಿಕೆ = committer_ಹೊಂದಿಕೆ;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SHORTLOG_ಗುಂಪು_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	ಸ್ಥಿತಿ SHORTLOG_ಗುಂಪು_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unhandled shortlog group");
	}

	ಆಗ (strbuf_getline_lf(&ident, stdin) != EOF) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;
		ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ident.buf, ಹೊಂದಿಕೆ[0], &v) &&
		    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ident.buf, ಹೊಂದಿಕೆ[1], &v))
			ಮುಂದುವರಿ;
		ಆಗ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		ಆಗ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		ಆದರೆ (parse_ident(ಲಘುಗಣಕ, &mapped_ident, v) < 0)
			ಮುಂದುವರಿ;

		ಸೇರಿಸು_one_ದಾಖಲೆ(ಲಘುಗಣಕ, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_ದಾಖಲೆಗಳು_from_trailers(ರಚನೆ shortlog *ಲಘುಗಣಕ,
					 ರಚನೆ strset *dups,
					 ರಚನೆ commit *commit,
					 ರಚನೆ pretty_print_context *ctx,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oneline)
{
	ರಚನೆ trailer_iterator iter;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *commit_ಬಫರ್, *body;
	ರಚನೆ strbuf ident = STRBUF_INIT;

	ಆದರೆ (!ಲಘುಗಣಕ->trailers.nr)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_ಬಫರ್ = repo_logmsg_reencode(the_repository, commit, ಖಾಲಿ,
					     ctx->output_encoding);
	body = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(commit_ಬಫರ್, "\n\n");
	ಆದರೆ (!body)
		ಹೋಗು ಹೊರಹೋಗು;

	trailer_iterator_init(&iter, body);
	ಆಗ (trailer_iterator_advance(&iter)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ = iter.val.buf;

		ಆದರೆ (!string_list_has_string(&ಲಘುಗಣಕ->trailers, iter.ಕೀಲಿ.buf))
			ಮುಂದುವರಿ;

		strbuf_reset(&ident);
		ಆದರೆ (!parse_ident(ಲಘುಗಣಕ, &ident, ಮೌಲ್ಯ))
			ಮೌಲ್ಯ = ident.buf;

		ಆದರೆ (!strset_ಕೂಡಿಸು(dups, ಮೌಲ್ಯ))
			ಮುಂದುವರಿ;
		ಸೇರಿಸು_one_ದಾಖಲೆ(ಲಘುಗಣಕ, ಮೌಲ್ಯ, oneline);
	}
	trailer_iterator_release(&iter);

ಹೊರಹೋಗು:
	strbuf_release(&ident);
	repo_unuse_commit_ಬಫರ್(the_repository, commit, commit_ಬಫರ್);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ shortlog_needs_dedup(ಸ್ಥಿರಾಂಕ ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	ಹಿಂತಿರುಗಿ HAS_MULTI_ಬಿಟ್ಟುಗಳು(ಲಘುಗಣಕ->groups) || ಲಘುಗಣಕ->format.nr > 1 || ಲಘುಗಣಕ->trailers.nr;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_ದಾಖಲೆಗಳು_from_format(ರಚನೆ shortlog *ಲಘುಗಣಕ,
				       ರಚನೆ strset *dups,
				       ರಚನೆ commit *commit,
				       ರಚನೆ pretty_print_context *ctx,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oneline)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಲಘುಗಣಕ->format) {
		strbuf_reset(&buf);

		repo_format_commit_ಸಂದೇಶ(the_repository, commit,
					   ಅಂಶ->string, &buf, ctx);

		ಆದರೆ (!shortlog_needs_dedup(ಲಘುಗಣಕ) || strset_ಕೂಡಿಸು(dups, buf.buf))
			ಸೇರಿಸು_one_ದಾಖಲೆ(ಲಘುಗಣಕ, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

ಶೂನ್ಯ shortlog_ಕೂಡಿಸು_commit(ರಚನೆ shortlog *ಲಘುಗಣಕ, ರಚನೆ commit *commit)
{
	ರಚನೆ strbuf oneline = STRBUF_INIT;
	ರಚನೆ strset dups = STRSET_INIT;
	ರಚನೆ pretty_print_context ctx = {0};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = ಲಘುಗಣಕ->abbrev;
	ctx.date_ಬಹುಲಕ = ಲಘುಗಣಕ->date_ಬಹುಲಕ;
	ctx.output_encoding = get_log_output_encoding();

	ಆದರೆ (!ಲಘುಗಣಕ->summary) {
		ಆದರೆ (ಲಘುಗಣಕ->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		ಇಲ್ಲದಿದ್ದರೆ
			repo_format_commit_ಸಂದೇಶ(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ಸೇರಿಸು_ದಾಖಲೆಗಳು_from_trailers(ಲಘುಗಣಕ, &dups, commit, &ctx, oneline_str);
	ಸೇರಿಸು_ದಾಖಲೆಗಳು_from_format(ಲಘುಗಣಕ, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ಸ್ಥಿರ ಶೂನ್ಯ get_from_rev(ರಚನೆ rev_info *rev, ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	ರಚನೆ commit *commit;

	ಆದರೆ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	ಆಗ ((commit = get_revision(rev)) != ಖಾಲಿ)
		shortlog_ಕೂಡಿಸು_commit(ಲಘುಗಣಕ, commit);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_uint(ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **arg, ಪೂರ್ಣಾಂಕ comma, ಪೂರ್ಣಾಂಕ defval)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ul;
	ಪೂರ್ಣಾಂಕ ret;
	ಅಕ್ಷರ *endp;

	ul = strtoul(*arg, &endp, 10);
	ಆದರೆ (*endp && *endp != comma)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (ul > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
		ಹಿಂತಿರುಗಿ -1;
	ret = *arg == endp ? defval : (ಪೂರ್ಣಾಂಕ)ul;
	*arg = *endp ? endp + 1 : endp;
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_WRAPLEN 76
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_INDENT1 6
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_INDENT2 9

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_wrap_ವಾದಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ shortlog *ಲಘುಗಣಕ = opt->ಮೌಲ್ಯ;

	ಲಘುಗಣಕ->wrap_ಸಾಲುಗಳು = !unset;
	ಆದರೆ (unset)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (!arg) {
		ಲಘುಗಣಕ->wrap = DEFAULT_WRAPLEN;
		ಲಘುಗಣಕ->in1 = DEFAULT_INDENT1;
		ಲಘುಗಣಕ->in2 = DEFAULT_INDENT2;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಲಘುಗಣಕ->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	ಲಘುಗಣಕ->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	ಲಘುಗಣಕ->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	ಆದರೆ (ಲಘುಗಣಕ->wrap < 0 || ಲಘುಗಣಕ->in1 < 0 || ಲಘುಗಣಕ->in2 < 0)
		ಹಿಂತಿರುಗಿ error(wrap_arg_usage);
	ಆದರೆ (ಲಘುಗಣಕ->wrap &&
	    ((ಲಘುಗಣಕ->in1 && ಲಘುಗಣಕ->wrap <= ಲಘುಗಣಕ->in1) ||
	     (ಲಘುಗಣಕ->in2 && ಲಘುಗಣಕ->wrap <= ಲಘುಗಣಕ->in2)))
		ಹಿಂತಿರುಗಿ error(wrap_arg_usage);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಗುಂಪು_option(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ shortlog *ಲಘುಗಣಕ = opt->ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ;

	ಆದರೆ (unset) {
		ಲಘುಗಣಕ->groups = 0;
		string_list_clear(&ಲಘುಗಣಕ->trailers, 0);
		string_list_clear(&ಲಘುಗಣಕ->format, 0);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(arg, "author"))
		ಲಘುಗಣಕ->groups |= SHORTLOG_ಗುಂಪು_AUTHOR;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcasecmp(arg, "committer"))
		ಲಘುಗಣಕ->groups |= SHORTLOG_ಗುಂಪು_COMMITTER;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "trailer:", &ಕ್ಷೇತ್ರ)) {
		ಲಘುಗಣಕ->groups |= SHORTLOG_ಗುಂಪು_TRAILER;
		string_list_append(&ಲಘುಗಣಕ->trailers, ಕ್ಷೇತ್ರ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "format:", &ಕ್ಷೇತ್ರ)) {
		ಲಘುಗಣಕ->groups |= SHORTLOG_ಗುಂಪು_FORMAT;
		string_list_append(&ಲಘುಗಣಕ->format, ಕ್ಷೇತ್ರ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strchr(arg, '%')) {
		ಲಘುಗಣಕ->groups |= SHORTLOG_ಗುಂಪು_FORMAT;
		string_list_append(&ಲಘುಗಣಕ->format, arg);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ error(_("unknown group type: %s"), arg);
	}

	ಹಿಂತಿರುಗಿ 0;
}


ಶೂನ್ಯ shortlog_init(ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	memset(ಲಘುಗಣಕ, 0, ಗಾತ್ರ(*ಲಘುಗಣಕ));

	read_mailmap(the_repository, &ಲಘುಗಣಕ->mailmap);

	ಲಘುಗಣಕ->list.strdup_ಅಕ್ಷರಸರಣಿಗಳು = 1;
	ಲಘುಗಣಕ->wrap = DEFAULT_WRAPLEN;
	ಲಘುಗಣಕ->in1 = DEFAULT_INDENT1;
	ಲಘುಗಣಕ->in2 = DEFAULT_INDENT2;
	ಲಘುಗಣಕ->trailers.strdup_ಅಕ್ಷರಸರಣಿಗಳು = 1;
	ಲಘುಗಣಕ->trailers.cmp = strcasecmp;
	ಲಘುಗಣಕ->format.strdup_ಅಕ್ಷರಸರಣಿಗಳು = 1;
}

ಶೂನ್ಯ shortlog_ಮುಕ್ತಾಯ_ಸಿದ್ಧತೆ(ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	ಆದರೆ (ಲಘುಗಣಕ->groups & SHORTLOG_ಗುಂಪು_AUTHOR)
		string_list_append(&ಲಘುಗಣಕ->format,
				   ಲಘುಗಣಕ->email ? "%aN <%aE>" : "%aN");
	ಆದರೆ (ಲಘುಗಣಕ->groups & SHORTLOG_ಗುಂಪು_COMMITTER)
		string_list_append(&ಲಘುಗಣಕ->format,
				   ಲಘುಗಣಕ->email ? "%cN <%cE>" : "%cN");

	string_list_ವಿಂಗಡಿಸು(&ಲಘುಗಣಕ->trailers);
}

ಪೂರ್ಣಾಂಕ cmd_shortlog(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ shortlog ಲಘುಗಣಕ = { STRING_LIST_INIT_NODUP };
	ರಚನೆ rev_info rev;
	ಪೂರ್ಣಾಂಕ nongit = !startup_info->have_repository;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_ಬಿಟ್('c', "committer", &ಲಘುಗಣಕ.groups,
			N_("group by committer rather than author"),
			SHORTLOG_ಗುಂಪು_COMMITTER),
		OPT_BOOL('n', "numbered", &ಲಘುಗಣಕ.ವಿಂಗಡಿಸು_by_ಸಂಖ್ಯೆ,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &ಲಘುಗಣಕ.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &ಲಘುಗಣಕ.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ಖಾಲಿ, &ಲಘುಗಣಕ, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_ವಾದಗಳು),
		OPT_CALLBACK(0, "group", &ಲಘುಗಣಕ, N_("field"),
			N_("group by field"), parse_ಗುಂಪು_option),
		OPT_ಕೊನೆ(),
	};

	ರಚನೆ parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	ಆದರೆ (nongit && !the_hash_algo)
		repo_ಗಣ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	shortlog_init(&ಲಘುಗಣಕ);
	repo_init_revisions(the_repository, &rev, ಪೂರ್ವಪ್ರತ್ಯಯ);
	parse_options_ಆರಂಭ(&ctx, argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	ಗಾಗಿ (;;) {
		ಆಯ್ಕೆ (parse_options_ಹೆಜ್ಜೆ(&ctx, options, shortlog_usage)) {
		ಸ್ಥಿತಿ PARSE_OPT_NON_OPTION:
		ಸ್ಥಿತಿ PARSE_OPT_UNKNOWN:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ PARSE_OPT_HELP:
			ನಿರ್ಗಮಿಸು(0);
		ಸ್ಥಿತಿ PARSE_OPT_HELP_ERROR:
		ಸ್ಥಿತಿ PARSE_OPT_ERROR:
		ಸ್ಥಿತಿ PARSE_OPT_SUBCOMMAND:
			ನಿರ್ಗಮಿಸು(129);
		ಸ್ಥಿತಿ PARSE_OPT_COMPLETE:
			ನಿರ್ಗಮಿಸು(0);
		ಸ್ಥಿತಿ PARSE_OPT_ಮುಗಿದ:
			ಹೋಗು parse_ಮುಗಿದ;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_ಮುಗಿದ:
	revision_opts_ಮುಕ್ತಾಯ(&rev);
	argc = parse_options_ಕೊನೆ(&ctx);

	ಆದರೆ (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	ಆದರೆ (!nongit && ಸಿದ್ಧತೆ_revisions(argc, argv, &rev, ಖಾಲಿ) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	ಲಘುಗಣಕ.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	ಲಘುಗಣಕ.abbrev = rev.abbrev;
	ಲಘುಗಣಕ.file = rev.diffopt.file;
	ಲಘುಗಣಕ.date_ಬಹುಲಕ = rev.date_ಬಹುಲಕ;

	ಆದರೆ (!ಲಘುಗಣಕ.groups)
		ಲಘುಗಣಕ.groups = SHORTLOG_ಗುಂಪು_AUTHOR;
	shortlog_ಮುಕ್ತಾಯ_ಸಿದ್ಧತೆ(&ಲಘುಗಣಕ);

	/* assume HEAD if from a tty */
	ಆದರೆ (!nongit && !rev.pending.nr && isatty(0))
		ಕೂಡಿಸು_ತಲೆ_ಗಮ್ಯ_pending(&rev);
	ಆದರೆ (rev.pending.nr == 0) {
		ಆದರೆ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&ಲಘುಗಣಕ);
	}
	ಇಲ್ಲದಿದ್ದರೆ
		get_from_rev(&rev, &ಲಘುಗಣಕ);

	shortlog_output(&ಲಘುಗಣಕ);
	release_revisions(&rev);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_wrapped_shortlog_ಸಂದೇಶ(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	strbuf_ಕೂಡಿಸು_wrapped_ಪಠ್ಯ(sb, s, ಲಘುಗಣಕ->in1, ಲಘುಗಣಕ->in2, ಲಘುಗಣಕ->wrap);
	strbuf_addch(sb, '\n');
}

ಶೂನ್ಯ shortlog_output(ರಚನೆ shortlog *ಲಘುಗಣಕ)
{
	ಗಾತ್ರ_t i, j;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (ಲಘುಗಣಕ->ವಿಂಗಡಿಸು_by_ಸಂಖ್ಯೆ)
		STABLE_QSORT(ಲಘುಗಣಕ->list.ವಸ್ತುಗಳು, ಲಘುಗಣಕ->list.nr,
		      ಲಘುಗಣಕ->summary ? compare_by_counter : compare_by_list);
	ಗಾಗಿ (i = 0; i < ಲಘುಗಣಕ->list.nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = &ಲಘುಗಣಕ->list.ವಸ್ತುಗಳು[i];
		ಆದರೆ (ಲಘುಗಣಕ->summary) {
			fprintf(ಲಘುಗಣಕ->file, "%6d\t%s\n",
				(ಪೂರ್ಣಾಂಕ)UTIL_ಗಮ್ಯ_INT(ಅಂಶ), ಅಂಶ->string);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ string_list *onelines = ಅಂಶ->util;
			fprintf(ಲಘುಗಣಕ->file, "%s (%"PRIuMAX"):\n",
				ಅಂಶ->string, (uintmax_t)onelines->nr);
			ಗಾಗಿ (j = onelines->nr; j >= 1; j--) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = onelines->ವಸ್ತುಗಳು[j - 1].string;

				ಆದರೆ (ಲಘುಗಣಕ->wrap_ಸಾಲುಗಳು) {
					strbuf_reset(&sb);
					ಕೂಡಿಸು_wrapped_shortlog_ಸಂದೇಶ(&sb, ಸಂದೇಶ, ಲಘುಗಣಕ);
					fwrite(sb.buf, sb.len, 1, ಲಘುಗಣಕ->file);
				}
				ಇಲ್ಲದಿದ್ದರೆ
					fprintf(ಲಘುಗಣಕ->file, "      %s\n", ಸಂದೇಶ);
			}
			putc('\n', ಲಘುಗಣಕ->file);
			onelines->strdup_ಅಕ್ಷರಸರಣಿಗಳು = 1;
			string_list_clear(onelines, 0);
			ಬಿಡುಗಡೆ(onelines);
		}

		ಲಘುಗಣಕ->list.ವಸ್ತುಗಳು[i].util = ಖಾಲಿ;
	}

	strbuf_release(&sb);
	ಲಘುಗಣಕ->list.strdup_ಅಕ್ಷರಸರಣಿಗಳು = 1;
	string_list_clear(&ಲಘುಗಣಕ->list, 1);
	clear_mailmap(&ಲಘುಗಣಕ->mailmap);
	string_list_clear(&ಲಘುಗಣಕ->format, 0);
	string_list_clear(&ಲಘುಗಣಕ->trailers, 0);
}
