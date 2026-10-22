/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "mailmap.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "prio-queue.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "userdiff.h"
#அந்தர்பாவ "line-range.h"
#அந்தர்பாவ "line-log.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "blame.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான மாறிலி எழுத்து blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
நிலையான மாறிலி எழுத்து annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

நிலையான மாறிலி எழுத்து *மாறிலி blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *மாறிலி annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	வெற்று
};

நிலையான முழு longest_file;
நிலையான முழு longest_author;
நிலையான முழு max_orig_digits;
நிலையான முழு max_digits;
நிலையான முழு max_score_digits;
நிலையான முழு show_வேர்;
நிலையான முழு reverse;
நிலையான முழு blank_boundary;
நிலையான முழு incremental;
நிலையான முழு xdl_opts;
நிலையான முழு abbrev = -1;
நிலையான முழு no_whole_file_rename;
நிலையான முழு show_progress;
நிலையான எழுத்து repeated_meta_color[COLOR_MAXLEN];
நிலையான முழு coloring_mode;
நிலையான கட்டமைப்பு string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
நிலையான முழு mark_unblamable_வரிகள்;
நிலையான முழு mark_ignored_வரிகள்;

நிலையான கட்டமைப்பு date_mode blame_date_mode = { DATE_ISO8601 };
நிலையான size_t blame_date_width;

நிலையான கட்டமைப்பு string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

நிலையான unsigned blame_move_score;
நிலையான unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

கட்டமைப்பு progress_info {
	கட்டமைப்பு progress *progress;
	முழு blamed_வரிகள்;
};

நிலையான மாறிலி எழுத்து *nth_வரி_cb(வெறுமை *data, long lno)
{
	திரும்பு blame_nth_வரி((கட்டமைப்பு blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
கட்டமைப்பு commit_info {
	கட்டமைப்பு strbuf author;
	கட்டமைப்பு strbuf author_mail;
	timestamp_t author_time;
	கட்டமைப்பு strbuf author_tz;

	/* filled only when asked for details */
	கட்டமைப்பு strbuf committer;
	கட்டமைப்பு strbuf committer_mail;
	timestamp_t committer_time;
	கட்டமைப்பு strbuf committer_tz;

	கட்டமைப்பு strbuf summary;
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
நிலையான வெறுமை get_ac_வரி(மாறிலி எழுத்து *inbuf, மாறிலி எழுத்து *what,
	கட்டமைப்பு strbuf *name, கட்டமைப்பு strbuf *mail,
	timestamp_t *time, கட்டமைப்பு strbuf *tz)
{
	கட்டமைப்பு ident_split ident;
	size_t len, maillen, namelen;
	மாறிலி எழுத்து *tmp, *endp;
	மாறிலி எழுத்து *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	எனில் (!tmp)
		செல் error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	எனில் (!endp)
		len = strlen(tmp);
	இல்லையெனில்
		len = endp - tmp;

	எனில் (split_ident_வரி(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		திரும்பு;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	எனில் (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, வெற்று, 10);
	இல்லையெனில்
		*time = 0;

	எனில் (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	இல்லையெனில்
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (முழு)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

நிலையான வெறுமை commit_info_destroy(கட்டமைப்பு commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

நிலையான வெறுமை get_commit_info(கட்டமைப்பு commit *commit, கட்டமைப்பு commit_info *ret)
{
	முழு len;
	மாறிலி எழுத்து *subject, *encoding;
	மாறிலி எழுத்து *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, வெற்று, encoding);
	get_ac_வரி(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_வரி(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	எனில் (len)
		strbuf_add(&ret->summary, subject, len);
	இல்லையெனில்
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
நிலையான வெறுமை write_filename_info(கட்டமைப்பு blame_origin *suspect)
{
	எனில் (suspect->previous) {
		கட்டமைப்பு blame_origin *prev = suspect->previous;
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
நிலையான முழு emit_one_suspect_detail(கட்டமைப்பு blame_origin *suspect, முழு repeat)
{
	கட்டமைப்பு commit_info ci = COMMIT_INFO_INIT;

	எனில் (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		திரும்பு 0;

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
	எனில் (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	திரும்பு 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
நிலையான வெறுமை found_guilty_entry(கட்டமைப்பு blame_entry *ent, வெறுமை *data)
{
	கட்டமைப்பு progress_info *pi = (கட்டமைப்பு progress_info *)data;

	எனில் (incremental) {
		கட்டமைப்பு blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->இலக்கம்_வரிகள்);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_வரிகள் += ent->இலக்கம்_வரிகள்;
	display_progress(pi->progress, pi->blamed_வரிகள்);
}

நிலையான மாறிலி எழுத்து *format_time(timestamp_t time, மாறிலி எழுத்து *tz_str,
			       முழு show_raw_time)
{
	நிலையான கட்டமைப்பு strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	எனில் (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	இல்லையெனில் {
		மாறிலி எழுத்து *time_str;
		size_t time_width;
		முழு tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		க்கு (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	திரும்பு time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_எண்          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_வரி_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_வரி           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

நிலையான வெறுமை emit_porcelain_details(கட்டமைப்பு blame_origin *suspect, முழு repeat)
{
	எனில் (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
நிலையான வெறுமை emit_porcelain_per_வரி_details(கட்டமைப்பு blame_entry *ent)
{
	எனில் (mark_unblamable_வரிகள் && ent->unblamable)
		puts("unblamable");
	எனில் (mark_ignored_வரிகள் && ent->ignored)
		puts("ignored");
}

நிலையான வெறுமை emit_porcelain(கட்டமைப்பு blame_scoreboard *sb, கட்டமைப்பு blame_entry *ent,
			   முழு opt)
{
	முழு repeat = opt & OUTPUT_வரி_PORCELAIN;
	முழு cnt;
	மாறிலி எழுத்து *cp;
	கட்டமைப்பு blame_origin *suspect = ent->suspect;
	எழுத்து hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->இலக்கம்_வரிகள்);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_வரி_details(ent);

	cp = blame_nth_வரி(sb, ent->lno);
	க்கு (cnt = 0; cnt < ent->இலக்கம்_வரிகள்; cnt++) {
		எழுத்து ch;
		எனில் (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			எனில் (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_வரி_details(ent);
		}
		putchar('\t');
		செய் {
			ch = *cp++;
			putchar(ch);
		} போது (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	எனில் (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

நிலையான கட்டமைப்பு color_field {
	timestamp_t hop;
	எழுத்து col[COLOR_MAXLEN];
} *colorfield;
நிலையான முழு colorfield_nr, colorfield_alloc;

நிலையான வெறுமை parse_color_fields(மாறிலி எழுத்து *s)
{
	கட்டமைப்பு string_list l = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list_item *item;
	எண்ணிக்கை { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		தேர்வு (next) {
		நிலை EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			நிறுத்து;
		நிலை EXPECT_COLOR:
			எனில் (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			நிறுத்து;
		}
	}

	எனில் (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

நிலையான வெறுமை setup_default_color_by_age(வெறுமை)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

நிலையான வெறுமை determine_வரி_heat(கட்டமைப்பு commit_info *ci, மாறிலி எழுத்து **dest_color)
{
	முழு i = 0;

	போது (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

நிலையான உள்வரி முழு maybe_putc(முழு c, FILE *out)
{
	திரும்பு out ? putc(c, out) : 0;
}

நிலையான size_t print_marks(FILE *out, மாறிலி கட்டமைப்பு blame_entry *ent, முழு opt)
{
	size_t len = 0;

	எனில் ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	எனில் (mark_unblamable_வரிகள் && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	எனில் (mark_ignored_வரிகள் && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	திரும்பு len;
}

நிலையான size_t மொத்தம்_marks(மாறிலி கட்டமைப்பு blame_entry *ent, முழு opt)
{
	திரும்பு print_marks(வெற்று, ent, opt);
}

நிலையான வெறுமை emit_other(கட்டமைப்பு blame_scoreboard *sb, கட்டமைப்பு blame_entry *ent,
		       முழு opt, கட்டமைப்பு blame_entry *prev_ent)
{
	முழு cnt;
	மாறிலி எழுத்து *cp;
	கட்டமைப்பு blame_origin *suspect = ent->suspect;
	கட்டமைப்பு commit_info ci = COMMIT_INFO_INIT;
	எழுத்து hex[GIT_MAX_HEXSZ + 1];
	முழு show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	மாறிலி எழுத்து *default_color = வெற்று, *color = வெற்று, *reset = வெற்று;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_வரி(sb, ent->lno);

	எனில் (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_வரி_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	எனில் (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	க்கு (cnt = 0; cnt < ent->இலக்கம்_வரிகள்; cnt++) {
		எழுத்து ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		எனில் (opt & OUTPUT_COLOR_வரி) {
			எனில் (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} இல்லையெனில்  {
				color = default_color ? default_color : வெற்று;
				reset = default_color ? GIT_COLOR_RESET : வெற்று;
			}
		}
		எனில் (color)
			fputs(color, stdout);

		எனில் ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (முழு)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		எனில் (opt & OUTPUT_ANNOTATE_COMPAT) {
			மாறிலி எழுத்து *name;
			எனில் (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			இல்லையெனில்
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} இல்லையெனில் {
			எனில் (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			எனில் (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			எனில் (opt & OUTPUT_SHOW_எண்)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			எனில் (!(opt & OUTPUT_NO_AUTHOR)) {
				மாறிலி எழுத்து *name;
				முழு pad;
				எனில் (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				இல்லையெனில்
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
		எனில் (reset)
			fputs(reset, stdout);
		செய் {
			ch = *cp++;
			putchar(ch);
		} போது (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	எனில் (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

நிலையான வெறுமை output(கட்டமைப்பு blame_scoreboard *sb, முழு option)
{
	கட்டமைப்பு blame_entry *ent, *prev_ent = வெற்று;

	எனில் (option & OUTPUT_PORCELAIN) {
		க்கு (ent = sb->ent; ent; ent = ent->next) {
			முழு மொத்தம் = 0;
			கட்டமைப்பு blame_origin *suspect;
			கட்டமைப்பு commit *commit = ent->suspect->commit;
			எனில் (commit->object.flags & MORE_THAN_ONE_PATH)
				தொடர்;
			க்கு (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				எனில் (suspect->guilty && மொத்தம்++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					நிறுத்து;
				}
			}
		}
	}

	க்கு (ent = sb->ent; ent; ent = ent->next) {
		எனில் (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		இல்லையெனில் {
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
நிலையான முழு read_ancestry(மாறிலி எழுத்து *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	எனில் (!fp)
		திரும்பு -1;
	போது (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		கட்டமைப்பு commit_graft *graft = read_graft_வரி(&buf);
		எனில் (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	திரும்பு 0;
}

நிலையான முழு update_auto_abbrev(முழு auto_abbrev, கட்டமைப்பு blame_origin *suspect)
{
	மாறிலி எழுத்து *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	முழு len = strlen(uniq);
	எனில் (auto_abbrev < len)
		திரும்பு len;
	திரும்பு auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
நிலையான வெறுமை find_alignment(கட்டமைப்பு blame_scoreboard *sb, முழு *option)
{
	முழு longest_src_வரிகள் = 0;
	முழு longest_dst_வரிகள் = 0;
	unsigned largest_score = 0;
	கட்டமைப்பு blame_entry *e;
	முழு compute_auto_abbrev = (abbrev < 0);
	முழு auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_மொத்தம் = 0;

	க்கு (e = sb->ent; e; e = e->next) {
		கட்டமைப்பு blame_origin *suspect = e->suspect;
		முழு இலக்கம்;
		size_t marks_மொத்தம் = மொத்தம்_marks(e, *option);

		எனில் (max_marks_மொத்தம் < marks_மொத்தம்)
			max_marks_மொத்தம் = marks_மொத்தம்;
		எனில் (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		எனில் (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		இலக்கம் = strlen(suspect->path);
		எனில் (longest_file < இலக்கம்)
			longest_file = இலக்கம்;
		எனில் (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			கட்டமைப்பு commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			எனில் (*option & OUTPUT_SHOW_EMAIL)
				இலக்கம் = utf8_strwidth(ci.author_mail.buf);
			இல்லையெனில்
				இலக்கம் = utf8_strwidth(ci.author.buf);
			எனில் (longest_author < இலக்கம்)
				longest_author = இலக்கம்;
			commit_info_destroy(&ci);
		}
		இலக்கம் = e->s_lno + e->இலக்கம்_வரிகள்;
		எனில் (longest_src_வரிகள் < இலக்கம்)
			longest_src_வரிகள் = இலக்கம்;
		இலக்கம் = e->lno + e->இலக்கம்_வரிகள்;
		எனில் (longest_dst_வரிகள் < இலக்கம்)
			longest_dst_வரிகள் = இலக்கம்;
		எனில் (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_வரிகள்);
	max_digits = decimal_width(longest_dst_வரிகள்);
	max_score_digits = decimal_width(largest_score);

	எனில் (compute_auto_abbrev)
		abbrev = auto_abbrev;
	எனில் (abbrev < (முழு)the_hash_algo->hexsz) {
		abbrev += max_marks_மொத்தம்;
		எனில் (abbrev > (முழு)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

நிலையான வெறுமை sanity_check_on_fail(கட்டமைப்பு blame_scoreboard *sb, முழு baa)
{
	முழு opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_எண் | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

நிலையான unsigned parse_score(மாறிலி எழுத்து *arg)
{
	எழுத்து *end;
	unsigned long score = strtoul(arg, &end, 10);
	எனில் (*end)
		திரும்பு 0;
	திரும்பு score;
}

நிலையான எழுத்து *add_prefix(மாறிலி எழுத்து *prefix, மாறிலி எழுத்து *path)
{
	திரும்பு prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

நிலையான முழு git_blame_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "blame.showroot")) {
		show_வேர் = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "blame.showemail")) {
		முழு *output_option = cb;
		எனில் (git_config_bool(var, மதிப்பு))
			*output_option |= OUTPUT_SHOW_EMAIL;
		இல்லையெனில்
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "blame.date")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		parse_date_format(மதிப்பு, &blame_date_mode);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "blame.ignorerevsfile")) {
		எழுத்து *str;
		முழு ret;

		ret = git_config_pathname(&str, var, மதிப்பு);
		எனில் (ret)
			திரும்பு ret;
		எனில் (str)
			string_list_செருகு(&ignore_revs_file_list, str);
		விடுவி(str);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_வரிகள் = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_வரிகள் = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "color.blame.repeatedlines")) {
		எனில் (color_parse_mem(மதிப்பு, strlen(மதிப்பு), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", மதிப்பு);
		திரும்பு 0;
	}
	எனில் (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "blame.coloring")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		எனில் (!strcmp(மதிப்பு, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_வரி;
		} இல்லையெனில் எனில் (!strcmp(மதிப்பு, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} இல்லையெனில் எனில் (!strcmp(மதிப்பு, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_வரி |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} இல்லையெனில் {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", மதிப்பு);
			திரும்பு 0;
		}
	}

	எனில் (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		diff_algorithm = parse_algorithm_மதிப்பு(மதிப்பு);
		எனில் (diff_algorithm < 0)
			திரும்பு error(_("unknown value for config '%s': %s"),
				     var, மதிப்பு);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		திரும்பு 0;
	}

	எனில் (git_diff_heuristic_config(var, மதிப்பு, cb) < 0)
		திரும்பு -1;
	எனில் (userdiff_config(var, மதிப்பு) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான முழு blame_copy_callback(மாறிலி கட்டமைப்பு option *option, மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt = option->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	எனில் (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	எனில் (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	எனில் (arg)
		blame_copy_score = parse_score(arg);
	திரும்பு 0;
}

நிலையான முழு blame_move_callback(மாறிலி கட்டமைப்பு option *option, மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt = option->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	எனில் (arg)
		blame_move_score = parse_score(arg);
	திரும்பு 0;
}

நிலையான முழு blame_diff_algorithm_minimal(மாறிலி கட்டமைப்பு option *option,
					மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt = option->மதிப்பு;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	எனில் (!unset)
		*opt |= XDF_NEED_MINIMAL;

	திரும்பு 0;
}

நிலையான முழு blame_diff_algorithm_callback(மாறிலி கட்டமைப்பு option *option,
					 மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt = option->மதிப்பு;
	long மதிப்பு = parse_algorithm_மதிப்பு(arg);

	BUG_ON_OPT_NEG(unset);

	எனில் (மதிப்பு < 0)
		திரும்பு error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= மதிப்பு;

	திரும்பு 0;
}

நிலையான முழு is_a_rev(மாறிலி எழுத்து *name)
{
	கட்டமைப்பு object_id oid;

	எனில் (repo_get_oid(the_repository, name, &oid))
		திரும்பு 0;
	திரும்பு OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, வெற்று);
}

நிலையான முழு peel_to_commit_oid(கட்டமைப்பு object_id *oid_ret, வெறுமை *cbdata)
{
	கட்டமைப்பு repository *r = ((கட்டமைப்பு blame_scoreboard *)cbdata)->repo;
	கட்டமைப்பு object_id oid;

	oidcpy(&oid, oid_ret);
	போது (1) {
		கட்டமைப்பு object *obj;
		முழு kind = odb_read_object_info(r->objects, &oid, வெற்று);
		எனில் (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			திரும்பு 0;
		}
		எனில் (kind != OBJ_TAG)
			திரும்பு -1;
		obj = deref_tag(r, parse_object(r, &oid), வெற்று, 0);
		எனில் (!obj)
			திரும்பு -1;
		oidcpy(&oid, &obj->oid);
	}
}

நிலையான வெறுமை build_ignorelist(கட்டமைப்பு blame_scoreboard *sb,
			     கட்டமைப்பு string_list *ignore_revs_file_list,
			     கட்டமைப்பு string_list *ignore_rev_list)
{
	கட்டமைப்பு string_list_item *i;
	கட்டமைப்பு object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		எனில் (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		இல்லையெனில்
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		எனில் (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_செருகு(&sb->ignore_list, &oid);
	}
}

முழு cmd_blame(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு rev_info revs;
	எழுத்து *path = வெற்று;
	கட்டமைப்பு blame_scoreboard sb;
	கட்டமைப்பு blame_origin *o;
	கட்டமைப்பு blame_entry *ent = வெற்று;
	long dashdash_pos, lno;
	கட்டமைப்பு progress_info pi = { வெற்று, 0 };

	கட்டமைப்பு string_list range_list = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	முழு output_option = 0, opt = 0;
	முழு show_stats = 0;
	மாறிலி எழுத்து *revs_file = வெற்று;
	மாறிலி எழுத்து *contents_from = வெற்று;
	மாறிலி கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', வெற்று, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_வேர், N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_எண்),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_வரி_PORCELAIN),
		OPT_BIT('c', வெற்று, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', வெற்று, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', வெற்று, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', வெற்று, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', வெற்று, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_வரி),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, வெற்று,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', வெற்று, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', வெற்று, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', வெற்று, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', வெற்று, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	கட்டமைப்பு parse_opt_ctx_t ctx;
	முழு cmd_is_annotate = !strcmp(argv[0], "annotate");
	கட்டமைப்பு range_set ranges;
	unsigned முழு range_i;
	long anchor;
	long இலக்கம்_வரிகள் = 0;
	மாறிலி எழுத்து *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	மாறிலி எழுத்து *மாறிலி *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, வெற்று);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	க்கு (;;) {
		தேர்வு (parse_options_step(&ctx, options, opt_usage)) {
		நிலை PARSE_OPT_NON_OPTION:
		நிலை PARSE_OPT_UNKNOWN:
			நிறுத்து;
		நிலை PARSE_OPT_HELP:
			exit(0);
		நிலை PARSE_OPT_HELP_ERROR:
		நிலை PARSE_OPT_ERROR:
		நிலை PARSE_OPT_SUBCOMMAND:
			exit(129);
		நிலை PARSE_OPT_COMPLETE:
			exit(0);
		நிலை PARSE_OPT_DONE:
			எனில் (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			செல் parse_done;
		}

		எனில் (!strcmp(ctx.argv[0], "--reverse")) {
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

	எனில் (incremental || (output_option & OUTPUT_PORCELAIN)) {
		எனில் (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} இல்லையெனில் எனில் (show_progress < 0)
		show_progress = isatty(2);

	எனில் (!abbrev)
		abbrev = the_hash_algo->hexsz;

	எனில் (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	எனில் (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} இல்லையெனில் {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	தேர்வு (blame_date_mode.type) {
	நிலை DATE_RFC2822:
		blame_date_width = அளவு("Thu, 19 Oct 2006 16:00:04 -0700");
		நிறுத்து;
	நிலை DATE_ISO8601_STRICT:
		blame_date_width = அளவு("2006-10-19T16:00:04-07:00");
		நிறுத்து;
	நிலை DATE_ISO8601:
		blame_date_width = அளவு("2006-10-19 16:00:04 -0700");
		நிறுத்து;
	நிலை DATE_RAW:
		blame_date_width = அளவு("1161298804 -0700");
		நிறுத்து;
	நிலை DATE_UNIX:
		blame_date_width = அளவு("1161298804");
		நிறுத்து;
	நிலை DATE_SHORT:
		blame_date_width = அளவு("2006-10-19");
		நிறுத்து;
	நிலை DATE_RELATIVE:
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
		நிறுத்து;
	நிலை DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = அளவு("Thu Oct 19 16:00");
		நிறுத்து;
	நிலை DATE_NORMAL:
		blame_date_width = அளவு("Thu Oct 19 16:00:04 2006 -0700");
		நிறுத்து;
	நிலை DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		நிறுத்து;
	}
	blame_date_width -= 1; /* strip the null */

	எனில் (revs.diffopt.flags.find_copies_harder)
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
	எனில் (dashdash_pos) {
		தேர்வு (argc - dashdash_pos - 1) {
		நிலை 2: /* (1b) */
			எனில் (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		நிலை 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = வெற்று;
			நிறுத்து;
		இயல்பு:
			usage_with_options(opt_usage, options);
		}
	} இல்லையெனில் {
		எனில் (argc < 2)
			usage_with_options(opt_usage, options);
		எனில் (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} இல்லையெனில் {	/* (2a) */
			எனில் (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_மரம்(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, வெற்று);
	எனில் (!revs.pending.nr && is_bare_repository(the_repository)) {
		கட்டமைப்பு commit *head_commit;
		கட்டமைப்பு object_id head_oid;

		எனில் (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, வெற்று) ||
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
	எனில் (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.இலக்கம்_வரிகள்;

	எனில் (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	க்கு (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, உச்சி;
		எனில் (parse_range_arg(range_list.பொருட்கள்[range_i].string,
				    nth_வரி_cb, &sb, lno, anchor,
				    &bottom, &உச்சி, sb.path,
				    the_repository->index))
			usage(str_usage);
		எனில் ((!lno && (உச்சி || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		எனில் (bottom < 1)
			bottom = 1;
		எனில் (உச்சி < 1 || lno < உச்சி)
			உச்சி = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, உச்சி);
		anchor = உச்சி + 1;
	}
	sort_and_merge_range_set(&ranges);

	க்கு (range_i = ranges.nr; range_i > 0; --range_i) {
		மாறிலி கட்டமைப்பு range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		இலக்கம்_வரிகள் += (r->end - r->start);
	}
	எனில் (!இலக்கம்_வரிகள்)
		இலக்கம்_வரிகள் = sb.இலக்கம்_வரிகள்;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = வெற்று;

	எனில் (blame_move_score)
		sb.move_score = blame_move_score;
	எனில் (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_வேர் = show_வேர்;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	எனில் (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     இலக்கம்_வரிகள்);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	எனில் (!incremental)
		setup_pager(the_repository);
	இல்லையெனில்
		செல் cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	எனில் (!(output_option & (OUTPUT_COLOR_வரி | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	எனில் (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		எனில் (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_வரி))
			xsnprintf(repeated_meta_color,
				  அளவு(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	எனில் (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_வரி | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	எனில் (show_stats) {
		printf("num read blob: %d\n", sb.இலக்கம்_read_blob);
		printf("num get patch: %d\n", sb.இலக்கம்_get_patch);
		printf("num commits: %d\n", sb.இலக்கம்_commits);
	}

cleanup:
	க்கு (ent = sb.ent; ent; ) {
		கட்டமைப்பு blame_entry *e = ent->next;
		விடுவி(ent);
		ent = e;
	}

	விடுவி(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	திரும்பு 0;
}
