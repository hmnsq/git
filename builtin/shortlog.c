#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shortlog.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trailer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strmap.h"

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ shortlog_usage[] = {
	N_("git shortlog [<options>] [<revision-range>] [[--] <path>...]"),
	N_("git log --pretty=short | git shortlog [<options>]"),
	ਨਿਰਾ
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTIL_ਮੰਜ਼ਿਲ_INT(x) ((intptr_t)(x)->util)

ਸਥਿਰ ਪੂਰਨ compare_by_counter(ਅਚਲ ਖਾਲੀ *a1, ਅਚਲ ਖਾਲੀ *a2)
{
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *i1 = a1, *i2 = a2;
	ਵਾਪਸ UTIL_ਮੰਜ਼ਿਲ_INT(i2) - UTIL_ਮੰਜ਼ਿਲ_INT(i1);
}

ਸਥਿਰ ਪੂਰਨ compare_by_list(ਅਚਲ ਖਾਲੀ *a1, ਅਚਲ ਖਾਲੀ *a2)
{
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *i1 = a1, *i2 = a2;
	ਅਚਲ ਬਣਤਰ string_list *l1 = i1->util, *l2 = i2->util;

	ਜੇ (l1->nr < l2->nr)
		ਵਾਪਸ 1;
	ਨਹੀਂਤਾਂ ਜੇ (l1->nr == l2->nr)
		ਵਾਪਸ 0;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ -1;
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_one_ਰਿਕਾਰਡ(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ,
			      ਅਚਲ ਅੱਖਰ *ident,
			      ਅਚਲ ਅੱਖਰ *oneline)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	ਵਸਤੂ = string_list_ਪਾਓ(&ਲਘੂਗਣਕ->list, ident);

	ਜੇ (ਲਘੂਗਣਕ->summary)
		ਵਸਤੂ->util = (ਖਾਲੀ *)(UTIL_ਮੰਜ਼ਿਲ_INT(ਵਸਤੂ) + 1);
	ਨਹੀਂਤਾਂ {
		ਅੱਖਰ *ਬਫਰ;
		ਬਣਤਰ strbuf subject = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *eol;

		/* Skip any leading whitespace, including any blank lines. */
		ਜਦੋਂ (*oneline && ਖਾਲੀ_ਥਾਂ_ਹੈ(*oneline))
			oneline++;
		eol = strchr(oneline, '\n');
		ਜੇ (!eol)
			eol = oneline + ਲੜੀ_ਲੰਬਾਈ(oneline);
		ਜੇ (starts_with(oneline, "[PATCH")) {
			ਅਚਲ ਅੱਖਰ *eob = strchr(oneline, ']');
			ਜੇ (eob && (!eol || eob < eol))
				oneline = eob + 1;
		}
		ਜਦੋਂ (*oneline && ਖਾਲੀ_ਥਾਂ_ਹੈ(*oneline) && *oneline != '\n')
			oneline++;
		format_subject(&subject, oneline, " ");
		ਬਫਰ = strbuf_detach(&subject, ਨਿਰਾ);

		ਜੇ (!ਵਸਤੂ->util) {
			ਵਸਤੂ->util = xmalloc(ਆਕਾਰ(ਬਣਤਰ string_list));
			string_list_init_nodup(ਵਸਤੂ->util);
		}
		string_list_append(ਵਸਤੂ->util, ਬਫਰ);
	}
}

ਸਥਿਰ ਪੂਰਨ parse_ident(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ,
		       ਬਣਤਰ strbuf *ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *ਵਿੱਚ)
{
	ਅਚਲ ਅੱਖਰ *mailbuf, *namebuf;
	ਮਾਪ_t namelen, maillen;
	ਬਣਤਰ ident_ਵੰਡੋ ident;

	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&ident, ਵਿੱਚ, ਲੜੀ_ਲੰਬਾਈ(ਵਿੱਚ)))
		ਵਾਪਸ -1;

	namebuf = ident.name_begin;
	mailbuf = ident.mail_begin;
	namelen = ident.name_ਸਿਰਾ - ident.name_begin;
	maillen = ident.mail_ਸਿਰਾ - ident.mail_begin;

	map_user(&ਲਘੂਗਣਕ->mailmap, &mailbuf, &maillen, &namebuf, &namelen);
	strbuf_ਜੋੜੋ(ਬਾਹਰਲਾ, namebuf, namelen);
	ਜੇ (ਲਘੂਗਣਕ->email)
		strbuf_addf(ਬਾਹਰਲਾ, " <%.*s>", (ਪੂਰਨ)maillen, mailbuf);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ read_from_stdin(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	ਬਣਤਰ strbuf ident = STRBUF_INIT;
	ਬਣਤਰ strbuf mapped_ident = STRBUF_INIT;
	ਬਣਤਰ strbuf oneline = STRBUF_INIT;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *author_ਮੇਲ[2] = { "Author: ", "author " };
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *committer_ਮੇਲ[2] = { "Commit: ", "committer " };
	ਅਚਲ ਅੱਖਰ **ਮੇਲ;

	ਜੇ (HAS_MULTI_ਬਿੱਟ(ਲਘੂਗਣਕ->groups))
		die(_("using multiple --group options with stdin is not supported"));

	ਚੋਣ (ਲਘੂਗਣਕ->groups) {
	ਹਾਲਤ SHORTLOG_ਸਮੂਹ_AUTHOR:
		ਮੇਲ = author_ਮੇਲ;
		ਰੋਕੋ;
	ਹਾਲਤ SHORTLOG_ਸਮੂਹ_COMMITTER:
		ਮੇਲ = committer_ਮੇਲ;
		ਰੋਕੋ;
	ਹਾਲਤ SHORTLOG_ਸਮੂਹ_TRAILER:
		die(_("using %s with stdin is not supported"), "--group=trailer");
	ਹਾਲਤ SHORTLOG_ਸਮੂਹ_FORMAT:
		die(_("using %s with stdin is not supported"), "--group=format");
	ਮੂਲ:
		BUG("unhandled shortlog group");
	}

	ਜਦੋਂ (strbuf_getline_lf(&ident, stdin) != EOF) {
		ਅਚਲ ਅੱਖਰ *v;
		ਜੇ (!skip_ਅਗੇਤਰ(ident.buf, ਮੇਲ[0], &v) &&
		    !skip_ਅਗੇਤਰ(ident.buf, ਮੇਲ[1], &v))
			ਜਾਰੀ;
		ਜਦੋਂ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       oneline.len)
			; /* discard headers */
		ਜਦੋਂ (strbuf_getline_lf(&oneline, stdin) != EOF &&
		       !oneline.len)
			; /* discard blanks */

		strbuf_reset(&mapped_ident);
		ਜੇ (parse_ident(ਲਘੂਗਣਕ, &mapped_ident, v) < 0)
			ਜਾਰੀ;

		ਪਾਓ_one_ਰਿਕਾਰਡ(ਲਘੂਗਣਕ, mapped_ident.buf, oneline.buf);
	}
	strbuf_release(&ident);
	strbuf_release(&mapped_ident);
	strbuf_release(&oneline);
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_ਰਿਕਾਰਡਾਂ_from_trailers(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ,
					 ਬਣਤਰ strset *dups,
					 ਬਣਤਰ commit *commit,
					 ਬਣਤਰ pretty_print_context *ctx,
					 ਅਚਲ ਅੱਖਰ *oneline)
{
	ਬਣਤਰ trailer_iterator iter;
	ਅਚਲ ਅੱਖਰ *commit_ਬਫਰ, *body;
	ਬਣਤਰ strbuf ident = STRBUF_INIT;

	ਜੇ (!ਲਘੂਗਣਕ->trailers.nr)
		ਵਾਪਸ;

	/*
	 * Using repo_format_commit_message("%B") would be simpler here, but
	 * this saves us copying the message.
	 */
	commit_ਬਫਰ = repo_logmsg_reencode(the_repository, commit, ਨਿਰਾ,
					     ctx->output_encoding);
	body = ਲੜੀ_ਖੋਜ(commit_ਬਫਰ, "\n\n");
	ਜੇ (!body)
		ਜਾਓ ਬਾਹਰਲਾ;

	trailer_iterator_init(&iter, body);
	ਜਦੋਂ (trailer_iterator_advance(&iter)) {
		ਅਚਲ ਅੱਖਰ *ਮੁੱਲ = iter.val.buf;

		ਜੇ (!string_list_has_string(&ਲਘੂਗਣਕ->trailers, iter.ਕੁੰਜੀ.buf))
			ਜਾਰੀ;

		strbuf_reset(&ident);
		ਜੇ (!parse_ident(ਲਘੂਗਣਕ, &ident, ਮੁੱਲ))
			ਮੁੱਲ = ident.buf;

		ਜੇ (!strset_ਜੋੜੋ(dups, ਮੁੱਲ))
			ਜਾਰੀ;
		ਪਾਓ_one_ਰਿਕਾਰਡ(ਲਘੂਗਣਕ, ਮੁੱਲ, oneline);
	}
	trailer_iterator_release(&iter);

ਬਾਹਰਲਾ:
	strbuf_release(&ident);
	repo_unuse_commit_ਬਫਰ(the_repository, commit, commit_ਬਫਰ);
}

ਸਥਿਰ ਪੂਰਨ shortlog_needs_dedup(ਅਚਲ ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	ਵਾਪਸ HAS_MULTI_ਬਿੱਟ(ਲਘੂਗਣਕ->groups) || ਲਘੂਗਣਕ->format.nr > 1 || ਲਘੂਗਣਕ->trailers.nr;
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_ਰਿਕਾਰਡਾਂ_from_format(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ,
				       ਬਣਤਰ strset *dups,
				       ਬਣਤਰ commit *commit,
				       ਬਣਤਰ pretty_print_context *ctx,
				       ਅਚਲ ਅੱਖਰ *oneline)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਲਘੂਗਣਕ->format) {
		strbuf_reset(&buf);

		repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit,
					   ਵਸਤੂ->string, &buf, ctx);

		ਜੇ (!shortlog_needs_dedup(ਲਘੂਗਣਕ) || strset_ਜੋੜੋ(dups, buf.buf))
			ਪਾਓ_one_ਰਿਕਾਰਡ(ਲਘੂਗਣਕ, buf.buf, oneline);
	}

	strbuf_release(&buf);
}

ਖਾਲੀ shortlog_ਜੋੜੋ_commit(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ, ਬਣਤਰ commit *commit)
{
	ਬਣਤਰ strbuf oneline = STRBUF_INIT;
	ਬਣਤਰ strset dups = STRSET_INIT;
	ਬਣਤਰ pretty_print_context ctx = {0};
	ਅਚਲ ਅੱਖਰ *oneline_str;

	ctx.fmt = CMIT_FMT_USERFORMAT;
	ctx.abbrev = ਲਘੂਗਣਕ->abbrev;
	ctx.date_ਬਹੁਲਕ = ਲਘੂਗਣਕ->date_ਬਹੁਲਕ;
	ctx.output_encoding = get_log_output_encoding();

	ਜੇ (!ਲਘੂਗਣਕ->summary) {
		ਜੇ (ਲਘੂਗਣਕ->user_format)
			pretty_print_commit(&ctx, commit, &oneline);
		ਨਹੀਂਤਾਂ
			repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit,
						   "%s", &oneline, &ctx);
	}
	oneline_str = oneline.len ? oneline.buf : "<none>";

	ਪਾਓ_ਰਿਕਾਰਡਾਂ_from_trailers(ਲਘੂਗਣਕ, &dups, commit, &ctx, oneline_str);
	ਪਾਓ_ਰਿਕਾਰਡਾਂ_from_format(ਲਘੂਗਣਕ, &dups, commit, &ctx, oneline_str);

	strset_clear(&dups);
	strbuf_release(&oneline);
}

ਸਥਿਰ ਖਾਲੀ get_from_rev(ਬਣਤਰ rev_info *rev, ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	ਬਣਤਰ commit *commit;

	ਜੇ (prepare_revision_walk(rev))
		die(_("revision walk setup failed"));
	ਜਦੋਂ ((commit = get_revision(rev)) != ਨਿਰਾ)
		shortlog_ਜੋੜੋ_commit(ਲਘੂਗਣਕ, commit);
}

ਸਥਿਰ ਪੂਰਨ parse_uint(ਅੱਖਰ ਅਚਲ **arg, ਪੂਰਨ comma, ਪੂਰਨ defval)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ul;
	ਪੂਰਨ ret;
	ਅੱਖਰ *endp;

	ul = strtoul(*arg, &endp, 10);
	ਜੇ (*endp && *endp != comma)
		ਵਾਪਸ -1;
	ਜੇ (ul > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
		ਵਾਪਸ -1;
	ret = *arg == endp ? defval : (ਪੂਰਨ)ul;
	*arg = *endp ? endp + 1 : endp;
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ wrap_arg_usage[] = "-w[<width>[,<indent1>[,<indent2>]]]";
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_WRAPLEN 76
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_INDENT1 6
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_INDENT2 9

ਸਥਿਰ ਪੂਰਨ parse_wrap_ਦਲੀਲਾਂ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ shortlog *ਲਘੂਗਣਕ = opt->ਮੁੱਲ;

	ਲਘੂਗਣਕ->wrap_ਸਤਰਾਂ = !unset;
	ਜੇ (unset)
		ਵਾਪਸ 0;
	ਜੇ (!arg) {
		ਲਘੂਗਣਕ->wrap = DEFAULT_WRAPLEN;
		ਲਘੂਗਣਕ->in1 = DEFAULT_INDENT1;
		ਲਘੂਗਣਕ->in2 = DEFAULT_INDENT2;
		ਵਾਪਸ 0;
	}

	ਲਘੂਗਣਕ->wrap = parse_uint(&arg, ',', DEFAULT_WRAPLEN);
	ਲਘੂਗਣਕ->in1 = parse_uint(&arg, ',', DEFAULT_INDENT1);
	ਲਘੂਗਣਕ->in2 = parse_uint(&arg, '\0', DEFAULT_INDENT2);
	ਜੇ (ਲਘੂਗਣਕ->wrap < 0 || ਲਘੂਗਣਕ->in1 < 0 || ਲਘੂਗਣਕ->in2 < 0)
		ਵਾਪਸ error(wrap_arg_usage);
	ਜੇ (ਲਘੂਗਣਕ->wrap &&
	    ((ਲਘੂਗਣਕ->in1 && ਲਘੂਗਣਕ->wrap <= ਲਘੂਗਣਕ->in1) ||
	     (ਲਘੂਗਣਕ->in2 && ਲਘੂਗਣਕ->wrap <= ਲਘੂਗਣਕ->in2)))
		ਵਾਪਸ error(wrap_arg_usage);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_ਸਮੂਹ_option(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ shortlog *ਲਘੂਗਣਕ = opt->ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *ਖੇਤਰ;

	ਜੇ (unset) {
		ਲਘੂਗਣਕ->groups = 0;
		string_list_clear(&ਲਘੂਗਣਕ->trailers, 0);
		string_list_clear(&ਲਘੂਗਣਕ->format, 0);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(arg, "author"))
		ਲਘੂਗਣਕ->groups |= SHORTLOG_ਸਮੂਹ_AUTHOR;
	ਨਹੀਂਤਾਂ ਜੇ (!strcasecmp(arg, "committer"))
		ਲਘੂਗਣਕ->groups |= SHORTLOG_ਸਮੂਹ_COMMITTER;
	ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "trailer:", &ਖੇਤਰ)) {
		ਲਘੂਗਣਕ->groups |= SHORTLOG_ਸਮੂਹ_TRAILER;
		string_list_append(&ਲਘੂਗਣਕ->trailers, ਖੇਤਰ);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "format:", &ਖੇਤਰ)) {
		ਲਘੂਗਣਕ->groups |= SHORTLOG_ਸਮੂਹ_FORMAT;
		string_list_append(&ਲਘੂਗਣਕ->format, ਖੇਤਰ);
	} ਨਹੀਂਤਾਂ ਜੇ (strchr(arg, '%')) {
		ਲਘੂਗਣਕ->groups |= SHORTLOG_ਸਮੂਹ_FORMAT;
		string_list_append(&ਲਘੂਗਣਕ->format, arg);
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ error(_("unknown group type: %s"), arg);
	}

	ਵਾਪਸ 0;
}


ਖਾਲੀ shortlog_init(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	memset(ਲਘੂਗਣਕ, 0, ਆਕਾਰ(*ਲਘੂਗਣਕ));

	read_mailmap(the_repository, &ਲਘੂਗਣਕ->mailmap);

	ਲਘੂਗਣਕ->list.strdup_ਲੜੀਆਂ = 1;
	ਲਘੂਗਣਕ->wrap = DEFAULT_WRAPLEN;
	ਲਘੂਗਣਕ->in1 = DEFAULT_INDENT1;
	ਲਘੂਗਣਕ->in2 = DEFAULT_INDENT2;
	ਲਘੂਗਣਕ->trailers.strdup_ਲੜੀਆਂ = 1;
	ਲਘੂਗਣਕ->trailers.cmp = strcasecmp;
	ਲਘੂਗਣਕ->format.strdup_ਲੜੀਆਂ = 1;
}

ਖਾਲੀ shortlog_ਅੰਤ_ਤਿਆਰੀ(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	ਜੇ (ਲਘੂਗਣਕ->groups & SHORTLOG_ਸਮੂਹ_AUTHOR)
		string_list_append(&ਲਘੂਗਣਕ->format,
				   ਲਘੂਗਣਕ->email ? "%aN <%aE>" : "%aN");
	ਜੇ (ਲਘੂਗਣਕ->groups & SHORTLOG_ਸਮੂਹ_COMMITTER)
		string_list_append(&ਲਘੂਗਣਕ->format,
				   ਲਘੂਗਣਕ->email ? "%cN <%cE>" : "%cN");

	string_list_ਛਾਂਟੋ(&ਲਘੂਗਣਕ->trailers);
}

ਪੂਰਨ cmd_shortlog(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ shortlog ਲਘੂਗਣਕ = { STRING_LIST_INIT_NODUP };
	ਬਣਤਰ rev_info rev;
	ਪੂਰਨ nongit = !startup_info->have_repository;

	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('c', "committer", &ਲਘੂਗਣਕ.groups,
			N_("group by committer rather than author"),
			SHORTLOG_ਸਮੂਹ_COMMITTER),
		OPT_BOOL('n', "numbered", &ਲਘੂਗਣਕ.ਛਾਂਟੋ_by_ਸੰਖਿਆ,
			 N_("sort output according to the number of commits per author")),
		OPT_BOOL('s', "summary", &ਲਘੂਗਣਕ.summary,
			 N_("suppress commit descriptions, only provides commit count")),
		OPT_BOOL('e', "email", &ਲਘੂਗਣਕ.email,
			 N_("show the email address of each author")),
		OPT_CALLBACK_F('w', ਨਿਰਾ, &ਲਘੂਗਣਕ, N_("<w>[,<i1>[,<i2>]]"),
			N_("linewrap output"), PARSE_OPT_OPTARG,
			&parse_wrap_ਦਲੀਲਾਂ),
		OPT_CALLBACK(0, "group", &ਲਘੂਗਣਕ, N_("field"),
			N_("group by field"), parse_ਸਮੂਹ_option),
		OPT_ਸਿਰਾ(),
	};

	ਬਣਤਰ parse_opt_ctx_t ctx;

	/*
	 * NEEDSWORK: Later on we'll call parse_revision_opt which relies on
	 * the hash algorithm being set but since we are operating outside of a
	 * Git repository we cannot determine one. This is only needed because
	 * parse_revision_opt expects hexsz for --abbrev which is irrelevant
	 * for shortlog outside of a git repository. For now explicitly set
	 * SHA1, but ideally the parsing machinery would be split between
	 * git/nongit so that we do not have to do this.
	 */
	ਜੇ (nongit && !the_hash_algo)
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	shortlog_init(&ਲਘੂਗਣਕ);
	repo_init_revisions(the_repository, &rev, ਅਗੇਤਰ);
	parse_options_ਸ਼ੁਰੂ(&ctx, argc, argv, ਅਗੇਤਰ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);

	ਲਈ (;;) {
		ਚੋਣ (parse_options_ਕਦਮ(&ctx, options, shortlog_usage)) {
		ਹਾਲਤ PARSE_OPT_NON_OPTION:
		ਹਾਲਤ PARSE_OPT_UNKNOWN:
			ਰੋਕੋ;
		ਹਾਲਤ PARSE_OPT_HELP:
			ਬਾਹਰ_ਨਿਕਲੋ(0);
		ਹਾਲਤ PARSE_OPT_HELP_ERROR:
		ਹਾਲਤ PARSE_OPT_ERROR:
		ਹਾਲਤ PARSE_OPT_SUBCOMMAND:
			ਬਾਹਰ_ਨਿਕਲੋ(129);
		ਹਾਲਤ PARSE_OPT_COMPLETE:
			ਬਾਹਰ_ਨਿਕਲੋ(0);
		ਹਾਲਤ PARSE_OPT_ਪੂਰਾ:
			ਜਾਓ parse_ਪੂਰਾ;
		}
		parse_revision_opt(&rev, &ctx, options, shortlog_usage);
	}
parse_ਪੂਰਾ:
	revision_opts_ਅੰਤ(&rev);
	argc = parse_options_ਸਿਰਾ(&ctx);

	ਜੇ (nongit && argc > 1) {
		error(_("too many arguments given outside repository"));
		usage_with_options(shortlog_usage, options);
	}

	ਜੇ (!nongit && ਤਿਆਰੀ_revisions(argc, argv, &rev, ਨਿਰਾ) != 1) {
		error(_("unrecognized argument: %s"), argv[1]);
		usage_with_options(shortlog_usage, options);
	}

	ਲਘੂਗਣਕ.user_format = rev.commit_format == CMIT_FMT_USERFORMAT;
	ਲਘੂਗਣਕ.abbrev = rev.abbrev;
	ਲਘੂਗਣਕ.file = rev.diffopt.file;
	ਲਘੂਗਣਕ.date_ਬਹੁਲਕ = rev.date_ਬਹੁਲਕ;

	ਜੇ (!ਲਘੂਗਣਕ.groups)
		ਲਘੂਗਣਕ.groups = SHORTLOG_ਸਮੂਹ_AUTHOR;
	shortlog_ਅੰਤ_ਤਿਆਰੀ(&ਲਘੂਗਣਕ);

	/* assume HEAD if from a tty */
	ਜੇ (!nongit && !rev.pending.nr && isatty(0))
		ਜੋੜੋ_ਸਿਰ_ਮੰਜ਼ਿਲ_pending(&rev);
	ਜੇ (rev.pending.nr == 0) {
		ਜੇ (isatty(0))
			fprintf(stderr, _("(reading log message from standard input)\n"));
		read_from_stdin(&ਲਘੂਗਣਕ);
	}
	ਨਹੀਂਤਾਂ
		get_from_rev(&rev, &ਲਘੂਗਣਕ);

	shortlog_output(&ਲਘੂਗਣਕ);
	release_revisions(&rev);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_wrapped_shortlog_ਸੁਨੇਹਾ(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *s,
				     ਅਚਲ ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	strbuf_ਜੋੜੋ_wrapped_ਪਾਠ(sb, s, ਲਘੂਗਣਕ->in1, ਲਘੂਗਣਕ->in2, ਲਘੂਗਣਕ->wrap);
	strbuf_addch(sb, '\n');
}

ਖਾਲੀ shortlog_output(ਬਣਤਰ shortlog *ਲਘੂਗਣਕ)
{
	ਮਾਪ_t i, j;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (ਲਘੂਗਣਕ->ਛਾਂਟੋ_by_ਸੰਖਿਆ)
		STABLE_QSORT(ਲਘੂਗਣਕ->list.ਚੀਜ਼ਾਂ, ਲਘੂਗਣਕ->list.nr,
		      ਲਘੂਗਣਕ->summary ? compare_by_counter : compare_by_list);
	ਲਈ (i = 0; i < ਲਘੂਗਣਕ->list.nr; i++) {
		ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = &ਲਘੂਗਣਕ->list.ਚੀਜ਼ਾਂ[i];
		ਜੇ (ਲਘੂਗਣਕ->summary) {
			fprintf(ਲਘੂਗਣਕ->file, "%6d\t%s\n",
				(ਪੂਰਨ)UTIL_ਮੰਜ਼ਿਲ_INT(ਵਸਤੂ), ਵਸਤੂ->string);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ string_list *onelines = ਵਸਤੂ->util;
			fprintf(ਲਘੂਗਣਕ->file, "%s (%"PRIuMAX"):\n",
				ਵਸਤੂ->string, (uintmax_t)onelines->nr);
			ਲਈ (j = onelines->nr; j >= 1; j--) {
				ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = onelines->ਚੀਜ਼ਾਂ[j - 1].string;

				ਜੇ (ਲਘੂਗਣਕ->wrap_ਸਤਰਾਂ) {
					strbuf_reset(&sb);
					ਜੋੜੋ_wrapped_shortlog_ਸੁਨੇਹਾ(&sb, ਸੁਨੇਹਾ, ਲਘੂਗਣਕ);
					fwrite(sb.buf, sb.len, 1, ਲਘੂਗਣਕ->file);
				}
				ਨਹੀਂਤਾਂ
					fprintf(ਲਘੂਗਣਕ->file, "      %s\n", ਸੁਨੇਹਾ);
			}
			putc('\n', ਲਘੂਗਣਕ->file);
			onelines->strdup_ਲੜੀਆਂ = 1;
			string_list_clear(onelines, 0);
			ਮੁਕਤ(onelines);
		}

		ਲਘੂਗਣਕ->list.ਚੀਜ਼ਾਂ[i].util = ਨਿਰਾ;
	}

	strbuf_release(&sb);
	ਲਘੂਗਣਕ->list.strdup_ਲੜੀਆਂ = 1;
	string_list_clear(&ਲਘੂਗਣਕ->list, 1);
	clear_mailmap(&ਲਘੂਗਣਕ->mailmap);
	string_list_clear(&ਲਘੂਗਣਕ->format, 0);
	string_list_clear(&ਲਘੂਗਣਕ->trailers, 0);
}
