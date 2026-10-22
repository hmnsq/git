/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "color.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "commit.h"
#অন্তর্ভাৱ "diff.h"
#অন্তর্ভাৱ "revision.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "mailmap.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "prio-queue.h"
#অন্তর্ভাৱ "utf8.h"
#অন্তর্ভাৱ "userdiff.h"
#অন্তর্ভাৱ "line-range.h"
#অন্তর্ভাৱ "line-log.h"
#অন্তর্ভাৱ "progress.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "pager.h"
#অন্তর্ভাৱ "blame.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "tag.h"
#অন্তর্ভাৱ "write-or-die.h"

স্থির ধ্রুৱ ৱর্ণ blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
স্থির ধ্রুৱ ৱর্ণ annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	রিক্ত
};

স্থির ধ্রুৱ ৱর্ণ *ধ্রুৱ annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক longest_file;
স্থির পূর্ণাঙ্ক longest_author;
স্থির পূর্ণাঙ্ক max_orig_digits;
স্থির পূর্ণাঙ্ক max_digits;
স্থির পূর্ণাঙ্ক max_score_digits;
স্থির পূর্ণাঙ্ক show_মূল;
স্থির পূর্ণাঙ্ক reverse;
স্থির পূর্ণাঙ্ক blank_boundary;
স্থির পূর্ণাঙ্ক incremental;
স্থির পূর্ণাঙ্ক xdl_opts;
স্থির পূর্ণাঙ্ক abbrev = -1;
স্থির পূর্ণাঙ্ক no_whole_file_rename;
স্থির পূর্ণাঙ্ক show_progress;
স্থির ৱর্ণ repeated_meta_color[COLOR_MAXLEN];
স্থির পূর্ণাঙ্ক coloring_mode;
স্থির সংরচনা string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
স্থির পূর্ণাঙ্ক mark_unblamable_সাৰিগুলি;
স্থির পূর্ণাঙ্ক mark_ignored_সাৰিগুলি;

স্থির সংরচনা date_mode blame_date_mode = { DATE_ISO8601 };
স্থির size_t blame_date_width;

স্থির সংরচনা string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

স্থির unsigned blame_move_score;
স্থির unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

সংরচনা progress_info {
	সংরচনা progress *progress;
	পূর্ণাঙ্ক blamed_সাৰিগুলি;
};

স্থির ধ্রুৱ ৱর্ণ *nth_সাৰি_cb(শূন্য *data, long lno)
{
	ঘূৰাই blame_nth_সাৰি((সংরচনা blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
সংরচনা commit_info {
	সংরচনা strbuf author;
	সংরচনা strbuf author_mail;
	timestamp_t author_time;
	সংরচনা strbuf author_tz;

	/* filled only when asked for details */
	সংরচনা strbuf committer;
	সংরচনা strbuf committer_mail;
	timestamp_t committer_time;
	সংরচনা strbuf committer_tz;

	সংরচনা strbuf summary;
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
স্থির শূন্য get_ac_সাৰি(ধ্রুৱ ৱর্ণ *inbuf, ধ্রুৱ ৱর্ণ *what,
	সংরচনা strbuf *name, সংরচনা strbuf *mail,
	timestamp_t *time, সংরচনা strbuf *tz)
{
	সংরচনা ident_split ident;
	size_t len, maillen, namelen;
	ধ্রুৱ ৱর্ণ *tmp, *endp;
	ধ্রুৱ ৱর্ণ *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	যদি (!tmp)
		গচ্ছ error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	যদি (!endp)
		len = strlen(tmp);
	অন্যথা
		len = endp - tmp;

	যদি (split_ident_সাৰি(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		ঘূৰাই;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	যদি (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, রিক্ত, 10);
	অন্যথা
		*time = 0;

	যদি (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	অন্যথা
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (পূর্ণাঙ্ক)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

স্থির শূন্য commit_info_destroy(সংরচনা commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

স্থির শূন্য get_commit_info(সংরচনা commit *commit, সংরচনা commit_info *ret)
{
	পূর্ণাঙ্ক len;
	ধ্রুৱ ৱর্ণ *subject, *encoding;
	ধ্রুৱ ৱর্ণ *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, রিক্ত, encoding);
	get_ac_সাৰি(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_সাৰি(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	যদি (len)
		strbuf_add(&ret->summary, subject, len);
	অন্যথা
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
স্থির শূন্য write_filename_info(সংরচনা blame_origin *suspect)
{
	যদি (suspect->previous) {
		সংরচনা blame_origin *prev = suspect->previous;
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
স্থির পূর্ণাঙ্ক emit_one_suspect_detail(সংরচনা blame_origin *suspect, পূর্ণাঙ্ক repeat)
{
	সংরচনা commit_info ci = COMMIT_INFO_INIT;

	যদি (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		ঘূৰাই 0;

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
	যদি (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	ঘূৰাই 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
স্থির শূন্য found_guilty_entry(সংরচনা blame_entry *ent, শূন্য *data)
{
	সংরচনা progress_info *pi = (সংরচনা progress_info *)data;

	যদি (incremental) {
		সংরচনা blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->অঙ্ক_সাৰিগুলি);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_সাৰিগুলি += ent->অঙ্ক_সাৰিগুলি;
	display_progress(pi->progress, pi->blamed_সাৰিগুলি);
}

স্থির ধ্রুৱ ৱর্ণ *format_time(timestamp_t time, ধ্রুৱ ৱর্ণ *tz_str,
			       পূর্ণাঙ্ক show_raw_time)
{
	স্থির সংরচনা strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	যদি (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	অন্যথা {
		ধ্রুৱ ৱর্ণ *time_str;
		size_t time_width;
		পূর্ণাঙ্ক tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		বাবে (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	ঘূৰাই time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_সংখ্যা          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_সাৰি_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_সাৰি           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

স্থির শূন্য emit_porcelain_details(সংরচনা blame_origin *suspect, পূর্ণাঙ্ক repeat)
{
	যদি (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
স্থির শূন্য emit_porcelain_per_সাৰি_details(সংরচনা blame_entry *ent)
{
	যদি (mark_unblamable_সাৰিগুলি && ent->unblamable)
		puts("unblamable");
	যদি (mark_ignored_সাৰিগুলি && ent->ignored)
		puts("ignored");
}

স্থির শূন্য emit_porcelain(সংরচনা blame_scoreboard *sb, সংরচনা blame_entry *ent,
			   পূর্ণাঙ্ক opt)
{
	পূর্ণাঙ্ক repeat = opt & OUTPUT_সাৰি_PORCELAIN;
	পূর্ণাঙ্ক cnt;
	ধ্রুৱ ৱর্ণ *cp;
	সংরচনা blame_origin *suspect = ent->suspect;
	ৱর্ণ hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->অঙ্ক_সাৰিগুলি);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_সাৰি_details(ent);

	cp = blame_nth_সাৰি(sb, ent->lno);
	বাবে (cnt = 0; cnt < ent->অঙ্ক_সাৰিগুলি; cnt++) {
		ৱর্ণ ch;
		যদি (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			যদি (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_সাৰি_details(ent);
		}
		putchar('\t');
		কৰা {
			ch = *cp++;
			putchar(ch);
		} যেতিয়ালৈ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	যদি (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

স্থির সংরচনা color_field {
	timestamp_t hop;
	ৱর্ণ col[COLOR_MAXLEN];
} *colorfield;
স্থির পূর্ণাঙ্ক colorfield_nr, colorfield_alloc;

স্থির শূন্য parse_color_fields(ধ্রুৱ ৱর্ণ *s)
{
	সংরচনা string_list l = STRING_LIST_INIT_DUP;
	সংরচনা string_list_item *item;
	গণনা { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		ৱিকল্প (next) {
		পক্ষ EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			ভঙ্গ;
		পক্ষ EXPECT_COLOR:
			যদি (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			ভঙ্গ;
		}
	}

	যদি (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

স্থির শূন্য setup_default_color_by_age(শূন্য)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

স্থির শূন্য determine_সাৰি_heat(সংরচনা commit_info *ci, ধ্রুৱ ৱর্ণ **dest_color)
{
	পূর্ণাঙ্ক i = 0;

	যেতিয়ালৈ (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক maybe_putc(পূর্ণাঙ্ক c, FILE *out)
{
	ঘূৰাই out ? putc(c, out) : 0;
}

স্থির size_t print_marks(FILE *out, ধ্রুৱ সংরচনা blame_entry *ent, পূর্ণাঙ্ক opt)
{
	size_t len = 0;

	যদি ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	যদি (mark_unblamable_সাৰিগুলি && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	যদি (mark_ignored_সাৰিগুলি && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	ঘূৰাই len;
}

স্থির size_t গুনতি_marks(ধ্রুৱ সংরচনা blame_entry *ent, পূর্ণাঙ্ক opt)
{
	ঘূৰাই print_marks(রিক্ত, ent, opt);
}

স্থির শূন্য emit_other(সংরচনা blame_scoreboard *sb, সংরচনা blame_entry *ent,
		       পূর্ণাঙ্ক opt, সংরচনা blame_entry *prev_ent)
{
	পূর্ণাঙ্ক cnt;
	ধ্রুৱ ৱর্ণ *cp;
	সংরচনা blame_origin *suspect = ent->suspect;
	সংরচনা commit_info ci = COMMIT_INFO_INIT;
	ৱর্ণ hex[GIT_MAX_HEXSZ + 1];
	পূর্ণাঙ্ক show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ধ্রুৱ ৱর্ণ *default_color = রিক্ত, *color = রিক্ত, *reset = রিক্ত;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_সাৰি(sb, ent->lno);

	যদি (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_সাৰি_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	যদি (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	বাবে (cnt = 0; cnt < ent->অঙ্ক_সাৰিগুলি; cnt++) {
		ৱর্ণ ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		যদি (opt & OUTPUT_COLOR_সাৰি) {
			যদি (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} অন্যথা  {
				color = default_color ? default_color : রিক্ত;
				reset = default_color ? GIT_COLOR_RESET : রিক্ত;
			}
		}
		যদি (color)
			fputs(color, stdout);

		যদি ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (পূর্ণাঙ্ক)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		যদি (opt & OUTPUT_ANNOTATE_COMPAT) {
			ধ্রুৱ ৱর্ণ *name;
			যদি (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			অন্যথা
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} অন্যথা {
			যদি (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			যদি (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			যদি (opt & OUTPUT_SHOW_সংখ্যা)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			যদি (!(opt & OUTPUT_NO_AUTHOR)) {
				ধ্রুৱ ৱর্ণ *name;
				পূর্ণাঙ্ক pad;
				যদি (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				অন্যথা
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
		যদি (reset)
			fputs(reset, stdout);
		কৰা {
			ch = *cp++;
			putchar(ch);
		} যেতিয়ালৈ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	যদি (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

স্থির শূন্য output(সংরচনা blame_scoreboard *sb, পূর্ণাঙ্ক option)
{
	সংরচনা blame_entry *ent, *prev_ent = রিক্ত;

	যদি (option & OUTPUT_PORCELAIN) {
		বাবে (ent = sb->ent; ent; ent = ent->next) {
			পূর্ণাঙ্ক গুনতি = 0;
			সংরচনা blame_origin *suspect;
			সংরচনা commit *commit = ent->suspect->commit;
			যদি (commit->object.flags & MORE_THAN_ONE_PATH)
				অনুৱর্ত;
			বাবে (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				যদি (suspect->guilty && গুনতি++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					ভঙ্গ;
				}
			}
		}
	}

	বাবে (ent = sb->ent; ent; ent = ent->next) {
		যদি (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		অন্যথা {
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
স্থির পূর্ণাঙ্ক read_ancestry(ধ্রুৱ ৱর্ণ *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	সংরচনা strbuf buf = STRBUF_INIT;
	যদি (!fp)
		ঘূৰাই -1;
	যেতিয়ালৈ (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		সংরচনা commit_graft *graft = read_graft_সাৰি(&buf);
		যদি (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক update_auto_abbrev(পূর্ণাঙ্ক auto_abbrev, সংরচনা blame_origin *suspect)
{
	ধ্রুৱ ৱর্ণ *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	পূর্ণাঙ্ক len = strlen(uniq);
	যদি (auto_abbrev < len)
		ঘূৰাই len;
	ঘূৰাই auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
স্থির শূন্য find_alignment(সংরচনা blame_scoreboard *sb, পূর্ণাঙ্ক *option)
{
	পূর্ণাঙ্ক longest_src_সাৰিগুলি = 0;
	পূর্ণাঙ্ক longest_dst_সাৰিগুলি = 0;
	unsigned largest_score = 0;
	সংরচনা blame_entry *e;
	পূর্ণাঙ্ক compute_auto_abbrev = (abbrev < 0);
	পূর্ণাঙ্ক auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_গুনতি = 0;

	বাবে (e = sb->ent; e; e = e->next) {
		সংরচনা blame_origin *suspect = e->suspect;
		পূর্ণাঙ্ক অঙ্ক;
		size_t marks_গুনতি = গুনতি_marks(e, *option);

		যদি (max_marks_গুনতি < marks_গুনতি)
			max_marks_গুনতি = marks_গুনতি;
		যদি (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		যদি (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		অঙ্ক = strlen(suspect->path);
		যদি (longest_file < অঙ্ক)
			longest_file = অঙ্ক;
		যদি (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			সংরচনা commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			যদি (*option & OUTPUT_SHOW_EMAIL)
				অঙ্ক = utf8_strwidth(ci.author_mail.buf);
			অন্যথা
				অঙ্ক = utf8_strwidth(ci.author.buf);
			যদি (longest_author < অঙ্ক)
				longest_author = অঙ্ক;
			commit_info_destroy(&ci);
		}
		অঙ্ক = e->s_lno + e->অঙ্ক_সাৰিগুলি;
		যদি (longest_src_সাৰিগুলি < অঙ্ক)
			longest_src_সাৰিগুলি = অঙ্ক;
		অঙ্ক = e->lno + e->অঙ্ক_সাৰিগুলি;
		যদি (longest_dst_সাৰিগুলি < অঙ্ক)
			longest_dst_সাৰিগুলি = অঙ্ক;
		যদি (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_সাৰিগুলি);
	max_digits = decimal_width(longest_dst_সাৰিগুলি);
	max_score_digits = decimal_width(largest_score);

	যদি (compute_auto_abbrev)
		abbrev = auto_abbrev;
	যদি (abbrev < (পূর্ণাঙ্ক)the_hash_algo->hexsz) {
		abbrev += max_marks_গুনতি;
		যদি (abbrev > (পূর্ণাঙ্ক)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

স্থির শূন্য sanity_check_on_fail(সংরচনা blame_scoreboard *sb, পূর্ণাঙ্ক baa)
{
	পূর্ণাঙ্ক opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_সংখ্যা | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

স্থির unsigned parse_score(ধ্রুৱ ৱর্ণ *arg)
{
	ৱর্ণ *end;
	unsigned long score = strtoul(arg, &end, 10);
	যদি (*end)
		ঘূৰাই 0;
	ঘূৰাই score;
}

স্থির ৱর্ণ *add_prefix(ধ্রুৱ ৱর্ণ *prefix, ধ্রুৱ ৱর্ণ *path)
{
	ঘূৰাই prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

স্থির পূর্ণাঙ্ক git_blame_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			    ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "blame.showroot")) {
		show_মূল = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "blame.showemail")) {
		পূর্ণাঙ্ক *output_option = cb;
		যদি (git_config_bool(var, মান))
			*output_option |= OUTPUT_SHOW_EMAIL;
		অন্যথা
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "blame.date")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		parse_date_format(মান, &blame_date_mode);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "blame.ignorerevsfile")) {
		ৱর্ণ *str;
		পূর্ণাঙ্ক ret;

		ret = git_config_pathname(&str, var, মান);
		যদি (ret)
			ঘূৰাই ret;
		যদি (str)
			string_list_ঢোকাও(&ignore_revs_file_list, str);
		মুক্ত(str);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_সাৰিগুলি = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_সাৰিগুলি = git_config_bool(var, মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "color.blame.repeatedlines")) {
		যদি (color_parse_mem(মান, strlen(মান), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", মান);
		ঘূৰাই 0;
	}
	যদি (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(মান);
		ঘূৰাই 0;
	}

	যদি (!strcmp(var, "blame.coloring")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		যদি (!strcmp(মান, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_সাৰি;
		} অন্যথা যদি (!strcmp(মান, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} অন্যথা যদি (!strcmp(মান, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_সাৰি |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} অন্যথা {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", মান);
			ঘূৰাই 0;
		}
	}

	যদি (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		diff_algorithm = parse_algorithm_মান(মান);
		যদি (diff_algorithm < 0)
			ঘূৰাই error(_("unknown value for config '%s': %s"),
				     var, মান);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		ঘূৰাই 0;
	}

	যদি (git_diff_heuristic_config(var, মান, cb) < 0)
		ঘূৰাই -1;
	যদি (userdiff_config(var, মান) < 0)
		ঘূৰাই -1;

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির পূর্ণাঙ্ক blame_copy_callback(ধ্রুৱ সংরচনা option *option, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *opt = option->মান;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	যদি (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	যদি (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	যদি (arg)
		blame_copy_score = parse_score(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক blame_move_callback(ধ্রুৱ সংরচনা option *option, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *opt = option->মান;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	যদি (arg)
		blame_move_score = parse_score(arg);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক blame_diff_algorithm_minimal(ধ্রুৱ সংরচনা option *option,
					ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *opt = option->মান;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	যদি (!unset)
		*opt |= XDF_NEED_MINIMAL;

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক blame_diff_algorithm_callback(ধ্রুৱ সংরচনা option *option,
					 ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক *opt = option->মান;
	long মান = parse_algorithm_মান(arg);

	BUG_ON_OPT_NEG(unset);

	যদি (মান < 0)
		ঘূৰাই error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= মান;

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক is_a_rev(ধ্রুৱ ৱর্ণ *name)
{
	সংরচনা object_id oid;

	যদি (repo_get_oid(the_repository, name, &oid))
		ঘূৰাই 0;
	ঘূৰাই OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, রিক্ত);
}

স্থির পূর্ণাঙ্ক peel_to_commit_oid(সংরচনা object_id *oid_ret, শূন্য *cbdata)
{
	সংরচনা repository *r = ((সংরচনা blame_scoreboard *)cbdata)->repo;
	সংরচনা object_id oid;

	oidcpy(&oid, oid_ret);
	যেতিয়ালৈ (1) {
		সংরচনা object *obj;
		পূর্ণাঙ্ক kind = odb_read_object_info(r->objects, &oid, রিক্ত);
		যদি (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			ঘূৰাই 0;
		}
		যদি (kind != OBJ_TAG)
			ঘূৰাই -1;
		obj = deref_tag(r, parse_object(r, &oid), রিক্ত, 0);
		যদি (!obj)
			ঘূৰাই -1;
		oidcpy(&oid, &obj->oid);
	}
}

স্থির শূন্য build_ignorelist(সংরচনা blame_scoreboard *sb,
			     সংরচনা string_list *ignore_revs_file_list,
			     সংরচনা string_list *ignore_rev_list)
{
	সংরচনা string_list_item *i;
	সংরচনা object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		যদি (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		অন্যথা
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		যদি (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ঢোকাও(&sb->ignore_list, &oid);
	}
}

পূর্ণাঙ্ক cmd_blame(পূর্ণাঙ্ক argc,
	      ধ্রুৱ ৱর্ণ **argv,
	      ধ্রুৱ ৱর্ণ *prefix,
	      সংরচনা repository *repo UNUSED)
{
	সংরচনা rev_info revs;
	ৱর্ণ *path = রিক্ত;
	সংরচনা blame_scoreboard sb;
	সংরচনা blame_origin *o;
	সংরচনা blame_entry *ent = রিক্ত;
	long dashdash_pos, lno;
	সংরচনা progress_info pi = { রিক্ত, 0 };

	সংরচনা string_list range_list = STRING_LIST_INIT_NODUP;
	সংরচনা string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	পূর্ণাঙ্ক output_option = 0, opt = 0;
	পূর্ণাঙ্ক show_stats = 0;
	ধ্রুৱ ৱর্ণ *revs_file = রিক্ত;
	ধ্রুৱ ৱর্ণ *contents_from = রিক্ত;
	ধ্রুৱ সংরচনা option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', রিক্ত, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_মূল, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_সংখ্যা),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_সাৰি_PORCELAIN),
		OPT_BIT('c', রিক্ত, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', রিক্ত, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', রিক্ত, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', রিক্ত, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', রিক্ত, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_সাৰি),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, রিক্ত,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', রিক্ত, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', রিক্ত, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', রিক্ত, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', রিক্ত, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	সংরচনা parse_opt_ctx_t ctx;
	পূর্ণাঙ্ক cmd_is_annotate = !strcmp(argv[0], "annotate");
	সংরচনা range_set ranges;
	unsigned পূর্ণাঙ্ক range_i;
	long anchor;
	long অঙ্ক_সাৰিগুলি = 0;
	ধ্রুৱ ৱর্ণ *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ধ্রুৱ ৱর্ণ *ধ্রুৱ *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, রিক্ত);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	বাবে (;;) {
		ৱিকল্প (parse_options_step(&ctx, options, opt_usage)) {
		পক্ষ PARSE_OPT_NON_OPTION:
		পক্ষ PARSE_OPT_UNKNOWN:
			ভঙ্গ;
		পক্ষ PARSE_OPT_HELP:
			exit(0);
		পক্ষ PARSE_OPT_HELP_ERROR:
		পক্ষ PARSE_OPT_ERROR:
		পক্ষ PARSE_OPT_SUBCOMMAND:
			exit(129);
		পক্ষ PARSE_OPT_COMPLETE:
			exit(0);
		পক্ষ PARSE_OPT_DONE:
			যদি (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			গচ্ছ parse_done;
		}

		যদি (!strcmp(ctx.argv[0], "--reverse")) {
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

	যদি (incremental || (output_option & OUTPUT_PORCELAIN)) {
		যদি (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} অন্যথা যদি (show_progress < 0)
		show_progress = isatty(2);

	যদি (!abbrev)
		abbrev = the_hash_algo->hexsz;

	যদি (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	যদি (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} অন্যথা {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	ৱিকল্প (blame_date_mode.type) {
	পক্ষ DATE_RFC2822:
		blame_date_width = পরিমাণ("Thu, 19 Oct 2006 16:00:04 -0700");
		ভঙ্গ;
	পক্ষ DATE_ISO8601_STRICT:
		blame_date_width = পরিমাণ("2006-10-19T16:00:04-07:00");
		ভঙ্গ;
	পক্ষ DATE_ISO8601:
		blame_date_width = পরিমাণ("2006-10-19 16:00:04 -0700");
		ভঙ্গ;
	পক্ষ DATE_RAW:
		blame_date_width = পরিমাণ("1161298804 -0700");
		ভঙ্গ;
	পক্ষ DATE_UNIX:
		blame_date_width = পরিমাণ("1161298804");
		ভঙ্গ;
	পক্ষ DATE_SHORT:
		blame_date_width = পরিমাণ("2006-10-19");
		ভঙ্গ;
	পক্ষ DATE_RELATIVE:
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
		ভঙ্গ;
	পক্ষ DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = পরিমাণ("Thu Oct 19 16:00");
		ভঙ্গ;
	পক্ষ DATE_NORMAL:
		blame_date_width = পরিমাণ("Thu Oct 19 16:00:04 2006 -0700");
		ভঙ্গ;
	পক্ষ DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		ভঙ্গ;
	}
	blame_date_width -= 1; /* strip the null */

	যদি (revs.diffopt.flags.find_copies_harder)
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
	যদি (dashdash_pos) {
		ৱিকল্প (argc - dashdash_pos - 1) {
		পক্ষ 2: /* (1b) */
			যদি (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		পক্ষ 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = রিক্ত;
			ভঙ্গ;
		স্ৱভাৱ:
			usage_with_options(opt_usage, options);
		}
	} অন্যথা {
		যদি (argc < 2)
			usage_with_options(opt_usage, options);
		যদি (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} অন্যথা {	/* (2a) */
			যদি (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_বৃক্ষ(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, রিক্ত);
	যদি (!revs.pending.nr && is_bare_repository(the_repository)) {
		সংরচনা commit *head_commit;
		সংরচনা object_id head_oid;

		যদি (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, রিক্ত) ||
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
	যদি (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.অঙ্ক_সাৰিগুলি;

	যদি (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	বাবে (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, শীৰ্ষ;
		যদি (parse_range_arg(range_list.বস্তুসমূহ[range_i].string,
				    nth_সাৰি_cb, &sb, lno, anchor,
				    &bottom, &শীৰ্ষ, sb.path,
				    the_repository->index))
			usage(str_usage);
		যদি ((!lno && (শীৰ্ষ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		যদি (bottom < 1)
			bottom = 1;
		যদি (শীৰ্ষ < 1 || lno < শীৰ্ষ)
			শীৰ্ষ = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, শীৰ্ষ);
		anchor = শীৰ্ষ + 1;
	}
	sort_and_merge_range_set(&ranges);

	বাবে (range_i = ranges.nr; range_i > 0; --range_i) {
		ধ্রুৱ সংরচনা range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		অঙ্ক_সাৰিগুলি += (r->end - r->start);
	}
	যদি (!অঙ্ক_সাৰিগুলি)
		অঙ্ক_সাৰিগুলি = sb.অঙ্ক_সাৰিগুলি;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = রিক্ত;

	যদি (blame_move_score)
		sb.move_score = blame_move_score;
	যদি (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_মূল = show_মূল;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	যদি (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     অঙ্ক_সাৰিগুলি);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	যদি (!incremental)
		setup_pager(the_repository);
	অন্যথা
		গচ্ছ cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	যদি (!(output_option & (OUTPUT_COLOR_সাৰি | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	যদি (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		যদি (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_সাৰি))
			xsnprintf(repeated_meta_color,
				  পরিমাণ(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	যদি (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_সাৰি | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	যদি (show_stats) {
		printf("num read blob: %d\n", sb.অঙ্ক_read_blob);
		printf("num get patch: %d\n", sb.অঙ্ক_get_patch);
		printf("num commits: %d\n", sb.অঙ্ক_commits);
	}

cleanup:
	বাবে (ent = sb.ent; ent; ) {
		সংরচনা blame_entry *e = ent->next;
		মুক্ত(ent);
		ent = e;
	}

	মুক্ত(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	ঘূৰাই 0;
}
