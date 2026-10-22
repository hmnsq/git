/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prio-queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "userdiff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "line-range.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "line-log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blame.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ longest_file;
ਸਥਿਰ ਪੂਰਨ longest_author;
ਸਥਿਰ ਪੂਰਨ max_orig_digits;
ਸਥਿਰ ਪੂਰਨ max_digits;
ਸਥਿਰ ਪੂਰਨ max_score_digits;
ਸਥਿਰ ਪੂਰਨ show_ਜੜ੍ਹ;
ਸਥਿਰ ਪੂਰਨ ਉਲਟ;
ਸਥਿਰ ਪੂਰਨ blank_boundary;
ਸਥਿਰ ਪੂਰਨ incremental;
ਸਥਿਰ ਪੂਰਨ xdl_opts;
ਸਥਿਰ ਪੂਰਨ abbrev = -1;
ਸਥਿਰ ਪੂਰਨ no_ਸਾਰਾ_file_rename;
ਸਥਿਰ ਪੂਰਨ show_ਤਰੱਕੀ;
ਸਥਿਰ ਅੱਖਰ repeated_meta_color[COLOR_MAXLEN];
ਸਥਿਰ ਪੂਰਨ coloring_ਬਹੁਲਕ;
ਸਥਿਰ ਬਣਤਰ string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਪੂਰਨ mark_unblamable_ਸਤਰਾਂ;
ਸਥਿਰ ਪੂਰਨ mark_ignored_ਸਤਰਾਂ;

ਸਥਿਰ ਬਣਤਰ date_ਬਹੁਲਕ blame_date_ਬਹੁਲਕ = { DATE_ISO8601 };
ਸਥਿਰ ਮਾਪ_t blame_date_ਚੌੜਾਈ;

ਸਥਿਰ ਬਣਤਰ string_list mailmap = STRING_LIST_INIT_NODUP;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DEBUG_BLAME
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEBUG_BLAME 0
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ blame_move_score;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ blame_copy_score;

/* Remember to update object flag allocation in object.h */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ METAINFO_SHOWN		(1u<<12)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਹੋਰ_THAN_ONE_ਰਸਤਾ	(1u<<13)

ਬਣਤਰ ਤਰੱਕੀ_info {
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;
	ਪੂਰਨ blamed_ਸਤਰਾਂ;
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *nth_ਸਤਰ_cb(ਖਾਲੀ *ਅੰਕੜੇ, ਲੰਮਾ lno)
{
	ਵਾਪਸ blame_nth_ਸਤਰ((ਬਣਤਰ blame_scoreboard *)ਅੰਕੜੇ, lno);
}

/*
 * Information on commits, used for output.
 */
ਬਣਤਰ commit_info {
	ਬਣਤਰ strbuf author;
	ਬਣਤਰ strbuf author_mail;
	timestamp_t author_ਸਮਾਂ;
	ਬਣਤਰ strbuf author_tz;

	/* filled only when asked for details */
	ਬਣਤਰ strbuf committer;
	ਬਣਤਰ strbuf committer_mail;
	timestamp_t committer_ਸਮਾਂ;
	ਬਣਤਰ strbuf committer_tz;

	ਬਣਤਰ strbuf summary;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ COMMIT_INFO_INIT { \
	.author = STRBUF_INIT, \
	.author_mail = STRBUF_INIT, \
	.author_tz = STRBUF_INIT, \
	.committer = STRBUF_INIT, \
	.committer_mail = STRBUF_INIT, \
	.committer_tz = STRBUF_INIT, \
	.summary = STRBUF_INIT, \
}

/*
 * Parse author/committer line in the commit object buffer
 */
ਸਥਿਰ ਖਾਲੀ get_ac_ਸਤਰ(ਅਚਲ ਅੱਖਰ *inbuf, ਅਚਲ ਅੱਖਰ *what,
	ਬਣਤਰ strbuf *name, ਬਣਤਰ strbuf *mail,
	timestamp_t *ਸਮਾਂ, ਬਣਤਰ strbuf *tz)
{
	ਬਣਤਰ ident_ਵੰਡੋ ident;
	ਮਾਪ_t len, maillen, namelen;
	ਅਚਲ ਅੱਖਰ *tmp, *endp;
	ਅਚਲ ਅੱਖਰ *namebuf, *mailbuf;

	tmp = ਲੜੀ_ਖੋਜ(inbuf, what);
	ਜੇ (!tmp)
		ਜਾਓ error_ਬਾਹਰਲਾ;
	tmp += ਲੜੀ_ਲੰਬਾਈ(what);
	endp = strchr(tmp, '\n');
	ਜੇ (!endp)
		len = ਲੜੀ_ਲੰਬਾਈ(tmp);
	ਨਹੀਂਤਾਂ
		len = endp - tmp;

	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&ident, tmp, len)) {
	error_ਬਾਹਰਲਾ:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*ਸਮਾਂ = 0;
		ਵਾਪਸ;
	}

	namelen = ident.name_ਸਿਰਾ - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_ਸਿਰਾ - ident.mail_begin;
	mailbuf = ident.mail_begin;

	ਜੇ (ident.date_begin && ident.date_ਸਿਰਾ)
		*ਸਮਾਂ = strtoul(ident.date_begin, ਨਿਰਾ, 10);
	ਨਹੀਂਤਾਂ
		*ਸਮਾਂ = 0;

	ਜੇ (ident.tz_begin && ident.tz_ਸਿਰਾ)
		strbuf_ਜੋੜੋ(tz, ident.tz_begin, ident.tz_ਸਿਰਾ - ident.tz_begin);
	ਨਹੀਂਤਾਂ
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (ਪੂਰਨ)maillen, mailbuf);
	strbuf_ਜੋੜੋ(name, namebuf, namelen);
}

ਸਥਿਰ ਖਾਲੀ commit_info_destroy(ਬਣਤਰ commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ਸਥਿਰ ਖਾਲੀ get_commit_info(ਬਣਤਰ commit *commit, ਬਣਤਰ commit_info *ret)
{
	ਪੂਰਨ len;
	ਅਚਲ ਅੱਖਰ *subject, *encoding;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2;

	encoding = get_log_output_encoding();
	ਸੁਨੇਹਾ_2 = repo_logmsg_reencode(the_repository, commit, ਨਿਰਾ, encoding);
	get_ac_ਸਤਰ(ਸੁਨੇਹਾ_2, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_ਸਮਾਂ, &ret->author_tz);

	get_ac_ਸਤਰ(ਸੁਨੇਹਾ_2, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_ਸਮਾਂ, &ret->committer_tz);

	len = ਲੱਭੋ_commit_subject(ਸੁਨੇਹਾ_2, &subject);
	ਜੇ (len)
		strbuf_ਜੋੜੋ(&ret->summary, subject, len);
	ਨਹੀਂਤਾਂ
		strbuf_addf(&ret->summary, "(%s)", oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));

	repo_unuse_commit_ਬਫਰ(the_repository, commit, ਸੁਨੇਹਾ_2);
}

/*
 * Write out any suspect information which depends on the path. This must be
 * handled separately from emit_one_suspect_detail(), because a given commit
 * may have changes in multiple paths. So this needs to appear each time
 * we mention a new group.
 *
 * To allow LF and other nonportable characters in pathnames,
 * they are c-style quoted as needed.
 */
ਸਥਿਰ ਖਾਲੀ write_filename_info(ਬਣਤਰ blame_origin *suspect)
{
	ਜੇ (suspect->ਪਿਛਲਾ) {
		ਬਣਤਰ blame_origin *prev = suspect->ਪਿਛਲਾ;
		ਲਿਖੋ("previous %s ", oid_ਮੰਜ਼ਿਲ_hex(&prev->commit->object.oid));
		write_name_quoted(prev->ਰਸਤਾ, stdout, '\n');
	}
	ਲਿਖੋ("filename ");
	write_name_quoted(suspect->ਰਸਤਾ, stdout, '\n');
}

/*
 * Porcelain/Incremental format wants to show a lot of details per
 * commit.  Instead of repeating this every line, emit it only once,
 * the first time each commit appears in the output (unless the
 * user has specifically asked for us to repeat).
 */
ਸਥਿਰ ਪੂਰਨ emit_one_suspect_detail(ਬਣਤਰ blame_origin *suspect, ਪੂਰਨ repeat)
{
	ਬਣਤਰ commit_info ci = COMMIT_INFO_INIT;

	ਜੇ (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		ਵਾਪਸ 0;

	suspect->commit->object.flags |= METAINFO_SHOWN;
	get_commit_info(suspect->commit, &ci);
	ਲਿਖੋ("author %s\n", ci.author.buf);
	ਲਿਖੋ("author-mail %s\n", ci.author_mail.buf);
	ਲਿਖੋ("author-time %"PRItime"\n", ci.author_ਸਮਾਂ);
	ਲਿਖੋ("author-tz %s\n", ci.author_tz.buf);
	ਲਿਖੋ("committer %s\n", ci.committer.buf);
	ਲਿਖੋ("committer-mail %s\n", ci.committer_mail.buf);
	ਲਿਖੋ("committer-time %"PRItime"\n", ci.committer_ਸਮਾਂ);
	ਲਿਖੋ("committer-tz %s\n", ci.committer_tz.buf);
	ਲਿਖੋ("summary %s\n", ci.summary.buf);
	ਜੇ (suspect->commit->object.flags & UNINTERESTING)
		ਲਿਖੋ("boundary\n");

	commit_info_destroy(&ci);

	ਵਾਪਸ 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ਸਥਿਰ ਖਾਲੀ ਮਿਲਿਆ_guilty_ਇੰਦਰਾਜ(ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ ਤਰੱਕੀ_info *pi = (ਬਣਤਰ ਤਰੱਕੀ_info *)ਅੰਕੜੇ;

	ਜੇ (incremental) {
		ਬਣਤਰ blame_origin *suspect = ent->suspect;

		ਲਿਖੋ("%s %d %d %d\n",
		       oid_ਮੰਜ਼ਿਲ_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->ਅੰਕ_ਸਤਰਾਂ);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_ਸਤਰਾਂ += ent->ਅੰਕ_ਸਤਰਾਂ;
	display_ਤਰੱਕੀ(pi->ਤਰੱਕੀ, pi->blamed_ਸਤਰਾਂ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *format_ਸਮਾਂ(timestamp_t ਸਮਾਂ, ਅਚਲ ਅੱਖਰ *tz_str,
			       ਪੂਰਨ show_raw_ਸਮਾਂ)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਸਮਾਂ_buf = STRBUF_INIT;

	strbuf_reset(&ਸਮਾਂ_buf);
	ਜੇ (show_raw_ਸਮਾਂ) {
		strbuf_addf(&ਸਮਾਂ_buf, "%"PRItime" %s", ਸਮਾਂ, tz_str);
	}
	ਨਹੀਂਤਾਂ {
		ਅਚਲ ਅੱਖਰ *ਸਮਾਂ_str;
		ਮਾਪ_t ਸਮਾਂ_ਚੌੜਾਈ;
		ਪੂਰਨ tz;
		tz = atoi(tz_str);
		ਸਮਾਂ_str = show_date(ਸਮਾਂ, tz, blame_date_ਬਹੁਲਕ);
		strbuf_addstr(&ਸਮਾਂ_buf, ਸਮਾਂ_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		ਲਈ (ਸਮਾਂ_ਚੌੜਾਈ = utf8_strwidth(ਸਮਾਂ_str);
		     ਸਮਾਂ_ਚੌੜਾਈ < blame_date_ਚੌੜਾਈ;
		     ਸਮਾਂ_ਚੌੜਾਈ++)
			strbuf_addch(&ਸਮਾਂ_buf, ' ');
	}
	ਵਾਪਸ ਸਮਾਂ_buf.buf;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_RAW_TIMESTAMP        (1U<<2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_PORCELAIN            (1U<<3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_SHOW_NAME            (1U<<4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_SHOW_ਸੰਖਿਆ          (1U<<5)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_SHOW_SCORE           (1U<<6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_NO_AUTHOR            (1U<<7)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_SHOW_EMAIL           (1U<<8)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_ਸਤਰ_PORCELAIN       (1U<<9)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_COLOR_ਸਤਰ           (1U<<10)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ਸਥਿਰ ਖਾਲੀ emit_porcelain_details(ਬਣਤਰ blame_origin *suspect, ਪੂਰਨ repeat)
{
	ਜੇ (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & ਹੋਰ_THAN_ONE_ਰਸਤਾ))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ਸਥਿਰ ਖਾਲੀ emit_porcelain_ਪ੍ਰਤੀ_ਸਤਰ_details(ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent)
{
	ਜੇ (mark_unblamable_ਸਤਰਾਂ && ent->unblamable)
		puts("unblamable");
	ਜੇ (mark_ignored_ਸਤਰਾਂ && ent->ignored)
		puts("ignored");
}

ਸਥਿਰ ਖਾਲੀ emit_porcelain(ਬਣਤਰ blame_scoreboard *sb, ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent,
			   ਪੂਰਨ opt)
{
	ਪੂਰਨ repeat = opt & OUTPUT_ਸਤਰ_PORCELAIN;
	ਪੂਰਨ cnt;
	ਅਚਲ ਅੱਖਰ *cp;
	ਬਣਤਰ blame_origin *suspect = ent->suspect;
	ਅੱਖਰ hex[GIT_MAX_HEXSZ + 1];

	oid_ਮੰਜ਼ਿਲ_hex_r(hex, &suspect->commit->object.oid);
	ਲਿਖੋ("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->ਅੰਕ_ਸਤਰਾਂ);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_ਪ੍ਰਤੀ_ਸਤਰ_details(ent);

	cp = blame_nth_ਸਤਰ(sb, ent->lno);
	ਲਈ (cnt = 0; cnt < ent->ਅੰਕ_ਸਤਰਾਂ; cnt++) {
		ਅੱਖਰ ch;
		ਜੇ (cnt) {
			ਲਿਖੋ("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			ਜੇ (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_ਪ੍ਰਤੀ_ਸਤਰ_details(ent);
		}
		ਅੱਖਰ_ਲਿਖੋ('\t');
		ਕਰੋ {
			ch = *cp++;
			ਅੱਖਰ_ਲਿਖੋ(ch);
		} ਜਦੋਂ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_ਮਾਪ);
	}

	ਜੇ (sb->final_buf_ਮਾਪ && cp[-1] != '\n')
		ਅੱਖਰ_ਲਿਖੋ('\n');
}

ਸਥਿਰ ਬਣਤਰ color_ਖੇਤਰ {
	timestamp_t hop;
	ਅੱਖਰ ਖਾਨਾ[COLOR_MAXLEN];
} *colorfield;
ਸਥਿਰ ਪੂਰਨ colorfield_nr, colorfield_alloc;

ਸਥਿਰ ਖਾਲੀ parse_color_fields(ਅਚਲ ਅੱਖਰ *s)
{
	ਬਣਤਰ string_list l = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਗਿਣਤੀ { EXPECT_DATE, EXPECT_COLOR } ਅਗਲਾ = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_ਵੰਡੋ(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &l) {
		ਚੋਣ (ਅਗਲਾ) {
		ਹਾਲਤ EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(ਵਸਤੂ->string);
			ਅਗਲਾ = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			ਰੋਕੋ;
		ਹਾਲਤ EXPECT_COLOR:
			ਜੇ (color_parse(ਵਸਤੂ->string, colorfield[colorfield_nr].ਖਾਨਾ))
				die(_("expecting a color: %s"), ਵਸਤੂ->string);
			ਅਗਲਾ = EXPECT_DATE;
			ਰੋਕੋ;
		}
	}

	ਜੇ (ਅਗਲਾ == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = ਸਮਾਂ_MAX;
	string_list_clear(&l, 0);
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_default_color_by_age(ਖਾਲੀ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ਸਥਿਰ ਖਾਲੀ determine_ਸਤਰ_heat(ਬਣਤਰ commit_info *ci, ਅਚਲ ਅੱਖਰ **dest_color)
{
	ਪੂਰਨ i = 0;

	ਜਦੋਂ (i < colorfield_nr && ci->author_ਸਮਾਂ > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].ਖਾਨਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ maybe_putc(ਪੂਰਨ c, FILE *ਬਾਹਰਲਾ)
{
	ਵਾਪਸ ਬਾਹਰਲਾ ? putc(c, ਬਾਹਰਲਾ) : 0;
}

ਸਥਿਰ ਮਾਪ_t print_marks(FILE *ਬਾਹਰਲਾ, ਅਚਲ ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent, ਪੂਰਨ opt)
{
	ਮਾਪ_t len = 0;

	ਜੇ ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', ਬਾਹਰਲਾ);
		len++;
	}
	ਜੇ (mark_unblamable_ਸਤਰਾਂ && ent->unblamable) {
		maybe_putc('*', ਬਾਹਰਲਾ);
		len++;
	}
	ਜੇ (mark_ignored_ਸਤਰਾਂ && ent->ignored) {
		maybe_putc('?', ਬਾਹਰਲਾ);
		len++;
	}
	ਵਾਪਸ len;
}

ਸਥਿਰ ਮਾਪ_t ਗਿਣਤੀ_marks(ਅਚਲ ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent, ਪੂਰਨ opt)
{
	ਵਾਪਸ print_marks(ਨਿਰਾ, ent, opt);
}

ਸਥਿਰ ਖਾਲੀ emit_other(ਬਣਤਰ blame_scoreboard *sb, ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent,
		       ਪੂਰਨ opt, ਬਣਤਰ blame_ਇੰਦਰਾਜ *prev_ent)
{
	ਪੂਰਨ cnt;
	ਅਚਲ ਅੱਖਰ *cp;
	ਬਣਤਰ blame_origin *suspect = ent->suspect;
	ਬਣਤਰ commit_info ci = COMMIT_INFO_INIT;
	ਅੱਖਰ hex[GIT_MAX_HEXSZ + 1];
	ਪੂਰਨ show_raw_ਸਮਾਂ = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ਅਚਲ ਅੱਖਰ *default_color = ਨਿਰਾ, *color = ਨਿਰਾ, *reset = ਨਿਰਾ;

	get_commit_info(suspect->commit, &ci);
	oid_ਮੰਜ਼ਿਲ_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_ਸਤਰ(sb, ent->lno);

	ਜੇ (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_ਸਤਰ_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	ਜੇ (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	ਲਈ (cnt = 0; cnt < ent->ਅੰਕ_ਸਤਰਾਂ; cnt++) {
		ਅੱਖਰ ch;
		ਮਾਪ_t ਲੰਮਾਈ = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (ਮਾਪ_t) abbrev;

		ਜੇ (opt & OUTPUT_COLOR_ਸਤਰ) {
			ਜੇ (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} ਨਹੀਂਤਾਂ  {
				color = default_color ? default_color : ਨਿਰਾ;
				reset = default_color ? GIT_COLOR_RESET : ਨਿਰਾ;
			}
		}
		ਜੇ (color)
			fputs(color, stdout);

		ਜੇ ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', ਲੜੀ_ਲੰਬਾਈ(hex));
		ਲੰਮਾਈ -= print_marks(stdout, ent, opt);

		ਲਿਖੋ("%.*s", (ਪੂਰਨ)(ਲੰਮਾਈ < GIT_MAX_HEXSZ ? ਲੰਮਾਈ : GIT_MAX_HEXSZ), hex);
		ਜੇ (opt & OUTPUT_ANNOTATE_COMPAT) {
			ਅਚਲ ਅੱਖਰ *name;
			ਜੇ (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			ਨਹੀਂਤਾਂ
				name = ci.author.buf;
			ਲਿਖੋ("\t(%10s\t%10s\t%d)", name,
			       format_ਸਮਾਂ(ci.author_ਸਮਾਂ, ci.author_tz.buf,
					   show_raw_ਸਮਾਂ),
			       ent->lno + 1 + cnt);
		} ਨਹੀਂਤਾਂ {
			ਜੇ (opt & OUTPUT_SHOW_SCORE)
				ਲਿਖੋ(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			ਜੇ (opt & OUTPUT_SHOW_NAME)
				ਲਿਖੋ(" %-*.*s", longest_file, longest_file,
				       suspect->ਰਸਤਾ);
			ਜੇ (opt & OUTPUT_SHOW_ਸੰਖਿਆ)
				ਲਿਖੋ(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			ਜੇ (!(opt & OUTPUT_NO_AUTHOR)) {
				ਅਚਲ ਅੱਖਰ *name;
				ਪੂਰਨ pad;
				ਜੇ (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				ਨਹੀਂਤਾਂ
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				ਲਿਖੋ(" (%s%*s %10s",
				       name, pad, "",
				       format_ਸਮਾਂ(ci.author_ਸਮਾਂ,
						   ci.author_tz.buf,
						   show_raw_ਸਮਾਂ));
			}
			ਲਿਖੋ(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		ਜੇ (reset)
			fputs(reset, stdout);
		ਕਰੋ {
			ch = *cp++;
			ਅੱਖਰ_ਲਿਖੋ(ch);
		} ਜਦੋਂ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_ਮਾਪ);
	}

	ਜੇ (sb->final_buf_ਮਾਪ && cp[-1] != '\n')
		ਅੱਖਰ_ਲਿਖੋ('\n');

	commit_info_destroy(&ci);
}

ਸਥਿਰ ਖਾਲੀ output(ਬਣਤਰ blame_scoreboard *sb, ਪੂਰਨ option)
{
	ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent, *prev_ent = ਨਿਰਾ;

	ਜੇ (option & OUTPUT_PORCELAIN) {
		ਲਈ (ent = sb->ent; ent; ent = ent->ਅਗਲਾ) {
			ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
			ਬਣਤਰ blame_origin *suspect;
			ਬਣਤਰ commit *commit = ent->suspect->commit;
			ਜੇ (commit->object.flags & ਹੋਰ_THAN_ONE_ਰਸਤਾ)
				ਜਾਰੀ;
			ਲਈ (suspect = get_blame_suspects(commit); suspect; suspect = suspect->ਅਗਲਾ) {
				ਜੇ (suspect->guilty && ਗਿਣਤੀ_2++) {
					commit->object.flags |= ਹੋਰ_THAN_ONE_ਰਸਤਾ;
					ਰੋਕੋ;
				}
			}
		}
	}

	ਲਈ (ent = sb->ent; ent; ent = ent->ਅਗਲਾ) {
		ਜੇ (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		ਨਹੀਂਤਾਂ {
			emit_other(sb, ent, option, prev_ent);
			prev_ent = ent;
		}
	}
}

/*
 * Add phony grafts for use with -S; this is primarily to
 * support git's cvsserver that wants to give a linear history
 * to its clients.
 */
ਸਥਿਰ ਪੂਰਨ read_ancestry(ਅਚਲ ਅੱਖਰ *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਜੇ (!fp)
		ਵਾਪਸ -1;
	ਜਦੋਂ (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ਬਣਤਰ commit_graft *graft = read_graft_ਸਤਰ(&buf);
		ਜੇ (graft)
			ਰਜਿਸਟਰ_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ update_auto_abbrev(ਪੂਰਨ auto_abbrev, ਬਣਤਰ blame_origin *suspect)
{
	ਅਚਲ ਅੱਖਰ *uniq = repo_ਲੱਭੋ_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(uniq);
	ਜੇ (auto_abbrev < len)
		ਵਾਪਸ len;
	ਵਾਪਸ auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ਸਥਿਰ ਖਾਲੀ ਲੱਭੋ_alignment(ਬਣਤਰ blame_scoreboard *sb, ਪੂਰਨ *option)
{
	ਪੂਰਨ longest_src_ਸਤਰਾਂ = 0;
	ਪੂਰਨ longest_dst_ਸਤਰਾਂ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਭ_ਤੋਂ_ਵੱਡਾ_score = 0;
	ਬਣਤਰ blame_ਇੰਦਰਾਜ *e;
	ਪੂਰਨ compute_auto_abbrev = (abbrev < 0);
	ਪੂਰਨ auto_abbrev = DEFAULT_ABBREV;
	ਮਾਪ_t max_marks_ਗਿਣਤੀ = 0;

	ਲਈ (e = sb->ent; e; e = e->ਅਗਲਾ) {
		ਬਣਤਰ blame_origin *suspect = e->suspect;
		ਪੂਰਨ ਅੰਕ;
		ਮਾਪ_t marks_ਗਿਣਤੀ = ਗਿਣਤੀ_marks(e, *option);

		ਜੇ (max_marks_ਗਿਣਤੀ < marks_ਗਿਣਤੀ)
			max_marks_ਗਿਣਤੀ = marks_ਗਿਣਤੀ;
		ਜੇ (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		ਜੇ (strcmp(suspect->ਰਸਤਾ, sb->ਰਸਤਾ))
			*option |= OUTPUT_SHOW_NAME;
		ਅੰਕ = ਲੜੀ_ਲੰਬਾਈ(suspect->ਰਸਤਾ);
		ਜੇ (longest_file < ਅੰਕ)
			longest_file = ਅੰਕ;
		ਜੇ (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ਬਣਤਰ commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			ਜੇ (*option & OUTPUT_SHOW_EMAIL)
				ਅੰਕ = utf8_strwidth(ci.author_mail.buf);
			ਨਹੀਂਤਾਂ
				ਅੰਕ = utf8_strwidth(ci.author.buf);
			ਜੇ (longest_author < ਅੰਕ)
				longest_author = ਅੰਕ;
			commit_info_destroy(&ci);
		}
		ਅੰਕ = e->s_lno + e->ਅੰਕ_ਸਤਰਾਂ;
		ਜੇ (longest_src_ਸਤਰਾਂ < ਅੰਕ)
			longest_src_ਸਤਰਾਂ = ਅੰਕ;
		ਅੰਕ = e->lno + e->ਅੰਕ_ਸਤਰਾਂ;
		ਜੇ (longest_dst_ਸਤਰਾਂ < ਅੰਕ)
			longest_dst_ਸਤਰਾਂ = ਅੰਕ;
		ਜੇ (ਸਭ_ਤੋਂ_ਵੱਡਾ_score < blame_ਇੰਦਰਾਜ_score(sb, e))
			ਸਭ_ਤੋਂ_ਵੱਡਾ_score = blame_ਇੰਦਰਾਜ_score(sb, e);
	}
	max_orig_digits = decimal_ਚੌੜਾਈ(longest_src_ਸਤਰਾਂ);
	max_digits = decimal_ਚੌੜਾਈ(longest_dst_ਸਤਰਾਂ);
	max_score_digits = decimal_ਚੌੜਾਈ(ਸਭ_ਤੋਂ_ਵੱਡਾ_score);

	ਜੇ (compute_auto_abbrev)
		abbrev = auto_abbrev;
	ਜੇ (abbrev < (ਪੂਰਨ)the_hash_algo->hexsz) {
		abbrev += max_marks_ਗਿਣਤੀ;
		ਜੇ (abbrev > (ਪੂਰਨ)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ਸਥਿਰ ਖਾਲੀ sanity_check_on_fail(ਬਣਤਰ blame_scoreboard *sb, ਪੂਰਨ baa)
{
	ਪੂਰਨ opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_ਸੰਖਿਆ | OUTPUT_SHOW_NAME;
	ਲੱਭੋ_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ parse_score(ਅਚਲ ਅੱਖਰ *arg)
{
	ਅੱਖਰ *ਸਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ score = strtoul(arg, &ਸਿਰਾ, 10);
	ਜੇ (*ਸਿਰਾ)
		ਵਾਪਸ 0;
	ਵਾਪਸ score;
}

ਸਥਿਰ ਅੱਖਰ *ਜੋੜੋ_ਅਗੇਤਰ(ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਵਾਪਸ ਅਗੇਤਰ_ਰਸਤਾ(the_repository, ਅਗੇਤਰ, ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0, ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ git_blame_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "blame.showroot")) {
		show_ਜੜ੍ਹ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "blame.showemail")) {
		ਪੂਰਨ *output_option = cb;
		ਜੇ (git_config_bool(var, ਮੁੱਲ))
			*output_option |= OUTPUT_SHOW_EMAIL;
		ਨਹੀਂਤਾਂ
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "blame.date")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		parse_date_format(ਮੁੱਲ, &blame_date_ਬਹੁਲਕ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "blame.ignorerevsfile")) {
		ਅੱਖਰ *str;
		ਪੂਰਨ ret;

		ret = git_config_pathname(&str, var, ਮੁੱਲ);
		ਜੇ (ret)
			ਵਾਪਸ ret;
		ਜੇ (str)
			string_list_ਪਾਓ(&ignore_revs_file_list, str);
		ਮੁਕਤ(str);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_ਸਤਰਾਂ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_ਸਤਰਾਂ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "color.blame.repeatedlines")) {
		ਜੇ (color_parse_mem(ਮੁੱਲ, ਲੜੀ_ਲੰਬਾਈ(ਮੁੱਲ), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "blame.coloring")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਜੇ (!strcmp(ਮੁੱਲ, "repeatedLines")) {
			coloring_ਬਹੁਲਕ |= OUTPUT_COLOR_ਸਤਰ;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਮੁੱਲ, "highlightRecent")) {
			coloring_ਬਹੁਲਕ |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਮੁੱਲ, "none")) {
			coloring_ਬਹੁਲਕ &= ~(OUTPUT_COLOR_ਸਤਰ |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} ਨਹੀਂਤਾਂ {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", ਮੁੱਲ);
			ਵਾਪਸ 0;
		}
	}

	ਜੇ (!strcmp(var, "diff.algorithm")) {
		ਲੰਮਾ diff_algorithm;
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		diff_algorithm = parse_algorithm_ਮੁੱਲ(ਮੁੱਲ);
		ਜੇ (diff_algorithm < 0)
			ਵਾਪਸ error(_("unknown value for config '%s': %s"),
				     var, ਮੁੱਲ);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_ਨਕਾਬ;
		xdl_opts |= diff_algorithm;
		ਵਾਪਸ 0;
	}

	ਜੇ (git_diff_heuristic_config(var, ਮੁੱਲ, cb) < 0)
		ਵਾਪਸ -1;
	ਜੇ (userdiff_config(var, ਮੁੱਲ) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਪੂਰਨ blame_copy_callback(ਅਚਲ ਬਣਤਰ option *option, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt = option->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	ਜੇ (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	ਜੇ (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	ਜੇ (arg)
		blame_copy_score = parse_score(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ blame_move_callback(ਅਚਲ ਬਣਤਰ option *option, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt = option->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	ਜੇ (arg)
		blame_move_score = parse_score(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ blame_diff_algorithm_ਨਿਊਨਤਮ(ਅਚਲ ਬਣਤਰ option *option,
					ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt = option->ਮੁੱਲ;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_ਨਕਾਬ;
	ਜੇ (!unset)
		*opt |= XDF_NEED_ਨਿਊਨਤਮ;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ blame_diff_algorithm_callback(ਅਚਲ ਬਣਤਰ option *option,
					 ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt = option->ਮੁੱਲ;
	ਲੰਮਾ ਮੁੱਲ = parse_algorithm_ਮੁੱਲ(arg);

	BUG_ON_OPT_NEG(unset);

	ਜੇ (ਮੁੱਲ < 0)
		ਵਾਪਸ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_ਨਕਾਬ;
	*opt |= ਮੁੱਲ;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ is_a_rev(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ object_id oid;

	ਜੇ (repo_get_oid(the_repository, name, &oid))
		ਵਾਪਸ 0;
	ਵਾਪਸ OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ peel_ਮੰਜ਼ਿਲ_commit_oid(ਬਣਤਰ object_id *oid_ret, ਖਾਲੀ *cbdata)
{
	ਬਣਤਰ repository *r = ((ਬਣਤਰ blame_scoreboard *)cbdata)->repo;
	ਬਣਤਰ object_id oid;

	oidcpy(&oid, oid_ret);
	ਜਦੋਂ (1) {
		ਬਣਤਰ object *obj;
		ਪੂਰਨ ਕਿਸਮ = odb_read_object_info(r->objects, &oid, ਨਿਰਾ);
		ਜੇ (ਕਿਸਮ == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			ਵਾਪਸ 0;
		}
		ਜੇ (ਕਿਸਮ != OBJ_ਟੈਗ)
			ਵਾਪਸ -1;
		obj = deref_ਟੈਗ(r, parse_object(r, &oid), ਨਿਰਾ, 0);
		ਜੇ (!obj)
			ਵਾਪਸ -1;
		oidcpy(&oid, &obj->oid);
	}
}

ਸਥਿਰ ਖਾਲੀ build_ignorelist(ਬਣਤਰ blame_scoreboard *sb,
			     ਬਣਤਰ string_list *ignore_revs_file_list,
			     ਬਣਤਰ string_list *ignore_rev_list)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *i;
	ਬਣਤਰ object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_ਵਸਤੂ(i, ignore_revs_file_list) {
		ਜੇ (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		ਨਹੀਂਤਾਂ
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_ਮੰਜ਼ਿਲ_commit_oid, sb);
	}
	for_each_string_list_ਵਸਤੂ(i, ignore_rev_list) {
		ਜੇ (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_ਮੰਜ਼ਿਲ_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ਪਾਓ(&sb->ignore_list, &oid);
	}
}

ਪੂਰਨ cmd_blame(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ rev_info revs;
	ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
	ਬਣਤਰ blame_scoreboard sb;
	ਬਣਤਰ blame_origin *o;
	ਬਣਤਰ blame_ਇੰਦਰਾਜ *ent = ਨਿਰਾ;
	ਲੰਮਾ dashdash_pos, lno;
	ਬਣਤਰ ਤਰੱਕੀ_info pi = { ਨਿਰਾ, 0 };

	ਬਣਤਰ string_list range_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	ਪੂਰਨ output_option = 0, opt = 0;
	ਪੂਰਨ show_stats = 0;
	ਅਚਲ ਅੱਖਰ *revs_file = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *contents_from = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ਨਿਰਾ, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ਜੜ੍ਹ, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_ਤਰੱਕੀ, N_("force progress reporting")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_ਸੰਖਿਆ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_ਸਤਰ_PORCELAIN),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('c', ਨਿਰਾ, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('t', ਨਿਰਾ, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('l', ਨਿਰਾ, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('s', ਨਿਰਾ, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('w', ਨਿਰਾ, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_ਸਤਰ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ਨਿਰਾ,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_ਨਿਊਨਤਮ),
		OPT_STRING('S', ਨਿਰਾ, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ਨਿਰਾ, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ਨਿਰਾ, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ਨਿਰਾ, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_ਸਿਰਾ()
	};

	ਬਣਤਰ parse_opt_ctx_t ctx;
	ਪੂਰਨ cmd_is_annotate = !strcmp(argv[0], "annotate");
	ਬਣਤਰ range_ਸੈੱਟ ranges;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ range_i;
	ਲੰਮਾ anchor;
	ਲੰਮਾ ਅੰਕ_ਸਤਰਾਂ = 0;
	ਅਚਲ ਅੱਖਰ *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ਅਚਲ ਅੱਖਰ *ਅਚਲ *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	ਤਿਆਰੀ_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
	revs.date_ਬਹੁਲਕ = blame_date_ਬਹੁਲਕ;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_ਬਫਰ = 0;
	dashdash_pos = 0;
	show_ਤਰੱਕੀ = -1;

	parse_options_ਸ਼ੁਰੂ(&ctx, argc, argv, ਅਗੇਤਰ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	ਲਈ (;;) {
		ਚੋਣ (parse_options_ਕਦਮ(&ctx, options, opt_usage)) {
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
			ਜੇ (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			ਜਾਓ parse_ਪੂਰਾ;
		}

		ਜੇ (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			ਉਲਟ = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_ਪੂਰਾ:
	revision_opts_ਅੰਤ(&revs);
	no_ਸਾਰਾ_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_ਸਿਰਾ(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (incremental || (output_option & OUTPUT_PORCELAIN)) {
		ਜੇ (show_ਤਰੱਕੀ > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_ਤਰੱਕੀ = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (show_ਤਰੱਕੀ < 0)
		show_ਤਰੱਕੀ = isatty(2);

	ਜੇ (!abbrev)
		abbrev = the_hash_algo->hexsz;

	ਜੇ (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	ਜੇ (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_ਬਹੁਲਕ.type = DATE_ISO8601;
	} ਨਹੀਂਤਾਂ {
		blame_date_ਬਹੁਲਕ = revs.date_ਬਹੁਲਕ;
	}

	/* The maximum width used to show the dates */
	ਚੋਣ (blame_date_ਬਹੁਲਕ.type) {
	ਹਾਲਤ DATE_RFC2822:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("Thu, 19 Oct 2006 16:00:04 -0700");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_ISO8601_STRICT:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("2006-10-19T16:00:04-07:00");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_ISO8601:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("2006-10-19 16:00:04 -0700");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_RAW:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("1161298804 -0700");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_UNIX:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("1161298804");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_SHORT:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("2006-10-19");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_RELATIVE:
		/*
		 * TRANSLATORS: This string is used to tell us the
		 * maximum display width for a relative timestamp in
		 * "git blame" output.  For C locale, "4 years, 11
		 * months ago", which takes 22 places, is the longest
		 * among various forms of relative timestamps, but
		 * your language may need more or fewer display
		 * columns.
		 */
		blame_date_ਚੌੜਾਈ = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		ਰੋਕੋ;
	ਹਾਲਤ DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("Thu Oct 19 16:00");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_ਸਾਧਾਰਨ:
		blame_date_ਚੌੜਾਈ = ਆਕਾਰ("Thu Oct 19 16:00:04 2006 -0700");
		ਰੋਕੋ;
	ਹਾਲਤ DATE_STRFTIME:
		blame_date_ਚੌੜਾਈ = ਲੜੀ_ਲੰਬਾਈ(show_date(0, 0, blame_date_ਬਹੁਲਕ)) + 1; /* add the null */
		ਰੋਕੋ;
	}
	blame_date_ਚੌੜਾਈ -= 1; /* strip the null */

	ਜੇ (revs.diffopt.flags.ਲੱਭੋ_copies_harder)
		opt |= (PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE |
			PICKAXE_BLAME_COPY_HARDER);

	/*
	 * We have collected options unknown to us in argv[1..unk]
	 * which are to be passed to revision machinery if we are
	 * going to do the "bottom" processing.
	 *
	 * The remaining are:
	 *
	 * (1) if dashdash_pos != 0, it is either
	 *     "blame [revisions] -- <path>" or
	 *     "blame -- <path> <rev>"
	 *
	 * (2) otherwise, it is one of the two:
	 *     "blame [revisions] <path>"
	 *     "blame <path> <rev>"
	 *
	 * Note that we must strip out <path> from the arguments: we do not
	 * want the path pruning but we may want "bottom" processing.
	 */
	ਜੇ (dashdash_pos) {
		ਚੋਣ (argc - dashdash_pos - 1) {
		ਹਾਲਤ 2: /* (1b) */
			ਜੇ (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		ਹਾਲਤ 1: /* (1a) */
			ਰਸਤਾ = ਜੋੜੋ_ਅਗੇਤਰ(ਅਗੇਤਰ, argv[--argc]);
			argv[argc] = ਨਿਰਾ;
			ਰੋਕੋ;
		ਮੂਲ:
			usage_with_options(opt_usage, options);
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (argc < 2)
			usage_with_options(opt_usage, options);
		ਜੇ (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			ਰਸਤਾ = ਜੋੜੋ_ਅਗੇਤਰ(ਅਗੇਤਰ, argv[1]);
			argv[1] = argv[2];
		} ਨਹੀਂਤਾਂ {	/* (2a) */
			ਜੇ (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_ਰੁੱਖ(the_repository))
				die("missing <path> to blame");
			ਰਸਤਾ = ਜੋੜੋ_ਅਗੇਤਰ(ਅਗੇਤਰ, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	ਤਿਆਰੀ_revisions(argc, argv, &revs, ਨਿਰਾ);
	ਜੇ (!revs.pending.nr && is_bare_repository(the_repository)) {
		ਬਣਤਰ commit *ਸਿਰ_commit;
		ਬਣਤਰ object_id ਸਿਰ_oid;

		ਜੇ (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &ਸਿਰ_oid, ਨਿਰਾ) ||
		    !(ਸਿਰ_commit = lookup_commit_reference_gently(revs.repo,
							     &ਸਿਰ_oid, 1)))
			die("no such ref: HEAD");

		ਜੋੜੋ_pending_object(&revs, &ਸਿਰ_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.ਉਲਟ = ਉਲਟ;
	sb.repo = the_repository;
	sb.ਰਸਤਾ = ਰਸਤਾ;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	ਤਿਆਰੀ_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	ਜੇ (!(opt & PICKAXE_BLAME_COPY))
		ਤਿਆਰੀ_blame_bloom_ਅੰਕੜੇ(&sb);

	lno = sb.ਅੰਕ_ਸਤਰਾਂ;

	ਜੇ (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_ਸੈੱਟ_init(&ranges, range_list.nr);
	ਲਈ (range_i = 0; range_i < range_list.nr; ++range_i) {
		ਲੰਮਾ bottom, ਸਿਖਰ;
		ਜੇ (parse_range_arg(range_list.ਚੀਜ਼ਾਂ[range_i].string,
				    nth_ਸਤਰ_cb, &sb, lno, anchor,
				    &bottom, &ਸਿਖਰ, sb.ਰਸਤਾ,
				    the_repository->ਸੂਚਕਾਂਕ))
			usage(str_usage);
		ਜੇ ((!lno && (ਸਿਖਰ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.ਰਸਤਾ, lno);
		ਜੇ (bottom < 1)
			bottom = 1;
		ਜੇ (ਸਿਖਰ < 1 || lno < ਸਿਖਰ)
			ਸਿਖਰ = lno;
		bottom--;
		range_ਸੈੱਟ_append_unsafe(&ranges, bottom, ਸਿਖਰ);
		anchor = ਸਿਖਰ + 1;
	}
	ਛਾਂਟੋ_and_ਮਿਲਾਓ_range_ਸੈੱਟ(&ranges);

	ਲਈ (range_i = ranges.nr; range_i > 0; --range_i) {
		ਅਚਲ ਬਣਤਰ range *r = &ranges.ranges[range_i - 1];
		ent = blame_ਇੰਦਰਾਜ_prepend(ent, r->ਸ਼ੁਰੂ, r->ਸਿਰਾ, o);
		ਅੰਕ_ਸਤਰਾਂ += (r->ਸਿਰਾ - r->ਸ਼ੁਰੂ);
	}
	ਜੇ (!ਅੰਕ_ਸਤਰਾਂ)
		ਅੰਕ_ਸਤਰਾਂ = sb.ਅੰਕ_ਸਤਰਾਂ;

	o->suspects = ent;
	prio_ਕਤਾਰ_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_ਸੈੱਟ_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ਨਿਰਾ;

	ਜੇ (blame_move_score)
		sb.move_score = blame_move_score;
	ਜੇ (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ਜੜ੍ਹ = show_ਜੜ੍ਹ;
	sb.xdl_opts = xdl_opts;
	sb.no_ਸਾਰਾ_file_rename = no_ਸਾਰਾ_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.ਮਿਲਿਆ_guilty_ਇੰਦਰਾਜ = &ਮਿਲਿਆ_guilty_ਇੰਦਰਾਜ;
	sb.ਮਿਲਿਆ_guilty_ਇੰਦਰਾਜ_ਅੰਕੜੇ = &pi;
	ਜੇ (show_ਤਰੱਕੀ)
		pi.ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(the_repository,
						     _("Blaming lines"),
						     ਅੰਕ_ਸਤਰਾਂ);

	ਸੌਂਪੋ_blame(&sb, opt);

	stop_ਤਰੱਕੀ(&pi.ਤਰੱਕੀ);

	ਜੇ (!incremental)
		ਤਿਆਰੀ_pager(the_repository);
	ਨਹੀਂਤਾਂ
		ਜਾਓ cleanup;

	blame_ਛਾਂਟੋ_final(&sb);

	blame_coalesce(&sb);

	ਜੇ (!(output_option & (OUTPUT_COLOR_ਸਤਰ | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_ਬਹੁਲਕ;

	ਜੇ (!(output_option & OUTPUT_PORCELAIN)) {
		ਲੱਭੋ_alignment(&sb, &output_option);
		ਜੇ (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_ਸਤਰ))
			xsnprintf(repeated_meta_color,
				  ਆਕਾਰ(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	ਜੇ (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_ਸਤਰ | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	ਜੇ (show_stats) {
		ਲਿਖੋ("num read blob: %d\n", sb.ਅੰਕ_read_blob);
		ਲਿਖੋ("num get patch: %d\n", sb.ਅੰਕ_get_patch);
		ਲਿਖੋ("num commits: %d\n", sb.ਅੰਕ_commits);
	}

cleanup:
	ਲਈ (ent = sb.ent; ent; ) {
		ਬਣਤਰ blame_ਇੰਦਰਾਜ *e = ent->ਅਗਲਾ;
		ਮੁਕਤ(ent);
		ent = e;
	}

	ਮੁਕਤ(ਰਸਤਾ);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	ਵਾਪਸ 0;
}
