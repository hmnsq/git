/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "mailmap.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "prio-queue.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "userdiff.h"
#અન્તર્ભાવ "line-range.h"
#અન્તર્ભાવ "line-log.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "blame.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર અચલ અક્ષર blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
સ્થિર અચલ અક્ષર annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

સ્થિર અચલ અક્ષર *અચલ blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ખાલી
};

સ્થિર અચલ અક્ષર *અચલ annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ખાલી
};

સ્થિર પૂર્ણાંક longest_file;
સ્થિર પૂર્ણાંક longest_author;
સ્થિર પૂર્ણાંક max_orig_digits;
સ્થિર પૂર્ણાંક max_digits;
સ્થિર પૂર્ણાંક max_score_digits;
સ્થિર પૂર્ણાંક show_મૂળ;
સ્થિર પૂર્ણાંક reverse;
સ્થિર પૂર્ણાંક blank_boundary;
સ્થિર પૂર્ણાંક incremental;
સ્થિર પૂર્ણાંક xdl_opts;
સ્થિર પૂર્ણાંક abbrev = -1;
સ્થિર પૂર્ણાંક no_whole_file_rename;
સ્થિર પૂર્ણાંક show_progress;
સ્થિર અક્ષર repeated_meta_color[COLOR_MAXLEN];
સ્થિર પૂર્ણાંક coloring_mode;
સ્થિર સંરચના string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
સ્થિર પૂર્ણાંક mark_unblamable_લીટીઓ;
સ્થિર પૂર્ણાંક mark_ignored_લીટીઓ;

સ્થિર સંરચના date_mode blame_date_mode = { DATE_ISO8601 };
સ્થિર size_t blame_date_width;

સ્થિર સંરચના string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

સ્થિર unsigned blame_move_score;
સ્થિર unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

સંરચના progress_info {
	સંરચના progress *progress;
	પૂર્ણાંક blamed_લીટીઓ;
};

સ્થિર અચલ અક્ષર *nth_લીટી_cb(શૂન્ય *data, long lno)
{
	પાછા blame_nth_લીટી((સંરચના blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
સંરચના commit_info {
	સંરચના strbuf author;
	સંરચના strbuf author_mail;
	timestamp_t author_time;
	સંરચના strbuf author_tz;

	/* filled only when asked for details */
	સંરચના strbuf committer;
	સંરચના strbuf committer_mail;
	timestamp_t committer_time;
	સંરચના strbuf committer_tz;

	સંરચના strbuf summary;
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
સ્થિર શૂન્ય get_ac_લીટી(અચલ અક્ષર *inbuf, અચલ અક્ષર *what,
	સંરચના strbuf *name, સંરચના strbuf *mail,
	timestamp_t *time, સંરચના strbuf *tz)
{
	સંરચના ident_split ident;
	size_t len, maillen, namelen;
	અચલ અક્ષર *tmp, *endp;
	અચલ અક્ષર *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	જો (!tmp)
		જાઓ error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	જો (!endp)
		len = strlen(tmp);
	નહીંતર
		len = endp - tmp;

	જો (split_ident_લીટી(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		પાછા;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	જો (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, ખાલી, 10);
	નહીંતર
		*time = 0;

	જો (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	નહીંતર
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (પૂર્ણાંક)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

સ્થિર શૂન્ય commit_info_destroy(સંરચના commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

સ્થિર શૂન્ય get_commit_info(સંરચના commit *commit, સંરચના commit_info *ret)
{
	પૂર્ણાંક len;
	અચલ અક્ષર *subject, *encoding;
	અચલ અક્ષર *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, ખાલી, encoding);
	get_ac_લીટી(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_લીટી(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	જો (len)
		strbuf_add(&ret->summary, subject, len);
	નહીંતર
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
સ્થિર શૂન્ય write_filename_info(સંરચના blame_origin *suspect)
{
	જો (suspect->previous) {
		સંરચના blame_origin *prev = suspect->previous;
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
સ્થિર પૂર્ણાંક emit_one_suspect_detail(સંરચના blame_origin *suspect, પૂર્ણાંક repeat)
{
	સંરચના commit_info ci = COMMIT_INFO_INIT;

	જો (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		પાછા 0;

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
	જો (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	પાછા 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
સ્થિર શૂન્ય found_guilty_entry(સંરચના blame_entry *ent, શૂન્ય *data)
{
	સંરચના progress_info *pi = (સંરચના progress_info *)data;

	જો (incremental) {
		સંરચના blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->અંક_લીટીઓ);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_લીટીઓ += ent->અંક_લીટીઓ;
	display_progress(pi->progress, pi->blamed_લીટીઓ);
}

સ્થિર અચલ અક્ષર *format_time(timestamp_t time, અચલ અક્ષર *tz_str,
			       પૂર્ણાંક show_raw_time)
{
	સ્થિર સંરચના strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	જો (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	નહીંતર {
		અચલ અક્ષર *time_str;
		size_t time_width;
		પૂર્ણાંક tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		માટે (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	પાછા time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_સંખ્યા          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_લીટી_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_લીટી           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

સ્થિર શૂન્ય emit_porcelain_details(સંરચના blame_origin *suspect, પૂર્ણાંક repeat)
{
	જો (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
સ્થિર શૂન્ય emit_porcelain_per_લીટી_details(સંરચના blame_entry *ent)
{
	જો (mark_unblamable_લીટીઓ && ent->unblamable)
		puts("unblamable");
	જો (mark_ignored_લીટીઓ && ent->ignored)
		puts("ignored");
}

સ્થિર શૂન્ય emit_porcelain(સંરચના blame_scoreboard *sb, સંરચના blame_entry *ent,
			   પૂર્ણાંક opt)
{
	પૂર્ણાંક repeat = opt & OUTPUT_લીટી_PORCELAIN;
	પૂર્ણાંક cnt;
	અચલ અક્ષર *cp;
	સંરચના blame_origin *suspect = ent->suspect;
	અક્ષર hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->અંક_લીટીઓ);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_લીટી_details(ent);

	cp = blame_nth_લીટી(sb, ent->lno);
	માટે (cnt = 0; cnt < ent->અંક_લીટીઓ; cnt++) {
		અક્ષર ch;
		જો (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			જો (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_લીટી_details(ent);
		}
		putchar('\t');
		કરો {
			ch = *cp++;
			putchar(ch);
		} જ્યારે (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	જો (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

સ્થિર સંરચના color_field {
	timestamp_t hop;
	અક્ષર col[COLOR_MAXLEN];
} *colorfield;
સ્થિર પૂર્ણાંક colorfield_nr, colorfield_alloc;

સ્થિર શૂન્ય parse_color_fields(અચલ અક્ષર *s)
{
	સંરચના string_list l = STRING_LIST_INIT_DUP;
	સંરચના string_list_item *item;
	ગણના { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		પસંદગી (next) {
		સ્થિતિ EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			તોડો;
		સ્થિતિ EXPECT_COLOR:
			જો (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			તોડો;
		}
	}

	જો (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

સ્થિર શૂન્ય setup_default_color_by_age(શૂન્ય)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

સ્થિર શૂન્ય determine_લીટી_heat(સંરચના commit_info *ci, અચલ અક્ષર **dest_color)
{
	પૂર્ણાંક i = 0;

	જ્યારે (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

સ્થિર અંતઃરેખ પૂર્ણાંક maybe_putc(પૂર્ણાંક c, FILE *out)
{
	પાછા out ? putc(c, out) : 0;
}

સ્થિર size_t print_marks(FILE *out, અચલ સંરચના blame_entry *ent, પૂર્ણાંક opt)
{
	size_t len = 0;

	જો ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	જો (mark_unblamable_લીટીઓ && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	જો (mark_ignored_લીટીઓ && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	પાછા len;
}

સ્થિર size_t ગણતરી_marks(અચલ સંરચના blame_entry *ent, પૂર્ણાંક opt)
{
	પાછા print_marks(ખાલી, ent, opt);
}

સ્થિર શૂન્ય emit_other(સંરચના blame_scoreboard *sb, સંરચના blame_entry *ent,
		       પૂર્ણાંક opt, સંરચના blame_entry *prev_ent)
{
	પૂર્ણાંક cnt;
	અચલ અક્ષર *cp;
	સંરચના blame_origin *suspect = ent->suspect;
	સંરચના commit_info ci = COMMIT_INFO_INIT;
	અક્ષર hex[GIT_MAX_HEXSZ + 1];
	પૂર્ણાંક show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	અચલ અક્ષર *default_color = ખાલી, *color = ખાલી, *reset = ખાલી;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_લીટી(sb, ent->lno);

	જો (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_લીટી_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	જો (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	માટે (cnt = 0; cnt < ent->અંક_લીટીઓ; cnt++) {
		અક્ષર ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		જો (opt & OUTPUT_COLOR_લીટી) {
			જો (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} નહીંતર  {
				color = default_color ? default_color : ખાલી;
				reset = default_color ? GIT_COLOR_RESET : ખાલી;
			}
		}
		જો (color)
			fputs(color, stdout);

		જો ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (પૂર્ણાંક)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		જો (opt & OUTPUT_ANNOTATE_COMPAT) {
			અચલ અક્ષર *name;
			જો (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			નહીંતર
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} નહીંતર {
			જો (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			જો (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			જો (opt & OUTPUT_SHOW_સંખ્યા)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			જો (!(opt & OUTPUT_NO_AUTHOR)) {
				અચલ અક્ષર *name;
				પૂર્ણાંક pad;
				જો (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				નહીંતર
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
		જો (reset)
			fputs(reset, stdout);
		કરો {
			ch = *cp++;
			putchar(ch);
		} જ્યારે (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	જો (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

સ્થિર શૂન્ય output(સંરચના blame_scoreboard *sb, પૂર્ણાંક option)
{
	સંરચના blame_entry *ent, *prev_ent = ખાલી;

	જો (option & OUTPUT_PORCELAIN) {
		માટે (ent = sb->ent; ent; ent = ent->next) {
			પૂર્ણાંક ગણતરી = 0;
			સંરચના blame_origin *suspect;
			સંરચના commit *commit = ent->suspect->commit;
			જો (commit->object.flags & MORE_THAN_ONE_PATH)
				ચાલુ;
			માટે (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				જો (suspect->guilty && ગણતરી++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					તોડો;
				}
			}
		}
	}

	માટે (ent = sb->ent; ent; ent = ent->next) {
		જો (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		નહીંતર {
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
સ્થિર પૂર્ણાંક read_ancestry(અચલ અક્ષર *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	સંરચના strbuf buf = STRBUF_INIT;
	જો (!fp)
		પાછા -1;
	જ્યારે (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		સંરચના commit_graft *graft = read_graft_લીટી(&buf);
		જો (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	પાછા 0;
}

સ્થિર પૂર્ણાંક update_auto_abbrev(પૂર્ણાંક auto_abbrev, સંરચના blame_origin *suspect)
{
	અચલ અક્ષર *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	પૂર્ણાંક len = strlen(uniq);
	જો (auto_abbrev < len)
		પાછા len;
	પાછા auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
સ્થિર શૂન્ય find_alignment(સંરચના blame_scoreboard *sb, પૂર્ણાંક *option)
{
	પૂર્ણાંક longest_src_લીટીઓ = 0;
	પૂર્ણાંક longest_dst_લીટીઓ = 0;
	unsigned largest_score = 0;
	સંરચના blame_entry *e;
	પૂર્ણાંક compute_auto_abbrev = (abbrev < 0);
	પૂર્ણાંક auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_ગણતરી = 0;

	માટે (e = sb->ent; e; e = e->next) {
		સંરચના blame_origin *suspect = e->suspect;
		પૂર્ણાંક અંક;
		size_t marks_ગણતરી = ગણતરી_marks(e, *option);

		જો (max_marks_ગણતરી < marks_ગણતરી)
			max_marks_ગણતરી = marks_ગણતરી;
		જો (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		જો (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		અંક = strlen(suspect->path);
		જો (longest_file < અંક)
			longest_file = અંક;
		જો (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			સંરચના commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			જો (*option & OUTPUT_SHOW_EMAIL)
				અંક = utf8_strwidth(ci.author_mail.buf);
			નહીંતર
				અંક = utf8_strwidth(ci.author.buf);
			જો (longest_author < અંક)
				longest_author = અંક;
			commit_info_destroy(&ci);
		}
		અંક = e->s_lno + e->અંક_લીટીઓ;
		જો (longest_src_લીટીઓ < અંક)
			longest_src_લીટીઓ = અંક;
		અંક = e->lno + e->અંક_લીટીઓ;
		જો (longest_dst_લીટીઓ < અંક)
			longest_dst_લીટીઓ = અંક;
		જો (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_લીટીઓ);
	max_digits = decimal_width(longest_dst_લીટીઓ);
	max_score_digits = decimal_width(largest_score);

	જો (compute_auto_abbrev)
		abbrev = auto_abbrev;
	જો (abbrev < (પૂર્ણાંક)the_hash_algo->hexsz) {
		abbrev += max_marks_ગણતરી;
		જો (abbrev > (પૂર્ણાંક)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

સ્થિર શૂન્ય sanity_check_on_fail(સંરચના blame_scoreboard *sb, પૂર્ણાંક baa)
{
	પૂર્ણાંક opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_સંખ્યા | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

સ્થિર unsigned parse_score(અચલ અક્ષર *arg)
{
	અક્ષર *end;
	unsigned long score = strtoul(arg, &end, 10);
	જો (*end)
		પાછા 0;
	પાછા score;
}

સ્થિર અક્ષર *add_prefix(અચલ અક્ષર *prefix, અચલ અક્ષર *path)
{
	પાછા prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

સ્થિર પૂર્ણાંક git_blame_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "blame.showroot")) {
		show_મૂળ = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "blame.showemail")) {
		પૂર્ણાંક *output_option = cb;
		જો (git_config_bool(var, કિંમત))
			*output_option |= OUTPUT_SHOW_EMAIL;
		નહીંતર
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		પાછા 0;
	}
	જો (!strcmp(var, "blame.date")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		parse_date_format(કિંમત, &blame_date_mode);
		પાછા 0;
	}
	જો (!strcmp(var, "blame.ignorerevsfile")) {
		અક્ષર *str;
		પૂર્ણાંક ret;

		ret = git_config_pathname(&str, var, કિંમત);
		જો (ret)
			પાછા ret;
		જો (str)
			string_list_ઉમેરો(&ignore_revs_file_list, str);
		મુક્ત(str);
		પાછા 0;
	}
	જો (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_લીટીઓ = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_લીટીઓ = git_config_bool(var, કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "color.blame.repeatedlines")) {
		જો (color_parse_mem(કિંમત, strlen(કિંમત), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "blame.coloring")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		જો (!strcmp(કિંમત, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_લીટી;
		} નહીંતર જો (!strcmp(કિંમત, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} નહીંતર જો (!strcmp(કિંમત, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_લીટી |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} નહીંતર {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", કિંમત);
			પાછા 0;
		}
	}

	જો (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		diff_algorithm = parse_algorithm_કિંમત(કિંમત);
		જો (diff_algorithm < 0)
			પાછા error(_("unknown value for config '%s': %s"),
				     var, કિંમત);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		પાછા 0;
	}

	જો (git_diff_heuristic_config(var, કિંમત, cb) < 0)
		પાછા -1;
	જો (userdiff_config(var, કિંમત) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર પૂર્ણાંક blame_copy_callback(અચલ સંરચના option *option, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt = option->કિંમત;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	જો (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	જો (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	જો (arg)
		blame_copy_score = parse_score(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક blame_move_callback(અચલ સંરચના option *option, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt = option->કિંમત;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	જો (arg)
		blame_move_score = parse_score(arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક blame_diff_algorithm_minimal(અચલ સંરચના option *option,
					અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt = option->કિંમત;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	જો (!unset)
		*opt |= XDF_NEED_MINIMAL;

	પાછા 0;
}

સ્થિર પૂર્ણાંક blame_diff_algorithm_callback(અચલ સંરચના option *option,
					 અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt = option->કિંમત;
	long કિંમત = parse_algorithm_કિંમત(arg);

	BUG_ON_OPT_NEG(unset);

	જો (કિંમત < 0)
		પાછા error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= કિંમત;

	પાછા 0;
}

સ્થિર પૂર્ણાંક is_a_rev(અચલ અક્ષર *name)
{
	સંરચના object_id oid;

	જો (repo_get_oid(the_repository, name, &oid))
		પાછા 0;
	પાછા OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ખાલી);
}

સ્થિર પૂર્ણાંક peel_to_commit_oid(સંરચના object_id *oid_ret, શૂન્ય *cbdata)
{
	સંરચના repository *r = ((સંરચના blame_scoreboard *)cbdata)->repo;
	સંરચના object_id oid;

	oidcpy(&oid, oid_ret);
	જ્યારે (1) {
		સંરચના object *obj;
		પૂર્ણાંક kind = odb_read_object_info(r->objects, &oid, ખાલી);
		જો (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			પાછા 0;
		}
		જો (kind != OBJ_TAG)
			પાછા -1;
		obj = deref_tag(r, parse_object(r, &oid), ખાલી, 0);
		જો (!obj)
			પાછા -1;
		oidcpy(&oid, &obj->oid);
	}
}

સ્થિર શૂન્ય build_ignorelist(સંરચના blame_scoreboard *sb,
			     સંરચના string_list *ignore_revs_file_list,
			     સંરચના string_list *ignore_rev_list)
{
	સંરચના string_list_item *i;
	સંરચના object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		જો (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		નહીંતર
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		જો (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ઉમેરો(&sb->ignore_list, &oid);
	}
}

પૂર્ણાંક cmd_blame(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	સંરચના rev_info revs;
	અક્ષર *path = ખાલી;
	સંરચના blame_scoreboard sb;
	સંરચના blame_origin *o;
	સંરચના blame_entry *ent = ખાલી;
	long dashdash_pos, lno;
	સંરચના progress_info pi = { ખાલી, 0 };

	સંરચના string_list range_list = STRING_LIST_INIT_NODUP;
	સંરચના string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	પૂર્ણાંક output_option = 0, opt = 0;
	પૂર્ણાંક show_stats = 0;
	અચલ અક્ષર *revs_file = ખાલી;
	અચલ અક્ષર *contents_from = ખાલી;
	અચલ સંરચના option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ખાલી, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_મૂળ, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_સંખ્યા),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_લીટી_PORCELAIN),
		OPT_BIT('c', ખાલી, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', ખાલી, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', ખાલી, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', ખાલી, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', ખાલી, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_લીટી),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ખાલી,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', ખાલી, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ખાલી, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ખાલી, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ખાલી, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	સંરચના parse_opt_ctx_t ctx;
	પૂર્ણાંક cmd_is_annotate = !strcmp(argv[0], "annotate");
	સંરચના range_set ranges;
	unsigned પૂર્ણાંક range_i;
	long anchor;
	long અંક_લીટીઓ = 0;
	અચલ અક્ષર *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	અચલ અક્ષર *અચલ *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ખાલી);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	માટે (;;) {
		પસંદગી (parse_options_step(&ctx, options, opt_usage)) {
		સ્થિતિ PARSE_OPT_NON_OPTION:
		સ્થિતિ PARSE_OPT_UNKNOWN:
			તોડો;
		સ્થિતિ PARSE_OPT_HELP:
			exit(0);
		સ્થિતિ PARSE_OPT_HELP_ERROR:
		સ્થિતિ PARSE_OPT_ERROR:
		સ્થિતિ PARSE_OPT_SUBCOMMAND:
			exit(129);
		સ્થિતિ PARSE_OPT_COMPLETE:
			exit(0);
		સ્થિતિ PARSE_OPT_DONE:
			જો (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			જાઓ parse_done;
		}

		જો (!strcmp(ctx.argv[0], "--reverse")) {
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

	જો (incremental || (output_option & OUTPUT_PORCELAIN)) {
		જો (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} નહીંતર જો (show_progress < 0)
		show_progress = isatty(2);

	જો (!abbrev)
		abbrev = the_hash_algo->hexsz;

	જો (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	જો (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} નહીંતર {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	પસંદગી (blame_date_mode.type) {
	સ્થિતિ DATE_RFC2822:
		blame_date_width = કદ("Thu, 19 Oct 2006 16:00:04 -0700");
		તોડો;
	સ્થિતિ DATE_ISO8601_STRICT:
		blame_date_width = કદ("2006-10-19T16:00:04-07:00");
		તોડો;
	સ્થિતિ DATE_ISO8601:
		blame_date_width = કદ("2006-10-19 16:00:04 -0700");
		તોડો;
	સ્થિતિ DATE_RAW:
		blame_date_width = કદ("1161298804 -0700");
		તોડો;
	સ્થિતિ DATE_UNIX:
		blame_date_width = કદ("1161298804");
		તોડો;
	સ્થિતિ DATE_SHORT:
		blame_date_width = કદ("2006-10-19");
		તોડો;
	સ્થિતિ DATE_RELATIVE:
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
		તોડો;
	સ્થિતિ DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = કદ("Thu Oct 19 16:00");
		તોડો;
	સ્થિતિ DATE_NORMAL:
		blame_date_width = કદ("Thu Oct 19 16:00:04 2006 -0700");
		તોડો;
	સ્થિતિ DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		તોડો;
	}
	blame_date_width -= 1; /* strip the null */

	જો (revs.diffopt.flags.find_copies_harder)
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
	જો (dashdash_pos) {
		પસંદગી (argc - dashdash_pos - 1) {
		સ્થિતિ 2: /* (1b) */
			જો (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		સ્થિતિ 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = ખાલી;
			તોડો;
		મૂળભૂત:
			usage_with_options(opt_usage, options);
		}
	} નહીંતર {
		જો (argc < 2)
			usage_with_options(opt_usage, options);
		જો (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} નહીંતર {	/* (2a) */
			જો (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_વૃક્ષ(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, ખાલી);
	જો (!revs.pending.nr && is_bare_repository(the_repository)) {
		સંરચના commit *head_commit;
		સંરચના object_id head_oid;

		જો (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, ખાલી) ||
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
	જો (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.અંક_લીટીઓ;

	જો (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	માટે (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, ટોચ;
		જો (parse_range_arg(range_list.વસ્તુઓ[range_i].string,
				    nth_લીટી_cb, &sb, lno, anchor,
				    &bottom, &ટોચ, sb.path,
				    the_repository->index))
			usage(str_usage);
		જો ((!lno && (ટોચ || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		જો (bottom < 1)
			bottom = 1;
		જો (ટોચ < 1 || lno < ટોચ)
			ટોચ = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, ટોચ);
		anchor = ટોચ + 1;
	}
	sort_and_merge_range_set(&ranges);

	માટે (range_i = ranges.nr; range_i > 0; --range_i) {
		અચલ સંરચના range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		અંક_લીટીઓ += (r->end - r->start);
	}
	જો (!અંક_લીટીઓ)
		અંક_લીટીઓ = sb.અંક_લીટીઓ;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ખાલી;

	જો (blame_move_score)
		sb.move_score = blame_move_score;
	જો (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_મૂળ = show_મૂળ;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	જો (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     અંક_લીટીઓ);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	જો (!incremental)
		setup_pager(the_repository);
	નહીંતર
		જાઓ cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	જો (!(output_option & (OUTPUT_COLOR_લીટી | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	જો (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		જો (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_લીટી))
			xsnprintf(repeated_meta_color,
				  કદ(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	જો (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_લીટી | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	જો (show_stats) {
		printf("num read blob: %d\n", sb.અંક_read_blob);
		printf("num get patch: %d\n", sb.અંક_get_patch);
		printf("num commits: %d\n", sb.અંક_commits);
	}

cleanup:
	માટે (ent = sb.ent; ent; ) {
		સંરચના blame_entry *e = ent->next;
		મુક્ત(ent);
		ent = e;
	}

	મુક્ત(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	પાછા 0;
}
