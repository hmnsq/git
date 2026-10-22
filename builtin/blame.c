/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "mailmap.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "prio-queue.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "userdiff.h"
#ಅನ್ತರ್ಭಾವ "line-range.h"
#ಅನ್ತರ್ಭಾವ "line-log.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "blame.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ longest_file;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ longest_author;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_orig_digits;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_digits;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_score_digits;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಬೇರು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಹಿಮ್ಮುಖ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ blank_boundary;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ incremental;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xdl_opts;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abbrev = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ no_ಸಂಪೂರ್ಣ_file_rename;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ;
ಸ್ಥಿರ ಅಕ್ಷರ repeated_meta_color[COLOR_MAXLEN];
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ coloring_ಬಹುಲಕ;
ಸ್ಥಿರ ರಚನೆ string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_unblamable_ಸಾಲುಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_ignored_ಸಾಲುಗಳು;

ಸ್ಥಿರ ರಚನೆ date_ಬಹುಲಕ blame_date_ಬಹುಲಕ = { DATE_ISO8601 };
ಸ್ಥಿರ ಗಾತ್ರ_t blame_date_ಅಗಲ;

ಸ್ಥಿರ ರಚನೆ string_list mailmap = STRING_LIST_INIT_NODUP;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DEBUG_BLAME
#ವ್ಯಾಖ್ಯಾನಿಸು DEBUG_BLAME 0
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ blame_move_score;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ blame_copy_score;

/* Remember to update object flag allocation in object.h */
#ವ್ಯಾಖ್ಯಾನಿಸು METAINFO_SHOWN		(1u<<12)
#ವ್ಯಾಖ್ಯಾನಿಸು ಇನ್ನಷ್ಟು_THAN_ONE_ಮಾರ್ಗ	(1u<<13)

ರಚನೆ ಪ್ರಗತಿ_info {
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;
	ಪೂರ್ಣಾಂಕ blamed_ಸಾಲುಗಳು;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *nth_ಸಾಲು_cb(ಶೂನ್ಯ *ದತ್ತಾಂಶ, ದೀರ್ಘ lno)
{
	ಹಿಂತಿರುಗಿ blame_nth_ಸಾಲು((ರಚನೆ blame_scoreboard *)ದತ್ತಾಂಶ, lno);
}

/*
 * Information on commits, used for output.
 */
ರಚನೆ commit_info {
	ರಚನೆ strbuf author;
	ರಚನೆ strbuf author_mail;
	timestamp_t author_ಸಮಯ;
	ರಚನೆ strbuf author_tz;

	/* filled only when asked for details */
	ರಚನೆ strbuf committer;
	ರಚನೆ strbuf committer_mail;
	timestamp_t committer_ಸಮಯ;
	ರಚನೆ strbuf committer_tz;

	ರಚನೆ strbuf summary;
};

#ವ್ಯಾಖ್ಯಾನಿಸು COMMIT_INFO_INIT { \
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
ಸ್ಥಿರ ಶೂನ್ಯ get_ac_ಸಾಲು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *inbuf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *what,
	ರಚನೆ strbuf *name, ರಚನೆ strbuf *mail,
	timestamp_t *ಸಮಯ, ರಚನೆ strbuf *tz)
{
	ರಚನೆ ident_ವಿಭಜಿಸು ident;
	ಗಾತ್ರ_t len, maillen, namelen;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tmp, *endp;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *namebuf, *mailbuf;

	tmp = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(inbuf, what);
	ಆದರೆ (!tmp)
		ಹೋಗು error_ಹೊರಹೋಗು;
	tmp += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(what);
	endp = strchr(tmp, '\n');
	ಆದರೆ (!endp)
		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(tmp);
	ಇಲ್ಲದಿದ್ದರೆ
		len = endp - tmp;

	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&ident, tmp, len)) {
	error_ಹೊರಹೋಗು:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*ಸಮಯ = 0;
		ಹಿಂತಿರುಗಿ;
	}

	namelen = ident.name_ಕೊನೆ - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_ಕೊನೆ - ident.mail_begin;
	mailbuf = ident.mail_begin;

	ಆದರೆ (ident.date_begin && ident.date_ಕೊನೆ)
		*ಸಮಯ = strtoul(ident.date_begin, ಖಾಲಿ, 10);
	ಇಲ್ಲದಿದ್ದರೆ
		*ಸಮಯ = 0;

	ಆದರೆ (ident.tz_begin && ident.tz_ಕೊನೆ)
		strbuf_ಕೂಡಿಸು(tz, ident.tz_begin, ident.tz_ಕೊನೆ - ident.tz_begin);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (ಪೂರ್ಣಾಂಕ)maillen, mailbuf);
	strbuf_ಕೂಡಿಸು(name, namebuf, namelen);
}

ಸ್ಥಿರ ಶೂನ್ಯ commit_info_destroy(ರಚನೆ commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ಸ್ಥಿರ ಶೂನ್ಯ get_commit_info(ರಚನೆ commit *commit, ರಚನೆ commit_info *ret)
{
	ಪೂರ್ಣಾಂಕ len;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subject, *encoding;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2;

	encoding = get_log_output_encoding();
	ಸಂದೇಶ_2 = repo_logmsg_reencode(the_repository, commit, ಖಾಲಿ, encoding);
	get_ac_ಸಾಲು(ಸಂದೇಶ_2, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_ಸಮಯ, &ret->author_tz);

	get_ac_ಸಾಲು(ಸಂದೇಶ_2, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_ಸಮಯ, &ret->committer_tz);

	len = ಕಂಡುಹಿಡಿ_commit_subject(ಸಂದೇಶ_2, &subject);
	ಆದರೆ (len)
		strbuf_ಕೂಡಿಸು(&ret->summary, subject, len);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(&ret->summary, "(%s)", oid_ಗಮ್ಯ_hex(&commit->object.oid));

	repo_unuse_commit_ಬಫರ್(the_repository, commit, ಸಂದೇಶ_2);
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
ಸ್ಥಿರ ಶೂನ್ಯ write_filename_info(ರಚನೆ blame_origin *suspect)
{
	ಆದರೆ (suspect->ಹಿಂದಿನ) {
		ರಚನೆ blame_origin *prev = suspect->ಹಿಂದಿನ;
		ಬರೆ("previous %s ", oid_ಗಮ್ಯ_hex(&prev->commit->object.oid));
		write_name_quoted(prev->ಮಾರ್ಗ, stdout, '\n');
	}
	ಬರೆ("filename ");
	write_name_quoted(suspect->ಮಾರ್ಗ, stdout, '\n');
}

/*
 * Porcelain/Incremental format wants to show a lot of details per
 * commit.  Instead of repeating this every line, emit it only once,
 * the first time each commit appears in the output (unless the
 * user has specifically asked for us to repeat).
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ emit_one_suspect_detail(ರಚನೆ blame_origin *suspect, ಪೂರ್ಣಾಂಕ repeat)
{
	ರಚನೆ commit_info ci = COMMIT_INFO_INIT;

	ಆದರೆ (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		ಹಿಂತಿರುಗಿ 0;

	suspect->commit->object.flags |= METAINFO_SHOWN;
	get_commit_info(suspect->commit, &ci);
	ಬರೆ("author %s\n", ci.author.buf);
	ಬರೆ("author-mail %s\n", ci.author_mail.buf);
	ಬರೆ("author-time %"PRItime"\n", ci.author_ಸಮಯ);
	ಬರೆ("author-tz %s\n", ci.author_tz.buf);
	ಬರೆ("committer %s\n", ci.committer.buf);
	ಬರೆ("committer-mail %s\n", ci.committer_mail.buf);
	ಬರೆ("committer-time %"PRItime"\n", ci.committer_ಸಮಯ);
	ಬರೆ("committer-tz %s\n", ci.committer_tz.buf);
	ಬರೆ("summary %s\n", ci.summary.buf);
	ಆದರೆ (suspect->commit->object.flags & UNINTERESTING)
		ಬರೆ("boundary\n");

	commit_info_destroy(&ci);

	ಹಿಂತಿರುಗಿ 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಸಿಕ್ಕಿತು_guilty_ನಮೂದು(ರಚನೆ blame_ನಮೂದು *ent, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ ಪ್ರಗತಿ_info *pi = (ರಚನೆ ಪ್ರಗತಿ_info *)ದತ್ತಾಂಶ;

	ಆದರೆ (incremental) {
		ರಚನೆ blame_origin *suspect = ent->suspect;

		ಬರೆ("%s %d %d %d\n",
		       oid_ಗಮ್ಯ_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->ಅಂಕೆ_ಸಾಲುಗಳು);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_ಸಾಲುಗಳು += ent->ಅಂಕೆ_ಸಾಲುಗಳು;
	display_ಪ್ರಗತಿ(pi->ಪ್ರಗತಿ, pi->blamed_ಸಾಲುಗಳು);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format_ಸಮಯ(timestamp_t ಸಮಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tz_str,
			       ಪೂರ್ಣಾಂಕ show_raw_ಸಮಯ)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಸಮಯ_buf = STRBUF_INIT;

	strbuf_reset(&ಸಮಯ_buf);
	ಆದರೆ (show_raw_ಸಮಯ) {
		strbuf_addf(&ಸಮಯ_buf, "%"PRItime" %s", ಸಮಯ, tz_str);
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಮಯ_str;
		ಗಾತ್ರ_t ಸಮಯ_ಅಗಲ;
		ಪೂರ್ಣಾಂಕ tz;
		tz = atoi(tz_str);
		ಸಮಯ_str = show_date(ಸಮಯ, tz, blame_date_ಬಹುಲಕ);
		strbuf_addstr(&ಸಮಯ_buf, ಸಮಯ_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		ಗಾಗಿ (ಸಮಯ_ಅಗಲ = utf8_strwidth(ಸಮಯ_str);
		     ಸಮಯ_ಅಗಲ < blame_date_ಅಗಲ;
		     ಸಮಯ_ಅಗಲ++)
			strbuf_addch(&ಸಮಯ_buf, ' ');
	}
	ಹಿಂತಿರುಗಿ ಸಮಯ_buf.buf;
}

#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_RAW_TIMESTAMP        (1U<<2)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_PORCELAIN            (1U<<3)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_SHOW_NAME            (1U<<4)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_SHOW_ಸಂಖ್ಯೆ          (1U<<5)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_SHOW_SCORE           (1U<<6)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_NO_AUTHOR            (1U<<7)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_SHOW_EMAIL           (1U<<8)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_ಸಾಲು_PORCELAIN       (1U<<9)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_COLOR_ಸಾಲು           (1U<<10)
#ವ್ಯಾಖ್ಯಾನಿಸು OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ಸ್ಥಿರ ಶೂನ್ಯ emit_porcelain_details(ರಚನೆ blame_origin *suspect, ಪೂರ್ಣಾಂಕ repeat)
{
	ಆದರೆ (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & ಇನ್ನಷ್ಟು_THAN_ONE_ಮಾರ್ಗ))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ಸ್ಥಿರ ಶೂನ್ಯ emit_porcelain_ಪ್ರತಿ_ಸಾಲು_details(ರಚನೆ blame_ನಮೂದು *ent)
{
	ಆದರೆ (mark_unblamable_ಸಾಲುಗಳು && ent->unblamable)
		puts("unblamable");
	ಆದರೆ (mark_ignored_ಸಾಲುಗಳು && ent->ignored)
		puts("ignored");
}

ಸ್ಥಿರ ಶೂನ್ಯ emit_porcelain(ರಚನೆ blame_scoreboard *sb, ರಚನೆ blame_ನಮೂದು *ent,
			   ಪೂರ್ಣಾಂಕ opt)
{
	ಪೂರ್ಣಾಂಕ repeat = opt & OUTPUT_ಸಾಲು_PORCELAIN;
	ಪೂರ್ಣಾಂಕ cnt;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp;
	ರಚನೆ blame_origin *suspect = ent->suspect;
	ಅಕ್ಷರ hex[GIT_MAX_HEXSZ + 1];

	oid_ಗಮ್ಯ_hex_r(hex, &suspect->commit->object.oid);
	ಬರೆ("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->ಅಂಕೆ_ಸಾಲುಗಳು);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_ಪ್ರತಿ_ಸಾಲು_details(ent);

	cp = blame_nth_ಸಾಲು(sb, ent->lno);
	ಗಾಗಿ (cnt = 0; cnt < ent->ಅಂಕೆ_ಸಾಲುಗಳು; cnt++) {
		ಅಕ್ಷರ ch;
		ಆದರೆ (cnt) {
			ಬರೆ("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			ಆದರೆ (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_ಪ್ರತಿ_ಸಾಲು_details(ent);
		}
		ಅಕ್ಷರ_ಬರೆ('\t');
		ಮಾಡು {
			ch = *cp++;
			ಅಕ್ಷರ_ಬರೆ(ch);
		} ಆಗ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_ಗಾತ್ರ);
	}

	ಆದರೆ (sb->final_buf_ಗಾತ್ರ && cp[-1] != '\n')
		ಅಕ್ಷರ_ಬರೆ('\n');
}

ಸ್ಥಿರ ರಚನೆ color_ಕ್ಷೇತ್ರ {
	timestamp_t hop;
	ಅಕ್ಷರ ಲಂಬಸಾಲು[COLOR_MAXLEN];
} *colorfield;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ colorfield_nr, colorfield_alloc;

ಸ್ಥಿರ ಶೂನ್ಯ parse_color_fields(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ರಚನೆ string_list l = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಎಣಿಕೆ { EXPECT_DATE, EXPECT_COLOR } ಮುಂದಿನ = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_ವಿಭಜಿಸು(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_ಅಂಶ(ಅಂಶ, &l) {
		ಆಯ್ಕೆ (ಮುಂದಿನ) {
		ಸ್ಥಿತಿ EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(ಅಂಶ->string);
			ಮುಂದಿನ = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ EXPECT_COLOR:
			ಆದರೆ (color_parse(ಅಂಶ->string, colorfield[colorfield_nr].ಲಂಬಸಾಲು))
				die(_("expecting a color: %s"), ಅಂಶ->string);
			ಮುಂದಿನ = EXPECT_DATE;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (ಮುಂದಿನ == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = ಸಮಯ_MAX;
	string_list_clear(&l, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_default_color_by_age(ಶೂನ್ಯ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ಸ್ಥಿರ ಶೂನ್ಯ determine_ಸಾಲು_heat(ರಚನೆ commit_info *ci, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **dest_color)
{
	ಪೂರ್ಣಾಂಕ i = 0;

	ಆಗ (i < colorfield_nr && ci->author_ಸಮಯ > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].ಲಂಬಸಾಲು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ maybe_putc(ಪೂರ್ಣಾಂಕ c, FILE *ಹೊರಹೋಗು)
{
	ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು ? putc(c, ಹೊರಹೋಗು) : 0;
}

ಸ್ಥಿರ ಗಾತ್ರ_t print_marks(FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ರಚನೆ blame_ನಮೂದು *ent, ಪೂರ್ಣಾಂಕ opt)
{
	ಗಾತ್ರ_t len = 0;

	ಆದರೆ ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', ಹೊರಹೋಗು);
		len++;
	}
	ಆದರೆ (mark_unblamable_ಸಾಲುಗಳು && ent->unblamable) {
		maybe_putc('*', ಹೊರಹೋಗು);
		len++;
	}
	ಆದರೆ (mark_ignored_ಸಾಲುಗಳು && ent->ignored) {
		maybe_putc('?', ಹೊರಹೋಗು);
		len++;
	}
	ಹಿಂತಿರುಗಿ len;
}

ಸ್ಥಿರ ಗಾತ್ರ_t ಎಣಿಕೆ_marks(ಸ್ಥಿರಾಂಕ ರಚನೆ blame_ನಮೂದು *ent, ಪೂರ್ಣಾಂಕ opt)
{
	ಹಿಂತಿರುಗಿ print_marks(ಖಾಲಿ, ent, opt);
}

ಸ್ಥಿರ ಶೂನ್ಯ emit_other(ರಚನೆ blame_scoreboard *sb, ರಚನೆ blame_ನಮೂದು *ent,
		       ಪೂರ್ಣಾಂಕ opt, ರಚನೆ blame_ನಮೂದು *prev_ent)
{
	ಪೂರ್ಣಾಂಕ cnt;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp;
	ರಚನೆ blame_origin *suspect = ent->suspect;
	ರಚನೆ commit_info ci = COMMIT_INFO_INIT;
	ಅಕ್ಷರ hex[GIT_MAX_HEXSZ + 1];
	ಪೂರ್ಣಾಂಕ show_raw_ಸಮಯ = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *default_color = ಖಾಲಿ, *color = ಖಾಲಿ, *reset = ಖಾಲಿ;

	get_commit_info(suspect->commit, &ci);
	oid_ಗಮ್ಯ_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_ಸಾಲು(sb, ent->lno);

	ಆದರೆ (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_ಸಾಲು_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	ಆದರೆ (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	ಗಾಗಿ (cnt = 0; cnt < ent->ಅಂಕೆ_ಸಾಲುಗಳು; cnt++) {
		ಅಕ್ಷರ ch;
		ಗಾತ್ರ_t ಉದ್ದಳತೆ = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (ಗಾತ್ರ_t) abbrev;

		ಆದರೆ (opt & OUTPUT_COLOR_ಸಾಲು) {
			ಆದರೆ (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} ಇಲ್ಲದಿದ್ದರೆ  {
				color = default_color ? default_color : ಖಾಲಿ;
				reset = default_color ? GIT_COLOR_RESET : ಖಾಲಿ;
			}
		}
		ಆದರೆ (color)
			fputs(color, stdout);

		ಆದರೆ ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(hex));
		ಉದ್ದಳತೆ -= print_marks(stdout, ent, opt);

		ಬರೆ("%.*s", (ಪೂರ್ಣಾಂಕ)(ಉದ್ದಳತೆ < GIT_MAX_HEXSZ ? ಉದ್ದಳತೆ : GIT_MAX_HEXSZ), hex);
		ಆದರೆ (opt & OUTPUT_ANNOTATE_COMPAT) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
			ಆದರೆ (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			ಇಲ್ಲದಿದ್ದರೆ
				name = ci.author.buf;
			ಬರೆ("\t(%10s\t%10s\t%d)", name,
			       format_ಸಮಯ(ci.author_ಸಮಯ, ci.author_tz.buf,
					   show_raw_ಸಮಯ),
			       ent->lno + 1 + cnt);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (opt & OUTPUT_SHOW_SCORE)
				ಬರೆ(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			ಆದರೆ (opt & OUTPUT_SHOW_NAME)
				ಬರೆ(" %-*.*s", longest_file, longest_file,
				       suspect->ಮಾರ್ಗ);
			ಆದರೆ (opt & OUTPUT_SHOW_ಸಂಖ್ಯೆ)
				ಬರೆ(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			ಆದರೆ (!(opt & OUTPUT_NO_AUTHOR)) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
				ಪೂರ್ಣಾಂಕ pad;
				ಆದರೆ (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				ಇಲ್ಲದಿದ್ದರೆ
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				ಬರೆ(" (%s%*s %10s",
				       name, pad, "",
				       format_ಸಮಯ(ci.author_ಸಮಯ,
						   ci.author_tz.buf,
						   show_raw_ಸಮಯ));
			}
			ಬರೆ(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		ಆದರೆ (reset)
			fputs(reset, stdout);
		ಮಾಡು {
			ch = *cp++;
			ಅಕ್ಷರ_ಬರೆ(ch);
		} ಆಗ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_ಗಾತ್ರ);
	}

	ಆದರೆ (sb->final_buf_ಗಾತ್ರ && cp[-1] != '\n')
		ಅಕ್ಷರ_ಬರೆ('\n');

	commit_info_destroy(&ci);
}

ಸ್ಥಿರ ಶೂನ್ಯ output(ರಚನೆ blame_scoreboard *sb, ಪೂರ್ಣಾಂಕ option)
{
	ರಚನೆ blame_ನಮೂದು *ent, *prev_ent = ಖಾಲಿ;

	ಆದರೆ (option & OUTPUT_PORCELAIN) {
		ಗಾಗಿ (ent = sb->ent; ent; ent = ent->ಮುಂದಿನ) {
			ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
			ರಚನೆ blame_origin *suspect;
			ರಚನೆ commit *commit = ent->suspect->commit;
			ಆದರೆ (commit->object.flags & ಇನ್ನಷ್ಟು_THAN_ONE_ಮಾರ್ಗ)
				ಮುಂದುವರಿ;
			ಗಾಗಿ (suspect = get_blame_suspects(commit); suspect; suspect = suspect->ಮುಂದಿನ) {
				ಆದರೆ (suspect->guilty && ಎಣಿಕೆ_2++) {
					commit->object.flags |= ಇನ್ನಷ್ಟು_THAN_ONE_ಮಾರ್ಗ;
					ನಿಲ್ಲಿಸು;
				}
			}
		}
	}

	ಗಾಗಿ (ent = sb->ent; ent; ent = ent->ಮುಂದಿನ) {
		ಆದರೆ (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		ಇಲ್ಲದಿದ್ದರೆ {
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_ancestry(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ -1;
	ಆಗ (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ರಚನೆ commit_graft *graft = read_graft_ಸಾಲು(&buf);
		ಆದರೆ (graft)
			ರಿಜಿಸ್ಟರ್_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_auto_abbrev(ಪೂರ್ಣಾಂಕ auto_abbrev, ರಚನೆ blame_origin *suspect)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *uniq = repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(uniq);
	ಆದರೆ (auto_abbrev < len)
		ಹಿಂತಿರುಗಿ len;
	ಹಿಂತಿರುಗಿ auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_alignment(ರಚನೆ blame_scoreboard *sb, ಪೂರ್ಣಾಂಕ *option)
{
	ಪೂರ್ಣಾಂಕ longest_src_ಸಾಲುಗಳು = 0;
	ಪೂರ್ಣಾಂಕ longest_dst_ಸಾಲುಗಳು = 0;
	ಚಿಹ್ನೆರಹಿತ ಅತಿದೊಡ್ಡ_score = 0;
	ರಚನೆ blame_ನಮೂದು *e;
	ಪೂರ್ಣಾಂಕ compute_auto_abbrev = (abbrev < 0);
	ಪೂರ್ಣಾಂಕ auto_abbrev = DEFAULT_ABBREV;
	ಗಾತ್ರ_t max_marks_ಎಣಿಕೆ = 0;

	ಗಾಗಿ (e = sb->ent; e; e = e->ಮುಂದಿನ) {
		ರಚನೆ blame_origin *suspect = e->suspect;
		ಪೂರ್ಣಾಂಕ ಅಂಕೆ;
		ಗಾತ್ರ_t marks_ಎಣಿಕೆ = ಎಣಿಕೆ_marks(e, *option);

		ಆದರೆ (max_marks_ಎಣಿಕೆ < marks_ಎಣಿಕೆ)
			max_marks_ಎಣಿಕೆ = marks_ಎಣಿಕೆ;
		ಆದರೆ (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		ಆದರೆ (strcmp(suspect->ಮಾರ್ಗ, sb->ಮಾರ್ಗ))
			*option |= OUTPUT_SHOW_NAME;
		ಅಂಕೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(suspect->ಮಾರ್ಗ);
		ಆದರೆ (longest_file < ಅಂಕೆ)
			longest_file = ಅಂಕೆ;
		ಆದರೆ (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ರಚನೆ commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			ಆದರೆ (*option & OUTPUT_SHOW_EMAIL)
				ಅಂಕೆ = utf8_strwidth(ci.author_mail.buf);
			ಇಲ್ಲದಿದ್ದರೆ
				ಅಂಕೆ = utf8_strwidth(ci.author.buf);
			ಆದರೆ (longest_author < ಅಂಕೆ)
				longest_author = ಅಂಕೆ;
			commit_info_destroy(&ci);
		}
		ಅಂಕೆ = e->s_lno + e->ಅಂಕೆ_ಸಾಲುಗಳು;
		ಆದರೆ (longest_src_ಸಾಲುಗಳು < ಅಂಕೆ)
			longest_src_ಸಾಲುಗಳು = ಅಂಕೆ;
		ಅಂಕೆ = e->lno + e->ಅಂಕೆ_ಸಾಲುಗಳು;
		ಆದರೆ (longest_dst_ಸಾಲುಗಳು < ಅಂಕೆ)
			longest_dst_ಸಾಲುಗಳು = ಅಂಕೆ;
		ಆದರೆ (ಅತಿದೊಡ್ಡ_score < blame_ನಮೂದು_score(sb, e))
			ಅತಿದೊಡ್ಡ_score = blame_ನಮೂದು_score(sb, e);
	}
	max_orig_digits = decimal_ಅಗಲ(longest_src_ಸಾಲುಗಳು);
	max_digits = decimal_ಅಗಲ(longest_dst_ಸಾಲುಗಳು);
	max_score_digits = decimal_ಅಗಲ(ಅತಿದೊಡ್ಡ_score);

	ಆದರೆ (compute_auto_abbrev)
		abbrev = auto_abbrev;
	ಆದರೆ (abbrev < (ಪೂರ್ಣಾಂಕ)the_hash_algo->hexsz) {
		abbrev += max_marks_ಎಣಿಕೆ;
		ಆದರೆ (abbrev > (ಪೂರ್ಣಾಂಕ)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sanity_check_on_fail(ರಚನೆ blame_scoreboard *sb, ಪೂರ್ಣಾಂಕ baa)
{
	ಪೂರ್ಣಾಂಕ opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_ಸಂಖ್ಯೆ | OUTPUT_SHOW_NAME;
	ಕಂಡುಹಿಡಿ_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ parse_score(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಅಕ್ಷರ *ಕೊನೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ score = strtoul(arg, &ಕೊನೆ, 10);
	ಆದರೆ (*ಕೊನೆ)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ score;
}

ಸ್ಥಿರ ಅಕ್ಷರ *ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0, ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_blame_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "blame.showroot")) {
		show_ಬೇರು = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "blame.showemail")) {
		ಪೂರ್ಣಾಂಕ *output_option = cb;
		ಆದರೆ (git_config_bool(var, ಮೌಲ್ಯ))
			*output_option |= OUTPUT_SHOW_EMAIL;
		ಇಲ್ಲದಿದ್ದರೆ
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "blame.date")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		parse_date_format(ಮೌಲ್ಯ, &blame_date_ಬಹುಲಕ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "blame.ignorerevsfile")) {
		ಅಕ್ಷರ *str;
		ಪೂರ್ಣಾಂಕ ret;

		ret = git_config_pathname(&str, var, ಮೌಲ್ಯ);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ ret;
		ಆದರೆ (str)
			string_list_ಸೇರಿಸು(&ignore_revs_file_list, str);
		ಬಿಡುಗಡೆ(str);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_ಸಾಲುಗಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_ಸಾಲುಗಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "color.blame.repeatedlines")) {
		ಆದರೆ (color_parse_mem(ಮೌಲ್ಯ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮೌಲ್ಯ), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "blame.coloring")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "repeatedLines")) {
			coloring_ಬಹುಲಕ |= OUTPUT_COLOR_ಸಾಲು;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "highlightRecent")) {
			coloring_ಬಹುಲಕ |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "none")) {
			coloring_ಬಹುಲಕ &= ~(OUTPUT_COLOR_ಸಾಲು |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} ಇಲ್ಲದಿದ್ದರೆ {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", ಮೌಲ್ಯ);
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಆದರೆ (!strcmp(var, "diff.algorithm")) {
		ದೀರ್ಘ diff_algorithm;
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		diff_algorithm = parse_algorithm_ಮೌಲ್ಯ(ಮೌಲ್ಯ);
		ಆದರೆ (diff_algorithm < 0)
			ಹಿಂತಿರುಗಿ error(_("unknown value for config '%s': %s"),
				     var, ಮೌಲ್ಯ);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_ಮುಖವಾಡ;
		xdl_opts |= diff_algorithm;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (git_diff_heuristic_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (userdiff_config(var, ಮೌಲ್ಯ) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ blame_copy_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	ಆದರೆ (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	ಆದರೆ (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	ಆದರೆ (arg)
		blame_copy_score = parse_score(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ blame_move_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	ಆದರೆ (arg)
		blame_move_score = parse_score(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ blame_diff_algorithm_ಕನಿಷ್ಠತಮ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option,
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_ಮುಖವಾಡ;
	ಆದರೆ (!unset)
		*opt |= XDF_NEED_ಕನಿಷ್ಠತಮ;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ blame_diff_algorithm_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *option,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt = option->ಮೌಲ್ಯ;
	ದೀರ್ಘ ಮೌಲ್ಯ = parse_algorithm_ಮೌಲ್ಯ(arg);

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (ಮೌಲ್ಯ < 0)
		ಹಿಂತಿರುಗಿ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_ಮುಖವಾಡ;
	*opt |= ಮೌಲ್ಯ;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_a_rev(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ object_id oid;

	ಆದರೆ (repo_get_oid(the_repository, name, &oid))
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ peel_ಗಮ್ಯ_commit_oid(ರಚನೆ object_id *oid_ret, ಶೂನ್ಯ *cbdata)
{
	ರಚನೆ repository *r = ((ರಚನೆ blame_scoreboard *)cbdata)->repo;
	ರಚನೆ object_id oid;

	oidcpy(&oid, oid_ret);
	ಆಗ (1) {
		ರಚನೆ object *obj;
		ಪೂರ್ಣಾಂಕ ಬಗೆ = odb_read_object_info(r->objects, &oid, ಖಾಲಿ);
		ಆದರೆ (ಬಗೆ == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆದರೆ (ಬಗೆ != OBJ_ಟ್ಯಾಗ್)
			ಹಿಂತಿರುಗಿ -1;
		obj = deref_ಟ್ಯಾಗ್(r, parse_object(r, &oid), ಖಾಲಿ, 0);
		ಆದರೆ (!obj)
			ಹಿಂತಿರುಗಿ -1;
		oidcpy(&oid, &obj->oid);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ build_ignorelist(ರಚನೆ blame_scoreboard *sb,
			     ರಚನೆ string_list *ignore_revs_file_list,
			     ರಚನೆ string_list *ignore_rev_list)
{
	ರಚನೆ string_list_ಅಂಶ *i;
	ರಚನೆ object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_ಅಂಶ(i, ignore_revs_file_list) {
		ಆದರೆ (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		ಇಲ್ಲದಿದ್ದರೆ
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_ಗಮ್ಯ_commit_oid, sb);
	}
	for_each_string_list_ಅಂಶ(i, ignore_rev_list) {
		ಆದರೆ (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_ಗಮ್ಯ_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ಸೇರಿಸು(&sb->ignore_list, &oid);
	}
}

ಪೂರ್ಣಾಂಕ cmd_blame(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ rev_info revs;
	ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
	ರಚನೆ blame_scoreboard sb;
	ರಚನೆ blame_origin *o;
	ರಚನೆ blame_ನಮೂದು *ent = ಖಾಲಿ;
	ದೀರ್ಘ dashdash_pos, lno;
	ರಚನೆ ಪ್ರಗತಿ_info pi = { ಖಾಲಿ, 0 };

	ರಚನೆ string_list range_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	ಪೂರ್ಣಾಂಕ output_option = 0, opt = 0;
	ಪೂರ್ಣಾಂಕ show_stats = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *revs_file = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *contents_from = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ಖಾಲಿ, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ಬೇರು, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_ಪ್ರಗತಿ, N_("force progress reporting")),
		OPT_ಬಿಟ್(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_ಬಿಟ್('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_ಬಿಟ್('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_ಸಂಖ್ಯೆ),
		OPT_ಬಿಟ್('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_ಬಿಟ್(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_ಸಾಲು_PORCELAIN),
		OPT_ಬಿಟ್('c', ಖಾಲಿ, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_ಬಿಟ್('t', ಖಾಲಿ, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_ಬಿಟ್('l', ಖಾಲಿ, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_ಬಿಟ್('s', ಖಾಲಿ, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_ಬಿಟ್('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_ಬಿಟ್('w', ಖಾಲಿ, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_ಬಿಟ್(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_ಸಾಲು),
		OPT_ಬಿಟ್(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ಖಾಲಿ,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_ಕನಿಷ್ಠತಮ),
		OPT_STRING('S', ಖಾಲಿ, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ಖಾಲಿ, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ಖಾಲಿ, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ಖಾಲಿ, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_ಕೊನೆ()
	};

	ರಚನೆ parse_opt_ctx_t ctx;
	ಪೂರ್ಣಾಂಕ cmd_is_annotate = !strcmp(argv[0], "annotate");
	ರಚನೆ range_ಗಣ ranges;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ range_i;
	ದೀರ್ಘ anchor;
	ದೀರ್ಘ ಅಂಕೆ_ಸಾಲುಗಳು = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	ಸಿದ್ಧತೆ_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
	revs.date_ಬಹುಲಕ = blame_date_ಬಹುಲಕ;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_ಬಫರ್ = 0;
	dashdash_pos = 0;
	show_ಪ್ರಗತಿ = -1;

	parse_options_ಆರಂಭ(&ctx, argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	ಗಾಗಿ (;;) {
		ಆಯ್ಕೆ (parse_options_ಹೆಜ್ಜೆ(&ctx, options, opt_usage)) {
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
			ಆದರೆ (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			ಹೋಗು parse_ಮುಗಿದ;
		}

		ಆದರೆ (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			ಹಿಮ್ಮುಖ = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_ಮುಗಿದ:
	revision_opts_ಮುಕ್ತಾಯ(&revs);
	no_ಸಂಪೂರ್ಣ_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_ಕೊನೆ(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (incremental || (output_option & OUTPUT_PORCELAIN)) {
		ಆದರೆ (show_ಪ್ರಗತಿ > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_ಪ್ರಗತಿ = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (show_ಪ್ರಗತಿ < 0)
		show_ಪ್ರಗತಿ = isatty(2);

	ಆದರೆ (!abbrev)
		abbrev = the_hash_algo->hexsz;

	ಆದರೆ (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	ಆದರೆ (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_ಬಹುಲಕ.type = DATE_ISO8601;
	} ಇಲ್ಲದಿದ್ದರೆ {
		blame_date_ಬಹುಲಕ = revs.date_ಬಹುಲಕ;
	}

	/* The maximum width used to show the dates */
	ಆಯ್ಕೆ (blame_date_ಬಹುಲಕ.type) {
	ಸ್ಥಿತಿ DATE_RFC2822:
		blame_date_ಅಗಲ = ಗಾತ್ರ("Thu, 19 Oct 2006 16:00:04 -0700");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_ISO8601_STRICT:
		blame_date_ಅಗಲ = ಗಾತ್ರ("2006-10-19T16:00:04-07:00");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_ISO8601:
		blame_date_ಅಗಲ = ಗಾತ್ರ("2006-10-19 16:00:04 -0700");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_RAW:
		blame_date_ಅಗಲ = ಗಾತ್ರ("1161298804 -0700");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_UNIX:
		blame_date_ಅಗಲ = ಗಾತ್ರ("1161298804");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_SHORT:
		blame_date_ಅಗಲ = ಗಾತ್ರ("2006-10-19");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_RELATIVE:
		/*
		 * TRANSLATORS: This string is used to tell us the
		 * maximum display width for a relative timestamp in
		 * "git blame" output.  For C locale, "4 years, 11
		 * months ago", which takes 22 places, is the longest
		 * among various forms of relative timestamps, but
		 * your language may need more or fewer display
		 * columns.
		 */
		blame_date_ಅಗಲ = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_ಅಗಲ = ಗಾತ್ರ("Thu Oct 19 16:00");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_ಸಾಮಾನ್ಯ:
		blame_date_ಅಗಲ = ಗಾತ್ರ("Thu Oct 19 16:00:04 2006 -0700");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DATE_STRFTIME:
		blame_date_ಅಗಲ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(show_date(0, 0, blame_date_ಬಹುಲಕ)) + 1; /* add the null */
		ನಿಲ್ಲಿಸು;
	}
	blame_date_ಅಗಲ -= 1; /* strip the null */

	ಆದರೆ (revs.diffopt.flags.ಕಂಡುಹಿಡಿ_copies_harder)
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
	ಆದರೆ (dashdash_pos) {
		ಆಯ್ಕೆ (argc - dashdash_pos - 1) {
		ಸ್ಥಿತಿ 2: /* (1b) */
			ಆದರೆ (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		ಸ್ಥಿತಿ 1: /* (1a) */
			ಮಾರ್ಗ = ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[--argc]);
			argv[argc] = ಖಾಲಿ;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			usage_with_options(opt_usage, options);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (argc < 2)
			usage_with_options(opt_usage, options);
		ಆದರೆ (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			ಮಾರ್ಗ = ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[1]);
			argv[1] = argv[2];
		} ಇಲ್ಲದಿದ್ದರೆ {	/* (2a) */
			ಆದರೆ (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_ವೃಕ್ಷ(the_repository))
				die("missing <path> to blame");
			ಮಾರ್ಗ = ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	ಸಿದ್ಧತೆ_revisions(argc, argv, &revs, ಖಾಲಿ);
	ಆದರೆ (!revs.pending.nr && is_bare_repository(the_repository)) {
		ರಚನೆ commit *ತಲೆ_commit;
		ರಚನೆ object_id ತಲೆ_oid;

		ಆದರೆ (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &ತಲೆ_oid, ಖಾಲಿ) ||
		    !(ತಲೆ_commit = lookup_commit_reference_gently(revs.repo,
							     &ತಲೆ_oid, 1)))
			die("no such ref: HEAD");

		ಕೂಡಿಸು_pending_object(&revs, &ತಲೆ_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.ಹಿಮ್ಮುಖ = ಹಿಮ್ಮುಖ;
	sb.repo = the_repository;
	sb.ಮಾರ್ಗ = ಮಾರ್ಗ;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	ಸಿದ್ಧತೆ_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	ಆದರೆ (!(opt & PICKAXE_BLAME_COPY))
		ಸಿದ್ಧತೆ_blame_bloom_ದತ್ತಾಂಶ(&sb);

	lno = sb.ಅಂಕೆ_ಸಾಲುಗಳು;

	ಆದರೆ (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_ಗಣ_init(&ranges, range_list.nr);
	ಗಾಗಿ (range_i = 0; range_i < range_list.nr; ++range_i) {
		ದೀರ್ಘ bottom, ತುದಿ;
		ಆದರೆ (parse_range_arg(range_list.ವಸ್ತುಗಳು[range_i].string,
				    nth_ಸಾಲು_cb, &sb, lno, anchor,
				    &bottom, &ತುದಿ, sb.ಮಾರ್ಗ,
				    the_repository->ಸೂಚ್ಯಂಕ))
			usage(str_usage);
		ಆದರೆ ((!lno && (ತುದಿ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.ಮಾರ್ಗ, lno);
		ಆದರೆ (bottom < 1)
			bottom = 1;
		ಆದರೆ (ತುದಿ < 1 || lno < ತುದಿ)
			ತುದಿ = lno;
		bottom--;
		range_ಗಣ_append_unsafe(&ranges, bottom, ತುದಿ);
		anchor = ತುದಿ + 1;
	}
	ವಿಂಗಡಿಸು_and_ವಿಲೀನ_range_ಗಣ(&ranges);

	ಗಾಗಿ (range_i = ranges.nr; range_i > 0; --range_i) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ range *r = &ranges.ranges[range_i - 1];
		ent = blame_ನಮೂದು_prepend(ent, r->ಆರಂಭ, r->ಕೊನೆ, o);
		ಅಂಕೆ_ಸಾಲುಗಳು += (r->ಕೊನೆ - r->ಆರಂಭ);
	}
	ಆದರೆ (!ಅಂಕೆ_ಸಾಲುಗಳು)
		ಅಂಕೆ_ಸಾಲುಗಳು = sb.ಅಂಕೆ_ಸಾಲುಗಳು;

	o->suspects = ent;
	prio_ಸರತಿ_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_ಗಣ_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ಖಾಲಿ;

	ಆದರೆ (blame_move_score)
		sb.move_score = blame_move_score;
	ಆದರೆ (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ಬೇರು = show_ಬೇರು;
	sb.xdl_opts = xdl_opts;
	sb.no_ಸಂಪೂರ್ಣ_file_rename = no_ಸಂಪೂರ್ಣ_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.ಸಿಕ್ಕಿತು_guilty_ನಮೂದು = &ಸಿಕ್ಕಿತು_guilty_ನಮೂದು;
	sb.ಸಿಕ್ಕಿತು_guilty_ನಮೂದು_ದತ್ತಾಂಶ = &pi;
	ಆದರೆ (show_ಪ್ರಗತಿ)
		pi.ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(the_repository,
						     _("Blaming lines"),
						     ಅಂಕೆ_ಸಾಲುಗಳು);

	ನಿಯೋಜಿಸು_blame(&sb, opt);

	stop_ಪ್ರಗತಿ(&pi.ಪ್ರಗತಿ);

	ಆದರೆ (!incremental)
		ಸಿದ್ಧತೆ_pager(the_repository);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹೋಗು cleanup;

	blame_ವಿಂಗಡಿಸು_final(&sb);

	blame_coalesce(&sb);

	ಆದರೆ (!(output_option & (OUTPUT_COLOR_ಸಾಲು | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_ಬಹುಲಕ;

	ಆದರೆ (!(output_option & OUTPUT_PORCELAIN)) {
		ಕಂಡುಹಿಡಿ_alignment(&sb, &output_option);
		ಆದರೆ (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_ಸಾಲು))
			xsnprintf(repeated_meta_color,
				  ಗಾತ್ರ(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	ಆದರೆ (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_ಸಾಲು | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	ಆದರೆ (show_stats) {
		ಬರೆ("num read blob: %d\n", sb.ಅಂಕೆ_read_blob);
		ಬರೆ("num get patch: %d\n", sb.ಅಂಕೆ_get_patch);
		ಬರೆ("num commits: %d\n", sb.ಅಂಕೆ_commits);
	}

cleanup:
	ಗಾಗಿ (ent = sb.ent; ent; ) {
		ರಚನೆ blame_ನಮೂದು *e = ent->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(ent);
		ent = e;
	}

	ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	ಹಿಂತಿರುಗಿ 0;
}
