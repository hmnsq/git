/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "mailmap.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "prio-queue.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "userdiff.h"
#അന്തര്ഭാവ "line-range.h"
#അന്തര്ഭാവ "line-log.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "blame.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
സ്ഥിതം സ്ഥിരം അക്ഷരം annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ longest_file;
സ്ഥിതം പൂർണ്ണസംഖ്യ longest_author;
സ്ഥിതം പൂർണ്ണസംഖ്യ max_orig_digits;
സ്ഥിതം പൂർണ്ണസംഖ്യ max_digits;
സ്ഥിതം പൂർണ്ണസംഖ്യ max_score_digits;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_വേര്;
സ്ഥിതം പൂർണ്ണസംഖ്യ reverse;
സ്ഥിതം പൂർണ്ണസംഖ്യ blank_boundary;
സ്ഥിതം പൂർണ്ണസംഖ്യ incremental;
സ്ഥിതം പൂർണ്ണസംഖ്യ xdl_opts;
സ്ഥിതം പൂർണ്ണസംഖ്യ abbrev = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ no_whole_file_rename;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_progress;
സ്ഥിതം അക്ഷരം repeated_meta_color[COLOR_MAXLEN];
സ്ഥിതം പൂർണ്ണസംഖ്യ coloring_mode;
സ്ഥിതം ഘടന string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
സ്ഥിതം പൂർണ്ണസംഖ്യ mark_unblamable_വരികൾ;
സ്ഥിതം പൂർണ്ണസംഖ്യ mark_ignored_വരികൾ;

സ്ഥിതം ഘടന date_mode blame_date_mode = { DATE_ISO8601 };
സ്ഥിതം size_t blame_date_width;

സ്ഥിതം ഘടന string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

സ്ഥിതം unsigned blame_move_score;
സ്ഥിതം unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

ഘടന progress_info {
	ഘടന progress *progress;
	പൂർണ്ണസംഖ്യ blamed_വരികൾ;
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *nth_വരി_cb(ശൂന്യം *data, long lno)
{
	തിരികെ blame_nth_വരി((ഘടന blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
ഘടന commit_info {
	ഘടന strbuf author;
	ഘടന strbuf author_mail;
	timestamp_t author_time;
	ഘടന strbuf author_tz;

	/* filled only when asked for details */
	ഘടന strbuf committer;
	ഘടന strbuf committer_mail;
	timestamp_t committer_time;
	ഘടന strbuf committer_tz;

	ഘടന strbuf summary;
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
സ്ഥിതം ശൂന്യം get_ac_വരി(സ്ഥിരം അക്ഷരം *inbuf, സ്ഥിരം അക്ഷരം *what,
	ഘടന strbuf *name, ഘടന strbuf *mail,
	timestamp_t *time, ഘടന strbuf *tz)
{
	ഘടന ident_split ident;
	size_t len, maillen, namelen;
	സ്ഥിരം അക്ഷരം *tmp, *endp;
	സ്ഥിരം അക്ഷരം *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	എങ്കിൽ (!tmp)
		പോകുക error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	എങ്കിൽ (!endp)
		len = strlen(tmp);
	അല്ലെങ്കിൽ
		len = endp - tmp;

	എങ്കിൽ (split_ident_വരി(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		തിരികെ;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	എങ്കിൽ (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, ശൂന്യ, 10);
	അല്ലെങ്കിൽ
		*time = 0;

	എങ്കിൽ (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	അല്ലെങ്കിൽ
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (പൂർണ്ണസംഖ്യ)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

സ്ഥിതം ശൂന്യം commit_info_destroy(ഘടന commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

സ്ഥിതം ശൂന്യം get_commit_info(ഘടന commit *commit, ഘടന commit_info *ret)
{
	പൂർണ്ണസംഖ്യ len;
	സ്ഥിരം അക്ഷരം *subject, *encoding;
	സ്ഥിരം അക്ഷരം *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, ശൂന്യ, encoding);
	get_ac_വരി(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_വരി(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	എങ്കിൽ (len)
		strbuf_add(&ret->summary, subject, len);
	അല്ലെങ്കിൽ
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
സ്ഥിതം ശൂന്യം write_filename_info(ഘടന blame_origin *suspect)
{
	എങ്കിൽ (suspect->previous) {
		ഘടന blame_origin *prev = suspect->previous;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ emit_one_suspect_detail(ഘടന blame_origin *suspect, പൂർണ്ണസംഖ്യ repeat)
{
	ഘടന commit_info ci = COMMIT_INFO_INIT;

	എങ്കിൽ (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		തിരികെ 0;

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
	എങ്കിൽ (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	തിരികെ 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
സ്ഥിതം ശൂന്യം found_guilty_entry(ഘടന blame_entry *ent, ശൂന്യം *data)
{
	ഘടന progress_info *pi = (ഘടന progress_info *)data;

	എങ്കിൽ (incremental) {
		ഘടന blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->അക്കം_വരികൾ);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_വരികൾ += ent->അക്കം_വരികൾ;
	display_progress(pi->progress, pi->blamed_വരികൾ);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *format_time(timestamp_t time, സ്ഥിരം അക്ഷരം *tz_str,
			       പൂർണ്ണസംഖ്യ show_raw_time)
{
	സ്ഥിതം ഘടന strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	എങ്കിൽ (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	അല്ലെങ്കിൽ {
		സ്ഥിരം അക്ഷരം *time_str;
		size_t time_width;
		പൂർണ്ണസംഖ്യ tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		വേണ്ടി (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	തിരികെ time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_സംഖ്യ          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_വരി_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_വരി           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

സ്ഥിതം ശൂന്യം emit_porcelain_details(ഘടന blame_origin *suspect, പൂർണ്ണസംഖ്യ repeat)
{
	എങ്കിൽ (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
സ്ഥിതം ശൂന്യം emit_porcelain_per_വരി_details(ഘടന blame_entry *ent)
{
	എങ്കിൽ (mark_unblamable_വരികൾ && ent->unblamable)
		puts("unblamable");
	എങ്കിൽ (mark_ignored_വരികൾ && ent->ignored)
		puts("ignored");
}

സ്ഥിതം ശൂന്യം emit_porcelain(ഘടന blame_scoreboard *sb, ഘടന blame_entry *ent,
			   പൂർണ്ണസംഖ്യ opt)
{
	പൂർണ്ണസംഖ്യ repeat = opt & OUTPUT_വരി_PORCELAIN;
	പൂർണ്ണസംഖ്യ cnt;
	സ്ഥിരം അക്ഷരം *cp;
	ഘടന blame_origin *suspect = ent->suspect;
	അക്ഷരം hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->അക്കം_വരികൾ);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_വരി_details(ent);

	cp = blame_nth_വരി(sb, ent->lno);
	വേണ്ടി (cnt = 0; cnt < ent->അക്കം_വരികൾ; cnt++) {
		അക്ഷരം ch;
		എങ്കിൽ (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			എങ്കിൽ (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_വരി_details(ent);
		}
		putchar('\t');
		ചെയ്യുക {
			ch = *cp++;
			putchar(ch);
		} ആകുമ്പോൾ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	എങ്കിൽ (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

സ്ഥിതം ഘടന color_field {
	timestamp_t hop;
	അക്ഷരം col[COLOR_MAXLEN];
} *colorfield;
സ്ഥിതം പൂർണ്ണസംഖ്യ colorfield_nr, colorfield_alloc;

സ്ഥിതം ശൂന്യം parse_color_fields(സ്ഥിരം അക്ഷരം *s)
{
	ഘടന string_list l = STRING_LIST_INIT_DUP;
	ഘടന string_list_item *item;
	എണ്ണൽ { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		തിരഞ്ഞെടുപ്പ് (next) {
		അവസ്ഥ EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			നിർത്തുക;
		അവസ്ഥ EXPECT_COLOR:
			എങ്കിൽ (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			നിർത്തുക;
		}
	}

	എങ്കിൽ (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

സ്ഥിതം ശൂന്യം setup_default_color_by_age(ശൂന്യം)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

സ്ഥിതം ശൂന്യം determine_വരി_heat(ഘടന commit_info *ci, സ്ഥിരം അക്ഷരം **dest_color)
{
	പൂർണ്ണസംഖ്യ i = 0;

	ആകുമ്പോൾ (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ maybe_putc(പൂർണ്ണസംഖ്യ c, FILE *out)
{
	തിരികെ out ? putc(c, out) : 0;
}

സ്ഥിതം size_t print_marks(FILE *out, സ്ഥിരം ഘടന blame_entry *ent, പൂർണ്ണസംഖ്യ opt)
{
	size_t len = 0;

	എങ്കിൽ ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	എങ്കിൽ (mark_unblamable_വരികൾ && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	എങ്കിൽ (mark_ignored_വരികൾ && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	തിരികെ len;
}

സ്ഥിതം size_t എണ്ണം_marks(സ്ഥിരം ഘടന blame_entry *ent, പൂർണ്ണസംഖ്യ opt)
{
	തിരികെ print_marks(ശൂന്യ, ent, opt);
}

സ്ഥിതം ശൂന്യം emit_other(ഘടന blame_scoreboard *sb, ഘടന blame_entry *ent,
		       പൂർണ്ണസംഖ്യ opt, ഘടന blame_entry *prev_ent)
{
	പൂർണ്ണസംഖ്യ cnt;
	സ്ഥിരം അക്ഷരം *cp;
	ഘടന blame_origin *suspect = ent->suspect;
	ഘടന commit_info ci = COMMIT_INFO_INIT;
	അക്ഷരം hex[GIT_MAX_HEXSZ + 1];
	പൂർണ്ണസംഖ്യ show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	സ്ഥിരം അക്ഷരം *default_color = ശൂന്യ, *color = ശൂന്യ, *reset = ശൂന്യ;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_വരി(sb, ent->lno);

	എങ്കിൽ (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_വരി_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	എങ്കിൽ (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	വേണ്ടി (cnt = 0; cnt < ent->അക്കം_വരികൾ; cnt++) {
		അക്ഷരം ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		എങ്കിൽ (opt & OUTPUT_COLOR_വരി) {
			എങ്കിൽ (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} അല്ലെങ്കിൽ  {
				color = default_color ? default_color : ശൂന്യ;
				reset = default_color ? GIT_COLOR_RESET : ശൂന്യ;
			}
		}
		എങ്കിൽ (color)
			fputs(color, stdout);

		എങ്കിൽ ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (പൂർണ്ണസംഖ്യ)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		എങ്കിൽ (opt & OUTPUT_ANNOTATE_COMPAT) {
			സ്ഥിരം അക്ഷരം *name;
			എങ്കിൽ (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			അല്ലെങ്കിൽ
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			എങ്കിൽ (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			എങ്കിൽ (opt & OUTPUT_SHOW_സംഖ്യ)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			എങ്കിൽ (!(opt & OUTPUT_NO_AUTHOR)) {
				സ്ഥിരം അക്ഷരം *name;
				പൂർണ്ണസംഖ്യ pad;
				എങ്കിൽ (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				അല്ലെങ്കിൽ
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
		എങ്കിൽ (reset)
			fputs(reset, stdout);
		ചെയ്യുക {
			ch = *cp++;
			putchar(ch);
		} ആകുമ്പോൾ (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	എങ്കിൽ (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

സ്ഥിതം ശൂന്യം output(ഘടന blame_scoreboard *sb, പൂർണ്ണസംഖ്യ option)
{
	ഘടന blame_entry *ent, *prev_ent = ശൂന്യ;

	എങ്കിൽ (option & OUTPUT_PORCELAIN) {
		വേണ്ടി (ent = sb->ent; ent; ent = ent->next) {
			പൂർണ്ണസംഖ്യ എണ്ണം = 0;
			ഘടന blame_origin *suspect;
			ഘടന commit *commit = ent->suspect->commit;
			എങ്കിൽ (commit->object.flags & MORE_THAN_ONE_PATH)
				തുടരുക;
			വേണ്ടി (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				എങ്കിൽ (suspect->guilty && എണ്ണം++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					നിർത്തുക;
				}
			}
		}
	}

	വേണ്ടി (ent = sb->ent; ent; ent = ent->next) {
		എങ്കിൽ (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		അല്ലെങ്കിൽ {
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
സ്ഥിതം പൂർണ്ണസംഖ്യ read_ancestry(സ്ഥിരം അക്ഷരം *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ഘടന strbuf buf = STRBUF_INIT;
	എങ്കിൽ (!fp)
		തിരികെ -1;
	ആകുമ്പോൾ (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ഘടന commit_graft *graft = read_graft_വരി(&buf);
		എങ്കിൽ (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ update_auto_abbrev(പൂർണ്ണസംഖ്യ auto_abbrev, ഘടന blame_origin *suspect)
{
	സ്ഥിരം അക്ഷരം *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	പൂർണ്ണസംഖ്യ len = strlen(uniq);
	എങ്കിൽ (auto_abbrev < len)
		തിരികെ len;
	തിരികെ auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
സ്ഥിതം ശൂന്യം find_alignment(ഘടന blame_scoreboard *sb, പൂർണ്ണസംഖ്യ *option)
{
	പൂർണ്ണസംഖ്യ longest_src_വരികൾ = 0;
	പൂർണ്ണസംഖ്യ longest_dst_വരികൾ = 0;
	unsigned largest_score = 0;
	ഘടന blame_entry *e;
	പൂർണ്ണസംഖ്യ compute_auto_abbrev = (abbrev < 0);
	പൂർണ്ണസംഖ്യ auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_എണ്ണം = 0;

	വേണ്ടി (e = sb->ent; e; e = e->next) {
		ഘടന blame_origin *suspect = e->suspect;
		പൂർണ്ണസംഖ്യ അക്കം;
		size_t marks_എണ്ണം = എണ്ണം_marks(e, *option);

		എങ്കിൽ (max_marks_എണ്ണം < marks_എണ്ണം)
			max_marks_എണ്ണം = marks_എണ്ണം;
		എങ്കിൽ (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		എങ്കിൽ (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		അക്കം = strlen(suspect->path);
		എങ്കിൽ (longest_file < അക്കം)
			longest_file = അക്കം;
		എങ്കിൽ (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ഘടന commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			എങ്കിൽ (*option & OUTPUT_SHOW_EMAIL)
				അക്കം = utf8_strwidth(ci.author_mail.buf);
			അല്ലെങ്കിൽ
				അക്കം = utf8_strwidth(ci.author.buf);
			എങ്കിൽ (longest_author < അക്കം)
				longest_author = അക്കം;
			commit_info_destroy(&ci);
		}
		അക്കം = e->s_lno + e->അക്കം_വരികൾ;
		എങ്കിൽ (longest_src_വരികൾ < അക്കം)
			longest_src_വരികൾ = അക്കം;
		അക്കം = e->lno + e->അക്കം_വരികൾ;
		എങ്കിൽ (longest_dst_വരികൾ < അക്കം)
			longest_dst_വരികൾ = അക്കം;
		എങ്കിൽ (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_വരികൾ);
	max_digits = decimal_width(longest_dst_വരികൾ);
	max_score_digits = decimal_width(largest_score);

	എങ്കിൽ (compute_auto_abbrev)
		abbrev = auto_abbrev;
	എങ്കിൽ (abbrev < (പൂർണ്ണസംഖ്യ)the_hash_algo->hexsz) {
		abbrev += max_marks_എണ്ണം;
		എങ്കിൽ (abbrev > (പൂർണ്ണസംഖ്യ)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

സ്ഥിതം ശൂന്യം sanity_check_on_fail(ഘടന blame_scoreboard *sb, പൂർണ്ണസംഖ്യ baa)
{
	പൂർണ്ണസംഖ്യ opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_സംഖ്യ | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

സ്ഥിതം unsigned parse_score(സ്ഥിരം അക്ഷരം *arg)
{
	അക്ഷരം *end;
	unsigned long score = strtoul(arg, &end, 10);
	എങ്കിൽ (*end)
		തിരികെ 0;
	തിരികെ score;
}

സ്ഥിതം അക്ഷരം *add_prefix(സ്ഥിരം അക്ഷരം *prefix, സ്ഥിരം അക്ഷരം *path)
{
	തിരികെ prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_blame_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "blame.showroot")) {
		show_വേര് = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "blame.showemail")) {
		പൂർണ്ണസംഖ്യ *output_option = cb;
		എങ്കിൽ (git_config_bool(var, മൂല്യം))
			*output_option |= OUTPUT_SHOW_EMAIL;
		അല്ലെങ്കിൽ
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "blame.date")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		parse_date_format(മൂല്യം, &blame_date_mode);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "blame.ignorerevsfile")) {
		അക്ഷരം *str;
		പൂർണ്ണസംഖ്യ ret;

		ret = git_config_pathname(&str, var, മൂല്യം);
		എങ്കിൽ (ret)
			തിരികെ ret;
		എങ്കിൽ (str)
			string_list_തിരുകുക(&ignore_revs_file_list, str);
		സ്വതന്ത്രം(str);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_വരികൾ = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_വരികൾ = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "color.blame.repeatedlines")) {
		എങ്കിൽ (color_parse_mem(മൂല്യം, strlen(മൂല്യം), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "blame.coloring")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		എങ്കിൽ (!strcmp(മൂല്യം, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_വരി;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(മൂല്യം, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(മൂല്യം, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_വരി |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} അല്ലെങ്കിൽ {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", മൂല്യം);
			തിരികെ 0;
		}
	}

	എങ്കിൽ (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		diff_algorithm = parse_algorithm_മൂല്യം(മൂല്യം);
		എങ്കിൽ (diff_algorithm < 0)
			തിരികെ error(_("unknown value for config '%s': %s"),
				     var, മൂല്യം);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		തിരികെ 0;
	}

	എങ്കിൽ (git_diff_heuristic_config(var, മൂല്യം, cb) < 0)
		തിരികെ -1;
	എങ്കിൽ (userdiff_config(var, മൂല്യം) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ blame_copy_callback(സ്ഥിരം ഘടന option *option, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt = option->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	എങ്കിൽ (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	എങ്കിൽ (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	എങ്കിൽ (arg)
		blame_copy_score = parse_score(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ blame_move_callback(സ്ഥിരം ഘടന option *option, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt = option->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	എങ്കിൽ (arg)
		blame_move_score = parse_score(arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ blame_diff_algorithm_minimal(സ്ഥിരം ഘടന option *option,
					സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt = option->മൂല്യം;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	എങ്കിൽ (!unset)
		*opt |= XDF_NEED_MINIMAL;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ blame_diff_algorithm_callback(സ്ഥിരം ഘടന option *option,
					 സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt = option->മൂല്യം;
	long മൂല്യം = parse_algorithm_മൂല്യം(arg);

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (മൂല്യം < 0)
		തിരികെ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= മൂല്യം;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ is_a_rev(സ്ഥിരം അക്ഷരം *name)
{
	ഘടന object_id oid;

	എങ്കിൽ (repo_get_oid(the_repository, name, &oid))
		തിരികെ 0;
	തിരികെ OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ peel_to_commit_oid(ഘടന object_id *oid_ret, ശൂന്യം *cbdata)
{
	ഘടന repository *r = ((ഘടന blame_scoreboard *)cbdata)->repo;
	ഘടന object_id oid;

	oidcpy(&oid, oid_ret);
	ആകുമ്പോൾ (1) {
		ഘടന object *obj;
		പൂർണ്ണസംഖ്യ kind = odb_read_object_info(r->objects, &oid, ശൂന്യ);
		എങ്കിൽ (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			തിരികെ 0;
		}
		എങ്കിൽ (kind != OBJ_TAG)
			തിരികെ -1;
		obj = deref_tag(r, parse_object(r, &oid), ശൂന്യ, 0);
		എങ്കിൽ (!obj)
			തിരികെ -1;
		oidcpy(&oid, &obj->oid);
	}
}

സ്ഥിതം ശൂന്യം build_ignorelist(ഘടന blame_scoreboard *sb,
			     ഘടന string_list *ignore_revs_file_list,
			     ഘടന string_list *ignore_rev_list)
{
	ഘടന string_list_item *i;
	ഘടന object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		എങ്കിൽ (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		അല്ലെങ്കിൽ
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		എങ്കിൽ (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_തിരുകുക(&sb->ignore_list, &oid);
	}
}

പൂർണ്ണസംഖ്യ cmd_blame(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	ഘടന rev_info revs;
	അക്ഷരം *path = ശൂന്യ;
	ഘടന blame_scoreboard sb;
	ഘടന blame_origin *o;
	ഘടന blame_entry *ent = ശൂന്യ;
	long dashdash_pos, lno;
	ഘടന progress_info pi = { ശൂന്യ, 0 };

	ഘടന string_list range_list = STRING_LIST_INIT_NODUP;
	ഘടന string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	പൂർണ്ണസംഖ്യ output_option = 0, opt = 0;
	പൂർണ്ണസംഖ്യ show_stats = 0;
	സ്ഥിരം അക്ഷരം *revs_file = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *contents_from = ശൂന്യ;
	സ്ഥിരം ഘടന option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ശൂന്യ, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_വേര്, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_സംഖ്യ),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_വരി_PORCELAIN),
		OPT_BIT('c', ശൂന്യ, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', ശൂന്യ, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', ശൂന്യ, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', ശൂന്യ, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', ശൂന്യ, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_വരി),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ശൂന്യ,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', ശൂന്യ, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ശൂന്യ, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ശൂന്യ, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ശൂന്യ, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	ഘടന parse_opt_ctx_t ctx;
	പൂർണ്ണസംഖ്യ cmd_is_annotate = !strcmp(argv[0], "annotate");
	ഘടന range_set ranges;
	unsigned പൂർണ്ണസംഖ്യ range_i;
	long anchor;
	long അക്കം_വരികൾ = 0;
	സ്ഥിരം അക്ഷരം *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	സ്ഥിരം അക്ഷരം *സ്ഥിരം *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ശൂന്യ);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	വേണ്ടി (;;) {
		തിരഞ്ഞെടുപ്പ് (parse_options_step(&ctx, options, opt_usage)) {
		അവസ്ഥ PARSE_OPT_NON_OPTION:
		അവസ്ഥ PARSE_OPT_UNKNOWN:
			നിർത്തുക;
		അവസ്ഥ PARSE_OPT_HELP:
			exit(0);
		അവസ്ഥ PARSE_OPT_HELP_ERROR:
		അവസ്ഥ PARSE_OPT_ERROR:
		അവസ്ഥ PARSE_OPT_SUBCOMMAND:
			exit(129);
		അവസ്ഥ PARSE_OPT_COMPLETE:
			exit(0);
		അവസ്ഥ PARSE_OPT_DONE:
			എങ്കിൽ (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			പോകുക parse_done;
		}

		എങ്കിൽ (!strcmp(ctx.argv[0], "--reverse")) {
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

	എങ്കിൽ (incremental || (output_option & OUTPUT_PORCELAIN)) {
		എങ്കിൽ (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (show_progress < 0)
		show_progress = isatty(2);

	എങ്കിൽ (!abbrev)
		abbrev = the_hash_algo->hexsz;

	എങ്കിൽ (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	എങ്കിൽ (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} അല്ലെങ്കിൽ {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	തിരഞ്ഞെടുപ്പ് (blame_date_mode.type) {
	അവസ്ഥ DATE_RFC2822:
		blame_date_width = വലുപ്പം("Thu, 19 Oct 2006 16:00:04 -0700");
		നിർത്തുക;
	അവസ്ഥ DATE_ISO8601_STRICT:
		blame_date_width = വലുപ്പം("2006-10-19T16:00:04-07:00");
		നിർത്തുക;
	അവസ്ഥ DATE_ISO8601:
		blame_date_width = വലുപ്പം("2006-10-19 16:00:04 -0700");
		നിർത്തുക;
	അവസ്ഥ DATE_RAW:
		blame_date_width = വലുപ്പം("1161298804 -0700");
		നിർത്തുക;
	അവസ്ഥ DATE_UNIX:
		blame_date_width = വലുപ്പം("1161298804");
		നിർത്തുക;
	അവസ്ഥ DATE_SHORT:
		blame_date_width = വലുപ്പം("2006-10-19");
		നിർത്തുക;
	അവസ്ഥ DATE_RELATIVE:
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
		നിർത്തുക;
	അവസ്ഥ DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = വലുപ്പം("Thu Oct 19 16:00");
		നിർത്തുക;
	അവസ്ഥ DATE_NORMAL:
		blame_date_width = വലുപ്പം("Thu Oct 19 16:00:04 2006 -0700");
		നിർത്തുക;
	അവസ്ഥ DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		നിർത്തുക;
	}
	blame_date_width -= 1; /* strip the null */

	എങ്കിൽ (revs.diffopt.flags.find_copies_harder)
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
	എങ്കിൽ (dashdash_pos) {
		തിരഞ്ഞെടുപ്പ് (argc - dashdash_pos - 1) {
		അവസ്ഥ 2: /* (1b) */
			എങ്കിൽ (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		അവസ്ഥ 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = ശൂന്യ;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			usage_with_options(opt_usage, options);
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (argc < 2)
			usage_with_options(opt_usage, options);
		എങ്കിൽ (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} അല്ലെങ്കിൽ {	/* (2a) */
			എങ്കിൽ (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_വൃക്ഷം(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, ശൂന്യ);
	എങ്കിൽ (!revs.pending.nr && is_bare_repository(the_repository)) {
		ഘടന commit *head_commit;
		ഘടന object_id head_oid;

		എങ്കിൽ (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, ശൂന്യ) ||
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
	എങ്കിൽ (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.അക്കം_വരികൾ;

	എങ്കിൽ (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	വേണ്ടി (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, മുകൾ;
		എങ്കിൽ (parse_range_arg(range_list.ഇനങ്ങൾ[range_i].string,
				    nth_വരി_cb, &sb, lno, anchor,
				    &bottom, &മുകൾ, sb.path,
				    the_repository->index))
			usage(str_usage);
		എങ്കിൽ ((!lno && (മുകൾ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		എങ്കിൽ (bottom < 1)
			bottom = 1;
		എങ്കിൽ (മുകൾ < 1 || lno < മുകൾ)
			മുകൾ = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, മുകൾ);
		anchor = മുകൾ + 1;
	}
	sort_and_merge_range_set(&ranges);

	വേണ്ടി (range_i = ranges.nr; range_i > 0; --range_i) {
		സ്ഥിരം ഘടന range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		അക്കം_വരികൾ += (r->end - r->start);
	}
	എങ്കിൽ (!അക്കം_വരികൾ)
		അക്കം_വരികൾ = sb.അക്കം_വരികൾ;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ശൂന്യ;

	എങ്കിൽ (blame_move_score)
		sb.move_score = blame_move_score;
	എങ്കിൽ (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_വേര് = show_വേര്;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	എങ്കിൽ (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     അക്കം_വരികൾ);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	എങ്കിൽ (!incremental)
		setup_pager(the_repository);
	അല്ലെങ്കിൽ
		പോകുക cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	എങ്കിൽ (!(output_option & (OUTPUT_COLOR_വരി | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	എങ്കിൽ (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		എങ്കിൽ (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_വരി))
			xsnprintf(repeated_meta_color,
				  വലുപ്പം(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	എങ്കിൽ (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_വരി | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	എങ്കിൽ (show_stats) {
		printf("num read blob: %d\n", sb.അക്കം_read_blob);
		printf("num get patch: %d\n", sb.അക്കം_get_patch);
		printf("num commits: %d\n", sb.അക്കം_commits);
	}

cleanup:
	വേണ്ടി (ent = sb.ent; ent; ) {
		ഘടന blame_entry *e = ent->next;
		സ്വതന്ത്രം(ent);
		ent = e;
	}

	സ്വതന്ത്രം(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	തിരികെ 0;
}
