/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "mailmap.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "prio-queue.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "userdiff.h"
#ଅନ୍ତର୍ଭାଵ "line-range.h"
#ଅନ୍ତର୍ଭାଵ "line-log.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "blame.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା longest_file;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା longest_author;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_orig_digits;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_digits;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_score_digits;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ମୂଳ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reverse;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା blank_boundary;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା incremental;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା xdl_opts;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_whole_file_rename;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress;
ସ୍ଥିର ଅକ୍ଷର repeated_meta_color[COLOR_MAXLEN];
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା coloring_mode;
ସ୍ଥିର ସଂରଚନା string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_unblamable_ଧାଡ଼ିଗୁଡ଼ିକ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_ignored_ଧାଡ଼ିଗୁଡ଼ିକ;

ସ୍ଥିର ସଂରଚନା date_mode blame_date_mode = { DATE_ISO8601 };
ସ୍ଥିର size_t blame_date_width;

ସ୍ଥିର ସଂରଚନା string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

ସ୍ଥିର unsigned blame_move_score;
ସ୍ଥିର unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

ସଂରଚନା progress_info {
	ସଂରଚନା progress *progress;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା blamed_ଧାଡ଼ିଗୁଡ଼ିକ;
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *nth_ଧାଡ଼ି_cb(ଶୂନ୍ୟ *data, long lno)
{
	ଫେରନ୍ତୁ blame_nth_ଧାଡ଼ି((ସଂରଚନା blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
ସଂରଚନା commit_info {
	ସଂରଚନା strbuf author;
	ସଂରଚନା strbuf author_mail;
	timestamp_t author_time;
	ସଂରଚନା strbuf author_tz;

	/* filled only when asked for details */
	ସଂରଚନା strbuf committer;
	ସଂରଚନା strbuf committer_mail;
	timestamp_t committer_time;
	ସଂରଚନା strbuf committer_tz;

	ସଂରଚନା strbuf summary;
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
ସ୍ଥିର ଶୂନ୍ୟ get_ac_ଧାଡ଼ି(ଅଚଳ ଅକ୍ଷର *inbuf, ଅଚଳ ଅକ୍ଷର *what,
	ସଂରଚନା strbuf *name, ସଂରଚନା strbuf *mail,
	timestamp_t *time, ସଂରଚନା strbuf *tz)
{
	ସଂରଚନା ident_split ident;
	size_t len, maillen, namelen;
	ଅଚଳ ଅକ୍ଷର *tmp, *endp;
	ଅଚଳ ଅକ୍ଷର *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	ଯଦି (!tmp)
		ଯାଅ error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	ଯଦି (!endp)
		len = strlen(tmp);
	ଅନ୍ୟଥା
		len = endp - tmp;

	ଯଦି (split_ident_ଧାଡ଼ି(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		ଫେରନ୍ତୁ;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	ଯଦି (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, ଖାଲି, 10);
	ଅନ୍ୟଥା
		*time = 0;

	ଯଦି (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	ଅନ୍ୟଥା
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

ସ୍ଥିର ଶୂନ୍ୟ commit_info_destroy(ସଂରଚନା commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ସ୍ଥିର ଶୂନ୍ୟ get_commit_info(ସଂରଚନା commit *commit, ସଂରଚନା commit_info *ret)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
	ଅଚଳ ଅକ୍ଷର *subject, *encoding;
	ଅଚଳ ଅକ୍ଷର *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, ଖାଲି, encoding);
	get_ac_ଧାଡ଼ି(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_ଧାଡ଼ି(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	ଯଦି (len)
		strbuf_add(&ret->summary, subject, len);
	ଅନ୍ୟଥା
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
ସ୍ଥିର ଶୂନ୍ୟ write_filename_info(ସଂରଚନା blame_origin *suspect)
{
	ଯଦି (suspect->previous) {
		ସଂରଚନା blame_origin *prev = suspect->previous;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା emit_one_suspect_detail(ସଂରଚନା blame_origin *suspect, ପୂର୍ଣ୍ଣସଂଖ୍ୟା repeat)
{
	ସଂରଚନା commit_info ci = COMMIT_INFO_INIT;

	ଯଦି (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		ଫେରନ୍ତୁ 0;

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
	ଯଦି (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	ଫେରନ୍ତୁ 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ସ୍ଥିର ଶୂନ୍ୟ found_guilty_entry(ସଂରଚନା blame_entry *ent, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା progress_info *pi = (ସଂରଚନା progress_info *)data;

	ଯଦି (incremental) {
		ସଂରଚନା blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_ଧାଡ଼ିଗୁଡ଼ିକ += ent->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ;
	display_progress(pi->progress, pi->blamed_ଧାଡ଼ିଗୁଡ଼ିକ);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *format_time(timestamp_t time, ଅଚଳ ଅକ୍ଷର *tz_str,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_raw_time)
{
	ସ୍ଥିର ସଂରଚନା strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	ଯଦି (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	ଅନ୍ୟଥା {
		ଅଚଳ ଅକ୍ଷର *time_str;
		size_t time_width;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		ପାଇଁ (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	ଫେରନ୍ତୁ time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_ସଂଖ୍ୟା          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_ଧାଡ଼ି_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_ଧାଡ଼ି           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ସ୍ଥିର ଶୂନ୍ୟ emit_porcelain_details(ସଂରଚନା blame_origin *suspect, ପୂର୍ଣ୍ଣସଂଖ୍ୟା repeat)
{
	ଯଦି (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ସ୍ଥିର ଶୂନ୍ୟ emit_porcelain_per_ଧାଡ଼ି_details(ସଂରଚନା blame_entry *ent)
{
	ଯଦି (mark_unblamable_ଧାଡ଼ିଗୁଡ଼ିକ && ent->unblamable)
		puts("unblamable");
	ଯଦି (mark_ignored_ଧାଡ଼ିଗୁଡ଼ିକ && ent->ignored)
		puts("ignored");
}

ସ୍ଥିର ଶୂନ୍ୟ emit_porcelain(ସଂରଚନା blame_scoreboard *sb, ସଂରଚନା blame_entry *ent,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା repeat = opt & OUTPUT_ଧାଡ଼ି_PORCELAIN;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt;
	ଅଚଳ ଅକ୍ଷର *cp;
	ସଂରଚନା blame_origin *suspect = ent->suspect;
	ଅକ୍ଷର hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_ଧାଡ଼ି_details(ent);

	cp = blame_nth_ଧାଡ଼ି(sb, ent->lno);
	ପାଇଁ (cnt = 0; cnt < ent->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ; cnt++) {
		ଅକ୍ଷର ch;
		ଯଦି (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			ଯଦି (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_ଧାଡ଼ି_details(ent);
		}
		putchar('\t');
		କର {
			ch = *cp++;
			putchar(ch);
		} ଯେତେବେଳେ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	ଯଦି (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

ସ୍ଥିର ସଂରଚନା color_field {
	timestamp_t hop;
	ଅକ୍ଷର col[COLOR_MAXLEN];
} *colorfield;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା colorfield_nr, colorfield_alloc;

ସ୍ଥିର ଶୂନ୍ୟ parse_color_fields(ଅଚଳ ଅକ୍ଷର *s)
{
	ସଂରଚନା string_list l = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list_item *item;
	ଗଣନା { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		ବାଛ (next) {
		ସ୍ଥିତି EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି EXPECT_COLOR:
			ଯଦି (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ setup_default_color_by_age(ଶୂନ୍ୟ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ସ୍ଥିର ଶୂନ୍ୟ determine_ଧାଡ଼ି_heat(ସଂରଚନା commit_info *ci, ଅଚଳ ଅକ୍ଷର **dest_color)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

	ଯେତେବେଳେ (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା maybe_putc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, FILE *out)
{
	ଫେରନ୍ତୁ out ? putc(c, out) : 0;
}

ସ୍ଥିର size_t print_marks(FILE *out, ଅଚଳ ସଂରଚନା blame_entry *ent, ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt)
{
	size_t len = 0;

	ଯଦି ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	ଯଦି (mark_unblamable_ଧାଡ଼ିଗୁଡ଼ିକ && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	ଯଦି (mark_ignored_ଧାଡ଼ିଗୁଡ଼ିକ && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	ଫେରନ୍ତୁ len;
}

ସ୍ଥିର size_t ଗଣନା_marks(ଅଚଳ ସଂରଚନା blame_entry *ent, ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt)
{
	ଫେରନ୍ତୁ print_marks(ଖାଲି, ent, opt);
}

ସ୍ଥିର ଶୂନ୍ୟ emit_other(ସଂରଚନା blame_scoreboard *sb, ସଂରଚନା blame_entry *ent,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt, ସଂରଚନା blame_entry *prev_ent)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnt;
	ଅଚଳ ଅକ୍ଷର *cp;
	ସଂରଚନା blame_origin *suspect = ent->suspect;
	ସଂରଚନା commit_info ci = COMMIT_INFO_INIT;
	ଅକ୍ଷର hex[GIT_MAX_HEXSZ + 1];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ଅଚଳ ଅକ୍ଷର *default_color = ଖାଲି, *color = ଖାଲି, *reset = ଖାଲି;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_ଧାଡ଼ି(sb, ent->lno);

	ଯଦି (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_ଧାଡ଼ି_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	ଯଦି (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	ପାଇଁ (cnt = 0; cnt < ent->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ; cnt++) {
		ଅକ୍ଷର ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		ଯଦି (opt & OUTPUT_COLOR_ଧାଡ଼ି) {
			ଯଦି (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} ଅନ୍ୟଥା  {
				color = default_color ? default_color : ଖାଲି;
				reset = default_color ? GIT_COLOR_RESET : ଖାଲି;
			}
		}
		ଯଦି (color)
			fputs(color, stdout);

		ଯଦି ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		ଯଦି (opt & OUTPUT_ANNOTATE_COMPAT) {
			ଅଚଳ ଅକ୍ଷର *name;
			ଯଦି (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			ଅନ୍ୟଥା
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} ଅନ୍ୟଥା {
			ଯଦି (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			ଯଦି (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			ଯଦି (opt & OUTPUT_SHOW_ସଂଖ୍ୟା)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			ଯଦି (!(opt & OUTPUT_NO_AUTHOR)) {
				ଅଚଳ ଅକ୍ଷର *name;
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad;
				ଯଦି (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				ଅନ୍ୟଥା
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
		ଯଦି (reset)
			fputs(reset, stdout);
		କର {
			ch = *cp++;
			putchar(ch);
		} ଯେତେବେଳେ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	ଯଦି (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

ସ୍ଥିର ଶୂନ୍ୟ output(ସଂରଚନା blame_scoreboard *sb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା option)
{
	ସଂରଚନା blame_entry *ent, *prev_ent = ଖାଲି;

	ଯଦି (option & OUTPUT_PORCELAIN) {
		ପାଇଁ (ent = sb->ent; ent; ent = ent->next) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
			ସଂରଚନା blame_origin *suspect;
			ସଂରଚନା commit *commit = ent->suspect->commit;
			ଯଦି (commit->object.flags & MORE_THAN_ONE_PATH)
				ଜାରି;
			ପାଇଁ (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				ଯଦି (suspect->guilty && ଗଣନା_2++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					ଭାଙ୍ଗ;
				}
			}
		}
	}

	ପାଇଁ (ent = sb->ent; ent; ent = ent->next) {
		ଯଦି (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		ଅନ୍ୟଥା {
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_ancestry(ଅଚଳ ଅକ୍ଷର *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଯଦି (!fp)
		ଫେରନ୍ତୁ -1;
	ଯେତେବେଳେ (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ସଂରଚନା commit_graft *graft = read_graft_ଧାଡ଼ି(&buf);
		ଯଦି (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_auto_abbrev(ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_abbrev, ସଂରଚନା blame_origin *suspect)
{
	ଅଚଳ ଅକ୍ଷର *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(uniq);
	ଯଦି (auto_abbrev < len)
		ଫେରନ୍ତୁ len;
	ଫେରନ୍ତୁ auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ସ୍ଥିର ଶୂନ୍ୟ find_alignment(ସଂରଚନା blame_scoreboard *sb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *option)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା longest_src_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା longest_dst_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
	unsigned largest_score = 0;
	ସଂରଚନା blame_entry *e;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା compute_auto_abbrev = (abbrev < 0);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_ଗଣନା = 0;

	ପାଇଁ (e = sb->ent; e; e = e->next) {
		ସଂରଚନା blame_origin *suspect = e->suspect;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ;
		size_t marks_ଗଣନା = ଗଣନା_marks(e, *option);

		ଯଦି (max_marks_ଗଣନା < marks_ଗଣନା)
			max_marks_ଗଣନା = marks_ଗଣନା;
		ଯଦି (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		ଯଦି (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		ଅଙ୍କ = strlen(suspect->path);
		ଯଦି (longest_file < ଅଙ୍କ)
			longest_file = ଅଙ୍କ;
		ଯଦି (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ସଂରଚନା commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			ଯଦି (*option & OUTPUT_SHOW_EMAIL)
				ଅଙ୍କ = utf8_strwidth(ci.author_mail.buf);
			ଅନ୍ୟଥା
				ଅଙ୍କ = utf8_strwidth(ci.author.buf);
			ଯଦି (longest_author < ଅଙ୍କ)
				longest_author = ଅଙ୍କ;
			commit_info_destroy(&ci);
		}
		ଅଙ୍କ = e->s_lno + e->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ;
		ଯଦି (longest_src_ଧାଡ଼ିଗୁଡ଼ିକ < ଅଙ୍କ)
			longest_src_ଧାଡ଼ିଗୁଡ଼ିକ = ଅଙ୍କ;
		ଅଙ୍କ = e->lno + e->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ;
		ଯଦି (longest_dst_ଧାଡ଼ିଗୁଡ଼ିକ < ଅଙ୍କ)
			longest_dst_ଧାଡ଼ିଗୁଡ଼ିକ = ଅଙ୍କ;
		ଯଦି (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_ଧାଡ଼ିଗୁଡ଼ିକ);
	max_digits = decimal_width(longest_dst_ଧାଡ଼ିଗୁଡ଼ିକ);
	max_score_digits = decimal_width(largest_score);

	ଯଦି (compute_auto_abbrev)
		abbrev = auto_abbrev;
	ଯଦି (abbrev < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)the_hash_algo->hexsz) {
		abbrev += max_marks_ଗଣନା;
		ଯଦି (abbrev > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ sanity_check_on_fail(ସଂରଚନା blame_scoreboard *sb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା baa)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_ସଂଖ୍ୟା | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ସ୍ଥିର unsigned parse_score(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଅକ୍ଷର *end;
	unsigned long score = strtoul(arg, &end, 10);
	ଯଦି (*end)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ score;
}

ସ୍ଥିର ଅକ୍ଷର *add_prefix(ଅଚଳ ଅକ୍ଷର *prefix, ଅଚଳ ଅକ୍ଷର *path)
{
	ଫେରନ୍ତୁ prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_blame_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "blame.showroot")) {
		show_ମୂଳ = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "blame.showemail")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *output_option = cb;
		ଯଦି (git_config_bool(var, ମୂଲ୍ୟ))
			*output_option |= OUTPUT_SHOW_EMAIL;
		ଅନ୍ୟଥା
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "blame.date")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		parse_date_format(ମୂଲ୍ୟ, &blame_date_mode);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "blame.ignorerevsfile")) {
		ଅକ୍ଷର *str;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

		ret = git_config_pathname(&str, var, ମୂଲ୍ୟ);
		ଯଦି (ret)
			ଫେରନ୍ତୁ ret;
		ଯଦି (str)
			string_list_ଭର୍ତ୍ତି(&ignore_revs_file_list, str);
		ମୁକ୍ତ(str);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_ଧାଡ଼ିଗୁଡ଼ିକ = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_ଧାଡ଼ିଗୁଡ଼ିକ = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "color.blame.repeatedlines")) {
		ଯଦି (color_parse_mem(ମୂଲ୍ୟ, strlen(ମୂଲ୍ୟ), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, "blame.coloring")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଯଦି (!strcmp(ମୂଲ୍ୟ, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_ଧାଡ଼ି;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(ମୂଲ୍ୟ, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(ମୂଲ୍ୟ, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_ଧାଡ଼ି |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} ଅନ୍ୟଥା {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", ମୂଲ୍ୟ);
			ଫେରନ୍ତୁ 0;
		}
	}

	ଯଦି (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		diff_algorithm = parse_algorithm_ମୂଲ୍ୟ(ମୂଲ୍ୟ);
		ଯଦି (diff_algorithm < 0)
			ଫେରନ୍ତୁ error(_("unknown value for config '%s': %s"),
				     var, ମୂଲ୍ୟ);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (git_diff_heuristic_config(var, ମୂଲ୍ୟ, cb) < 0)
		ଫେରନ୍ତୁ -1;
	ଯଦି (userdiff_config(var, ମୂଲ୍ୟ) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା blame_copy_callback(ଅଚଳ ସଂରଚନା option *option, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt = option->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	ଯଦି (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	ଯଦି (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	ଯଦି (arg)
		blame_copy_score = parse_score(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା blame_move_callback(ଅଚଳ ସଂରଚନା option *option, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt = option->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	ଯଦି (arg)
		blame_move_score = parse_score(arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା blame_diff_algorithm_minimal(ଅଚଳ ସଂରଚନା option *option,
					ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt = option->ମୂଲ୍ୟ;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	ଯଦି (!unset)
		*opt |= XDF_NEED_MINIMAL;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା blame_diff_algorithm_callback(ଅଚଳ ସଂରଚନା option *option,
					 ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt = option->ମୂଲ୍ୟ;
	long ମୂଲ୍ୟ = parse_algorithm_ମୂଲ୍ୟ(arg);

	BUG_ON_OPT_NEG(unset);

	ଯଦି (ମୂଲ୍ୟ < 0)
		ଫେରନ୍ତୁ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= ମୂଲ୍ୟ;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_a_rev(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା object_id oid;

	ଯଦି (repo_get_oid(the_repository, name, &oid))
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା peel_to_commit_oid(ସଂରଚନା object_id *oid_ret, ଶୂନ୍ୟ *cbdata)
{
	ସଂରଚନା repository *r = ((ସଂରଚନା blame_scoreboard *)cbdata)->repo;
	ସଂରଚନା object_id oid;

	oidcpy(&oid, oid_ret);
	ଯେତେବେଳେ (1) {
		ସଂରଚନା object *obj;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = odb_read_object_info(r->objects, &oid, ଖାଲି);
		ଯଦି (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			ଫେରନ୍ତୁ 0;
		}
		ଯଦି (kind != OBJ_TAG)
			ଫେରନ୍ତୁ -1;
		obj = deref_tag(r, parse_object(r, &oid), ଖାଲି, 0);
		ଯଦି (!obj)
			ଫେରନ୍ତୁ -1;
		oidcpy(&oid, &obj->oid);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ build_ignorelist(ସଂରଚନା blame_scoreboard *sb,
			     ସଂରଚନା string_list *ignore_revs_file_list,
			     ସଂରଚନା string_list *ignore_rev_list)
{
	ସଂରଚନା string_list_item *i;
	ସଂରଚନା object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		ଯଦି (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		ଅନ୍ୟଥା
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		ଯଦି (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ଭର୍ତ୍ତି(&sb->ignore_list, &oid);
	}
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_blame(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା rev_info revs;
	ଅକ୍ଷର *path = ଖାଲି;
	ସଂରଚନା blame_scoreboard sb;
	ସଂରଚନା blame_origin *o;
	ସଂରଚନା blame_entry *ent = ଖାଲି;
	long dashdash_pos, lno;
	ସଂରଚନା progress_info pi = { ଖାଲି, 0 };

	ସଂରଚନା string_list range_list = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_option = 0, opt = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_stats = 0;
	ଅଚଳ ଅକ୍ଷର *revs_file = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *contents_from = ଖାଲି;
	ଅଚଳ ସଂରଚନା option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ଖାଲି, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ମୂଳ, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_ସଂଖ୍ୟା),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_ଧାଡ଼ି_PORCELAIN),
		OPT_BIT('c', ଖାଲି, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', ଖାଲି, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', ଖାଲି, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', ଖାଲି, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', ଖାଲି, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_ଧାଡ଼ି),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ଖାଲି,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', ଖାଲି, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ଖାଲି, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ଖାଲି, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ଖାଲି, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	ସଂରଚନା parse_opt_ctx_t ctx;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_is_annotate = !strcmp(argv[0], "annotate");
	ସଂରଚନା range_set ranges;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା range_i;
	long anchor;
	long ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
	ଅଚଳ ଅକ୍ଷର *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ଅଚଳ ଅକ୍ଷର *ଅଚଳ *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ଖାଲି);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	ପାଇଁ (;;) {
		ବାଛ (parse_options_step(&ctx, options, opt_usage)) {
		ସ୍ଥିତି PARSE_OPT_NON_OPTION:
		ସ୍ଥିତି PARSE_OPT_UNKNOWN:
			ଭାଙ୍ଗ;
		ସ୍ଥିତି PARSE_OPT_HELP:
			exit(0);
		ସ୍ଥିତି PARSE_OPT_HELP_ERROR:
		ସ୍ଥିତି PARSE_OPT_ERROR:
		ସ୍ଥିତି PARSE_OPT_SUBCOMMAND:
			exit(129);
		ସ୍ଥିତି PARSE_OPT_COMPLETE:
			exit(0);
		ସ୍ଥିତି PARSE_OPT_DONE:
			ଯଦି (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			ଯାଅ parse_done;
		}

		ଯଦି (!strcmp(ctx.argv[0], "--reverse")) {
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

	ଯଦି (incremental || (output_option & OUTPUT_PORCELAIN)) {
		ଯଦି (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} ଅନ୍ୟଥା ଯଦି (show_progress < 0)
		show_progress = isatty(2);

	ଯଦି (!abbrev)
		abbrev = the_hash_algo->hexsz;

	ଯଦି (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	ଯଦି (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} ଅନ୍ୟଥା {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	ବାଛ (blame_date_mode.type) {
	ସ୍ଥିତି DATE_RFC2822:
		blame_date_width = ଆକାର("Thu, 19 Oct 2006 16:00:04 -0700");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_ISO8601_STRICT:
		blame_date_width = ଆକାର("2006-10-19T16:00:04-07:00");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_ISO8601:
		blame_date_width = ଆକାର("2006-10-19 16:00:04 -0700");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_RAW:
		blame_date_width = ଆକାର("1161298804 -0700");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_UNIX:
		blame_date_width = ଆକାର("1161298804");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_SHORT:
		blame_date_width = ଆକାର("2006-10-19");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_RELATIVE:
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
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = ଆକାର("Thu Oct 19 16:00");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_NORMAL:
		blame_date_width = ଆକାର("Thu Oct 19 16:00:04 2006 -0700");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		ଭାଙ୍ଗ;
	}
	blame_date_width -= 1; /* strip the null */

	ଯଦି (revs.diffopt.flags.find_copies_harder)
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
	ଯଦି (dashdash_pos) {
		ବାଛ (argc - dashdash_pos - 1) {
		ସ୍ଥିତି 2: /* (1b) */
			ଯଦି (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		ସ୍ଥିତି 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = ଖାଲି;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			usage_with_options(opt_usage, options);
		}
	} ଅନ୍ୟଥା {
		ଯଦି (argc < 2)
			usage_with_options(opt_usage, options);
		ଯଦି (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} ଅନ୍ୟଥା {	/* (2a) */
			ଯଦି (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_ବୃକ୍ଷ(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, ଖାଲି);
	ଯଦି (!revs.pending.nr && is_bare_repository(the_repository)) {
		ସଂରଚନା commit *head_commit;
		ସଂରଚନା object_id head_oid;

		ଯଦି (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, ଖାଲି) ||
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
	ଯଦି (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ;

	ଯଦି (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	ପାଇଁ (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, ଶୀର୍ଷ;
		ଯଦି (parse_range_arg(range_list.ବସ୍ତୁଗୁଡ଼ିକ[range_i].string,
				    nth_ଧାଡ଼ି_cb, &sb, lno, anchor,
				    &bottom, &ଶୀର୍ଷ, sb.path,
				    the_repository->index))
			usage(str_usage);
		ଯଦି ((!lno && (ଶୀର୍ଷ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		ଯଦି (bottom < 1)
			bottom = 1;
		ଯଦି (ଶୀର୍ଷ < 1 || lno < ଶୀର୍ଷ)
			ଶୀର୍ଷ = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, ଶୀର୍ଷ);
		anchor = ଶୀର୍ଷ + 1;
	}
	sort_and_merge_range_set(&ranges);

	ପାଇଁ (range_i = ranges.nr; range_i > 0; --range_i) {
		ଅଚଳ ସଂରଚନା range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ += (r->end - r->start);
	}
	ଯଦି (!ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ)
		ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ = sb.ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ଖାଲି;

	ଯଦି (blame_move_score)
		sb.move_score = blame_move_score;
	ଯଦି (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ମୂଳ = show_ମୂଳ;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	ଯଦି (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	ଯଦି (!incremental)
		setup_pager(the_repository);
	ଅନ୍ୟଥା
		ଯାଅ cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	ଯଦି (!(output_option & (OUTPUT_COLOR_ଧାଡ଼ି | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	ଯଦି (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		ଯଦି (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_ଧାଡ଼ି))
			xsnprintf(repeated_meta_color,
				  ଆକାର(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	ଯଦି (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_ଧାଡ଼ି | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	ଯଦି (show_stats) {
		printf("num read blob: %d\n", sb.ଅଙ୍କ_read_blob);
		printf("num get patch: %d\n", sb.ଅଙ୍କ_get_patch);
		printf("num commits: %d\n", sb.ଅଙ୍କ_commits);
	}

cleanup:
	ପାଇଁ (ent = sb.ent; ent; ) {
		ସଂରଚନା blame_entry *e = ent->next;
		ମୁକ୍ତ(ent);
		ent = e;
	}

	ମୁକ୍ତ(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	ଫେରନ୍ତୁ 0;
}
