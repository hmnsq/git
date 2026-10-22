/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ longest_file;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ longest_author;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_orig_digits;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_digits;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_score_digits;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ಬೇರು;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ reverse;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ blank_boundary;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ incremental;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xdl_opts;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abbrev = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ no_whole_file_rename;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_progress;
ಸ್ಥಿರ ವರ್ಣ repeated_meta_color[COLOR_MAXLEN];
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ coloring_mode;
ಸ್ಥಿರ ಸಂರಚನಾ string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_unblamable_ಸಾಲುಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_ignored_ಸಾಲುಗಳು;

ಸ್ಥಿರ ಸಂರಚನಾ date_mode blame_date_mode = { DATE_ISO8601 };
ಸ್ಥಿರ size_t blame_date_width;

ಸ್ಥಿರ ಸಂರಚನಾ string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

ಸ್ಥಿರ unsigned blame_move_score;
ಸ್ಥಿರ unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

ಸಂರಚನಾ progress_info {
	ಸಂರಚನಾ progress *progress;
	ಪೂರ್ಣಾಙ್ಕ blamed_ಸಾಲುಗಳು;
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *nth_ಸಾಲು_cb(ಶೂನ್ಯ *data, long lno)
{
	ಪ್ರತ್ಯಾಗಮ blame_nth_ಸಾಲು((ಸಂರಚನಾ blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
ಸಂರಚನಾ commit_info {
	ಸಂರಚನಾ strbuf author;
	ಸಂರಚನಾ strbuf author_mail;
	timestamp_t author_time;
	ಸಂರಚನಾ strbuf author_tz;

	/* filled only when asked for details */
	ಸಂರಚನಾ strbuf committer;
	ಸಂರಚನಾ strbuf committer_mail;
	timestamp_t committer_time;
	ಸಂರಚನಾ strbuf committer_tz;

	ಸಂರಚನಾ strbuf summary;
};

#define COMMIT_INFO_INIT { \
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
ಸ್ಥಿರ ಶೂನ್ಯ get_ac_ಸಾಲು(ಧ್ರುವ ವರ್ಣ *inbuf, ಧ್ರುವ ವರ್ಣ *what,
	ಸಂರಚನಾ strbuf *name, ಸಂರಚನಾ strbuf *mail,
	timestamp_t *time, ಸಂರಚನಾ strbuf *tz)
{
	ಸಂರಚನಾ ident_split ident;
	size_t len, maillen, namelen;
	ಧ್ರುವ ವರ್ಣ *tmp, *endp;
	ಧ್ರುವ ವರ್ಣ *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	ಯದಿ (!tmp)
		ಗಚ್ಛ error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	ಯದಿ (!endp)
		len = strlen(tmp);
	ಅನ್ಯಥಾ
		len = endp - tmp;

	ಯದಿ (split_ident_ಸಾಲು(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		ಪ್ರತ್ಯಾಗಮ;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	ಯದಿ (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, ರಿಕ್ತ, 10);
	ಅನ್ಯಥಾ
		*time = 0;

	ಯದಿ (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	ಅನ್ಯಥಾ
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (ಪೂರ್ಣಾಙ್ಕ)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

ಸ್ಥಿರ ಶೂನ್ಯ commit_info_destroy(ಸಂರಚನಾ commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ಸ್ಥಿರ ಶೂನ್ಯ get_commit_info(ಸಂರಚನಾ commit *commit, ಸಂರಚನಾ commit_info *ret)
{
	ಪೂರ್ಣಾಙ್ಕ len;
	ಧ್ರುವ ವರ್ಣ *subject, *encoding;
	ಧ್ರುವ ವರ್ಣ *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, ರಿಕ್ತ, encoding);
	get_ac_ಸಾಲು(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_ಸಾಲು(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	ಯದಿ (len)
		strbuf_add(&ret->summary, subject, len);
	ಅನ್ಯಥಾ
		strbuf_addf(&ret->summary, "(%s)", oid_to_hex(&commit->object.oid));

	repo_unuse_commit_buffer(the_repository, commit, message);
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
ಸ್ಥಿರ ಶೂನ್ಯ write_filename_info(ಸಂರಚನಾ blame_origin *suspect)
{
	ಯದಿ (suspect->previous) {
		ಸಂರಚನಾ blame_origin *prev = suspect->previous;
		printf("previous %s ", oid_to_hex(&prev->commit->object.oid));
		write_name_quoted(prev->path, stdout, '\n');
	}
	printf("filename ");
	write_name_quoted(suspect->path, stdout, '\n');
}

/*
 * Porcelain/Incremental format wants to show a lot of details per
 * commit.  Instead of repeating this every line, emit it only once,
 * the first time each commit appears in the output (unless the
 * user has specifically asked for us to repeat).
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ emit_one_suspect_detail(ಸಂರಚನಾ blame_origin *suspect, ಪೂರ್ಣಾಙ್ಕ repeat)
{
	ಸಂರಚನಾ commit_info ci = COMMIT_INFO_INIT;

	ಯದಿ (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		ಪ್ರತ್ಯಾಗಮ 0;

	suspect->commit->object.flags |= METAINFO_SHOWN;
	get_commit_info(suspect->commit, &ci);
	printf("author %s\n", ci.author.buf);
	printf("author-mail %s\n", ci.author_mail.buf);
	printf("author-time %"PRItime"\n", ci.author_time);
	printf("author-tz %s\n", ci.author_tz.buf);
	printf("committer %s\n", ci.committer.buf);
	printf("committer-mail %s\n", ci.committer_mail.buf);
	printf("committer-time %"PRItime"\n", ci.committer_time);
	printf("committer-tz %s\n", ci.committer_tz.buf);
	printf("summary %s\n", ci.summary.buf);
	ಯದಿ (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ಸ್ಥಿರ ಶೂನ್ಯ found_guilty_entry(ಸಂರಚನಾ blame_entry *ent, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ progress_info *pi = (ಸಂರಚನಾ progress_info *)data;

	ಯದಿ (incremental) {
		ಸಂರಚನಾ blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->ಅಂಕೆ_ಸಾಲುಗಳು);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_ಸಾಲುಗಳು += ent->ಅಂಕೆ_ಸಾಲುಗಳು;
	display_progress(pi->progress, pi->blamed_ಸಾಲುಗಳು);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *format_time(timestamp_t time, ಧ್ರುವ ವರ್ಣ *tz_str,
			       ಪೂರ್ಣಾಙ್ಕ show_raw_time)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	ಯದಿ (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	ಅನ್ಯಥಾ {
		ಧ್ರುವ ವರ್ಣ *time_str;
		size_t time_width;
		ಪೂರ್ಣಾಙ್ಕ tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		ಪ್ರತಿ (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	ಪ್ರತ್ಯಾಗಮ time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_ಸಂಖ್ಯೆ          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_ಸಾಲು_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_ಸಾಲು           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ಸ್ಥಿರ ಶೂನ್ಯ emit_porcelain_details(ಸಂರಚನಾ blame_origin *suspect, ಪೂರ್ಣಾಙ್ಕ repeat)
{
	ಯದಿ (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ಸ್ಥಿರ ಶೂನ್ಯ emit_porcelain_per_ಸಾಲು_details(ಸಂರಚನಾ blame_entry *ent)
{
	ಯದಿ (mark_unblamable_ಸಾಲುಗಳು && ent->unblamable)
		puts("unblamable");
	ಯದಿ (mark_ignored_ಸಾಲುಗಳು && ent->ignored)
		puts("ignored");
}

ಸ್ಥಿರ ಶೂನ್ಯ emit_porcelain(ಸಂರಚನಾ blame_scoreboard *sb, ಸಂರಚನಾ blame_entry *ent,
			   ಪೂರ್ಣಾಙ್ಕ opt)
{
	ಪೂರ್ಣಾಙ್ಕ repeat = opt & OUTPUT_ಸಾಲು_PORCELAIN;
	ಪೂರ್ಣಾಙ್ಕ cnt;
	ಧ್ರುವ ವರ್ಣ *cp;
	ಸಂರಚನಾ blame_origin *suspect = ent->suspect;
	ವರ್ಣ hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->ಅಂಕೆ_ಸಾಲುಗಳು);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_ಸಾಲು_details(ent);

	cp = blame_nth_ಸಾಲು(sb, ent->lno);
	ಪ್ರತಿ (cnt = 0; cnt < ent->ಅಂಕೆ_ಸಾಲುಗಳು; cnt++) {
		ವರ್ಣ ch;
		ಯದಿ (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			ಯದಿ (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_ಸಾಲು_details(ent);
		}
		putchar('\t');
		ಕುರು {
			ch = *cp++;
			putchar(ch);
		} ಯಾವತ್ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	ಯದಿ (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

ಸ್ಥಿರ ಸಂರಚನಾ color_field {
	timestamp_t hop;
	ವರ್ಣ col[COLOR_MAXLEN];
} *colorfield;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ colorfield_nr, colorfield_alloc;

ಸ್ಥಿರ ಶೂನ್ಯ parse_color_fields(ಧ್ರುವ ವರ್ಣ *s)
{
	ಸಂರಚನಾ string_list l = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list_item *item;
	ಗಣನಾ { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		ವಿಕಲ್ಪ (next) {
		ಪಕ್ಷ EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			ಭಙ್ಗ;
		ಪಕ್ಷ EXPECT_COLOR:
			ಯದಿ (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			ಭಙ್ಗ;
		}
	}

	ಯದಿ (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ setup_default_color_by_age(ಶೂನ್ಯ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ಸ್ಥಿರ ಶೂನ್ಯ determine_ಸಾಲು_heat(ಸಂರಚನಾ commit_info *ci, ಧ್ರುವ ವರ್ಣ **dest_color)
{
	ಪೂರ್ಣಾಙ್ಕ i = 0;

	ಯಾವತ್ (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ maybe_putc(ಪೂರ್ಣಾಙ್ಕ c, FILE *out)
{
	ಪ್ರತ್ಯಾಗಮ out ? putc(c, out) : 0;
}

ಸ್ಥಿರ size_t print_marks(FILE *out, ಧ್ರುವ ಸಂರಚನಾ blame_entry *ent, ಪೂರ್ಣಾಙ್ಕ opt)
{
	size_t len = 0;

	ಯದಿ ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	ಯದಿ (mark_unblamable_ಸಾಲುಗಳು && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	ಯದಿ (mark_ignored_ಸಾಲುಗಳು && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	ಪ್ರತ್ಯಾಗಮ len;
}

ಸ್ಥಿರ size_t ಎಣಿಕೆ_marks(ಧ್ರುವ ಸಂರಚನಾ blame_entry *ent, ಪೂರ್ಣಾಙ್ಕ opt)
{
	ಪ್ರತ್ಯಾಗಮ print_marks(ರಿಕ್ತ, ent, opt);
}

ಸ್ಥಿರ ಶೂನ್ಯ emit_other(ಸಂರಚನಾ blame_scoreboard *sb, ಸಂರಚನಾ blame_entry *ent,
		       ಪೂರ್ಣಾಙ್ಕ opt, ಸಂರಚನಾ blame_entry *prev_ent)
{
	ಪೂರ್ಣಾಙ್ಕ cnt;
	ಧ್ರುವ ವರ್ಣ *cp;
	ಸಂರಚನಾ blame_origin *suspect = ent->suspect;
	ಸಂರಚನಾ commit_info ci = COMMIT_INFO_INIT;
	ವರ್ಣ hex[GIT_MAX_HEXSZ + 1];
	ಪೂರ್ಣಾಙ್ಕ show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ಧ್ರುವ ವರ್ಣ *default_color = ರಿಕ್ತ, *color = ರಿಕ್ತ, *reset = ರಿಕ್ತ;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_ಸಾಲು(sb, ent->lno);

	ಯದಿ (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_ಸಾಲು_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	ಯದಿ (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	ಪ್ರತಿ (cnt = 0; cnt < ent->ಅಂಕೆ_ಸಾಲುಗಳು; cnt++) {
		ವರ್ಣ ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		ಯದಿ (opt & OUTPUT_COLOR_ಸಾಲು) {
			ಯದಿ (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} ಅನ್ಯಥಾ  {
				color = default_color ? default_color : ರಿಕ್ತ;
				reset = default_color ? GIT_COLOR_RESET : ರಿಕ್ತ;
			}
		}
		ಯದಿ (color)
			fputs(color, stdout);

		ಯದಿ ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (ಪೂರ್ಣಾಙ್ಕ)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		ಯದಿ (opt & OUTPUT_ANNOTATE_COMPAT) {
			ಧ್ರುವ ವರ್ಣ *name;
			ಯದಿ (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			ಅನ್ಯಥಾ
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} ಅನ್ಯಥಾ {
			ಯದಿ (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			ಯದಿ (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			ಯದಿ (opt & OUTPUT_SHOW_ಸಂಖ್ಯೆ)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			ಯದಿ (!(opt & OUTPUT_NO_AUTHOR)) {
				ಧ್ರುವ ವರ್ಣ *name;
				ಪೂರ್ಣಾಙ್ಕ pad;
				ಯದಿ (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				ಅನ್ಯಥಾ
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				printf(" (%s%*s %10s",
				       name, pad, "",
				       format_time(ci.author_time,
						   ci.author_tz.buf,
						   show_raw_time));
			}
			printf(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		ಯದಿ (reset)
			fputs(reset, stdout);
		ಕುರು {
			ch = *cp++;
			putchar(ch);
		} ಯಾವತ್ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	ಯದಿ (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

ಸ್ಥಿರ ಶೂನ್ಯ output(ಸಂರಚನಾ blame_scoreboard *sb, ಪೂರ್ಣಾಙ್ಕ option)
{
	ಸಂರಚನಾ blame_entry *ent, *prev_ent = ರಿಕ್ತ;

	ಯದಿ (option & OUTPUT_PORCELAIN) {
		ಪ್ರತಿ (ent = sb->ent; ent; ent = ent->next) {
			ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
			ಸಂರಚನಾ blame_origin *suspect;
			ಸಂರಚನಾ commit *commit = ent->suspect->commit;
			ಯದಿ (commit->object.flags & MORE_THAN_ONE_PATH)
				ಅನುವರ್ತ;
			ಪ್ರತಿ (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				ಯದಿ (suspect->guilty && ಎಣಿಕೆ++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					ಭಙ್ಗ;
				}
			}
		}
	}

	ಪ್ರತಿ (ent = sb->ent; ent; ent = ent->next) {
		ಯದಿ (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		ಅನ್ಯಥಾ {
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_ancestry(ಧ್ರುವ ವರ್ಣ *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯಾವತ್ (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ಸಂರಚನಾ commit_graft *graft = read_graft_ಸಾಲು(&buf);
		ಯದಿ (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update_auto_abbrev(ಪೂರ್ಣಾಙ್ಕ auto_abbrev, ಸಂರಚನಾ blame_origin *suspect)
{
	ಧ್ರುವ ವರ್ಣ *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	ಪೂರ್ಣಾಙ್ಕ len = strlen(uniq);
	ಯದಿ (auto_abbrev < len)
		ಪ್ರತ್ಯಾಗಮ len;
	ಪ್ರತ್ಯಾಗಮ auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ಸ್ಥಿರ ಶೂನ್ಯ find_alignment(ಸಂರಚನಾ blame_scoreboard *sb, ಪೂರ್ಣಾಙ್ಕ *option)
{
	ಪೂರ್ಣಾಙ್ಕ longest_src_ಸಾಲುಗಳು = 0;
	ಪೂರ್ಣಾಙ್ಕ longest_dst_ಸಾಲುಗಳು = 0;
	unsigned largest_score = 0;
	ಸಂರಚನಾ blame_entry *e;
	ಪೂರ್ಣಾಙ್ಕ compute_auto_abbrev = (abbrev < 0);
	ಪೂರ್ಣಾಙ್ಕ auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_ಎಣಿಕೆ = 0;

	ಪ್ರತಿ (e = sb->ent; e; e = e->next) {
		ಸಂರಚನಾ blame_origin *suspect = e->suspect;
		ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ;
		size_t marks_ಎಣಿಕೆ = ಎಣಿಕೆ_marks(e, *option);

		ಯದಿ (max_marks_ಎಣಿಕೆ < marks_ಎಣಿಕೆ)
			max_marks_ಎಣಿಕೆ = marks_ಎಣಿಕೆ;
		ಯದಿ (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		ಯದಿ (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		ಅಂಕೆ = strlen(suspect->path);
		ಯದಿ (longest_file < ಅಂಕೆ)
			longest_file = ಅಂಕೆ;
		ಯದಿ (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ಸಂರಚನಾ commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			ಯದಿ (*option & OUTPUT_SHOW_EMAIL)
				ಅಂಕೆ = utf8_strwidth(ci.author_mail.buf);
			ಅನ್ಯಥಾ
				ಅಂಕೆ = utf8_strwidth(ci.author.buf);
			ಯದಿ (longest_author < ಅಂಕೆ)
				longest_author = ಅಂಕೆ;
			commit_info_destroy(&ci);
		}
		ಅಂಕೆ = e->s_lno + e->ಅಂಕೆ_ಸಾಲುಗಳು;
		ಯದಿ (longest_src_ಸಾಲುಗಳು < ಅಂಕೆ)
			longest_src_ಸಾಲುಗಳು = ಅಂಕೆ;
		ಅಂಕೆ = e->lno + e->ಅಂಕೆ_ಸಾಲುಗಳು;
		ಯದಿ (longest_dst_ಸಾಲುಗಳು < ಅಂಕೆ)
			longest_dst_ಸಾಲುಗಳು = ಅಂಕೆ;
		ಯದಿ (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_ಸಾಲುಗಳು);
	max_digits = decimal_width(longest_dst_ಸಾಲುಗಳು);
	max_score_digits = decimal_width(largest_score);

	ಯದಿ (compute_auto_abbrev)
		abbrev = auto_abbrev;
	ಯದಿ (abbrev < (ಪೂರ್ಣಾಙ್ಕ)the_hash_algo->hexsz) {
		abbrev += max_marks_ಎಣಿಕೆ;
		ಯದಿ (abbrev > (ಪೂರ್ಣಾಙ್ಕ)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sanity_check_on_fail(ಸಂರಚನಾ blame_scoreboard *sb, ಪೂರ್ಣಾಙ್ಕ baa)
{
	ಪೂರ್ಣಾಙ್ಕ opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_ಸಂಖ್ಯೆ | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ಸ್ಥಿರ unsigned parse_score(ಧ್ರುವ ವರ್ಣ *arg)
{
	ವರ್ಣ *end;
	unsigned long score = strtoul(arg, &end, 10);
	ಯದಿ (*end)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ score;
}

ಸ್ಥಿರ ವರ್ಣ *add_prefix(ಧ್ರುವ ವರ್ಣ *prefix, ಧ್ರುವ ವರ್ಣ *path)
{
	ಪ್ರತ್ಯಾಗಮ prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_blame_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "blame.showroot")) {
		show_ಬೇರು = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "blame.showemail")) {
		ಪೂರ್ಣಾಙ್ಕ *output_option = cb;
		ಯದಿ (git_config_bool(var, ಮೌಲ್ಯ))
			*output_option |= OUTPUT_SHOW_EMAIL;
		ಅನ್ಯಥಾ
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "blame.date")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		parse_date_format(ಮೌಲ್ಯ, &blame_date_mode);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "blame.ignorerevsfile")) {
		ವರ್ಣ *str;
		ಪೂರ್ಣಾಙ್ಕ ret;

		ret = git_config_pathname(&str, var, ಮೌಲ್ಯ);
		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ ret;
		ಯದಿ (str)
			string_list_ಸೇರಿಸು(&ignore_revs_file_list, str);
		ಮುಕ್ತ(str);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_ಸಾಲುಗಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_ಸಾಲುಗಳು = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "color.blame.repeatedlines")) {
		ಯದಿ (color_parse_mem(ಮೌಲ್ಯ, strlen(ಮೌಲ್ಯ), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "blame.coloring")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಯದಿ (!strcmp(ಮೌಲ್ಯ, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_ಸಾಲು;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಮೌಲ್ಯ, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಮೌಲ್ಯ, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_ಸಾಲು |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} ಅನ್ಯಥಾ {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", ಮೌಲ್ಯ);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಯದಿ (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		diff_algorithm = parse_algorithm_ಮೌಲ್ಯ(ಮೌಲ್ಯ);
		ಯದಿ (diff_algorithm < 0)
			ಪ್ರತ್ಯಾಗಮ error(_("unknown value for config '%s': %s"),
				     var, ಮೌಲ್ಯ);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (git_diff_heuristic_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (userdiff_config(var, ಮೌಲ್ಯ) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ blame_copy_callback(ಧ್ರುವ ಸಂರಚನಾ option *option, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	ಯದಿ (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	ಯದಿ (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	ಯದಿ (arg)
		blame_copy_score = parse_score(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ blame_move_callback(ಧ್ರುವ ಸಂರಚನಾ option *option, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	ಯದಿ (arg)
		blame_move_score = parse_score(arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ blame_diff_algorithm_minimal(ಧ್ರುವ ಸಂರಚನಾ option *option,
					ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt = option->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	ಯದಿ (!unset)
		*opt |= XDF_NEED_MINIMAL;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ blame_diff_algorithm_callback(ಧ್ರುವ ಸಂರಚನಾ option *option,
					 ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt = option->ಮೌಲ್ಯ;
	long ಮೌಲ್ಯ = parse_algorithm_ಮೌಲ್ಯ(arg);

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (ಮೌಲ್ಯ < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= ಮೌಲ್ಯ;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_a_rev(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ object_id oid;

	ಯದಿ (repo_get_oid(the_repository, name, &oid))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ peel_to_commit_oid(ಸಂರಚನಾ object_id *oid_ret, ಶೂನ್ಯ *cbdata)
{
	ಸಂರಚನಾ repository *r = ((ಸಂರಚನಾ blame_scoreboard *)cbdata)->repo;
	ಸಂರಚನಾ object_id oid;

	oidcpy(&oid, oid_ret);
	ಯಾವತ್ (1) {
		ಸಂರಚನಾ object *obj;
		ಪೂರ್ಣಾಙ್ಕ kind = odb_read_object_info(r->objects, &oid, ರಿಕ್ತ);
		ಯದಿ (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		ಯದಿ (kind != OBJ_TAG)
			ಪ್ರತ್ಯಾಗಮ -1;
		obj = deref_tag(r, parse_object(r, &oid), ರಿಕ್ತ, 0);
		ಯದಿ (!obj)
			ಪ್ರತ್ಯಾಗಮ -1;
		oidcpy(&oid, &obj->oid);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ build_ignorelist(ಸಂರಚನಾ blame_scoreboard *sb,
			     ಸಂರಚನಾ string_list *ignore_revs_file_list,
			     ಸಂರಚನಾ string_list *ignore_rev_list)
{
	ಸಂರಚನಾ string_list_item *i;
	ಸಂರಚನಾ object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		ಯದಿ (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		ಅನ್ಯಥಾ
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		ಯದಿ (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ಸೇರಿಸು(&sb->ignore_list, &oid);
	}
}

ಪೂರ್ಣಾಙ್ಕ cmd_blame(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ rev_info revs;
	ವರ್ಣ *path = ರಿಕ್ತ;
	ಸಂರಚನಾ blame_scoreboard sb;
	ಸಂರಚನಾ blame_origin *o;
	ಸಂರಚನಾ blame_entry *ent = ರಿಕ್ತ;
	long dashdash_pos, lno;
	ಸಂರಚನಾ progress_info pi = { ರಿಕ್ತ, 0 };

	ಸಂರಚನಾ string_list range_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	ಪೂರ್ಣಾಙ್ಕ output_option = 0, opt = 0;
	ಪೂರ್ಣಾಙ್ಕ show_stats = 0;
	ಧ್ರುವ ವರ್ಣ *revs_file = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *contents_from = ರಿಕ್ತ;
	ಧ್ರುವ ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ರಿಕ್ತ, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ಬೇರು, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_ಸಂಖ್ಯೆ),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_ಸಾಲು_PORCELAIN),
		OPT_BIT('c', ರಿಕ್ತ, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', ರಿಕ್ತ, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', ರಿಕ್ತ, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', ರಿಕ್ತ, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', ರಿಕ್ತ, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_ಸಾಲು),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ರಿಕ್ತ,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', ರಿಕ್ತ, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ರಿಕ್ತ, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ರಿಕ್ತ, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ರಿಕ್ತ, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	ಸಂರಚನಾ parse_opt_ctx_t ctx;
	ಪೂರ್ಣಾಙ್ಕ cmd_is_annotate = !strcmp(argv[0], "annotate");
	ಸಂರಚನಾ range_set ranges;
	unsigned ಪೂರ್ಣಾಙ್ಕ range_i;
	long anchor;
	long ಅಂಕೆ_ಸಾಲುಗಳು = 0;
	ಧ್ರುವ ವರ್ಣ *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	ಪ್ರತಿ (;;) {
		ವಿಕಲ್ಪ (parse_options_step(&ctx, options, opt_usage)) {
		ಪಕ್ಷ PARSE_OPT_NON_OPTION:
		ಪಕ್ಷ PARSE_OPT_UNKNOWN:
			ಭಙ್ಗ;
		ಪಕ್ಷ PARSE_OPT_HELP:
			exit(0);
		ಪಕ್ಷ PARSE_OPT_HELP_ERROR:
		ಪಕ್ಷ PARSE_OPT_ERROR:
		ಪಕ್ಷ PARSE_OPT_SUBCOMMAND:
			exit(129);
		ಪಕ್ಷ PARSE_OPT_COMPLETE:
			exit(0);
		ಪಕ್ಷ PARSE_OPT_DONE:
			ಯದಿ (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			ಗಚ್ಛ parse_done;
		}

		ಯದಿ (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			reverse = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_done:
	revision_opts_finish(&revs);
	no_whole_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_end(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ಯದಿ (incremental || (output_option & OUTPUT_PORCELAIN)) {
		ಯದಿ (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} ಅನ್ಯಥಾ ಯದಿ (show_progress < 0)
		show_progress = isatty(2);

	ಯದಿ (!abbrev)
		abbrev = the_hash_algo->hexsz;

	ಯದಿ (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	ಯದಿ (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} ಅನ್ಯಥಾ {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	ವಿಕಲ್ಪ (blame_date_mode.type) {
	ಪಕ್ಷ DATE_RFC2822:
		blame_date_width = ಪರಿಮಾಣ("Thu, 19 Oct 2006 16:00:04 -0700");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_ISO8601_STRICT:
		blame_date_width = ಪರಿಮಾಣ("2006-10-19T16:00:04-07:00");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_ISO8601:
		blame_date_width = ಪರಿಮಾಣ("2006-10-19 16:00:04 -0700");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_RAW:
		blame_date_width = ಪರಿಮಾಣ("1161298804 -0700");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_UNIX:
		blame_date_width = ಪರಿಮಾಣ("1161298804");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_SHORT:
		blame_date_width = ಪರಿಮಾಣ("2006-10-19");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_RELATIVE:
		/*
		 * TRANSLATORS: This string is used to tell us the
		 * maximum display width for a relative timestamp in
		 * "git blame" output.  For C locale, "4 years, 11
		 * months ago", which takes 22 places, is the longest
		 * among various forms of relative timestamps, but
		 * your language may need more or fewer display
		 * columns.
		 */
		blame_date_width = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = ಪರಿಮಾಣ("Thu Oct 19 16:00");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_NORMAL:
		blame_date_width = ಪರಿಮಾಣ("Thu Oct 19 16:00:04 2006 -0700");
		ಭಙ್ಗ;
	ಪಕ್ಷ DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		ಭಙ್ಗ;
	}
	blame_date_width -= 1; /* strip the null */

	ಯದಿ (revs.diffopt.flags.find_copies_harder)
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
	ಯದಿ (dashdash_pos) {
		ವಿಕಲ್ಪ (argc - dashdash_pos - 1) {
		ಪಕ್ಷ 2: /* (1b) */
			ಯದಿ (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		ಪಕ್ಷ 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = ರಿಕ್ತ;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			usage_with_options(opt_usage, options);
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (argc < 2)
			usage_with_options(opt_usage, options);
		ಯದಿ (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} ಅನ್ಯಥಾ {	/* (2a) */
			ಯದಿ (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_ವೃಕ್ಷ(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, ರಿಕ್ತ);
	ಯದಿ (!revs.pending.nr && is_bare_repository(the_repository)) {
		ಸಂರಚನಾ commit *head_commit;
		ಸಂರಚನಾ object_id head_oid;

		ಯದಿ (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, ರಿಕ್ತ) ||
		    !(head_commit = lookup_commit_reference_gently(revs.repo,
							     &head_oid, 1)))
			die("no such ref: HEAD");

		add_pending_object(&revs, &head_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.reverse = reverse;
	sb.repo = the_repository;
	sb.path = path;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	setup_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	ಯದಿ (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.ಅಂಕೆ_ಸಾಲುಗಳು;

	ಯದಿ (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	ಪ್ರತಿ (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, ತುದಿ;
		ಯದಿ (parse_range_arg(range_list.ವಸ್ತುಗಳು[range_i].string,
				    nth_ಸಾಲು_cb, &sb, lno, anchor,
				    &bottom, &ತುದಿ, sb.path,
				    the_repository->index))
			usage(str_usage);
		ಯದಿ ((!lno && (ತುದಿ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		ಯದಿ (bottom < 1)
			bottom = 1;
		ಯದಿ (ತುದಿ < 1 || lno < ತುದಿ)
			ತುದಿ = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, ತುದಿ);
		anchor = ತುದಿ + 1;
	}
	sort_and_merge_range_set(&ranges);

	ಪ್ರತಿ (range_i = ranges.nr; range_i > 0; --range_i) {
		ಧ್ರುವ ಸಂರಚನಾ range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		ಅಂಕೆ_ಸಾಲುಗಳು += (r->end - r->start);
	}
	ಯದಿ (!ಅಂಕೆ_ಸಾಲುಗಳು)
		ಅಂಕೆ_ಸಾಲುಗಳು = sb.ಅಂಕೆ_ಸಾಲುಗಳು;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ರಿಕ್ತ;

	ಯದಿ (blame_move_score)
		sb.move_score = blame_move_score;
	ಯದಿ (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ಬೇರು = show_ಬೇರು;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	ಯದಿ (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     ಅಂಕೆ_ಸಾಲುಗಳು);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	ಯದಿ (!incremental)
		setup_pager(the_repository);
	ಅನ್ಯಥಾ
		ಗಚ್ಛ cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	ಯದಿ (!(output_option & (OUTPUT_COLOR_ಸಾಲು | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	ಯದಿ (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		ಯದಿ (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_ಸಾಲು))
			xsnprintf(repeated_meta_color,
				  ಪರಿಮಾಣ(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	ಯದಿ (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_ಸಾಲು | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	ಯದಿ (show_stats) {
		printf("num read blob: %d\n", sb.ಅಂಕೆ_read_blob);
		printf("num get patch: %d\n", sb.ಅಂಕೆ_get_patch);
		printf("num commits: %d\n", sb.ಅಂಕೆ_commits);
	}

cleanup:
	ಪ್ರತಿ (ent = sb.ent; ent; ) {
		ಸಂರಚನಾ blame_entry *e = ent->next;
		ಮುಕ್ತ(ent);
		ent = e;
	}

	ಮುಕ್ತ(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	ಪ್ರತ್ಯಾಗಮ 0;
}
